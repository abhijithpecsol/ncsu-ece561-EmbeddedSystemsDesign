KWIKSTIK_DEMO
==================================================================

This folder contains demo presenting TSS with the ARM®Cortex™-M4
Kinetis plaform MK40X256VMD100 on the KWIKSTIK board. The demo presents 
touch sensing function by 6 onboard electrodes. The sensing is 
performed by TSI module inside MCU. Touched button is displayed on the SLCD. 
Buzzer is turned on for 250ms with each touch.

Features & Information:
------------------------------------------------------------------
- KWIKSTIK board, MK40X256VMD100
- CodeWarrior for MCU v 10.1 or higher supported
- IAR Embedded Workbench for ARM Version 6.21 or higher supported
- Keil MDK-ARM 4.22 or higher supported
- TSI module performing sensing function
- FreeMASTER Visualization GUI supported via serial connection provided by 
  TWR-SER board connected to elevator boards 
- BUSclk 100MHz used
- Onboard J-Link programmer available
- Triggering function

Demo Control
------------------------------------------------------------------
- The touch is displayed on the SLCD display. Buzzer beeps when the electrode
  is touched.

FreeMASTER
------------------------------------------------------------------
Connection options:
- SCI RS232 serial connection via TWR-SER board, speed 115200 bauds

Notes
------------------------------------------------------------------
- Kwikstik board must be inserted into primary elevator module if serial
  communication is desired.
- Serial board needs to powered from elevator modules and the switch (PWR SW
  on primary elevator module) is switched to USB position (top).


References
------------------------------------------------------------------
KWIKSTIK User's Manual (Document KWIKSTIKK40UM)
KWIKSTIK Quick Start Guide (Document KWIKSTIKQSG)
Tower System Serial Module - User Manual (DOcument TWRSERUM)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2012, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
