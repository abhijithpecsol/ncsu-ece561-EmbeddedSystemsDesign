/*********************************************************************
*
* Michael Meli
* ECE 561 - Spring 2015
*
* Header file for timers.c
*
*********************************************************************/

#define SIM		SIM_BASE_PTR 
#define PIT  	PIT_BASE_PTR
#define PIT_IRQn	22

void PIT_init(unsigned period);
void Start_PIT(void);
void Stop_PIT(void);
