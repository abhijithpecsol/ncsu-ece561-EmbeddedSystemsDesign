/*
 * File:		io.c
 * Purpose:		Serial Input/Output routines
 *
 * Notes:       TERMINAL_PORT defined in <board>.h
 */

#include "common.h"
#include "uart.h"

/********************************************************************/
char
in_char (void)
{
#if (TERM_PORT_NUM == 0)
    return uart0_getchar(TERM_PORT);
#else	
    return uart_getchar(TERM_PORT);
#endif
}
/********************************************************************/
void
out_char (char ch)
{
#if (TERM_PORT_NUM == 0)
    uart0_putchar(TERM_PORT, ch);
#else	
    uart_putchar(TERM_PORT, ch);
#endif	
}
/********************************************************************/
int
char_present (void)
{
#if (TERM_PORT_NUM == 0)
    return uart0_getchar_present(TERM_PORT);
#else
    return uart_getchar_present(TERM_PORT);
#endif
}
/********************************************************************/
