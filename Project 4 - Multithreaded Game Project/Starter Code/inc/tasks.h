#ifndef TASKS_H
#define TASKS_H
#include <RTL.h>

#define TASK_READ_TS_PERIOD_TICKS (20)  // 1 tick/ms
#define TASK_READ_ACCELEROMETER_PERIOD_TICKS (250)  // 1 tick/ms
#define TASK_SOUND_PERIOD_TICKS (1)  // 1 tick/ms

// Debug outputs on Port B
#define DEBUG_T0_POS 	0		// J10 pin 2
#define DEBUG_T1_POS 	1 	// J10 pin 4
#define DEBUG_T2_POS 2 	// J10 pin 6
#define DEBUG_T3_POS 	3		// J10 pin 8

extern void	Init_Debug_Signals(void);

#define EV_PLAYSOUND (1) 

#define WAIT_FOREVER (0xffff)

extern __task void Task_Init(void);
extern __task void Task_Read_TS(void);
extern __task void Task_Read_Accelerometer(void);
extern __task void Task_Sound(void);
#endif
