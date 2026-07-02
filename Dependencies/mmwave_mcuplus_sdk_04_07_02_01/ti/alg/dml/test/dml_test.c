/**
 *   @file  dml_test.c
 *
 *   @brief
 *      Unit Test code for DML.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2025 Texas Instruments, Inc.
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
#include <string.h>
#include <math.h>

#ifdef SUBSYS_DSS
#include <ti/alg/dml/test/dssgenerated/ti_drivers_config.h>
#include <ti/alg/dml/test/dssgenerated/ti_board_config.h>
#include <ti/alg/dml/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/alg/dml/test/dssgenerated/ti_board_open_close.h>
#elif SUBSYS_M4
#include <ti/alg/dml/test/m4generated/ti_drivers_config.h>
#include <ti/alg/dml/test/m4generated/ti_board_config.h>
#include <ti/alg/dml/test/m4generated/ti_drivers_open_close.h>
#include <ti/alg/dml/test/m4generated/ti_board_open_close.h>
#endif

#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/CycleCounterP.h>
#if defined(SOC_AWR2X44P)
#include <drivers/hw_include/awr2x44p/cslr_soc_defines.h>
#else
#include <drivers/hw_include/awr294x/cslr_soc_defines.h>
#endif
#ifndef SUBSYS_M4
#include "FreeRTOS.h"
#include "task.h"
#endif
/* mmwave SDK files */
#include <ti/common/syscommon.h>
#include <ti/alg/dml/dml.h>
#include <ti/utils/mathutils/mathutils.h>

#include <ti/alg/dml/test/dml_test.h>

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

#define DML_TEST_L3_SIZE (0x7000U)
/*! L3 RAM buffer for object detection DPC */
#ifdef SUBSYS_M4
uint8_t gL3Ram[DML_TEST_L3_SIZE] __attribute__((aligned(4096), section(".l3ram")));
#endif
#ifdef SUBSYS_DSS
uint8_t gL3Ram[DML_TEST_L3_SIZE];
#pragma DATA_ALIGN(gL3Ram, 4096U);
#pragma DATA_SECTION(gL3Ram, ".l3ram");
#endif

/*! Local RAM buffer for DML test */
#define DML_TEST_LOCAL_RAM_SIZE (5U * 1024U)

#ifdef SUBSYS_M4
uint8_t localRAM[DML_TEST_LOCAL_RAM_SIZE] __attribute__((section(".localRAM")));
#endif
#ifdef SUBSYS_DSS
uint8_t localRAM[DML_TEST_LOCAL_RAM_SIZE];
#pragma DATA_ALIGN(localRAM, 4096U);
#pragma DATA_SECTION(localRAM, ".localRAM");
#endif
/*
 * @brief Memory Configuration used during init API
 */
typedef struct DML_HWATest_MemCfg_t
{
    /*! @brief   Start address of memory provided by the application
     *           from which DPC will allocate.
     */
    void *addr;

    /*! @brief   Size limit of memory allowed to be consumed by the DPC */
    uint32_t size;
} DML_HWATest_MemCfg;

/*
 * @brief Memory pool object to manage memory
 */
typedef struct MemPoolObj_t
{
    /*! @brief Memory configuration */
    DML_HWATest_MemCfg cfg;

    /*! @brief   Pool running adress.*/
    uintptr_t currAddr;

    /*! @brief   Pool max address. This pool allows setting address to desired
     *           (e.g for rewinding purposes), so having a running maximum
     *           helps in finding max pool usage
     */
    uintptr_t maxCurrAddr;
} MemPoolObj;

/* Error codes */
#define DML_HWA_TEST_ENOMEM -100
#define DML_HWA_TEST_EINVAL -101


/* EDMA channels */
#define EDMA_DML_AZIMFFT_IN         EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ0
#define EDMA_DML_SQABS_OUT          EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ1
#define EDMA_DML_AZIMFFT_IN_HOTSIG  EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ2
#define EDMA_DML_MAX_CIJ            EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ3

#define DML_EDMA_SHADOW_BASE  64

/* EDMA shadow channels */
#define EDMA_DML_AZIMFFT_IN_SHADOW      (DML_EDMA_SHADOW_BASE + 1U)
#define EDMA_DML_SQABS_OUT_SHADOW       (DML_EDMA_SHADOW_BASE + 2U)
#define EDMA_DML_AZIMFFT_IN_HOTSIG_SHADOW   (DML_EDMA_SHADOW_BASE + 3U)
#define EDMA_DML_MAX_CIJ_SHADOW            (DML_EDMA_SHADOW_BASE + 4U)

#define DML_Test_PARAMSET_START_IDX     0

#define MAX_ERROR_CIJ 5
#define MAX_ERROR_IDX 0

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
#define CONST_10LOG10_2 3.0103

/* Test structure */
typedef struct DMLTestConfig_t {
    DML_HWA_StaticConfig dmlTestStaticCfg;
    DML_HWA_HW_Resources hwRes;
    uint32_t                        numBytesToRead;
    bool                            readDataAgain;
}DMLTestConfig_t;

/* Global structures */
DMLTestConfig_t testConfigStruct;
DMLTestConfig_t * testConfig = &testConfigStruct;

HWA_Handle  hwaHandle;
/* test configuration */
DML_HWA_Handle    DMLHandle __attribute__((aligned(32)));
DML_HWA_cfg    dmlTestConfig __attribute__((aligned(32)));

/**************************************************************/
/******************** DML TEST FUNCTIONS **********************/
/**************************************************************/
/**
 *  @b Description
 *  @n
 *      Allocates Shawdow paramset
 *
 *  @param[in] edmaHandle EDMA Handle
 *  @param[in] param Set the value to TCC to be allocated.
 *
 *  @retval
 *      none
 */
static void allocateEDMAShadowChannel(EDMA_Handle edmaHandle, uint32_t *param)
{
    int32_t             testStatus = SystemP_SUCCESS;
    EDMA_Config        *config;
    EDMA_Object        *object;

    config = (EDMA_Config *) edmaHandle;
    object = config->object;

    if((object->allocResource.paramSet[*param / 32] & (1U << *param % 32)) != (1U << *param % 32))
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
 *  \ingroup DML_HWATest__INTERNAL_FUNCTION
 *
 *  @retval
 *      none
 */
static void DML_HWATest_MemPoolReset(MemPoolObj *pool)
{
    pool->currAddr = (uintptr_t)pool->cfg.addr;
    pool->maxCurrAddr = pool->currAddr;
}

/**
 *  @b Description
 *  @n
 *      Utility function for getting memory pool current address.
 *
 *  @param[in]  pool Handle to pool object.
 *
 *  \ingroup DML_HWATest__INTERNAL_FUNCTION
 *
 *  @retval
 *      pointer to current address of the pool (from which next allocation will
 *      allocate to the desired alignment).
 */
static void *DML_HWATest_MemPoolGet(MemPoolObj *pool)
{
    return((void *)pool->currAddr);
}

/**
 *  @b Description
 *  @n
 *      Utility function for allocating from a static memory pool.
 *
 *  @param[in]  pool Handle to pool object.
 *  @param[in]  size Size in bytes to be allocated.
 *  @param[in]  align Alignment in bytes
 *
 *  \ingroup DML_HWATest__INTERNAL_FUNCTION
 *
 *  @retval
 *      pointer to beginning of allocated block. NULL indicates could not
 *      allocate.
 */
static void *DML_HWATest_MemPoolAlloc(MemPoolObj *pool,
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
 *     Assumes paramsetNumber = channelNumber = TCC
 *
 *  @param[in]  handle   EDMA handle
 *  @param[in]  chNum    DMA channel number
 *  @param[in]  shadowParamId    DMA shadow paramId
 *  @param[in]  eventQueue    Event queue num
 *  @param[out]  chanCfg    Stores channel configuration
 *  @retval   None
 *
 */
void DML_HWATest_EDMAChannelConfigAssist(EDMA_Handle handle,
                                        uint32_t chNum,
                                        uint32_t shadowParam,
                                        uint32_t eventQueue,
                                        DPEDMA_ChanCfg *chanCfg)
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
 *  Quantizes a real-world value x to an integer equivalent in the <I.F, S> notation.
 *
 *  @param[in]  inVec Input Vector
 *  @param[out]  outVec Output Vector
 *  @param[in]  numSamples Number of samples
 *  @param[in]  I I value <I.F, S>
 *  @param[in]  F F value <I.F, S>
 *  @param[in]  S Signed
 *  @param[in]  type Type
 *
 *  @retval Error code
 *
 */
int32_t mathUtils_asymQuantIntType(float * inVec,
                                    void * outVec,
                                    uint32_t numSamples,
                                    uint32_t I,
                                    uint32_t F,
                                    uint32_t S,
                                    uint32_t type)
{

    uint32_t bits, peak, i;
    int32_t xVal, q;

    if(inVec == NULL || outVec == NULL){
        return -1;
    }

    /* Number of bits and clip value for the asymmetric quantizer */
    bits = I + F + 1 - S;
    peak = 1 << (I - S);

    q = 1 << (bits - 1);

    for(i = 0; i < numSamples; i++){

        xVal = MATHUTILS_ROUND_FLOAT(q * inVec[i] / peak);
        xVal = (xVal > q - 1) ? (q - 1) : xVal;
        if(S){
            xVal = (xVal < -q) ? (-q) : (xVal);
        }
        else{
            xVal = (xVal < 0) ? (0) : (xVal);
        }
        if (S){
            if(type == 0){
                *((int16_t *)outVec + i) = xVal;
            }
            else if(type == 1){
                *((int32_t *)outVec + i) = xVal;
            }
            else if(type == 2){
                *((int64_t *)outVec + i) = xVal;
            }
        }
        else{
            if(type == 0){
                *((uint16_t *)outVec + i) = xVal;
            }
            else if(type == 1){
                *((uint32_t *)outVec + i) = xVal;
            }
            else if(type == 2){
                *((uint64_t *)outVec + i) = xVal;
            }
        }
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Populates the config structure (HW Res allocation, static config, etc.)
 *
 *  @return Error code
 */
int32_t DML_test_Config()
{

    DML_HWA_StaticConfig * staticCfg;
    DML_HWA_HW_Resources * hwRes;

    staticCfg = &testConfig->dmlTestStaticCfg;
    memcpy(staticCfg, &testConfig->dmlTestStaticCfg, sizeof(DML_HWA_StaticConfig));

    DML_HWATest_MemPoolReset(L3ramObj);
    DML_HWATest_MemPoolReset(CoreLocalRamObj);

    int32_t retVal = 0;
    void * l3RamStartPoolAddrNext;
    void * CoreLocalScratchStartPoolAddrNext;

    hwRes = &testConfig->hwRes;
    DML_HWA_EdmaCfg *edmaCfg = &hwRes->edmaCfg;
    DML_HWA_HwaCfg *hwaCfg = &hwRes->hwaCfg;
    DPIF_Buf azimFFTInBuf, bijBuf, b1ijBuf;

    hwRes->edmaCfg.edmaHandle = gEdmaHandle[CONFIG_EDMA0];

    staticCfg->azimFFTCfg.numSamples = DMLtest_numAntennas;
    staticCfg->azimFFTCfg.nFFTbins = DMLAzimFFTBins;

    /* Azim FFT bins should be a multiple of 2 or 3 */
    if(((staticCfg->azimFFTCfg.nFFTbins % 2) != 0) && ((staticCfg->azimFFTCfg.nFFTbins % 3) != 0))
    {
        retVal = DML_HWA_ERROR_NUM_AZIMUTH_FFT_BINS;
        goto exit;
    }
    staticCfg->n1 = DMLtest_n1;
    staticCfg->n2 = DMLtest_n2;
    if(staticCfg->n2 > staticCfg->n1)
    {
        staticCfg->numSteerVecs = staticCfg->n2 - staticCfg->n1 + 1;
    }
    else
    {
        staticCfg->numSteerVecs = staticCfg->n2 - (staticCfg->n1 - staticCfg->azimFFTCfg.nFFTbins) + 1;
    }

    staticCfg->virtualAntennaArraySize = DMLVirtualAntennaSize;

    if(staticCfg->n2 > staticCfg->n1)
    {
        if(((DMLPeakLoc[0] < (staticCfg->n1 / (float)staticCfg->azimFFTCfg.nFFTbins) * staticCfg->virtualAntennaArraySize)
            || (DMLPeakLoc[0] > (staticCfg->n2 / (float)staticCfg->azimFFTCfg.nFFTbins) * staticCfg->virtualAntennaArraySize)) &&
            ((DMLPeakLoc[1] < (staticCfg->n1 / (float)staticCfg->azimFFTCfg.nFFTbins) * staticCfg->virtualAntennaArraySize)
            || (DMLPeakLoc[1] > (staticCfg->n2 / (float)staticCfg->azimFFTCfg.nFFTbins) * staticCfg->virtualAntennaArraySize)))
        {
            retVal = DML_HWA_ERROR_INPDATA;
            goto exit;
        }
    }
    else
    {
        if(((DMLPeakLoc[0] < (staticCfg->n1 / (float)staticCfg->azimFFTCfg.nFFTbins) * staticCfg->virtualAntennaArraySize)
            && (DMLPeakLoc[0] > (staticCfg->n2 / (float)staticCfg->azimFFTCfg.nFFTbins) * staticCfg->virtualAntennaArraySize)) &&
            ((DMLPeakLoc[1] < (staticCfg->n1 / (float)staticCfg->azimFFTCfg.nFFTbins) * staticCfg->virtualAntennaArraySize)
            && (DMLPeakLoc[1] > (staticCfg->n2 / (float)staticCfg->azimFFTCfg.nFFTbins) * staticCfg->virtualAntennaArraySize)))
        {
            retVal = DML_HWA_ERROR_INPDATA;
            goto exit;
        }
    }

    staticCfg->sizeOfInputSamples = staticCfg->azimFFTCfg.numSamples * sizeof(cmplx32ImRe_t);
    staticCfg->zeroInsrtMaskAzim = DMLzeroInsertionMask;

    /* L3 allocations */
    /* L3 - Input of Azimuth FFT */
    azimFFTInBuf.dataSize = staticCfg->sizeOfInputSamples;
    azimFFTInBuf.data = DML_HWATest_MemPoolAlloc(L3ramObj, azimFFTInBuf.dataSize, sizeof(uint32_t));
    if (azimFFTInBuf.data == NULL)
    {
        retVal = DML_HWA_TEST_ENOMEM;
        goto exit;
    }

    /* L3 - Buffer to store bij values */
    bijBuf.dataSize = staticCfg->numSteerVecs * sizeof(cmplx16ImRe_t);
    bijBuf.data = DML_HWATest_MemPoolAlloc(L3ramObj, bijBuf.dataSize, sizeof(cmplx16ImRe_t));
    if (bijBuf.data == NULL)
    {
        retVal = DML_HWA_TEST_ENOMEM;
        goto exit;
    }

    /* Set the window ram offset */
    /* Ensure that winRamOffset + windowSize doesn't exceed the Window RAM size */
    hwaCfg->winRamOffset = DMLwindowRamOffset;
    hwaCfg->windowSize = (staticCfg->numSteerVecs - 1) * sizeof(int16_t) * 2;
    if(hwaCfg->winRamOffset + (hwaCfg->windowSize / sizeof(cmplx16ImRe_t)) > 2047U){
        retVal = DML_HWA_ERROR_WIN_RAM;
        goto exit;
    }

    /* L3 - Buffer to store b1ij values */
    b1ijBuf.dataSize = (staticCfg->numSteerVecs - 1) * sizeof(uint32_t) * 4;
    b1ijBuf.data = DML_HWATest_MemPoolAlloc(L3ramObj, b1ijBuf.dataSize, sizeof(uint32_t));
    if (b1ijBuf.data == NULL)
    {
        retVal = DML_HWA_TEST_ENOMEM;
        goto exit;
    }

    /* Set the Vector Multiplication RAM offset */
    /* Ensure that vecMultRamOffset + size of vectorMultRAMSamples doesn't exceed the Vector Multiplication RAM size */
    hwaCfg->vecMultRamOffset = DMLVectorMultRAMOffset;
    hwaCfg->vectorMultRAMSamples = (staticCfg->numSteerVecs - 1) + (b1ijBuf.dataSize / sizeof(cmplx32ImRe_t));
    if(hwaCfg->vecMultRamOffset + hwaCfg ->vectorMultRAMSamples > 1023U){
        retVal = DML_HWA_ERROR_VECT_MULT_RAM;
        goto exit;
    }

    /* Buffer for vector mult RAM offset update */
    hwRes->reconfigBuf.preproc_buf = DML_HWATest_MemPoolAlloc(CoreLocalRamObj,
                                                staticCfg->numSteerVecs * sizeof(uint32_t),
                                                sizeof(uint32_t));

    /* Buffer for srcAddr and srcAcnt update */
    hwRes->reconfigBuf.src1_buf = DML_HWATest_MemPoolAlloc(CoreLocalRamObj,
                                                staticCfg->numSteerVecs * sizeof(uint32_t) * 2,
                                                sizeof(uint32_t));

    /* Buffer for destAddr and destAcnt update */
    hwRes->reconfigBuf.dst1_buf = DML_HWATest_MemPoolAlloc(CoreLocalRamObj,
                                                staticCfg->numSteerVecs * sizeof(uint32_t) * 2,
                                                sizeof(uint32_t));

    /* Buffer for srcAddr and srcAcnt update */
    hwRes->reconfigBuf.src2_buf = DML_HWATest_MemPoolAlloc(CoreLocalRamObj,
                                                staticCfg->numSteerVecs * sizeof(uint32_t) * 2,
                                                sizeof(uint32_t));

    /* Buffer for destAddr update */
    hwRes->reconfigBuf.dst2_buf = DML_HWATest_MemPoolAlloc(CoreLocalRamObj,
                                                staticCfg->numSteerVecs * sizeof(uint32_t),
                                                sizeof(uint32_t));

    /* Buffer for channel combining size update */
    hwRes->reconfigBuf.chanComb_buf = DML_HWATest_MemPoolAlloc(CoreLocalRamObj,
                                                staticCfg->numSteerVecs * sizeof(uint32_t),
                                                sizeof(uint32_t));

    /********************************************
     * Allocating memory resources              *
     *******************************************/

    l3RamStartPoolAddrNext = DML_HWATest_MemPoolGet(L3ramObj);
    if (l3RamStartPoolAddrNext == NULL){
        retVal = DML_HWA_TEST_EINVAL;
        goto exit;
    }

    /* We don't need any L2 resources to be retained till the end of the next  */
    CoreLocalScratchStartPoolAddrNext = DML_HWATest_MemPoolGet(CoreLocalRamObj);
    if (CoreLocalScratchStartPoolAddrNext == NULL){
        retVal = DML_HWA_TEST_EINVAL;
        goto exit;
    }

    /* Assign the detection matrix, radar cube */
    hwRes->azimFFTIn = azimFFTInBuf;
    hwRes->bijBuf = bijBuf;
    hwRes->b1ijBuf = b1ijBuf;

    /********************************************
     * Allocating hw resources (decomp stage)   *
     *******************************************/
    {{
    DML_HWATest_EDMAChannelConfigAssist(hwRes->edmaCfg.edmaHandle,
                                       EDMA_DML_AZIMFFT_IN,
                                       EDMA_DML_AZIMFFT_IN_SHADOW,
                                       0,
                                       &edmaCfg->azimFFTEdmaCfg.edmaIn);

    DML_HWATest_EDMAChannelConfigAssist(hwRes->edmaCfg.edmaHandle,
                                       EDMA_DML_SQABS_OUT,
                                       EDMA_DML_SQABS_OUT_SHADOW,
                                       0,
                                       &edmaCfg->azimFFTEdmaCfg.edmaOut);

    DML_HWATest_EDMAChannelConfigAssist(hwRes->edmaCfg.edmaHandle,
                                       EDMA_DML_AZIMFFT_IN_HOTSIG,
                                       EDMA_DML_AZIMFFT_IN_HOTSIG_SHADOW,
                                       0,
                                       &edmaCfg->azimFFTEdmaCfg.edmaInSignature);

    hwaCfg->paramSetStartIdx = DML_Test_PARAMSET_START_IDX;
    hwaCfg->numParamSets = DML_NUM_HWA_PARAMSETS;

    if(hwaCfg->paramSetStartIdx + hwaCfg->numParamSets > 64)
    {
        retVal = DML_HWA_TEST_EINVAL;
    }

    }}

exit:
    return retVal;

}

#ifndef USE_GENERATED_BIJ_AND_B1IJ
/**
 *  @b Description
 *  @n
 *      Function to generate bij and b1ij values
 *
 *  @return Error code
 */
int32_t DML_generate_bij_and_b1ij()
{
    int32_t i, j, k, adder;
    float max_b1ij = 0;
    float divisionFactor = sqrtf(dmlTestConfig.staticCfg.azimFFTCfg.numSamples);
    float bij_abs;
    int32_t retVal = 0;

    DPIF_Buf steerVecBuf, bijGenBuf, b1ijGenBuf;

    /* Allocate space for generating steering vectors */
    steerVecBuf.dataSize = dmlTestConfig.staticCfg.numSteerVecs *
                        dmlTestConfig.staticCfg.virtualAntennaArraySize *
                        sizeof(float) * 2;
    steerVecBuf.data = DML_HWATest_MemPoolAlloc(L3ramObj, steerVecBuf.dataSize, sizeof(uint32_t));
    if (steerVecBuf.data == NULL)
    {
        retVal = DML_HWA_TEST_ENOMEM;
        goto exit;
    }

    /* Allocate space to generate bij */
    bijGenBuf.dataSize = dmlTestConfig.staticCfg.numSteerVecs * sizeof(float) * 2;
    bijGenBuf.data = DML_HWATest_MemPoolAlloc(L3ramObj, bijGenBuf.dataSize, sizeof(uint32_t));
    if (bijGenBuf.data == NULL)
    {
        retVal = DML_HWA_TEST_ENOMEM;
        goto exit;
    }

    /* Allocate space to generate b1ij */
    b1ijGenBuf.dataSize = dmlTestConfig.staticCfg.numSteerVecs * sizeof(float) * 4;
    b1ijGenBuf.data = DML_HWATest_MemPoolAlloc(L3ramObj, b1ijGenBuf.dataSize, sizeof(uint32_t));
    if (b1ijGenBuf.data == NULL)
    {
        retVal = DML_HWA_TEST_ENOMEM;
        goto exit;
    }

    /* Generate steering vectors */
    if(dmlTestConfig.staticCfg.n1 < dmlTestConfig.staticCfg.n2)
    {
        for(j = 0; j < dmlTestConfig.staticCfg.virtualAntennaArraySize; j++)
        {
            if((dmlTestConfig.staticCfg.zeroInsrtMaskAzim >> j) & 0x1U)
            {
                adder = j * dmlTestConfig.staticCfg.numSteerVecs;
                for(i = dmlTestConfig.staticCfg.n1, k = 0; i <= dmlTestConfig.staticCfg.n2; i++, k++)
                {
                    *((float *)steerVecBuf.data + (2 * (k + adder))) =
                        cos((2.0f * PI_ * i * j) / dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins) /
                        divisionFactor;
                    *((float *)steerVecBuf.data + (2 * (k + adder)) + 1) =
                        sin((2.0f * PI_ * i * j) / dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins) /
                        divisionFactor;
                }
            }
        }
    }
    else
    {
        for(j = 0; j < dmlTestConfig.staticCfg.virtualAntennaArraySize; j++)
        {
            if((dmlTestConfig.staticCfg.zeroInsrtMaskAzim >> j) & 0x1U)
            {
                adder = j * dmlTestConfig.staticCfg.numSteerVecs;
                for(i = dmlTestConfig.staticCfg.n1 - dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins, k = 0; i <= dmlTestConfig.staticCfg.n2; i++, k++)
                {
                    *((float *)steerVecBuf.data + (2 * (k + adder))) =
                        cos((2.0f * PI_ * i * j) / dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins) /
                        divisionFactor;
                    *((float *)steerVecBuf.data + (2 * (k + adder)) + 1) =
                        sin((2.0f * PI_ * i * j) / dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins) /
                        divisionFactor;
                }
            }
        }
    }

    /* Matix multiplication of steervec and conjugate of steervec transpose to get bij matrix */
    /* Since we get a symmetric matrix (and circular), we only need 1st row elements */
    i = 0;
    for(k = 0; k < dmlTestConfig.staticCfg.numSteerVecs; k++)
    {
        for(j = 0; j < dmlTestConfig.staticCfg.virtualAntennaArraySize; j++)
        {
            adder = j * dmlTestConfig.staticCfg.numSteerVecs;
            *((float *)bijGenBuf.data + (2 * k)) = *((float *)bijGenBuf.data + (2 * k)) + \
                ((*((float *)steerVecBuf.data + ((adder + i) * 2))) * (*((float *)steerVecBuf.data + ((adder + k) * 2)))) + \
                ((*((float *)steerVecBuf.data + ((adder + i) * 2) + 1)) * (*((float *)steerVecBuf.data + ((adder + k) * 2) + 1)));
            *((float *)bijGenBuf.data + (2 * k) + 1) = *((float *)bijGenBuf.data + (2 * k) + 1) + \
                ((*((float *)steerVecBuf.data + ((adder + i) * 2))) * (*((float *)steerVecBuf.data + ((adder + k) * 2) + 1))) - \
                ((*((float *)steerVecBuf.data + ((adder + i) * 2) + 1)) * (*((float *)steerVecBuf.data + ((adder + k) * 2))));
        }

        /* We need -2bij to be stored in the Window RAM.*/
        /* If we multiply bij by -2, the values will exceed 1, so we should divide by 2 for normalization which will finally result -bij */
        if (k != 0)
        {
            (*((float *)bijGenBuf.data + (2 * k))) *= -1;
            (*((float *)bijGenBuf.data + (2 * k) + 1)) *= -1;

            bij_abs = ((*((float *)bijGenBuf.data + (2 * k))) * (*((float *)bijGenBuf.data + (2 * k)))) + \
                ((*((float *)bijGenBuf.data + (2 * k) + 1)) * (*((float *)bijGenBuf.data + (2 * k) + 1)));
            *((float *)b1ijGenBuf.data + (4 * k)) = 1/(1 - bij_abs);

            if(*((float *)b1ijGenBuf.data + (4 * k)) > max_b1ij)
            {
                max_b1ij = *((float *)b1ijGenBuf.data + (4 * k));
            }
        }

    }

    DML_scale_factor_b1ij = ceil(log2(max_b1ij));
    /* Normalize b1ij matrix */
    for(k = 0; k < dmlTestConfig.staticCfg.numSteerVecs; k++)
    {
        *((float *)b1ijGenBuf.data + (4 * k)) = *((float *)b1ijGenBuf.data + (4 * k)) / pow(2, DML_scale_factor_b1ij);
        *((float *)b1ijGenBuf.data + (4 * k) + 2) = *((float *)b1ijGenBuf.data + (4 * k));
    }

    /* Evaluate the float bij values into quantized values acceptable by HWA */
    retVal = mathUtils_asymQuantIntType((float *)bijGenBuf.data + 2,
                                    (void *)dmlTestConfig.hwRes.bijBuf.data,
                                    dmlTestConfig.staticCfg.numSteerVecs * 2 - 2,
                                    1,
                                    15,
                                    1, /* Signed */
                                    0); /* 16 bit array */
    if (retVal != 0)
    {
        goto exit;
    }

    /* Evaluate the float b1ij values into quantized values acceptable by HWA */
    retVal = mathUtils_asymQuantIntType((float *)b1ijGenBuf.data + 4,
                                    (void *)dmlTestConfig.hwRes.b1ijBuf.data,
                                    (dmlTestConfig.staticCfg.numSteerVecs - 1) * 4,
                                    1,
                                    20,
                                    1, /* Signed */
                                    1); /* b1ijBuf is 32 bit array */
    if (retVal != 0)
    {
        goto exit;
    }

exit:
    return retVal;
}
#endif

/**
 * @brief DML test task
 *
 * @param args
 */
void DML_Test_task(void* args)
{
    uint64_t taskStartTime, taskEndTime;
    int32_t retVal;
    float orig_peak[2];
    float amp_x, amp_y;
    float energy_diff;

    CycleCounterP_reset();

    DML_HWA_OutParams out;

    Drivers_open();
    Board_driversOpen();

    DebugP_log("...... Initialization ...... \n");

    DML_HWATest_MemCfg L3ramCfg, CoreLocalRamCfg;
    L3ramCfg.addr = (void *)&gL3Ram[0];
    L3ramCfg.size = sizeof(gL3Ram);
    CoreLocalRamCfg.addr = &localRAM[0];
    CoreLocalRamCfg.size = sizeof(localRAM);

    L3ramObj->cfg = L3ramCfg;
    CoreLocalRamObj->cfg = CoreLocalRamCfg;

    DebugP_log("DML Test start ...... \n");

    /* hwa, edma, DML alg inialization */
    hwaHandle = HWA_open(0, NULL, &retVal);
    if (hwaHandle == NULL)
    {
        DebugP_log("HWA failed to open\n");
        DebugP_assert(0);
    }

    DMLHandle = DML_HWA_init(hwaHandle, &retVal);
    if (DMLHandle == NULL)
    {
        DebugP_log("DML algo failed to initialize. Error %d.\n", retVal);
        return;
    }

    /* DML alg config*/
    retVal = DML_test_Config();
    if (retVal != 0)
    {
        DebugP_log("DML test failed to configure. Error %d.\n", retVal);
        DebugP_assert(0);
    }

    memcpy(&dmlTestConfig.staticCfg, &testConfig->dmlTestStaticCfg, sizeof(DML_HWA_StaticConfig));
    memcpy(&dmlTestConfig.hwRes, &testConfig->hwRes, sizeof(DML_HWA_HW_Resources));

    /* Copy test data (azimuth samples) to the buffers */
    memcpy((uint32_t *)dmlTestConfig.hwRes.azimFFTIn.data, &DMLtest_azimSamples, dmlTestConfig.hwRes.azimFFTIn.dataSize);

#ifdef USE_GENERATED_BIJ_AND_B1IJ
    /* Copy bij and b1ij to the buffers */
    memcpy((uint32_t *)dmlTestConfig.hwRes.bijBuf.data, &DMLtest_bij, dmlTestConfig.hwRes.bijBuf.dataSize);
    memcpy((uint32_t *)dmlTestConfig.hwRes.b1ijBuf.data, &DMLtest_b1ij, dmlTestConfig.hwRes.b1ijBuf.dataSize);
#else
    retVal = DML_generate_bij_and_b1ij();
    if (retVal != 0)
    {
        DebugP_log("DML test failed to generate bij and b1ij. Error %d.\n", retVal);
        DebugP_assert(0);
    }
#endif

    /* Configure HWA to perform DML */
    retVal = DML_HWA_config(DMLHandle, &dmlTestConfig);
    if (retVal != 0)
    {
        DebugP_log("Error in configuring HWA with DML configuration. Error %d.\n", retVal);
        DebugP_assert(0);
    }

    /* HWA is enabled at the start of DML_HWA_process function. */
    /* Few of the HWA commonn registers are configured at the start of DML_HWA_process function. So, HWA should be disabled. */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        DebugP_assert(0);
    }

    /**
     * Ensure that the HWA common config uses the same configuration as mentioned below.
     * If the below config is updated in the processing chain, reconfigure the channel combining vector
     * before running the DML algorithm.
     * for(i = 0; i < 8; i++)
     * {
     *      hwaCommonConfig.chanCombConfig.vector[i] = 0x33333333; // Sequence of 1100
     * }
    */

    taskStartTime = ClockP_getTimeUsec();

    /* Start DML algorithm on HWA */
    retVal = DML_HWA_process(DMLHandle, &dmlTestConfig, &out);
    if (retVal != 0)
    {
        DebugP_log("DML failed to run. Error %d.\n", retVal);
        DebugP_assert(0);
    }

    taskEndTime = ClockP_getTimeUsec();

    /**
     * HWA is disabled at the end of DML_HWA_process function.
    */

    /* Compute the original peak location (interms of azimuth sample number) */
    orig_peak[0] = ((dmlTestConfig.staticCfg.n1 + out.peak_x) % dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins) /
                (float)dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins * dmlTestConfig.staticCfg.virtualAntennaArraySize;
    orig_peak[1] = ((dmlTestConfig.staticCfg.n1 + out.peak_y) % dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins) /
                (float)dmlTestConfig.staticCfg.azimFFTCfg.nFFTbins * dmlTestConfig.staticCfg.virtualAntennaArraySize;

    DebugP_log("Peak location: %d, %d\n", out.peak_x, out.peak_y);
    DebugP_log("Original Peak location: %f, %f\n", orig_peak[0], orig_peak[1]);

    /* Compute amplitude of each target */ // TODO: Verify this logic
    amp_x = sqrt((out.result_MaxCij.real - (out.sqAbs_x >> (DML_scale_factor_b1ij + 1))) *
        ((*((uint32_t *)dmlTestConfig.hwRes.b1ijBuf.data + (out.peak_x_b1ij << 2))) /
        pow(2, 20U - DML_scale_factor_b1ij)));
    amp_y = sqrt((out.result_MaxCij.real - (out.sqAbs_y >> (DML_scale_factor_b1ij + 1))) *
        ((*((uint32_t *)dmlTestConfig.hwRes.b1ijBuf.data + (out.peak_x_b1ij << 2))) /
        pow(2, 20U - DML_scale_factor_b1ij)));
    energy_diff = (log2f(amp_x/amp_y)) * CONST_10LOG10_2 * 2;

    DebugP_log("Scaled Peak amplitude: %f, %f\n", amp_x, amp_y);
    DebugP_log("Energy difference (dB): %f\n", energy_diff);

#ifndef USE_GENERATED_BIJ_AND_B1IJ
    /* Validate the results from HWA */
    if(((peak_index - out.result_MaxCij.imag) <= MAX_ERROR_IDX) &&
        ((peak_index - out.result_MaxCij.imag) >= (MAX_ERROR_IDX * -1)))
    {
        if(((peak_cij - out.result_MaxCij.real) <= MAX_ERROR_CIJ) &&
            ((peak_cij - out.result_MaxCij.real) >= (MAX_ERROR_CIJ * -1)))
        {
            DebugP_log("Results matched!\n");
        }
        else
        {
            DebugP_log("Results doesn't match!\n");
        }
    }
    else
    {
        DebugP_log("Results doesn't match!\n");
    }
#endif

    /* hwa, edma, DML alg deinialization */
    retVal = DML_HWA_deinit(DMLHandle);
    if (retVal != 0)
    {
        DebugP_log("Failed to deinitialize. Error %d.\n", retVal);
        DebugP_assert(0);
    }

    retVal = HWA_close(hwaHandle);
    if (retVal != 0)
    {
        DebugP_log("HWA failed to close\n");
        DebugP_assert(0);
    }

    DebugP_log ("--- Test Completed ---\n");
    DebugP_log("----------------------------\n\n");

    DebugP_log("Test Time Cycles: %lld\n",taskEndTime-taskStartTime);

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
    DML_Test_task(args);
#else
    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( DML_Test_task,   /* Pointer to the function that implements the task. */
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
