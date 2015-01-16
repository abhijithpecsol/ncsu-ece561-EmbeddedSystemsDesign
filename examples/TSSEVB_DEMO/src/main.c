/***************************************************************************************************
*
* This program uses the TSS. The project includes four demos to test the TSSEVB funtionality 
* along with the TSS Software. For more information regarding the demo software please refer
* to the "Demo Application Guide" document. This document is included in the Documentation
* folder.    
*
* Before to run this example, verify that all jumpers are in their default
* position. See the "System Overview" chapter in the TSSEVB user's manual.
*                                                                                       
* ---------------------------------------------------------------------------------------------------
*                                                                                                         
* @file   main.c
*
* @brief  Demo application for Freescale TSS and TSSEVB Demo Board 
*
* @authors : A20309
*            b24509
             B04997
*
* @version 1.0.5 Updated I2C Driver to support writing sensitivity values from EGT to all 64 electrodes
*          1.0.4 Remove LCDRVC_INIT initialization register 
*          1.0.3 Initial Release
* 
* @date January - 19 - 2010                                                       
*      
* @WARNING : This Software version can only work with Electrode Graphing Tool version 0.9 or higher. 
*                                                                                    
* ---------------------------------------------------------------------------------------------------
*
* RTAC Americas
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*****************************************************************************************************/

#include <hidef.h>      /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "TSS_API.h"    /* Include Touch Sense Software */
#include "main.h"

#include "freemaster.h" /* include FreeMASTER GUI features */

/****************************************************************************************************
* Variables
*****************************************************************************************************/

#pragma DATA_SEG MY_ZEROPAGE

uint8_t near gu8State;                    /* Variable use to store the current state */
uint8_t near gu8Status;                   /* Variable that reports when is a pending event of the decoders */
extern DEMOFLAGS1 near gu8DemoWash;     /* Flags structure for the Washing Machine demo             */
struct
{                                       /* Flags structure for the SCI communication module         */
    uint8_t rx_flag      : 1;  	          /* there are characters in the rx buffer                    */
    uint8_t tx_buf_empty : 1;  	          /* the tx buffer is empty                                   */
} __near flags;

#pragma DATA_SEG DEFAULT
                                        
uint8_t gu8touchcount = 0;                /* Count for the activation of the Demo Set state           */
const uint8_t Message1[]  = {"0123456789*#"};   /* Characters array to dysplay on the LCD             */

 /* Buzzer */
uint8_t gu8SDuration;                     /* This variable counts until the required duration of the buzzer is reached */
uint8_t gu8SetDuration;                   /* Buzzer duration value                                    */

/* RTC */
uint8_t gu8RTCsecond;                     /* Counts the seconds of the Clock                          */
struct time RTC;                        /* Clock information structure                              */
extern struct time ALARM;               /* Alarm information structure                              */

/* MCVE */
uint8_t gu8MCVEsecond;                    /* Counts the seconds of the Decrementing clock             */
uint8_t gu8MCVEcountActive;               /* Flag that indicates  when the MCVE application is counting */
uint8_t gu8MCVEDelayEnable;               /* MCVE Delay enable/disable flag                           */
uint8_t gu8MCVEDelayCount;                /* Count for displaying the clock on the MCVE application   */
extern uint8_t gu8MCVEclock;              /* MCVE clock enable/disable flag                           */

/* SCI */
uint8_t au8txbuffer[TX_BUF_SIZE];         /* SCI Transmit buffer                                      */
uint8_t au8rxbuffer[RX_BUF_SIZE];         /* SCI Recive buffer                                        */
uint8_t gu8tx_buf_read_index;             /* SCI Transmit read index                                  */
uint8_t gu8tx_buf_write_index;            /* SCI Transmit write index                                 */
uint8_t gu8rx_buf_read_index;             /* SCI Recive read index                                    */
uint8_t gu8rx_buf_write_index;            /* SCI Transmit write index                                 */

/* LCD */

byte bLCD_CharPosition = 0;             /* Character position within the LCD                        */
byte RamBuffer[RamBufferSize];          /* Buffer to store the LCD segments information             */

byte bpaPlace [ ] =
{
	Char1a,     /* LCD8  --- Pin:9  */ 
	Char1b,     /* LCD9  --- Pin:10 */
	Char2a,     /* LCD10 --- Pin:11 */
	Char2b,     /* LCD11 --- Pin:12 */
	Char3a,     /* LCD12 --- Pin:13 */
	Char3b,     /* LCD13 --- Pin:14 */
	Char4a,     /* LCD14 --- Pin:15 */
	Char4b      /* LCD15 --- Pin:16 */
};

byte baBackup [ ] =
{
   _Backupa,
   _Backupb,
};

const byte baAscii [ ] =               /* Array containing all the characters that can be displayed on LCD */
{
	_0
	_1
	_2
	_3
	_4
	_5
	_6
	_7
	_8
	_9
	_AllOff
	_SemiColon
	_MinusThan
	_Equal
	_GreaterThan
	_Question
	_AllOn
	_A
	_B
	_C
	_D
	_E
	_F
	_G
	_H
	_I
	_J
	_K
	_L
	_M
	_N
	_O
	_P
	_Q
	_R
	_S
	_T
	_U
	_V
	_W
	_X
	_Y
	_Z        

} ;

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

/****************************************************************************************************
* Functions
*****************************************************************************************************/

/* State machine functions */

void vfnDemoSetConfigure(void);
void vfnDemoSet(void);
void vfnShadowDemoConfigure(void);
void vfnShadowDemo(void);
void vfnMicrowaveConfigure(void);
void vfnMicrowave(void);
void vfnAlarmClockDemoConfigure(void);
void vfnAlarmClockDemo(void);
void vfnWashingMachineConfigure(void);
void vfnWashingMachine(void);
void vfnUpdateClock(void);

/* State Machine */

void (*const DemoStateMachine[])(void) = 
{
    vfnDemoSetConfigure,
    vfnDemoSet,
    vfnShadowDemoConfigure,
    vfnShadowDemo,
    vfnAlarmClockDemoConfigure,
    vfnAlarmClockDemo,
    vfnMicrowaveConfigure,
    vfnMicrowave,
    vfnWashingMachineConfigure,
    vfnWashingMachine,  
};


/****************************************************************************************************
* vfnTestSpecialState
* ---------------------------------------------------------------------------------------------------
* Task to monitor the buttons use to navigate between demos 
*****************************************************************************************************/
void vfnTestSpecialState(void)
{
    /* If the buttons to navigate between demos are active */  
    if((tss_au8ElectrodeStatus[4] == 0x04) && (tss_au8ElectrodeStatus[3] == 0x40) &&\
    (tss_au8ElectrodeStatus[2] == 0x00) && (tss_au8ElectrodeStatus[1] == 0x00) && (tss_au8ElectrodeStatus[0] == 0x00))
    {
        if(gu8touchcount >=3 )  /* If the buttons have been active for 3 time base executions */
        {
          gu8touchcount = 0;    /* Set touch counts to zero */
          gu8State = 0;         /* Go to DemoSet STATE (Demo Selection) */
        }
    }
    else                        /* If buttons are not active */
    {
        gu8touchcount = 0;      /* Set touch counts to zero */
    }
}

/***************************************************************************************************
* MCU_Init
* --------------------------------------------------------------------------------------------------
* Initializes various registers and peripherals
****************************************************************************************************/

void MCU_Init(void) 
{
           
/* Enables stop mode, reset and BKGD. Disables watchdog */

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

    /* BUS CLK set to 10 MHz */
    if((*(uint8_t *)0xFFAF) == 0xFF)      /* If the trimming value has NOT been calculated by the BDM */
    {
        ICSSC |= ICSSC_DMX32_MASK;      /* DCO is fined tuned for maximum frequency with 32.768 kHz reference */                            
    } 
    else                                /* If the trimming value HAS been calculated by the BDM */
    {                                   
        ICSTRM = (*(uint8_t *)0xFFAF);    /* Trim value is loaded from the non volatile TRIM registers */                            
        /* FTRIM loaded from the non volatile and DCO is fined tuned for maximum frequency with 32.768 kHz reference */    
        ICSSC |= ((*(uint8_t *)0xFFAE)&0x01) | ICSSC_DMX32_MASK;  
    }    

    while(ICSC1_CLKS!=ICSSC_CLKST);     /* Waits until the Frequency is locked */
      
    SCGC1 = 0xFF;                       /* Enables the bus clocks for the MCU periperals */
    
    SCGC2 = 0xFF;                       /* Enables the bus clocks for the MCU periperals */

    PINPS3 = PINPS3_SDA_MASK|PINPS3_SCL_MASK; /* Selects PTI4 & PTI5 to be associated with IIC operation*/
    
    LED_INIT(LED1);                     /* Initializes the corresponding pin as output to control D1 */
    LED_INIT(LED2);                     /* Initializes the corresponding pin as output to control D2 */
    LED_INIT(LED3);                     /* Initializes the corresponding pin as output to control D3 */
    LED_INIT(LED4);                     /* Initializes the corresponding pin as output to control D4 */
    
    //RTCSC = RTCSC_RTIE_MASK | RTCSC_RTCPS_MASK;  /* Configures the Base time for the clock (1 second = 1 second) */
    //DEMO CODE TO BE REMOVED
    RTCSC = RTCSC_RTIE_MASK | RTCSC_RTCPS2_MASK; /* Configures a faster time base for the clock (1 minute = 1 second) */
    //DEMO CODE TO BE REMOVED
}
                                                                                                    
/****************************************************************************************************
* Fault interrupt (software interrupt)
* ---------------------------------------------------------------------------------------------------
* Re-enables the larger electrode in case is disabled (This electrode has a very large capacitance)
*****************************************************************************************************/

interrupt VectorNumber_Vswi void FAULT_ISR(void)
{
    uint8_t lu8ElectrodeEnable;
    
    if(tss_CSSys.Faults.ChargeTimeout || tss_CSSys.Faults.SmallCapacitor) /* If large or small capacitor fault  */
    {    
        asm BGND;
        (void)TSS_SetSystemConfig(System_Faults_Register,0x00);         /* Clear the fault flag               */
        lu8ElectrodeEnable = tss_au8ElectrodeEnablers[3];               /* Read the current active electrodes */
        lu8ElectrodeEnable = (lu8ElectrodeEnable | 0x40);               /* Set E31                            */    
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,lu8ElectrodeEnable);  /* re-enable E31  */
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

/***************************************************************************************************
* MAIN
* --------------------------------------------------------------------------------------------------
* Entry point
****************************************************************************************************/

void main(void) 
{ 
    
    MCU_Init(); /* Initializes all the MCU peripherals */
    
    I2CInit();  /* Initializes the EGT module */
    
    SCIInit();  /* Initializes the SCI communication module */
    
    BUZZInit(); /* Initializes the Buzzer module */
    
#if TSS_USE_FREEMASTER_GUI
    (void)FMSTR_Init(); /* Init FreeMASTER resources */
#endif 
    (void)TSS_Init(); /* Initializes the TSS */

    /* Sets the Sensitivity value for each electrode */

    vfnSensitivityConfig();
         
    /* Sets the Number of samples for every electrode */
    (void)TSS_SetSystemConfig(System_NSamples_Register,0x0C);
   
    /* Slider Electrode Group */
    /* Configure the TSS to report the touch, release and movement events */
    (void)TSS_SliderConfig(cSlider0.ControlId,Slider_Events_Register,TSS_SLIDER_MOVEMENT_EVENT_EN_MASK|TSS_SLIDER_INITIAL_TOUCH_EVENT_EN_MASK);
    /* Enables Callback function. Enables the control */
    (void)TSS_SliderConfig(cSlider0.ControlId,Slider_ControlConfig_Register,(TSS_SLIDER_CONTROL_EN_MASK|TSS_SLIDER_CALLBACK_EN_MASK));
    
        /* Rotary Electrode Group */
    /* Configure the TSS to report the touch, release and movement events */
    (void)TSS_RotaryConfig(cRotary1.ControlId,Rotary_Events_Register,TSS_ROTARY_MOVEMENT_EVENT_EN_MASK|TSS_ROTARY_INITIAL_TOUCH_EVENT_EN_MASK|TSS_ROTARY_RELEASE_EVENT_EN_MASK);
    /* Enables Callback function. Enables the control */
    (void)TSS_RotaryConfig(cRotary1.ControlId,Rotary_ControlConfig_Register,(TSS_ROTARY_CONTROL_EN_MASK|TSS_ROTARY_CALLBACK_EN_MASK));
    /* Configure the TSS to report the touch and release events */
    (void)TSS_KeypadConfig(cKey6.ControlId,Keypad_Events_Register,TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK);
    /* Enables Callback function. Enables the control */
    (void)TSS_KeypadConfig(cKey6.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK|TSS_KEYPAD_CONTROL_EN_MASK));

        /* LED Electrode Group */
    /* Configure the TSS to report the touch and release events */
    (void)TSS_KeypadConfig(cKey2.ControlId,Keypad_Events_Register,TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK);
    /* Enables Callback function. Enables the control */
    (void)TSS_KeypadConfig(cKey2.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK|TSS_KEYPAD_CONTROL_EN_MASK));

        /* Keyboard Electrode Group */
    /* Configure the TSS to report the touch and release events */
    (void)TSS_KeypadConfig(cKey3.ControlId,Keypad_Events_Register,TSS_KEYPAD_TOUCH_EVENT_EN_MASK|TSS_KEYPAD_RELEASE_EVENT_EN_MASK);
    /* Enables Callback function. Enables the control */
    (void)TSS_KeypadConfig(cKey3.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK|TSS_KEYPAD_CONTROL_EN_MASK));

        /* MUX Electrode Group */
    /* Configure the TSS to report the touch and release events */
    (void)TSS_KeypadConfig(cKey4.ControlId,Keypad_Events_Register,TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK);
    /* Enables Callback function. Enables the control */
    (void)TSS_KeypadConfig(cKey4.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK|TSS_KEYPAD_CONTROL_EN_MASK));

       /* Sizes Electrode Group */
    /* Configure the TSS to report the touch and release events */
    (void)TSS_KeypadConfig(cKey5.ControlId,Keypad_Events_Register,TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK);
    /* Enables Callback function. Enables the control */
    (void)TSS_KeypadConfig(cKey5.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK|TSS_KEYPAD_CONTROL_EN_MASK));
    
    /* Enables the TSS. Enables DC Tracker. Enables software interrupt */
    (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK|TSS_SWI_EN_MASK|TSS_DC_TRACKER_EN_MASK));
    
    /* Configures the Response Time */
    (void)TSS_SetSystemConfig(System_ResponseTime_Register,4);
    
    /* Configures the DC Traker value */
    (void)TSS_SetSystemConfig(System_DCTrackerRate_Register,40);
    
    EnableInterrupts; /* enable interrupts */
    /* include your code here */
      
    /* Initializes the clock structure */
    RTC.HourMSB = 1;
    RTC.HourLSB = 1;
    RTC.MinuteMSB = 5;
    RTC.MinuteLSB = 8;
    
    /* Initializes the Alarm structure */
    ALARM.HourMSB = 1;
    ALARM.HourLSB = 2;
    
    vfnLCD_Init ();                             /* Initializes the LCD Driver                                       */
    vfnLCD_Clear_Display ();                    /* Clears the Display                                               */
    fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);   /* Sends the turn of all LEDs command to the communications MCU     */
    fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);   /* Sends the turn of all LEDs command to the communications MCU     */
    fnWriteDataTxBuffer((uint8_t)SHADOW_DEMO);    /* Sends the turn on the shadow demo LED to the communications MCU  */
    gu8State = SHADOW_DEMO*2;                   /* Sets the Shadow Demo STATE                                       */
    
    for(;;) 
    {
        /* FreeMASTER */
#if TSS_USE_FREEMASTER_GUI
        FMSTR_Poll();
#endif
    
        while (TSS_Task() != TSS_STATUS_OK);    /* Executes the TSS main task */
        DemoStateMachine[gu8State]();           /* Executes the State Machine Task */
        EGTTask();                              /* Executes the EGT main task */
        vfnTestSpecialState();                  /* Scans if the buttons to navigate between demos are touched */
        /*__RESET_WATCHDOG();*/ /* feeds the dog */
    } /* loop forever */
    /* please make sure that you never leave main */
}


/***************************************************************************************************
* RTC interrupt servicing function
* --------------------------------------------------------------------------------------------------
* Time base interrupt. Handles all the time based operation. 
****************************************************************************************************/

void interrupt VectorNumber_Vrtc RTC_ISR(void)
{
   
    gu8touchcount++;                         /* Increments the touch count                                      */
    gu8RTCsecond++;                          /* Increments the Seconds variable of the Clock information        */
    gu8RTCsecond+=5;
    
    if (gu8MCVEcountActive)                  /* If the Microwave/WashingMachine(MCVE/WASH) back count is active */
    {   
        gu8MCVEsecond+=6;                    /* Increments the Second variable of the MCVE/WASH back count      */
        vfnMCVEdecrementTime();              /* Decrements the time structure of the MCVE/WASH back count       */
        gu8MCVEDelayCount = 0;               /* Sets the MCVE Delay count to zero                               */
    } 
    else if (gu8MCVEDelayEnable)             /* If the MCVE delay to show the clock is enabled                  */
    {
        gu8MCVEDelayCount++;                 /* Increment the MCVE Delay count                                  */
        if (gu8MCVEDelayCount == MCVE_DELAY) /* If the MCVE Delay count has reach the stablished value          */
        {    
            gu8MCVEclock = 1;                /* Set the Show Clock flag                                         */ 
            gu8MCVEDelayCount = 0;           /* Reset the MCVE Delay count                                      */
        }
    }
    else                                     /* Else do nothing                                                 */
    {
    
    }
    
    RTCSC_RTIF = 1;                          /* Clear the RTC interrupt flag                                    */
    vfnUpdateClock();                        /* Update the Clock Structure                                      */
    TSS_SET_SAMPLE_INTERRUPTED();            /* Informs the TSS that an interrupt has occurred                  */
}
     
/***************************************************************************************************
* BUZZInit
* --------------------------------------------------------------------------------------------------
* Initializes the buzzer peripherals
****************************************************************************************************/

void BUZZInit(void)
{
    /* Initializes buzzer */
        
    TPM2SC = 0x47;           
/*           0b01000111 
               |||||+++- Clock source divided by 128
               |||++---- No clock TPM disable
               ||+------ Edge aligned PWM
               |+------- Interrupt requested when TOF = 1
               +-------- TOF
*/
    
    /* Sets the buzzer frequency */
    
    TPM2MOD = (uint16_t)PWM_1KHZ;     /* Configures the TPM module for a frequicency of 1KHz */   
    
    TPM2C1SC = 0x24;    
/*             0b00100100 
                 ||||||||
                 |||||||+-- Reserved
                 ||||||+--- Reserved
                 ||||++---- Low-true pulses (setoutput on compare) 
                 ||++------ Edge-aligned PWM  
                 |+-------- Channel interrupt requests disabled
                 +--------- Channel flag.               
*/
            
    TPM2C1V = (uint16_t)DUTY_CYCLE;   /* Sets the duty cycle to 50% */
}

/***************************************************************************************************
* vfnBUZZ
* --------------------------------------------------------------------------------------------------
* Activates the buzzer
****************************************************************************************************/

void vfnBUZZ(void)
{ 
    if(gu8DemoWash.fBuzzLong)               /* If the long sound flag is set                        */
    {
        gu8SetDuration = 0xF0;              /* Set duration = 240ms                                 */
        gu8DemoWash.fBuzzLong = 0;          /* Clears the long sound flag                           */
    }
    else                                    /* If long sound flag is NOT set                        */
    {
        gu8SetDuration = 0x32;              /* Set duration = 50ms                                  */
    }
    gu8SDuration = 0;                       /* Clears the duration count                            */
    TPM2CNT = 0x0000;                       /* Resets TPM2                                          */    
    TPM2SC |= 0x08;                         /* Activates TPM2                                       */         
} 

/***************************************************************************************************
*  TPM overflow interrupt servicing function
* --------------------------------------------------------------------------------------------------
*  Deactivates the buzzer depending on the duration stablished
****************************************************************************************************/

void interrupt VectorNumber_Vtpm2ovf tpm2_ovflw_isr(void)
{
    TPM2SC_TOF = 0;                          /* Clears the overflow flag                            */
    gu8SDuration++;                          /* Increments the duration count                       */
    if (gu8SDuration == gu8SetDuration)      /* If the count variable has reach the duration        */
    {
        TPM2SC &= 0xF7;                      /* Stops the TPM */
    }
    TSS_SET_SAMPLE_INTERRUPTED();            /* Informs the TSS that an interrupt has occurred      */
}


/***************************************************************************************************
*  SCI receive interrupt servicing function
* --------------------------------------------------------------------------------------------------
*  The received characters are stored into the receive buffer
****************************************************************************************************/

void interrupt VectorNumber_Vsci2rx IsrSci2Rx(void)
{
    TSS_SET_SAMPLE_INTERRUPTED();            /* Informs the TSS that an interrupt has occurred      */
        
    if (SCI2S1 & ERROR)   		                      /* if error is detected                         */
    {                     
        (void)SCI2D;  		                          /* dummy read to clear the interrupt flag       */
        return;                                     /* return                                       */
    } 
    else                                            /* if no error is detected                      */
    {        
        
        au8rxbuffer[gu8rx_buf_write_index] = SCI2D; /* read and store the received character        */
        gu8rx_buf_write_index++;                    /* increment the Rx write index                 */
         
        if (gu8rx_buf_write_index >= RX_BUF_SIZE)   /* if the index is bigger than the buff size    */
        {
            gu8rx_buf_write_index = 0;              /* Reset buffer write index                     */
        }
        
        flags.rx_flag = 1;              /* set the rx_flag. There is data into the Rx buffer        */
    }
}

/****************************************************************************************************
*  SCI transmit interrupt servicing function
*  --------------------------------------------------------------------------------------------------
*  Get characters from the transmit buffer and transmit them through the SCI
*  If there are no more characters to be transmitted, disable the TX interrupt.
****************************************************************************************************/  

void interrupt VectorNumber_Vsci2tx IsrSci2Tx(void)
{
    TSS_SET_SAMPLE_INTERRUPTED();            /* Informs the TSS that an interrupt has occurred      */
    
    /* if read and write pointers are equal, there is no more data to transmit */
    if (gu8tx_buf_read_index == gu8tx_buf_write_index)
    {
        SCI2C2_TIE = 0;		                              /* disable the TX interrupt                 */
        flags.tx_buf_empty = 1;	                        /* tx buffer is empty                       */
    } 
    else                                                /* if there is any data in the tx buffer    */ 
    {  
        if (SCI2S1_TDRE)                                /* if new data can be stored in SCI2D       */
        {
            SCI2D = au8txbuffer[gu8tx_buf_read_index];  /* transmit the next data                   */
            gu8tx_buf_read_index++;                     /* increment the tx buffer read index       */ 
            
            /* if the tx read buffer is bigger than the buffer size */ 
            if (gu8tx_buf_read_index >= TX_BUF_SIZE)
            {
                gu8tx_buf_read_index = 0;               /* set the pointer to zero                  */
            }
        }
    }
}

/****************************************************************************************************
*  Function for writing a char into the sci Tx buffer 
*  --------------------------------------------------------------------------------------------------
*  fnWriteDataTxBuffer
****************************************************************************************************/

void fnWriteDataTxBuffer(uint8_t u8Data)
{
    flags.tx_buf_empty = 0;   	                    /* the tx buffer is not empty                   */    
    au8txbuffer[gu8tx_buf_write_index] = u8Data;    /* write the data into the tx buffer            */
    gu8tx_buf_write_index++;                        /* increment the tx write buffer                */ 
    
    if (gu8tx_buf_write_index >= TX_BUF_SIZE)       /* if the tx write buffer index is bigger than the buffer size */ 
    {        
       gu8tx_buf_write_index = 0;                   /* Reset the tx write buffer                    */
    }
    
    SCI2C2_TIE = 1;		                              /* enable TX interrupt                          */
}

/****************************************************************************************************
* Function for reading a character from the Rx buffer 
* ---------------------------------------------------------------------------------------------------
* fnReadDataRxBuffer
****************************************************************************************************/ 

uint8_t fnReadDataRxBuffer(void)                    
{
    uint8_t temp;
    temp = au8rxbuffer[gu8rx_buf_read_index];           /* read one element of the Rx buffer       */
    gu8rx_buf_read_index++;                             /* increments the Rx Buffer index          */                   
    
    if (gu8rx_buf_read_index >= RX_BUF_SIZE)            /* if the read index is bigger than the buffer size */
    {   
        gu8rx_buf_read_index = 0;                       /* Set the index to zero                   */
    }    
    SCI2C2_RIE = 0;                                     /* disable the receive interrupt           */ 
    
    /* if read and write pointers are equal, there is no more data to read */
    if (gu8rx_buf_read_index == gu8rx_buf_write_index)  
    {   
        flags.rx_flag = 0;                              /* clear the rx_flag                       */ 
    }
    
    SCI2C2_RIE = 1;                                     /* re-enable the receive interrupt         */ 
    return (temp);    	                                /* return the character                    */
}

/****************************************************************************************************
* SCIInit
* ---------------------------------------------------------------------------------------------------
* Function to initialize the SCI module
****************************************************************************************************/  

void SCIInit(void)
{ 
    SCI2BD = BR_TSS;                                  /* SCI baud rate = 10MHz/(16*65) = 9615 bps */            
    
    /* Enable TX and RX section, RX interrupt */
    SCI2C2 = SCI2C2_RIE_MASK | SCI2C2_TE_MASK | SCI2C2_RE_MASK;  
}


/****************************************************************************************************
* vfnLCD_Init
* ---------------------------------------------------------------------------------------------------
* Initialize all the registers on the mcu module
****************************************************************************************************/ 

void vfnLCD_Init  (void)
{
  	SCGC2_LCD = 1;						        /* Enable clock to LCD module */
  	LCDDISABLE;
  	LCDSUPPLY_INIT;						        /* Configure  the Voltage Supply */
  	LCDC1_INIT;							          /* Configure  the LCD1 register */
  	LCDC0_INIT;							          /* Configure  the LCD0 register */  			     
  	LCDBCTL_INIT; 						        /* Configure  the Blink Control register */
  	vfnEnablePins ();					        /* Give to the LCDx the functionality  of a LCD pin */
  	vfnLCD_ConfigureBackplanes (); 		/* Enable the characterisitcs of a Backplane */
  	LCDENABLE; 							          /* Enable the LCD module */
}	

/****************************************************************************************************
* vfnEnablePins
*  --------------------------------------------------------------------------------------------------
* Enable all the pins of the mcu directly connected to the  LCD on use
****************************************************************************************************/ 
  						         	
void vfnEnablePins (void)
{
  	PEN0;							      	/* Give to the LCDx the functionality  of a LCD pin */
  	PEN1;
  	PEN2;
  	PEN3;
  	PEN4;
  	PEN5;
}
  
 /****************************************************************************************************
* vfnLCD_ConfigureBackplanes
*  --------------------------------------------------------------------------------------------------
* Enable the function of Backplane and Set the COM number that corresponds 
****************************************************************************************************/ 

void vfnLCD_ConfigureBackplanes (void)
{

  	LCDBPEN0 = 0;
  	LCDBPEN1 = 0;
  	LCDBPEN2 = 0;
  	LCDBPEN3 = 0;
  	LCDBPEN4 = 0;
  	LCDBPEN5 = 0;

  	  
  	BPEN0; 							      /* Enable the characterisitcs of a Backplane */
  	BPEN1;
  	BPEN2;
  	BPEN3;
  	BPEN4;
  	BPEN5;

  	SetCom1;	 				        /* Give to each backplane previously enbale his COM number */
  	SetCom2;
  	SetCom3;
  	SetCom4;
  	SetCom5;	 								
  	SetCom6;
  	SetCom7;
  	SetCom8;
}
  
 /****************************************************************************************************
* vfnLCD_Write_Msg
*  --------------------------------------------------------------------------------------------------
* Writes one message on the LCD   if the message is longer than the number of character only write
* the maximum number of characters
****************************************************************************************************/ 
 
void  vfnLCD_Write_Msg (byte  _POINTER lbpMessage)
{
    byte lbCounter;          
    
    vfnLCD_Home();						    /* Point to the first alphanumeric */
    lbCounter = _CHARNUM;
    
    
    while (lbCounter--)						
    {
      if (*lbpMessage)					  /* Write  String */
      {
        vfnLCD_Write_Char (*lbpMessage);
        lbpMessage ++;
      }
      else								        /* Write Blanks */
      {
       	vfnLCD_Write_Char(' ');
      }
    }
     
}

void  vfnLCD_Write_MsgPlace(byte _POINTER lbpMessage, byte n_chars, byte lbPlace) 
{
    byte lb_Counter = 0;          
   
    vfnLCD_GoTo(lbPlace);				  /* Point to the alphanumeric */
  
    while (n_chars--)					    /* Write String */
    {
      if (bLCD_CharPosition <= _CHARNUM*_LCDTYPE) 
      {
	       vfnLCD_Write_Char (*lbpMessage);
	       lbpMessage ++;     
      }
    }
} 

/****************************************************************************************************
* vfnLCD_Write_Char
*  --------------------------------------------------------------------------------------------------
* Writes one char after the last character was write
****************************************************************************************************/ 
void vfnLCD_Write_Char (byte lbValue)
{
    byte *lbpLCDWF = &LCDWFRef;
    byte lbOffset=0;
    byte lbCounter ;
    byte *lbpLCDWFCopy = &RamBuffer[0];

    if(lbValue >= 'a' && lbValue <= 'z') lbValue=lbValue-'a' + 'A';	/* Change 	small letters to capitals */
    if(lbValue < '0' || lbValue > 'Z') lbValue=':';					        /* Change ascii space to alloff */
    
    lbValue = (lbValue-'0')*_LCDTYPE;								                /* Find the value at the ascii table */
    lbCounter = _LCDTYPE;							/* Set the number of waveforms needed to write one alphanumeric */
    
    while (lbCounter--)
    {
    
       lbOffset = bpaPlace[bLCD_CharPosition];					           	/* Find the Waveform offset */            
       *(lbpLCDWFCopy + lbOffset)&=(~(baBackup[lbCounter] & 0xFF));	/* Backup of the waveforms */
       *(lbpLCDWFCopy + lbOffset) |= baAscii[lbValue];
       *(lbpLCDWF + lbOffset) = *(lbpLCDWFCopy + lbOffset);			    /* Write the value to the waveform */
       lbValue++;  
        bLCD_CharPosition++;           
    }            
  
}

/****************************************************************************************************
* vfnLCD_Set_Display
*  --------------------------------------------------------------------------------------------------
* Turns on all the segments of the LCD
****************************************************************************************************/ 

void vfnLCD_Set_Display (void)
{
    unsigned char lu8temp;
  	
  	Segment1_ON;       					             /* Turn on all the segments from the LCD */
  	Segment2_ON;
  	Segment3_ON;
  	Segment4_ON;
  	Segment5_ON;
  	Segment6_ON;
  	Segment7_ON;
  	Segment8_ON;
  	Segment9_ON;
  	Segment10_ON;
  	Segment11_ON;       									
  	Segment12_ON ;
  	Segment13_ON;
  	Segment14_ON;
    
    for(lu8temp = 0; lu8temp < 45; lu8temp++)     /* Set the RAM Buffer */
  	{
  	  RamBuffer[lu8temp] = 0xFF;
  	} 
}

/****************************************************************************************************
* vfnLCD_Clear_Display
* --------------------------------------------------------------------------------------------------
* Turns off all the segments of the LCD
****************************************************************************************************/ 

void vfnLCD_Clear_Display (void)
{
  	unsigned char lu8temp;
  	
  	Segment1_OFF;       				            /* Turn off all the segments fo the LCD */
  	Segment2_OFF;
  	Segment3_OFF;
  	Segment4_OFF;
  	Segment5_OFF;
  	Segment6_OFF;
  	Segment7_OFF;
  	Segment8_OFF;
  	Segment9_OFF;
  	Segment10_OFF;
  	Segment11_OFF;       									
  	Segment12_OFF;
  	Segment13_OFF;
  	Segment14_OFF;
  	
  	for(lu8temp = 0; lu8temp < 45; lu8temp++)     /* Clear the RAM Buffer */
  	{
  	  RamBuffer[lu8temp] = 0;
  	}
	
}

/****************************************************************************************************
* vfnSensitivityConfig
* --------------------------------------------------------------------------------------------------
* Configures the default sensitivity of all the electrodes 
****************************************************************************************************/  

void vfnSensitivityConfig(void)
{
     /* SLIDER */
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+0,0x18); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+1,0x18); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+2,0x18); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+3,0x18); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+4,0x18); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+5,0x18); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+6,0x18); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+7,0x18); 

    /* ROTARY */    

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+8,0x15); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+9,0x15); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+10,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+11,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+12,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+13,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+14,0x15); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+15,0x15);

    /* LED */     
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+16,0x14); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+17,0x14); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+18,0x12); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+19,0x14);

    /* KEYPAD */     

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+20,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+21,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+22,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+23,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+24,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+25,0x10); 

    /* MUX */   

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+26,0x08);
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+27,0x0C);
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+28,0x0C);
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+29,0x0C);       

    /* DIMENSION */

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+30,0x1A);
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+31,0x32); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+32,0x32); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+33,0x32);

    /* ROTARY */

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+34,0x10);  
}
