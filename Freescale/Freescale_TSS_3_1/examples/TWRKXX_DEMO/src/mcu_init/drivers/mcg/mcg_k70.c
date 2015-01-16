/*
 * File:    mcg_k70.c
 * Purpose: Driver for enabling the PLL in 1 of 4 options
 *
 * Notes:
 *
 *
 *
 */

#include "common.h"
#include "mcg_k70.h"

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
#ifdef CPU_MK70F120
  // check external frequency is less than the maximum frequency
  if  (crystal_val > 60000000) {return 0x21;}
#elif defined CPU_MK20DZ50
  if  (crystal_val > 50000000) {return 0x21;}
#endif

  // check crystal frequency is within spec. if crystal osc is being used as PLL ref
  if (erefs_val)
  {
  #ifdef CPU_MK70F120
    if ((crystal_val < 8000000) || (crystal_val > 32000000)) {return 0x22;} // return 1 if one of the available crystal options is not available
  #elif defined CPU_MK20DZ50
	if ((crystal_val < 3000000) || (crystal_val > 32000000)) {return 0x22;} // return 1 if one of the available crystal options is not available
  #endif
  }

  // make sure HGO will never be greater than 1. Could return an error instead if desired.
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

  // Check PLL divider settings are within spec.
  if ((prdiv_val < 1) || (prdiv_val > 8)) {return 0x41;}
    #ifdef CPU_MK70F120
    if ((vdiv_val < 16) || (vdiv_val > 47)) {return 0x42;}
  #elif defined CPU_MK20DZ50
	if ((vdiv_val < 24) || (vdiv_val > 55)) {return 0x42;}
  #endif


  // Check PLL reference clock frequency is within spec.
  ref_freq = crystal_val / prdiv_val;
  #ifdef CPU_MK70F120
    if ((ref_freq < 8000000) || (ref_freq > 32000000)) {return 0x43;}
  #elif defined CPU_MK20DZ50
	 if ((ref_freq < 2000000) || (ref_freq > 4000000)) {return 0x43;}
  #endif


  // Check PLL output frequency is within spec.
  pll_freq = (crystal_val / prdiv_val) * vdiv_val;
  #ifdef CPU_MK70F120
  if ((pll_freq < 180000000) || (pll_freq > 360000000)) {return 0x45;}
  #elif defined CPU_MK20DZ50
  if ((pll_freq < 48000000) || (pll_freq > 100000000)) {return 0x45;}
  #endif
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
#ifdef CPU_MK70F120
    // wait for Reference clock Status bit to clear
    for (i = 0 ; i < 2000 ; i++)
    {
      if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
    }
    if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set
#endif
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
#ifdef CPU_MK70F120
  return (((crystal_val / prdiv) * vdiv) / 2); //MCGOUT equals PLL output frequency/2
#elif defined CPU_MK20DZ50
  return ((crystal_val / prdiv) * vdiv); //MCGOUT equals PLL output frequency
#endif
} // pll_init
