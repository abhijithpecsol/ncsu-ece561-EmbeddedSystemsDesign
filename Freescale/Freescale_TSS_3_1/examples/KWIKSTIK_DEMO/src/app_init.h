/**HEADER********************************************************************
* 
* Copyright (c) 2008 Freescale Semiconductor;
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
* $FileName: app_init.h$
* $Version : 3.6.2.0$
* $Date    : Jul-10-2012$
*
* Comments:
*
*   This file contains the init functions.
*
*END************************************************************************/
#ifndef APP_INIT_H
#define APP_INIT_H

/***************************** Prototypes **********************************/

void InitPorts(void);
/***************************************************************************
* InitPorts
* --------------------------------------------------------------------------
* Global init of the board ports
****************************************************************************/

void TSS_Init_Keypad0(void);
/****************************************************************************************************
* TSS_Init_Keypad0
* ---------------------------------------------------------------------------------------------------
* Inits TSS for Keypad0 module
*****************************************************************************************************/

void TSS_Init_Keypad1(void);
/****************************************************************************************************
* TSS_Init_Keypad0
* ---------------------------------------------------------------------------------------------------
* Inits TSS for Keypad1 module
*****************************************************************************************************/

void TSS_Init_Rotary(void);
/****************************************************************************************************
* TSS_Init_Rotary
* ---------------------------------------------------------------------------------------------------
* Inits TSS for Rotary module
*****************************************************************************************************/

void FreeMASTER_Init(void);
/**************************************************************************//*!
* FreeMASTER_Init
* --------------------------------------------------------------------------
* Initialize FreeMASTER resources
******************************************************************************/

void DelayMS(uint16_t u16delay);
/***************************************************************************
* DelayMS
* --------------------------------------------------------------------------
* Common Delay function in ms units
****************************************************************************/

#endif /* __Events_H*/

/********************************************************************/
