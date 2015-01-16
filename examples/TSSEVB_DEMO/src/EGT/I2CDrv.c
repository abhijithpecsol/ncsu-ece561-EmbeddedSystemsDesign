/***********************************************************************************************************************
* RTAC Americas
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
*********************************************************************************************************************//*!
*
* @file   I2CDrv.c
*
* @brief  I2C module to comunicate with the Comm MCU for the use of the Electrode Graphing Tool
*
* @version 1.1.11.0
*          1.0.0.0 Initial Release
* 
* @date Feb-3-2012
* 
*
***********************************************************************************************************************/

#include <hidef.h>
#include "main.h"
#include "TSS_API.h"

volatile uint8_t u8I2CStatus = 0;
volatile uint8_t * volatile pu8I2CData;
uint8_t u8Dumy=0x00;
uint8_t u8ChangeFlag=0;

extern TSS_CSSystem tss_CSSys;
extern uint8_t        tss_au8ElectrodeEnablers[];
extern uint8_t        tss_au8ElectrodeStatus[];
extern uint8_t        tss_au8Sensitivity[TSS_N_ELECTRODES];
extern int8_t         tss_ai8InstantDelta[TSS_N_ELECTRODES];
extern uint16_t       tss_au16ElecBaseline[TSS_N_ELECTRODES];
extern uint8_t        tss_au8ConfCheckSum;

uint8_t gu8electrodeNumber = (uint8_t)TSS_N_ELECTRODES;

/*****************************************************************************/

void I2CInit(void)
{
    /* Configure I2C module */
    IICA = DEVICE_SLAVE_ADDRESS;
    IICC = (IICC_IICEN_MASK|IICC_IICIE_MASK);
}

/*****************************************************************************/

void I2CDrv(void)
{
    if(u8I2CStatus & (1<<I2C_INTERRUPT_FLAG))
    {
        u8I2CStatus &= ~(1<<I2C_INTERRUPT_FLAG);
        /* Arbitration Lost ? */
        if(IICS & IICS_ARBL_MASK)
        {
            IICS |= IICS_ARBL_MASK;
            return;
        }
        /* Handle Address */
        if(IICS & IICS_IAAS_MASK)
        {
            IICC_TX = IICS_SRW;
            u8I2CStatus |= (1<<I2C_AAS_FLAG);
        }

        /* Handle Byte */ 
        if(IICS & IICS_TCF_MASK)
        {
            if(IICC_TX) /* Slave Data Out */
            {                                    
                if(IICS& IICS_RXAK_MASK)
                {
                    IICC |= IICC_TXAK_MASK;
                    IICC_TX = 0;
                    (void)IICD;   /* Dummy Read */
                    return;
                }
                /* Transmitt current pointer address */
                IICD = *pu8I2CData++;
               
            }
            else        /* Slave Data In */
            {
                /* Traps Physical address to avoid writing it to buffer */
                if(u8I2CStatus & (1<<I2C_AAS_FLAG))
                {
                    u8I2CStatus &= ~(1<<I2C_AAS_FLAG);
                    u8I2CStatus |= (1<<I2C_ADDRESS_BYTE_FLAG);
                    IICC &= ~((uint8_t) IICC_TXAK_MASK);
                    (void)IICD;   /* Dummy Read */
                    return;
                }
                /* Traps Logical address to avoid writing it to buffer */
                #ifdef EIGHT_BIT_LOGICAL_ADDRESS
                if(u8I2CStatus & (1<<I2C_ADDRESS_BYTE_FLAG))
                {
                    u8I2CStatus &= ~(1<<I2C_ADDRESS_BYTE_FLAG);
                    IICC &= ~((uint8_t) IICC_TXAK_MASK);
                    /* Address Read */
                    pu8I2CData = u16AddressDecoder(IICD);
                    return;
                }
                #endif /* EIGHT_BIT_LOGICAL_ADDRESS */
                /* Write data at current pointer address */
                IICC &= ~ ((uint8_t) IICC_TXAK_MASK);
                *pu8I2CData++ = IICD;
                
            }
        }
        #ifdef SLAVE_INTERRUPT
        /* Slave Interrupt pin to get Master's Attention */
        I2C_SLAVE_ASSERT_INTERRUPT();
        #endif /* SLAVE_INTERRUPT */
    }


}

/*****************************************************************************/

interrupt VectorNumber_Viic void I2CIsr(void)
{
    
    /* Clear Interrupt Flag */
    IICS |= IICS_IICIF_MASK;
    TSS_SET_SAMPLE_INTERRUPTED();
    //EnableInterrupts;
    /* Set Software Flag */
    u8I2CStatus |= (1<<I2C_INTERRUPT_FLAG);
    /* I2C SW Driver */
    I2CDrv();
}

/*****************************************************************************/
uint8_t* u16AddressDecoder(uint8_t u8SoftAddress)
{
    uint8_t *pu8Pointer;
    
    switch(u8SoftAddress & 0xF0)
    {
        case GENERAL_REGISTERS:
            pu8Pointer=(uint8_t*)&tss_CSSys;
            break;
            
        case ELECTRODE_NUMBER:
            pu8Pointer=(uint8_t*)&gu8electrodeNumber;
            break;

        case STATUS_ELECTRODES:
            pu8Pointer=(uint8_t*)&tss_au8ElectrodeStatus;
            break;
            
        case ELECTRODES_ENABLERS:
            pu8Pointer=(uint8_t*)&tss_au8ElectrodeEnablers;
            break;
            
        case SENSITIVITY:
            pu8Pointer=(uint8_t*)&tss_au8Sensitivity;
            break;
            
        case INSTANT_DELTAS:
            pu8Pointer=(uint8_t*)&tss_ai8InstantDelta;
            break;
        
        #if TSS_N_ELECTRODES>15
        case SENSITIVITY1:
            pu8Pointer=(uint8_t*) (&tss_au8Sensitivity)+16;
            break;
        #endif          
          
            
        #if TSS_N_ELECTRODES>31
        case SENSITIVITY2:
            pu8Pointer=(uint8_t*) (&tss_au8Sensitivity)+32;
            break;
        #endif
        
        #if TSS_N_ELECTRODES>47
        case SENSITIVITY3:
            pu8Pointer=(uint8_t*) (&tss_au8Sensitivity)+48;
            break;
        #endif
                
        case BASE_LINES:
            pu8Pointer=(uint8_t*)&tss_au16ElecBaseline;
            break;
        
        case CHECKSUM:
            pu8Pointer=(uint8_t*)&u8Dumy;
            u8ChangeFlag=1;
            break;
            
        default:
            pu8Pointer=(uint8_t*)&u8Dumy;
            return pu8Pointer;
            break;
    }
    pu8Pointer+=(u8SoftAddress & 0x0F);
    return pu8Pointer;
}

/*************************************************************************************/
void EGTTask(void)
{    
    uint8_t *SystemConfigTemp;
    if(u8ChangeFlag)     /* GUI communication flag for debugging */
    {
        SystemConfigTemp = (uint8_t*)&tss_CSSys.SystemConfig;
        (void)TSS_SetSystemConfig(System_SystemConfig_Register,(uint8_t) (*SystemConfigTemp|0x80));
        u8ChangeFlag=0;
    } 
}
