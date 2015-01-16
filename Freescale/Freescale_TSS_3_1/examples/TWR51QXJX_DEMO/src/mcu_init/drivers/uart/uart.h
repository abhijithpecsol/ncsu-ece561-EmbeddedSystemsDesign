/*!
 * \file    uart.h
 * \brief   UART Driver
 *
 * This driver provides an initialization routine as well as the basic
 * UART I/O functions
 *
 * \version $Revision: 1.2 $
 * \author  Michael Norman
 */

#ifndef __UART_H__
#define __UART_H__


interrupt void UART0_ISR(void);
interrupt void UART1_ISR(void);

/********************************************************************/
/*!
 * \brief   UART Configuration Information
 * 
 * This structure is used to pass UART configuration information to
 * the the UART driver.
 */
typedef struct 
{
    unsigned char chan;     /*!< UART channel to initialize */
    unsigned char dbits;    /*!< Data bits per character */
    unsigned char sbits;    /*!< Stop bits */
    unsigned char parity;   /*!< Parity Mode/Type */
    unsigned char dma;      /*!< Use DMA flag */
    unsigned char ffull;    /*!< Interrupt/DMA Request on FIFO Full flag */
    unsigned char flow;     /*!< Hardware Flow Control flag */
    unsigned char clksrc;   /*!< Source of the UART Clock */
    int   clkfreq;  /*!< UART Clock Frequency in Hz */
    int   baud;     /*!< Desired Baud Rate in bits/second */
} UART_INFO;

/* 
 * UART Data Bits
 */
#define UART_DATA_BITS_8        8   /* 8 Data bits */
#define UART_DATA_BITS_7        7   /* 7 Data bits */
#define UART_DATA_BITS_6        6   /* 6 Data bits */
#define UART_DATA_BITS_5        5   /* 5 Data bits */

/* 
 * UART Stop Bits
 */
#define UART_STOP_BITS_1        0   /* 1 Stop bit */
#define UART_STOP_BITS_1p5      1   /* 1.5 Stop bits */
#define UART_STOP_BITS_2        2   /* 2 Stop bits */

/*
 * Parity Mode/Type Parameter Encodings
 */
#define UART_PARITY_EVEN        0   /*!< Even Parity */
#define UART_PARITY_ODD         1   /*!< Odd Parity */
#define UART_PARITY_LOW         2   /*!< Force Parity Low */
#define UART_PARITY_HIGH        3   /*!< Force Parity High */
#define UART_PARTIY_NONE        4   /*!< No Parity */
#define UART_PARITY_MD_DATA     5   /*!< Multidrop Mode - Data Character */
#define UART_PARITY_MD_ADDR     6   /*!< Multidrop Mode - Address Character */

/********************************************************************/

void uart_init (UART_MemMapPtr, int, int);
char uart_getchar (UART_MemMapPtr);
void uart_putchar (UART_MemMapPtr, char);
char uart_getchar_present (UART_MemMapPtr);

/********************************************************************/

#endif /* __UART_H__ */
