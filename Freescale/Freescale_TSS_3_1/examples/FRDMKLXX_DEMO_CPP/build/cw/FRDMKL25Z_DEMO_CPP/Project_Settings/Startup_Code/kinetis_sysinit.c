/*
 *    kinetis_sysinit.c - Default init routines for Flycatcher
 *                          Kinetis ARM systems
 *    Copyright © 2012 Freescale semiConductor Inc. All Rights Reserved.
 */

#include "kinetis_sysinit.h"
#include "uart.h"
#include "derivative.h"
#include "isr.h"


/* Actual system clock frequency */
int mcg_clk_hz;

#ifdef __cplusplus
extern "C" {
#endif
extern unsigned long _estack;
extern void __thumb_startup( void );
#ifdef __cplusplus
}
#endif

/* ISR prototype */
typedef void (*const tIsrFunc)(void);
typedef struct {
  uint32_t * __ptr;
  tIsrFunc __fun[0x2F];
} tVectorTable;


__attribute__ ((section (".vectortable"))) const tVectorTable __vector_table = { /* Interrupt vector table */

  /* ISR name                             No. Address      Pri Name                          Description */
  &_estack,                       /* 0x00  0x00000000   -   ivINT_Initial_Stack_Pointer   used by PE */
  {
  (tIsrFunc)&__thumb_startup,     /* 0x01  0x00000004   -   ivINT_Initial_Program_Counter used by PE */
  (tIsrFunc)&Default_Handler,     /* 0x02  0x00000008   -2   ivINT_NMI                     used by PE */
  (tIsrFunc)&Default_Handler,     /* 0x03  0x0000000C   -1   ivINT_Hard_Fault              unused by PE */
  (tIsrFunc)&VECTOR_003,          /* 0x04  0x00000010   -   ivINT_Reserved4               unused by PE */
  (tIsrFunc)&VECTOR_004,          /* 0x05  0x00000014   -   ivINT_Reserved5               unused by PE */
  (tIsrFunc)&VECTOR_005,          /* 0x06  0x00000018   -   ivINT_Reserved6               unused by PE */
  (tIsrFunc)&VECTOR_006,          /* 0x07  0x0000001C   -   ivINT_Reserved7               unused by PE */
  (tIsrFunc)&VECTOR_007,          /* 0x08  0x00000020   -   ivINT_Reserved8               unused by PE */
  (tIsrFunc)&VECTOR_008,          /* 0x09  0x00000024   -   ivINT_Reserved9               unused by PE */
  (tIsrFunc)&VECTOR_009,          /* 0x0A  0x00000028   -   ivINT_Reserved10              unused by PE */
  (tIsrFunc)&VECTOR_010,          /* 0x0B  0x0000002C   -   ivINT_SVCall                  unused by PE */
  (tIsrFunc)&VECTOR_011,          /* 0x0C  0x00000030   -   ivINT_Reserved12              unused by PE */
  (tIsrFunc)&VECTOR_012,          /* 0x0D  0x00000034   -   ivINT_Reserved13              unused by PE */
  (tIsrFunc)&VECTOR_013,          /* 0x0E  0x00000038   -   ivINT_PendableSrvReq          unused by PE */
  (tIsrFunc)&VECTOR_014,          /* 0x0F  0x0000003C   -   ivINT_SysTick                 unused by PE */
  (tIsrFunc)&VECTOR_015,          /* 0x10  0x00000040   -   ivINT_DMA0                    unused by PE */
  (tIsrFunc)&VECTOR_016,          /* 0x11  0x00000044   -   ivINT_DMA1                    unused by PE */
  (tIsrFunc)&VECTOR_017,          /* 0x12  0x00000048   -   ivINT_DMA2                    unused by PE */
  (tIsrFunc)&VECTOR_018,          /* 0x13  0x0000004C   -   ivINT_DMA3                    unused by PE */
  (tIsrFunc)&VECTOR_019,          /* 0x14  0x00000050   -   ivINT_Reserved20              unused by PE */
  (tIsrFunc)&VECTOR_020,          /* 0x15  0x00000054   -   ivINT_FTFA                    unused by PE */
  (tIsrFunc)&VECTOR_021,          /* 0x16  0x00000058   -   ivINT_LVD_LVW                 unused by PE */
  (tIsrFunc)&VECTOR_023,          /* 0x17  0x0000005C   -   ivINT_LLW                     unused by PE */
  (tIsrFunc)&VECTOR_024,          /* 0x18  0x00000060   -   ivINT_I2C0                    unused by PE */
  (tIsrFunc)&VECTOR_025,          /* 0x19  0x00000064   -   ivINT_I2C1                    unused by PE */
  (tIsrFunc)&VECTOR_026,          /* 0x1A  0x00000068   -   ivINT_SPI0                    unused by PE */
  (tIsrFunc)&VECTOR_027,          /* 0x1B  0x0000006C   -   ivINT_SPI1                    unused by PE */
  (tIsrFunc)&VECTOR_028,          /* 0x1C  0x00000070   -   ivINT_UART0                   unused by PE */
  (tIsrFunc)&VECTOR_029,          /* 0x1D  0x00000074   -   ivINT_UART1                   unused by PE */
  (tIsrFunc)&VECTOR_030,          /* 0x1E  0x00000078   -   ivINT_UART2                   unused by PE */
  (tIsrFunc)&VECTOR_031,          /* 0x1F  0x0000007C   -   ivINT_ADC0                    unused by PE */
  (tIsrFunc)&VECTOR_032,          /* 0x20  0x00000080   -   ivINT_CMP0                    unused by PE */
  (tIsrFunc)&VECTOR_033,          /* 0x21  0x00000084   -   ivINT_TPM0                    unused by PE */
  (tIsrFunc)&VECTOR_034,          /* 0x22  0x00000088   -   ivINT_TPM1                    unused by PE */
  (tIsrFunc)&VECTOR_035,          /* 0x23  0x0000008C   -   ivINT_TPM2                    unused by PE */
  (tIsrFunc)&VECTOR_036,          /* 0x24  0x00000090   -   ivINT_RTC                     unused by PE */
  (tIsrFunc)&VECTOR_037,          /* 0x25  0x00000094   -   ivINT_RTC_Seconds             unused by PE */
  (tIsrFunc)&VECTOR_038,          /* 0x26  0x00000098   -   ivINT_PIT                     unused by PE */
  (tIsrFunc)&VECTOR_039,          /* 0x27  0x0000009C   -   ivINT_Reserved39              unused by PE */
  (tIsrFunc)&VECTOR_040,          /* 0x28  0x000000A0   -   ivINT_USB0                    unused by PE */
  (tIsrFunc)&VECTOR_041,          /* 0x29  0x000000A4   -   ivINT_DAC0                    unused by PE */
  (tIsrFunc)&VECTOR_042,          /* 0x2A  0x000000A8   -   ivINT_TSI0                    used by PE */
  (tIsrFunc)&VECTOR_043,          /* 0x2B  0x000000AC   -   ivINT_MCG                     unused by PE */
  (tIsrFunc)&VECTOR_044,          /* 0x2C  0x000000B0   -   ivINT_LPTimer                 unused by PE */
  (tIsrFunc)&VECTOR_045,          /* 0x2D  0x000000B4   -   ivINT_Reserved45              unused by PE */
  (tIsrFunc)&VECTOR_046,          /* 0x2E  0x000000B8   -   ivINT_PORTA                   unused by PE */
  (tIsrFunc)&VECTOR_047           /* 0x2F  0x000000BC   -   ivINT_PORTD                   unused by PE */
  }
};

/**
 **===========================================================================
 **  Default interrupt handler
 **===========================================================================
 */
void Default_Handler(void)
{
    __asm("bkpt");
}


/********************************************************************/

/**
 **===========================================================================
 **  Reset handler
 **===========================================================================
 */

void __attribute__ ((constructor)) __init_hardware()
{
    unsigned char frdiv_val;
    unsigned char temp_reg;
    unsigned char prdiv, vdiv;
    short i;
    int ref_freq;
    int pll_freq;
    int crystal_val=CLK0_FREQ_HZ;
    unsigned char hgo_val=LOW_POWER;
    unsigned char erefs_val=CLK0_TYPE;
    signed char prdiv_val=PLL0_PRDIV;
    signed char vdiv_val=PLL0_VDIV;
    int error_code;

    SCB_VTOR = (uint32_t)&__vector_table; /* Set the interrupt vector table position */

    /* Disable the WDOG module */
    /* SIM_COPC: ??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,COPT=0,COPCLKS=0,COPW=0 */
    SIM_COPC = (uint32_t)0x00UL;
    /* Enable all of the port clocks. These have to be enabled to configure
    * pin muxing options, so most code will need all of these on anyway.
    */
    SIM_SCGC5 |= (SIM_SCGC5_PORTA_MASK
          | SIM_SCGC5_PORTB_MASK
          | SIM_SCGC5_PORTC_MASK
          | SIM_SCGC5_PORTD_MASK
          | SIM_SCGC5_PORTE_MASK );

     /* clk_out_init(); */

#if defined(NO_PLL_INIT)

      mcg_clk_hz = 21000000; //FEI mode

      SIM_SOPT2 &= ~SIM_SOPT2_PLLFLLSEL_MASK; // clear PLLFLLSEL to select the FLL for this clock source

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



      // check if in FEI mode
      if (!((((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x0) && // check CLKS mux has selcted FLL output
          (MCG_S & MCG_S_IREFST_MASK) &&                                  // check FLL ref is internal ref clk
          (!(MCG_S & MCG_S_PLLST_MASK))))                                 // check PLLS mux has selected FLL
      {
          error_code = 0x1;                                                     // return error code
      }

      // check external frequency is less than the maximum frequency
      if  (crystal_val > 50000000) {error_code= 0x21;}

      // check crystal frequency is within spec. if crystal osc is being used as PLL ref
      if (erefs_val)
      {
        if ((crystal_val < 3000000) || (crystal_val > 32000000)) {error_code= 0x22;} // return 1 if one of the available crystal options is not available
      }

      // make sure HGO will never be greater than 1. Could return an error instead if desired.
      if (hgo_val > 0)
      {
        hgo_val = 1; // force hgo_val to 1 if > 0
      }

      // Check PLL divider settings are within spec.
      if ((prdiv_val < 1) || (prdiv_val > 25)) {error_code= 0x41;}
      if ((vdiv_val < 24) || (vdiv_val > 50)) {error_code= 0x42;}

      // Check PLL reference clock frequency is within spec.
      ref_freq = crystal_val / prdiv_val;
      if ((ref_freq < 2000000) || (ref_freq > 4000000)) {error_code= 0x43;}

      // Check PLL output frequency is within spec.
      pll_freq = (crystal_val / prdiv_val) * vdiv_val;
      if ((pll_freq < 48000000) || (pll_freq > 100000000)) {error_code= 0x45;}

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
      if (!(MCG_S & MCG_S_OSCINIT0_MASK)) error_code= 0x23; // check bit is really set and return with error if not set
      }

      // wait for Reference clock Status bit to clear
      for (i = 0 ; i < 2000 ; i++)
      {
        if (!(MCG_S & MCG_S_IREFST_MASK)) break; // jump out early if IREFST clears before loop finishes
      }
      if (MCG_S & MCG_S_IREFST_MASK) error_code= 0x11; // check bit is really clear and return with error if not set

      // Wait for clock status bits to show clock source is ext ref clk
      for (i = 0 ; i < 2000 ; i++)
      {
        if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) == 0x2) break; // jump out early if CLKST shows EXT CLK slected before loop finishes
      }
      if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x2) error_code= 0x1A; // check EXT CLK is really selected and return with error if not

      // Now in FBE
      // It is recommended that the clock monitor is enabled when using an external clock as the clock source/reference.
      // It is enabled here but can be removed if this is not required.
      MCG_C6 |= MCG_C6_CME0_MASK;

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
      if (!(MCG_S & MCG_S_PLLST_MASK)) error_code= 0x16; // check bit is really set and return with error if not set

      // Wait for LOCK bit to set
      for (i = 0 ; i < 4000 ; i++)
      {
        if (MCG_S & MCG_S_LOCK0_MASK) break; // jump out early if LOCK sets before loop finishes
      }
      if (!(MCG_S & MCG_S_LOCK0_MASK)) error_code= 0x44; // check bit is really set and return with error if not set

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
      if (((MCG_S & MCG_S_CLKST_MASK) >> MCG_S_CLKST_SHIFT) != 0x3) error_code= 0x1B; // check CLKST is set correctly and return with error if not

      // Now in PEE

      mcg_clk_hz = ((crystal_val / prdiv) * vdiv); //MCGOUT equals PLL output frequency

     /* Check the value returned from pll_init() to make sure there wasn't an error */
     if (mcg_clk_hz < 0x100)
       while(1);

     SIM_SOPT2 |= SIM_SOPT2_PLLFLLSEL_MASK; // set PLLFLLSEL to select the PLL for this clock source

#endif
     (void) error_code;
}

void isrINT_NMI(void)
{
  /* Write your interrupt code here ... */

}
/* end of isrINT_NMI */

/**
 **===========================================================================
 **  External declarations
 **===========================================================================
 */
#if __cplusplus
extern "C" {
#endif

#if __cplusplus
}
#endif


/* Interrupt vector table */
#ifndef UNASSIGNED_ISR
  #define UNASSIGNED_ISR Default_Handler   /* unassigned interrupt service routine */
#endif



