/*
 *   @file  dss_remote.c
 *
 *   @brief
 *      Unit Test for the DPM Framework which executes on the DSS
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
#include <ti/control/dpm/test/remote/dss/dssgenerated/ti_drivers_config.h>
#include <ti/control/dpm/test/remote/dss/dssgenerated/ti_board_config.h>
#include <ti/control/dpm/test/remote/dss/dssgenerated/ti_drivers_open_close.h>
#include <ti/control/dpm/test/remote/dss/dssgenerated/ti_board_open_close.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmWave SDK Include Files. */
#include <ti/common/syscommon.h>
#include <ti/common/mmwavesdk_version.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/control/dpm/test/remote/dpc/ll/ll_profile_remote.h>


#define APP_TASK_PRI                    (2U)
#define APP_DATA_INJECTION_TASK_PRI     (3U)
#define APP_DPM_TEST_TASK_PRI           (5U)

#define APP_TSK_STACK_MAIN              (3U * 1024U)
#define DATA_INJECTION_TSK_STACK        (4U * 1024U)
#define DPM_TSK_STACK                   (4U * 1024U)


TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

TaskHandle_t    gDpmTestTask;
StaticTask_t    gDpmTestTaskObj;

TaskHandle_t    gDataInjectionTask;
StaticTask_t    gDataInjectionTaskObj;

volatile uint32_t gdebug = 1;
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
     * @brief   Handle to the DPM Instance: This is responsible for
     * executing the LL Data Path Profile
     */
    DPM_Handle              llDPMHandle;

    /**
     * @brief   The LL Data Path Profile Status: Applications track the status since the
     * profile is distributed
     */
    volatile bool                    llProfileStatus;

    /**
     * @brief   Raw Data Counter: This is just used to create a unique raw data for every
     * iteration
     */
    volatile uint32_t       rawDataCounter;

    /**
     * @brief   This is the number of results which have been successfully validated
     * On the DSS we are executing only the LL Profile.
     */
    uint32_t                validationCounter;

    /**
     * @brief   Inject Data: This is input data which is being injected into the DPM Data
     * Path Profile.
     */
    char                    injectData[128];
}DPMTest_MCB;

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
StackType_t gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
StackType_t gDataInjectTskStack[DATA_INJECTION_TSK_STACK] __attribute__((aligned(32)));
StackType_t gDpmTskStack[DPM_TSK_STACK] __attribute__((aligned(32)));

/**
 * @brief   DSS Unit Test MCB
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

/* Unit Test DSS Task(s) */
static void DPMTest_dssInitTask(void* args);
static void DPMTest_dataInjectionTask(void* args);
static void DPMTest_dpmTask(void* args);

/* DPM Reporting API: */
static void DPMTest_reportFxn(DPM_Report reportType, uint32_t profileId, int32_t errCode, uint32_t arg0, uint32_t arg1);

/**************************************************************************
 ************************* Unit Test DSS Functions ************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      DPM Registered Report Handler. The DPM Module uses this registered function to notify
 *      the application about DPM reports.
 *
 *  @param[in]  reportType
 *      Report Type
 *  @param[in]  profileId
 *      Profile Identifier which generated the report
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
static void DPMTest_reportFxn
(
    DPM_Report  reportType,
    uint32_t    profileId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
)
{
    char    logString[128];

    /* Sanity Checking: */
    DebugP_assert ((profileId == LL_DSS_DPM_INSTANCE_ID) || (profileId == LL_MSS_DPM_INSTANCE_ID));

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
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * Profile has been started without an error:
             * - Setup the application status
             *****************************************************************/
            gDPMTestMCB.llProfileStatus = true;
            MCPI_setFeatureTestResult ("LL DPC Started", MCPI_TestResult_PASS);
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
            gDPMTestMCB.llProfileStatus = false;

            /* Create the MCPI Logging String: */
            sprintf (logString, "LL DPC Stopped Results validated=%d", gDPMTestMCB.validationCounter);
            MCPI_setFeatureTestResult (logString, MCPI_TestResult_PASS);

            /* Reset the counter: */
            gDPMTestMCB.validationCounter = 0U;
            break;
        }
    }
    return;
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
              "[Raw Data: %d]LL Processing [FFTC 10:20] [Peak Grouping: 30]",
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
 *      LL Profile Injection Task:The  Task periodically executes
 *      and injects data into the profile. This is to simulate the
 *      reception of raw data.
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_dataInjectionTask(void* args)
{
    DPM_Buffer      buffer;
    int32_t         errCode;
    volatile uint8_t         injectDataState;
    
    /**
    * The following registers are used to control the data injection on the DSS
    * This is controlled by the MSS.
    * MSS_CTRL:MSS_BOOT_INFO_REG6  
    * MSS_CTRL:MSS_BOOT_INFO_REG7
    */
    volatile CSL_mss_ctrlRegs *mssCtrl  = (CSL_mss_ctrlRegs*) CSL_MSS_CTRL_U_BASE;

    /* Loop around forever: */
    while (1)
    {
        /* Take a snapshot of the Inject Data State: */
        injectDataState = mssCtrl->MSS_BOOT_INFO_REG6;

        /* Do we need to inject the data? */
        if (injectDataState == 1U)
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
                printf ("Error: Unable to inject the data [Error code %d]\n", errCode);
                MCPI_setFeatureTestResult ("DPM Inject Data", MCPI_TestResult_FAIL);
            }
            else
            {
                mssCtrl->MSS_BOOT_INFO_REG6 = 0;
            }
        }

        /* Wait for some time: */
        ClockP_usleep(30 * 1000);

        /* Did the MSS change the state? */
        if (injectDataState != mssCtrl->MSS_BOOT_INFO_REG6)
        {
            /* YES: State Change Detected; so acknowledge it. */
            MCPI_setFeatureTestResult ("Injecting Data State Change Detected", MCPI_TestResult_PASS);
            mssCtrl->MSS_BOOT_INFO_REG7 = 1U;
        }
    }
}

/**
 *  @b Description
 *  @n
 *      DPM Execution Task
 *
 *  @retval
 *      Not Applicable.
 */
static void DPMTest_dpmTask(void* args)
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

        /* Do we have results which are available and is the DPC started? */
        if ((result.size[0] != 0U) && (gDPMTestMCB.llProfileStatus == true))
        {
            /* Sanity Checking: Validate the received results */
            if (DPMTest_validateResults (&result) < 0)
            {
                /* Error: Validation Failed. Log this error */
                MCPI_setFeatureTestResult ("DPM Validate Results", MCPI_TestResult_FAIL);
            }

            /* YES: Results are available send them. */
            errCode = DPM_sendResult (gDPMTestMCB.llDPMHandle, false, &result);
            if (errCode < 0)
            {
                test_print ("Error: LL Profile failed to send results [Error: %d]\n", errCode);
                MCPI_setFeatureTestResult ("DPM Send Result", MCPI_TestResult_FAIL);
            }
        }
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
static void DPMTest_dssInitTask (void* args)
{
    int32_t             errCode;
    DPM_InitCfg         dpmInitCfg;
    volatile CSL_mss_ctrlRegs *mssCtrl  = (CSL_mss_ctrlRegs*) CSL_MSS_CTRL_U_BASE;

    /*****************************************************************************
     * Initialize the mmWave SDK components:
     *****************************************************************************/
    Drivers_open();
    Board_driversOpen();

    /* Initialize and populate the demo MCB */
    memset ((void*)&gDPMTestMCB, 0, sizeof(DPMTest_MCB));

    /* Initialize the MCPI framework: */
    MCPI_Initialize ();

    /*****************************************************************************
     * Initialization of the DPM Module:
     *****************************************************************************/
    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Setup the configuration: */
    dpmInitCfg.domain          = DPM_Domain_REMOTE;
    dpmInitCfg.ptrProcChainCfg = &gLLProfileCfg;
    dpmInitCfg.instanceId      = LL_DSS_DPM_INSTANCE_ID;
    dpmInitCfg.reportFxn       = DPMTest_reportFxn;

    /* Initialize the DPM Module: */
    gDPMTestMCB.llDPMHandle = DPM_init (&dpmInitCfg, &errCode);
    if (gDPMTestMCB.llDPMHandle == NULL)
    {
        test_print ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        MCPI_setFeatureTestResult ("DPM Remote Domain Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("DPM Remote Domain Initialization", MCPI_TestResult_PASS);

    gDpmTestTask = xTaskCreateStatic( DPMTest_dpmTask,   /* Pointer to the function that implements the task. */
                   "test_dpmtest_task",   /* Text name for the task.  This is to facilitate debugging only. */
                   DPM_TSK_STACK, /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                   NULL,              /* We are not using the task parameter. */
                   APP_DPM_TEST_TASK_PRI, /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                   gDpmTskStack,  /* pointer to stack base */
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

    /* Launch the Data Simulation Task: */
    gDataInjectionTask = xTaskCreateStatic( DPMTest_dataInjectionTask,   /* Pointer to the function that implements the task. */
                        "test_datainjection_task",   /* Text name for the task.  This is to facilitate debugging only. */
                        DATA_INJECTION_TSK_STACK, /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                        NULL,              /* We are not using the task parameter. */
                        APP_DATA_INJECTION_TASK_PRI, /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                        gDataInjectTskStack,  /* pointer to stack base */
                        &gDataInjectionTaskObj );/* pointer to statically allocated task object memory */

    configASSERT(gDataInjectionTask != NULL);

    while(mssCtrl->MSS_BOOT_INFO_REG7 != 87655678U);

    vTaskDelete(NULL);

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
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
    gAppTask = xTaskCreateStatic( DPMTest_dssInitTask,   /* Pointer to the function that implements the task. */
                                  "test_task_main", /* Text name for the task.  This is to facilitate debugging only. */
                                  APP_TSK_STACK_MAIN,  /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
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

