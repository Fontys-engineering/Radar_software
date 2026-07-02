/**
 *   @file  dpif_types.h
 *
 *   @brief
 *      Defines the common data types used in data path data interface.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 - 2025 Texas Instruments, Inc.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DPIF_TYPES_H
#define DPIF_TYPES_H


#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  Data format for data Path interface buffers
 *
 * @details
 *  The enumeration describes the data format used in data interface buffers between data processing unit
 */
typedef uint32_t DPIF_DATAFORMAT;
/*! @brief  Complex 16bit for imaginary/real */
#define DPIF_DATAFORMAT_COMPLEX16_IMRE          (uint32_t)  0
/*! @brief  Complex 16bit for real/imaginary */
#define DPIF_DATAFORMAT_COMPLEX16_REIM          (uint32_t)  1
/*! @brief  Complex 32bit for imaginary/real */
#define DPIF_DATAFORMAT_COMPLEX32_IMRE          (uint32_t)  2
/*! @brief  Complex 32bit for real/imaginary */
#define DPIF_DATAFORMAT_COMPLEX32_REIM          (uint32_t)  3
/*! @brief  Float data format */
#define DPIF_DATAFORMAT_FLOAT                   (uint32_t)  4
/*! @brief  Real 16bit data */
#define DPIF_DATAFORMAT_REAL16                  (uint32_t)  5
/*! @brief  Real 32bit data */
#define DPIF_DATAFORMAT_REAL32                  (uint32_t)  6

#ifdef __cplusplus
}
#endif

#endif /* DPIF_TYPES_H */
