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
* @file   CommFunctions.c
*
* @brief  Functions shared for all the Demos
*
* @version 1.0.0.0
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

#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */
#include "TSS_API.h"    /* Include Touch Sense Software */
#include "main.h"

/****************************************************************************************************
* Variables
*****************************************************************************************************/

extern int8_t  i8Rotary1LSB;                 /* Alphanumeric character value */
extern int8_t  i8Rotary2LSB;                 /* Alphanumeric character value */
extern int8_t  i8Rotary3LSB;                 /* Alphanumeric character value */
extern int8_t  i8Rotary4LSB;                 /* Alphanumeric character value */

#pragma DATA_SEG MY_ZEROPAGE
extern uint8_t near gu8State;                /* Variable use to store the current state          */
extern uint8_t near gu8Status;               /* Variable that reports when is a pending event of the decoders */
extern DEMOFLAGS near gu8Washf;            /* Flags variable of the washing machine demo       */
extern DEMOFLAGS1 near gu8DemoWash;        /* Flags variable of the washing machine demo       */
#pragma DATA_SEG DEFAULT

extern uint8_t Message1[];                   /* Characters array                                 */
extern uint8_t gu8RTCsecond;                 /* Counts the seconds of the Clock                  */
extern uint8_t gu8MCVEsecond;                /* Counts the seconds of the Decrementing clock     */
extern uint8_t gu8MCVEcountActive;           /* Flag that indicates  when the MCVE application is counting */
extern uint8_t gu8MCVEopen;                  /* Microwave OPEN door flag                         */
extern uint8_t gu8MCVEDelayEnable;           /* Enables o Disables The delay count to show clock */
extern uint8_t gu8MCVEon;                    /* Microwave on/off flag                            */
extern uint8_t gs8Volume;                    /* Slider control variable (volume)                 */
extern uint8_t Keylock;                      /* Show clock flag                                  */
extern struct time RTC;                    /* Clock information structure                      */
struct time ROT;                           /* Alarm Structure                                  */
struct time MCVE;                          /* Microwave time structure                         */
uint8_t gu8MCVEActiveLCD;                    /* Rotary segment number                            */
uint8_t gu8MCVEActiveLCD2;                   /* Rotary segment number -1                         */
uint8_t lsu8ElectPressed = 0;                /* Event in the MUX electrodes ctrl                 */
uint8_t lu8Elec = 0;                         /* Number of event electrode for the MUX electrodes ctrl */
uint8_t gu8DimElecNum;                       /* Number of the event electrode of dimension ctrl  */
uint8_t gu8DimStatus;                        /* Event in dimension ctrl                          */
uint8_t gu8KeyBoard;                         /* Number of the event electrode of keyboard ctrl   */
uint8_t gu8KeyBoardEvent;                    /* Event in keyboard ctrl                           */
uint8_t  u8DemoNum = 1;                      /* Demo number (Default 1, Shadow Demo)             */


/****************************************************************************************************
* RotaryToAscii
* ---------------------------------------------------------------------------------------------------
* Increment/Decrement the numbers displayed on the alphanumeric characters of the LCD depending on the
* movement detected in the Rotary control
*****************************************************************************************************/


void RotaryToAscii(uint8_t Direction, uint8_t Displacement)
{    
    if (Direction)
    {
        i8Rotary1LSB = i8Rotary1LSB + (Displacement*Displacement);
        while(i8Rotary1LSB > 9)
        {    
            i8Rotary1LSB -= 10;
            i8Rotary2LSB ++;
            while(i8Rotary2LSB > 9)
            {
                i8Rotary2LSB -= 10;
                i8Rotary3LSB++;
                while(i8Rotary3LSB > 9)
                {
                    i8Rotary3LSB -= 10;
                    i8Rotary4LSB++;
                    while(i8Rotary4LSB>9)
                    {
                      i8Rotary4LSB=0;
                    }
                }
            }
        }
    } 
    else
    {
        i8Rotary1LSB = i8Rotary1LSB - (Displacement*Displacement);
        while(i8Rotary1LSB < 0)
        {    
            i8Rotary1LSB += 10;
            i8Rotary2LSB--;
            while(i8Rotary2LSB < 0)
            {
                i8Rotary2LSB += 10;
                i8Rotary3LSB--;
                while(i8Rotary3LSB < 0)
                {
                    i8Rotary3LSB += 10;
                    i8Rotary4LSB--;
                    while(i8Rotary4LSB < 0)
                    {
                      i8Rotary4LSB=9;
                    }
                }
            }
        }
    }
    vfnBUZZ(); 
}

/****************************************************************************************************
* RotaryToAsciiHours
* ---------------------------------------------------------------------------------------------------
* Increment/Decrement the clock/alarm values displayed on the alphanumeric characters of the LCD 
* depending on the movement detected in the Rotary control
*****************************************************************************************************/

void RotaryToAsciiHours(uint8_t Direction, uint8_t Displacement)
{
    if (Direction)
    {
        ROT.MinuteLSB = ROT.MinuteLSB + (Displacement*Displacement);
        while (ROT.MinuteLSB > 9)
        {    
            ROT.MinuteLSB -= 10;
            ROT.MinuteMSB ++;
            while(ROT.MinuteMSB > 5)
            {
                ROT.MinuteMSB -= 6;
                ROT.HourLSB++;
                while(ROT.HourLSB > 9)
                {
                    ROT.HourLSB -= 10;
                    ROT.HourMSB++;
                }
                if((ROT.HourMSB==1)&&(ROT.HourLSB==2))
                {
                  ROT.AMPM = ~ROT.AMPM;
                }
                if((ROT.HourMSB==1)&&(ROT.HourLSB==3))
                {
                  ROT.HourLSB=1;
                  ROT.HourMSB=0;
                }
            }
        }
        if((ROT.HourMSB==1)&&(ROT.HourLSB>=3))
        {
          ROT.HourLSB=2;
          ROT.HourMSB=1;
        }
    } 
    else
    {
        ROT.MinuteLSB = ROT.MinuteLSB - (Displacement*Displacement);
        while(ROT.MinuteLSB < 0)
        {    
            ROT.MinuteLSB += 10;
            ROT.MinuteMSB--;
            while(ROT.MinuteMSB < 0)
            {
                ROT.MinuteMSB += 6;
                ROT.HourLSB--;
                if(ROT.HourLSB < 2)
                {
                    if((ROT.HourMSB==0)&&(ROT.HourLSB==0))
                    {
                      ROT.HourLSB = 2;
                      ROT.HourMSB = 1;
                    }
                    else if((ROT.HourMSB==1)&&(ROT.HourLSB==1))
                    {
                      ROT.AMPM = ~ROT.AMPM;
                    }
                }
            }
        }
    }
}

/****************************************************************************************************
* vfsLCD_Write_NUMS
* ---------------------------------------------------------------------------------------------------
* Display a time structure on the alphanumeric characters of the LCD
*****************************************************************************************************/

void vfsLCD_Write_NUMS(struct time nums)
{
    vfnLCD_Home();
    vfnLCD_Write_Char(Message1[nums.HourMSB]);
    vfnLCD_Write_Char(Message1[nums.HourLSB]);
    vfnLCD_Write_Char(Message1[nums.MinuteMSB]);
    vfnLCD_Write_Char(Message1[nums.MinuteLSB]);

}

/****************************************************************************************************
* vfnUpdateClock
* ---------------------------------------------------------------------------------------------------
* Updates the information in the clock time structure
*****************************************************************************************************/

void vfnUpdateClock(void)
{
    if(gu8RTCsecond>=60)
    {
      gu8RTCsecond -= 60;
      RTC.MinuteLSB++;
      if(RTC.MinuteLSB>=10)
      {
        RTC.MinuteLSB-=10;
        RTC.MinuteMSB++;
        if(RTC.MinuteMSB>=6)
        {
          RTC.MinuteMSB-=6;
          RTC.HourLSB++;
          if(RTC.HourLSB>=10)
          {
            RTC.HourLSB-=10;
            RTC.HourMSB++;
          }
          if((RTC.HourMSB==1)&&(RTC.HourLSB==2))
          {
            RTC.AMPM = ~RTC.AMPM;
          }
          if((RTC.HourMSB==1)&&(RTC.HourLSB==3))
          {
            RTC.HourLSB=1;
            RTC.HourMSB=0;
          }
        }
      }
    }
}

/****************************************************************************************************
* gfMuxElectrodes
* ---------------------------------------------------------------------------------------------------
* Read and store the events in the MUX electrodes control 
*****************************************************************************************************/

void gfMuxElectrodes(void)
{  
    uint8_t u8ElecNum; /* 8 bits local variable used to store the event information */
    gu8Status &= 0xEF;
    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey4))      /* While unread events are in the buffer */
    {
        TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey4);  /* Read the buffer and store the event in the u8Event variable */
        if(u8ElecNum & 0x80)
        {//Released
          lsu8ElectPressed--;
          u8ElecNum = u8ElecNum & 0x0F;
          u8ElecNum = 1<<u8ElecNum;
          lu8Elec &= ~u8ElecNum;
        }
        else
        {//Pressed
          lsu8ElectPressed++;
          u8ElecNum = 1<<u8ElecNum;
          lu8Elec |= u8ElecNum; 
        }
    }
    _LCD_D1_OFF;
    _LCD_D2_OFF;
    _LCD_D3_OFF;
    _LCD_D4_OFF;
    switch(lu8Elec){
      case 0x05:
        _LCD_D1_ON;
        vfnBUZZ();
        break;
      case 0x06:
        _LCD_D2_ON;
        vfnBUZZ();
        break;
      case 0x09:
        _LCD_D3_ON;
        vfnBUZZ();
        break;
      case 0x0A:
        _LCD_D4_ON;
        vfnBUZZ();
        break;
      default:
        break;
    }
}


/****************************************************************************************************
* gfDimentionElectrodes
* ---------------------------------------------------------------------------------------------------
* Read and store the events in the Dimension electrodes control 
*****************************************************************************************************/


void gfDimentionElectrodes(void)
{  
    uint8_t u8ElecNum; /* 8 bits local variable used to store the event information */
    
    gu8Status&=0xDF;
    
    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey5))      /* While unread events are in the buffer */
    {
        TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey5);  /* Read the buffer and store the event in the u8Event variable */
        gu8DimStatus = u8ElecNum & 0x80;
        gu8DimElecNum = u8ElecNum & 0x0F;
        if(gu8DimStatus & 0x80)
        {   
          if(gu8DimElecNum==0)
          {
            _LCD_S4_OFF;
          }
          else if(gu8DimElecNum==1)
          {
            _LCD_S2_OFF;
          }
          else if(gu8DimElecNum==2)
          {
            _LCD_S1_OFF;
          }
          else if(gu8DimElecNum==3)
          {
            _LCD_S3_OFF;
          }
        }
        else
        {
          if(gu8DimElecNum==0)
          {
            _LCD_S4_ON;
          }
          else if(gu8DimElecNum==1)
          {
            _LCD_S2_ON;
          }
          else if(gu8DimElecNum==2)
          {
            _LCD_S1_ON;
          }
          else if(gu8DimElecNum==3)
          {
            _LCD_S3_ON;
          }
          vfnBUZZ(); 
        }
    }
}

/****************************************************************************************************
* gfKeyBoardElectrodes
* ---------------------------------------------------------------------------------------------------
* Read and store the events in the Keyboard electrodes control 
*****************************************************************************************************/

void gfKeyBoardElectrodes(void)
{
        uint8_t u8ElecNum;                            /* 8 bits local variable used to store the event information */
        uint8_t u8ElecMask;

        gu8Status &= 0xF7;
        while (!TSS_KEYPAD_BUFFER_EMPTY(cKey3))         /* While unread events are in the buffer */
        {
            TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey3);    /* Read the buffer and store the event in the u8Event variable */        
            gu8KeyBoardEvent = u8ElecNum & 0x80; 
            u8ElecMask = (uint8_t)(1u << (u8ElecNum & 0x0F)); 
            
            if (u8ElecNum & 0x80)                   /* If the event was a release */
            {
                gu8KeyBoard &= ~u8ElecMask;     
            } 
            else 
            {
                gu8KeyBoard |= u8ElecMask;
                 
            }

        }
        _LCD_N1_OFF;
        _LCD_N2_OFF;
        _LCD_N3_OFF;
        _LCD_N4_OFF;
        _LCD_N5_OFF;
        _LCD_N6_OFF;
        _LCD_N7_OFF;
        _LCD_N8_OFF;
        _LCD_N9_OFF;
        _LCD_N10_OFF;
        _LCD_N11_OFF;
        _LCD_N12_OFF;
        if (gu8KeyBoard == 0x05)
        {
            _LCD_N1_ON;
            vfnBUZZ();
        } 
        else if (gu8KeyBoard == 0x09)
        {
            _LCD_N2_ON;
            vfnBUZZ();
        }
        else if (gu8KeyBoard == 0x0a)
        {
            _LCD_N3_ON;
            vfnBUZZ();
        }
        else if (gu8KeyBoard == 0x14)
        {
            _LCD_N4_ON;
            vfnBUZZ();
        }    
        else if (gu8KeyBoard == 0x18)
        {
            _LCD_N5_ON;
            vfnBUZZ();
        }    
        else if (gu8KeyBoard == 0x0C)
        {
            _LCD_N6_ON;
            vfnBUZZ();
        }
        else if (gu8KeyBoard == 0x12)
        {
            _LCD_N7_ON;
            vfnBUZZ();
        }
        else if (gu8KeyBoard == 0x30)
        {
            _LCD_N8_ON;
            vfnBUZZ();
        }    
        else if (gu8KeyBoard == 0x24)
        {
            _LCD_N9_ON;
            vfnBUZZ();
        }    
        else if (gu8KeyBoard == 0x21)
        {
            _LCD_N10_ON;
            vfnBUZZ();
        }
        else if (gu8KeyBoard == 0x03)
        {
            _LCD_N11_ON;
            vfnBUZZ();
        }
        else if (gu8KeyBoard == 0x22)
        {
            _LCD_N12_ON;
            vfnBUZZ();
        }
        else
        {
       
        }      

}

/****************************************************************************************************
* vfnMCVEdecrementTime
* ---------------------------------------------------------------------------------------------------
* Decrement the values in the back time structure
*****************************************************************************************************/

void vfnMCVEdecrementTime(void)
{
    gu8MCVEActiveLCD2 = gu8MCVEActiveLCD;
    gu8MCVEActiveLCD = ((gu8MCVEActiveLCD + 1) & 0x07);
    if(gu8MCVEsecond>=60) 
    {
        
        gu8MCVEsecond -= 60;
        MCVE.MinuteLSB--;
        if(MCVE.MinuteLSB<0)
        {
            MCVE.MinuteLSB=9;
            MCVE.MinuteMSB--;
            if(MCVE.MinuteMSB<0)
            {
                MCVE.MinuteMSB=5;
                MCVE.HourLSB--;
                if(MCVE.HourLSB<0)
                {
                    MCVE.HourLSB=9;
                    MCVE.HourMSB--;
               
                    if (MCVE.HourMSB<0)
                    {
                        MCVE.HourMSB=0;
                        if (!MCVE.HourMSB && !MCVE.HourLSB && !MCVE.MinuteMSB && !MCVE.MinuteLSB) 
                        {
                               
                        } 
                        else
                        {
                            gu8MCVEcountActive = 0;
                            gu8State = gu8State--;
                            gu8MCVEActiveLCD = 4; 
                            gu8DemoWash.fBuzzLong = 1;
                            vfnBUZZ();
                        }  
                    }
                }
            }
        }
    }
}

/****************************************************************************************************
* vfnDemoSetConfigure
* ---------------------------------------------------------------------------------------------------
* Configures the Demo selection STATE
*****************************************************************************************************/

void vfnDemoSetConfigure(void)
{
    /* Enable the electrodes used by the demo */
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register,0x00);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x00);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x04);
    Keylock = 0;                        /* Clear the Key Lock function flag */
    vfnLCD_Clear_Display ();            /* Clear all the LCD segments */
    _LCD_S4_ON; 
    vfnWriteDemoSelection();            /* Display in the LCD the current selected Demo */
    gu8MCVEDelayEnable=0;               /* Clear the Clock delay flag  */
    gu8MCVEon = 0;                      /* Set the On/Off flag */
    _LCD_FREESCALE_ON;                  /* Turn on the Freescale segment in the LCD */
    LED_OFF(LED1);                      /* Turn off all the buttons LEDs */
    LED_OFF(LED2);
    LED_OFF(LED3);
    LED_OFF(LED4);
    gu8State = gu8State++;              /* Go to the Washing Set Demo STATE */
}

/****************************************************************************************************
* vfnDemoSet
* ---------------------------------------------------------------------------------------------------
* Menu to navigate between demos
*****************************************************************************************************/

void vfnDemoSet(void)
{
  if(gu8Status)                                /* If an event has ocurred in one of the controls */
  {
    uint8_t u8ElecNum;
    if(gu8Status&0x40)                         /* If the event has ocurred in the Rotary Central Button */
    {   
        gu8Status &= 0xBF;                            /* Clear the Rotary Central Button event flag */
        while (!TSS_KEYPAD_BUFFER_EMPTY(cKey6))       /* While unread events are in the buffer */
        {
            TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey6);  /* Read the buffer and store the event in the u8Event variable */
            if(u8ElecNum & 0x80)               /* If the event is a release */
            {
                _LCD_R9_OFF;                   /* Turn off the central rotary segment of the LCD */
            }
            else                               /* If the event is a touch */
            {
              u8DemoNum = u8DemoNum ++;        /* Increment the demo number */
              if (u8DemoNum > MAX_DEMO_NUM)    /* If the demo number has reach the upper limit */
              {
                  u8DemoNum = 1;               /* Set the lowest demo number */
              }
               _LCD_R9_ON;                     /* Turn on the central rotary segment of the LCD */
              vfnBUZZ();                       /* Activate the buzzer for 50ms */
            }
            
        }
    }
    if(gu8Status&0x20)                         /* If the event has ocurred in the DIMENSION Electrodes control */
    {   
        gu8Status&=0xDF;                             /* Clear the Dimension control event flag */
        
        while (!TSS_KEYPAD_BUFFER_EMPTY(cKey5))      /* While unread events are in the buffer */
        {
            TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey5);  /* Read the buffer and store the event in the u8Event variable */
            if(u8ElecNum & 0x80)                      /* If the event is a release */ 
            {  
                u8ElecNum = u8ElecNum & 0x0F;         /* Store number of the event electrode */
                if(u8ElecNum==0)                      /* If the Open/Close button is touched */
                {
                    _LCD_S4_OFF;                      /* Turn off the Open button segment of the LCD */
                    if(u8DemoNum == SHADOW_DEMO)      /* If the demo selected is Shadow demo */
                    {
                        gu8State = SHADOW_DEMO*2;     /* Go to the Shadow demo config STATE */
                        fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);  /* Send the Turn off all LED to Comm MCU */
                        fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);  /* Send the Turn off all LED to Comm MCU */
                        fnWriteDataTxBuffer((uint8_t)SHADOW_DEMO);   /* Send the Turn on Shadow demo LED to Comm MCU */
                            
                    }
                    else if(u8DemoNum==ALARM_CLOCK_DEMO)           /* If the demo selected is Shadow demo */
                    {
                        gu8State = ALARM_CLOCK_DEMO*2;             /* Go to the Clock demo config STATE */
                        fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);  /* Send the Turn off all LED to Comm MCU */
                        fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);  /* Send the Turn off all LED to Comm MCU */
                        fnWriteDataTxBuffer((uint8_t)ALARM_CLOCK_DEMO); /* Send the Turn on Clock demo LED to Comm MCU */
                    }
                    else if(u8DemoNum==MICROWAVE_DEMO)             /* If the demo selected is Shadow demo */
                    {
                        gu8State = MICROWAVE_DEMO*2;               /* Go to the Clock demo config STATE */
                        gu8MCVEon = 1;                             /* Set the On/Off flag */
                        gs8Volume = 8;                             
                        fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);  /* Send the Turn off all LED to Comm MCU */
                        fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);  /* Send the Turn off all LED to Comm MCU */
                        fnWriteDataTxBuffer((uint8_t)MICROWAVE_DEMO); /* Send the Turn on Microwave demo LED to Comm MCU */
                    } 
                    else if(u8DemoNum==WASHING_MACHINE_DEMO)       /* If the demo selected is Shadow demo */
                    {                                              
                        gu8State = WASHING_MACHINE_DEMO*2;         /* Go to the Clock demo config STATE */
                        gu8Washf.fOnOff = 1;                       /* Set the On/Off flag */ 
                        fnWriteDataTxBuffer((uint8_t)TURN_OFF_ALL);   /* Send the Turn off all LED to Comm MCU */
                        fnWriteDataTxBuffer((uint8_t)WASHING_MACHINE_DEMO); /* Send the Turn on Washing Machine demo LED */
                        fnWriteDataTxBuffer((uint8_t)OVERLAY_CONFIG);       /* Send the Turn on Overlay config LED */
                    } 
                    else
                    {
                    }
                }
          }
          else
          {
          
          }
       }
    }
    if(gu8Status&0x80)                                              /* If no valid control event */
    {
      asm BGND;                                                     /* Hardware Breakpoint */
    }
  }
  vfnWriteDemoSelection();                                          /* Display in the LCD the current selected Demo */
 
}

/****************************************************************************************************
* vfnWriteDemoSelection
* ---------------------------------------------------------------------------------------------------
* Display in the LCD the current selected Demo
*****************************************************************************************************/

void vfnWriteDemoSelection(void)
{
    
        if (u8DemoNum == SHADOW_DEMO)
        {
            _LCD_N1_ON;
            _LCD_N2_OFF;
            _LCD_N3_OFF;
            _LCD_N4_OFF;
            vfnLCD_Write_Msg ("DEMO");
        } 
        else if (u8DemoNum == ALARM_CLOCK_DEMO)
        {
            _LCD_N2_ON;
            _LCD_N1_OFF;
            _LCD_N3_OFF;
            _LCD_N4_OFF;
           vfnLCD_Write_Msg ("CLCK");
        }
        else if (u8DemoNum == MICROWAVE_DEMO)
        {  
            _LCD_N3_ON;
            _LCD_N2_OFF;
            _LCD_N1_OFF;
            _LCD_N4_OFF;
            vfnLCD_Write_Msg ("OVEN");
            gu8MCVEopen = 0;
     
        } 
        else if (u8DemoNum == WASHING_MACHINE_DEMO)
        {  
            _LCD_N3_OFF;
            _LCD_N2_OFF;
            _LCD_N1_OFF;
            _LCD_N4_ON;
            vfnLCD_Write_Msg ("WASH");
     
        } 
        else
        {
        }
}
