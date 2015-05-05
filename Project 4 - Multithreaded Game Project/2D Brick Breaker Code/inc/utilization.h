#ifndef utilization_h
#define utilization_h

#include <stdint.h>

#define NUM_TASKS		(7)

#define TASK_IDLE			(0)
#define TASK_READ_TS	(1)
#define TASK_ACCEL		(2)
#define TASK_SND_MNGR	(3)
#define TASK_SND_RFL	(4)
#define TASK_GAME			(5)
#define TASK_STATUS		(6)
#define TASK_GAME_PAUSED (7)

#define STACK_RA_BOT		(63)
#define STACK_GAME_BOT	(127)
#define STACK_RTS_BOT		(256/8-1)
#define STACK_SM_BOT		(256/8-1)
#define STACK_SR_BOT		(256/8-1)

#define CURRENT_STACK	(1)
#define MAX_STACK			(2)

#define TRACK_STACK		(1)

void Init_Utilization_Profiling(void);
void Reset_Utilization_Stats(void);
float Get_Utilization_Stats(uint8_t task_id);
void Pause_Game_Utilization(void);
void Update_Stack_Pointer(unsigned int sp, uint8_t task_id);
unsigned int Get_Stack_Depth(uint8_t task_id, uint8_t type);
void Display_CPU_Stats(void);
void Display_Stack_Stats(void);
void Display_Max_Stack_Stats(void);

#endif
