/**
 *   @file  mss_remote.c
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
#include <ti/control/dpm/test/remote/mss/mssgenerated/ti_drivers_config.h>
#include <ti/control/dpm/test/remote/mss/mssgenerated/ti_board_config.h>
#include <ti/control/dpm/test/remote/mss/mssgenerated/ti_drivers_open_close.h>
#include <ti/control/dpm/test/remote/mss/mssgenerated/ti_board_open_close.h>
#include <drivers/hw_include/cslr_soc.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <ti/control/dpm/dpm.h>
#include <ti/utils/testlogger/logger.h>
/* Demo Include Files: */
#include <ti/control/dpm/test/remote/dpc/hl/hl_profile_remote.h>
#include <ti/control/dpm/test/remote/dpc/ll/ll_profile_remote.h>

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (2U)
#define APP_DPM_TEST_TASK_PRI (10U)
#define APP_DPM_TEST_HL_TASK_PRI (4U)
#define APP_TASK_STACK_SIZE  (8*1024U)
#define APP_DPM_TEST_TASK_STACK_SIZE (4 * 1024U)
#define APP_DPM_TEST_HL_TASK_STACK_SIZE (4*1024U)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

TaskHandle_t    gDpmTestTask;
StaticTask_t    gDpmTestTaskObj;

TaskHandle_t    gDpmTestHlTask;
StaticTask_t    gDpmTestHlTaskObj;


StackType_t  gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));
StackType_t  gDpmTestHlTskStack[APP_DPM_TEST_HL_TASK_STACK_SIZE] __attribute__((aligned(32)));
StackType_t  gDpmTestLlTskStack[APP_DPM_TEST_TASK_STACK_SIZE] __attribute__((aligned(32)));

/**************************************************************************
 *************************** Local Definitions ****************************
 **************************************************************************/

/**
 * @brief   Maximum number of Configuration/Start/Stop Loops which will be
 * done during the testing
 */
#define TEST_MAX_ITERATIONS                         2U

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
     * @brief   DPM Instance Handle: This is responsible for the control & execution
     * of the LL Data Path profiles which execute on the MSS & DSS
     */
    DPM_Handle          llDPMHandle;

    /**
     * @brief   DPM Instance Handle: This is responsible for the control & execution
     * of the HL Data Path profile which executes only on the MSS
     */
    DPM_Handle          hlDPMHandle;

    /**
     * @brief   This is the number of results which have been successfully validated
     * On the MSS we are executing only the HL Profile
     */
    uint32_t            validationCounter;

    /**
     * @brief   The LL Data Path Profile Status
     */
    volatile bool       llProfileStatus;

    /**
     * @brief   The HL Data Path Profile Status
     */
    volatile bool       hlProfileStatus;
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

/**
* The following registers are used to control the data injection on the DSS
* This is controlled by the MSS.
* MSS_CTRL:MSS_BOOT_INFO_REG6  
* MSS_CTRL:MSS_BOOT_INFO_REG7
*/

/**************************************************************************
 **************************** Local Functions *****************************
 **************************************************************************/

/* Unit Test MSS Task(s) */
static void DPMTest_mssInitTask(void* args);
static void DPMTest_llDPMTask(void* args);

/* DPC Mgmt Functions: */
static int32_t DPMTest_startProfiles (void);
static int32_t DPMTest_configureDPC (void);
static int32_t DPMTest_stopProfiles (void);

/* DPM Reporting API: */
static void DPMTest_hlReportFxn(DPM_Report reportType, uint32_t profileId, int32_t errCode, uint32_t arg0, uint32_t arg1);
static void DPMTest_llReportFxn(DPM_Report reportType, uint32_t profileId, int32_t errCode, uint32_t arg0, uint32_t arg1);

/**************************************************************************
 ************************* Unit Test MSS Functions ************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      This is the DPM Registered Report Handler which handles reports for the LL
 *      Data Path Profile.
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
    /* Sanity Checking: */
    DebugP_assert ((instanceId == LL_MSS_DPM_INSTANCE_ID) || (instanceId == LL_DSS_DPM_INSTANCE_ID));

    /* Only errors are logged on the console: */
    if (errCode != 0)
    {
        /* Error: Detected log on the console and die all errors are FATAL currently. */
        test_print ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                        reportType, errCode, arg0, arg1);
        MCPI_setFeatureTestResult ("DPM Remote Domain Report", MCPI_TestResult_FAIL);
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
                MCPI_setFeatureTestResult ("LL DPC Peak Grouping Configuration", MCPI_TestResult_PASS);
            }
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * Profile has been started without an error:
             *****************************************************************/
            gDPMTestMCB.llProfileStatus = true;
            MCPI_setFeatureTestResult ("LL DPC Started", MCPI_TestResult_PASS);
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT:
        {
            /*****************************************************************
             * LL Profile has reported results
             *****************************************************************/
            DPM_Buffer*     ptrResult;
            uint32_t        translatedAddress;

            /* Get the result: */
            ptrResult = (DPM_Buffer*)arg0;
   
            translatedAddress=(uint32_t) AddrTranslateP_getLocalAddr((uint32_t)ptrResult->ptrBuffer[0]);

            /* Update to the translated address: */
            ptrResult->ptrBuffer[0] = (uint8_t*)translatedAddress;
            

            /*****************************************************************
             * Inject this data to the HL Profile (Only if it is active)
             *****************************************************************/
            if (gDPMTestMCB.hlProfileStatus == true)
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
            gDPMTestMCB.llProfileStatus = false;
            MCPI_setFeatureTestResult ("LL DPC Stopped", MCPI_TestResult_PASS);
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
                MCPI_setFeatureTestResult ("HL DPC GTRACK Configuration", MCPI_TestResult_PASS);
            }
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * Profile has been started without an error:
             *****************************************************************/
            gDPMTestMCB.hlProfileStatus = true;
            MCPI_setFeatureTestResult ("HL DPC Started", MCPI_TestResult_PASS);
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
             *****************************************************************/
            gDPMTestMCB.hlProfileStatus = false;

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
 *      DPM Execution Task which executes the DPM Instance which manages the
 *      LL Profile
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

    /* Sanity Checking: Ensure that the result has passed through the HL DPC module
     * The Raw Data results have been validated already on the DSS where the LL DPC
     * profile is executing */
    if (strstr ((char*)ptrResult->ptrBuffer[0], "HL Processing [GTrack") != NULL)
    {
        /* Increment the number of successful validations: */
        gDPMTestMCB.validationCounter++;
        return 0;
    }

    /* No match: */
    test_print ("Result String: '%s'\n", (char*)ptrResult->ptrBuffer[0]);
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

        /* Do we have results which are available and is the DPC active? */
        if ((result.size[0] != 0U) && (gDPMTestMCB.hlProfileStatus == true))
        {
            /* Sanity Checking: Validate the received results */
            if (DPMTest_validateResults (&result) < 0)
            {
                MCPI_setFeatureTestResult ("DPM Validate Results", MCPI_TestResult_FAIL);
            }

            /* Reset the buffer for the next iteration */
            memset ((void *)result.ptrBuffer[0], 0, result.size[0]);
        }
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
    fftcCfg.dummy1 = 10;
    fftcCfg.dummy2 = 20;

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
    peakGroupingCfg.dummy1 = 30;

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
    gtrackCfg.dummy1 = 40;
    gtrackCfg.dummy2 = 50;
    gtrackCfg.dummy3 = 60;

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

    /* Start the LL Data Path Profile: */
    errCode = DPM_start (gDPMTestMCB.llDPMHandle);
    if (errCode < 0)
    {
        test_print ("Error: Unable to start the LL Profile [Error code %d]\n", errCode);
        goto exit;
    }

    /* Start the HL Data Path Profile: */
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

    /* Stop the Distributed LL Data Path Profile: */
    errCode = DPM_stop (gDPMTestMCB.llDPMHandle);
    if (errCode < 0)
    {
        test_print ("Error: Unable to Stop the LL Profile [Error code %d]\n", errCode);
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
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_mssInitTask(void* args)
{
    int32_t             errCode;
    DPM_InitCfg         dpmInitCfg;
    uint8_t             numLoops       = 0U;
    bool                testCompletion = false;
    /**
    * The following registers are used to control the data injection on the DSS
    * This is controlled by the MSS.
    * MSS_CTRL:MSS_BOOT_INFO_REG6  
    * MSS_CTRL:MSS_BOOT_INFO_REG7
    */
    volatile CSL_mss_ctrlRegs *mssCtrl  = (CSL_mss_ctrlRegs*) CSL_MSS_CTRL_U_BASE;

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
     * DPM Instance1:
     * - The DPM Instance is responsible for executing the LL Profile. The profile
     *   is executing on the DSS and the MSS is controlling this profile. This is
     *   the reason why we dont setup the profile function table
     *****************************************************************************/
    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Setup the configuration: */
    dpmInitCfg.ptrProcChainCfg = NULL;
    dpmInitCfg.instanceId    = LL_MSS_DPM_INSTANCE_ID;
    dpmInitCfg.domain        = DPM_Domain_REMOTE;
    dpmInitCfg.reportFxn     = DPMTest_llReportFxn;

    /* Initialize the DPM Module: */
    gDPMTestMCB.llDPMHandle = DPM_init (&dpmInitCfg, &errCode);
    if (gDPMTestMCB.llDPMHandle == NULL)
    {
        test_print ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        MCPI_setFeatureTestResult ("DPM Remote Domain Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("DPM Remote Domain Initialization", MCPI_TestResult_PASS);

    /* Launch the DPM Task:
     * - This is always a lower priority than the mmWave control task because
     *   control events should always be higher priority. */
    gDpmTestTask = xTaskCreateStatic( DPMTest_llDPMTask,   /* Pointer to the function that implements the task. */
                   "test_dpmLLtest_task",   /* Text name for the task.  This is to facilitate debugging only. */
                   APP_DPM_TEST_TASK_STACK_SIZE, /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                   NULL,              /* We are not using the task parameter. */
                   APP_DPM_TEST_TASK_PRI, /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                   gDpmTestLlTskStack,  /* pointer to stack base */
                   &gDpmTestTaskObj );/* pointer to statically allocated task object memory */

    configASSERT(gDpmTestTask != NULL);

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
            MCPI_setFeatureTestResult ("DPM Remote Domain Synchronization", MCPI_TestResult_FAIL);
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
    MCPI_setFeatureTestResult ("DPM Remote Domain Synchronization", MCPI_TestResult_PASS);

     ClockP_usleep(100 * 1000);

    /*****************************************************************************
     * DPM Instance2:
     * - The DPM Instance is responsible for executing the HL Profiles on the
     *   MSS only so this is a single domain.
     *****************************************************************************/
    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Setup the configuration: */
    dpmInitCfg.domain        = DPM_Domain_LOCALIZED;
    dpmInitCfg.ptrProcChainCfg = &gHLProfileCfg;
    dpmInitCfg.instanceId    = HL_DPM_INSTANCE_ID;
    dpmInitCfg.reportFxn     = DPMTest_hlReportFxn;

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
    gDpmTestHlTask = xTaskCreateStatic( DPMTest_hlDPMTask,   /* Pointer to the function that implements the task. */
                    "test_dpmHLtest_task",   /* Text name for the task.  This is to facilitate debugging only. */
                    APP_DPM_TEST_HL_TASK_STACK_SIZE, /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                    NULL,              /* We are not using the task parameter. */
                    APP_DPM_TEST_HL_TASK_PRI, /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                    gDpmTestHlTskStack,  /* pointer to stack base */
                    &gDpmTestHlTaskObj );/* pointer to statically allocated task object memory */

    configASSERT(gDpmTestHlTask != NULL);

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
        ClockP_usleep(100 * 1000);

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
        while (gDPMTestMCB.llProfileStatus == false);
        while (gDPMTestMCB.hlProfileStatus == false);

        /* The data is being injected on the DSS; so we have a simple state machine
         * on a global shared variable between the MSS & DSS. This is analogous to the
         * sensor start; wait for the DSS to acknowledge the request */
        mssCtrl->MSS_BOOT_INFO_REG6 = 1U;
        mssCtrl->MSS_BOOT_INFO_REG7 = 0U;
        while (mssCtrl->MSS_BOOT_INFO_REG7 == 0U);

        /* Wait for some time and then stop the profiles. */
        ClockP_usleep(10000 * 1000);

        /* Stop the Data Injection: Wait for the acknowledgment. */
        mssCtrl->MSS_BOOT_INFO_REG6 = 1U;
        mssCtrl->MSS_BOOT_INFO_REG7 = 0U;
        while (mssCtrl->MSS_BOOT_INFO_REG7 == 0U);

        /* Stop the DPC Profiles: */
        if (DPMTest_stopProfiles () < 0)
        {
            /* Error: Unable to stop the DPC. */
            testCompletion = true;
            MCPI_setFeatureTestResult ("DPM Stop", MCPI_TestResult_FAIL);
            continue;
        }
        MCPI_setFeatureTestResult ("DPM Stop", MCPI_TestResult_PASS);

        /* Synchronization: Before we start & reconfigure the profiles ensure that these
         * have been stopped properly; especially since the LL profile is executing on
         * another domain */
        while (gDPMTestMCB.llProfileStatus == true);
        while (gDPMTestMCB.hlProfileStatus == true);

        /* Increment the number of loops: */
        numLoops = numLoops + 1U;

        /* Did we reach the max threshold? */
        if (numLoops == TEST_MAX_ITERATIONS)
        {
            testCompletion = true;
            continue;
        }
    }

    /* Indicate DSS core that test is complete. */
    mssCtrl->MSS_BOOT_INFO_REG7 = 87655678U;
    test_print ("Debug: DPM Remote Unit Testing completed\n");
    MCPI_setTestResult ();

    test_print ("All tests have passed!!\n");

    vTaskDelete(NULL);

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
