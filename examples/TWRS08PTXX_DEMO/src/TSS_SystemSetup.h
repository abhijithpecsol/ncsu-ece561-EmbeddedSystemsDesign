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
*   GLOBAL (common for the project)
*
*END************************************************************************/
#ifndef TSS_SYSTEM_SETUP_H
#define TSS_SYSTEM_SETUP_H

/*********************** TSS Features Configuration **************************/

#define TSS_USE_SIMPLE_LOW_LEVEL               0   /* 0 = Full Background (Auto-Trigger), 1 = Consecutive (No Auto-Trigger) */

#define TSS_USE_DELTA_LOG                      1

#define TSS_USE_SIGNAL_LOG                     1

#define TSS_USE_GPIO_STRENGTH                  0

#define TSS_USE_GPIO_SLEW_RATE                 0

#define TSS_USE_DEFAULT_ELECTRODE_LEVEL_LOW    0

#define TSS_USE_IIR_FILTER                     1

#define TSS_USE_NOISE_AMPLITUDE_FILTER         0

#define TSS_USE_FREEMASTER_GUI                 1

#define TSS_USE_SIGNAL_SHIELDING               1

#define TSS_USE_SIGNAL_DIVIDER                 0

#define TSS_USE_SIGNAL_MULTIPLIER              1

#define TSS_USE_CONTROL_PRIVATE_DATA           0

#define TSS_USE_AUTO_SENS_CALIBRATION          1

#define TSS_USE_AUTO_SENS_CALIB_INIT_DURATION  100

#define TSS_USE_AUTO_SENS_CALIB_LOW_LIMIT      0

#define TSS_USE_BASELINE_INIT_DURATION         15

/* Function Source Definition */

#define TSS_USE_AUTOTRIGGER_SOURCE             RTC

#define TSS_USE_LOWPOWER_CONTROL_SOURCE        UNUSED

/* Code Size Reduction Options */

#define TSS_USE_DATA_CORRUPTION_CHECK          1

#define TSS_USE_TRIGGER_FUNCTION               1

#define TSS_USE_STUCK_KEY                      1

#define TSS_USE_NEGATIVE_BASELINE_DROP         1

#define TSS_USE_AUTO_HW_RECALIBRATION          1

/* Callback Definition */

#define TSS_ONFAULT_CALLBACK                   TSS_fOnFault /* Identifier of the user's OnFault callback */

#define TSS_ONINIT_CALLBACK                    TSS_fOnInit /* Identifier of the user's OnInit callback */

#define TSS_ONPROXIMITY_CALLBACK               TSS_fOnProximity /* Identifier of the user's OnProximity callback */

/* Debug Options */

#define TSS_ENABLE_DIAGNOSTIC_MESSAGES         0

/**************************** Electrode Configuration **************************/

#define TSS_N_ELECTRODES            37         /* Number of electrodes present in the system */

/* Electrode's GPIOs configuration */

/* Electrode Types definition */

  /* Standard */
#define TSS_E0_TYPE                 TSI_CH0
#define TSS_E1_TYPE                 TSI_CH1
#define TSS_E2_TYPE                 TSI_CH2
#define TSS_E3_TYPE                 TSI_CH3
#define TSS_E4_TYPE                 TSI_CH4
#define TSS_E5_TYPE                 TSI_CH5
#define TSS_E6_TYPE                 TSI_CH6
#define TSS_E7_TYPE                 TSI_CH7
#define TSS_E8_TYPE                 TSI_CH8
#define TSS_E9_TYPE                 TSI_CH9
#define TSS_E10_TYPE                TSI_CH10
#define TSS_E11_TYPE                TSI_CH11
#define TSS_E12_TYPE                TSI_CH12
#define TSS_E13_TYPE                TSI_CH13
#define TSS_E14_TYPE                TSI_CH14
#define TSS_E15_TYPE                TSI_CH15
/* Matrix Daughter Board Pads */
#define TSS_E16_TYPE                TSI_CH2
#define TSS_E17_TYPE                TSI_CH3
#define TSS_E18_TYPE                TSI_CH4
#define TSS_E19_TYPE                TSI_CH5
#define TSS_E20_TYPE                TSI_CH6
#define TSS_E21_TYPE                TSI_CH7
#define TSS_E22_TYPE                TSI_CH8
#define TSS_E23_TYPE                TSI_CH9
#define TSS_E24_TYPE                TSI_CH10
#define TSS_E25_TYPE                TSI_CH11
#define TSS_E26_TYPE                TSI_CH12
#define TSS_E27_TYPE                TSI_CH13
/* Shield 1 Daughter Board Pads */
#define TSS_E28_TYPE                TSI_CH2
#define TSS_E29_TYPE                TSI_CH3
#define TSS_E30_TYPE                TSI_CH4
#define TSS_E31_TYPE                TSI_CH5
#define TSS_E32_TYPE                TSI_CH6
#define TSS_E33_TYPE                TSI_CH7
/* Shield 2 Daughter Board Pads */
#define TSS_E34_TYPE                TSI_CH2
#define TSS_E35_TYPE                TSI_CH3
#define TSS_E36_TYPE                TSI_CH4

/* Shield Configuration */

/* Shield1 Configuration */
#define TSS_E29_SHIELD_ELECTRODE    28
#define TSS_E31_SHIELD_ELECTRODE    30
#define TSS_E33_SHIELD_ELECTRODE    32

/* Shield2 Configuration */
#define TSS_E34_SHIELD_ELECTRODE    5
#define TSS_E35_SHIELD_ELECTRODE    5
#define TSS_E36_SHIELD_ELECTRODE    5

/* Electrode Signal Divider & Multiplier */

#define TSS_E28_SIGNAL_MULTIPLIER    2
#define TSS_E30_SIGNAL_MULTIPLIER    2
#define TSS_E32_SIGNAL_MULTIPLIER    2
#define TSS_E37_SIGNAL_MULTIPLIER    2

/************************** Controls Configuration ***************************/

#define TSS_N_CONTROLS              13

#define TSS_C0_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C0_INPUTS               {0,1,14,15}       /* Control electrodes */
#define TSS_C0_STRUCTURE            cKey0             /* Name of the C&S struct to create */
#define TSS_C0_CALLBACK             TSS1_fCallBack0   /* Identifier of the user's callback */

#define TSS_C1_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C1_INPUTS               {2,3,4,5,6,7,8,9,10,11,12,13} /* Control electrodes */
#define TSS_C1_STRUCTURE            cKey1             /* Name of the C&S struct to create */
#define TSS_C1_CALLBACK             TSS1_fCallBack1   /* Identifier of the user's callback */

#define TSS_C2_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C2_INPUTS               {6}               /* Control electrodes */
#define TSS_C2_STRUCTURE            cKey2             /* Name of the C&S struct to create */
#define TSS_C2_CALLBACK             TSS1_fCallBack2   /* Identifier of the user's callback */

#define TSS_C3_TYPE                 TSS_CT_ROTARY     /* Control type */
#define TSS_C3_INPUTS               {6,5,9,3,8,4,7,2,10} /* Control electrodes */
#define TSS_C3_STRUCTURE            cRotary3          /* Name of the C&S struct to create */
#define TSS_C3_CALLBACK             TSS1_fCallBack3   /* Identifier of the user's callback */

#define TSS_C4_TYPE                 TSS_CT_ASLIDER    /* Control type */
#define TSS_C4_INPUTS               {3,2}
#define TSS_C4_STRUCTURE            cASlider4         /* Name of the C&S struct to create */
#define TSS_C4_CALLBACK             TSS1_fCallBack4   /* Identifier of the user's callback */

#define TSS_C5_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C5_INPUTS               {4,5,6}
#define TSS_C5_STRUCTURE            cKey5             /* Name of the C&S struct to create */
#define TSS_C5_CALLBACK             TSS1_fCallBack5   /* Identifier of the user's callback */

#define TSS_C6_TYPE                 TSS_CT_AROTARY    /* Control type */
#define TSS_C6_INPUTS               {2,3,4,5}
#define TSS_C6_STRUCTURE            cARotary6         /* Name of the C&S struct to create */
#define TSS_C6_CALLBACK             TSS1_fCallBack6   /* Identifier of the user's callback */

#define TSS_C7_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C7_INPUTS               {6}
#define TSS_C7_STRUCTURE            cKey7             /* Name of the C&S struct to create */
#define TSS_C7_CALLBACK             TSS1_fCallBack7   /* Identifier of the user's callback */

#define TSS_C8_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C8_INPUTS               {2,3,4,5,6,7}
#define TSS_C8_KEYS                 {0x05,0x09,0x0A,0x14,0x18,0x0C,0x12,0x30,0x24,0x03,0x21,0x22}
#define TSS_C8_STRUCTURE            cKey8             /* Name of the C&S struct to create */
#define TSS_C8_CALLBACK             TSS1_fCallBack8   /* Identifier of the user's callback */

#define TSS_C9_TYPE                 TSS_CT_MATRIX     /* Control type */
#define TSS_C9_INPUTS               {22,23,24,25,26,27,16,17,18,19,20,21}
#define TSS_C9_INPUTS_NUM_X         6
#define TSS_C9_STRUCTURE            cMatrix9          /* Name of the C&S struct to create */
#define TSS_C9_CALLBACK             TSS1_fCallBack9   /* Identifier of the user's callback */

#define TSS_C10_TYPE                TSS_CT_KEYPAD     /* Control type */
#define TSS_C10_INPUTS              {28,29,30,31,32,33}
#define TSS_C10_STRUCTURE           cKey10            /* Name of the C&S struct to create */
#define TSS_C10_CALLBACK            TSS1_fCallBack10  /* Identifier of the user's callback */

#define TSS_C11_TYPE                TSS_CT_KEYPAD     /* Control type */
#define TSS_C11_INPUTS              {34,35,36,5}
#define TSS_C11_STRUCTURE           cKey11            /* Name of the C&S struct to create */
#define TSS_C11_CALLBACK            TSS1_fCallBack11  /* Identifier of the user's callback */

#define TSS_C12_TYPE                TSS_CT_KEYPAD     /* Control type */
#define TSS_C12_INPUTS              {6,4,2,3,5}
#define TSS_C12_STRUCTURE           cKey12            /* Name of the C&S struct to create */
#define TSS_C12_CALLBACK            TSS1_fCallBack12  /* Identifier of the user's callback */

/********************* Peripheral Specific Configuration *********************/

/* TSI specific parameters */

/* TSI Auto-Calibration Settings */
#define TSS_TSI_RESOLUTION          11 /* Required Bit resolution of the TSI */
#define TSS_TSI_EXTCHRG_LOW_LIMIT   0  /* Low Limit of External OSC Charge Current register value for TSI auto-calibration */
#define TSS_TSI_EXTCHRG_HIGH_LIMIT  7  /* High Limit of External OSC Charge Current register value for TSI auto-calibration */
#define TSS_TSI_PS_LOW_LIMIT        0  /* Low Limit of External OSC Prescaler register value for TSI auto-calibration */
#define TSS_TSI_PS_HIGH_LIMIT       7  /* High Limit of External OSC Prescaler regiters value for TSI auto-calibration */
/* Proximity configuration */
#define TSS_TSI_PROX_EXTCHRG_LOW_LIMIT   4  /* Low Limit of External OSC Charge Current register value for TSI Proximity autocalibration */
#define TSS_TSI_PROX_EXTCHRG_HIGH_LIMIT  4  /* High Limit of External OSC Charge Current register value for TSI Proximity autocalibration */
#define TSS_TSI_PROX_PS_LOW_LIMIT        6  /* Low Limit of External OSC Prescaler register value for TSI Proximity autocalibration */
#define TSS_TSI_PROX_PS_HIGH_LIMIT       6  /* High Limit of External OSC Prescaler regiters value for TSI Proximity autocalibration */

#endif /*__TSS_SYSTEM_SETUP_H__*/
