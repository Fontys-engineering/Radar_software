/*
 *  Copyright (c) Texas Instruments Incorporated 2024
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* Standard Include Files. */
#include <string.h>
#include <stdio.h>

/* MCU+SDK Include Files: */
#include <ti/utils/test/mailbox_driver/awr2x44p/mssgenerated/ti_drivers_config.h>
#include <ti/utils/test/mailbox_driver/awr2x44p/mssgenerated/ti_board_config.h>
#include <ti/utils/test/mailbox_driver/awr2x44p/mssgenerated/ti_drivers_open_close.h>
#include <ti/utils/test/mailbox_driver/awr2x44p/mssgenerated/ti_board_open_close.h>
#include "FreeRTOS.h"
#include "task.h"

/* Mailbox Driver: */
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/common/syscommon.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (5U)
#define APP_TASK_STACK_SIZE  (2*1024U)

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void bssVersionTest(void);

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

typedef struct mboxRXType_t
{
    uint32_t sync;
    rlVersion_t data;
    uint16_t crc;
}mboxRXType;

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

StackType_t     gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));

static const uint16_t g_TestMessage_MSS[11] =
{
    0x1234, 0x4321, 0x0448, 0x0012,
    0x0000, 0x0000, 0x0001, 0xFBA4,
    0x0220, 0x0004, 0xBA39
};

mboxRXType gVersionData = {};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */
static void bssVersionTest(void)
{
    int32_t  status = SystemP_SUCCESS;
    uint32_t iterations = 10, index;

    for(index=0; index<iterations; index++)
    {
        status = Mailbox_write(
                    CSL_CORE_ID_RSS_R4,
                    (uint8_t*)g_TestMessage_MSS,
                    sizeof(g_TestMessage_MSS),
                    SystemP_WAIT_FOREVER
                );
        DebugP_assert(status==SystemP_SUCCESS);

        status = Mailbox_read(
                    CSL_CORE_ID_RSS_R4,
                    (uint8_t *)&gVersionData,
                    sizeof(mboxRXType),
                    SystemP_WAIT_FOREVER
                );
        DebugP_assert(status==SystemP_SUCCESS);

        status = Mailbox_readDone(CSL_CORE_ID_RSS_R4);
        DebugP_assert(status==SystemP_SUCCESS);

        test_print ("Test Iteration    : %d\n", (index + 1));
        test_print ("RF H/W Version    : %02d.%02d\n",
                        gVersionData.data.rf.hwMajor, gVersionData.data.rf.hwMinor);
        test_print ("RF F/W Version    : %02d.%02d.%02d.%02d (%02d.%02d.%02d)\n",
                        gVersionData.data.rf.fwMajor, gVersionData.data.rf.fwMinor,
                        gVersionData.data.rf.fwBuild, gVersionData.data.rf.fwDebug,
                        gVersionData.data.rf.fwDay, gVersionData.data.rf.fwMonth, gVersionData.data.rf.fwYear);
        test_print ("RF Patch Version  : %02d.%02d.%02d.%02d\n\n",
                        gVersionData.data.rf.patchMajor, gVersionData.data.rf.patchMinor, ((gVersionData.data.rf.patchBuildDebug & 0xF0)>>4), (gVersionData.data.rf.patchBuildDebug & 0xF));

    }

    return;
}

void Test_initTask(void* args)
{
    Drivers_open();
    Board_driversOpen();

    /* Debug Message: */
    test_print ("\n******************************************\n");
    test_print ("Debug: MSS Mailbox Driver Test Case.\n");
    test_print ("******************************************\n");

    bssVersionTest();

    Board_driversClose();
    Drivers_close();

    test_print("------ end of test --------\n");
    test_print ("All tests have passed!!\n");

    vTaskDelete(NULL);
}

int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( Test_initTask,
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

