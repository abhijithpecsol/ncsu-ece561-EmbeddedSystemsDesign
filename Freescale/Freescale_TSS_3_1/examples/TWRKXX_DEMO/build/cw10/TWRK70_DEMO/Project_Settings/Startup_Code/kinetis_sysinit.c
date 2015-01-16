/*
 *    kinetis_sysinit.c - Default init routines for
 *                     		Kinetis ARM systems
 *    Copyright © 2010 Freescale semiConductor Inc. All Rights Reserved.
 */

#include "derivative.h"
#include "kinetis_sysinit.h"
#include "isr.h"

typedef void (*const tIsrFunc)(void);
typedef struct {
  uint32_t * __ptr;
  tIsrFunc __fun[121];
} tVectorTable;

extern uint32_t __vector_table[];

#pragma overload void __init_hardware();
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
        *(volatile unsigned short *)KINETIS_WDOG_UNLOCK_ADDR = KINETIS_WDOG_UNLOCK_SEQ_1;
        *(volatile unsigned short *)KINETIS_WDOG_UNLOCK_ADDR = KINETIS_WDOG_UNLOCK_SEQ_2;
        *(volatile unsigned short *)KINETIS_WDOG_STCTRLH_ADDR = KINETIS_WDOG_DISABLED_CTRL;
        
#ifdef SDRAM //SDRAM/PLL already initailized
       mcg_clk_hz = 120000000;
       pll_1_clk_khz = 150000;
       pll_0_clk_khz = mcg_clk_hz / 1000;
#else  // if not using dram script
    /*
     * Enable all of the port clocks. These have to be enabled to configure
     * pin muxing options, so most code will need all of these on anyway.
     */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK
                  | SIM_SCGC5_PORTC_MASK
                  | SIM_SCGC5_PORTD_MASK
                  | SIM_SCGC5_PORTE_MASK
                  | SIM_SCGC5_PORTF_MASK );

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
                        | SIM_CLKDIV1_OUTDIV4(5) );

       /* Initialize PLL0 */
       /* PLL0 will be the source for MCG CLKOUT so the core, system, FlexBus, and flash clocks are derived from it */
       mcg_clk_hz = pll_init(OSCINIT,   /* Initialize the oscillator circuit */
                             OSC_0,     /* Use CLKIN0 as the input clock */
                             CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                             LOW_POWER,     /* Set the oscillator for low power mode */
                             CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                             PLL_0,         /* PLL to initialize, in this case PLL0 */
                             PLL0_PRDIV,    /* PLL predivider value */
                             PLL0_VDIV,     /* PLL multiplier */
                             MCGOUT);       /* Use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
       if (mcg_clk_hz < 0x100)
         while(1);

       /* Initialize PLL1 */
       /* PLL1 will be the source for the DDR controller, but NOT the MCGOUT */
       pll_1_clk_khz = (pll_init(NO_OSCINIT, /* Don't init the osc circuit, already done */
                                 OSC_0,      /* Use CLKIN0 as the input clock */
                                 CLK0_FREQ_HZ,  /* CLKIN0 frequency */
                                 LOW_POWER,     /* Set the oscillator for low power mode */
                                 CLK0_TYPE,     /* Crystal or canned oscillator clock input */
                                 PLL_1,         /* PLL to initialize, in this case PLL1 */
                                 PLL1_PRDIV,    /* PLL predivider value */
                                 PLL1_VDIV,     /* PLL multiplier */
                                 PLL_ONLY) / 1000);   /* Don't use the output from this PLL as the MCGOUT */

       /* Check the value returned from pll_init() to make sure there wasn't an error */
       if ((pll_1_clk_khz * 1000) < 0x100)
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
//    trace_clk_init();
//      fb_clk_init();

        /* Initialize the DDR if the project option if defined */
        #ifdef DDR_INIT
          twr_ddr2_script_init();
        #endif
   
}

int pll_init(unsigned char init_osc, unsigned char osc_select, int crystal_val, unsigned char hgo_val, unsigned char erefs_val, unsigned char pll_select, signed char prdiv_val, signed char vdiv_val, unsigned char mcgout_select)
{
  unsigned char frdiv_val;
  unsigned char temp_reg;
  unsigned char prdiv, vdiv;
  short i;
  int ref_freq;
  int pll_freq;

  // If using the PLL as MCG_OUT must check if the MCG is in FEI mode first
  if (mcgout_select)
  {
    // check if in FEI mode
    if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
        (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
        (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
    {
      return 0x1;                                                     // return error code
    }
  } // if (mcgout_select)

  // Check if OSC1 is being used as a reference for the MCGOUT PLL
  // This requires a more complicated MCG configuration.
  // At this time (Sept 8th 2011) this driver does not support this option
  if (osc_select && mcgout_select)
  {
    return 0x80; // Driver does not support using OSC1 as the PLL reference for the system clock on MCGOUT
  }

  // check external frequency is less than the maximum frequency
  if  (crystal_val > 60000000) {return 0x21;}

  // check crystal frequency is within spec. if crystal osc is being used as PLL ref
  if (erefs_val)
  {
    if ((crystal_val < 8000000) || (crystal_val > 32000000)) {return 0x22;} // return 1 if one of the available crystal options is not available
  }

  // make sure HGO will never be greater than 1. Could return an error instead if desired.
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

  // Check PLL divider settings are within spec.
  if ((prdiv_val < 1) || (prdiv_val > 8)) {return 0x41;}
  if ((vdiv_val < 16) || (vdiv_val > 47)) {return 0x42;}

  // Check PLL reference clock frequency is within spec.
  ref_freq = crystal_val / prdiv_val;
  if ((ref_freq < 8000000) || (ref_freq > 32000000)) {return 0x43;}

  // Check PLL output frequency is within spec.
  pll_freq = (crystal_val / prdiv_val) * vdiv_val;
  if ((pll_freq < 180000000) || (pll_freq > 360000000)) {return 0x45;}

  // Determine if oscillator needs to be set up
  if (init_osc)
  {
    // Check if the oscillator needs to be configured
    if (!osc_select)
    {
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
        // On rev. 1.0 of silicon there is an issue where the the input bufferd are enabled when JTAG is connected.
        // This has the affect of sometimes preventing the oscillator from running. To keep the oscillator amplitude
        // low, RANGE = 2 should not be used. This should be removed when fixed silicon is available.
        //temp_reg |= (MCG_C2_RANGE(2) | (hgo_val << MCG_C2_HGO_SHIFT) | (erefs_val << MCG_C2_EREFS_SHIFT));
        temp_reg |= (MCG_C2_RANGE0(1) | (hgo_val << MCG_C2_HGO0_SHIFT) | (erefs_val << MCG_C2_EREFS0_SHIFT));
      }
      MCG_C2 = temp_reg;
    }
    else
    {
      // configure the MCG_C10 register
      // the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
      // it still needs to be set correctly even if the oscillator is not being used
      temp_reg = MCG_C10;
      temp_reg &= ~(MCG_C10_RANGE1_MASK | MCG_C10_HGO1_MASK | MCG_C10_EREFS1_MASK); // clear fields before writing new values
      if (crystal_val <= 8000000)
      {
        temp_reg |= MCG_C10_RANGE1(1) | (hgo_val << MCG_C10_HGO1_SHIFT) | (erefs_val << MCG_C10_EREFS1_SHIFT);
      }
      else
      {
        // On rev. 1.0 of silicon there is an issue where the the input bufferd are enabled when JTAG is connected.
        // This has the affect of sometimes preventing the oscillator from running. To keep the oscillator amplitude
        // low, RANGE = 2 should not be used. This should be removed when fixed silicon is available.
        //temp_reg |= MCG_C10_RANGE2(2) | (hgo_val << MCG_C10_HGO2_SHIFT) | (erefs_val << MCG_C10_EREFS2_SHIFT);
        temp_reg |= MCG_C10_RANGE1(1) | (hgo_val << MCG_C10_HGO1_SHIFT) | (erefs_val << MCG_C10_EREFS1_SHIFT);
      }
      MCG_C10 = temp_reg;
    } // if (!osc_select)
  } // if (init_osc)

  if (mcgout_select)
  {
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
    temp_reg = MCG_C1_CLKS(2) | MCG_C1_FRDIV(frdiv_val); // Set the required CLKS and FRDIV values
    MCG_C1 = temp_reg;

    // if the external oscillator is used need to wait for OSCINIT to set
    if (erefs_val)
    {
      for (i = 0 ; i < 10000 ; i++)
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
    MCG_C6 |= MCG_C6_CME0_MASK;

    // Select which PLL to enable
    if (!pll_select)
    {
      // Configure PLL0
      // Ensure OSC0 is selected as the reference clock
      MCG_C5 &= ~MCG_C5_PLLREFSEL0_MASK;
      //Select PLL0 as the source of the PLLS mux
      MCG_C11 &= ~MCG_C11_PLLCS_MASK;
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
      temp_reg |= MCG_C6_PLLS_MASK | MCG_C6_VDIV0(vdiv_val - 16); // write new VDIV and enable PLL
      MCG_C6 = temp_reg; // update MCG_C6

      // wait for PLLST status bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
      }
      if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

      // Wait for LOCK bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S & MCG_S_LOCK0_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S & MCG_S_LOCK0_MASK)) return 0x44; // check bit is really set and return with error if not set

      // Use actual PLL settings to calculate PLL frequency
      prdiv = ((MCG_C5 & MCG_C5_PRDIV0_MASK) + 1);
      vdiv = ((MCG_C6 & MCG_C6_VDIV0_MASK) + 16);
    }
    else
    {
      // Configure PLL1
      // Ensure OSC0 is selected as the reference clock
      MCG_C11 &= ~MCG_C11_PLLREFSEL1_MASK;
      //Select PLL1 as the source of the PLLS mux
      MCG_C11 |= MCG_C11_PLLCS_MASK;
      // Configure MCG_C11
      // If the PLL is to run in STOP mode then the PLLSTEN2 bit needs to be OR'ed in here or in user code.
      temp_reg = MCG_C11;
      temp_reg &= ~MCG_C11_PRDIV1_MASK;
      temp_reg |= MCG_C11_PRDIV1(prdiv_val - 1);    //set PLL ref divider
      MCG_C11 = temp_reg;

      // Configure MCG_C12
      // The PLLS bit is set to enable the PLL, MCGOUT still sourced from ext ref clk
      // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE2 bit in MCG_C12
      temp_reg = MCG_C12; // store present C12 value
      temp_reg &= ~MCG_C12_VDIV1_MASK; // clear VDIV settings
      temp_reg |=  MCG_C12_VDIV1(vdiv_val - 16); // write new VDIV and enable PLL
      MCG_C12 = temp_reg; // update MCG_C12
      // Enable PLL by setting PLLS bit
      MCG_C6 |= MCG_C6_PLLS_MASK;

      // wait for PLLCST status bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S2 & MCG_S2_PLLCST_MASK) break; // jump out early if PLLST sets before loop finishes
      }
      if (!(MCG_S2 & MCG_S2_PLLCST_MASK)) return 0x17; // check bit is really set and return with error if not set

      // wait for PLLST status bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S & MCG_S_PLLST_MASK) break; // jump out early if PLLST sets before loop finishes
      }
      if (!(MCG_S & MCG_S_PLLST_MASK)) return 0x16; // check bit is really set and return with error if not set

      // Wait for LOCK bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S2 & MCG_S2_LOCK1_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S2 & MCG_S2_LOCK1_MASK)) return 0x44; // check bit is really set and return with error if not set

      // Use actual PLL settings to calculate PLL frequency
      prdiv = ((MCG_C11 & MCG_C11_PRDIV1_MASK) + 1);
      vdiv = ((MCG_C12 & MCG_C12_VDIV1_MASK) + 16);
    } // if (!pll_select)

    // now in PBE

    MCG_C1 &= ~MCG_C1_CLKS_MASK; // clear CLKS to switch CLKS mux to select PLL as MCG_OUT

    // Wait for clock status bits to update
    for (i = 0 ; i < 2000 ; i++)
    {
      if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x3) break; // jump out early if CLKST = 3 before loop finishes
    }
    if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) return 0x1B; // check CLKST is set correctly and return with error if not

    // Now in PEE
  }
  else
  {
    // Setup PLL for peripheral only use
    if (pll_select)
    {
      // Setup and enable PLL1
      // Select ref source
      if (osc_select)
      {
        MCG_C11 |= MCG_C11_PLLREFSEL1_MASK; // Set select bit to choose OSC1
      }
      else
      {
        MCG_C11 &= ~MCG_C11_PLLREFSEL1_MASK; // Clear select bit to choose OSC0
      }
      // Configure MCG_C11
      // If the PLL is to run in STOP mode then the PLLSTEN2 bit needs to be OR'ed in here or in user code.
      temp_reg = MCG_C11;
      temp_reg &= ~MCG_C11_PRDIV1_MASK;
      temp_reg |= MCG_C11_PRDIV1(prdiv_val - 1);    //set PLL ref divider
      MCG_C11 = temp_reg;

      // Configure MCG_C12
      // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE2 bit in MCG_C12
      temp_reg = MCG_C12; // store present C12 value
      temp_reg &= ~MCG_C12_VDIV1_MASK; // clear VDIV settings
      temp_reg |=  MCG_C12_VDIV1(vdiv_val - 16); // write new VDIV and enable PLL
      MCG_C12 = temp_reg; // update MCG_C12
      // Now enable the PLL
      MCG_C11 |= MCG_C11_PLLCLKEN1_MASK; // Set PLLCLKEN2 to enable PLL1

      // Wait for LOCK bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S2 & MCG_S2_LOCK1_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S2 & MCG_S2_LOCK1_MASK)) return 0x44; // check bit is really set and return with error if not set

      // Use actual PLL settings to calculate PLL frequency
      prdiv = ((MCG_C11 & MCG_C11_PRDIV1_MASK) + 1);
      vdiv = ((MCG_C12 & MCG_C12_VDIV1_MASK) + 16);
    }
    else
    {
      // Setup and enable PLL0
      // Select ref source
      if (osc_select)
      {
        MCG_C5 |= MCG_C5_PLLREFSEL0_MASK; // Set select bit to choose OSC1
      }
      else
      {
        MCG_C5 &= ~MCG_C5_PLLREFSEL0_MASK; // Clear select bit to choose OSC0
      }
      // Configure MCG_C5
      // If the PLL is to run in STOP mode then the PLLSTEN bit needs to be OR'ed in here or in user code.
      temp_reg = MCG_C5;
      temp_reg &= ~MCG_C5_PRDIV0_MASK;
      temp_reg |= MCG_C5_PRDIV0(prdiv_val - 1);    //set PLL ref divider
      MCG_C5 = temp_reg;

      // Configure MCG_C6
      // The loss of lock interrupt can be enabled by seperately OR'ing in the LOLIE bit in MCG_C6
      temp_reg = MCG_C6; // store present C6 value
      temp_reg &= ~MCG_C6_VDIV0_MASK; // clear VDIV settings
      temp_reg |=  MCG_C6_VDIV0(vdiv_val - 16); // write new VDIV and enable PLL
      MCG_C6 = temp_reg; // update MCG_C6
      // Now enable the PLL
      MCG_C5 |= MCG_C5_PLLCLKEN0_MASK; // Set PLLCLKEN to enable PLL0

      // Wait for LOCK bit to set
      for (i = 0 ; i < 2000 ; i++)
      {
        if (MCG_S & MCG_S_LOCK0_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S & MCG_S_LOCK0_MASK)) return 0x44; // check bit is really set and return with error if not set

      // Use actual PLL settings to calculate PLL frequency
      prdiv = ((MCG_C5 & MCG_C5_PRDIV0_MASK) + 1);
      vdiv = ((MCG_C6 & MCG_C6_VDIV0_MASK) + 16);
    } // if (pll_select)

  } // if (mcgout_select)

  return (((crystal_val / prdiv) * vdiv) / 2); //MCGOUT equals PLL output frequency/2
} // pll_init

/********************************************************************/
void trace_clk_init(void)
{
    /* Set the trace clock to the core clock frequency */
    SIM_SOPT2 |= SIM_SOPT2_TRACECLKSEL_MASK;

    /* Enable the TRACE_CLKOUT pin function on PTF23 (alt6 function) */
    PORTF_PCR23 = ( PORT_PCR_MUX(0x6));
}


/********************************************************************/
void fb_clk_init(void)
{
    /* Enable the clock to the FlexBus module */
        SIM_SCGC7 |= SIM_SCGC7_FLEXBUS_MASK;

     /* Enable the FB_CLKOUT function on PTC3 (alt5 function) */
    PORTC_PCR3 = ( PORT_PCR_MUX(0x5)
                    | PORT_PCR_DSE_MASK);    // enable high drive strength to support high toggle rate
}
/********************************************************************/

void isr_default(void)
{
  /* Write your interrupt code here ... */
    
  #define IRQNUM                     (*(volatile uint8_t*)(0xE000ED04))
    
  printf("\n****default_isr entered on vector %d*****\r\n\n",IRQNUM);
  return;
}
/* end of isr_default */

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

/* Interrupt vector table */
#ifndef UNASSIGNED_ISR
  #define UNASSIGNED_ISR isr_default   /* unassigned interrupt service routine */
#endif

#pragma define_section vectortable ".vectortable" ".vectortable" ".vectortable" far_abs R
static __declspec(vectortable) tVectorTable __vect_table = { /* Interrupt vector table */
   __SP_INIT,                                              /* 0 (0x00000000) (prior: -) */
   {
   (tIsrFunc)__thumb_startup,                              /* 1 (0x00000004) (prior: -) */
   (tIsrFunc)isrINT_NMI,                                   /* 2 (0x00000008) (prior: -2) */
   (tIsrFunc)VECTOR_003,                               /* 3 (0x0000000C) (prior: -1) */
   (tIsrFunc)VECTOR_004,                               /* 4 (0x00000010) (prior: -) */
   (tIsrFunc)VECTOR_005,                               /* 5 (0x00000014) (prior: -) */
   (tIsrFunc)VECTOR_006,                               /* 6 (0x00000018) (prior: -) */
   (tIsrFunc)VECTOR_007,                               /* 7 (0x0000001C) (prior: -) */
   (tIsrFunc)VECTOR_008,                               /* 8 (0x00000020) (prior: -) */
   (tIsrFunc)VECTOR_009,                               /* 9 (0x00000024) (prior: -) */
   (tIsrFunc)VECTOR_010,                               /* 10 (0x00000028) (prior: -) */
   (tIsrFunc)VECTOR_011,                               /* 11 (0x0000002C) (prior: -) */
   (tIsrFunc)VECTOR_012,                               /* 12 (0x00000030) (prior: -) */
   (tIsrFunc)VECTOR_013,                               /* 13 (0x00000034) (prior: -) */
   (tIsrFunc)VECTOR_014,                               /* 14 (0x00000038) (prior: -) */
   (tIsrFunc)VECTOR_015,                               /* 15 (0x0000003C) (prior: -) */
   (tIsrFunc)VECTOR_016,                               /* 16 (0x00000040) (prior: -) */
   (tIsrFunc)VECTOR_017,                               /* 17 (0x00000044) (prior: -) */
   (tIsrFunc)VECTOR_018,                               /* 18 (0x00000048) (prior: -) */
   (tIsrFunc)VECTOR_019,                               /* 19 (0x0000004C) (prior: -) */
   (tIsrFunc)VECTOR_020,                               /* 20 (0x00000050) (prior: -) */
   (tIsrFunc)VECTOR_021,                               /* 21 (0x00000054) (prior: -) */
   (tIsrFunc)VECTOR_022,                               /* 22 (0x00000058) (prior: -) */
   (tIsrFunc)VECTOR_023,                               /* 23 (0x0000005C) (prior: -) */
   (tIsrFunc)VECTOR_024,                               /* 24 (0x00000060) (prior: -) */
   (tIsrFunc)VECTOR_025,                               /* 25 (0x00000064) (prior: -) */
   (tIsrFunc)VECTOR_026,                               /* 26 (0x00000068) (prior: -) */
   (tIsrFunc)VECTOR_027,                               /* 27 (0x0000006C) (prior: -) */
   (tIsrFunc)VECTOR_028,                               /* 28 (0x00000070) (prior: -) */
   (tIsrFunc)VECTOR_029,                               /* 29 (0x00000074) (prior: -) */
   (tIsrFunc)VECTOR_030,                               /* 30 (0x00000078) (prior: -) */
   (tIsrFunc)VECTOR_031,                               /* 31 (0x0000007C) (prior: -) */
   (tIsrFunc)VECTOR_032,                               /* 32 (0x00000080) (prior: -) */
   (tIsrFunc)VECTOR_033,                               /* 33 (0x00000084) (prior: -) */
   (tIsrFunc)VECTOR_034,                               /* 34 (0x00000088) (prior: -) */
   (tIsrFunc)VECTOR_035,                               /* 35 (0x0000008C) (prior: -) */
   (tIsrFunc)VECTOR_036,                               /* 36 (0x00000090) (prior: -) */
   (tIsrFunc)VECTOR_037,                               /* 37 (0x00000094) (prior: -) */
   (tIsrFunc)VECTOR_038,                               /* 38 (0x00000098) (prior: -) */
   (tIsrFunc)VECTOR_039,                               /* 39 (0x0000009C) (prior: -) */
   (tIsrFunc)VECTOR_040,                               /* 40 (0x000000A0) (prior: -) */
   (tIsrFunc)VECTOR_041,                               /* 41 (0x000000A4) (prior: -) */
   (tIsrFunc)VECTOR_042,                               /* 42 (0x000000A8) (prior: -) */
   (tIsrFunc)VECTOR_043,                               /* 43 (0x000000AC) (prior: -) */
   (tIsrFunc)VECTOR_044,                               /* 44 (0x000000B0) (prior: -) */
   (tIsrFunc)VECTOR_045,                               /* 45 (0x000000B4) (prior: -) */
   (tIsrFunc)VECTOR_046,                               /* 46 (0x000000B8) (prior: -) */
   (tIsrFunc)VECTOR_047,                               /* 47 (0x000000BC) (prior: -) */
   (tIsrFunc)VECTOR_048,                               /* 48 (0x000000C0) (prior: -) */
   (tIsrFunc)VECTOR_049,                               /* 49 (0x000000C4) (prior: -) */
   (tIsrFunc)VECTOR_050,                               /* 50 (0x000000C8) (prior: -) */
   (tIsrFunc)VECTOR_051,                               /* 51 (0x000000CC) (prior: -) */
   (tIsrFunc)VECTOR_052,                               /* 52 (0x000000D0) (prior: -) */
   (tIsrFunc)VECTOR_053,                               /* 53 (0x000000D4) (prior: -) */
   (tIsrFunc)VECTOR_054,                               /* 54 (0x000000D8) (prior: -) */
   (tIsrFunc)VECTOR_055,                               /* 55 (0x000000DC) (prior: -) */
   (tIsrFunc)VECTOR_056,                               /* 56 (0x000000E0) (prior: -) */
   (tIsrFunc)VECTOR_057,                               /* 57 (0x000000E4) (prior: -) */
   (tIsrFunc)VECTOR_058,                               /* 58 (0x000000E8) (prior: -) */
   (tIsrFunc)VECTOR_059,                               /* 59 (0x000000EC) (prior: -) */
   (tIsrFunc)VECTOR_060,                               /* 60 (0x000000F0) (prior: -) */
   (tIsrFunc)VECTOR_061,                               /* 61 (0x000000F4) (prior: -) */
   (tIsrFunc)VECTOR_062,                               /* 62 (0x000000F8) (prior: -) */
   (tIsrFunc)VECTOR_063,                               /* 63 (0x000000FC) (prior: -) */
   (tIsrFunc)VECTOR_064,                               /* 64 (0x00000100) (prior: -) */
   (tIsrFunc)VECTOR_065,                               /* 65 (0x00000104) (prior: -) */
   (tIsrFunc)VECTOR_066,                               /* 66 (0x00000108) (prior: -) */
   (tIsrFunc)VECTOR_067,                               /* 67 (0x0000010C) (prior: -) */
   (tIsrFunc)VECTOR_068,                               /* 68 (0x00000110) (prior: -) */
   (tIsrFunc)VECTOR_069,                               /* 69 (0x00000114) (prior: -) */
   (tIsrFunc)VECTOR_070,                               /* 70 (0x00000118) (prior: -) */
   (tIsrFunc)VECTOR_071,                               /* 71 (0x0000011C) (prior: -) */
   (tIsrFunc)VECTOR_072,                               /* 72 (0x00000120) (prior: -) */
   (tIsrFunc)VECTOR_073,                               /* 73 (0x00000124) (prior: -) */
   (tIsrFunc)VECTOR_074,                               /* 74 (0x00000128) (prior: -) */
   (tIsrFunc)VECTOR_075,                               /* 75 (0x0000012C) (prior: -) */
   (tIsrFunc)VECTOR_076,                               /* 76 (0x00000130) (prior: -) */
   (tIsrFunc)VECTOR_077,                               /* 77 (0x00000134) (prior: -) */
   (tIsrFunc)VECTOR_078,                               /* 78 (0x00000138) (prior: -) */
   (tIsrFunc)VECTOR_079,                               /* 79 (0x0000013C) (prior: -) */
   (tIsrFunc)VECTOR_080,                               /* 80 (0x00000140) (prior: -) */
   (tIsrFunc)VECTOR_081,                               /* 81 (0x00000144) (prior: -) */
   (tIsrFunc)VECTOR_082,                               /* 82 (0x00000148) (prior: -) */
   (tIsrFunc)VECTOR_083,                               /* 83 (0x0000014C) (prior: -) */
   (tIsrFunc)VECTOR_084,                               /* 84 (0x00000150) (prior: -) */
   (tIsrFunc)VECTOR_085,                               /* 85 (0x00000154) (prior: -) */
   (tIsrFunc)VECTOR_086,                               /* 86 (0x00000158) (prior: -) */
   (tIsrFunc)VECTOR_087,                               /* 87 (0x0000015C) (prior: -) */
   (tIsrFunc)VECTOR_088,                               /* 88 (0x00000160) (prior: -) */
   (tIsrFunc)VECTOR_089,                               /* 89 (0x00000164) (prior: -) */
   (tIsrFunc)VECTOR_090,                               /* 90 (0x00000168) (prior: -) */
   (tIsrFunc)VECTOR_091,                               /* 91 (0x0000016C) (prior: -) */
   (tIsrFunc)VECTOR_092,                               /* 92 (0x00000170) (prior: -) */
   (tIsrFunc)VECTOR_093,                               /* 93 (0x00000174) (prior: -) */
   (tIsrFunc)VECTOR_094,                               /* 94 (0x00000178) (prior: -) */
   (tIsrFunc)VECTOR_095,                               /* 95 (0x0000017C) (prior: -) */
   (tIsrFunc)VECTOR_096,                               /* 96 (0x00000180) (prior: -) */
   (tIsrFunc)VECTOR_097,                               /* 97 (0x00000184) (prior: -) */
   (tIsrFunc)VECTOR_098,                               /* 98 (0x00000188) (prior: -) */
   (tIsrFunc)VECTOR_099,                               /* 99 (0x0000018C) (prior: -) */
   (tIsrFunc)VECTOR_100,                               /* 100 (0x00000190) (prior: -) */
   (tIsrFunc)VECTOR_101,                               /* 101 (0x00000194) (prior: -) */
   (tIsrFunc)VECTOR_102,                               /* 102 (0x00000198) (prior: -) */
   (tIsrFunc)VECTOR_103,                               /* 103 (0x0000019C) (prior: -) */
   (tIsrFunc)VECTOR_104,                               /* 104 (0x000001A0) (prior: -) */
   (tIsrFunc)VECTOR_105,                               /* 105 (0x000001A4) (prior: -) */
   (tIsrFunc)VECTOR_106,                               /* 106 (0x000001A8) (prior: -) */
   (tIsrFunc)VECTOR_107,                               /* 107 (0x000001AC) (prior: -) */
   (tIsrFunc)VECTOR_108,                               /* 108 (0x000001B0) (prior: -) */
   (tIsrFunc)VECTOR_109,                               /* 109 (0x000001B4) (prior: -) */
   (tIsrFunc)VECTOR_110,                               /* 110 (0x000001B8) (prior: -) */
   (tIsrFunc)VECTOR_111,                               /* 111 (0x000001BC) (prior: -) */
   (tIsrFunc)VECTOR_112,                               /* 112 (0x000001C0) (prior: -) */
   (tIsrFunc)VECTOR_113,                               /* 113 (0x000001C4) (prior: -) */
   (tIsrFunc)VECTOR_114,                               /* 114 (0x000001C8) (prior: -) */
   (tIsrFunc)VECTOR_115,                               /* 115 (0x000001CC) (prior: -) */
   (tIsrFunc)VECTOR_116,                               /* 116 (0x000001D0) (prior: -) */
   (tIsrFunc)VECTOR_117,                               /* 117 (0x000001D4) (prior: -) */
   (tIsrFunc)VECTOR_118,                               /* 118 (0x000001D8) (prior: -) */
   (tIsrFunc)VECTOR_119,                               /* 119 (0x000001DC) (prior: -) */
   (tIsrFunc)VECTOR_120,                               /* 120 (0x000001E0) (prior: -) */
   (tIsrFunc)VECTOR_121                                /* 121 (0x000001E4) (prior: -) */
   }
};
