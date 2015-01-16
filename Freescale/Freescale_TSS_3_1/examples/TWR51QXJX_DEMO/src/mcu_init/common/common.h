#include <hidef.h> /* for EnableInterrupts macro */
#include "derivative.h" /* include peripheral declarations */


#include "exceptions.h"
#include "startcf.h"
#include "stdlib.h"
/* 
 * Include common utilities
 */
#include "io.h"
#include "uart.h"
#include "irq_common.h"
#include "mcg.h"
#include "pmc_driver.h"
#include "llwu_common.h"

/*
* Include driver header files
*/

extern int print_str(const char *fmt, ...);


/*
 * Misc. Defines
 */
#ifdef	FALSE
#undef	FALSE
#endif
#define FALSE	(0)

#ifdef	TRUE
#undef	TRUE
#endif
#define	TRUE	(1)

#ifdef	NULL
#undef	NULL
#endif
#define NULL	(0)

#ifdef  ON
#undef  ON
#endif
#define ON      (1)

#ifdef  OFF
#undef  OFF
#endif
#define OFF     (0)

/*  Define the target for the code 
 *  tower
 *  FBDC
 *  baces board
 */
#define twrjf

/*
* System Bus Clock Info
*/
#define REF_CLK             XTAL8
#define CORE_CLK_MHZ        PLL50      /* system/core frequency in MHz */

/* Serial Port Info */
#define TERMINAL_BAUD       19200    
#define TERM_PORT           UART0_BASE_PTR

