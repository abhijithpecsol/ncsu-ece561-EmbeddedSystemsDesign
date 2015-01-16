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
* $FileName: TSS_SystemSetup.h$
* $Version : 0.1.54.0$
* $Date    : Sep-21-2012$
*
* Comments:
*
*   TSS configuration file.
*
*END************************************************************************/
#ifndef TSS_SYSTEM_SETUP_H
  #define TSS_SYSTEM_SETUP_H

  /* TSS Features Configuration */

  #include "TSS_SystemSetupGlobal.h"

  #define TSS_USE_AFID_FAST_FILTER_RATIO  25  /* Ratio is fsample/fcutoff */

  #define TSS_USE_AFID_SLOW_FILTER_RATIO  100 /* Ratio is fsample/fcutoff */

  /* Electrode's configuration */

  #define TSS_N_ELECTRODES            12         /* Number of electrodes present in the system */

  /* Electrode Types definition */

  #define TSS_E0_TYPE                 TSI0_IN0
  #define TSS_E1_TYPE                 TSI0_IN1
#if (TSS_USE_FREEMASTER_GUI == 1)
  #define TSS_E2_TYPE                 TSI0_IN0  /* Conflict with UART0 */
  #define TSS_E3_TYPE                 TSI0_IN1  /* Conflict with UART0 */
#else
  #define TSS_E2_TYPE                 TSI0_IN2
  #define TSS_E3_TYPE                 TSI0_IN3
#endif
  #define TSS_E4_TYPE                 TSI0_IN4
  #define TSS_E5_TYPE                 TSI0_IN5
  #define TSS_E6_TYPE                 TSI0_IN6
  #define TSS_E7_TYPE                 TSI0_IN7
  #define TSS_E8_TYPE                 TSI0_IN8
  #define TSS_E9_TYPE                 TSI0_IN9
  #define TSS_E10_TYPE                TSI0_IN10
  #define TSS_E11_TYPE                TSI0_IN11

  /* Controls configuration */

  #define TSS_N_CONTROLS              5

  #define TSS_C0_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C0_INPUTS               {1,0}
  #define TSS_C0_STRUCTURE            cKey0             /* Name of the C&S struct to create */
  #define TSS_C0_CALLBACK             TSS1_fCallBack0   /* Identifier of the user's callback */

  #define TSS_C1_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C1_INPUTS               {0,1,2,3,4,5,6,7,8,9,10,11}
  #define TSS_C1_STRUCTURE            cKey1             /* Name of the C&S struct to create */
  #define TSS_C1_CALLBACK             TSS1_fCallBack1   /* Identifier of the user's callback */

  #define TSS_C2_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C2_INPUTS               {4} /* Center Pad */
  #define TSS_C2_STRUCTURE            cKey2             /* Name of the C&S struct to create */
  #define TSS_C2_CALLBACK             TSS1_fCallBack2   /* Identifier of the user's callback */

  #define TSS_C3_TYPE                 TSS_CT_ROTARY     /* Control type */
  #define TSS_C3_INPUTS               {3,7,1,6,2,5,0,8}
  #define TSS_C3_STRUCTURE            cRotary3          /* Name of the C&S struct to create */
  #define TSS_C3_CALLBACK             TSS1_fCallBack3   /* Identifier of the user's callback */

  #define TSS_C4_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C4_INPUTS               {0,1,2,3,4,5}
  #define TSS_C4_KEYS                 {0x05,0x09,0x0A,0x14,0x18,0x0C,0x12,0x30,0x24,0x03,0x21,0x22} /* Group keys */
  #define TSS_C4_STRUCTURE            cKey4             /* Name of the C&S struct to create */
  #define TSS_C4_CALLBACK             TSS1_fCallBack4   /* Identifier of the user's callback */

  /* TSI Specific parameter */

  #define TSS_TSI_RESOLUTION          11 /* Required Bit resolution of the TSI */
  #define TSS_TSI_EXTCHRG_LOW_LIMIT   0  /* Low Limit of External OSC Charge Current register value for TSI autocalibration */
  #define TSS_TSI_EXTCHRG_HIGH_LIMIT  7  /* High Limit of External OSC Charge Current register value for TSI autocalibration */
  #define TSS_TSI_PS_LOW_LIMIT        0  /* Low Limit of External OSC Prescaler register value for TSI autocalibration */
  #define TSS_TSI_PS_HIGH_LIMIT       7  /* High Limit of External OSC Prescaler regiters value for TSI autocalibration */

#endif /* TSS_SYSTEM_SETUP_H */
