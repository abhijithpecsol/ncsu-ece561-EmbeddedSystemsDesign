/**HEADER********************************************************************
*
* Copyright (c) 2012 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: events.c$
* $Version : 0.5.26.0$
* $Date    : Nov-7-2012$
*
* Comments:
*
*   This is user's event module code.
*
*END************************************************************************/

#include "TSS_API.h"
#include "module_id.h"
#include "main.h"
#include "board.h"
#include "app_init.h"
#include "events.h"

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
  uint8_t u8Event; /* 8 bits local variable used to store the event information */

  while (!TSS_KEYPAD_BUFFER_EMPTY(cKey0))  /* While unread events are in the buffer */
  {
    TSS_KEYPAD_BUFFER_READ(u8Event,cKey0); /* Read the buffer and store the event in the u8Event variable */

    if (u8Event & 0x80)
    {                                      /* If was a release event */
      u8Event = (uint8_t) (u8Event & 0x0F);  /* Remove the release flag */

      if (u8Event == 0x00) LED1_OFF;
      if (u8Event == 0x01) LED2_OFF;
      if (u8Event == 0x02) LED3_OFF;
      if (u8Event == 0x03) LED4_OFF;
    }
    else
    {                                      /* If was a touch event */
      if (u8Event == 0x00) LED1_ON;
      if (u8Event == 0x01) LED2_ON;
      if (u8Event == 0x02) LED3_ON;
      if (u8Event == 0x03) LED4_ON;
    }
  }

  (void) u8ControlId;

  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack1 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 1. This event is enabled
**         only if Control 1 is enabled.
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
void TSS1_fCallBack1(TSS_CONTROL_ID u8ControlId)
{
  uint8_t u8Event; /* 8 bits local variable used to store the event information */

  while (!TSS_KEYPAD_BUFFER_EMPTY(cKey1))  /* While unread events are in the buffer */
  {
    TSS_KEYPAD_BUFFER_READ(u8Event,cKey1); /* Read the buffer and store the event in the u8Event variable */

    if (u8Event & 0x80)
    {                                      /* If was a release event */
      /* Do Nothing */
    }
    else
    {                                      /* If was a touch event */
      /* Turn Off all LED's */
      LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF;
      /* Binary indicate the last Touched pad */
      if ((u8Event + 1u) & 0x01) LED1_ON;
      if ((u8Event + 1u) & 0x02) LED2_ON;
      if ((u8Event + 1u) & 0x04) LED3_ON;
      if ((u8Event + 1u) & 0x08) LED4_ON;
    }

  }

  (void) u8ControlId;

  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack2 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 1. This event is enabled
**         only if Control 2 is enabled.
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
void TSS1_fCallBack2(TSS_CONTROL_ID u8ControlId)
{
  uint8_t u8Event; /* 8 bits local variable used to store the event information */

  while (!TSS_KEYPAD_BUFFER_EMPTY(cKey2))  /* While unread events are in the buffer */
  {
    TSS_KEYPAD_BUFFER_READ(u8Event,cKey2); /* Read the buffer and store the event in the u8Event variable */

    if (u8Event & 0x80)
    {                                      /* If was a release event */
      u8Event = (uint8_t) (u8Event & 0x0F);  /* Remove the release flag */

      if (u8Event == 0x00) LED1_OFF;
    }
    else
    {                                      /* If was a touch event */
      if (u8Event == 0x00) LED1_ON;
    }
  }

  (void) u8ControlId;

  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack3 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 1. This event is enabled
**         only if Control 3 is enabled.
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
void TSS1_fCallBack3(TSS_CONTROL_ID u8ControlId)
{
  static uint8_t u8LEDCounter = 0u;

  if (cRotary3.DynamicStatus.Direction)
  {
  if (u8LEDCounter < 3) u8LEDCounter++; else u8LEDCounter = 0u;
  } else {
  if (u8LEDCounter > 0) u8LEDCounter--; else u8LEDCounter = 3u;
  }

  if ((u8LEDCounter & 0x03) == 0x00) {LED1_ON; LED2_OFF; LED3_OFF; LED4_OFF;}
  if ((u8LEDCounter & 0x03) == 0x01) {LED1_OFF; LED2_ON; LED3_OFF; LED4_OFF;}
  if ((u8LEDCounter & 0x03) == 0x02) {LED1_OFF; LED2_OFF; LED3_ON; LED4_OFF;}
  if ((u8LEDCounter & 0x03) == 0x03) {LED1_OFF; LED2_OFF; LED3_OFF; LED4_ON;}

  (void) u8ControlId;

  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack4 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 8. This event is enabled
**         only if Control 8 is enabled.
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
void TSS1_fCallBack4(TSS_CONTROL_ID u8ControlId)
{
  uint8_t u8Event; /* 8 bits local variable used to store the event information */

  while (!TSS_KEYPAD_BUFFER_EMPTY(cKey4))  /* While unread events are in the buffer */
  {
    TSS_KEYPAD_BUFFER_READ(u8Event,cKey4); /* Read the buffer and store the event in the u8Event variable */

    if (u8Event & 0x80)
    {                                      /* If was a release event */
      /* Binary indicate the last Touched pad */
      if ((u8Event + 1u) & 0x01) LED1_OFF;
      if ((u8Event + 1u) & 0x02) LED2_OFF;
      if ((u8Event + 1u) & 0x04) LED3_OFF;
      if ((u8Event + 1u) & 0x08) LED4_OFF;
    }
    else
    {                                      /* If was a touch event */
      /* Binary indicate the last Touched pad */
      if ((u8Event + 1u) & 0x01) LED1_ON;
      if ((u8Event + 1u) & 0x02) LED2_ON;
      if ((u8Event + 1u) & 0x04) LED3_ON;
      if ((u8Event + 1u) & 0x08) LED4_ON;
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
**     Parameters  : uint8_t
**     Returns     : Nothing
** ===================================================================
*/
void TSS_fOnFault(uint8_t electrode_number)
{
  if(tss_CSSys.Faults.ChargeTimeout || tss_CSSys.Faults.SmallCapacitor) /* If large or small capacitor fault  */
  {
    (void) TSS_SetSystemConfig(System_Faults_Register, 0x00);           /* Clear the fault flag */
    /*  */
  }

  if(tss_CSSys.Faults.SmallTriggerPeriod)
  {
    (void) TSS_SetSystemConfig(System_Faults_Register, 0x00);           /* Clear the fault flag */

    /* Enables the TSS. Enables the DC Tracking feature. */
    (void)TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
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
  /* Call platform specific initialization */
  InitDevices();
}

/**************************************************************************//*!
* MODULE_ID_ChangedCB callback
* --------------------------------------------------------------------------
* Callback's function which manages Daughter Board replacing
******************************************************************************/
void MODULE_ID_ChangedCB(TWRPI_DEVICES *pModule_id_twrpi_modules, MODULES module)
{
  if (module == MODULE_A)
  {
    TSS_Init_TWRPI_Board(pModule_id_twrpi_modules[MODULE_A]);
    /* Turn Off all LED's */
    LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF;
  }
}

/**************************************************************************//*!
* DelayMS_Callback
* --------------------------------------------------------------------------
* Callback's function called during delay
******************************************************************************/
void DelayMS_Callback(void)
{
  TSS_Task();
}

/* END Events */
