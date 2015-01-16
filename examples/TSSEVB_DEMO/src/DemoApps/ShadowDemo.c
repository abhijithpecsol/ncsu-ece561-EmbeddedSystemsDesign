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
* @file   ShadowDemo.c
*
* @brief  Detects when any electrodes is touched and turns on the corresponding LCD segment
*
* @version 1.0.0.0
* 
* @date September - 17 - 2009                                                       
*                                                                                          
* --------------------------------------------------------------------------------------
*
* RTAC Americas
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2009 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*****************************************************************************************/

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "TSS_API.h"    /* Include Touch Sense Software */
#include "main.h"

/****************************************************************************************************
* Variables
*****************************************************************************************************/

#pragma DATA_SEG MY_ZEROPAGE
extern uint8_t near gu8State;             /* Variable use to store the current state */
extern uint8_t near gu8Status;            /* Variable that reports when is a pending event of the decoders */
#pragma DATA_SEG DEFAULT
                                        
extern int8_t gs8Volume;                  /* Slider control variable (volume) */   
extern uint8_t Message1[];                /* Characters array */

int8_t  i8Rotary4LSB = 0;                 /* Alphanumeric character value */
int8_t  i8Rotary3LSB = 1;                 /* Alphanumeric character value */
int8_t  i8Rotary2LSB = 2;                 /* Alphanumeric character value */
int8_t  i8Rotary1LSB = 3;                 /* Alphanumeric character value */

/****************************************************************************************************
* vfnShadowDemoConfigure
* ---------------------------------------------------------------------------------------------------
* Configures the Shadow Demo State
*****************************************************************************************************/

void vfnShadowDemoConfigure(void)
{
    /* Enables all the electrodes used in the Shadow demo application */
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register,0xFF);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0xFF);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0xFF);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0xFF);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0xFF);
    vfnSensitivityConfig();                     /* Configures de sensitivity values for each electrode */
    _LCD_N1_OFF;                                /* Turns off the N1 segment of the LCD */
    vfnLCD_Home();                              /* Go the most significant alphanumeric character of the LCD */
    vfnLCD_Write_Char(Message1[i8Rotary4LSB]);  /* writes into the LCD the most significant character */
    vfnLCD_Write_Char(Message1[i8Rotary3LSB]);  /* writes into the LCD the next character */
    vfnLCD_Write_Char(Message1[i8Rotary2LSB]);  /* writes into the LCD the next character */
    vfnLCD_Write_Char(Message1[i8Rotary1LSB]);  /* writes into the LCD the next character */
    _LCD_FREESCALE_ON;                          /* turns on the freescale segment */
    gu8State = gu8State++;                      /* Sets the Shadow Demo STATE */
    fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);   /* Sends the turn of all LEDs command to the communications MCU */
    fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);   /* Sends the turn of all LEDs command to the communications MCU */
    fnWriteDataTxBuffer((uint8_t)SHADOW_DEMO);    /* Sends the turn on the shadow demo LED to the communications MCU */
}

/****************************************************************************************************
* vfnShadowDemo
* ---------------------------------------------------------------------------------------------------
* Detects when any electrodes is touched and turns on the corresponding LCD segment
*****************************************************************************************************/

void vfnShadowDemo(void)
{
  if(gu8Status)                                 /* If an event has ocurred in one of the controls     */
  {
    if(gu8Status&0x01)                          /* If the event has ocurred in the SLIDER control     */
    {
        uint8_t lcv;
        gu8Status &= 0xFE;                      /* Clear the Slider event flag                      */
        if (cSlider0.DynamicStatus.Direction)   /* If the direction is from left to right           */
        {   
            /* Increment the volume variable the number of advanced positions                       */
            gs8Volume = gs8Volume + cSlider0.DynamicStatus.Displacement;  
            if (gs8Volume > 0x08)               /* If volume has reached the upper limit            */
            {    
                gs8Volume = 0x08;               /* Keep volume the the maximum value                */
            }
        } 
        else                                    /* If the movement detected was from right to left  */
        {
            /* Decrement the volume variable the number of advanced positions */
            gs8Volume = gs8Volume - cSlider0.DynamicStatus.Displacement;
            if (gs8Volume < 0)                  /* If volume has reached the lower limit            */
            {    
                gs8Volume = 0;                  /* Keep volume in the minimum value                 */
            }
        }
            
                                                
        for(lcv=0;lcv<gs8Volume;lcv++)          /* Displays in the LCD the current volume value     */
        {  
            SymbolON(18,lcv);
        } 
        for(;lcv<0x08;lcv++)
        {  
            SymbolOFF(18,lcv);
        }                                      
        vfnBUZZ();                              /* Activates the buzzer for 50ms                    */
    }
    if(gu8Status&0x02)                          /* If the event has ocurred in the ROTARY control   */
    {
        uint8_t u8Position; /* 8 bits local variable used to store the event information              */
        uint8_t lcv;

        gu8Status &= 0xFD;                     /* Clear the Rotary event flag                       */
        u8Position = cRotary1.StaticStatus.Position; /* Store the current position of the rotary control */
        for(lcv=0;lcv<0x08;lcv++)              /* Clear the rotary segments of the LCD */
        {  
            SymbolOFF(16,lcv);
        }
        
        if (cRotary1.StaticStatus.Touch)       /* If the event was a touch                          */
        {    
            if (u8Position ==  0)              /* Turns on the Segments that correspond to the current position */
            {
                _LCD_R1_ON;
            } 
            else if (u8Position ==  1) 
            {
                 _LCD_R1_ON; 
                 _LCD_R2_ON;
            }
            else if (u8Position ==  2) 
            {
                 _LCD_R2_ON;
            }
            else if (u8Position ==  3) 
            {
                 _LCD_R2_ON;
                 _LCD_R3_ON;
            }
            else if (u8Position ==  4) 
            {
                 _LCD_R3_ON;
            }
            else if (u8Position ==  5) 
            {
                 _LCD_R3_ON;
                 _LCD_R4_ON;
            }
            else if (u8Position ==  6) 
            {
                 _LCD_R4_ON;
            }
            else if (u8Position ==  7) 
            {
                 _LCD_R4_ON;
                 _LCD_R5_ON;
            }
            else if (u8Position ==  8) 
            {
                 _LCD_R5_ON;
            }
            else if (u8Position ==  9) 
            {
                 _LCD_R5_ON;
                 _LCD_R6_ON;
            }
            else if (u8Position ==  10) 
            {
                 _LCD_R6_ON;
            }
            else if (u8Position ==  11) 
            {
                 _LCD_R6_ON;
                 _LCD_R7_ON;
            }
            else if (u8Position ==  12) 
            {
                 _LCD_R7_ON;
            }
            else if (u8Position ==  13) 
            {
                 _LCD_R7_ON;
                 _LCD_R8_ON;
            }
            else if (u8Position ==  14) 
            {
                 _LCD_R8_ON;
            } 
            else if (u8Position ==  15) 
            {
                 _LCD_R8_ON;
                 _LCD_R1_ON;
            }
            else                               /* If is not a valid position */
            {
                for(lcv=0;lcv<0x08;lcv++)      /* Turn all the rotary segments off */
                {  
                    SymbolOFF(16,lcv);
                }
            }
             

        }                                     /* If was a release event */
        else
        {                                     /* Do nothing */

        }

        if (cRotary1.DynamicStatus.Movement)  /* If the event was movement */
        {
            /* Increment or decrement the numbers of the alphanumeric characters according to the 
               displacement and direction detected on the rotary decoder */
            RotaryToAscii((uint8_t)cRotary1.DynamicStatus.Direction, (uint8_t)cRotary1.DynamicStatus.Displacement);
        }
        vfnLCD_Home();                              /* Go the most significant alphanumeric character of the LCD */
        vfnLCD_Write_Char(Message1[i8Rotary4LSB]);  /* writes into the LCD the most significant character */
        vfnLCD_Write_Char(Message1[i8Rotary3LSB]);  /* writes into the LCD the next character */
        vfnLCD_Write_Char(Message1[i8Rotary2LSB]);  /* writes into the LCD the next character */
        vfnLCD_Write_Char(Message1[i8Rotary1LSB]);  /* writes into the LCD the next character */
    }
    if(gu8Status&0x04)                              /* If the event has ocurred in the LED ELECTRODES control */
    {
        uint8_t u8ElecNum; /* 8 bits local variable used to store the event information */
        
        gu8Status &= 0xFB;                          /* Clear the LED Electrodes event flag */
        while (!TSS_KEYPAD_BUFFER_EMPTY(cKey2))     /* While unread events are in the buffer */
        {
            TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey2);/* Read the buffer and store the event in the u8Event variable */
            if(u8ElecNum & 0x80)                    /* If the event was a release */
            {
                u8ElecNum = u8ElecNum & 0x0F;       /* Store the electrode number */
                if(u8ElecNum==0)                    /* Determine the electrode released */
                {
                    _LCD_E1_OFF;                    /* Turn off the corresponding LCD segment */

                }
                else if(u8ElecNum==1)
                {
                    _LCD_E2_OFF;
                }
                else if(u8ElecNum==2)
                {
                    _LCD_E3_OFF;
                }
                else if(u8ElecNum==3)
                {
                    _LCD_E4_OFF;
                }
            }
            else                                    /* If the event was a touch */
            {
                if(u8ElecNum==0)                    /* Determine the electrode touched */
                {                                   /* Toggle the corresponding LED */
                    LED_TOGGLE(LED1);               /* Turn on the corresponding LCD segment */
                    _LCD_E1_ON;
                }
                else if(u8ElecNum==1)
                {
                    _LCD_E2_ON;
                    LED_TOGGLE(LED2);
                }
                else if(u8ElecNum==2)
                {
                    _LCD_E3_ON;
                    LED_TOGGLE(LED3);
                }
                else if(u8ElecNum==3)
                {
                    _LCD_E4_ON;
                    LED_TOGGLE(LED4);
                }
                vfnBUZZ();
            }
        }
    }
    if(gu8Status&0x08)                  /* If the event has ocurred in the KEYBOARD control */
    {
       gfKeyBoardElectrodes();          /* Read and store the events */
    }
    if(gu8Status&0x10)                  /* If the event has ocurred in the MUX ELECTRODES control */
    {
        gfMuxElectrodes();              /* Read and store the events */
    }
    if(gu8Status&0x20)                  /* If the event has ocurred in the DIMENSION ELECTRODES control */
    {
        gfDimentionElectrodes();        /* Read and store the events */
    }
    if(gu8Status&0x40)                  /* If the event has ocurred in the Rotary Central Button */
    {
        uint8_t u8ElecNum;
        
        gu8Status &= 0xBF;                                  
        while (!TSS_KEYPAD_BUFFER_EMPTY(cKey6))         /* While unread events are in the buffer */
        {
            TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey6);    /* Read the buffer and store the event in the u8Event variable */
            if(u8ElecNum & 0x80)                        /* If the event was a releas */
            {
                _LCD_R9_OFF;                            /* Turn off the corresponding segment of the LCD */
            }
            else                                        /* If the event was a touch */
            {
                _LCD_R9_ON;                             /* Turn on the corresponding LCD segment */
                vfnBUZZ();                              /* Activates the buzzer for 30 ms */
            }
            
        }
    }
    if(gu8Status&0x80)
    {
      asm BGND;
    }  
     
  }
}
