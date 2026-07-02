/**
 *   @file  mss_distributed.c
 *
 *   @brief
 *     MSS Unit Test for the DPM
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
/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* MCU + SDK Include Files. */
#include <ti/control/dpm/test/distributed/mss/mssgenerated/ti_drivers_config.h>
#include <ti/control/dpm/test/distributed/mss/mssgenerated/ti_board_config.h>
#include <ti/control/dpm/test/distributed/mss/mssgenerated/ti_drivers_open_close.h>
#include <ti/control/dpm/test/distributed/mss/mssgenerated/ti_board_open_close.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/control/dpm/dpm.h>
#include <ti/utils/testlogger/logger.h>
/* DPC Include Files: */
#include <ti/control/dpm/test/distributed/dpc/ll/ll_common_profile.h>
#include <ti/control/dpm/test/distributed/dpc/hl/hl_profile_distributed.h>

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (2U)
#define MMW_CTRL_TASK_PRI    (6U)
#define LL_DPM_TASK_PRI      (5U)
#define HL_DPM_TASK_PRI      (4U)
#define DPM_DATA_TASK_PRI    (3U)

#define APP_TASK_STACK_SIZE  (2*1024U)
#define MMW_CTRL_TASK_STACK_SIZE (3 * 1024U)
#define LL_DPM_TASK_STACK_SIZE   (4 * 1024U)
#define HL_DPM_TASK_STACK_SIZE   (4 * 1024U)
#define DPM_DATA_TASK_STACK_SIZE (4 * 1024U)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

TaskHandle_t    gMMWCtrlTask;
StaticTask_t    gMMWCtrlTaskObj;

TaskHandle_t    gLLDPMTask;
StaticTask_t    gLLDPMTaskObj;

TaskHandle_t    gHLDPMTask;
StaticTask_t    gHLDPMTaskObj;

StaticTask_t    gDpmDataTaskObj;

StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));
StackType_t gDpmDataTskStack[DPM_DATA_TASK_STACK_SIZE] __attribute__((aligned(32)));
StackType_t gDpmTestHlTskStack[HL_DPM_TASK_STACK_SIZE] __attribute__((aligned(32)));
StackType_t gDpmTestLlTskStack[LL_DPM_TASK_STACK_SIZE] __attribute__((aligned(32)));
StackType_t gDpmTestMmwCtrlTskStack[MMW_CTRL_TASK_STACK_SIZE] __attribute__((aligned(32)));

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/**
 * @brief   Status flag which indicates that the LL Profile has been started
 * successfully
 */
#define TEST_APP_LL_PROFILE_START                   0x1

/**
 * @brief   Status flag which indicates that the ML Profile has been started
 * successfully
 */
#define TEST_APP_ML_PROFILE_START                   0x2

/**
 * @brief   Maximum number of Configuration/Start/Stop Loops which will be
 * done during the testing
 */
#define TEST_MAX_ITERATIONS                         2U

/**
 * @brief   Counter after which the DPC Stop Profiles are issued.
 */
#define TEST_STOP_COUNTER                           100U 

/**************************************************************************
 *************************** Local Structures *****************************
 **************************************************************************/

/**
 * @brief
 *  Unit Test MCB
 *
 * @details
 *  MCB which holds all the relevant information required to execute
 *  the Unit Test.
 */
typedef struct DPMTest_MCB_t
{
    /**
     * @brief   Handle to the mmWave control module
     */
    MMWave_Handle       ctrlHandle;

    /**
     * @brief   DPM Instance Handle: This is responsible for the control & execution
     * of the LL & ML Data Path profiles which execute on the MSS & DSS
     */
    DPM_Handle          llDPMHandle;

    /**
     * @brief   DPM Instance Handle: This is responsible for the control & execution
     * of the HL Data Path profile which executes only on the MSS
     */
    DPM_Handle          hlDPMHandle;

    /**
     * @brief   LL DPC Status: Applications track the status since the
     * profile is distributed
     */
    volatile uint32_t   llProfileStatus;

    /**
     * @brief   HL DPC Status:
     */
    volatile uint32_t   hlProfileStatus;

    /**
     * @brief   This is the number of results which have been successfully validated
     */
    uint32_t            validationCounter;

    /**
     * @brief   Raw Data Counter: This is just used to create a unique raw data for every
     * iteration
     */
    volatile uint32_t   rawDataCounter;

    /**
     * @brief   Inject Data: This is input data which is being injected into the DPM Data
     * Path Profile.
     */
    char                injectData[128];

    /**
     * @brief   Task Handle: This is to simulate the reception of the chirp data from
     * the BSS
     */
    TaskHandle_t         dataTaskHandle;
}DPMTest_MCB;

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/**
 * @brief   MSS Unit Test MCB
 */
DPMTest_MCB     gDPMTestMCB;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/

/* Unit Test MSS Task(s) */
static void DPMTest_mssInitTask (void* args);
static void DPMTest_mmWaveCtrlTask (void* args);
static void DPMTest_llDPMTask (void* args);
static void DPMTest_hlDPMTask (void* args);
static void DPMTest_dataTask (void* args);

/* DPC Mgmt Functions: */
static int32_t DPMTest_startProfiles (void);
static int32_t DPMTest_configureDPC (void);
static int32_t DPMTest_stopProfiles (void);

/* MMWave Event Handler: */
static int32_t DPMTest_mmWaveEventCallbackFxn(uint8_t devIndex, uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t* payload);

/* DPM Reporting API: */
static void DPMTest_hlReportFxn(DPM_Report reportType, uint32_t instanceId, int32_t errCode, uint32_t arg0, uint32_t arg1);
static void DPMTest_llReportFxn(DPM_Report reportType, uint32_t instanceId, int32_t errCode, uint32_t arg0, uint32_t arg1);

/**************************************************************************
 ************************* Unit Test MSS Functions ************************
 **************************************************************************/

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
 *      Always returns 0 [Continue passing the event to the peer domain]
 */
static int32_t DPMTest_mmWaveEventCallbackFxn
(
    uint8_t     devIndex,
    uint16_t    msgId,
    uint16_t    sbId,
    uint16_t    sbLen,
    uint8_t*    payload
)
{
    int32_t     errCode;
    uint16_t    asyncSB = RL_GET_SBID_FROM_UNIQ_SBID(sbId);

    /* Process the received message: */
    switch (msgId)
    {
        case RL_RF_ASYNC_EVENT_MSG:
        {
            /* Received Asychronous Message: */
            switch (asyncSB)
            {
                case RL_RF_AE_CPUFAULT_SB:
                {
                    break;
                }
                case RL_RF_AE_ESMFAULT_SB:
                {
                    break;
                }
                case RL_RF_AE_ANALOG_FAULT_SB:
                {
                    break;
                }
                case RL_RF_AE_INITCALIBSTATUS_SB:
                {
                    rlRfInitComplete_t*  ptrRFInitCompleteMessage;
                    uint32_t             calibrationStatus;

                    /* Get the RF-Init completion message: */
                    ptrRFInitCompleteMessage = (rlRfInitComplete_t*)payload;
                    calibrationStatus = ptrRFInitCompleteMessage->calibStatus & 0xFFFU;

                    /* Display the calibration status: */
                    test_print ("Debug: Init Calibration Status = 0x%x\n", calibrationStatus);
                    break;
                }
                case RL_RF_AE_FRAME_TRIGGER_RDY_SB:
                {
                    /* This event is not handled on MSS */
                    break;
                }
                case RL_RF_AE_MON_TIMING_FAIL_REPORT_SB:
                {
                    /* Increment the statistics for the number of failed reports */
                    break;
                }
                case RL_RF_AE_RUN_TIME_CALIB_REPORT_SB:
                {
                    /* Increment the statistics for the number of received calibration reports */
                    break;
                }
                case RL_RF_AE_FRAME_END_SB:
                {
                    /******************************************************************
                     * Frame Mode:
                     *  The ASYNC Event can be generated on either of the following
                     *  conditions:-
                     *   1. Sensor Stop CLI command has been issued which has caused
                     *      the MMWave_stop to be issued and the BSS has acknowledged
                     *      this
                     *   2. The configuration has caused all the frames to be sent out.
                     *  In both the cases the data path profile can be stopped.
                     ******************************************************************/
                    errCode = DPM_stop (gDPMTestMCB.llDPMHandle);
                    if (errCode < 0)
                    {
                        test_print ("Error: Unable to stop the profile [Error: %d]\n", errCode);
                        return -1;
                    }
                    break;
                }
                default:
                {
                    test_print ("Error: Asynchronous Event SB Id %d not handled\n", asyncSB);
                    break;
                }
            }
            break;
        }
        default:
        {
            test_print ("Error: Asynchronous message %d is NOT handled\n", msgId);
            break;
        }
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      This is the DPM Registered Report Handler which handles reports for the LL
 *      and ML Data Paths. These paths are executing on the MSS & DSS so this is
 *      a multiple domain DPM Instance.
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  instanceId
 *      Instance Identifier which generated the report
 *  @param[in]  errCode
 *      Error code if any.
 *  @param[in] arg0
 *      Argument 0 interpreted with the report type
 *  @param[in] arg1
 *      Argument 1 interpreted with the report type
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_llReportFxn
(
    DPM_Report  reportType,
    uint32_t    instanceId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
)
{
    /* Sanity Checking: The LL Report should only be invoked from the distributed LL or ML
     * Profile. */
    DebugP_assert ((instanceId == LL_DPM_INSTANCE_ID) || (instanceId == ML_DPM_INSTANCE_ID));

    /* Only errors are logged on the console: */
    if (errCode != 0)
    {
        /* Error: Detected log on the console and die all errors are FATAL currently. */
        test_print ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                        reportType, errCode, arg0, arg1);
        MCPI_setFeatureTestResult ("DPM Distributed Domain Report", MCPI_TestResult_FAIL);
        DebugP_assert (0);
    }

    /* Processing further is based on the reports received: This is the control of the profile
     * state machine: */
    switch (reportType)
    {
        case DPM_Report_IOCTL:
        {
            /*****************************************************************
             * Profile has been configured without an error:
             * - This is an indication that the profile configuration commands
             *   went through without any issues.
             *****************************************************************/
            if (arg0 == LL_CMD_FFTC_CFG)
            {
                MCPI_setFeatureTestResult ("LL DPC FFTC Configuration", MCPI_TestResult_PASS);
            }
            if (arg0 == LL_CMD_PEAK_GROUPING_CFG)
            {
                MCPI_setFeatureTestResult ("ML DPC Peak Grouping Configuration", MCPI_TestResult_PASS);
            }
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * Profile has been started without an error:
             * - Setup the application status
             *****************************************************************/
            if (instanceId == LL_DPM_INSTANCE_ID)
            {
                gDPMTestMCB.llProfileStatus = gDPMTestMCB.llProfileStatus | TEST_APP_LL_PROFILE_START;
                MCPI_setFeatureTestResult ("LL DPC Started", MCPI_TestResult_PASS);
            }
            else if (instanceId == ML_DPM_INSTANCE_ID)
            {
                gDPMTestMCB.llProfileStatus = gDPMTestMCB.llProfileStatus | TEST_APP_ML_PROFILE_START;
                MCPI_setFeatureTestResult ("ML DPC Started", MCPI_TestResult_PASS);
            }
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT:
        {
            /*****************************************************************
             * ML Profile has reported results
             *****************************************************************/
            DPM_Buffer*     ptrResult;
            uint32_t        translatedAddress;

            /* Sanity Checking: */
            DebugP_assert (instanceId == ML_DPM_INSTANCE_ID);
            ptrResult = (DPM_Buffer*)arg0;

            translatedAddress=(uint32_t) AddrTranslateP_getLocalAddr((uint32_t)ptrResult->ptrBuffer[0]);
            
            /* Update to the translated address: */
            ptrResult->ptrBuffer[0] = (uint8_t*)translatedAddress;

            /*****************************************************************
             * Inject this data to the HL Profile:-
             *****************************************************************/
            if (gDPMTestMCB.hlProfileStatus == 1U)
            {
                errCode = DPM_ioctl (gDPMTestMCB.hlDPMHandle,
                                     DPM_CMD_INJECT_DATA,
                                     ptrResult,
                                     sizeof(DPM_Buffer));
                if (errCode < 0)
                {
                    test_print ("Error: Unable to inject the data into the HL Profile [Error code %d]\n", errCode);
                    MCPI_setFeatureTestResult ("DPM Inject Data", MCPI_TestResult_FAIL);
                    return;
                }
            }
            break;
        }
        case DPM_Report_DPC_ASSERT:
        {
            /* Profile asserted: Kill the system */
            DebugP_assert (0);
            break;
        }
        case DPM_Report_DPC_STOPPED:
        {
            /*****************************************************************
             * Profile has been stopped without an error:
             *****************************************************************/
            if (instanceId == LL_DPM_INSTANCE_ID)
            {
                gDPMTestMCB.llProfileStatus = gDPMTestMCB.llProfileStatus & (~TEST_APP_LL_PROFILE_START);
                MCPI_setFeatureTestResult ("LL DPC Stopped", MCPI_TestResult_PASS);
            }
            else if (instanceId == ML_DPM_INSTANCE_ID)
            {
                gDPMTestMCB.llProfileStatus = gDPMTestMCB.llProfileStatus & (~TEST_APP_ML_PROFILE_START);
                MCPI_setFeatureTestResult ("ML DPC Stopped", MCPI_TestResult_PASS);
            }
            break;
        }
        case DPM_Report_DPC_INFO:
        {
            DebugP_log("DPM_Report_DPC_INFO");
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT_ACKED:
        {
            DebugP_log("DPM_Report_NOTIFY_DPC_RESULT_ACKED");
            break;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      This is the DPM Registered Report Handler which handles reports for the HL
 *      Data Paths. This is executing on the MSS only
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  instanceId
 *      Instance Identifier which generated the report
 *  @param[in]  errCode
 *      Error code if any.
 *  @param[in] arg0
 *      Argument 0 interpreted with the report type
 *  @param[in] arg1
 *      Argument 1 interpreted with the report type
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_hlReportFxn
(
    DPM_Report  reportType,
    uint32_t    instanceId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
)
{
    char    logString[128];

    /* Sanity Checking: Only the HL Profile should generated the report */
    DebugP_assert (instanceId == HL_DPM_INSTANCE_ID);

    /* Only errors are logged on the console: */
    if (errCode != 0)
    {
        /* Error: Detected log on the console and die all errors are FATAL currently. */
        test_print ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                        reportType, errCode, arg0, arg1);
        MCPI_setFeatureTestResult ("DPM Local Domain Report", MCPI_TestResult_FAIL);
        DebugP_assert (0);
    }

    /* Processing further is based on the reports received: This is the control of the profile
     * state machine: */
    switch (reportType)
    {
        case DPM_Report_IOCTL:
        {
            /*****************************************************************
             * Profile has been configured without an error:
             * - This is an indication that the profile configuration commands
             *   went through without any issues.
             *****************************************************************/
            if (arg0 == HL_CMD_GTRACK_CFG)
            {
                MCPI_setFeatureTestResult ("HL DPC GTrack Configuration", MCPI_TestResult_PASS);
            }
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * Profile has been started without an error:
             *****************************************************************/
            MCPI_setFeatureTestResult ("HL DPC Started", MCPI_TestResult_PASS);
            gDPMTestMCB.hlProfileStatus = 1U;
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT:
        {
            break;
        }
        case DPM_Report_DPC_ASSERT:
        {
            /* Profile asserted: Kill the system */
            DebugP_assert (0);
            break;
        }
        case DPM_Report_DPC_STOPPED:
        {
            /*****************************************************************
             * Profile has been stopped without an error:
             * - This implies that the profile can either be reconfigured or
             *   restarted. Please refer to the profile documentation for more
             *   information.
             *****************************************************************/
            gDPMTestMCB.hlProfileStatus = 0U;

            /* Create the MCPI Logging String: */
            sprintf (logString, "HL DPC Stopped Results validated=%d", gDPMTestMCB.validationCounter);
            MCPI_setFeatureTestResult (logString, MCPI_TestResult_PASS);

            /* Reset the counter: */
            gDPMTestMCB.validationCounter = 0U;
            break;
        }
        case DPM_Report_DPC_INFO:
        {
            DebugP_log("DPM_Report_DPC_INFO");
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT_ACKED:
        {
            DebugP_log("DPM_Report_NOTIFY_DPC_RESULT_ACKED");
            break;
        }
    }
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
static void DPMTest_mmWaveCtrlTask(void* args)
{
    int32_t errCode;

    while (1)
    {
        /* Execute the mmWave control module: */
        if (MMWave_execute (gDPMTestMCB.ctrlHandle, &errCode) < 0)
            test_print ("Error: mmWave control execution failed [Error code %d]\n", errCode);
    }
}

/**
 *  @b Description
 *  @n
 *      DPM Execution Task which executes the DPM Instance which manages the
 *      LL & ML Profiles executing on the MSS & DSS.
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_llDPMTask(void* args)
{
    int32_t     errCode;
    DPM_Buffer  result;

    while (1)
    {
        /* Execute the DPM module: */
        errCode = DPM_execute (gDPMTestMCB.llDPMHandle, &result);
        if (errCode < 0)
        {
            test_print ("Error: DPM execution failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("DPM Execute", MCPI_TestResult_FAIL);
        }
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to validate the results.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t DPMTest_validateResults (DPM_Buffer* ptrResult)
{
    char        resultString[256];

    /* Sanity Checking: The Test Profiles only generate one result */
    if ((ptrResult->ptrBuffer[0] == NULL) || (ptrResult->size[0] == 0U))
    {
        /* Error: Invalid result detected */
        return -1;
    }

    /* Sanity Checking: The Test Profiles only generate one result */
    if ((ptrResult->ptrBuffer[1] != NULL) || (ptrResult->size[1] != 0U))
    {
        /* Error: Invalid result detected */
        return -1;
    }

    /* Sanity Checking: The Test Profiles only generate one result */
    if ((ptrResult->ptrBuffer[2] != NULL) || (ptrResult->size[2] != 0U))
    {
        /* Error: Invalid result detected */
        return -1;
    }

    /* Construct the expected result: The raw data counter has already been incremented so
     * we need to discount it here. All the configurations are hardcoded here. */
    snprintf (resultString, sizeof(resultString),
              "[Raw Data: %d]LL Processing [FFTC 100:200]ML Processing [Peak Grouping 300]HL Processing [GTrack 400:500:600]",
              (gDPMTestMCB.rawDataCounter - 1));

    /* Compare the strings: */
    if (strcmp (resultString, (char*)ptrResult->ptrBuffer[0]) == 0)
    {
        /* Increment the number of successful validations: */
        gDPMTestMCB.validationCounter++;
        return 0;
    }

    /* No match: */
    test_print ("Result String  : '%s'\n", resultString);
    test_print ("Expected String: '%s'\n", (char*)ptrResult->ptrBuffer[0]);
    return -1;
}

/**
 *  @b Description
 *  @n
 *      DPM Execution Task which executes the DPM Instance which manages the
 *      HL Profiles executing on the MSS.
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_hlDPMTask(void* args)
{
    int32_t     errCode;
    DPM_Buffer  result;

    while (1)
    {
        /* Execute the DPM module: */
        errCode = DPM_execute (gDPMTestMCB.hlDPMHandle, &result);
        if (errCode < 0)
        {
            test_print ("Error: DPM execution failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("DPM Execute", MCPI_TestResult_FAIL);
        }

        /* Do we have results which are available and is the HL DPC still active. */
        if ((result.size[0] != 0U) && (gDPMTestMCB.hlProfileStatus == 1U))
        {
            /* Sanity Checking: Validate the received results */
            if (DPMTest_validateResults (&result) < 0)
            {
                MCPI_setFeatureTestResult ("DPM Validate Results", MCPI_TestResult_FAIL);
            }
        }
        /* Reset the buffer for the next iteration */
        memset ((void *)result.ptrBuffer[0], 0, result.size[0]);
    }
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure the DPC Instances in the system
 *      This will configure the LL & HL Profiles with a hardcoded
 *      configuration
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t DPMTest_configureDPC (void)
{
    int32_t             errCode;
    LL_FFTCCfg          fftcCfg;
    LL_PeakGroupingCfg  peakGroupingCfg;
    HL_GTrackCfg        gtrackCfg;
    int32_t             retVal = MINUS_ONE;

    /************************************************************************************
     * LL Data Path Profile:
     * - Configure the FFTC
     ************************************************************************************/
    memset ((void *)&fftcCfg, 0, sizeof(LL_FFTCCfg));

    /* Populate the FFTC configuration: */
    fftcCfg.dummy1 = 100;
    fftcCfg.dummy2 = 200;

    /* Configure the FFTC */
    errCode = DPM_ioctl (gDPMTestMCB.llDPMHandle,
                         LL_CMD_FFTC_CFG,
                         &fftcCfg,
                         sizeof(LL_FFTCCfg));

    if (errCode < 0)
    {
        test_print ("Error: Unable to configure the FFTC [Error code %d]\n", errCode);
        goto exit;
    }

    /************************************************************************************
     * LL Data Path Profile:
     * - Configure the Peak Grouping
     ************************************************************************************/
    memset ((void *)&peakGroupingCfg, 0, sizeof(LL_PeakGroupingCfg));

    /* Populate the Peak Grouping configuration: */
    peakGroupingCfg.dummy1 = 300;

    /* Configure the Peak Grouping */
    errCode = DPM_ioctl (gDPMTestMCB.llDPMHandle,
                         LL_CMD_PEAK_GROUPING_CFG,
                         &peakGroupingCfg,
                         sizeof(LL_PeakGroupingCfg));
    if (errCode < 0)
    {
        test_print ("Error: Unable to configure the Peak Grouping [Error code %d]\n", errCode);
        goto exit;
    }

    /************************************************************************************
     * HL Data Path Profile:
     * - Configure the GTrack
     ************************************************************************************/
    memset ((void *)&gtrackCfg, 0, sizeof(HL_GTrackCfg));

    /* Populate the GTrack configuration: */
    gtrackCfg.dummy1 = 400;
    gtrackCfg.dummy2 = 500;
    gtrackCfg.dummy3 = 600;

    /* Configure the GTrack. */
    errCode = DPM_ioctl (gDPMTestMCB.hlDPMHandle,
                         HL_CMD_GTRACK_CFG,
                         &gtrackCfg,
                         sizeof(HL_GTrackCfg));

    if (errCode < 0)
    {
        test_print ("Error: Unable to configure the GTrack [Error code %d]\n", errCode);
        goto exit;
    }

    /************************************************************************************
     * Configurations have been completed:
     *  - Once the reports have been received to indicate that the configuration is
     *    completed the profiles can be started
     ************************************************************************************/
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to start the DPC Profiles
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t DPMTest_startProfiles (void)
{
    int32_t     errCode;
    int32_t     retVal = MINUS_ONE;

    /* Start the Distributed LL/ML Data Path Profile: */
    errCode = DPM_start (gDPMTestMCB.llDPMHandle);
    if (errCode < 0)
    {
        test_print ("Error: Unable to start the LL/ML Profile [Error code %d]\n", errCode);
        goto exit;
    }

    /* Start the HL Data Path Profiles: */
    errCode = DPM_start (gDPMTestMCB.hlDPMHandle);
    if (errCode < 0)
    {
        test_print ("Error: Unable to start the HL Profile [Error code %d]\n", errCode);
        goto exit;
    }

    /* Profiles have been started: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is used to stop the DPC Profiles
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
static int32_t DPMTest_stopProfiles (void)
{
    int32_t     errCode;
    int32_t     retVal = MINUS_ONE;

    /* Stop the Distributed LL/ML Data Path Profile: */
    errCode = DPM_stop (gDPMTestMCB.llDPMHandle);
    if (errCode < 0)
    {
        test_print ("Error: Unable to Stop the LL/ML Profile [Error code %d]\n", errCode);
        goto exit;
    }

    /* Stop the HL Data Path Profiles: */
    errCode = DPM_stop (gDPMTestMCB.hlDPMHandle);
    if (errCode < 0)
    {
        test_print ("Error: Unable to Stop the HL Profile [Error code %d]\n", errCode);
        goto exit;
    }

    /* Profiles have been stopped: */
    retVal = 0;

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Test Task: The Task periodically executes and injects data
 *      into the profile. This is to simulate the reception of raw
 *      data.
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_dataTask (void* args)
{
    DPM_Buffer      buffer;
    int32_t         errCode;

    /* Loop around forever: */
    while (1)
    {
        /* Is the profile operational? */
        if (gDPMTestMCB.llProfileStatus == (TEST_APP_LL_PROFILE_START | TEST_APP_ML_PROFILE_START))
        {
            /* YES: Initialize the buffer */
            memset ((void *)&buffer, 0, sizeof(DPM_Buffer));

            /* Setup the data buffer: */
            sprintf (gDPMTestMCB.injectData, "Raw Data: %d", gDPMTestMCB.rawDataCounter++);
            buffer.ptrBuffer[0] = (uint8_t*)&gDPMTestMCB.injectData[0];
            buffer.size[0]      = strlen (gDPMTestMCB.injectData);

            /* Inject the buffer: */
            errCode = DPM_ioctl (gDPMTestMCB.llDPMHandle,
                                 DPM_CMD_INJECT_DATA,
                                 &buffer,
                                 sizeof(DPM_Buffer));
            if (errCode < 0)
            {
                test_print ("Error: Unable to inject the data [Error code %d]\n", errCode);
                MCPI_setFeatureTestResult ("DPM Inject Data", MCPI_TestResult_FAIL);
            }
        }

        /* Wait for some time: */
        ClockP_usleep(100 * 1000);
    }
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
static void DPMTest_mssInitTask(void* args)
{
    int32_t             errCode;
    MMWave_InitCfg      initCfg;
    DPM_InitCfg         dpmInitCfg;
    uint32_t            initArg;
    uint8_t             numLoops       = 0U;
    bool                testCompletion = false;

    /* Initialize and populate the demo MCB */
    memset ((void*)&gDPMTestMCB, 0, sizeof(DPMTest_MCB));

    /*****************************************************************************
     * Initialize the mmWave SDK components:
     *****************************************************************************/
    Drivers_open();
    Board_driversOpen();

    /* Initialize the MCPI framework: */
    MCPI_Initialize ();

    /*****************************************************************************
     * mmWave: Initialization of the high level module
     *****************************************************************************/

    /* Initialize the mmWave control init configuration */
    memset ((void*)&initCfg, 0 , sizeof(MMWave_InitCfg));

    /* Populate the init configuration for mmwave library: */
    initCfg.domain                      = MMWave_Domain_MSS;
    initCfg.eventFxn                    = DPMTest_mmWaveEventCallbackFxn;
    initCfg.linkCRCCfg.crcBaseAddr      = (uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR);
    initCfg.linkCRCCfg.useCRCDriver     = 1U;
    initCfg.linkCRCCfg.crcChannel       = CRC_CHANNEL_1;
    initCfg.cfgMode                     = MMWave_ConfigurationMode_FULL;

    /* Initialize and setup the mmWave Control module */
    gDPMTestMCB.ctrlHandle = MMWave_init (&initCfg, &errCode);
    if (gDPMTestMCB.ctrlHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        test_print("Error: MMWDemoMSS mmWave Control Initialization failed [Error code %d]\n", errCode);
        return;
    }

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    gMMWCtrlTask = xTaskCreateStatic( DPMTest_mmWaveCtrlTask,   /* Pointer to the function that implements the task. */
                                  "test_mmwCtrl_task", /* Text name for the task.  This is to facilitate debugging only. */
                                  MMW_CTRL_TASK_STACK_SIZE,  /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                  NULL,              /* We are not using the task parameter. */
                                  MMW_CTRL_TASK_PRI,      /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                  gDpmTestMmwCtrlTskStack,  /* pointer to stack base */
                                  &gMMWCtrlTaskObj );    /* pointer to statically allocated task object memory */
    configASSERT(gMMWCtrlTask != NULL);

    /*****************************************************************************
     * DPM Instance1:
     * - The DPM Instance is responsible for executing the LL & ML Profiles on the
     *   MSS and DSS so this is a multiple domain instance.
     *****************************************************************************/
    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Initialize the LL Processing Init Argument: */
    initArg = LL_INIT_ARGUMENT;

    /* Setup the configuration: */
    dpmInitCfg.arg              = (void *)&initArg;
    dpmInitCfg.argSize          = sizeof(initArg);
    dpmInitCfg.ptrProcChainCfg  = &gLLProfileCfg;
    dpmInitCfg.instanceId       = LL_DPM_INSTANCE_ID;
    dpmInitCfg.domain           = DPM_Domain_DISTRIBUTED;
    dpmInitCfg.reportFxn        = DPMTest_llReportFxn;

    /* Initialize the DPM Module: */
    gDPMTestMCB.llDPMHandle = DPM_init (&dpmInitCfg, &errCode);
    if (gDPMTestMCB.llDPMHandle == NULL)
    {
        test_print ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        MCPI_setFeatureTestResult ("DPM Distributed Domain Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("DPM Distributed Domain Initialization", MCPI_TestResult_PASS);

    /* Launch the DPM Task:
     * - This is always a lower priority than the mmWave control task because
     *   control events should always be higher priority. */
    gLLDPMTask = xTaskCreateStatic( DPMTest_llDPMTask,   /* Pointer to the function that implements the task. */
                                  "test_llDPM_task", /* Text name for the task.  This is to facilitate debugging only. */
                                  LL_DPM_TASK_STACK_SIZE,  /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                  NULL,              /* We are not using the task parameter. */
                                  LL_DPM_TASK_PRI,      /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                  gDpmTestLlTskStack,  /* pointer to stack base */
                                  &gLLDPMTaskObj );    /* pointer to statically allocated task object memory */
    configASSERT(gLLDPMTask != NULL);

    /* Synchronization: This will synchronize the execution of the control module
     * between the domains. This is a prerequiste and always needs to be invoked. */
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = DPM_synch (gDPMTestMCB.llDPMHandle, &errCode);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the framework */
            test_print ("Error: DPM Synchronization failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("DPM Distributed Domain Synchronization", MCPI_TestResult_FAIL);
            return;
        }
        if (syncStatus == 1)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        ClockP_usleep(1 * 1000);
    }
    MCPI_setFeatureTestResult ("DPM Distributed Domain Synchronization", MCPI_TestResult_PASS);

    /*****************************************************************************
     * DPM Instance2:
     * - The DPM Instance is responsible for executing the HL Profiles on the
     *   MSS only so this is a single domain.
     *****************************************************************************/
    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Initialize the HL Processing Init Argument: */
    initArg = HL_INIT_ARGUMENT;

    /* Setup the configuration: */
    dpmInitCfg.domain           = DPM_Domain_LOCALIZED;
    dpmInitCfg.arg              = &initArg;
    dpmInitCfg.argSize          = sizeof(uint32_t);
    dpmInitCfg.ptrProcChainCfg  = &gHLProfileCfg;
    dpmInitCfg.instanceId       = HL_DPM_INSTANCE_ID;
    dpmInitCfg.reportFxn        = DPMTest_hlReportFxn;

    /* Initialize the DPM Module: */
    gDPMTestMCB.hlDPMHandle = DPM_init (&dpmInitCfg, &errCode);
    if (gDPMTestMCB.hlDPMHandle == NULL)
    {
        test_print ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        MCPI_setFeatureTestResult ("DPM Local Domain Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("DPM Local Domain Initialization", MCPI_TestResult_PASS);

    /************************************************************************************
     * NOTE: No need to perform the DPM synchronization since this is executing
     * in a Local domain
     ************************************************************************************/

    /* Launch the DPM Task:
     * - This is always a lower priority than the mmWave control task because
     *   control events should always be higher priority. Also this is set to
     *   a lower priority than the LL DPM Instance */
    gHLDPMTask = xTaskCreateStatic( DPMTest_hlDPMTask,   /* Pointer to the function that implements the task. */
                                  "test_hl_dpm_task", /* Text name for the task.  This is to facilitate debugging only. */
                                  HL_DPM_TASK_STACK_SIZE,  /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                  NULL,              /* We are not using the task parameter. */
                                  HL_DPM_TASK_PRI,      /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                  gDpmTestHlTskStack,  /* pointer to stack base */
                                  &gHLDPMTaskObj );    /* pointer to statically allocated task object memory */
    configASSERT(gHLDPMTask != NULL);

    /*****************************************************************************
     * Test Executing Loop:
     *****************************************************************************/
    while (testCompletion == false)
    {
        /* Configure the DPC Profiles: */
        if (DPMTest_configureDPC () < 0)
        {
            /* Error: Unable to configure the DPC. */
            testCompletion = true;
            MCPI_setFeatureTestResult ("DPM Configuration", MCPI_TestResult_FAIL);
            continue;
        }
        MCPI_setFeatureTestResult ("DPM Configuration", MCPI_TestResult_PASS);

        /* Initialize the Raw Data counter: */
        gDPMTestMCB.rawDataCounter = 1U;

        /* Start the DPC Profiles: */
        if (DPMTest_startProfiles () < 0)
        {
            /* Error: Unable to start the DPC. */
            testCompletion = true;
            MCPI_setFeatureTestResult ("DPM Start", MCPI_TestResult_FAIL);
            continue;
        }
        MCPI_setFeatureTestResult ("DPM Start", MCPI_TestResult_PASS);

        /* Synchronization: Once the DPC LL & HL profiles have been started only then can we
         * can start the Data Injection Task i.e. sensor start */
        while ((gDPMTestMCB.llProfileStatus & (TEST_APP_LL_PROFILE_START | TEST_APP_ML_PROFILE_START)) == 0);
        while (gDPMTestMCB.hlProfileStatus == 0);

        gDPMTestMCB.dataTaskHandle = xTaskCreateStatic( DPMTest_dataTask,   /* Pointer to the function that implements the task. */
                                    "test_dpm_data_task", /* Text name for the task.  This is to facilitate debugging only. */
                                    DPM_DATA_TASK_STACK_SIZE,  /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                    NULL,              /* We are not using the task parameter. */
                                    DPM_DATA_TASK_PRI,      /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                    gDpmDataTskStack,  /* pointer to stack base */
                                    &gDpmDataTaskObj );    /* pointer to statically allocated task object memory */
        configASSERT(gDPMTestMCB.dataTaskHandle != NULL);

        /* Loop around till we can stop the profile... */
        while ((gDPMTestMCB.rawDataCounter % TEST_STOP_COUNTER) != 0U);

        /* Stop the sensor i.e. the Data Injection Task */
        vTaskDelete(gDPMTestMCB.dataTaskHandle);

        /* Stop the DPC Profiles: */
        if (DPMTest_stopProfiles () < 0)
        {
            /* Error: Unable to stop the DPC. */
            testCompletion = true;
            MCPI_setFeatureTestResult ("DPM Stop", MCPI_TestResult_FAIL);
            continue;
        }
        MCPI_setFeatureTestResult ("DPM Stop", MCPI_TestResult_PASS);

        /* Increment the number of loops: */
        numLoops = numLoops + 1U;

        /* Did we reach the max threshold? */
        if (numLoops == TEST_MAX_ITERATIONS)
        {
            testCompletion = true;
            continue;
        }
    }
    test_print ("Debug: DPM Distributed Unit Testing completed\n");
    MCPI_setTestResult ();

    test_print ("All tests have passed!!\n");

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the Millimeter Wave Demo
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( DPMTest_mssInitTask,   /* Pointer to the function that implements the task. */
                                  "test_task_main", /* Text name for the task.  This is to facilitate debugging only. */
                                  APP_TASK_STACK_SIZE,  /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                  NULL,              /* We are not using the task parameter. */
                                  APP_TASK_PRI,      /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                  gAppTskStackMain,  /* pointer to stack base */
                                  &gAppTaskObj );    /* pointer to statically allocated task object memory */
    configASSERT(gAppTask != NULL);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    /* The following line should never be reached because vTaskStartScheduler()
    will only return if there was not enough FreeRTOS heap memory available to
    create the Idle and (if configured) Timer tasks.  Heap management, and
    techniques for trapping heap exhaustion, are described in the book text. */
    DebugP_assertNoLog(0);
}
