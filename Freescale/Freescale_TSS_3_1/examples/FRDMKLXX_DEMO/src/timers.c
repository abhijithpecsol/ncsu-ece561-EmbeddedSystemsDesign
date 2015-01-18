/*********************************************************************
*
* Michael Meli
* ECE 561 - Spring 2015
*
* This file handles configurations and interrupts for the periodic
* interrupt timers.
* Based off of the demo code.
*
*********************************************************************/

#include "timers.h"
#include "MKL25Z4.h"
#include "arm_cm0.h"
#include "LEDs.h"

unsigned int timer1ms = 0;

// Sets up configuration for the PIT. Period is how long for an interrupt.
void PIT_init(unsigned period) {
	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
	
	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK;			// enable module = 0
	PIT->MCR |= PIT_MCR_FRZ_MASK;				// freeze timers in debug = 1
	
	// Initialize PIT0 to count down from argument 
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);

	// No chaining of timers
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	// Generate interrupts
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

	/* Enable Interrupts */
	set_irq_priority(PIT_IRQn, 128); // 0, 64, 128 or 192
	//NVIC_ClearPendingIRQ(PIT_IRQn); 
	enable_irq(PIT_IRQn);	
	
	// make sure interrupts are not masked globally
	__enable_irq();
}

void Start_PIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void Stop_PIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}

void PIT_IRQHandler() {
	//clear pending IRQ
	//NVIC_ClearPendingIRQ(PIT_IRQn);
	
	// channel 1
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		
		// Do ISR work
		timer1ms++;
		if (timer1ms == 5000){
			setLEDColor(200,0,0);
		}
		if (timer1ms == 10000){
			timer1ms = 0;
			setLEDColor(0,0,0);
		}
	} 
	// channel 2
	else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	} 
}
