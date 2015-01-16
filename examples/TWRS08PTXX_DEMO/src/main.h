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
*   This file contains the source for the PT60 TSS example program.
*
*END************************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include "common.h"

/* Serial Port Info */
#define USE_SCI                        1
/* define UART baud rate */
#define USE_SCI_BAUDRATE               57600

#if (USE_SCI == 0)
  #define FMSTR_UART_PORT              0x00003080 // SCI0
  #define FMSTR_UART_RX_VECTOR         VectorNumber_Vsci0rx
  #define FMSTR_UART_TX_VECTOR         VectorNumber_Vsci0tx
#elif (USE_SCI == 1)
  #define FMSTR_UART_PORT              0x00003088 // SCI1
  #define FMSTR_UART_RX_VECTOR         VectorNumber_Vsci1rx
  #define FMSTR_UART_TX_VECTOR         VectorNumber_Vsci1tx
#elif (USE_SCI == 2)
  #define FMSTR_UART_PORT              0x00003090 // SCI2
  #define FMSTR_UART_RX_VECTOR         VectorNumber_Vsci2rx
  #define FMSTR_UART_TX_VECTOR         VectorNumber_Vsci2tx
#endif

/* LED Control Macros */

#define LED1_TOGGLE   PORT_PTGD_PTGD0  ^= 1
#define LED1_ON       PORT_PTGD_PTGD0   = 0
#define LED1_OFF      PORT_PTGD_PTGD0   = 1

#define LED2_TOGGLE   PORT_PTGD_PTGD1  ^= 1
#define LED2_ON       PORT_PTGD_PTGD1   = 0
#define LED2_OFF      PORT_PTGD_PTGD1   = 1

#define LED3_TOGGLE   PORT_PTGD_PTGD2  ^= 1
#define LED3_ON       PORT_PTGD_PTGD2   = 0
#define LED3_OFF      PORT_PTGD_PTGD2   = 1

#define LED4_TOGGLE   PORT_PTGD_PTGD3  ^= 1
#define LED4_ON       PORT_PTGD_PTGD3   = 0
#define LED4_OFF      PORT_PTGD_PTGD3   = 1

/* Proximity board specific setup */
#define APP_TWRPI_PROXIMITY_TIMEOUT    400u

/* Application Constants */

#define ALWAYS                         1     /* ~5ms  period of TSI measurement */
#define SW                             2     /* set 20ms  period of TSI measurement */
#define AUTO                           3     /* set 20ms  period of TSI measurement */

/* Application Setup */

#define APP_TSS_USE_DCTRACKER          1
#define APP_TSS_USE_TRIGGER_MODE       ALWAYS

#endif
