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
* $FileName: Driver_Buzzer.h$
* $Version : 3.6.2.0$
* $Date    : Feb-6-2012$
*
* Comments:
*
*   This file contains the source for the buzzer. This file was edited
*   for purpose of this demo.
*
*END************************************************************************/
#ifndef __DRIVER_BUZZER_H__
#define __DRIVER_BUZZER_H__

#include "TSS_DataTypes.h"
#include "derivative.h"

#ifndef FALSE
  #define FALSE          0
#endif
#ifndef TRUE
  #define TRUE           1
#endif

  /**
    External variables
  */
  extern unsigned char isBuzzerOn;

  /**
    Public Function Prototypes
  */
  void _Buzzer_Init(void);
  void _Buzzer_On(uint16_t MilliSeconds);
  void _Buzzer_Off(void);
  void _PIT0_Isr(void);

#endif /*__DRIVER_BUZZER_H__*/
