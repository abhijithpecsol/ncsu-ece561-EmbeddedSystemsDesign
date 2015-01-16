/*
 * File:    mcg.c
 * Purpose: Driver for enabling the PLL in 1 of 4 options
 *
 * Notes:
 * Assumes the MCG mode is in the default FEI mode out of reset
 * One of 4 clocking oprions can be selected.
 * One of 16 crystal values can be used
 */

#include "common.h"
#include "mcg.h"
extern int core_clk_khz;
extern int core_clk_mhz;


unsigned char pll_init(unsigned char clk_option, unsigned char crystal_val)
{
  unsigned char mcg_freq;
  SIM_SCGC5 |= SIM_SCGC5_MCG_MASK | SIM_SCGC5_OSC1_MASK | SIM_SCGC5_OSC2_MASK;
  
  if (clk_option > 3) {return 0;} //return 0 if one of the available options is not selected
  if (crystal_val > 15) {return 1;} // return 1 if one of the available crystal options is not available
//This assumes that the MCG is in default FEI mode out of reset.

// First move to FBE mode
#if (defined(CANNED_OSC) || defined(EXT_CLK))
     MCG_C2 = 0;
#else
// Enable external oscillator, RANGE=1, HGO=1, EREFS=1, LP=0, IRCS=0
    MCG_C2 = MCG_C2_RANGE(1) | MCG_C2_HGO_MASK | MCG_C2_EREFS_MASK;
// If 1M feedback resistor is not placed then use this instead.
//     MCG_C2 = MCG_C2_RANGE(1)  | MCG_C2_EREFS_MASK;
#endif

    //clear ACKISO before setting up the oscillator
    PMC_REGSC |= PMC_REGSC_ACKISO_MASK;
    
// Select external oscillator and Reference Divider and clear IREFS to start ext osc
// CLKS=2, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
  MCG_C1 = MCG_C1_CLKS(2) | MCG_C1_FRDIV(3);

  /* if we aren't using an osc input we don't need to wait for the osc to init */
#if (!defined(CANNED_OSC) && !defined(EXT_CLK))
    while (!(MCG_S & MCG_S_OSCINIT_MASK)){};  // wait for oscillator to initialize
#endif

  while (MCG_S & MCG_S_IREFST_MASK){}; // wait for Reference clock Status bit to clear

  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){}; // Wait for clock status bits to show clock source is ext ref clk

// Now in FBE

// Configure PLL Ref Divider, PLLCLKEN=0, PLLSTEN=0, PRDIV=5
// The crystal frequency is used to select the PRDIV value. Only even frequency crystals are supported
// that will produce a 2MHz reference clock to the PLL.
  MCG_C5 = MCG_C5_PLLSTEN_MASK | MCG_C5_PRDIV(crystal_val); // Set PLL ref divider to match the crystal used & Enable PLL in Normal Stop Mode

  // Ensure MCG_C6 is at the reset default of 0. LOLIE disabled, PLL disabled, clk monitor disabled, PLL VCO divider is clear
  MCG_C6 = 0x0;
// Select the PLL VCO divider and system clock dividers depending on clocking option
  switch (clk_option) {
    case 0:
      // Set system options dividers
      //MCG=PLL, core = MCG
      SIM_CLKDIV0 =  SIM_CLKDIV0_OUTDIV(0);
      // Set the VCO divider and enable the PLL for 50MHz, LOLIE=0, PLLS=1, CME=0, VDIV=1
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(1); //VDIV = 1 (x25)
      mcg_freq = 50;
      break;
   case 1:
      // Set system options dividers
      //MCG=PLL, core = MCG/2
	  SIM_CLKDIV0 =  SIM_CLKDIV0_OUTDIV(1);
      // Set the VCO divider and enable the PLL for 100MHz, LOLIE=0, PLLS=1, CME=0, VDIV=26
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(26); //VDIV = 26 (x50)
      mcg_freq = 50;
      break;
    case 2:
      // Set system options dividers
      //MCG=PLL, core = MCG/2
      SIM_CLKDIV0 =  SIM_CLKDIV0_OUTDIV(1);
      // Set the VCO divider and enable the PLL for 96MHz, LOLIE=0, PLLS=1, CME=0, VDIV=24
      MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV(24); //VDIV = 24 (x48)
      mcg_freq = 48;
      break;
   case 3:
      // Set system options dividers
      //MCG=PLL, core = MCG
	  SIM_CLKDIV0 =  SIM_CLKDIV0_OUTDIV(0);
      // Set the VCO divider and enable the PLL for 48MHz, LOLIE=0, PLLS=1, CME=0, VDIV=0
      MCG_C6 = MCG_C6_PLLS_MASK; //VDIV = 0 (x24)
      mcg_freq = 48;
      break;
  }
  
  while (!(MCG_S & MCG_S_PLLST_MASK)){}; // wait for PLL status bit to set

  while (!(MCG_S & MCG_S_LOCK_MASK)){}; // Wait for LOCK bit to set

// Now running PBE Mode

// Transition into PEE by setting CLKS to 0
// CLKS=0, FRDIV=3, IREFS=0, IRCLKEN=0, IREFSTEN=0
  MCG_C1 &= ~MCG_C1_CLKS_MASK;

// Wait for clock status bits to update
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){};

// Now running PEE Mode

return mcg_freq;
} //pll_init
void pbe_pee(void)
{  
  MCG_C1 &= ~MCG_C1_CLKS_MASK; // select PLL as MCG_OUT
  // Wait for clock status bits to update 
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){}; 

  switch (CORE_CLK_MHZ) {
    case PLL50:
      core_clk_khz = 50000;
      break;
    case PLL100:
      core_clk_khz = 100000;
      break;
    case PLL96:
      core_clk_khz = 96000;
      break;  
    case PLL48:
      core_clk_khz = 48000;
      break;  
  }
      
  uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  
} 
/********************************************************************/
void mcg_fei_2_blpi(void)
{
    unsigned char temp_reg;
    // Transition from FEI -> FBI -> BLPI
  
     // Step 1: FEI -> FBI
    MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
    MCG_C2 |= MCG_C2_IRCS_MASK;  // Select fast (1MHz) internal reference
    temp_reg = MCG_C1;
    temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK);
    temp_reg |= (MCG_C1_CLKS(1) | MCG_C1_IREFS_MASK);  // Select internal reference (fast IREF clock @ 1MHz) as MCG clock source.
    MCG_C1 = temp_reg;
  
    while (!(MCG_S & MCG_S_IREFST_MASK)){}; // wait for Reference Status bit to update
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update
    
    // Step 4: FBI -> BLPI
    MCG_C1 |= MCG_C1_IREFSTEN_MASK;  // Keep internal reference clock running in STOP modes.
    MCG_C2 |= MCG_C2_LP_MASK;  // FLL remains disabled in bypassed modes.
    while (!(MCG_S & MCG_S_IREFST_MASK)){};  // Wait for Reference Status bit to update.
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update.
    core_clk_khz = 1728; // 1740 sample at work
    uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
         
} // end MCG FEI to BLPI
/********************************************************************/
void mcg_blpi_2_fei(void)
{
    unsigned char temp_reg;
    // Transition from BLPI to FEI: BLPI -> FBI -> FEI 
  
    // Step 1: BLPI -> FBI
    MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
    while (!(MCG_S & MCG_S_IREFST_MASK)){};  // Wait for Reference Status bit to update.
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update
    
    // Step 2: FBI -> FEI
    MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
    temp_reg = MCG_C2;  // assign temporary variable of MCG_C2 contents
    temp_reg &= ~MCG_C2_RANGE_MASK;  // set RANGE field location to zero
    temp_reg |= (0x2 << 0x4);  // OR in new values
    MCG_C1 = (MCG_C1_IREFS_MASK);  // Select internal clock as MCG source, FRDIV=%000, internal reference selected.
 
    while (!(MCG_S & MCG_S_IREFST_MASK)){};   // Wait for Reference Status bit to update 
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0){}; // Wait for clock status bits to update
	core_clk_mhz = 50;
	core_clk_khz = core_clk_mhz*1000;
    uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
    
} // end MCG BLPI to FEI

/********************************************************************/
void mcg_blpi_2_pee(void)
{
    unsigned char temp_reg;
    // Transition from BLPI to PEE: BLPI -> FBI -> FEI -> FBE -> PBE -> PEE
  
    // Step 1: BLPI -> FBI
    MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
    while (!(MCG_S & MCG_S_IREFST_MASK)){};  // Wait for Reference Status bit to update.
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update
    
    // Step 2: FBI -> FEI
    MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
    temp_reg = MCG_C2;  // assign temporary variable of MCG_C2 contents
    temp_reg &= ~MCG_C2_RANGE_MASK;  // set RANGE field location to zero
    temp_reg |= (0x2 << 0x4);  // OR in new values
    MCG_C2 = temp_reg;  // store new value in MCG_C2
    MCG_C4 = 0x0E;  // Low-range DCO output (~10MHz bus).  FCTRIM=%0111.
    MCG_C1 = 0x04;  // Select internal clock as MCG source, FRDIV=%000, internal reference selected.
 
    while (!(MCG_S & MCG_S_IREFST_MASK)){};   // Wait for Reference Status bit to update 
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x0){}; // Wait for clock status bits to update
    
    // Handle FEI to PEE transitions using standard clock initialization routine.
    core_clk_mhz = pll_init(CORE_CLK_MHZ, REF_CLK); 

    /* Use the value obtained from the pll_init function to define variables
    * for the core clock in kHz and also the peripheral clock. These
    * variables can be used by other functions that need awareness of the
    * system frequency.
    */
    core_clk_khz = core_clk_mhz * 1000;
    uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);

} // end MCG BLPI to PEE
/********************************************************************/
/*
 * autotrim.c
 *
 *  Created on: Mar 7, 2011
 *      Author: RYHG20
 */

void atc (unsigned int slow_fast, unsigned long int frequency)
{
    unsigned char clks_val;
    unsigned long int regvalue;
    unsigned int regindex;
    unsigned long int temp_reg = 0;
    unsigned char temp_reg8 = 0;
    unsigned char orig_SIM_CLKDIV0;
    unsigned char atcv;
    
    int result = 0;
         
    print_str("\n-------------------------------------------\n");
    print_str("  MCG AutoTrim Utility\n");
    print_str("-------------------------------------------\n");
    print_str("\n");
      
    regindex = slow_fast;
    regvalue = frequency;
    clks_val = ((MCG_C1 & MCG_C1_CLKS_MASK) >> 6);
        
    if (((clks_val == 1) || (clks_val ==3)))
        {
          print_str("\nAuto trim error - cannot use the internal clock source.\r\n\n");
          return; //error using IRC as ssytem clock
        }
              
        if ((clks_val == 0) && (!(MCG_C6 & MCG_C6_PLLS_MASK)) && (MCG_C1 & MCG_C1_IREFS_MASK))
        {
          print_str("\nAuto trim error - cannot use the FLL with internal clock source.\r\n\n");
          return; // error using FLL with IRC
        }
        if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 3)
        {
          print_str("\nWARNING - PLL is not the internal clock source. Auto trim value will not be correct\r\n\n");
        }
                
        if(!regindex) //determine if slow or fast IRC to be trimmed
        {
          // NOTE: IRCLKEN should not be required - 1.0 errata.
          // Remove IRCLKEN when silicon is fixed.         
          MCG_C2 &= ~MCG_C2_IRCS_MASK; //must select the slow IRC to be enabled
          MCG_C1 |= MCG_C1_IRCLKEN_MASK;
        } // if
        orig_SIM_CLKDIV0 = SIM_CLKDIV0; // bus clock needs to be between 8 and 16 MHz
        SIM_CLKDIV0 = SIM_CLKDIV0_OUTDIV(1); // Assume PLL = 50MHz, div by 2 gives bus clk = 12.5MHz
// Set up autocal registers, must use floating point calculation
        if (regindex)
          atcv = (unsigned char)(128.0f * (21.0f * (12500000.0f / (float)regvalue))); 
        else
          atcv = (unsigned char)(21.0f * (12500000.0f / (float)regvalue));
        
        MCG_ATCVL = (atcv & 0xFF); //Set ATCVL to lower 8 bits of count value
        MCG_ATCVH = ((atcv & 0xFF00) >> 8); // Set ATCVH to upper 8 bits of count value

// Enable autocal
        MCG_ATC = 0x0; // clear auto trim control register
        temp_reg8 |= (MCG_ATC_ATME_MASK | (regindex << MCG_ATC_ATMS_SHIFT)); //Select IRC to trim and enable trim machine
        MCG_ATC = temp_reg8;
        
        while (MCG_ATC & MCG_ATC_ATME_MASK) {}; //poll for ATME bit to clear
        
        SIM_CLKDIV0 = orig_SIM_CLKDIV0;     //restore the divider value
        
        if (MCG_ATC & MCG_ATC_ATMF_MASK)
        {
          print_str("Autotrim error.\r\n\n");
          print_str("\n");
          print_str("MCG_C1   = %#02X \r\n", (MCG_C1));
          print_str("MCG_C2   = %#02X \r\n", (MCG_C2));
          print_str("MCG_C3   = %#02X \r\n", (MCG_C3));
          print_str("MCG_C4   = %#02X \r\n", (MCG_C4)) ;
          print_str("MCG_C5   = %#02X \r\n", (MCG_C5));
          print_str("MCG_C6   = %#02X \r\n\n", (MCG_C6));
          print_str("MCG_S    = %#02X \r\n\n", (MCG_S)) ;
          print_str("MCG_ATC   = %#02X \r\n",   (MCG_ATC)) ;
          print_str("MCG_ATCVL = %#02X \r\n",   (MCG_ATCVL)) ;
          print_str("MCG_ATVCH = %#02X \r\n",   (MCG_ATCVH));
          MCG_ATC |= MCG_ATC_ATMF_MASK; // clear fail flag
        } 
        else 
        {      
          print_str("Autotrim Passed.\r\n\n");
          print_str("MCG_C3   = %#02X \r\n", (MCG_C3));
          print_str("MCG_C4   = %#02X \r\n", (MCG_C4));
          // Check trim value is not at either extreme of the range
          if (!regindex)
          {
            if ((MCG_C3 == 0xFF) || (MCG_C3 == 0))
            {
              print_str("\nAutotrim result is not valid.\r\n\n");
            }
            MCG_C1 &= ~MCG_C1_IRCLKEN_MASK; // also disable IRCLKEN (due to 1.0 errata)
          }
          else
          {
            if ((((MCG_C4 & MCG_C4_FCTRIM_MASK) >> MCG_C4_FCTRIM_SHIFT) == 0xF) ||
              (((MCG_C4 & MCG_C4_FCTRIM_MASK) >> MCG_C4_FCTRIM_SHIFT) == 0))
            {
              print_str("\nAutotrim result is not valid.\r\n\n");
            }
          }
        }
}// end cmd_atc
/********************************************************************/
void mcg_pee_2_blpi(void)
{
    unsigned char temp_reg;
    // Transition from PEE to BLPI: PEE -> PBE -> FBE -> FBI -> BLPI
  
    // Step 1: PEE -> PBE
    MCG_C1 |= MCG_C1_CLKS(2);  // System clock from external reference OSC, not PLL.
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2){};  // Wait for clock status to update.
    
    // Step 2: PBE -> FBE
    MCG_C6 &= ~MCG_C6_PLLS_MASK;  // Clear PLLS to select FLL, still running system from ext OSC.
    while (MCG_S & MCG_S_PLLST_MASK){};  // Wait for PLL status flag to reflect FLL selected.
    
    // Step 3: FBE -> FBI
    MCG_C2 &= ~MCG_C2_LP_MASK;  // FLL remains active in bypassed modes.
    MCG_C2 |= MCG_C2_IRCS_MASK;  // Select fast (1MHz) internal reference
    temp_reg = MCG_C1;
    temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_IREFS_MASK);
    temp_reg |= (MCG_C1_CLKS(1) | MCG_C1_IREFS_MASK);  // Select internal reference (fast IREF clock @ 1MHz) as MCG clock source.
    MCG_C1 = temp_reg;
  
    while (MCG_S & MCG_S_IREFST_MASK){};  // Wait for Reference Status bit to update.
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update
    
    // Step 4: FBI -> BLPI
    MCG_C1 |= MCG_C1_IREFSTEN_MASK;  // Keep internal reference clock running in STOP modes.
    MCG_C2 |= MCG_C2_LP_MASK;  // FLL remains disabled in bypassed modes.
    while (!(MCG_S & MCG_S_IREFST_MASK)){};  // Wait for Reference Status bit to update.
    while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x1){};  // Wait for clock status bits to update.
    core_clk_khz = 1740; // 1740 sample at work
    uart_init (TERM_PORT, core_clk_khz, TERMINAL_BAUD);
  
} // end MCG PEE to BLPI
/********************************************************************/
/********************************************************************/

void mcg_pbe_2_pee(void)
{  
  MCG_C1 &= ~MCG_C1_CLKS_MASK; // select PLL as MCG_OUT
  // Wait for clock status bits to update 
  while (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3){}; 

  switch (CORE_CLK_MHZ) {
    case PLL50:
      core_clk_khz = 50000;
      break;
    case PLL100:
      core_clk_khz = 100000;
      break;
    case PLL96:
      core_clk_khz = 96000;
      break;  
    case PLL48:
      core_clk_khz = 48000;
      break;  
  }
}
