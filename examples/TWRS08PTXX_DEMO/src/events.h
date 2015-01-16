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
*   This file contains callbacks declarations.
*
*END************************************************************************/
#ifndef EVENTS_H
#define EVENTS_H

#ifdef __cplusplus
  extern "C" {
#endif

void TSS1_fCallBack0(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack1(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack2(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack3(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack4(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack5(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack6(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack7(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack8(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack9(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack10(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack11(TSS_CONTROL_ID u8ControlId);
void TSS1_fCallBack12(TSS_CONTROL_ID u8ControlId);
void TSS_fOnProximity(void);
void TSS1_fOnFault(void);
void TSS1_fOnInit(void);
void MODULE_ID_ChangedCB(TWRPI_DEVICES *pModule_id_twrpi_modules, MODULES module);
void PeriodicTimer_Callback(void);
void DelayMS_Callback(void);

#ifdef __cplusplus
  }
#endif

#endif /* __Events_H */
