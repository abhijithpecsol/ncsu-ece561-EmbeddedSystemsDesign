/*
 * File: exceptions.h
 * Purpose: Generic exception handling for ColdFire processors
 *
 * Notes:
 */

#ifndef _MCF_EXCEPTIONS_H
#define _MCF_EXCEPTIONS_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************/
/*
 *    Dummy routine for initializing hardware.  For user's custom systems, you
 *    can create your own routine of the same name that will perform HW
 *    initialization.  The linker will do the right thing to ignore this
 *    definition and use the version in your file.
 *
 */

void __initialize_hardware(void);

/***********************************************************************/
/*
 * This is the handler for all exceptions which are not common to all 
 * ColdFire Chips.  
 *
 * Called by mcf_exception_handler
 * 
 */
void derivative_interrupt(unsigned long vector);

/***********************************************************************/
/*
 * This is the exception handler for all  exceptions common to all 
 * chips ColdFire.  Most exceptions do nothing, but some of the more 
 * important ones are handled to some extent.
 *
 * Called by asm_exception_handler 
 */
void mcf_exception_handler(void *framepointer);


/***********************************************************************/
/*
 * This is the assembly exception handler defined in the vector table.  
 * This function is in assembler so that the frame pointer can be read  
 * from the stack.
 * Note that the way to give the stack frame as argument to the c handler
 * depends on the used ABI (Register, Compact or Standard).
 *
 */
asm __declspec(register_abi) void asm_exception_handler(void);

/***********************************************************************/
/*
* printf() TRAP #14 handler
 *
 */
#if CONSOLE_IO_SUPPORT == 1
asm __declspec(register_abi) void TrapHandler_printf(void);
#endif

#ifdef __cplusplus
}
#endif

#define VECTOR_002      asm_exception_handler
#define VECTOR_003      asm_exception_handler
#define VECTOR_004      asm_exception_handler
#define VECTOR_005      asm_exception_handler
#define VECTOR_006      asm_exception_handler
#define VECTOR_007      asm_exception_handler                           
#define VECTOR_008      asm_exception_handler                           
#define VECTOR_009      asm_exception_handler
#define VECTOR_010      asm_exception_handler
#define VECTOR_011      asm_exception_handler
#define VECTOR_012      asm_exception_handler
#define VECTOR_013      asm_exception_handler                          
#define VECTOR_014      asm_exception_handler
#define VECTOR_015      asm_exception_handler
#define VECTOR_016      asm_exception_handler
#define VECTOR_017      asm_exception_handler
#define VECTOR_018      asm_exception_handler
#define VECTOR_019      asm_exception_handler
#define VECTOR_020      asm_exception_handler
#define VECTOR_021      asm_exception_handler
#define VECTOR_022      asm_exception_handler
#define VECTOR_023      asm_exception_handler
#define VECTOR_024      asm_exception_handler
#define VECTOR_025      asm_exception_handler
#define VECTOR_026      asm_exception_handler
#define VECTOR_027      asm_exception_handler
#define VECTOR_028      asm_exception_handler
#define VECTOR_029      asm_exception_handler
#define VECTOR_030      asm_exception_handler
#define VECTOR_031      asm_exception_handler
#define VECTOR_032      asm_exception_handler
#define VECTOR_033      asm_exception_handler
#define VECTOR_034      asm_exception_handler
#define VECTOR_035      asm_exception_handler
#define VECTOR_036      asm_exception_handler
#define VECTOR_037      asm_exception_handler
#define VECTOR_038      asm_exception_handler
#define VECTOR_039      asm_exception_handler
#define VECTOR_040      asm_exception_handler
#define VECTOR_041      asm_exception_handler
#define VECTOR_042      asm_exception_handler
#define VECTOR_043      asm_exception_handler
#define VECTOR_044      asm_exception_handler
#define VECTOR_045      asm_exception_handler
#define VECTOR_046      asm_exception_handler
#define VECTOR_047      asm_exception_handler
#define VECTOR_048      asm_exception_handler
#define VECTOR_049      asm_exception_handler
#define VECTOR_050      asm_exception_handler
#define VECTOR_051      asm_exception_handler
#define VECTOR_052      asm_exception_handler
#define VECTOR_053      asm_exception_handler
#define VECTOR_054      asm_exception_handler
#define VECTOR_055      asm_exception_handler
#define VECTOR_056      asm_exception_handler
#define VECTOR_057      asm_exception_handler
#define VECTOR_058      asm_exception_handler
#define VECTOR_059      asm_exception_handler
#define VECTOR_060      asm_exception_handler 
#define VECTOR_061      asm_exception_handler
#define VECTOR_062      asm_exception_handler
#define VECTOR_063      asm_exception_handler
#define VECTOR_064      asm_exception_handler
#define VECTOR_065      asm_exception_handler
#define VECTOR_066      asm_exception_handler
#define VECTOR_067      asm_exception_handler
#define VECTOR_068      asm_exception_handler
#define VECTOR_069      asm_exception_handler
#define VECTOR_070      asm_exception_handler
#define VECTOR_071      asm_exception_handler
#define VECTOR_072      asm_exception_handler
#define VECTOR_073      asm_exception_handler
#define VECTOR_074      asm_exception_handler
#define VECTOR_075      asm_exception_handler 
#define VECTOR_076      asm_exception_handler
#define VECTOR_077      asm_exception_handler
#define VECTOR_078      asm_exception_handler
#define VECTOR_079      asm_exception_handler
#define VECTOR_080      asm_exception_handler
#define VECTOR_081      asm_exception_handler
#define VECTOR_082      asm_exception_handler
#define VECTOR_083      asm_exception_handler
#define VECTOR_084      asm_exception_handler
#define VECTOR_085      asm_exception_handler
#define VECTOR_086      asm_exception_handler
#define VECTOR_087      asm_exception_handler
#define VECTOR_088      asm_exception_handler
#define VECTOR_089      asm_exception_handler
#define VECTOR_090      asm_exception_handler
#define VECTOR_091      asm_exception_handler			 
#define VECTOR_092      asm_exception_handler
#define VECTOR_093      asm_exception_handler
#define VECTOR_094      asm_exception_handler
#define VECTOR_095      asm_exception_handler
#define VECTOR_096      asm_exception_handler
#define VECTOR_097      asm_exception_handler
#define VECTOR_098      asm_exception_handler
#define VECTOR_099      asm_exception_handler
#define VECTOR_100      asm_exception_handler
#define VECTOR_101      asm_exception_handler
#define VECTOR_102      asm_exception_handler
#define VECTOR_103      asm_exception_handler
#define VECTOR_104      asm_exception_handler
#define VECTOR_105      asm_exception_handler
#define VECTOR_106      asm_exception_handler
#define VECTOR_107      asm_exception_handler
#define VECTOR_108      asm_exception_handler    
#define VECTOR_109      asm_exception_handler    
#define VECTOR_110      asm_exception_handler    
#define VECTOR_111      asm_exception_handler    
#define VECTOR_112      asm_exception_handler    
#define VECTOR_113      asm_exception_handler    
#define VECTOR_114      asm_exception_handler    

#endif   /* _MCF_EXCEPTIONS_H */

