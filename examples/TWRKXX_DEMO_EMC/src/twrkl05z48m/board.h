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
  #define CPU_MKL05Z32LF4
  #define DEBUG_PRINT
  #define CLK0_FREQ_HZ        32768
  #define CLK0_TYPE           CRYSTAL
  #define PLL0_PRDIV      1       // divider eference by 2 = 4 MHz
  #define PLL0_VDIV       24      // multiply reference by 24 = 96 MHz

  /* FreeMASTER Serial Port Info */
  #define FMSTR_UART_PORT              UART0_BASE_PTR
  #define FMSTR_UART_VECTOR            28
  #define FMSTR_UART_BAUD              115200
  #define CORE_CLK_HZ                  47972352

  /* LED Control Macros */
  #define LED1    (1 << 20)
  #define LED2    (1 << 19)
  /* LED3 and LED4 are not present */
  #define LED3
  #define LED4

  #define LED1_ON    GPIOB_PSOR = LED1
  #define LED2_ON    GPIOB_PSOR = LED2
  #define LED3_ON
  #define LED4_ON

  #define LED1_OFF   GPIOB_PCOR = LED1
  #define LED2_OFF   GPIOB_PCOR = LED2
  #define LED3_OFF
  #define LED4_OFF

  #define LED_LAST_ON  LED2_ON
  #define LED_LAST_OFF LED2_OFF

  /* ADC channels */
  #define   TWR_ADC_CHANN_A  (TWR_ADC0 | 0x00)  /* ADC0_SE0 */
  #define   TWR_ADC_CHANN_B  (TWR_ADC0 | 0x01)  /* ADC0_SE1 */
  #define   TWR_ADC_CHANN_C  TWR_ADC_CHANN_A    /* Unknown */
  #define   TWR_ADC_CHANN_D  TWR_ADC_CHANN_B    /* Unknown */

  #define TWR_ADC0_PULLUP_ENABLE  PORTA_PCR12 = PORT_PCR_MUX(0x1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;\
                                  PORTB_PCR5 = PORT_PCR_MUX(0x1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK;
  #define TWR_ADC0_AVAILABLE

  #define ADC0_CLOCK_ENABLE sim->SCGC6 |= SIM_SCGC6_ADC0_MASK /* ADC0 clock enablement */

  #define ADC_CONFIGURATION_REGISTER adc->CFG1 = 0x04
  #define ADC_STATUS_REGISTER adc->SC1[0]
  #define ADC_RESULT_REGISTER adc->R[0]

  /* Value ranges for purposes of conversion of ADC values  to index values */
  #define TWRPI_MAIN_RES        28000.0

  /* Board Specific Electrode Settings */
  #define APP_BOARD_SPECIFIC_INIT  BoardSpecificInit

  /*******************************************************
  *******          Function Prototypes           *********
  *******************************************************/

  void InitDevices(void);
  /*
  function    InitDevices
  brief:      Global init of the devices
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

  void BoardSpecificInit(void);
  /*
  function    DelayMS
  brief:      Board-specific initialization after app general settings
  param:      none
  return:     void
  */

  /********************************************************************/

#endif  /* BOARD_H */
