/**
 *   @file  hwa_main.c
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
#include <ti/datapath/dpu/rangeprocDDMA/test/dssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/dssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/dssgenerated/ti_board_open_close.h>
#endif
#ifdef SUBSYS_MSS
#include <ti/datapath/dpu/rangeprocDDMA/test/mssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/mssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/mssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/mssgenerated/ti_board_open_close.h>
#endif
#ifdef SUBSYS_M4
#include <ti/datapath/dpu/rangeprocDDMA/test/m4generated/ti_drivers_config.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/m4generated/ti_board_config.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/m4generated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/rangeprocDDMA/test/m4generated/ti_board_open_close.h>
#endif

#include <kernel/dpl/DebugP.h>
#ifndef SUBSYS_M4
#include "FreeRTOS.h"
#include "task.h"
#endif
/* mmwave SDK files */
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpu/rangeprocDDMA/rangeprochwaDDMA.h>
#include <ti/datapath/dpu/rangeproc/include/rangeprochwa_internal.h>
#include <ti/utils/mathutils/mathutils.h>

#if defined(SOC_AWR2X44P)
#include <ti/demo/awr2x44P/mmw_ddm/mmw_resDDM.h>
#elif defined(SOC_AWR294X)
#include <ti/demo/awr294x/mmw/mmw_resDDM.h>
#endif

#define MAX_NUM_RANGEBIN 1024
#define MAX_NUM_RX_ANTENNA 4
#define MAX_NUM_ADCSAMPLE_PERCHIRP (MAX_NUM_RANGEBIN * MAX_NUM_RX_ANTENNA)
#define HWA_MEMBANK_START_ADDRESS 0x82000000
#define HWA_MEMBANK_SIZE 0x4000
#define HWA_MEMBANK_DEST_ADDRESS 0x82000000 + 4 * HWA_MEMBANK_SIZE
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
#define MAX_NUM_TX_ANTENNA 4
#else
#define MAX_NUM_TX_ANTENNA 3
#endif

#define MAX_NUM_CHIRPS_PERFRAME 6 //matlab generate input: tx=3, number of chirps = 6, otherwise = 4,
#define NUM_REPEAT_PERFRAME 2     //for testing, repeat same data per frame
#define MAX_ALLOWED_ERROR 32
#define MMWDEMO_OBJDET_L2RAM_SIZE (87U * 1024U)

/* Interrupt object for rangeProc */
Edma_IntrObject rangProcIntrObj[RANGEPROCHWADDMA_NUM_EDMA_INTERRUPTS];

#if defined(SUBSYS_MSS) || defined(SUBSYS_M4)
/*! L3 RAM buffer for object detection DPC */
uint8_t gL3Ram[(0x80000)] __attribute__((aligned(4096), section(".l3ram")));

/*! L2 RAM buffer for object detection DPC */
uint8_t gL2Heap[MMWDEMO_OBJDET_L2RAM_SIZE] __attribute__((aligned(4096), section(".dpc_l2Heap")));
#endif
#ifdef SUBSYS_DSS
/*! L3 RAM buffer for object detection DPC */
uint8_t gL3Ram[(0x80000)] __attribute__((aligned(4096)));
#pragma DATA_ALIGN(gL3Ram, 4096U);
#pragma DATA_SECTION(gL3Ram, ".l3ram");

/*! L2 RAM buffer for object detection DPC */
uint8_t gL2Heap[MMWDEMO_OBJDET_L2RAM_SIZE];
#pragma DATA_ALIGN(gL2Heap, 4096U);
#pragma DATA_SECTION(gL2Heap, ".dpc_l2Heap");
#endif

#if defined(SUBSYS_MSS) || defined(SUBSYS_M4)
/* data memeory */
uint32_t dataInBuffer[384 * 96 * 4] __attribute__((section(".dataBuf")));
uint32_t adcDataIn[MAX_NUM_ADCSAMPLE_PERCHIRP]
    __attribute__((aligned(CSL_CACHE_L1D_LINESIZE), section(".dataBufL2"))); /*adc buffer for one chirp*/
#endif
#ifdef SUBSYS_DSS
/* data memeory */
#pragma DATA_SECTION(dataInBuffer, ".dataBuf");
uint32_t dataInBuffer[384 * 96 * 4]; /*to save all adc buffer data */
#pragma DATA_SECTION(adcDataIn, ".dataBufL2");
uint32_t adcDataIn[MAX_NUM_ADCSAMPLE_PERCHIRP]
    __attribute__((aligned(CSL_CACHE_L1D_LINESIZE))); /*adc buffer for one chirp*/
#endif

#ifdef SUBSYS_M4
uint8_t decompRCubeBlock[32768] __attribute__((section(".dataBuf"))); /*adc buffer for one chirp*/
uint8_t * decompRadarCubeAddress = &decompRCubeBlock[0];
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(decompRCubeBlock, ".dataBuf");
uint8_t decompRCubeBlock[32768]
    __attribute__((aligned(CSL_CACHE_L1D_LINESIZE))); /*adc buffer for one chirp*/
uint8_t * decompRadarCubeAddress = &decompRCubeBlock[0];
#endif

#if defined(SUBSYS_MSS) || defined(SUBSYS_M4)
/* hwa 18 bits, whole length */
uint32_t window1DCoef[MAX_NUM_RANGEBIN] __attribute__((aligned(8), section(".dataBufL2")));
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(window1DCoef, ".dataBufL2");
#pragma DATA_ALIGN(window1DCoef, 8);
uint32_t window1DCoef[MAX_NUM_RANGEBIN]; //hwa 18 bits, whole length
#endif

/************** radar cube *******************************/
#ifdef SUBSYS_MSS
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT \
    DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F
#else
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT \
    DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP
#endif

#if defined(SUBSYS_MSS) || defined(SUBSYS_M4)
uint8_t gRadarCubeBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_ADCSAMPLE_PERCHIRP *
                                               MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME *
                                               sizeof(cmplx16ImRe_t),
                                           DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)]
    __attribute__((aligned(DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT), section(".l3data")));
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(gRadarCubeBuf, ".l3data");
uint8_t gRadarCubeBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_ADCSAMPLE_PERCHIRP *
                                               MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME *
                                               sizeof(cmplx16ImRe_t),
                                           DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)]
    __attribute__((aligned(DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)));
#endif

cmplx16ImRe_t *gRadarCube = (cmplx16ImRe_t *)&gRadarCubeBuf[0];

#if defined(SUBSYS_MSS) || defined(SUBSYS_M4)
uint8_t gRadarCubeIdealBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_ADCSAMPLE_PERCHIRP *
                                                    MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME *
                                                    sizeof(cmplx16ImRe_t),
                                                DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)]
    __attribute__((aligned(DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT), section(".l3data")));
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(gRadarCubeIdealBuf, ".l3data");
uint8_t gRadarCubeIdealBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_ADCSAMPLE_PERCHIRP *
                                                    MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME *
                                                    sizeof(cmplx16ImRe_t),
                                                DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)]
    __attribute__((aligned(DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)));
#endif

cmplx16ImRe_t *gRadarCubeIdeal = (cmplx16ImRe_t *)&gRadarCubeIdealBuf[0];

#if defined(SUBSYS_MSS) || defined(SUBSYS_M4)
/* per rx antenna, per chirp for windowing results */
uint32_t dataWinOut[MAX_NUM_RANGEBIN] __attribute__((section(".dataBufL2")));
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(dataWinOut, ".dataBufL2");
uint32_t dataWinOut[MAX_NUM_RANGEBIN]; /* per rx antenna, per chirp for windowing results */
#endif

#define DPC_OBJDET_QFORMAT_RANGE_FFT 17
#define DPC_USE_SYMMETRIC_WINDOW_RANGE_DPU
#define DPC_DPU_RANGEPROC_FFT_WINDOW_TYPE MATHUTILS_WIN_HANNING
#define DPC_OBJDET_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL (16U)
#define DPC_DPU_RANGEPROC_INTERFMITIG_WINDOW_TYPE MATHUTILS_WIN_HANNING
#define DPC_OBJDET_QFORMAT_RANGEPROC_INTERFMITIG_WINDOW (5U)
#define DPU_RANGEPROCDDMATEST_ENOMEM__L3_RAM_RADAR_CUBE (-2)
#define DPU_RANGEPROCDDMATEST_RANGE_BINS_ERR (-3)
#define DPU_RANGEPROCDDMATEST_ENOMEM__CORE_LOCAL_RAM_RANGE_HWA_WINDOW (-4)
#define DPU_RANGEPROCDDMATEST_ENOMEM_HWA_WINDOW_RAM (-5)
#define DPU_RANGEPROCDDMATEST_ENOMEM_PREPROC_BUFFER (-6)
#define DPC_OBJDET_HWA_MAX_WINDOW_RAM_SIZE_IN_SAMPLES (CSL_DSS_HWA_WINDOW_RAM_U_SIZE >> 3)
/*! Radar cube data buffer alignment in bytes. */
#ifdef SUBSYS_MSS
#define DPC_OBJDET_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F
#else
#define DPC_OBJDET_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP
#endif

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

/**
 * @brief
 *  Decompression parameters for testing.
 */
typedef struct rangeProcDDMATestDecompressionCfg
{
    /*! @brief Flag that indicates if decompression is enabled */
    bool  isEnabled;

    /*! @brief Compression Method, 0 indicates EGE */
    uint8_t  compressionMethod;

    /*! @brief Number of samples in a single input block to be decompressed */
    uint16_t inputSamplesPerBlock;

    /*! @brief Number of samples in a single decompressed output block */
    uint16_t outputSamplesPerBlock;

    /*! @brief Number of blocks */
    uint16_t numBlocks;

    /*! @brief Bytes per input/output sample */
    uint16_t bytesPerSample;

    /*! @brief Number of bytes per input block */
    uint16_t inputBytesPerBlock;

    /*! @brief Number of bytes per decompressed output block */
    uint16_t outputBytesPerBlock;

    /*! @brief Number of Rx Antennas per block */
    uint16_t rxAntPerBlock;

    /*! @brief Number of Range Bins per block */
    uint16_t rangeBinsPerBlock;

    /*! @brief The actual compression ratio achieved */
    float achievedCompressionRatio;

    /*! @brief Blocks decompressed in single HWA pass */
    uint16_t blocksInOnePass;

    /*! @brief Total Number of blocks to decompress */
    uint16_t totalBlocksToDecompress;

    /*! @brief  HWA Common Config */
    HWA_CommonConfig    hwaCommonConfig;

}rangeProcDDMATestDecompressionCfg;


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

    /*! @brief     Data Input Mode, */
    DPU_RangeProcHWA_InputMode dataInputMode;

    /*! @brief     Compression Cfg */
    DPU_RangeProcHWA_CompressionCfg compressionCfg;

    /*! @brief Shift/Scale config for Interf Stats Mag Diff in range DPU */
    //TODO: Also add isenabled
    DPU_RangeProcHWADDMA_intfStatsdBCfg intfStatsdBCfg;

    /*! @brief The structure is used to hold the config needed for Range Proc HWA Chain */
    DPU_RangeProcHWADDMA_rangeProcCfg rangeProcCfg;

    /*! @brief     Decompression Cfg */
    rangeProcDDMATestDecompressionCfg decompCfg;

} DPU_RangeProcTest_StaticCfg;



DPU_RangeProcTest_StaticCfg testStaticCfg;
DPU_RangeProcTest_StaticCfg *staticCfg = &testStaticCfg;

/*! @brief L3 ram memory pool object */
MemPoolObj L3ramObjS;
MemPoolObj *L3ramObj = &L3ramObjS;

/*! @brief Core Local ram memory pool object */
MemPoolObj CoreLocalRamObjS;
MemPoolObj *CoreLocalRamObj = &CoreLocalRamObjS;

HWA_Handle hwaHandle;
volatile uint8_t subframeCount = 0U;
uint32_t chirpIdx = 0;

/* EDMA Out Signature channel is used to indicate that chirp is completed using
    below transfer completion tcc */
uint8_t edmaOutSignatureCompletionTcc;

DPU_RangeProcHWA_Handle rangeProcDpuHandle;
DPU_RangeProcHWA_Config rangeProcDpuCfg;
/*! @brief     EDMA interrupt object */
Edma_IntrObject intrObj;

uint8_t testFeatureStatus;

uint8_t checkResultFlag;

uint8_t finalResults;


#ifndef SUBSYS_M4
/* FreeRTOS Task declarations. */
#define APP_TASK_PRI (5U)
#define APP_TASK_STACK_SIZE (6 * MAX_NUM_RANGEBIN)

TaskHandle_t gAppTask;
StaticTask_t gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));
#endif

FILE *fileId;
FILE *fileId2;
FILE *fileId3;

static uint32_t DPU_RangeProcTest_GetRangeWinGenLen(DPU_RangeProcHWA_Config *cfg);

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
    uint32_t interfMitigWindow[DPC_OBJDET_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL >> 1];
    uint8_t idx;

    mathUtils_genWindow((uint32_t *)interfMitigWindow,
                        DPC_OBJDET_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL,
                        DPC_OBJDET_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL >> 1,
                        DPC_DPU_RANGEPROC_INTERFMITIG_WINDOW_TYPE,
                        DPC_OBJDET_QFORMAT_RANGEPROC_INTERFMITIG_WINDOW);

    /* Only 5 win samples are supported by the HWA */
    for (idx = 0; idx < DPU_RANGEPROCHWADDMA_NUM_INTFMITIG_WIN_HWACOMMONCFG_SIZE; idx++)
    {
        cfg->hwRes.hwaCfg.hwaInterfMitigWindow[DPU_RANGEPROCHWADDMA_NUM_INTFMITIG_WIN_HWACOMMONCFG_SIZE - 1 - idx] =
            (uint8_t)interfMitigWindow[(DPC_OBJDET_RANGEPROC_NUM_INTFMITIG_WIN_SIZE_TOTAL >> 1) - 2 - idx];
    }

    /* Range FFT window */
    mathUtils_genWindow((uint32_t *)cfg->staticCfg.window,
                        cfg->staticCfg.ADCBufData.dataProperty.numAdcSamples,
                        DPU_RangeProcTest_GetRangeWinGenLen(cfg),
                        DPC_DPU_RANGEPROC_FFT_WINDOW_TYPE,
                        DPC_OBJDET_QFORMAT_RANGE_FFT);
}

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
void DPU_RangeProcDDMA_EDMAChannelConfigAssist(EDMA_Handle handle, uint32_t chNum, uint32_t shadowParam, uint32_t eventQueue, DPEDMA_ChanCfg *chanCfg)
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

int32_t DPU_RangeProcTest_getNumSubBands(uint32_t numTxAntennas){

    int32_t numBandsEmpty;
    /* Empty subbands */
    switch (numTxAntennas)
    {
        case 2:
            numBandsEmpty = 1;
            break;
        case 3:
            numBandsEmpty = 1;
            break;
        case 4:
            numBandsEmpty = 2;
            break;
        default:
            numBandsEmpty = -1;
            goto exit;
    }

exit:
    return (numTxAntennas + numBandsEmpty);
}

/**
*  @b Description
*  @n
*    Based on the test configuration, set up the range processing DPU configurations
*/
int32_t Test_setProfile()
{

    /////////////////////////////////////////////////////////////////
    int32_t retVal = 0;
    DPIF_RadarCube radarCube;
    uint32_t hwaWindowOffset;
    float achievedCompressionRatio;
    uint32_t outputBytesPerBlock, inputBytesPerBlock;
    uint32_t radarCubeDecompressedSizeInBytes;
    uint32_t dcEstBufSize = 0, intfThresBufSize = 0;
    DPU_RangeProcHWA_EDMAInputConfig *edmaIn = &rangeProcDpuCfg.hwRes.edmaInCfg;
    DPU_RangeProcHWA_EDMAOutputConfig *edmaOut = &rangeProcDpuCfg.hwRes.edmaOutCfg;
    DPU_RangeProcHWA_HwaConfig *hwaCfg = &rangeProcDpuCfg.hwRes.hwaCfg;
    DPU_RangeProcHWA_HW_Resources *hwRes = &rangeProcDpuCfg.hwRes;
    int32_t *windowBuffer;
    uint32_t winGenLen;

    hwaWindowOffset = DPC_OBJDET_HWA_WINDOW_RAM_OFFSET;

    DPU_RangeProcTest_MemPoolReset(L3ramObj);
    DPU_RangeProcTest_MemPoolReset(CoreLocalRamObj);

    /* L3 allocations */
    /* L3 - radar cube */
    /* Input and output samples out of the rangeproc/compression DPU */
    if(staticCfg->compressionCfg.compressionMethod==HWA_COMPRESS_METHOD_BFP)
    {
        inputBytesPerBlock = 4 * staticCfg->compressionCfg.rangeBinsPerBlock;
        staticCfg->compressionCfg.bfpCompExtraParamSets = 2*(staticCfg->compressionCfg.numRxAntennaPerBlock - 1);
    }
    else
    {
        inputBytesPerBlock = 4 * staticCfg->compressionCfg.numRxAntennaPerBlock * staticCfg->compressionCfg.rangeBinsPerBlock;
        staticCfg->compressionCfg.bfpCompExtraParamSets = 0U;
    }

    outputBytesPerBlock = (MATHUTILS_CEILING_POS_FLOAT(inputBytesPerBlock * staticCfg->compressionCfg.compressionRatio / 4.0))
                            * 4; /* 32-bit boundary aligned */
    // DebugP_log("inputBytesPerBlock = %d, outputBytesPerBlock = %d\n", inputBytesPerBlock, outputBytesPerBlock);
    achievedCompressionRatio = (float)outputBytesPerBlock / (float)inputBytesPerBlock;
    // DebugP_log("achievedCompressionRatio = %f\n", achievedCompressionRatio);

    radarCubeDecompressedSizeInBytes = staticCfg->numRangeBins * staticCfg->numChirps *
                                       staticCfg->ADCBufData.dataProperty.numRxAntennas * sizeof(cmplx16ReIm_t);
    radarCube.dataSize = radarCubeDecompressedSizeInBytes * achievedCompressionRatio;
    radarCube.data = DPU_RangeProcTest_MemPoolAlloc(L3ramObj, radarCube.dataSize,
                                                    DPC_OBJDET_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT);
    if (radarCube.data == NULL)
    {
        retVal = DPU_RANGEPROCDDMATEST_ENOMEM__L3_RAM_RADAR_CUBE;
        goto exit;
    }
    radarCube.datafmt = DPIF_RADARCUBE_FORMAT_2;

    /////////////////////////////////////////////////////////////////

    // DebugP_log("Performing range config.. \n");

    memset(&rangeProcDpuCfg, 0, sizeof(DPU_RangeProcHWA_Config));

    /* static configuration */
    rangeProcDpuCfg.staticCfg.ADCBufData = staticCfg->ADCBufData;
    rangeProcDpuCfg.staticCfg.ADCBufData.data = staticCfg->ADCBufData.data;
    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty = staticCfg->ADCBufData.dataProperty;

    rangeProcDpuCfg.staticCfg.numChirpsPerFrame = staticCfg->numChirpsPerFrame;
    rangeProcDpuCfg.staticCfg.numRangeBins = staticCfg->numRangeBins;
    rangeProcDpuCfg.staticCfg.numFFTBins = staticCfg->numRangeFFTBins;
    rangeProcDpuCfg.staticCfg.numTxAntennas = staticCfg->numTxAntennas;
    rangeProcDpuCfg.staticCfg.numVirtualAntennas = staticCfg->numVirtualAntennas;
    rangeProcDpuCfg.staticCfg.numBandsTotal = DPU_RangeProcTest_getNumSubBands(staticCfg->numTxAntennas);

    if (rangeProcDpuCfg.staticCfg.numRangeBins == rangeProcDpuCfg.staticCfg.numFFTBins)
    {
        rangeProcDpuCfg.staticCfg.isChirpDataReal = 0;
    }
    else if (rangeProcDpuCfg.staticCfg.numRangeBins == rangeProcDpuCfg.staticCfg.numFFTBins / 2)
    {
        rangeProcDpuCfg.staticCfg.isChirpDataReal = 1;
    }
    else
    {
        retVal = DPU_RANGEPROCDDMATEST_RANGE_BINS_ERR;
        goto exit;
    }

    memcpy(&rangeProcDpuCfg.staticCfg.compressionCfg,
           &staticCfg->compressionCfg,
           sizeof(DPU_RangeProcHWA_CompressionCfg));
    memcpy(&rangeProcDpuCfg.staticCfg.rangeProcCfg, 
           &staticCfg->rangeProcCfg, 
           sizeof(DPU_RangeProcHWADDMA_rangeProcCfg));

    /* radarCube */
    rangeProcDpuCfg.hwRes.radarCube = radarCube;

    if(rangeProcDpuCfg.staticCfg.rangeProcCfg.rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE)
    {
        /* In 1st RangeProcChain, only DC estimates of every subframe index's last subframe is stored */
        dcEstBufSize = staticCfg->ADCBufData.dataProperty.numRxAntennas * sizeof(uint32_t);
        if(rangeProcDpuCfg.staticCfg.rangeProcCfg.isReal2XEnabled)
        {
            dcEstBufSize >>= 1; /* In Real 2X Mode, the DC estimate of other half of channels is stored as imaginary part of complex estimate */
        }
    }
    else if(rangeProcDpuCfg.staticCfg.rangeProcCfg.rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
    {
        /* In 2nd RangeProcChain, DC estimates and interference statistics are stored for every subbands last chirp */
        dcEstBufSize = rangeProcDpuCfg.staticCfg.numBandsTotal * staticCfg->ADCBufData.dataProperty.numRxAntennas * sizeof(uint32_t);
        intfThresBufSize = dcEstBufSize; /* Interference Thresholds are stored only in 2nd RangeProcChain */
    }
    if((rangeProcDpuCfg.staticCfg.rangeProcCfg.rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE)
        || (rangeProcDpuCfg.staticCfg.rangeProcCfg.rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE))
    {
        hwRes->dcEstIVal = (uint32_t *)DPU_RangeProcTest_MemPoolAlloc(CoreLocalRamObj, dcEstBufSize, sizeof(uint32_t));
        if (hwRes->dcEstIVal == NULL)
        {
            retVal = DPU_RANGEPROCDDMATEST_ENOMEM_PREPROC_BUFFER;
            goto exit;
        }
        memset(hwRes->dcEstIVal, 0, dcEstBufSize);
        if(rangeProcDpuCfg.staticCfg.rangeProcCfg.isReal2XEnabled)
        {
            hwRes->dcEstQVal = (uint32_t *)DPU_RangeProcTest_MemPoolAlloc(CoreLocalRamObj, dcEstBufSize, sizeof(uint32_t));
            if (hwRes->dcEstQVal == NULL)
            {
                retVal = DPU_RANGEPROCDDMATEST_ENOMEM_PREPROC_BUFFER;
                goto exit;
            }
            memset(hwRes->dcEstQVal, 0, dcEstBufSize);
        }
    }
    if(rangeProcDpuCfg.staticCfg.rangeProcCfg.rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
    {
        hwRes->intfThresMagVal = (uint32_t *)DPU_RangeProcTest_MemPoolAlloc(CoreLocalRamObj, intfThresBufSize, sizeof(uint32_t));
        if (hwRes->intfThresMagVal == NULL)
        {
            retVal = DPU_RANGEPROCDDMATEST_ENOMEM_PREPROC_BUFFER;
            goto exit;
        }

        hwRes->intfThresMagDiffVal = (uint32_t *)DPU_RangeProcTest_MemPoolAlloc(CoreLocalRamObj, intfThresBufSize, sizeof(uint32_t));
        if (hwRes->intfThresMagDiffVal == NULL)
        {
            retVal = DPU_RANGEPROCDDMATEST_ENOMEM_PREPROC_BUFFER;
            goto exit;
        }

        memset(hwRes->intfThresMagVal, 0, intfThresBufSize);
        memset(hwRes->intfThresMagDiffVal, 0, intfThresBufSize);
    }
    rangeProcHWA_loadPreProcStats(&rangeProcDpuCfg, dcEstBufSize, intfThresBufSize, 0);

    /* DC Est shift and scale */
    retVal = DPU_RangeProcHWA_findDCEstStaticParams(staticCfg->ADCBufData.dataProperty.numAdcSamples,
                                                    &rangeProcDpuCfg.staticCfg.dcEstShiftScaleCfg.scale, &rangeProcDpuCfg.staticCfg.dcEstShiftScaleCfg.shift);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Interf config */
    retVal = DPU_RangeProcHWA_findIntfStatsStaticParams(staticCfg->ADCBufData.dataProperty.numAdcSamples,
                                                        staticCfg->intfStatsdBCfg.intfMitgMagSNRdB,
                                                        &rangeProcDpuCfg.staticCfg.intfStatsMagShiftScaleCfg.scale, &rangeProcDpuCfg.staticCfg.intfStatsMagShiftScaleCfg.shift);
    if (retVal != 0)
    {
        goto exit;
    }
    retVal = DPU_RangeProcHWA_findIntfStatsStaticParams(staticCfg->ADCBufData.dataProperty.numAdcSamples,
                                                        staticCfg->intfStatsdBCfg.intfMitgMagDiffSNRdB,
                                                        &rangeProcDpuCfg.staticCfg.intfStatsMagDiffShiftScaleCfg.scale, &rangeProcDpuCfg.staticCfg.intfStatsMagDiffShiftScaleCfg.shift);
    if (retVal != 0)
    {
        goto exit;
    }

    /* static configuration - windows */
    /* Generating 1D window, allocate first */
    winGenLen = DPU_RangeProcTest_GetRangeWinGenLen(&rangeProcDpuCfg);
    rangeProcDpuCfg.staticCfg.windowSize = winGenLen * sizeof(uint32_t);
    windowBuffer = (int32_t *)DPU_RangeProcTest_MemPoolAlloc(CoreLocalRamObj, rangeProcDpuCfg.staticCfg.windowSize, sizeof(uint32_t));
    if (windowBuffer == NULL)
    {
        retVal = DPU_RANGEPROCDDMATEST_ENOMEM__CORE_LOCAL_RAM_RANGE_HWA_WINDOW;
        goto exit;
    }
    rangeProcDpuCfg.staticCfg.window = windowBuffer;
    DPU_RangeProcTest_GenRangeWindow(&rangeProcDpuCfg);

    /* hwres - edma */
    hwRes->edmaHandle = gEdmaHandle[CONFIG_EDMA0];
    /* We have choosen ISOLATE mode, so we have to fill in dataIn */

    DPU_RangeProcDDMA_EDMAChannelConfigAssist(hwRes->edmaHandle,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAIN_CH,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SHADOW,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAIN_EVENT_QUE,
                                              &edmaIn->dataIn);
    DPU_RangeProcDDMA_EDMAChannelConfigAssist(hwRes->edmaHandle,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SIG_CH,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SIG_SHADOW,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAIN_SIG_EVENT_QUE,
                                              &edmaIn->dataInSignature);

    DPU_RangeProcDDMA_EDMAChannelConfigAssist(hwRes->edmaHandle,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_SIG_CH,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_SIG_SHADOW,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_SIG_EVENT_QUE,
                                              &edmaOut->dataOutSignature);

    /* Ping */
    DPU_RangeProcDDMA_EDMAChannelConfigAssist(hwRes->edmaHandle,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_FMT1_PING_CH,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_FMT1_PING_SHADOW,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_FMT1_PING_EVENT_QUE,
                                              &edmaOut->u.fmt1.dataOutPing);

    /* Pong */
    DPU_RangeProcDDMA_EDMAChannelConfigAssist(hwRes->edmaHandle,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_FMT1_PONG_CH,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_FMT1_PONG_SHADOW,
                                              DPC_OBJDET_DPU_RANGEPROC_EDMAOUT_FMT1_PONG_EVENT_QUE,
                                              &edmaOut->u.fmt1.dataOutPong);

    {
        {

            uint32_t intrIdx = 0;

            /* Allocate interrupt object */
            rangeProcDpuCfg.hwRes.edmaTransferCompleteIntrObj = &rangProcIntrObj[intrIdx++];
        }
    }

    /* In this case HWA hardware trigger source is equal to HWA param index value*/
    hwaCfg->dataInputMode = DPU_RangeProcHWA_InputMode_ISOLATED;

#ifdef DPC_USE_SYMMETRIC_WINDOW_RANGE_DPU
    hwaCfg->hwaWinSym = HWA_FFT_WINDOW_SYMMETRIC;
#else
    hwaCfg->hwaWinSym = HWA_FFT_WINDOW_NONSYMMETRIC;
#endif
    hwaCfg->hwaWinRamOffset = (uint16_t)hwaWindowOffset;
    if ((hwaCfg->hwaWinRamOffset + winGenLen) > DPC_OBJDET_HWA_MAX_WINDOW_RAM_SIZE_IN_SAMPLES)
    {
        retVal = DPU_RANGEPROCDDMATEST_ENOMEM_HWA_WINDOW_RAM;
        goto exit;
    }

    hwaCfg->numParamSet = DPU_RANGEPROCHWADDMA_NUM_HWA_PARAM_SETS + staticCfg->compressionCfg.bfpCompExtraParamSets
                - (2 * staticCfg->rangeProcCfg.rangeProcChain);
    hwaCfg->paramSetStartIdx = DPC_OBJDET_DPU_RANGEPROC_PARAMSET_START_IDX;

exit:

    return retVal;
}

void rangeProcDpuTest_hwaInit()
{
    int32_t errorCode = 0;

    hwaHandle = HWA_open(0, NULL, &errorCode);
    if (hwaHandle == NULL)
    {
        DebugP_log("Error: Unable to open the HWA Instance err:%d\n", errorCode);
        DebugP_assert(0);
        return;
    }
}

void rangeProcDpuTest_dpuInit()
{
    int32_t errorCode = 0;
    DPU_RangeProcHWA_InitParams initParams;
    initParams.hwaHandle = hwaHandle;

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
        DebugP_log("Debug: RangeProc DPU initialization returned error %d\n", errorCode);
        DebugP_assert(0);
        return;
    }
}

bool dpuRangeProcHwa_IsZeroOrPowerOfTwo(uint32_t x)
{
    return ((x & (x - 1)) == 0);
}

bool dpuRangeProcHwa_compareResults(cmplx16ImRe_t *ideal, cmplx16ImRe_t *obtained, int32_t numSamplesToCompare)
{

    uint32_t i;
    int32_t diffReal, diffImag;
    bool result = 1;
    for (i = 0; i < numSamplesToCompare; i++)
    {
        int16_t idealReal, idealImag;
        idealReal = ideal[i].real >> testStaticCfg.rangeProcCfg.fftOutputScaling;
        idealImag = ideal[i].imag >> testStaticCfg.rangeProcCfg.fftOutputScaling;
        diffReal = ((idealReal - obtained[i].real) > 0) ? (idealReal - obtained[i].real) : -(idealReal - obtained[i].real);
        diffImag = ((idealImag - obtained[i].imag) > 0) ? (idealImag - obtained[i].imag) : -(idealImag - obtained[i].imag);
        if (diffReal > MAX_ALLOWED_ERROR || diffImag > MAX_ALLOWED_ERROR)
        {
            if (diffReal > 2*MAX_ALLOWED_ERROR || diffImag > 2*MAX_ALLOWED_ERROR)
            {
                DebugP_log("Mismatch at idx %d: Ideal %d + i(%d), Obtained %d + i(%d)\n", i, idealReal, idealImag, obtained[i].real, obtained[i].imag);
                result = 0;
            }
            else{
                DebugP_log("High error seen at idx %d: Ideal %d + i(%d), Obtained %d + i(%d)\n", i, idealReal, idealImag, obtained[i].real, obtained[i].imag);
            }
            finalResults = 0;
        }
    }

    return result;

}

int32_t dpuRangeProcHwa_configHwaDecompressionParamSet()
{
    HWA_ParamConfig         hwaParamCfg[4];
    uint32_t                paramsetIdx = 0;
    int32_t                 errCode = 0;
    uint8_t                 hwParamsetIdx = testStaticCfg.decompCfg.hwaCommonConfig.paramStartIdx;
    rangeProcDDMATestDecompressionCfg* pDPDecompParams;
    uint8_t                 index, rxAntIdx;

    pDPDecompParams = &testStaticCfg.decompCfg;
    for (index = hwParamsetIdx; index <= testStaticCfg.decompCfg.hwaCommonConfig.paramStopIdx; index++)
    {
        errCode = HWA_disableParamSetInterrupt(hwaHandle, index,
                                               HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    memset((void*) &hwaParamCfg, 0, 4 * sizeof(HWA_ParamConfig));

    /********************************************************************************/

    /*******************************/
    /* DECOMPRESSION PARAMSET */
    /*******************************/
    {{
        /* adcbuf not mapped, HWA is triggered after edma copy is done */
        hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_SOFTWARE;
        hwaParamCfg[paramsetIdx].triggerSrc = hwParamsetIdx;

        hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_COMPRESS;

        /* ACCELMODE CONFIG */
        hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_DISABLE;  // Disable dither
        hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.compressDecompress = HWA_CMP_DCMP_DECOMPRESS;
        hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.method = pDPDecompParams->compressionMethod;
        hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.passSelect = HWA_COMPRESS_PATHSELECT_BOTHPASSES;
        hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.headerEnable = HWA_FEATURE_BIT_ENABLE;
        hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.scaleFactorBW = 4; //log2(sample bits)

        /* SRC CONFIG */
        hwaParamCfg[paramsetIdx].source.srcAddr = HWA_MEMBANK_START_ADDRESS;

        hwaParamCfg[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        hwaParamCfg[paramsetIdx].source.srcSign = HWA_SAMPLES_UNSIGNED;
        hwaParamCfg[paramsetIdx].source.srcConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg[paramsetIdx].source.srcScale = 0;

        /* DEST CONFIG */
        hwaParamCfg[paramsetIdx].dest.dstAddr = HWA_MEMBANK_DEST_ADDRESS;

        hwaParamCfg[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; /* 16 bit real, 16 bit imag */
        hwaParamCfg[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED;
        hwaParamCfg[paramsetIdx].dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg[paramsetIdx].dest.dstScale = 0;
        hwaParamCfg[paramsetIdx].dest.dstSkipInit = 0;

        if(pDPDecompParams->compressionMethod == HWA_COMPRESS_METHOD_BFP)
        {
            /***************************************/
            /* PING BFP DECOMPRESSION PARAMSET RX1 */
            /***************************************/

            hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.BFPMantissaBW = floor((pDPDecompParams->inputBytesPerBlock*8U - hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.scaleFactorBW)\
                                                                                    /(pDPDecompParams->outputSamplesPerBlock * 2U));

            /* SRC CONFIG */
            hwaParamCfg[paramsetIdx].source.srcAcnt = pDPDecompParams->inputSamplesPerBlock - 1;
            hwaParamCfg[paramsetIdx].source.srcAIdx = pDPDecompParams->bytesPerSample;
            hwaParamCfg[paramsetIdx].source.srcBcnt = pDPDecompParams->blocksInOnePass/pDPDecompParams->rxAntPerBlock - 1;
            hwaParamCfg[paramsetIdx].source.srcBIdx = pDPDecompParams->inputBytesPerBlock * pDPDecompParams->rxAntPerBlock;

            /* DEST CONFIG */
            hwaParamCfg[paramsetIdx].dest.dstAcnt = pDPDecompParams->outputSamplesPerBlock - 1;
            hwaParamCfg[paramsetIdx].dest.dstAIdx = pDPDecompParams->bytesPerSample * pDPDecompParams->rxAntPerBlock;
            hwaParamCfg[paramsetIdx].dest.dstBIdx = pDPDecompParams->outputBytesPerBlock * pDPDecompParams->rxAntPerBlock;

            errCode = HWA_configParamSet(hwaHandle,
                                        hwParamsetIdx,
                                        &hwaParamCfg[paramsetIdx],NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            /**********************************/
            /* BFP DECOMPRESSION PARAMSET RXN */
            /**********************************/
            for(rxAntIdx=1; rxAntIdx<pDPDecompParams->rxAntPerBlock; rxAntIdx++)
            {
                paramsetIdx++;
                hwParamsetIdx++;
                hwaParamCfg[paramsetIdx] = hwaParamCfg[paramsetIdx-1];
                hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_IMMEDIATE;
                hwaParamCfg[paramsetIdx].source.srcAddr = HWA_MEMBANK_START_ADDRESS + rxAntIdx*pDPDecompParams->inputBytesPerBlock;
                hwaParamCfg[paramsetIdx].dest.dstAddr = HWA_MEMBANK_DEST_ADDRESS + rxAntIdx*pDPDecompParams->bytesPerSample;
                errCode = HWA_configParamSet(hwaHandle,
                                            hwParamsetIdx,
                                            &hwaParamCfg[paramsetIdx],NULL);
                if (errCode != 0)
                {
                    goto exit;
                }
            }
        }
        else if(pDPDecompParams->compressionMethod == HWA_COMPRESS_METHOD_EGE)
        {
            /******************************/
            /* EGE DECOMPRESSION PARAMSET */
            /******************************/
            hwaParamCfg[paramsetIdx].accelModeArgs.compressMode.EGEKarrayLength = 3; //log2(8)

            /* SRC CONFIG */
            hwaParamCfg[paramsetIdx].source.srcAcnt = pDPDecompParams->inputSamplesPerBlock - 1;
            hwaParamCfg[paramsetIdx].source.srcAIdx = pDPDecompParams->bytesPerSample;
            hwaParamCfg[paramsetIdx].source.srcBcnt = pDPDecompParams->blocksInOnePass - 1;
            hwaParamCfg[paramsetIdx].source.srcBIdx = pDPDecompParams->inputBytesPerBlock;

            /* DEST CONFIG */
            hwaParamCfg[paramsetIdx].dest.dstAcnt = pDPDecompParams->outputSamplesPerBlock - 1;
            hwaParamCfg[paramsetIdx].dest.dstAIdx = pDPDecompParams->bytesPerSample;
            hwaParamCfg[paramsetIdx].dest.dstBIdx = pDPDecompParams->outputBytesPerBlock;

            errCode = HWA_configParamSet(hwaHandle,
                                        hwParamsetIdx,
                                        &hwaParamCfg[paramsetIdx],NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }

    }}

exit:
    return(errCode);
}


int32_t dpuRangeProcHwa_configHwaDecompression()
{
    uint8_t numLoops;
    int32_t retVal;

    /* Decompression params */
    {{
        testStaticCfg.decompCfg.isEnabled = testStaticCfg.compressionCfg.isEnabled;
        testStaticCfg.decompCfg.compressionMethod = testStaticCfg.compressionCfg.compressionMethod;
        testStaticCfg.decompCfg.bytesPerSample = sizeof(cmplx16ImRe_t);
        testStaticCfg.decompCfg.rxAntPerBlock = testStaticCfg.compressionCfg.numRxAntennaPerBlock;
        testStaticCfg.decompCfg.rangeBinsPerBlock = testStaticCfg.compressionCfg.rangeBinsPerBlock;
        if(testStaticCfg.decompCfg.compressionMethod == HWA_COMPRESS_METHOD_BFP)
            testStaticCfg.decompCfg.outputSamplesPerBlock = testStaticCfg.compressionCfg.rangeBinsPerBlock;
        else
            testStaticCfg.decompCfg.outputSamplesPerBlock = testStaticCfg.decompCfg.rxAntPerBlock * testStaticCfg.compressionCfg.rangeBinsPerBlock;
        testStaticCfg.decompCfg.outputBytesPerBlock = testStaticCfg.decompCfg.outputSamplesPerBlock * testStaticCfg.decompCfg.bytesPerSample;
        testStaticCfg.decompCfg.numBlocks = testStaticCfg.numRangeBins * testStaticCfg.ADCBufData.dataProperty.numRxAntennas / testStaticCfg.decompCfg.outputSamplesPerBlock;
        testStaticCfg.decompCfg.inputBytesPerBlock = (MATHUTILS_CEILING_POS_FLOAT(testStaticCfg.decompCfg.outputBytesPerBlock * testStaticCfg.compressionCfg.compressionRatio / 4.0))
                                                        * 4; /* 32-bit boundary aligned */
        testStaticCfg.decompCfg.inputSamplesPerBlock = testStaticCfg.decompCfg.inputBytesPerBlock / testStaticCfg.decompCfg.bytesPerSample;
        testStaticCfg.decompCfg.achievedCompressionRatio = (float)testStaticCfg.decompCfg.inputBytesPerBlock / testStaticCfg.decompCfg.outputBytesPerBlock;
        testStaticCfg.decompCfg.totalBlocksToDecompress = testStaticCfg.decompCfg.numBlocks * testStaticCfg.numChirps;
        /* how many blocks fit in 32 kb? */
        testStaticCfg.decompCfg.blocksInOnePass = 32768 / testStaticCfg.decompCfg.outputBytesPerBlock;

        while (testStaticCfg.decompCfg.totalBlocksToDecompress % testStaticCfg.decompCfg.blocksInOnePass != 0)
        {
            testStaticCfg.decompCfg.blocksInOnePass /=2U;
        }
        numLoops = testStaticCfg.decompCfg.totalBlocksToDecompress / testStaticCfg.decompCfg.blocksInOnePass;

        /* Populate HWA Common config structure */
        testStaticCfg.decompCfg.hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG; /* numLoops, paramStartIdx, paramStopIdx combined here */

        if(testStaticCfg.decompCfg.compressionMethod == HWA_COMPRESS_METHOD_EGE)
        {
            testStaticCfg.decompCfg.hwaCommonConfig.configMask |= HWA_COMMONCONFIG_MASK_EGECOMRESS_KPARAM;

            /* EGE Compression values */
            testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.EGEKparam[0] = 3;
            testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.EGEKparam[1] = 4;
            testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.EGEKparam[2] = 5;
            testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.EGEKparam[3] = 7;
            testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.EGEKparam[4] = 9;
            testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.EGEKparam[5] = 11;
            testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.EGEKparam[6] = 13;
            testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.EGEKparam[7] = 15;
        }
        testStaticCfg.decompCfg.hwaCommonConfig.configMask |= HWA_COMMONCONFIG_MASK_CMP_LFSRSEED0;
        testStaticCfg.decompCfg.hwaCommonConfig.compressConfig.cmpLfsrSeed0 = 0x0000000B;
        testStaticCfg.decompCfg.hwaCommonConfig.numLoops = numLoops;
        testStaticCfg.decompCfg.hwaCommonConfig.paramStartIdx = DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS_DDMA + testStaticCfg.compressionCfg.bfpCompExtraParamSets
                                            - (2 * testStaticCfg.rangeProcCfg.rangeProcChain);
        testStaticCfg.decompCfg.hwaCommonConfig.paramStopIdx = testStaticCfg.decompCfg.hwaCommonConfig.paramStartIdx + testStaticCfg.compressionCfg.bfpCompExtraParamSets/2U;
    }}

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle,0);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = dpuRangeProcHwa_configHwaDecompressionParamSet();
    if (retVal < 0){
        goto exit;
    }

    /* Config Common Registers for decompression stage */
    retVal = HWA_configCommon(hwaHandle, &testStaticCfg.decompCfg.hwaCommonConfig);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Enable the HWA */
    retVal = HWA_enable(hwaHandle,1);
    if (retVal != 0)
    {
        goto exit;
    }

exit:
    return retVal;

}

int32_t dpuRangeProcHwa_decompress(DSSHWACCRegs* ctrlBaseAddr)
{

    uint8_t numLoops;
    uint32_t numBytesToDecompress, numBytesDecompressed;
    uint32_t compRadarCubeSrcAddress, idx;
    int32_t retVal;
    int8_t decompParamStopIdx;
    bool result = 1;

    retVal = dpuRangeProcHwa_configHwaDecompression();
    if(retVal < 0){
        goto exit;
    }

    numBytesToDecompress = testStaticCfg.decompCfg.blocksInOnePass * testStaticCfg.decompCfg.inputBytesPerBlock;
    numBytesDecompressed = testStaticCfg.decompCfg.blocksInOnePass * testStaticCfg.decompCfg.outputBytesPerBlock;

    numLoops = testStaticCfg.decompCfg.hwaCommonConfig.numLoops;

    compRadarCubeSrcAddress = (uint32_t)rangeProcDpuCfg.hwRes.radarCube.data;
    decompParamStopIdx = testStaticCfg.decompCfg.hwaCommonConfig.paramStopIdx;
    for(idx = 0; idx < numLoops; idx++){

        DebugP_log("Checking idx %d out of %d..\n", idx, numLoops - 1);

        memcpy((void *)HWA_MEMBANK_START_ADDRESS, (void *)compRadarCubeSrcAddress, numBytesToDecompress);
        HWA_setSoftwareTrigger(hwaHandle, HWA_TRIG_MODE_SOFTWARE);

        /* Wait for Decompression paramset done interrupt from HWA */
        while(CSL_FEXTR(ctrlBaseAddr->PARAM_DONE_SET_STATUS[0], decompParamStopIdx, decompParamStopIdx)!=1U);
        CSL_FINSR(ctrlBaseAddr->PARAM_DONE_CLR[0], decompParamStopIdx, decompParamStopIdx, 1U);

        memcpy((void *)decompRadarCubeAddress, (void *)(HWA_MEMBANK_DEST_ADDRESS), numBytesDecompressed);

        fread((void *)(gRadarCubeIdeal), sizeof(uint16_t), numBytesDecompressed/2, fileId3);

        compRadarCubeSrcAddress += numBytesToDecompress;
        // DebugP_log("numBytesDecompressed %d\n", numBytesDecompressed);
        result &= dpuRangeProcHwa_compareResults((cmplx16ImRe_t*)gRadarCubeIdeal, (cmplx16ImRe_t*)decompRadarCubeAddress, numBytesDecompressed / sizeof(cmplx16ImRe_t));

    }

    if(result){
        DebugP_log("Test Passed!\n");
    }
    else{
        DebugP_log("Test Failed!\n");
    }

exit:
    return retVal;

}

void dpuRangeProcHwa_enableEdmaTransferCompletion(uint32_t baseAddr,
                       DPEDMA_ChanCfg chanCfg)
{
    uint32_t paRAMId            = chanCfg.paramId;
    uint32_t shadowPaRAMId      = chanCfg.shadowPramId;
    uint32_t currPaRAMAddr      = baseAddr + EDMA_TPCC_OPT(paRAMId);
    uint32_t shadowPaRAMAddr    = baseAddr + EDMA_TPCC_OPT(shadowPaRAMId);
    uint32_t optVal;
    uint32_t* optAddr;

    optAddr     = (uint32_t*) currPaRAMAddr; // opt is the first element of EdmaPaRAM
    optVal      = *optAddr;
    optVal      |= (EDMA_OPT_TCINTEN_MASK | EDMA_OPT_ITCINTEN_MASK);
    optVal      &= ~EDMA_OPT_TCC_MASK;
    optVal      |= (((edmaOutSignatureCompletionTcc) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK);
    *optAddr    = optVal;

    optAddr     = (uint32_t*) shadowPaRAMAddr; // opt is the first element of EdmaPaRAM
    *optAddr    = optVal;
}

void DPU_RangeProcHWA_Test_adcDataIn(uint32_t intrIdx, uint32_t paramSet, void * arg)
{
    uint32_t j;
    uint32_t baseAddr, regionId;
    uint16_t chirpIdxRef;
    uint8_t numBytesPerInputSample;
    if (testStaticCfg.isChirpDataReal)
    {
        numBytesPerInputSample = sizeof(int16_t);
    }
    else
    {
        numBytesPerInputSample = sizeof(uint32_t); /* Complex (16 bit imag, 16 bit real) */
    }
    /* process chirp */
    /* chirpIdx % testConfig.numChirpsPerFrameRef*/
    chirpIdxRef = chirpIdx - (chirpIdx / testStaticCfg.numChirpsPerFrame * testStaticCfg.numChirps);

    /* read in one chirp data */
    for (j = 0; j < testStaticCfg.ADCBufData.dataProperty.numRxAntennas; j++)
    {
        if (numBytesPerInputSample == 2)
        {
            //16 byte aligned, 8 uint32 symbols
            memcpy((void *)&adcDataIn[(j * ((testStaticCfg.ADCBufData.dataProperty.numAdcSamples + 7) / 8 * 8)) / 2],
                    (void *)&dataInBuffer[(chirpIdxRef * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * testStaticCfg.ADCBufData.dataProperty.numAdcSamples + j * testStaticCfg.ADCBufData.dataProperty.numAdcSamples) / 2], numBytesPerInputSample * testStaticCfg.ADCBufData.dataProperty.numAdcSamples);
        }
        else
        {
            //16 byte aligned, 4 uint32 symbols
            memcpy((void *)&adcDataIn[j * ((testStaticCfg.ADCBufData.dataProperty.numAdcSamples + 3) / 4 * 4)],
                    (void *)&dataInBuffer[chirpIdxRef * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * testStaticCfg.ADCBufData.dataProperty.numAdcSamples + j * testStaticCfg.ADCBufData.dataProperty.numAdcSamples], numBytesPerInputSample * testStaticCfg.ADCBufData.dataProperty.numAdcSamples);
        }
    }

    // CacheP_wbInv(adcDataIn, (numBytesPerInputSample * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * ((testStaticCfg.ADCBufData.dataProperty.numAdcSamples + 3) / 4 * 4)), CacheP_TYPE_ALLD);

#if defined(SOC_AWR294X)
#ifdef SUBSYS_MSS
    if (!gIsChirpDataReal)
    {
        CacheP_wb(adcDataIn, (numBytesPerInputSample * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * ((testStaticCfg.ADCBufData.dataProperty.numAdcSamples + 3) / 4 * 4)), CacheP_TYPE_ALLD);
    }
    else
    {
        CacheP_wb(adcDataIn, (numBytesPerInputSample * testStaticCfg.ADCBufData.dataProperty.numRxAntennas * ((testStaticCfg.ADCBufData.dataProperty.numAdcSamples + 7) / 8 * 8)), CacheP_TYPE_ALLD);
    }
#endif
#endif

    baseAddr = EDMA_getBaseAddr(gEdmaHandle[0]);
    DebugP_assert(baseAddr != 0);

    regionId = EDMA_getRegionId(gEdmaHandle[0]);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    if(chirpIdx != 0)
    {
        while(EDMA_readIntrStatusRegion(baseAddr, regionId, edmaOutSignatureCompletionTcc) != 1);
        EDMA_clrIntrRegion(baseAddr, regionId, edmaOutSignatureCompletionTcc);
    }

    EDMA_enableTransferRegion(baseAddr, regionId, DPC_OBJDET_DPU_RANGEPROC_EDMAIN_CH, EDMA_TRIG_MODE_MANUAL);
    chirpIdx++;
    /* end of chirp */
}

/**
*  @b Description
*  @n
*        Testing Task
*/
void rangeProcDpuTest_Task(void *args)
{
    uint32_t i;
    uint32_t numTests, testIdx;
    uint32_t numBytesPerInputSample;
    int32_t retVal = 0;
    DPU_RangeProcHWA_OutParams outParms;
    uint32_t baseAddr, regionId;
    uint8_t readDataAgain;
    uint32_t numBytesToRead;
    char fileName[100];
    uint64_t taskStartTime, taskEndTime;
#if defined(SOC_AWR2943)
    char deviceType[] = "awr2943";
#elif defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
    char deviceType[] = "awr2944";
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
    taskStartTime= ClockP_getTimeUsec();
    DebugP_log("...... Initialization ...... \n");

    /* hwa, edma, and rangeprocDPU initialization*/
    rangeProcDpuTest_hwaInit();
    rangeProcDpuTest_dpuInit();

    strcpy(fileName, "testdata\\rangeprocDDMA_testVectors_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    fileId = fopen(fileName, "rb");
    if (fileId == NULL)
    {
        DebugP_log("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    strcpy(fileName, "testdata\\rangeprocDDMA_testAdcDataIn_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    fileId2 = fopen(fileName, "rb");
    if (fileId == NULL)
    {
        DebugP_log("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    strcpy(fileName, "testdata\\rangeprocDDMA_testResult_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    fileId3 = fopen(fileName, "rb");
    if (fileId3 == NULL)
    {
        DebugP_log("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    fread(&numTests, sizeof(uint32_t), 1, fileId);

    /* process each test */
    for (testIdx = 0; testIdx < numTests; testIdx++)
    {

        /* read in test config */
        fread(&testStaticCfg.numTxAntennas, sizeof(uint32_t), 1, fileId);
        fread(&testStaticCfg.ADCBufData.dataProperty.numRxAntennas, sizeof(uint32_t), 1, fileId);
        fread(&testStaticCfg.ADCBufData.dataProperty.numAdcSamples, sizeof(uint32_t), 1, fileId);
        fread(&testStaticCfg.numChirpsPerFrame, sizeof(uint32_t), 1, fileId);
        testStaticCfg.numChirps = testStaticCfg.numChirpsPerFrame;
        testStaticCfg.ADCBufData.dataProperty.numChirpsPerChirpEvent = 1;

        uint8_t compEnable;
        fread(&compEnable, sizeof(uint8_t), 1, fileId);
        testStaticCfg.compressionCfg.isEnabled = compEnable;
        fread(&testStaticCfg.compressionCfg.compressionMethod, sizeof(uint16_t), 1, fileId);
        fread(&testStaticCfg.compressionCfg.compressionRatio, sizeof(float), 1, fileId);
        fread(&testStaticCfg.compressionCfg.rangeBinsPerBlock, sizeof(uint16_t), 1, fileId);
        fread(&testStaticCfg.compressionCfg.numRxAntennaPerBlock, sizeof(uint16_t), 1, fileId);

        fread(&testStaticCfg.intfStatsdBCfg.intfMitgMagSNRdB, sizeof(uint32_t), 1, fileId);
        fread(&testStaticCfg.intfStatsdBCfg.intfMitgMagDiffSNRdB, sizeof(uint32_t), 1, fileId);
        testStaticCfg.dataInputMode = DPU_RangeProcHWA_InputMode_ISOLATED;

        fread(&testStaticCfg.isChirpDataReal, sizeof(uint16_t), 1, fileId);
        fread(&testStaticCfg.rangeProcCfg.rangeProcChain, sizeof(uint16_t), 1, fileId);
        fread(&testStaticCfg.rangeProcCfg.intfMitigMagThresMinLim, sizeof(uint16_t), 1, fileId);
        fread(&testStaticCfg.rangeProcCfg.intfMitigMagDiffThresMinLim, sizeof(uint16_t), 1, fileId);
        fread(&testStaticCfg.rangeProcCfg.isReal2XEnabled, sizeof(uint16_t), 1, fileId);
        testStaticCfg.rangeProcCfg.fftOutputScaling = 3U;
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
            testStaticCfg.numRangeFFTBins = mathUtils_getValidFFTSize(testStaticCfg.ADCBufData.dataProperty.numAdcSamples);
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
        testStaticCfg.ADCBufData.dataProperty.interleave = DPIF_RXCHAN_INTERLEAVE_MODE;

        fread(&readDataAgain, sizeof(uint8_t), 1, fileId);

        numBytesToRead = testStaticCfg.ADCBufData.dataProperty.numRxAntennas * testStaticCfg.ADCBufData.dataProperty.numAdcSamples * numBytesPerInputSample;

        if (readDataAgain)
        {
            DebugP_log("Reading Data From File.. 0 percent\n");
            for (i = 0; i < testStaticCfg.numChirps; i++)
            {
                fread((((uint16_t *)&dataInBuffer[0]) + i * numBytesToRead / 2),
                      sizeof(uint16_t), numBytesToRead / 2, fileId2);
                if(i%10 == 0){
                    DebugP_log("Reading Data From File.. %d percent\n", i * 100 / testStaticCfg.numChirps);
                }
            }
        }

        DebugP_log("##################################\n");
        DebugP_log("\nTest #%d  start\r\n", testIdx);
        DebugP_log("------------\n");
        DebugP_log("NumRx %d, NumTx %d\n", testStaticCfg.ADCBufData.dataProperty.numRxAntennas, testStaticCfg.numTxAntennas);
        DebugP_log("NumADCSamples %d, NumChirps %d AdcDataReal %d\n", testStaticCfg.ADCBufData.dataProperty.numAdcSamples, testStaticCfg.numChirpsPerFrame, testStaticCfg.isChirpDataReal);
        DebugP_log("RangeProcChain %d, Real2XEnable %d\n", testStaticCfg.rangeProcCfg.rangeProcChain, testStaticCfg.rangeProcCfg.isReal2XEnabled);
        DebugP_log("Compression: Method %d, Ratio %f, RangeBinsPerBlock %d\n", testStaticCfg.compressionCfg.compressionMethod, testStaticCfg.compressionCfg.compressionRatio, testStaticCfg.compressionCfg.rangeBinsPerBlock);
        DebugP_log("Interference: MagSNRdB %d, MagDiffSNRdB %d\n", testStaticCfg.intfStatsdBCfg.intfMitgMagSNRdB, testStaticCfg.intfStatsdBCfg.intfMitgMagDiffSNRdB);
        DebugP_log("------------\n");

        retVal = Test_setProfile();
        if (retVal < 0)
        {
            DebugP_log("DEBUG: Error in setting up profile:%d \n", retVal);
            DebugP_assert(0);
        }

        retVal = DPU_RangeProcHWA_config(rangeProcDpuHandle, &rangeProcDpuCfg);
        if (retVal < 0)
        {
            DebugP_log("DEBUG: RANGE DPU config return error:%d \n", retVal);
            DebugP_assert(0);
        }

        edmaOutSignatureCompletionTcc = rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPing.channel;

        DSSHWACCRegs* ctrlBaseAddr = (DSSHWACCRegs *)(((HWA_Object*)hwaHandle)->hwAttrs->ctrlBaseAddr);

        ctrlBaseAddr->PARAM_DONE_CLR[0] = 0xFFFFFFFFU;
        /* Reset the chirpIdx counter */
        chirpIdx = 0;
        /* Initialize this status bit */
        EDMA_clrIntrRegion(baseAddr, regionId, edmaOutSignatureCompletionTcc);
        /* Enable transfer completion on EDMA out signature channel to monitor completion of chirp */
        dpuRangeProcHwa_enableEdmaTransferCompletion(baseAddr, rangeProcDpuCfg.hwRes.edmaOutCfg.dataOutSignature);

        /* control the rangeproc hwa*/
        retVal = DPU_RangeProcHWA_control(rangeProcDpuHandle, &rangeProcDpuCfg,DPU_RangeProcHWA_Cmd_triggerProc, NULL, 0);
        if (retVal < 0)
        {
            /* Not Expected */
            DebugP_log("RangeProc DPU control error %d\n", retVal);
            DebugP_assert(0);
        }

        memset((void *)&outParms, 0, sizeof(DPU_RangeProcHWA_OutParams));

        retVal = DPU_RangeProcHWA_process(rangeProcDpuHandle, &rangeProcDpuCfg, &outParms);

        dpuRangeProcHwa_decompress(ctrlBaseAddr);

#ifdef SUBSYS_MSS
#if defined(SOC_AWR294X)
        CacheP_inv(gRadarCube, (sizeof(cmplx16ImRe_t) * MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME), CacheP_TYPE_ALLD);
#endif
#endif

        /* Free EDMA Channels. */
        /* DPEDMA_freeEDMAChannel(uint32_t *dmaCh, uint32_t *tcc, uint32_t *param, uint32_t *shadowParam)*/
        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.channel, &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.tcc,
                               &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.paramId, &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.shadowPramId);

        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.channel, &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.tcc,
                               &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.paramId, &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.shadowPramId);

        retVal = EDMA_unregisterIntr(gEdmaHandle[CONFIG_EDMA0], &rangProcIntrObj[0]);
        DebugP_assert(retVal == SystemP_SUCCESS);

        DebugP_log("\n");

        DebugP_log("%d Tests finished!\n\r", testIdx);
    }

    taskEndTime=ClockP_getTimeUsec();
    DebugP_log("Test Time Cycles: %lld\n",taskEndTime-taskStartTime);
    DebugP_log("All Tests finished!\n\r");

    /*delete the handler*/
    HWA_close(hwaHandle);
    DPU_RangeProcHWA_deinit(rangeProcDpuHandle);

    Board_driversClose();
    Drivers_close();

    DebugP_log("--- Test Completed ---\n");
    if (finalResults == 0)
    {
        DebugP_log ("Some tests have failed!!\n");
    }
    else
    {
        DebugP_log ("All tests have passed!!\n");
    }

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
    rangeProcDpuTest_Task(args);
#else
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
#endif
}
