/**
 *   @file  dopplerprochwa_test_main.c
 *
 *   @brief
 *      Unit Test code for doppler processing.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 - 2025 Texas Instruments, Inc.
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

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* MCU Plus Include Files. */
#ifdef SUBSYS_DSS
#include <ti/datapath/dpu/dopplerproc/test/dssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/dopplerproc/test/dssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/dopplerproc/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/dopplerproc/test/dssgenerated/ti_board_open_close.h>
#endif

#ifdef SUBSYS_MSS
#include <ti/datapath/dpu/dopplerproc/test/mssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/dopplerproc/test/mssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/dopplerproc/test/mssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/dopplerproc/test/mssgenerated/ti_board_open_close.h>
#endif
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmwave SDK files */
#include <ti/common/syscommon.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/datapath/dpu/dopplerproc/dopplerprochwa.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/utils/randomdatagenerator/gen_rand_data.h>
#include <ti/utils/fft/fft.h>

#define EDMA_NUM_DMA_CHANNELS               64

/* EDMA resource partitioning */
#define EDMA_SHADOW_LNK_PARAM_BASE_ID       EDMA_NUM_DMA_CHANNELS

/* USE_PN_SEQ_GENERATOR: 0 - Use sine plus Gaussian noise generator, 1 - Use PN sequence generator, (runs faster) */
#define USE_PN_SEQ_GENERATOR            1
/* USE_MAG_LOG2_APPROX: 0 - Functions square root and log2 from math.h, 1 - approximation approach */
#define USE_MAG_LOG2_APPROX             0 //1
/* USE_FFT_FUNCTION_FOR_REF_CALC: 0 - Use DFT for Fourier transform calculation, 1 - Use FFT for Fourier transform calculation */
#define USE_FFT_FUNCTION_FOR_REF_CALC   1


#define EDMA_INSTANCE_ID                      EDMA_DRV_INST_DSS_A

/* EDMA channels */
#define TEST_EDMA_DOPPLERPROC_IN_PING         EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ4
#define TEST_EDMA_DOPPLERPROC_IN_PONG         EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ5

/*This has to match the HWA DMA number*/
#define TEST_EDMA_DOPPLERPROC_OUT_PING        EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ2

/*This has to match the HWA DMA number*/
#define TEST_EDMA_DOPPLERPROC_OUT_PONG        EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ3

                                                /* Free channels */
#define TEST_EDMA_DOPPLERPROC_HOTSIG_PING     EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ0
#define TEST_EDMA_DOPPLERPROC_HOTSIG_PONG     EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ1


/*EDMA shadow channels*/
#define TEST_EDMA_DOPPLERPROC_IN_PING_SHADOW     (EDMA_SHADOW_LNK_PARAM_BASE_ID + 4U)
#define TEST_EDMA_DOPPLERPROC_IN_PONG_SHADOW     (EDMA_SHADOW_LNK_PARAM_BASE_ID + 5U)
#define TEST_EDMA_DOPPLERPROC_OUT_PING_SHADOW    (EDMA_SHADOW_LNK_PARAM_BASE_ID + 6U)
#define TEST_EDMA_DOPPLERPROC_OUT_PONG_SHADOW    (EDMA_SHADOW_LNK_PARAM_BASE_ID + 7U)
#define TEST_EDMA_DOPPLERPROC_HOTSIG_PING_SHADOW (EDMA_SHADOW_LNK_PARAM_BASE_ID + 8U)
#define TEST_EDMA_DOPPLERPROC_HOTSIG_PONG_SHADOW (EDMA_SHADOW_LNK_PARAM_BASE_ID + 9U)


#ifdef DRIVERS_RADAR_HWA_V1
#define TEST_MAX_NUM_DOPPLER_BINS 512
#define TEST_MAX_NUM_RANGE_BINS   1024
#define HWA_MAX_WINRAM_SIZE       1024
#endif

#ifdef DRIVERS_RADAR_HWA_V2
#define TEST_MAX_NUM_DOPPLER_BINS 1024
#define TEST_MAX_NUM_RANGE_BINS   2048
#define HWA_MAX_WINRAM_SIZE       2048
#endif

#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
#define MAX_NUM_TX_ANTENNAS      4
#else
#define MAX_NUM_TX_ANTENNAS      3
#endif

#ifdef DRIVERS_RADAR_HWA_V1
//1 Tx, 1, 2, 4 rx, the maximum size is 1024(range)*32(doppler)
//2 Tx, 2 rx, the maximum size is 1024(range)*32(doppler) , 4 rx, the maximum size is 1024(range)*16(doppler)
//3 tx, 4 rx, the maximum size is 1024(range)*16 (doppler)
#define TEST_MAX_RADAR_CUBE_SIZE  (48 * 1024 * 4)
#endif

#ifdef DRIVERS_RADAR_HWA_V2
//1 Tx, 1, 2, 4 rx, the maximum size is 2048(range)*32(doppler)
//2 Tx, 2 rx, the maximum size is 2048(range)*32(doppler) , 4 rx, the maximum size is 2048(range)*16(doppler)
//3 tx, 4 rx, the maximum size is 2048(range)*16 (doppler)
//4 tx, 4 rx, the maximum size is 2048(range)*8 (doppler)
#define TEST_MAX_RADAR_CUBE_SIZE  (48 * 2048 * 4)
#endif

/*cube */
/* msb is real, lsb 16 is imag */
cmplx16ImRe_t testRadarCube[TEST_MAX_RADAR_CUBE_SIZE]__attribute__((aligned(64), section(".l3ram")));

/*detection matrix*/
#ifdef DRIVERS_RADAR_HWA_V1
#define TEST_MAX_DETMATRIX_SIZE    (32 * 1024)
#endif

#ifdef DRIVERS_RADAR_HWA_V2
#define TEST_MAX_DETMATRIX_SIZE    (32 * 2048)
#endif


/* log2(abs) sum over all tx and rx antenna */
int16_t calculatedDetMatrix[TEST_MAX_DETMATRIX_SIZE]__attribute__((aligned(64), section(".l3ram")));

int32_t testDopplerWindow[TEST_MAX_NUM_DOPPLER_BINS]__attribute__((aligned(64), section(".coreLocalRam")));

uint32_t fftDataInBuf[TEST_MAX_NUM_DOPPLER_BINS]__attribute__((aligned(64), section(".coreLocalRam")));

#if USE_FFT_FUNCTION_FOR_REF_CALC
cplxf_t fftBuf[TEST_MAX_NUM_DOPPLER_BINS]__attribute__((aligned(64), section(".coreLocalRam")));
#endif

int32_t fftDataWindow[TEST_MAX_NUM_DOPPLER_BINS * 2]__attribute__((aligned(64), section(".coreLocalRam")));

int32_t fftDataOutBuf[TEST_MAX_NUM_DOPPLER_BINS * 2]__attribute__((aligned(64), section(".coreLocalRam")));

/* save the reference   */
int32_t detMatRefTemp[TEST_MAX_NUM_DOPPLER_BINS]__attribute__((aligned(64), section(".coreLocalRam")));

uint16_t detMatRef[TEST_MAX_NUM_DOPPLER_BINS]__attribute__((aligned(64), section(".coreLocalRam")));  /* shift it into 16 bits */

/* Sine table of (MAX_FFT_SIZE/4+1) samples: (Table = sin(2*pi*[0:MAX_FFT_SIZE/4]/MAX_FFT_SIZE)) */
float SinCosTab[MAX_FFT_SIZE/4+1];

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (5U)
#define APP_TASK_STACK_SIZE  (8 * 1024)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));

#define PI 3.141592653589793
#define MAX_ERROR 50

typedef struct dopplerProcDpuTestConfig_t {
    uint32_t numTxAntennas;
    uint32_t numRxAntennas;
    uint32_t numRangeBins;
    uint32_t numDopplerBins;
    uint32_t numChirps; //numChirps * numTxAnt = numChirps per frame
    uint32_t winSymFlag; //0 - symmetric , 1 -non-symmetric
    uint32_t winType;   //0 - hanning window, 1 - rec window
    bool     staticClutterRemovalEnabled;
    uint16_t dcEstShift;
    uint16_t dcEstScaleQ8;
}dopplerProcDpuTestConfig_t;

HWA_Handle  hwaHandle;
volatile uint8_t subFrameCount = 0;

/* test configuration */
DPU_DopplerProcHWA_Config    dopplerProcDpuCfg;
DPU_DopplerProcHWA_Handle    dopplerProcDpuHandle;
Edma_IntrObject   intrObj;

MCPI_LOGBUF_INIT(9216);

uint8_t finalResults;
uint8_t checkResults;

void dopplerProc_test_hwainit()
{
    int32_t             errorCode;

    hwaHandle = HWA_open(0, NULL, &errorCode);
    if (hwaHandle == NULL)
    {
        test_print("HWA failed to open\n");
        DebugP_assert(0);
        return;
    }
}

void dopplerProc_test_dpuInit()
{
    int32_t errorCode;
    DPU_DopplerProcHWA_InitParams   initCfg;

    initCfg.hwaHandle = hwaHandle;
    dopplerProcDpuHandle = DPU_DopplerProcHWA_init(&initCfg, subFrameCount, &errorCode);
    if (dopplerProcDpuHandle == NULL)
    {
        test_print("doppler DPU failed to initialize. Error %d.\n", errorCode);
        return;
    }
}

/**
 *  @b Description
 *  @n
 *      Allocates Shawdow paramset
 */
static void allocateEDMAShadowChannel(uint32_t *param)
{
    int32_t             testStatus = SystemP_SUCCESS;

    testStatus = EDMA_allocParam(gEdmaHandle[0], param);
    DebugP_assert(testStatus == SystemP_SUCCESS);

    return;
}

void dopplerProc_test_dpuConfig()
{
    uint32_t dmaCh, tcc, param;

    memset((void*)&dopplerProcDpuCfg, 0, sizeof(DPU_DopplerProcHWA_Config));

    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHandle = gEdmaHandle[CONFIG_EDMA0];
    dopplerProcDpuCfg.hwRes.edmaCfg.intrObj = &intrObj;

    dmaCh = TEST_EDMA_DOPPLERPROC_IN_PING;
    tcc   = TEST_EDMA_DOPPLERPROC_IN_PING;
    param = TEST_EDMA_DOPPLERPROC_IN_PING;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[0].channel = dmaCh;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[0].paramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[0].tcc     = tcc;

    param = TEST_EDMA_DOPPLERPROC_IN_PING_SHADOW;
    allocateEDMAShadowChannel(&param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[0].shadowPramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[0].eventQueue = 0;


    dmaCh = TEST_EDMA_DOPPLERPROC_IN_PONG;
    tcc   = TEST_EDMA_DOPPLERPROC_IN_PONG;
    param = TEST_EDMA_DOPPLERPROC_IN_PONG;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[1].channel = dmaCh;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[1].paramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[1].tcc     = tcc;

    param = TEST_EDMA_DOPPLERPROC_IN_PONG_SHADOW;
    allocateEDMAShadowChannel(&param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[1].shadowPramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaIn.pingPong[1].eventQueue = 0;

    dmaCh = TEST_EDMA_DOPPLERPROC_OUT_PING;
    tcc   = TEST_EDMA_DOPPLERPROC_OUT_PING;
    param = TEST_EDMA_DOPPLERPROC_OUT_PING;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[0].channel = dmaCh;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[0].paramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[0].tcc     = tcc;

    param = TEST_EDMA_DOPPLERPROC_OUT_PING_SHADOW;
    allocateEDMAShadowChannel(&param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[0].shadowPramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[0].eventQueue = 0;

    dmaCh = TEST_EDMA_DOPPLERPROC_OUT_PONG;
    tcc   = TEST_EDMA_DOPPLERPROC_OUT_PONG;
    param = TEST_EDMA_DOPPLERPROC_OUT_PONG;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[1].channel = dmaCh;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[1].paramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[1].tcc     = tcc;

    param = TEST_EDMA_DOPPLERPROC_OUT_PONG_SHADOW;
    allocateEDMAShadowChannel(&param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[1].shadowPramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaOut.pingPong[1].eventQueue = 0;

    dmaCh = TEST_EDMA_DOPPLERPROC_HOTSIG_PING;
    tcc   = TEST_EDMA_DOPPLERPROC_HOTSIG_PING;
    param = TEST_EDMA_DOPPLERPROC_HOTSIG_PING;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[0].channel = dmaCh;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[0].paramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[0].tcc     = tcc;

    param = TEST_EDMA_DOPPLERPROC_HOTSIG_PING_SHADOW;
    allocateEDMAShadowChannel(&param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[0].shadowPramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[0].eventQueue = 0;

    dmaCh = TEST_EDMA_DOPPLERPROC_HOTSIG_PONG;
    tcc   = TEST_EDMA_DOPPLERPROC_HOTSIG_PONG;
    param = TEST_EDMA_DOPPLERPROC_HOTSIG_PONG;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[1].channel = dmaCh;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[1].paramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[1].tcc     = tcc;

    param = TEST_EDMA_DOPPLERPROC_HOTSIG_PONG_SHADOW;
    allocateEDMAShadowChannel(&param);
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[1].shadowPramId = param;
    dopplerProcDpuCfg.hwRes.edmaCfg.edmaHotSig.pingPong[1].eventQueue = 0;

    /* window factor */
    dopplerProcDpuCfg.hwRes.hwaCfg.window = testDopplerWindow;
    //dopplerProcDpuCfg.hwRes.hwaCfg.winRamOffset = 256;
    // dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling = DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED; // DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED
    dopplerProcDpuCfg.hwRes.hwaCfg.paramSetStartIdx = 0;

    /* cube input*/
    dopplerProcDpuCfg.hwRes.radarCube.datafmt = DPIF_RADARCUBE_FORMAT_1; //only format 1
    dopplerProcDpuCfg.hwRes.radarCube.data = (void *)testRadarCube;

    /* output */
    dopplerProcDpuCfg.hwRes.detMatrix.datafmt = DPIF_DETMATRIX_FORMAT_1;
    dopplerProcDpuCfg.hwRes.detMatrix.data = (void *)calculatedDetMatrix;
}

/**********************************************************
 * Calculate scale and shift same as in HWA
 *********************************************************/
void Test_calcDCEstimParams(dopplerProcDpuTestConfig_t * testConfig)
{
    uint32_t dcestShift;
    uint16_t dcestScaleQ8;
    uint32_t numChirps = testConfig->numChirps;

    dcestShift = mathUtils_ceilLog2(numChirps);

    dcestScaleQ8 = (uint16_t) (256. * (float) (1<<dcestShift) / (float) numChirps + 0.5);

    testConfig->dcEstScaleQ8 = dcestScaleQ8;
    testConfig->dcEstShift = dcestShift;
}

/* set up the doppler proc dpu configuration per test*/
void Test_setProfile(dopplerProcDpuTestConfig_t * testConfig)
{
    DPU_DopplerProcHWA_StaticConfig * params;
    DPU_DopplerProcHWA_HW_Resources * hwResParams;

    uint32_t ii;

    params = &dopplerProcDpuCfg.staticCfg;
    params->numTxAntennas = testConfig->numTxAntennas;
    params->numRxAntennas = testConfig->numRxAntennas;
    params->numVirtualAntennas = testConfig->numTxAntennas * testConfig->numRxAntennas;
    params->numRangeBins = testConfig->numRangeBins;
    params->numDopplerChirps = testConfig->numChirps;
    params->numDopplerBins = testConfig->numDopplerBins;
    ii = 1;
    while ((1 << ii) < testConfig->numDopplerBins)
    {
        ii++;
    }
    params->log2NumDopplerBins = ii;

    /* static clutter removal */
    params->staticClutterCfg.isEnabled = testConfig->staticClutterRemovalEnabled;

    hwResParams = &dopplerProcDpuCfg.hwRes;
    /* windowing */
    dopplerProcDpuCfg.hwRes.hwaCfg.winSym = testConfig->winSymFlag; // HWA_FFT_WINDOW_SYMMETRIC;
    dopplerProcDpuCfg.hwRes.hwaCfg.winRamOffset = HWA_MAX_WINRAM_SIZE - (testConfig->numChirps);


    //dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling = DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED; // DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED;
    if (testConfig->winType == 1)     //if rec(1) win, enable, if hanning(0) disable
        dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling = DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED;
    else
        dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling = DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED;

    if (dopplerProcDpuCfg.hwRes.hwaCfg.winSym == HWA_FFT_WINDOW_NONSYMMETRIC)
    {
        hwResParams->hwaCfg.windowSize = testConfig->numChirps * sizeof(int32_t);
    }
    else
    {
        hwResParams->hwaCfg.windowSize = (testConfig->numChirps + 1) / 2 * sizeof(int32_t);
    }
    if (params->staticClutterCfg.isEnabled)
    {
        hwResParams->hwaCfg.numParamSets =
                DPU_DOPPLERPROCHWA_NUM_HWA_PARAMS_CLUTTER_REMOVAL_ENABLED(testConfig->numTxAntennas);
    }
    else
    {
        hwResParams->hwaCfg.numParamSets =
                DPU_DOPPLERPROCHWA_NUM_HWA_PARAMS_CLUTTER_REMOVAL_DISABLED(testConfig->numTxAntennas);
    }
    /* cube */
    hwResParams->radarCube.dataSize = testConfig->numTxAntennas * testConfig->numRangeBins * testConfig->numDopplerBins * testConfig->numRxAntennas * 4;
   /* output*/
    hwResParams->detMatrix.dataSize = testConfig->numRangeBins * testConfig->numDopplerBins * sizeof(int16_t);     //output is uint16

}


#if USE_PN_SEQ_GENERATOR
volatile int32_t gScramblerReg = 0;
#define SCR_INPUT 0xF8000000
#define SCR_MASK  0x07FFFFFF
#define SCR_SAMP_BITWIDTH 10
#endif

 /*
 * generates the radar cube data in format 1. for each range bin, it generates the exp signal with one peak. the peak
   is at mod ((2*rangeBinIdx), numberofchirps);

 */
#if USE_PN_SEQ_GENERATOR
void Test_cubedata_gen(dopplerProcDpuTestConfig_t * testConfig)
{
    int32_t cubeIdx;
    int32_t cubeSize;
    //int16_t reVal, imVal;


    /* generate radar cube 1D fft output  X[numTxAnt][numChirps][numRxAnt][numRngBin] */
    cubeSize = testConfig->numTxAntennas * testConfig->numChirps * testConfig->numRxAntennas * testConfig->numRangeBins;
    cubeIdx = 0;

#if 1
    for (cubeIdx = 0; cubeIdx < cubeSize; cubeIdx++)
    {
        uint32_t bits;
        bits = SCR_INPUT ^ (gScramblerReg << 18) ^ (gScramblerReg << 23);
        gScramblerReg = (bits & SCR_INPUT) | (SCR_MASK & (gScramblerReg >> 5));
        testRadarCube[cubeIdx].real =  gScramblerReg >> (32 - SCR_SAMP_BITWIDTH);

        bits = SCR_INPUT ^ (gScramblerReg << 18) ^ (gScramblerReg << 23);
        gScramblerReg = (bits & SCR_INPUT) | (SCR_MASK & (gScramblerReg >> 5));
        testRadarCube[cubeIdx].imag =  gScramblerReg >> (32 - SCR_SAMP_BITWIDTH);
    }
#else
    for (cubeIdx = 0; cubeIdx < cubeSize; cubeIdx++)
    {
        PNSeqGenerator(10, &testRadarCube[cubeIdx].real, &testRadarCube[cubeIdx].imag);
    }

#endif
}
#else
void Test_cubedata_gen(dopplerProcDpuTestConfig_t * testConfig)
{

    uint16_t ii, jj, kk;
    uint16_t numChirpsFrame;
    uint32_t radarCubeIdx;
    float amplitude; //signal amplitude
    //float snr; //signal snr
    float noiseScale; //noiseScale^2 is noisevaraince
    float dopplerIncr, dopplerAngle;
    float real, imag, noiseRe, noiseIm;
    uint32_t doppleBin;
    uint32_t txAntIdx;
    float var = 1;
    int32_t tempvalue;

    if (testConfig->numDopplerBins == 512)
    {
        amplitude = 25.f; // peak amplitude
        noiseScale = 620.f; // noise scale
    }
    else if (testConfig->numDopplerBins == 256)
    {
        amplitude = 25.f;
        noiseScale = 650.f;
    }
    else if (testConfig->numDopplerBins == 128)
    {
        amplitude = 50.f;
        noiseScale = 650.f;
    }
    else
    {
        amplitude = 60.f;
        noiseScale = 800.f;
    }

    //amplitude = 200.f;
    //noiseScale = 10.f;

    numChirpsFrame = testConfig->numTxAntennas * testConfig->numChirps;

    /* generate radar cube 1D fft output*/
    for (ii = 0; ii < numChirpsFrame; ii++) //chirp loop
    {
        for (jj = 0; jj < testConfig->numRxAntennas; jj++) //rx antenna loop
        {
            for (kk = 0; kk < testConfig->numRangeBins; kk++)
            {

                if (testConfig->numChirps < 8)
                    /* peak is at mod (kk, numChirps) */
                    doppleBin = kk - kk/ testConfig->numChirps * testConfig->numChirps;
                else
                    /* peak is at mod (2*kk, numChirps) */
                    doppleBin = (2 * kk) - (2 * kk) / testConfig->numChirps * testConfig->numChirps;
                dopplerIncr = 2.f * PI *  (float)doppleBin / (float)numChirpsFrame;
                dopplerAngle = ii * dopplerIncr;  //increase every chirp

                if (testConfig->numTxAntennas == 1)
                {
                    radarCubeIdx = ii * testConfig->numRxAntennas * testConfig->numRangeBins;

                }
                else
                {
                    txAntIdx = ii - ii / testConfig->numTxAntennas * testConfig->numTxAntennas; //mod(ii, numTx)
                    radarCubeIdx = txAntIdx * testConfig->numChirps * testConfig->numRxAntennas * testConfig->numRangeBins;
                    radarCubeIdx += (ii / testConfig->numTxAntennas) * testConfig->numRxAntennas * testConfig->numRangeBins;
                }

                radarCubeIdx += jj * testConfig->numRangeBins;
                radarCubeIdx += kk;


                GaussianNoise(var, &noiseRe, &noiseIm);
                real = amplitude * cosf(dopplerAngle) + noiseScale * noiseRe;
                imag = amplitude * sinf(dopplerAngle) + noiseScale * noiseIm;
                tempvalue = (int32_t)MATHUTILS_ROUND_FLOAT(real);
                MATHUTILS_SATURATE16(tempvalue);
                testRadarCube[radarCubeIdx].real = (int16_t)tempvalue;
                tempvalue = (int32_t)MATHUTILS_ROUND_FLOAT(imag);
                MATHUTILS_SATURATE16(tempvalue);
                testRadarCube[radarCubeIdx].imag = (int16_t)tempvalue;

            }
        }/* end of rx antenna loop*/
    }//end of chirp loop
}
#endif

/* generates the rectangle and hanning window coefficients */
void Test_window2DCoef_gen(uint32_t numSamples, uint8_t winType)
{
    float ephyR, ephyI;
    float phi = 2 * PI_ / ((float)numSamples - 1);


    uint32_t ii;
    float a0;
    float winValue;
    int32_t winValueFixed;
    float cosValue, sinValue ;
    float temp;


    cosValue = 1.f;
    sinValue = 0.f;

    ephyR = cos(phi);
    ephyI = sin(phi);
    a0 = 0.5f;


    if (winType == 1)
    {
        for (ii = 0; ii < numSamples; ii++)
        {
            testDopplerWindow[ii] = (1 << 17) - 1; //rec window, for debug
        }
    }
    else
    {
        /* hanning window */
        for (ii = 0; ii < numSamples; ii++)
        {
            winValue = a0 * (1.f - cosValue);       //0.5*(1-cos(2*pi*n/N-1))

            winValue = winValue * (1 << 17) + 0.5;     //Q17 format
            winValueFixed = (uint32_t)winValue;
            if (winValueFixed >= (1 << 17))
            {
                winValueFixed = (1 << 17) - 1;
            }
            temp = cosValue;
            cosValue = cosValue * ephyR - sinValue * ephyI;
            sinValue = temp * ephyI + sinValue * ephyR;

            //testDopplerWindow[ii] = (1<<17) - 1; //rec window, for debug
            testDopplerWindow[ii] = winValueFixed;
        }
    }

}

/*  calculates fft, input is 32 bits I/Q, output is also 32x32 bits I/Q*/
void Test_2Dfft_ref(int32_t * dataOut, int32_t * dataIn, uint32_t fftSize, uint32_t numAdcSamples)
{
#if USE_FFT_FUNCTION_FOR_REF_CALC
    uint32_t ii;
    uint16_t divValue;
    float scale;
#else
    uint32_t ii, jj;
    float cosValue, sinValue;
    float cosStep, sinStep;
    float alpha;
    float realValueTemp, imagValueTemp;
    float realValue, imagValue;
    float inReal, inImag;
    int32_t outReal, outImag;
    float tempValue;
    uint16_t divValue;
    float scale;
#endif
    /* hard code all the scale, mathc with hwa fft scale settings, asssume first fft scale is disabled. */
    divValue = 0;
    if (fftSize == 2)
        divValue = 0;
    else if (fftSize == 4)
        divValue = 1;
    else if (fftSize == 8)
        divValue = 2;
    else if (fftSize == 16)
        divValue = 3; //3 fft right shift, first one disabled
    else if (fftSize == 32)
        divValue = 4;
    else if (fftSize == 64)
        divValue = 5;
    else if (fftSize == 128)
        divValue = 6;
    else if (fftSize == 256)
        divValue = 7;
    else if (fftSize == 512)
        divValue = 8;
    else if (fftSize == 1024)
        divValue = 9;
#ifdef DRIVERS_RADAR_HWA_V2
    else if (fftSize == 2048)
        divValue = 10;
#endif

    /* if first fft scale is enabled, scale + 1*/
    if (dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling == DPU_DOPPLERPROCHWA_FIRST_SCALING_ENABLED)
        divValue += 1;

    scale = 1. / ((float)(1 << divValue));

#if USE_FFT_FUNCTION_FOR_REF_CALC
    for (ii = 0; ii < numAdcSamples; ii++)
    {
        fftBuf[ii].real = dataIn[2*ii+1];
        fftBuf[ii].imag = dataIn[2*ii];
    }
    for (ii = numAdcSamples; ii < fftSize; ii++)
    {
        fftBuf[ii].real = 0.;
        fftBuf[ii].imag = 0.;
    }

    /* FFT */
    if(fftSize == 2)
    {
        fft2(fftBuf);
    }
    else
    {
        fft(SinCosTab, fftBuf, mathUtils_ceilLog2(fftSize));
    }

    for (ii = 0; ii < fftSize; ii++)
    {
        dataOut[2*ii] = MATHUTILS_ROUND_FLOAT(scale * fftBuf[ii].real);
        dataOut[2*ii+1] = MATHUTILS_ROUND_FLOAT(scale * fftBuf[ii].imag);
    }

#else
    //in 32 bits input, Real16/imag16
    for (ii = 0; ii < fftSize; ii++)
    {
        alpha = -2.f * ii * PI_ / (float)fftSize;
        cosValue = 1.f;
        sinValue = 0.f;
        cosStep = cos(alpha);
        sinStep = sin(alpha);
        //realValue = (float)((int16_t)(dataIn[0] >> 16));
        //imagValue = (float)((int16_t)(dataIn[0] & 0xFFFF));   //lsb at even
        realValue = (float)dataIn[1];
        imagValue = (float)dataIn[0];
        for (jj = 1; jj < numAdcSamples; jj++)
        {
            tempValue = cosValue;
            cosValue = cosValue * cosStep - sinValue * sinStep;
            sinValue = sinValue * cosStep + tempValue *sinStep;

            //inReal = (float)((int16_t)(dataIn[jj] >> 16));
            //inImag = (float)((int16_t)(dataIn[jj] & 0xFFFF));
            inReal = (float)dataIn[2 * jj + 1];
            inImag = (float)dataIn[2 * jj];

            realValueTemp = inReal * cosValue - inImag * sinValue;
            realValue += realValueTemp;
            imagValueTemp = inReal * sinValue + inImag * cosValue;
            imagValue += imagValueTemp;
        }
        /*match with hwa hardware */
        realValue = realValue * scale;
        outReal = (int32_t)MATHUTILS_ROUND_FLOAT(realValue);

        imagValue = imagValue * scale;
        outImag = (int32_t)MATHUTILS_ROUND_FLOAT(imagValue);

        dataOut[2 * ii] = outReal;
        dataOut[2 * ii + 1] = outImag ;
    }
#endif
}

/******************************************************************
 * Magnitude approximation
 * U = max(|I|,|Q|) and V = min(|I|,|Q|).
 * Magnitude = max (U + V / 8, 7U / 8 + V / 2)
 * Inputs re and im are 24-bit integers, range: -2^23 to +2^23-1
******************************************************************/
inline uint32_t magnitudeApprox (int32_t re, int32_t im)
{
    int32_t u,v;
    int32_t mag;
    int32_t p,q;

    if(re<0) re = -re;
    if(im<0) im = -im;
    if(re > 8388607) re = 8388607;
    if(im > 8388607) im = 8388607;

    u = re;
    if(im > re) u = im;
    v = re;
    if(im < re) v = im;

    p = 8*u + v;
    q = 7*u + 4*v;

    mag = p;
    if(q > p) mag = q;

    mag = (mag + 0x4) >> 3;

    return (uint32_t) mag;
}

/**********************************************************/
/*Lookup table for log2 approximation in Q11 format       */
/* T = round(2048*log2(1+f))    f=[0:64]/64               */
/**********************************************************/
uint16_t log2TabFrac[65] = {
        0,   46,   91,  135,  179,  222,  265,  307,  348,  389,  429,  469,  508,  546,  585,  622,
      659,  696,  732,  768,  803,  838,  873,  907,  941,  974, 1007, 1040, 1072, 1104, 1136, 1167,
     1198, 1229, 1259, 1289, 1319, 1348, 1377, 1406, 1435, 1463, 1491, 1519, 1546, 1573, 1600, 1627,
     1653, 1680, 1706, 1732, 1757, 1783, 1808, 1833, 1857, 1882, 1906, 1930, 1954, 1978, 2001, 2025,
     2048
};

/**********************************************************************/
/* Calculates log2 using look up table and norm instruction           */
/* Log2 value is in Q11 format. For x=0 function returns zero         */
/* Approximation:                                                     */
/* log2(x) = k + log2(1+f),                                           */
/* where k is floor(log2(x)), and log2(1+f) is from LUT               */
/**********************************************************************/
inline uint16_t log2Approx(uint32_t x)
{
    uint16_t log2q11;
    uint32_t k, f;

    if (x == 0)
    {
        log2q11 = 0;
        return log2q11;
    }

    k = mathUtils_floorLog2(x);

    x = x & ((1<<k)-1);
    if (k < 6)
    {
        x = x << (6-k);
    }
    if (k > 6)
    {
        x = (x + (1<<(k-7))) >> (k-6);
    }
    f = log2TabFrac[x];
    log2q11 = (k<<11) + f;
    return log2q11;
}

/********************************************************************
 * Removes mean vlue form the samples
********************************************************************/
void Test_staticClutterRemoval(uint32_t *buffer, uint32_t numChirps, uint16_t scaleQ8, uint16_t shift)
{
     int32_t accReal;
     int32_t accImag;
     int32_t chirpInd;
     int32_t xRe, xIm;
     int32_t temp;
     int32_t rounding;

     accReal = 0;
     accImag = 0;
     rounding = 0; //No rounding in HWA (1 << (7+shift));

     for (chirpInd = 0; chirpInd < numChirps; chirpInd++)
     {
         temp = (int32_t) buffer[chirpInd];
         xRe = temp >> 16;
         xIm = (temp << 16) >> 16;

         accReal += xRe;
         accImag += xIm;
     }

     temp = (accReal * scaleQ8 + rounding) >> (8 + shift);
     accReal = (int32_t) temp;

     temp = (accImag * scaleQ8 + rounding) >> (8 + shift);
     accImag = (int32_t) temp;

     for (chirpInd = 0; chirpInd < numChirps; chirpInd++)
     {
         temp = (int32_t) buffer[chirpInd];
         xRe = temp >> 16;
         xIm = (temp << 16) >> 16;

         xRe -= accReal;
         xIm -= accImag;
         MATHUTILS_SATURATE16(xRe);
         MATHUTILS_SATURATE16(xIm);
         buffer[chirpInd] = (xRe << 16) | (xIm & 0x0000FFFF);
     }
}

/*
    Generates the reference for Doppler proc DPU.
    it implements windowing 16bits I/Q input,  32 bits I/Q output
    fft calculation,
    abs calculation,
    log2 calcuations is Q 11 format
*/
void  Test_2Dfft_refGen(uint32_t numSamples, uint32_t fftSize)
{
    uint32_t ii;
    int32_t winCoef;
    uint32_t dataSample;
    int64_t realValue, imagValue;
    int32_t fftoutR, fftoutI;
#if !USE_MAG_LOG2_APPROX
    double absValue, log2Value;
#endif
    int64_t tempValue;
    //int16_t leftScale;

    int32_t log2Temp;

    /* based on hwa setting,
    srcScale = 0; ---> shift 16bits input left (8-srcScale) bits
    test DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED mode, so fft scale is
    (cfg->staticCfg.numDopplerBins - 1) >> 1 ---> every stage, 1/2 scale, except the first one
    dstScale = 0; --> right shift dstScale bits,
    output is Q11 format
    */

    /* windowing, input is 16x16 complex,  output is 32 bits
       to match with hwa settings, the input is left shift 8 bits
    */
    for (ii = 0; ii < numSamples; ii++)
    {
        winCoef = (int32_t)testDopplerWindow[ii];
        dataSample = fftDataInBuf[ii];

        imagValue = (int64_t)  ((int16_t)(dataSample & 0x0000FFFF));      //lsb is imag
        imagValue = imagValue<< 8;
        tempValue = (imagValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
        fftDataWindow[ii * 2] = (int32_t) tempValue;       //imag is even

        realValue = (int64_t) ((int16_t)(dataSample >> 16)) ;
        realValue = realValue << 8;
        tempValue = (realValue * winCoef + 0x10000) >> 17;
        fftDataWindow[ii * 2 + 1] = (int32_t)tempValue;     //real is odd
    }


    /* fft */
    Test_2Dfft_ref(fftDataOutBuf, fftDataWindow, fftSize, numSamples );

    /* abs, log2 sum*/
    for (ii = 0; ii < fftSize; ii++)
    {

        fftoutR = fftDataOutBuf[2 * ii] ;
        fftoutI = fftDataOutBuf[2 * ii + 1] ;
#if USE_MAG_LOG2_APPROX
        log2Temp = log2Approx(magnitudeApprox (fftoutR, fftoutI));
#else
        absValue = sqrt(((double)fftoutR * fftoutR + (double) fftoutI * fftoutI));
        if (absValue == 0)
            log2Value = 0;      //for debug
        else
            log2Value = log2(absValue);

        log2Value = log2Value  * 2048.f;  //in Q 11 format

        if (log2Value >= 0)
            log2Temp = (int32_t)(log2Value + 0.5);
        else
            log2Temp = (int32_t)(log2Value - 0.5);
#endif

        detMatRefTemp[ii] += log2Temp;

    }
}

/* check the results from dpu with the reference */
void Test_printResults(dopplerProcDpuTestConfig_t * testConfig)
{
    uint32_t ii;
    uint32_t jj, kk, mm;
    uint32_t radarCubeIdx;
    int16_t rightScale;
    float sumTemp;
    uint32_t numZeros;
    uint8_t testFlag;
    int32_t diff;


    uint32_t *cubePtr = (uint32_t *)testRadarCube;     //msb is real, lsb is imag

    testFlag = 1;
    /* to match with sum done by hwa fft
    srcScale = 3, left shift 5 bits
    fftstage shift is enabled, fftsize is testConfig->numTxAntennas * testConfig->numRxAntennas
    dstScale = 8,  right shift 8
    */
    rightScale = 0;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 1)
         // no fft, but still goes to input and output
        rightScale = 3; //right shift by 3
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 2)      //fft every stage >> 1
        rightScale = 4;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 4)      //fft every stage >> 1
        rightScale = 5;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 8)
        rightScale = 6;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 12)
        rightScale = 7;
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 16)
        rightScale = 7;


    for (ii = 0; ii < testConfig->numRangeBins; ii++) //range bin
    {
        /* for accumulation over all tx antenna and rx antenna*/
        memset((void *)detMatRefTemp, 0, sizeof(int32_t) * testConfig->numDopplerBins);
        numZeros = 0;
        for (jj = 0; jj < testConfig->numTxAntennas; jj++)
        {
            for (kk = 0; kk < testConfig->numRxAntennas; kk++)
            {
                for (mm = 0; mm < testConfig->numChirps; mm++)
                {
                    radarCubeIdx = jj * testConfig->numChirps * testConfig->numRxAntennas *  testConfig->numRangeBins;
                    radarCubeIdx += (mm * testConfig->numRxAntennas * testConfig->numRangeBins);
                    radarCubeIdx += (kk * testConfig->numRangeBins);
                    radarCubeIdx += ii;

                    fftDataInBuf[mm] = cubePtr[radarCubeIdx];
                   // test_print("%d,%d\n", testRadarCube[radarCubeIdx].real, testRadarCube[radarCubeIdx].imag);

                }  /* end of doppler bin loop*/

                /* Clutter removal */
                if (testConfig->staticClutterRemovalEnabled)
                {
                    Test_staticClutterRemoval(fftDataInBuf,
                                              testConfig->numChirps,
                                              testConfig->dcEstScaleQ8,
                                              testConfig->dcEstShift);
                }

                /* call reference generation */
                Test_2Dfft_refGen(testConfig->numChirps, testConfig->numDopplerBins);


            } //end of receiver antenna

        }//end of tx antenna


        for (mm = 0; mm < testConfig->numDopplerBins; mm++)
        {
            sumTemp = detMatRefTemp[mm] >> rightScale;
            detMatRef[mm] = sumTemp;
        }


        /* verify the test results*/
        for (mm = 0; mm < testConfig->numDopplerBins; mm++)
        {
            /* dpu output is doppler, then range bin */
            diff = (int32_t) detMatRef[mm] - (int32_t) calculatedDetMatrix[ii * testConfig->numDopplerBins + mm];
            if (diff < 0) diff = - diff;
            if (diff > MAX_ERROR)
            {
                test_print("Rangebin = %d, DopplerBin = %d, DopplerSize = %d, Ref=%d, dpu=%d err= %d fail\n", ii, mm, testConfig->numDopplerBins, detMatRef[mm], calculatedDetMatrix[ii * testConfig->numDopplerBins + mm], abs(detMatRef[mm] - calculatedDetMatrix[ii * testConfig->numDopplerBins + mm]));
                finalResults = 0;
                testFlag = 0;
            }

            if (calculatedDetMatrix[ii * testConfig->numDopplerBins + mm] < 2)
                numZeros++;

       }

        if ((testConfig->numDopplerBins < 8) && (numZeros == testConfig->numDopplerBins))
        {
            test_print("Rangebin = %d, DopplerSize = %d, all zero output, fail\n", ii, testConfig->numDopplerBins);
            finalResults = 0;
            testFlag = 0;
        }
        if ((testConfig->numDopplerBins >= 8) && (numZeros > testConfig->numDopplerBins - 5))
        {
            test_print("Rangebin = %d, DopplerSize = %d, all zero output, fail\n", ii, testConfig->numDopplerBins);
            finalResults = 0;
            testFlag = 0;
        }

    }//end of rangebin

    if (testFlag == 1)
    {
        test_print("Test Pass\n");
    }
    else
    {
        test_print("Test Fail\n");
    }
    test_print("\n");
}



/******************************************************
    test main task
*******************************************************/
void Test_task(void* args)
{
    uint64_t taskStartTime, taskTime;
    uint64_t taskEndTime;
    uint32_t ii,jj;
    uint32_t cluttRemFlag;

    uint32_t numRangeBin[7] = {16, 32, 64, 128, 256, 512, 1024 };
    uint32_t numDopplerBin[9] = {2, 4, 8, 16, 32, 64, 128, 256, 512 };
    uint32_t numRxAnt[3] = {4, 2, 1 };
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
    uint32_t numTxAnt[4] = {4, 3, 2, 1 };
#else
    uint32_t numTxAnt[4] = {3, 2, 1 };
#endif

    uint32_t txAntIdx, rxAntIdx, numTestRangeBin;
    uint32_t numTests;
    dopplerProcDpuTestConfig_t testConfig;
    char featureName[200];
    char cycleSummaryText[200];
    uint32_t startTime, bench1, bench2;
    int32_t   retVal;
    DPU_DopplerProcHWA_OutParams out;
    uint32_t testCount;
    uint32_t preNumChirps;
    uint32_t maxNumChirps;

    Drivers_open();
    Board_driversOpen();

    /* Initialize test logger */
    MCPI_Initialize();

    finalResults = 1;

    taskStartTime = ClockP_getTimeUsec();

    test_print("...... Initialization ...... \n");

    fftInit(SinCosTab);

    /* hwa, edma, dpu inialization */
    dopplerProc_test_hwainit();
    dopplerProc_test_dpuInit();

    /* doppler dpu config*/
    dopplerProc_test_dpuConfig();

    test_print("Doppler Proc DPU Test start ...... \n");

    /* to fit cuba data in L3 memory */
    /*1tx, dopplerbin <= 16, upto 1024 7 range bin sizes
           dopplerbin = 32, upto 1024 7 range bin sizes
           dopplerbin = 64, upto 512, 6 range bin sizes,
           dopplerbin = 128, upto 256, 5 range bin sizes,
           dopplerbin = 256, upto 128, 4 range bin sizes,
           doppler bin = 512, upto 64, 3 range bin sizes
    for rx = 1, 2, 4 3 cases
    */
    numTests = (7 * 5 + 6 + 5 + 4 + 3) * 3;
    /*2tx, 2rx
    dopplerbin <= 16, upto 1024 7 range bin sizes
    dopplerbin = 32, upto 1024 7 range bin sizes
    dopplerbin = 64, upto 512, 6 range bin sizes,
    dopplerbin = 128, upto 256, 5 range bin sizes,
    dopplerbin = 256, upto 128, 4 range bin sizes,
    doppler bin = 512, upto 64, 3 range bin sizes
    4rx
    dopplerbin <= 16, upto 1024 7 range bin sizes
    dopplerbin = 32, upto 512, 6 range bin sizes,
    dopplerbin = 64, upto 256, 5 range bin sizes,
    dopplerbin = 128, upto 128, 4 range bin sizes,
    doppler bin = 256, upto 64, 3 range bin sizes
    doppler bin = 512, upto 64, 2 range bin sizes
    */

    numTests += ((7 * 5 + 6 + 5 + 4 + 3) + (7 * 4 + 6 + 5 + 4 + 3 + 2));
    /* 3tx,  4rx same as 2tx, 4 rx*/
    numTests += (7 * 4 + 6 + 5 + 4 + 3 + 2);

#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
    /* 4tx,  4rx
    dopplerbin <= 8, upto 1024 7 range bin sizes
    dopplerbin = 16, upto 512, 6 range bin sizes,
    dopplerbin = 32, upto 256, 5 range bin sizes,
    dopplerbin = 64, upto 128, 4 range bin sizes,
    doppler bin = 128, upto 64, 3 range bin sizes
    doppler bin = 256, upto 64, 2 range bin sizes
    doppler bin = 512, upto 64, 1 range bin sizes
    */

    numTests += (7 * 3 + 6 + 5 + 4 + 3 + 2 + 1);
#endif

    printf("\n...... %d Tests to be run .......\n\n", numTests*2); /* Static clutter removal enabled/disabled */

    testCount = 0;

    for (txAntIdx = 0; txAntIdx < MAX_NUM_TX_ANTENNAS; txAntIdx++)
    {
        for (rxAntIdx = 0; rxAntIdx < 3; rxAntIdx++)
        {

            testConfig.numTxAntennas = numTxAnt[txAntIdx];
            testConfig.numRxAntennas = numRxAnt[rxAntIdx];

            if (testConfig.numRxAntennas < testConfig.numTxAntennas)
            {
                continue;
            }

            for (ii = 0; ii < 9; ii++) /* doppler bin loop, 2, 4, 8, 16, 32, 64, 128, 256, 512 */
            {
                uint32_t sizeLimit;

                testConfig.numDopplerBins = numDopplerBin[ii];

                sizeLimit = 32768U;

                /* Comply with the config API size limits */
                maxNumChirps = sizeLimit / (testConfig.numTxAntennas * testConfig.numRxAntennas * sizeof(cmplx16ImRe_t));

                if (maxNumChirps > numDopplerBin[ii])
                    maxNumChirps = numDopplerBin[ii];

                preNumChirps = 0;
                {

//#ifdef DRIVERS_RADAR_HWA_V2
                    //numTestRangeBin = 1;
//#else
                    //1 tx, 1,2,4 rx, or 2tx/2rx, 16, 32 doppler bin, upto 1024 range bin
                    if ((testConfig.numTxAntennas == 1) || ((testConfig.numTxAntennas == 2) && (testConfig.numRxAntennas == 2)))
                    {
                        if (ii < 5)
                            numTestRangeBin = 7;
                        else
                            numTestRangeBin = 11 - ii;
                    }
                    else if ((testConfig.numTxAntennas == 2) || ((testConfig.numTxAntennas == 3) && (testConfig.numRxAntennas == 4)))
                     //2tx/4rx, 3tx/4 rx only <=16 doppler bin, upto 1024 range bin
                    {
                        if (ii < 4)
                            numTestRangeBin = 7;
                        else
                            numTestRangeBin = 10 - ii;
                    }
                     else  //4tx/4rx, <=8> doppler bin, upto 1024 range bin
                    {
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
                        if (ii < 3)
                            numTestRangeBin = 7;
                        else
                            numTestRangeBin = 9 - ii;
#endif
                    }
// #endif

                    for (jj = 0; jj < numTestRangeBin; jj++)
                    {
                        for (cluttRemFlag = 0; cluttRemFlag <2; cluttRemFlag++)
                        {

                            checkResults = 1;

                            testConfig.staticClutterRemovalEnabled = (bool) cluttRemFlag;

                            if (numDopplerBin[ii] == 2)
                                testConfig.numChirps = 2;
                            else if (numDopplerBin[ii] == 4)
                            {
                                testConfig.numChirps = 3 + (jj & 0x1);
                            }
                            else if (numDopplerBin[ii] == 8)
                            {
                                testConfig.numChirps = 5 + (jj & 0x3);
                            }
                            else if (numDopplerBin[ii] == 16)
                            {
                                testConfig.numChirps = 9 + (jj & 0x7);
                            }
                            else
                            {
                                if ((jj & 0x3) == 0)
                                    testConfig.numChirps = maxNumChirps;
                                else
                                {
                                    testConfig.numChirps = maxNumChirps - (ii + 1) * 4 - jj / 2 * 2;
                                }

                                if (testConfig.numChirps == preNumChirps)
                                    testConfig.numChirps++;
                                if (testConfig.numChirps > testConfig.numDopplerBins)
                                    testConfig.numChirps = testConfig.numDopplerBins;

                                if ((numTestRangeBin == 2) && (jj) && (maxNumChirps == numDopplerBin[ii]))        //for doppler size is 512, add an odd number chirp tests
                                    testConfig.numChirps++;
                            }

                            preNumChirps = testConfig.numChirps;

                            if ( (testCount & 0x7 )== 0) /* mod (8) ==0, set to rec window*/
                                testConfig.winType = 1;
                            else
                                testConfig.winType = 0; //hanning window


                            if (testConfig.numChirps == 2)
                                testConfig.winType = 1; //if only 2 chirp samples, can only use rec window, hanning window coefficients are all zero.

                            /* generate the windowing factors*/
                            Test_window2DCoef_gen(testConfig.numChirps, testConfig.winType);

                            testConfig.numRangeBins = numRangeBin[jj];
                            testConfig.winSymFlag = (ii + jj + txAntIdx + rxAntIdx) & 0x1;

                            test_print("\nTest #%d  start\r\n", testCount);

                              /* set up test profile based on each test*/
                            Test_setProfile(&testConfig);

                            /* Calculate same accumulator scale parameters for DC estimation as HWA does */
                            Test_calcDCEstimParams(&testConfig);

                            sprintf(featureName, "%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d",
                                ":Tx", testConfig.numTxAntennas,
                                " Rx", testConfig.numRxAntennas,
                                " Chirp", testConfig.numChirps,
                                " R", testConfig.numRangeBins,
                                " D", testConfig.numDopplerBins,
                                " WinSym", testConfig.winSymFlag,
                                " 1stFftScale", dopplerProcDpuCfg.hwRes.hwaCfg.firstStageScaling,
                                " ClutterRemov", testConfig.staticClutterRemovalEnabled);

                            test_print("Test feature : %s\n", featureName);

                            /* generate test 1D fft output*/
                            Test_cubedata_gen(&testConfig);

#if (defined(SUBSYS_DSS) || defined(SUBSYS_MSS))
                            CacheP_wbInv((void *)&testRadarCube,TEST_MAX_RADAR_CUBE_SIZE * sizeof(cmplx16ImRe_t), CacheP_TYPE_ALLD);
                            CacheP_wbInv((void *)&calculatedDetMatrix, TEST_MAX_DETMATRIX_SIZE*sizeof(int16_t), CacheP_TYPE_ALLD);
#endif

                            /* call the doppler dpu*/
                            startTime = ClockP_getTimeUsec();
                            retVal = DPU_DopplerProcHWA_config(dopplerProcDpuHandle, &dopplerProcDpuCfg);
                            if (retVal < 0)
                            {
                                test_print("doppler DPU failed to configure. Error %d.\n", retVal);
                                DebugP_assert(0);
                                return;
                            }
                            bench1 = ClockP_getTimeUsec() - startTime;

                            test_print("Doppler DPU configured. Cycles = %d\n", bench1);

                            startTime = ClockP_getTimeUsec();
                            retVal = DPU_DopplerProcHWA_process(dopplerProcDpuHandle, &out);
                            if (retVal < 0)
                            {
                                test_print("doppler DPU failed to run. Error %d.\n", retVal);
                                DebugP_assert(0);
                                return;
                            }
                            bench2 = ClockP_getTimeUsec() - startTime;
                            test_print("Doppler DPU Process. Cycles = %d. \n", bench2);

                            if (checkResults)
                            {
                                /******************************/
                                /* CHECK RESULTS              */
                                /******************************/
                                Test_printResults(&testConfig);
                                test_print("Test #%d finished!\n\r", testCount);
                            }
                            else
                            {
                                test_print("Test #%d crashes, KNOW ISSUE!\n\r", testCount);
                            }

                            taskTime = ClockP_getTimeUsec();

                            /* System_printf does not directly support length modifiers */
                            snprintf(cycleSummaryText, sizeof (cycleSummaryText),
                                    "TEST TIME cycles: %lld\n\r", taskTime-taskStartTime);
                            test_print("%s", cycleSummaryText);

                            testCount++;
                        }// end of cluttRemFlag for loop
                    }// end of jj for loop
                }

            }
        }
    }

    HWA_close(hwaHandle);

    retVal = DPU_DopplerProcHWA_deinit(dopplerProcDpuHandle);
    if (retVal < 0)
    {
        test_print("doppler DPU failed to deinit. Error %d.\n", retVal);
        DebugP_assert(0);
        return;
    }



    test_print("----------------------------\n\n");
    taskEndTime=ClockP_getTimeUsec();
    test_print("Total Test Time Cycles: %lld\n",taskEndTime-taskStartTime);
    test_print("All Test Finished\n");
    if (finalResults == 1)
    {
        MCPI_setFeatureTestResult("doppler_DPU", MCPI_TestResult_PASS);
        test_print("All Tests PASSED!\n");
        test_print ("All tests have passed!!\n");
    }
    else
    {
        test_print("Test FAILED!\n");
        MCPI_setFeatureTestResult("doppler_DPU", MCPI_TestResult_FAIL);
    }
    MCPI_setTestResult();

    Board_driversClose();
    Drivers_close();

    vTaskDelete(NULL);


}

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
}
