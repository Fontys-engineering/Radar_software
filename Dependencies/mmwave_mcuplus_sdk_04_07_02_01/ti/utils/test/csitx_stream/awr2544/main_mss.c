/*
 * Copyright (C) 2025 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 /** 
 *   @file  main_mss.c
 *
 *   @brief
 *      CSI-TX STREAM - TEST application code
 */

 /** @mainpage MSS CSITX Stream Application for AWR2544
 *
 * ## Introduction #
 *
 * CSI-TX Stream: This application can be executed on MSS only.
 *  The CSI-TX Stream application on AWR2544 is responsible for following:
 *   - Setup the control path through MMWave module.
 *   - Setup data path to stream ADC data over from CBUFF using CSI-TX interface.
 *  
 *  The application initializes the modules for the control & datapath. This 
 *  would imply the configurations of the following modules:-
 *   1. Control path - Communication with BSS over mailbox to configure the
 *      sensor with hardcoded configuration to open mmWave control module.
 *   2. Data Path - Configuration of ADCBUF and CBUFF  module to stream 
 *      ADC data received from BSS on to CSI-TX interface.
 *
 * ## Configuration #
 *
 * The application uses hardcoded configuration for the chirp, profile and frame. 
 * This configuration is then passed to the BSS via the mmWave control module.
 * 
 * Below are the configuration details for control path:
 * 
 *  \ref TESTSOURCE_NUM_RX_ANTENNAS
 * 
 *  \ref TESTSOURCE_NUM_ADC_SAMPLES
 * 
 *  \ref TEST_SOURCE_NUM_LOOPS
 * 
 *  \ref TESTSOURCE_CHIRP_START_INDEX
 * 
 *  \ref TESTSOURCE_CHIRP_END_INDEX
 * 
 *  \ref TESTSOURCE_NUM_FRAMES
 *
 *  @note 
 * 1.  Enable CSI-TX interface for Cbuff driver in Sysconfig, and add required
 *       configurations for CSI-TX driver in the Sysconfig to use Cbuff with CSI-TX.
 * 2.  CSI-TX will not work on AWR2544 EVM board by default as LVDS is enabled.
 *       HW changes are required on AWR2544 EVM board to enable CSI-TX. Refer EVM Schematic
 *       and EVM Userguide for more details on the HW changes need to be done.
 */
/* ============================================================================
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

/* MCU+SDK Include Files: */
#include <ti/utils/test/csitx_stream/awr2544/mssgenerated/ti_drivers_config.h>
#include <ti/utils/test/csitx_stream/awr2544/mssgenerated/ti_board_config.h>
#include <ti/utils/test/csitx_stream/awr2544/mssgenerated/ti_drivers_open_close.h>
#include <ti/utils/test/csitx_stream/awr2544/mssgenerated/ti_board_open_close.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmWave SDK Include Files: */
#include <ti/control/mmwave/mmwave.h>
#include <ti/common/syscommon.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/utils/test/csitx_stream/csitx_stream.h>

/** @addtogroup TEST_CONTROL_PATH_CONFIG_DEFINITIONS
 @{ */

 /**
 * @brief
 * This macro defines the number of receive antennas.
 */
#define TESTSOURCE_NUM_RX_ANTENNAS         (4U)

 /**
 * @brief
 * This macro defines the number of ADC Samples.
 */
#define TESTSOURCE_NUM_ADC_SAMPLES         (256U)

 /**
 * @brief
 * This macro defines the number of chirps.
 */
#define TEST_SOURCE_NUM_LOOPS              (1U)

 /**
 * @brief
 * This macro defines the chirp start index.
 */
#define TESTSOURCE_CHIRP_START_INDEX       (0U)

 /**
 * @brief
 * This macro defines the chirp end index.
 */
#define TESTSOURCE_CHIRP_END_INDEX         (0U)

 /**
 * @brief
 * This macro defines the number of frame.
 */
#define TESTSOURCE_NUM_FRAMES              (2U)

/**
@}
*/

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (5U)
#define APP_CTRL_TASK_PRI    (7U)
#define APP_TASK_STACK_SIZE  (8*1024U)
#define APP_CTRL_TASK_STACK_SIZE (6*1024U)

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/
TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

TaskHandle_t    gMmwCtrlTask;
StaticTask_t    gMmwCtrlTaskObj;

 /**
 * @brief
 * CSI-TX Stream MCB.
 */
csitxStream_MCB    gCsiStreamMCB;

 /**
 * @brief
 * Test main task stack variable.
 */
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));

 /**
 * @brief
 * MMWave control task stack variable.
 */
StackType_t gCtrlTskStack[APP_CTRL_TASK_STACK_SIZE] __attribute__((aligned(32)));

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

csitxStream_ADCBufCfg adcbufconfig = 
{
    1,
    0,
    1,
    1
};

/**************************************************************************
 *********************** mmWave Unit Test Functions ***********************
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
static void csitxStream_populateDefaultProfileCfg (rlProfileCfg_t* ptrProfileCfg)
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
    ptrProfileCfg->numAdcSamples         = TESTSOURCE_NUM_ADC_SAMPLES;
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
static void csitxStream_populateDefaultChirpCfg (rlChirpCfg_t* ptrChirpCfg)
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
 *      The function is used to populate the default open configuration.
 *
 *  @param[out]  ptrOpenCfg
 *      Pointer to the open configuration
 *
 *  @retval
 *      Not applicable
 */
static void csitxStream_populateDefaultOpenCfg (MMWave_OpenCfg* ptrOpenCfg)
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
    ptrOpenCfg->chCfg.rxChannelEn = (1U << TESTSOURCE_NUM_RX_ANTENNAS) - 1U;
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
static void csitxStream_populateDefaultChirpControlCfg (MMWave_CtrlCfg* ptrCtrlCfg)
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
    csitxStream_populateDefaultProfileCfg (&profileCfg);

    /* Create the profile: */
    ptrCtrlCfg->u.frameCfg[0].profileHandle[0] = MMWave_addProfile (gCsiStreamMCB.mmWaveHandle, &profileCfg, &errCode);
    if (ptrCtrlCfg->u.frameCfg[0].profileHandle[0] == NULL)
    {
        test_print ("Error: Unable to add the profile [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add Profile", MCPI_TestResult_FAIL);
        DebugP_assert(0);
    }
    MCPI_setFeatureTestResult ("MMWave Add Profile", MCPI_TestResult_PASS);

    /**************************************************************************************************
     * Unit Test: Verify the Full Configuration Profile API
     **************************************************************************************************/
    {
        rlProfileCfg_t          profileCfgTmp;
        uint32_t                numProfiles;
        MMWave_ProfileHandle    tmpProfileHandle;

        /* Verify the number of profiles */
        if (MMWave_getNumProfiles (gCsiStreamMCB.mmWaveHandle, &numProfiles, &errCode) < 0)
        {
            test_print ("Error: Unable to get the number of profiles [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        if (numProfiles != 1U)
        {
            test_print ("Error: Invalid number of profiles detected [%d]\n", numProfiles);
            MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_FAIL);
        }
        MCPI_setFeatureTestResult ("MMWave Get Number Profile", MCPI_TestResult_PASS);

        /* Get the profile handle: */
        if (MMWave_getProfileHandle (gCsiStreamMCB.mmWaveHandle, 0U, &tmpProfileHandle, &errCode) < 0)
        {
            test_print ("Error: Unable to get the profile handle [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_FAIL);
        }
        if (tmpProfileHandle != ptrCtrlCfg->u.frameCfg[0].profileHandle[0])
        {
            test_print ("Error: Invalid profile handle detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_FAIL);
        }
        MCPI_setFeatureTestResult ("MMWave Get Profile Handle", MCPI_TestResult_PASS);

        /* Get the profile configuration */
        if (MMWave_getProfileCfg (ptrCtrlCfg->u.frameCfg[0].profileHandle[0], &profileCfgTmp, &errCode) < 0)
        {
            test_print ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        if (memcmp ((void*)&profileCfg, (void*)&profileCfgTmp, sizeof(rlProfileCfg_t)) != 0)
        {
            test_print ("Error: Invalid profile configuration detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        MCPI_setFeatureTestResult ("MMWave Get Profile", MCPI_TestResult_PASS);
    }

    /* Populate the default chirp configuration */
    csitxStream_populateDefaultChirpCfg (&chirpCfg);

    /* Add the chirp to the profile: */
    chirpHandle = MMWave_addChirp (ptrCtrlCfg->u.frameCfg[0].profileHandle[0], &chirpCfg, &errCode);
    if (chirpHandle == NULL)
    {
        test_print ("Error: Unable to add the chirp [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave Add Chirp", MCPI_TestResult_FAIL);
        DebugP_assert(0);
    }
    MCPI_setFeatureTestResult ("MMWave Add Chirp", MCPI_TestResult_PASS);

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
            MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        if (numChirps != 1U)
        {
            test_print ("Error: Invalid number of chirps detected [%d]\n", numChirps);
            MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        MCPI_setFeatureTestResult ("MMWave Get Number of Chirps", MCPI_TestResult_PASS);

        /* Get the Chirp Handle */
        if (MMWave_getChirpHandle (ptrCtrlCfg->u.frameCfg[0].profileHandle[0], 1U, &chirpHandleTmp, &errCode) < 0)
        {
            test_print ("Error: Unable to get the chirp handle [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        if (chirpHandleTmp != chirpHandle)
        {
            test_print ("Error: Chirp handle validation failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        MCPI_setFeatureTestResult ("MMWave Get Chirp Handle", MCPI_TestResult_PASS);

        /* Get the chirp configuration */
        if (MMWave_getChirpCfg (chirpHandle, &chirpCfgTmp, &errCode) < 0)
        {
            test_print ("Error: Unable to get the profile configuration [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("MMWave Get Chirp", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        if (memcmp ((void*)&chirpCfg, (void*)&chirpCfgTmp, sizeof(rlChirpCfg_t)) != 0)
        {
            test_print ("Error: Invalid chirp configuration detected\n");
            MCPI_setFeatureTestResult ("MMWave Get Chirp Configuration", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        MCPI_setFeatureTestResult ("MMWave Get Chirp Configuration", MCPI_TestResult_PASS);
    }

    /* Populate the frame configuration: */
    ptrCtrlCfg->u.frameCfg[0].frameCfg.chirpStartIdx      = TESTSOURCE_CHIRP_START_INDEX;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.chirpEndIdx        = TESTSOURCE_CHIRP_END_INDEX;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.numLoops           = TEST_SOURCE_NUM_LOOPS;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.numFrames          = TESTSOURCE_NUM_FRAMES;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.numAdcSamples      = TESTSOURCE_NUM_ADC_SAMPLES;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.framePeriodicity   = 10 * 1000000 / 5;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.triggerSelect      = 1;
    ptrCtrlCfg->u.frameCfg[0].frameCfg.frameTriggerDelay  = 0;

    return;
}


/**
 *  @b Description
 *  @n
 *      Registered event function which is invoked when an event from the
 *      BSS is received.
 *
 *  @param[in]  devIndex
 *      Device Index
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
static int32_t csitxStream_eventFxn(uint8_t devIndex, uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload)
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
                    
                    DebugP_log("Debug: Init Calibration Status = 0x%x\n", calibrationStatus);;
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
static void csitxStream_populateDefaultCalibrationCfg (MMWave_CalibrationCfg* ptrCalibrationCfg, MMWave_DFEDataOutputMode dfeOutputMode)
{
    /* Populate the calibration configuration: */
    ptrCalibrationCfg->dfeDataOutputMode                          = dfeOutputMode;
    ptrCalibrationCfg->u.chirpCalibrationCfg.enableCalibration    = false;
    ptrCalibrationCfg->u.chirpCalibrationCfg.enablePeriodicity    = false;
    ptrCalibrationCfg->u.chirpCalibrationCfg.periodicTimeInFrames = 10U;
    ptrCalibrationCfg->u.chirpCalibrationCfg.reportEn             = 0;
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
static void csitxStream_ctrlTask(void* args)
{
    int32_t errCode;

    while (1)
    {
        /* Execute the mmWave control module: */
        if (MMWave_execute (gCsiStreamMCB.mmWaveHandle, &errCode) < 0)
            test_print ("Error: mmWave control execution failed [Error code %d]\n", errCode);
    }
}

/**
 *  @b Description
 *  @n
 *      Function configures ADCBuf driver with data path parameters parsed from configurations
 *
 *  @param[in] adcBufHandle   ADCBuf driver handle
 *  @param[in] rxChannelEn    rx channel enable bit mask as described in rlChanCfg_t in rl_sensor.h
 *  @param[in] chirpThreshold  Chirp threshold
 *  @param[in] chanDataSize   Data size of the ADC channel
 *  @param[in] adcBufCfg     pointer to ADCBuf configuration
 *  @param[out] rxChanOffset  pointer to rx channel offset in the ADC buffer,
 *                            for each of enabled rx antenna
 *
 *
 *  @retval
 *      Success -   0
 *      Fail   -   None (it will assert and stop on failure and print the error code)
 */
int32_t csitxStream_ADCBufConfig
(
    ADCBuf_Handle         adcBufHandle,
    uint16_t              rxChannelEn,
    uint8_t               chirpThreshold,
    uint32_t              chanDataSize,
    csitxStream_ADCBufCfg  *adcBufCfg,
    uint16_t              *rxChanOffset
)
{
    ADCBuf_dataFormat   dataFormat;
    ADCBuf_RxChanConf   rxChanConf;
    int32_t             retVal = 0U;
    uint8_t             channel;
    uint32_t            rxChanMask = 0xF;
    int32_t             rxChanOffsetIndx = 0;

    /* ADCBuf requires argument pointer at 4bytes boundary*/
    uint32_t            chirpThresholdVal = chirpThreshold;

    /*****************************************************************************
     * Data path :: ADCBUF driver Configuration
     *****************************************************************************/
    /* Populate data format from configuration */
    dataFormat.adcOutFormat       = adcBufCfg->adcFmt;
    dataFormat.channelInterleave  = adcBufCfg->chInterleave;
    dataFormat.sampleInterleave   = adcBufCfg->iqSwapSel;

    /* Disable all ADCBuf channels */
    if ((retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_CHANNEL_DISABLE, (void *)&rxChanMask)) < 0)
    {
       test_print("Error: Disable ADCBuf channels failed with [Error=%d]\n", retVal);
       DebugP_assert(0);
    }

    retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_CONF_DATA_FORMAT, (void *)&dataFormat);
    if (retVal < 0)
    {
        test_print("Error: ADCBuf_control failed with [Error=%d]\n", retVal);
        DebugP_assert(0);
    }

    memset((void*)&rxChanConf, 0, sizeof(ADCBuf_RxChanConf));

    /* Enable Rx Channels */
    for (channel = 0; channel < SYS_COMMON_NUM_RX_CHANNEL; channel++)
    {
        if(rxChannelEn & (0x1U << channel))
        {
            /* Populate the receive channel configuration: */
            rxChanConf.channel = channel;
            retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_CHANNEL_ENABLE, (void *)&rxChanConf);
            if (retVal < 0)
            {
                test_print("Error: MSS ADCBuf Control for Channel %d Failed with error[%d]\n", channel, retVal);
                DebugP_assert(0);
            }
            /* Offset starts from 0 for the first channel */
            rxChanOffset[rxChanOffsetIndx++] = rxChanConf.offset;

            /* Calculate offset for the next channel */
            rxChanConf.offset  += chanDataSize * chirpThresholdVal;
        }
    }

    /* Set ping/pong chirp threshold: */
    retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_SET_PING_CHIRP_THRESHHOLD,
                            (void *)&chirpThresholdVal);
    if(retVal < 0)
    {
        test_print("Error: ADCbuf Ping Chirp Threshold Failed with Error[%d]\n", retVal);
        DebugP_assert(0);
    }
    retVal = ADCBuf_control(adcBufHandle, ADCBufMMWave_CMD_SET_PONG_CHIRP_THRESHHOLD,
                            (void *)&chirpThresholdVal);
    if(retVal < 0)
    {
        test_print("Error: ADCbuf Pong Chirp Threshold Failed with Error[%d]\n", retVal);
        DebugP_assert(0);
    }

    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Test implementation
 *
 *  @retval
 *      Not Applicable. It will return if success, and on failure - stop (assert fail) and print error code
 */
void csitxStream_mmWaveInit (void)
{
    MMWave_InitCfg          initCfg;
    MMWave_CtrlCfg          ctrlCfg;
    MMWave_OpenCfg          openCfg;
    int32_t                 errCode;
    MMWave_CalibrationCfg   calibrationCfg;
    int32_t                 retVal;
    MMWave_ErrorLevel       errorLevel;
    int16_t                 mmWaveErrorCode;
    int16_t                 subsysErrorCode;
    
    /* Initialize the configuration: */
    memset ((void *)&initCfg, 0, sizeof(MMWave_InitCfg));

    initCfg.domain                  = MMWave_Domain_MSS;
    initCfg.eventFxn                = csitxStream_eventFxn;
    initCfg.linkCRCCfg.crcBaseAddr  = (uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR);
    initCfg.linkCRCCfg.useCRCDriver = 1U;
    initCfg.linkCRCCfg.crcChannel   = CRC_CHANNEL_1;
    initCfg.cfgMode                 = MMWave_ConfigurationMode_FULL;

    /* Initialize and setup the mmWave Control module */
    gCsiStreamMCB.mmWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gCsiStreamMCB.mmWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        test_print ("Error Level: %s mmWave: %d Subsys: %d\n",
                       (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                       mmWaveErrorCode, subsysErrorCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave MSS Initialization", MCPI_TestResult_FAIL);
        DebugP_assert(0);
    }
    
    test_print ("MMWave MSS Initialization\n");

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    gMmwCtrlTask = xTaskCreateStatic( csitxStream_ctrlTask,
                                      "test_ctrl_task",
                                      APP_CTRL_TASK_STACK_SIZE,
                                      NULL,
                                      APP_CTRL_TASK_PRI,
                                      gCtrlTskStack,
                                      &gMmwCtrlTaskObj );

    configASSERT(gMmwCtrlTask != NULL);

    csitxStream_populateDefaultOpenCfg (&openCfg);
    csitxStream_populateDefaultChirpControlCfg (&ctrlCfg); /* regular frame config */
    
    /************************************************************************
     * Open the mmWave:
     ************************************************************************/
    if (MMWave_open (gCsiStreamMCB.mmWaveHandle, &openCfg, NULL, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        test_print ("Error: mmWave open failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave MSS Open", MCPI_TestResult_FAIL);
        DebugP_assert(0);
    }
   
    test_print ("MMWave MSS Open done.\n");

    /************************************************************************
     * Configure the mmWave:
     ************************************************************************/
    if (MMWave_config (gCsiStreamMCB.mmWaveHandle, &ctrlCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        test_print ("Error: mmWave configuration failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave MSS Configuration", MCPI_TestResult_FAIL);
        DebugP_assert(0);
    }

    
    test_print ("MMWave MSS Configuration done\n");

    /* Populate the calibration configuration: */
    memset ((void *)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));
    csitxStream_populateDefaultCalibrationCfg (&calibrationCfg, MMWave_DFEDataOutputMode_FRAME);

    /************************************************************************
     * Start the mmWave:
     ************************************************************************/
    if (MMWave_start (gCsiStreamMCB.mmWaveHandle, &calibrationCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        test_print ("Error: mmWave start failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave MSS Start", MCPI_TestResult_FAIL);
        DebugP_assert(0);
    }

    /* Wait for the CSI-TX  transfers to be complete. */
    while(gCsiStreamMCB.csitxStream.hwFrameDoneCount != TESTSOURCE_NUM_FRAMES)
    {
        /* Sleep and poll again: */
        ClockP_usleep(1 * 1000);
    }

    /************************************************************************
     * Stop the mmWave:
     ************************************************************************/
    retVal = MMWave_stop (gCsiStreamMCB.mmWaveHandle, &errCode);
    if (retVal < 0)
    {
        /* Error: Stopping the sensor failed. Decode the error code. */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        test_print ("Error Level: %s mmWave: %d Subsys: %d\n",
                       (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                       mmWaveErrorCode, subsysErrorCode);

        /* Did we fail because of an error? */
        if (errorLevel == MMWave_ErrorLevel_ERROR)
        {
            /* Error Level: The test has failed. */
            MCPI_setFeatureTestResult ("MMWave MSS Stop", MCPI_TestResult_FAIL);
            DebugP_assert(0);
        }
        else
        {
            /* Informational Level: The test has passed. Fall through...*/
        }
    }
    test_print ("MMWave MSS Stop done.\n");

    /************************************************************************
     * Close the mmWave:
     ************************************************************************/
    if (MMWave_close (gCsiStreamMCB.mmWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        test_print ("Error: mmWave close failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave MSS Close", MCPI_TestResult_FAIL);
        DebugP_assert(0);
    }
    MCPI_setFeatureTestResult ("MMWave MSS Close", MCPI_TestResult_PASS);
    test_print ("MMWave MSS close done.\n");
    
    /************************************************************************
     * Deinitialize the mmWave module:
     ************************************************************************/
    if (MMWave_deinit(gCsiStreamMCB.mmWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        test_print ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave MSS Deinitialized", MCPI_TestResult_FAIL);
        DebugP_assert(0);
    }
    MCPI_setFeatureTestResult ("MMWave MSS Deinitialized", MCPI_TestResult_PASS);
    
    return;
}

/**
 *  @b Description
 *  @n
 *      Function initializes and opens ADCBuf driver
 *
 *
 *  @retval
 *      Success -   ADCBuf driver handle
 *      Fail        NULL
 */
static ADCBuf_Handle csitxStream_ADCBufOpen(void)
{
    ADCBuf_Params       ADCBufparams;
    ADCBuf_Handle       ADCBufHandle = NULL;

    /*****************************************************************************
     * Start ADCBUF driver:
     *****************************************************************************/
    /* ADCBUF Params initialize */
    ADCBuf_Params_init(&ADCBufparams);
    ADCBufparams.chirpThresholdPing = 1;
    ADCBufparams.chirpThresholdPong = 1;
    ADCBufparams.continousMode  = 0;

    /* Open ADCBUF driver */
    ADCBufHandle = ADCBuf_open(0, &ADCBufparams);

    return ADCBufHandle;
}


/**
 *  @b Description
 *  @n
 *      Perform Data path driver open 
 *
 *  @retval
 *      Not Applicable.
 */
static void csitxStream_dataPathOpen(void)
{
    gCsiStreamMCB.adcBufHandle = csitxStream_ADCBufOpen();
    if(gCsiStreamMCB.adcBufHandle == NULL)
    {
        test_print ("Error: unable to open ADCBUFF instance.\n");
    }

    test_print("Debug: ADCBUF Instance %p has been opened successfully.\n", gCsiStreamMCB.adcBufHandle);
}

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void csitxStream_initTask(void* args)
{
    int32_t             errCode;
    uint16_t            rxChanOffset[4];

    /*Initialize the EDMA and CSI-TX driver in case it's selected as interface*/
    Drivers_open();
    Board_driversOpen();

    /* Debug Message: */
    test_print ("******************************************\n");
    test_print ("Debug: Launching CSI-TX Streaming Application.\n");
    test_print ("******************************************\n");

    /* Configure HSI Clock (default set at 1200 MHz). */
    HW_WR_REG32(CSL_MSS_TOPRCM_U_BASE + CSL_MSS_TOPRCM_HSI_CLK_SRC_SEL, 0x333);

    gCsiStreamMCB.edmaHandle = gEdmaHandle[CONFIG_EDMA0];
    
    /* Initialize Csi-Tx streaming components */
    if ((errCode = csitxStream_Init()) < 0 )
    {
        test_print ("Error: MSS Csi-Tx stream init failed with Error[%d]\n",errCode);
        DebugP_assert(0);
    }

    /* Ensure that interface selected for CBUFF is CSI-TX and not LVDS*/
    if(gCsiStreamMCB.cbuffInterface == CBUFF_Interface_CSI )
    {
        /*Interface is CSI-TX*/
        test_print("Debug: CSI-TX stream initialized successfully.\n");
    }
    else
    {
        test_print ("Error: Wrong Interface! \r\n");
        DebugP_assert(0);
    }


    /* Configure ADCBUF module. */
    csitxStream_dataPathOpen();
    
    /* Configure ADCBUF driver. */
    errCode = csitxStream_ADCBufConfig(gCsiStreamMCB.adcBufHandle,
                                 15,
                                 1,
                                 ((TESTSOURCE_NUM_ADC_SAMPLES * 2) + 15U) / 16U * 16U,
                                 &adcbufconfig,
                                 &rxChanOffset[0]);
    
    if (errCode < 0)
    {
        test_print("Error: ADCBuf config failed with error[%d]\n", errCode);
        DebugP_assert(0);
    }
    
    gCsiStreamMCB.subFrameCfg[0].numAdcSamples = TESTSOURCE_NUM_ADC_SAMPLES;
    gCsiStreamMCB.subFrameCfg[0].numChirpsPerSubFrame = TEST_SOURCE_NUM_LOOPS;
    gCsiStreamMCB.subFrameCfg[0].adcBufCfg.chirpThreshold = 1;
    gCsiStreamMCB.subFrameCfg[0].csitxStreamCfg.dataFmt = CSITX_STREAM_CFG_DATAFMT_ADC;

    gCsiStreamMCB.subFrameCfg[0].csitxStreamCfg.isHeaderEnabled = false;
    gCsiStreamMCB.subFrameCfg[0].csitxStreamCfg.isSwEnabled = false;
    gCsiStreamMCB.subFrameCfg[0].adcBufCfg.chInterleave = 1; /* non-interleaved */
    
    /* Configure HW Cbuff with CSI-TX stream for the ADC Date that will start upon
     * ADC Capture Complete */
    csitxStream_configCbuffHwData(0);

    /* Configure BSS. */
    csitxStream_mmWaveInit();

    if(CBUFF_deactivateSession (gCsiStreamMCB.csitxStream.hwSessionHandle, &errCode) < 0)
    {
        test_print("CSI-STREAM: CBUFF HW session deactivation error %d.\n", errCode);
        /* Error: Unable to deactivate the session. */
        DebugP_assert(0);
    }
    test_print("CSI-STREAM: CBUFF HW session deactivated.\n");

    /* Close CBUFF HW Session. */
    csitxStream_DeleteHwSession();

    test_print("------ end of test --------\n");
    test_print ("All tests have passed!!\n");

    vTaskDelete(NULL);
}

/**
 *  @b Description
 *  @n
 *      Entry point into the mmWave Unit Test
 *
 *  @retval
 *      Not Applicable.
 */
int32_t main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( csitxStream_initTask,
                                  "test_task_main",
                                  APP_TASK_STACK_SIZE,
                                  NULL,
                                  APP_TASK_PRI,
                                  gAppTskStackMain,
                                  &gAppTaskObj );
    configASSERT(gAppTask != NULL);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    /* The following line should never be reached because vTaskStartScheduler()
    will only return if there was not enough FreeRTOS heap memory available to
    create the Idle and (if configured) Timer tasks.  Heap management, and
    techniques for trapping heap exhaustion, are described in the book text. */
    DebugP_assertNoLog(0);
}
