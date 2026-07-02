/*
 *   @file  main_mss.c
 *
 *   @brief
 *      This is the mmWave link unit test which executes on the R5 on
 *      the XWR29xx.
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

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

/* mmWave PDK Include Files: */
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/HwiP.h>
#include "mssgenerated/ti_drivers_config.h"
#include "mssgenerated/ti_board_config.h"
#include "mssgenerated/ti_drivers_open_close.h"
#include "mssgenerated/ti_board_open_close.h"
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"
#include <drivers/gpio.h>
#include <drivers/mailbox.h>
#include <drivers/crc.h>

#include <drivers/adcbuf.h>
#include <drivers/edma.h>

#include <ti/common/syscommon.h>
#include <ti/control/mmwavelink/mmwavelink.h>

#include <ti/control/mmwave/link_test/common/link_testcase_setting.h>
#include <ti/control/mmwave/link_test/common/mmwavelink_test_common.h>

/* Macros to extract APLL Calibration status. */
#define APLL_CAL_STATUS_MASK                          (0x00020000U)
#define APLL_CAL_STATUS_SHIFT                         (0x00000011U)

#define APP_TASK_STACK_SIZE    (10*1024U)
/* FreeRTOS Task declarations. */
#define APP_TASK_PRI           (1U)
#define MMWDEMO_MMWAVE_CTRL_TASK_STACK_SIZE (8*1024U)

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/
StackType_t gAppTskStackMain[MMWDEMO_MMWAVE_CTRL_TASK_STACK_SIZE] __attribute__((aligned(32)));

TaskHandle_t gAppTask;

/*RF Monitoring test*/
bool gLinkMonitoringTest = false;

/*RF calibration test*/
bool gLinkCalibTest      = false;

/* RF Advance frame test
   Note: If gLinkContModeTest is enabled then gLinkMonitoringTest and gLinkCalibTest
   must be disabled
*/
/*RF Advance frame test*/
bool gLinkAdvanceFrameTest  = false;

/* CQ2 data verification test
    Here is the the flow to verify the CQ2 RAM data
   1. At start of each chirp in a frame application triggers eDMA to transfer CQ2
   data of single chirp to L3 memory.
   2. CQ2 data verification is not covered in this application, but user can verify this by putting another
      EVM running countinous mode of frequency range which cause interference on EVM under test. In case
      of any interference recieved on EVM under test, CQ2 data will have non-zero values.
*/
/* enable this flag to verify CQ data in this application */
bool cqDataVerifyTest = false;

/*Continous mode test*/
bool gLinkContModeTest  = false;

/* Dynamic Chirp configuration test */
bool gDynamicChirpTest  = false;

/* Advance Chirp configuration test */
bool gAdvChirpTest = false;

/*Binary phase modulation test*/
bool gLinkBpmEnableTest  = false;

/*RF program filter test*/
bool gLinkProgFiltTest  = false;

/* Sub frame trigger test. Valid when gLinkAdvanceFrameTest flag is enabled */
bool gLinkSubFrameTrigTest = false;

/*To enable multi chirp config*/
bool multichirpen=false;

/*To enable the test source*/
bool testsrcen=false;

/* To enable analog fault injection test */
bool gAnalogFaultInjTest = false;

/* To enable digital latent fault test */
bool gDigLatentFaultTest = false;

/*frame interrupt Semaphore  */
SemaphoreP_Object gLinkFrameSem;


/*! @brief   ADCBUF handle */
ADCBuf_Handle             adcbufHandle;

/**************************************************************************
 ************************* mmWave Link Test Functions *********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
static void MmwaveLink_initTask(void* arg0)
{
    SemaphoreP_Object             demoInitTaskCompleteSemHandle;
    /* Debug Message: */
    printf("Debug: Launched the mmwaveLink Initialization Task\n");

    /* All mailbox driver init happends within mmwavelink communication callback */
    SemaphoreP_constructBinary(&gLinkFrameSem, 0);

    MmwaveLink_startTest(RL_AR_DEVICETYPE_2544, RL_PLATFORM_MSS);

    /* Debug Message: */
    printf ("******************************************\n");
    printf ("Debug: MmwaveLink Test completed\n");
    printf ("******************************************\n");

    /* Create binary semaphore to pend Main task, */
    SemaphoreP_constructBinary(&demoInitTaskCompleteSemHandle, 0);

    /* Never return for this task. */
    SemaphoreP_pend(&demoInitTaskCompleteSemHandle, SystemP_WAIT_FOREVER);

    /* The following line should never be reached. */
    DebugP_assertNoLog(0);
}


StaticTask_t    gAppTaskObj;
HwiP_Object     hwiFrameStartHwiObject;

/**
 *  @b Description
 *  @n
 *      Entry point into the mmWave Link Unit Test
 *
 *  @retval
 *      Not Applicable.
 */
int32_t main (void)
{
    int32_t           retVal;
    HwiP_Params       hwiPrms;
    ADCBuf_Params     ADCBufparams;
    int32_t           timeout = 1000;

    /* init SOC specific modules */
    System_init();
    Board_init();
    Drivers_open();

    /*****************************************************************************
     * Initialize ADCBUF driver
     *****************************************************************************/
#if defined(SOC_AWR2544)
    ADCBuf_init(timeout);
#else
   ADCBuf_init();
#endif

    /* ADCBUF Params initialize */
    ADCBuf_Params_init(&ADCBufparams);
    ADCBufparams.chirpThresholdPing = 1;
    ADCBufparams.chirpThresholdPong = 1;
    ADCBufparams.continousMode  = 0;

    adcbufHandle = ADCBuf_open(0, &ADCBufparams);
    if (adcbufHandle == NULL)
    {
        printf("Error: Unable to open the ADCBUF Instance\n");
        return -1;
    }
    printf("Debug: ADCBUF Instance %p has been reopened successfully\n", adcbufHandle);


    /* Register chirp interrupt */
    HwiP_Params_init(&hwiPrms);
    hwiPrms.isPulse     =1;
    hwiPrms.intNum      = CSL_MSS_INTR_DFE_CHIRP_CYCLE_START;
    hwiPrms.callback    = &Mmwavelink_chirpAvailInterrupCallBackFunc;
    hwiPrms.args        = (void *)NULL;

    if(SystemP_SUCCESS != HwiP_construct(&hwiChirpStartHwiObject, &hwiPrms))
    {
       retVal = -1;
    }
    else
    {
       HwiP_enableInt((uint32_t)CSL_MSS_INTR_DFE_CHIRP_CYCLE_START);
    }

    /* Register frame interrupt */
    HwiP_Params_init(&hwiPrms);
    hwiPrms.isPulse     =1;
    hwiPrms.intNum      = CSL_MSS_INTR_DFE_FRAME_START_TO_MSS;
    hwiPrms.callback    = &Mmwavelink_frameInterrupCallBackFunc;
    hwiPrms.args        = (void *)NULL;

    if(SystemP_SUCCESS != HwiP_construct(&hwiFrameStartHwiObject, &hwiPrms))
    {
       retVal = -1;
    }
    else
    {
       HwiP_enableInt((uint32_t)CSL_MSS_INTR_DFE_FRAME_START_TO_MSS);
    }

    /* Debug Message: */
    printf ("******************************************\n");
    printf ("Debug: Launching the mmwaveLink\n");
    printf ("******************************************\n");

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( MmwaveLink_initTask,
                                  "test_task_main",
                                  MMWDEMO_MMWAVE_CTRL_TASK_STACK_SIZE,
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

    return retVal;
}
