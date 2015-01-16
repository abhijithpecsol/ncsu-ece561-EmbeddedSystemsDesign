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

#define TSS_USE_KEYDETECTOR_VERSION       2

#define TSS_USE_DELTA_LOG                 1

#define TSS_USE_SIGNAL_LOG                1

#define TSS_USE_GPIO_STRENGTH             0

#define TSS_USE_GPIO_SLEW_RATE            0

#define TSS_USE_IIR_FILTER                0

#define TSS_USE_NOISE_AMPLITUDE_FILTER    0

#define TSS_USE_TRIGGER_SOURCE            TSI0

#define TSS_USE_LOWPOWER_CONTROL_SOURCE   UNUSED

#define TSS_USE_DATA_CORRUPTION_CHECK     1

#define TSS_USE_FREEMASTER_GUI            1

#define TSS_USE_SIGNAL_SHIELDING          0

#define TSS_USE_STUCK_KEY                 1

#define TSS_USE_NEGATIVE_BASELINE_DROP    1

#define TSS_USE_CONTROL_PRIVATE_DATA      0

#define TSS_ENABLE_DIAGNOSTIC_MESSAGES    0

#define TSS_ONFAULT_CALLBACK              TSS_fOnFault /* Identifier of the user's OnFault callback */

#define TSS_ONINIT_CALLBACK               TSS_fOnInit /* Identifier of the user's OnInit callback */

#define TSS_N_ELECTRODES                  6         /* Number of electrodes present in the system */

/* Electrode's GPIOs configuration */

#define TSS_E2_P                    A               /* Electrode port */
#define TSS_E2_B                    24               /* Electrode bit  */
#define TSS_E3_P                    A               /* Electrode port */
#define TSS_E3_B                    25               /* Electrode bit  */
#define TSS_E4_P                    A               /* Electrode port */
#define TSS_E4_B                    26               /* Electrode bit  */
#define TSS_E5_P                    A               /* Electrode port */
#define TSS_E5_B                    27               /* Electrode bit  */

/* Electrode Types definition */

/* TWR Pads */
#define TSS_E0_TYPE                   TSI0_CH0
#define TSS_E1_TYPE                   TSI0_CH5
#define TSS_E2_TYPE                   GPIO
#define TSS_E3_TYPE                   GPIO
#define TSS_E4_TYPE                   GPIO
#define TSS_E5_TYPE                   GPIO

/* Controls configuration */

#define TSS_N_CONTROLS              1

#define TSS_C0_TYPE                 TSS_CT_KEYPAD     /* Control type */
#define TSS_C0_INPUTS               {0,1,2,3,4,5}     /* Control electrodes */
#define TSS_C0_STRUCTURE            cKey0             /* Name of the C&S struct to create */
#define TSS_C0_CALLBACK             TSS1_fCallBack0   /* Identifier of the user's callback */

/* TSI Specific parameter */
#define TSS_HW_TIMER                FTM1            /* Defines the TSS HW Timer */
#define TSS_SENSOR_PRESCALER        2               /* Defines the prescaler for all used Timers */
#define TSS_SENSOR_TIMEOUT          511             /* Defines the timeout for all used Timers */

/* TSI Specific parameter */

/* TSI Autocalibration Settings */
#define TSS_TSI_RESOLUTION                11 /* Required Bit resolution of the TSI */
#define TSS_TSI_SCANC_EXTCHRG_LOW_LIMIT   1  /* Low Limit of External OSC Charge Current register value for TSI autocalibration */
#define TSS_TSI_SCANC_EXTCHRG_HIGH_LIMIT  31 /* High Limit of External OSC Charge Current register value for TSI autocalibration */
#define TSS_TSI_GENCS_PS_LOW_LIMIT        0  /* Low Limit of External OSC Prescaler register value for TSI autocalibration */
#define TSS_TSI_GENCS_PS_HIGH_LIMIT       7  /* High Limit of External OSC Prescaler regiters value for TSI autocalibration */
/* Active Mode Clock Settings */
#define TSS_TSI_SCANC_AMCLKS              0  /* Set Input Active Mode Clock Source ( 0 = BUSclk, 1 = MCGIRCLK, 2 = OSCERCLK, 3 - NA ) */
#define TSS_TSI_SCANC_AMPSC               0  /* Set Input Active Mode Clock Prescaler ( 0 = divide 1, 7 = divide 128 ) */
#define TSS_TSI_SCANC_AMCLKDIV            1  /* Set Input Active Mode Clock Divider ( 0 = divide 1, 1 = divide 2048 ) */
#define TSS_TSI_SMOD                      255 /* Set Scan Modulo ( 0 - 255 ) */
/* Low Power TSI definition */
#define TSS_TSI_GENCS_LPCLKS              0   /* Set Low Power Mode Clock Source ( 0 = LPOCLK, 1 = VLPOSCCLK ) */
#define TSS_TSI_LPSCNITV                  8  /* Set Low Power Mode Scan Interval ( 0 = 1ms, 15 = 500ms ) */
  
#endif
