/***********************************************************************************************************************
* RTAC Americas
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
************************************************************************************************************************         
*
* @file   I2CDvr.h
*
* @brief  Header file of the I2C module to comunicate with the Comm MCU for the use of the Electrode Graphing Tool
*
* @version 1.1.0.0 Updated SENSITIVITY and INSTANT_DELTA macros. Added SENSITIVITY1,SENSITIVITY2 and SENSITIVITY3 macros.
*          1.0.0.0 Initial Release
* 
* @date Jan-19-2010
* 
*
***********************************************************************************************************************/

#ifndef __I2CDvr
#define __I2CDvr

#include "TSS_API.h"
#include "derivative.h" 
#define     EIGHT_BIT_LOGICAL_ADDRESS
#undef      SIXTEEN_BIT_LOGICAL_ADDRESS
#define     DEVICE_SLAVE_ADDRESS                      (0x58)
                                    
#ifndef IICA
    #define     IICA                IIC1A   
    #define     IICF                IIC1F
    #define     IICC                IIC1C
    #define     IICS                IIC1S
    #define     IICD                IIC1D
    #define     IICC2               IIC1C2
    #define     IICC_IICEN_MASK     IIC1C_IICIE_MASK
    #define     IICC_IICIE_MASK     IIC1C_IICEN_MASK
    #define     IICS_ARBL_MASK      IIC1S_ARBL_MASK
    #define     IICS_IAAS_MASK      IIC1S_IAAS_MASK
    #define     IICS_TCF_MASK       IIC1S_TCF_MASK
    #define     IICS_RXAK_MASK      IIC1S_RXAK_MASK
    #define     IICC_TXAK_MASK      IIC1C_TXAK_MASK
    #define     IICS_IICIF_MASK     IIC1S_IICIF_MASK
    #define     IICC_TX             IIC1C_TX
    #define     IICS_SRW            IIC1S_SRW
#endif

#ifndef VectorNumber_Viic
    #ifdef VectorNumber_Viic1
      #define VectorNumber_Viic   VectorNumber_Viic1
    #endif
    #ifdef VectorNumber_Viicx
      #define VectorNumber_Viic   VectorNumber_Viicx
    #endif
#endif

#define GENERAL_REGISTERS       0x00
#define STATUS_ELECTRODES       0x10 
#define ELECTRODES_ENABLERS     0x20 
#define SENSITIVITY             0x40 
#define INSTANT_DELTAS          0x30 
#define SENSITIVITY1            0x50 
#define SENSITIVITY2            0x60
#define SENSITIVITY3            0x70 
#define BASE_LINES              0x80
#define CHECKSUM                0xE0
#define ELECTRODE_NUMBER        0xF0

#define FLAG_SET(BitNumber, Register)        (Register |=(1<<BitNumber))
#define FLAG_CLR(BitNumber, Register)        (Register &=~(1<<BitNumber))
#define FLAG_CHK(BitNumber, Register)        (Register & (1<<BitNumber))

enum ISR
{
    I2C_Flag = 1
};

typedef enum
{
  I2C_INTERRUPT_FLAG,
  I2C_AAS_FLAG,
  I2C_ADDRESS_BYTE_FLAG,
}_eI2CFlags;

/**
 * \brief       This function contains the I2C communications state machine.
                This could be placed at the Interrupt function.
 * \author      Antonio Ramos Salido Maurer
 */  
void I2CDvr(void);
/**
 * \brief       Initialization of the I2C module as slave.
 * \author      Antonio Ramos Salido Maurer
 */  
void I2CInit(void);
uint8_t* u16AddressDecoder(uint8_t);
void EGTTask(void);

/*****************************************************************************/

#endif  /* __I2CDvr */
