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
  #define CPU_MK60N512VMD100
  #define DEBUG_PRINT
  #define K60_CLK                  1
  #define REF_CLK                  XTAL8   /* value isn't used, but we still need something defined */
  #define CORE_CLK_MHZ             PLL96

  /* Serial Port Info */
  #define TERM_PORT                UART3_BASE_PTR
  #define TERM_PORT_VECTOR         67
  #define TERMINAL_BAUD            115200
  #undef  HW_FLOW_CONTROL

  /* FreeMASTER Serial Port Info */
  #define FMSTR_UART_PORT          UART3_BASE_PTR
  #define FMSTR_UART_VECTOR        67
  #define FMSTR_UART_BAUD          115200
  #define CORE_CLK_HZ              96000000

  /* LED Control Macros */
  #define LED1    (1 << 11)
  #define LED2    (1 << 28)
  #define LED3    (1 << 29)
  #define LED4    (1 << 10)

  #define LED1_ON    GPIOA_PCOR = LED1
  #define LED2_ON    GPIOA_PCOR = LED2
  #define LED3_ON    GPIOA_PCOR = LED3
  #define LED4_ON    GPIOA_PCOR = LED4

  #define LED1_OFF   GPIOA_PSOR = LED1
  #define LED2_OFF   GPIOA_PSOR = LED2
  #define LED3_OFF   GPIOA_PSOR = LED3
  #define LED4_OFF   GPIOA_PSOR = LED4
  
  #define LED_LAST_ON  LED4_ON
  #define LED_LAST_OFF LED4_OFF

  /* Low Power Types */
  #define LP_TYPE_LLS      0x03
  #define LP_TYPE_VLLS1    0x07
  #define LP_TYPE_VLLS2    0x06
  #define LP_TYPE_VLLS3    0x05
  #define LOW_POWER_MODE   LP_TYPE_LLS

  /* ADC channels */
  #define TWR_ADC_CHANN_A  (TWR_ADC1 | 0x01)  /* ADC1_DP1 */
  #define TWR_ADC_CHANN_B  (TWR_ADC1 | 0x10)  /* ADC1_SE16 */
  #define TWR_ADC_CHANN_C  (TWR_ADC0 | 0x01)  /* ADC0_DP1 */
  #define TWR_ADC_CHANN_D  (TWR_ADC0 | 0x14)  /* ADC0_DM1 */
  
  #define TWR_ADC0_AVAILABLE
  #define TWR_ADC1_AVAILABLE

  #define ADC0_CLOCK_ENABLE sim->SCGC6 |= SIM_SCGC6_ADC0_MASK /* ADC0 clock enablement */
  #define ADC1_CLOCK_ENABLE sim->SCGC3 |= SIM_SCGC3_ADC1_MASK /* ADC1 clock enablement */
  #define ADC2_CLOCK_ENABLE sim->SCGC6 |= SIM_SCGC6_ADC2_MASK /* ADC2 clock enablement */
  
  #define ADC_CONFIGURATION_REGISTER adc->CFG1 = 0x04
  #define ADC_STATUS_REGISTER adc->SC1[0]
  #define ADC_RESULT_REGISTER adc->R[0]
  
  /* Value ranges for purposes of conversion of ADC values  to index values */
  #define TWRPI_MAIN_RES        10000.0

  /* Application Periodic Timer Handling */
  #define APP_PERIODIC_TIMER_STOP    PITStop
  #define APP_PERIODIC_TIMER_INIT    PITInit  
  
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

  void PITInit(void);
  /*
  function    PITInit
  brief:      PIT Timer Init
  param:      void
  return:     void
  */

  void PITStop(void);
  /*
  function    PITStop
  brief:      PIT Timer Stop
  param:      void
  return:     void
  */

  void PITIsr(void);
  /*
  function    PITIsr
  brief:      PIT Timer Isr Routine
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
