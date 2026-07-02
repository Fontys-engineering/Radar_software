/**
 *   @file  sys_common_awr294x.h
 *
 *   @brief
 *      This is the common header file used by the various mmWave SDK
 *      modules for AWR294X device.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020-2021 Texas Instruments, Inc.
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

#ifndef SYS_COMMON_AWR294X_H
#define SYS_COMMON_AWR294X_H

#ifdef __cplusplus
extern "C" {
#endif

#if (defined(SOC_AWR2944))

/*************************************************************
 * System level defines
 *************************************************************/
#define SYS_COMMON_NUM_TX_ANTENNAS             4U

#define SYS_COMMON_NUM_AZIMTX_ANTENNAS         3U
#define SYS_COMMON_NUM_ELEVTX_ANTENNAS         1U

/* DSP Clock Frequency for AWR294X */
#define DSP_CLOCK_MHZ                          360U           /* DSP Clock frequency in MHz */

/* System level memory defines */
#define SYS_COMMON_HSRAM_SIZE                  0x8000U        /* Size: 32KB */
#define SOC_ADCBUF_SIZE                        0x8000U        /* Size: 32KB */


#endif /* #if (defined(SOC_AWR2944)) */

#ifdef __cplusplus
}
#endif

#endif /* SYS_COMMON_AWR294X_H */
