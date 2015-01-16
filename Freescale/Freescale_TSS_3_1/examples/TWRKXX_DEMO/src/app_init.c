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
* IN ANY WAY OUT OF THE USE OF THShile1
IS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: app_init.c$
* $Version : 3.6.39.0$
* $Date    : Nov-14-2012$
*
* Comments:
*
*   This file contains the init functions.
*
*END************************************************************************/

#include "TSS_API.h"
#include "board.h"
#include "main.h"
#include "app_init.h"

/***************************** Local Types *********************************/

uint16_t u16ProximityElectrodeTimeout = APP_TWRPI_PROXIMITY_TIMEOUT;
extern uint8_t PeriodicTimerFlag;

/****************************** Prototypes *********************************/

/****************************************************************************************************
* TSS_Init_TWRPI_board
* ---------------------------------------------------------------------------------------------------
* Inits TSS for selected TWRPI board
*****************************************************************************************************/
void TSS_Init_TWRPI_Board(TWRPI_DEVICES eTWRPI_module)
{
  uint8_t u8temp;

  /* Application Periodic Timer Stop */
  APP_PERIODIC_TIMER_STOP();

  /* Delay for Signal Stabilization */
  DelayMS(300, FALSE);

  /* Initializes the TSS */
  (void) TSS_Init();

  /* Set Number of Samples */
  (void) TSS_SetSystemConfig(System_NSamples_Register, 0x08);

  /* Sefault Sensitivty definition */
  #if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
    /* Sets the Sensitivity value for each electrode */
    for (u8temp = 0; u8temp < TSS_N_ELECTRODES; u8temp++) {
      (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x1A);
    }
  #endif
  /* Auto Trigger Config */
  #if (APP_TSS_USE_TRIGGER_MODE == AUTO)
    (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_AUTO);   /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
  #elif (APP_TSS_USE_TRIGGER_MODE == ALWAYS)
    (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_ALWAYS);   /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
  #elif (APP_TSS_USE_TRIGGER_MODE == SW)
    (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_SW);   /* TSS_TRIGGER_MODE_AUTO/TSS_TRIGGER_MODE_ALWAYS/TSS_TRIGGER_MODE_SW */
  #endif

  /* Set DC-Tracker Rates */
  (void) TSS_SetSystemConfig(System_DCTrackerRate_Register, 0x64);
  (void) TSS_SetSystemConfig(System_SlowDCTrackerFactor_Register, 0x0F);

  /* Disable all controls */

  /* Configure Control 1 */
  (void) TSS_SetKeypadConfig(cKey0.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 2 */
  (void) TSS_SetKeypadConfig(cKey1.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 3 */
  (void) TSS_SetKeypadConfig(cKey2.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 4 */
  (void) TSS_SetRotaryConfig(cRotary3.ControlId,Rotary_ControlConfig_Register,0x00);
  /* Configure Control 5 */
  (void) TSS_SetASliderConfig(cASlider4.ControlId,ASlider_ControlConfig_Register,0x00);
  /* Configure Control 6 */
  (void) TSS_SetKeypadConfig(cKey5.ControlId,Keypad_ControlConfig_Register,0x00);
  /* Configure Control 7 */
  (void) TSS_SetASliderConfig(cARotary6.ControlId,ARotary_ControlConfig_Register, 0x00);
  /* Configure Control 8 */
  (void) TSS_SetKeypadConfig(cKey7.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 9 */
  (void) TSS_SetKeypadConfig(cKey8.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 10 */
  (void) TSS_SetMatrixConfig(cMatrix9.ControlId,Matrix_ControlConfig_Register,0x00);
  /* Configure Control 11 */
  (void) TSS_SetKeypadConfig(cKey10.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 12 */
  (void) TSS_SetKeypadConfig(cKey11.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 13 */
  (void) TSS_SetKeypadConfig(cKey12.ControlId,Keypad_ControlConfig_Register, 0x00);

  /* Set Control specific settings */
  if ((eTWRPI_module == TWRPI_UNPLUGGED) || (eTWRPI_module == TWRPI_UNKNOWN))
  {
    /* Set Sensitivity */
    #if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
      /* Sets the Sensitivity value for each electrode */
      for (u8temp = 0; u8temp < TSS_N_ELECTRODES; u8temp++) {
        (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x40);
      }
    #endif
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey0.ControlId.ControlNumber, 0u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x40);
    /* Configure Control 1 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey0.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey0.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
  /* TWRPI-KEYPAD board init */
  if (eTWRPI_module == TWRPI_KEYPAD)
  {
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey1.ControlId.ControlNumber, 2u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
    /* Configure Control 2 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey1.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey1.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
  /* TWRPI-ROTARY board init */
  if (eTWRPI_module == TWRPI_ROTARY)
  {
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey2.ControlId.ControlNumber, 0u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
    /* Configure Control 3 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey2.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey2.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
    /* Configure Control 4 */
    /* Configure the TSS to report the touch, release and movement events */
    (void) TSS_SetRotaryConfig(cRotary3.ControlId,Rotary_Events_Register,TSS_ROTARY_MOVEMENT_EVENT_EN_MASK);
    /* Enables Callback function. Enables the control */
    (void) TSS_SetRotaryConfig(cRotary3.ControlId,Rotary_ControlConfig_Register,(TSS_ROTARY_CONTROL_EN_MASK | TSS_ROTARY_CALLBACK_EN_MASK));
  }
  /* TWRPI-SLIDER2 board init */
  if (eTWRPI_module == TWRPI_SLIDER)
  {
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey5.ControlId.ControlNumber, 2u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
    /* Configure Control 5 */
    /* Configure the range */
    (void) TSS_SetASliderConfig(cASlider4.ControlId,ASlider_Range_Register, 4);
    /* Configure the TSS to report the touch, release and movement events */
    (void) TSS_SetASliderConfig(cASlider4.ControlId,ASlider_Events_Register,(TSS_ASLIDER_MOVEMENT_EVENT_EN_MASK | TSS_ASLIDER_INITIAL_TOUCH_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetASliderConfig(cASlider4.ControlId,ASlider_ControlConfig_Register,(TSS_ASLIDER_CONTROL_EN_MASK|TSS_ASLIDER_CALLBACK_EN_MASK));
    /* Configure Control 6 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey5.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey5.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
  /* TWRPI-ROTARY3 board init */
  if (eTWRPI_module == TWRPI_ROTARY2)
  {
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey7.ControlId.ControlNumber, 0u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
    /* Configure Control 7 */
    /* Configure the range */
    (void) TSS_SetASliderConfig(cARotary6.ControlId,ARotary_Range_Register, 32);
    /* Configure the TSS to report the touch, release and movement events */
    (void) TSS_SetASliderConfig(cARotary6.ControlId,ARotary_Events_Register,(TSS_AROTARY_MOVEMENT_EVENT_EN_MASK | TSS_AROTARY_INITIAL_TOUCH_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetASliderConfig(cARotary6.ControlId,ARotary_ControlConfig_Register,(TSS_AROTARY_CONTROL_EN_MASK|TSS_AROTARY_CALLBACK_EN_MASK));
    /* Configure Control 8 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey7.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey7.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
  /* TWRPI-KEYPAD2 board init */
  if (eTWRPI_module == TWRPI_GKEYPAD)
  {
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey8.ControlId.ControlNumber, 4u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
    /* Configure Control 9 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey8.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey8.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
  /* TWRPI-TOUCHPAD board init */
  if (eTWRPI_module == TWRPI_TOUCHPAD)
  {
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cMatrix9.ControlId.ControlNumber, 9u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
    /* Configure Control 10 */
    /* Configure the range */
    (void) TSS_SetMatrixConfig(cMatrix9.ControlId,Matrix_RangeX_Register,128);
    (void) TSS_SetMatrixConfig(cMatrix9.ControlId,Matrix_RangeY_Register,128);
    /* Configure the TSS to report the touch, release and movement events */
    (void) TSS_SetMatrixConfig(cMatrix9.ControlId,Matrix_Events_Register,(TSS_MATRIX_GESTURES_EVENT_EN_MASK | TSS_MATRIX_MOVEMENT_EVENT_EN_MASK | TSS_MATRIX_INITIAL_TOUCH_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetMatrixConfig(cMatrix9.ControlId,Matrix_ControlConfig_Register,(TSS_MATRIX_CONTROL_EN_MASK | TSS_MATRIX_CALLBACK_EN_MASK));
  }
#if (TSS_USE_KEYDETECTOR_VERSION == 1)
  /* TWRPI-SHIELD1 board init */
  if (eTWRPI_module == TWRPI_SHIELD1)
  {
    /* Set Sensitivity */
    #if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
      /* Sets the Sensitivity value for each electrode */
      for (u8temp = 0; u8temp < TSS_N_ELECTRODES; u8temp++) {
        (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x55);
      }
    #endif
    /* Shielding 1 electrodes sensitivity for Water function */
    /* Shielding Electrodes Sensitivity */
    u8temp = TSS_GetControlElectrodeIndex(cKey10.ControlId.ControlNumber, 0u);
    (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x7D);
    u8temp = TSS_GetControlElectrodeIndex(cKey10.ControlId.ControlNumber, 2u);
    (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x7D);
    u8temp = TSS_GetControlElectrodeIndex(cKey10.ControlId.ControlNumber, 4u);
    (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x7D);
    /* Shielded Electrodes Initial Sensitivity */
    u8temp = TSS_GetControlElectrodeIndex(cKey10.ControlId.ControlNumber, 1u);
    (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x18);
    u8temp = TSS_GetControlElectrodeIndex(cKey10.ControlId.ControlNumber, 3u);
    (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x18);
    u8temp = TSS_GetControlElectrodeIndex(cKey10.ControlId.ControlNumber, 5u);
    (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0x18);
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey10.ControlId.ControlNumber, 1u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x55);
    /* Configure Control 11 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey10.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey10.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
  /* TWRPI-SHIELD2 board init */
  if (eTWRPI_module == TWRPI_SHIELD2)
  {
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey11.ControlId.ControlNumber, 2u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x1A);
    /* Configure Control 12 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey11.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey11.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
#endif
  /* TWRPI-PROXIMITY board init */
  if (eTWRPI_module == TWRPI_PROXIMITY)
  {
    /* Low Power Config */
    u8temp = TSS_GetControlElectrodeIndex(cKey12.ControlId.ControlNumber, 4u);
    TSS_SetSystemConfig(System_LowPowerElectrode_Register, u8temp);
    TSS_SetSystemConfig(System_LowPowerElectrodeSensitivity_Register, 0x30);
    /* Configure Control 13 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey12.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey12.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }

  #if APP_TSS_USE_DCTRACKER
    if (eTWRPI_module == TWRPI_SHIELD1)
    {
      /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64. Enables Water Tolerance */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK | TSS_WATER_TOLERANCE_EN_MASK));
    }
    else if (eTWRPI_module == TWRPI_PROXIMITY)
    {
      /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64. Enables Proximity */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK | TSS_PROXIMITY_EN_MASK));
      /* Proximity Enter Indication */
      DoLedBlinking(50u, 5u, FALSE);
    }
    else
    {
      /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64 */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
    }
  #else
    if (eTWRPI_module == TWRPI_SHIELD1)
    {
      /* Enables the TSS and Water Tolerance */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_WATER_TOLERANCE_EN_MASK));
    }
    else if (eTWRPI_module == TWRPI_PROXIMITY)
    {
      /* Enables the TSS. Enables Proximity */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_PROXIMITY_EN_MASK));
      /* Proximity Enter Indication */
      DoLedBlinking(50u, 5u, FALSE);
    }
    else
    {
      /* Enables the TSS */
      (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK));
    }
  #endif

  /* Board Specific Electrode Settings */
  #ifdef APP_BOARD_SPECIFIC_INIT
    APP_BOARD_SPECIFIC_INIT();
  #endif

  /* Application Periodic Timer Start */
  APP_PERIODIC_TIMER_INIT();

}

/**************************************************************************//*!
* PeriodicTimerControl
* --------------------------------------------------------------------------
* Control function periodic timer
******************************************************************************/
void PeriodicTimerControl(void)
{
  if (PeriodicTimerFlag)
  {
  #if APP_TSS_USE_DCTRACKER
    if (MODULE_ID_Get_Device(MODULE_A) == TWRPI_PROXIMITY)
    {
      if (u16ProximityElectrodeTimeout == 0u)
      {
        if ((TSS_GetSystemConfig(System_SystemConfig_Register) & TSS_PROXIMITY_EN_MASK) != TSS_PROXIMITY_EN_MASK)
        {
          /* Enables the TSS. Enables the DC Tracking feature. Default DC Tracking value is 0x64. Enables Proximity */
          (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK | TSS_PROXIMITY_EN_MASK));
          /* Proximity Enter Indication */
          DoLedBlinking(50u, 5u, TRUE);
        }
      }
      else
      {
        u16ProximityElectrodeTimeout--;
      }
    }
  #else
    if (MODULE_ID_Get_Device(MODULE_A) == TWRPI_PROXIMITY)
    {
      if (u16ProximityElectrodeTimeout == 0u)
      {
        if ((TSS_GetSystemConfig(System_SystemConfig_Register) & TSS_PROXIMITY_EN_MASK) != TSS_PROXIMITY_EN_MASK)
        {
          /* Enables the TSS. Enables Proximity */
          (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_PROXIMITY_EN_MASK));
          /* Proximity Enter Indication */
          DoLedBlinking(50u, 5u, TRUE);
        }
      }
      else
      {
        u16ProximityElectrodeTimeout--;
      }
  #endif
      PeriodicTimerFlag = 0u;
    }
}

/**************************************************************************//*!
* DoLedBlinking
* --------------------------------------------------------------------------
* Perform blinking of all LEDs in defined period
******************************************************************************/
void DoLedBlinking(uint16_t u16Period, uint8_t u8Loops, uint8_t u8TSSEnabler)
{
  uint8_t u8temp;
  /* Indication */
  for (u8temp = 0u; u8temp < u8Loops; u8temp++)
  {
    LED1_ON;LED2_ON;LED3_ON;LED4_ON;
    DelayMS(u16Period, u8TSSEnabler);
    LED1_OFF; LED2_OFF; LED3_OFF; LED4_OFF;
    DelayMS(u16Period, u8TSSEnabler);
  }
}

