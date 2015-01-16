/*
 * File:        common.h
 * Purpose:     File to be included by all project files
 *
 * Notes:
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/********************************************************************/

/*
 * Debug prints ON (#define) or OFF (#undef)
 */
#define DEBUG
#define DEBUG_PRINT

/*
 * Include the generic CPU header file
 */
#include "derivative.h"

/*
 * Include the platform specific header file
 */
#if (defined(TOWER))
  #include "board.h"
#else
  #error "No valid platform defined"
#endif

/*
 * Include the generic mcg init header file
 */

#if defined(CPU_MK40X256VMD100) || defined(CPU_MK60N512VMD100) || defined(CPU_MK53DZ10)
  #include "mcg.h"
#elif defined(CPU_MK70F120)
  #include "mcg_k70.h"
#elif defined(CPU_MK20DZ50) || defined(CPU_MK20DZ72)
  #include "mcg_k20.h"
#elif defined(CPU_MKL25Z128LK4) || defined(CPU_MKL46Z48M)
  #include "mcg_kl25.h"
#elif defined(CPU_MKL05Z32LF4)
  #include "mcg_kl05.h"
#elif defined(CPU_MK60D10)
  #include "mcg_k60d.h"
#else
  #error "No mcg init file selected"
#endif

/*
 * Include common utilities
 */

#include "startup.h"
#include "stdlib.h"

/*
 * Include any specfic header files
 */
#if (defined(IAR))
  #include "intrinsics.h"
#endif


/********************************************************************/

#endif /* _COMMON_H_ */
