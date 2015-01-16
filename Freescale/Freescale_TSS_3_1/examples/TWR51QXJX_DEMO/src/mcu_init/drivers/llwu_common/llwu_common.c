/*!
 * \file    llwu_common.c
 * \brief   common LLWU routines
 *
 * This file defines the functions/interrupt handlers/macros used for LLWU to be used as wakeup source.
 * And some common initializations.
 *
 * \version $Revision: 1.0 $
 * \author  Philip Drake(rxaa60)
 ***/

#include "common.h"
#include "llwu_common.h"

extern void fb_clk_init(void);
extern void clkout_init(unsigned char clockout_select, unsigned char clkoutdiv);

extern int core_clk_khz;
extern int core_clk_mhz;

/* function: llwu_configure

   description: Set up the LLWU for wakeup the MCU from LLS and VLLSx modes 
   from the selected pin or module.
   
   inputs:
   pin_en - unsigned integer, bit position indicates the pin is enabled.  
            More than one bit can be set to enable more than one pin at a time.  
   
   rise_fall - 0x00 = External input disabled as wakeup
               0x01 - External input enabled as rising edge detection
               0x02 - External input enabled as falling edge detection
               0x03 - External input enablge as any edge detection
   module_en - unsigned char, bit position indicates the module is enabled.  
               More than one bit can be set to enabled more than one module                   
   
   for example:  if bit 0 and 1 need to be enabled as rising edge detect call this  routine with
   pin_en = 0x0003 and rise_fall = 0x02
   
   Note: to set up one set of pins for rising and another for falling, 2 calls to this 
         function are required, 1st for rising then the second for falling.
   
*/
void llwu_reset_enable(void){
    LLWU_RST = LLWU_RST_LLRSTE_MASK;   //no reset filter for now 
}
void llwu_configure(unsigned int pin_en, unsigned char rise_fall, unsigned char module_en ) {

    if( pin_en & 0x0001){
       LLWU_PE1 |= LLWU_PE1_WUPE0(rise_fall); 
       LLWU_F1 |= LLWU_F1_WUF0_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0002){
       LLWU_PE1 |= LLWU_PE1_WUPE1(rise_fall); 
       LLWU_F1 |= LLWU_F1_WUF1_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0004){
       LLWU_PE1 |= LLWU_PE1_WUPE2(rise_fall); 
       LLWU_F1 |= LLWU_F1_WUF2_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0008){
       LLWU_PE1 |= LLWU_PE1_WUPE3(rise_fall); 
       LLWU_F1 |= LLWU_F1_WUF3_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0010){
       LLWU_PE2 |= LLWU_PE2_WUPE4(rise_fall); 
       LLWU_F1 |= LLWU_F1_WUF4_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0020){
       LLWU_PE2 |= LLWU_PE2_WUPE5(rise_fall); 
       LLWU_F1 |= LLWU_F1_WUF5_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0040){
       LLWU_PE2 |= LLWU_PE2_WUPE6(rise_fall); 
       LLWU_F1 |= LLWU_F1_WUF6_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0080){
       LLWU_PE2 |= LLWU_PE2_WUPE7(rise_fall); 
       LLWU_F1 |= LLWU_F1_WUF7_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0100){
       LLWU_PE3 |= LLWU_PE3_WUPE8(rise_fall); 
       LLWU_F2 |= LLWU_F2_WUF8_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0200){
       LLWU_PE3 |= LLWU_PE3_WUPE9(rise_fall); 
       LLWU_F2 |= LLWU_F2_WUF9_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0400){
       LLWU_PE3 |= LLWU_PE3_WUPE10(rise_fall); 
       LLWU_F2 |= LLWU_F2_WUF10_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x0800){
       LLWU_PE3 |= LLWU_PE3_WUPE11(rise_fall); 
       LLWU_F2 |= LLWU_F2_WUF11_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x1000){
       LLWU_PE4 |= LLWU_PE4_WUPE12(rise_fall); 
       LLWU_F2 |= LLWU_F2_WUF12_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x2000){
       LLWU_PE4 |= LLWU_PE4_WUPE13(rise_fall); 
       LLWU_F2 |= LLWU_F2_WUF13_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x4000){
       LLWU_PE4 |= LLWU_PE4_WUPE14(rise_fall); 
       LLWU_F2 |= LLWU_F2_WUF14_MASK;   // write one to clear the flag
    }
    if( pin_en & 0x8000){
       LLWU_PE4 |= LLWU_PE4_WUPE15(rise_fall); 
       LLWU_F2 |= LLWU_F2_WUF15_MASK;   // write one to clear the flag
    }
    LLWU_ME = module_en;  //Set up modules to wakeup up
    
    //function ends
}

//Interrupt handler for LLWU 
interrupt void llwu_isr(void){
	//Disable Watchdog Timer
	SIM_COPC = SIM_COPC_COPT(0);

	//Release hold if not already done so.
	if (PMC_REGSC & PMC_REGSC_ACKISO_MASK)
			  PMC_REGSC |= PMC_REGSC_ACKISO_MASK;

   if (LLWU_F1 & LLWU_F1_WUF0_MASK) {
       LLWU_F1 |= LLWU_F1_WUF0_MASK;   // write one to clear the flag
   }
   if (LLWU_F1 & LLWU_F1_WUF1_MASK) {
       LLWU_F1 |= LLWU_F1_WUF2_MASK;   // write one to clear the flag
   }
   if (LLWU_F1 & LLWU_F1_WUF2_MASK) {
       LLWU_F1 |= LLWU_F1_WUF2_MASK;   // write one to clear the flag
   }
   if (LLWU_F1 & LLWU_F1_WUF3_MASK){
       LLWU_F1 |= LLWU_F1_WUF3_MASK;   // write one to clear the flag
    }
   if (LLWU_F1 & LLWU_F1_WUF4_MASK) {
       LLWU_F1 |= LLWU_F1_WUF4_MASK;   // write one to clear the flag
   }
   if (LLWU_F1 & LLWU_F1_WUF5_MASK) {
       LLWU_F1 |= LLWU_F1_WUF5_MASK;   // write one to clear the flag
   }
   if (LLWU_F1 & LLWU_F1_WUF6_MASK) {
       LLWU_F1 |= LLWU_F1_WUF6_MASK;   // write one to clear the flag
    }
   if (LLWU_F1 & LLWU_F1_WUF7_MASK) {
       LLWU_F1 |= LLWU_F1_WUF7_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF8_MASK) {
       LLWU_F2 |= LLWU_F2_WUF8_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF9_MASK) {
       LLWU_F2 |= LLWU_F2_WUF9_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF10_MASK) {
       LLWU_F2 |= LLWU_F2_WUF10_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF11_MASK) {
       LLWU_F2 |= LLWU_F2_WUF11_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF12_MASK) {
       LLWU_F2 |= LLWU_F2_WUF12_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF13_MASK) {
       LLWU_F2 |= LLWU_F2_WUF13_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF14_MASK) {
       LLWU_F2 |= LLWU_F2_WUF14_MASK;   // write one to clear the flag
   }
   if (LLWU_F2 & LLWU_F2_WUF15_MASK) {
       LLWU_F2 |= LLWU_F2_WUF15_MASK;   // write one to clear the flag
   }
   if (LLWU_F3 & LLWU_F3_MWUF0_MASK) {
       LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // write 1 to TCF to clear the LPT timer compare flag
       LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );
       LLWU_F3 |= LLWU_F3_MWUF0_MASK;   // write one to clear the flag
   }
   if (LLWU_F3 & LLWU_F3_MWUF1_MASK) {
	     LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   // write 1 to TCF to clear the LPT timer compare flag
	     LPTMR0_CSR = ( LPTMR_CSR_TEN_MASK | LPTMR_CSR_TIE_MASK | LPTMR_CSR_TCF_MASK  );
       LLWU_F3 |= LLWU_F3_MWUF1_MASK;   // write one to clear the flag
   }
   if (LLWU_F3 & LLWU_F3_MWUF2_MASK) {
       LLWU_F3 |= LLWU_F3_MWUF2_MASK;   // write one to clear the flag
   }
   if (LLWU_F3 & LLWU_F3_MWUF3_MASK) {
       LLWU_F3 |= LLWU_F3_MWUF3_MASK;   // write one to clear the flag
   }
}
