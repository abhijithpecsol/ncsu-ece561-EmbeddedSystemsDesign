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

  #define TSS_USE_NOISE_MODE                   0

  #define TSS_USE_NOISE_SIGNAL_LOG             0
#if (TSS_USE_KEYDETECTOR_VERSION == 2)
  #define TSS_USE_AFID_FAST_FILTER_RATIO           25  /* Ratio is fsample/fcutoff */

  #define TSS_USE_AFID_SLOW_FILTER_RATIO           100 /* Ratio is fsample/fcutoff */
#endif
  #define TSS_USE_AUTOTRIGGER_SOURCE           LPTMR

  /* Electrode's configuration */

  #define TSS_N_ELECTRODES                     33         /* Number of electrodes present in the system */

  /* Electrode's GPIOs configuration */

  /* Electrode Types definition */

  /* Standard  - Divider 2 */
  #define TSS_E0_TYPE                  TSI0_CH0
  #define TSS_E1_TYPE                  TSI0_CH1
  #define TSS_E2_TYPE                  TSI0_CH3
  #define TSS_E3_TYPE                  TSI0_CH4
  #define TSS_E4_TYPE                  TSI0_CH6
  #define TSS_E5_TYPE                  TSI0_CH7
  #define TSS_E6_TYPE                  TSI0_CH8
  #define TSS_E7_TYPE                  TSI0_CH9
  #define TSS_E8_TYPE                  TSI0_CH10
  #define TSS_E9_TYPE                  TSI0_CH11
  #define TSS_E10_TYPE                 TSI0_CH12
  #define TSS_E11_TYPE                 TSI0_CH13
  /* Standard - used for Matrix */
  #define TSS_E12_TYPE                 TSI0_CH0
  #define TSS_E13_TYPE                 TSI0_CH1
  #define TSS_E14_TYPE                 TSI0_CH3
  #define TSS_E15_TYPE                 TSI0_CH4
  #define TSS_E16_TYPE                 TSI0_CH6
  #define TSS_E17_TYPE                 TSI0_CH7
  #define TSS_E18_TYPE                 TSI0_CH8
  #define TSS_E19_TYPE                 TSI0_CH9
  #define TSS_E20_TYPE                 TSI0_CH10
  #define TSS_E21_TYPE                 TSI0_CH11
  #define TSS_E22_TYPE                 TSI0_CH12
  #define TSS_E23_TYPE                 TSI0_CH13
  /* Shield 1 */
  #define TSS_E24_TYPE                 TSI0_CH0
  #define TSS_E25_TYPE                 TSI0_CH9
  #define TSS_E26_TYPE                 TSI0_CH10
  #define TSS_E27_TYPE                 TSI0_CH11
  #define TSS_E28_TYPE                 TSI0_CH12
  #define TSS_E29_TYPE                 TSI0_CH13
  /* Shield 2 */
  #define TSS_E30_TYPE                 TSI0_CH9
  #define TSS_E31_TYPE                 TSI0_CH10
  #define TSS_E32_TYPE                 TSI0_CH11

  /* Shield configuration */

  /* Shield1 Configuration */
  #define TSS_E26_SHIELD_ELECTRODE   25
  #define TSS_E28_SHIELD_ELECTRODE   27
  #define TSS_E24_SHIELD_ELECTRODE   29
  /* Shield2 Configuration */
  #define TSS_E30_SHIELD_ELECTRODE   10
  #define TSS_E31_SHIELD_ELECTRODE   10
  #define TSS_E32_SHIELD_ELECTRODE   10

  /* Electrode Signal Divider */
  #define TSS_E0_SIGNAL_DIVIDER        2
  #define TSS_E1_SIGNAL_DIVIDER        2
  #define TSS_E2_SIGNAL_DIVIDER        2
  #define TSS_E3_SIGNAL_DIVIDER        2
  #define TSS_E4_SIGNAL_DIVIDER        2
  #define TSS_E5_SIGNAL_DIVIDER        2
  #define TSS_E6_SIGNAL_DIVIDER        2
  #define TSS_E7_SIGNAL_DIVIDER        2
  #define TSS_E8_SIGNAL_DIVIDER        2
  #define TSS_E9_SIGNAL_DIVIDER        2
  #define TSS_E10_SIGNAL_DIVIDER       2
  #define TSS_E11_SIGNAL_DIVIDER       2
  #define TSS_E24_SIGNAL_DIVIDER       2
  #define TSS_E25_SIGNAL_MULTIPLIER    2
  #define TSS_E26_SIGNAL_DIVIDER       2
  #define TSS_E27_SIGNAL_MULTIPLIER    2
  #define TSS_E28_SIGNAL_DIVIDER       2
  #define TSS_E29_SIGNAL_MULTIPLIER    2
  #define TSS_E30_SIGNAL_DIVIDER       2
  #define TSS_E31_SIGNAL_DIVIDER       2
  #define TSS_E32_SIGNAL_DIVIDER       2

  /* Controls configuration */

  #define TSS_N_CONTROLS              13

  #define TSS_C0_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C0_INPUTS               {7,8}
  #define TSS_C0_STRUCTURE            cKey0             /* Name of the C&S struct to create */
  #define TSS_C0_CALLBACK             TSS1_fCallBack0   /* Identifier of the user's callback */

  #define TSS_C1_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C1_INPUTS               {7,8,9,10,11,0,4,5,6,1,3,2}
  #define TSS_C1_STRUCTURE            cKey1             /* Name of the C&S struct to create */
  #define TSS_C1_CALLBACK             TSS1_fCallBack1   /* Identifier of the user's callback */

  #define TSS_C2_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C2_INPUTS               {11} /* Center Pad */
  #define TSS_C2_STRUCTURE            cKey2             /* Name of the C&S struct to create */
  #define TSS_C2_CALLBACK             TSS1_fCallBack2   /* Identifier of the user's callback */

  #define TSS_C3_TYPE                 TSS_CT_ROTARY     /* Control type */
  #define TSS_C3_INPUTS               {10,5,8,4,9,0,7,6}
  #define TSS_C3_STRUCTURE            cRotary3          /* Name of the C&S struct to create */
  #define TSS_C3_CALLBACK             TSS1_fCallBack3   /* Identifier of the user's callback */

  #define TSS_C4_TYPE                 TSS_CT_ASLIDER    /* Control type */
  #define TSS_C4_INPUTS               {8,7}                 /* Number of electrodes in the control */
  #define TSS_C4_STRUCTURE            cASlider4         /* Name of the C&S struct to create */
  #define TSS_C4_CALLBACK             TSS1_fCallBack4   /* Identifier of the user's callback */

  #define TSS_C5_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C5_INPUTS               {9,10,11} /* SW1-3 */
  #define TSS_C5_STRUCTURE            cKey5             /* Name of the C&S struct to create */
  #define TSS_C5_CALLBACK             TSS1_fCallBack5   /* Identifier of the user's callback */

  #define TSS_C6_TYPE                 TSS_CT_AROTARY    /* Control type */
  #define TSS_C6_INPUTS               {7,8,9,10}
  #define TSS_C6_STRUCTURE            cARotary6         /* Name of the C&S struct to create */
  #define TSS_C6_CALLBACK             TSS1_fCallBack6   /* Identifier of the user's callback */

  #define TSS_C7_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C7_INPUTS               {11} /* Center Pad */
  #define TSS_C7_STRUCTURE            cKey7             /* Name of the C&S struct to create */
  #define TSS_C7_CALLBACK             TSS1_fCallBack7   /* Identifier of the user's callback */

  #define TSS_C8_TYPE                 TSS_CT_KEYPAD     /* Control type */
  #define TSS_C8_INPUTS               {7,8,9,10,11,0}
  #define TSS_C8_KEYS                 {0x05,0x09,0x0A,0x14,0x18,0x0C,0x12,0x30,0x24,0x03,0x21,0x22} /* Group keys */
  #define TSS_C8_STRUCTURE            cKey8             /* Name of the C&S struct to create */
  #define TSS_C8_CALLBACK             TSS1_fCallBack8   /* Identifier of the user's callback */

  #define TSS_C9_TYPE                 TSS_CT_MATRIX     /* Control type */
  #define TSS_C9_INPUTS               {16,17,18,13,15,14,19,20,21,22,23,12}
  #define TSS_C9_INPUTS_NUM_X         6
  #define TSS_C9_STRUCTURE            cMatrix9          /* Name of the C&S struct to create */
  #define TSS_C9_CALLBACK             TSS1_fCallBack9   /* Identifier of the user's callback */

  #define TSS_C10_TYPE                TSS_CT_KEYPAD     /* Control type */
  #define TSS_C10_INPUTS              {25,26,27,28,29,24}
  #define TSS_C10_STRUCTURE           cKey10            /* Name of the C&S struct to create */
  #define TSS_C10_CALLBACK            TSS1_fCallBack10  /* Identifier of the user's callback */

  #define TSS_C11_TYPE                TSS_CT_KEYPAD     /* Control type */
  #define TSS_C11_INPUTS              {30,31,32,10}
  #define TSS_C11_STRUCTURE           cKey11            /* Name of the C&S struct to create */
  #define TSS_C11_CALLBACK            TSS1_fCallBack11  /* Identifier of the user's callback */

  #define TSS_C12_TYPE                TSS_CT_KEYPAD     /* Control type */
  #define TSS_C12_INPUTS              {11,9,7,8,22}
  #define TSS_C12_STRUCTURE           cKey12            /* Name of the C&S struct to create */
  #define TSS_C12_CALLBACK            TSS1_fCallBack12  /* Identifier of the user's callback */

  /* TSI Specific parameter */

  /* TSI Autocalibration Settings */
  #define TSS_TSI_RESOLUTION               12 /* Required Bit resolution of the TSI */
  #define TSS_TSI_EXTCHRG_LOW_LIMIT        0  /* Low Limit of External OSC Charge Current register value for TSI autocalibration */
  #define TSS_TSI_EXTCHRG_HIGH_LIMIT       7 /* High Limit of External OSC Charge Current register value for TSI autocalibration */
  #define TSS_TSI_PS_LOW_LIMIT             0  /* Low Limit of External OSC Prescaler register value for TSI autocalibration */
  #define TSS_TSI_PS_HIGH_LIMIT            7  /* High Limit of External OSC Prescaler regiters value for TSI autocalibration */
  /* Proximity configuration */
  #define TSS_TSI_PROX_EXTCHRG_LOW_LIMIT   4  /* Low Limit of External OSC Charge Current register value for TSI Proximity autocalibration */
  #define TSS_TSI_PROX_EXTCHRG_HIGH_LIMIT  4  /* High Limit of External OSC Charge Current register value for TSI Proximity autocalibration */
  #define TSS_TSI_PROX_PS_LOW_LIMIT        6  /* Low Limit of External OSC Prescaler register value for TSI Proximity autocalibration */
  #define TSS_TSI_PROX_PS_HIGH_LIMIT       6  /* High Limit of External OSC Prescaler regiters value for TSI Proximity autocalibration */

#endif /* TSS_SYSTEM_SETUP_H */
