/*----------------------------------------------------------------------------
 *----------------------------------------------------------------------------*/
#include <MKL25Z4.H>
#include <stdio.h>
#include "gpio_defs.h"
#include "LEDs.h"
#include "i2c.h"
#include "mma8451.h"
#include "delay.h"
#include "timers.h"
#include "tasks.h"

extern volatile uint8_t run_Read_Accel;
extern volatile uint8_t run_Update_LEDs;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	Init_RGB_LEDs();
	Control_RGB_LEDs(1, 1, 1);								// white = configuration
	
	// I2C and MMA
	i2c_init();																/* init i2c	*/
	if (!init_mma()) {												/* init mma peripheral */
		Control_RGB_LEDs(1, 0, 0);							/* Light red error LED */
		while (1)																/* not able to initialize mma */
			;
	}
	
	// configure low power modes
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK;				// allow low leakage stop mode
	SMC->PMCTRL = SMC_PMCTRL_RUNM(0) | SMC_PMCTRL_STOPM(3);	// enable normal run mode (00) and low leakage stop mode (011)
	SMC->STOPCTRL = SMC_STOPCTRL_PSTOPO(0) | SMC_STOPCTRL_VLLSM(3);	// normal stop mode and VLL stop3 (not needed?)
	
	// configure low leakage wakeup unit (LLWU)
	LLWU->ME |= LLWU_ME_WUME0_MASK;						// internal module 0 is wakeup source which is apparently the LPTMR
	
	// enable stop mode (deep sleep)
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
	Delay(1000);
	
	// LPTMR
	Init_LPTMR();
	Start_LPTMR();

	__enable_irq();
	
	while (1) {		
		Control_RGB_LEDs(0,0,0);		// done to ensure LEDs blink...right now only 100 ms
		
		// read acceleration every 100 ms
		if (run_Read_Accel){
			run_Read_Accel = 0;
			Read_Accel();
		}
		
		// update LEDs every 500 ms
		if (run_Update_LEDs){
			run_Update_LEDs = 0;
			Update_LEDs();
		}
		
		__wfi();			// go to sleep
		
		//read_full_xyz();
		//convert_xyz_to_roll_pitch();
		// Light green LED if pitch > 10 degrees
		// Light blue LED if roll > 10 degrees
		//Control_RGB_LEDs(0, (fabs(roll) > 10)? 1:0, (fabs(pitch) > 10)? 1:0);
	}
}

