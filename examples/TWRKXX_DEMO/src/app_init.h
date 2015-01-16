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
* $FileName: app_init.h$
* $Version : 3.6.9.0$
* $Date    : Sep-19-2012$
*
* Comments:
*
*   This file contains the init functions.
*
*END************************************************************************/
#ifndef APP_INIT_H
#define APP_INIT_H

#include "module_id.h"

/***************************** Prototypes **********************************/

void TSS_Init_TWRPI_Board(TWRPI_DEVICES eTWRPI_module);
/**************************************************************************//*!
* TSS_Init_TWRPI_board
* ---------------------------------------------------------------------------
* Inits TSS for selected TWRPI board
******************************************************************************/

void PeriodicTimerControl(void);
/**************************************************************************//*!
* PeriodicTimerControl
* --------------------------------------------------------------------------
* Control function of periodic timer
******************************************************************************/

void DoLedBlinking(uint16_t u16Period, uint8_t u8Loops, uint8_t u8TSSEnabler);
/**************************************************************************//*!
* DoLedBlinking
* --------------------------------------------------------------------------
* Perfomr blinking of all LEDs in defined period
******************************************************************************/

#endif /* APP_INIT_H*/

/********************************************************************/
