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
/* divsp.c - single precision floating point divide                        */
/*              optimized  C implementation (w/ intrinsics) single sample  */
/* ======================================================================= */

#include "divsp_i.h"

#if (OVERRIDE_RTS == 1)
/* RTS override version: divf is an internal RTS function and is not intended 
   to be called directly from application program. Naming convention can be 
   found at Table 7-11 of TMS320C6000 Optimizing Compiler v7.6 User's Guide. */
#if defined(__TI_EABI__)
float __c6xabi_divf (float a, float b) /* refered as __c6xabi_divf by RTS */
#else
float _divf (float a, float b)         /* refered as __divf by RTS */
#endif
#else
/* Normal MATHLIB usage version: supposed to be assembly version compared to C
   version divsp_c, but for now calls the same inline function as C version. */
#ifndef __cplusplus         /* FOR PROTECTION PURPOSE - C++ NOT SUPPORTED. */
#pragma CODE_SECTION(divsp, ".text:optasm");
#endif
#include "divsp.h"
float divsp (float a, float b)
#endif

{
  return (divsp_i(a,b));
}

/* ======================================================================== */
/*  End of file: divsp.c                                                    */
/* ======================================================================== */

