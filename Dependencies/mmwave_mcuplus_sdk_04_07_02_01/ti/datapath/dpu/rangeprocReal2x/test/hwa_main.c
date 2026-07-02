/**
 *   @file  hwa_main.c
 *
 *   @brief
 *      Unit Test code for range hwa dpu processing.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 -2023 Texas Instruments, Inc.
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
#include <drivers/hw_include/awr2544/cslr_soc_baseaddress.h>
#include <drivers/edma.h>
#include <ti/datapath/dpu/rangeprocReal2x/test/mssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/rangeprocReal2x/test/mssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/rangeprocReal2x/test/mssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/rangeprocReal2x/test/mssgenerated/ti_board_open_close.h>
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmwave SDK files */
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpu/rangeprocReal2x/rangeprochwaReal2x.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/demo/awr2544/mmw/mmw_res.h>

#define MAX_NUM_ADCSAMPLES 384
#define MAX_NUM_RANGEBINS  192
#define MAX_NUM_RX_ANTENNA 4
#define MAX_NUM_ADCSAMPLE_PERCHIRP (MAX_NUM_ADCSAMPLES * MAX_NUM_RX_ANTENNA)
#define MAX_NUM_BINS_PERCHIRP (MAX_NUM_RANGEBINS * MAX_NUM_RX_ANTENNA)
#define MAX_ALLOWED_ERROR 32
#define L3_SIZE      0x80000U // CSL_DSS_L3_U_SIZE
#define TEST_CHIRPS_2X 96U

#define DPC_USE_SYMMETRIC_WINDOW_RANGE_DPU
#define DPC_DPU_RANGEPROC_FFT_WINDOW_TYPE MATHUTILS_WIN_HANNING
#define DPC_COMPFFT_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL (16U)
#define DPC_DPU_RANGEPROC_INTERFMITIG_WINDOW_TYPE MATHUTILS_WIN_HANNING
#define DPC_COMPFFT_QFORMAT_RANGEPROC_INTERFMITIG_WINDOW (5U)
#define DPC_MAX_PAYLOAD_SIZE    4096U// 512U //1536U

/* Test Error Codes */
#define DPU_RANGEPROCREAL2XTEST_ENOMEM__L3_RAM_RADAR_CUBE (-2)
#define DPU_RANGEPROCREAL2XTEST_RANGE_BINS_ERR (-3)
#define DPU_RANGEPROCREAL2XTEST_ENOMEM__CORE_LOCAL_RAM_RANGE_HWA_WINDOW (-4)
#define DPU_RANGEPROCREAL2XTEST_ENOMEM_HWA_WINDOW_RAM (-5)
#define DPU_RANGEPROCREAL2XTEST_L3_REUSE_ITERATIONS (-6)
#define DPU_RANGEPROCREAL2XTEST_PAYLOAD_SIZE (-7)
#define DPU_RANGEPROCREAL2XTEST_2XMODE_ERR  (-8)
#define DPC_COMPFFT_HWA_MAX_WINDOW_RAM_SIZE_IN_SAMPLES (CSL_DSS_HWA_WINDOW_RAM_U_SIZE >> 3)
/*! L2 RAM buffer */
#define MMWDEMO_COMPFFT_L2RAM_SIZE (2U * 1024U)
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT  DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F
/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(2 * 9216);

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI (5U)
#define APP_TASK_STACK_SIZE (6 * MAX_NUM_RANGEBINS)

typedef struct cmplxUint16ImRe_t_
{
    uint16_t imag; /*!< @brief imaginary part */
    uint16_t real; /*!< @brief real part */
} cmplxUint16ImRe_t;


/*
 * @brief Memory Configuration used during init API
 */
typedef struct DPU_RangeProcTest_MemCfg_t
{
    /*! @brief   Start address of memory provided by the application
     *           from which DPC will allocate.
     */
    void *addr;

    /*! @brief   Size limit of memory allowed to be consumed by the DPC */
    uint32_t size;
} DPU_RangeProcTest_MemCfg;

/*
 * @brief Memory pool object to manage memory
 */
typedef struct MemPoolObj_t
{
    /*! @brief Memory configuration */
    DPU_RangeProcTest_MemCfg cfg;

    /*! @brief   Pool running adress.*/
    uintptr_t currAddr;

    /*! @brief   Pool max address. This pool allows setting address to desired
     *           (e.g for rewinding purposes), so having a running maximum
     *           helps in finding max pool usage
     */
    uintptr_t maxCurrAddr;
} MemPoolObj;


/*
 * @brief Static Configuration for the test.
 */
typedef struct DPU_RangeProcTest_StaticCfg_t
{
    /*! @brief      ADCBuf buffer interface */
    DPIF_ADCBufData ADCBufData;

    /*! @brief  Number of transmit antennas */
    uint8_t numTxAntennas;

    /*! @brief  Number of virtual antennas */
    uint8_t numVirtualAntennas;

    /*! @brief  Number of range FFT bins, this is at a minimum the next power of 2 of
               @ref DPIF_ADCBufProperty_t::numAdcSamples, in case of complex ADC data,
               and half that value in case of real only data.
                If range zoom is supported, this can be bigger than the minimum. */
    uint16_t numRangeBins;

    /*! @brief  Number of bins used in Range FFT Calculation. In case of real only samples,
                this is twice the number of range bins. Else it is equal to the number
                of range bins. */
    uint16_t numRangeFFTBins;

    /*! @brief  Number of chirps per frame */
    uint16_t numChirpsPerFrame;

    /*! @brief Number of chirps for Doppler computation purposes. */
    uint16_t numChirps;

    /*! @brief  1 if ADC Samples are real */
    uint16_t isChirpDataReal;

    /*! @brief 1 if 2X Mode is enabled for real data */
    uint16_t  isMode2x;

    /*! @brief     Compression Cfg */
    DPU_RangeProcHWA_CompressionCfg compressionCfg;

    /*! @brief Shift/Scale config for Interf Stats Mag Diff in range DPU */
    DPU_RangeProcHWA_intfStatsdBCfg intfStatsdBCfg;

} DPU_RangeProcTest_StaticCfg;

/*! L3 RAM buffer */
uint8_t gL3Ram[L3_SIZE] __attribute__((section(".l3ram")));
float achievedCompressionRatio;

uint8_t gL2Heap[MMWDEMO_COMPFFT_L2RAM_SIZE] __attribute__((aligned(4096), section(".dpc_l2Heap")));

uint32_t adcDataIn[MAX_NUM_BINS_PERCHIRP  *TEST_CHIRPS_2X]
    __attribute__((aligned(CSL_CACHE_L1D_LINESIZE), section(".l3ram")));


uint8_t gRadarCubeIdealBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_BINS_PERCHIRP * sizeof(cmplx16ImRe_t) * TEST_CHIRPS_2X,
                                                DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)]
    __attribute__((aligned(DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT), section(".dataBuf")));

cmplxUint16ImRe_t *gRadarCubeIdeal = (cmplxUint16ImRe_t *)&gRadarCubeIdealBuf[0];

DPU_RangeProcTest_StaticCfg testStaticCfg;
DPU_RangeProcTest_StaticCfg *staticCfg = &testStaticCfg;

/*! @brief L3 ram memory pool object */
MemPoolObj L3ramObjS;
MemPoolObj *L3ramObj = &L3ramObjS;

/*! @brief Core Local ram memory pool object */
MemPoolObj CoreLocalRamObjS;
MemPoolObj *CoreLocalRamObj = &CoreLocalRamObjS;

volatile uint8_t subframeCount = 0U;

DPU_RangeProcHWA_Handle rangeProcDpuHandle;
DPU_RangeProcHWA_Config rangeProcDpuCfg;
/*! @brief     EDMA interrupt object */
Edma_IntrObject intrObj;

uint8_t testFeatureStatus;

uint8_t checkResultFlag;

uint32_t numL3ReUseIterations;
uint32_t baseAddr, regionId;
uint32_t vectorReadPtr=0, dataReadPtr=0, resultReadPtr=0;

TaskHandle_t gAppTask;
StaticTask_t gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));

FILE *testResultFileId = NULL;
FILE *testVectorsFileId = NULL;
FILE *testDataFileId = NULL;

/**
 *  @b Description
 *  @n
 *      Computes the length of window to generate for range DPU.
 *
 *  @param[in]  cfg Range DPU configuration
 *
 *  @retval   Length of window to generate
 *
 * \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 */
static uint32_t DPU_RangeProcTest_GetRangeWinGenLen(DPU_RangeProcHWA_Config *cfg)
{
    uint16_t numAdcSamples;
    uint32_t winGenLen;

    numAdcSamples = cfg->staticCfg.ADCBufData.dataProperty.numAdcSamples;

#ifdef DPC_USE_SYMMETRIC_WINDOW_RANGE_DPU
    winGenLen = (numAdcSamples + 1) / 2;
#else
    winGenLen = numAdcSamples;
#endif
    return (winGenLen);
}

/**
 *  @b Description
 *  @n
 *      Generate the range DPU window using mathutils API.
 *
 *  @param[in]  cfg Range DPU configuration, output window is generated in window
 *                  pointer in the staticCfg of this.
 *
 *  @retval   None
 *
 * \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 */
static void DPU_RangeProcTest_GenRangeWindow(DPU_RangeProcHWA_Config *cfg)
{

    /* Symmetric window */
    uint32_t interfMitigWindow[DPC_COMPFFT_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL >> 1];
    uint8_t idx;

    mathUtils_genWindow((uint32_t *)interfMitigWindow,
                        DPC_COMPFFT_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL,
                        DPC_COMPFFT_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL >> 1,
                        DPC_DPU_RANGEPROC_INTERFMITIG_WINDOW_TYPE,
                        DPC_COMPFFT_QFORMAT_RANGEPROC_INTERFMITIG_WINDOW);

    /* Only 5 win samples are supported by the HWA */
    for (idx = 0; idx < DPU_RANGEPROCHWA_INTFMITIG_WIN_SIZE; idx++)
    {
        cfg->hwRes.hwaCfg.hwaInterfMitigWindow[DPU_RANGEPROCHWA_INTFMITIG_WIN_SIZE - 1 - idx] =
            (uint8_t)interfMitigWindow[(DPC_COMPFFT_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL >> 1) - 2 - idx];
    }

    if(cfg->staticCfg.isMode2x)
        /* Range FFT window */
        mathUtils_genWindowQ15((uint16_t *)cfg->staticCfg.window,
                            cfg->staticCfg.ADCBufData.dataProperty.numAdcSamples,
                            DPU_RangeProcTest_GetRangeWinGenLen(cfg),
                            DPC_DPU_RANGEPROC_FFT_WINDOW_TYPE,
                            15U);
    else
    {
        /* Range FFT window */
        mathUtils_genWindow((uint32_t *)cfg->staticCfg.window,
                            cfg->staticCfg.ADCBufData.dataProperty.numAdcSamples,
                            DPU_RangeProcTest_GetRangeWinGenLen(cfg),
                            DPC_DPU_RANGEPROC_FFT_WINDOW_TYPE,
                            17U);
    }
}

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
static void DPU_RangeProcTest_MemPoolReset(MemPoolObj *pool)
{
    pool->currAddr = (uintptr_t)pool->cfg.addr;
    pool->maxCurrAddr = pool->currAddr;
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
 *  \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 *
 *  @retval
 *      pointer to beginning of allocated block. NULL indicates could not
 *      allocate.
 */
static void *DPU_RangeProcTest_MemPoolAlloc(MemPoolObj *pool,
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
 *     Datapath assumes paramsetNumber = channelNumber = TCC
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
void DPU_RangeProcReal2x_EDMAChannelConfigAssist(EDMA_Handle handle, uint32_t chNum, uint32_t shadowParam, uint32_t eventQueue, DPEDMA_ChanCfg *chanCfg)
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
 *     Function calls EDMA param, channel, tcc allocation with 3 link channels.
 *     Datapath assumes paramsetNumber = channelNumber = TCC
 *
 *  @param[in]  handle   EDMA handle
 *  @param[in]  chNum    DMA channel number
 *  @param[in]  shadowParamId1    DMA shadow paramId
 *  @param[in]  shadowParamId2    DMA shadow paramId
 *  @param[in]  shadowParamId3    DMA shadow paramId
 *  @param[in]  eventQueue    Event queue num
 *  @param[out]  chanCfg    Stores channel configuration
 *  @retval   None
 *
 * \ingroup DPU_RangeProcTest__INTERNAL_FUNCTION
 */
void DPU_RangeProcReal2x_EDMAChannelThreeLinksConfigAssist(EDMA_Handle handle, uint32_t chNum, uint32_t shadowParam1, uint32_t shadowParam2, uint32_t shadowParam3,
                                                             uint32_t eventQueue, DPEDMA_3LinkChanCfg *chanCfg)
{

    DebugP_assert(chanCfg != NULL);

    DPEDMA_allocateEDMAChannel(handle, &chNum, &chNum, &chNum);

    chanCfg->channel = chNum;
    chanCfg->tcc = chNum;
    chanCfg->paramId = chNum;

    allocateEDMAShadowChannel(handle, &shadowParam1);
    allocateEDMAShadowChannel(handle, &shadowParam2);
    allocateEDMAShadowChannel(handle, &shadowParam3);

    chanCfg->ShadowPramId[0] = shadowParam1;
    chanCfg->ShadowPramId[1] = shadowParam2;
    chanCfg->ShadowPramId[2] = shadowParam3;

    chanCfg->eventQueue = eventQueue;

    return;
}

/**
*  @b Description
*  @n
*    Based on the test configuration, set up the range processing DPU configurations
*/
int32_t Test_setProfile()
{
    int32_t retVal = 0;
    DPIF_RadarCube radarCube;
    uint32_t hwaWindowOffset;
    uint32_t outputBytesPerBlock, inputBytesPerBlock, samplesPerBlock, numBlocksPerChirp;
    uint32_t radarCubeCompressedSizeInBytes;
    DPU_RangeProcHWA_EDMAInputConfig *edmaIn = &rangeProcDpuCfg.hwRes.edmaInCfg;
    DPU_RangeProcHWA_EDMAOutputConfig *edmaOut = &rangeProcDpuCfg.hwRes.edmaOutCfg;
    DPU_RangeProcHWA_EDMAPacketConfig *edmaPacket = &rangeProcDpuCfg.hwRes.edmaPacketCfg;
    DPU_RangeProcHWA_HwaConfig *hwaCfg = &rangeProcDpuCfg.hwRes.hwaCfg;
    DPU_RangeProcHWA_HW_Resources *hwRes = &rangeProcDpuCfg.hwRes;
    int32_t *windowBuffer;
    uint32_t winGenLen;
    uint32_t dataSizePerChirp, maxChirpsInL3;
    uint32_t totalChirps, iter;
    uint32_t maxPayloadSize, numBlocksPerPayload;

    hwaWindowOffset = 0;

    DPU_RangeProcTest_MemPoolReset(L3ramObj);
    DPU_RangeProcTest_MemPoolReset(CoreLocalRamObj);

    memset(&rangeProcDpuCfg, 0, sizeof(DPU_RangeProcHWA_Config));

    /* static configuration */
    rangeProcDpuCfg.staticCfg.appHeaderSize = 16U;
    rangeProcDpuCfg.staticCfg.appFooterSize = 8U;
    rangeProcDpuCfg.staticCfg.ADCBufData = staticCfg->ADCBufData;
    rangeProcDpuCfg.staticCfg.ADCBufData.data = staticCfg->ADCBufData.data;
    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty = staticCfg->ADCBufData.dataProperty;

    rangeProcDpuCfg.staticCfg.numChirpsPerFrame = staticCfg->numChirpsPerFrame;
    rangeProcDpuCfg.staticCfg.numRangeBins = staticCfg->numRangeBins;
    rangeProcDpuCfg.staticCfg.numFFTBins = staticCfg->numRangeFFTBins;
    rangeProcDpuCfg.staticCfg.numTxAntennas = staticCfg->numTxAntennas;
    rangeProcDpuCfg.staticCfg.numVirtualAntennas = staticCfg->numVirtualAntennas;
    rangeProcDpuCfg.staticCfg.isMode2x = staticCfg->isMode2x;
    if ((rangeProcDpuCfg.staticCfg.numRangeBins == rangeProcDpuCfg.staticCfg.numFFTBins / 2)||(rangeProcDpuCfg.staticCfg.isMode2x))
    {
        rangeProcDpuCfg.staticCfg.isChirpDataReal = 1;
        rangeProcDpuCfg.staticCfg.sizeOfInputSample = sizeof(int16_t);
    }
    else if (rangeProcDpuCfg.staticCfg.numRangeBins == rangeProcDpuCfg.staticCfg.numFFTBins)
    {
        rangeProcDpuCfg.staticCfg.isChirpDataReal = 0;
        rangeProcDpuCfg.staticCfg.sizeOfInputSample = sizeof(cmplx16ImRe_t);
    }
    else
    {
        retVal = DPU_RANGEPROCREAL2XTEST_RANGE_BINS_ERR;
        goto exit;
    }

    memcpy(&rangeProcDpuCfg.staticCfg.compressionCfg,
           &staticCfg->compressionCfg,
           sizeof(DPU_RangeProcHWA_CompressionCfg));
    memcpy(&rangeProcDpuCfg.staticCfg.intfStatsCfgdB,
           &staticCfg->intfStatsdBCfg,
           sizeof(DPU_RangeProcHWA_intfStatsdBCfg));

    /* static configuration - windows */
    /* Generating 1D window, allocate first */
    winGenLen = DPU_RangeProcTest_GetRangeWinGenLen(&rangeProcDpuCfg);
    rangeProcDpuCfg.staticCfg.windowSize = (rangeProcDpuCfg.staticCfg.isMode2x == 1) ? (winGenLen * sizeof(uint16_t)) : (winGenLen * sizeof(uint32_t));

    windowBuffer = (int32_t *)DPU_RangeProcTest_MemPoolAlloc(CoreLocalRamObj, rangeProcDpuCfg.staticCfg.windowSize, sizeof(uint32_t));
    if (windowBuffer == NULL)
    {
        retVal = DPU_RANGEPROCREAL2XTEST_ENOMEM__CORE_LOCAL_RAM_RANGE_HWA_WINDOW;
        goto exit;
    }
    rangeProcDpuCfg.staticCfg.window = windowBuffer;
    DPU_RangeProcTest_GenRangeWindow(&rangeProcDpuCfg);

    /* L3 allocations */
    /* L3 - radar cube */
    /* Input and output samples out of the rangeproc/compression DPU */
    if(staticCfg->compressionCfg.compressionMethod==HWA_COMPRESS_METHOD_BFP)
    {
        samplesPerBlock = staticCfg->compressionCfg.rangeBinsPerBlock;
    }
    else
    {
        samplesPerBlock = staticCfg->compressionCfg.numRxAntennaPerBlock * staticCfg->compressionCfg.rangeBinsPerBlock;
    }

    inputBytesPerBlock = 4*samplesPerBlock;
    /* 32-bit boundary aligned */
    outputBytesPerBlock = (MATHUTILS_CEILING_POS_FLOAT(inputBytesPerBlock * staticCfg->compressionCfg.compressionRatio / 4.0)) * 4;
    achievedCompressionRatio = (float)outputBytesPerBlock / (float)inputBytesPerBlock;

    radarCubeCompressedSizeInBytes = staticCfg->numRangeBins * staticCfg->numChirpsPerFrame *
                                       staticCfg->ADCBufData.dataProperty.numRxAntennas * sizeof(cmplx16ReIm_t) * achievedCompressionRatio;
    numBlocksPerChirp = (staticCfg->numRangeBins  * staticCfg->ADCBufData.dataProperty.numRxAntennas)/samplesPerBlock;

    maxPayloadSize = DPC_MAX_PAYLOAD_SIZE-(rangeProcDpuCfg.staticCfg.appHeaderSize + rangeProcDpuCfg.staticCfg.appFooterSize);
    numBlocksPerPayload = floor((float)maxPayloadSize/(float)outputBytesPerBlock);
    rangeProcDpuCfg.staticCfg.numPayloads = ceil((float)numBlocksPerChirp/numBlocksPerPayload);
    numBlocksPerPayload = numBlocksPerChirp/rangeProcDpuCfg.staticCfg.numPayloads;
    if((numBlocksPerPayload * outputBytesPerBlock)>maxPayloadSize)
    {
        /* Data Size per Payload exceeds the maximum payload size */
        retVal = DPU_RANGEPROCREAL2XTEST_PAYLOAD_SIZE;
        goto exit;
    }

    /*radarCube*/
    radarCube.dataSize = radarCubeCompressedSizeInBytes + \
                        ((rangeProcDpuCfg.staticCfg.appHeaderSize + rangeProcDpuCfg.staticCfg.appFooterSize)*staticCfg->numChirpsPerFrame* \
                        rangeProcDpuCfg.staticCfg.numPayloads);

    uint32_t l3Size = (testStaticCfg.isMode2x == 1) ? L3_SIZE : 0xA000U;
    /* Calculate L3 Re-Use Iterations and Chirps per Iterations */
    numL3ReUseIterations = ceil((float)radarCube.dataSize/ (float)l3Size);
    dataSizePerChirp = radarCube.dataSize/staticCfg->numChirpsPerFrame;
    // maxChirps that can allocated in L3; should be a multiple of 2
    maxChirpsInL3 = floor(floor(l3Size/dataSizePerChirp)/2U)*2U;

    /* In 2X mode test, data is tested from L3, so L3 reuse is not supported */
    uint32_t maxL3Reuse = (rangeProcDpuCfg.staticCfg.isMode2x==1)?1U:RANGEPROCHWA_L3REUSE_MAX_ITERATIONS;

    if(numL3ReUseIterations > maxL3Reuse)
    {
        /* Limit on Max iterations of L3 because of EDMA Channel linking */
        retVal = DPU_RANGEPROCREAL2XTEST_L3_REUSE_ITERATIONS;
        goto exit;
    }
    else
    {
        iter = 0U;
        totalChirps = staticCfg->numChirpsPerFrame;
        while(totalChirps>0U)
        {
            rangeProcDpuCfg.staticCfg.numChirpsEachIter[iter] = (totalChirps > maxChirpsInL3)?maxChirpsInL3:totalChirps;
            totalChirps-=rangeProcDpuCfg.staticCfg.numChirpsEachIter[iter++];
        }
    }

    rangeProcDpuCfg.hwRes.radarCubeBufSizeL3 = (radarCube.dataSize/staticCfg->numChirpsPerFrame)*rangeProcDpuCfg.staticCfg.numChirpsEachIter[0];

    radarCube.data = DPU_RangeProcTest_MemPoolAlloc(L3ramObj,rangeProcDpuCfg.hwRes.radarCubeBufSizeL3,
                                                    DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT);
    if (radarCube.data == NULL)
    {
        retVal = DPU_RANGEPROCREAL2XTEST_ENOMEM__L3_RAM_RADAR_CUBE;
        goto exit;
    }
    radarCube.datafmt = DPIF_RADARCUBE_FORMAT_2;
    rangeProcDpuCfg.hwRes.radarCube = radarCube;

    /* hwres - edma */
    hwRes->edmaHandle = gEdmaHandle[CONFIG_EDMA0];

    /* EDMA to copy the Payload Header from bookkeep registers to L3. */
    DPU_RangeProcReal2x_EDMAChannelThreeLinksConfigAssist(hwRes->edmaHandle,
                                              MMW_RES_DPU_RANGE_EDMAHEADER_CH,
                                              MMW_RES_DPU_RANGE_EDMAHEADER_SHADOW1,
                                              MMW_RES_DPU_RANGE_EDMAHEADER_SHADOW2,
                                              MMW_RES_DPU_RANGE_EDMAHEADER_SHADOW3,
                                              MMW_RES_DPU_RANGE_EDMAHEADER_EVENT_QUE,
                                              &edmaPacket->appHeader);

    /* We have choosen ISOLATE mode, so we have to fill in dataIn */
    DPU_RangeProcReal2x_EDMAChannelConfigAssist(hwRes->edmaHandle,
                                              MMW_RES_DPU_RANGE_EDMAIN_CH,
                                              MMW_RES_DPU_RANGE_EDMAIN_SHADOW,
                                              MMW_RES_DPU_RANGE_EDMAIN_EVENT_QUE,
                                              &edmaIn->dataIn);

    DPU_RangeProcReal2x_EDMAChannelConfigAssist(hwRes->edmaHandle,
                                              MMW_RES_DPU_RANGE_EDMAIN_SIG_CH,
                                              MMW_RES_DPU_RANGE_EDMAIN_SIG_SHADOW,
                                              MMW_RES_DPU_RANGE_EDMAIN_SIG_EVENT_QUE,
                                              &edmaIn->dataInSignature);

    DPU_RangeProcReal2x_EDMAChannelConfigAssist(hwRes->edmaHandle,
                                              MMW_RES_DPU_RANGE_EDMAOUT_SIG_CH,
                                              MMW_RES_DPU_RANGE_EDMAOUT_SIG_SHADOW,
                                              MMW_RES_DPU_RANGE_EDMAOUT_SIG_EVENT_QUE,
                                              &edmaOut->dataOutSignature);

    /* Ping */
    DPU_RangeProcReal2x_EDMAChannelThreeLinksConfigAssist(hwRes->edmaHandle,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PING_CH,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PING_SHADOW1,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PING_SHADOW2,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PING_SHADOW3,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PING_EVENT_QUE,
                                              &edmaOut->dataOutPing);

    /* Pong */
    DPU_RangeProcReal2x_EDMAChannelThreeLinksConfigAssist(hwRes->edmaHandle,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PONG_CH,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PONG_SHADOW1,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PONG_SHADOW2,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PONG_SHADOW3,
                                              MMW_RES_DPU_RANGE_EDMAOUT_PONG_EVENT_QUE,
                                              &edmaOut->dataOutPong);

    /* EDMA to copy the Payload Footer from RTI Timestamp registers to L3. */
    DPU_RangeProcReal2x_EDMAChannelThreeLinksConfigAssist(hwRes->edmaHandle,
                                              MMW_RES_DPU_RANGE_EDMAFOOTER_CH,
                                              MMW_RES_DPU_RANGE_EDMAFOOTER_SHADOW1,
                                              MMW_RES_DPU_RANGE_EDMAFOOTER_SHADOW2,
                                              MMW_RES_DPU_RANGE_EDMAFOOTER_SHADOW3,
                                              MMW_RES_DPU_RANGE_EDMAFOOTER_EVENT_QUE,
                                              &edmaPacket->appFooter);

    /* In this case HWA hardware trigger source is equal to HWA param index value*/
    hwaCfg->dataInputMode = DPU_RangeProcHWA_InputMode_ISOLATED;

#ifdef DPC_USE_SYMMETRIC_WINDOW_RANGE_DPU
    hwaCfg->hwaWinSym = HWA_FFT_WINDOW_SYMMETRIC;
#else
    hwaCfg->hwaWinSym = HWA_FFT_WINDOW_NONSYMMETRIC;
#endif
    hwaCfg->hwaWinRamOffset = (uint16_t)hwaWindowOffset;
    if ((hwaCfg->hwaWinRamOffset + winGenLen) > DPC_COMPFFT_HWA_MAX_WINDOW_RAM_SIZE_IN_SAMPLES)
    {
        retVal = DPU_RANGEPROCREAL2XTEST_ENOMEM_HWA_WINDOW_RAM;
        goto exit;
    }

    hwaCfg->numParamSet = MMW_RES_DPU_RANGE_NUM_HWA_PARAMSETS;
    hwaCfg->paramSetStartIdx = MMW_RES_DPU_RANGE_PARAMSET_START_IDX;

exit:

    return retVal;
}


void rangeProcDpuTest_dpuInit()
{
    int32_t errorCode = 0;
    DPU_RangeProcHWA_InitParams initParams;
    initParams.hwaHandle = gHwaHandle[0];

    DPU_RangeProcTest_MemCfg L3ramCfg, CoreLocalRamCfg;
    L3ramCfg.addr = (void *)&gL3Ram[0];
    L3ramCfg.size = sizeof(gL3Ram);
    CoreLocalRamCfg.addr = &gL2Heap[0];
    CoreLocalRamCfg.size = sizeof(gL2Heap);

    L3ramObj->cfg = L3ramCfg;
    CoreLocalRamObj->cfg = CoreLocalRamCfg;

    DPU_RangeProcTest_MemPoolReset(L3ramObj);
    DPU_RangeProcTest_MemPoolReset(CoreLocalRamObj);

    /* generate the dpu handler*/
    rangeProcDpuHandle =  DPU_RangeProcHWA_init (&initParams, subframeCount, &errorCode);
    if (rangeProcDpuHandle == NULL)
    {
        test_print("Debug: RangeProc DPU initialization returned error %d\n", errorCode);
        DebugP_assert(0);
        return;
    }
}

bool dpuRangeProcHwa_compareMemBankResults(cmplxUint16ImRe_t *obtained, uint32_t chirpIdx)
{
    uint32_t i, payloadIdx=0;
    int32_t diffReal, diffImag;
    bool result = 1;
    uint32_t numBytesToReadPerPaylaod = ((rangeProcDpuCfg.hwRes.radarCube.dataSize/testStaticCfg.numChirps)/rangeProcDpuCfg.staticCfg.numPayloads) - (rangeProcDpuCfg.staticCfg.appFooterSize+rangeProcDpuCfg.staticCfg.appHeaderSize);

    for(payloadIdx=0; payloadIdx<rangeProcDpuCfg.staticCfg.numPayloads; payloadIdx++)
    {

        fread((void *)(gRadarCubeIdeal), sizeof(uint16_t), numBytesToReadPerPaylaod/2, testResultFileId);
        for (i = 0; i < numBytesToReadPerPaylaod/sizeof(cmplx16ImRe_t); i++)
        {
            diffReal = ((gRadarCubeIdeal[i].real - obtained[i].real) > 0) ? (gRadarCubeIdeal[i].real - obtained[i].real) : -(gRadarCubeIdeal[i].real - obtained[i].real);
            diffImag = ((gRadarCubeIdeal[i].imag - obtained[i].imag) > 0) ? (gRadarCubeIdeal[i].imag - obtained[i].imag) : -(gRadarCubeIdeal[i].imag - obtained[i].imag);
            if (diffReal > MAX_ALLOWED_ERROR || diffImag > MAX_ALLOWED_ERROR)
            {
                if (diffReal > 2*MAX_ALLOWED_ERROR || diffImag > 2*MAX_ALLOWED_ERROR)
                {
                    test_print("Mismatch at idx %d of payload%d-chirp%d: Ideal %d + i(%d), Obtained %d + i(%d)\n", i, payloadIdx, chirpIdx, gRadarCubeIdeal[i].real, gRadarCubeIdeal[i].imag, obtained[i].real, obtained[i].imag);
                    result = 0;
                }
                else{
                    test_print("High error at idx %d of payload%d-chirp%d: Ideal %d + i(%d), Obtained %d + i(%d)\n", i, payloadIdx, chirpIdx, gRadarCubeIdeal[i].real, gRadarCubeIdeal[i].imag, obtained[i].real, obtained[i].imag);
                }
            }
        }

        /* offset by header and footer after every paylaod*/
        obtained = &obtained[i]+((rangeProcDpuCfg.staticCfg.appFooterSize+rangeProcDpuCfg.staticCfg.appHeaderSize)/sizeof(uint32_t));
    }
    return result;
}


bool dpuRangeProcHwa_compareL3RAMResults()
{
    uint32_t i, payloadIdx=0;
    int32_t diffReal, diffImag;
    bool result = 1;
    uint32_t numBytesToReadPerPaylaod = ((rangeProcDpuCfg.hwRes.radarCube.dataSize/testStaticCfg.numChirps)/rangeProcDpuCfg.staticCfg.numPayloads) - (rangeProcDpuCfg.staticCfg.appFooterSize+rangeProcDpuCfg.staticCfg.appHeaderSize);
    uint32_t chirpIdx;
    cmplx16ImRe_t *gRadarCubeIdeal;
    uint32_t offset=0;

    cmplx16ImRe_t *obtained = (cmplx16ImRe_t*)rangeProcDpuCfg.hwRes.radarCube.data + rangeProcDpuCfg.staticCfg.appHeaderSize/sizeof(cmplx16ImRe_t);

    for(chirpIdx = 0U; chirpIdx< testStaticCfg.numChirps; chirpIdx++)
    {
        offset = chirpIdx*testStaticCfg.numRangeBins*4U*4U*achievedCompressionRatio;
        gRadarCubeIdeal = (cmplx16ImRe_t *)(&gRadarCubeIdealBuf[offset]);

        for(payloadIdx=0; payloadIdx<rangeProcDpuCfg.staticCfg.numPayloads; payloadIdx++)
        {

            for (i = 0; i < numBytesToReadPerPaylaod/sizeof(cmplx16ImRe_t); i++)
            {
                diffReal = ((gRadarCubeIdeal[i].real - obtained[i].real) > 0) ? (gRadarCubeIdeal[i].real - obtained[i].real) : -(gRadarCubeIdeal[i].real - obtained[i].real);
                diffImag = ((gRadarCubeIdeal[i].imag - obtained[i].imag) > 0) ? (gRadarCubeIdeal[i].imag - obtained[i].imag) : -(gRadarCubeIdeal[i].imag - obtained[i].imag);
                if (diffReal > MAX_ALLOWED_ERROR || diffImag > MAX_ALLOWED_ERROR)
                {
                    if (diffReal > 2*MAX_ALLOWED_ERROR || diffImag > 2*MAX_ALLOWED_ERROR)
                    {
                        test_print("Mismatch at idx %d of payload%d-chirp%d: Ideal %d + i(%d), Obtained %d + i(%d)\n", i, payloadIdx, chirpIdx, gRadarCubeIdeal[i].real, gRadarCubeIdeal[i].imag, obtained[i].real, obtained[i].imag);
                        result = 0;
                    }
                    else{
                        test_print("High error at idx %d of payload%d-chirp%d: Ideal %d + i(%d), Obtained %d + i(%d)\n", i, payloadIdx, chirpIdx, gRadarCubeIdeal[i].real, gRadarCubeIdeal[i].imag, obtained[i].real, obtained[i].imag);
                    }
                }
            }

            /* offset by header and footer after every paylaod*/
            obtained = &obtained[i]+((rangeProcDpuCfg.staticCfg.appFooterSize+rangeProcDpuCfg.staticCfg.appHeaderSize)/sizeof(uint32_t));
        }
    }
    return result;
}

bool run1xModeTest()
{
    uint32_t i, j, chirpIdx=0;
    cmplxUint16ImRe_t *obtainedData;
    int32_t retVal;
    bool testResult =1;
    DPU_RangeProcHWA_OutParams outParms;


    /* control the rangeproc hwa*/
    retVal = DPU_RangeProcHWA_control(rangeProcDpuHandle, DPU_RangeProcHWA_Cmd_triggerProc, NULL, 0);
    if (retVal < 0)
    {
        /* Not Expected */
        test_print("RangeProc DPU control error %d\n", retVal);
        DebugP_assert(0);
    }

    /* process chirps loop in the frame*/
    for(uint32_t iter=0; iter<numL3ReUseIterations; iter++)
    {
        obtainedData = (cmplxUint16ImRe_t*)rangeProcDpuCfg.hwRes.radarCube.data + rangeProcDpuCfg.staticCfg.appHeaderSize/sizeof(cmplxUint16ImRe_t);

        for (i = 0; i < rangeProcDpuCfg.staticCfg.numChirpsEachIter[iter]; i++)
        {
            /* read in one chirp data */
            for (j = 0; j <  testStaticCfg.ADCBufData.dataProperty.numRxAntennas; j++)
            {
                /* 16 byte aligned, 8 uint16 symbols */
                int32_t alignedAddress8Samples = j * (((testStaticCfg.ADCBufData.dataProperty.numAdcSamples + 7) / 8) * 8);
                int32_t alignedAddress32Bits = alignedAddress8Samples/2;
                fread((uint16_t *)&adcDataIn[alignedAddress32Bits],  sizeof(uint16_t), testStaticCfg.ADCBufData.dataProperty.numAdcSamples, testDataFileId);
            }

            CacheP_wb(adcDataIn, (2 * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * ((testStaticCfg.ADCBufData.dataProperty.numAdcSamples + 3) / 4 * 4)), CacheP_TYPE_ALLD);


            if(chirpIdx%10 == 0){
                test_print("Reading, Processing and Comparing... %d percent\n", chirpIdx * 100 / testStaticCfg.numChirps);
            }

            EDMA_enableTransferRegion(baseAddr, regionId, MMW_RES_DPU_RANGE_EDMAIN_CH, EDMA_TRIG_MODE_MANUAL);

            EDMA_enableTransferRegion(baseAddr, regionId, MMW_RES_DPU_RANGE_EDMAHEADER_CH, EDMA_TRIG_MODE_MANUAL);

            while (EDMA_readIntrStatusRegion(baseAddr, regionId, MMW_RES_DPU_RANGE_EDMAOUT_SIG_CH) != 1);

            EDMA_clrIntrRegion(baseAddr, regionId, MMW_RES_DPU_RANGE_EDMAOUT_SIG_CH);

            // ClockP_usleep(200);

            testResult &= dpuRangeProcHwa_compareMemBankResults(obtainedData, chirpIdx++);

            /* update the obtained radar cube address by the header-footer offset*/
            obtainedData += (rangeProcDpuCfg.hwRes.radarCube.dataSize/testStaticCfg.numChirps)/sizeof(cmplxUint16ImRe_t);

        } /* end of chirp loop */
    } /* end of L3 ReUse Iterations Loop */

    resultReadPtr = ftell(testResultFileId);
    fclose(testResultFileId);

    memset((void *)&outParms, 0, sizeof(DPU_RangeProcHWA_OutParams));

    retVal = DPU_RangeProcHWA_process(rangeProcDpuHandle, &outParms);

    test_print("Processing and Comparison Done!\n");

    if(testResult){
        test_print("Test Passed!\n");
    }
    else{
        test_print("Test Failed!\n");
    }

    return testResult;
}

bool run2xModeTest()
{
    int32_t retVal;
    bool testResult =1;
    DPU_RangeProcHWA_OutParams outParms;

    fread((uint16_t *)&adcDataIn[0],  sizeof(uint16_t), testStaticCfg.ADCBufData.dataProperty.numAdcSamples * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * testStaticCfg.numChirpsPerFrame, testDataFileId);

    fread((void *)(gRadarCubeIdeal), sizeof(uint32_t), testStaticCfg.numRangeBins * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * testStaticCfg.numChirpsPerFrame * achievedCompressionRatio, testResultFileId);
    resultReadPtr = ftell(testResultFileId);
    fclose(testResultFileId);

    /* control the rangeproc hwa*/
    retVal = DPU_RangeProcHWA_control(rangeProcDpuHandle, DPU_RangeProcHWA_Cmd_triggerProc, NULL, 0);
    if (retVal < 0)
    {
        /* Not Expected */
        test_print("RangeProc DPU control error %d\n", retVal);
        DebugP_assert(0);
    }

    for (uint32_t chirpIdx=0; chirpIdx < rangeProcDpuCfg.staticCfg.numChirpsPerFrame; chirpIdx++)
    {
        /* Update the address for Ping chirp */
        if(chirpIdx%2==0)
        {
            retVal = DPEDMA_updateAddressAndTrigger(gEdmaHandle[0],
                    (chirpIdx*testStaticCfg.ADCBufData.dataProperty.numAdcSamples * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * 2U) + (uint32_t)(&adcDataIn[0]), /* src addr */
                    0,                                                 /* don't update dest addr */
                    rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.channel, /* param Id */
                    false);
            if (retVal != 0) { DebugP_assert(0);}
        }

        EDMA_enableTransferRegion(baseAddr, regionId, MMW_RES_DPU_RANGE_EDMAIN_CH, EDMA_TRIG_MODE_MANUAL);

        while (EDMA_readIntrStatusRegion(baseAddr, regionId, MMW_RES_DPU_RANGE_EDMAOUT_SIG_CH) != 1);

        EDMA_clrIntrRegion(baseAddr, regionId, MMW_RES_DPU_RANGE_EDMAOUT_SIG_CH);

    } /* end of chirp loop */

    memset((void *)&outParms, 0, sizeof(DPU_RangeProcHWA_OutParams));

    retVal = DPU_RangeProcHWA_process(rangeProcDpuHandle, &outParms);

    testResult &= dpuRangeProcHwa_compareL3RAMResults();

    test_print("Processing and Comparison Done!\n");

    if(testResult){
        test_print("Test Passed!\n");
    }
    else{
        test_print("Test Failed!\n");
    }

    return testResult;
}

/**
*  @b Description
*  @n
*        Testing Task
*/
void rangeProcDpuTest_Task(void *args)
{
    uint32_t i=0;
    uint32_t numTests, testIdx;
    uint32_t numBytesPerInputSample;
    int32_t retVal = 0;
    uint8_t readDataAgain;
    char testResultfile[100];
    char testVectorsfile[100];
    char testDatafile[100];
    uint64_t taskStartTime, taskEndTime;

    Drivers_open();
    Board_driversOpen();

    baseAddr = EDMA_getBaseAddr(gEdmaHandle[CONFIG_EDMA0]);
    DebugP_assert(baseAddr != 0);

    regionId = EDMA_getRegionId(gEdmaHandle[CONFIG_EDMA0]);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* Initialize test logger */
    MCPI_Initialize();

    bool finalResults = 1, testResult=1;
    taskStartTime= ClockP_getTimeUsec();
    test_print("...... Initialization ...... \n");

    /* hwa, edma, and rangeprocDPU initialization*/
    rangeProcDpuTest_dpuInit();

    /* open the test result, test data and test vetors file */
    strcpy(testResultfile, "testdata\\rangeprocReal2x_testResult_awr2544.bin");

    strcpy(testVectorsfile, "testdata\\rangeprocReal2x_testVectors_awr2544.bin");
    testVectorsFileId = fopen(testVectorsfile, "rb");
    if (testVectorsFileId == NULL)
    {
        test_print("Error:  Cannot open %s !\n", testVectorsfile);
        exit(0);
    }

    strcpy(testDatafile, "testdata\\rangeprocReal2x_testAdcDataIn_awr2544.bin");
    testDataFileId = fopen(testDatafile, "rb");
    if (testDataFileId == NULL)
    {
        test_print("Error:  Cannot open %s !\n", testDatafile);
        exit(0);
    }

    fread(&numTests, sizeof(uint32_t), 1, testVectorsFileId);
    vectorReadPtr = ftell(testVectorsFileId);
    fclose(testVectorsFileId);

    /* process each test */
    for (testIdx = 0; testIdx < numTests; testIdx++)
    {
        testVectorsFileId = fopen(testVectorsfile, "rb");
        if (testVectorsFileId == NULL)
        {
            test_print("Error:  Cannot open %s !\n", testVectorsfile);
            exit(0);
        }
        if(fseek(testVectorsFileId, vectorReadPtr, SEEK_SET)!=0)  DebugP_assert(0);

        /* read in test config */
        fread(&testStaticCfg.numTxAntennas, sizeof(uint32_t), 1, testVectorsFileId);
        fread(&testStaticCfg.ADCBufData.dataProperty.numRxAntennas, sizeof(uint32_t), 1, testVectorsFileId);
        fread(&testStaticCfg.ADCBufData.dataProperty.numAdcSamples, sizeof(uint32_t), 1, testVectorsFileId);
        fread(&testStaticCfg.numChirpsPerFrame, sizeof(uint32_t), 1, testVectorsFileId);
        testStaticCfg.ADCBufData.dataProperty.numChirpsPerChirpEvent = 1;

        fread(&testStaticCfg.compressionCfg.isEnabled, sizeof(uint8_t), 1, testVectorsFileId);
        fread(&testStaticCfg.compressionCfg.compressionMethod, sizeof(uint16_t), 1, testVectorsFileId);
        fread(&testStaticCfg.compressionCfg.compressionRatio, sizeof(float), 1, testVectorsFileId);
        fread(&testStaticCfg.compressionCfg.rangeBinsPerBlock, sizeof(uint16_t), 1, testVectorsFileId);
        fread(&testStaticCfg.compressionCfg.numRxAntennaPerBlock, sizeof(uint16_t), 1, testVectorsFileId);

        fread(&testStaticCfg.intfStatsdBCfg.intfMitgMagSNRdB, sizeof(uint32_t), 1, testVectorsFileId);
        fread(&testStaticCfg.intfStatsdBCfg.intfMitgMagDiffSNRdB, sizeof(uint32_t), 1, testVectorsFileId);

        fread(&testStaticCfg.isChirpDataReal, sizeof(uint16_t), 1, testVectorsFileId);
        testStaticCfg.numRangeFFTBins = mathUtils_getValidFFTSize(testStaticCfg.ADCBufData.dataProperty.numAdcSamples);
        if (testStaticCfg.isChirpDataReal)
        {
            numBytesPerInputSample = sizeof(int16_t);
            testStaticCfg.numRangeBins = testStaticCfg.numRangeFFTBins / 2;
            testStaticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_REAL16;
        }
        else
        {
            numBytesPerInputSample = sizeof(uint32_t); /* Complex (16 bit imag, 16 bit real) */
            testStaticCfg.numRangeBins = testStaticCfg.numRangeFFTBins;
            testStaticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_COMPLEX16_IMRE;
        }
        uint32_t adcBufChanDataSize = (numBytesPerInputSample * testStaticCfg.ADCBufData.dataProperty.numAdcSamples + 15U) / 16U * 16U;
        testStaticCfg.ADCBufData.data = (void *)&adcDataIn[0];
        for (i = 0; i < SYS_COMMON_NUM_RX_CHANNEL; i++)
        {
            testStaticCfg.ADCBufData.dataProperty.rxChanOffset[i] = i * adcBufChanDataSize;
        }
        testStaticCfg.ADCBufData.dataProperty.adcBits = 2;
        testStaticCfg.ADCBufData.dataProperty.interleave = DPIF_RXCHAN_NON_INTERLEAVE_MODE;
        fread(&testStaticCfg.isMode2x, sizeof(uint16_t), 1, testVectorsFileId);

        /* 2X mode should only be selected with real data */
        if((testStaticCfg.isMode2x==1) && (testStaticCfg.isChirpDataReal==0))
        {
            retVal = DPU_RANGEPROCREAL2XTEST_2XMODE_ERR;
        }

        if(testStaticCfg.isMode2x)
        {
            /* In 2X mode, FFT size is configured half of actual */
            testStaticCfg.numRangeFFTBins = testStaticCfg.numRangeFFTBins / 2;

            /* Max TEST_CHIRPS_2X can be tested */
            testStaticCfg.numChirpsPerFrame = (testStaticCfg.numChirpsPerFrame > TEST_CHIRPS_2X)? TEST_CHIRPS_2X: testStaticCfg.numChirpsPerFrame;
        }
        testStaticCfg.numChirps = testStaticCfg.numChirpsPerFrame;
        fread(&readDataAgain, sizeof(uint8_t), 1, testVectorsFileId);

        vectorReadPtr = ftell(testVectorsFileId);
        fclose(testVectorsFileId);

        test_print("##################################\n");
        test_print("\nTest #%d  start\r\n", testIdx);
        test_print("------------\n");
        test_print("NumRx %d, NumTx %d\n", testStaticCfg.ADCBufData.dataProperty.numRxAntennas, testStaticCfg.numTxAntennas);
        test_print("NumADCSamples %d NumChirps %d AdcDataReal %d 2XMode %d\n", testStaticCfg.ADCBufData.dataProperty.numAdcSamples, testStaticCfg.numChirpsPerFrame, testStaticCfg.isChirpDataReal, testStaticCfg.isMode2x);
        test_print("Compression: Method %d Ratio %f RangeBinsPerBlock %d\n", testStaticCfg.compressionCfg.compressionMethod, testStaticCfg.compressionCfg.compressionRatio, testStaticCfg.compressionCfg.rangeBinsPerBlock);
        test_print("Interference: MagSNRdB %d MagDiffSNRdB %d\n", testStaticCfg.intfStatsdBCfg.intfMitgMagSNRdB, testStaticCfg.intfStatsdBCfg.intfMitgMagDiffSNRdB);
        test_print("------------\n");

        retVal = Test_setProfile();
        if (retVal < 0)
        {
            test_print("DEBUG: Error in setting up profile:%d \n", retVal);
            DebugP_assert(0);
        }

        retVal = DPU_RangeProcHWA_config(rangeProcDpuHandle, &rangeProcDpuCfg);
        if (retVal < 0)
        {
            test_print("DEBUG: RANGE DPU config return error:%d \n", retVal);
            DebugP_assert(0);
        }

        if(testStaticCfg.isMode2x)
        {
            /* overwrite the srcBIdx of EDMA_in channel */
            *((volatile uint32_t*)(CSL_DSS_TPCC_A_U_BASE + 0x4000U+MMW_RES_DPU_RANGE_EDMAIN_CH*32U+0x10)) |= testStaticCfg.ADCBufData.dataProperty.numAdcSamples * testStaticCfg.ADCBufData.dataProperty.numRxAntennas *2U;

            /* overwrite the srcBIdx of shadow channel */
            *((volatile uint32_t*)(CSL_DSS_TPCC_A_U_BASE + 0x4000U+MMW_RES_DPU_RANGE_EDMAIN_SHADOW*32U+0x10)) |= testStaticCfg.ADCBufData.dataProperty.numAdcSamples * testStaticCfg.ADCBufData.dataProperty.numRxAntennas *2U;
        }

        if(readDataAgain){
            /* update the file read ptr to read from next data location */
            dataReadPtr = ftell(testDataFileId);
        }
        else{
            /* set the file read ptr back to start data location */
            if(fseek(testDataFileId, dataReadPtr, SEEK_SET)!=0U)  DebugP_assert(0);
        }

        testResultFileId = fopen(testResultfile, "rb");
        if (testResultFileId == NULL)
        {
            test_print("Error:  Cannot open %s !\n", testResultfile);
            exit(0);
        }
        if(fseek(testResultFileId, resultReadPtr, SEEK_SET)!=0)  DebugP_assert(0);

        if(testStaticCfg.isMode2x)
        {
            run2xModeTest();
        }
        else
        {
            run1xModeTest();
        }

        finalResults&=testResult;

        /* Free EDMA Channels. */
        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.channel, &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.tcc,
                               &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.paramId, &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.shadowPramId);

        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.channel, &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.tcc,
                               &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.paramId, &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.shadowPramId);

        test_print("\n");

        test_print("%d Tests finished!\n\r", testIdx);
    } /* end of test loop */

    taskEndTime=ClockP_getTimeUsec();
    test_print("Test Time Cycles: %lld\n",taskEndTime-taskStartTime);
    test_print("All Tests finished!\n\r");

    /*delete the handler*/
    DPU_RangeProcHWA_deinit(rangeProcDpuHandle);
    MCPI_setTestResult();

    Board_driversClose();
    Drivers_close();
    fclose(testDataFileId);
    fclose(testResultFileId);

    test_print("--- Test Completed ---\n");
    if (finalResults == 0)
    {
        test_print ("Some/All tests have failed!!\n");
    }
    else
    {
        test_print ("All tests have passed!!\n");
    }

    vTaskDelete(NULL);
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

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic(rangeProcDpuTest_Task, /* Pointer to the function that implements the task. */
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
}
