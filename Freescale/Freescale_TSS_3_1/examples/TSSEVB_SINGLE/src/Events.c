/** ###################################################################
**     Filename  : Events.C
**     Project   : TSSEVB_SINGLE
**     Processor : MC9S08LG32CLK
**     Component : Events
**     Version   : Driver 01.02
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 18.5.2010, 14:10
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         TSS1_fCallBack0 - void TSS1_fCallBack0(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack1 - void TSS1_fCallBack1(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack2 - void TSS1_fCallBack2(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack3 - void TSS1_fCallBack3(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack4 - void TSS1_fCallBack4(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack5 - void TSS1_fCallBack5(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack6 - void TSS1_fCallBack6(TSS_CONTROL_ID u8ControlId);
**
** ###################################################################*/
/* MODULE Events */

#include "Events.h"
#include "TSS_SystemSetup.h"
#include "derivative.h" /* include peripheral declarations */
#include "TSS_API.h"
#include "LCD.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

/****************************************************************************************************
* Variables
*****************************************************************************************************/

uint8_t gu8touchcount;    /* Variable for counting the times Electrode 1 (E1) is touched */

/*
** ===================================================================
**     Event       :  TSS1_fCallBack0 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 0. This event is enabled
**         only if Control 0 is enabled.
**         The default CallBack Name is automatically generated with
**         automatic prefix update by current Component Name. User can
**         define own name, but then the automatic name update is not
**         functional.
**     Parameters  :
**         NAME            - DESCRIPTION
**         u8ControlId     - Valid unique Identifier of
**                           the Control which generated the CallBack
**                           function. This Id can be used for finding
**                           of Callback's source Control.
**     Returns     : Nothing
** ===================================================================
*/
void TSS1_fCallBack0(TSS_CONTROL_ID u8ControlId)
{
  /* Write your code here ... */
      uint8_t u8Event; /* 8 bits local variable used to store the event information */
    
    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey0))      /* While unread events are in the buffer */
    {
        TSS_KEYPAD_BUFFER_READ(u8Event,cKey0);   /* Read the buffer and store the event in the u8Event variable */
        if (u8Event & 0x80) 
        {                                    /* If was a release event */
            _LCD_S4_OFF;                     /* Turn off the E1 segment */
        } 
        else
        {   
            _LCD_S4_ON;                     /*  If was a touch event */
            gu8touchcount++;                /* Increments the touch count by one */
        }
    }   
  (void) u8ControlId;
  return;
}

/* END TSS1 Events */

