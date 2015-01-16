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
* $FileName: isr.h$
* $Version : 3.6.23.0$
* $Date    : Sep-13-2012$
*
* Comments:
*
*   Define interrupt service routines referenced by the vector table.
*   Only "vectors.c" should include this header file.
*
*END************************************************************************/
#ifndef ISR_H
  #define ISR_H

  #include "TSS_API.h"
  #include "freemaster.h"
  #include "board.h"

  /* Interrupt Vectors definition */

  #undef  VECTOR_053
  #define VECTOR_053       TSS_TSI0Isr
  #undef  VECTOR_046
  #define VECTOR_046       PITIsr
  #undef  VECTOR_058
  #define VECTOR_058       LPSWIsr
  #undef  VECTOR_025
  #define VECTOR_025       LLWUIsr

  #if (FMSTR_SCI_INTERRUPT == 49)
    #undef  VECTOR_049
    #define VECTOR_049 FMSTR_Isr
  #elif (FMSTR_SCI_INTERRUPT == 61)
    #undef  VECTOR_061
    #define VECTOR_061 FMSTR_Isr
  #elif (FMSTR_SCI_INTERRUPT == 63)
    #undef  VECTOR_063
    #define VECTOR_063 FMSTR_Isr
  #elif (FMSTR_SCI_INTERRUPT == 67)
    #undef  VECTOR_067
    #define VECTOR_067 FMSTR_Isr
  #elif (FMSTR_SCI_INTERRUPT == 65)
    #undef  VECTOR_065
    #define VECTOR_065 FMSTR_Isr
  #elif (FMSTR_SCI_INTERRUPT == 34)
    #undef  VECTOR_034
    #define VECTOR_034 FMSTR_Isr
  #elif (FMSTR_SCI_INTERRUPT == 28)
    #undef  VECTOR_028
    #define VECTOR_028 FMSTR_Isr
  #endif

  /* Example */
  /*
  #undef  VECTOR_101
  #define VECTOR_101 lpt_isr

  // ISR(s) are defined in your project directory.
  extern void lpt_isr(void);
  */

#endif  /* ISR_H */

/* End of "isr.h" */
