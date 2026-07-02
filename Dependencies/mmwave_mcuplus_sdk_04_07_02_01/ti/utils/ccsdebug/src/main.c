/*
 * Copyright (c) 2016-24, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 /* ============================================================================
  */
 /**
 *   @file  main.c
 *
 *   @brief
 *      CCS Debug Utility application code
 */

 /** @mainpage CCS Debug Utility
 *
 *      CCS Debug Utility: This utility can be flashed on the QSPI.
 *      The application is written over FreeRTOS (MSS), FreeRTOS (DSP), NoRTOS (DSS_CM4) and will loop forever.
 *      In the meantime CCS can be attached and the developers can then download the real
 *      application which needs to be debugged.
 *
 *      This utility provides while loop application for :\n
 *      MSS and DSS in AWR294X\n
 *      MSS in AWR2544\n
 *      MSS DSS and DSS_CM4 in AWR2x44P/AWR2x44ECO
 *      MSS and DSS_CM4 in AWR2x44LC
 *
 */
/* ============================================================================
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

#include <ti/common/syscommon.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/DebugP.h>

#ifndef SUBSYS_M4
#include "FreeRTOS.h"
#include "task.h"
#endif

#ifdef SUBSYS_MSS
#include <ti/utils/ccsdebug/mssgenerated/ti_drivers_config.h>
#include <ti/utils/ccsdebug/mssgenerated/ti_board_config.h>
#include <ti/utils/ccsdebug/mssgenerated/ti_drivers_open_close.h>
#include <ti/utils/ccsdebug/mssgenerated/ti_board_open_close.h>
#endif

#ifdef SUBSYS_M4
#include <ti/utils/ccsdebug/m4generated/ti_drivers_config.h>
#include <ti/utils/ccsdebug/m4generated/ti_board_config.h>
#include <ti/utils/ccsdebug/m4generated/ti_drivers_open_close.h>
#include <ti/utils/ccsdebug/m4generated/ti_board_open_close.h>
#endif

#ifdef SUBSYS_DSS
#include <ti/utils/ccsdebug/dssgenerated/ti_drivers_config.h>
#include <ti/utils/ccsdebug/dssgenerated/ti_board_config.h>
#include <ti/utils/ccsdebug/dssgenerated/ti_drivers_open_close.h>
#include <ti/utils/ccsdebug/dssgenerated/ti_board_open_close.h>
#endif

#ifndef SUBSYS_M4
#define APP_TASK_PRI         (5U)
#define APP_TASK_STACK_SIZE  (2*1024U)

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/
/* FreeRTOS Task variables */
TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;
StackType_t     gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));
#endif

/* Global Variable which can be used to halt the debugger on startup*/
volatile int32_t gDebug = 0;

static void CcsDebug_initTask(void* args)
{
    Drivers_open();
    Board_driversOpen();

    DebugP_log  ("CCSDEBUG: Application up and running!\r\n");
    while (gDebug == 0)
    {
        ;
    }

    Board_driversClose();
    Drivers_close();

    return;
}

/**
 *  @b Description
 *  @n
 *      Entry point into the CCS Debug Utility. The function does some system
 *      initialization and then simply causes the execution to be blocked forever.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

#ifdef SUBSYS_M4
    CcsDebug_initTask(NULL);
#else
    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( CcsDebug_initTask,
                                  "CcsDebug_task_main",
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
#endif
}
