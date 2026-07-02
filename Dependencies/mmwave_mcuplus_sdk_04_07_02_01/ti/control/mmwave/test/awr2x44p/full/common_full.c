/*
 *   @file  common_full.c
 *
 *   @brief
 *      The file contains functions which are reused among the DSS and
 *      MSS code base
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2024 Texas Instruments, Inc.
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
#define DebugP_LOG_ENABLED 1

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <ti/control/mmwave/mmwave.h>

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/
#define TEST_MAX_PROFILE                  (2)
#define TEST_MAX_CHIRP                    (4)
#define TEST_MAX_BPM_CHIRP                (8)
#define TEST_NUM_ADC_SAMPLES              (256U)
#define TEST_NUM_RX_ANTENNAS              (4)
#define TEST_CHIRP_START_INDEX            (0)
#define TEST_CHIRP_END_INDEX              (0)
#define TEST_NUM_LOOPS                    (1)
#define TEST_NUM_FRAMES                   (0)

/* CHIRP tab settings */
/* CHIRP Sub-tab */
#define CHIRP_0_PROFILE_ID                (0U)
#define CHIRP_0_START_INDEX               (0U)
#define CHIRP_0_END_INDEX                 (0U)
#define CHIRP_0_START_FREQ_VAL            (0U)
#define CHIRP_0_FREQ_SLOPE_VAL            (0U)
#define CHIRP_0_IDLE_TIME_VAL             (0U)
#define CHIRP_0_ADC_START_TIME_VAL        (0U)

#define CHIRP_1_PROFILE_ID                (1U)
#define CHIRP_1_START_INDEX               (1U)
#define CHIRP_1_END_INDEX                 (1U)
#define CHIRP_1_START_FREQ_VAL            (0U)
#define CHIRP_1_FREQ_SLOPE_VAL            (0U)
#define CHIRP_1_IDLE_TIME_VAL             (0U)
#define CHIRP_1_ADC_START_TIME_VAL        (0U)
#define CHIRP_1_TX_CHANNEL                CHIRP_TX_1_CHANNEL_ENABLE

/* PROFILE Sub-tab */
#define PROFILE_0_PROFILE_ID              (0U)
#define PROFILE_0_HPFCORNER_FREQ1_VAL     RL_RX_HPF1_175_KHz
#define PROFILE_0_HPFCORNER_FREQ2_VAL     RL_RX_HPF2_350_KHz
#define PROFILE_0_RX_GAIN_VAL             (30U)
#define PROFILE_0_DIGOUT_SAMPLERATE_VAL   (10000U)
#define PROFILE_0_ADC_SAMPLE_VAL          (224U)
#define PROFILE_0_IDLE_TIME_VAL           (1500U)
#define PROFILE_0_RAMP_END_TIME_VAL       (3000U)
#define PROFILE_0_START_FREQ_VAL          (1454025386U)
#define PROFILE_0_TXOUT_POWER_BACKOFF     (0U)
#define PROFILE_0_TXPHASESHIFTER_VAL      (0U)
#define PROFILE_0_FREQ_SLOPE_VAL          (1657U)
#define PROFILE_0_TX_START_TIME_VAL       (100U)  // 1us
#define PROFILE_0_ADC_START_TIME_VAL      (600U)  // 6us

/* PROFILE Sub-tab */
#define PROFILE_1_PROFILE_ID              (1U)
#define PROFILE_1_HPFCORNER_FREQ1_VAL     RL_RX_HPF1_175_KHz
#define PROFILE_1_HPFCORNER_FREQ2_VAL     RL_RX_HPF2_350_KHz
#define PROFILE_1_RX_GAIN_VAL             (30U)
#define PROFILE_1_DIGOUT_SAMPLERATE_VAL   (10000U)
#define PROFILE_1_ADC_SAMPLE_VAL          (512U)
#define PROFILE_1_IDLE_TIME_VAL           (1500U)
#define PROFILE_1_RAMP_END_TIME_VAL       (6000U)
#define PROFILE_1_START_FREQ_VAL          (1454025386U)  // 78Ghz
#define PROFILE_1_TXOUT_POWER_BACKOFF     (0U)
#define PROFILE_1_TXPHASESHIFTER_VAL      (0U)
#define PROFILE_1_FREQ_SLOPE_VAL          (828U)
#define PROFILE_1_TX_START_TIME_VAL       (100U)  // 1us
#define PROFILE_1_ADC_START_TIME_VAL      (600U)  // 6us

/* FRAME Sub-tab */
#define FRAME_CHIRP_START_IDX           (0U)
#define FRAME_CHIRP_END_IDX             (0U)
#define FRAME_COUNT_VAL                 (0)
#define FRAME_LOOP_COUNT                (64)
#define FRAME_PRIODICITY_VAL            (16000000)
#define FRAME_TRIGGER_DELAY_VAL         (0U)
#define FRAME_ADC_SAMPLE_NUM            (512U)


/* CHIRP Tx Channel Settings */
#define CHIRP_TX_1_CHANNEL_ENABLE           (1U << 0U)
#define CHIRP_TX_2_CHANNEL_ENABLE           (1U << 1U)
#define CHIRP_TX_3_CHANNEL_ENABLE           (1U << 2U)
#define CHIRP_TX_1_2_CHANNEL_ENABLE         (CHIRP_TX_1_CHANNEL_ENABLE | CHIRP_TX_2_CHANNEL_ENABLE)
#define CHIRP_TX_1_3_CHANNEL_ENABLE         (CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)
#define CHIRP_TX_2_3_CHANNEL_ENABLE         (CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)
#define CHIRP_TX_1_2_3_CHANNEL_ENABLE       (CHIRP_TX_1_CHANNEL_ENABLE | CHIRP_TX_2_CHANNEL_ENABLE | CHIRP_TX_3_CHANNEL_ENABLE)

/**************************************************************************
 ************************* Extern Declarations ****************************
 **************************************************************************/
extern MMWave_Handle    gMMWaveHandle;

/**************************************************************************
 ************************* Common Test Functions **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      Utility function which populates the profile configuration with
 *      well defined defaults.
 *
 *  @param[out]  ptrProfileCfg
 *      Pointer to the populated profile configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultProfileCfg (rlProfileCfg_t* ptrProfileCfg)
{
    /* Initialize the profile configuration: */
    memset ((void*)ptrProfileCfg, 0, sizeof(rlProfileCfg_t));

    /* Populate the *default* profile configuration: */
    ptrProfileCfg->profileId             = 0;
    ptrProfileCfg->startFreqConst        = (uint32_t) ((float)77 * (1U << 26) / 3.6);
    ptrProfileCfg->idleTimeConst         = 100 * 1000 / 10;
    ptrProfileCfg->adcStartTimeConst     = 6 * 1000 / 10;
    ptrProfileCfg->rampEndTime           = 60 * 1000 /10;
    ptrProfileCfg->txOutPowerBackoffCode = 0;
    ptrProfileCfg->txPhaseShifter        = 0;
    ptrProfileCfg->freqSlopeConst        = 30 * 1000 / 48;
    ptrProfileCfg->txStartTime           = 0 * 1000 / 10;
    ptrProfileCfg->numAdcSamples         = TEST_NUM_ADC_SAMPLES;
    ptrProfileCfg->digOutSampleRate      = 10000;
    ptrProfileCfg->hpfCornerFreq1        = 0;
    ptrProfileCfg->hpfCornerFreq2        = 0;
    ptrProfileCfg->rxGain                = 30;
}

/**
 *  @b Description
 *  @n
 *      Utility function which populates the chirp configuration with
 *      well defined defaults.
 *
 *  @param[out]  ptrChirpCfg
 *      Pointer to the populated chirp configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultChirpCfg (rlChirpCfg_t* ptrChirpCfg)
{
    /* Initialize the chirp configuration: */
    memset ((void*)ptrChirpCfg, 0, sizeof(rlChirpCfg_t));

    /* Populate the chirp configuration: */
    ptrChirpCfg->chirpStartIdx   = 0;
    ptrChirpCfg->chirpEndIdx     = 0;
    ptrChirpCfg->profileId       = 0;
    ptrChirpCfg->startFreqVar    = 0;
    ptrChirpCfg->freqSlopeVar    = 0;
    ptrChirpCfg->idleTimeVar     = 0;
    ptrChirpCfg->adcStartTimeVar = 0;
    ptrChirpCfg->txEnable        = 1;
}

/**
 *  @b Description
 *  @n
 *      Utility function which populates the BPM chirp configuration with
 *      well defined defaults.
 *
 *  @param[out]  ptrBpmChirpCfg
 *      Pointer to the populated BPM chirp configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultBpmChirpCfg (rlBpmChirpCfg_t* ptrBpmChirpCfg)
{
    /* Initialize the configuration: */
    memset ((void*)ptrBpmChirpCfg, 0, sizeof(rlBpmChirpCfg_t));

    ptrBpmChirpCfg->chirpStartIdx   = 0;
    ptrBpmChirpCfg->chirpEndIdx     = 1;
    ptrBpmChirpCfg->constBpmVal     = 2;
}

/**
 *  @b Description
 *  @n
 *      Utility function which populates the default advanced frame configuration
 *
 *  @param[out]  ptrAdvFrameCfg
 *      Pointer to the populated advanced frame configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultAdvancedFrameCfg (rlAdvFrameCfg_t* ptrAdvFrameCfg)
{
    uint8_t numOfSubFrame = 0U;

    /* Initialize the advanced frame configuration: */
    memset ((void*)ptrAdvFrameCfg, 0, sizeof(rlAdvFrameCfg_t));

    ptrAdvFrameCfg->frameSeq.forceProfile  = 0; // 1: force Profile,  0: Don't force profile
    ptrAdvFrameCfg->frameSeq.numFrames     = 0; // infinite
    ptrAdvFrameCfg->frameSeq.triggerSelect = 1; // SW Trigger
    ptrAdvFrameCfg->frameSeq.frameTrigDelay= 0;

    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].forceProfileIdx    = 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numLoops           = 64;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfBurst         = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfBurstLoops    = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].chirpStartIdxOffset= 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].numOfChirps        = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].chirpStartIdx      = 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].subFramePeriodicity= 20200000; // 101 msec
    ptrAdvFrameCfg->frameSeq.subFrameCfg[0].burstPeriodicity   = 20000000; // 100 msec

    ptrAdvFrameCfg->frameData.subframeDataCfg[0].numAdcSamples = PROFILE_0_ADC_SAMPLE_VAL*2;
    ptrAdvFrameCfg->frameData.subframeDataCfg[0].totalChirps   = 64;
    ptrAdvFrameCfg->frameData.subframeDataCfg[0].numChirpsInDataPacket = 1;
    numOfSubFrame++;

    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].forceProfileIdx    = 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].numLoops           = 32;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].numOfBurst         = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].numOfBurstLoops    = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].chirpStartIdxOffset= 0;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].numOfChirps        = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].chirpStartIdx      = 1;
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].subFramePeriodicity= 20200000; //101 msec
    ptrAdvFrameCfg->frameSeq.subFrameCfg[1].burstPeriodicity   = 20000000; // 100 msec

    ptrAdvFrameCfg->frameData.subframeDataCfg[1].numAdcSamples = PROFILE_1_ADC_SAMPLE_VAL*2;
    ptrAdvFrameCfg->frameData.subframeDataCfg[1].totalChirps   = 32;
    ptrAdvFrameCfg->frameData.subframeDataCfg[1].numChirpsInDataPacket = 1;
    numOfSubFrame++;

    ptrAdvFrameCfg->frameSeq.numOfSubFrames = numOfSubFrame;
    ptrAdvFrameCfg->frameData.numSubFrames  = numOfSubFrame;

}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default open configuration.
 *
 *  @param[out]  ptrOpenCfg
 *      Pointer to the open configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultOpenCfg (MMWave_OpenCfg* ptrOpenCfg)
{
    /* Initialize the open configuration: */
    memset ((void*)ptrOpenCfg, 0, sizeof(MMWave_OpenCfg));

    /* Setup the frequency for calibrations. */
    ptrOpenCfg->freqLimitLow  = 760U;
    ptrOpenCfg->freqLimitHigh = 810U;

    /* Enable start/stop async events */
    ptrOpenCfg->disableFrameStartAsyncEvent = false;
    ptrOpenCfg->disableFrameStopAsyncEvent  = false;

    /* Initialize the channel configuration: */
    ptrOpenCfg->chCfg.rxChannelEn = (1U << TEST_NUM_RX_ANTENNAS) - 1U;
    ptrOpenCfg->chCfg.txChannelEn = 0x1;
    ptrOpenCfg->chCfg.cascading   = 0;
    ptrOpenCfg->chCfg.cascadingPinoutCfg   = (1U << 5U); /* Disbale OSC_CLKOUT */

    /* Initialize the low power mode configuration: */
    ptrOpenCfg->lowPowerMode.lpAdcMode     = 0; /* regular ADC mode */

    /* Initialize the ADCOut configuration: */
    ptrOpenCfg->adcOutCfg.fmt.b2AdcBits   = 2; /* 16 bit */
    ptrOpenCfg->adcOutCfg.fmt.b2AdcOutFmt = 0; /* real */

    /* No custom calibration: */
    ptrOpenCfg->useCustomCalibration        = false;
    ptrOpenCfg->customCalibrationEnableMask = 0x0;

    /* calibration monitoring base time unit
     * setting it to one frame duration as the test doesnt support any
     * monitoring related functionality
     */
    ptrOpenCfg->calibMonTimeUnit            = 1;
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default control configuration
 *      in chirp configuration mode
 *
 *  @param[out]  ptrCtrlCfg
 *      Pointer to the control configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultChirpControlCfg (MMWave_CtrlCfg* ptrCtrlCfg)
{
    rlProfileCfg_t      profileCfg;
    rlChirpCfg_t        chirpCfg;
    int32_t             errCode;
    MMWave_ChirpHandle  chirpHandle;

    /* Initialize the control configuration: */
    memset ((void*)ptrCtrlCfg, 0, sizeof(MMWave_CtrlCfg));

    /* This is frame mode configuration */
    ptrCtrlCfg->dfeDataOutputMode = MMWave_DFEDataOutputMode_FRAME;

    /* Populate the profile configuration: */
    Mmwave_populateDefaultProfileCfg (&profileCfg);

    /* Create the profile: */
    ptrCtrlCfg->u.frameCfg[0].profileHandle[0] = MMWave_addProfile (gMMWaveHandle, &profileCfg, &errCode);
    if (ptrCtrlCfg->u.frameCfg[0].profileHandle[0] == NULL)
    {
        test_print ("Error: Unable to add the profile [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add Profile: Failed\n");
        return;
    }
    DebugP_log ("MMWave Add Profile: Passed\n");

    /**************************************************************************************************
     * Unit Test: Verify the Full Configuration Profile API
     **************************************************************************************************/
    {
        rlProfileCfg_t          profileCfgTmp;
        uint32_t                numProfiles;
        MMWave_ProfileHandle    tmpProfileHandle;

        /* Verify the number of profiles */
        if (MMWave_getNumProfiles (gMMWaveHandle, &numProfiles, &errCode) < 0)
        {
            test_print ("Error: Unable to get the number of profiles [Error code %d]\n", errCode);
            DebugP_log ("MMWave Get Number Profile: Failed\n");
            return;
        }
        if (numProfiles != 1U)
        {
            test_print ("Error: Invalid number of profiles detected [%d]\n", numProfiles);
            DebugP_log ("MMWave Get Number Profile: Failed\n");
        }
        DebugP_log ("MMWave Get Number Profile: Passed\n");

        /* Get the profile handle: */
        if (MMWave_getProfileHandle (gMMWaveHandle, 0U, &tmpProfileHandle, &errCode) < 0)
        {
            test_print ("Error: Unable to get the profile handle [Error code %d]\n", errCode);
            DebugP_log ("MMWave Get Profile Handle: Failed\n");
        }
        if (tmpProfileHandle != ptrCtrlCfg->u.frameCfg[0].profileHandle[0])
        {
            test_print ("Error: Invalid profile handle detected\n");
            DebugP_log ("MMWave Get Profile Handle: Failed\n");
        }
        DebugP_log ("MMWave Get Profile Handle: Passed\n");

        /* Get the profile configuration */
        if (MMWave_getProfileCfg (ptrCtrlCfg->u.frameCfg[0].profileHandle[0], &profileCfgTmp, &errCode) < 0)
        {
            test_print ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            DebugP_log ("MMWave Get Profile: Failed\n");
            return;
        }
        if (memcmp ((void*)&profileCfg, (void*)&profileCfgTmp, sizeof(rlProfileCfg_t)) != 0)
        {
            test_print ("Error: Invalid profile configuration detected\n");
            DebugP_log ("MMWave Get Profile: Failed\n");
            return;
        }
        DebugP_log ("MMWave Get Profile: Passed\n");
    }

    /* Populate the default chirp configuration */
    Mmwave_populateDefaultChirpCfg (&chirpCfg);

    /* Add the chirp to the profile: */
    chirpHandle = MMWave_addChirp (ptrCtrlCfg->u.frameCfg[0].profileHandle[0], &chirpCfg, &errCode);
    if (chirpHandle == NULL)
    {
        test_print ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add Chirp: Failed\n");
        return;
    }
    DebugP_log ("MMWave Add Chirp: Passed\n");

    /**************************************************************************************************
     * Unit Test: Verify the Full Configuration Chirp API
     **************************************************************************************************/
    {
        rlChirpCfg_t        chirpCfgTmp;
        uint32_t            numChirps;
        MMWave_ChirpHandle  chirpHandleTmp;

        /* Get the number of chirps attached to the profile */
        if (MMWave_getNumChirps (ptrCtrlCfg->u.frameCfg[0].profileHandle[0], &numChirps, &errCode) < 0)
        {
            test_print ("Error: Unable to get the number of chirps [Error code %d]\n", errCode);
            DebugP_log ("MMWave Get Number of Chirps: Failed\n");
            return;
        }
        if (numChirps != 1U)
        {
            test_print ("Error: Invalid number of chirps detected [%d]\n", numChirps);
            DebugP_log ("MMWave Get Number of Chirps: Failed\n");
            return;
        }
        DebugP_log ("MMWave Get Number of Chirps: Passed\n");

        /* Get the Chirp Handle */
        if (MMWave_getChirpHandle (ptrCtrlCfg->u.frameCfg[0].profileHandle[0], 1U, &chirpHandleTmp, &errCode) < 0)
        {
            test_print ("Error: Unable to get the chirp handle [Error code %d]\n", errCode);
            DebugP_log ("MMWave Get Chirp Handle: Failed\n");
            return;
        }
        if (chirpHandleTmp != chirpHandle)
        {
            test_print ("Error: Chirp handle validation failed [Error code %d]\n", errCode);
            DebugP_log ("MMWave Get Chirp Handle: Failed\n");
            return;
        }
        DebugP_log ("MMWave Get Chirp Handle: Passed\n");

        /* Get the chirp configuration */
        if (MMWave_getChirpCfg (chirpHandle, &chirpCfgTmp, &errCode) < 0)
        {
            test_print ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            DebugP_log ("MMWave Get Chirp: Failed\n");
            return;
        }
        if (memcmp ((void*)&chirpCfg, (void*)&chirpCfgTmp, sizeof(rlChirpCfg_t)) != 0)
        {
            test_print ("Error: Invalid chirp configuration detected\n");
            DebugP_log ("MMWave Get Chirp Configuration: Failed\n");
            return;
        }
        DebugP_log ("MMWave Get Chirp Configuration: Passed\n");
    }

    /* Populate the frame configuration: */
    ptrCtrlCfg->u.frameCfg[0].frameCfg.chirpStartIdx      = TEST_CHIRP_START_INDEX;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.chirpEndIdx        = TEST_CHIRP_END_INDEX;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.numLoops           = TEST_NUM_LOOPS;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.numFrames          = TEST_NUM_FRAMES;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.numAdcSamples      = TEST_NUM_ADC_SAMPLES;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.framePeriodicity   = 10 * 1000000 / 5;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.triggerSelect      = 1;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.frameTriggerDelay  = 0;

    return;
}


/**
 *  @b Description
 *  @n
 *      The function initializes the profile configuration with the default
 *      parameters.
 *
 *  @param[in] profileNum
 *      Profile number to be initialized
 *  @param[out] ptrProfileCfg
 *      Pointer to the profile configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultAdvancedFrameProfileCfg (uint8_t profileNum, rlProfileCfg_t* ptrProfileCfg)
{
    /* Initialize the configuration: */
    memset ((void*)ptrProfileCfg, 0, sizeof(rlProfileCfg_t));

    if (profileNum == 0U)
    {
        /* Populate the default configuration for profile0  */
        ptrProfileCfg->profileId             = PROFILE_0_PROFILE_ID;
        ptrProfileCfg->startFreqConst        = PROFILE_0_START_FREQ_VAL;
        ptrProfileCfg->idleTimeConst         = PROFILE_0_IDLE_TIME_VAL;
        ptrProfileCfg->adcStartTimeConst     = PROFILE_0_ADC_START_TIME_VAL;
        ptrProfileCfg->rampEndTime           = PROFILE_0_RAMP_END_TIME_VAL;
        ptrProfileCfg->txOutPowerBackoffCode = PROFILE_0_TXOUT_POWER_BACKOFF;
        ptrProfileCfg->txPhaseShifter        = PROFILE_0_TXPHASESHIFTER_VAL;
        ptrProfileCfg->freqSlopeConst        = PROFILE_0_FREQ_SLOPE_VAL;
        ptrProfileCfg->txStartTime           = PROFILE_0_TX_START_TIME_VAL;
        ptrProfileCfg->numAdcSamples         = PROFILE_0_ADC_SAMPLE_VAL;
        ptrProfileCfg->digOutSampleRate      = PROFILE_0_DIGOUT_SAMPLERATE_VAL;
        ptrProfileCfg->hpfCornerFreq1        = PROFILE_0_HPFCORNER_FREQ1_VAL;
        ptrProfileCfg->hpfCornerFreq2        = PROFILE_0_HPFCORNER_FREQ2_VAL;
        ptrProfileCfg->rxGain                = PROFILE_0_RX_GAIN_VAL;
    }
    else
    {
        /* Populate the default configuration for profile0  */
        ptrProfileCfg->profileId             = PROFILE_1_PROFILE_ID;
        ptrProfileCfg->startFreqConst        = PROFILE_1_START_FREQ_VAL;
        ptrProfileCfg->idleTimeConst         = PROFILE_1_IDLE_TIME_VAL;
        ptrProfileCfg->adcStartTimeConst     = PROFILE_1_ADC_START_TIME_VAL;
        ptrProfileCfg->rampEndTime           = PROFILE_1_RAMP_END_TIME_VAL;
        ptrProfileCfg->txOutPowerBackoffCode = PROFILE_1_TXOUT_POWER_BACKOFF;
        ptrProfileCfg->txPhaseShifter        = PROFILE_1_TXPHASESHIFTER_VAL;
        ptrProfileCfg->freqSlopeConst        = PROFILE_1_FREQ_SLOPE_VAL;
        ptrProfileCfg->txStartTime           = PROFILE_1_TX_START_TIME_VAL;
        ptrProfileCfg->numAdcSamples         = PROFILE_1_ADC_SAMPLE_VAL;
        ptrProfileCfg->digOutSampleRate      = PROFILE_1_DIGOUT_SAMPLERATE_VAL;
        ptrProfileCfg->hpfCornerFreq1        = PROFILE_1_HPFCORNER_FREQ1_VAL;
        ptrProfileCfg->hpfCornerFreq2        = PROFILE_1_HPFCORNER_FREQ2_VAL;
        ptrProfileCfg->rxGain                = PROFILE_1_RX_GAIN_VAL;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function initializes the chirp configuration with the default
 *      parameters.
 *
 *  @param[out] chirpNum
 *      Chirp Number to be configured
 *  @param[out] ptrChirpCfg
 *      Pointer to the chirp configuration
 *
 *  @retval
 *      Not applicable
 */
static void Mmwave_populateDefaultAdvancedFrameChirpCfg (uint8_t chirpNum, rlChirpCfg_t* ptrChirpCfg)
{
    /* Initialize the configuration: */
    memset ((void*)ptrChirpCfg, 0, sizeof(rlChirpCfg_t));

    if (chirpNum == 0U)
    {
        /* Populate the default configuration for chirp 0 */
        ptrChirpCfg->profileId       = CHIRP_0_PROFILE_ID;
        ptrChirpCfg->adcStartTimeVar = CHIRP_0_ADC_START_TIME_VAL;
        ptrChirpCfg->chirpEndIdx     = CHIRP_0_START_INDEX;
        ptrChirpCfg->chirpStartIdx   = CHIRP_0_END_INDEX;
        ptrChirpCfg->idleTimeVar     = CHIRP_0_IDLE_TIME_VAL;
        ptrChirpCfg->txEnable        = CHIRP_1_TX_CHANNEL;
        ptrChirpCfg->startFreqVar    = CHIRP_0_START_FREQ_VAL;
        ptrChirpCfg->freqSlopeVar    = CHIRP_0_FREQ_SLOPE_VAL;
    }
    else
    {
        /* Populate the default configuration for chirp 1 */
        ptrChirpCfg->profileId       = CHIRP_1_PROFILE_ID;
        ptrChirpCfg->adcStartTimeVar = CHIRP_1_ADC_START_TIME_VAL;
        ptrChirpCfg->chirpEndIdx     = CHIRP_1_START_INDEX;
        ptrChirpCfg->chirpStartIdx   = CHIRP_1_END_INDEX;
        ptrChirpCfg->idleTimeVar     = CHIRP_1_IDLE_TIME_VAL;
        ptrChirpCfg->txEnable        = CHIRP_1_TX_CHANNEL;
        ptrChirpCfg->startFreqVar    = CHIRP_1_START_FREQ_VAL;
        ptrChirpCfg->freqSlopeVar    = CHIRP_1_FREQ_SLOPE_VAL;
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      The function is used to populate the default control configuration
 *      in chirp configuration mode
 *
 *  @param[out]  ptrCtrlCfg
 *      Pointer to the control configuration
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultAdvancedControlCfg (MMWave_CtrlCfg* ptrCtrlCfg)
{
    rlProfileCfg_t         profileCfg;
    rlChirpCfg_t           chirpCfg;
    rlBpmChirpCfg_t        bpmChirpCfg;
    int32_t                errCode;
    MMWave_ChirpHandle     chirpHandle;
    MMWave_BpmChirpHandle  bpmChirpHandle;

    /* Initialize the control configuration: */
    memset ((void*)ptrCtrlCfg, 0, sizeof(MMWave_CtrlCfg));

    /* This is frame mode configuration */
    ptrCtrlCfg->dfeDataOutputMode = MMWave_DFEDataOutputMode_ADVANCED_FRAME;

    /* Populate the profile configuration: */
    Mmwave_populateDefaultAdvancedFrameProfileCfg (0U, &profileCfg);

    /* Create the profile: */
    ptrCtrlCfg->u.advancedFrameCfg[0].profileHandle[0] = MMWave_addProfile (gMMWaveHandle, &profileCfg, &errCode);
    if (ptrCtrlCfg->u.advancedFrameCfg[0].profileHandle[0] == NULL)
    {
        test_print ("Error: Unable to add the profile [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add Profile: Failed\n");
        return;
    }
    DebugP_log ("MMWave Add Profile: Passed\n");

    /* Populate the profile configuration: */
    Mmwave_populateDefaultAdvancedFrameProfileCfg (1U, &profileCfg);

    /* Create the profile: */
    ptrCtrlCfg->u.advancedFrameCfg[0].profileHandle[1] = MMWave_addProfile (gMMWaveHandle, &profileCfg, &errCode);
    if (ptrCtrlCfg->u.advancedFrameCfg[0].profileHandle[1] == NULL)
    {
        test_print ("Error: Unable to add the profile [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add Profile: Failed\n");
        return;
    }
    DebugP_log ("MMWave Add Profile: Passed\n");

    /* Populate the default chirp configuration */
    Mmwave_populateDefaultAdvancedFrameChirpCfg (0U, &chirpCfg);

    /* Add the chirp to the profile: */
    chirpHandle = MMWave_addChirp (ptrCtrlCfg->u.advancedFrameCfg[0].profileHandle[0], &chirpCfg, &errCode);
    if (chirpHandle == NULL)
    {
        test_print ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add Chirp: Failed\n");
        return;
    }
    DebugP_log ("MMWave Add Chirp: Passed\n");

    /* Populate the default chirp configuration */
    Mmwave_populateDefaultAdvancedFrameChirpCfg (1U, &chirpCfg);

    /* Add the chirp to the profile: */
    chirpHandle = MMWave_addChirp (ptrCtrlCfg->u.advancedFrameCfg[0].profileHandle[1], &chirpCfg, &errCode);
    if (chirpHandle == NULL)
    {
        test_print ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add Chirp: Failed\n");
        return;
    }
    DebugP_log ("MMWave Add Chirp: Passed\n");

    /* Populate the BPM configuration. */
    Mmwave_populateDefaultBpmChirpCfg (&bpmChirpCfg);
    bpmChirpHandle = MMWave_addBpmChirp (gMMWaveHandle, &bpmChirpCfg, &errCode);
    if (bpmChirpHandle == NULL)
    {
        test_print ("Error: Unable to add BPM chirp [Error code %d]\n", errCode);
        DebugP_log ("MMWave Add BPM Chirp: Failed\n");
        return;
    }
    DebugP_log ("MMWave Add BPM Chirp: Passed\n");

    /* Populate the advanced frame configuration: */
    Mmwave_populateDefaultAdvancedFrameCfg(&ptrCtrlCfg->u.advancedFrameCfg[0].frameCfg);
    return;
}


/**
 *  @b Description
 *  @n
 *      Registered event function which is invoked when an event from the
 *      BSS is received.
 *
 *  @param[in]  msgId
 *      Message Identifier
 *  @param[in]  sbId
 *      Subblock identifier
 *  @param[in]  sbLen
 *      Length of the subblock
 *  @param[in]  payload
 *      Pointer to the payload buffer
 *
 *  @retval
 *      Always return 0 to pass the event to the peer domain- there is no more peer domain, this can be void
 */
int32_t Mmwave_eventFxn(uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload)
{
    uint16_t asyncSB = RL_GET_SBID_FROM_UNIQ_SBID(sbId);

    /* Process the received message: */
    switch (msgId)
    {
        case RL_RF_ASYNC_EVENT_MSG:
        {
            /* Received Asychronous Message: */
            switch (asyncSB)
            {
                case RL_RF_AE_INITCALIBSTATUS_SB:
                {
                    rlRfInitComplete_t*  ptrRFInitCompleteMessage;
                    uint32_t            calibrationStatus;

                    /* Get the RF-Init completion message: */
                    ptrRFInitCompleteMessage = (rlRfInitComplete_t*)payload;
                    calibrationStatus = ptrRFInitCompleteMessage->calibStatus & 0x1FFFU;

                    /* Display the calibration status: */

                    test_print("Debug: Init Calibration Status = 0x%x\n", calibrationStatus);;
                    break;
                }
            }
        }
    }
    return 0;
}



/**
 *  @b Description
 *  @n
 *      The function is used to populate the default calibration
 *      configuration which is passed to start the mmWave module
 *
 *  @retval
 *      Not applicable
 */
void Mmwave_populateDefaultCalibrationCfg (MMWave_CalibrationCfg* ptrCalibrationCfg, MMWave_DFEDataOutputMode dfeOutputMode)
{
    /* Populate the calibration configuration: */
    ptrCalibrationCfg->dfeDataOutputMode                          = dfeOutputMode;
    ptrCalibrationCfg->u.chirpCalibrationCfg.enableCalibration    = true;
    ptrCalibrationCfg->u.chirpCalibrationCfg.enablePeriodicity    = true;
    ptrCalibrationCfg->u.chirpCalibrationCfg.periodicTimeInFrames = 10U;
    ptrCalibrationCfg->u.chirpCalibrationCfg.reportEn             = 1;
    return;
}


/**
 *  @b Description
 *  @n
 *      The task is used to provide an execution context for the mmWave
 *      control task
 *
 *  @retval
 *      Not Applicable.
 */
void Mmwave_ctrlTask(void* arg0, void* arg1)
{
    int32_t errCode;

    while (1)
    {
        /* Execute the mmWave control module: */
        if (MMWave_execute (gMMWaveHandle, &errCode) < 0)
            test_print ("Error: mmWave control execution failed [Error code %d]\n", errCode);
    }
}
