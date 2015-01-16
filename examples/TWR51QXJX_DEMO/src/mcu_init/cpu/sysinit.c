#include "common.h"


/* system clock frequency */
int core_clk_khz;
int core_clk_mhz;
void fb_clk_init(void);
void clkout_init(unsigned char clockout_select, unsigned char clkoutdiv)
;
/**
 * __initialize_hardware:   This function does the common hardware initialization
 *
 * Parameters:    none
 *
 *
 * Return:        void
 */ 
void __initialize_hardware(void)
{
	//Disable Watchdog Timer
	SIM_COPC = SIM_COPC_COPT(0);
	outsrs();
	/*
      * Enable all of the port clocks. These have to be enabled to configure
      * pin muxing options, so most code will need all of these on anyway.
      */
	SIM_SCGC6 |= SIM_SCGC6_PORTA_MASK |
	             SIM_SCGC6_PORTB_MASK | 
	             SIM_SCGC6_PORTC_MASK | 
	             SIM_SCGC6_PORTD_MASK |
	             SIM_SCGC6_PORTE_MASK |
	             SIM_SCGC6_PORTF_MASK;
	
	PMC_LVDSC1 = 0x00;  //disable LVD
	
	// BDM pin PTB4 - pull up on, drive strength high
	PCTLB_PUE |= PCTL_PUE_PTPUE(0x10);  //1 Pulling resistor is enabled.
	PCTLB_PUS |= PCTL_PUS_PTPUS(0x10);  //1 Pullup resistor is selected.
	PCTLB_DS |= PCTL_DS_PTDS(0x10);     // 1 High output drive strength selected
	
#if defined(CLK_TST)
		clkout_init(MCG_OUT,0); // select CLKOUT source and division factor
		fb_clk_init();

#endif
	SIM_CLKOUT = SIM_CLKOUT_CLKOUTDIV(0)|SIM_CLKOUT_CS(3);

	// setup PLL frequency and move to PEE mode
	core_clk_mhz = pll_init(CORE_CLK_MHZ, REF_CLK);
	core_clk_khz = core_clk_mhz*1000;
	
	uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);		
	
}

/********************************************************************/
void fb_clk_init(void)
{
	/* Enable the clock to the FlexBus module */
        SIM_SCGC5 |= SIM_SCGC5_MFBUS_MASK;

 	/* Enable the FB_CLKOUT function on PTB1 (alt7 function) */
        MXC_PTBPF4 = MXC_PTBPF4_B1(7);
        PCTLB_DS |= 0x02;			// enable high drive strength to support high toggle rate	// enable high drive strength to support high toggle rate
}
/********************************************************************/
void clkout_init(unsigned char clockout_select, unsigned char clkoutdiv)
{
	/* Set the trace clock to the core clock frequency */
	// should add test of clkoutdiv to check it is is range 0 to 7
	SIM_CLKOUT = SIM_CLKOUT_CLKOUTDIV(clkoutdiv)|SIM_CLKOUT_CS(clockout_select);
        	
	//Enable CLKOUT on PTA5
	MXC_PTAPF2 = MXC_PTAPF2_A5(6);
	PCTLA_DS |= 0x20;			// enable high drive strength to support high toggle rate
}

