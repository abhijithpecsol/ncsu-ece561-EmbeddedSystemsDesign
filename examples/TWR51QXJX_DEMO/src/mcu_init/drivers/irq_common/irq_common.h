/*!
 * \file    spi_common.h
 * \brief   common SPI defines
 *
 * This file defines the functions/interrupt handlers/macros used for IRQ to be used as wakeup source.
 * And some common function prototypes and initializations.
 *
 * \version $Revision: 1.1 $
 * \author  Manish Bharti[b17254]
 */

#ifndef __IRQ_COMMON_H__
#define __IRQ_COMMON_H__

/* 
 * Misc. Defines
 */

/* 
 * Function prototypes
 */

void configure_irq_for_rising_edge(void);
void configure_irq_for_low_level(void);
void interrupt_handler_irq(void);
void disable_irq_for_rising_edge(void);

#endif /* __IRQ_COMMON_H__ */
