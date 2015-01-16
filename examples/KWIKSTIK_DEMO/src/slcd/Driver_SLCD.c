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
* $FileName: Driver_SLCD.c$
* $Version : 3.6.2.0$
* $Date    : Feb-6-2012$
*
* Comments:
*
*   This file contains the source for the SLCD. This file was edited
*   for purpose of this demo.
*
*END************************************************************************/
#include "Driver_SLCD.h"

/**
  Global Constants
*/
const uint8_t WaveFormShiftRegisterTable[] =
{
  16,               // FrontPlane 2
  24,               // FrontPlane 3
  0,                // FrontPlane 4
  8,                // FrontPlane 5
  16,               // FrontPlane 6
  24,               // FrontPlane 7
  0,                // FrontPlane 8
  8,                // FrontPlane 9
  16,               // FrontPlane 10
  24,               // FrontPlane 11
  0,                // FrontPlane 12
  8,                // FrontPlane 13
  16,               // FrontPlane 14
  24,               // FrontPlane 15
  0,                // FrontPlane 16
  8,                // FrontPlane 17
  16,               // FrontPlane 18
  24,               // FrontPlane 19
  0,                // FrontPlane 20
  8,                // FrontPlane 21
  16,               // FrontPlane 22
  24,               // FrontPlane 23
  0,                // FrontPlane 24
  8,                // FrontPlane 25
  16,               // FrontPlane 26
  24,               // FrontPlane 27
  0,                // FrontPlane 28
  8,                // FrontPlane 29
  16,               // FrontPlane 30
  24,               // FrontPlane 31
  0,                // FrontPlane 32
  8,                // FrontPlane 33
  16,               // FrontPlane 34
  24,               // FrontPlane 35
  0,                // FrontPlane 36
  8,                // FrontPlane 37
  16,               // FrontPlane 38
};

const uint32_t WaveFormRegisterClearTable[] =
{
  0xFF00FFFF,               // FrontPlane 2
  0x00FFFFFF,               // FrontPlane 3
  0xFFFFFF00,               // FrontPlane 4
  0xFFFF00FF,               // FrontPlane 5
  0xFF00FFFF,               // FrontPlane 6
  0x00FFFFFF,               // FrontPlane 7
  0xFFFFFF00,               // FrontPlane 8
  0xFFFF00FF,               // FrontPlane 9
  0xFF00FFFF,               // FrontPlane 10
  0x00FFFFFF,               // FrontPlane 11
  0xFFFFFF00,               // FrontPlane 12
  0xFFFF00FF,               // FrontPlane 13
  0xFF00FFFF,               // FrontPlane 14
  0x00FFFFFF,               // FrontPlane 15
  0xFFFFFF00,               // FrontPlane 16
  0xFFFF00FF,               // FrontPlane 17
  0xFF00FFFF,               // FrontPlane 18
  0x00FFFFFF,               // FrontPlane 19
  0xFFFFFF00,               // FrontPlane 20
  0xFFFF00FF,               // FrontPlane 21
  0xFF00FFFF,               // FrontPlane 22
  0x00FFFFFF,               // FrontPlane 23
  0xFFFFFF00,               // FrontPlane 24
  0xFFFF00FF,               // FrontPlane 25
  0xFF00FFFF,               // FrontPlane 26
  0x00FFFFFF,               // FrontPlane 27
  0xFFFFFF00,               // FrontPlane 28
  0xFFFF00FF,               // FrontPlane 29
  0xFF00FFFF,               // FrontPlane 30
  0x00FFFFFF,               // FrontPlane 31
  0xFFFFFF00,               // FrontPlane 32
  0xFFFF00FF,               // FrontPlane 33
  0xFF00FFFF,               // FrontPlane 34
  0x00FFFFFF,               // FrontPlane 35
  0xFFFFFF00,               // FrontPlane 36
  0xFFFF00FF,               // FrontPlane 37
  0xFF00FFFF,               // FrontPlane 38
};

/**
  Global Variables
*/
char * Error = {"ERROR"};

/**
  Local Functions Prototypes
*/
void _SLCDModule_StringWFRegisters(uint8_t FrontPlaneValues, uint8_t FrontPlanePosition);
void _SLCDModule_MarqueeWFRegisters(uint8_t FrontPlaneValues, int16_t FrontPlanePosition, uint8_t MaxPosition, uint8_t MinPosition);
void _SLCDModule_WriteWFRegister(uint32_t WFRegister, uint8_t Position);
uint32_t _SLCDModule_CopyWFRegister(uint8_t Position);
uint8_t _SLCDModule_SelectBackPlanePhase(void);

/**
  Function Name    : _SLCDModule_Init
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : This function is used to configure the SLCD module of the K40.
*/
void _SLCDModule_Init(void)
{
  MCG_C1 |= MCG_C1_IRCLKEN_MASK;
  //enable clock gate for Ports
  SIM_SCGC5 |=  ( !SIM_SCGC5_LPTIMER_MASK
                | !SIM_SCGC5_REGFILE_MASK
                );
  // Master General Purpose Control Register - Set mux to LCD analog operation.
  // After RESET these register are configured as 0 but indicated here for reference
  PORTB_PCR1  = PORT_PCR_MUX(0); //LCD_P1
  PORTB_PCR2  = PORT_PCR_MUX(0); //LCD_P2
  PORTB_PCR3  = PORT_PCR_MUX(0); //LCD_P3
  PORTB_PCR4  = PORT_PCR_MUX(0); //LCD_P4
  PORTB_PCR5  = PORT_PCR_MUX(0); //LCD_P5
  PORTB_PCR6  = PORT_PCR_MUX(0); //LCD_P6
  PORTB_PCR7  = PORT_PCR_MUX(0); //LCD_P7
  PORTB_PCR8  = PORT_PCR_MUX(0); //LCD_P8
  PORTB_PCR9  = PORT_PCR_MUX(0); //LCD_P9
  PORTB_PCR10 = PORT_PCR_MUX(0); //LCD_P10
  PORTB_PCR11 = PORT_PCR_MUX(0); //LCD_P11
  PORTB_PCR16 = PORT_PCR_MUX(0); //LCD_P12
  PORTB_PCR17 = PORT_PCR_MUX(0); //LCD_P13
  PORTB_PCR18 = PORT_PCR_MUX(0); //LCD_P14
  PORTB_PCR19 = PORT_PCR_MUX(0); //LCD_P15
  PORTB_PCR20 = PORT_PCR_MUX(0); //LCD_P16
  PORTB_PCR21 = PORT_PCR_MUX(0); //LCD_P17
  PORTB_PCR22 = PORT_PCR_MUX(0); //LCD_P18
  PORTB_PCR23 = PORT_PCR_MUX(0); //LCD_P19
  PORTC_PCR0  = PORT_PCR_MUX(0); //LCD_P20
  PORTC_PCR1  = PORT_PCR_MUX(0); //LCD_P21
  PORTC_PCR2  = PORT_PCR_MUX(0); //LCD_P22
  PORTC_PCR3  = PORT_PCR_MUX(0); //LCD_P23
  PORTC_PCR4  = PORT_PCR_MUX(0); //LCD_P24
  PORTC_PCR5  = PORT_PCR_MUX(0); //LCD_P25
  PORTC_PCR6  = PORT_PCR_MUX(0); //LCD_P26
  PORTC_PCR7  = PORT_PCR_MUX(0); //LCD_P27
  PORTC_PCR8  = PORT_PCR_MUX(0); //LCD_P28
  PORTC_PCR9  = PORT_PCR_MUX(0); //LCD_P29
  PORTC_PCR10 = PORT_PCR_MUX(0); //LCD_P30
  PORTC_PCR11 = PORT_PCR_MUX(0); //LCD_P31
  PORTC_PCR12 = PORT_PCR_MUX(0); //LCD_P32
  PORTC_PCR13 = PORT_PCR_MUX(0); //LCD_P33
  PORTC_PCR14 = PORT_PCR_MUX(0); //LCD_P34
  PORTC_PCR15 = PORT_PCR_MUX(0); //LCD_P35
  PORTC_PCR16 = PORT_PCR_MUX(0); //LCD_P36
  PORTC_PCR17 = PORT_PCR_MUX(0); //LCD_P37
  PORTC_PCR18 = PORT_PCR_MUX(0); //LCD_P38
  PORTC_PCR19 = PORT_PCR_MUX(0); //LCD_P39
  PORTD_PCR0  = PORT_PCR_MUX(0); //LCD_P40
  PORTD_PCR1  = PORT_PCR_MUX(0); //LCD_P41
  PORTD_PCR2  = PORT_PCR_MUX(0); //LCD_P42
  PORTD_PCR3  = PORT_PCR_MUX(0); //LCD_P43
  PORTD_PCR4  = PORT_PCR_MUX(0); //LCD_P44
  PORTD_PCR5  = PORT_PCR_MUX(0); //LCD_P45
  PORTD_PCR6  = PORT_PCR_MUX(0); //LCD_P46
  PORTD_PCR7  = PORT_PCR_MUX(0); //LCD_P47

  // SLCD clock gate on
  SIM_SCGC3 |= SIM_SCGC3_SLCD_MASK;

  // Disable LCD
  LCD_GCR = 0;

  // Enable LCD pins 1-47
  LCD_PENH = 0;
  LCD_PENH = 0x0000FFFF;
  LCD_PENL = 0;
  LCD_PENL = 0xFFFFFFFE;
  // Enable LCD pins used as Backplanes 0-7
  LCD_BPENH = 0;
  LCD_BPENH = 0x0000FF00;
  LCD_BPENL = 0;
  // Configure backplane phase
  LCD_WF43TO40 = 0x08040201;
  LCD_WF47TO44 = 0x80402010;
  // Fill information on what segments are going to be turned on. Front Plane information
  LCD_WF3TO0   = 0;
  LCD_WF7TO4   = 0;
  LCD_WF11TO8  = 0;
  LCD_WF15TO12 = 0;
  LCD_WF19TO16 = 0;
  LCD_WF23TO20 = 0;
  LCD_WF27TO24 = 0;
  LCD_WF31TO28 = 0;
  LCD_WF35TO32 = 0;
  LCD_WF39TO36 = 0;

  // Configure LCD Control Register
  LCD_GCR |=  ( LCD_GCR_CPSEL_MASK
              | LCD_GCR_RVEN_MASK
              | LCD_GCR_RVTRIM(15)
              | !LCD_GCR_HREFSEL_MASK // ( 0 - 3 Volts LCDs / 1 - 5 Volts LCDs)
              | LCD_GCR_LADJ(1) //0-3
              | LCD_GCR_VSUPPLY(3) //0-3
              | !LCD_GCR_LCDIEN_MASK
              | !LCD_GCR_FDCIEN_MASK
              | LCD_GCR_ALTDIV(0) //0-3
              | !LCD_GCR_LCDWAIT_MASK
              | !LCD_GCR_LCDSTP_MASK
              | !LCD_GCR_LCDEN_MASK
              | LCD_GCR_SOURCE_MASK
              | LCD_GCR_LCLK(0) //0-3
              | LCD_GCR_DUTY(7) //0-7
              );
  // Enable LCD module
  LCD_GCR |= LCD_GCR_LCDEN_MASK;
}

/**
  Function Name    : _SLCDModule_PrintString
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : char * SourceString, uint8_t StartPosition
  Returns       : Nothing
  Notes         : This function issues the LCD to write a given string at given start position.
                  Since the LCD is limited to a maximum of 6 characters (5x7 and a column of separation)
                  characters outside this size limit will be excluded.
*/
void _SLCDModule_PrintString(char * SourceString, uint8_t StartPosition)
{
  uint8_t StringCharCopy = 0;
  uint8_t Position = StartPosition;

  do
  {
    StringCharCopy = *SourceString;
    SourceString++;
    if (StringCharCopy > 0x80)
    {
      StringCharCopy = 0x80;
    }
    else
    {
      StringCharCopy -= 0x20;
    }

    if (Fonts[StringCharCopy] != _NULL)
    {
      if (Position <= CHAR_MAX_POSITION_VALUE)
      {
        _SLCDModule_StringWFRegisters(StringCharCopy,Position);
        Position += CHAR_POSITION_INCREMENT;
      }
      else
      {
        //Do nothing
      }
    }
    else
    {
      //Do nothing
    }
  }while (*SourceString != _NULL);
}

/**
  Function Name    : _SLCDModule_StringWFRegisters
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : uint8_t FrontPlaneValues, uint8_t FrontPlanePosition
  Returns       : Nothing
  Notes         : This function builds the correspoding waveform register value and then writes it. This function is for
                  printing a simple string.
*/
void _SLCDModule_StringWFRegisters(uint8_t FrontPlaneValues, uint8_t FrontPlanePosition)
{
  uint8_t ColumnCounter = 0;
  uint8_t CharArrayCounter = 0;
  uint32_t WaveFormRegister = 0;
  uint8_t BackPlane = 0;
  uint8_t SegmentOnOff = 0;
  uint32_t Registro = 0xFFFFFFFF;

  do
  {
    if (ColumnCounter > MAX_COLUMNS_PER_CHAR)
    {
      BackPlane = _SLCDModule_SelectBackPlanePhase();
      ColumnCounter = 0;
      FrontPlanePosition -= CHAR_RESET_POSITION;
    }

    WaveFormRegister = _SLCDModule_CopyWFRegister(FrontPlanePosition);

    SegmentOnOff = Fonts[FrontPlaneValues][CharArrayCounter];
    if (SegmentOnOff == 0)
    {
      Registro = ~(1 << (BackPlane + WaveFormShiftRegisterTable[FrontPlanePosition]));
      WaveFormRegister &= Registro;
    }
    else
    {
      WaveFormRegister |= (SegmentOnOff << (BackPlane + WaveFormShiftRegisterTable[FrontPlanePosition]));
    }
    WaveFormRegister |= (SegmentOnOff << (BackPlane + WaveFormShiftRegisterTable[FrontPlanePosition]));

    _SLCDModule_WriteWFRegister(WaveFormRegister,FrontPlanePosition);

    CharArrayCounter++;
    ColumnCounter++;
    FrontPlanePosition++;
  }while (CharArrayCounter < CHAR_SIZE);
}

/**
  Function Name    : _SLCDModule_TurnOnPounceSign
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns on the segment with the Pounce sign.
*/
void _SLCDModule_TurnOnPounceSign(void)
{
  uint32_t WFRegisterCopy = 0;

  WFRegisterCopy = LCD_WF39TO36;
  WFRegisterCopy |= (1 << PHASE_F + 24);
  LCD_WF39TO36 = WFRegisterCopy;
}

/**
  Function Name    : _SLCDModule_TurnOnFreescaleSign
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns on the segment with the Freescale sign.
*/
void _SLCDModule_TurnOnFreescaleSign(void)
{
  uint32_t WFRegisterCopy = 0;

  WFRegisterCopy = LCD_WF3TO0;
  WFRegisterCopy |= (1 << PHASE_E + 8);
  LCD_WF3TO0 = WFRegisterCopy;
}

/**
  Function Name    : _SLCDModule_TurnOnJLinkSign
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns on the segment with the J-Link sign.
*/
void _SLCDModule_TurnOnJLinkSign(void)
{
  uint32_t WFRegisterCopy = 0;

  WFRegisterCopy = LCD_WF3TO0;
  WFRegisterCopy |= (1 << PHASE_F + 8);
  LCD_WF3TO0 = WFRegisterCopy;
}


/**
  Function Name    : _SLCDModule_TurnOffPounceSign
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns off the Pounce sign segment.
*/
void _SLCDModule_TurnOffPounceSign(void)
{
  uint32_t WFRegisterCopy = 0;

  WFRegisterCopy = LCD_WF39TO36;
  WFRegisterCopy &= ~(1 << PHASE_F + 24);
  LCD_WF39TO36 = WFRegisterCopy;
}

/**
  Function Name    : _SLCDModule_TurnOffFreescaleSign
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns off the Freescale sign segment.
*/
void _SLCDModule_TurnOffFreescaleSign(void)
{
  uint32_t WFRegisterCopy = 0;

  WFRegisterCopy = LCD_WF3TO0;
  WFRegisterCopy &= ~(1 << PHASE_E + 8);
  LCD_WF3TO0 = WFRegisterCopy;
}

/**
  Function Name    : _SLCDModule_TurnOffJLinkSign
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns off the J-Link sign segment.
*/
void _SLCDModule_TurnOffJLinkSign(void)
{
  uint32_t WFRegisterCopy = 0;

  WFRegisterCopy = LCD_WF3TO0;
  WFRegisterCopy &= ~(1 << PHASE_F + 8);
  LCD_WF3TO0 = WFRegisterCopy;
}

/**
  Function Name    : _SLCDModule_TurnOnBattery
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : uint8_t BatteryLevel
  Returns       : Nothing
  Notes         : Turns the 3 segments, representing the levels of battery charge.
*/
void _SLCDModule_TurnOnBattery(uint8_t BatteryLevel)
{
  uint32_t WFRegisterCopy1 = 0;
  uint32_t WFRegisterCopy2 = 0;

  WFRegisterCopy1 = LCD_WF3TO0;
  WFRegisterCopy2 = LCD_WF39TO36;

  if (BatteryLevel == CHARGED)
  {
    WFRegisterCopy1 |= (uint32_t)(1 << PHASE_H +  8); //Battery Upper Block
    WFRegisterCopy2 |= (uint32_t)(1 << PHASE_G + 24); //Battery Middle Block
    WFRegisterCopy2 |= (uint32_t)((1u << PHASE_H + 24) & 0xFFFFFFFFu); //Battery Lower Block
  }

  else if (BatteryLevel == MID_LEVEL)
  {
    WFRegisterCopy1 &= ~(uint32_t)(1 << PHASE_H +  8); //Battery Upper Block
    WFRegisterCopy2 |= (uint32_t)(1 << PHASE_G + 24); //Battery Middle Block
    WFRegisterCopy2 |= (uint32_t)((1u << PHASE_H + 24) & 0xFFFFFFFFu); //Battery Lower Block
  }

  else if (BatteryLevel == LOWER_LEVEL)
  {
    WFRegisterCopy1 &= ~(uint32_t)(1 << PHASE_H +  8); //Battery Upper Block
    WFRegisterCopy2 &= ~(uint32_t)(1 << PHASE_G + 24); //Battery Middle Block
    WFRegisterCopy2 |= (uint32_t)((1u << PHASE_H + 24) & 0xFFFFFFFFu); //Battery Lower Block
  }
  else
  {
    WFRegisterCopy1 &= ~(uint32_t)(1 << PHASE_H +  8); //Battery Upper Block
    WFRegisterCopy2 &= ~(uint32_t)(1 << PHASE_G + 24); //Battery Middle Block
    WFRegisterCopy2 &= ~(uint32_t)((1u << PHASE_H + 24) & 0xFFFFFFFFu); //Battery Lower Block
  }

  LCD_WF3TO0 = WFRegisterCopy1;
  LCD_WF39TO36 = WFRegisterCopy2;
}

/**
  Function Name    : _SLCDModule_TurnOffBattery
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns off the Battery Outline and Levels.
*/
void _SLCDModule_TurnOffBattery(void)
{
  uint32_t WFRegisterCopy1 = 0;
  uint32_t WFRegisterCopy2 = 0;

  WFRegisterCopy1 = LCD_WF3TO0;
  WFRegisterCopy2 = LCD_WF39TO36;

  WFRegisterCopy1 &= ~(1 << PHASE_G + 8); //Battery Outline
  WFRegisterCopy1 &= ~(1 << PHASE_H + 8); //Battery Upper Block
  WFRegisterCopy2 &= ~(1 << PHASE_G + 24); //Battery Middle Block
  WFRegisterCopy2 &= ~((1u << PHASE_H + 24) & 0xFFFFFFFFu); //Battery Lower Block
                    
  LCD_WF3TO0 = WFRegisterCopy1;
  LCD_WF39TO36 = WFRegisterCopy2;
}

/**
  Function Name    : _SLCDModule_SetSegment
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : uint8_t X, uint8_t Y
  Returns       : Nothing
  Notes         : This function is used to set a pixel at a given position. This is currently not used.
*/
void _SLCDModule_SetSegment(uint8_t X, uint8_t Y)
{
  uint32_t WFRegisterCopy = 0;

  if (X <= CHAR_MAX_POSITION_VALUE)
  {
    WFRegisterCopy = _SLCDModule_CopyWFRegister(X);

    WFRegisterCopy &= WaveFormRegisterClearTable[X];

    if (Y <= 7)
    {
      //do nothing
    }
    else
    {
      Y = 0;
    }

    WFRegisterCopy |= (1 << (Y + WaveFormShiftRegisterTable[X]));

    _SLCDModule_WriteWFRegister(WFRegisterCopy,X);
  }
}

/**
  Function Name    : _SLCDModule_ClearSegment
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : uint8_t X, uint8_t Y
  Returns       : Nothing
  Notes         : Turns off a pixel at a given position. This is currently not used.
*/
void _SLCDModule_ClearSegment(uint8_t X, uint8_t Y)
{
  uint32_t WFRegisterCopy = 0;

  if (X <= CHAR_MAX_POSITION_VALUE)
  {
    WFRegisterCopy = _SLCDModule_CopyWFRegister(X);

    WFRegisterCopy &= WaveFormRegisterClearTable[X];
    WFRegisterCopy |= (0 << (Y + WaveFormShiftRegisterTable[X]));

    _SLCDModule_WriteWFRegister(WFRegisterCopy,X);
  }
}

/**
  Function Name    : _SLCDMoudle_TurnOffAllSegments
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns off all the LCD segments.
*/
void _SLCDMoudle_TurnOffAllSegments(void)
{
  LCD_WF3TO0   = 0x00000000;
  LCD_WF7TO4   = 0x00000000;
  LCD_WF11TO8  = 0x00000000;
  LCD_WF15TO12 = 0x00000000;
  LCD_WF19TO16 = 0x00000000;
  LCD_WF23TO20 = 0x00000000;
  LCD_WF27TO24 = 0x00000000;
  LCD_WF31TO28 = 0x00000000;
  LCD_WF35TO32 = 0x00000000;
  LCD_WF39TO36 = 0x00000000;
}

/**
  Function Name    : _SLCDMoudle_TurnOnAllSegments
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : Nothing
  Notes         : Turns on all the LCD segments.
*/
void _SLCDMoudle_TurnOnAllSegments(void)
{
  LCD_WF3TO0   = 0xFFFFFF00;
  LCD_WF7TO4   = 0xFFFFFFFF;
  LCD_WF11TO8  = 0xFFFFFFFF;
  LCD_WF15TO12 = 0xFFFFFFFF;
  LCD_WF19TO16 = 0xFFFFFFFF;
  LCD_WF23TO20 = 0xFFFFFFFF;
  LCD_WF27TO24 = 0xFFFFFFFF;
  LCD_WF31TO28 = 0xFFFFFFFF;
  LCD_WF35TO32 = 0xFFFFFFFF;
  LCD_WF39TO36 = 0xFFFFFFFF;
}

/**
  Function Name    : _SLCDModule_ClearLCD
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : uint8_t ArrowsOn
  Returns       : Nothing
  Notes         : Clears the LCD character displaying area. It has two options, currently those are hardcoded to work
                  with the limits defined by the left and right arrows.
*/
void _SLCDModule_ClearLCD(uint8_t ArrowsOn)
{
  if (ArrowsOn == ARROWS)
  {
    LCD_WF3TO0   &= 0xFFFFFF00;
    LCD_WF7TO4   &= 0x00000000;
    LCD_WF11TO8  &= 0x00000000;
    LCD_WF15TO12 &= 0x00000000;
    LCD_WF19TO16 &= 0x00000000;
    LCD_WF23TO20 &= 0x00000000;
    LCD_WF27TO24 &= 0x00000000;
    LCD_WF31TO28 &= 0x00000000;
    LCD_WF35TO32 &= 0x00000000;
    LCD_WF39TO36 &= 0xFFFFFF00;
  }
  else if (ArrowsOn == NO_ARROWS)
  {
    LCD_WF3TO0   &= 0x0000FF00;
    LCD_WF7TO4   &= 0x00000000;
    LCD_WF11TO8  &= 0x00000000;
    LCD_WF15TO12 &= 0x00000000;
    LCD_WF19TO16 &= 0x00000000;
    LCD_WF23TO20 &= 0x00000000;
    LCD_WF27TO24 &= 0x00000000;
    LCD_WF31TO28 &= 0x00000000;
    LCD_WF35TO32 &= 0x00000000;
    LCD_WF39TO36 &= 0xFF000000;
  }
  else
  {
  }
}

/**
  Function Name    : _SLCDModule_WriteWFRegister
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : uint32_t WFRegister, uint8_t Position
  Returns       : Nothing
  Notes         : This function writes a value to a given waveform register. It receives the builded register from the Marquee or String
                  functions. It validates that the FrontPlane (Position) is within the boundaries of the SLCD supported waveform registers.
                  Anything outside this is discarded. The Position has a +2 constant value added, because LCD_P0 (the first 8 bits) is currently
                  used as an electrode.
*/
void _SLCDModule_WriteWFRegister(uint32_t WFRegister, uint8_t Position)
{
  if (((Position + 2) >= FRONT_PLANE_0_TO_3) && ((Position + 2) < FRONT_PLANE_4_TO_7))
  {
    LCD_WF3TO0 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_4_TO_7) && ((Position + 2) < FRONT_PLANE_8_TO_11))
  {
    LCD_WF7TO4 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_8_TO_11) && ((Position + 2) < FRONT_PLANE_12_TO_15))
  {
    LCD_WF11TO8 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_12_TO_15) && ((Position + 2) < FRONT_PLANE_16_TO_19))
  {
    LCD_WF15TO12 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_16_TO_19) && ((Position + 2) < FRONT_PLANE_20_TO_23))
  {
    LCD_WF19TO16 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_20_TO_23) && ((Position + 2) < FRONT_PLANE_24_TO_27))
  {
    LCD_WF23TO20 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_24_TO_27) && ((Position + 2) < FRONT_PLANE_28_TO_31))
  {
    LCD_WF27TO24 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_28_TO_31) && ((Position + 2) < FRONT_PLANE_32_TO_35))
  {
    LCD_WF31TO28 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_32_TO_35) && ((Position + 2) < FRONT_PLANE_36_TO_39))
  {
    LCD_WF35TO32 = WFRegister;
  }
  else if (((Position + 2) >= FRONT_PLANE_36_TO_39) && ((Position + 2) < FRONT_PLANE_40_TO_43))
  {
    LCD_WF39TO36 = WFRegister;
  }
  else
  {
    //do nothing
  }
}

/**
  Function Name    : _SLCDModule_CopyWFRegister
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : uint8_t Position
  Returns       : WFRegister
  Notes         : This function is used to make a back-up of the current value of a given waveform register.
                  If the waveform register position is not supported, it returns a null value.
*/
uint32_t _SLCDModule_CopyWFRegister(uint8_t Position)
{
  uint32_t WFRegister = 0;
  if (((Position + 2) >= FRONT_PLANE_0_TO_3) && ((Position + 2) < FRONT_PLANE_4_TO_7))
  {
    WFRegister = LCD_WF3TO0;
  }
  else if (((Position + 2) >= FRONT_PLANE_4_TO_7) && ((Position + 2) < FRONT_PLANE_8_TO_11))
  {
    WFRegister = LCD_WF7TO4;
  }
  else if (((Position + 2) >= FRONT_PLANE_8_TO_11) && ((Position + 2) < FRONT_PLANE_12_TO_15))
  {
    WFRegister = LCD_WF11TO8;
  }
  else if (((Position + 2) >= FRONT_PLANE_12_TO_15) && ((Position + 2) < FRONT_PLANE_16_TO_19))
  {
    WFRegister = LCD_WF15TO12;
  }
  else if (((Position + 2) >= FRONT_PLANE_16_TO_19) && ((Position + 2) < FRONT_PLANE_20_TO_23))
  {
    WFRegister = LCD_WF19TO16;
  }
  else if (((Position + 2) >= FRONT_PLANE_20_TO_23) && ((Position + 2) < FRONT_PLANE_24_TO_27))
  {
    WFRegister = LCD_WF23TO20;
  }
  else if (((Position + 2) >= FRONT_PLANE_24_TO_27) && ((Position + 2) < FRONT_PLANE_28_TO_31))
  {
    WFRegister = LCD_WF27TO24;
  }
  else if (((Position + 2) >= FRONT_PLANE_28_TO_31) && ((Position + 2) < FRONT_PLANE_32_TO_35))
  {
    WFRegister = LCD_WF31TO28;
  }
  else if (((Position + 2) >= FRONT_PLANE_32_TO_35) && ((Position + 2) < FRONT_PLANE_36_TO_39))
  {
    WFRegister = LCD_WF35TO32;
  }
  else
  {
    if (((Position + 2) >= FRONT_PLANE_36_TO_39) && ((Position + 2) < FRONT_PLANE_40_TO_43))
    {
      WFRegister = LCD_WF39TO36;
    }
    else
    {
      WFRegister = _NULL;
    }
  }
  return  WFRegister;
}

/**
  Function Name    : _SLCDModule_SelectBackPlanePhase
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters    : Nothing
  Returns       : BackPlane
  Notes         : This function is used to select the current back plane phase. It returns
                  a value ranging within 0~ 7 (8 backplanes).
*/
uint8_t _SLCDModule_SelectBackPlanePhase(void)
{
  static uint8_t CurrentBackPlanePhase = 1;
  uint8_t BackPlane = 0;

  if (CurrentBackPlanePhase == 1)
  {
    BackPlane = PHASE_B;
    CurrentBackPlanePhase++;
  }
  else if (CurrentBackPlanePhase == 2)
  {
    BackPlane = PHASE_C;
    CurrentBackPlanePhase++;
  }
  else if (CurrentBackPlanePhase == 3)
  {
    BackPlane = PHASE_D;
    CurrentBackPlanePhase++;
  }
  else if (CurrentBackPlanePhase == 4)
  {
    BackPlane = PHASE_E;
    CurrentBackPlanePhase++;
  }
  else if (CurrentBackPlanePhase == 5)
  {
    BackPlane = PHASE_F;
    CurrentBackPlanePhase++;
  }
  else if (CurrentBackPlanePhase == 6)
  {
    BackPlane = PHASE_G;
    CurrentBackPlanePhase = 1;
    //CurrentBackPlane++;
  }

  /**
    Not used for printing strings

  else if (CurrentBackPlanePhase == 7)
  {
    BackPlane = PHASE_H;
    CurrentBackPlanePhase = 0;
  }*/
  else
  {
    BackPlane = PHASE_A;
    CurrentBackPlanePhase = 0;
  }

  return BackPlane;
}

/**
  Function Name	: _SLCDModule_PrintNumber
  Engineer      : Manglio González Carrasco
  Date          : 05/01/2011

  Parameters	: uint8_t Number, uint8_t Position
  Returns       : Nothing
  Notes         : This function is used to print a given number (0~9) at a given position.
                  if the number value is greater than what is allowed the LCD will show an
                  error message.
*/
void _SLCDModule_PrintNumber(uint8_t Number, uint8_t Position)
{
  if (Number < 10)
  {
    uint8_t ColumnCounter = 0;
    uint8_t CharArrayCounter = 0;
    uint32_t WaveFormRegister = 0;
    uint8_t BackPlane = 0;
    uint8_t SegmentOnOff = 0;
    uint32_t Registro = 0xFFFFFFFF;

    do
    {
      if (ColumnCounter > MAX_COLUMNS_PER_CHAR)
      {
        BackPlane = _SLCDModule_SelectBackPlanePhase();
        ColumnCounter = 0;
        Position -= CHAR_RESET_POSITION;
      }

      if (Position < CHAR_MAX_POSITION_VALUE)
      {
        WaveFormRegister = _SLCDModule_CopyWFRegister(Position);

        SegmentOnOff = Fonts[Number + 0x10][CharArrayCounter];
        if (SegmentOnOff == 0)
        {
          Registro = ~(1 << (BackPlane + WaveFormShiftRegisterTable[Position]));
          WaveFormRegister &= Registro;
        }
        else
        {
          WaveFormRegister |= (SegmentOnOff << (BackPlane + WaveFormShiftRegisterTable[Position]));
        }
        _SLCDModule_WriteWFRegister(WaveFormRegister,Position);
      }

      CharArrayCounter++;
      ColumnCounter++;
      Position++;
    }while (CharArrayCounter < CHAR_SIZE);
  }
  else
  {
    _SLCDModule_ClearLCD(NO_ARROWS);
    _SLCDModule_PrintString(Error,4);
  }
}
