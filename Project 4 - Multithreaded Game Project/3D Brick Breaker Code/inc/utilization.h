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

void Init_Utilization_Profiling(void);
void Reset_Utilization_Stats(void);
float Get_Utilization_Stats(uint8_t task_id);
void Pause_Game_Utilization(void);

#endif
