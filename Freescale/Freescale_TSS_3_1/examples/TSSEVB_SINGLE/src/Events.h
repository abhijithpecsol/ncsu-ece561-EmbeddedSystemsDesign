/** ###################################################################
**     Filename  : Events.H
**     Project   : TSSEVB_SINGLE
**     Processor : MC9S08LG32CLK
**     Component : Events
**     Version   : Driver 01.02
**     Compiler  : CodeWarrior HCS08 C Compiler
**     Date/Time : 18.5.2010, 14:10
**     Abstract  :
**         This is user's event module.
**         Put your event handler code here.
**     Settings  :
**     Contents  :
**         TSS1_fCallBack0 - void TSS1_fCallBack0(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack1 - void TSS1_fCallBack1(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack2 - void TSS1_fCallBack2(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack3 - void TSS1_fCallBack3(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack4 - void TSS1_fCallBack4(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack5 - void TSS1_fCallBack5(TSS_CONTROL_ID u8ControlId);
**         TSS1_fCallBack6 - void TSS1_fCallBack6(TSS_CONTROL_ID u8ControlId);
**
** ###################################################################*/

#ifndef __Events_H
#define __Events_H

/* MODULE Events */

#include "TSS_DataTypes.h"

void TSS1_fCallBack0(TSS_CONTROL_ID u8ControlId);
/*
** ===================================================================
**     Event       :  TSS1_fCallBack0 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 0. This event is enabled
**         only if Control 0 is enabled.
**         The default CallBack Name is automatically generated with
**         automatic prefix update by current Component Name. User can
**         define own name, but then the automatic name update is not
**         functional.
**     Parameters  :
**         NAME            - DESCRIPTION
**         u8ControlId     - Valid unique Identifier of
**                           the Control which generated the CallBack
**                           function. This Id can be used for finding
**                           of Callback's source Control.
**     Returns     : Nothing
** ===================================================================
*/

/* END TSS1 Events */
#endif /* __Events_H*/
