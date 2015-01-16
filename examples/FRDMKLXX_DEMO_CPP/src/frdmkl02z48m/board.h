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
*   This file contains the specific header code on the FRDMKL02 board.
*
*END************************************************************************/
#ifndef BOARD_H
#define BOARD_H

#include "derivative.h"

#define CPU_MKL02Z32LF4

/*
* Input Clock Info
*/
#define CLK0_FREQ_HZ        32768
#define CLK0_TYPE           CRYSTAL

/* Serial Port Info */
/* Uses UART0 for both OSJTAG and TWR-SER Tower card */
#define TERM_PORT           UART0_BASE_PTR

/** USE_UART0 must be defined if you want to use UART0 **/
#define USE_UART0
#define TERMINAL_BAUD       115200
#undef  HW_FLOW_CONTROL

/* LED Control */
#define LED0_MUX(number)  PORTB_PCR6 = (PORT_PCR_MUX(number))
#define LED0_ON           GPIOB_PDDR |= (1u << 6u)
#define LED0_OFF          GPIOB_PDDR &= ~(1u << 6u)

/* Electrodes MUX - GPIO method */
#define ELEC0_MUX(number)
#define ELEC1_MUX(number)

/* Function prototypes */
#ifdef __cplusplus
  extern "C" {
#endif
  void InitDevices(void);
#ifdef __cplusplus
  }
#endif

#endif  /* BOARD_H */
