/**
 *   @file  rangecfarprochwa_test_main.c
 *
 *   @brief
 *      Unit Test code for range hwa dpu processing.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 -2025 Texas Instruments, Inc.
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
#include <math.h>

/* MCU Plus Include Files. */
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/ClockP.h>
#include <drivers/edma.h>
#ifdef SUBSYS_DSS
#include <ti/datapath/dpu/rangecfarprocDDMA/test/dssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/test/dssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/test/dssgenerated/ti_board_open_close.h>
#endif
#ifdef SUBSYS_M4
#include <ti/datapath/dpu/rangecfarprocDDMA/test/m4generated/ti_drivers_config.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/test/m4generated/ti_board_config.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/test/m4generated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/test/m4generated/ti_board_open_close.h>
#endif
#include <kernel/dpl/DebugP.h>

#ifndef SUBSYS_M4
#include "FreeRTOS.h"
#include "task.h"
#endif

/* mmwave SDK files */
#include <ti/datapath/dpu/rangecfarprocDDMA/rangecfarprochwa.h>
#include <ti/utils/mathutils/mathutils.h>
#if defined(SOC_AWR2X44P)
#include <ti/demo/awr2x44P/mmw_ddm/mmw_resDDM.h>
#elif defined(SOC_AWR294X)
#include <ti/demo/awr294x/mmw/mmw_resDDM.h>
#endif

/* Interrupt object for rangeCfarProc */
Edma_IntrObject rangeCfarProcIntrObj[RANGECFARPROCHWADDMA_NUM_EDMA_INTERRUPTS];

/*! L3 RAM buffer for object detection DPC */
#ifdef SUBSYS_M4
uint8_t gL3Ram[(0x80000)] __attribute__((aligned(4096), section(".l3ram")));
#endif
#ifdef SUBSYS_DSS
uint8_t gL3Ram[(0x80000)] __attribute__((aligned(4096)));
#pragma DATA_ALIGN(gL3Ram, 4096U);
#pragma DATA_SECTION(gL3Ram, ".l3ram");
#endif

/*! L2 RAM buffer for object detection DPC */
#define MMWDEMO_OBJDET_L2RAM_SIZE (87U * 1024U)
#ifdef SUBSYS_M4
uint8_t gL2Heap[MMWDEMO_OBJDET_L2RAM_SIZE] __attribute__((aligned(4096), section(".dpc_l2Heap")));
#endif
#ifdef SUBSYS_DSS
uint8_t gL2Heap[MMWDEMO_OBJDET_L2RAM_SIZE];
#pragma DATA_ALIGN(gL2Heap, 4096U);
#pragma DATA_SECTION(gL2Heap, ".dpc_l2Heap");
#endif

#define DPU_RANGECFARDDMA_TEST_CFAR_THRESHOLD_ENCODING_FACTOR (100.0)
#define DPU_RANGECFARDDMA_TEST_MAX_NUM_OBJECTS             500U
#define DPU_RANGECFARHWADDMA_TEST_PARAMSET_START_IDX        0U
#define PING 0
#define PONG 1
/* Error codes */
#define DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__L3_RAM_DET_MATRIX   -1
#define DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__OBJ_PARAMS_RAM_RANGE_CFAR_BUF   -2
#define DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_RANGECFAR_NUMOBJ_PER_DOPPLER_BUF -3
#define DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_RANGECFAR_SCRATCH_BUF_PING   -4
#define DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_RANGECFAR_SCRATCH_BUF_PONG   -5

/* Max permissible error values */
#define MAX_RANGEIDX_ERR    0
#define MAX_DOPIDX_ERR  0
#define MAX_RANGECFARNOISE_ERR 0
#define RANGECFAR_TEST_ABS(x) (((x) >= 0) ? (x) : -(x))

/*! Ideal det obj output */
#ifdef SUBSYS_M4
RangeCfarListObj gDetObjIdealResult[DPU_RANGECFARDDMA_TEST_MAX_NUM_OBJECTS] __attribute__((aligned(4096), section(".l3ram")));
#endif
#ifdef SUBSYS_DSS
RangeCfarListObj gDetObjIdealResult[DPU_RANGECFARDDMA_TEST_MAX_NUM_OBJECTS];
#pragma DATA_ALIGN(gDetObjIdealResult, 4096U);
#pragma DATA_SECTION(gDetObjIdealResult, ".l3ram");
#endif
uint8_t gIdealObjFoundList[DPU_RANGECFARDDMA_TEST_MAX_NUM_OBJECTS];
uint8_t gIsReal =1;
/*
 * @brief Memory Configuration used during init API
 */
typedef struct DPU_RangeCfarProcTest_MemCfg_t
{
    /*! @brief   Start address of memory provided by the application
     *           from which DPC will allocate.
     */
    void *addr;

    /*! @brief   Size limit of memory allowed to be consumed by the DPC */
    uint32_t size;
} DPU_RangeCfarProcTest_MemCfg;

/*
 * @brief Memory pool object to manage memory
 */
typedef struct MemPoolObj_t
{
    /*! @brief Memory configuration */
    DPU_RangeCfarProcTest_MemCfg cfg;

    /*! @brief   Pool running adress.*/
    uintptr_t currAddr;

    /*! @brief   Pool max address. This pool allows setting address to desired
     *           (e.g for rewinding purposes), so having a running maximum
     *           helps in finding max pool usage
     */
    uintptr_t maxCurrAddr;
} MemPoolObj;

DPU_RangeCfarProcHWA_Config testConfigStruct;
DPU_RangeCfarProcHWA_Config *testConfig = &testConfigStruct;

/*! @brief L3 ram memory pool object */
MemPoolObj L3ramObjS;
MemPoolObj *L3ramObj = &L3ramObjS;

/*! @brief Core Local ram memory pool object */
MemPoolObj CoreLocalRamObjS;
MemPoolObj *CoreLocalRamObj = &CoreLocalRamObjS;

HWA_Handle hwaHandle;
volatile uint8_t subframeCount = 0U;

DPU_RangeCFARProcHWA_Handle rangeCfarProcDpuHandle;

uint8_t finalResults;

#ifndef SUBSYS_M4
/* FreeRTOS Task declarations. */
#define APP_TASK_PRI (5U)
#define APP_TASK_STACK_SIZE (4 * 1024U)

TaskHandle_t gAppTask;
StaticTask_t gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(64)));
#endif

FILE *testVectors_fileId;
FILE *detMatrix_fileId;
FILE *testResult_fileId;

/**
 *  @b Description
 *  @n
 *      Utility function for reseting memory pool.
 *
 *  @param[in]  pool Handle to pool object.
 *
 *  \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 *
 *  @retval
 *      none.
 */
static void DPU_RangeCfarProcTest_MemPoolReset(MemPoolObj *pool)
{
    pool->currAddr = (uintptr_t)pool->cfg.addr;
    pool->maxCurrAddr = pool->currAddr;
}

#if 0 /* Not used in this implementation */
/**
 *  @b Description
 *  @n
 *      Utility function for setting memory pool to desired address in the pool.
 *      Helps to rewind for example.
 *
 *  @param[in]  pool Handle to pool object.
 *  @param[in]  addr Address to assign to the pool's current address.
 *
 *  \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 *
 *  @retval
 *      None
 */
static void DPU_RangeProcTest_MemPoolSet(MemPoolObj *pool, void *addr)
{
    pool->currAddr = (uintptr_t)addr;
    pool->maxCurrAddr = CSL_MAX(pool->currAddr, pool->maxCurrAddr);
}
#endif

#if 0
/**
 *  @b Description
 *  @n
 *      Utility function for getting memory pool current address.
 *
 *  @param[in]  pool Handle to pool object.
 *
 *  \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 *
 *  @retval
 *      pointer to current address of the pool (from which next allocation will
 *      allocate to the desired alignment).
 */
static void *DPU_RangeProcTest_MemPoolGet(MemPoolObj *pool)
{
    return((void *)pool->currAddr);
}
#endif

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
static uint32_t DPU_RangeProcTest_MemPoolGetCurrentUsage(MemPoolObj *pool)
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
 *  \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 *
 *  @retval
 *      Amount of pool used in bytes.
 */
static uint32_t DPU_RangeProcTest_MemPoolGetMaxUsage(MemPoolObj *pool)
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
 *  \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 *
 *  @retval
 *      pointer to beginning of allocated block. NULL indicates could not
 *      allocate.
 */
static void *DPU_RangeCfarProcTest_MemPoolAlloc(MemPoolObj *pool,
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

    return (retAddr);
}

/**
 *  @b Description
 *  @n
 *      Allocates Shawdow paramset
 */
static void allocateEDMAShadowChannel(EDMA_Handle edmaHandle, uint32_t *param)
{
    int32_t testStatus = SystemP_SUCCESS;
    EDMA_Config *config;
    EDMA_Object *object;

    config = (EDMA_Config *)edmaHandle;
    object = config->object;

    if ((object->allocResource.paramSet[*param / 32] & (1U << *param % 32)) != (1U << *param % 32))
    {
        testStatus = EDMA_allocParam(edmaHandle, param);
        DebugP_assert(testStatus == SystemP_SUCCESS);
    }

    return;
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
 * \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 */
void DPU_RangeCfarProcHWATest_EDMAChannelConfigAssist(EDMA_Handle handle, uint32_t chNum, uint32_t shadowParam, uint32_t eventQueue, DPEDMA_ChanCfg *chanCfg)
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
*    Based on the test configuration, set up the range cfar processing DPU configurations
*/
int32_t rangeCfarProc_test_dpuConfig()
{

    int32_t retVal = 0;
    void* scratchBufMem;
    DPIF_DetMatrix detMatrix;
    DPU_RangeCFARProcHWA_StaticConfig *staticCfg = &testConfig->staticCfg;
    DPU_RangeCFARProcHWA_HW_Resources *hwRes = &testConfig->res;
    EDMA_Handle edmaHandle = gEdmaHandle[0];

    DPU_RangeCfarProcTest_MemPoolReset(L3ramObj);
    DPU_RangeCfarProcTest_MemPoolReset(CoreLocalRamObj);

    /********************************************
     * Allocating memory resources              *
     *******************************************/
    {
    {

    /* L3 allocations - detection matrix */
    detMatrix.dataSize = staticCfg->numRangeBins * (staticCfg->numDopplerBins/staticCfg->numSubBandsTotal) * sizeof(uint16_t);
    detMatrix.data = DPU_RangeCfarProcTest_MemPoolAlloc(L3ramObj, detMatrix.dataSize,
                                            CSL_CACHE_L1D_LINESIZE);
    if (detMatrix.data == NULL)
    {
        retVal = DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__L3_RAM_DET_MATRIX;
        goto exit;
    }
    detMatrix.datafmt = DPIF_DETMATRIX_FORMAT_1;
    /* Assign the detection matrix */
    hwRes->detMatrix = detMatrix;

    /* L3 allocations - DPU Output Resource */
    hwRes->rangeCfarListSizeBytes = sizeof(RangeCfarListObj) * DPU_RANGECFARDDMA_TEST_MAX_NUM_OBJECTS;
    scratchBufMem = DPU_RangeCfarProcTest_MemPoolAlloc(L3ramObj, hwRes->rangeCfarListSizeBytes,  CSL_CACHE_L1D_LINESIZE);
    if (scratchBufMem == NULL)
    {
        retVal = DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__OBJ_PARAMS_RAM_RANGE_CFAR_BUF;
        goto exit;
    }
    hwRes->rangeCfarList = (RangeCfarListObj *)scratchBufMem;

    hwRes->rangeCfarNumObjPerDopplerBinSizeBytes = sizeof(uint16_t) * staticCfg->numDopplerBins / staticCfg->numSubBandsTotal;

    scratchBufMem = DPU_RangeCfarProcTest_MemPoolAlloc(CoreLocalRamObj, hwRes->rangeCfarNumObjPerDopplerBinSizeBytes, sizeof(uint32_t));
    if (scratchBufMem == NULL)
    {
        retVal = DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_RANGECFAR_NUMOBJ_PER_DOPPLER_BUF;
        goto exit;
    }
    hwRes->rangeCfarNumObjPerDopplerBinBuf = (uint8_t *)scratchBufMem;

    hwRes->rangeCfarScratchBufSizeBytes = sizeof(cmplx32ImRe_t) * DPU_RANGECFARDDMA_TEST_MAX_NUM_OBJECTS;

    scratchBufMem = DPU_RangeCfarProcTest_MemPoolAlloc(CoreLocalRamObj, hwRes->rangeCfarScratchBufSizeBytes/2, sizeof(uint32_t));
    if (scratchBufMem == NULL)
    {
        retVal = DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_RANGECFAR_SCRATCH_BUF_PING;
        goto exit;
    }
    hwRes->rangeCfarScratchBuf[PING] = (uint8_t *)scratchBufMem;

    scratchBufMem = DPU_RangeCfarProcTest_MemPoolAlloc(CoreLocalRamObj, hwRes->rangeCfarScratchBufSizeBytes/2, sizeof(uint32_t));
    if (scratchBufMem == NULL)
    {
        retVal = DPU_RANGECFARPROCCHWA_DDMA_TEST_ENOMEM__CORE_LOCAL_RAM_RANGECFAR_SCRATCH_BUF_PONG;
        goto exit;
    }
    hwRes->rangeCfarScratchBuf[PONG] = (uint8_t *)scratchBufMem;

    }
	}

    /* hwres - edma */
    hwRes->edmaHandle = gEdmaHandle[CONFIG_EDMA0];;
    hwRes->detMatBytesPerSample = sizeof(uint16_t);
    hwRes->maxNumCFARObj = DPU_RANGECFARDDMA_TEST_MAX_NUM_OBJECTS;
    CacheP_inv(hwRes->detMatrix.data,hwRes->detMatrix.dataSize,CacheP_TYPE_ALL);
    DPU_RangeCfarProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_RANGECFARPROC_CFAR_IN_PING,
                                       EDMA_RANGECFARPROC_CFAR_IN_PING_SHADOW,
                                       DPC_OBJDET_DPU_RANGECFARPROC_EVENT_QUE,
                                       &hwRes->edmaIn.pingPong[PING]);
    DPU_RangeCfarProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_RANGECFARPROC_CFAR_IN_HOTSIG_PING,
                                       EDMA_RANGECFARPROC_CFAR_IN_HOTSIG_PING_SHADOW,
                                       DPC_OBJDET_DPU_RANGECFARPROC_EVENT_QUE,
                                       &hwRes->edmaInSignature.pingPong[PING]);

    DPU_RangeCfarProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_RANGECFARPROC_CFAR_IN_PONG,
                                       EDMA_RANGECFARPROC_CFAR_IN_PONG_SHADOW,
                                       DPC_OBJDET_DPU_RANGECFARPROC_EVENT_QUE,
                                       &hwRes->edmaIn.pingPong[PONG]);
    DPU_RangeCfarProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_RANGECFARPROC_CFAR_IN_HOTSIG_PONG,
                                       EDMA_RANGECFARPROC_CFAR_IN_HOTSIG_PONG_SHADOW,
                                       DPC_OBJDET_DPU_RANGECFARPROC_EVENT_QUE,
                                       &hwRes->edmaInSignature.pingPong[PONG]);

    DPU_RangeCfarProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_RANGECFARPROC_CFAR_OUT_PING,
                                       EDMA_RANGECFARPROC_CFAR_OUT_PING_SHADOW,
                                       DPC_OBJDET_DPU_RANGECFARPROC_EVENT_QUE,
                                       &hwRes->edmaOut.pingPong[PING]);

    DPU_RangeCfarProcHWATest_EDMAChannelConfigAssist(edmaHandle,
                                       EDMA_RANGECFARPROC_CFAR_OUT_PONG,
                                       EDMA_RANGECFARPROC_CFAR_OUT_PONG_SHADOW,
                                       DPC_OBJDET_DPU_RANGECFARPROC_EVENT_QUE,
                                       &hwRes->edmaOut.pingPong[PONG]);

    {
        {

        uint32_t intrIdx = 0;

        /* Allocate interrupt object */
        hwRes->edmaIntrObj.pingPong[PING] = &rangeCfarProcIntrObj[intrIdx++];
        hwRes->edmaIntrObj.pingPong[PONG] = &rangeCfarProcIntrObj[intrIdx++];
        }
    }

    hwRes->hwaCfg.numParamSet = DPU_RANGECFARPROCHWADDMA_NUM_HWA_PARAMSETS;
    hwRes->hwaCfg.paramSetStartIdx = DPU_RANGECFARHWADDMA_TEST_PARAMSET_START_IDX;

    retVal = DPU_RangeCFARProcHWA_config(rangeCfarProcDpuHandle, testConfig);
    if (retVal != 0)
    {
        test_print("range Cfar DPU failed to configure. Error %d.\n",retVal);
        DebugP_assert(0);
        goto exit;
    }

exit:
    return retVal;
}

void rangeCfarProcDpuTest_hwaInit()
{
    int32_t errorCode = 0;

    hwaHandle = HWA_open(0, NULL, &errorCode);
    if (hwaHandle == NULL)
    {
        test_print("Error: Unable to open the HWA Instance err:%d\n", errorCode);
        DebugP_assert(0);
        return;
    }
}

void rangeCfarProcDpuTest_dpuInit()
{
    int32_t errorCode = 0;
    DPU_RangeCFARProcHWA_InitParams initParams;
    initParams.hwaHandle = hwaHandle;

    DPU_RangeCfarProcTest_MemCfg L3ramCfg, CoreLocalRamCfg;
    L3ramCfg.addr = (void *)&gL3Ram[0];
    L3ramCfg.size = sizeof(gL3Ram);
    CoreLocalRamCfg.addr = &gL2Heap[0];
    CoreLocalRamCfg.size = sizeof(gL2Heap);

    L3ramObj->cfg = L3ramCfg;
    CoreLocalRamObj->cfg = CoreLocalRamCfg;

    DPU_RangeCfarProcTest_MemPoolReset(L3ramObj);
    DPU_RangeCfarProcTest_MemPoolReset(CoreLocalRamObj);

    /* generate the dpu handler*/
    rangeCfarProcDpuHandle =  DPU_RangeCFARProcHWA_init (&initParams, subframeCount, &errorCode);
    if (rangeCfarProcDpuHandle == NULL)
    {
        test_print("Debug: RangeCfarProc DPU initialization returned error %d\n", errorCode);
        DebugP_assert(0);
        return;
    }
}

/**
 *  @b Description
 *  @n
 *      Compares two RangeCfarListObj structures and returns 0 if within error limits
 */
int32_t compareObjOut(RangeCfarListObj * obj1, RangeCfarListObj * obj2){

    if(RANGECFAR_TEST_ABS((int32_t)obj1->rangeIdx - (int32_t)obj2->rangeIdx) <= MAX_RANGEIDX_ERR){
        if(RANGECFAR_TEST_ABS((int32_t)obj1->dopIdx - (int32_t)obj2->dopIdx) <= MAX_DOPIDX_ERR){
            if(RANGECFAR_TEST_ABS((int32_t)obj1->rangeCFARNoise - (int32_t)obj2->rangeCFARNoise) <= MAX_RANGECFARNOISE_ERR){
                return 0;
            }
        }
    }
    return 1;
}

/**
 *  @b Description
 *  @n
 *      Compares two RangeCfarListObj lists
 */
int32_t compareResults(RangeCfarListObj * dpuList, RangeCfarListObj * idealList, uint32_t numObjOut, uint32_t numObjOutIdeal){

    int32_t retVal = 0;
    uint32_t i, j;
    uint8_t extraObjFound = 0;

    memset(gIdealObjFoundList, 0, DPU_RANGECFARDDMA_TEST_MAX_NUM_OBJECTS * sizeof(uint8_t));

    printf("NumObjOut = %d, NumObjOutIdeal = %d\n", numObjOut, numObjOutIdeal);
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
            printf("Extra Object Found. ObjIdx = %d, RangeBin = %d, DopBin = %d, rangeCFARNoise = %d, ",
                    i, dpuList[i].rangeIdx, dpuList[i].dopIdx, dpuList[i].rangeCFARNoise);
			printf("\n");
            finalResults=0;
        }
    }

    for(j = 0; j < numObjOutIdeal; j++){
        if(!gIdealObjFoundList[j]){
            printf("Object Not Found. ObjIdx = %d, RangeBin = %d, DopBin = %d, rangeCFARNoise = %d\n",
                    j, idealList[j].rangeIdx, idealList[j].dopIdx, idealList[j].rangeCFARNoise);

			printf("\n");
            finalResults=0;
        }
    }

    if(numObjOut!=numObjOutIdeal)
        finalResults=0;

    printf("\n");

    return retVal;

}

/**
 *  @b Description
 *  @n
 *      Utility function to convert the CFAR threshold
 *      from a CLI encoded dB value to a log2 value
 *      as expected by the Range CFAR DPU
 *
 *  @param[in] codedCfarVal CFAR SNR threshold in dB as encoded in the CLI
 *  @param[in] numBands Total number of subbands
 *
 *  @retval
 *      CFAR threshold in dB format
 */
static uint16_t rangeCfarProcDpuTest_convertRangeCfarToThresh(uint16_t codedCfarVal, uint8_t numBands)
{
    uint16_t linearVal;
    float    dbVal, linVal;
    uint32_t defaultScaling = 1 << 11;
    float additionalScaling =  numBands / (float)(1 << mathUtils_ceilLog2(numBands));

    /* dbVal is a float value from 0-100dB. It needs to
    be converted to linear scale..
    First, recover float dbVal that was encoded in CLI. */
    dbVal = (float)(codedCfarVal / DPU_RANGECFARDDMA_TEST_CFAR_THRESHOLD_ENCODING_FACTOR);

    /* Now convert it to linear value */
    linVal = (uint32_t)(log2f(pow(10, (float)dbVal/20.0)) * additionalScaling * defaultScaling + 0.5);

    linearVal = (uint16_t) linVal;
    return (linearVal);
}


/**
*  @b Description
*  @n
*        Testing Task
*/
void rangecfarProcDpuTest_Task(void *args)
{
    uint32_t numTests, testIdx;
    int32_t retVal = 0;
    DPU_RangeCFARProcHWA_OutParams outParams;
    uint32_t numObjIdeal;
    uint32_t baseAddr, regionId;
    uint8_t readDataAgain;
    uint32_t numBytesToRead;
    char fileName[100];
    uint64_t taskStartTime, taskEndTime;
#if defined(SOC_AWR2943)
    char deviceType[] = "awr2943";
    gIsReal =1;
#elif defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
    char deviceType[] = "awr2944";
    gIsReal =1;
#else
    char deviceType[] = "";
#endif

    Drivers_open();
    Board_driversOpen();

    baseAddr = EDMA_getBaseAddr(gEdmaHandle[CONFIG_EDMA0]);
    DebugP_assert(baseAddr != 0);

    regionId = EDMA_getRegionId(gEdmaHandle[CONFIG_EDMA0]);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    finalResults = 1;

    taskStartTime=ClockP_getTimeUsec();
    test_print("...... Initialization ...... \n");

    /* hwa, edma, and rangeCfarProcDPU initialization*/
    rangeCfarProcDpuTest_hwaInit();
    rangeCfarProcDpuTest_dpuInit();

    strcpy(fileName, "testdata\\rangecfarprocDDMA_testVectors_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    testVectors_fileId = fopen(fileName, "rb");
    if (testVectors_fileId == NULL)
    {
        test_print("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    strcpy(fileName, "testdata\\rangecfarprocDDMA_testDataIn_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    detMatrix_fileId = fopen(fileName, "rb");
    if (detMatrix_fileId == NULL)
    {
        test_print("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    strcpy(fileName, "testdata\\rangecfarprocDDMA_testResult_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    testResult_fileId = fopen(fileName, "rb");
    if (testResult_fileId == NULL)
    {
        test_print("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    fread(&numTests, sizeof(uint32_t), 1, testVectors_fileId);

    memset((void*)testConfig, 0, sizeof(testConfig));
    /* process each test */
    for (testIdx = 0; testIdx < numTests; testIdx++)
    {
        /* read in test config */
        fread(&testConfig->staticCfg.numSubBandsTotal, sizeof(uint8_t), 1, testVectors_fileId);
        fread(&testConfig->staticCfg.numRangeBins, sizeof(uint16_t), 1, testVectors_fileId);
        if(gIsReal==1){
            testConfig->staticCfg.numRangeBins /= 2;
        }
        fread(&testConfig->staticCfg.numDopplerBins, sizeof(uint16_t), 1, testVectors_fileId);
        testConfig->staticCfg.cfarCfg.isEnabled = 1;
        fread(&testConfig->staticCfg.cfarCfg.thresholdScale, sizeof(uint16_t), 1, testVectors_fileId);

        test_print("##################################\n");
        test_print("\nTest #%d  start\r\n", testIdx);
        test_print("------------\n");
        
        testConfig->staticCfg.cfarCfg.thresholdScale = rangeCfarProcDpuTest_convertRangeCfarToThresh(testConfig->staticCfg.cfarCfg.thresholdScale*DPU_RANGECFARDDMA_TEST_CFAR_THRESHOLD_ENCODING_FACTOR, testConfig->staticCfg.numSubBandsTotal);
        fread(&testConfig->staticCfg.cfarCfg.averageMode, sizeof(uint8_t), 1, testVectors_fileId);
        fread(&testConfig->staticCfg.cfarCfg.winLen, sizeof(uint8_t), 1, testVectors_fileId);
        fread(&testConfig->staticCfg.cfarCfg.guardLen, sizeof(uint8_t), 1, testVectors_fileId);
        fread(&testConfig->staticCfg.cfarCfg.noiseDivShift, sizeof(uint8_t), 1, testVectors_fileId);
        fread(&testConfig->staticCfg.cfarCfg.cyclicMode, sizeof(uint8_t), 1, testVectors_fileId);
        fread(&testConfig->staticCfg.cfarCfg.peakGroupingEn, sizeof(uint8_t), 1, testVectors_fileId);
        fread(&testConfig->staticCfg.cfarCfg.osKvalue, sizeof(uint8_t), 1, testVectors_fileId);
        fread(&testConfig->staticCfg.cfarCfg.osEdgeKscaleEn, sizeof(uint8_t), 1, testVectors_fileId);
        test_print("numRangeBins:  %d \nnumDopplerBins:  %d \ncfarThreshold(dB):  %d \naverageMode:  %d \nwinLen:  %d \nguardLen:  %d \nwisCyclic:  %d \n",
                testConfig->staticCfg.numRangeBins,
                testConfig->staticCfg.numDopplerBins,
                testConfig->staticCfg.cfarCfg.thresholdScale,
                testConfig->staticCfg.cfarCfg.averageMode,
                testConfig->staticCfg.cfarCfg.winLen,
                testConfig->staticCfg.cfarCfg.guardLen,
                testConfig->staticCfg.cfarCfg.cyclicMode);
        test_print("------------\n");

        retVal = rangeCfarProc_test_dpuConfig();
        fread(&readDataAgain, sizeof(uint8_t), 1, testVectors_fileId);

        numBytesToRead = testConfig->res.detMatrix.dataSize/2;

        if (readDataAgain)
        {
            fread(((uint16_t *)testConfig->res.detMatrix.data), sizeof(uint16_t), numBytesToRead, detMatrix_fileId);
        }

        memset((void *)&outParams, 0, sizeof(DPU_RangeCFARProcHWA_OutParams));
        retVal= DPU_RangeCFARProcHWA_process(rangeCfarProcDpuHandle, testConfig, &outParams);
        if (retVal < 0)
        {
            test_print("Range Cfar DPU failed to run. Error %d.\n", retVal);
            DebugP_assert(0);
            return;
        }

        fread(&numObjIdeal, sizeof(int32_t), 1, testResult_fileId);
        fread(&gDetObjIdealResult, sizeof(RangeCfarListObj), numObjIdeal, testResult_fileId);
        retVal = compareResults(testConfig->res.rangeCfarList, &gDetObjIdealResult[0],  outParams.numCfarDetectedPoints, numObjIdeal);
        if(retVal < 0){
            printf("Error in compareResults!\n");
        }

        /* Free EDMA Channels. */
        /* DPEDMA_freeEDMAChannel(uint32_t *dmaCh, uint32_t *tcc, uint32_t *param, uint32_t *shadowParam)*/
        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &testConfig->res.edmaIn.pingPong[PING].channel, &testConfig->res.edmaIn.pingPong[PING].tcc,
                               &testConfig->res.edmaIn.pingPong[PING].paramId, &testConfig->res.edmaIn.pingPong[PING].shadowPramId);

        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &testConfig->res.edmaIn.pingPong[PONG].channel, &testConfig->res.edmaIn.pingPong[PONG].tcc,
                               &testConfig->res.edmaIn.pingPong[PONG].paramId, &testConfig->res.edmaIn.pingPong[PONG].shadowPramId);

        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &testConfig->res.edmaInSignature.pingPong[PING].channel, &testConfig->res.edmaInSignature.pingPong[PING].tcc,
                               &testConfig->res.edmaInSignature.pingPong[PING].paramId, &testConfig->res.edmaInSignature.pingPong[PING].shadowPramId);

        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &testConfig->res.edmaInSignature.pingPong[PONG].channel, &testConfig->res.edmaInSignature.pingPong[PONG].tcc,
                               &testConfig->res.edmaInSignature.pingPong[PONG].paramId, &testConfig->res.edmaInSignature.pingPong[PONG].shadowPramId);

        retVal = EDMA_unregisterIntr(gEdmaHandle[CONFIG_EDMA0], &rangeCfarProcIntrObj[0]);
        DebugP_assert(retVal == SystemP_SUCCESS);

        retVal = EDMA_unregisterIntr(gEdmaHandle[CONFIG_EDMA0], &rangeCfarProcIntrObj[1]);
        DebugP_assert(retVal == SystemP_SUCCESS);
        test_print("\n");

        test_print("%d Tests finished!\n\r", testIdx+1);
    }
    taskEndTime=ClockP_getTimeUsec();
    test_print("Test Time Cycles: %lld\n",taskEndTime-taskStartTime);
    test_print("All Tests finished!\n\r");
    if (finalResults == 0)
    {
        test_print("......TEST FAILS......\n");
    }
    else
    {
        test_print ("All tests have passed!!\n");
        test_print("......TEST PASSES......\n");
    }
    fclose(testResult_fileId);
    fclose(detMatrix_fileId);
    fclose(testVectors_fileId);
    /*delete the handler*/
    HWA_close(hwaHandle);
    DPU_RangeCFARProcHWA_deinit(rangeCfarProcDpuHandle);

    Board_driversClose();
    Drivers_close();

    test_print("--- Test Completed ---\n");

#ifndef SUBSYS_M4
    vTaskDelete(NULL);
#endif
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main(void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

#ifdef SUBSYS_M4
    void *args = NULL;
    rangecfarProcDpuTest_Task(args);
#else
    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic(rangecfarProcDpuTest_Task, /* Pointer to the function that implements the task. */
                                 "test_task_main",      /* Text name for the task.  This is to facilitate debugging only. */
                                 APP_TASK_STACK_SIZE,   /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                 NULL,                  /* We are not using the task parameter. */
                                 APP_TASK_PRI,          /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                 gAppTskStackMain,      /* pointer to stack base */
                                 &gAppTaskObj);         /* pointer to statically allocated task object memory */
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
