/***************************************************************************************************
*
* Demo project for Freescale TSS and TSSEVB Demo Board
*
* This program uses the TSS. The project includes three Demos to test the TSSEVB funtionality 
* along with the TSS Software. For more information regarding the Demo Software please refer
* to the "Demo Application Guide" document.   
*
* Before to run this example, verify that all jumpers are in their default
* position. See the "System Overview" chapter in the TSSEVB user's manual.
*                                                                                       
* --------------------------------------------------------------------------------------------------
*                                                                                                         
* @file   Microwave.c
*
* @brief  Microwave Demo Application
*
* @version 1.0.0.0
* 
* @date September - 18 - 2009                                                            
*                                                                                          
* --------------------------------------------------------------------------------------------------
*
* RTAC Americas
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*****************************************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "TSS_API.h"    /* Include Touch Sense Software */
#include "main.h"

/****************************************************************************************************
* Variables
*****************************************************************************************************/

#pragma DATA_SEG MY_ZEROPAGE
extern uint8_t near gu8State;         /* Variable use to store the current state          */
extern uint8_t near gu8Status;        /* Variable that reports when is a pending event of the decoders */
#pragma DATA_SEG DEFAULT

extern uint8_t Message1[];            /* Characters array                                 */
extern uint8_t Keylock;               /* Lock function flag                               */
extern int8_t  gs8Volume;             /* Slider control variable (volume)                 */
extern uint8_t lu8Elec;               /* Number of electrode for the MUX electrodes ctrl  */
extern struct time MCVE;            /* Microwave time structure                         */
extern struct time RTC;             /* Clock structure                                  */
extern uint8_t gu8MCVEcountActive;    /* Microwave count flag                             */
extern uint8_t gu8DimStatus;          /* Dimention Ctrl touch or release informtation     */
extern uint8_t gu8DimElecNum;         /* Rlectrode of the Dimention Ctrl being touched    */
extern uint8_t gu8KeyBoard;           /* Code for each key                                */
extern uint8_t gu8KeyBoardEvent;      /* KeyBoard touch or release information            */
extern uint8_t gu8MCVEDelayEnable;    /* Enables o Disables The delay count to show clock */
extern uint8_t gu8MCVEActiveLCD;      /* Rotary segment number                            */
extern uint8_t gu8MCVEActiveLCD2;     /* Rotary segment number - 1                        */
uint8_t gu8MCVEdigitDisplacement;     /* Displacement of the Digits flag                  */
uint8_t gu8MCVEopen;                  /* Microwave OPEN door flag                         */                                  
uint8_t gu8MCVEon;                    /* Microwave on/off flag                            */
uint8_t gu8MCVEclock;                 /* Show clock flag                                  */

/****************************************************************************************************
* vfnMicrowaveConfigure
* ---------------------------------------------------------------------------------------------------
* Configures the microwave demo
*****************************************************************************************************/
 
void vfnMicrowaveConfigure(void)
{
    vfnLCD_Clear_Display();         /* Clear all the LCD segments */
    vfnSensitivityConfig();         /* Configures de sensitivity values for each electrode */
    if (gu8MCVEon)                  /* if oven is TURN ON */
    {                               /* Enable all the electrodes used in the Microwave demo */
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0xFF);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0xF1);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x7F);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x07);
        
        _LCD_COL1_ON;               /* Turn on the COL1(":") segment of the LCD */
        _LCD_FREESCALE_ON;          /* Turn on the Freescale segment of the LCD */
    } 
    else                            /* if oven is TURN OFF */
    {   
        /* Disable most of the Electrodes */
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0x00);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x00);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x02);
        LED_OFF(LED1);              /* Turn off all the buttons LEDs */
        LED_OFF(LED2);
        LED_OFF(LED3);
        LED_OFF(LED4);        
    }
    gu8MCVEclock = 0;               /* Clear the display clock flag */
    gu8MCVEcountActive = 0;         /* Clear the active count flag */
    gu8MCVEopen = 0;                /* Clear the open flag */
    Keylock = 0;                    /* Clear the lock flag */
    gu8MCVEDelayEnable = 1;         /* Set the delay flag (Wait some time before displaying the clock) */
    gu8State++;                     /* Go to the Microwave Demo STATE */
    gu8MCVEActiveLCD = 4;           /* Set the first segment of the rotary control that must be turned on */
    MCVE.HourMSB=0;                 /* Clear the backcount time values */
    MCVE.HourLSB=0;
    MCVE.MinuteMSB=0;
    MCVE.MinuteLSB=0;        
    SymbolOFF(16,0);                /* Turn off the rotary control segment of the LCD */
    SymbolOFF(16,1);
    SymbolOFF(16,2);
    SymbolOFF(16,3);
    SymbolOFF(16,4);
    SymbolOFF(16,5);
    SymbolOFF(16,6);
    SymbolOFF(16,7);
}

/****************************************************************************************************
* vfnMicrowave
* ---------------------------------------------------------------------------------------------------
* Microwave function
*****************************************************************************************************/

void vfnMicrowave(void)
{   
    uint8_t lcv;
    
    if(gu8Status)                                       /* If an event has ocurred in one of the controls */
    {   
        /* QUICK ACCESS KEYS */
        if(gu8Status&0x10)                              /* If the event has ocurred in the MUX Electrodes control */
        {
            gfMuxElectrodes();                          /* Read and store the events */
            
            /* If the count is not active AND the demo is turned on AND the is not in the open mode */
            if ((!gu8MCVEcountActive) && (gu8MCVEon) && (!gu8MCVEopen))
            {    
                if (lu8Elec == POPCORN)                 /* if the POPCORN button is toched */
                {
                    gs8Volume = 6;                      /* Set a specific power value */
                    MCVE.HourMSB=0;                     /* Set a specific value of time */
                    MCVE.HourLSB=2;
                    MCVE.MinuteMSB=3;
                    MCVE.MinuteLSB=0;
                    gu8MCVEDelayEnable = 0;             /* Disable delay */
                }                                       
                else if (lu8Elec == REHEAT)             /* if the REHEAT button is toched */
                {
                    gs8Volume = 4;                      /* Set a specific power value */
                    MCVE.HourMSB=0;                     /* Set a specific value of time */
                    MCVE.HourLSB=4;
                    MCVE.MinuteMSB=0;
                    MCVE.MinuteLSB=0;
                    gu8MCVEDelayEnable = 0;             /* Disable the delay mode */
                } 
                else if (lu8Elec == SOUP)               /* if the SOUP button is toched */
                {
                    gs8Volume = 2;                      /* Set a specific power value */
                    MCVE.HourMSB=0;                     /* Set a specific value of time */
                    MCVE.HourLSB=1;
                    MCVE.MinuteMSB=3;
                    MCVE.MinuteLSB=0;
                    gu8MCVEDelayEnable = 0;             /* Disable delay */
                
                } 
                else if (lu8Elec == COFFE)              /* if the COFFE button is toched */
                {
                    gs8Volume = 8;                      /* Set a specific power value */
                    MCVE.HourMSB=0;                     /* Set a specific value of time */
                    MCVE.HourLSB=3;
                    MCVE.MinuteMSB=0;
                    MCVE.MinuteLSB=0;
                    gu8MCVEDelayEnable = 0;             /* Disable delay */
                } 
                else
                {
                }
            }
            gu8MCVEclock = 0;                           /* Disable the displaying of the clock */
        }
        if(gu8Status&0x20)                              /* If the event has ocurred in the DIMENSION Electrodes control */
        {
            /* SPECIAL FUNCTIONS KEYS */                /* Read and store the events */
            gfDimentionElectrodes();
                                                       
            if (!(gu8DimStatus & 0x80))                 /* If the event is a touch */
            {
                if (gu8DimElecNum == START_PAUSE)       /* If the Start/Pause button is touched */
                {
                    if ((gu8MCVEon)&&(!gu8MCVEopen))    /* If the Demo is turned on AND the open flag is cleared */
                    {    
                        if (gu8MCVEcountActive)         /* If the Active count flag is set */
                        {
                            gu8MCVEcountActive = 0;     /* Clear the Active count flag */
                        }
                        else                            /* If the Active count flag is cleared */
                        {    
                            /* If the microwave time is different from zero */
                            if (MCVE.HourMSB || MCVE.HourLSB || MCVE.MinuteMSB || MCVE.MinuteLSB) 
                            {
                                gu8MCVEcountActive = 1; /* Set the active count flag */
                                gu8MCVEDelayEnable = 0; /* Disable delay */  
                            } 
                        }                       
                    }
                }                                       /* If the On/Off button is touched */
                else if (gu8DimElecNum == ON_OFF)  
                {                                       
                    gu8MCVEon = ~(gu8MCVEon | 0xFE);    /* Toggle the on/off flag */
                    gu8State = MICROWAVE_DEMO*2;        /* Go to the Microwave config STATE */
                    gu8MCVEDelayEnable = 0;             /* Disable delay */
                }
                else if (gu8DimElecNum == OPEN)         /* If the Open button is touched */
                {
                    if (gu8MCVEon)                      /* If the demo is turn on */
                    {                
                        gu8MCVEopen = ~(gu8MCVEopen | 0xFE); /* Toggle the microwave open flag */
                        gu8MCVEcountActive = 0;         /* clear the count active flag */
                        if (gu8MCVEopen)                /* If the Open flag is set */
                        {
                            _LCD_COL1_OFF;              /* Turn off the COL1 segment (":") of the LCD */
                        } 
                        else                            /* If the Open flag is cleared */
                        {
                            _LCD_COL1_ON;               /* Turn of the COL1 segment (":") of the LCD */
                        }  
                    }

                }
            } 
            else                                        /* If the event is a release */
            {
                if(gu8MCVEopen)                         /* If the Open flag is set */
                {    
                    _LCD_S4_ON;                         /* Turn off the Open button segment of the LCD */
                }
            }
        }
                
        if(gu8Status&0x01)                              /* If the event has ocurred in the SLIDER control */
        {
            /* POWER SELECTION KEYS */
            gu8MCVEDelayEnable = 0;                     /* Disable delay */
            gu8MCVEclock = 0;                           /* Disable the displaying of the clock */
            gu8Status &= 0xFE;                          /* Clear the SLIDER control event flag */
            
            /* If the count is not active AND the demo is turned on AND the open flag is cleared */
            if ((!gu8MCVEcountActive) && (gu8MCVEon) && (!gu8MCVEopen))
            { 
                if (cSlider0.DynamicStatus.Direction)   /* If the direction is from left to right*/
                {
                    if(gs8Volume < 0x08)                /* If volume is smaller than the upper limit */
                    {   
                        /* Increment the volume variable the number of advanced positions */ 
                        gs8Volume = gs8Volume + cSlider0.DynamicStatus.Displacement; 
                        if (gs8Volume > 0x08)           /* If volume has reached the upper limit */
                        {    
                            gs8Volume = 0x08;           /* Keep volume the the maximum value */
                        }
                    }
                } 
                else                                    /* If the movement detected was from right to left  */
                {
                    if (gs8Volume > 0)                  /* If the volume is bigger than zero */
                    {
                         /* Decrement the volume variable the number of advanced positions */
                        gs8Volume = gs8Volume - cSlider0.DynamicStatus.Displacement;
                        if (gs8Volume < 0)              /* If volume has reached the lower limit */
                        {    
                            gs8Volume = 0;              /* Keep volume in the minimum value */
                        }
                     }
                }
                vfnBUZZ();                              /* Activate the buzzer for 50ms */
            }
            /* If the Microwave time is zero */
            if (!(MCVE.HourMSB || MCVE.HourLSB || MCVE.MinuteMSB || MCVE.MinuteLSB))  
            {
                gu8MCVEDelayEnable = 1;                 /* Enable delay */
            }
                   
        }
        if(gu8Status&0x04)                              /* If the event has ocurred in the LED ELECTRODES control */
        {
            /* LOCK & TIME FUNCTIONS KEYS */
            uint8_t u8ElecNum;           
            gu8Status &= 0xFB;                          /* Clear the LED Electrodes event flag */
            
            while (!TSS_KEYPAD_BUFFER_EMPTY(cKey2))     /* While unread events are in the buffer */
            {
                TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey2);/* Read the buffer and store the event in the u8Event variable */  
                
                if ((gu8MCVEon) && (!gu8MCVEopen))      /* If the demo is turned on AND the open flag is cleared */
                {  
                    if(u8ElecNum & 0x80)                /* If the event is a release */
                    {
                                                        /* Do nothing */
                    }
                    else                                /* If the event is a touch */
                    {  
                        if(u8ElecNum == LOCK)           /* If the LOCK button is touched */
                        {
                            if(Keylock==0)              /* If the lock flag is cleared */
                            {
                                LED_ON(LED1);           /* Turn on the lock LED */
                                
                                Keylock = 1;            /* Set the lock flag */
                                
                                /* Disable most elctrodes */
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0x00);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x01);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x06);
                            }
                            else                         /* If the lock flag is set */
                            {
                                LED_OFF(LED1);           /* Turn on the lock LED */
                                
                                Keylock = 0;             /* Set the lock flag */
                                
                                /* Enable elctrodes */
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0xFF);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0xF1);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x7F);
                                (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x07);
                            }
                        }
                        vfnBUZZ();                          /* Activate buzzer for 50ms */
                    }
                }
            }
        }        
        if(gu8Status&0x08)                                  /* If the event has ocurred in the KEYBOARD control */
        {  
            /* KEYBOARD Functions */                        /* Read and store the events */
            gfKeyBoardElectrodes();                         /* If the event is a touch */
            if (!gu8KeyBoardEvent)
            {  
                if ((gu8MCVEon) && (!gu8MCVEopen))          /* If the demo is turned on AND the open flag is cleared */
                {  
                    if (!gu8MCVEdigitDisplacement)          /* If the digit displacement flag is cleared */
                    {
                        MCVE.HourMSB=0;                     /* Clear the microwave time information */
                        MCVE.HourLSB=0;
                        MCVE.MinuteMSB=0;
                        MCVE.MinuteLSB=0;
                        
                        gu8MCVEdigitDisplacement = 1;       /* Set the digit displacement flag */
                    } 
                    
                    if (gu8KeyBoard == CANCEL_CLEAR)        /* If the cancel/clear button is touched */     
                    {
                        gu8State = MICROWAVE_DEMO*2;        /* Go to the Microwave config State */
                        gu8MCVEDelayEnable = 0;             /* Disable delay */
                    } 
                    if (!gu8MCVEcountActive)                /* If the active count flag is cleared */
                    {
                        if (gu8KeyBoard == KEY_1)           /* If Key 1 is touched */
                        {
                            vfnDigitDisplacement();         /* Displace the current microwave time values to the right */
                            MCVE.MinuteLSB=1;               /* Store the key touched in the less significant character */
                            gu8MCVEclock = 0;               /* Turn off the clock */
                            gu8MCVEDelayEnable = 0;         /* Disable delay */
                        } 
                        else if (gu8KeyBoard == KEY_2)      /* If Key 2 is touched */
                        {
                            vfnDigitDisplacement();         /* Displace the current microwave time values to the right */
                            MCVE.MinuteLSB=2;               /* Store the key touched in the less significant character */
                            gu8MCVEclock = 0;               /* Turn off the clock */
                            gu8MCVEDelayEnable = 0;
                        }
                        else if (gu8KeyBoard == KEY_3)      /* Same for the rest of the keys */
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=3;
                            gu8MCVEclock = 0;               
                            gu8MCVEDelayEnable = 0;  
                        }
                        else if (gu8KeyBoard == KEY_4)   
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=4;
                            gu8MCVEclock = 0;               
                            gu8MCVEDelayEnable = 0;
                        }    
                        else if (gu8KeyBoard == KEY_5)    
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=5;
                            gu8MCVEclock = 0;              
                            gu8MCVEDelayEnable = 0;
                        }                                  
                        else if (gu8KeyBoard == KEY_6)
                        {        
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=6;
                            gu8MCVEclock = 0;               
                            gu8MCVEDelayEnable = 0;
                        }                                      
                        else if (gu8KeyBoard == KEY_7)
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=7;
                            gu8MCVEclock = 0;              
                            gu8MCVEDelayEnable = 0;
                        }
                        else if (gu8KeyBoard == KEY_8)         
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=8;
                            gu8MCVEclock = 0;               
                            gu8MCVEDelayEnable = 0;
                        }                                      
                        else if (gu8KeyBoard == KEY_9)
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=9;
                            gu8MCVEclock = 0;              
                            gu8MCVEDelayEnable = 0;
                        }    
                        else if (gu8KeyBoard == KEY_10)      
                        {
                            vfnDigitDisplacement();
                            MCVE.MinuteLSB=0;
                            gu8MCVEclock = 0;               
                            gu8MCVEDelayEnable = 0;
                        }
                        else if (gu8KeyBoard == INCREMENT_30 ) /* If +30 key is touched */       
                        {
                            gu8MCVEDelayEnable = 0;         /* Disable delay */
                            gu8MCVEclock = 0;               /* Turn off the clock */
                            
                            MCVE.MinuteMSB+=3;              /* Add 30 seconds to the current microwave time value */
                            if (MCVE.MinuteMSB > 5)
                            {   
                                MCVE.HourLSB ++; 
                                MCVE.MinuteMSB -= 6;
                                
                                if (MCVE.HourLSB > 9)
                                {
                                    MCVE.HourLSB = 0;
                                    MCVE.HourMSB++;
                                    if (MCVE.HourMSB > 9)
                                    {
                                        MCVE.HourMSB=0;
                                        MCVE.HourLSB=0;
                                        MCVE.MinuteMSB=0;
                                        MCVE.MinuteLSB=0;    
                                    }
                                }
                            }
                            
                        }
                        else                                /* If no valid key is touched */
                        {
                                                            /* Do nothing */
                        }
                    }
                }   
            }
            
        }
        if(gu8Status&0x40)                                 /* If the event has ocurred in the central rotary button */ 
        {
            uint8_t u8ElecNum;
            
            gu8Status &= 0xBF;                              /* Clear the central rotary button event flag */
            while (!TSS_KEYPAD_BUFFER_EMPTY(cKey6))         /* While unread events are in the buffer */
            {   
                /* Read the buffer and store the event in the u8Event variable */
                TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey6);  
                if(u8ElecNum & 0x80)                        /* If the event is a release */
                {
                                                            /* Do nothing */
                }
                else                                        /* If the event is a touch */
                {
                    vfnBUZZ();                              /* Activate the buzzer for 50ms */
                }
                
            }
        }


    }
    if (gu8MCVEon)                                          /* If the on/off flag is set */ 
    {    
        for(lcv=0;lcv<gs8Volume;lcv++)                      /* Display the power information (volume) */
        {  
            SymbolON(18,lcv);
        } 
        for(;lcv<0x08;lcv++)
        {  
            SymbolOFF(18,lcv);
        }
        if(!gu8MCVEopen)                                     /* If the open flag is cleared */
        {
            if (gu8MCVEclock)                                /* If the display clock flag is set */
            {
                vfnLCD_Home();                               /* Display the clock */ 
                vfnLCD_Write_Char(Message1[RTC.HourMSB]);
                vfnLCD_Write_Char(Message1[RTC.HourLSB]);
                vfnLCD_Write_Char(Message1[RTC.MinuteMSB]);
                vfnLCD_Write_Char(Message1[RTC.MinuteLSB]);
                gu8MCVEDelayEnable = 0;                      /* Disable delay */
                if(RTC.AMPM==0)
                {
                  _LCD_AM_ON;
                  _LCD_PM_OFF;
                }
                else
                {
                  _LCD_AM_OFF;
                  _LCD_PM_ON;
                }
            }
            else                                             /* If the display clock flag is cleared */
            {
                vfnLCD_Home();                               /* Display the microwave time */
                vfnLCD_Write_Char(Message1[MCVE.HourMSB]);
                vfnLCD_Write_Char(Message1[MCVE.HourLSB]);
                vfnLCD_Write_Char(Message1[MCVE.MinuteMSB]);
                vfnLCD_Write_Char(Message1[MCVE.MinuteLSB]);
                _LCD_AM_OFF;                                  /* Turn off the AM and PM segments of the LCD */
                _LCD_PM_OFF;
            } 
        }
        else                                                  /* If the open flag is set */
        {
            vfnLCD_Write_Msg ("Open");                        /* Display the "OPEN" message in the LCD */
            _LCD_AM_OFF;                                      /* Turn off the AM and PM segments of the LCD */
            _LCD_PM_OFF;
        }
    } 
    else                                                      /* If the On/Off flag is cleared (demo is turned off) */
    {
        vfnLCD_Clear_Display ();                              /* Clear all the segments of the LCD */
        vfnLCD_Write_Msg (" OFF");                            /* Display the " OFF" messge in the LCD */
        _LCD_FREESCALE_OFF;                                   /* Turn off the Freescale segment in the LCD */
    }
    if (gu8MCVEcountActive)                                   /* If the active count flag is set */
    {
        SymbolOFF(16,0);                                      /* Turn on the rotary control segments in a secuence */
        SymbolOFF(16,1);
        SymbolOFF(16,2);
        SymbolOFF(16,3);
        SymbolOFF(16,4);
        SymbolOFF(16,5);
        SymbolOFF(16,6);
        SymbolOFF(16,7);
        SymbolON(16,gu8MCVEActiveLCD);                        /* Turn on one of the rotary segments and increments it */
        SymbolON(16,gu8MCVEActiveLCD2);
    }
 }

/****************************************************************************************************
* vfnDigitDisplacement
* ---------------------------------------------------------------------------------------------------
* Moves the microwave time one character to the left
*****************************************************************************************************/

void vfnDigitDisplacement(void)
{
    MCVE.HourMSB=MCVE.HourLSB;
    MCVE.HourLSB=MCVE.MinuteMSB;
    MCVE.MinuteMSB=MCVE.MinuteLSB;
}
