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
  uint8_t nPhaseBins;   /* 1..7, odd */
  uint8_t objDetEnable; /* run TI CFAR/AoA */
  uint8_t rsvd;
  uint32_t outputMask;
  float gateMinM, gateMaxM;
  float snrThrDb;
  float rangeOffsetM; /* R_cal */
  float phaseOffsetRad;
} Plasma_Cfg;

/* ---- Plasma_Record: one record per (sub)frame, MSS -> Ethernet (Doc/07) ----
 * Little-endian, record start 8 B aligned, sections 4 B aligned + zero pad.
 * Layout is the wire format: fields are naturally aligned, so no packing.
 * Sections follow the header back to back: rec + headerBytes. */

/* compile time size check, C89 safe so the C66x side builds it as well */
#define PLASMA_STATIC_ASSERT(cond, name) typedef char name[(cond) ? 1 : -1]

#define PLASMA_RECORD_MAGIC 0x4D534C50U /* 'P','L','S','M' on the wire */
#define PLASMA_RECORD_VERSION 1U

/* Plasma_Record.flags */
#define PLASMA_FLAG_VALID (1U << 0) /* peak above snrThrDb */
#define PLASMA_FLAG_CAL_APPLIED (1U << 1)
#define PLASMA_FLAG_PHASE_UNWRAP_RESET (1U << 2)
#define PLASMA_FLAG_SATURATION (1U << 3)
#define PLASMA_FLAG_TRUNCATED (1U << 4) /* section clamped to fit the slot */

/* Plasma_SectionHdr.type, host skips unknown types using .bytes */
#define PLASMA_SECT_BOUNDARY 1U /* always present */
#define PLASMA_SECT_RANGE_PROFILE 2U
#define PLASMA_SECT_IQ_TAPS 3U
#define PLASMA_SECT_DIAG 4U

typedef struct {
  uint32_t magic;       /* PLASMA_RECORD_MAGIC */
  uint16_t version;     /* PLASMA_RECORD_VERSION */
  uint16_t headerBytes; /* sizeof(Plasma_Record) */
  uint32_t totalBytes;  /* header + all sections */
  uint32_t seq;         /* +1 per committed record */
  uint32_t frameNumber; /* BSS frame counter */
  uint32_t dropCount;   /* cumulative producer drops */
  uint64_t timestampUs; /* MSS time at record build */
  uint8_t subFrameIdx;
  uint8_t numSubFrames;
  uint16_t numSections;
  uint32_t fCenterKHz; /* chirp centre freq of this (sub)frame */
  uint32_t dspCycles;  /* PlasmaDsp_process cycle count */
  uint32_t flags;      /* PLASMA_FLAG_* */
} Plasma_Record;

typedef struct {
  uint16_t type; /* PLASMA_SECT_* */
  uint16_t rsvd;
  uint32_t bytes; /* incl. this header and padding */
} Plasma_SectionHdr;

typedef struct {
  Plasma_SectionHdr hdr;
  float rangeM; /* gated peak, quadFit, minus rangeOffsetM */
  float velCoarseMps;
  float velFineMps;
  float snrDb;
  float phaseRad; /* unwrapped across frames */
  float displacementM;
  uint16_t peakBin;
  uint16_t rsvd;
} Plasma_SectBoundary;

/* followed by uint16_t powDbX100[numBins], padded to 4 B */
typedef struct {
  Plasma_SectionHdr hdr;
  uint16_t firstBin;
  uint16_t numBins;
  float binSizeM;
} Plasma_SectRangeProfile;

/* followed by Plasma_Cplx16[numBins][numChirps], Rx combined */
typedef struct {
  Plasma_SectionHdr hdr;
  uint16_t firstBin;
  uint16_t numBins;
  uint16_t numChirps;
  uint16_t chirpPeriodX10ns; /* Tc in 10 ns units, for velFine on the host */
} Plasma_SectIqTaps;

typedef struct {
  int16_t re;
  int16_t im;
} Plasma_Cplx16;

typedef struct {
  Plasma_SectionHdr hdr;
  uint32_t interFrameMarginUs;
  int16_t tempC;
  uint16_t rsvd;
} Plasma_SectDiag;

/* sizes are part of the wire contract with the host, see Doc/07 */
PLASMA_STATIC_ASSERT(sizeof(Plasma_Record) == 48, Plasma_assertRecord);
PLASMA_STATIC_ASSERT(sizeof(Plasma_SectionHdr) == 8, Plasma_assertSectHdr);
PLASMA_STATIC_ASSERT(sizeof(Plasma_SectBoundary) == 36, Plasma_assertBoundary);
PLASMA_STATIC_ASSERT(sizeof(Plasma_SectRangeProfile) == 16,
                     Plasma_assertRangeProf);
PLASMA_STATIC_ASSERT(sizeof(Plasma_SectIqTaps) == 16, Plasma_assertIqTaps);
PLASMA_STATIC_ASSERT(sizeof(Plasma_SectDiag) == 16, Plasma_assertDiag);

#endif
