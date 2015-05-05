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
unsigned int stack_pointers[NUM_TASKS];
unsigned int max_stack_pointers[NUM_TASKS];
extern OS_MUT LCD_mutex;
extern U64 RA_Stack[], GAME_Stack[], RTS_Stack[], SM_Stack[], SR_Stack[];

void Init_Utilization_Profiling(void) {
	unsigned i;
	
	// Clear counts
	total_ticks = 0;
  for (i=0; i<NUM_TASKS; i++) {
	  task_ticks[i]=0;
		stack_pointers[i] = 0;
		max_stack_pointers[i] = UINT32_MAX;
  }
	
	// Initialize and start PIT timer
	PIT_Init(2399); 		// 24 MHz/(23999+1) = 1000 samples per second
	PIT_Start();
}

// Sets utilization stats back to zero
void Reset_Utilization_Stats(void){
	unsigned i;
	total_ticks = 0;
  for (i=0; i<NUM_TASKS; i++) {
	  task_ticks[i]=0;
  }
}

// Get CPU utilization for a specific task
float Get_Utilization_Stats(uint8_t task_id){
	if (task_id == TASK_GAME_PAUSED){
		return (float)task_ticks[TASK_GAME]/total_ticks_game_paused*100.0;
	}
	else {
		return (float)task_ticks[task_id]/total_ticks*100.0;
	}
}

// Since the utilization stats show in game thread, save game thread utilization to show accurate value
void Pause_Game_Utilization(void) {
	total_ticks_game_paused = total_ticks;
}

// Updates the value of the stack pointer for a specific task
void Update_Stack_Pointer(unsigned int sp, uint8_t task_id){
	// track greatest stack depth
	if (sp < stack_pointers[task_id]){
		max_stack_pointers[task_id] = sp;
	}
	
	// track current stack depth
	stack_pointers[task_id] = sp;
}

// Returns the current size of the stack for a specific task
unsigned int Get_Stack_Depth(uint8_t task_id, uint8_t type){
	unsigned int task_sp;
	unsigned int bot_stk;
	
	switch(type){
		case CURRENT_STACK:
			task_sp = stack_pointers[task_id];
			break;
		
		case MAX_STACK:
			task_sp = max_stack_pointers[task_id];
			break;
		
		default:
			return 0;
	}
	
	#if TRACK_STACK == 0
		return 0;
	#endif
	
	if (task_sp == 0 || task_sp == UINT32_MAX){
		return 0;
	}
	
	// get task's bottom of stack
	switch(task_id){
		case TASK_ACCEL:
			bot_stk = (unsigned int)&RA_Stack[STACK_RA_BOT];
			break;
		
		case TASK_READ_TS:
			bot_stk = (unsigned int)&RTS_Stack[STACK_RTS_BOT];
			break;
		
		case TASK_SND_MNGR:
			bot_stk = (unsigned int)&SM_Stack[STACK_SM_BOT];
			break;
		
		case TASK_SND_RFL:
			bot_stk = (unsigned int)&SR_Stack[STACK_SR_BOT];
			break;
		
		case TASK_GAME:
			bot_stk = (unsigned int)&GAME_Stack[STACK_GAME_BOT];
			break;
		
		default:
			return 0;
	}
	
	// 8 + difference is stack depth because bottom of stack is 8 bytes below last element
	// each task has 64B of kernel info by default
	return 64 + (bot_stk - task_sp + 8);
}

void Display_CPU_Stats(void){
	char buffer[20];
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

void Display_Stack_Stats(void){
	char buffer[20];
	unsigned int val;
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	//TFT_Text_PrintStr_RC(3,0, "Task          Stack");
	TFT_Text_PrintStr_RC(3,0, "Task  Current Stack");
	val = Get_Stack_Depth(TASK_READ_TS, CURRENT_STACK);
	sprintf(buffer, "Read TS      %5uB", val);
	TFT_Text_PrintStr_RC(5,0, buffer);
	val = Get_Stack_Depth(TASK_ACCEL, CURRENT_STACK);
	sprintf(buffer, "Read Accel   %5uB", val);
	TFT_Text_PrintStr_RC(6,0, buffer);
	val = Get_Stack_Depth(TASK_SND_MNGR, CURRENT_STACK);
	sprintf(buffer, "Sound Mngr   %5uB", val);
	TFT_Text_PrintStr_RC(7,0, buffer);
	val = Get_Stack_Depth(TASK_SND_RFL, CURRENT_STACK);
	sprintf(buffer, "Sound Refil  %5uB", val);
	TFT_Text_PrintStr_RC(8,0, buffer);
	val = Get_Stack_Depth(TASK_GAME, CURRENT_STACK);
	sprintf(buffer, "Update Game  %5uB", val);
	TFT_Text_PrintStr_RC(9,0, buffer);
	TFT_Text_PrintStr_RC(11,0, "                   ");
	TFT_Text_PrintStr_RC(12,0, "                   ");
	os_mut_release(&LCD_mutex);
}

void Display_Max_Stack_Stats(void){
	char buffer[20];
	unsigned int val;
	os_mut_wait(&LCD_mutex, WAIT_FOREVER);
	TFT_Text_PrintStr_RC(3,0, "Task      Max Stack");
	val = Get_Stack_Depth(TASK_READ_TS, MAX_STACK);
	sprintf(buffer, "Read TS      %5uB", val);
	TFT_Text_PrintStr_RC(5,0, buffer);
	val = Get_Stack_Depth(TASK_ACCEL, MAX_STACK);
	sprintf(buffer, "Read Accel   %5uB", val);
	TFT_Text_PrintStr_RC(6,0, buffer);
	val = Get_Stack_Depth(TASK_SND_MNGR, MAX_STACK);
	sprintf(buffer, "Sound Mngr   %5uB", val);
	TFT_Text_PrintStr_RC(7,0, buffer);
	val = Get_Stack_Depth(TASK_SND_RFL, MAX_STACK);
	sprintf(buffer, "Sound Refil  %5uB", val);
	TFT_Text_PrintStr_RC(8,0, buffer);
	val = Get_Stack_Depth(TASK_GAME, MAX_STACK);
	sprintf(buffer, "Update Game  %5uB", val);
	TFT_Text_PrintStr_RC(9,0, buffer);
	TFT_Text_PrintStr_RC(11,0, "                   ");
	TFT_Text_PrintStr_RC(12,0, "                   ");
	os_mut_release(&LCD_mutex);
}
