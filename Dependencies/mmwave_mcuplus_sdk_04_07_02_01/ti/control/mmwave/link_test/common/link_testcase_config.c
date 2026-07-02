/* ======================================================================
 *   Copyright (C) 2015 Texas Instruments Incorporated
 *
 *   All rights reserved. Property of Texas Instruments Incorporated.
 *   Restricted rights to use, duplicate or disclose this code are
 *   granted through contract.
 *
 *   The program may not be used without the written permission
 *   of Texas Instruments Incorporated or against the terms and conditions
 *   stipulated in the agreement under which this program has been
 *   supplied.
 * ==================================================================== */

/**
 *  \file link_testcase_config.c
 *
 *  \brief This file defines the device configurations like driver config etc...
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files */
#include <stdio.h>
#include <stdbool.h>

/* mmWave SDK Include File */
#include <ti/control/mmwavelink/mmwavelink.h>

/* Link Test Include File */
#include <ti/control/mmwave/link_test/common/link_testcase_setting.h>

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/


/**************************************************************************
 *************************** Global Structures*****************************
 **************************************************************************/

const rlChanCfg_t chCfg  =
{
    .rxChannelEn =RX_CHANNEL_1_2_3_4_ENABLE,
    .txChannelEn = TX_CHANNEL_1_2_3_4_ENABLE,
    .cascading   = 0x0,
    .cascadingPinoutCfg   = 0x0,
};

const rlAdcOutCfg_t adcOutCfgArgs =
{
    .fmt.b2AdcBits = ADC_BITS_16,
    .fmt.b2AdcOutFmt = ADC_FORMAT_REAL,
    .fmt.b8FullScaleReducFctr = 0U,
    .reserved0  = 0x0,
    .reserved1  = 0x0,
};

const rlLowPowerModeCfg_t lowPowerModeCfg =
{
    .reserved  = 0x0,
    .lpAdcMode = LP_ADC_MODE_LOW_POWER,
};

const rlDevHsiClk_t deviceHsiClk =
{
    .hsiClk = 9,
    .reserved = 0x0,
};

const rlProfileCfg_t  dummyProfileCfg =
{
    .profileId = 0,
    .pfVcoSelect = 0x0,
    .pfCalLutUpdate = 0x0,
    .startFreqConst = PROFILE_START_FREQ_VAL,
    .idleTimeConst = PROFILE_IDLE_TIME_VAL,
    .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
    .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
    .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
    .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
    .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL,
    .txStartTime = PROFILE_TX_START_TIME_VAL,
    .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
    .digOutSampleRate = 10000,
    .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
    .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
    .txCalibEnCfg = 0x0,
    .rxGain = PROFILE_RX_GAIN_VAL,
    .reserved = 0x0
};

const rlProfileCfg_t  profileCfg[7U] =
{
    {
        .profileId = CHIRP_PROFILE_ID,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = 10000,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+1,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+2,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+3,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+4,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+5,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    },
    {
        .profileId = CHIRP_PROFILE_ID+6,
        .pfVcoSelect = 0x0,
        .pfCalLutUpdate = 0x0,
        .startFreqConst = PROFILE_START_FREQ_VAL,
        .idleTimeConst = PROFILE_IDLE_TIME_VAL,
        .adcStartTimeConst = PROFILE_ADC_START_TIME_VAL,
        .rampEndTime = PROFILE_RAMP_END_TIME_VAL,
        .txOutPowerBackoffCode = PROFILE_TXOUT_POWER_BACKOFF,
        .txPhaseShifter = PROFILE_TXPHASESHIFTER_VAL,
        .freqSlopeConst = PROFILE_FREQ_SLOPE_VAL,
        .txStartTime = PROFILE_TX_START_TIME_VAL,
        .numAdcSamples = PROFILE_ADC_SAMPLE_VAL,
        .digOutSampleRate = PROFILE_DIGOUT_SAMPLERATE_VAL2,
        .hpfCornerFreq1 = PROFILE_HPFCORNER_FREQ1_VAL,
        .hpfCornerFreq2 = PROFILE_HPFCORNER_FREQ2_VAL,
        .txCalibEnCfg = 0x0,
        .rxGain = PROFILE_RX_GAIN_VAL,
        .reserved = 0x0
    }
};

const rlChirpCfg_t  dummyChirpCfg =
{
    .chirpStartIdx = CHIRP_START_INDEX,
    .chirpEndIdx = CHIRP_END_INDEX + 4,
    .profileId = CHIRP_PROFILE_ID,
    .reserved = 0x0,
    .startFreqVar = CHIRP_START_FREQ_VAL,
    .freqSlopeVar= CHIRP_FREQ_SLOPE_VAL,
    .idleTimeVar = CHIRP_IDLE_TIME_VAL,
    .adcStartTimeVar = CHIRP_ADC_START_TIME_VAL,
    .txEnable = TX_CHANNEL_1_2_3_4_ENABLE
};

const rlChirpCfg_t  chirpCfg[2] =
{
    {
        .chirpStartIdx = CHIRP_START_INDEX,
        .chirpEndIdx = CHIRP_END_INDEX+255,
        .profileId = CHIRP_PROFILE_ID,
        .reserved = 0x0,
        .startFreqVar = CHIRP_START_FREQ_VAL,
        .freqSlopeVar= CHIRP_FREQ_SLOPE_VAL,
        .idleTimeVar = CHIRP_IDLE_TIME_VAL,
        .adcStartTimeVar = CHIRP_ADC_START_TIME_VAL,
        .txEnable = TX_CHANNEL_1_2_3_4_ENABLE
    },
    {
        .chirpStartIdx = CHIRP_START_INDEX + 256U,
        .chirpEndIdx = CHIRP_END_INDEX + 511U,
        .profileId = CHIRP_PROFILE_ID + 1U,
        .reserved = 0x0,
        .startFreqVar = CHIRP_START_FREQ_VAL,
        .freqSlopeVar= CHIRP_FREQ_SLOPE_VAL,
        .idleTimeVar = CHIRP_IDLE_TIME_VAL,
        .adcStartTimeVar = CHIRP_ADC_START_TIME_VAL,
        .txEnable = TX_CHANNEL_1_2_3_4_ENABLE
    }
};

const rlDynChirpCfg_t dynChirpCfg[32]=
{
    {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 0,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 1,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 2,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 3,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 4,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 5,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 6,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 7,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 8,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 9,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 10,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 11,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 12,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 13,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 14,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 15,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 16,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 17,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 18,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 19,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 20,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 21,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 22,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 23,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 24,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 25,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 26,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 27,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 28,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 29,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
      {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 30,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },
     {
        .chirpRowSelect = 0x00,//0x30,
        .chirpSegSel    = 31,
        .programMode    = 0,
        .chirpRow={ {0x00000FF0, 0x00000FF0, 0x00000FF0},
                    {0x00000FF1, 0x00000FF1, 0x00000FF0},
                     {0x00000FF2, 0x00000FF0, 0x00000FF1},
                    {0x00000FF3, 0x00000FFF, 0x00000FF2},
                    {0x00000FF4, 0x00000FF0, 0x00000FF3},
                     {0x00000FF5, 0x00000FFE, 0x00000FF4},
                     {0x00000FF6, 0x00000FF0, 0x00000FF0},
                     {0x00000FF7, 0x00000FFD, 0x00000FF0},
                     {0x00000FF8, 0x00000FF0, 0x00000FF1},
                     {0x00000FF9, 0x00000FFA, 0x00000FF0},
                     {0x00000FFA, 0x00000FFC, 0x00000FF0},
                     {0x00000FFB, 0x00000FFB, 0x00000FF0},
                     {0x00000FFC, 0x00000FF2, 0x00000FF0},
                     {0x00000FFD, 0x00000FF8, 0x00000FF0},
                     {0x00000FFE, 0x00000FF9, 0x00000FF8},
                    {0x00000FF5, 0x00000FF5, 0x00000FF0} }
     },

};

const rlAdvDynPerChirpPhShftCfg_t dynChirpPhaseCfg[32]=
{
    {
        .chirpSegSel=0,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=1,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=2,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=3,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=4,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },
    {
        .chirpSegSel=5,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=6,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=7,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=8,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=9,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=10,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=11,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=12,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=13,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=14,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=15,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=16,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=17,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=18,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=19,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=20,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=21,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=22,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=23,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=24,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=25,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=26,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=27,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=28,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=29,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=30,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },

    {
        .chirpSegSel=31,
        .programMode=0,
        .phShiftPerTx={ {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},
                        {0x4,0x4,0x4,0x4},}

    },
};

const rlRxSatMonConf_t rxSatMonCfg =
{
    .profileIndx   = 0U,
    .satMonSel     = 3U,
    .reserved0     = 0U,
    .primarySliceDuration = 5,
    .numSlices     = CQ_NUM_SLICES,
    .rxChannelMask = 0U,
    .reserved1     = 0U,
    .reserved2     = 0U,
    .reserved3     = 0U,
    .reserved4     = 0U
};

rlFrameCfg_t  dummyFrameCfg =
{
    .reserved0 = 0x0,
    .chirpStartIdx = 0,
    .chirpEndIdx = 4,
    .numLoops = 1,
    .numFrames = 1,
    .numAdcSamples = 256,
    .framePeriodicity = 40000000,
    .triggerSelect = 0x1U,
    .reserved1 = 0x0,
    .frameTriggerDelay = 0U,
};

rlFrameCfg_t  frameCfg =
{
    .reserved0 = 0x0,
    .chirpStartIdx = FRAME_CHIRP_START_IDX,
    .chirpEndIdx = FRAME_CHIRP_END_IDX,
    .numLoops = FRAME_LOOP_COUNT,
    .numFrames = FRAME_COUNT_VAL,
    .numAdcSamples = FRAME_ADC_SAMPLE_NUM,
    .framePeriodicity = FRAME_PRIODICITY_VAL,
    .triggerSelect = RL_FRAMESTRT_API_TRIGGER,
    .reserved1 = 0x0,
    .frameTriggerDelay = FRAME_TRIGGER_DELAY_VAL,
};


const rlRfCalMonTimeUntConf_t calMonTimeUnitConf =
{
    .calibMonTimeUnit = 1,
    .numOfCascadeDev = 1,
    .devId = 0,
    .reserved = 0
};

const rlRfCalMonFreqLimitConf_t freqLimit =
{
    .freqLimitLow = 760,
    .freqLimitHigh = 810,
    .reserved0 = 0,
    .reserved1 = 0
};

const rlRfInitCalConf_t rfInitCalib =
{
    .calibEnMask = 0x0FF0, /* Enable All Init time Calibration */
    .reserved0 = 0,
    .reserved1 = 0,
    .txPowCalCfg = 0,
    .reserved3 = 0
};

const rlRunTimeCalibConf_t runTimeCalib =
{
    .oneTimeCalibEnMask = 0x610,  /* Enable Run time Calibration as per choice */
    .periodicCalibEnMask = 0x610, /* Enable Run time Calibration as per choice*/
    .calibPeriodicity = 5,
    .reportEn = 1,
    .reserved0 = 0,
    .txPowerCalMode = 0,
    .CalTempIdxOverrideEn = 0,
    .CalTempIdxTx = 0,
    .CalTempIdxRx = 0,
    .CalTempIdxLodist = 0,
    .CalTempIdxTxPhase = 0

};

rlAdvFrameCfg_t advFrameCfg =
{
        .frameSeq.numOfSubFrames = 1,// Can go upto 4
        .frameSeq.forceProfile = 0,
        .frameSeq.loopBackCfg = 0,
        .frameSeq.subFrameTrigger = 0,

        .frameSeq.subFrameCfg[0].forceProfileIdx = 0,
        .frameSeq.subFrameCfg[0].chirpStartIdx = 0,
        .frameSeq.subFrameCfg[0].numOfChirps = 512,
        .frameSeq.subFrameCfg[0].numLoops = 1,
        .frameSeq.subFrameCfg[0].burstPeriodicity = 8000000,
        .frameSeq.subFrameCfg[0].chirpStartIdxOffset = 0,
        .frameSeq.subFrameCfg[0].numOfBurst = 6,
        .frameSeq.subFrameCfg[0].numOfBurstLoops = 1,
        .frameSeq.subFrameCfg[0].reserved0 = 0,
        .frameSeq.subFrameCfg[0].subFramePeriodicity = 9000000*6,
        .frameSeq.subFrameCfg[0].reserved1 = 0,
        .frameSeq.subFrameCfg[0].reserved2 = 0,
        .frameSeq.numFrames =10,//10
        .frameSeq.triggerSelect = 1,
        .frameSeq.frameTrigDelay = 0,
        .frameSeq.reserved0 = 0,
        .frameSeq.reserved1 = 0,

};

const rlBpmCommonCfg_t bpmCommonCfg =
{
    .mode.b2SrcSel = 0,
    .mode.b1Reserved0 = 0,
    .mode.b13Reserved1 = 0,
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
    .reserved4 = 0
};

const rlBpmChirpCfg_t bpmChirpCfg[3] =
{
    {
        .chirpStartIdx = CHIRP_START_INDEX,
        .chirpEndIdx = CHIRP_END_INDEX,
        .constBpmVal = 0,
        .reserved = 0x0
    },
    {
        .chirpStartIdx = CHIRP_START_INDEX+1,
        .chirpEndIdx = CHIRP_END_INDEX+1,
        .constBpmVal = 21,
        .reserved = 0x0
    },
    {
        .chirpStartIdx = CHIRP_START_INDEX+2,
        .chirpEndIdx = CHIRP_END_INDEX+2,
        .constBpmVal = 21,
        .reserved = 0x0
    }
};

const rlContModeCfg_t contModeCfg =
{
    .startFreqConst = 1435384036,
    .txOutPowerBackoffCode = 0,
    .txPhaseShifter = 0,
    .digOutSampleRate = 10000,
    .hpfCornerFreq1 = 0,
    .hpfCornerFreq2 = 0,
    .rxGain = 30,
    .vcoSelect = 0x0,
    .reserved0  = 0x0
};

const rlContModeEn_t contModeEn =
{
    .contModeEn = 1,
    .reserved   = 0
};

const rlDynPwrSave_t dynPwrSave =
{
    .blkCfg   = 0x7,
    .reserved = 0
};

const rlGpAdcCfg_t gpAdcCfg =
{
    .enable = 0x3F,
    .bufferEnable = 0x3F,
    .numOfSamples[0].sampleCnt = 20,
    .numOfSamples[0].settlingTime = 3,
    .numOfSamples[1].sampleCnt = 14,
    .numOfSamples[1].settlingTime = 3,
    .numOfSamples[2].sampleCnt = 14,
    .numOfSamples[2].settlingTime = 3,
    .numOfSamples[3].sampleCnt = 14,
    .numOfSamples[3].settlingTime = 3,
    .numOfSamples[4].sampleCnt = 14,
    .numOfSamples[4].settlingTime = 3,
    .numOfSamples[5].sampleCnt = 14,
    .numOfSamples[5].settlingTime = 3,
    .reserved0 = 0,
	.reserved1[0] = 0,
	.reserved1[1] = 0,
	.reserved1[2] = 0
};

const rlRfLdoBypassCfg_t rfLdoBypassCfg =
{
    .ldoBypassEnable   = 1,
    .supplyMonIrDrop   = 0,
    .ioSupplyIndicator = 0,
};

const rlTempMonConf_t tempMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .anaTempThreshMin = -10,
    .anaTempThreshMax = 60,
    .digTempThreshMin = -10,
    .digTempThreshMax = 60,
    .tempDiffThresh = 20,
    .reserved1 = 0,
    .reserved2 = 0,
};

const rlRxGainPhaseMonConf_t rxGainPhaseMonCfg =
{
    .profileIndx = 0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .txSel = 1,
    .rxGainAbsThresh = 100,
    .rxGainMismatchErrThresh = 100,
    .rxGainFlatnessErrThresh = 100,
    .rxGainPhaseMismatchErrThresh = (40 * (1U << 16))/360U,
    .rxGainMismatchOffsetVal[0][0] = 0x1A,
    .rxGainMismatchOffsetVal[0][1] = 0x1A,
    .rxGainMismatchOffsetVal[0][2] = 0x1A,
    .rxGainMismatchOffsetVal[0][3] = 0x1A,
    .rxGainMismatchOffsetVal[1][0] = 0x1A,
    .rxGainMismatchOffsetVal[1][1] = 0x1A,
    .rxGainMismatchOffsetVal[1][2] = 0x1A,
    .rxGainMismatchOffsetVal[1][3] = 0x1A,
    .rxGainMismatchOffsetVal[2][0] = 0x1A,
    .rxGainMismatchOffsetVal[2][1] = 0x1A,
    .rxGainMismatchOffsetVal[2][2] = 0x1A,
    .rxGainMismatchOffsetVal[2][3] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[0][0] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[0][1] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[0][2] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[0][3] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[1][0] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[1][1] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[1][2] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[1][3] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[2][0] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[2][1] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[2][2] = 0x1A,
    .rxGainPhaseMismatchOffsetVal[2][3] = 0x1A,
    .rf3FreqDitherLimits = 0,
    .reserved0 = 0
};

const rlRxNoiseMonConf_t rxNoiseMonCfg =
{
    .profileIndx = 0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .reserved0 = 0x0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved1 = 0x0,
    .noiseThresh = 350,
    .reserved2 = 0x0,
};

const rlRxIfStageMonConf_t monRxIfStageCfg =
{
    .profileIndx = 0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .reserved1 = 0,
    .hpfCutoffErrThresh = 60,
    .lpfCutoffBandEdgeDroopThresh = 128,
    .lpfCutoffStopBandAttenThresh = 128,
    .ifaGainErrThresh = 100,
    .reserved2 = 0,
};

const rlTxnPowMonConf_t allTxPowMonCfg =
{
    .profileIndx = 0x0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .txSelMask = 0xF,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .txPowAbsErrThresh = 30,
    .txPowFlatnessErrThresh = 30,
    .txPowOffsetValRF1 = 0x0,
    .txPowOffsetValRF2 = 0x0,
    .txPowOffsetValRF3 = 0x0,

};

const rlTxnBallbreakMonConf_t allTxBallbreakMonCfg =
{
  .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
  .txSelMask = 0xF,
  .reserved1 = 0,
  .reserved2 = 0,
  .monStartFreqConst = PROFILE_START_FREQ_VAL,
  .txPowBackOff = 0,
  .txReflCoeffMagThresh = -60,
};

const rlAdvTxGainPhaseMismatchMonConf_t AdvtxGainPhMisMatchMonCfg =
{
    .profileIndx = 0x0,
    .rfFreqBitMask = HIGHEST_CENTER_LOWEST_RF_FRQ_IN_PROFILES_SWEEP_BW,
    .txEn = 0x1,
    .rxEn = 0xF,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .monChirpSlope  = 0x1,
    .txGainMismatchThresh = 30,
    .txPhaseMismatchThresh = (25 * (1 << 16))/360 ,
    .txGainMismatchOffsetVal =  {{0,11,-75,-75},{-12,0,-88,-88},{-29,-23,-100,-100}},
    .txPhaseMismatchOffsetVal = {{0,1820,1274,1274},{50050,51688,52780},{34398,35490,39494}},
    .reserved1 = 0x0,
    .reserved0 = 0x0,
};

const rlSynthFreqMonConf_t synthFreqMonCfg =
{
    .profileIndx = 0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .freqErrThresh = 4000,
    .monStartTime = 10,
	.monitorMode = 0,
	.vcoMonEn = 0,
    .profileBitMaskLiveMode = 0,
    .reserved2 = 0
};

const rlSynthFreqMonConf_t synthFreqNonLiveMonCfg[2U] =
{
    {
        .profileIndx = 0,
        .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
        .freqErrThresh = 4000,
        .monStartTime = 10,
        .monitorMode = 1,
        .vcoMonEn = 3,
        .profileBitMaskLiveMode = 0,
        .reserved2 = 0
    },
    {
        .profileIndx = 0,
        .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
        .freqErrThresh = 4000,
        .monStartTime = 10,
        .monitorMode = 2,
        .vcoMonEn = 3,
        .profileBitMaskLiveMode = 0,
        .reserved2 = 0
    }
};

const rlExtAnaSignalsMonConf_t extAnaSigMonCfg =
{
    .reportMode = 2,
    .reserved0 = 0,
    .signalInpEnables = 0x3F,
    .signalBuffEnables = 0x1F,
    .signalSettlingTime[0] = 10,
    .signalSettlingTime[1] = 10,
    .signalSettlingTime[2] = 10,
    .signalSettlingTime[3] = 10,
    .signalSettlingTime[4] = 10,
    .signalSettlingTime[5] = 10,
    .signalThresh[0] = 0,
    .signalThresh[1] = 0,
    .signalThresh[2] = 0,
    .signalThresh[3] = 0,
    .signalThresh[4] = 0,
    .signalThresh[5] = 0,
    .signalThresh[6] = 200,
    .signalThresh[7] = 200,
    .signalThresh[8] = 200,
    .signalThresh[9] = 200,
    .signalThresh[10] = 200,
    .signalThresh[11] = 200,
    .reserved1 = 0,
    .reserved2 = 0,
    .reserved3 = 0,
};

const rlTxnIntAnaSignalsMonConf_t allTxanaSigMonCfg =
{
    .profileIndx = 0,
    .reportMode = 2,
    .reserved1 = 0,
    .reserved2 = 0,
    .txSelMask = 0xF,
    .txPhShiftDacMonThresh = 5,
};

const rlRxIntAnaSignalsMonConf_t rxIntAnaSigMonCfg =
{
    .profileIndx = 0,
    .reportMode = 2,
    .reserved0 = 0,
    .reserved1 = 0,
};

const rlPmClkLoIntAnaSignalsMonConf_t pmClkLoIntAnaSigMonCfg =
{
    .profileIndx = 0,
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .reserved1 = 0,
};

const rlGpadcIntAnaSignalsMonConf_t gpadcIntAnaSigMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .reserved1 = 0,
    .reserved2 = 0,
};

const rlPllContrVoltMonConf_t pllConVoltMonCfg =
{
    .reportMode = MON_REPORT_MODE_PERIODIC_WITH_THRESHOLD_CHECK,
    .reserved0 = 0,
    .signalEnables = 7,
    .reserved1 = 0,
};

const rlDualClkCompMonConf_t dualClkCompMonCfg =
{
    .reportMode = 0,
    .reserved0 = 0,
    .dccPairEnables = 63,
    .reserved1 = 0,
};

const rlRfProgFiltConf_t progFiltCfg =
{
    .profileId = 0,
    .coeffStartIdx = 0,
    .progFiltLen = 14,
    .progFiltFreqShift = 100,
};

const rlRfProgFiltCoeff_t progFiltCoef =
{
    .coeffArray[0] = -876,
    .coeffArray[1] = -272,
    .coeffArray[2] = 1826,
    .coeffArray[3] = -395,
    .coeffArray[4] = -3672,
    .coeffArray[5] = 3336,
    .coeffArray[6] = 15976,
    .coeffArray[7] = 15976,
    .coeffArray[8] = 3336,
    .coeffArray[9] = -3672,
    .coeffArray[10] = -395,
    .coeffArray[11] = 1826,
    .coeffArray[12] = -272,
    .coeffArray[13] = -876,
};

const rlSubFrameStartCfg_t subFrameTrigCfg =
{
    .startCmd = 1,
    .reserved = 0,
};

const rlRfMiscConf_t miscCfg =
{
    .miscCtl  = 1,
    .crdNSlopeMag = 0,
    .fastResetEndTime = 0
};

const rlRfPhaseShiftCfg_t phaseShiftCfg[2] =
{
    {
        .chirpStartIdx = CHIRP_START_INDEX,
        .chirpEndIdx   = CHIRP_END_INDEX + 255,
        .tx0PhaseShift = 42,
        .tx1PhaseShift = 62,
        .tx2PhaseShift = 15,
        .tx3PhaseShift = 0
    },
    {
        .chirpStartIdx = CHIRP_START_INDEX + 256,
        .chirpEndIdx   = CHIRP_END_INDEX + 511,
        .tx0PhaseShift = 63,
        .tx1PhaseShift = 50,
        .tx2PhaseShift = 25,
        .tx3PhaseShift = 0
    },
};

const rlDynPerChirpPhShftCfg_t dynPhaseShiftCfg[10] =
{
    {
        .reserved     = 0,
        .chirpSegSel  = 0,
        .phShiftPerTx = {{47, 1,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 49},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 48, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {51, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {50, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 1,
        .phShiftPerTx = {{0,  1,  2},  {3,  4,  56},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 54, 20}, {21, 53, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {57, 31, 32}, {33, 34, 35},
                         {52, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 2,
        .phShiftPerTx = {{0, 61,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 59}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {58, 28, 29},
                         {60, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 62, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 3,
        .phShiftPerTx = {{50, 1,  2},  {3,  4,  5},
                         {6,  7,  8},  {49, 10, 11},
                         {12, 13, 14}, {15, 16, 48},
                         {47, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 63, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 4,
        .phShiftPerTx = {{0, 51,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 52}, {53, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 5,
        .phShiftPerTx = {{0,  1,  2},  {3,  4,  55},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 54}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 6,
        .phShiftPerTx = {{0,  1,  2}, {3,  4,  5},
                         {6,  7,  8}, {9,  10, 11},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 7,
        .phShiftPerTx = {{0,  1,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 11},
                         {56, 13, 14}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {57, 58, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 8,
        .phShiftPerTx = {{47, 48, 49}, {50, 51, 52},
                         {53, 54, 55}, {56, 57, 58},
                         {59, 60, 61}, {62, 63, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
    {
        .reserved     = 0,
        .chirpSegSel  = 9,
        .phShiftPerTx = {{0,  1,  2},  {3,  4,  5},
                         {6,  7,  8},  {9,  10, 11},
                         {12, 13, 14}, {15, 16, 17},
                         {18, 19, 20}, {21, 22, 23},
                         {24, 25, 26}, {27, 28, 29},
                         {30, 31, 32}, {33, 34, 35},
                         {36, 37, 38}, {39, 40, 41},
                         {42, 42, 43}, {44, 45, 46}},
        .programMode  = 0
    },
};

const rlInterRxGainPhConf_t interRxGainPhCfg =
{
    .profileIndx     = 0,
    .digCompEn       = 0,
    .reserved1       = 1,
    .digRxGainComp[0]    = 100,
    .digRxGainComp[1]    = 20,
    .digRxGainComp[2]    = 75,
    .digRxGainComp[3]    = 105,
    .digRxPhShiftComp[0] = 25102,
    .digRxPhShiftComp[1] = 102,
    .digRxPhShiftComp[2] = 65535,
    .digRxPhShiftComp[3] = 55201,
    .digRxDelayComp[0]       = 0,
    .digRxDelayComp[1]       = 0,
    .digRxDelayComp[2]       = 0,
    .digRxDelayComp[3]       = 0,
    .reserved2[0]       =0,
    .reserved2[1]       =0,
    .reserved2[2]       =0,
    .reserved2[3]       =0,
    .digRxFreqShift[0] = 0,
    .digRxFreqShift[1] = 0,
    .digRxFreqShift[2] = 0,
    .digRxFreqShift[3] = 0,
    .digTxFreqShift[0] = 0,
    .digTxFreqShift[1] = 0,
    .digTxFreqShift[2] = 0,
    .digTxFreqShift[3] = 0,
    .reserved3[0]       = 0,
    .reserved3[1]       = 0,
    .reserved3[2]       = 0,
    .reserved3[3]       = 0
};

const rlTestSource_t testSourceCfg =
{
    .testObj[0]      = {102, 2, 5698, 5, 8, 0, 100, 1256, 2589, 225, 2489, 12256, 112},
    .testObj[1]      = {10, 5, 0, 5, 5, 5, 255, 0, 216, 2251, 341, 556, 5112},
    .rxAntPos[0]     = {15, 15},
    .rxAntPos[1]     = {15, 15},
    .rxAntPos[2]     = {15, 15},
    .rxAntPos[3]     = {15, 15},
    .reserved0       = 0,
    .reserved1       = 0,
};

const rlTestSourceEnable_t testSourceEn =
{
    .tsEnable  = 1,
    .reserved  = 0,
};

const rlRxGainTempLutReadReq_t rxGainTempLutReadCfg =
{
    .profileIndx   = 0,
    .reserved0     = 0,
    .reserved1     = 0,
};

const rlTxGainTempLutReadReq_t txGainTempLutReadCfg =
{
    .profileIndx   = 0,
    .reserved0     = 0,
    .reserved1     = 0,
};

const rlAdvTxGainTempLutReadReq_t txGainTempLutReadCfg0 =
{
    .profileIndx   = 0,
    .txIndex       = 0,
    .reserved0     = 0,
    .reserved1     = {0},
};

const rlAdvTxGainTempLutReadReq_t txGainTempLutReadCfg1 =
{
    .profileIndx   = 0,
    .txIndex       = 1,
    .reserved0     = 0,
    .reserved1     = {0},
};

const rlAdvTxGainTempLutReadReq_t txGainTempLutReadCfg2 =
{
    .profileIndx   = 0,
    .txIndex       = 2,
    .reserved0     = 0,
    .reserved1     = {0},
};

const rlAdvTxGainTempLutReadReq_t txGainTempLutReadCfg3 =
{
    .profileIndx   = 0,
    .txIndex       = 3,
    .reserved0     = 0,
    .reserved1     = {0},
};

