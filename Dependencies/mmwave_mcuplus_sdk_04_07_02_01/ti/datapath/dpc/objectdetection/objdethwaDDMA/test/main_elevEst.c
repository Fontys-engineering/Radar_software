/**
 *   @file  main_elevEst.c
 *
 *   @brief
 *      Unit Test code for the object detection DDMA DPC.
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
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "FreeRTOS.h"
#include "task.h"
#include <kernel/dpl/TaskP.h>
#include <kernel/dpl/SemaphoreP.h>

/* MCU Plus Include Files. */
#ifdef SUBSYS_DSS
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/test/dssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/test/dssgenerated/ti_board_config.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/test/dssgenerated/ti_board_open_close.h>
#endif

#ifdef SUBSYS_MSS
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/test/mssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/test/mssgenerated/ti_board_config.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/test/mssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/test/mssgenerated/ti_board_open_close.h>
#endif

#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/objectdetection.h>
#ifdef SOC_AWR2X44P
#include <ti/demo/awr2x44P/mmw_ddm/mmw_common.c>
#endif
#include <ti/common/syscommon.h>
#include <ti/utils/mathutils/mathutils.h>

#define APP_TASK_PRI         (1U)
#define APP_TASK_STACK_SIZE  (2*1024U)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(64)));


uint32_t gCnt = 0;
uint64_t gWaitCnt = 0;
DPC_ObjectDetection_ElevEstCfg xyzEstCfg = {0};
SemaphoreP_Object   dssSemHandle;

static void MmwDemo_DPC_ObjectDetection_reportFxn (void *arg, uint16_t dataLen)
{
    // int32_t     errCode;
    gCnt++;
    static uint32_t subFrameCnt = RL_MAX_SUBFRAMES;

    if(dataLen == sizeof(DPC_ObjectDetection_ElevEstCommonCfg))
    {
        xyzEstCfg.commonCfg = *(DPC_ObjectDetection_ElevEstCommonCfg*)arg;
        subFrameCnt = xyzEstCfg.commonCfg.numSubFrames;
    }
    else if (dataLen == sizeof(DPC_ObjectDetection_ElevEstSubframeCfg))
    {
        /* Subframe count is run from numSubFrames to 0, according to the order of config sent */
        xyzEstCfg.subframeCfg[--subFrameCnt] = *(DPC_ObjectDetection_ElevEstSubframeCfg *)(arg);
    }
    else if(dataLen == sizeof(uint32_t))
    {
        /* start the execution */
        SemaphoreP_post (&dssSemHandle);
    }
    return;
}



volatile uint64_t gAoAProcTime = 0;
void elevationEstimation_Test_task(void* args)
{
    int32_t retVal;
    DPM_Handle dpmHandle;
    DPM_InitCfg         dpmInitCfg;

    CycleCounterP_reset();

    retVal = SemaphoreP_constructBinary(&dssSemHandle, 0);
    DebugP_assert(retVal==SystemP_SUCCESS);

    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

    /* Setup the configuration: */
#ifdef SUBSYS_DSS
    dpmInitCfg.localEndPt  = gRemoteCoreEndPt[CSL_CORE_ID_C66SS0];
    dpmInitCfg.setBitPos   = DPM_DSS_BOOT_INFO_BIT_POS;
#elif defined SUBSYS_MSS
    dpmInitCfg.localEndPt  = gRemoteCoreEndPt[CSL_CORE_ID_R5FSS0_0];
    dpmInitCfg.setBitPos   = DPM_MSS_BOOT_INFO_BIT_POS;
#endif
    dpmInitCfg.reportFxn   = MmwDemo_DPC_ObjectDetection_reportFxn;


    /* Initialize the DPM Module: */
    dpmHandle = DPM_init (&dpmInitCfg, &retVal);
    if (dpmHandle == NULL)
    {
        test_print ("Error: Unable to initialize the DPM Module [Error: %d]\n", retVal);
        DebugP_assert (0);
        return;
    }

    while (1)
    {
        int32_t syncStatus;
        int32_t syncStatusRefVal = (uint32_t)1U << DPM_DSS_CM4_BOOT_INFO_BIT_POS;
#if MSS_AOA_ENABLED
        syncStatusRefVal += (uint32_t)1U << DPM_MSS_BOOT_INFO_BIT_POS;
#else
        syncStatusRefVal += (uint32_t)1U << DPM_DSS_BOOT_INFO_BIT_POS;
#endif
        /* Get the synchronization status: */
        syncStatus = DPM_synch (dpmHandle, &retVal);
        if (syncStatus < 0)
        {
            /* Error: Unable to synchronize the framework */
            DebugP_log ("Error: DPM Synchronization failed [Error code %d]\n", retVal);
            DebugP_assert (0);
            return;
        }
        if (syncStatus == syncStatusRefVal)
        {
            /* Synchronization acheived: */
            break;
        }
        /* Sleep and poll again: */
        ClockP_usleep(1 * 1000U);
    }

    while(1)
    {
        /* wait for getting the EXECUTE NOTIF */
        SemaphoreP_pend(&dssSemHandle, SystemP_WAIT_FOREVER);

        DPC_ObjectDetection_ExecuteResult *result = xyzEstCfg.commonCfg.result;
        uint64_t startTime = CycleCounterP_getCount32();
        DPC_ObjDet_estimateXYZ(&xyzEstCfg,  (DetObjParams*)SOC_phyToVirt((uint32_t)result->detObjList),
                                    result->objOut,
                                    result->dopNumObjOut,
                                    &result->numObjOut);
        gAoAProcTime = CycleCounterP_getCount32() - startTime;

        /* Execution done - send the notofication */
        uint32_t msg = MMWDEMO_DPC_RESULT;
        retVal = DPM_send(dpmHandle, (void*)(&msg), sizeof(uint32_t), CSL_CORE_ID_M4SS0_1);
        DebugP_assert(0U == retVal);
    }
}

int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();


    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( elevationEstimation_Test_task,
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
