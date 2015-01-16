/**HEADER********************************************************************
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
* $FileName: board.c$
* $Version : 1.0.00.0$
* $Date    : Apr-15-2012$
*
* Comments:
*
*   This file contains the specific source code on the MCU.
*
*END************************************************************************/

#include "TSS_API.h"
#include "wdog.h"
#include "uart_kl05.h"
#include "freemaster.h"
#include "events.h"
#include "board.h"

/******************************* Macros ************************************/

#define APP_LPTRM_USE_OSCERKSEL 1

/****************************** Types **************************************/

uint8_t LPSWflag = 0u;

/***************************************************************************
* InitPorts
* --------------------------------------------------------------------------
* Global init of the board ports
****************************************************************************/
void InitPorts(void)
{
  SIM_MemMapPtr sim = SIM_BASE_PTR;

  /* Disable WatchDog */
  wdog_disable();
  /* PORT clock enabling */
  sim->SCGC5 |= SIM_SCGC5_PORTB_MASK; /* PORTB clock enablement */
  /* LED's MUX */
  PORTB_PCR20 = PORT_PCR_MUX(MUX_ALT1); /* LED1 */
  PORTB_PCR19 = PORT_PCR_MUX(MUX_ALT1); /* LED2 */
  /* LED's Init */
  GPIOB_PDDR |= (LED1 | LED2); /* Set pin to OUTPUT */
  /* Turn Off All LED's */
  LED1_OFF; LED2_OFF;
  /* Set SW2 for button which enters to Low Power mode */
  PORTB_PCR14 = 0u;
  PORTB_PCR14 = PORT_PCR_MUX(MUX_ALT1); /* Set SW3 for GPIO function */
  /* Set Interrupt */
  PORTB_PCR14 &= ~PORT_PCR_IRQC_MASK; /* Disable Interrupt */
  PORTB_PCR14 |= PORT_PCR_IRQC(0x0A) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK; /* Set Interrupt on Falling Edge */
  enable_irq(INT_PORTD - 16); /* Enable LPSW intr */
}

/***************************************************************************
* InitDevices
* --------------------------------------------------------------------------
* Global init of the devices
****************************************************************************/
void InitDevices(void)
{
  SIM_MemMapPtr sim = SIM_BASE_PTR;

  /* Modules Clock enabling */
  sim->SCGC5 |= SIM_SCGC5_TSI_MASK;  /* TSI clock enablement */
  sim->SCGC6 |= SIM_SCGC6_TPM1_MASK; /* TSS_HW_TIMER clock enablement */
  /* Set Electrodes for TSI function */
  PORTA_PCR9 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR8 = PORT_PCR_MUX(MUX_ALT0);
#if (TSS_USE_FREEMASTER_GUI == 0)
  PORTB_PCR2 = PORT_PCR_MUX(MUX_ALT0); /* Conflict with UART0 */
  PORTB_PCR1 = PORT_PCR_MUX(MUX_ALT0); /* Conflict with UART0 */
#endif
  PORTB_PCR0 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR7 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR11 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR10 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR12 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR13 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR11 = PORT_PCR_MUX(MUX_ALT0);
  PORTA_PCR10 = PORT_PCR_MUX(MUX_ALT0);
}

/**************************************************************************//*!
* FreeMASTER_Init
* --------------------------------------------------------------------------
* Initialize FreeMASTER resources
******************************************************************************/
void FreeMASTER_Init(void)
{
  /* Enable the SCI0_TXD function on the pin */
  PORTB_PCR1 = PORT_PCR_MUX(0x2);
  /* Enable the SCI0_RXD function on the pin */
  PORTB_PCR2 = PORT_PCR_MUX(0x2);
  /* Select the clock source for UART0 */
  SIM_SOPT2 |= SIM_SOPT2_UART0SRC(1);
  /* UART0 and UART1 are clocked from the core clock, but all other UARTs are
  * clocked from the peripheral clock. So we have to determine which clock
  * to send to the uart_init function.
  */
  uart0_init (FMSTR_UART_PORT, CORE_CLK_HZ, FMSTR_UART_BAUD);
  /* Enable UART Isr */
  enable_irq(FMSTR_UART_VECTOR - 16);
  /* Initialize FMSTR stack */
  FMSTR_Init();
}

/***************************************************************************
* DelayMS
* --------------------------------------------------------------------------
* Common Delay function in ms units
****************************************************************************/
void DelayMS(uint16_t u16delay, uint8_t u8CallbackEnabler)
{
  uint16_t i;
  uint16_t u16timeout;
  SIM_MemMapPtr sim = SIM_BASE_PTR;

  /* Timer Init */
  sim->SCGC6 |= SIM_SCGC6_TPM0_MASK;  /* TPM0 clock enable */
  sim->SOPT2 |= SIM_SOPT2_TPMSRC(1);  /* 00=Clk.disabled,01=MCGFLLCLK/MCGPLLCLK/2,10 = OSCERCLK,11=MCGIRCLK */
  TPM0_SC = 0u;
  TPM0_CNT = 0x0000U;
  TPM0_SC |= TPM_SC_PS(7);
  TPM0_MOD = 188u;                    /* 1ms@48MHz */
  TPM0_SC |= 0x08U;

  for (i=0; i < u16delay; i++)
  {
    u16timeout = 0u;
    do {
	  if (u8CallbackEnabler) DelayMS_Callback();
      u16timeout++;
    } while (!(TPM0_SC & TPM_SC_TOF_MASK) && (u16timeout < 0x7FFFu));
    TPM0_SC |= TPM_SC_TOF_MASK;      /* Clear Flag */
    TPM0_CNT = 0x0000U;
  }

  TPM0_SC = 0u;
}

/***************************************************************************
* BoardSpecificSettings
* --------------------------------------------------------------------------
* Board-specific initialization after app general settings
****************************************************************************/
void BoardSpecificInit(void)
{
#if (TSS_USE_FREEMASTER_GUI == 1)
  uint8_t u8Temp;
  /* Disable electrodes which have confict with UART0 (PTB2/TSI_CH2, PTB1/TSI_CH3) */
  u8Temp = TSS_GetSystemConfig(System_ElectrodeEnablers_Register+0);
  TSS_SetSystemConfig(System_ElectrodeEnablers_Register+0, u8Temp & ~0x0Cu );
#endif
}

/***************************************************************************/
