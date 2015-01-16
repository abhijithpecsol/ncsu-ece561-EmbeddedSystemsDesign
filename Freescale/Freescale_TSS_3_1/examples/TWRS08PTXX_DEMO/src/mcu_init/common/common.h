#ifndef _COMMON_H_
#define _COMMON_H_

/* 
 * Include the generic CPU header file 
 */
#include <derivative.h>
#include <hidef.h> /* for EnableInterrupts macro */

/*
 * Include common type definitions
 */

/* 
 * Include the cpu specific header file 
 */
/* 
 * Include the cpu specific header file 
 */

extern void Sys_Init(void);

/* Other Settings */
#define PT60_2N40C      1           /* 2N40C maskset has chip errata for SCIs */
#define PT60_OTHER      2

#define MASKSET         PT60_OTHER  /* 2N40C maskset has chip errata for SCIs */

/* 
 * Settings 
 */
/* define output bus clock */
#define OUTPUT_BUSCLK

/* define what clock mode at startup 
 * if USE_FEE is not defined by commenting out the following line, 
 * then it will use FEI mode which is default mode 
 * after reset  
 */
#define USE_FEE     /* FEE clock mode */    

/* define external crystal frequency */
#define EXT_CLK_CRYST   8000    /* in KHz */    /* PT60 tower board includes 8MHz crystal */   

/* define BUS clock in HZ */
#define BUS_CLK_HZ      16000000L
/* define BUS clock in kHZ */
#define CORE_CLK_KHZ    16000

#endif /* #ifndef _COMMON_H_ */
