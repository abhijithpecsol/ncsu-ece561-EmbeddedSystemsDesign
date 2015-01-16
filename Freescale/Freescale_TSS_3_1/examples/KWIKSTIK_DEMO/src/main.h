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
    #define FMSTR_UART_PORT              UART5_BASE_PTR
    #define FMSTR_UART_VECTOR            71
  #else
    #define FMSTR_UART_PORT              UART0_BASE_PTR
    #define FMSTR_UART_VECTOR            61
  #endif
  #define FMSTR_UART_BAUD                115200
  #define CORE_CLK_KHZ                   100000

/* Multiplexing Macros */

#define MUX_ALT0     0 /* Signal */
#define MUX_ALT1     1 /* GPIO */
#define MUX_ALT2     2
#define MUX_ALT3     3

/* Interrupt Macros */

#define ISR_ENABLE_VECT(vect)    NVIC_ISER_REG(NVIC_BASE_PTR, (vect-16)/32) = 1<<((vect-16)%32)
#define ISR_DISABLE_VECT(vect)   NVIC_ICER_REG(NVIC_BASE_PTR, (vect-16)/32) = 1<<((vect-16)%32)

#define ISR_ENABLE_IRQ(vect)     NVIC_ISER_REG(NVIC_BASE_PTR, vect/32) = 1<<(vect%32)
#define ISR_DISABLE_IRQ(vect)    NVIC_ICER_REG(NVIC_BASE_PTR, vect/32) = 1<<(vect%32)

/* Application Setup */

#define TSS_USE_DCTRACKER        1

#endif
