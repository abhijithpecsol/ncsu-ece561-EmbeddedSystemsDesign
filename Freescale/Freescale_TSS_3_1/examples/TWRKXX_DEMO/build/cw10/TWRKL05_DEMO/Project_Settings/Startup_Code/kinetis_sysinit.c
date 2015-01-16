/*
 *    kinetis_sysinit.c - Default init routines for Flycatcher
 *                     		Kinetis ARM systems
 *    Copyright © 2012 Freescale semiConductor Inc. All Rights Reserved.
 */

#include "derivative.h"
#include "kinetis_sysinit.h"
#include "isr.h"

/* Actual system clock frequency */
int mcg_clk_hz;

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

/**
 **===========================================================================
 **  Default interrupt handler
 **===========================================================================
 */
void Default_Handler()
{
	__asm("bkpt");
}

int fll_freq(int fll_ref)
{
  int fll_freq_hz = 0u;

  if (MCG_C4 & MCG_C4_DMX32_MASK) // if DMX32 set
  {
    switch ((MCG_C4 & MCG_C4_DRST_DRS_MASK) >> MCG_C4_DRST_DRS_SHIFT) // determine multiplier based on DRS
    {
    case 0:
      fll_freq_hz = (fll_ref * 732);
      if (fll_freq_hz < 20000000) {return 0x33;}
      else if (fll_freq_hz > 25000000) {return 0x34;}
      break;
    case 1:
      fll_freq_hz = (fll_ref * 1464);
      if (fll_freq_hz < 40000000) {return 0x35;}
      else if (fll_freq_hz > 50000000) {return 0x36;}
      break;
    case 2:
      fll_freq_hz = (fll_ref * 2197);
      if (fll_freq_hz < 60000000) {return 0x37;}
      else if (fll_freq_hz > 75000000) {return 0x38;}
      break;
    case 3:
      fll_freq_hz = (fll_ref * 2929);
      if (fll_freq_hz < 80000000) {return 0x39;}
      else if (fll_freq_hz > 100000000) {return 0x3A;}
      break;
    }
  }
  else // if DMX32 = 0
  {
    switch ((MCG_C4 & MCG_C4_DRST_DRS_MASK) >> MCG_C4_DRST_DRS_SHIFT) // determine multiplier based on DRS
    {
    case 0:
      fll_freq_hz = (fll_ref * 640);
      if (fll_freq_hz < 20000000) {return 0x33;}
      else if (fll_freq_hz > 25000000) {return 0x34;}
      break;
    case 1:
      fll_freq_hz = (fll_ref * 1280);
      if (fll_freq_hz < 40000000) {return 0x35;}
      else if (fll_freq_hz > 50000000) {return 0x36;}
      break;
    case 2:
      fll_freq_hz = (fll_ref * 1920);
      if (fll_freq_hz < 60000000) {return 0x37;}
      else if (fll_freq_hz > 75000000) {return 0x38;}
      break;
    case 3:
      fll_freq_hz = (fll_ref * 2560);
      if (fll_freq_hz < 80000000) {return 0x39;}
      else if (fll_freq_hz > 100000000) {return 0x3A;}
      break;
    }
  }
  return fll_freq_hz;
} // fll_freq

void rtc_reset( void )
{
    SIM_SCGC6 |= SIM_SCGC6_RTC_MASK;
    disable_irq(20);
    disable_irq(21);
    RTC_CR  = RTC_CR_SWR_MASK;
    RTC_CR  &= ~RTC_CR_SWR_MASK;
}

/********************************************************************/
/* Functon name : fei_fee
 *
 * Mode transition: FEI to FEE mode
 *
 * This function transitions the MCG from FEI mode to FEE mode. This is
 * achieved by setting the MCG_C2[LP] bit. There is no status bit to
 * check so 0 is always returned if the function was called with the MCG
 * in FBI mode. The MCGCLKOUT frequency does not change
 *
 * Parameters: crystal_val - external clock frequency in Hz
 *             hgo_val     - selects whether low power or high gain mode is selected
 *                           for the crystal oscillator. This has no meaning if an
 *                           external clock is used.
 *             erefs_val   - selects external clock (=0) or crystal osc (=1)
 *
 * Return value : MCGCLKOUT frequency (Hz) or error code
 */
int fei_fee(int crystal_val, unsigned char hgo_val, unsigned char erefs_val)
{
  unsigned char frdiv_val;
  unsigned char temp_reg;
 // short i;
  int mcg_out, fll_ref_freq, i;

  // check if in FEI mode
/*
	if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
		(MCG_S & MCG_S_IREFST_MASK))) 								// check PLLS mux has selected FLL

    {
        return 0x1;                                                     // return error code
    }
*/

// check external frequency is less than the maximum frequency
  if  (crystal_val > 50000000) {return 0x21;}

 //check crystal frequency is within spec. if crystal osc is being used
  if (erefs_val)
  {
  if (
       ((crystal_val > 40000) && (crystal_val < 3000000)) ||
      (crystal_val > 32000000)) {return 0x22;} // return error if one of the available crystal options is not available
  }

// make sure HGO will never be greater than 1. Could return an error instead if desired.
  if (hgo_val > 0)
  {
    hgo_val = 1; // force hgo_val to 1 if > 0
  }

  OSC0_CR = OSC_CR_ERCLKEN_MASK
	  |OSC_CR_EREFSTEN_MASK
	  |OSC_CR_SC16P_MASK;

// configure the MCG_C2 register
// the RANGE value is determined by the external frequency. Since the RANGE parameter affects the FRDIV divide value
// it still needs to be set correctly even if the oscillator is not being used
  temp_reg = MCG_C2;
  temp_reg &= ~(MCG_C2_RANGE0_MASK | MCG_C2_HGO0_MASK | MCG_C2_EREFS0_MASK); // clear fields before writing new values
  if (crystal_val <= 40000)
  {
    temp_reg |= (MCG_C2_RANGE0(0) | (hgo_val << MCG_C2_HGO0_SHIFT) | (erefs_val << MCG_C2_EREFS0_SHIFT));
  }
  else if (crystal_val <= 8000000)
  {
    temp_reg |= (MCG_C2_RANGE0(1) | (hgo_val << MCG_C2_HGO0_SHIFT) | (erefs_val << MCG_C2_EREFS0_SHIFT));
  }
  else
  {
    temp_reg |= (MCG_C2_RANGE0(2) | (hgo_val << MCG_C2_HGO0_SHIFT) | (erefs_val << MCG_C2_EREFS0_SHIFT));
  }
  MCG_C2 = temp_reg;
  // if the external oscillator is used need to wait for OSCINIT to set
  if (erefs_val)
  {
	for (i = 0 ; i < 20000000 ; i++)
	{
	  if (MCG_S & MCG_S_OSCINIT0_MASK) break; // jump out early if OSCINIT sets before loop finishes
	}
	if (!(MCG_S & MCG_S_OSCINIT0_MASK)) return 0x23; // check bit is really set and return with error if not set
  }

// determine FRDIV based on reference clock frequency
// since the external frequency has already been checked only the maximum frequency for each FRDIV value needs to be compared here.
  if (crystal_val <= 1250000) {frdiv_val = 0;}
  else if (crystal_val <= 2500000) {frdiv_val = 1;}
  else if (crystal_val <= 5000000) {frdiv_val = 2;}
  else if (crystal_val <= 10000000) {frdiv_val = 3;}
  else if (crystal_val <= 20000000) {frdiv_val = 4;}
  else {frdiv_val = 5;}

  // The FLL ref clk divide value depends on FRDIV and the RANGE value
  if (((MCG_C2 & MCG_C2_RANGE0_MASK) >> MCG_C2_RANGE0_SHIFT) > 0)
  {
    fll_ref_freq = ((crystal_val) / (32 << frdiv_val));
  }
  else
  {
    fll_ref_freq = ((crystal_val) / (1 << frdiv_val));
  }
   if( crystal_val == 32768 )
   {
      MCG_C4 |= MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(1);
   }

// Check resulting FLL frequency
  mcg_out = fll_freq(fll_ref_freq); // FLL reference frequency calculated from ext ref freq and FRDIV
  if (mcg_out < 0x5B) {return mcg_out;} // If error code returned, return the code to calling function

// Select external oscilator and Reference Divider and clear IREFS to start ext osc
// If IRCLK is required it must be enabled outside of this driver, existing state will be maintained
// CLKS=0, FRDIV=frdiv_val, IREFS=0, IRCLKEN=0, IREFSTEN=0
  temp_reg = MCG_C1;
  temp_reg &= ~(MCG_C1_CLKS_MASK | MCG_C1_FRDIV_MASK | MCG_C1_IREFS_MASK); // Clear values in these fields
  temp_reg |= (MCG_C1_CLKS(0) | MCG_C1_FRDIV(frdiv_val)); // Set the required CLKS and FRDIV values
  MCG_C1 = temp_reg;

// wait for Reference clock Status bit to clear
  for (i = 0 ; i < 20000 ; i++)
  {
    if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
  }
  if (MCG_S & MCG_S_IREFST_MASK) return 0x11; // check bit is really clear and return with error if not set

// Now in FBE
// It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
// It is enabled here but can be removed if this is not required.
  //MCG_C6 |= MCG_C6_CME0_MASK;

  return mcg_out; // MCGOUT frequency equals FLL frequency
} // fei_fee

/**
 **===========================================================================
 **  Reset handler
 **===========================================================================
 */
void __init_hardware()
{
	SCB_VTOR = (uint32_t)__vector_table; /* Set the interrupt vector table position */

	// Disable the Watchdog because it may reset the core before entering main().
	SIM_COPC = KINETIS_WDOG_DISABLED_CTRL;
    /* Setup Allowed Power modes */
    SMC_PMPROT = (SMC_PMPROT_AVLP_MASK
                  | SMC_PMPROT_ALLS_MASK
                  | SMC_PMPROT_AVLLS_MASK);

    /* Read RCM for identification of Chip Reset reason */
    if (RCM_SRS0 & RCM_SRS0_WAKEUP_MASK)
    {
      /* Releases hold with ACKISO:  Only has an effect if recovering from VLLS1, VLLS2, or VLLS3
         if ACKISO is set you must clear ackiso before calling pll_init
         or pll init hangs waiting for OSC to initialize
         if osc enabled in low power modes - enable it first before ack
         if I/O needs to be maintained without glitches enable outputs and modules first before ack. */
      if (PMC_REGSC &  PMC_REGSC_ACKISO_MASK)
      {
        PMC_REGSC |= PMC_REGSC_ACKISO_MASK;	/* Clear ACKISO bit */
        LLWU_ME &= ~LLWU_ME_WUME4_MASK; /* TSI LLWU Input Disable = LLWU_M4IF */
      }
    } else if (RCM_SRS0 & RCM_SRS0_LOC_MASK)
    {
      /* Loss of clock case */
    }

    /* Enable all of the port clocks. These have to be enabled to configure
     * pin muxing options, so most code will need all of these on anyway.
     */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
                  | SIM_SCGC5_PORTB_MASK );

    //clk_out_init();

#if defined(NO_PLL_INIT)

    mcg_clk_hz = 21000000; //FEI mode

#else
   /* Ramp up the system clock */
   /* Set the system dividers */
   /* NOTE: The PLL init will not configure the system clock dividers,
    * so they must be configured appropriately before calling the PLL
    * init function to ensure that clocks remain in valid ranges.
    */
    SIM_CLKDIV1 = ( 0
                    | SIM_CLKDIV1_OUTDIV1(0)
                    | SIM_CLKDIV1_OUTDIV4(1) );

   /* Reset RTC if it was started from the startup */
   rtc_reset();
   /* Initialize PLL */
   /* PLL will be the source for MCG CLKOUT so the core, system, and flash clocks are derived from it */
   mcg_clk_hz = fei_fee(CLK0_FREQ_HZ,HIGH_GAIN,CLK0_TYPE);

   /* Check the value returned from pll_init() to make sure there wasn't an error */
   if (mcg_clk_hz < 0x100)
     while(1);
#endif
}

/* Weak definitions of handlers point to Default_Handler if not implemented */
void NMI_Handler() __attribute__ ((weak, alias("Default_Handler")));
void HardFault_Handler() __attribute__ ((weak, alias("Default_Handler")));
void SVC_Handler() __attribute__ ((weak, alias("Default_Handler")));
void PendSV_Handler() __attribute__ ((weak, alias("Default_Handler")));
void SysTick_Handler() __attribute__ ((weak, alias("Default_Handler")));

void DMA0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void DMA1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void DMA2_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void DMA3_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void MCM_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void FTFL_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PMC_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void LLW_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void I2C0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void I2C1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void SPI0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void SPI1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void UART0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void UART1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void UART2_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void ADC0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void CMP0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void FTM0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void FTM1_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void FTM2_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void RTC_Alarm_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void RTC_Seconds_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PIT_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void USBOTG_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void DAC0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void TSI0_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void MCG_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void LPTimer_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PORTA_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));
void PORTD_IRQHandler() __attribute__ ((weak, alias("Default_Handler")));

/* The Interrupt Vector Table */
void (* const InterruptVector[])() __attribute__ ((section(".vectortable"))) = {
    /* Processor exceptions */
    (void(*)(void)) &_estack,
    __thumb_startup,
    NMI_Handler,
    HardFault_Handler,
    0,
    0,
    0,
    0,
    0,
    0,
    0,
    SVC_Handler,
    0,
    0,
    PendSV_Handler,
    SysTick_Handler,

    /* Interrupts */
    DMA0_IRQHandler, /* DMA Channel 0 Transfer Complete and Error */
    DMA1_IRQHandler, /* DMA Channel 1 Transfer Complete and Error */
    DMA2_IRQHandler, /* DMA Channel 2 Transfer Complete and Error */
    DMA3_IRQHandler, /* DMA Channel 3 Transfer Complete and Error */
    MCM_IRQHandler, /* Normal Interrupt */
    FTFL_IRQHandler, /* FTFL Interrupt */
    PMC_IRQHandler, /* PMC Interrupt */
    LLW_IRQHandler, /* Low Leakage Wake-up */
    I2C0_IRQHandler, /* I2C0 interrupt */
    I2C1_IRQHandler, /* I2C1 interrupt */
    SPI0_IRQHandler, /* SPI0 Interrupt */
    SPI1_IRQHandler, /* SPI1 Interrupt */
    FMSTR_Isr, /* UART0 Status and Error interrupt */
    UART1_IRQHandler, /* UART1 Status and Error interrupt */
    UART2_IRQHandler, /* UART2 Status and Error interrupt */
    ADC0_IRQHandler, /* ADC0 interrupt */
    CMP0_IRQHandler, /* CMP0 interrupt */
    FTM0_IRQHandler, /* FTM0 fault, overflow and channels interrupt */
    FTM1_IRQHandler, /* FTM1 fault, overflow and channels interrupt */
    FTM2_IRQHandler, /* FTM2 fault, overflow and channels interrupt */
    RTC_Alarm_IRQHandler, /* RTC Alarm interrupt */
    RTC_Seconds_IRQHandler, /* RTC Seconds interrupt */
    PIT_IRQHandler, /* PIT timer all channels interrupt */
    Default_Handler, /* Reserved interrupt 39/23 */
    USBOTG_IRQHandler, /* USB interrupt */
    DAC0_IRQHandler, /* DAC0 interrupt */
    TSS_TSI0Isr, /* TSI0 Interrupt */
    MCG_IRQHandler, /* MCG Interrupt */
    LPTMRIsr, /* LPTimer interrupt */
    Default_Handler, /* Reserved interrupt 45/29 */
    PORTA_IRQHandler, /* Port A interrupt */
    LPSWIsr /* Port D interrupt */
};

