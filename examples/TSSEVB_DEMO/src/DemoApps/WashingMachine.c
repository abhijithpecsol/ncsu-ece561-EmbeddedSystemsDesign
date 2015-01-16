/***************************************************************************************
*
* Demo project for Freescale TSS and TSSEVB Demo Board
*
* This program uses the TSS. The project includes four Demos to test the TSSEVB funtionality 
* along with the TSS Software. For more information regarding the Demo Software please refer
* to the "Demo Application Guide" document.   
*
* Before to run this example, verify that all jumpers are in their default
* position. See the "System Overview" chapter in the TSSEVB user's manual.
*                                                                                       
* --------------------------------------------------------------------------------------
*                                                                                                         
* @file   WashingMachine.c
*
* @brief  Washing Machine Demo Application
*
* @version 1.0.0.1
* 
* @date September - 18 - 2009                                                           
*                                                                                          
* --------------------------------------------------------------------------------------
*
* RTAC Americas
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*****************************************************************************************/

#include <hidef.h>                  /* for EnableInterrupts macro */
#include "derivative.h"             /* include peripheral declarations */
#include "TSS_API.h"                /* Include Touch Sense Software */
#include "main.h"

/****************************************************************************************************
* Variables
*****************************************************************************************************/

extern uint8_t Message1[];            /* Characters array                                 */
extern uint8_t lu8Elec;               /* Number of electrode for the MUX electrodes ctrl  */
extern struct time MCVE;            /* Structure of the backwards count                 */
extern uint8_t gu8DimStatus;          /* Dimention Ctrl touch or release informtation     */
extern uint8_t gu8MCVEcountActive;    /* Microwave count active flag                      */
extern uint8_t gu8DimElecNum;         /* Electrode of the Dimention Ctrl being touched    */
extern uint8_t Keylock;               /* Lock function flag                               */
extern uint8_t gu8KeyBoard;           /* Code for each key                                */
extern uint8_t gu8KeyBoardEvent;      /* KeyBoard touch or release information            */
extern uint8_t gu8MCVEActiveLCD;      /* Rotary segment number                            */
extern uint8_t gu8MCVEActiveLCD2;     /* Rotary segment number - 1                        */
                                    
uint8_t gu8TemperatureBack;           /* Store the Slider electrodes status               */
uint8_t gu8TemperatureDelay = 0x0A;   /* Delay for touch of the slider buttons            */
uint8_t gu8WashCycle;                 /* Currnet Whash Cycle inforamtion                  */
 
#pragma DATA_SEG MY_ZEROPAGE        
extern uint8_t near gu8State;         /* Variable use to store the current state          */
extern uint8_t near gu8Status;        /* Variable that reports when is a pending event of the decoders */
DEMOFLAGS near gu8Washf;            /* Flags variable of the washing machine demo       */
DEMOFLAGS1 near gu8DemoWash;        /* Flags variable of the washing machine demo       */
#pragma DATA_SEG DEFAULT 

/****************************************************************************************************
* vfnWashingMachineConfigure
* ---------------------------------------------------------------------------------------------------
* Configures the washing machine demo
*****************************************************************************************************/
 
void vfnWashingMachineConfigure(void)  
{
    gu8State++;                     /* Go to the Washing Machine Demo STATE */
    if (gu8Washf.fOnOff)            /* If the demo is turned on           */
    {
        /* Enable the electrodes used by the demo */
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0xFF);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x55);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0xFF);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x7F);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x07);
    }
    
    /* Sets the Sensitivity value for each electrode for the Washing Machine overlay */

    /* SLIDER Electrodes */

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+0,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+1,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+2,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+3,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+4,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+5,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+6,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+7,0x10); 

    /* ROTARY Electrodes */    

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+8,0x0C); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+9,0x0C); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+10,0x0C); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+11,0x0C); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+12,0x0C); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+13,0x0C); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+14,0x0C); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+15,0x0C);

    /* LED Electrodes */     
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+16,0x0a); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+17,0x13); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+18,0x09); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+19,0x08);

    /* KEYPAD Electrodes */     

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+20,0x08); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+21,0x08); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+22,0x08); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+23,0x08); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+24,0x08); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+25,0x08); 

    /* MUX Electrodes */   

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+26,0x06);
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+27,0x06);
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+28,0x06);
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+29,0x06);        

    /* DIMENSION Electrodes */

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+30,0x18); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+31,0x10); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+32,0x1E); 
    (void)TSS_SetSystemConfig(System_Sensitivity_Register+33,0x1E);

    /* Central ROTARY Electrode */

    (void)TSS_SetSystemConfig(System_Sensitivity_Register+34,0x10); 

    vfnLCD_Clear_Display ();          /* Clear all the LCD segments */
    MCVE.HourMSB=0;                   /* Clear the backcount time values */
    MCVE.HourLSB=0;
    MCVE.MinuteMSB=0;
    MCVE.MinuteLSB=0;                 
    gu8MCVEcountActive = 0;           /* Clear the active count flag */
    gu8Washf.fOpen = 0;               /* Clear the open button flag */
    gu8Washf.fTimer = 0;              /* Clear the Timer function flag */
    gu8Washf.fEnergySaving = 0;       /* Clear the Energy Saving button flag */
    Keylock = 0;                      /* Clear the Key Lock function flag */
    _LCD_V1_ON;                       /* Display the default temperature value in the LCD */
    _LCD_V2_ON;
    _LCD_D1_ON;                       /* Display the default Load Size value in the LCD */
    _LCD_D2_OFF;
    _LCD_D3_OFF;
    _LCD_D4_OFF;
    _LCD_FREESCALE_ON;                /* Turn on the Freescale segment of the LCD */
    _LCD_COL1_ON;                     /* Turn on the COL1(":") segment of the LCD */
    LED_OFF(LED1);                    /* Turn off all the buttons LEDs */
    LED_OFF(LED2);
    LED_OFF(LED3);
    LED_OFF(LED4);
             
}

/****************************************************************************************************
* vfnWashingMachine
* ---------------------------------------------------------------------------------------------------
* Washing Machine function
*****************************************************************************************************/

void vfnWashingMachine(void)
{
      
    uint8_t u8Hot; 
    uint8_t u8Warm;
    uint8_t u8Cold;
    uint8_t u8Position;           /* 8 bits local variable used to store the event information */
    uint8_t lcv;        
    if(gu8Status)                                           /* If an event has ocurred in one of the controls */
    {
        if(gu8Status&0x10)                                  /* If the event has ocurred in the MUX Electrodes control */
        {
            /* LOAD SIZE KEYS */
            uint8_t u8ElecNum;    /* 8 bits local variable used to store the event information */
            gu8Status &= 0xEF;                              /* Clear the MUX Electrodes event flag */ 
            while (!TSS_KEYPAD_BUFFER_EMPTY(cKey4))         /* While unread events are in the buffer */
            {
                TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey4);    /* Read the buffer and store the event in the u8Event variable */
                if(u8ElecNum & 0x80)                        /* If the event is a release */
                {//Released                                 
                    u8ElecNum = u8ElecNum & 0x0F;           /* Saves the Electrode number */
                    u8ElecNum = 1<<u8ElecNum;               
                    lu8Elec &= ~u8ElecNum;                  /* Clears the corresponding bit of the electrode released */
                }                                           
                else                                        /* If the event is a touch */ 
                {//Pressed
                    u8ElecNum = 1<<u8ElecNum;               
                    lu8Elec |= u8ElecNum;                   /* Set the corresponding bit of the electrode touched */
                }
            }
            switch(lu8Elec)                                 /* Test the variable where the events were stored */
            {
                case 0x05:                                  /* If electrodes 1 & 3 are touched (small) */
                    _LCD_D1_ON;                             /* Turn on the SMALL button segment of the LCD */
                    _LCD_D2_OFF;                            /* Turn off all the others Load Size butons */
                    _LCD_D3_OFF;
                    _LCD_D4_OFF;
                    vfnBUZZ();                              /* Activate the buzzer for 50ms */
                    break;
                case 0x06:
                    _LCD_D1_OFF;                            /* If electrodes 2 & 3 are touched (med) */
                    _LCD_D2_ON;                             /* Turn on the MED button segment of the LCD */
                    _LCD_D3_OFF;                            /* Turn off all the others Load Size butons */
                    _LCD_D4_OFF;
                    vfnBUZZ();
                    break;                                  /* Activate the buzzer for 50ms */
                case 0x09:
                    _LCD_D1_OFF;                            /* If electrodes 1 & 4 are touched (LARGE) */
                    _LCD_D2_OFF;                            /* Turn on the LARGE button segment of the LCD */
                    _LCD_D3_ON;                             /* Turn off all the others Load Size butons */
                    _LCD_D4_OFF;
                    vfnBUZZ();
                    break;                                  /* Activate the buzzer for 50ms */
                case 0x0A:
                    _LCD_D1_OFF;                            /* If electrodes 1 & 3 are touched (SUPER PLUS) */
                    _LCD_D2_OFF;                            /* Turn on the SUPER PLUS button segment of the LCD */
                    _LCD_D3_OFF;                            /* Turn off all the others Load Size butons */
                    _LCD_D4_ON;
                    vfnBUZZ();
                    break;                                  /* Activate the buzzer for 50ms */
                default:
                    break;
            }

        }
                                                            
        if(gu8Status&0x20)                              /* If the event has ocurred in the DIMENSION Electrodes control */
        {/* ON/OFF, OPEN & LIGHTS ON/OFF KEYS */
                    
            gfDimentionElectrodes();                        /* Read and store the events */
            if (!(gu8DimStatus & 0x80))                     /* If the event is a touch */
            {
                if (gu8DimElecNum == LIGHT_ON_OFF)          /* if the LIGHT On/Off button is touched */
                {
                    gu8DemoWash.fLights = ~gu8DemoWash.fLights;  /* Toggle the LightOn/Off flag */
                    if (gu8DemoWash.fLights)                     /* If the LightOn/Off flag is set */
                    {
                        fnWriteDataTxBuffer((uint8_t)TURN_LIGHTS_ON);  /* Send the turn On LED1 & LED2 cmd */
                        fnWriteDataTxBuffer((uint8_t)TURN_LIGHTS_ON);
                        fnWriteDataTxBuffer((uint8_t)TURN_LIGHTS_ON);
                    }
                    else                                             /* If the LightOn/Off flag is cleared */
                    {   
                        fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);    /* Send the turn of all LEDs cmd */
                        fnWriteDataTxBuffer((uint8_t)WASHING_MACHINE_DEMO); /* Send the turn On LED6 cmd */
                        fnWriteDataTxBuffer((uint8_t)OVERLAY_CONFIG);  /* Send the turn On LED7 cmd */
                    }
                }

                else if (gu8DimElecNum == ON_OFF)            /* if the On/Off button is touched */
                {
                    if (gu8Washf.fOnOff)                     /* If the On/Off flag is set */
                    {
                        gu8Washf.fOnOff = 0;                 /* Clears the On/Off flag */
                        /* Disable most of the electrodes */
                        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0x00);
                        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x00);
                        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
                        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x06);
                        gu8State = WASHING_MACHINE_DEMO*2;   /* Go to the Washing machine config STATE */
                    }
                    else                                     /* If the On/Off flag is cleared */
                    {
                        gu8Washf.fOnOff = 1;                 /* Set the On/Off flag */
                        gu8State = WASHING_MACHINE_DEMO*2;   /* Go to the Washing machine config STATE */
                    }
                }
                else if (gu8DimElecNum == OPEN)              /* If the Open button is touched */
                {
                    if (gu8Washf.fOnOff)                     /* If the On/Off flag is set */ 
                    {                
                        gu8Washf.fOpen = ~gu8Washf.fOpen;    /* Toggle the open button flag */
                        gu8MCVEcountActive = 0;              /* clear the count active flag */
                        if (gu8Washf.fOpen)                  /* If the Open button flag is set */
                        {
                            _LCD_COL1_OFF;                   /* Turn off the COL1 segment (":") of the LCD */
                        } 
                        else                                 /* If the Open flag is cleared */
                        {
                            _LCD_COL1_ON;                    /* Turn of the COL1 segment (":") of the LCD */
                        }  
                    }

                }
            }                                                /* If the event is a release */
            else 
            {                                                /* If the Open flag is set */
                if(gu8Washf.fOpen)                           
                {                                            /* Turn off the Open button segment of the LCD */
                    _LCD_S4_ON;
                }
            }

        }                                                    /* If the event has ocurred in the LED ELECTRODES control */
        if(gu8Status&0x04)    
        {
        /* LOCK, TIMER, ENERGY SAVING & EXTRA RINSE KEYS */
            uint8_t u8ElecNum;           
            gu8Status &= 0xFB;                               /* Clear the LED Electrodes event flag */
            
            while (!TSS_KEYPAD_BUFFER_EMPTY(cKey2))          /* While unread events are in the buffer */
            {
                TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey2);     /* Read the buffer and store the event in the u8Event variable */ 
                
                if ((gu8Washf.fOnOff) && (!gu8Washf.fOpen))  /* If the demo is turned on AND the open flag is cleared */
                {  
                    if(u8ElecNum & 0x80)                     /* If the event is a release */
                    {
                                                             /* Do nothing */
                    }
                    else                                     /* If the event is a touch */
                    {  
                        if(u8ElecNum == LOCK)                /* If the LOCK button is touched */
                        {
                            if(Keylock==0)                   /* If the lock flag is cleared */
                            {
                                LED_ON(LED1);                /* Turn on the lock LED */
                                
                                Keylock = 1;                 /* Set the lock flag */
                                
                                /* Disable most elctrodes */
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0x00);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x01);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x02);
                            }
                            else                             /* If the lock flag is set */
                            {
                                LED_OFF(LED1);               /* Turn on the lock LED */
                                Keylock = 0;                 /* Set the lock flag */
                                /* Enable elctrodes */
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0xFF);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x55);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0xFF);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x7F);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x07);
                            }
                        }
                        else if(u8ElecNum==EXTRA_RINSE)        /* If the EXTRA RINSE button is touched */
                        {
                            LED_TOGGLE(LED2);                  /* Toggle the extra rinse LED */
                        }
                        else if(u8ElecNum==TIMER)              /* If the TIMER button is touched */
                        {
                            gu8Washf.fTimer = ~gu8Washf.fTimer;  /* Toggle the TIMER flag */
                            if (gu8Washf.fTimer)               /* If the Timer flag is set */
                            {
                                MCVE.HourMSB=0;                /* Clear the Back Count values */
                                MCVE.HourLSB=0;
                                MCVE.MinuteMSB=0;
                                MCVE.MinuteLSB=0;              
                                LED_ON(LED3);                  /* Turn on the Timer button LED */
                                for(lcv=0;lcv<0x08;lcv++)      /* Clear the rotary control segments (Delete Wash cycle segments)*/
                                {  
                                    SymbolOFF(16,lcv);
                                }
                            } 
                            else                               /* If the Timer flag is cleared */
                            {
                                MCVE.HourMSB=0;                /* Clear the Back Count values */
                                MCVE.HourLSB=0;
                                MCVE.MinuteMSB=0;
                                MCVE.MinuteLSB=0;
                                LED_OFF(LED3);                 /* Turn off the Timer button LED */
                            }
                        }
                        else if(u8ElecNum==ENERGY_SAVING)       /* If the EXTRA RINSE button is touched */
                        {
                            LED_TOGGLE(LED4);                   /* Toggle the extra rinse LED */
                            gu8Washf.fEnergySaving = ~gu8Washf.fEnergySaving; /* Toggle the Energy Saving flag */
                        }
                        vfnBUZZ();                              /* Activate the buzzer for 50ms */
                    }
                }
            }
        }         
        if(gu8Status&0x08)                                      /* If the event has ocurred in the KEYBOARD control */
        {  
            /* KEYBOARD Functions */
            gfKeyBoardElectrodes();                             /* Read and store the events */
            if (!gu8KeyBoardEvent)                              /* If the event is a touch */
            {  
                if ((gu8Washf.fOnOff) && (!gu8Washf.fOpen))     /* If the demo is turned on AND the open flag is cleared */
                {  
                     
                    if (gu8KeyBoard == CANCEL_CLEAR)            /* If the Cancel/Clear key is touched */
                    {
                        MCVE.HourMSB=0;                         /* Clear the Back Count values */
                        MCVE.HourLSB=0;
                        MCVE.MinuteMSB=0;
                        MCVE.MinuteLSB=0;
                    } 
                    else if (gu8KeyBoard == RESET )             /* If the RESET Key is touched */
                    {
                           gu8State = WASHING_MACHINE_DEMO*2;   /* Go to the Washing machine config STATE */                       
                    } 
                    else
                    {
                    
                    }
                    /* If the active count flag is cleared AND the timer flag is set */
                    if ((!gu8MCVEcountActive) && (gu8Washf.fTimer)) 
                    {
                        if (gu8KeyBoard == KEY_1)               /* If Key 1 is touched */
                        {
                            vfnDigitDisplacement();             /* Displace the current back count time values to the right */
                            MCVE.MinuteLSB=1;                   /* Store the key touched in the less significant character */
                        } 
                        else if (gu8KeyBoard == KEY_2)          /* Do the same for all the remaining keys when touched */
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=2;
                        }
                        else if (gu8KeyBoard == KEY_3)    
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=3;
                        }
                        else if (gu8KeyBoard == KEY_4)   
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=4;
                        }    
                        else if (gu8KeyBoard == KEY_5)    
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=5;
                        }                                  
                        else if (gu8KeyBoard == KEY_6)
                        {        
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=6;
                        }                                      
                        else if (gu8KeyBoard == KEY_7)
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=7;
                        }
                        else if (gu8KeyBoard == KEY_8)          
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=8;
                        }                                      
                        else if (gu8KeyBoard == KEY_9)
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=9;
                        }    
                        else if (gu8KeyBoard == KEY_10)       
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=0;
                        }

                        else                                    /* If no valid key is detected as touched */
                        {
                                                                /* Do nothing */
                        } 
                    }
                }   
            }
            
        }
        if(gu8Status&0x02)                                     /* If the event has ocurred in the ROTARY control */
        {
            gu8Status &= 0xFD;                                 /* Clear the ROTARY Electrodes event flag */
            u8Position = cRotary1.StaticStatus.Position;       /* Stores the position information */ 
            if (cRotary1.StaticStatus.Touch)                   /* If the event was a touch */
            {   
                /* If the demo is turned on AND the Open button flag is cleared AND the count flag is cleared */
                if ((gu8Washf.fOnOff) && (!gu8Washf.fOpen) && (!gu8Washf.fTimer)&&(!gu8MCVEcountActive))
                {   
                    if (u8Position ==  NORMAL_WASH)             /* If the Normal Wash cycle button is touched */
                    {
                        _LCD_R1_ON;                             /* Turn on/off the corresponing LCD segments */
                        _LCD_R2_OFF;
                        _LCD_R3_OFF;
                        _LCD_R4_OFF;
                        _LCD_R5_OFF;
                        _LCD_R6_OFF;
                        _LCD_R7_OFF;
                        _LCD_R8_OFF;
                        MCVE.HourMSB=0;                 /* Set a pre-defined time value in the back count structure (00:20) */
                        MCVE.HourLSB=0;
                        MCVE.MinuteMSB=2;
                        MCVE.MinuteLSB=0;
                        gu8WashCycle = 4;               /* Display the 4th rotary segment in the LCD when the count starts */
                    }
                    else if (u8Position ==  EXTRA_CARE_WASH)    /* If the Extra Care cycle button is touched */ 
                    {
                        _LCD_R1_OFF;                            /* Turn on/off the corresponing LCD segments */
                        _LCD_R2_OFF;
                        _LCD_R3_ON;
                        _LCD_R4_OFF;
                        _LCD_R5_OFF;
                        _LCD_R6_OFF;
                        _LCD_R7_OFF;
                        _LCD_R8_OFF;
                        MCVE.HourMSB=0;                 /* Set a pre-defined time value in the back count structure (00:05) */
                        MCVE.HourLSB=0;
                        MCVE.MinuteMSB=0;
                        MCVE.MinuteLSB=5;
                        gu8WashCycle = 2;               /* Display the 2nd rotary segment in the LCD when the count starts */
                    }
                    else if (u8Position ==  CASUAL_WASH)        /* If the Casual wash cycle button is touched */ 
                    {
                        _LCD_R1_OFF;                            /* Turn on/off the corresponing LCD segments */
                        _LCD_R2_OFF;
                        _LCD_R3_OFF;
                        _LCD_R4_OFF;
                        _LCD_R5_ON;
                        _LCD_R6_OFF;
                        _LCD_R7_OFF;
                        _LCD_R8_OFF;
                        MCVE.HourMSB=0;                 /* Set a pre-defined time value in the back count structure (00:10) */
                        MCVE.HourLSB=0;
                        MCVE.MinuteMSB=1;
                        MCVE.MinuteLSB=0;
                        gu8WashCycle = 0;               /* Display the zero rotary segment in the LCD when the count starts */
                    }
                    else if (u8Position ==  HAND_WASH)        /* If the hand Wash cycle button is touched */ 
                    {
                        _LCD_R1_OFF;                          /* Turn on/off the corresponing LCD segments */
                        _LCD_R2_OFF;
                        _LCD_R3_OFF;
                        _LCD_R4_OFF;
                        _LCD_R5_OFF;
                        _LCD_R6_OFF;
                        _LCD_R7_ON;
                        _LCD_R8_OFF;
                        MCVE.HourMSB=0;               /* Set a pre-defined time value in the back count structure (00:20) */
                        MCVE.HourLSB=0;
                        MCVE.MinuteMSB=1;
                        MCVE.MinuteLSB=0;
                        gu8WashCycle = 6;             /* Display the 4th rotary segment in the LCD when the count starts */
                        
                    }
                    else                              /* If is not a valid key */
                    {
                                                      /* Do nothing */
                    }
                    gu8MCVEActiveLCD = gu8WashCycle;  /* Stores the starting rotary segment */
                    vfnBUZZ(); 
                }                                     /* Activates the buzzer for 50ms */
            }
            else                                      /* If the event was a release */
            {

            }                                         /* Do nothing */
        }
        if(gu8Status&0x40)                            /* If the event has ocurred in the Rotary Central Button */
        {
            uint8_t u8ElecNum;            
            gu8Status &= 0xBF;                           /* Clear the Rotary Central Button event flag */
            while (!TSS_KEYPAD_BUFFER_EMPTY(cKey6))      /* While unread events are in the buffer */
            {
                TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey6); /* Read the buffer and store the event in the u8Event variable */
                if(u8ElecNum & 0x80)                     /* If the event is a release */
                {
                    _LCD_R9_OFF;                         /* Turn off the central rotary segment of the LCD */
                }
                else                                     /* If the event is a touch */ 
                {
                    /* If the demo is turned on AND the open button flag is cleared */
                    if ((gu8Washf.fOnOff)&&(!gu8Washf.fOpen))   
                    {         
                        if (gu8MCVEcountActive)     /* If the active count flag is set */
                        {
                            gu8MCVEcountActive = 0; /* Clear the active count flag */
                        }
                        else                        /* If the active count flag is cleared */
                        {   
                            /* If the back count time is NOT zero */ 
                            if (MCVE.HourMSB || MCVE.HourLSB || MCVE.MinuteMSB || MCVE.MinuteLSB) 
                            {
                                gu8MCVEcountActive = 1;  /* Set the active count flag */
                            } 
                        }                       
                    }
                    _LCD_R9_ON;                     /* Turn on the central rotary segment of the LCD */
                    vfnBUZZ();                      /* Activate the buzzer for 50ms */
                }
                
            }
        }
    }

    if (gu8Washf.fOnOff)                            /* If the demo is turned on */
    {                                               /* If the Open button flag is cleared */
        if(!gu8Washf.fOpen)
        {
                                                    /* Display the back count time information */
            vfnLCD_Home();
            vfnLCD_Write_Char(Message1[MCVE.HourMSB]);
            vfnLCD_Write_Char(Message1[MCVE.HourLSB]);
            vfnLCD_Write_Char(Message1[MCVE.MinuteMSB]);
            vfnLCD_Write_Char(Message1[MCVE.MinuteLSB]);
            _LCD_AM_OFF;
            _LCD_PM_OFF;
        }                                           /* If the Open button flag is set */
        else
        {
            vfnLCD_Write_Msg ("Open");              /* Display the "OPEN" message in the LCD */
            _LCD_AM_OFF;
            _LCD_PM_OFF;
        }
    } 
    else                                            /* If the demo is turned off */
    {
        vfnLCD_Clear_Display ();                    /* Turn off all the segements of the LCD */
        vfnLCD_Write_Msg (" OFF");                  /* Display the " OFF" message*/
        _LCD_FREESCALE_OFF;                         /* Turn off the Freescale segment of the LCD */
    }
    if (gu8MCVEcountActive)                         /* If the active count flag is active */
    {
        SymbolOFF(16,0);                            /* Turn off the Rotary control segments of the LCD */
        SymbolOFF(16,1);
        SymbolOFF(16,2);
        SymbolOFF(16,3);
        SymbolOFF(16,4);
        SymbolOFF(16,5);
        SymbolOFF(16,6);
        SymbolOFF(16,7);
        _LCD_R9_ON;                                 /* Turn on the central rotary segment of the LCD */
        SymbolON(16,gu8MCVEActiveLCD);              /* Turn on two segments of the rotary control */
        SymbolON(16,gu8MCVEActiveLCD2);             /* These segments will change when the Active count flag is set */
        if (!gu8Washf.fTimer)                       /* If the Timer flag is cleared */
        SymbolON(16,gu8WashCycle);                  /* Turn on the corresponding Wash Cycle segment */
    }
    /* TEMPERATURE SELECTION KEYS */                        
    if ((!gu8MCVEcountActive) && (gu8Washf.fOnOff) && (!gu8Washf.fOpen))
    /* If the demo is turned on AND the Open button flag is cleared AND the count flag is cleared */
    {    
        if (gu8TemperatureBack != tss_au8ElectrodeStatus[0]) /* If the past Temperature value is different than current */
        {
            gu8TemperatureDelay--;                           /* Decremt the temperature debounce count */
            if (!gu8TemperatureDelay)                        /* If the temperatur debounce count is over */
            {
                gu8TemperatureDelay = 0x0A;                  /* Set the temperature debounce count value */
                gu8TemperatureBack = tss_au8ElectrodeStatus[0];  /* Store the current value of temperature in the past variable */
                if(gu8TemperatureBack)vfnBUZZ();             /* If the temperature value is NOT zero */
            }                                                /* Activate the buzzer for 50ms */
            if (gu8TemperatureBack)                          /* If the temperature value is NOT zero */
            {
                                                             /* Store the past temperature values into local variables */
                u8Hot = gu8TemperatureBack & 0x03;           
                u8Warm = gu8TemperatureBack & 0x3C;
                u8Cold = gu8TemperatureBack & 0xC0;
            
                if (u8Warm && u8Cold)                        /* if warm and cold buttons are pressed */
                {
                    _LCD_V1_OFF;                             /* Turn on/off the corresponding LCD segments */
                    _LCD_V2_OFF;
                    _LCD_V3_ON;
                    _LCD_V4_ON;
                    _LCD_V5_ON;
                    _LCD_V6_ON;
                    _LCD_V7_ON;
                    _LCD_V8_ON;
                }
                
                else if (u8Hot && u8Cold)                    /* if hot and cold buttons are pressed */
                {
                    _LCD_V1_ON;                              /* Turn on/off the corresponding LCD segments */
                    _LCD_V2_ON;
                    _LCD_V3_OFF;
                    _LCD_V4_OFF;
                    _LCD_V5_OFF;
                    _LCD_V6_OFF;
                    _LCD_V7_ON;
                    _LCD_V8_ON;
                }
                
                else if (u8Hot && u8Warm)                    /* if hot and warm buttons are pressed */
                {
                    _LCD_V1_ON;                              /* Turn on/off the corresponding LCD segments */
                    _LCD_V2_ON;
                    _LCD_V3_ON;
                    _LCD_V4_ON;
                    _LCD_V5_ON;
                    _LCD_V6_ON;
                    _LCD_V7_OFF;
                    _LCD_V8_OFF;
                } 
                else if (u8Hot)                              /* if hot button is pressed */
                {
                    _LCD_V1_ON;                              /* Turn on/off the corresponding LCD segments */
                    _LCD_V2_ON;
                    _LCD_V3_OFF;
                    _LCD_V4_OFF;
                    _LCD_V5_OFF;
                    _LCD_V6_OFF;
                    _LCD_V7_OFF;
                    _LCD_V8_OFF;
                } 
                else if (u8Warm)                             /* if warm button is pressed */
                {
                    _LCD_V1_OFF;                             /* Turn on/off the corresponding LCD segments */
                    _LCD_V2_OFF;
                    _LCD_V3_ON;
                    _LCD_V4_ON;
                    _LCD_V5_ON;
                    _LCD_V6_ON;
                    _LCD_V7_OFF;
                    _LCD_V8_OFF;
                } 
                else if (u8Cold)
                {                                            /* if cold button is pressed */
                    _LCD_V1_OFF;
                    _LCD_V2_OFF;                             /* Turn on/off the corresponding LCD segments */
                    _LCD_V3_OFF;
                    _LCD_V4_OFF;
                    _LCD_V5_OFF;
                    _LCD_V6_OFF;
                    _LCD_V7_ON;
                    _LCD_V8_ON;
                }                                            /* If no valid combination is detected */
                else
                {                                            /* Do nothing */
                
                }
                
            }
        
        }
        
      
      }
    

    
}

