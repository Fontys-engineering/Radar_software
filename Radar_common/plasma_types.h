/*
 * Copyright (c) 2026,
 *      Fontys University of Applied Sciences
 *          Mete Han Keskin < m.keskin @ student . fontys . nl >
 *          Vlad Nepriakhin < v.nepriakhin @ student . fontys . nl >
 *      All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS `AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _PLASMA_TYPES_H
#define _PLASMA_TYPES_H

#include <stdint.h>

#define FNTYS_PLASMARAD_VERSION 1

#define PLASMA_OUT_RANGE_PROFILE (1U << 0)
#define PLASMA_OUT_IQ_TAPS (1U << 1)
#define PLASMA_OUT_DIAG (1U << 2)
#define PLASMA_OUT_UART_TLV (1U << 3)

typedef struct {
  int8_t subFrameIdx;   /* -1 = all */
  uint8_t nPhaseBins;   /* 1..8, odd */
  uint8_t objDetEnable; /* run TI CFAR/AoA */
  uint8_t rsvd;
  uint32_t outputMask;
  float gateMinM, gateMaxM;
  float snrThrDb;
  float rangeOffsetM; /* R_cal */
  float phaseOffsetRad;
} Plasma_Cfg;

#endif
