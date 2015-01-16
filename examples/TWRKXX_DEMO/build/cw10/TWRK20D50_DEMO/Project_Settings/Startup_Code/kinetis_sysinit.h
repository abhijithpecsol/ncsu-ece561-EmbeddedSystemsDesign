/*
  FILE    : kinetis_sysinit.h
  PURPOSE : system initialization header for Kinetis ARM architecture
  LANGUAGE: C
  Copyright � 2010 Freescale semiConductor Inc. All Rights Reserved.
*/
#ifndef KINETIS_SYSINIT_H
#define KINETIS_SYSINIT_H

int pll_init(int crystal_val, unsigned char hgo_val, unsigned char erefs_val, signed char prdiv_val, signed char vdiv_val, unsigned char mcgout_select);


#if !(defined(SYNCH_MODE) | defined(NO_PLL_INIT))
  #define ASYNCH_MODE    /* PLL1 is source for MCGCLKOUT and DDR controller */
#endif

  /*
   * Input Clock Info
   */
  #define CLK0_FREQ_HZ        8000000
  #define CLK0_TYPE           CRYSTAL

  /*
   * PLL Configuration Info
   */
//  #define NO_PLL_INIT

/* The expected PLL output frequency is:
 * PLL out = (((CLKIN/PRDIV) x VDIV) / 2)
 * where the CLKIN can be either CLK0_FREQ_HZ or CLK1_FREQ_HZ.
 * 
 * For more info on PLL initialization refer to the mcg driver files.
 */

  #define PLL0_PRDIV      4
  #define PLL0_VDIV       25



  /*
   * System Bus Clock Info
   */
  #define MGG_OSC_MODE        XTAL_MODE     // EXTOSC_MODE or XTAL_MODE
  #define REF_CLK             XTAL8         // see available frequencies in osc_frequency   in mcg.h
  #define CORE_CLK_MHZ        PLL50         // see pll_options  frequencies in              in mcg.h


// Constants for use in pll_init
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

// MCG Mode defines
/*
#define FEI  1
#define FEE  2
#define FBI  3
#define FBE  4
#define BLPI 5
#define BLPE 6
#define PBE  7
#define PEE  8
*/

#define BLPI 1
#define FBI  2
#define FEI  3
#define FEE  4
#define FBE  5
#define BLPE 6
#define PBE  7
#define PEE  8

// IRC defines
#define SLOW_IRC 0
#define FAST_IRC 1

#ifdef __cplusplus
extern "C" {
#endif

/* Address of Watchdog Unlock Register (16 bits) */
#define KINETIS_WDOG_UNLOCK_ADDR	0x4005200E

/* Address of Watchdog Status and Control Register High (16 bits) */
#define KINETIS_WDOG_STCTRLH_ADDR	0x40052000

/* Unlocking Watchdog sequence words*/
#define KINETIS_WDOG_UNLOCK_SEQ_1	0xC520
#define KINETIS_WDOG_UNLOCK_SEQ_2	0xD928

/* Word to be written in in STCTRLH after unlocking sequence in order to disable the Watchdog */
#define KINETIS_WDOG_DISABLED_CTRL	0xD2

/* 
	Initializes the Kinetis hardware: e.g. disables the Watchdog
*/
void __init_hardware();

/*
** ===================================================================
**     Method      :  isr_default
**
**     Description :
**         The default ISR.
** ===================================================================
*/
void isr_default(void);

/*
** ===================================================================
**     Method      :  isrINT_NMI
**
**     Description :
**         This ISR services the Non Maskable Interrupt interrupt.
** ===================================================================
*/
void isrINT_NMI(void);

#ifdef __cplusplus
}
#endif

/* Interrupt Vector Table Function Pointers */

#define VECTOR_003      isr_default     // 0x0000_000C 3 -          ARM core        Hard Fault
#define VECTOR_004      isr_default     // 0x0000_0010 4 -
#define VECTOR_005      isr_default     // 0x0000_0014 5 -          ARM core         Bus Fault
#define VECTOR_006      isr_default     // 0x0000_0018 6 -          ARM core         Usage Fault
#define VECTOR_007      isr_default     // 0x0000_001C 7 -                           
#define VECTOR_008      isr_default     // 0x0000_0020 8 -                           
#define VECTOR_009      isr_default     // 0x0000_0024 9 -
#define VECTOR_010      isr_default     // 0x0000_0028 10 -
#define VECTOR_011      isr_default     // 0x0000_002C 11 -         ARM core         Supervisor call (SVCall)
#define VECTOR_012      isr_default     // 0x0000_0030 12 -         ARM core         Debug Monitor
#define VECTOR_013      isr_default     // 0x0000_0034 13 -                          
#define VECTOR_014      isr_default     // 0x0000_0038 14 -         ARM core         Pendable request for system service (PendableSrvReq)
#define VECTOR_015      isr_default     // 0x0000_003C 15 -         ARM core         System tick timer (SysTick)
#define VECTOR_016      isr_default     // 0x0000_0040 16     0     DMA              DMA Channel 0 transfer complete
#define VECTOR_017      isr_default     // 0x0000_0044 17     1     DMA              DMA Channel 1 transfer complete
#define VECTOR_018      isr_default     // 0x0000_0048 18     2     DMA              DMA Channel 2 transfer complete
#define VECTOR_019      isr_default     // 0x0000_004C 19     3     DMA              DMA Channel 3 transfer complete
#define VECTOR_020      isr_default     // 0x0000_0050 20     4     DMA              DMA Channel 4 transfer complete
#define VECTOR_021      isr_default     // 0x0000_0054 21     5     DMA              DMA Channel 5 transfer complete
#define VECTOR_022      isr_default     // 0x0000_0058 22     6     DMA              DMA Channel 6 transfer complete
#define VECTOR_023      isr_default     // 0x0000_005C 23     7     DMA              DMA Channel 7 transfer complete
#define VECTOR_024      isr_default     // 0x0000_0060 24     8     DMA              DMA Channel 8 transfer complete
#define VECTOR_025      isr_default     // 0x0000_0064 25     9     DMA              DMA Channel 9 transfer complete
#define VECTOR_026      isr_default     // 0x0000_0068 26    10     DMA              DMA Channel 10 transfer complete
#define VECTOR_027      isr_default     // 0x0000_006C 27    11     DMA              DMA Channel 11 transfer complete
#define VECTOR_028      isr_default     // 0x0000_0070 28    12     DMA              DMA Channel 12 transfer complete
#define VECTOR_029      isr_default     // 0x0000_0074 29    13     DMA              DMA Channel 13 transfer complete
#define VECTOR_030      isr_default     // 0x0000_0078 30    14     DMA              DMA Channel 14 transfer complete
#define VECTOR_031      isr_default     // 0x0000_007C 31    15     DMA              DMA Channel 15 transfer complete
#define VECTOR_032      isr_default     // 0x0000_0080 32    16     DMA              DMA Error Interrupt Channels 0-15
#define VECTOR_033      isr_default     // 0x0000_0084 33    17     MCM              Normal interrupt
#define VECTOR_034      isr_default     // 0x0000_0088 34    18     Flash memory     Command Complete
#define VECTOR_035      isr_default     // 0x0000_008C 35    19     Flash memory     Read Collision
#define VECTOR_036      isr_default     // 0x0000_0090 36    20     Mode Controller  Low Voltage Detect,Low Voltage Warning, Low Leakage Wakeup
#define VECTOR_037      isr_default     // 0x0000_0094 37    21     LLWU
#define VECTOR_038      isr_default     // 0x0000_0098 38    22     WDOG
#define VECTOR_039      isr_default     // 0x0000_009C 39    23		RNGB
#define VECTOR_040      isr_default     // 0x0000_00A0 40    24     I2C0
#define VECTOR_041      isr_default     // 0x0000_00A4 41    25     I2C1
#define VECTOR_042      isr_default     // 0x0000_00A8 42    26     SPI0             Single interrupt vector for all sources
#define VECTOR_043      isr_default     // 0x0000_00AC 43    27     SPI1             Single interrupt vector for all sources
#define VECTOR_044      isr_default     // 0x0000_00B0 44    28     SPI2             Single interrupt vector for all sources
#define VECTOR_045      isr_default     // 0x0000_00B4 45    29     CAN0             OR'ed Message buffer (0-15)
#define VECTOR_046      isr_default     // 0x0000_00B8 46    30     CAN0             Bus Off
#define VECTOR_047      isr_default     // 0x0000_00BC 47    31     CAN0             Error
#define VECTOR_048      isr_default     // 0x0000_00C0 48    32     CAN0             Transmit Warning
#define VECTOR_049      isr_default     // 0x0000_00C4 49    33     CAN0             Receive Warning
#define VECTOR_050      isr_default     // 0x0000_00C8 50    34     CAN0             Wake Up
#define VECTOR_051      isr_default     // 0x0000_00CC 51    35     CAN0             Individual Matching Elements Update (IMEU)
#define VECTOR_052      isr_default     // 0x0000_00D0 52    36     CAN0             Lost receive
#define VECTOR_053      isr_default     // 0x0000_00D4 53    37     CAN1             OR'ed Message buffer (0-15)
#define VECTOR_054      isr_default     // 0x0000_00D8 54    38     CAN1             Bus off
#define VECTOR_055      isr_default     // 0x0000_00DC 55    39     CAN1             Error
#define VECTOR_056      isr_default     // 0x0000_00E0 56    40     CAN1             Transmit Warning
#define VECTOR_057      isr_default     // 0x0000_00E4 57    41     CAN1             Receive Warning
#define VECTOR_058      isr_default     // 0x0000_00E8 58    42     CAN1             Wake Up
#define VECTOR_059      isr_default     // 0x0000_00EC 59    43     CAN1             Individual Matching Elements Update (IMEU)
#define VECTOR_060      isr_default     // 0x0000_00F0 60    44     CAN1             Lost receive 
#define VECTOR_061      isr_default     // 0x0000_00F4 61    45     UART0            Single interrupt vector for UART status sources



#endif /* #ifndef KINETIS_SYSINIT_H */
