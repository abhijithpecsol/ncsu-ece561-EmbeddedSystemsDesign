/***************************************************************************************                                                                          ***************!    NGE_ACT
*
* Sample for Freescale TSS and TSSEVB Demo Board
*
* This program uses the TSS. Configure electrode one (E1) of the TSSEVB and detects 
* every time the electrode is touched.  
*
* Before to run this example, verify that all jumpers are in their default
* position. See the "System Overview" chapter in the TSSEVB user's manual.
*                                                                                       
* --------------------------------------------------------------------------------------
*                                                                                                         
* CodeWarrior V6.2 for HC(S)08                                                          
*                                                                                          
* --------------------------------------------------------------------------------------
*
* RTAC Americas
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*****************************************************************************************/

#include <hidef.h>      /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "TSS_API.h"    /* Include Touch Sense Software */
#include "I2CDrv.h"     /* Include EGT communication software */
#include "LCD.h"        /* Include LCD module */
#include "freemaster.h" /* include FreeMASTER GUI features */

/*********************** GUI - FreeMASTER TSA table ************************/

#if FMSTR_USE_TSA
    /* Example of FreeMASTER User Application Table */
    uint8_t u8Dummy;
    FMSTR_TSA_TABLE_BEGIN(UserAppTable)
      FMSTR_TSA_RW_VAR(u8Dummy, FMSTR_TSA_UINT8)
    FMSTR_TSA_TABLE_END()

    /* FreeMASTER Global Table List definition */
    FMSTR_TSA_TABLE_LIST_BEGIN()
      #if TSS_USE_FREEMASTER_GUI
        FMSTR_TSA_TABLE(TSS_Table)
      #endif
      FMSTR_TSA_TABLE(UserAppTable)
    FMSTR_TSA_TABLE_LIST_END()
#endif

/****************************************************************************************
* PeriphInit
* ---------------------------------------------------------------------------------------
* Initializes various registers and peripherals
*****************************************************************************************/
void MCU_Init(void) 
{
    SOPT1 = 0b00100011;                 
/*            ||||||||
              |||||||+-- PTC6/RESET pin functions as RESET
              ||||||+--- PTC5/BKGD/MS pin functions as BKGD/MS
              |||+++---- Unimplemented or Reserved
              ||+------- Stop mode enabled
              |+-------- Short timeout period selected
              +--------- COP watchdog timer disabled       
*/
 
     
/* Configures FEI mode, BUSCLK = 10 MHz */   
    
    ICSC1 = 0b00000110;
/*            ||||||||
              |||||||+-- Internal reference clock is disabled in stop
              ||||||+--- ICSIRCLK inactive
              |||||+---- Internal reference clock selected (The TSSEVB only supports internal reference clock)
              ||+++----- (Reference clock)/1 (Range = 0) 
              ++-------- Output of FLL is selected      
*/               
    ICSC2 = 0b00000000; 
/*            ||||||||
              |||||||+-- External reference clock source is disabled in stop
              ||||||+--- ICSERCLK inactive
              |||||+---- External Clock Source requested
              ||||+----- FLL is not disabled in bypass mode
              |||+------ Configure external oscillator for low-power operation
              ||+------- Low frequency range selected for the external oscillator 
              ++-------- Bus frequency divider (Bus Freq)/1    
*/ 

/* Configures the TRIM Clock Frequency values from the reserve flash memory locations */ 
     
 
    if((*(uint8_t *)0xFFAF)==0xFF)
    {
        ICSSC |= ICSSC_DMX32_MASK;     /* ISC FTRIM value */
    } 
    else 
    {
        ICSTRM = (*(uint8_t *)0xFFAF);            /* ISC TRIM value */
        ICSSC |= ((*(uint8_t *)0xFFAE)&0x01) | ICSSC_DMX32_MASK;     /* ISC FTRIM value */
    }
    
    while(ICSC1_CLKS!=ICSSC_CLKST);     /* Waits for the frequency to be configure within the MCU */

      
    SCGC1 = 0b11111111;
/*            ||||||||
              |||||||+-- Bus clock to the SCI1 module is enabled.
              ||||||+--- Bus clock to the SCI2 module is enabled.
              |||||+---- Bus clock to the IIC module is enabled.
              ||||+----- Bus clock to the MTIM module is enabled.
              |||+------ Bus clock to the ADC module is enabled.
              ||+------- Bus clock to the TPM1 module is enabled.
              |+-------- Bus clock to the TPM2 module is enabled.
              +--------- Bus clock to the RTC module is enabled.  
*/     
        
    SCGC2 = 0b11111111;
/*            ||||||||
              |||||||+-- Bus clock to the SPI module is enabled.
              ||||||+--- Bus clock to the LCD module is enabled.
              |||||+---- Unimplemented or Reserved
              ||||+----- Unimplemented or Reserved
              |||+------ Bus clock to the KBI module is enabled.
              ||+------- Bus clock to the IRQ module is enabled.
              |+-------- Bus clock to the Flash module is enabled.
              +--------- Bus clock to the DBG module is enabled.    
*/
    PINPS3 = PINPS3_SDA_MASK|PINPS3_SCL_MASK; 
/*                 |               |
                   |               +------- SCL sourced from PTI5.
                   +----------------------- SDA sourced from PTI4.
                            
*/ 

}

/****************************************************************************************************
* Fault interrupt (software interrupt)
* ---------------------------------------------------------------------------------------------------
* Re-enables the larger electrode in case is disabled (This electrode has a very large capacitance)
*****************************************************************************************************/

interrupt VectorNumber_Vswi void FAULT_ISR(void)
{
    if(tss_CSSys.Faults.ChargeTimeout || tss_CSSys.Faults.SmallCapacitor) /* If large or small capacitor fault  */
    {    
        (void)TSS_SetSystemConfig(System_Faults_Register,0x00);         /* Clear the fault flag               */
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0xFF);  /* re-enable all  */
    }
}

/***************************************************************************************************
* TSS_fOnInit
* --------------------------------------------------------------------------------------------------
* TSS OnInit Callback
****************************************************************************************************/

void TSS_fOnInit(void) 
{
    /* Include your TSS Init code here */  
}

/****************************************************************************************
* MAIN
* --------------------------------------------------------------------------------------
* Entry point
*****************************************************************************************/

void main(void) {
    
    uint8_t lcv;
    
    MCU_Init(); /* Initializes all the MCU peripherals */
    
    I2CInit();  /* Initializes the EGT module */
    
#if TSS_USE_FREEMASTER_GUI
    (void)FMSTR_Init(); /* Init FreeMASTER resources */
#endif 

    (void)TSS_Init(); /* Initializes the TSS */
    
/* Sets the Sensitivity value for each electrode */
    
    for(lcv=0;lcv<TSS_N_ELECTRODES;lcv++)
    {  
        (void)TSS_SetSystemConfig(System_Sensitivity_Register+lcv,0x20); 
    }

/* Configure the TSS to report the touch and release events */
    
    (void)TSS_KeypadConfig(cKey0.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK|TSS_KEYPAD_RELEASE_EVENT_EN_MASK));

/* Enables Callback function. Enables the control */
    
    (void)TSS_KeypadConfig(cKey0.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK|TSS_KEYPAD_CONTROL_EN_MASK));

    
/* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking values is 0x64 */
    
    (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK | TSS_SWI_EN_MASK));
    
    EnableInterrupts; /* enable interrupts */
    /* include your code here */
   
    vfnLCD_Init ();   /* Initializes the LCD module */
   
    vfnLCD_Clear_Display ();  /* Clears the LCD */
    
    _LCD_FREESCALE_ON;
    
    for(;;) {
        /* FreeMASTER */
#if TSS_USE_FREEMASTER_GUI
        FMSTR_Poll();
#endif
      
        while (TSS_Task() != TSS_STATUS_OK);    /* Executes the TSS main task */        
        EGTTask();    /* Executes the EGT main task */

        /*__RESET_WATCHDOG();*/ /* feeds the dog */
    } /* loop forever */
    /* please make sure that you never leave main */
}
