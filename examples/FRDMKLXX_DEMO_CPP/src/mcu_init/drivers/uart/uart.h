/*
 * File:		uart.h
 * Purpose:     Provide common ColdFire UART routines for polled serial IO
 *
 * Notes:
 */

#ifndef __UART_H__
#define __UART_H__

#include "derivative.h"

/********************************************************************/

#if  !defined(FRDM_KL05Z48) && !defined(FRDM_KL02Z48)
void uart_init (UART_MemMapPtr uartch, int sysclk, int baud);
char uart_getchar (UART_MemMapPtr channel);
void uart_putchar (UART_MemMapPtr channel, char ch);
int uart_getchar_present (UART_MemMapPtr channel);
#endif

#if defined(FRDM_KL25Z48) || defined(FRDM_KL05Z48) || defined(FRDM_KL02Z48)  || defined(FRDM_KL26Z48)
void uart0_init (UART0_MemMapPtr uartch, int sysclk, int baud);
char uart0_getchar (UART0_MemMapPtr channel);
void uart0_putchar (UART0_MemMapPtr channel, char ch);
int uart0_getchar_present (UART0_MemMapPtr channel);
#endif

/********************************************************************/

#endif /* __UART_H__ */
