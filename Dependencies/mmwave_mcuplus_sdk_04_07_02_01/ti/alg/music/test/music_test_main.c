/*
 *  Copyright (C) 2024 Texas Instruments Incorporated
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

/**
 * This example performs MUSIC tests for 4 configuration one after the other.
 *
 * Input Data is present in header file music_test.h
 * The MUSIC Spectrum Output is stored in the array: MUSICtest_resSpectrumOut
 *
 * After the algorithm is complete, data validation is performed by comparing
 * ideal and obtained result. If the equality test is passed, the test is successful.
 */

/* ========================================================================= */
/*                             Include Files                                 */
/* ========================================================================= */

#include "music_test.h"

#include "dssgenerated/ti_drivers_config.h"
#include "dssgenerated/ti_drivers_open_close.h"
#include "dssgenerated/ti_board_open_close.h"

/* ========================================================================= */
/*                           Macros & Typedefs                               */
/* ========================================================================= */

#define APP_TASK_PRI         (1U)
#define APP_TASK_STACK_SIZE  (2*1024U)


/* ========================================================================= */
/*                            Global Variables                               */
/* ========================================================================= */
TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));

uint8_t gMUSICtest_localRam[100U *1024U] __attribute__((aligned(CSL_CACHE_L1D_LINESIZE)));


/* ========================================================================= */
/*                          Function Definitions                             */
/* ========================================================================= */

/**
*  @b Description
*  @n
*    Function which configures and excute the MUSIC algorithm.
*/
void MUSIC_test_run(cplxf_t* inputData, float32_t* idealSpectrum){
    static uint8_t           testIdx = 0;
    int32_t                  retVal = 0;
    volatile uint32_t        startCycle, endCycle;
    uint32_t                 cpuCycles;
    int16_t                  i;
    MUSIC_Handle             handle = NULL;
    MUSIC_Config             musicDspCfg = {0};
    MUSIC_OutParams          outParams = {0};

    DebugP_log("Running Test %d.. \r\n", testIdx+1);
    DebugP_log("N_ant: %d, AnglesSampled: %d [%.1fdeg : %.1fdeg]\r\n",\
            MUSICtest_numAntennas[testIdx], NUM_ANGLES, MUSICtest_startAngleDeg[testIdx], \
            MUSICtest_startAngleDeg[testIdx]+MUSICtest_stepSize[testIdx]*NUM_ANGLES);

    handle = MUSIC_init(&retVal);
    if (retVal != 0)
    {
        DebugP_log("Error: Init Failed! Error: %d\r\n", retVal);
        DebugP_assert(0);
    }

    /* Configurations: Static Configurations */
    musicDspCfg.staticCfg.numAntennas = MUSICtest_numAntennas[testIdx];

    /* Steering Matrix Generation Configurations */
    musicDspCfg.staticCfg.steerMatCfg.numAnglesToSample = NUM_ANGLES;
    musicDspCfg.staticCfg.steerMatCfg.startAngleDeg = MUSICtest_startAngleDeg[testIdx];
    musicDspCfg.staticCfg.steerMatCfg.stepSize = MUSICtest_stepSize[testIdx];

    /* Jacobi EVD Configurations */
    musicDspCfg.staticCfg.jacobiEVDCfg.tolerance = 1e-15;
    musicDspCfg.staticCfg.jacobiEVDCfg.zeroThrsldFactor = 1e-3;
    musicDspCfg.staticCfg.jacobiEVDCfg.maxIterations = 3;

    /* Configurations: Hardware Resources */
    musicDspCfg.hwRes.inputData = (cplxf_t *)inputData;
    musicDspCfg.hwRes.musicSpectrum = (float32_t *)MUSICtest_resSpectrumOut;
    musicDspCfg.hwRes.coreLocalRamCfg.addr = (void*)gMUSICtest_localRam;
    musicDspCfg.hwRes.coreLocalRamCfg.size = sizeof(gMUSICtest_localRam);

    /* Configure the MUSIC Module */
    retVal = MUSIC_config(handle, &musicDspCfg);
    if (retVal != 0)
    {
        DebugP_log("Error: Config Failed! Error: %d\r\n", retVal);
        DebugP_assert(0);
    }
    DebugP_log("Scratch Buffer Memory: Total %d bytes,  Used: %d bytes\n", musicDspCfg.hwRes.coreLocalRamCfg.size, musicDspCfg.hwRes.coreLocalRamCfg.memUsedSize);

    startCycle = CycleCounterP_getCount32();
    retVal = MUSIC_process(handle, &musicDspCfg, &outParams);
    if(retVal != 0)
    {
        DebugP_log("Error: Processing Failed! Error: %d\r\n", retVal);
        DebugP_assert(0);
    }
    endCycle = CycleCounterP_getCount32(); /* get CPU cycle count and calculate diff, we dont expect any overflow for this short duration */
    cpuCycles = endCycle - startCycle;
    DebugP_log("Processing time (MUSIC Algorithm) = CPU cycles = %d.\r\n", cpuCycles);

    retVal = MUSIC_deinit(handle);
    if (retVal != 0)
    {
        DebugP_log("Error: Deinit Failed! Error: %d\r\n", retVal);
        DebugP_assert(0);
    }

    /* compare results */
    for(i=0; i < NUM_ANGLES; i++)
    {
        /* If error is greater than 2 percent, then fail. */
        if(fabsf(MUSICtest_resSpectrumOut[i] - idealSpectrum[i]) > 0.02 * idealSpectrum[i])
        {
            DebugP_log("Test case failed %d!\n\r", testIdx);
            DebugP_assert(0);
        }
    }
    DebugP_log("Test case passed!\n\r");

    testIdx++;
}


/**
*  @b Description
*  @n
*    Test Main Task to run different tests.
*/
void Test_task(void *args)
{
    Drivers_open();
    Board_driversOpen();
    CycleCounterP_reset();

    DebugP_log("MUSIC Test Started.. \r\n\n");

    MUSIC_test_run(MUSICtest_inputData_12ant, MUSICtest_idealSpectrum_12ant);
    MUSIC_test_run(MUSICtest_inputData_15ant, MUSICtest_idealSpectrum_15ant);
    MUSIC_test_run(MUSICtest_inputData_16ant, MUSICtest_idealSpectrum_16ant);
    MUSIC_test_run(MUSICtest_inputData_18ant, MUSICtest_idealSpectrum_18ant);


    Board_driversClose();
    Drivers_close();
}

/**
 *  @b Description
 *  @n
 *      Main function
 */
int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( Test_task,   /* Pointer to the function that implements the task. */
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

    return 0;
}
