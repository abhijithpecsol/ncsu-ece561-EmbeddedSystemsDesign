#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <RTL.H>
#include "timers.h"
#include "TFT_LCD.h"
#include "font.h"
#include "tasks.h"
#include "utilization.h"

volatile unsigned long total_ticks;
unsigned long total_ticks_game_paused;
unsigned long task_ticks[NUM_TASKS];
extern OS_MUT util_mutex;

void Init_Utilization_Profiling(void) {
	unsigned i;
	
	// Clear counts
	total_ticks = 0;
  for (i=0; i<NUM_TASKS; i++) {
	  task_ticks[i]=0;
  }
	
	// Initialize and start PIT timer
	PIT_Init(2399); 		// 24 MHz/(23999+1) = 1000 samples per second
	PIT_Start();
}

void Reset_Utilization_Stats(void){
	unsigned i;
	total_ticks = 0;
  for (i=0; i<NUM_TASKS; i++) {
	  task_ticks[i]=0;
  }
}

float Get_Utilization_Stats(uint8_t task_id){
	if (task_id == TASK_GAME_PAUSED){
		return (float)task_ticks[TASK_GAME]/total_ticks_game_paused*100.0;
	}
	else {
		return (float)task_ticks[task_id]/total_ticks*100.0;
	}
}

void Pause_Game_Utilization(void) {
	total_ticks_game_paused = total_ticks;
}
