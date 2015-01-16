/*
 * File:        sysinit.c
 * Purpose:     Kinetis Configuration
 *              Initializes processor to a default state
 *
 * Notes: K40, K60 sysinit
 *
 */

#include "common.h"
#include "sysinit.h"
#include "uart.h"

/********************************************************************/
/* Actual system clock frequency */
int core_clk_khz;
int core_clk_mhz;
int periph_clk_khz;
/********************************************************************/

void trace_clk_init(void)
{
	/* Set the trace clock to the core clock frequency */
	SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;
        	
	/* Enable the TRACE_CLKOUT pin function on PTA6 (alt7 function) */
	PORTA_PCR6 = ( PORT_PCR_MUX(0x7) | PORT_PCR_DSE_MASK);	// enable high drive strength to support high toggle rate
}

/********************************************************************/
void fb_clk_init(void)
{
	/* Enable the clock to the FlexBus module */
    SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

 	/* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
	PORTC_PCR3 = ( PORT_PCR_MUX(0x5) | PORT_PCR_DSE_MASK);	// enable high drive strength to support high toggle rate
}
/********************************************************************/

void sysinit (void)
{
  /* Setup Allowed Power modes */
  MC_PMPROT = MC_PMPROT_ALLS_MASK | MC_PMPROT_AVLLS1_MASK | MC_PMPROT_AVLLS2_SHIFT | MC_PMPROT_AVLLS3_MASK;

  /* Recover MCU from VLLS1,2,3 mode */          
  SIM_SCGC4 |= SIM_SCGC4_LLWU_MASK; /* Enable LLWU for LowPower wake-up */
  if (LLWU_CS & LLWU_CS_ACKISO_MASK)
  {
    LLWU_CS |= LLWU_CS_ACKISO_MASK; /* Clear ACKISO bit */
    LLWU_ME &= ~LLWU_ME_WUME4_MASK; /* TSI LLWU Input Disable = LLWU_M4IF */
  }

  /*
   * Enable all of the port clocks. These have to be enabled to configure
   * pin muxing options, so most code will need all of these on anyway.
   */
  SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                | SIM_SCGC5_PORTB_MASK
                | SIM_SCGC5_PORTC_MASK
                | SIM_SCGC5_PORTD_MASK
                | SIM_SCGC5_PORTE_MASK );

 	/* Ramp up the system clock */
	core_clk_mhz = pll_init(CORE_CLK_MHZ, REF_CLK);

	/*
         * Use the value obtained from the pll_init function to define variables
	 * for the core clock in kHz and also the peripheral clock. These
	 * variables can be used by other functions that need awareness of the
	 * system frequency.
	 */
	core_clk_khz = core_clk_mhz * 1000;
    periph_clk_khz = core_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);

  	/* For debugging purposes, enable the trace clock and/or FB_CLK so that
  	 * we'll be able to monitor clocks and know the PLL is at the frequency
  	 * that we expect.
  	 */
	trace_clk_init();
    fb_clk_init();
}


