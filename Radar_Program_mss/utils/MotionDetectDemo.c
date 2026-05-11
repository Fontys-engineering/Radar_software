/* Standard Include Files. */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include <drivers/gpio.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include "ti_drivers_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"


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

#include "ti\datapath\dpif\dpif_pointcloud.h"

/*
* Motion Sensor Code
* by: Petri Miettinen 11/05/26
*
* Below is a relatively simple implementation of a motion sensor code
* implemented as a proof of concept for custom integrations in the AWR2944EVM Out of Box Demo code
*
* The code makes use of the PointCloudCartesian type structure
* by referring to the velocity value of the pointcloud.
*
* Effectively, as the sensor runs, any change in velocity (object moving to/from sensor) registers as "movement"
*
* Though this means any movement taking place "parallel" to the radar does not actually register as movement
*/


volatile bool gMotionDetected;
static bool motionState = false;

bool MotionDetection_detectMotion(
    DPIF_PointCloudCartesian *objOut,
    uint32_t numObj
)
{
    const float ON_THRESHOLD  = 0.20f;
    const float OFF_THRESHOLD = 0.10f;

    bool motionNow = false;

    for (uint32_t i = 0; i < numObj; i++)
    {
        float velocity = objOut[i].velocity;

        if (velocity < 0)
        {
            velocity = -velocity;
        }

        if (!motionState)
        {
            if (velocity > ON_THRESHOLD)
            {
                motionNow = true;
                break;
            }
        }
        else
        {
            if (velocity > OFF_THRESHOLD)
            {
                motionNow = true;
                break;
            }
        }
    }

    motionState = motionNow;

    return motionState;
}

void motion_led_task(void *args)
{
    while (1)
    {
        if (gMotionDetected)
        {
            GPIO_pinWriteHigh(GPIO_LED_BASE_ADDR, GPIO_LED_PIN);
        }
        else
        {
            GPIO_pinWriteLow(GPIO_LED_BASE_ADDR, GPIO_LED_PIN);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}