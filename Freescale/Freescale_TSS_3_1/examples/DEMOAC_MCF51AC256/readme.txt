DEMOAC_MCF51AC256
==================================================================

This folder contains demo presenting TSS on the ColdFire®V1 plaform
MCF51AC256 connected with PROXIMITYEVM board. The demo presents 
touch sensing function by 8 electrode keypad.

Features & Information:
------------------------------------------------------------------
- Freescale DEMOAC development board, MCF51AC256
- KITPROXIMITYEVM board provides 8 electrodes multiplexed to 3 Controls (KEYPAD,SLIDER,ROTARY)
- CodeWarrior Development Studio for Microcontrollers version 6.3 or higher
- CodeWarrior for MCU v 10.1 or higher
- The application was created using built-in CodeWarrior project creation
  wizard (no Processor Expert support)
- FreeMASTER Visualization GUI supported
- Onboard P&E BDM Multilink available
- BUSclk 25MHz used	

Jumper Configuration 
------------------------------------------------------------------
VX_EN       = ON
PWR_SEL     = Set to BDM
USER        = All ON
JP2         = All OFF
I2C_PULL_EN = All OFF
USB_BG      = All ON
GP_IN       = OFF
GP_OUT      = OFF
DATA1       = ON
DATA2       = ON
RXD         = ON
TXD         = ON

* Not default settings

FreeMASTER
------------------------------------------------------------------
Connection options:
- P&E Multilink BDM Coldfire V1 connection <- Default

Additional Instructions
------------------------------------------------------------------
Please, connect KITPROXIMITYEVM board as written in TSSELECTRODEEVMQSG
document (KITPROXIMITYEVM connector pin 1 to DEMOAC main connector pin 1).

References
------------------------------------------------------------------
DEMOAC KIT Quick Start Guide (document DEMOACKIT_QSG)
DEMOAC User Manual (document DEMOAC_UG)
TSSELECTRODEEVM Quick Start Guide (document TSSELECTRODEEVMQSG)
Touch Sensing Software API Reference Manual (document TSSAPIRM)
Touch Sensing Software Users Guide (document TSSUG)


All mentioned documents are available on www.freescale.com webpage.

----
Copyright (c) 1999-2013, Freescale Semiconductor
ALL RIGHTS RESERVED, www.freescale.com
