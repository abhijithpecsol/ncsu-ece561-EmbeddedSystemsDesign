KWIKSTIK_DEMO_MQX
==================================================================

This folder contains demo (HMI) presenting MQX and TSS with the
ARM®Cortex™-M4 Kinetis plaform MK40X256VMD100 on the KWIKSTIK board.
The demo presents touch sensing functionality by 6 onboard electrodes. The
sensing is performed by the TSI module inside MCU.
Main task initializes the HMI layer and registers callbacks for buttons.
There are two different callbacks functions. A push callback prints on
the SLCD which button was touched and a release callback clears the SLCD.

Features & Information:
------------------------------------------------------------------
- KWIKSTIK board, MK40X256VMD100
- MQX RTOS 4.0.2
- CodeWarrior for MCU v 10.1 or higher supported
- IAR Embedded Workbench for ARM Version 6.5 or higher supported
- Serial connection provided by TWR-SER board connected to elevator boards
- Onboard J-Link programmer

Demo Control
------------------------------------------------------------------
-  The push callback function prints an input parameter to the terminal
   and displays an input parameter on the SLCD display.

Serial Communication
------------------------------------------------------------------
Connection options:
- SCI RS232 serial connection via TWR-SER board, speed 115200 bauds

Notes
------------------------------------------------------------------
- Kwikstik should be inserted into primary elevator module if serial
  communication is desired.
- Demo does not have set any starting breakpoint in CodeWarrior
  therefore it is running when debug starts. Because MQX libraries
  are precompiled without debug information.
- Serial board needs to be powered from elevator modules and a
  switch (PWR SW on the primary elevator module) is switched to USB
  position (top).
- MQX already installed is recommended, but not required for the demo
  purpose.

References
------------------------------------------------------------------
KWIKSTIK User's Manual (Document KWIKSTIKK40UM)
KWIKSTIK Quick Start Guide (Document KWIKSTIKQSG)
MQX Getting Started (Document FSL_MQX_Getting_Started)
MQX I/O User Guide (Document MQXIOUG)
Tower System Serial Module - User Manual (Document TWRSERUM)
Touch Sensing Software API Reference Manual (Document TSSAPIRM)
Touch Sensing Software Users Guide (Document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
