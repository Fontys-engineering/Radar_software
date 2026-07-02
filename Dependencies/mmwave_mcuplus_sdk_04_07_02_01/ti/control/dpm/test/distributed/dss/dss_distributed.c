/*
 *   @file  dss_distributed.c
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
#include <ti/control/dpm/test/distributed/dss/dssgenerated/ti_drivers_config.h>
#include <ti/control/dpm/test/distributed/dss/dssgenerated/ti_board_config.h>
#include <ti/control/dpm/test/distributed/dss/dssgenerated/ti_drivers_open_close.h>
#include <ti/control/dpm/test/distributed/dss/dssgenerated/ti_board_open_close.h>
#include <kernel/dpl/ClockP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmWave SDK Include Files. */
#include <ti/common/syscommon.h>
#include <ti/control/dpm/dpm.h>
#include <ti/common/mmwavesdk_version.h>
#include <ti/utils/testlogger/logger.h>

/* Demo Include Files: */
#include <ti/control/dpm/test/distributed/dpc/ll/ll_common_profile.h>

#define APP_TASK_PRI                    (2U)
#define DPM_TASK_PRI                    (5U)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

TaskHandle_t    gDpmTask;
StaticTask_t    gDpmTaskObj;

#define APP_TSK_STACK_MAIN              (3U * 1024U)
#define DPM_TSK_STACK                   (4U * 1024U)

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
     * executing the ML Data Path Profile
     */
    DPM_Handle          mlDPMHandle;
}DPMTest_MCB;

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
StackType_t gAppTskStackMain[APP_TSK_STACK_MAIN] __attribute__((aligned(32)));
StackType_t gDpmTskStackMain[DPM_TSK_STACK] __attribute__((aligned(32)));

/**
 * @brief   DSS Unit Test MCB
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

/* Unit Test DSS Task(s) */
static void DPMTest_dssInitTask(void* args);
static void DPMTest_dpmTask(void* args);

/* DPM Reporting API: */
static void DPMTest_reportFxn(DPM_Report reportType, uint32_t instanceId, int32_t errCode, uint32_t arg0, uint32_t arg1);

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
static void DPMTest_reportFxn
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
             * - This implies that the mmWave module can now inform the BSS to
             *   start the sensor. Once this is done the chirps will end up
             *   being received on the profile.
             *****************************************************************/
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
            break;
        }
    }
    return;
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
        errCode = DPM_execute (gDPMTestMCB.mlDPMHandle, &result);
        if (errCode < 0)
        {
            test_print ("Error: DPM execution failed [Error code %d]\n", errCode);
            MCPI_setFeatureTestResult ("DPM Execute", MCPI_TestResult_FAIL);
        }

        /* Do we have results which are available? */
        if (result.size[0] != 0U)
        {
            /* YES: Results are available send them. */
            errCode = DPM_sendResult (gDPMTestMCB.mlDPMHandle, false, &result);
            if (errCode < 0)
            {
                test_print ("Error: ML Profile failed to send results [Error: %d]\n", errCode);
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
    uint32_t            initArg;

    /*****************************************************************************
     * Initialize the mmWave SDK components:
     *****************************************************************************/
    Drivers_open();
    Board_driversOpen();

    /* Initialize the MCPI framework: */
    MCPI_Initialize ();

    /*****************************************************************************
     * Initialization of the DPM Module:
     *****************************************************************************/
    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Initialize the ML Processing Init Argument: */
    initArg = ML_INIT_ARGUMENT;

    /* Setup the configuration: */
    dpmInitCfg.domain           = DPM_Domain_DISTRIBUTED;
    dpmInitCfg.arg              = &initArg;
    dpmInitCfg.argSize          = sizeof(uint32_t);
    dpmInitCfg.ptrProcChainCfg  = &gMLProfileCfg;
    dpmInitCfg.instanceId       = ML_DPM_INSTANCE_ID;
    dpmInitCfg.reportFxn        = DPMTest_reportFxn;

    /* Initialize the DPM Module: */
    gDPMTestMCB.mlDPMHandle = DPM_init (&dpmInitCfg, &errCode);
    if (gDPMTestMCB.mlDPMHandle == NULL)
    {
        test_print ("Error: Unable to initialize the DPM Module [Error: %d]\n", errCode);
        MCPI_setFeatureTestResult ("DPM Distributed Domain Initialization", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("DPM Distributed Domain Initialization", MCPI_TestResult_PASS);

    /* Launch the DPM Task:
     * - This is always a lower priority than the mmWave control task because
     *   control events should always be higher priority. */
    gDpmTask = xTaskCreateStatic( DPMTest_dpmTask,   /* Pointer to the function that implements the task. */
                                  "test_dpm_task", /* Text name for the task.  This is to facilitate debugging only. */
                                  DPM_TSK_STACK,  /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                  NULL,              /* We are not using the task parameter. */
                                  DPM_TASK_PRI,      /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                  gDpmTskStackMain,  /* pointer to stack base */
                                  &gDpmTaskObj );    /* pointer to statically allocated task object memory */
    configASSERT(gDpmTask != NULL);

    /* Synchronization: This will synchronize the execution of the control module
     * between the domains. This is a prerequiste and always needs to be invoked. */
    while (1)
    {
        int32_t syncStatus;

        /* Get the synchronization status: */
        syncStatus = DPM_synch (gDPMTestMCB.mlDPMHandle, &errCode);
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
        ClockP_usleep(1 * 1000U);
    }
    MCPI_setFeatureTestResult ("DPM Distributed Domain Synchronization", MCPI_TestResult_PASS);

    while(1);

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

