/**
 *   @file  mmwave_error.h
 *
 *   @brief
 *      Base error codes for the mmWave modules.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016-2021 Texas Instruments, Inc.
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

#ifndef MMWAVE_ERROR_H
#define MMWAVE_ERROR_H


#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************
 * Base Error Code for the mmWave modules
 **************************************************************************/
#define MMWAVE_ERRNO_BASE                    (-3100)
#define MMWAVE_ERRNO_DPM_BASE                (-4200)

/**************************************************************************
 * Base Error Code for the mmWave data path (-30000 - -59999)
 **************************************************************************/
#define MMWAVE_ERRNO_DPU_BASE               (-30000)
#define MMWAVE_ERRNO_DPC_BASE               (-40000)
#define MMWAVE_ERRNO_DEMO_BASE              (-50000)

#ifdef __cplusplus
}
#endif

#endif  /* MMWAVE_ERROR_H */

