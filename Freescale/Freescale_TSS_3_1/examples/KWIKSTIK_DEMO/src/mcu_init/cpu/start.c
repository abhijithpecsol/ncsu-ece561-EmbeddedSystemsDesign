/*
 * File:    start.c
 * Purpose:    Kinetis start up routines.
 *
 * Notes:        
 */

#include "start.h"
#include "common.h"
#include "sysinit.h"
#include "wdog.h"

/********************************************************************/
/*!
 * \brief   Kinetis Start
 * \return  None
 *
 * This function calls all of the needed starup routines and then
 * branches to the main process.
 */
void start(void)
{
    /* Disable the watchdog timer */
    wdog_disable();
    /* Copy any vector or data sections that need to be in RAM */
    common_startup();
    /* Perform processor initialization */
    sysinit();

    /* Determine specific Kinetis device and revision */
    //cpu_indentify();
    
    /* Jump to main process */
    main();

    /* No actions to perform after this so wait forever */
    while(1);
}
/********************************************************************/
/*!
 * \brief   Kinetis Identify
 * \return  None
 *
 * This is primarly a reporting function that displays information
 * about the specific CPU to the default terminal including:
 * - Kinetis family
 * - package
 * - die revision
 * - P-flash size
 * - Ram size
 */
void cpu_indentify (void)
{
    /* Determine the Kinetis family */
    switch((SIM_SDID & SIM_SDID_FAMID(0x7))>>SIM_SDID_FAMID_SHIFT)
    {
        case 0x0:
            printf("\nK10-");
            break;
        case 0x1:
            printf("\nK20-");
            break;
        case 0x2:
            printf("\nK30-");
            break;
        case 0x3:
            printf("\nK40-");
            break;
        case 0x4:
            printf("\nK60-");
            break;
        case 0x5:
            printf("\nK70-");
            break;
        case 0x6:
            printf("\nK50-");
            break;
        case 0x7:
            printf("\nK53-");
            break;
    default:
        printf("\nUnrecognized Kinetis family device.\n");
        break;      
    }

    /* Determine the package size */
    switch((SIM_SDID & SIM_SDID_PINID(0xF))>>SIM_SDID_PINID_SHIFT)
    {
        case 0x2:
            printf("32pin\n");
            break;
        case 0x4:
            printf("48pin\n");
            break;
        case 0x5:
            printf("64pin\n");
            break;
        case 0x6:
            printf("80pin\n");
            break;
        case 0x7:
            printf("81pin\n");
            break;
        case 0x8:
            printf("100pin\n");
            break;
        case 0x9:
            printf("104pin\n");
            break;
        case 0xA:
            printf("144pin\n");
            break;
        case 0xC:
            printf("196pin\n");
            break;
        case 0xE:
            printf("256pin\n");
            break;
    default:
        printf("\nUnrecognized Kinetis family device.\n");
        break;      
    }

    /* Determine the revision ID */
    printf("rev %d\n", SIM_SDID & SIM_SDID_REVID(0xF));


    /* Determine the P-flash size */
    switch((SIM_FCFG1 & SIM_FCFG1_PFSIZE_MASK)>>SIM_FCFG1_PFSIZE_SHIFT)

    {
        case 0x0:
            printf("12 kBytes of P-flash    ");
            break;
        case 0x1:
            printf("16 kBytes of P-flash    ");
            break;
        case 0x2:
            printf("32 kBytes of P-flash    ");
            break;
        case 0x3:
            printf("48 kBytes of P-flash    ");
            break;
        case 0x4:
            printf("64 kBytes of P-flash    ");
            break;
        case 0x5:
            printf("96 kBytes of P-flash    ");
            break;
        case 0x6:
            printf("128 kBytes of P-flash    ");
            break;
        case 0x7:
            printf("192 kBytes of P-flash    ");
            break;
        case 0x8:
            printf("256 kBytes of P-flash    ");
            break;
        case 0x9:
            printf("320 kBytes of P-flash    ");
            break;
        case 0xA:
            printf("384 kBytes of P-flash    ");
            break;
        case 0xB:
            printf("448 kBytes of P-flash    ");
            break;
        case 0xC:
            printf("512 kBytes of P-flash    ");
            break;
        case 0xD:
        case 0xF:
            printf("1024 kBytes of P-flash    ");
            break;
        case 0xFF:
            printf("Full size P-flash    ");
            break;
        default:
            printf("ERR!! Undefined P-flash size\n");
            break;          
    }

    /* Determine the RAM size */
    switch((SIM_SOPT1 & SIM_SOPT1_RAMSIZE(0xF))>>SIM_SOPT1_RAMSIZE_SHIFT)
    {
        case 0x5:
            printf(" 32 kBytes of RAM\n\n");
            break;
        case 0x7:
            printf(" 64 kBytes of RAM\n\n");
            break;
        case 0x8:
            printf(" 96 kBytes of RAM\n\n");
            break;
        case 0x9:
            printf(" 128 kBytes of RAM\n\n");
            break;
        default:
            printf(" ERR!! Undefined RAM size\n\n");
            break;          
    }
}
/********************************************************************/

