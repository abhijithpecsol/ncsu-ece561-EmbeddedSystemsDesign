/**HEADER********************************************************************
*
* Copyright (c) 2011 Freescale Semiconductor;
* All Rights Reserved
*
* Copyright (c) 1989-2008 ARC International;
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
* $FileName: Driver_SLCD.h$
* $Version : 3.6.2.0$
* $Date    : Feb-3-2012$
*
* Comments:
*
*   This file contains the header for SLCD.
*
*END************************************************************************/
#ifndef _DRIVER_SLCD_H_
#define _DRIVER_SLCD_H_

  #include "TSS_DataTypes.h"
  #include "derivative.h"
  #include "Fonts_LCD.h"

  /**
    Global defines
  */
  #define MAX_COLUMNS_PER_CHAR                        4
  #define CHAR_RESET_POSITION                         5
  #define CHAR_MAX_POSITION_VALUE                     34
  #define CHAR_POSITION_INCREMENT                     6
  #define MARQUEE_MAX_POSITION_WITHOUT_ARROW          37
  #define MARQUEE_MAX_POSITION_WITH_ARROW             35
  #define MARQUEE_MIN_POSITION_WITHOUT_ARROW          0
  #define MARQUEE_MIN_POSITION_WITH_ARROW             2
  #define MARQUEE_WITH_ARROWS                         1
  #define MARQUEE_WITHOUT_ARROWS                      0

  #define FRONT_PLANE_0_TO_3                          0
  #define FRONT_PLANE_4_TO_7                          4
  #define FRONT_PLANE_8_TO_11                         8
  #define FRONT_PLANE_12_TO_15                        12
  #define FRONT_PLANE_16_TO_19                        16
  #define FRONT_PLANE_20_TO_23                        20
  #define FRONT_PLANE_24_TO_27                        24
  #define FRONT_PLANE_28_TO_31                        28
  #define FRONT_PLANE_32_TO_35                        32
  #define FRONT_PLANE_36_TO_39                        36
  #define FRONT_PLANE_40_TO_43                        40
  #define FRONT_PLANE_44_TO_47                        44

  #define UPPER_LEFT                                  0
  #define MIDDLE_LEFT                                 1
  #define LOWER_LEFT                                  2
  #define UPPER_RIGHT                                 3
  #define MIDDLE_RIGHT                                4
  #define LOWER_RIGHT                                 5

  #define CHARGED                                     2
  #define MID_LEVEL                                   1
  #define LOWER_LEVEL                                 0

  #define ARROWS                                      1
  #define NO_ARROWS                                   0

  /**
    Enumeration for the backplane phases
*/
  typedef enum
  {
    PHASE_A = 0,
    PHASE_B,
    PHASE_C,
    PHASE_D,
    PHASE_E,
    PHASE_F,
    PHASE_G,
    PHASE_H
  }eBackPlanePhases;

  /**
    External Variables
  */
  extern uint8_t isMarqueeRestart;

  /**
    Public Function Prototypes
  */
  void _SLCDModule_Init(void);

  void _SLCDMoudle_TurnOnAllSegments(void);
  void _SLCDMoudle_TurnOffAllSegments(void);
  void _SLCDModule_SetSegment(uint8_t X, uint8_t Y);
  void _SLCDModule_ClearSegment(uint8_t X, uint8_t Y);
  void _SLCDModule_PrintString(char * SourceString, uint8_t StartPosition);
  void _SLCDModule_ClearLCD(uint8_t ArrowsOn);
  
  void _SLCDModule_PrintNumber(uint8_t Number, uint8_t Position);

  void _SLCDModule_TurnOnPounceSign(void);
  void _SLCDModule_TurnOnFreescaleSign(void);
  void _SLCDModule_TurnOnJLinkSign(void);
  void _SLCDModule_TurnOnBatteryOutline(void);
  void _SLCDModule_TurnOnBattery(uint8_t BatteryLevel);

  void _SLCDModule_TurnOffPounceSign(void);
  void _SLCDModule_TurnOffFreescaleSign(void);
  void _SLCDModule_TurnOffJLinkSign(void);
  void _SLCDModule_TurnOffBattery(void);

#endif/*_DRIVER_SLCD_H_*/
