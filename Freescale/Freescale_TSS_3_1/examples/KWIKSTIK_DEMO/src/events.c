/** ###################################################################
**     Filename  : Events.c
**     Project   : ProcessorExpert
**     Processor : MKXX
**     Component : Events
**     Version   : Driver 01.00
**     Compiler  : CodeWarrior ARM C Compiler
**     Date/Time : 2011-02-10, 13:20, # CodeGen: 0
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**
** ###################################################################*/

#include "TSS_API.h"
#include "main.h"
#include "app_init.h"
#include "uart.h"
#include "events.h"
#include "Driver_SLCD.h"
#include "Driver_Buzzer.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

/*
** ===================================================================
**     Event       :  TSS1_fCallBack0 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 1. This event is enabled
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

    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey0))  /* While unread events are in the buffer */
    {
        TSS_KEYPAD_BUFFER_READ(u8Event, cKey0); /* Read the buffer and store the event in the u8Event variable */

        if (u8Event & 0x80)
        {                                      /* If was a release event */
        u8Event = (uint8_t) (u8Event & 0x0F);  /* Remove the release flag */

        switch (u8Event)
        {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                _SLCDModule_ClearLCD(0);
                break;
            default: break;
        }
      }
      else
      {                                      /* If was a touch event */
        switch (u8Event)
        {
            case 0:
                _SLCDModule_PrintString("BUT 1", 0);    /* print on SLCD which button was touched */
                _Buzzer_On(100);
                break;
            case 1:
                _SLCDModule_PrintString("BUT 2", 0);
                _Buzzer_On(100);
                break;
            case 2:
                _SLCDModule_PrintString("BUT 3", 0);
                _Buzzer_On(100);
                break;
            case 3:
                _SLCDModule_PrintString("BUT 4", 0);
                _Buzzer_On(100);
                break;
            case 4:
                _SLCDModule_PrintString("BUT 5", 0);
                _Buzzer_On(100);
                break;
            case 5:
                _SLCDModule_PrintString("BUT 6", 0);
                _Buzzer_On(100);
                break;
            default: break;
        }
      }

    }
  (void) u8ControlId;
  return;
}



/*
** ===================================================================
**     Event       :  TSS1_fOnFault (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         This callback function is called by TSS after Fault
**         occurence. This event is enabled always and depends on
**         selection 'generate code' if the callback is used.
**         The default CallBack Name is automatically generated with
**         automatic prefix update by current Component Name. User can
**         define own name, but then the automatic name update is not
**         functional.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TSS_fOnFault(void)
{
    if(tss_CSSys.Faults.ChargeTimeout || tss_CSSys.Faults.SmallCapacitor) /* If large or small capacitor fault  */
    {
        (void) TSS_SetSystemConfig(System_Faults_Register, 0x00);           /* Clear the fault flag */
        (void)TSS_SetSystemConfig(System_ElectrodeEnablers_Register + 0u, 0x3F);
    }
    if(tss_CSSys.Faults.SmallTriggerPeriod)
    {
      (void) TSS_SetSystemConfig(System_Faults_Register, 0x00);           /* Clear the fault flag */

      #if TSS_USE_DCTRACKER
        /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64 */
        (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
      #else
        /* Enables the TSS */
        (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK));
      #endif
    }

}

/*
** ===================================================================
**     Event       :  TSS1_fOnInit (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         This callback function is called after whole TSS Init. This
**         event is enabled always and depends on selection 'generate
**         code' if the callback is used.
**         The default CallBack Name is automatically generated with
**         automatic prefix update by current Component Name. User can
**         define own name, but then the automatic name update is not
**         functional.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TSS_fOnInit(void)
{
    SIM_MemMapPtr sim = SIM_BASE_PTR;

    sim->SCGC5 |= SIM_SCGC5_TSI_MASK;   /* TSI clock enablement */
    sim->SCGC6 |= SIM_SCGC6_FTM1_MASK;

    /* Set Electrodes for TSI function */
    PORTB_PCR0 = PORT_PCR_MUX(0);
    PORTA_PCR4 = PORT_PCR_MUX(0);
}

/* END Events */

/*
** ###################################################################
**
**     This file was created by Processor Expert 5.00 [04.48]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
