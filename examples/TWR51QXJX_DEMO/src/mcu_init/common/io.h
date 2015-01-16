/*
 * File:		io.h
 * Purpose:		Serial Input/Output routines
 *
 */

#ifndef _IO_H
#define _IO_H
extern char uart_getchar (UART_MemMapPtr);
extern void uart_putchar (UART_MemMapPtr, char);
extern char uart_getchar_present (UART_MemMapPtr);

/********************************************************************/

char	
in_char(void);

void
out_char(char);

char
char_present(void);

int		
printf(const char *, ... );

int
sprintf(char *, const char *, ... );

/********************************************************************/

#endif
