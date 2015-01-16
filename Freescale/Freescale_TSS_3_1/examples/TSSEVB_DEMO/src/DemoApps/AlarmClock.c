/***************************************************************************************
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
* --------------------------------------------------------------------------------------
*                                                                                                         
* @file   AlarmClock.c
*
* @brief  Alarm Clock Demo Application
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

int8_t gs8Volume;                         /* Slider control variable (volume) */
uint8_t showTime;                         /* Display Clock flag */
uint8_t ALARMSET,EDITTIME, POWER;
uint8_t Keylock;                          /* Lock function flag */
struct time ALARM;                      /* Alarm Structure */
extern struct time RTC;                 /* Clock structure */
extern struct time ROT;                 /* Edit time structure */
extern uint8_t Message1[];                /* Characters array */
                                                                                  
/****************************************************************************************************
* vfnAlarmClockDemoConfigure
* ---------------------------------------------------------------------------------------------------
* Configures the alarm clock demo
*****************************************************************************************************/

void vfnAlarmClockDemoConfigure(void)
{
    uint8_t lcv;
    /* Enables all the electrodes used in the AlarmClock demo application */
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0xFF);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x0F);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
    (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x06);
    vfnSensitivityConfig();             /* Configures de sensitivity values for each electrode */
    vfnLCD_Clear_Display ();            /* Clear the LCD */
    _LCD_COL1_ON;
    vfnLCD_Write_Msg (&Message1[0]);    
    _LCD_FREESCALE_ON;                  /* Turns off the N1 segment of the LCD */
    gu8State++;                         /* Set the Clock Demo STATE */
    gs8Volume = 4;                      /* Set the volume value to four */
    POWER = 1;                          /* Initializes the demo variables */
    showTime = 0;                       
    EDITTIME = 0;                       
    for(lcv=0;lcv<gs8Volume;lcv++)      /* Display the current volume value in the LCD */
    {  
        SymbolON(18,lcv);
    }
}

/****************************************************************************************************
* vfnAlarmClockDemo
* ---------------------------------------------------------------------------------------------------
* Alarm clock function
*****************************************************************************************************/

void vfnAlarmClockDemo(void)          
{

  if(gu8Status)                                     /* If an event has ocurred in one of the controls */
  {
    //SLIDER VOLUME                                 /* If the event has ocurred in the SLIDER control */
    if(gu8Status&0x01)
    {
        uint8_t lcv;

        gu8Status &= 0xFE;                          /* Clear the Slider event flag */
        if (cSlider0.DynamicStatus.Direction)       /* If the direction is from left to right */
        {
            if(gs8Volume < 0x08)                    /* If volume has not reached the upper limit */
            {                                       /* Increment the volume variable the number of advanced positions */
                gs8Volume = gs8Volume + cSlider0.DynamicStatus.Displacement;
                if (gs8Volume > 0x08)               /* If volume has reached the upper limit */
                {    
                    gs8Volume = 0x08;               /* Keep volume the the maximum value */
                }
            }
        } 
        else                                        /* If the movement detected was from right to left  */
        {
            if (gs8Volume > 0)
            {   /* Decrement the volume variable the number of advanced positions */
                gs8Volume = gs8Volume - cSlider0.DynamicStatus.Displacement;
                if (gs8Volume < 0)                  /* If volume has reached the lower limit */
                {    
                    gs8Volume = 0;                  /* Keep volume in the minimum value */
                }
             }
        }        
        for(lcv=0;lcv<gs8Volume;lcv++)              /* Displays in the LCD the current volume value */
        {  
            SymbolON(18,lcv);
        }
        for(;lcv<0x08;lcv++)
        {  
            SymbolOFF(18,lcv);
        }
        vfnBUZZ();                                  /* Activates the buzzer for 50ms */

    }
    //ROTARY CLOCK
    if(gu8Status&0x02)                              /* If the event has ocurred in the ROTARY control */
    {
        uint8_t u8Position; /* 8 bits local variable used to store the event information */

        gu8Status &= 0xFD;                          /* Clear the Rotary event flag */
        
        u8Position = cRotary1.StaticStatus.Position; /* Store the current position of the rotary control */
        
        if (cRotary1.DynamicStatus.Movement)        /* If the event was movement */
        {
            //ROT = ALARM;
            /* Increments and Decrements the Clock and Alarm values acording to the displacement and 
               Direction of the rotary control  */
            RotaryToAsciiHours(cRotary1.DynamicStatus.Direction,cRotary1.DynamicStatus.Displacement);
            
            if(EDITTIME==1)                         /* If the Edit Clock is set */
            {
              RTC = ROT;                            /* Modified values are store in the Clock structure */
            }
            else if(EDITTIME==2)                    /* If the Edit Alarm is set */ 
            {
              ALARM = ROT;                          /* Modified values are store in the Alarm structure */
            }
            else
            {
              asm BGND;
            }
        }
        vfnBUZZ();                                  /* Activate buzzer for 50ms */
    }
    //LEDS SPECIAL FUNCTIONS
    if(gu8Status&0x04)                              /* If the event has ocurred in the LED ELECTRODES control */
    {
        uint8_t u8ElecNum;                            /* 8 bits local variable used to store the event information */
        
        gu8Status &= 0xFB;                          /* Clear the LED Electrodes event flag */
        while (!TSS_KEYPAD_BUFFER_EMPTY(cKey2))     /* While unread events are in the buffer */
        {
            TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey2); /* Read the buffer and store the event in the u8Event variable */
            if(u8ElecNum & 0x80)                     /* If the event was a release */
            {
              u8ElecNum = u8ElecNum & 0x0F;             
              if(u8ElecNum==0){}                    /* Do nothing */
              else if(u8ElecNum==1){}
              else if(u8ElecNum==2){}
              else if(u8ElecNum==3){}
            }
            else                                    /* If the event was a touch */
            {
                                                    /* If the LOCK button is touched */
              if(u8ElecNum==0)
              {                                         
                if(Keylock==0)                      /* If lock is disabled */
                {                                       
                  LED_ON(LED1);                     /* Turn the corresponding LED on */
                  Keylock = 1;                      /* Enable Lock */
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0x00); /* Disable electrodes */
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x01);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x06);
                }
                else                                /* If lock is enabled */
                {
                  LED_OFF(LED1);                    /* Turn the corresponding LED off */      
                  Keylock = 0;                      /* Disable Lock */
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0xFF); /* Reenable electrodes */
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x0D);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x06);
                }
              }                                     /* If the AM/PM button is touched */
              else if(u8ElecNum==1)
              {                                     /* If the application IS displaying the clock */
                if(showTime==0)
                {                                   /* If edit clock mode is enabled */
                  if(EDITTIME==1)
                  {
                    RTC.AMPM = ~RTC.AMPM;           /* Toggle the AM/PM variable of the clock structure */
                    ROT = RTC;                      /* store the change made into the edit structure */
                  }
                }
                else                                /* If the application is NOT displaying the clock */
                {
                  if(EDITTIME==2)                   /* If edit alarm mode is enabled */
                  {
                    ALARM.AMPM = ~ALARM.AMPM;       /* Toggle the AM/PM variable of the alarm structure */
                  }
                }
                
              }
              else if(u8ElecNum==2)                 /* If the TIMER button is touched */
              {
                LED_TOGGLE(LED3);                   /* Toggle the corresponding LED */
                
                if(EDITTIME==0)                     /* If Edit time is disable */
                { 
                    /* Enable rotary control */
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0xFF);
                  
                  if(showTime==0)                   /* If the application is NOT displaying the clock */
                  {
                    EDITTIME = 1;                   /* Enable edit clock mode */
                    ROT = RTC;                      /* Store the Clock information in the Time Edit structure (ROT) */
                  }
                  else                              /* If the application IS displaying  the clock */
                  {                                 
                    EDITTIME = 2;                   /* Enable edit alarm mode */  
                    ROT = ALARM;                    /* Store the alarm information in the Time Edit structure (ROT) */
                  }
                }
                else                                /* If the edit clock/alarm mode IS enable */
                {
                    /* Disable rotary control */
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                  LED_OFF(LED3);                    /* Turn off the Timer LED */
                  LED_OFF(LED4);                    /* Turn off the Alarm LED */
                  EDITTIME = 0;                     /* Disable edit alarm/clock mode */
                  showTime = 0;                     /* Set the application to display the clock */
                }
              }                                     /* If the alarm button is touched */
              else if(u8ElecNum==3)
              {                                     /* if edit alarm/clock is enabled */
                if(EDITTIME)
                {                                   /* If the alarm is NOT set */
                  if(ALARMSET==0)
                  {                                 /* Set the alarm */
                    ALARMSET=1;                     /* Turn on the alarm segment of the LCD */
                    _LCD_CLOCK_ON;
                    _LCD_CLOCK_ON;
                  }
                  else                              /* If the alarm IS set */
                  {
                    ALARMSET=0;                     /* Disable the alarm */
                    _LCD_CLOCK_OFF;                 /* Turn off the alarm segment of the LCD */
                  }
                }
                else                                /* if Edit alarm/clock is disabled */
                {
                  showTime = ~showTime;             /* Toggle the display clock flag */
                  LED_TOGGLE(LED4);                 /* Toggle alarm LED */
                }
              }
              vfnBUZZ();                            /* Activate buzzer for 50ms */
            }
        }
    }
    if(gu8Status&0x20)                              /* If the event has ocurred in the DIMENSION ELECTRODES control */
    {
        uint8_t u8ElecNum; /* 8 bits local variable used to store the event information */
        
        gu8Status&=0xDF;                            /* Clear the DIMENSION Electrodes event flag */
        while (!TSS_KEYPAD_BUFFER_EMPTY(cKey5))     /* While unread events are in the buffer */
        {
            TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey5);  /* Read the buffer and store the event in the u8Event variable */
            if(u8ElecNum & 0x80)                    /* If was a release event */
            {
              u8ElecNum = u8ElecNum & 0x0F;         /* Saves the electrode number inforamtion */
              if(u8ElecNum==0)                      /* If the SNOOZE buton is released */
              {
                //SNOOZE BUTTON RELEASE             /* Do nothing */
              }
            }
            else                                    /* If the event is a touch */
            {
              if(u8ElecNum==0)                      /* If the SNOOZE button is touched */
              {
                //SNOOZE BUTTON PRESS               /* Set the alarm */
                ALARMSET++;
              }
              else                                  /* If SNOOZE button is NOT touched */
              {
                //OFF
                if(POWER==1)                        /* If the Demo is turned on */
                {
                  uint8_t lcv;
                                                    /* Turn off all the LEDs */
                  LED_OFF(LED1);
                  LED_OFF(LED2);
                  LED_OFF(LED3);
                  LED_OFF(LED4);                   
                  POWER = 0;                        /* Set the Demo to turn off mode */
                  EDITTIME = 0;                     /* Disable edit clock/alarm mode */
                  Keylock = 0;                      /* Disable Lock function */
                  showTime = 0;                     /* Set the application to display the clock */
                  _LCD_FREESCALE_OFF;               /* Turn off the Freescale segment of the LCD */
                  /* Disable most of the electrodes */
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0x00);
                  for(lcv=0;lcv<8;lcv++)
                  {  
                      SymbolOFF(18,lcv);            /* Turn off the slider control segments of the LCD */
                  }
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x00);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x06);
                }
                else                                /* If the demo is turned off */
                {
                  uint8_t lcv;
                                                     
                  POWER = 1;                        /* Set the Demo to turn on mode */
                  _LCD_FREESCALE_ON;                /* Turn on the Freescale segment on the lcd */
                  for(lcv=0;lcv<gs8Volume;lcv++)
                  {  
                      SymbolON(18,lcv);             /* Turn on the slider control segments of the LCD */
                  }
                  /* Re-enable all the electrodes */
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0,0xFF);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+1,0x00);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+2,0x0D);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+3,0x40);
                  (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register+4,0x06);
                }
              }
              vfnBUZZ();                            /* Activate buzzer for 50ms */
            }
        }
    }
    if(gu8Status&0x40)                              /* If the event has ocurred in the central rotary button */ 
    {
        uint8_t u8ElecNum;
        
        gu8Status &= 0xBF;                          /* Clear the central rotary button event flag */
        while (!TSS_KEYPAD_BUFFER_EMPTY(cKey6))     /* While unread events are in the buffer */
        {
            TSS_KEYPAD_BUFFER_READ(u8ElecNum,cKey6); /* Read the buffer and store the event in the u8Event variable */
            if(u8ElecNum & 0x80)                     /* If the event is a release */
            {
                //CENTER BUTTON RELEASE             /* Do nothing */
            }
            else                                    /* If the event is a touch */
            {
                vfnBUZZ();                          /* Activate buzzer for 50ms */
                //CENTER BUTTON PRESS                
            }
            
        }
    }
    if(gu8Status&0x80)                              /* If no valid control event */
    {
      asm BGND;                                     /* Hardware breakpoint */
    }

  }                                               
  if(showTime==0)                                   /* If displaiyng the clock mode is enabled */
  {
    vfsLCD_Write_NUMS(RTC);                         /* Display the Clock time information */
    if(RTC.AMPM==0)                                 /* if the AM/PM flag is disabled */
    {
      _LCD_AM_ON;                                   /* Turn on the AM segment of the LCD */
      _LCD_PM_OFF;                                  /* Turn off the PM segment of the LCD */
    }
    else                                            /* if the AM/PM flag is enabled */
    {
      _LCD_AM_OFF;                                  /* Turn off the AM segment of the LCD */
      _LCD_PM_ON;                                   /* Turn on the PM segment of the LCD */
    }
  }
  else                                              /* If displaiyng the clock mode is disabled */
  {
    vfsLCD_Write_NUMS(ALARM);                       /* Display the Alarm time information */
    if(ALARM.AMPM==0)                               /* if the AM/PM flag is disabled */
    {
      _LCD_AM_ON;                                   /* Turn on the AM segment of the LCD */
      _LCD_PM_OFF;                                  /* Turn off the PM segment of the LCD */
    }
    else                                            /* if the AM/PM flag is enabled */
    {                                               
      _LCD_AM_OFF;                                  /* Turn off the AM segment of the LCD */
      _LCD_PM_ON;                                   /* Turn on the PM segment of the LCD */
    }
  }
  if((EDITTIME==0)&&(ALARMSET==1))                  /* If edit clock/alarm is disabled AND alarm is set */
  {
    if((RTC.MinuteMSB==ALARM.MinuteMSB) && (RTC.MinuteLSB==ALARM.MinuteLSB)) /* If the clock has reach the alarm time */
    {
      if((RTC.HourMSB==ALARM.HourMSB) && (RTC.HourLSB==ALARM.HourLSB))
      {
        if(RTC.AMPM==ALARM.AMPM)
        {
          ALARMSET=2;
          asm NOP;                                  
          asm BGND;                                 /* Hardware Breakpoint*/
          asm NOP;
        }
      }
    }
  }
}
