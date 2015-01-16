/*
 *    kinetis_sysinit.c - Default init routines for P2
 *                     		Kinetis ARM systems
 *    Copyright © 2012 Freescale semiConductor Inc. All Rights Reserved.
 */

#include "kinetis_sysinit.h"
#include "derivative.h"
#include "isr.h"

typedef void (*const tIsrFunc)(void);
typedef struct {
  uint32_t * __ptr;
  tIsrFunc __fun[121];
} tVectorTable;

/**
 **===========================================================================
 **  External declarations
 **===========================================================================
 */
#if __cplusplus
extern "C" {
#endif
extern uint32_t __vector_table[];
extern unsigned long _estack;
extern void __thumb_startup(void);
#if __cplusplus
}
#endif

void trace_clk_init(void)
{
	/* Set the trace clock to the core clock frequency */
	SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

	/* Enable the TRACE_CLKOUT pin function on PTA6 (alt7 function) */
	PORTA_PCR6 = ( PORT_PCR_MUX(0x7)
					| PORT_PCR_DSE_MASK);	// enable high drive strength to support high toggle rate
}
/********************************************************************/
void fb_clk_init(void)
{
	/* Enable the clock to the FlexBus module */
        SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

 	/* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
	PORTC_PCR3 = ( PORT_PCR_MUX(0x5)
					| PORT_PCR_DSE_MASK);	// enable high drive strength to support high toggle rate
}
/********************************************************************/
void enable_abort_button(void)
{
    /* Configure the PTA4 pin for its GPIO function */
    PORTA_PCR4 = PORT_PCR_MUX(0x1); // GPIO is alt1 function for this pin

    /* Configure the PTA4 pin for rising edge interrupts */
    PORTA_PCR4 |= PORT_PCR_IRQC(0x9);

    /* Enable the associated IRQ in the NVIC */
    ISR_ENABLE_VECT(103);
}

/**
 **===========================================================================
 **  Reset handler
 **===========================================================================
 */
void __init_hardware()
{
    /* Actual system clock frequency */
    int core_clk_khz;
    int periph_clk_khz;
    int mcg_clk_hz;
    int mcg_clk_khz;
    int pll_0_clk_khz;
    int pll_1_clk_khz;

	SCB_VTOR = (uint32_t)__vector_table; /* Set the interrupt vector table position */
	/*
		Disable the Watchdog because it may reset the core before entering main().
		There are 2 unlock words which shall be provided in sequence before
		accessing the control register.
	*/
	WDOG_UNLOCK = KINETIS_WDOG_UNLOCK_SEQ_1;
	WDOG_UNLOCK = KINETIS_WDOG_UNLOCK_SEQ_2;
	WDOG_STCTRLH = KINETIS_WDOG_DISABLED_CTRL;

  #ifdef SDRAM //SDRAM/PLL already initailized
       mcg_clk_hz = 120000000;
       pll_1_clk_khz = 150000;
       pll_0_clk_khz = mcg_clk_hz / 1000;
#else  // if not using dram script

     /* Setup Allowed Power modes */
     SMC_PMPROT = SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLP_MASK | SMC_PMPROT_AVLLS_MASK;
     /*
     * Enable all of the port clocks. These have to be enabled to configure
     * pin muxing options, so most code will need all of these on anyway.
     */
     SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK
				  );

    // releases hold with ACKISO:  Only has an effect if recovering from VLLS1, VLLS2, or VLLS3
    // if ACKISO is set you must clear ackiso before calling pll_init
    //    or pll init hangs waiting for OSC to initialize
    // if osc enabled in low power modes - enable it first before ack
    // if I/O needs to be maintained without glitches enable outputs and modules first before ack.
    if (PMC_REGSC &  PMC_REGSC_ACKISO_MASK)
        PMC_REGSC |= PMC_REGSC_ACKISO_MASK;

    #if defined(NO_PLL_INIT)
        mcg_clk_hz = 21000000; //FEI mode
    #elif defined (ASYNCH_MODE)
       /* Set the system dividers */
       /* NOTE: The PLL init will not configure the system clock dividers,
        * so they must be configured appropriately before calling the PLL
        * init function to ensure that clocks remain in valid ranges.
        */
        SIM_CLKDIV1 = ( 0
                        | SIM_CLKDIV1_OUTDIV1(0)
                        | SIM_CLKDIV1_OUTDIV2(1)
                        | SIM_CLKDIV1_OUTDIV3(1)
                        | SIM_CLKDIV1_OUTDIV4(3)
                      );
        /* Initialize PLL */
        /* PLL will be the source for MCG CLKOUT so the core, system, and flash clocks are derived from it */
        mcg_clk_hz = pll_init(CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                              LOW_POWER,     /* Set the oscillator for low power mode */
                              CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                              PLL0_PRDIV,    /* PLL predivider value */
                              PLL0_VDIV,     /* PLL multiplier */
                              MCGOUT);       /* Use the output from this PLL as the MCGOUT */
        /* Check the value returned from pll_init() to make sure there wasn't an error */
        if (mcg_clk_hz < 0x100)
        while(1);
        pll_0_clk_khz = mcg_clk_hz / 1000;

    #elif defined (SYNCH_MODE)
       /* Set the system dividers */
       /* NOTE: The PLL init will not configure the system clock dividers,
        * so they must be configured appropriately before calling the PLL
        * init function to ensure that clocks remain in valid ranges.
        */
        SIM_CLKDIV1 = ( 0
                        | SIM_CLKDIV1_OUTDIV1(0)
                        | SIM_CLKDIV1_OUTDIV2(2)
                        | SIM_CLKDIV1_OUTDIV3(2)
                        | SIM_CLKDIV1_OUTDIV4(5) );

       /* Initialize PLL1 */
       /* PLL1 will be the source MCGOUT and the DDR controller */
       mcg_clk_hz = pll_init(OSCINIT, /* Don't init the osc circuit, already done */
                                 OSC_0,      /* Use CLKIN0 as the input clock */
                                 CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                                 LOW_POWER,     /* Set the oscillator for low power mode */
                                 CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                                 PLL_1,         /* PLL to initialize, in this case PLL1 */
                                 PLL1_PRDIV,    /* PLL predivider value */
                                 PLL1_VDIV,     /* PLL multiplier */
                                 MCGOUT);   /* Don't use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
       if (mcg_clk_hz < 0x100)
         while(1);

       /* Initialize PLL0 */
       /* PLL0 is initialized, but not used as the MCGOUT */
       pll_0_clk_khz = (pll_init(NO_OSCINIT,   /* Initialize the oscillator circuit */
                             OSC_0,     /* Use CLKIN0 as the input clock */
                             CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                             LOW_POWER,     /* Set the oscillator for low power mode */
                             CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                             PLL_0,         /* PLL to initialize, in this case PLL0 */
                             PLL0_PRDIV,    /* PLL predivider value */
                             PLL0_VDIV,     /* PLL multiplier */
                             PLL_ONLY) / 1000);       /* Use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
       if ((pll_0_clk_khz * 1000) < 0x100)
         while(1);

       pll_1_clk_khz = mcg_clk_hz / 1000;

    #else
        #error "A PLL configuration for this platform is NOT defined"
    #endif
#endif

	/*
         * Use the value obtained from the pll_init function to define variables
	 * for the core clock in kHz and also the peripheral clock. These
	 * variables can be used by other functions that need awareness of the
	 * system frequency.
	 */
    mcg_clk_khz = mcg_clk_hz / 1000;
  	core_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV1_MASK) >> 28)+ 1);
  	periph_clk_khz = mcg_clk_khz / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV2_MASK) >> 24)+ 1);

  	/* For debugging purposes, enable the trace clock and/or FB_CLK so that
  	 * we'll be able to monitor clocks and know the PLL is at the frequency
  	 * that we expect.
  	 */
  	trace_clk_init();
  	fb_clk_init();
    /* Enable pin interrupt for the abort button - PTA4 */
    /* This pin could also be used as the NMI interrupt, but since the NMI
     * is level sensitive each button press will cause multiple interrupts.
     * Using the GPIO interrupt instead means we can configure for an edge
     * sensitive interrupt instead = one interrupt per button press.
     */
    enable_abort_button();
    /* Initialize the DDR if the project option if defined */
    #ifdef DDR_INIT
      twr_ddr2_script_init();
    #endif
}

/*********************************************************************************************/
/* Functon name : pll_init
 *
 * Mode transition: Option to move from FEI to PEE mode or to just initialize the PLL
 *
 * This function initializess either PLL0 or PLL1. Either OSC0 or OSC1 can be selected for the
 * reference clock source. The oscillators can be configured to use a crystal or take in an
 * external square wave clock.
 * NOTE : This driver does not presently (as of Sept 9 2011) support the use of OSC1 as the
 * reference clock for the MCGOUT clock used for the system clocks.
 * The PLL outputs a PLLCLK and PLLCLK2X. PLLCLK2X is the actual PLL frequency and PLLCLK is
 * half this frequency. PLLCLK is used for MCGOUT and is also typically used by the
 * peripherals that can select the PLL as a clock source. So the PLL frequency generated will
 * be twice the desired frequency.
 * Using the function parameter names the PLL frequency is calculated as follows:
 * PLL freq = ((crystal_val / prdiv_val) * vdiv_val)
 * Refer to the readme file in the mcg driver directory for examples of pll_init configurations.
 * All parameters must be provided, for example crystal_val must be provided even if the
 * oscillator associated with that parameter is already initialized.
 * The various passed parameters are checked to ensure they are within the allowed range. If any
 * of these checks fail the driver will exit and return a fail/error code. An error code will
 * also be returned if any error occurs during the PLL initialization sequence. Refer to the
 * readme file in the mcg driver directory for a list of all these codes.
 *
 * Parameters: crystal_val - external clock frequency in Hz either from a crystal or square
 *                           wave clock source
 *             hgo_val     - selects whether low power or high gain mode is selected
 *                           for the crystal oscillator. This has no meaning if an
 *                           external clock is used.
 *             erefs_val   - selects external clock (=0) or crystal osc (=1)
 *             prdiv_val   - value to divide the external clock source by to create the desired
 *                           PLL reference clock frequency
 *             vdiv_val    - value to multiply the PLL reference clock frequency by
 *             mcgout_select  - 0 if the PLL is just to be enabled, non-zero if the PLL is used
 *                              to provide the MCGOUT clock for the system.
 *
 * Return value : PLL frequency (Hz) divided by 2 or error code
 */

int pll_init(int crystal_val, unsigned char hgo_val, unsigned char erefs_val, signed char prdiv_val, signed char vdiv_val, unsigned char mcgout_select)
{
  unsigned char frdiv_val;
  unsigned char temp_reg;
  unsigned char prdiv, vdiv;
  short i;
  int ref_freq;
  int pll_freq;

  // check if in FEI mode
  if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
      (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
      (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
  {
    return 0x1;                                                     // return error code
  }

  // check external frequency is less than the maximum frequency
  if  (crystal_val > 50000000) {return 0x21;}

  // check crystal frequency is within spec. if crystal osc is being used as PLL ref
  if (erefs_val)
  {
    if ((crystal_val < 3000000) || (crystal_val > 32000000)) {return 0x22;} // return 1 if one of the available crystal options is not available
  }

  // make sure HGO will never be greater than 1. Could return an error instead if desired.
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

  // Check PLL divider settings are within spec.
  if ((prdiv_val < 1) || (prdiv_val > 25)) {return 0x41;}
  if ((vdiv_val < 24) || (vdiv_val > 55)) {return 0x42;}

  // Check PLL reference clock frequency is within spec.
  ref_freq = crystal_val / prdiv_val;
  if ((ref_freq < 2000000) || (ref_freq > 4000000)) {return 0x43;}

  // Check PLL output frequency is within spec.
  pll_freq = (crystal_val / prdiv_val) * vdiv_val;
  if ((pll_freq < 48000000) || (pll_freq > 100000000)) {return 0x45;}

  // configure the MCG_C2 register
  // the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
  // it still needs to be set correctly even if the oscillator is not being used

  temp_reg = MCG_C2;
  temp_reg &= ~(MCG_C2_RANGE0_MASK | MCG_C2_HGO0_MASK | MCG_C2_EREFS0_MASK); // clear fields before writing new values

  if (crystal_val <= 8000000)
  {
    temp_reg |= (MCG_C2_RANGE0(1) | (hgo_val << MCG_C2_HGO0_SHIFT) | (erefs_val << MCG_C2_EREFS0_SHIFT));
  }
  else
  {
    temp_reg |= (MCG_C2_RANGE0(2) | (hgo_val << MCG_C2_HGO0_SHIFT) | (erefs_val << MCG_C2_EREFS0_SHIFT));
  }
  MCG_C2 = temp_reg;

  // determine FRDIV based on reference clock frequency
  // since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here.
  if (crystal_val <= 1250000) {frdiv_val = 0;}
  else if (crystal_val <= 2500000) {frdiv_val = 1;}
  else if (crystal_val <= 5000000) {frdiv_val = 2;}
  else if (crystal_val <= 10000000) {frdiv_val = 3;}
  else if (crystal_val <= 20000000) {frdiv_val = 4;}
  else {frdiv_val = 5;}

  // Select external oscillator and Reference Divider and clear IREFS to start ext osc
  // If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
  // CLKS=2, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
  temp_reg = MCG_C1;
  temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK); // Clear values in these fields
  temp_reg |= (MCG_C1_CLKS(2) | MCG_C1_FRDIV(frdiv_val)); // Set the required CLKS and FRDIV values
  MCG_C1 = temp_reg;

  // if the external oscillator is used need to wait for OSCINIT to set
  if (erefs_val)
  {
    for (i = 0 ; i < 20000 ; i++)
    {
      if (MCG_S & MCG_S_OSCINIT0_MASK) break; // jump out early if OSCINIT sets before loop finishes
    }
  if (!(MCG_S & MCG_S_OSCINIT0_MASK)) return 0x23; // check bit is really set and return with error if not set
  }

  // wait for Reference clock Status bit to clear
  for (i = 0 ; i < 2000 ; i++)
  {
    if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
  }
  if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set

  // Wait for clock status bits to show clock source is ext ref clk
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) return 0x1A; // check EXT CLK is really selected and return with error if not

  // Now in FBE
  // It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
  // It is enabled here but can be removed if this is not required.
  //MCG_C6 |= MCG_C6_CME0_MASK;

  // Configure PLL
  // Configure MCG_C5
  // If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.
  temp_reg = MCG_C5;
  temp_reg &= ~MCG_C5_PRDIV0_MASK;
  temp_reg |= MCG_C5_PRDIV0(prdiv_val - 1);    //set PLL ref divider
  MCG_C5 = temp_reg;

  // Configure MCG_C6
  // The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
  // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
  temp_reg = MCG_C6; // store present C6 value
  temp_reg &= ~MCG_C6_VDIV0_MASK; // clear VDIV settings
  temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV0(vdiv_val - 24); // write new VDIV and enable PLL
  MCG_C6 = temp_reg; // update MCG_C6

  // wait for PLLST status bit to set
  for (i = 0 ; i < 2000 ; i++)
  {
    if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
  }
  if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

  // Wait for LOCK bit to set
  for (i = 0 ; i < 4000 ; i++)
  {
    if (MCG_S & MCG_S_LOCK0_MASK) break; // jump out early if LOCK sets before loop finishes
  }
  if (!(MCG_S & MCG_S_LOCK0_MASK)) return 0x44; // check bit is really set and return with error if not set

  // Use actual PLL settings to calculate PLL frequency
  prdiv = ((MCG_C5 & MCG_C5_PRDIV0_MASK) + 1);
  vdiv = ((MCG_C6 & MCG_C6_VDIV0_MASK) + 24);

  // now in PBE

  MCG_C1 &= ~MCG_C1_CLKS_MASK; // clear CLKS to switch CLKS mux to select PLL as MCG_OUT

  // Wait for clock status bits to update
  for (i = 0 ; i < 2000 ; i++)
  {
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) break; // jump out early if CLKST = 3 before loop finishes
  }
  if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) return 0x1B; // check CLKST is set correctly and return with error if not

  // Now in PEE

  return ((crystal_val / prdiv) * vdiv); //MCGOUT equals PLL output frequency
} // pll_init

/********************************************************************/

/**
 **===========================================================================
 **  Default interrupt handler
 **===========================================================================
 */
void Default_Handler()
{
	__asm("bkpt");
}

void isrINT_NMI(void)
{
  /* Write your interrupt code here ... */

}
/* end of isrINT_NMI */

#ifdef __cplusplus
extern "C" {
#endif
extern uint32_t __SP_INIT[];
extern void __thumb_startup( void );
#ifdef __cplusplus
}
#endif

/* Weak definitions of handlers point to Default_Handler if not implemented */
void NMI_Handler() __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler() __attribute__ ((weak, alias("Default_Handler")));
void MemManage_Handler() __attribute__ ((weak, alias("Default_Handler")));
void BusFault_Handler() __attribute__ ((weak, alias("Default_Handler")));
void UsageFault_Handler() __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler() __attribute__ ((weak, alias("Default_Handler")));
void DebugMonitor_Handler() __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler() __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler() __attribute__ ((weak, alias("Default_Handler")));

/* The Interrupt Vector Table */
void (* const InterruptVector[])() __attribute__ ((section(".vectortable"))) = {
    /* Processor exceptions */
    (void(*)(void)) &_estack, /* 00 */
    __thumb_startup, /* 01 */
    NMI_Handler, /* 02 */
    HardFault_Handler, /* 03 */
    MemManage_Handler, /* 04 */
    BusFault_Handler,/* 05 */
    UsageFault_Handler, /* 06 */
    0, /* 07 */
    0, /* 08 */
    0, /* 09 */
    0, /* 10 */
    SVC_Handler, /* 11 */
    DebugMonitor_Handler, /* 12 */
    0,/* 13 */
    PendSV_Handler,/* 14 */
    SysTick_Handler,/* 15 */

    /* Interrupts */
    Default_Handler,/* 16 */
    Default_Handler,/* 17 */
    Default_Handler,/* 18 */
    Default_Handler,/* 19 */
    Default_Handler,/* 20 */
    Default_Handler,/* 21 */
    Default_Handler,/* 22 */
    Default_Handler,/* 23 */
    Default_Handler,/* 24 */
    Default_Handler,/* 25 */
    Default_Handler,/* 26 */
    Default_Handler,/* 27 */
    Default_Handler,/* 28 */
    Default_Handler,/* 29 */
    Default_Handler,/* 30 */
    Default_Handler,/* 31 */
    Default_Handler,/* 32 */
    Default_Handler,/* 33 */
    Default_Handler,/* 34 */
    Default_Handler,/* 35 */
    Default_Handler,/* 36 */
    LLWUIsr,/* 37 */
    Default_Handler,/* 38 */
    Default_Handler,/* 39 */
    Default_Handler,/* 40 */
    Default_Handler,/* 41 */
    Default_Handler,/* 42 */
    Default_Handler,/* 43 */
    Default_Handler,/* 44 */
    Default_Handler,/* 45 */
    Default_Handler,/* 46 */
    Default_Handler,/* 47 */
    Default_Handler,/* 48 */
    Default_Handler,/* 49 */
    Default_Handler,/* 50 */
    Default_Handler,/* 51 */
    Default_Handler,/* 52 */
    Default_Handler,/* 53 */
    Default_Handler,/* 54 */
    Default_Handler,/* 55 */
    Default_Handler,/* 56 */
    Default_Handler,/* 57 */
    Default_Handler,/* 58 */
    Default_Handler,/* 59 */
    Default_Handler,/* 60 */
    Default_Handler,/* 61 */
    Default_Handler,/* 62 */
    Default_Handler,/* 63 */
    Default_Handler,/* 64 */
    Default_Handler,/* 65 */
    Default_Handler,/* 66 */
    Default_Handler,/* 67 */
    Default_Handler,/* 68 */
    Default_Handler,/* 69 */
    Default_Handler,/* 70 */
    Default_Handler,/* 71 */
    Default_Handler,/* 72 */
    Default_Handler,/* 73 */
    Default_Handler,/* 74 */
    Default_Handler,/* 75 */
    Default_Handler,/* 76 */
    Default_Handler,/* 77 */
    Default_Handler,/* 78 */
    Default_Handler,/* 79 */
    Default_Handler,/* 80 */
    Default_Handler,/* 81 */
    Default_Handler,/* 82 */
    Default_Handler,/* 83 */
    PITIsr,/* 84 */
    Default_Handler,/* 85 */
    Default_Handler,/* 86 */
    Default_Handler,/* 87 */
    Default_Handler,/* 88 */
    Default_Handler,/* 89 */
    Default_Handler,/* 90 */
    Default_Handler,/* 91 */
    Default_Handler,/* 92 */
    Default_Handler,/* 93 */
    Default_Handler,/* 94 */
    Default_Handler,/* 95 */
    Default_Handler,/* 96 */
    Default_Handler,/* 97 */
    Default_Handler,/* 98 */
    TSS_TSI0Isr,/* 99 */
    Default_Handler,/* 100 */
    Default_Handler,/* 101 */
    Default_Handler,/* 102 */
    Default_Handler,/* 103 */
    Default_Handler,/* 104 */
    Default_Handler,/* 105 */
    Default_Handler,/* 106 */
    LPSWIsr,/* 107 */
    Default_Handler,/* 108 */
    Default_Handler,/* 109 */
    Default_Handler,/* 110 */
    Default_Handler,/* 111 */
    Default_Handler,/* 112 */
    Default_Handler,/* 113 */
    Default_Handler,/* 114 */
    Default_Handler,/* 115 */
    Default_Handler,/* 116 */
    Default_Handler,/* 117 */
    Default_Handler,/* 118 */
    Default_Handler,/* 119 */
};
