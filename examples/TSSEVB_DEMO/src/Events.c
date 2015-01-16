/** ###################################################################
**     Filename  : Events.C
**     Project   : TSSEVB_DEMO
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
/* MODULE Events */

#include "Events.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

/****************************************************************************************************
* Variables
*****************************************************************************************************/

extern uint8_t near gu8Status;                   /* Variable that reports when is a pending event of the decoders */

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
void TSS1_fCallBack0(TSS_CONTROL_ID u8ControlId)
{
  /* Write your code here ... */
  gu8Status |= 0x01;
  (void) u8ControlId;
  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack1 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 1. This event is enabled
**         only if Control 1 is enabled.
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
void TSS1_fCallBack1(TSS_CONTROL_ID u8ControlId)
{
  /* Write your code here ... */
   gu8Status |= 0x02;
  (void) u8ControlId;
  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack2 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 2. This event is enabled
**         only if Control 2 is enabled.
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
void TSS1_fCallBack2(TSS_CONTROL_ID u8ControlId)
{
  /* Write your code here ... */
  gu8Status |= 0x04;                    /* Sets the flag event for the corresponding decoder */
  (void) u8ControlId;
  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack3 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 3. This event is enabled
**         only if Control 3 is enabled.
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
void TSS1_fCallBack3(TSS_CONTROL_ID u8ControlId)
{
  /* Write your code here ... */
  gu8Status |= 0x08;                    /* Sets the flag event for the corresponding decoder */
  (void) u8ControlId;
  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack4 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 4. This event is enabled
**         only if Control 4 is enabled.
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
void TSS1_fCallBack4(TSS_CONTROL_ID u8ControlId)
{
  /* Write your code here ... */
  gu8Status |= 0x10;                    /* Sets the flag event for the corresponding decoder */
  (void) u8ControlId;
  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack5 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 5. This event is enabled
**         only if Control 5 is enabled.
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
void TSS1_fCallBack5(TSS_CONTROL_ID u8ControlId)
{
  /* Write your code here ... */
  gu8Status |= 0x20;                    /* Sets the flag event for the corresponding decoder */
  (void) u8ControlId;
  return;
}

/*
** ===================================================================
**     Event       :  TSS1_fCallBack6 (module Events)
**
**     Component   :  TSS1 [TSS_Library]
**     Description :
**         Callback definition for Control 6. This event is enabled
**         only if Control 6 is enabled.
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
void TSS1_fCallBack6(TSS_CONTROL_ID u8ControlId)
{
  /* Write your code here ... */
  gu8Status |= 0x40;                    /* Sets the flag event for the corresponding decoder */
  (void) u8ControlId;
  return;
}

/* END TSS1 Events */

