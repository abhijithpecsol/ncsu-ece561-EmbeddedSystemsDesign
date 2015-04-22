#include <stdint.h>
#include <stdio.h>
#include <RTL.h>
#include <MKL25Z4.h>

#include "tft_lcd.h"
#include "tasks.h"
#include "mma8451.h"
#include "sound.h"
#include "gpio_defs.h"
#include "game.h"

U64 RA_Stack[64];

extern float roll;
OS_TID t_Read_TS, t_Read_Accelerometer, t_Sound, t_Update_Game;
OS_MUT LCD_mutex;
OS_MUT TS_mutex;
OS_MUT roll_mutex;

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
	os_mut_init(&roll_mutex);
	
	t_Read_TS = os_tsk_create(Task_Read_TS, 4);
	t_Read_Accelerometer = os_tsk_create(Task_Read_Accelerometer, 3);
	t_Sound = os_tsk_create(Task_Sound, 2);
	t_Update_Game = os_tsk_create_user(Task_Update_Game_State, 5, RA_Stack, 512);
  os_tsk_delete_self ();
}

__task void Task_Read_TS(void) {
	PT_T p, pp;
	COLOR_T c;
	
	c.R = 150;
	c.G = 200;
	c.B = 255;
	
	os_itv_set(TASK_READ_TS_PERIOD_TICKS);

	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T1_POS);
		if (TFT_TS_Read(&p)) { 
			// Send message indicating screen was pressed
			os_evt_set(EV_PLAYSOUND, t_Sound);
			// Also draw on screen
			if ((pp.X == 0) && (pp.Y == 0)) {
				pp = p;
			}
			os_mut_wait(&LCD_mutex, WAIT_FOREVER);
			TFT_Draw_Line(&p, &pp, &c);
			os_mut_release(&LCD_mutex);
			pp = p;
		} else {
			pp.X = 0;
			pp.Y = 0;
		}
		PTB->PCOR = MASK(DEBUG_T1_POS);
	}
}

__task void Task_Read_Accelerometer(void) {	
	os_itv_set(TASK_READ_ACCELEROMETER_PERIOD_TICKS);

	while (1) {
		os_itv_wait();
		PTB->PSOR = MASK(DEBUG_T0_POS);
		read_full_xyz();
		os_mut_wait(&roll_mutex, WAIT_FOREVER);
		convert_xyz_to_roll_pitch();
		os_mut_release(&roll_mutex);
		PTB->PCOR = MASK(DEBUG_T0_POS);
	}
}

__task void Task_Sound(void) {
	uint32_t num_cycles, phase;
	uint16_t val, out_val;
	uint32_t temp_val;
	
	os_itv_set(TASK_SOUND_PERIOD_TICKS);

	while (1) {
		os_evt_wait_and(EV_PLAYSOUND, WAIT_FOREVER); // wait for trigger
		val = MAX_DAC_CODE/2;
		out_val = 0;
		phase = 1;
		for (num_cycles = 0; num_cycles < 1000; num_cycles++) {
			PTB->PSOR = MASK(DEBUG_T2_POS);
#if 0
			out_val = MAX_DAC_CODE - out_val;	
#else
			// Generate next sample
			if (phase > 0) {
				out_val = MAX_DAC_CODE/2 + val;
			} else {
				out_val = MAX_DAC_CODE/2 - val;
			}
			phase = 1 - phase;
			temp_val = val * 0xFF00;
			val = temp_val >> 16;
#endif
			// Wait for next interval to play the sample
			os_itv_wait(); 
			// Send sample to DAC
			DAC0->DAT[0].DATH = DAC_DATH_DATA1(out_val >> 8);
			DAC0->DAT[0].DATL = DAC_DATL_DATA0(out_val);
			PTB->PCOR = MASK(DEBUG_T2_POS);

		}
	}
}

__task void Task_Update_Game_State(void) {
	CHARACTER_T ch;
	
	Game_Init(&ch);
	
	os_itv_set(TASK_UPDATE_GAME_STATE_TICKS);

	while (1) {
		os_itv_wait();
		//PTB->PSOR = MASK(DEBUG_T0_POS);
		
		Detect_Collision(&ch);
		Move_Character(&ch);
		Redraw_Platforms();

		//PTB->PCOR = MASK(DEBUG_T0_POS);
	}
}

