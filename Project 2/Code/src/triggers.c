/*----------------------------------------------------------------------------
 *
 * triggers.c
 * Michael Meli
 *
 * Handles setup of input triggers.
 *
 *----------------------------------------------------------------------------*/
 
 #include <MKL25Z4.H>
 #include "gpio_defs.h"
 #include "triggers.h"
 
void init_triggers(void){
	// Enable clock to ports B and D
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	 
	// Make the pins for triggers 1 and 2 GPIOs
	PORTB->PCR[TRIGGER_1] &= ~PORT_PCR_MUX_MASK;   
	PORTB->PCR[TRIGGER_1] = PORT_PCR_MUX(1);
	PORTB->PCR[TRIGGER_2] &= ~PORT_PCR_MUX_MASK;   
	PORTB->PCR[TRIGGER_2] = PORT_PCR_MUX(1);
	
	// Enable pull-up resistors on inputs
	PORTB->PCR[TRIGGER_1] |= PORT_PCR_PE_MASK;
	PORTB->PCR[TRIGGER_1] |= PORT_PCR_PS_MASK;
	PORTB->PCR[TRIGGER_2] |= PORT_PCR_PE_MASK;
	PORTB->PCR[TRIGGER_2] |= PORT_PCR_PS_MASK;
	
	// Make pins inputs
	PTB->PDDR &= ~MASK(TRIGGER_1);
	PTB->PDDR &= ~MASK(TRIGGER_2);
}
