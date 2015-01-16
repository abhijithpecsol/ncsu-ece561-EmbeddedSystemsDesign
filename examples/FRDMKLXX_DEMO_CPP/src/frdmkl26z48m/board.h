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
*   This file contains the specific header code on the FRDMKL25 board.
*
*END************************************************************************/
#ifndef BOARD_H
#define BOARD_H

#include "derivative.h"

/* Global defines */
#define DEBUG_PRINT

/* Define the CPU used on this hardware platform */
#define CPU_MKL26Z128VLH4

/* Define the clock parameters on this hardware platform */
#define CLK0_FREQ_HZ        8000000
#define CLK0_FREQ_MHZ       8
#define CLK0_TYPE           CRYSTAL
#undef  NO_PLL_INIT
#define PLL0_PRDIV          (CLK0_FREQ_MHZ/2)
#define PLL0_VDIV           24
#define CORE_CLK_HZ         24000000

/* Define the default serial port interface */
#define TERM_PORT           UART0_BASE_PTR
#define TERM_PORT_NUM       0
#define TERMINAL_BAUD       115200
#undef  HW_FLOW_CONTROL

/* LED Control */
#define LED0_MUX(number)  PORTE_PCR29 = (PORT_PCR_MUX(number))
#define LED0_ON           GPIOE_PDDR |= (1u << 29u)
#define LED0_OFF          GPIOE_PDDR &= ~(1u << 29u)

/* Electrodes MUX */
#define ELEC0_MUX(number) PORTB_PCR16 = PORT_PCR_MUX(number)
#define ELEC1_MUX(number) PORTB_PCR17 = PORT_PCR_MUX(number)

/* Function prototypes */
#ifdef __cplusplus
  extern "C" {
#endif
  void InitDevices(void);
#ifdef __cplusplus
  }
#endif

#endif  /* BOARD_H */
