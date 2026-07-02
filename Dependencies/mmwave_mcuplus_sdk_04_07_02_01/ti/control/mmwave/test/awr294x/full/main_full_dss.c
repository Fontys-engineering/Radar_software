/*
 *   @file  main_full_dss.c
 *
 *   @brief
 *      Unit Test code for the mmWave 
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
#include <c6x.h>

/* mmWave PDK Include Files: */
#include <ti/control/mmwave/test/awr294x/full/dssgenerated/ti_drivers_config.h>
#include <ti/control/mmwave/test/awr294x/full/dssgenerated/ti_board_config.h>
#include <ti/control/mmwave/test/awr294x/full/dssgenerated/ti_drivers_open_close.h>
#include <ti/control/mmwave/test/awr294x/full/dssgenerated/ti_board_open_close.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/utils/testlogger/logger.h>

#define APP_TASK_PRI         (5U)
#define APP_CTRL_TASK_PRI    (7U)
#define APP_TASK_STACK_SIZE  (8*1024U)
#define APP_CTRL_TASK_STACK_SIZE (6*1024U)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

TaskHandle_t    gMmwCtrlTask;
StaticTask_t    gMmwCtrlTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));
StackType_t gMmwCtrlTskStack[APP_CTRL_TASK_STACK_SIZE] __attribute__((aligned(32)));

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/
/* Global Variable: This is the handle to the mmWave module */
MMWave_Handle   gMMWaveHandle;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(9216);


/**************************************************************************
 ************************** Extern Definitions ****************************
 **************************************************************************/
extern void Mmwave_populateDefaultOpenCfg (MMWave_OpenCfg* ptrOpenCfg);
extern void Mmwave_populateDefaultChirpControlCfg (MMWave_CtrlCfg* ptrCtrlCfg);
extern void Mmwave_populateDefaultAdvancedControlCfg (MMWave_CtrlCfg* ptrCtrlCfg);
extern void Mmwave_populateDefaultCalibrationCfg (MMWave_CalibrationCfg* ptrCalibrationCfg, MMWave_DFEDataOutputMode dfeOutputMode);
extern int32_t Mmwave_eventFxn (uint8_t devIndex, uint16_t msgId, uint16_t sbId, uint16_t sbLen, uint8_t *payload);
extern void Mmwave_ctrlTask(void* args);

/**************************************************************************
 *********************** mmWave Unit Test Functions ***********************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      The function implements the mmwave test
 *
 *  @retval
 *      Not Applicable.
 */
void Mmwave_Test (void)
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

    /***********************************************************************
     * : DSS executes the mmWave link
     ***********************************************************************/
    initCfg.domain                  = MMWave_Domain_DSS;
    initCfg.eventFxn                = Mmwave_eventFxn;
    initCfg.linkCRCCfg.useCRCDriver = 1U;
    initCfg.linkCRCCfg.crcBaseAddr  = (uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR);
    initCfg.linkCRCCfg.crcChannel   = CRC_CHANNEL_1;
    initCfg.cfgMode                 = MMWave_ConfigurationMode_FULL;

    /* Initialize and setup the mmWave Control module */
    gMMWaveHandle = MMWave_init (&initCfg, &errCode);
    if (gMMWaveHandle == NULL)
    {
        /* Error: Unable to initialize the mmWave control module */
        MMWave_decodeError (errCode, &errorLevel, &mmWaveErrorCode, &subsysErrorCode);

        /* Debug Message: */
        test_print ("Error Level: %s mmWave: %d Subsys: %d\n",
                       (errorLevel == MMWave_ErrorLevel_ERROR) ? "Error" : "Warning",
                       mmWaveErrorCode, subsysErrorCode);

        /* Log into the MCPI Test Logger: */
        MCPI_setFeatureTestResult ("MMWave DSS Initialization", MCPI_TestResult_FAIL);
        return;
    }
    
    test_print ("MMWave DSS Initialization\n");

    /*****************************************************************************
     * Launch the mmWave control execution task
     * - This should have a higher priroity than any other task which uses the
     *   mmWave control API
     *****************************************************************************/
    gMmwCtrlTask = xTaskCreateStatic( Mmwave_ctrlTask,   /* Pointer to the function that implements the task. */
                                      "test_ctrl_task",   /* Text name for the task.  This is to facilitate debugging only. */
                                      APP_CTRL_TASK_STACK_SIZE, /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                      NULL,              /* We are not using the task parameter. */
                                      APP_CTRL_TASK_PRI, /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                      gMmwCtrlTskStack,  /* pointer to stack base */
                                      &gMmwCtrlTaskObj );/* pointer to statically allocated task object memory */

    configASSERT(gMmwCtrlTask != NULL);


    /***********************************************************************
     * : DSS executes the mmWave link
     ***********************************************************************/
    Mmwave_populateDefaultOpenCfg (&openCfg);
    Mmwave_populateDefaultChirpControlCfg (&ctrlCfg);
    
    /* Override the default configuration and ensure that the DSS is the default
     * asynchronous event handler */
    openCfg.defaultAsyncEventHandler = MMWave_DefaultAsyncEventHandler_DSS;

    /************************************************************************
     * Open the mmWave:
     ************************************************************************/
    if (MMWave_open (gMMWaveHandle, &openCfg, NULL, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        test_print ("Error: mmWave open failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave DSS Open", MCPI_TestResult_FAIL);
        return;
    }
   
    test_print ("MMWave DSS Open done.\n");

    /************************************************************************
     * Configure the mmWave:
     ************************************************************************/
    if (MMWave_config (gMMWaveHandle, &ctrlCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        test_print ("Error: mmWave configuration failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave DSS Configuration", MCPI_TestResult_FAIL);
        return;
    }
    
    test_print ("MMWave DSS Configuration done\n");

    /* Populate the calibration configuration: */
    memset ((void *)&calibrationCfg, 0, sizeof(MMWave_CalibrationCfg));
    Mmwave_populateDefaultCalibrationCfg (&calibrationCfg, MMWave_DFEDataOutputMode_FRAME);

    /************************************************************************
     * Start the mmWave:
     ************************************************************************/
    if (MMWave_start (gMMWaveHandle, &calibrationCfg, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        test_print ("Error: mmWave start failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave DSS Start", MCPI_TestResult_FAIL);
        return;
    }
   
    DebugP_log  ("MMWave DSS Start done\n");
    
    ClockP_usleep(25000 * 1000);
    /************************************************************************
     * Stop the mmWave:
     ************************************************************************/
    retVal = MMWave_stop (gMMWaveHandle, &errCode);
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
            MCPI_setFeatureTestResult ("MMWave DSS Stop", MCPI_TestResult_FAIL);
            return;
        }
        else
        {
            /* Informational Level: The test has passed. Fall through...*/
        }
    }
    test_print ("MMWave DSS Stop done.\n");

    /************************************************************************
     * Close the mmWave:
     ************************************************************************/
    if (MMWave_close (gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to configure the mmWave control module */
        test_print ("Error: mmWave close failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Close", MCPI_TestResult_PASS);
    DebugP_log ("MMWave DSS close done.\n");
    
    /************************************************************************
     * Deinitialize the mmWave module:
     ************************************************************************/
    if (MMWave_deinit(gMMWaveHandle, &errCode) < 0)
    {
        /* Error: Unable to deinitialize the mmWave control module */
        test_print ("Error: mmWave Deinitialization failed [Error code %d]\n", errCode);
        MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_FAIL);
        return;
    }
    MCPI_setFeatureTestResult ("MMWave DSS Deinitialized", MCPI_TestResult_PASS);
    
    return;
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
 void Mmwave_initTask(void* args)
{
    /* Debug Message: */
    test_print ("******************************************\n");
    test_print ("Debug: Launching mmwave\n");
    test_print ("******************************************\n");

    Drivers_open();
    Board_driversOpen();

    Mmwave_Test();

    Board_driversClose();
    Drivers_close();

    test_print ("--- Test Completed ---\n");
    test_print ("All tests have passed!!\n");

    vTaskDelete(NULL);
    return;
}

/**
 *  @b Description
 *  @n
 *      This is the entry point into the unit test code
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
    gAppTask = xTaskCreateStatic( Mmwave_initTask,   /* Pointer to the function that implements the task. */
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


