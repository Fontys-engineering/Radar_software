/*
 *   @file  mmwavelink_test_common.h
 *
 *   @brief
 *      Header file for link_test common function across different platforms
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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

#ifndef MMWAVELINKTESTCOMMON_H
#define MMWAVELINKTESTCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <kernel/dpl/HwiP.h>

#define OSAL_DEFAULT_PRIORITY   (~((uint32_t) 0U))

typedef void (*MmwLink_OsalIsrFxn)(uintptr_t arg);

HwiP_Object                     hwiFrameStartHwiObject;
HwiP_Object                     hwiChirpStartHwiObject;

int32_t mmWaveLink_osalRegisterIntr(MmwLink_OsalIsrFxn isrFxn,
                                   uint32_t coreIntrNum,
                                   uint32_t intrPriority,
                                   void *arg,
                                   HwiP_Object *hwiFrameStartHwiObject);


extern void MmwaveLink_startTest(uint8_t platformType, uint8_t platformCore);
extern void Mmwavelink_frameInterrupCallBackFunc(void* arg);
extern void Mmwavelink_chirpAvailInterrupCallBackFunc(void* arg);

/************************************************************************************************
 * Advanced chirp configuration to achieve the Modulation scheme
 *In following MACROs, we have defined multiple device configuration, user can change that as needed
 * 
 */

#define FREQ_DITHER_MHZ          (1.4/1000)  /* 1.4 MHz */

#define INCR_START_FREQ_DITH     ((rlInt32_t)(((double)(FREQ_DITHER_MHZ) * 67108864.0) / (3.6)))

#define DCR_START_FREQ_DITH     ((rlInt32_t) -(((double)(FREQ_DITHER_MHZ) * 67108864.0) / (3.6)))

/* Chirp IDle Time Dither : Based idle time from Profile (ID0 and ID1) config is (25+8)=33usec */
#define CHIRP_IDLE_TIME_DITH   (2) /* 2 usec */

/* change num of chirps per cycle here */
#define CHIRPS_PER_CYCLE   512
#define NUM_UNIQ_CYCLES    6
#define TX_ENABLE_BIT      0xF
#define TX_LUT_PATTERNS_SIZE  4

/* Profile Configuration paramters */
/***** PROFILE-0 ****/
#define PROFILE_0_START_FREQ     78
#define PROFILE_0_IDLE_TIME      11
#define PROFILE_0_ADC_START_TIME 3
#define PROFILE_0_RAMP_END_TIME  30
#define PROFILE_0_FREQ_SLOPE     2.84
#define PROFILE_0_TX_START_TIME  0
#define PROFILE_0_NUM_SAMPLE     512
#define PROFILE_0_SAMPLE_RATE    30000
#define PROFILE_0_RX_GAIN        36
/***** PROFILE-1 ****/
#define PROFILE_1_START_FREQ     (78+((CHIRPS_PER_CYCLE/2) * FREQ_DITHER_MHZ)) /* [78.358 GHz] applied after 256 chirp in given cycle (512 chirps) */
#define PROFILE_1_IDLE_TIME      11
#define PROFILE_1_ADC_START_TIME 3
#define PROFILE_1_RAMP_END_TIME  30
#define PROFILE_1_FREQ_SLOPE     2.84 /* positive slope */
#define PROFILE_1_TX_START_TIME  0
#define PROFILE_1_NUM_SAMPLE     512
#define PROFILE_1_SAMPLE_RATE    30000
#define PROFILE_1_RX_GAIN        36
/***** PROFILE-2 ****/
#define PROFILE_2_START_FREQ     (78+(CHIRPS_PER_CYCLE * FREQ_DITHER_MHZ))  /* [78.716 GHz] */
#define PROFILE_2_IDLE_TIME      11
#define PROFILE_2_ADC_START_TIME 3
#define PROFILE_2_RAMP_END_TIME  30
#define PROFILE_2_FREQ_SLOPE     -2.84 /* negative slope */
#define PROFILE_2_TX_START_TIME  0
#define PROFILE_2_NUM_SAMPLE     512
#define PROFILE_2_SAMPLE_RATE    30000
#define PROFILE_2_RX_GAIN        36
/***** PROFILE-3 ****/
#define PROFILE_3_START_FREQ     (78+((CHIRPS_PER_CYCLE/2) * FREQ_DITHER_MHZ))  /* [78.358 GHz] */
#define PROFILE_3_IDLE_TIME      11
#define PROFILE_3_ADC_START_TIME 3
#define PROFILE_3_RAMP_END_TIME  30
#define PROFILE_3_FREQ_SLOPE     -2.84 /* negative slope */
#define PROFILE_3_TX_START_TIME  0
#define PROFILE_3_NUM_SAMPLE     512
#define PROFILE_3_SAMPLE_RATE    30000
#define PROFILE_3_RX_GAIN        36


/* LUT Buffer size for Advanced chirp 
   Max size = 12KB (12*1024) */
#define LUT_ADVCHIRP_TABLE_SIZE                4*1024
#define DUMMY_LUT_ADDR_OFFSET                  3*1024 /* point to dummy LUT address offset */

#define NUM_ANALOG_FAULT_TEST                   10
#define ANALOG_FAULT_RX_GAIN_DROP               0
#define ANALOG_FAULT_RX_PHASE_INV               1
#define ANALOG_FAULT_RX_IF_STAGE                2
#define ANALOG_FAULT_TX_GAIN_DROP               3
#define ANALOG_FAULT_TX_PHASE_INV               4
#define ANALOG_FAULT_SYNTH_FREQ                 5
#define ANALOG_FAULT_SUPPLY_LDO                 6
#define ANALOG_FAULT_MISC                       7
#define ANALOG_FAULT_MISC_THRESHOLD             8
#define ANALOG_FAULT_TX_PS_DAC                  9

#ifdef __cplusplus
}
#endif

#endif
