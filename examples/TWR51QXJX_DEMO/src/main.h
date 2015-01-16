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
*
* Comments:
*
*   This file contains the source for the TSS example program.
*
*END************************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include "derivative.h"

/* FreeMASTER UART Settings */

#define TWR_USE_SERIAL_BOARD             1

/* Serial Port Info */
#if TWR_USE_SERIAL_BOARD
  #define FMSTR_UART_PORT              UART0_BASE_PTR
  #define FMSTR_UART_VECTOR            87
#else
  #error "TWR - Option TWR_USE_SERIAL_BOARD = 0 is not supported "
#endif
#define FMSTR_UART_BAUD                115200
#define CORE_CLK_KHZ                   50000

/* LED Control Macros */
#define LED1_ON  PTA_D &= 0xFE
#define LED2_ON  PTC_D &= 0xDF

#define LED1_OFF  PTA_D |= 0x01
#define LED2_OFF  PTC_D |= 0x20

#define LED1_TOG  if(PTA_D & 0x01) PTA_D &= 0xFE; else PTA_D |= 0x01
#define LED2_TOG  if(PTC_D & 0x20) PTC_D &= 0xDF; else PTC_D |= 0x20

/* Multiplexing Macros */

#define MUX_ALT0     0 /* Signal */
#define MUX_ALT1     1 /* GPIO */
#define MUX_ALT2     2
#define MUX_ALT3     3

/* Low Power Types */
#define LP_TYPE_LLS      0x03
#define LP_TYPE_VLLS1    0x07
#define LP_TYPE_VLLS2    0x06
#define LP_TYPE_VLLS3    0x05

/* Proximity board specific setup */
#define APP_TWRPI_PROXIMITY_TIMEOUT    400u

/* Application Constants */

#define ALWAYS                         1     /* ~17ms  period of TSI measurement */
#define SW                             2     /* set 20ms  period of TSI measurement */
#define AUTO                           3     /* set 20ms  period of TSI measurement */

/* Application Setup */

#define APP_TSS_USE_DCTRACKER          1
#define APP_TSS_USE_TRIGGER_MODE       ALWAYS

#ifdef __cplusplus
  extern "C" {
#endif

void LPSWIsr(void);

#ifdef __cplusplus
  }
#endif

#endif

