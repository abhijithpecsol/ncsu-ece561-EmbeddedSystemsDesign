/*HEADER********************************************************************
*
* Copyright (c) 2013 Freescale Semiconductor;
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
* Comments:
*
*   This file contains callbacks definitions.
*
*END************************************************************************/
#include "TSS_API.h"
#include "module_id.h"
#include "main.h"
#include "app_init.h"

#include "events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

extern uint16_t u16ProximityElectrodeTimeout;
uint8_t PeriodicTimerFlag = 0u;

/**
 * \brief Callback definition for Control 0.
 *
 *  This event is enabled only if Control 0 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack0(TSS_CONTROL_ID u8ControlId)
{
    /* Write your code here ... */
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

/**
 * \brief Callback definition for Control 1.
 *
 *  This event is enabled only if Control 1 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack1(TSS_CONTROL_ID u8ControlId)
{
    /* Write your code here ... */
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

/**
 * \brief Callback definition for Control 2.
 *
 *  This event is enabled only if Control 2 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack2(TSS_CONTROL_ID u8ControlId)
{
    /* Write your code here ... */
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

/**
 * \brief Callback definition for Control 3.
 *
 *  This event is enabled only if Control 3 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
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

/**
 * \brief Callback definition for Control 4.
 *
 *  This event is enabled only if Control 4 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack4(TSS_CONTROL_ID u8ControlId)
{
  if ((cASlider4.Position % 4) == 0x00) {LED1_ON; LED2_OFF; LED3_OFF; LED4_OFF;}
  if ((cASlider4.Position % 4) == 0x01) {LED1_OFF; LED2_ON; LED3_OFF; LED4_OFF;}
  if ((cASlider4.Position % 4) == 0x02) {LED1_OFF; LED2_OFF; LED3_ON; LED4_OFF;}
  if ((cASlider4.Position % 4) == 0x03) {LED1_OFF; LED2_OFF; LED3_OFF; LED4_ON;}

  (void) u8ControlId;

  return;
}

/**
 * \brief Callback definition for Control 5.
 *
 *  This event is enabled only if Control 5 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack5(TSS_CONTROL_ID u8ControlId)
{
    /* Write your code here ... */
    uint8_t u8Event; /* 8 bits local variable used to store the event information */

    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey5))  /* While unread events are in the buffer */
    {
      TSS_KEYPAD_BUFFER_READ(u8Event,cKey5); /* Read the buffer and store the event in the u8Event variable */

      if (u8Event & 0x80)
      {                                      /* If was a release event */
        u8Event = (uint8_t) (u8Event & 0x0F);  /* Remove the release flag */

        if (u8Event == 0x00) LED1_OFF;
        if (u8Event == 0x01) LED2_OFF;
        if (u8Event == 0x02) LED3_OFF;
      }
      else
      {                                      /* If was a touch event */
        if (u8Event == 0x00) LED1_ON;
        if (u8Event == 0x01) LED2_ON;
        if (u8Event == 0x02) LED3_ON;
      }
    }

  (void) u8ControlId;

  return;
}

/**
 * \brief Callback definition for Control 6.
 *
 *  This event is enabled only if Control 6 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack6(TSS_CONTROL_ID u8ControlId)
{
  (void) u8ControlId;

  if ((cARotary6.Position % 4) == 0x00) {LED1_ON; LED2_OFF; LED3_OFF; LED4_OFF;}
  if ((cARotary6.Position % 4) == 0x01) {LED1_OFF; LED2_ON; LED3_OFF; LED4_OFF;}
  if ((cARotary6.Position % 4) == 0x02) {LED1_OFF; LED2_OFF; LED3_ON; LED4_OFF;}
  if ((cARotary6.Position % 4) == 0x03) {LED1_OFF; LED2_OFF; LED3_OFF; LED4_ON;}

  return;
}

/**
 * \brief Callback definition for Control 7.
 *
 *  This event is enabled only if Control 7 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack7(TSS_CONTROL_ID u8ControlId)
{
    /* Write your code here ... */
    uint8_t u8Event; /* 8 bits local variable used to store the event information */

    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey7))  /* While unread events are in the buffer */
    {
      TSS_KEYPAD_BUFFER_READ(u8Event,cKey7); /* Read the buffer and store the event in the u8Event variable */

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

/**
 * \brief Callback definition for Control 8.
 *
 *  This event is enabled only if Control 8 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack8(TSS_CONTROL_ID u8ControlId)
{
    /* Write your code here ... */
    uint8_t u8Event; /* 8 bits local variable used to store the event information */

    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey8))  /* While unread events are in the buffer */
    {
      TSS_KEYPAD_BUFFER_READ(u8Event,cKey8); /* Read the buffer and store the event in the u8Event variable */

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

/**
 * \brief Callback definition for Control 9.
 *
 *  This event is enabled only if Control 9 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack9(TSS_CONTROL_ID u8ControlId)
{
    if ((cMatrix9.PositionY % 4) == 0x00) {LED1_ON; LED2_OFF; LED3_OFF; LED4_OFF;}
    if ((cMatrix9.PositionY % 4) == 0x01) {LED1_OFF; LED2_ON; LED3_OFF; LED4_OFF;}
    if ((cMatrix9.PositionY % 4) == 0x02) {LED1_OFF; LED2_OFF; LED3_ON; LED4_OFF;}
    if ((cMatrix9.PositionY % 4) == 0x03) {LED1_OFF; LED2_OFF; LED3_OFF; LED4_ON;}

    (void) u8ControlId;

    return;
}

/**
 * \brief Callback definition for Control 10.
 *
 *  This event is enabled only if Control 10 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack10(TSS_CONTROL_ID u8ControlId)
{
    /* Write your code here ... */
    uint8_t u8Event; /* 8 bits local variable used to store the event information */
    uint16_t u16ControlElectrodeStatus;

    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey10))  /* While unread events are in the buffer */
    {
      TSS_KEYPAD_BUFFER_READ(u8Event,cKey10); /* Read the buffer and store the event in the u8Event variable */

      if (u8Event & 0x80)
      {                                      /* If was a release event */
          u8Event = (uint8_t) (u8Event & 0x0F);  /* Remove the release flag */

          if (u8Event == 0x01) LED1_OFF;
          if (u8Event == 0x03) LED2_OFF;
          if (u8Event == 0x05) LED3_OFF;
          /* Indicates Water Presence*/
          if ((u8Event == 0x00) || (u8Event == 0x02) || (u8Event == 0x04))
          {
            u16ControlElectrodeStatus = TSS_GetControlElectrodesStatus(u8ControlId.ControlNumber);

            if ((u16ControlElectrodeStatus & 0x15) == 0u)
            {
        	  LED4_OFF;
            }
          }
      }
      else
      {                                      /* If was a touch event */
		  if (u8Event == 0x01) LED1_ON;
		  if (u8Event == 0x03) LED2_ON;
		  if (u8Event == 0x05) LED3_ON;
		  /* Indicates Water Presence*/
		  if ((u8Event == 0x00) || (u8Event == 0x02) || (u8Event == 0x04)) LED4_ON;
      }

    }

  (void) u8ControlId;

  return;
}

/**
 * \brief Callback definition for Control 11.
 *
 *  This event is enabled only if Control 11 is enabled.
 *
 * \param u8ControlId Valid unique Identifier of the Control which
 *                    generated the CallBack function. This Id can be
 *                    used for finding of Callback's source Control.
 *
 * \return void
 */
void TSS1_fCallBack11(TSS_CONTROL_ID u8ControlId)
{
    /* Write your code here ... */
    uint8_t u8Event; /* 8 bits local variable used to store the event information */

    while (!TSS_KEYPAD_BUFFER_EMPTY(cKey11))  /* While unread events are in the buffer */
    {
      TSS_KEYPAD_BUFFER_READ(u8Event,cKey11); /* Read the buffer and store the event in the u8Event variable */

      if (u8Event & 0x80)
      {                                      /* If was a release event */
          u8Event = (uint8_t) (u8Event & 0x0F);  /* Remove the release flag */

          if (u8Event == 0x00) LED1_OFF;
          if (u8Event == 0x01) LED2_OFF;
          if (u8Event == 0x02) LED3_OFF;
          /* Indicates Water Presence*/
          if (u8Event == 0x03) LED4_OFF;
      }
      else
      {                                      /* If was a touch event */
		  if (u8Event == 0x00) LED1_ON;
		  if (u8Event == 0x01) LED2_ON;
		  if (u8Event == 0x02) LED3_ON;
		  /* Indicates Water Presence*/
		  if (u8Event == 0x03) LED4_ON;
      }

    }

  (void) u8ControlId;

  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack12 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 11. This event is enabled
**         only if Control 11 is enabled.
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
void TSS1_fCallBack12(TSS_CONTROL_ID u8ControlId)
{
  uint8_t u8Event; /* 8 bits local variable used to store the event information */

  while (!TSS_KEYPAD_BUFFER_EMPTY(cKey12))  /* While unread events are in the buffer */
  {
    TSS_KEYPAD_BUFFER_READ(u8Event,cKey12); /* Read the buffer and store the event in the u8Event variable */

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
      /* Reset Proximity Electrode Timeout if any touch happen */
      if (u8Event & 0x03)
      {
        u16ProximityElectrodeTimeout = APP_TWRPI_PROXIMITY_TIMEOUT;
      }
    }
  }

  (void) u8ControlId;
  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fOnProximity (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         This callback function is called by TSS if Proximity event
**         is detected. This event is disabled in default and depends
**         on selection 'generate code' if the proximity function is
**         enabled and used in TSS.
**         The default CallBack Name is automatically generated with
**         automatic prefix update by current Component Name. User can
**         define own name, but then the automatic name update is not
**         functional.
**         The option is available from TSS3.0 version.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void TSS_fOnProximity(void)
{
  #if APP_TSS_USE_DCTRACKER
    if (MODULE_ID_Get_Device(MODULE_A) == TWRPI_PROXIMITY)
    {
      u16ProximityElectrodeTimeout = APP_TWRPI_PROXIMITY_TIMEOUT;
      /* Enables the TSS. Enables the DC Tracking feature. Disables Proximity */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
      /* Set proximity sensitivity */
      (void) TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x30);
      /* Normal Mode Enter Indication */
      DoLedBlinking(500u, 1u, TRUE);
    }
  #else
    if (MODULE_ID_Get_Device(MODULE_A) == TWRPI_PROXIMITY)
    {
      u16ProximityElectrodeTimeout = APP_TWRPI_PROXIMITY_TIMEOUT;
      /* Enables the TSS. Disables Proximity */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK));
      /* Set proximity sensitivity */
      TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x30);
      /* Normal Mode Enter Indication */
      DoLedBlinking(500u, 1u, TRUE);
    }
  #endif
}

/**
 * \brief On Fault callback definition.
 *
 *  This callback function is called by TSS after Fault occurence.
 *
 * \param u8FaultElecNum Electrode name which caused a generation of on
 *                       fault callback or TSS_FAULT_UNDEFINED_ELECTRODE.
 *
 * \return void
 */
void TSS_fOnFault(uint8_t u8FaultElecNum)
{
    TSS_RTCStop();

    if(tss_CSSys.Faults.ChargeTimeout || tss_CSSys.Faults.SmallCapacitor) /* If large or small capacitor fault  */
    {
      (void) TSS_SetSystemConfig(System_Faults_Register, 0x00);           /* Clear the fault flag */
    }

    if(tss_CSSys.Faults.SmallTriggerPeriod)
    {
      (void) TSS_SetSystemConfig(System_Faults_Register, 0x00);           /* Clear the fault flag */

#if APP_TSS_USE_DCTRACKER
      /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64 */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
#else
      /* Enables the TSS */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK ));
#endif
    }

    (void) u8FaultElecNum;
    /* Enable Interrupts globally */
    EnableInterrupts;
    TSS_RTCInit();
}

/**
 * \brief On Init callback definition.
 *
 *  This callback function is called during TSS_Init function execution.
 *  It is recommended to enable clocks for used electrodes (TSI/HWTimer).
 *
 * \param void
 *
 * \return void
 */
void TSS_fOnInit(void)
{

  SCG_C4_TSI = 1u;  /* Enable TSI clock */

}

/**
 * \brief Module changed callback
 *
 *  This callback function is invoked if a module in TWRPI has changed.
 *
 * \param pModule_id_twrpi_modules Pointer to the TWRPI modules array.
 *
 * \param module Number of module which has changed
 *
 * \return void
 */
void MODULE_ID_ChangedCB(TWRPI_DEVICES *pModule_id_twrpi_modules, MODULES module)
{
  if (module == MODULE_A)
  {
    TSS_Init_TWRPI_Board(pModule_id_twrpi_modules[MODULE_A]);

    LED1_OFF;
    LED2_OFF;
    LED3_OFF;
    LED4_OFF;
  }
}

/**************************************************************************//*!
* PeriodicTimer_Control
* --------------------------------------------------------------------------
* Callback's function called by periodic timer
******************************************************************************/
void PeriodicTimer_Callback(void)
{
  #if (APP_TSS_USE_TRIGGER_MODE == SW)
    if ((TSS_GetSystemConfig(System_SystemTrigger_Register) & TSS_TRIGGER_MODE_MASK) == TSS_TRIGGER_MODE_SW)
    {
      /* Toggle TSS SW trigger if SW trigger mode is enabled */
      (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_SWTRIGGER_MASK | TSS_TRIGGER_MODE_SW);
    }
  #endif
  /* Set Periodic Timer Flag */
  PeriodicTimerFlag = 1u;
}

/**************************************************************************//*!
* DelayMS_Callback
* --------------------------------------------------------------------------
* Callback's function called during delay
******************************************************************************/
void DelayMS_Callback(void)
{
  (void) TSS_Task();
}
