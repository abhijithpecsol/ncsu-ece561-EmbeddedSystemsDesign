#include <stdint.h>
#include <stdio.h>
#include <RTL.h>
#include <MKL25Z4.h>
#include <stdlib.h>
#include "TFT_LCD.h"
#include "font.h"
#include "tasks.h"
#include "MMA8451.h"
#include "sound.h"
#include "DMA.h"
#include "gpio_defs.h"
#include "game.h"


U64 RA_Stack[64];
U64 GAME_Stack[128];

OS_TID t_Read_TS, t_Read_Accelerometer, t_Sound_Manager, t_US, t_Refill_Sound_Buffer;
OS_MUT LCD_mutex;
OS_MUT TS_mutex;
os_mbx_declare(tilt_mbx, 2);

void Init_Debug_Signals(void) {
	// Enable clock to port B
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	
	// Make 3 pins GPIO
	PORTB->PCR[DEBUG_T0_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[DEBUG_T0_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[DEBUG_T1_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[DEBUG_T1_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[DEBUG_T2_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[DEBUG_T2_POS] |= PORT_PCR_MUX(1);          
	PORTB->PCR[DEBUG_T3_POS] &= ~PORT_PCR_MUX_MASK;          
	PORTB->PCR[DEBUG_T3_POS] |= PORT_PCR_MUX(1);          
	
	// Set ports to outputs
	PTB->PDDR |= MASK(DEBUG_T0_POS);
	PTB->PDDR |= MASK(DEBUG_T1_POS);
	PTB->PDDR |= MASK(DEBUG_T2_POS);
	PTB->PDDR |= MASK(DEBUG_T3_POS);
	
	// Initial values are 0
	PTB->PCOR = MASK(DEBUG_T0_POS);
	PTB->PCOR = MASK(DEBUG_T1_POS);
	PTB->PCOR = MASK(DEBUG_T2_POS);
	PTB->PCOR = MASK(DEBUG_T3_POS);
}	

__task void Task_Init(void) {
	
	os_mut_init(&LCD_mutex);
	
	os_mbx_init(&tilt_mbx, sizeof(tilt_mbx));
	
	t_Read_TS = os_tsk_create(Task_Read_TS, 4);
	t_Read_Accelerometer = os_tsk_create_user(Task_Read_Accelerometer, 3, RA_Stack, 512);
	t_Sound_Manager = os_tsk_create(Task_Sound_Manager, 2);
	t_US = os_tsk_create_user(Task_Update_Game_State, 5, GAME_Stack, sizeof(GAME_Stack));
	t_Refill_Sound_Buffer = os_tsk_create(Task_Refill_Sound_Buffer, 1);


  os_tsk_delete_self ();
}

__task void Task_Read_TS(void) {
	PT_T p;
	uint8_t initial_press = 0;

	os_itv_set(TASK_READ_TS_PERIOD_TICKS);
	
	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T1_POS);
		if (TFT_TS_Read(&p)) { 
			if(!initial_press){
				initial_press++;											// we have been pressed
				os_evt_set(EV_INITIAL_PRESS, t_US);		// so start the game
			}
		}
		PTB->PCOR = MASK(DEBUG_T1_POS);
	}
}

__task void Task_Read_Accelerometer(void) {
	float * msg;
	
	os_itv_set(TASK_READ_ACCELEROMETER_PERIOD_TICKS);

	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T0_POS);
		read_full_xyz();
		convert_xyz_to_roll_pitch();
		
		// send roll
		msg = malloc(sizeof(float));
		*msg = roll;
		os_mbx_send(&tilt_mbx, (void *)msg, WAIT_FOREVER);
		
		// send pitch
		msg = malloc(sizeof(float));
		*msg = pitch;
		os_mbx_send(&tilt_mbx, (void *)msg, WAIT_FOREVER);

		PTB->PCOR = MASK(DEBUG_T0_POS);
	}
}

__task void Task_Update_Game_State(void) {	
	GAME_T game;
	
	Game_Init(&game);
	
	os_evt_wait_and(EV_INITIAL_PRESS, WAIT_FOREVER);
	
	os_itv_set(TASK_UPDATE_GAME_STATE_TICKS);
	
	game.ball.vy = BALL_INITIAL_VY;
	game.ball.vx = BALL_INITIAL_VX;

	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T3_POS);
		
		// test - automatic ball movement
		if (game.ball.loc.Y > TFT_HEIGHT - 2*PADDLE_BUFFER) game.ball.vy = BALL_INITIAL_VY;
		else if (game.ball.loc.Y < INFO_BAR_HEIGHT + HORZ_PADDLE_HEIGHT) game.ball.vy = -BALL_INITIAL_VY;
		if (game.ball.loc.X > TFT_WIDTH - PADDLE_BUFFER - BALL_SIDE_LENGTH) game.ball.vx = BALL_INITIAL_VX;
		else if (game.ball.loc.X < PADDLE_BUFFER) game.ball.vx = -BALL_INITIAL_VX;
		
		// brick processing
		Detect_Brick_Collision(&game);	// look for ball collisions with a brick
		Redraw_Bricks(&game);						// redraw all bricks, adjusting color where necessary
		
		// ball processing
		Detect_Paddle_Collision(&game);	// look for ball collisions with the paddle
		Move_Ball(&game);								// adjust the position of the ball based on velocities
		Redraw_Ball(&game);							// redraw ball based on new position
		
		// paddle processing
		Convert_Tilt(&game);						// Covert the pitch/roll values to velocities for the paddles
		Move_Paddles(&game);						// adjust position of paddles based on velocities
		Redraw_Paddles(&game);					// redraw paddles based on new positions
		
		// check for win
		if (game.hits >= BRICK_TOTAL_NUM) {
			TFT_Text_PrintStr_RC(3,0, "VICTORY!");
			while(1);
		}

		PTB->PCOR = MASK(DEBUG_T3_POS);
	}
}
