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
*   This file contains the specific source code on the FRDMKL02 board.
*
*END************************************************************************/
#include "TSS_API.h"
#include "uart_kl05.h"
#include "board.h"
#include "app_init.h"
#include "freemaster.h"
#include "main.h"

/**
 * \brief  Initialize FreeMASTER resources
 *
 * \param  void
 *
 * \return void
 */
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
  #if (FMSTR_UART_PORT_NUM == 0)
    uart0_init (FMSTR_UART_PORT, CORE_CLK_HZ , FMSTR_UART_BAUD);
  #else
    uart_init (FMSTR_UART_PORT, CORE_CLK_HZ / (((SIM_CLKDIV1 & SIM_CLKDIV1_OUTDIV4_MASK) >> 16)+ 1), FMSTR_UART_BAUD);
  #endif

  /* Enable UART Isr */
  enable_irq(FMSTR_UART_VECTOR - 16);

  /* Initialize FMSTR stack */
  FMSTR_Init();
}

/**
 * \brief  Function PWM init for LED control
 *
 * \param  void
 *
 * \return void
 */
void PWM_init(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK | SIM_SCGC5_PORTA_MASK;
	SIM_SCGC6 |=( SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM1_MASK);
	SIM_SOPT2 |= SIM_SOPT2_TPMSRC(1); // FLL as clock
	
	PORTB_PCR6   = (0|PORT_PCR_MUX(2)); /* TPM1_CH1 enable on PTB6 */
	PORTB_PCR7   = (0|PORT_PCR_MUX(2)); /* TPM1_CH0 enable on PTB7 */
	PORTB_PCR10  = (0|PORT_PCR_MUX(2)); /* TPM0_CH1 enable on PTB10 */
	
	TPM0_CONF = TPM_CONF_GTBEEN_MASK;
	
	TPM1_MOD  = TPM_MODULE;  /* 0x0063 / 25MHz = 4uS PWM period */
	TPM1_C0SC = TPM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
	TPM1_C0V  = TPM_INIT_VAL;  /* 90% pulse width */
	TPM1_C1SC = TPM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
	TPM1_C1V  = TPM_INIT_VAL;  /* 90% pulse width */
	TPM0_C1SC = TPM_Cn_MODE;   /* No Interrupts; High True pulses on Edge Aligned PWM */
	TPM0_C1V  = TPM_INIT_VAL;  /* 90% pulse width */
	TPM1_SC   = TPM_SC_CMOD(1) | TPM_SC_PS(0);     /* Edge Aligned PWM running from BUSCLK / 1 */
}

/**
 * \brief  LPTMR timer init
 *
 * \param  void
 *
 * \return void
 */
void LPTMRInit(void)
{
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;

  #if defined(APP_LPTRM_USE_LPO) /* Does not run in VLLS0 */
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) /* 0000 is div 2 */
                 | LPTMR_PSR_PBYP_MASK   /* Selected Clk directly to LPTMR */
                 | LPTMR_PSR_PCS(1)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(18); /* Set compare value for 20 ms */
  #elif defined(APP_LPTRM_USE_MCGIRCLK)
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) /* 0000 is div 2 */
                 | LPTMR_PSR_PBYP_MASK   /* Selected Clk directly to LPTMR */
                 | LPTMR_PSR_PCS(0)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(480); /* Set compare value for 20 ms */
  #elif defined(APP_LPTRM_USE_OSCERKSEL)
    /* Select ERCLK32 clock for RTC/LPTMR (0 = OSC32KCLK, 2 = RTC_CLKIN extpin (32.768kHz osc), 3 = LPO (1kHz)) */
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0);
    OSC0_CR = OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(3) /* 3 is div 16 */
                 | LPTMR_PSR_PCS(3)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */

    LPTMR0_CMR = LPTMR_CMR_COMPARE(250); /* Set compare value for  20 ms */

  #elif defined(APP_LPTRM_USE_OSC32KSEL)
    /* Select ERCLK32 clock for RTC/LPTMR (0 = OSC32KCLK, 2 = RTC_CLKIN extpin (32.768kHz osc), 3 = LPO (1kHz)) */
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(0);
    OSC0_CR = OSC_CR_ERCLKEN_MASK | OSC_CR_EREFSTEN_MASK;
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) /* 0000 is div 2 */
                 | LPTMR_PSR_PBYP_MASK   /* Selected Clk directly to LPTMR */
                 | LPTMR_PSR_PCS(2)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(660); /* Set compare value for 20 ms */
  #elif defined(APP_LPTRM_USE_RTC_CLKIN) /* Run in all MCU modes, but RTC_CLKIN is shared with TSI_CH14 pin on KL25 */
    /* Set pin PTC1/RTC_CLKIN for MUX_ALT1 function where RTC_CLKIN is enabled */

    PORTA_PCR5 |= PORT_PCR_MUX(MUX_ALT1);

    /* Select ERCLK32 clock for RTC/LPTMR (0 = OSC32KCLK, 2 = RTC_CLKIN extpin (32.768kHz osc), 3 = LPO (1kHz)) */
    SIM_SOPT1 |= SIM_SOPT1_OSC32KSEL(2);
    LPTMR0_PSR = ( LPTMR_PSR_PRESCALE(0) /* 0000 is div 2 */
                 | LPTMR_PSR_PBYP_MASK   /* Selected Clk directly to LPTMR */
                 | LPTMR_PSR_PCS(2)) ;   /* use the choice of clock 0=MCGIRCLK/1=LPOCLK/2=ERCLK32/3=OSCERCLK */
    LPTMR0_CMR = LPTMR_CMR_COMPARE(660); /* Set compare value for 20 ms */
  #endif

  LPTMR0_CSR =(  LPTMR_CSR_TCF_MASK    /* Clear any pending interrupt */
               | LPTMR_CSR_TIE_MASK    /* LPTMR interrupt enabled */
               | LPTMR_CSR_TPS(0)      /* TMR pin select */
              );

  enable_irq(INT_LPTimer - 16);      /* LPTMR vector enable */

  LPTMR0_CSR |= LPTMR_CSR_TEN_MASK;    /* Turn on LPT and start counting */

}

/**
 * \brief  LPTMR timer stop
 *
 * \param  void
 *
 * \return void
 */
void LPTMRStop(void)
{
  SIM_SCGC5 |= SIM_SCGC5_LPTMR_MASK;
  disable_irq(INT_LPTimer - 16);    /* LPTMR vector disable */
  LPTMR0_CSR &= ~LPTMR_CSR_TEN_MASK;  /* Turn off LPTMR and stoprt counting */
}

/**
 * \brief  LPTMR Timer Isr function
 *
 * \param  void
 *
 * \return void
 */
void LPTMRIsr(void)
{
  LPTMR0_CSR |=  LPTMR_CSR_TCF_MASK;   /* Clear the LPT timer compare flag */

  #if (APP_TSS_USE_TRIGGER_MODE == SW)
    if ((TSS_GetSystemConfig(System_SystemTrigger_Register) & TSS_TRIGGER_MODE_MASK) == TSS_TRIGGER_MODE_SW)
    {
      /* Toggle TSS SW trigger if SW trigger mode is enabled */
      (void) TSS_SetSystemConfig(System_SystemTrigger_Register, TSS_SWTRIGGER_MASK | TSS_TRIGGER_MODE_SW);
    }
  #elif (APP_TSS_USE_TRIGGER_MODE == AUTO)
    #error "APP - The example does not support AUTO trigger mode, because TSI module is not available on KL02 MCU"
  #endif
}

/**
 * \brief  Port Initialization (TSSOnInit redirection)
 *
 * \param  void
 *
 * \return void
 */
void InitDevices(void)
{
  SIM_MemMapPtr sim = SIM_BASE_PTR;

  /* Modules Clock enablement */
  sim->SCGC5 |= SIM_SCGC5_PORTB_MASK;  /* Port B clock enablement */
  sim->SCGC6 |= SIM_SCGC6_TPM0_MASK;   /* HW Timer */

  /* Set Electrodes for TSI function */
  PORTB_PCR16 = PORT_PCR_MUX(MUX_ALT0);
  PORTB_PCR17 = PORT_PCR_MUX(MUX_ALT0);
}
