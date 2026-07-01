/*
* Radar Embedded Programming Project Code
* Worked on by: Petri Miettinen
* as part of third year internship project for DSS
* 
* The below code is a boiled down, edited version of the Out of Box demo for
* AWR2944EVM radar board by Texas Instruments
* Code still mostly uses the same infrastructure of the TI, so public distribution
* Of the code at the time is not likely allowed. Further customization is warranted beforehand.
*
* The main code file has been altered for readability.
* This is meant to make it easier to focus on implementing more custom implementations without browsing through
* 4000 Lines of comments first. 
* Instead, the original main code of the OOB Demo has been moved into RadarSetup.c
* This file should be referred to for the base functionality of the radar, as well as when implementing
* Code that may need to refer to the main code or alter it in any way.
* Eg, the motion detection code also refers to the ProcessedOutput function in the main code itself.
*
* For additional information refer to DSS Radar Program Custom Function manual available through DSS Teams
*
*
*
*

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


#define MMWDEMO_INIT_TASK_PRI         (1U)

#define MMWDEMO_INIT_TASK_STACK_SIZE  (4*1024U)
#define MMWDEMO_CLI_TASK_PRIORITY                 7
#define MMWDEMO_LED_TASK_PRIORITY                 1

StackType_t gAppMainTskStack[MMWDEMO_INIT_TASK_STACK_SIZE] __attribute__((aligned(32), section(".bss.dll.l3")));



#ifdef MOTION_TASK
StackType_t gMotionTskStack[1024U] __attribute__((aligned(32), section(".bss.dll.l3")));
extern volatile bool gMotionDetected;
extern void motion_led_task(void *args);
#endif

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
    //initEnetTask();
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
    #ifdef MOTION_TASK
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
    #endif
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
