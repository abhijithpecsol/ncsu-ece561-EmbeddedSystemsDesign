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
* $Version : 3.6.4.0$
* $Date    : Aug-23-2012$
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
#include "app_init.h"
#include "freemaster.h"
#include "irq_common.h"
#include "llwu_common.h"

#undef  VECTOR_110
#define VECTOR_110 TSS_MTIMIsr

#undef VECTOR_066
#define VECTOR_066 llwu_isr

#undef  VECTOR_097
#define VECTOR_097 TSS_TSI0Isr

#undef VECTOR_064
#define VECTOR_064 interrupt_handler_irq

/* Example */
/*
#undef  VECTOR_101
#define VECTOR_101 lpt_isr

// ISR(s) are defined in your project directory.
extern void lpt_isr(void);
*/


#endif  //__ISR_H

/* End of "isr.h" */
