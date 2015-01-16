/*
  FILE    : kinetis_sysinit.h
  PURPOSE : system initialization header for Kinetis ARM architecture
  LANGUAGE: C
  Copyright © 2012 Freescale semiConductor Inc. All Rights Reserved.
*/
#ifndef KINETIS_SYSINIT_H
#define KINETIS_SYSINIT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Word to be written in SIM_COP in order to disable the Watchdog */
#define KINETIS_WDOG_DISABLED_CTRL	0x0

/* Constants for use in pll_init */
#define NO_OSCINIT 0
#define OSCINIT 1

#define OSC_0 0
#define OSC_1 1

#define LOW_POWER 0
#define HIGH_GAIN 1

#define CANNED_OSC  0
#define CRYSTAL 1

#define PLL_0 0
#define PLL_1 1

#define PLL_ONLY 0
#define MCGOUT 1

/* Define the clock parameters on this hardware platform */
#define CLK0_FREQ_HZ        8000000
#define CLK0_FREQ_MHZ       8
#define CLK0_TYPE           CRYSTAL
#undef  NO_PLL_INIT
#define PLL0_PRDIV          (CLK0_FREQ_MHZ/2)
#define PLL0_VDIV           24

/* 
	Initializes the Kinetis hardware: e.g. disables the Watchdog
*/
void __init_hardware();

/*
** ===================================================================
**     Method      :  Default_Handler
**
**     Description :
**         The default interrupt handler.
** ===================================================================
*/
void Default_Handler();

#ifdef __cplusplus
}
#endif

#define VECTOR_003      Default_Handler     // 0x0000_000C 3 -          ARM core        Hard Fault
#define VECTOR_004      Default_Handler     // 0x0000_0010 4 -
#define VECTOR_005      Default_Handler     // 0x0000_0014 5 -          ARM core         Bus Fault
#define VECTOR_006      Default_Handler     // 0x0000_0018 6 -          ARM core         Usage Fault
#define VECTOR_007      Default_Handler     // 0x0000_001C 7 -                           
#define VECTOR_008      Default_Handler     // 0x0000_0020 8 -                           
#define VECTOR_009      Default_Handler     // 0x0000_0024 9 -
#define VECTOR_010      Default_Handler     // 0x0000_0028 10 -
#define VECTOR_011      Default_Handler     // 0x0000_002C 11 -         ARM core         Supervisor call (SVCall)
#define VECTOR_012      Default_Handler     // 0x0000_0030 12 -         ARM core         Debug Monitor
#define VECTOR_013      Default_Handler     // 0x0000_0034 13 -                          
#define VECTOR_014      Default_Handler     // 0x0000_0038 14 -         ARM core         Pendable request for system service (PendableSrvReq)
#define VECTOR_015      Default_Handler     // 0x0000_003C 15 -         ARM core         System tick timer (SysTick)
#define VECTOR_016      Default_Handler     // 0x0000_0040 16     0     DMA              DMA Channel 0 transfer complete
#define VECTOR_017      Default_Handler     // 0x0000_0044 17     1     DMA              DMA Channel 1 transfer complete
#define VECTOR_018      Default_Handler     // 0x0000_0048 18     2     DMA              DMA Channel 2 transfer complete
#define VECTOR_019      Default_Handler     // 0x0000_004C 19     3     DMA              DMA Channel 3 transfer complete
#define VECTOR_020      Default_Handler     // 0x0000_0050 20     4     DMA              DMA Channel 4 transfer complete
#define VECTOR_021      Default_Handler     // 0x0000_0054 21     5     DMA              DMA Channel 5 transfer complete
#define VECTOR_022      Default_Handler     // 0x0000_0058 22     6     DMA              DMA Channel 6 transfer complete
#define VECTOR_023      Default_Handler     // 0x0000_005C 23     7     DMA              DMA Channel 7 transfer complete
#define VECTOR_024      Default_Handler     // 0x0000_0060 24     8     DMA              DMA Channel 8 transfer complete
#define VECTOR_025      Default_Handler     // 0x0000_0064 25     9     DMA              DMA Channel 9 transfer complete
#define VECTOR_026      Default_Handler     // 0x0000_0068 26    10     DMA              DMA Channel 10 transfer complete
#define VECTOR_027      Default_Handler     // 0x0000_006C 27    11     DMA              DMA Channel 11 transfer complete
#define VECTOR_028      Default_Handler     // 0x0000_0070 28    12     DMA              DMA Channel 12 transfer complete
#define VECTOR_029      Default_Handler     // 0x0000_0074 29    13     DMA              DMA Channel 13 transfer complete
#define VECTOR_030      Default_Handler     // 0x0000_0078 30    14     DMA              DMA Channel 14 transfer complete
#define VECTOR_031      Default_Handler     // 0x0000_007C 31    15     DMA              DMA Channel 15 transfer complete
#define VECTOR_032      Default_Handler     // 0x0000_0080 32    16     DMA              DMA Error Interrupt Channels 0-15
#define VECTOR_033      Default_Handler     // 0x0000_0084 33    17     MCM              Normal interrupt
#define VECTOR_034      Default_Handler     // 0x0000_0088 34    18     Flash memory     Command Complete
#define VECTOR_035      Default_Handler     // 0x0000_008C 35    19     Flash memory     Read Collision
#define VECTOR_036      Default_Handler     // 0x0000_0090 36    20     Mode Controller  Low Voltage Detect,Low Voltage Warning, Low Leakage Wakeup
#define VECTOR_037      Default_Handler     // 0x0000_0094 37    21     LLWU
#define VECTOR_038      Default_Handler     // 0x0000_0098 38    22     WDOG
#define VECTOR_039      Default_Handler     // 0x0000_009C 39    23   RNGB
#define VECTOR_040      Default_Handler     // 0x0000_00A0 40    24     I2C0
#define VECTOR_041      Default_Handler     // 0x0000_00A4 41    25     I2C1
#define VECTOR_042      Default_Handler     // 0x0000_00A8 42    26     SPI0             Single interrupt vector for all sources
#define VECTOR_043      Default_Handler     // 0x0000_00AC 43    27     SPI1             Single interrupt vector for all sources
#define VECTOR_044      Default_Handler     // 0x0000_00B0 44    28     SPI2             Single interrupt vector for all sources
#define VECTOR_045      Default_Handler     // 0x0000_00B4 45    29     CAN0             OR'ed Message buffer (0-15)
#define VECTOR_046      Default_Handler     // 0x0000_00B8 46    30     CAN0             Bus Off
#define VECTOR_047      Default_Handler     // 0x0000_00BC 47    31     CAN0             Error

#endif /* #ifndef KINETIS_SYSINIT_H */
