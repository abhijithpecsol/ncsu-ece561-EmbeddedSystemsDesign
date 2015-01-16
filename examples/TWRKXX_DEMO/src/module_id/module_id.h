/**HEADER********************************************************************
*
* Copyright (c) 2012 Freescale Semiconductor;
* All Rights Reserved
*
***************************************************************************
*
* THIS SOFTWARE IS PROVIDED BY FREESCALE "AS IS" AND ANY EXPRESSED OR
* IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
* OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL FREESCALE OR ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
* IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
* THE POSSIBILITY OF SUCH DAMAGE.
*
**************************************************************************
*
* $FileName: module_id.h$
* $Version : 3.6.17.0$
* $Date    : Sep-19-2012$
*
* Comments:
*
*   Detection of modules and devices on TOWER BOARD
*
*END************************************************************************/
#ifndef MODULE_ID_H
#define MODULE_ID_H

#include "derivative.h"
#include "board.h"

/* Standard macros */

#ifndef FALSE
  #define FALSE          0
#endif
#ifndef TRUE
  #define TRUE           1
#endif

/* Module_id settings */

#define MODULE_SLOT_CNT  2  //  number of all modules on TWR-SENSOR board

/* ADC modules */

#define TWR_ADC_MASK     0xC0
#define TWR_ADC0         0x00
#define TWR_ADC1         0x40
#define TWR_ADC2         0x80
#define TWR_ADC3         0xC0

#define TWRPI_ADC_RANGE       4096.0

#define TWRPI_RES_0     0.0
#define TWRPI_RES_1     1300.0
#define TWRPI_RES_2     3300.0
#define TWRPI_RES_3     5600.0
#define TWRPI_RES_4     10000.0
#define TWRPI_RES_5     18000.0
#define TWRPI_RES_6     30000.0
#define TWRPI_RES_7     62000.0

#define TWRPI_ID_TRESHOLD(res) (Word)((TWRPI_ADC_RANGE *(res))/(TWRPI_MAIN_RES + (res)))

#define TWRPI_ADC_TRESHOLD_1  (Word)((TWRPI_ID_TRESHOLD(TWRPI_RES_0) + TWRPI_ID_TRESHOLD(TWRPI_RES_1)) / 2)
#define TWRPI_ADC_TRESHOLD_2  (Word)((TWRPI_ID_TRESHOLD(TWRPI_RES_1) + TWRPI_ID_TRESHOLD(TWRPI_RES_2)) / 2)
#define TWRPI_ADC_TRESHOLD_3  (Word)((TWRPI_ID_TRESHOLD(TWRPI_RES_2) + TWRPI_ID_TRESHOLD(TWRPI_RES_3)) / 2)
#define TWRPI_ADC_TRESHOLD_4  (Word)((TWRPI_ID_TRESHOLD(TWRPI_RES_3) + TWRPI_ID_TRESHOLD(TWRPI_RES_4)) / 2)
#define TWRPI_ADC_TRESHOLD_5  (Word)((TWRPI_ID_TRESHOLD(TWRPI_RES_4) + TWRPI_ID_TRESHOLD(TWRPI_RES_5)) / 2)
#define TWRPI_ADC_TRESHOLD_6  (Word)((TWRPI_ID_TRESHOLD(TWRPI_RES_5) + TWRPI_ID_TRESHOLD(TWRPI_RES_6)) / 2)
#define TWRPI_ADC_TRESHOLD_7  (Word)((TWRPI_ID_TRESHOLD(TWRPI_RES_6) + TWRPI_ID_TRESHOLD(TWRPI_RES_7)) / 2)
#define TWRPI_ADC_TRESHOLD_8  (Word)((TWRPI_ID_TRESHOLD(TWRPI_RES_7) + TWRPI_ADC_RANGE) / 2)

/* Standard Types */

typedef unsigned char Byte;
typedef unsigned char byte;
typedef unsigned int Word;
typedef unsigned int word;

/* All supported devices on TWR-SENSOR board */
typedef enum
{
   TWRPI_UNKNOWN   = 0,
   TWRPI_UNPLUGGED = 1,
   TWRPI_KEYPAD    = 2,
   TWRPI_ROTARY    = 3,
   TWRPI_SLIDER    = 4,
   TWRPI_ROTARY2   = 5,
   TWRPI_GKEYPAD   = 6,
   TWRPI_TOUCHPAD  = 7,
   TWRPI_SHIELD1   = 8,
   TWRPI_SHIELD2   = 9,
   TWRPI_PROXIMITY = 10,
   // add new module number here
   TWRPI_NUMBER_MODULES       // do not modify
} TWRPI_DEVICES;

/* All supported modules on TWR-SENSOR board */
typedef enum
{
   MODULE_A     = 0,
   MODULE_B     = 1
} MODULES;

typedef struct {
 MODULES module;
 TWRPI_DEVICES prev_dev;
 TWRPI_DEVICES new_dev;
}TWRPI_MODULES_EVENTS;

#define TWRPI_BUFFER_EV_LEN 6

typedef struct{
  byte id0;
  byte id1;
}TWRPI_MODULES_ADC_CHANNELS;

typedef  TWRPI_MODULES_ADC_CHANNELS TWRPI_MODULES_IDS;

typedef struct{
  word range_0;
}TWRPI_MODULES_ADC_VALUES;


#define CALLBACK MODULE_ID_ChangedCB

/* CALLBACK Function for changing of current states */
#ifdef  CALLBACK
  void CALLBACK(TWRPI_DEVICES *module_id_twrpi_modules, MODULES module);
#endif

/* API Function */
Byte  MODULE_ID_Check_Modules(void);
Byte  MODULE_ID_Check_Module(MODULES module, TWRPI_DEVICES* dev);
TWRPI_DEVICES  MODULE_ID_Get_Device(MODULES module);
Byte MODULE_GetEvent(TWRPI_MODULES_EVENTS* pEvent);
void MODULE_ID_Init(void);
Byte MODULE_ID_Identify_Device(MODULES module);
#endif
