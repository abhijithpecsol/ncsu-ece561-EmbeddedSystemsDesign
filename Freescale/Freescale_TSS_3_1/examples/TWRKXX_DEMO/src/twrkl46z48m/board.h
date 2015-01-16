/**HEADER********************************************************************
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
* $FileName: board.h$
* $Version : 1.0.00.0$
* $Date    : Apr-15-2013$
*
* Comments:
*
*   This file contains the specific defintions for the board
*
*END************************************************************************/

#ifndef BOARD_H
  #define BOARD_H

  #include "derivative.h"
  #include "module_id.h"
  #include "main.h"

  /* System Info */
  #define CPU_MKL46Z48M
  #define DEBUG_PRINT
  #define CLK0_FREQ_HZ        8000000
  #define CLK0_TYPE           CRYSTAL
  #define PLL0_PRDIV      4       // divider eference by 2 = 4 MHz
  #define PLL0_VDIV       24      // multiply reference by 24 = 96 MHz

  /* Serial Port Info */
  /* Uses UART0 for both OSJTAG and TWR-SER Tower card */
  #define TERM_PORT           UART0_BASE_PTR
  #define TERM_PORT_NUM       0
  /** USE_UART0 must be defined if you want to use UART0 **/
  #define USE_UART0
  #define TERMINAL_BAUD       115200
  #undef  HW_FLOW_CONTROL

  /* FreeMASTER Serial Port Info */
  #define FMSTR_UART_PORT              UART0_BASE_PTR
  #define FMSTR_UART_VECTOR            28
  #define FMSTR_UART_PORT_NUM          0
  #define FMSTR_UART_BAUD                115200
  #define CORE_CLK_HZ                    24000000

  /* LED Control Macros */
  #define LED1    (1 << 5)
  #define LED2    (1 << 16)
  /* LED3 and LED4 are not present */
  #define LED3
  #define LED4

  #define LED1_ON    GPIOA_PCOR = LED1
  #define LED2_ON    GPIOA_PCOR = LED2
  #define LED3_ON
  #define LED4_ON

  #define LED1_OFF   GPIOA_PSOR = LED1
  #define LED2_OFF   GPIOA_PSOR = LED2
  #define LED3_OFF
  #define LED4_OFF

  #define LED_LAST_ON  LED2_ON
  #define LED_LAST_OFF LED2_OFF

  /* Low Power Types */
  #define LP_TYPE_LLS      0x03
  #define LP_TYPE_VLPS     0x02
  #define LP_TYPE_VLLSX    0x04
  #define LOW_POWER_MODE   LP_TYPE_LLS

  /* ADC channels */
  #define   TWR_ADC_CHANN_A  (TWR_ADC0 | 0x00)  /* ADC0_DP0/ADC0_SE0 */
  #define   TWR_ADC_CHANN_B  (TWR_ADC0 | 0x04)  /* ADC0_DM0/ADC0_SE4A */
  #define   TWR_ADC_CHANN_C  (TWR_ADC0 | 0x06)  /* ADC0_SE6B */
  #define   TWR_ADC_CHANN_D  (TWR_ADC0 | 0x07)  /* ADC0_SE7B */

  #define TWR_ADC0_AVAILABLE

  #define ADC0_CLOCK_ENABLE sim->SCGC6 |= SIM_SCGC6_ADC0_MASK /* ADC0 clock enablement */

  #define ADC_CONFIGURATION_REGISTER adc->CFG1 = 0x04
  #define ADC_STATUS_REGISTER adc->SC1[0]
  #define ADC_RESULT_REGISTER adc->R[0]

  /* Value ranges for purposes of conversion of ADC values  to index values */
  #define TWRPI_MAIN_RES        10000.0

  /* Application Periodic Timer Handling */
  #define APP_PERIODIC_TIMER_STOP    LPTMRStop
  #define APP_PERIODIC_TIMER_INIT    LPTMRInit

  /* Proximity board specific setup */
  #define APP_TWRPI_PROXIMITY_TIMEOUT 400u

  /*******************************************************
  *******          Function Prototypes           *********
  *******************************************************/

  void LPSWIsr(void);
  /*
  function    LPSWIsr
  brief:      Interrupt Service routine for detection of LPSW push
  param:      void
  return:     void
  */

  void InitDevices(void);
  /*
  function    InitDevices
  brief:      Global init of the devices
  param:      void
  return:     void
  */

  void LowPowerControl(void);
  /*
  function    InitDevices
  brief:      Function for low power management
  param:      void
  return:     void
  */

  void LPTMRInit(void);
  /*
  function    LPTMRInit
  brief:      LPTMR Timer Init for Auto Trigger Mode function
  param:      void
  return:     void
  */

  void LPTMRStop(void);
  /*
  function    LPTMRStop
  brief:      LPTMR Timer Stop for Auto Trigger Mode function
  param:      void
  return:     void
  */

  void LPTMRIsr(void);
  /*
  function    LPTMRIsr
  brief:      LPTMR Timer Isr function
  param:      void
  return:     void
  */

  void InitPorts(void);
  /*
  function    InitPorts
  brief:      Global init of the board ports
  param:      void
  return:     void
  */

  void FreeMASTER_Init(void);
  /*
  function    FreeMASTER_Init
  brief:      Initialize FreeMASTER resources
  param:      void
  return:     void
  */

  void DelayMS(uint16_t u16delay, uint8_t u8CallbackEnabler);
  /*
  function    DelayMS
  brief:      Common Delay function in ms units
  param:      u16delay - delay definition
              u8CallbackEnabler - true enables callback
  return:     void
  */

  /********************************************************************/

#endif  /* BOARD_H */
