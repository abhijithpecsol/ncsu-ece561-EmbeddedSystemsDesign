FRDMKLXX_DEMO_CPP
==================================================================

This demo presents C++ wrapper for TSS on freedom boards. C++ wrapper
files are located in LIB/wrapper_cpp.
For further information, read doxygen comments in the TSS_cpp header
file.

Features & Information:
------------------------------------------------------------------
- C++ TSS wrapper
- FRDM-KL02Z evaluation board, KL02Z32
- FRDM-KL05Z evaluation board, KL05Z32
- FRDM-KL25Z evaluation board, KL25Z128
- FRDM-KL26Z evaluation board, KL26Z128
- CodeWarrior for MCU v 10.3 or higher supported
- IAR Embedded Workbench for ARM Version 6.50.2 or higher supported
- KEIL uVision 4.71.2 or higher supported
- TSI module performing sensing function
- Open SDA debug interface available
- JTAG programing port available for J-Link Connection

Demo Control
------------------------------------------------------------------
- LED indicates if there has been a movement detected on the slider
  and the reported position is higher than 20. Otherwise LED is off.

Demo Settings
------------------------------------------------------------------
- Each board has a separate board code file and a header file.

References
------------------------------------------------------------------
FRDM-KL02Z48M User's Manual
FRDM-KL05Z48M User's Manual
FRDM-KL25Z48M User's Manual
FRDM-KL46Z48M User's Manual
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

------------------------------------------------------------------
Copyright (c) 2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
