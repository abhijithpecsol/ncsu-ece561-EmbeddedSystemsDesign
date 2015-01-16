/*!
 * \file    irq_common.c
 * \brief   common IRQ defines
 *
 * This file defines the functions/interrupt handlers/macros used for IRQ to be used as wakeup source.
 * And some common function prototypes and initializations.
 *
 * \version $Revision: 1.1 $
 * \author  Manish Bharti[b17254]
 ***/

#include "common.h"
#include "main.h"
#include "irq_common.h"

extern unsigned char state;
unsigned char flag_irq = 0; //interrupt flag

void configure_irq_for_low_level(void) 
{
	  SIM_SCGC4 |= SIM_SCGC4_IRQ_MASK; 
	  MXC_PTBPF4 = MXC_PTBPF4_B0(5);  //IRQ Enabled
	  //it is checked on board that through jumper J49, IRQ can be made high and on pressing of the 
	  //switch low level will be generated thereby resulting in IRQ interrupt
	  IRQ_SC |= IRQ_SC_IRQMOD_MASK ; //select to generate low level int
	  //Enable the IRQ pin 
	  IRQ_SC |= IRQ_SC_IRQPE_MASK ;
	  //Enable the interrupt on IRQ pin
	  IRQ_SC |=  IRQ_SC_IRQACK_MASK ;
	  IRQ_SC |= IRQ_SC_IRQIE_MASK;
	  //function ends
  
}
void configure_irq_for_rising_edge(void) 
{
	  SIM_SCGC4 |= SIM_SCGC4_IRQ_MASK; 
	  // IRQ pin PTB0 - pull up on
	  PCTLB_PUE |= PCTL_PUE_PTPUE(0x1);  //1 Pulling resistor is enabled.
	  PCTLB_PUS |= PCTL_PUS_PTPUS(0x1);  //1 Pullup resistor is selected.

	  //it is checked on board that through jumper J49, IRQ can be made high and on pressing of the 
	  //switch low level will be generated thereby resulting in IRQ interrupt
	  IRQ_SC = !IRQ_SC_IRQMOD_MASK | !IRQ_SC_IRQEDG_MASK ; //select to generate low level int
	  //Enable the IRQ pin 
	  IRQ_SC |= IRQ_SC_IRQPE_MASK ;
	  //Enable the interrupt on IRQ pin
	  IRQ_SC |=  IRQ_SC_IRQACK_MASK ; // clear any pending interrupt that may have happened.
	  IRQ_SC |=  IRQ_SC_IRQIE_MASK;
	  //function ends
}

void disable_irq_for_rising_edge(void) 
{
	  //Disable the interrupt on IRQ pin
	  IRQ_SC &= ~IRQ_SC_IRQIE_MASK;  
	  IRQ_SC &= ~IRQ_SC_IRQACK_MASK; // clear any pending interrupt that may have happened.
	  //Disable the IRQ pin 
	  IRQ_SC &= ~IRQ_SC_IRQPE_MASK;
	  //it is checked on board that through jumper J49, IRQ can be made high and on pressing of the 
	  //switch low level will be generated thereby resulting in IRQ interrupt
	  IRQ_SC = 0u;
	  // IRQ pin PTB0 - pull up off
	  PCTLB_PUS &= ~PCTL_PUS_PTPUS(0x1);  //1 Pullup resistor is selected.
	  PCTLB_PUE &= ~PCTL_PUE_PTPUE(0x1);  //1 Pulling resistor is disabled.
      /* Disable Irq module */	  
	  SIM_SCGC4 &= ~SIM_SCGC4_IRQ_MASK; 
}

//Interrupt handler for IRQ 
interrupt void  interrupt_handler_irq(void)
{
	//clear the interrupt by setting ack bit to 1
	IRQ_SC |=  IRQ_SC_IRQACK_MASK ;
	
	LPSWIsr(); // Call calback
	
}
