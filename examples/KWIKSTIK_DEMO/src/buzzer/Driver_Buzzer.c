/**HEADER********************************************************************
*
* Copyright (c) 2011 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
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
* $FileName: Driver_Buzzer.c$
* $Version : 3.6.2.0$
* $Date    : Feb-3-2012$
*
* Comments:
*
*   This file contains the source for the buzzer. This file was edited
*   for purpose of this demo.
*
*END************************************************************************/
#include "Driver_Buzzer.h"

/**
  Global variables
*/
static uint16_t mSec = 0;
unsigned char isBuzzerOn = FALSE;

/**
  Function Name    : _Buzzer_Init
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : This function is used to configure the correspoding pin and PIT0 to drive
                  the buzzer.
*/
void _Buzzer_Init(void)
{
    /* GPIO INIT */
    PORT_PCR_REG(PORTA_BASE_PTR, 8) = PORT_PCR_MUX(1);
    SIM_SCGC5 |= SIM_SCGC5_PORTA_MASK ;
    /* PIT0 INIT */
    SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT_MCR &= ~PIT_MCR_MDIS_MASK;
    PIT_MCR |= PIT_MCR_FRZ_MASK;
    PIT_LDVAL0 = 12000; // 250uS

    NVICISER2 |= (unsigned long)(84%32);
    NVICICPR2 |= (unsigned long)(84%32);

    PIT_TCTRL0 |= PIT_TCTRL_TIE_MASK;
}

/**
  Function Name    : _Buzzer_On
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : uint16_t MilliSeconds
  Returns       : Nothing
  Notes         : This function turns the Buzzer on for the received parameter
*/
void _Buzzer_On(uint16_t MilliSeconds)
{
  mSec = MilliSeconds;
  isBuzzerOn = TRUE;
  /* PIT ENABLE */
  PIT_TCTRL0 |= PIT_TCTRL_TEN_MASK;
}

/**
  Function Name    : _Buzzer_Off
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : This function disables the PIT0, thus the buzzer turns off.
*/
void _Buzzer_Off(void)
{
    /* PIT DISABLE */
    PIT_TCTRL0 &= ~PIT_TCTRL_TEN_MASK;
}

/**
  Function Name    : _PIT0_Isr
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : pointer isr
  Returns       : Nothing
  Notes         : This is the user defined interrupt service for the PIT0. The
                  PIT's load value is rewritten, otherwise this interrupt will not
                  happen again.
*/
void _PIT0_Isr(void)
{
    volatile static uint8_t _250uS = 0;

    PIT_TFLG0 = 1;
    PIT_LDVAL0 = 12000; // 250uS

    if (_250uS < 1)
    {
        /* SET PIN */
        GPIOA_PSOR |= (1ul << 8);
        GPIOA_PDDR |= (1ul << 8);
        GPIOA_PCOR &= ~(1ul << 8);
        _250uS++;
    }
    else
    {
        /* CLEAR PIN */
        GPIOA_PCOR |= (1ul << 8);
        GPIOA_PSOR &= ~(1ul << 8);
        _250uS++;
    if (_250uS == 4)
    {
      _250uS = 0;
      mSec--;
      if (mSec == 0)
      {
        _Buzzer_Off();
        isBuzzerOn = FALSE;
      }
    }
  }
}
