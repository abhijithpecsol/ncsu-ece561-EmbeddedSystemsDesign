/******************************************************************************
* File:    isr.h
* Purpose: Define interrupt service routines referenced by the vector table.
* Note: Only "vectors.c" should include this header file.
******************************************************************************/
#ifndef ISR_H
#define ISR_H

#include "TSS_API.h"
#include "main.h"
#include "freemaster.h"
#include "Driver_Buzzer.h"

#undef  VECTOR_099
#define VECTOR_099 TSS_TSI0Isr
  
#undef  VECTOR_084
#define VECTOR_084 _PIT0_Isr
  
#undef  VECTOR_071
#define VECTOR_071 FMSTR_Isr

#undef  VECTOR_079
#define VECTOR_079 TSS_HWTimerIsr

/* Example */
/*
#undef  VECTOR_101
#define VECTOR_101 lpt_isr

// ISR(s) are defined in your project directory.
extern void lpt_isr(void);
*/


#endif  //__ISR_H

/* End of "isr.h" */
