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
 *
 */
#if defined(FRDM_KL25Z48) || defined(FRDM_KL26Z48)
  #include "mcg_kl25.h"
#elif defined(FRDM_KL05Z48) || defined(FRDM_KL02Z48)
  #include "mcg_kl05.h"
#endif

/*
 * Include the platform specific header file
 */
#if (defined(FREEDOM))
  #include "board.h"
#else
  #error "No valid platform defined"
#endif

/*
 * Include any toolchain specfic header files
 */
#if (defined(CW))
  #include "cw.h"
#elif (defined(IAR))
  /* Nothing */
#endif

/*
 * Include common utilities
 */
#include "assert.h"
#include "io.h"
#include "startup.h"
#include "stdlib.h"

#if (defined(IAR))
  #include "intrinsics.h"
#endif

/********************************************************************/

#endif /* _COMMON_H_ */
