/*HEADER********************************************************************
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
* Comments:
*
*   TSS configuration file.
*
*END************************************************************************/
#ifndef TSS_SYSTEM_SETUP_H
#define TSS_SYSTEM_SETUP_H

/* TSS Features Configuration */

#define TSS_USE_KEYDETECTOR_VERSION            1

#define TSS_USE_DELTA_LOG                      1

#define TSS_USE_SIGNAL_LOG                     1

#define TSS_USE_INTEGRATION_DELTA_LOG          1

#define TSS_USE_GPIO_STRENGTH                  0

#define TSS_USE_GPIO_SLEW_RATE                 0

#define TSS_USE_IIR_FILTER                     0

#define TSS_USE_NOISE_AMPLITUDE_FILTER         0

#define TSS_USE_TRIGGER_FUNCTION               1

#define TSS_USE_TRIGGER_SOURCE	               TSI0

#define TSS_USE_LOWPOWER_CONTROL_SOURCE        TSI0

#define TSS_USE_LOWPOWER_THRESHOLD_BASELINE    0

#define TSS_USE_AUTO_SENS_CALIBRATION          1

#define TSS_USE_AUTO_SENS_CALIB_INIT_DURATION  100

#define TSS_USE_AUTO_SENS_CALIB_LOW_LIMIT      0

#define TSS_USE_BASELINE_INIT_DURATION         15

#define TSS_USE_DATA_CORRUPTION_CHECK          1

#define TSS_USE_FREEMASTER_GUI                 1
#if (TSS_USE_KEYDETECTOR_VERSION == 1)
#define TSS_USE_SIGNAL_SHIELDING               1
#endif
#define TSS_USE_SIGNAL_DIVIDER                 0

#define TSS_USE_SIGNAL_MULTIPLIER              1

#define TSS_USE_STUCK_KEY                      1

#define TSS_USE_NEGATIVE_BASELINE_DROP         1

#define TSS_USE_AUTO_HW_RECALIBRATION          1

#define TSS_USE_CONTROL_PRIVATE_DATA           0

#define TSS_ENABLE_DIAGNOSTIC_MESSAGES         0

#define TSS_ONFAULT_CALLBACK                   TSS1_fOnFault /* Identifier of the user's OnFault callback */

#define TSS_ONINIT_CALLBACK                    TSS1_fOnInit /* Identifier of the user's OnInit callback */

#define TSS_ONPROXIMITY_CALLBACK               TSS_fOnProximity /* Identifier of the user's OnProximity callback */

#define TSS_N_ELECTRODES                 	   15        /* Number of electrodes present in the system */

/* Electrode's GPIOs configuration */

/* Electrode Types definition */

#define TSS_E0_TYPE                 TSI0_CH1
#define TSS_E1_TYPE                 TSI0_CH9
#define TSS_E2_TYPE                 TSI0_CH10
#define TSS_E3_TYPE                 TSI0_CH11
#define TSS_E4_TYPE                 TSI0_CH12
#define TSS_E5_TYPE                 TSI0_CH13

/* Shield 1 Daughter Board Pads */
#define TSS_E6_TYPE                 TSI0_CH1
#define TSS_E7_TYPE                 TSI0_CH9
#define TSS_E8_TYPE                 TSI0_CH10
#define TSS_E9_TYPE                 TSI0_CH11
#define TSS_E10_TYPE                TSI0_CH12
#define TSS_E11_TYPE                TSI0_CH13
/* Shield 2 Daughter Board Pads */
#define TSS_E12_TYPE                TSI0_CH1
#define TSS_E13_TYPE                TSI0_CH9
#define TSS_E14_TYPE                TSI0_CH10

/* Shield1 Configuration */
#define TSS_E7_SHIELD_ELECTRODE     6
#define TSS_E9_SHIELD_ELECTRODE     8
#define TSS_E11_SHIELD_ELECTRODE    10

/* Shield2 Configuration */
#define TSS_E12_SHIELD_ELECTRODE    3
#define TSS_E13_SHIELD_ELECTRODE    3
#define TSS_E14_SHIELD_ELECTRODE    3

/* Electrode Signal Divider & Multiplier */

#define TSS_E6_SIGNAL_MULTIPLIER    2
#define TSS_E8_SIGNAL_MULTIPLIER    2
#define TSS_E10_SIGNAL_MULTIPLIER   2
#define TSS_E15_SIGNAL_MULTIPLIER   2

/* Controls configuration */

#define TSS_N_CONTROLS              12

#define TSS_C0_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C0_INPUTS               {1,2}             /* Control electrodes */
#define TSS_C0_STRUCTURE            cKey0             /* Name of the C&S struct to create */
#define TSS_C0_CALLBACK             TSS1_fCallBack0   /* Identifier of the user's callback */

#define TSS_C1_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C1_INPUTS               {0,1,2,3,4,5}     /* Control electrodes */
#define TSS_C1_STRUCTURE            cKey1             /* Name of the C&S struct to create */
#define TSS_C1_CALLBACK             TSS1_fCallBack1   /* Identifier of the user's callback */

#define TSS_C2_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C2_INPUTS               {4}               /* Control electrodes */
#define TSS_C2_STRUCTURE            cKey2             /* Name of the C&S struct to create */
#define TSS_C2_CALLBACK             TSS1_fCallBack2   /* Identifier of the user's callback */

#define TSS_C3_TYPE                 TSS_CT_ROTARY     /* Control type */
#define TSS_C3_INPUTS               {3,1,2,5,0}       /* Control electrodes */
#define TSS_C3_STRUCTURE            cRotary3          /* Name of the C&S struct to create */
#define TSS_C3_CALLBACK             TSS1_fCallBack3   /* Identifier of the user's callback */

#define TSS_C4_TYPE                 TSS_CT_ASLIDER    /* Control type */
#define TSS_C4_INPUTS               {1,0}             /* Control electrodes */
#define TSS_C4_STRUCTURE            cASlider4         /* Name of the C&S struct to create */
#define TSS_C4_CALLBACK             TSS1_fCallBack4   /* Identifier of the user's callback */

#define TSS_C5_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C5_INPUTS               {2,3,4}           /* Control electrodes */
#define TSS_C5_STRUCTURE            cKey5             /* Name of the C&S struct to create */
#define TSS_C5_CALLBACK             TSS1_fCallBack5   /* Identifier of the user's callback */

#define TSS_C6_TYPE                 TSS_CT_AROTARY    /* Control type */
#define TSS_C6_INPUTS               {0,1,2,3}         /* Control electrodes */
#define TSS_C6_STRUCTURE            cARotary6         /* Name of the C&S struct to create */
#define TSS_C6_CALLBACK             TSS1_fCallBack6   /* Identifier of the user's callback */

#define TSS_C7_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C7_INPUTS               {4}               /* Control electrodes */
#define TSS_C7_STRUCTURE            cKey7             /* Name of the C&S struct to create */
#define TSS_C7_CALLBACK             TSS1_fCallBack7   /* Identifier of the user's callback */

#define TSS_C8_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C8_INPUTS               {0,1,2,3,4,5}     /* Control electrodes */
#define TSS_C8_KEYS                 {0x05,0x09,0x0A,0x14,0x18,0x0C}
#define TSS_C8_STRUCTURE            cKey8             /* Name of the C&S struct to create */
#define TSS_C8_CALLBACK             TSS1_fCallBack8   /* Identifier of the user's callback */

#define TSS_C9_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C9_INPUTS               {6,7,8,9,10,11}     /* Control electrodes */
#define TSS_C9_STRUCTURE            cKey9             /* Name of the C&S struct to create */
#define TSS_C9_CALLBACK             TSS1_fCallBack9   /* Identifier of the user's callback */

#define TSS_C10_TYPE                TSS_CT_KEYPAD     /* Control type */
#define TSS_C10_INPUTS              {12,13,14,3}         /* Control electrodes */
#define TSS_C10_STRUCTURE           cKey10            /* Name of the C&S struct to create */
#define TSS_C10_CALLBACK            TSS1_fCallBack10  /* Identifier of the user's callback */

#define TSS_C11_TYPE                TSS_CT_KEYPAD     /* Control type */
#define TSS_C11_INPUTS              {4,2,0,1,3}
#define TSS_C11_STRUCTURE           cKey11            /* Name of the C&S struct to create */
#define TSS_C11_CALLBACK            TSS1_fCallBack11  /* Identifier of the user's callback */

/* TSI Specific parameter */

/* TSI Autocalibration Settings */
#define TSS_TSI_RESOLUTION          11 /* Required Bit resolution of the TSI */
#define TSS_TSI_EXTCHRG_LOW_LIMIT   1  /* Low Limit of External OSC Charge Current register value for TSI autocalibration */
#define TSS_TSI_EXTCHRG_HIGH_LIMIT  31 /* High Limit of External OSC Charge Current register value for TSI autocalibration */
#define TSS_TSI_PS_LOW_LIMIT        0  /* Low Limit of External OSC Prescaler register value for TSI autocalibration */
#define TSS_TSI_PS_HIGH_LIMIT       7  /* High Limit of External OSC Prescaler regiters value for TSI autocalibration */
/* Proximity configuration */
#define TSS_TSI_PROX_EXTCHRG_LOW_LIMIT   5  /* Low Limit of External OSC Charge Current register value for TSI Proximity autocalibration */
#define TSS_TSI_PROX_EXTCHRG_HIGH_LIMIT  5  /* High Limit of External OSC Charge Current register value for TSI Proximity autocalibration */
#define TSS_TSI_PROX_PS_LOW_LIMIT        6  /* Low Limit of External OSC Prescaler register value for TSI Proximity autocalibration */
#define TSS_TSI_PROX_PS_HIGH_LIMIT       6  /* High Limit of External OSC Prescaler regiters value for TSI Proximity autocalibration */
/* Active Mode Clock Settings */
#define TSS_TSI_AMCLKS              0  /* Set Input Active Mode Clock Source ( 0 = BUSclk, 1 = MCGIRCLK, 2 = OSCERCLK, 3 - NA ) */
#define TSS_TSI_AMPSC               0  /* Set Input Active Mode Clock Prescaler ( 0 = divide 1, 7 = divide 128 ) */
#define TSS_TSI_AMCLKDIV            1  /* Set Input Active Mode Clock Divider ( 0 = divide 1, 1 = divide 2048 ) */
#define TSS_TSI_SMOD                40 /* Set Scan Modulo ( 0 - 255 ) */
/* Low Power TSI definition */
#define TSS_TSI_LPCLKS              0  /* Set Low Power Mode Clock Source ( 0 = LPOCLK, 1 = VLPOSCCLK ) */
#define TSS_TSI_LPSCNITV            8  /* Set Low Power Mode Scan Interval ( 0 = 1ms, 15 = 500ms ) */
  
#endif /*__TSS_SYSTEM_SETUP_H__*/
