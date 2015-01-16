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

/****************************** Prototypes *********************************/

/****************************************************************************************************
* TSS_Init_TWRPI_board
* ---------------------------------------------------------------------------------------------------
* Inits TSS for selected TWRPI board
*****************************************************************************************************/
void TSS_Init_TWRPI_Board(TWRPI_DEVICES eTWRPI_module)
{
  #if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
    uint8_t u8temp;
  #endif

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
      (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 8u);
    }
  #endif
  /* Auto Trigger Config */
  (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_TRIGGER_MODE_ALWAYS);

  /* Set DC-Tracker Rates */
  (void) TSS_SetSystemConfig(System_DCTrackerRate_Register, 0x64);
  (void) TSS_SetSystemConfig(System_SlowDCTrackerFactor_Register, 0x0F);
  /* Increase Response Time for higher immunity against noise */
  (void) TSS_SetSystemConfig(System_ResponseTime_Register, 16u);

  /* Disable all controls */

  /* Configure Control 1 */
  (void) TSS_SetKeypadConfig(cKey0.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 2 */
  (void) TSS_SetKeypadConfig(cKey1.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 3 */
  (void) TSS_SetKeypadConfig(cKey2.ControlId,Keypad_ControlConfig_Register, 0x00);
  /* Configure Control 4 */
  (void) TSS_SetRotaryConfig(cRotary3.ControlId,Rotary_ControlConfig_Register,0x00);
  /* Configure Control 9 */
  (void) TSS_SetKeypadConfig(cKey4.ControlId,Keypad_ControlConfig_Register, 0x00);

  /* Set Control specific settings */
  if ((eTWRPI_module == TWRPI_UNPLUGGED) || (eTWRPI_module == TWRPI_UNKNOWN))
  {
    /* Set Sensitivity */
    #if (TSS_USE_AUTO_SENS_CALIBRATION == 0)
      /* Sets the Sensitivity value for each electrode */
      for (u8temp = 0; u8temp < TSS_N_ELECTRODES; u8temp++) {
        (void) TSS_SetSystemConfig(System_Sensitivity_Register + u8temp, 0xE);
      }
    #endif
    /* Configure Control 1 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey0.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey0.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
  /* TWRPI-KEYPAD board init */
  if (eTWRPI_module == TWRPI_KEYPAD)
  {
    /* Configure Control 2 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey1.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey1.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }
  /* TWRPI-ROTARY board init */
  if (eTWRPI_module == TWRPI_ROTARY)
  {
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
  /* TWRPI-KEYPAD2 board init */
  if (eTWRPI_module == TWRPI_GKEYPAD)
  {
    /* Configure Control 9 */
    /* Configure the TSS Keypad Control to report the touch and release events */
    (void) TSS_SetKeypadConfig(cKey4.ControlId,Keypad_Events_Register,(TSS_KEYPAD_TOUCH_EVENT_EN_MASK | TSS_KEYPAD_RELEASE_EVENT_EN_MASK));
    /* Enables Callback function. Enables the control */
    (void) TSS_SetKeypadConfig(cKey4.ControlId,Keypad_ControlConfig_Register,(TSS_KEYPAD_CALLBACK_EN_MASK | TSS_KEYPAD_CONTROL_EN_MASK));
  }

  /* Enables the TSS. Enables the DC Tracking feature. */
  (void) TSS_SetSystemConfig(System_SystemConfig_Register,(TSS_SYSTEM_EN_MASK | TSS_DC_TRACKER_EN_MASK));
  
  /* Board Specific Electrode Settings */
  #ifdef APP_BOARD_SPECIFIC_INIT
    APP_BOARD_SPECIFIC_INIT();
  #endif
  
}
