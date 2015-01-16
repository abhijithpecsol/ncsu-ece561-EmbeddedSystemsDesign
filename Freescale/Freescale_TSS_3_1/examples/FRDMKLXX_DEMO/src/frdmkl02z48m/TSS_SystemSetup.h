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
* Comments:
*
*   This file contains electrode and other Touch Sensing System setup parameters
*   which are specific for each board
*
*END************************************************************************/
#ifndef TSS_SYSTEM_SETUP_H
#define TSS_SYSTEM_SETUP_H

  #include "TSS_SystemSetupGlobal.h"

  /* TSS Features Configuration */

  #define TSS_USE_AUTOTRIGGER_SOURCE             UNUSED

  #define TSS_USE_LOWPOWER_CONTROL_SOURCE        UNUSED

  /* Electrode Types definition */

  #define TSS_E0_TYPE                 GPIO
  #define TSS_E1_TYPE                 GPIO

  #define TSS_E0_P                    A
  #define TSS_E0_B                    13
  #define TSS_E1_P                    B
  #define TSS_E1_B                    12

  /* Signal Multiplier & Divider */

  #define TSS_E0_SIGNAL_DIVIDER       2
  #define TSS_E1_SIGNAL_DIVIDER       2

  /* Controls configuration */

  #define TSS_N_CONTROLS              1

  #define TSS_C0_TYPE                 TSS_CT_ASLIDER    /* Control type */
  #define TSS_C0_INPUTS               {0,1}
  #define TSS_C0_STRUCTURE            cASlider1         /* Name of the C&S struct to create */
  #define TSS_C0_CALLBACK             TSS1_fCallBack1   /* Identifier of the user's callback */

  /* Timer Specific Parameters */

  #define TSS_HW_TIMER                           TPM0
  #define TSS_SENSOR_PRESCALER                   2
  #define TSS_SENSOR_TIMEOUT                     1024

  /* TSI Specific parameter */

  /* TSI Autocalibration Settings */
  #define TSS_TSI_RESOLUTION          6 /* Required Bit resolution of the TSI */
  #define TSS_TSI_EXTCHRG_LOW_LIMIT   0  /* Low Limit of External OSC Charge Current register value for TSI autocalibration */
  #define TSS_TSI_EXTCHRG_HIGH_LIMIT  7 /* High Limit of External OSC Charge Current register value for TSI autocalibration */
  #define TSS_TSI_PS_LOW_LIMIT        0  /* Low Limit of External OSC Prescaler register value for TSI autocalibration */
  #define TSS_TSI_PS_HIGH_LIMIT       7  /* High Limit of External OSC Prescaler regiters value for TSI autocalibration */

#endif
