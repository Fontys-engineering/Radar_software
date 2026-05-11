
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

/* MCU+SDK include files. */
#include <drivers/uart.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <ti/common/mmwavesdk_version.h>
#include <ti/control/mmwave/mmwave.h>
#include <ti/control/dpm/dpm.h>
#include <ti/utils/cli/cli.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/utils/testlogger/logger.h>

/* Demo Include Files */
#include <ti/demo/awr294x/mmw/include/mmw_config.h>
#include <ti/demo/utils/mmwdemo_rfparser.h>
#include <ti/demo/utils/mmwdemo_adcconfig.h>
#include <ti/demo/utils/mmwdemo_monitor.h>
#include <ti/demo/utils/enet_stream.h>
#ifdef MMWDEMO_TDM
#include <ti/demo/awr294x/mmw/mmw_resTDM.h>
#else
#include <ti/demo/awr294x/mmw/mmw_resDDM.h>
#endif
#include <ti/demo/awr294x/mmw/mss/mmw_mss.h>
#include <ti/demo/awr294x/mmw/include/mmw_output.h>
#include <ti/board/antenna_geometry.h>
#include <ti/demo/utils/mmwdemo_flash.h>

/* Profiler Include Files */
#include <kernel/dpl/CycleCounterP.h>

/**
 * @brief Task Priority settings:
 * Mmwave task is at higher priority because of potential async messages from BSS
 * that need quick action in real-time.
 *
 * CLI task must be at a lower priority than object detection
 * dpm task priority because the dynamic CLI command handling in the objection detection
 * dpm task assumes CLI task is held back during this processing. The alternative
 * is to use a semaphore between the two tasks.
 */

/* Demo tasks should have priority higher than enet/lwip tasks */
#define MMWDEMO_INIT_TASK_PRI         (1U)

#define MMWDEMO_INIT_TASK_STACK_SIZE  (4*1024U)
#define MMWDEMO_CLI_TASK_PRIORITY                 7
#define MMWDEMO_LED_TASK_PRIORITY                 1





StackType_t gAppMainTskStack[MMWDEMO_INIT_TASK_STACK_SIZE] __attribute__((aligned(32), section(".bss.dll.l3")));
StackType_t gLedTskStack[1024U] __attribute__((aligned(32), section(".bss.dll.l3")));
StackType_t gMotionTskStack[1024U] __attribute__((aligned(32), section(".bss.dll.l3")));



/**
 * @brief
 *  Global Variable for tracking information required by the mmw Demo
 */
extern MmwDemo_MSS_MCB    gMmwMssMCB;



/**
 * @brief
 *  Global Variable for HSRAM buffer used to share results to remote
 */
extern MmwDemo_HSRAM gHSRAM;
extern MmwDemo_calibData gCalibDataStorage;

/**************************************************************************
 *************************** Extern Definitions ***************************
 **************************************************************************/
extern void setupInit();

/*Task Handlers*/
extern void initCtrlTask();
extern void initEnetTask();
extern void initDPMTask();
extern void initUartTask();


extern void MmwDemo_CLIInit(uint8_t taskPriority);
#ifdef MMWDEMO_DDM
extern MmwDemo_RFParserHwAttr MmwDemo_RFParserHwCfg;
#endif

// Custom Definitions + Variables
extern volatile bool gMotionDetected;


extern void motion_led_task(void *args);
extern void led_task(void *args);


/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */

static void MmwDemo_initTask(void* args)
{
setupInit();
initCtrlTask();
#ifdef ENET_STREAM
initEnetTask();
#endif
initDPMTask(); 
initUartTask();

    /*****************************************************************************
     * Initialize the Profiler
     *****************************************************************************/
    CycleCounterP_reset();

    /*****************************************************************************
     * Initialize the CLI Module:
     *****************************************************************************/
    MmwDemo_CLIInit(MMWDEMO_CLI_TASK_PRIORITY);


    /*LED TEST TASK*/
    // gMmwMssMCB.taskHandles.ledTask = xTaskCreateStatic(led_task,
    // "Led_Demo_Task",
    // 512,
    // NULL,
    // MMWDEMO_LED_TASK_PRIORITY,
    // gLedTskStack,
    // &gMmwMssMCB.taskHandles.ledTaskObj);
    // configASSERT(gMmwMssMCB.taskHandles.ledTask != NULL);


    gMmwMssMCB.taskHandles.motionTask =
    xTaskCreateStatic(
        motion_led_task,
        "motion_task",
        512,
        NULL,
        1,
        gMotionTskStack,
        &gMmwMssMCB.taskHandles.motionTaskObj);

configASSERT(gMmwMssMCB.taskHandles.motionTask != NULL);

    /* Never return for this task. */
    SemaphoreP_pend(&gMmwMssMCB.demoInitTaskCompleteSemHandle, SystemP_WAIT_FOREVER);

    /* The following line should never be reached. */
    DebugP_assertNoLog(0);
}


/**
 *  @b Description
 *  @n
 *      Entry point into the Millimeter Wave Demo
 *
 *  @retval
 *      Not Applicable.
 */
int32_t main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    gMmwMssMCB.taskHandles.initTask = xTaskCreateStatic( MmwDemo_initTask,
                                  "mmwdemo_init_task",
                                  MMWDEMO_INIT_TASK_STACK_SIZE,
                                  NULL,
                                  MMWDEMO_INIT_TASK_PRI,
                                  gAppMainTskStack,
                                  &gMmwMssMCB.taskHandles.initTaskObj );
    configASSERT(gMmwMssMCB.taskHandles.initTask != NULL);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    /* The following line should never be reached because vTaskStartScheduler()
    will only return if there was not enough FreeRTOS heap memory available to
    create the Idle and (if configured) Timer tasks.  Heap management, and
    techniques for trapping heap exhaustion, are described in the book text. */
    DebugP_assertNoLog(0);
}
