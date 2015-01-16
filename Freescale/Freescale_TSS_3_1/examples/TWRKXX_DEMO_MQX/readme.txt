TWRKXX_DEMO_MQX
==================================================================

This folder contains demo presenting MQX RTOS and TSS with the ARM®Cortex™-M4
Kinetis plaform MK40X256VMD100, MK53N512CMD100, MK60N512VMD100 and MK70F12 on the
TWRK40X256, TWRK53N512, TWRK60N512 and TWRK70F120M board. The demo presents touch
sensing function by 4 onboard electrodes, a daughter board TWRPI-KEYPAD
(12 electrodes) and a daughter board TWRPI-ROTARY (9 electrodes).
Two kinds of controls (KEYPAD, ROTARY) are supported. The sensing is
performed by the TSI module inside MCU.

Features & Information:
------------------------------------------------------------------
- TWRK40X256 tower board, MK40X256VMD100
- TWRK60N512 tower board, MK60N512VMD100
- TWRK53N512 tower board, MK53N512CMD100
- TWRK70F120M tower board, MK70F12
- CodeWarrior for MCU v 10.1 or higher supported
- MQX RTOS 4.0.2
- IAR Embedded Workbench for ARM Version 6.5 or higher supported
- TSI module performing sensing function
- Serial connection provided by OSBDM/OSJTAG
- Onboard OpenSource JTAG (OSJTAG) programmer available
- JTAG programing port available for J-Link Connection
- For more details, see MQX Getting Started.

Demo Control
------------------------------------------------------------------
- Module is selected with shell commands, on board buttons are set
  by default. Help command in a shell is available.
  Module 0 represents on board buttons. Module 1 represents a rotary,
  module 2 represents a keypad.
- If the module is plugged into the TWRPI, select the module using
  an appropriate shell command (module number).
- The touch is indicated by four onboard LED's. Onboard electrodes
  indicate each touch by related electrode's LED. TWRPI-KEYPAD daughter
  board indicates which electrode was touched/released with first four
  LED's. The rest of the electrodes print the action (push/release) to
  the terminal. TWRPI-ROTARY callback is called if there is a movement
  detected.

Serial Communication
------------------------------------------------------------------
Connection options:
- Serial IO channel routed via combined debugging and a communication port
  (OSBDM/OSJTAG) directly on the board. Speed 115200 bauds.
- To download terminal utility (virtual OSBDM toolkit) or update the OSBDM
  firmware, please visit:
  http://www.pemicro.com/osbdm/index.cfm

Notes
------------------------------------------------------------------
- There is a conflict with TWRK60N512 and TWR-SER board. It's caused by
  TSI electrodes and Ethernet peripheral. If TWRPI-KEYPAD is used
  the EL1 (within control) is not functional. If TWRPI-ROTARY is used
  the EL8 (within control) is not functional.
- In case the fault occurs, an electrode is disabled and not re-enabled again.
- MQX already installed is recommended, but not required for the demo
  purpose.

References
------------------------------------------------------------------
TWR-K40X256 User's Manual (Document TWR-K40X256-UM)
TWR-K53N512 User's Manual (Document TWR-K53N512-UM)
TWR-K60N512 User's Manual (Document TWR-K60N512-UM)
TWR-K70F120M User's Manual (Document TWRK70F120MUM)
TWR-K40X256-KIT Quick Start Guide (Document TWRK40QSG)
TWR-K53N512-KIT Quick Start Guide (Document TWRK53QSG)
TWR-K60N512-KIT Quick Start Guide (Document TWRK60QSG)
TWR-K70F120M-KIT Quick Start Guide (Document TWRK70F120MQSG)
MQX Getting Started (Document FSL_MQX_Getting_Started)
MQX I/O User Guide (Document MQXIOUG)
Tower System Serial Module - User Manual (DOcument TWRSERUM)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
