/* ======================================================================== *
 * MATHLIB -- TI Floating-Point Math Function Library                       *
 *                                                                          *
 *                                                                          *
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/   *
 *                                                                          *
 *                                                                          *
 *  Redistribution and use in source and binary forms, with or without      *
 *  modification, are permitted provided that the following conditions      *
 *  are met:                                                                *
 *                                                                          *
 *    Redistributions of source code must retain the above copyright        *
 *    notice, this list of conditions and the following disclaimer.         *
 *                                                                          *
 *    Redistributions in binary form must reproduce the above copyright     *
 *    notice, this list of conditions and the following disclaimer in the   *
 *    documentation and/or other materials provided with the                *
 *    distribution.                                                         *
 *                                                                          *
 *    Neither the name of Texas Instruments Incorporated nor the names of   *
 *    its contributors may be used to endorse or promote products derived   *
 *    from this software without specific prior written permission.         *
 *                                                                          *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     *
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       *
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR   *
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT    *
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   *
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        *
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   *
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   *
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     *
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   *
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    *
 * ======================================================================== */

/* ======================================================================= */
/* exp10sp_i.h - single precision floating point exp base 10               */
/*              optimized inlined C Implementation (w/ Intrinsics)         */
/* ======================================================================= */

#ifndef EXP10SP_I_H_
#define EXP10SP_I_H_ 1

#include <ti/mathlib/src/common/common.h>

/*!blw BEGIN PROTOYPE*/
static inline float divspMod_exp10spi (float a, float b);
static inline float exp10sp_i (float a);
/*!blw END PROTOYPE*/

#ifndef __cplusplus         /* FOR PROTECTION PURPOSE - C++ NOT SUPPORTED. */
#pragma CODE_SECTION(divspMod_exp10spi, ".text:optci");
#endif

/* Pull in inline for divsp */
static inline float divspMod_exp10spi (float a, float b) {
  return cmn_DIVSP (a,b);
}

#ifndef __cplusplus         /* FOR PROTECTION PURPOSE - C++ NOT SUPPORTED. */
#pragma CODE_SECTION(exp10sp_i, ".text:optci");
#endif

static inline float exp10sp_i (float a)
{
  float log2_base_x16 =  16.0f * 3.321928095f;
  float Halfe         =   0.5f;
  float LnMine        = -87.33654475f;
  float LnMaxe        =  88.72283905f;
  float Maxe          =   3.402823466E+38f;
  float c0            =   0.1667361910f;
  float c1            =   0.4999999651f;
  float c2            =   0.9999998881f;
  float P1            =   0.04331970214844f;
  float P2            =   1.99663646e-6f;
  float k10e          =   2.302585093f;
  float pol, r, r2, r3, res, Ye;
  unsigned int Ttemp, J, K;
  float	       Nf;
  int          N;
  double       dT;

  Ye = k10e*a;

  /* Get N such that |N - x*16/ln(2)| is minimized */
  Nf = (a * log2_base_x16) + Halfe;
  N  = (int) Nf; /* Cast from intermediate variable to appease MISRA */
  
  if ((a * log2_base_x16) < -Halfe) {
    N--;
  }

  /* Argument reduction, r, and polynomial approximation pol(r) */
  r  = (Ye - (P1 * (float) N)) - (P2 * (float) N);
  r2 = r * r;
  r3 = r * r2;
  
  pol = (r * c2) + ((r3 * c0) + (r2 * c1));
 
  /* Get index for ktable and jtable */
  K  = _extu ((unsigned int)N, 28u, 30u);
  J  = (unsigned int)N & 0x3u;
  dT = ti_math_kTable[K] * ti_math_jTable[J];

  /* Scale exponent to adjust for 2^M */
  Ttemp = _hi(dT) + (((unsigned int) N >> 4) << 20);
  dT    = _itod(Ttemp, _lo(dT));

  res = (float)(dT * (1.0 + (double)pol));
  
  /* Early exit for small a */
  if (_extu(_ftoi(Ye), 1u, 24u) < 114u) {
    res = 1.0f + Ye;        
  }

  /* < LnMin returns 0 */
  if (Ye < LnMine) {
    res = 0.0f; 
  }

  /* > LnMax returns MAX */   
  if (Ye > LnMaxe) {
    res = Maxe;
  } 

  return(res);
}

#endif

/* ======================================================================== */
/*  End of file: exp10sp_i.h                                                */
/* ======================================================================== */
