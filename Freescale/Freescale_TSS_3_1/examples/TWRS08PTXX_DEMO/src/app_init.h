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
*   This file contains macros for the RTC module and function prototypes.
*
*END************************************************************************/
#ifndef APP_INIT_H
#define APP_INIT_H

/* RTC Trigger specific parameters */

#define APP_RTC_CLKS                3       /* Set RTC Clock Source (0 = External, 1 = Real-Time clock 1kHz, 2 = Reserved, 3 = Bus clock) */
#define APP_RTC_PS                  7       /* Set RTC Prescaler (0 = Off, 1 = divide 1, 7 = divide 64) */
#define APP_RTC_MOD                 2560    /* Set RTC MOD = 20 ms, 2048 = 16ms */

#ifdef __cplusplus
  extern "C" {
#endif

void InitPorts(void);
void TSS_Init_TWRPI_Board(TWRPI_DEVICES eTWRPI_module);
void FreeMASTER_Init(void);
void DelayMS(uint16_t u16delay, uint8_t u8CallbackEnabler);
void DoLedBlinking(uint16_t u16Period, uint8_t u8Loops, uint8_t u8TSSEnabler);
void PeriodicTimerControl(void);

void TSS_RTCInit(void);
void TSS_RTCStop(void);

#ifdef __cplusplus
  }
#endif

#endif

