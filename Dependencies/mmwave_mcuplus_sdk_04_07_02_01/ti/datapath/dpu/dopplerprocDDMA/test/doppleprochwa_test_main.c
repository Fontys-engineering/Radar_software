/**
 *   @file  dopplerprochwaDDMA_test_main.c
 *
 *   @brief
 *      Unit Test code for doppler DDMA processing.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 - 2026 Texas Instruments, Inc.
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

#ifdef SUBSYS_DSS
#include <ti/datapath/dpu/dopplerprocDDMA/test/dssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/dopplerprocDDMA/test/dssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/dopplerprocDDMA/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/dopplerprocDDMA/test/dssgenerated/ti_board_open_close.h>
#elif SUBSYS_M4
#include <ti/datapath/dpu/dopplerprocDDMA/test/m4generated/ti_drivers_config.h>
#include <ti/datapath/dpu/dopplerprocDDMA/test/m4generated/ti_board_config.h>
#include <ti/datapath/dpu/dopplerprocDDMA/test/m4generated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/dopplerprocDDMA/test/m4generated/ti_board_open_close.h>
#endif

#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/CycleCounterP.h>
#ifndef SUBSYS_M4
#include "FreeRTOS.h"
#include "task.h"
#endif
/* mmwave SDK files */
#include <ti/common/syscommon.h>
#include <ti/datapath/dpu/dopplerprocDDMA/dopplerprochwaDDMA.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/utils/randomdatagenerator/gen_rand_data.h>
#include <ti/utils/fft/fft.h>

#if defined(SOC_AWR2X44P)
#include <ti/demo/awr2x44P/mmw_ddm/mmw_resDDM.h>
#elif defined(SOC_AWR294X)
#include <ti/demo/awr294x/mmw/mmw_resDDM.h>
#endif

#ifndef SUBSYS_M4
/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (5U)
#define APP_TASK_STACK_SIZE  (8 * 1024)
#endif

#define DPU_DopplerProcHWATest_MAX_NUM_OBJECTS 500
#define DPU_DopplerProcHWATest_MAX_NUM_CFAR_PEAKS 500
#define DPU_DopplerProcHWATest_DPU_DOPPLERPROCHWADDMA_PARAMSET_START_IDX 0
#define DPU_DopplerProcHWATest_HWA_MAX_WINDOW_RAM_SIZE_IN_SAMPLES (CSL_DSS_HWA_WINDOW_RAM_U_SIZE >> 3)
#define DPC_DPU_DOPPLERPROC_FFT_WINDOW_TYPE          MATHUTILS_WIN_HANNING
#define DPC_USE_SYMMETRIC_WINDOW_DOPPLER_DPU
#define MAX_NUM_RX_ANTENNAS 4

/* Max permissible error values */
#define MAX_RANGEIDX_ERR    0
#define MAX_DOPIDX_ERR  0
#define MAX_AZIMIDX_ERR 0
#define MAX_ELEVSAMP_ERR    0
#define MAX_AZIMSAMP_ERR    0
#define MAX_AZIMPEAKSAMP_ERR    0
#define DOPPLER_TEST_ABS(x) (((x) >= 0) ? (x) : -(x))

/*! L3 RAM buffer for object detection DPC */
#ifdef SUBSYS_M4
uint8_t gL3Ram[(0x150000)] __attribute__((aligned(4096), section(".l3ram")));
#endif
#ifdef SUBSYS_DSS
uint8_t gL3Ram[(0x150000)] __attribute__((aligned(4096)));
#pragma DATA_ALIGN(gL3Ram, 4096U);
#pragma DATA_SECTION(gL3Ram, ".l3ram");
#endif

/*! Ideal det obj output */
#ifdef SUBSYS_M4
DetObjParams gDetObjIdealResult[DPU_DopplerProcHWATest_MAX_NUM_OBJECTS] __attribute__((aligned(4096), section(".l3ram")));
#endif
#ifdef SUBSYS_DSS
DetObjParams gDetObjIdealResult[DPU_DopplerProcHWATest_MAX_NUM_OBJECTS];
#pragma DATA_ALIGN(gDetObjIdealResult, 4096U);
#pragma DATA_SECTION(gDetObjIdealResult, ".l3ram");
#endif

uint8_t gIdealObjFoundList[DPU_DopplerProcHWATest_MAX_NUM_OBJECTS];

/*! L2 RAM buffer for object detection DPC */
#define MMWDEMO_OBJDET_L2RAM_SIZE (107U * 1024U)

uint32_t gcfarThreshScaleLUT[192U];

#ifdef SUBSYS_M4
uint8_t gL2Heap[MMWDEMO_OBJDET_L2RAM_SIZE] __attribute__((aligned(4096), section(".dpc_l2Heap")));
#endif
#ifdef SUBSYS_DSS
uint8_t gL2Heap[MMWDEMO_OBJDET_L2RAM_SIZE];
#pragma DATA_ALIGN(gL2Heap, 4096U);
#pragma DATA_SECTION(gL2Heap, ".dpc_l2Heap");
#endif
/*
 * @brief Memory Configuration used during init API
 */
typedef struct DPU_DopplerProcHWATest_MemCfg_t
{
    /*! @brief   Start address of memory provided by the application
     *           from which DPC will allocate.
     */
    void *addr;

    /*! @brief   Size limit of memory allowed to be consumed by the DPC */
    uint32_t size;
} DPU_DopplerProcHWATest_MemCfg;

/*
 * @brief Memory pool object to manage memory
 */
typedef struct MemPoolObj_t
{
    /*! @brief Memory configuration */
    DPU_DopplerProcHWATest_MemCfg cfg;

    /*! @brief   Pool running adress.*/
    uintptr_t currAddr;

    /*! @brief   Pool max address. This pool allows setting address to desired
     *           (e.g for rewinding purposes), so having a running maximum
     *           helps in finding max pool usage
     */
    uintptr_t maxCurrAddr;
} MemPoolObj;

/* Error codes */
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__L3_RAM_RADAR_CUBE -1
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__L3_RAM_DET_MATRIX -2
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__OBJ_PARAMS_RAM_DOPPLER_DECOMP_BUF -3
#define DPU_DOPPLERPROCHWA_DDMA_TEST_EINVAL -5
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_DECOMP_BUF -6
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_DOPFFT_SUBMAT  -7
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_MAXDOP_SUBBAND -8
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_AZIMFFT_SCRATCH -10
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_CFAR_SCRATCH -11
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_LOCALMAX_SCRATCH -12
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_HWA_WINDOW -13
#define DPU_DOPPLERPROCHWA_DDMA_TEST_WIN_ERR -14
#define DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM_HWA_WINDOW_RAM -15

/* Interrupt object for dopplerProc */
Edma_IntrObject         dopplerProcIntrObj[DOPPLERPROCHWADDMA_NUM_EDMA_INTERRUPTS];

/*! @brief L3 ram memory pool object */
MemPoolObj    L3ramObjS;
MemPoolObj *  L3ramObj = &L3ramObjS;

/*! @brief Core Local ram memory pool object */
MemPoolObj    CoreLocalRamObjS;
MemPoolObj *  CoreLocalRamObj = &CoreLocalRamObjS;

#ifndef SUBSYS_M4
TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));
#endif
#define PI 3.141592653589793

/* Test structure */
typedef struct dopplerProcDpuTestConfig_t {
    DPU_DopplerProcHWA_StaticConfig dopTestStaticCfg;
    DPU_DopplerProcHWA_HW_Resources hwRes;
    uint32_t                        numBytesToRead;
    bool                            readDataAgain;
}dopplerProcDpuTestConfig_t;

/* Global structures */
dopplerProcDpuTestConfig_t testConfigStruct;
dopplerProcDpuTestConfig_t * testConfig = &testConfigStruct;

HWA_Handle  hwaHandle;
volatile uint8_t    subframeCount = 0U;
/* test configuration */
DPU_DopplerProcHWA_Handle    dopplerProcDpuHandle __attribute__((aligned(32)));
DPU_DopplerProcHWA_Config    dopTestConfig __attribute__((aligned(32)));
Edma_IntrObject   intrObj;

uint8_t finalResults;
uint8_t checkResults;

void dopplerProc_test_hwainit()
{
    int32_t             errorCode;

    hwaHandle = HWA_open(0, NULL, &errorCode);
    if (hwaHandle == NULL)
    {
        DebugP_log("HWA failed to open\n");
        DebugP_assert(0);
        return;
    }
}

void dopplerProc_test_dpuInit()
{
    int32_t errorCode;
    DPU_DopplerProcHWA_InitParams   initCfg;

    initCfg.hwaHandle = hwaHandle;
    dopplerProcDpuHandle = DPU_DopplerProcHWA_init(&initCfg,subframeCount, &errorCode);
    if (dopplerProcDpuHandle == NULL)
    {
        DebugP_log("doppler DPU failed to initialize. Error %d.\n", errorCode);
        return;
    }
}

void dopplerProc_test_hwaDeinit()
{
    int32_t             errorCode=0;

    errorCode = HWA_close(hwaHandle);
    if (errorCode != 0)
    {
        DebugP_log("HWA failed to close\n");
        DebugP_assert(0);
        return;
    }
}

void dopplerProc_test_dpuDeinit()
{
    int32_t errorCode =0;
    errorCode = DPU_DopplerProcHWA_deinit(dopplerProcDpuHandle);
    if (errorCode != 0)
    {
        DebugP_log("doppler DPU failed to deinitialize. Error %d.\n", errorCode);
        return;
    }
}

/**
 *  @b Description
 *  @n
 *      Allocates Shawdow paramset
 */
static void allocateEDMAShadowChannel(EDMA_Handle edmaHandle, uint32_t *param)
{
    int32_t             testStatus = SystemP_SUCCESS;
    EDMA_Config        *config;
    EDMA_Object        *object;

    config = (EDMA_Config *) edmaHandle;
    object = config->object;

    if((object->allocResource.paramSet[*param/32] & (1U << *param%32)) != (1U << *param%32))
    {
        testStatus = EDMA_allocParam(edmaHandle, param);
        DebugP_assert(testStatus == SystemP_SUCCESS);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      Utility function for reseting memory pool.
 *
 *  @param[in]  pool Handle to pool object.
 *
 *  \ingroup DPU_DopplerProcHWATest__INTERNAL_FUNCTION
 *
 *  @retval
 *      none.
 */
static void DPU_DopplerProcHWATest_MemPoolReset(MemPoolObj *pool)
{
    pool->currAddr = (uintptr_t)pool->cfg.addr;
    pool->maxCurrAddr = pool->currAddr;
}

#if 0
/**
 *  @b Description
 *  @n
 *      Utility function for setting memory pool to desired address in the pool.
 *      Helps to rewind for example.
 *
 *  @param[in]  pool Handle to pool object.
 *  @param[in]  addr Address to assign to the pool's current address.
 *
 *  \ingroup DPU_DopplerProcHWATest__INTERNAL_FUNCTION
 *
 *  @retval
 *      None
 */
static void DPU_DopplerProcHWATest_MemPoolSet(MemPoolObj *pool, void *addr)
{
    pool->currAddr = (uintptr_t)addr;
    pool->maxCurrAddr = CSL_MAX(pool->currAddr, pool->maxCurrAddr);
}
#endif

/**
 *  @b Description
 *  @n
 *      Utility function for getting memory pool current address.
 *
 *  @param[in]  pool Handle to pool object.
 *
 *  \ingroup DPU_DopplerProcHWATest__INTERNAL_FUNCTION
 *
 *  @retval
 *      pointer to current address of the pool (from which next allocation will
 *      allocate to the desired alignment).
 */
static void *DPU_DopplerProcHWATest_MemPoolGet(MemPoolObj *pool)
{
    return((void *)pool->currAddr);
}

#if 0 /* may be useful in future */
/**
 *  @b Description
 *  @n
 *      Utility function for getting current memory pool usage.
 *
 *  @param[in]  pool Handle to pool object.
 *
 *  @retval
 *      Amount of pool used in bytes.
 */
static uint32_t DPU_DopplerProcHWATest_MemPoolGetCurrentUsage(MemPoolObj *pool)
{
    return((uint32_t)(pool->currAddr - (uintptr_t)pool->cfg.addr));
}
#endif

#if 0
/**
 *  @b Description
 *  @n
 *      Utility function for getting maximum memory pool usage.
 *
 *  @param[in]  pool Handle to pool object.
 *
 *  \ingroup DPU_DopplerProcHWATest__INTERNAL_FUNCTION
 *
 *  @retval
 *      Amount of pool used in bytes.
 */
static uint32_t DPU_DopplerProcHWATest_MemPoolGetMaxUsage(MemPoolObj *pool)
{
    return((uint32_t)(pool->maxCurrAddr - (uintptr_t)pool->cfg.addr));
}
#endif

/**
 *  @b Description
 *  @n
 *      Utility function for allocating from a static memory pool.
 *
 *  @param[in]  pool Handle to pool object.
 *  @param[in]  size Size in bytes to be allocated.
 *  @param[in]  align Alignment in bytes
 *
 *  \ingroup DPU_DopplerProcHWATest__INTERNAL_FUNCTION
 *
 *  @retval
 *      pointer to beginning of allocated block. NULL indicates could not
 *      allocate.
 */
static void *DPU_DopplerProcHWATest_MemPoolAlloc(MemPoolObj *pool,
                              uint32_t size,
                              uint8_t align)
{
    void *retAddr = NULL;
    uintptr_t addr;

    addr = CSL_MEM_ALIGN(pool->currAddr, align);
    if ((addr + size) <= ((uintptr_t)pool->cfg.addr + pool->cfg.size))
    {
        retAddr = (void *)addr;
        pool->currAddr = addr + size;
        pool->maxCurrAddr = CSL_MAX(pool->currAddr, pool->maxCurrAddr);
    }

    return(retAddr);
}

/**
 *  @b Description
 *  @n
 *     Function calls EDMA param, channel, tcc allocation.
 *     DDMA Datapath assumes paramsetNumber = channelNumber = TCC
 *
 *  @param[in]  handle   EDMA handle
 *  @param[in]  chNum    DMA channel number
 *  @param[in]  shadowParamId    DMA shadow paramId
 *  @param[in]  eventQueue    Event queue num
 *  @param[out]  chanCfg    Stores channel configuration
 *  @retval   None
 *
 */
void DPU_DopplerProcHWATest_EDMAChannelConfigAssist(EDMA_Handle handle, uint32_t chNum, uint32_t shadowParam, uint32_t eventQueue, DPEDMA_ChanCfg *chanCfg)
{

    DebugP_assert(chanCfg != NULL);

    DPEDMA_allocateEDMAChannel(handle, &chNum, &chNum, &chNum);

    chanCfg->channel = chNum;
    chanCfg->tcc = chNum;
    chanCfg->paramId = chNum;

    chanCfg->shadowPramId = shadowParam;

    allocateEDMAShadowChannel(handle, &shadowParam);

    chanCfg->eventQueue = eventQueue;

    return;

}

/**
 *  @b Description
 *  @n
 *      Generates doppler window length
 */
static uint32_t DPU_DopplerProcHWATest_GetDopplerWinGenLen(uint32_t numChirps)
{
    uint16_t numDopplerChirps;
    uint32_t winGenLen;

    numDopplerChirps = numChirps;

    winGenLen = (numDopplerChirps + 1)/2;

    return(winGenLen);
}

/**
 *  @b Description
 *  @n
 *      Converts CFAR to linear value
 */
static uint16_t MmwDemo_convertCfarToLinear(uint16_t codedCfarVal)
{
    uint16_t linearVal;
    float    dbVal, linVal;

    dbVal = (float)(codedCfarVal);

    linVal = log2(pow(10, (dbVal / 20.0))) * (1 << 11) + 0.5;

    linearVal = (uint16_t) linVal;
    return (linearVal);
}

/**
 *  @b Description
 *  @n
 *      Generates doppler window
 */
static uint32_t DPU_DopplerProcHWATest_GenDopplerWindow(DPU_DopplerProcHWA_HW_Resources *hwRes, uint32_t numChirps)
{
    uint32_t winType;

    /* For too small window, force rectangular window to avoid loss of information
     * due to small window values (e.g. hanning has first and last coefficients 0) */
    if (numChirps <= 4)
    {
        winType = MATHUTILS_WIN_RECT;
    }
    else
    {
        winType = DPC_DPU_DOPPLERPROC_FFT_WINDOW_TYPE;
    }

    mathUtils_genWindow((uint32_t *)hwRes->hwaCfg.window,
                        numChirps,
                        DPU_DopplerProcHWATest_GetDopplerWinGenLen(numChirps),
                        winType,
                        17);

    return(winType);
}

/**
 *  @b Description
 *  @n
 *      Populates the config structure (HW Res allocation, static config, etc.)
 */
int32_t dopplerProc_test_dpuConfig()
{

    DPU_DopplerProcHWA_StaticConfig * staticCfg;
    DPU_DopplerProcHWA_HW_Resources * hwRes;

    staticCfg = &testConfig->dopTestStaticCfg;

    memcpy(staticCfg, &testConfig->dopTestStaticCfg, sizeof(DPU_DopplerProcHWA_StaticConfig));

    DPU_DopplerProcHWATest_MemPoolReset(L3ramObj);
    DPU_DopplerProcHWATest_MemPoolReset(CoreLocalRamObj);

    DPIF_RadarCube radarCube;
    DPIF_DetMatrix detMatrix;

    uint32_t *windowBuffer, winGenLen, winType;
    void * scratchBufMem;
    uint8_t pingPongIdx;
    int32_t retVal = 0;
    void * l3RamStartPoolAddrNextDPU;
    void * CoreLocalScratchStartPoolAddrNextDPU;
    EDMA_Handle edmaHandle = gEdmaHandle[0];
    uint32_t objOutSizeInBytes, detObjListSizeInBytes;
    uint32_t windowOffset = 0;
    float temp;

    hwRes = &testConfig->hwRes;
    DPU_DopplerProcHWA_EdmaCfg *edmaCfg = &hwRes->edmaCfg;
    DPU_DopplerProcHWA_HwaCfg *hwaCfg = &hwRes->hwaCfg;

    hwRes->edmaCfg.edmaHandle = gEdmaHandle[CONFIG_EDMA0];

    /* L3 allocations */
    /* L3 - radar cube */
    /* Input and output samples out of the rangeproc/compression DPU */
    uint32_t inputBytesPerBlock, outputBytesPerBlock, radarCubeDecompressedSizeInBytes;
    float achievedCompressionRatio;
    if(staticCfg->decompCfg.compressionMethod==HWA_COMPRESS_METHOD_BFP)
        inputBytesPerBlock = 4 * staticCfg->decompCfg.rangeBinsPerBlock;
    else
        inputBytesPerBlock = 4 * staticCfg->decompCfg.numRxAntennaPerBlock * staticCfg->decompCfg.rangeBinsPerBlock;

    temp = (((staticCfg->decompCfg.compressionRatio * (float)inputBytesPerBlock) + 3.99F) / 4.0F);
    outputBytesPerBlock = (uint16_t)temp * 4U; /* Word aligned */
    achievedCompressionRatio = (float) outputBytesPerBlock / (float) inputBytesPerBlock;

    radarCubeDecompressedSizeInBytes = staticCfg->numRangeBins * staticCfg->numChirps *
                                        staticCfg->numRxAntennas * sizeof(cmplx16ReIm_t);
    radarCube.dataSize =  radarCubeDecompressedSizeInBytes * achievedCompressionRatio;
    radarCube.data = DPU_DopplerProcHWATest_MemPoolAlloc(L3ramObj, radarCube.dataSize,
                                             sizeof(uint16_t));
    if (radarCube.data == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__L3_RAM_RADAR_CUBE;
        goto exit;
    }
    radarCube.datafmt = DPIF_RADARCUBE_FORMAT_2;

    /* L3 - detection matrix */
    detMatrix.dataSize = staticCfg->numRangeBins * (staticCfg->numDopplerFFTBins/staticCfg->numBandsTotal) * sizeof(uint16_t);
    detMatrix.data = DPU_DopplerProcHWATest_MemPoolAlloc(L3ramObj, detMatrix.dataSize,
                                             sizeof(uint16_t));
    if (detMatrix.data == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__L3_RAM_DET_MATRIX;
        goto exit;
    }
    detMatrix.datafmt = DPIF_DETMATRIX_FORMAT_1;

    /********************************************
     * Allocating memory resources              *
     *******************************************/

    /* DPU Output Resource */
    detObjListSizeInBytes = sizeof(DetObjParams) * DPU_DopplerProcHWATest_MAX_NUM_OBJECTS;
    scratchBufMem = DPU_DopplerProcHWATest_MemPoolAlloc(L3ramObj, detObjListSizeInBytes, sizeof(uint32_t));
    if (scratchBufMem == NULL){
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__OBJ_PARAMS_RAM_DOPPLER_DECOMP_BUF;
        goto exit;
    }
    hwRes->detObjList = (DetObjParams *)scratchBufMem;
    hwRes->maxObjListPerRGateSize = MAX_NUM_OBJ_PER_RANGE_BIN * sizeof(DetObjParams);

    objOutSizeInBytes = sizeof(DPIF_PointCloudCartesian) * DPU_DopplerProcHWATest_MAX_NUM_OBJECTS;
    scratchBufMem = DPU_DopplerProcHWATest_MemPoolAlloc(L3ramObj, objOutSizeInBytes, sizeof(uint32_t));
    if (scratchBufMem == NULL){
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__OBJ_PARAMS_RAM_DOPPLER_DECOMP_BUF;
        goto exit;
    }
    hwRes->objOut = (DPIF_PointCloudCartesian *)scratchBufMem;

    l3RamStartPoolAddrNextDPU = DPU_DopplerProcHWATest_MemPoolGet(L3ramObj);
    if (l3RamStartPoolAddrNextDPU == NULL){
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_EINVAL;
        goto exit;
    }

    /* We don't need any L2 resources to be retained till the end of the next DPU */
    CoreLocalScratchStartPoolAddrNextDPU = DPU_DopplerProcHWATest_MemPoolGet(CoreLocalRamObj);
    if (CoreLocalScratchStartPoolAddrNextDPU == NULL){
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_EINVAL;
        goto exit;
    }
    /* Resources to be saved through the doppler stage */
    {{
    /* This resource needs to be saved through the doppler stage */
    if (staticCfg->decompCfg.rangeBinsPerBlock < 8U)
    {
        hwRes->decompScratchBufferSizeBytes = radarCubeDecompressedSizeInBytes /
                                (staticCfg->numRangeBins / 8U);
    }
    else
    {
        hwRes->decompScratchBufferSizeBytes = radarCubeDecompressedSizeInBytes /
                                 (staticCfg->numRangeBins / staticCfg->decompCfg.rangeBinsPerBlock);
    }
    scratchBufMem = DPU_DopplerProcHWATest_MemPoolAlloc(L3ramObj, hwRes->decompScratchBufferSizeBytes, sizeof(uint32_t));
    if (scratchBufMem == NULL){
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_DECOMP_BUF;
        goto exit;
    }
    hwRes->decompScratchBuf = (uint8_t *)scratchBufMem;

    /* Allocate memory for Max Doppler Sub Band Buffers */
    hwRes->dopMaxSubBandScratchBufferSizeBytes = (staticCfg->numDopplerFFTBins / staticCfg->numBandsTotal) * sizeof(uint8_t) * 2; /* Ping and Pong */
    for(pingPongIdx = 0; pingPongIdx < 2; pingPongIdx++){
        scratchBufMem = DPU_DopplerProcHWATest_MemPoolAlloc(CoreLocalRamObj, hwRes->dopMaxSubBandScratchBufferSizeBytes / 2, sizeof(uint32_t));
        if (scratchBufMem == NULL){
            retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_MAXDOP_SUBBAND;
            goto exit;
        }
        hwRes->dopMaxSubBandScratchBuf[pingPongIdx] = (uint8_t *)scratchBufMem;
    }


#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    hwRes->maxCfarPeaksToDetect = DPU_DopplerProcHWATest_MAX_NUM_CFAR_PEAKS;

    hwRes->cfarThreshScaleLUT = gcfarThreshScaleLUT;
#endif

    /* Assign the detection matrix, radar cube */
    hwRes->detMatrix = detMatrix;
    hwRes->radarCube = radarCube;

    }}
    /********************************************
     * Allocating hw resources (decomp stage)   *
     *******************************************/
    {{
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DECOMP_IN_PING,
                                       EDMA_DOPPLERPROC_DECOMP_IN_PING_SHADOW,
                                       0,
                                       &edmaCfg->decompEdmaCfg.edmaIn.pingPong[0]);
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DECOMP_IN_PONG,
                                       EDMA_DOPPLERPROC_DECOMP_IN_PONG_SHADOW,
                                       0,
                                       &edmaCfg->decompEdmaCfg.edmaIn.pingPong[1]);

    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DECOMP_OUT_PING,
                                       EDMA_DOPPLERPROC_DECOMP_OUT_PING_SHADOW,
                                       0,
                                       &edmaCfg->decompEdmaCfg.edmaOut.pingPong[0]);
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DECOMP_OUT_PONG,
                                       EDMA_DOPPLERPROC_DECOMP_OUT_PONG_SHADOW,
                                       0,
                                       &edmaCfg->decompEdmaCfg.edmaOut.pingPong[1]);

    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DECOMP_IN_HOTSIG_PING,
                                       EDMA_DOPPLERPROC_DECOMP_IN_HOTSIG_PING_SHADOW,
                                       0,
                                       &edmaCfg->decompEdmaCfg.edmaInSignature.pingPong[0]);
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DECOMP_IN_HOTSIG_PONG,
                                       EDMA_DOPPLERPROC_DECOMP_IN_HOTSIG_PONG_SHADOW,
                                       0,
                                       &edmaCfg->decompEdmaCfg.edmaInSignature.pingPong[1]);

    hwaCfg->decompStageHwaStateMachineCfg.paramSetStartIdx = DPU_DopplerProcHWATest_DPU_DOPPLERPROCHWADDMA_PARAMSET_START_IDX;
    hwaCfg->decompStageHwaStateMachineCfg.numParamSets = DPU_DOPPLERPOCHWADDMA_DECOMP_NUM_HWA_PARAMSETS + staticCfg->decompCfg.bfpCompExtraParamSets;

    }}

    /********************************************
     * Allocating hw resources (doppler stage)  *
     *******************************************/
    {{

    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DOPPLER_IN_PING,
                                       EDMA_DOPPLERPROC_DOPPLER_IN_PING_SHADOW,
                                       0,
                                       &edmaCfg->dopplerEdmaCfg.edmaIn.pingPong[0]);
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DOPPLER_IN_PONG,
                                       EDMA_DOPPLERPROC_DOPPLER_IN_PONG_SHADOW,
                                       0,
                                       &edmaCfg->dopplerEdmaCfg.edmaIn.pingPong[1]);

    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DOPPLER_IN_HOTSIG_PING,
                                       EDMA_DOPPLERPROC_DOPPLER_IN_HOTSIG_PING_SHADOW,
                                       0,
                                       &edmaCfg->dopplerEdmaCfg.edmaInSignature.pingPong[0]);
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_DOPPLER_IN_HOTSIG_PONG,
                                       EDMA_DOPPLERPROC_DOPPLER_IN_HOTSIG_PONG_SHADOW,
                                       0,
                                       &edmaCfg->dopplerEdmaCfg.edmaInSignature.pingPong[1]);

    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_MAXSUBBAND_OUT_PING,
                                       EDMA_DOPPLERPROC_MAXSUBBAND_OUT_PING_SHADOW,
                                       0,
                                       &edmaCfg->dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[0]);
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_MAXSUBBAND_OUT_PONG,
                                       EDMA_DOPPLERPROC_MAXSUBBAND_OUT_PONG_SHADOW,
                                       0,
                                       &edmaCfg->dopplerEdmaCfg.edmaMaxSubbandOut.pingPong[1]);


    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_SUMTX_OUT_PING,
                                       EDMA_DOPPLERPROC_SUMTX_OUT_PING_SHADOW,
                                       0,
                                       &edmaCfg->dopplerEdmaCfg.edmaSumLogAbsOut.pingPong[0]);
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_SUMTX_OUT_PONG,
                                       EDMA_DOPPLERPROC_SUMTX_OUT_PONG_SHADOW,
                                       0,
                                       &edmaCfg->dopplerEdmaCfg.edmaSumLogAbsOut.pingPong[1]);


    hwaCfg->dopplerStageHwaStateMachineCfg.paramSetStartIdx = hwaCfg->decompStageHwaStateMachineCfg.paramSetStartIdx
                                                            + hwaCfg->decompStageHwaStateMachineCfg.numParamSets;
    if(staticCfg->isSumTxEnabled){
        hwaCfg->dopplerStageHwaStateMachineCfg.numParamSets = DPU_DOPPLERPOCHWADDMA_DOPPLER_NUM_HWA_PARAMSETS;
    }
    else{
        hwaCfg->dopplerStageHwaStateMachineCfg.numParamSets = DPU_DOPPLERPOCHWADDMA_DOPPLER_NUM_HWA_PARAMSETS - DPU_DOPPLERPOCHWADDMA_SUMTX_NUM_HWA_PARAMSETS;
    }

    }}

    /********************************************
     * Allocating hw resources (azim stage)     *
     *******************************************/
    {{
    /* Allocate the EDMA channel to copy the antenna samples of detected object. */
    DPU_DopplerProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_DOPPLERPROC_EXTRACT_OBJECT_LIST,
                                       EDMA_DOPPLERPROC_EXTRACT_OBJECT_LIST_SHADOW,
                                       0,
                                       &edmaCfg->edmaDetObjAntSamples);

    hwaCfg->azimCfarStageHwaStateMachineCfg.paramSetStartIdx = hwaCfg->dopplerStageHwaStateMachineCfg.paramSetStartIdx
                                                             + hwaCfg->dopplerStageHwaStateMachineCfg.numParamSets;
    hwaCfg->azimCfarStageHwaStateMachineCfg.numParamSets = DPU_DOPPLERPOCHWADDMA_AZIM_NUM_HWA_PARAMSETS + 2 * (staticCfg->numRxAntennas - MAX_NUM_RX_ANTENNAS);

    }}

    {{

        uint32_t intrIdx = 0;

        /* Allocating interrupt objects */
        edmaCfg->decompEdmaCfg.edmaIntrObjDecompOut                  = &dopplerProcIntrObj[intrIdx++];

        edmaCfg->dopplerEdmaCfg.edmaIntrObjMaxSubbandOut.pingPong[0] = &dopplerProcIntrObj[intrIdx++];
        edmaCfg->dopplerEdmaCfg.edmaIntrObjMaxSubbandOut.pingPong[1] = &dopplerProcIntrObj[intrIdx++];
    }}

    /* hwaCfg - window */
    winGenLen = DPU_DopplerProcHWATest_GetDopplerWinGenLen(staticCfg->numChirps);
    hwaCfg->windowSize = winGenLen * sizeof(int32_t);
    windowBuffer = DPU_DopplerProcHWATest_MemPoolAlloc(CoreLocalRamObj, hwaCfg->windowSize, sizeof(uint32_t));
    if (windowBuffer == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_DOPPLER_HWA_WINDOW;
        goto exit;
    }
    hwaCfg->window = (int32_t *)windowBuffer;
    hwaCfg->winRamOffset = (uint16_t) windowOffset;
    winType = DPU_DopplerProcHWATest_GenDopplerWindow(hwRes, staticCfg->numChirps);
    if(winType != DPC_DPU_DOPPLERPROC_FFT_WINDOW_TYPE){
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_WIN_ERR;
        goto exit;
    }

#ifdef DPC_USE_SYMMETRIC_WINDOW_DOPPLER_DPU
    hwaCfg->winSym = HWA_FFT_WINDOW_SYMMETRIC;
#else
    hwaCfg->winSym = HWA_FFT_WINDOW_NONSYMMETRIC;
#endif
    if ((hwaCfg->winRamOffset + winGenLen) > DPU_DopplerProcHWATest_HWA_MAX_WINDOW_RAM_SIZE_IN_SAMPLES)
    {
        retVal = DPU_DOPPLERPROCHWA_DDMA_TEST_ENOMEM_HWA_WINDOW_RAM;
        goto exit;
    }
    windowOffset += winGenLen;

exit:
    return retVal;

}

/**
 *  @b Description
 *  @n
 *      Compares two DetObjParams structures and returns 0 if within error limits
 */
int32_t compareObjOut(DetObjParams * obj1, DetObjParams * obj2){

    uint32_t nextStep, i;
    if(DOPPLER_TEST_ABS((int32_t)obj1->rangeIdx - (int32_t)obj2->rangeIdx) <= MAX_RANGEIDX_ERR){
        if(DOPPLER_TEST_ABS((int32_t)obj1->dopIdxActual - (int32_t)obj2->dopIdxActual) <= MAX_DOPIDX_ERR){
            if(DOPPLER_TEST_ABS((int32_t)obj1->azimIdx - (int32_t)obj2->azimIdx) <= MAX_AZIMIDX_ERR){
                nextStep = 1;
				for(i = 0; i < ((NUM_TXANT_ELEV) * (NUM_RXANT)); i++){
                    if(DOPPLER_TEST_ABS((int32_t)obj1->elevSamples[i].real - (int32_t)obj2->elevSamples[i].real) > MAX_ELEVSAMP_ERR){
                        nextStep = 0;
                    }
                    if(DOPPLER_TEST_ABS((int32_t)obj1->elevSamples[i].imag - (int32_t)obj2->elevSamples[i].imag) > MAX_ELEVSAMP_ERR){
                        nextStep = 0;
                    }
                }
                if(nextStep){
                    nextStep = 1;
    				for(i = 0; i < ((NUM_TXANT_AZIM) * (NUM_RXANT)); i++){
                        if(DOPPLER_TEST_ABS((int32_t)obj1->azimSamples[i].real - (int32_t)obj2->azimSamples[i].real) > MAX_AZIMSAMP_ERR){
                            nextStep = 0;
                        }
                        if(DOPPLER_TEST_ABS((int32_t)obj1->azimSamples[i].imag - (int32_t)obj2->azimSamples[i].imag) > MAX_AZIMSAMP_ERR){
                            nextStep = 0;
                        }
                    }
                }
                if(nextStep){
                    nextStep = 1;
                    for(i = 0; i < 3; i++){
                        if(DOPPLER_TEST_ABS((int32_t)obj1->azimPeakSamples[i] - (int32_t)obj2->azimPeakSamples[i]) > MAX_AZIMPEAKSAMP_ERR){
                            nextStep = 0;
                        }
                    }
                }
                if(nextStep){
                    return 0;
                }
            }
        }
    }
    
    return 1;

}

/**
 *  @b Description
 *  @n
 *      Compares two DetObjParams lists
 */
int32_t compareResults(DetObjParams * dpuList, DetObjParams * idealList, uint32_t numObjOut, uint32_t numObjOutIdeal){

    int32_t retVal = 0;
    uint32_t i, j, k;
    uint8_t extraObjFound = 0;
    uint32_t finalNumObjOutIdeal = numObjOutIdeal;

    memset(gIdealObjFoundList, 0, DPU_DopplerProcHWATest_MAX_NUM_OBJECTS * sizeof(uint8_t));

    /* Do not worry about the 0 range bin objects */
    for(j = 0; j < numObjOutIdeal; j++){
        if(idealList[j].rangeIdx == 0)
        {
            gIdealObjFoundList[j] = 1;
            finalNumObjOutIdeal --;
        }
    }

    DebugP_log("NumObjOut = %d, NumObjOutIdeal = %d\n", numObjOut, finalNumObjOutIdeal);
    for(i = 0; i < numObjOut; i++){
        extraObjFound = 1;
        for(j = 0; j < numObjOutIdeal; j++){
            if((gIdealObjFoundList[j] == 0) && (compareObjOut(&dpuList[i], &idealList[j]) == 0)){
                gIdealObjFoundList[j] = 1;
                extraObjFound = 0;
                break;
            }
        }
        if(extraObjFound){        
            finalResults=0;
            DebugP_log("Extra Object Found. ObjIdx = %d, RangeBin = %d, DopBinActual = %d, AzimIdx = %d, ",
                    i, dpuList[i].rangeIdx, dpuList[i].dopIdxActual, dpuList[i].azimIdx);
			for(k = 0; k < (NUM_TXANT_AZIM) * (NUM_RXANT); k++)
			{
				DebugP_log("azimSamples[%d] = (%d ,%d) ", k, dpuList[i].azimSamples[k].real,dpuList[i].azimSamples[k].imag);
			}

			for(k = 0; k < (NUM_TXANT_ELEV) * (NUM_RXANT); k++)
			{
				DebugP_log("elevSamples[%d] = (%d ,%d) ", k, dpuList[i].elevSamples[k].real,dpuList[i].elevSamples[k].imag);
			}

			DebugP_log(", azimPeaks = [");
			for(k = 0; k < 3; k++)
			{
				DebugP_log("%d, ", dpuList[i].azimPeakSamples[k]);
			}
			DebugP_log("]\n");
        }
    }

    for(j = 0; j < numObjOutIdeal; j++){
        if(!gIdealObjFoundList[j]){
            finalResults=0;
            DebugP_log("Object Not Found. ObjIdx = %d, RangeBin = %d, DopBinActual = %d, AzimIdx = %d, ",
                    j, idealList[j].rangeIdx, idealList[j].dopIdxActual, idealList[j].azimIdx);


			for(k = 0; k < (NUM_TXANT_AZIM) * (NUM_RXANT); k++)
			{
				DebugP_log("azimSamples[%d] = (%d ,%d) ", k, idealList[j].azimSamples[k].real,idealList[j].azimSamples[k].imag);
			}

			for(k = 0; k < (NUM_TXANT_ELEV) * (NUM_RXANT); k++)
			{
				DebugP_log("elevSamples[%d] = (%d ,%d) ", k, idealList[j].elevSamples[k].real,idealList[j].elevSamples[k].imag);
			}
			DebugP_log(", azimPeaks = [");
			for(k = 0; k < 3; k++)
			{
				DebugP_log("%d, ", idealList[j].azimPeakSamples[k]);
			}
			DebugP_log("]\n");
        }
    }

    DebugP_log("\n");

    return retVal;

}

/******************************************************
    test main task
*******************************************************/
void Test_task(void* args)
{
#if defined(SOC_AWR2943) && defined(ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION)
    DebugP_log("Test vectors not available !\n");
    exit(0);
#endif

    FILE *fileIdTestInfo, *fileIdCompRCube, *fileIdTestResult;
    char fileName[100];
    uint64_t taskStartTime, taskEndTime;
#if defined(SOC_AWR2943)
    char deviceType[] = "awr2943";
    uint16_t antennaGeometryCfg[SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL] = {0,1,2,3,4,5,6,7,8,9,10,11};
#elif defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
    char deviceType[] = "awr2944";
    uint16_t antennaGeometryCfg[SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

#endif

    uint32_t numTests, testIdx, numObjOutIdeal, idealObjIdx, subBandIdx;
    int32_t retVal;
    DPU_DopplerProc_CfarCfg cfarCfg;

    /* opening test configuration file*/
    strcpy(fileName, "testdata\\dopplerData_");
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    strcat(fileName, "hist_");
#endif
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    fileIdTestInfo = fopen(fileName, "rb");
    if (fileIdTestInfo == NULL)
    {
        DebugP_log("Error:  Cannot open the file %s !\n", fileName);
        exit(0);
    }

    /* opening input data (compressed radar cube) file */
    strcpy(fileName, "testdata\\compRCube_");
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    strcat(fileName, "hist_");
#endif
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    fileIdCompRCube = fopen(fileName, "rb");
    if (fileIdCompRCube == NULL)
    {
        DebugP_log("Error:  Cannot open compRCube.bin !\n");
        exit(0);
    }

    /* opening output data (detected object) file */
    strcpy(fileName, "testdata\\detObj_");
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    strcat(fileName, "hist_");
#endif
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    fileIdTestResult = fopen(fileName, "rb");
    if (fileIdTestResult == NULL)
    {
        DebugP_log("Error:  Cannot open detObj.bin !\n");
        exit(0);
    }
    CycleCounterP_reset();

    DPU_DopplerProcHWA_OutParams out;

    Drivers_open();
    Board_driversOpen();

    finalResults = 1;

    taskStartTime = ClockP_getTimeUsec();

    DebugP_log("...... Initialization ...... \n");


    DPU_DopplerProcHWATest_MemCfg L3ramCfg, CoreLocalRamCfg;
    L3ramCfg.addr = (void *)&gL3Ram[0];
    L3ramCfg.size = sizeof(gL3Ram);
    CoreLocalRamCfg.addr = &gL2Heap[0];
    CoreLocalRamCfg.size = sizeof(gL2Heap);

    L3ramObj->cfg = L3ramCfg;
    CoreLocalRamObj->cfg = CoreLocalRamCfg;

    DebugP_log("Doppler Proc DPU Test start ...... \n");

    /* fread numTests
       For every test
            fread num azim tx
                  num elev tx
                  num rx
                  numRangeBins
                  numChirps
                  compression enable
                  compression method
                  compression ratio
                  compression range bins per block
                  compression rx antenna per block
                  cfar thresh scale
                  cfar average mode
                  cfar win length
                  cfar guard length
                  cfar noise div shift
                  cfar cyclic mode
                  cfar peak grouping scheme
                  cfar peak grouping enable
                  cfar osKvalue
                  cfar osEdgeKScaleEn
                  local max azim threshold
                  local max doppler threshold
                  antenna calib params
                  read data again flag
            if read data again flag
                read compressed radar cube data
            read out object list
            perform DPU operations
            compare results */

    fread(&numTests, sizeof(uint32_t), 1, fileIdTestInfo);
    for(testIdx = 0; testIdx < numTests; testIdx++){
        /* hwa, edma, dpu inialization */
        dopplerProc_test_hwainit();
        dopplerProc_test_dpuInit();

        fread(&testConfig->dopTestStaticCfg.numTxAntennas, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&testConfig->dopTestStaticCfg.numAzimTxAntennas, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&testConfig->dopTestStaticCfg.numRxAntennas, sizeof(uint8_t), 1, fileIdTestInfo);
        testConfig->dopTestStaticCfg.numVirtualAntennas = testConfig->dopTestStaticCfg.numTxAntennas * testConfig->dopTestStaticCfg.numRxAntennas;
        fread(&testConfig->dopTestStaticCfg.numRangeBins, sizeof(uint16_t), 1, fileIdTestInfo);
        fread(&testConfig->dopTestStaticCfg.numChirps, sizeof(uint16_t), 1, fileIdTestInfo);
        testConfig->dopTestStaticCfg.numDopplerFFTBins = mathUtils_getValidFFTSize(testConfig->dopTestStaticCfg.numChirps);
        testConfig->dopTestStaticCfg.sizeOfInputSample = sizeof(cmplx16ImRe_t);
        switch (testConfig->dopTestStaticCfg.numTxAntennas)
        {
            case 2:
                testConfig->dopTestStaticCfg.numBandsTotal = 3;
                break;
            case 3:
                testConfig->dopTestStaticCfg.numBandsTotal = 4;
                break;
            case 4:
                testConfig->dopTestStaticCfg.numBandsTotal = 6;
                break;
            default:
                retVal = DPU_DOPPLERPROCHWA_EINVAL;
        }
        uint8_t compEnable;
        fread(&compEnable, sizeof(uint8_t), 1, fileIdTestInfo);
        testConfig->dopTestStaticCfg.decompCfg.isEnabled = compEnable;
        uint16_t compMethod;
        fread(&compMethod, sizeof(uint16_t), 1, fileIdTestInfo);
        testConfig->dopTestStaticCfg.decompCfg.compressionMethod = (uint8_t)compMethod;
        fread(&testConfig->dopTestStaticCfg.decompCfg.compressionRatio, sizeof(float), 1, fileIdTestInfo);
        fread(&testConfig->dopTestStaticCfg.decompCfg.rangeBinsPerBlock, sizeof(uint16_t), 1, fileIdTestInfo);
        fread(&testConfig->dopTestStaticCfg.decompCfg.numRxAntennaPerBlock, sizeof(uint16_t), 1, fileIdTestInfo);
        fread(&cfarCfg.thresholdScale, sizeof(uint16_t), 1, fileIdTestInfo);
        cfarCfg.thresholdScale = MmwDemo_convertCfarToLinear(cfarCfg.thresholdScale);
        fread(&cfarCfg.averageMode, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&cfarCfg.winLen, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&cfarCfg.guardLen, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&cfarCfg.noiseDivShift, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&cfarCfg.cyclicMode, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&cfarCfg.peakGroupingEn, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&cfarCfg.osKvalue, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&cfarCfg.osEdgeKscaleEn, sizeof(uint8_t), 1, fileIdTestInfo);
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
        /* Put inside the macro so that Histogram test is not affected */
        fread(&cfarCfg.variableThresholdMode, sizeof(uint8_t), 1, fileIdTestInfo);
        testConfig->dopTestStaticCfg.cfarCfg = cfarCfg;
#endif
        fread(&testConfig->dopTestStaticCfg.localMaxCfg.azimThreshold, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&testConfig->dopTestStaticCfg.localMaxCfg.dopplerThreshold, sizeof(uint8_t), 1, fileIdTestInfo);
        fread(&testConfig->dopTestStaticCfg.numAzimFFTBins, sizeof(uint16_t), 1, fileIdTestInfo);
        fread(&testConfig->dopTestStaticCfg.antennaCalibParams, sizeof(float),
                NUM_TXANT * NUM_RXANT * 2,
                fileIdTestInfo);
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
        testConfig->dopTestStaticCfg.zeroInsrtMaskAzim = 0xFFF;
#elif defined(SOC_AWR2943)
        testConfig->dopTestStaticCfg.zeroInsrtMaskAzim = 0xFF;
#endif
        memcpy((void*)testConfig->dopTestStaticCfg.antennaGeometryCfg, (void*)antennaGeometryCfg, sizeof(antennaGeometryCfg));

        testConfig->dopTestStaticCfg.isSumTxEnabled = 1;
		testConfig->dopTestStaticCfg.maxNumObj      = DPU_DopplerProcHWATest_MAX_NUM_OBJECTS;
        DebugP_log("\nTest %d\n", testIdx);
        DebugP_log("\tNumRx%d NumTx%d NumAzimTx%d \n\tNumRangeBins%d NumChirps%d \n\tCompMethod%d CompRatio%.2f RBinsPerBlk%d RxAntPerBlk%d \n\tcfarThresh%d cfarAvgMode%d isCyclic%d\n\tlmAzimThresh%d lmDopThresh%d\n\tnumAzimFFTBins%d\n",
                testConfig->dopTestStaticCfg.numRxAntennas,
                testConfig->dopTestStaticCfg.numTxAntennas,
                testConfig->dopTestStaticCfg.numAzimTxAntennas,
                testConfig->dopTestStaticCfg.numRangeBins,
                testConfig->dopTestStaticCfg.numChirps,
                testConfig->dopTestStaticCfg.decompCfg.compressionMethod,
                testConfig->dopTestStaticCfg.decompCfg.compressionRatio,
                testConfig->dopTestStaticCfg.decompCfg.rangeBinsPerBlock,
                testConfig->dopTestStaticCfg.decompCfg.numRxAntennaPerBlock,
                cfarCfg.thresholdScale,
                cfarCfg.averageMode,
                cfarCfg.cyclicMode,
                testConfig->dopTestStaticCfg.localMaxCfg.azimThreshold,
                testConfig->dopTestStaticCfg.localMaxCfg.dopplerThreshold,
                testConfig->dopTestStaticCfg.numAzimFFTBins);
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
        /* Put inside the macro so that Histogram test is not affected */
        DebugP_log("\tIsVariableThresholdMode%d\n", cfarCfg.variableThresholdMode);
#endif

        if(testConfig->dopTestStaticCfg.decompCfg.compressionMethod == HWA_COMPRESS_METHOD_BFP)
            testConfig->dopTestStaticCfg.decompCfg.bfpCompExtraParamSets = 2*(testConfig->dopTestStaticCfg.decompCfg.numRxAntennaPerBlock - 1);
        else
            testConfig->dopTestStaticCfg.decompCfg.bfpCompExtraParamSets = 0;

        /* doppler dpu config*/
        retVal = dopplerProc_test_dpuConfig();
        if (retVal < 0)
        {
            DebugP_log("doppler test failed to configure. Error %d.\n", retVal);
            DebugP_assert(0);
            return;
        }

        memcpy(&dopTestConfig.staticCfg, &testConfig->dopTestStaticCfg, sizeof(DPU_DopplerProcHWA_StaticConfig));
        memcpy(&dopTestConfig.hwRes, &testConfig->hwRes, sizeof(DPU_DopplerProcHWA_HW_Resources));

        retVal = DPU_DopplerProcHWA_config(dopplerProcDpuHandle, &dopTestConfig, 1);
        if (retVal < 0)
        {
            DebugP_log("doppler DPU failed to configure. Error %d.\n", retVal);
            DebugP_assert(0);
            return;
        }

        uint32_t numBytesToRead = testConfig->dopTestStaticCfg.numChirps
                                    * testConfig->dopTestStaticCfg.numRangeBins
                                    * sizeof(cmplx16ImRe_t) * testConfig->dopTestStaticCfg.numRxAntennas
                                    * testConfig->dopTestStaticCfg.decompCfg.compressionRatio;

        fread(((uint16_t *)dopTestConfig.hwRes.radarCube.data), sizeof( uint16_t ), numBytesToRead/sizeof( uint16_t ), fileIdCompRCube);
        DebugP_log("File Read Done\n");

        retVal = DPU_DopplerProcHWA_process(dopplerProcDpuHandle, &dopTestConfig, &out);
        if (retVal < 0)
        {
            DebugP_log("doppler DPU failed to run. Error %d.\n", retVal);
            DebugP_assert(0);
            return;
        }
        // DebugP_log("Doppler DPU Process. Cycles = %d. \n", bench2);

        fread(&numObjOutIdeal, sizeof(uint32_t), 1, fileIdTestResult);

        for(idealObjIdx =0; idealObjIdx<numObjOutIdeal; idealObjIdx++)
        {
            fread(&gDetObjIdealResult[idealObjIdx].azimIdx, sizeof(uint32_t), 1, fileIdTestResult);
            fread(&gDetObjIdealResult[idealObjIdx].dopIdx, sizeof(uint32_t), 1, fileIdTestResult);
            fread(&gDetObjIdealResult[idealObjIdx].rangeIdx, sizeof(uint32_t), 1, fileIdTestResult);
            fread(&subBandIdx, sizeof(uint32_t), 1, fileIdTestResult);
            fread(&gDetObjIdealResult[idealObjIdx].dopIdxActual, sizeof(uint32_t), 1, fileIdTestResult);
            fread(&gDetObjIdealResult[idealObjIdx].dopCfarNoise, sizeof(uint32_t), 1, fileIdTestResult);
            fread(&gDetObjIdealResult[idealObjIdx].elevSamples[0], sizeof(int32_t), NUM_TXANT_ELEV*NUM_RXANT*2, fileIdTestResult);
            fread(&gDetObjIdealResult[idealObjIdx].azimSamples[0], sizeof(int32_t), NUM_TXANT_AZIM*NUM_RXANT*2, fileIdTestResult);
            fread(&gDetObjIdealResult[idealObjIdx].azimPeakSamples[0], sizeof(uint32_t), 3, fileIdTestResult);
        }

        retVal = compareResults(dopTestConfig.hwRes.detObjList, &gDetObjIdealResult[0], out.numObjOut, numObjOutIdeal);
        if(retVal < 0){
            DebugP_log("Error in compareResults!\n");
        }

        DebugP_log("%d Tests finished!\n\r", testIdx);

        /* hwa, edma, dpu DEinialization */
        dopplerProc_test_dpuDeinit();
        dopplerProc_test_hwaDeinit();

        DebugP_log("----------------------------\n\n");

    }

    DebugP_log ("--- Test Completed ---\n");
    DebugP_log("----------------------------\n\n");
    taskEndTime=ClockP_getTimeUsec();
    DebugP_log("Test Time Cycles: %lld\n",taskEndTime-taskStartTime);
    DebugP_log("All Tests Finished\n");

    if(finalResults)
    {
        DebugP_log("All tests have passed!!\n");
    }
    else
    {
        DebugP_log("Some tests have failed!!\n");
    }

    fclose(fileIdTestInfo);
    fclose(fileIdCompRCube);
    fclose(fileIdTestResult);
    Board_driversClose();
    Drivers_close();

    while(1);

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

#ifdef SUBSYS_M4
    void *args = NULL;
    Test_task(args);
#else
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
#endif
}
