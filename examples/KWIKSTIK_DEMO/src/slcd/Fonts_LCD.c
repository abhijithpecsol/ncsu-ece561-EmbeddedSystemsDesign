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
* $FileName: Fonts_LCD.c$
* $Version : 3.6.1.0$
* $Date    : Dec-15-2011$
*
* Comments:
*
*   This file contains the source for the SLCD fonts.
*
*END************************************************************************/
#include "Fonts_LCD.h"

/**
  Global Constant Arrays.
  These arrays have the definition for the different characters. Currently
  only numbers, capital letters, exclamation and colon mark.
*/
const uint8_t _0[CHAR_SIZE] =
{
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,1,1,
  1,0,1,0,1,
  1,1,0,0,1,
  1,0,0,0,1,
  0,1,1,1,0,
};
const uint8_t _1[CHAR_SIZE]=
{
  0,0,1,0,0,
  0,1,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,1,1,1,0
};
const uint8_t _2[CHAR_SIZE]=
{
  0,1,1,1,0,
  1,0,0,0,1,
  0,0,0,0,1,
  0,0,0,1,0,
  0,0,1,0,0,
  0,1,0,0,0,
  1,1,1,1,1
};
const uint8_t _3[CHAR_SIZE]=
{
  1,1,1,1,1,
  0,0,0,1,0,
  0,0,1,0,0,
  0,0,0,1,0,
  0,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,0
};
const uint8_t _4[CHAR_SIZE]=
{
  0,0,0,1,0,
  0,0,1,1,0,
  0,1,0,1,0,
  1,0,0,1,0,
  1,1,1,1,1,
  0,0,0,1,0,
  0,0,0,1,0
};
const uint8_t _5[CHAR_SIZE]=
{
  1,1,1,1,1,
  1,0,0,0,0,
  1,1,1,1,0,
  0,0,0,0,1,
  0,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,0
};
const uint8_t _6[CHAR_SIZE]=
{
  0,0,1,1,0,
  0,1,0,0,0,
  1,0,0,0,0,
  1,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,0
};
const uint8_t _7[CHAR_SIZE]=
{
  1,1,1,1,1,
  0,0,0,0,1,
  0,0,0,1,0,
  0,0,1,0,0,
  0,1,0,0,0,
  0,1,0,0,0,
  0,1,0,0,0
};
const uint8_t _8[CHAR_SIZE]=
{
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,0
};
const uint8_t _9[CHAR_SIZE]=
{
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,1,
  0,0,0,0,1,
  0,0,0,1,0,
  0,1,1,0,0
};
const uint8_t _A[CHAR_SIZE]=
{
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,1,1,1,1,
  1,0,0,0,1,
  1,0,0,0,1
};
const uint8_t _B[CHAR_SIZE]=
{
  1,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  1,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  1,1,1,1,0
};
const uint8_t _C[CHAR_SIZE]=
{
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,0,
  1,0,0,0,0,
  1,0,0,0,0,
  1,0,0,0,1,
  0,1,1,1,0
};
const uint8_t _D[CHAR_SIZE]=
{
  1,1,1,0,0,
  1,0,0,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,1,0,
  1,1,1,0,0
};
const uint8_t _E[CHAR_SIZE]=
{
  1,1,1,1,1,
  1,0,0,0,0,
  1,0,0,0,0,
  1,1,1,1,0,
  1,0,0,0,0,
  1,0,0,0,0,
  1,1,1,1,1
};
const uint8_t _F[CHAR_SIZE]=
{
  1,1,1,1,1,
  1,0,0,0,0,
  1,0,0,0,0,
  1,1,1,1,0,
  1,0,0,0,0,
  1,0,0,0,0,
  1,0,0,0,0
};
const uint8_t _G[CHAR_SIZE]=
{
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,0,
  1,0,1,1,1,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,1
};
const uint8_t _H[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,1,1,1,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1
};
const uint8_t _I[CHAR_SIZE]=
{
  0,1,1,1,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,1,1,1,0
};
const uint8_t _J[CHAR_SIZE]=
{
  0,0,1,1,1,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  0,0,0,1,0,
  1,0,0,1,0,
  0,1,1,0,0
};
const uint8_t _K[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,0,0,1,0,
  1,0,1,0,0,
  1,1,0,0,0,
  1,0,1,0,0,
  1,0,0,1,0,
  1,0,0,0,1
};
const uint8_t _L[CHAR_SIZE]=
{
  1,0,0,0,0,
  1,0,0,0,0,
  1,0,0,0,0,
  1,0,0,0,0,
  1,0,0,0,0,
  1,0,0,0,0,
  1,1,1,1,1
};
const uint8_t _M[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,1,0,1,1,
  1,0,1,0,1,
  1,0,1,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1
};
const uint8_t _N[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,0,0,0,1,
  1,1,0,0,1,
  1,0,1,0,1,
  1,0,0,1,1,
  1,0,0,0,1,
  1,0,0,0,1
};
const uint8_t _O[CHAR_SIZE]=
{
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,0
};
const uint8_t _P[CHAR_SIZE]=
{
  1,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  1,1,1,1,0,
  1,0,0,0,0,
  1,0,0,0,0,
  1,0,0,0,0
};
const uint8_t _Q[CHAR_SIZE]=
{
  0,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,1,0,1,
  1,0,0,1,0,
  0,1,1,0,1
};
const uint8_t _R[CHAR_SIZE]=
{
  1,1,1,1,0,
  1,0,0,0,1,
  1,0,0,0,1,
  1,1,1,1,0,
  1,0,1,0,0,
  1,0,0,1,0,
  1,0,0,0,1
};
const uint8_t _S[CHAR_SIZE]=
{
  0,1,1,1,1,
  1,0,0,0,0,
  1,0,0,0,0,
  0,1,1,1,0,
  0,0,0,0,1,
  0,0,0,0,1,
  1,1,1,1,0
};
const uint8_t _T[CHAR_SIZE]=
{
  1,1,1,1,1,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0
};
const uint8_t _U[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,1,1,0
};
const uint8_t _V[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,0,1,0,
  0,0,1,0,0
};
const uint8_t _W[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,1,0,1,
  1,0,1,0,1,
  1,0,1,0,1,
  0,1,0,1,0
};
const uint8_t _X[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,0,1,0,
  0,0,1,0,0,
  0,1,0,1,0,
  1,0,0,0,1,
  1,0,0,0,1
};
const uint8_t _Y[CHAR_SIZE]=
{
  1,0,0,0,1,
  1,0,0,0,1,
  1,0,0,0,1,
  0,1,0,1,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0
};
const uint8_t _Z[CHAR_SIZE]=
{
  1,1,1,1,1,
  0,0,0,0,1,
  0,0,0,1,0,
  0,0,1,0,0,
  0,1,0,0,0,
  1,0,0,0,0,
  1,1,1,1,1
};

const uint8_t _Exclamation[CHAR_SIZE] =
{
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,1,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,1,0,0
};

const uint8_t _Dot[CHAR_SIZE] =
{
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,1,1,0,0,
  0,1,1,0,0
};

const uint8_t _Colon[CHAR_SIZE] =
{
  0,0,0,0,0,
  0,1,1,0,0,
  0,1,1,0,0,
  0,0,0,0,0,
  0,1,1,0,0,
  0,1,1,0,0,
  0,0,0,0,0
};

const uint8_t _SPACE[CHAR_SIZE] =
{
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0,
  0,0,0,0,0
};

const uint8_t * Fonts[] =
{
  _SPACE,
  _Exclamation,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _Dot,
  _NULL,
  (uint8_t *)_0,
  (uint8_t *)_1,
  (uint8_t *)_2,
  (uint8_t *)_3,
  (uint8_t *)_4,
  (uint8_t *)_5,
  (uint8_t *)_6,
  (uint8_t *)_7,
  (uint8_t *)_8,
  (uint8_t *)_9,
  (uint8_t *)_Colon,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  (uint8_t *)_A,
  (uint8_t *)_B,
  (uint8_t *)_C,
  (uint8_t *)_D,
  (uint8_t *)_E,
  (uint8_t *)_F,
  (uint8_t *)_G,
  (uint8_t *)_H,
  (uint8_t *)_I,
  (uint8_t *)_J,
  (uint8_t *)_K,
  (uint8_t *)_L,
  (uint8_t *)_M,
  (uint8_t *)_N,
  (uint8_t *)_O,
  (uint8_t *)_P,
  (uint8_t *)_Q,
  (uint8_t *)_R,
  (uint8_t *)_S,
  (uint8_t *)_T,
  (uint8_t *)_U,
  (uint8_t *)_V,
  (uint8_t *)_W,
  (uint8_t *)_X,
  (uint8_t *)_Y,
  (uint8_t *)_Z,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL,
  _NULL
};
