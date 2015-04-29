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
#include "utilization.h"


U64 RA_Stack[64];
U64 GAME_Stack[128];

OS_TID t_Read_TS, t_Read_Accelerometer, t_Sound_Manager, t_US, t_Refill_Sound_Buffer;
OS_MUT LCD_mutex;
OS_MUT TS_mutex;
OS_MUT util_mutex;
os_mbx_declare(tilt_mbx, 2);
os_mbx_declare(touch_mbx, 3);
volatile uint8_t current_task;

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
	os_mut_init(&util_mutex);
	
	os_mbx_init(&tilt_mbx, sizeof(tilt_mbx));
	os_mbx_init(&touch_mbx, sizeof(touch_mbx));
	
	t_Read_TS = os_tsk_create(Task_Read_TS, 4);
	t_Read_Accelerometer = os_tsk_create_user(Task_Read_Accelerometer, 3, RA_Stack, 512);
	t_Sound_Manager = os_tsk_create(Task_Sound_Manager, 2);
	t_US = os_tsk_create_user(Task_Update_Game_State, 5, GAME_Stack, sizeof(GAME_Stack));
	t_Refill_Sound_Buffer = os_tsk_create(Task_Refill_Sound_Buffer, 1);

	current_task = TASK_IDLE;
	Init_Utilization_Profiling();			// begin utilization tracking

  os_tsk_delete_self ();
}

__task void Task_Read_TS(void) {
	PT_T * touch_msg;
	PT_T p;
	uint8_t initial_press = 0;

	os_itv_set(TASK_READ_TS_PERIOD_TICKS);
	
	while (1) {
		os_itv_wait();
		current_task = TASK_READ_TS;
		
		PTB->PSOR = MASK(DEBUG_T1_POS);
		if (TFT_TS_Read(&p)) { 
			if(!initial_press){
				initial_press++;											// we have been pressed
				os_evt_set(EV_INITIAL_PRESS, t_US);		// so start the game
			}
			else {
				// if the game has been started, send the touch position in a mailbox
				touch_msg = malloc(sizeof(PT_T));
				*touch_msg = p;
				os_mbx_send(&touch_mbx, (void *)touch_msg, WAIT_FOREVER);
			}
		}
		PTB->PCOR = MASK(DEBUG_T1_POS);
		
		current_task = TASK_IDLE;
	}
}

__task void Task_Read_Accelerometer(void) {
	float * msg;
	
	os_itv_set(TASK_READ_ACCELEROMETER_PERIOD_TICKS);

	while (1) {
		os_itv_wait();
		current_task = TASK_ACCEL;
		PTB->PSOR = MASK(DEBUG_T0_POS);
		read_full_xyz();
		convert_xyz_to_roll_pitch();
		
		// send accel data if there is room
		if (os_mbx_check(&tilt_mbx) == 2){
			// send roll
			msg = malloc(sizeof(float));
			*msg = roll;
			os_mbx_send(&tilt_mbx, (void *)msg, WAIT_FOREVER);
			
			// send pitch
			msg = malloc(sizeof(float));
			*msg = pitch;
			os_mbx_send(&tilt_mbx, (void *)msg, WAIT_FOREVER);
		}

		PTB->PCOR = MASK(DEBUG_T0_POS);
		
		current_task = TASK_IDLE;
	}
}

__task void Task_Update_Game_State(void) {	
	char buffer[20];
	uint8_t state = STATUS_CLOSED;
	uint8_t util_update_freq = STATUS_CPU_FREQ;
	GAME_T game;
	
	Game_Init(&game);
	
	os_evt_wait_and(EV_INITIAL_PRESS, WAIT_FOREVER);
	
	Reset_Utilization_Stats();
	
	os_itv_set(TASK_UPDATE_GAME_STATE_TICKS);
	
	game.ball.vy = BALL_INITIAL_VY;
	game.ball.vx = BALL_INITIAL_VX;

	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T3_POS);
		
		// if the status window is closed, process the game normally
		if (state == STATUS_CLOSED) {
			current_task = TASK_GAME;
			
			// redraw info bar
			sprintf(buffer, "[STATUS]   Lives: %d", game.lives);
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Text_PrintStr_RC(0,0, buffer);
			os_mut_release(&LCD_mutex);
			
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
				Reset_Utilization_Stats();
				while(1);
			}
			
			// check for screen press from mailbox
			if (os_mbx_check(&touch_mbx) < 3){
				void * msg;
				PT_T p;
				os_mbx_wait(touch_mbx, &msg, WAIT_FOREVER);
				p = *(PT_T *)msg;
				free(msg);
				// check if pressed in status box
				if (p.X < STATUS_BUT_WIDTH && p.Y < STATUS_BUT_HEIGHT){
					Pause_Game_Utilization();
					TFT_Erase();
					state = STATUS_OPEN;
					util_update_freq = STATUS_CPU_FREQ;
				}
			}
		}
		// if the status window is opened, display the status window and pause the game
		else if (state == STATUS_OPEN){			
			current_task = TASK_GAME_PAUSED;
			
			// redraw info bars
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Text_PrintStr_RC(0,0, "[EXIT]  System Info");
			TFT_Text_PrintStr_RC(TFT_MAX_ROWS - 1,0, "      PAUSED");
			os_mut_release(&LCD_mutex);
			
			// CPU statistics
			if(util_update_freq == STATUS_CPU_FREQ) {	
				float val;
				float total = 0.0;
				os_mut_wait(&LCD_mutex, WAIT_FOREVER);
				TFT_Text_PrintStr_RC(3,0, "Task      CPU Usage");
				TFT_Text_PrintStr_RC(4,0, "-------------------");
				val = Get_Utilization_Stats(TASK_READ_TS);
				total += val;
				sprintf(buffer, "Read TS      %5.2f%%", val);
				TFT_Text_PrintStr_RC(5,0, buffer);
				val = Get_Utilization_Stats(TASK_ACCEL);
				total += val;
				sprintf(buffer, "Read Accel   %5.2f%%", val);
				TFT_Text_PrintStr_RC(6,0, buffer);
				val = Get_Utilization_Stats(TASK_SND_MNGR);
				total += val;
				sprintf(buffer, "Sound Mngr   %5.2f%%", val);
				TFT_Text_PrintStr_RC(7,0, buffer);
				val = Get_Utilization_Stats(TASK_SND_RFL);
				total += val;
				sprintf(buffer, "Sound Refil  %5.2f%%", val);
				TFT_Text_PrintStr_RC(8,0, buffer);
				val = Get_Utilization_Stats(TASK_GAME_PAUSED);
				total += val;
				sprintf(buffer, "Update Game  %5.2f%%", val);
				TFT_Text_PrintStr_RC(9,0, buffer);
				TFT_Text_PrintStr_RC(10,0, "-------------------");
				sprintf(buffer, "Total Usage  %5.2f%%", total);
				TFT_Text_PrintStr_RC(11,0, buffer);	
				val = 100.0 - total;
				sprintf(buffer, "Idle Time    %5.2f%%", val);
				TFT_Text_PrintStr_RC(12,0, buffer);
				os_mut_release(&LCD_mutex);
			}
			else if (util_update_freq == STATUS_STK_FREQ){
				int val;
				os_mut_wait(&LCD_mutex, WAIT_FOREVER);
				TFT_Text_PrintStr_RC(3,0, "Task          Stack");
				val = 0;
				sprintf(buffer, "Read TS      %5db", val);
				TFT_Text_PrintStr_RC(5,0, buffer);
				sprintf(buffer, "Read Accel   %5db", val);
				TFT_Text_PrintStr_RC(6,0, buffer);
				sprintf(buffer, "Sound Mngr   %5db", val);
				TFT_Text_PrintStr_RC(7,0, buffer);
				sprintf(buffer, "Sound Refil  %5db", val);
				TFT_Text_PrintStr_RC(8,0, buffer);
				sprintf(buffer, "Update Game  %5db", val);
				TFT_Text_PrintStr_RC(9,0, buffer);
				TFT_Text_PrintStr_RC(11,0, "                   ");
				TFT_Text_PrintStr_RC(12,0, "                   ");
				os_mut_release(&LCD_mutex);
			}
			// maximum stack depth, based on configuration
			else if (util_update_freq == STATUS_MAX_STK_FREQ){
				util_update_freq = 0;
				os_mut_wait(&LCD_mutex, WAIT_FOREVER);
				TFT_Text_PrintStr_RC(3,0, "Task      Max Stack");
				sprintf(buffer, "Read TS      %5dB", 256);
				TFT_Text_PrintStr_RC(5,0, buffer);
				sprintf(buffer, "Read Accel   %5dB", 512);
				TFT_Text_PrintStr_RC(6,0, buffer);
				sprintf(buffer, "Sound Mngr   %5dB", 256);
				TFT_Text_PrintStr_RC(7,0, buffer);
				sprintf(buffer, "Sound Refil  %5dB", 256);
				TFT_Text_PrintStr_RC(8,0, buffer);
				sprintf(buffer, "Update Game  %5dB", 1024);
				TFT_Text_PrintStr_RC(9,0, buffer);
				os_mut_release(&LCD_mutex);
			}
			util_update_freq++;
			
			// check for screen press from mailbox
			if (os_mbx_check(&touch_mbx) < 3){
				void * msg;
				PT_T p;
				os_mbx_wait(touch_mbx, &msg, WAIT_FOREVER);
				p = *(PT_T *)msg;
				free(msg);
				// check if pressed in exit box
				if (p.X < STATUS_BUT_WIDTH && p.Y < STATUS_BUT_HEIGHT){
					TFT_Erase();
					state = STATUS_CLOSED;
					Reset_Utilization_Stats();
				}
			}
		}

		PTB->PCOR = MASK(DEBUG_T3_POS);
		current_task = TASK_IDLE;
	}
}
