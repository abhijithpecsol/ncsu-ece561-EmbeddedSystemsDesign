DEMOQE_MCF51QE128
==================================================================

This folder contains demo presenting TSS on the ColdFire®V1 plaform
MCF51QE128 connected with PROXIMITYEVM board. The demo presents 
touch sensing function by 8 electrode keypad.

Features & Information:
------------------------------------------------------------------
- Freescale DEMOQE128 development board, MCF51QE128
- KITPROXIMITYEVM board provides 8 electrodes multiplexed to 3 Controls (KEYPAD,SLIDER,ROTARY)
- CodeWarrior Development Studio for Microcontrollers version 6.3 or higher
- CodeWarrior for MCU v 10.1 or higher
- The application was created using built-in CodeWarrior project creation
  wizard (no Processor Expert support)
- FreeMASTER Visualization GUI supported
- Onboard P&E BDM Multilink available
- BUSclk 25MHz used	

Jumper Configuration 
(The settings listed indicates the “on” (or installed) position):
------------------------------------------------------------------
J3  = 2&3
J4  = 3&4
J5  = 1&2,5&6 *
J6  = 1&2 *
J7  = 1&2 *
J8  = 1&2 *
J9  = 9&10,11&12,13&14,15&16 *
J11 = both off *
J12 = 5&6,7&8 *
J13 = 2&3
J14 = 2&3
J15 = 2&3
J16 = 1&2,3&4,7&8
J18 = 1&2,3&4
J19 = off *
J20 = 1&2,3&4
J21 = 1&2,3&4

* Not default settings

FreeMASTER
------------------------------------------------------------------
Connection options:
- P&E Multilink BDM Coldfire V1 connection - Default
- SCI RS232 serial connection, speed 57600 bauds

Additional Instructions
------------------------------------------------------------------
Please, connect KITPROXIMITYEVM board as written in TSSELECTRODEEVMQSG
document (KITPROXIMITYEVM connector pin 1 to DEMOQE128 main connector pin 1).

References
------------------------------------------------------------------
DEMOQE128 User Manual (document DEMOQE128UM)
TSSELECTRODEEVM Quick Start Guide (document TSSELECTRODEEVMQSG)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
