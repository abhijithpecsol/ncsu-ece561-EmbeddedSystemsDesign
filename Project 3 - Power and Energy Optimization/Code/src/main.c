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
#include "project3.h"

extern volatile uint8_t run_Read_Accel;
extern volatile uint8_t run_Update_LEDs;
extern volatile uint8_t led_on_period;

/*----------------------------------------------------------------------------
  MAIN function
 *----------------------------------------------------------------------------*/
int main (void) {
	Init_RGB_LEDs();
	
	// I2C and MMA
	i2c_init();																/* init i2c	*/
	if (!init_mma()) {												/* init mma peripheral */
		Control_RGB_LEDs(1, 0, 0);							/* Light red error LED */
		while (1)																/* not able to initialize mma */
			;
	}
	
	#if RUN_I2C_FAST == 1
		// increase i2c baud rate
		I2C_DISABLE;
		I2C0->F = (I2C_F_ICR(0x00) | I2C_F_MULT(0));
		I2C_ENABLE;
	#endif
	
	// configure low power modes
	SMC->PMPROT = SMC_PMPROT_ALLS_MASK;				// allow low leakage stop mode
	SMC->PMCTRL = SMC_PMCTRL_RUNM(0) | SMC_PMCTRL_STOPM(3);	// enable normal run mode (00) and low leakage stop mode (011)
	SMC->STOPCTRL = SMC_STOPCTRL_PSTOPO(0) | SMC_STOPCTRL_VLLSM(3);	// normal stop mode and VLL stop3 (not needed?)
	
	// configure low leakage wakeup unit (LLWU)
	LLWU->ME |= LLWU_ME_WUME0_MASK;						// internal module 0 is wakeup source which is apparently the LPTMR
	
	// enable stop mode (deep sleep)
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	
	#if DISABLE_CLOCKS == 1
		// disable clocks to unused peripherals
		SIM->SCGC5 &= ~(SIM_SCGC5_PORTA_MASK);
		SIM->SCGC6 &= ~SIM_SCGC6_FTF_MASK;
		SIM->SCGC7 &= ~SIM_SCGC7_DMA_MASK;
	#endif
	
	// LPTMR
	Init_LPTMR();
	Start_LPTMR();

	__enable_irq();
	
	while (1) {				
		// read acceleration every 100 ms
		if (run_Read_Accel){
			run_Read_Accel = 0;
			Read_Accel();
		}
		
		// update LEDs every 500 ms; keep them on for 10 ms
		if (run_Update_LEDs){
			run_Update_LEDs = 0;
			Update_LEDs();
			
			#if USE_PWM == 1
				SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;
				__wfi();
				SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
				//while(led_on_period);
//				OSC0->CR |= OSC_CR_EREFSTEN_MASK;
//				SMC->PMCTRL = SMC_PMCTRL_RUNM(0) | SMC_PMCTRL_STOPM(0);			// switch to "normal" stop model OSCERCLK (PWM) does not work in LLS
//				__wfi();
//				OSC0->CR &= ~OSC_CR_EREFSTEN_MASK;
//				SMC->PMCTRL = SMC_PMCTRL_RUNM(0) | SMC_PMCTRL_STOPM(3);			// return to LLS
			#endif
		}
		
		__wfi();			// go to sleep
	}
}

