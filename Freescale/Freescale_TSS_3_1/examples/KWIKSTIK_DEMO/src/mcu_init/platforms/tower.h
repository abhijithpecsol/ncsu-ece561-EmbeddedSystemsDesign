/*
 * File:        tower.h
 * Purpose:     Kinetis tower CPU card definitions
 *
 * Notes:
 */

#ifndef __TOWER_H__
#define __TOWER_H__

#include "mcg.h"

/********************************************************************/

/* Global defines to use for all Firebird daughter cards */
#define DEBUG_PRINT
#define TWR_USE_SERIAL_BOARD       1


  #define CPU_MK40X256VMD100

  /*
   * System Bus Clock Info
   */
  #define REF_CLK                  XTAL4
  #define CORE_CLK_MHZ             PLL100      /* system/core frequency in MHz */

  /* Serial Port Info */
  #if TWR_USE_SERIAL_BOARD
    #define TERM_PORT              UART5_BASE_PTR
    #define TERM_PORT_VECTOR       71
  #else
    #define TERM_PORT              UART0_BASE_PTR
    #define TERM_PORT_VECTOR       61
  #endif
  #define TERMINAL_BAUD            115200
  #undef  HW_FLOW_CONTROL



#endif /* __TOWER_H__ */
