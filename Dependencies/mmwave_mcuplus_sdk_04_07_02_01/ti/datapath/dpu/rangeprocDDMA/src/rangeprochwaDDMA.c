/*
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
/**
 *   @file  rangeprochwaDDMA.c
 *
 *   @brief
 *      Implements Range FFT data processing Unit using HWA.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* MCU+SDK Include files */
#include <drivers/hw_include/hw_types.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/HeapP.h>
#include <drivers/edma.h>
#ifdef SUBSYS_MSS
#include <kernel/dpl/CacheP.h>
#endif

/* HWA_SOC Include files */
#include <drivers/soc.h>

/* Data Path Include files */
#include <ti/datapath/dpu/rangeprocDDMA/rangeprochwaDDMA.h>

/* MATH utils library Include files */
#include <ti/utils/mathutils/mathutils.h>

/* Internal include Files */
#include <ti/datapath/dpu/rangeprocDDMA/include/rangeprochwaDDMA_internal.h>

#include <ti/control/mmwavelink/mmwavelink.h>

rangeProcHWAObj RangeObj __attribute__((aligned(HeapP_BYTE_ALIGNMENT)));

/* Flag to check input parameters */
#define DEBUG_CHECK_PARAMS   1

#define DPU_RANGEHWADDMA_MEM_BANK_DCEST_PING_IN     0
#define DPU_RANGEHWADDMA_MEM_BANK_DCSUB_PING_OUT    2
#define DPU_RANGEHWADDMA_MEM_BANK_FFT_PING_OUT      4
#define DPU_RANGEHWADDMA_MEM_BANK_COMP_PING_OUT     2

#define DPU_RANGEHWADDMA_MEM_BANK_DCEST_PONG_IN     1
#define DPU_RANGEHWADDMA_MEM_BANK_DCSUB_PONG_OUT    3
#define DPU_RANGEHWADDMA_MEM_BANK_FFT_PONG_OUT      6
#define DPU_RANGEHWADDMA_MEM_BANK_COMP_PONG_OUT     3

#define DPU_RANGEHWADDMA_ADDR_DCEST_PING_IN     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCEST_PING_IN])
/* DPU_RANGEHWADDMA_ADDR_DCEST_PING_OUT is not required */
#define DPU_RANGEHWADDMA_ADDR_DCSUB_PING_IN     DPU_RANGEHWADDMA_ADDR_DCEST_PING_IN
#define DPU_RANGEHWADDMA_ADDR_DCSUB_PING_OUT    HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCSUB_PING_OUT])
#define DPU_RANGEHWADDMA_ADDR_FFT_PING_IN       DPU_RANGEHWADDMA_ADDR_DCSUB_PING_OUT
#define DPU_RANGEHWADDMA_ADDR_FFT_PING_OUT      HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_FFT_PING_OUT])
#define DPU_RANGEHWADDMA_ADDR_COMP_PING_IN      DPU_RANGEHWADDMA_ADDR_FFT_PING_OUT
#define DPU_RANGEHWADDMA_ADDR_COMP_PING_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_COMP_PING_OUT])

#define DPU_RANGEHWADDMA_ADDR_DCEST_PONG_IN     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCEST_PONG_IN])
/* DPU_RANGEHWADDMA_ADDR_DCEST_PONG_OUT is not required */
#define DPU_RANGEHWADDMA_ADDR_DCSUB_PONG_IN     DPU_RANGEHWADDMA_ADDR_DCEST_PONG_IN
#define DPU_RANGEHWADDMA_ADDR_DCSUB_PONG_OUT    HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCSUB_PONG_OUT])
#define DPU_RANGEHWADDMA_ADDR_FFT_PONG_IN       DPU_RANGEHWADDMA_ADDR_DCSUB_PONG_OUT
#define DPU_RANGEHWADDMA_ADDR_FFT_PONG_OUT      HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_FFT_PONG_OUT])
#define DPU_RANGEHWADDMA_ADDR_COMP_PONG_IN      DPU_RANGEHWADDMA_ADDR_FFT_PONG_OUT
#define DPU_RANGEHWADDMA_ADDR_COMP_PONG_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_COMP_PONG_OUT])

/**************************************************************************
 ************************ Internal Functions Prototype       **********************
 **************************************************************************/
static void rangeProcHWADoneIsrCallback(uint32_t threadIdx, void * arg);
void rangeProcHWA_EDMA_transferCompletionCallbackFxn(Edma_IntrHandle intrHandle,
   void *args);

static int32_t rangeProcHWA_ConfigHWA
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint8_t     destChanPing,
    uint8_t     destChanPong
);

static int32_t rangeProcHWA_TriggerHWA
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProcHWA_Config*    rangeHwaCfg
);
static int32_t rangeProcHWA_ConfigEDMA_DataOut_interleave
(
    rangeProcHWAObj     *rangeProcObj,
    rangeProc_dpParams  *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t            hwaInPingOffset,
    uint32_t            hwaInPongOffset,
    uint32_t            hwaOutPingOffset,
    uint32_t            hwaOutPongOffset
);
static int32_t rangeProcHWA_ConfigEDMA_DataIn
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
);
static int32_t rangeProcHWA_ConfigInterleaveMode
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
);

extern void DPU_RangeProcHWA_Test_adcDataIn
(
    uint32_t intrIdx, 
    uint32_t paramSet, 
    void * arg
);

/**************************************************************************
 ************************RangeProcHWA Internal Functions **********************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      HWA processing completion call back function as per HWA API.
 *      Depending on the programmed transfer completion codes,
 *      posts HWA done semaphore.
 *
 *  @param[in]  threadIdx           Thread index
 *  @param[in]  arg                 Argument to the callback function
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
volatile uint32_t hwadoneisr = 0;
static void rangeProcHWADoneIsrCallback(uint32_t threadIdx, void * arg)
{
    hwadoneisr++;
    if (arg != NULL)
    {
        SemaphoreP_post((SemaphoreP_Object*)arg);
    }
}
/**
 *  @b Description
 *  @n
 *      EDMA processing completion call back function as per EDMA API.
 *
 *  @param[in]  intrHandle               EDMA Interrupt handle
 *  @param[in]  args                     Argument to the callback function
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
void rangeProcHWA_EDMA_transferCompletionCallbackFxn(Edma_IntrHandle intrHandle,
                                                     void *args)
{
    rangeProcHWAObj     *rangeProcObj;

    /* Get rangeProc object */
    rangeProcObj = (rangeProcHWAObj *)args;

    rangeProcObj->numEdmaDataOutCnt++;
    SemaphoreP_post(&rangeProcObj->edmaDoneSemaHandle);
}

/**
 *  @b Description
 *  @n
 *      Function to calculate the DC Estimation scale and shift
 *      to be sent to the HWA.
 *
 *  @param[in]   numSamples              number of ADC samples
 *  @param[out]  scaleVal                scale to be fed to HWA
 *  @param[out]  shiftVal                shift value to be fed to HWA
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
int32_t DPU_RangeProcHWA_findDCEstStaticParams(uint32_t numSamples, uint32_t *scaleVal, uint32_t *shiftVal)
{

    uint32_t scale_best = 0U;
    uint32_t shift_best = 6U;
    uint32_t shift, scale_curr;

    for(shift = 0U; shift <= 14U; shift++)
    {
        scale_curr = ((uint32_t)1U << (shift + 8U + 6U)) / numSamples;
        if((scale_curr > scale_best) && (scale_curr < (((uint32_t)1U << 9U) - 1U)))
        {
            scale_best = scale_curr;
            shift_best = shift;
        }
    }

    *scaleVal = scale_best;
    *shiftVal = shift_best;
    return 0U;

}


/**
 *  @b Description
 *  @n
 *      Function to calculate the Interference mitigation static params
 *      to be sent to the HWA.
 *
 *  @param[in]   numSamples              number of ADC samples
 *  @param[in]   SNRdB                   SNR in dB
 *  @param[out]  scaleVal                scale to be fed to HWA
 *  @param[out]  shiftVal                shift value to be fed to HWA
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
int32_t DPU_RangeProcHWA_findIntfStatsStaticParams(uint32_t numSamples, uint32_t SNRdB, uint32_t *scaleVal, uint32_t *shiftVal)
{

    uint32_t scale_best = 0;
    uint32_t shift_best = 6;
    uint32_t shift, scale_curr;
    float scale_curr_float;
    uint32_t temp;

    for(shift = 0U; shift <= 6U; shift++)
    {
        temp = 256U * 2U * ((uint32_t)1U << shift);
        scale_curr_float = MATHUTILS_ROUND_FLOAT(((float)pow(10.0, ((double)SNRdB/20.0))/(float)numSamples)*(float)temp);
        scale_curr = (uint32_t) (scale_curr_float);
        if((scale_curr > scale_best) && (scale_curr < (((uint32_t)1U << 8U) - 1U)))
        {
            scale_best = scale_curr;
            shift_best = shift;
        }
    }

    *scaleVal = scale_best;
    *shiftVal = shift_best;

    return 0;

}

/**
 *  @b Description
 *  @n
 *      Function to config a EDMA in transpose format, when compression is enabled
 *
 *  @param[in]  dpParams                Pointer to data path parameters
 *  @param[in]  handle                  EDMA handle
 *  @param[in]  compressionParams       Compression parameters
 *  @param[in]  chanCfg                 EDMA channel configuraton
 *  @param[in]  chainingCfg             EDMA chaining configuration
 *  @param[in]  srcAddress              EDMA copy source address
 *  @param[in]  destAddress             EDMA copy destination address
 *  @param[in]  isTransferCompletionEnabled Number of iterations the dummy channel will be excuted.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *  @param[out] intrObj                 EDMA Interrupt Object
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static int32_t rangeProcHWA_ConfigEDMATransposeCompressed
(
    rangeProc_dpParams      *dpParams,
    rangeProcHWACompressionCfg  *compressionParams,
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    uint32_t                srcAddress,
    uint32_t                destAddress,
    bool                    isTransferCompletionEnabled,
    Edma_EventCallback      transferCompletionCallbackFxn,
    void*                   transferCompletionCallbackFxnArg,
    Edma_IntrObject         *intrObj
)
{
    DPEDMA_syncABCfg        syncABCfg;
    int32_t                 retVal;

    /* dpedma configuration */
    if(compressionParams->compressionMethod==HWA_COMPRESS_METHOD_BFP)
    {
        syncABCfg.aCount = compressionParams->outputBytesPerBlock * compressionParams->rxAntPerBlock;
        syncABCfg.bCount = compressionParams->numBlocks / compressionParams->rxAntPerBlock;
        syncABCfg.cCount = dpParams->numChirpsPerFrame/2U;
        syncABCfg.srcBIdx = (int32_t) compressionParams->outputBytesPerBlock * (int32_t) compressionParams->rxAntPerBlock;
        syncABCfg.srcCIdx = (int16_t) 0U;
        syncABCfg.dstBIdx = (int32_t) compressionParams->outputBytesPerBlock * (int32_t) compressionParams->rxAntPerBlock * (int32_t) dpParams->numChirpsPerFrame;
        syncABCfg.dstCIdx = (int16_t) compressionParams->outputBytesPerBlock * (int8_t) compressionParams->rxAntPerBlock * 2;
    }
    else
    {
        syncABCfg.aCount = compressionParams->outputBytesPerBlock;
        syncABCfg.bCount = compressionParams->numBlocks;
        syncABCfg.cCount = dpParams->numChirpsPerFrame/2U;
        syncABCfg.srcBIdx = (int32_t) compressionParams->outputBytesPerBlock;
        syncABCfg.srcCIdx = (int16_t) 0U;
        syncABCfg.dstBIdx = (int32_t) compressionParams->outputBytesPerBlock * (int32_t) dpParams->numChirpsPerFrame;
        syncABCfg.dstCIdx = (int16_t) compressionParams->outputBytesPerBlock * 2;
    }

    syncABCfg.srcAddress = srcAddress;
    syncABCfg.destAddress= destAddress;

    retVal = DPEDMA_configSyncAB(handle,
            chanCfg,
            chainingCfg,
            &syncABCfg,
            true,    /* isEventTriggered */
            false,   /* isIntermediateTransferCompletionEnabled */
            isTransferCompletionEnabled,   /* isTransferCompletionEnabled */
            transferCompletionCallbackFxn,
            transferCompletionCallbackFxnArg,
            intrObj);

    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Internal function to config HWA to perform range FFT
 *
 *  @param[in]  rangeProcObj                  Pointer to rangeProc object
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigHWACommon
(
    rangeProcHWAObj                     *rangeProcObj
)
{

    int32_t     retVal;
    rangeProc_dpParams *DPParams;
    DPU_RangeProcHWA_HwaConfig *hwaCfg;
    uint8_t EGEKparam[8] = {3, 4, 5, 7, 9, 11, 13, 15};
    
    if (rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    DPParams = &rangeProcObj->params;
    hwaCfg = &rangeProcObj->hwaCfg;

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    retVal = HWA_initConfig(rangeProcObj->initParms.hwaHandle);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = HWA_configCmpLFSRSeed0(0x0000000B);
    if (retVal != 0)
    {
        goto exit;
    }

    if(rangeProcObj->compressionCfg.compressionMethod == HWA_COMPRESS_METHOD_EGE)
    {
        retVal = HWA_configEGECompressKParams(EGEKparam);
        if (retVal != 0)
        {
            goto exit;
        }
    }
#if defined(DATAPATH_TEST) || defined(OBJ_DETECTION_DDMA_TEST)
    retVal = HWA_configTwidDitherEnable(HWA_FEATURE_BIT_DISABLE); // Disable dither for datapath bit-exact test
    if (retVal != 0)
    {
        goto exit;
    }
#else
    retVal = HWA_configTwidDitherEnable(HWA_FEATURE_BIT_ENABLE); // Enable Dither for FFT twiddle factor to attenuate quantization spurs.
    if (retVal != 0)
    {
        goto exit;
    }
#endif
    retVal = HWA_configFFTLFSRSeed(0x0B); /*Some non-zero value*/
    if (retVal != 0)
    {
        goto exit;
    }
    retVal = HWA_configStateMachine(DPParams->numChirpsPerFrame / 2U, rangeProcObj->hwaCfg.paramSetStartIdx, 
            (uint16_t) rangeProcObj->hwaCfg.paramSetStartIdx + rangeProcObj->hwaCfg.numParamSet - 1U);
    if (retVal != 0)
    {
        goto exit;
    }

    /* DC Est shift and scale */
    retVal = HWA_configDCEstimationProfile1((uint16_t)rangeProcObj->dcEstShiftScaleCfg.scale, (uint8_t)rangeProcObj->dcEstShiftScaleCfg.shift);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Interf config */
    retVal = HWA_configInterfSumMagScaler((uint16_t)rangeProcObj->intfStatsMagShiftScaleCfg.scale, (int8_t)rangeProcObj->intfStatsMagShiftScaleCfg.shift);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = HWA_configInterfSumMagDiffScaler((uint16_t)rangeProcObj->intfStatsMagDiffShiftScaleCfg.scale, (int8_t)rangeProcObj->intfStatsMagDiffShiftScaleCfg.shift);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = HWA_configInterfMitigationWindowParams(hwaCfg->hwaInterfMitigWindow);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = HWA_configTwiddleIncrDeltaFrac(0);
    if (retVal != 0)
    {
        goto exit;
    }

    /**********************************************/
    /* ENABLE NUMLOOPS DONE INTERRUPT FROM HWA */
    /**********************************************/
    retVal = HWA_enableDoneInterrupt(rangeProcObj->initParms.hwaHandle,
                                        0, //thread index
                                        rangeProcHWADoneIsrCallback,
                                        (void*)&rangeProcObj->hwaDoneSemaHandle);
    if (retVal != 0)
    {
        goto exit;
    }

exit:
    return(retVal);
}
/**
 *  @b Description
 *  @n
 *      Internal function to config HWA to perform range FFT
 *
 *  @param[in]  rangeProcObj                  Pointer to rangeProc object
 *  @param[in]  pHwConfig                     Pointer to rangeProc hardware resources
 *  @param[in]  destChanPing                  Destination channel id for PING
 *  @param[in]  destChanPong                  Destination channel id for PONG
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigHWA(
    rangeProcHWAObj *rangeProcObj,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint8_t destChanPing,
    uint8_t destChanPong)
{
    HWA_InterruptConfig paramISRConfig;
    int32_t errCode = 0;
    uint8_t pingHwParamsetIdx, pongHwParamsetIdx;
    HWA_ParamConfig hwaParamCfg;
    HWA_Handle hwaHandle;
    rangeProcHWACompressionCfg *pDPCompParams;
    rangeProc_dpParams *pDPParams;
    uint16_t index = 0U, rxAntIdx = 0U;

    hwaHandle = rangeProcObj->initParms.hwaHandle;
    pDPParams = &rangeProcObj->params;
    pDPCompParams = &rangeProcObj->compressionCfg;

    pingHwParamsetIdx = rangeProcObj->hwaCfg.paramSetStartIdx;
    pongHwParamsetIdx = pingHwParamsetIdx + (rangeProcObj->hwaCfg.numParamSet / 2U);
    for (index = 0; index < rangeProcObj->hwaCfg.numParamSet; index++)
    {
        errCode = HWA_disableParamSetInterrupt(hwaHandle, (uint8_t)index + rangeProcObj->hwaCfg.paramSetStartIdx,
                                               HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    /***********************/
    /* PING DUMMY PARAMSET */
    /***********************/
    {
        (void)memset(&hwaParamCfg, 0, sizeof(hwaParamCfg));
        if (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
        {
            /* Once, the DC Est. and Interf. Stats. are programmed, trigger is given by software */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_SOFTWARE;
        }
        else
        {
            /* No SW intervenetion, once ping chirp result is copied out, signature channel triggers the paramset. */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            hwaParamCfg.triggerSrc = pingHwParamsetIdx;
        }
        hwaParamCfg.accelMode = HWA_ACCELMODE_NONE;

        errCode = HWA_configParamSet(hwaHandle,
                                     pingHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
#ifdef DATAPATH_TEST
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1;
        paramISRConfig.cpu.callbackFn = DPU_RangeProcHWA_Test_adcDataIn;
        paramISRConfig.cpu.callbackArg = NULL;
        errCode = HWA_enableParamSetInterrupt(hwaHandle, pingHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
#endif
    }

    /***********************/
    /* PONG DUMMY PARAMSET */
    /***********************/
    {
        if (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
        {
            /* Once, the DC Est. and Interf. Stats. are programmed, trigger is given by software */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_SOFTWARE;
        }
        else
        {
            /* No SW intervenetion, once ping chirp result is copied out, signature channel triggers the paramset. */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            hwaParamCfg.triggerSrc = pongHwParamsetIdx;
        }
        hwaParamCfg.accelMode = HWA_ACCELMODE_NONE;
        errCode = HWA_configParamSet(hwaHandle,
                                     pongHwParamsetIdx,
                                     &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
#ifdef DATAPATH_TEST
        errCode = HWA_enableParamSetInterrupt(hwaHandle, pongHwParamsetIdx, &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
#endif
    }

    if(rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_DEFAULT_MODE)
    {
        (void)memset(&hwaParamCfg, 0, sizeof(hwaParamCfg));
        /**************************************************************/
        /* DPU_RANGEPROCHWA_DEFAULT_MODE: PING DC ESTIMATION PARAMSET */
        /**************************************************************/
        {
            pingHwParamsetIdx++;

            if (rangeProcObj->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
            {
                /* At a HWA trigger time adc samples are already in HWA memory */
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_HARDWARE;
                hwaParamCfg.triggerSrc = rangeProcObj->hwaCfg.hardwareTrigSrc;
            }
            else
            {
                /* adcbuf not mapped, HWA is triggered after edma copy is done */
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
                hwaParamCfg.triggerSrc = pingHwParamsetIdx;
            }

            hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

            /* PREPROC CONFIG */
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET;

            /* ACCELMODE CONFIG */
            hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE; /* No FFT is being performed here */

            hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
            hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

            /* SOURCE CONFIG */
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_DCEST_PING_IN;

            hwaParamCfg.source.srcAcnt = pDPParams->numAdcSamples - 1U; /* this is samples - 1 */
            hwaParamCfg.source.srcAIdx = (int32_t)pDPParams->numRxAntennas * (int32_t)pDPParams->sizeOfInputSample;
            if(rangeProcObj->isReal2XEnabled)
            {
                /* In Real2X Mode and size of sample is doubled as it is considered complex, 
                 * number of Rx antennas is effectively halved, hence numRxAntennas needs to be even.*/
                hwaParamCfg.source.srcBcnt = ((uint16_t)(pDPParams->numRxAntennas) / 2U) - 1U;
                hwaParamCfg.source.srcBIdx = (int32_t)pDPParams->sizeOfInputSample * 2;
            }
            else
            {
                hwaParamCfg.source.srcBcnt = (uint16_t)pDPParams->numRxAntennas - 1U;
                hwaParamCfg.source.srcBIdx = (int32_t)pDPParams->sizeOfInputSample;
            }

            if (((bool)pDPParams->isReal) && (rangeProcObj->isReal2XEnabled == false))
            {
                hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL;
            }
            else
            {
                /* In Real 2x Mode, samples are considered complex. */
                hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
            }
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
            hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
            hwaParamCfg.source.srcConjugate = 0;
            hwaParamCfg.source.srcScale = 8;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_DCSUB_PING_OUT; /* Shouldn't matter */

            hwaParamCfg.dest.dstAcnt = pDPParams->numAdcSamples - 1U; /* No FFT is being performed here */
            hwaParamCfg.dest.dstAIdx = (int32_t) pDPParams->numRxAntennas * (int32_t) pDPParams->sizeOfInputSample;
            if(rangeProcObj->isReal2XEnabled)
            {
                /* In Real 2x Mode, samples are considered complex, hence twice the sample size. */
                hwaParamCfg.dest.dstBIdx = (int32_t) pDPParams->sizeOfInputSample * 2;
            }
            else
            {
                hwaParamCfg.dest.dstBIdx = (int32_t) pDPParams->sizeOfInputSample;
            }

            if (((bool)pDPParams->isReal) && (rangeProcObj->isReal2XEnabled == false))
            {
                hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
            }
            else
            {
                /* In Real 2x Mode, samples are considered complex. */
                hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
            }
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
            hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
            hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
            hwaParamCfg.dest.dstScale = 0;
            hwaParamCfg.dest.dstSkipInit = 0;

            errCode = HWA_configParamSet(hwaHandle,
                                         pingHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }

        /**************************************************************/
        /* DPU_RANGEPROCHWA_DEFAULT_MODE: PONG DC ESTIMATION PARAMSET */
        /**************************************************************/
        {
            pongHwParamsetIdx++;
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_DCEST_PONG_IN;
            hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_DCSUB_PONG_OUT;

            if (rangeProcObj->hwaCfg.dataInputMode != DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
            {
                hwaParamCfg.triggerSrc = pongHwParamsetIdx;
            }

            errCode = HWA_configParamSet(hwaHandle,
                                         pongHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

    if((rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_DEFAULT_MODE)
        || (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE))
    {
        (void)memset(&hwaParamCfg, 0, sizeof(hwaParamCfg));
        /****************************************************************************************************************************/
        /* DPU_RANGEPROCHWA_DEFAULT_MODE: PING DC SUBTRACTION, INTERFERENCE STATISTICS ESTIMATION PARAMSET                          */
        /* DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE: PING DC ESTIMATION, DC SUBTRACTION, INTERFERENCE STATISTICS ESTIMATION PARAMSET */
        /****************************************************************************************************************************/

        {
            pingHwParamsetIdx++;

            if(rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE)
            {
                if (rangeProcObj->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
                {
                    /* At a HWA trigger time adc samples are already in HWA memory */
                    hwaParamCfg.triggerMode = HWA_TRIG_MODE_HARDWARE;
                    hwaParamCfg.triggerSrc = rangeProcObj->hwaCfg.hardwareTrigSrc;
                }
                else
                {
                    /* adcbuf not mapped, HWA is triggered after edma copy is done */
                    hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
                    hwaParamCfg.triggerSrc = pingHwParamsetIdx;
                }
            }
            else
            {
                /* In DPU_RANGEPROCHWA_DEFAULT_MODE, this paramset is immediate triggered */
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            }
            hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

            /* PREPROC CONFIG */
            if(rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE)
            {
                /* In this rangeProcChain, internal DC Estimate accumulator is reset only in the first iteration of loop.
                 * This is to calculate the average over a (sub)frame. */
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET_ZEROLPCONT;
            }
            else
            {
                /* InDPU_RANGEPROCHWA_DEFAULT_MODE mode, this paramset does not update DC Estimate registers as it updated in previous paramset */
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
            }
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_ENABLE;
            if(pHwConfig->dcEstIVal == NULL)
            {
                /* When dcEstIVal pointer is null, internal DC Estimate value is used, otherwise processor programs the SW value */
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCEST;
            }
            else
            {
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCSW;
            }
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfStat.resetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET;

            /* ACCELMODE CONFIG */
            hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_DISABLE; /* No FFT is being performed here */

            hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
            hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

            /* SOURCE CONFIG */
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_DCSUB_PING_IN;

            hwaParamCfg.source.srcAcnt = pDPParams->numAdcSamples - 1U; /* this is samples - 1 */
            hwaParamCfg.source.srcAIdx = (int32_t) pDPParams->numRxAntennas * (int32_t) pDPParams->sizeOfInputSample;
            if(rangeProcObj->isReal2XEnabled)
            {
                /* In Real2X Mode and size of sample is doubled as it is considered complex, 
                 * number of Rx antennas is effectively halved, hence numRxAntennas needs to be even.*/
                hwaParamCfg.source.srcBcnt = ((uint16_t)(pDPParams->numRxAntennas) / 2U) - 1U;
                hwaParamCfg.source.srcBIdx = (int32_t) pDPParams->sizeOfInputSample * 2;
            }
            else
            {
                hwaParamCfg.source.srcBcnt = (uint16_t)pDPParams->numRxAntennas - 1U;
                hwaParamCfg.source.srcBIdx = (int32_t) pDPParams->sizeOfInputSample;
            }

            if (((bool)pDPParams->isReal) && (rangeProcObj->isReal2XEnabled == false))
            {
                /* In Real 2x Mode, samples are considered complex. */
                hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL;
            }
            else
            {
                hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
            }
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
            hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
            hwaParamCfg.source.srcConjugate = 0;
            hwaParamCfg.source.srcScale = 8;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_DCSUB_PING_OUT;

            hwaParamCfg.dest.dstAcnt = pDPParams->numAdcSamples - 1U; /* No FFT is being performed here */
            hwaParamCfg.dest.dstAIdx = (int32_t) pDPParams->numRxAntennas * (int32_t) pDPParams->sizeOfInputSample;
            if(rangeProcObj->isReal2XEnabled)
            {
                /* In Real 2x Mode, samples are considered complex, hence twice the sample size. */
                hwaParamCfg.dest.dstBIdx = (int32_t) pDPParams->sizeOfInputSample * 2;
            }
            else
            {
                hwaParamCfg.dest.dstBIdx = (int32_t) pDPParams->sizeOfInputSample;
            }

            if (((bool)pDPParams->isReal) && (rangeProcObj->isReal2XEnabled == false))
            {
                hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
            }
            else
            {
                hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
            }
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
            hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
            hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
            hwaParamCfg.dest.dstScale = 0;
            hwaParamCfg.dest.dstSkipInit = 0;

            errCode = HWA_configParamSet(hwaHandle,
                                        pingHwParamsetIdx,
                                        &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }

        /****************************************************************************************************************************/
        /* DPU_RANGEPROCHWA_DEFAULT_MODE: PONG DC SUBTRACTION, INTERFERENCE STATISTICS ESTIMATION PARAMSET                          */
        /* DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE: PONG DC ESTIMATION, DC SUBTRACTION, INTERFERENCE STATISTICS ESTIMATION PARAMSET */
        /****************************************************************************************************************************/
        {
            pongHwParamsetIdx++;
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_DCSUB_PONG_IN;
            hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_DCSUB_PONG_OUT;

            if (rangeProcObj->hwaCfg.dataInputMode != DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
            {
                hwaParamCfg.triggerSrc = pongHwParamsetIdx;
            }

            errCode = HWA_configParamSet(hwaHandle,
                                        pongHwParamsetIdx,
                                        &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

    (void)memset(&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /**********************************************************************************************************************************************************/
    /* DPU_RANGEPROCHWA_DEFAULT_MODE: PING INTERFERENCE MITIGATION, FFT PARAMSET                                                                              */
    /* DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE: PING INTERFERENCE MITIGATION, FFT PARAMSET                                                                    */
    /* DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE: PING DC ESTIMATION, DC SUBTRACTION, INTERFERENCE STATISTICS, INTERFERENCE MITIGATION, FFT PARAMSET */
    /**********************************************************************************************************************************************************/
    {
        pingHwParamsetIdx++;
        if (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
        {
            if (rangeProcObj->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
            {
                /* At a HWA trigger time adc samples are already in HWA memory */
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_HARDWARE;
                hwaParamCfg.triggerSrc = rangeProcObj->hwaCfg.hardwareTrigSrc;
            }
            else
            {
                /* adcbuf not mapped, HWA is triggered after edma copy is done */
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
                hwaParamCfg.triggerSrc = pingHwParamsetIdx;
            }
        }
        else
        {
            /* Only in DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE mode, 
             * this param is hardware triggered. */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;
        }

        /* PREPROC CONFIG */
        if (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
        {
            /* In DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE, DC estimation, DC Subtration
             * and Interference Statistics is done in this paramset. DC estimates and Interference statistics 
             * are to used in next chirps, while that of previous chirps in software populated registers is used here.*/
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_ENABLE;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCSW;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfStat.resetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET;
        }
        else
        {
            /* In rangeProcChain DPU_RANGEPROCHWA_DEFAULT_MODE and DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE, 
             * DC estimation, DC Subtration and Interference Statistics is done in the pevious paramsets. */
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE; /* Already done in previous paramset */
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfStat.resetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
        }
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdEnable = HWA_FEATURE_BIT_ENABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdMode = HWA_INTERFTHRESH_MODE_MAGDIFF;
        if(rangeProcObj->isReal2XEnabled)
        {
            /* In Real2X mode, a single interference threshold average is used for all the Rx channels. 
             * This is because due to Real2X mode, if there are 4 Rx channels, only 2 interference thresholds 
             * are obtained, hence their average is considered. */
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdSelect = HWA_INTERFTHRESH_SELECT_EST_AVERAGE;
        }
        else if(rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
        {
            /* In this rangeProcChain, SW programmed thresholds are used, which in turn are obtained from previous runs of this paramset */
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdSelect = HWA_INTERFTHRESH_SELECT_SW;
        }
        else
        {
            /* In other cases, a per channel interference threshold from internal registers is used. */
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfLocalize.thresholdSelect = HWA_INTERFTHRESH_SELECT_EST_INDIVIDUAL;
        }

        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfMitigation.enable = HWA_FEATURE_BIT_ENABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfMitigation.countThreshold = 1;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfMitigation.pathSelect = HWA_INTERFMITIGATION_PATH_WINDOWZEROOUT;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfMitigation.leftHystOrder = 3;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.interfMitigation.rightHystOrder = 3;

        /* ACCELMODE CONFIG (FFT) */
        hwaParamCfg.accelModeArgs.fftMode.fftEn = HWA_FEATURE_BIT_ENABLE;
        if ((pDPParams->numFFTBins % 3U) == 0U)
        {
            hwaParamCfg.accelModeArgs.fftMode.fftSize = (uint8_t)mathUtils_ceilLog2((uint32_t)pDPParams->numFFTBins / 3U);
            hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_ENABLE;
        }
        else
        {
            hwaParamCfg.accelModeArgs.fftMode.fftSize = (uint8_t)mathUtils_ceilLog2(pDPParams->numFFTBins);
            hwaParamCfg.accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_DISABLE;
        }

        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 7; /* 1 << 3 - 1 */
        hwaParamCfg.accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE;
        hwaParamCfg.accelModeArgs.fftMode.windowStart = rangeProcObj->hwaCfg.hwaWinRamOffset;
        hwaParamCfg.accelModeArgs.fftMode.winSymm = rangeProcObj->hwaCfg.hwaWinSym;

        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        /* SOURCE CONFIG */
        if(rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
        {
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_DCEST_PING_IN;
        }
        else
        {
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_FFT_PING_IN;
        }

        hwaParamCfg.source.srcAcnt = pDPParams->numAdcSamples - 1U; /* this is samples - 1 */
        hwaParamCfg.source.srcAIdx = (int32_t) pDPParams->numRxAntennas * (int32_t) pDPParams->sizeOfInputSample;
        hwaParamCfg.source.srcBcnt = (uint16_t)pDPParams->numRxAntennas - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t) pDPParams->sizeOfInputSample;

        if ((bool)pDPParams->isReal)
        {
            hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL;
        }
        else
        {
            hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        }
        hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
        hwaParamCfg.source.srcConjugate = 0;
        hwaParamCfg.source.srcScale = 8;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_FFT_PING_OUT;

        hwaParamCfg.dest.dstAcnt = pDPParams->numRangeBins - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t) pDPParams->numRxAntennas * (int32_t) sizeof(cmplx16ImRe_t);
        hwaParamCfg.dest.dstBIdx = (int32_t) sizeof(cmplx16ImRe_t);

        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = rangeProcObj->fftOutputScaling;
        hwaParamCfg.dest.dstSkipInit = 0;

        errCode = HWA_configParamSet(hwaHandle,
                                    pingHwParamsetIdx,
                                    &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    /**********************************************************************************************************************************************************/
    /* DPU_RANGEPROCHWA_DEFAULT_MODE: PONG INTERFERENCE MITIGATION, FFT PARAMSET                                                                              */
    /* DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE: PONG INTERFERENCE MITIGATION, FFT PARAMSET                                                                    */
    /* DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE: PONG DC ESTIMATION, DC SUBTRACTION, INTERFERENCE STATISTICS, INTERFERENCE MITIGATION, FFT PARAMSET */
    /**********************************************************************************************************************************************************/
    {
        pongHwParamsetIdx++;
        if(rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
        {
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_DCEST_PONG_IN;
        }
        else
        {
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_FFT_PONG_IN;
        }
        hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_FFT_PONG_OUT;

        if (rangeProcObj->hwaCfg.dataInputMode != DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
        {
            hwaParamCfg.triggerSrc = pongHwParamsetIdx;
        }

        errCode = HWA_configParamSet(hwaHandle,
                                    pongHwParamsetIdx,
                                    &hwaParamCfg, NULL);
        if (errCode != 0)
        {
            goto exit;
        }
    }
    (void)memset(&hwaParamCfg, 0, sizeof(hwaParamCfg));
    /*****************************/
    /* PING COMPRESSION PARAMSET */
    /*****************************/
    {
        pingHwParamsetIdx++;

        if (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
        {
            /* once ping chirp result is copied out, signature channel triggers the paramset. */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            hwaParamCfg.triggerSrc = pingHwParamsetIdx;
        }
        else
        {
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        }

        hwaParamCfg.accelMode = HWA_ACCELMODE_COMPRESS;

        /* ACCELMODE CONFIG */
#if defined(DATAPATH_TEST) || defined(OBJ_DETECTION_DDMA_TEST)
        hwaParamCfg.accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_DISABLE; // Disable dither for datapath bit-exact test
#else
        hwaParamCfg.accelModeArgs.compressMode.ditherEnable = HWA_FEATURE_BIT_ENABLE; // Enable dither to suppress quantization spurs
#endif
        hwaParamCfg.accelModeArgs.compressMode.compressDecompress = HWA_CMP_DCMP_COMPRESS;
        hwaParamCfg.accelModeArgs.compressMode.method = pDPCompParams->compressionMethod;
        hwaParamCfg.accelModeArgs.compressMode.passSelect = HWA_COMPRESS_PATHSELECT_BOTHPASSES;
        hwaParamCfg.accelModeArgs.compressMode.headerEnable = HWA_FEATURE_BIT_ENABLE;
        hwaParamCfg.accelModeArgs.compressMode.scaleFactorBW = 4; // log2(sample bits)

        /* SRC CONFIG */
        hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_COMP_PING_IN;

        hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
        hwaParamCfg.source.srcConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.source.srcScale = 8;

        /* DEST CONFIG */
        hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_COMP_PING_OUT;
        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; /* 16 bit real, 16 bit imag */
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_UNSIGNED;
        hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.dest.dstScale = 0;
        hwaParamCfg.dest.dstSkipInit = 0;

        if (pDPCompParams->compressionMethod == HWA_COMPRESS_METHOD_BFP)
        {
            /*************************************/
            /* PING BFP COMPRESSION PARAMSET RX1 */
            /*************************************/
            hwaParamCfg.accelModeArgs.compressMode.BFPMantissaBW = (uint8_t) (((double)pDPCompParams->outputBytesPerBlock * 8.0) - 
                                                                                    (double)hwaParamCfg.accelModeArgs.compressMode.scaleFactorBW) / 
                                                                                    ((double)pDPCompParams->inputSamplesPerBlock * 2.0);
            /* SRC CONFIG */
            hwaParamCfg.source.srcAcnt = pDPCompParams->inputSamplesPerBlock - 1U;
            hwaParamCfg.source.srcAIdx = (int32_t) pDPCompParams->bytesPerSample * (int32_t) pDPCompParams->rxAntPerBlock;
            hwaParamCfg.source.srcBcnt = (pDPCompParams->numBlocks / pDPCompParams->rxAntPerBlock) - 1U;
            hwaParamCfg.source.srcBIdx = (int32_t) pDPCompParams->inputBytesPerBlock * (int32_t) pDPCompParams->rxAntPerBlock;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAcnt = pDPCompParams->outputSamplesPerBlock - 1U;
            hwaParamCfg.dest.dstAIdx = (int32_t) pDPCompParams->bytesPerSample;
            hwaParamCfg.dest.dstBIdx = (int32_t) pDPCompParams->outputBytesPerBlock * (int32_t) pDPCompParams->rxAntPerBlock;

            errCode = HWA_configParamSet(hwaHandle,
                                         pingHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            /*****************************/
            /* PONG BFP COMPRESSION PARAMSET RX1 */
            /*****************************/
            pongHwParamsetIdx++;
            if (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
            {
                /* once pong chirp result is copied out, signature channel triggers the paramset. */
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
                hwaParamCfg.triggerSrc = pongHwParamsetIdx;
            }
            else
            {
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            }
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_COMP_PONG_IN;
            hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_COMP_PONG_OUT;

            errCode = HWA_configParamSet(hwaHandle,
                                         pongHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            for (rxAntIdx = 1; rxAntIdx < pDPCompParams->rxAntPerBlock; rxAntIdx++)
            {
                /*************************************/
                /* PING BFP COMPRESSION PARAMSET RXN */
                /*************************************/
                pingHwParamsetIdx++;
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
                hwaParamCfg.source.srcAddr = (uint32_t) DPU_RANGEHWADDMA_ADDR_COMP_PING_IN + ((uint32_t) rxAntIdx * (uint32_t) pDPCompParams->bytesPerSample);
                hwaParamCfg.dest.dstAddr = (uint32_t) DPU_RANGEHWADDMA_ADDR_COMP_PING_OUT + ((uint32_t) rxAntIdx * (uint32_t) pDPCompParams->outputBytesPerBlock);
                errCode = HWA_configParamSet(hwaHandle,
                                             pingHwParamsetIdx,
                                             &hwaParamCfg, NULL);
                if (errCode != 0)
                {
                    goto exit;
                }

                /*************************************/
                /* PONG BFP COMPRESSION PARAMSET RXN */
                /*************************************/
                pongHwParamsetIdx++;
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
                hwaParamCfg.source.srcAddr = (uint32_t) DPU_RANGEHWADDMA_ADDR_COMP_PONG_IN + ((uint32_t) rxAntIdx * (uint32_t) pDPCompParams->bytesPerSample);
                hwaParamCfg.dest.dstAddr = (uint32_t) DPU_RANGEHWADDMA_ADDR_COMP_PONG_OUT + ((uint32_t) rxAntIdx * (uint32_t) pDPCompParams->outputBytesPerBlock);
                errCode = HWA_configParamSet(hwaHandle,
                                             pongHwParamsetIdx,
                                             &hwaParamCfg, NULL);
                if (errCode != 0)
                {
                    goto exit;
                }
            }
        }

        else if (pDPCompParams->compressionMethod == HWA_COMPRESS_METHOD_EGE)
        {
            /*********************************/
            /* PING EGE COMPRESSION PARAMSET */
            /*********************************/
            hwaParamCfg.accelModeArgs.compressMode.EGEKarrayLength = 3; // log2(8)

            /* SRC CONFIG */
            hwaParamCfg.source.srcAcnt = pDPCompParams->inputSamplesPerBlock - 1U;
            hwaParamCfg.source.srcAIdx = (int32_t) pDPCompParams->bytesPerSample;
            hwaParamCfg.source.srcBcnt = pDPCompParams->numBlocks - 1U;
            hwaParamCfg.source.srcBIdx = (int32_t) pDPCompParams->inputBytesPerBlock;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAcnt = pDPCompParams->outputSamplesPerBlock - 1U;
            hwaParamCfg.dest.dstAIdx = (int32_t) pDPCompParams->bytesPerSample;
            hwaParamCfg.dest.dstBIdx = (int32_t) pDPCompParams->outputBytesPerBlock;

            errCode = HWA_configParamSet(hwaHandle,
                                         pingHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            /*********************************/
            /* PONG EGE COMPRESSION PARAMSET */
            /*********************************/

            pongHwParamsetIdx++;
            if (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
            {
                /* once pong chirp result is copied out, signature channel triggers the paramset. */
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
                hwaParamCfg.triggerSrc = pongHwParamsetIdx;
            }
            else
            {
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            }
            hwaParamCfg.source.srcAddr = DPU_RANGEHWADDMA_ADDR_COMP_PONG_IN;
            hwaParamCfg.dest.dstAddr = DPU_RANGEHWADDMA_ADDR_COMP_PONG_OUT;

            errCode = HWA_configParamSet(hwaHandle,
                                         pongHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }
        }
        else
        {
            /* There is no other valid case */
            DebugP_assert(0);
        }

        /* enable the DMA hookup to this paramset so that data gets copied out */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
        paramISRConfig.dma.dstChannel = destChanPing;
        errCode = HWA_enableParamSetInterrupt(hwaHandle,
                                              pingHwParamsetIdx,
                                              &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }

        /* Enable the DMA hookup to this paramset so that data gets copied out */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
        paramISRConfig.dma.dstChannel = destChanPong;
        errCode = HWA_enableParamSetInterrupt(hwaHandle,
                                              pongHwParamsetIdx,
                                              &paramISRConfig);
        if (errCode != 0)
        {
            goto exit;
        }
    }
exit:
    return (errCode);
}

/**
 *  @b Description
 *  @n
 *      Trigger HWA for range processing.
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  rangeHwaCfg               Pointer to rangeProcHWA configuration structure
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_TriggerHWA
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProcHWA_Config*    rangeHwaCfg
)
{
    int32_t             retVal = 0;
    HWA_Handle          hwaHandle;
    uint8_t             saveRestoreDataSize;

    /* Get HWA driver handle */
    hwaHandle = rangeProcObj->initParms.hwaHandle;

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Configure HWA common parameters */
    retVal = rangeProcHWA_ConfigHWACommon(rangeProcObj);
    if(retVal < 0)
    {
        goto exit;
    }

    /* Enable the HWA */
    retVal = HWA_enable(hwaHandle, 1);
    if (retVal != 0)
    {
        goto exit;
    }

    if (rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
    {
        saveRestoreDataSize = rangeProcObj->params.numRxAntennas * 4U;
        DSSHWACCRegs *ctrlBaseAddr = (DSSHWACCRegs *)(((HWA_Object *)rangeProcObj->initParms.hwaHandle)->hwAttrs->ctrlBaseAddr);

        /* Load First Chirp DC and Interference Estimates */
        rangeProcHWA_loadPreProcStats(rangeHwaCfg, (uint32_t)saveRestoreDataSize, (uint32_t)saveRestoreDataSize, 0U);

        /* SW trigger to HWA */
        CSL_FINSR(ctrlBaseAddr->FW2HWA_TRIG_0, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START, 1U);
    }

    /* Trigger the HWA paramset for Ping */
    retVal = HWA_setDMA2ACCManualTrig(hwaHandle, rangeProcObj->dataOutTrigger[0]);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Trigger the HWA paramset for Pong */
    retVal = HWA_setDMA2ACCManualTrig(hwaHandle, rangeProcObj->dataOutTrigger[1]);
    if (retVal != 0)
    {
        goto exit;
    }

exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      EDMA configuration for rangeProc data output in interleave mode
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  DPParams                  Pointer to datapath parameter
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *  @param[in]  hwaInPingOffset           Ping HWA memory Input address offset
 *  @param[in]  hwaInPongOffset           Pong HWA memory Input address offset
 *  @param[in]  hwaOutPingOffset          Ping HWA memory Output address offset
 *  @param[in]  hwaOutPongOffset          Pong HWA memory Output address offset
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_DataOut_interleave
(
    rangeProcHWAObj     *rangeProcObj,
    rangeProc_dpParams  *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t            hwaInPingOffset,
    uint32_t            hwaInPongOffset,
    uint32_t            hwaOutPingOffset,
    uint32_t            hwaOutPongOffset
)
{
    int32_t             errorCode = SystemP_SUCCESS;
    EDMA_Handle         handle ;
    DPEDMA_ChainingCfg  chainingCfg;
    uint16_t            pong_offset=1;

    /* Get rangeProc hardware resources pointer */
    handle = rangeProcObj->edmaHandle;

    /* Setup Chaining configuration */
    chainingCfg.chainingChannel = (uint8_t) pHwConfig->edmaOutCfg.dataOutSignature.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled = true;

    if(rangeProcObj->compressionCfg.isEnabled)
    {
        errorCode = rangeProcHWA_ConfigEDMATransposeCompressed(DPParams,
                                            &rangeProcObj->compressionCfg,
                                            handle,
                                            &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                                            &chainingCfg,
                                            hwaInPingOffset,
                                            (uint32_t)rangeProcObj->radarCubebuf,
                                            false,  /* isTransferCompletionEnabled */
                                            NULL,   /* transferCompletionCallbackFxn */
                                            NULL,
                                            pHwConfig->edmaTransferCompleteIntrObj);
    }
    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }
    if(rangeProcObj->compressionCfg.isEnabled)
    {

        if(rangeProcObj->compressionCfg.compressionMethod == HWA_COMPRESS_METHOD_BFP)
        {
            pong_offset = rangeProcObj->compressionCfg.rxAntPerBlock;
        }

        errorCode = rangeProcHWA_ConfigEDMATransposeCompressed(DPParams,
                                            &rangeProcObj->compressionCfg,
                                            handle,
                                            &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                                            &chainingCfg,
                                            hwaInPongOffset,
                                            (uint32_t)(rangeProcObj->radarCubebuf + (rangeProcObj->compressionCfg.outputSamplesPerBlock * pong_offset)),
                                            true,
                                            rangeProcHWA_EDMA_transferCompletionCallbackFxn,
                                            (void *)rangeProcObj,
                                            pHwConfig->edmaTransferCompleteIntrObj);
    }
    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

     /**************************************************************************
      *  HWA hot signature EDMA, chained to the transpose EDMA channels
      *************************************************************************/
    errorCode = DPEDMAHWA_configTwoHotSignature(handle,
                                                  &pHwConfig->edmaOutCfg.dataOutSignature,
                                                  rangeProcObj->initParms.hwaHandle,
                                                  rangeProcObj->dataOutTrigger[0],
                                                  rangeProcObj->dataOutTrigger[1],
                                                  false);
    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

exit:
    return(errorCode);
}


/**
 *  @b Description
 *  @n
 *      EDMA configuration for rangeProc data in when EDMA is used to copy data from
 *  ADCBuf to HWA memory
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object handle
 *  @param[in]  DPParams                  Pointer to datapath parameter
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_DataIn
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
)
{
    int32_t             errorCode = SystemP_SUCCESS;
    EDMA_Handle         handle ;
    uint16_t            bytePerRxChan;
    DPEDMA_ChainingCfg  chainingCfg;

    /* Get rangeProc Configuration */
    handle = rangeProcObj->edmaHandle;

    bytePerRxChan = DPParams->numAdcSamples * DPParams->sizeOfInputSample;

    /**********************************************/
    /* ADCBuf -> Ping/Pong Buffer(M0 and M1)           */
    /**********************************************/
    chainingCfg.chainingChannel = (uint8_t)pHwConfig->edmaInCfg.dataInSignature.channel;
    chainingCfg.isFinalChainingEnabled = true;
    chainingCfg.isIntermediateChainingEnabled = true;

    if (rangeProcObj->interleave == DPIF_RXCHAN_NON_INTERLEAVE_MODE)
    {
        DPEDMA_syncABCfg    syncABCfg;

        syncABCfg.srcAddress = (uint32_t)rangeProcObj->ADCdataBuf;
        syncABCfg.destAddress = rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCEST_PING_IN];

        syncABCfg.aCount = bytePerRxChan;
        syncABCfg.bCount = DPParams->numRxAntennas;
        syncABCfg.cCount = 2U; /* ping and pong */

        syncABCfg.srcBIdx = (int32_t) rangeProcObj->rxChanOffset;
        syncABCfg.dstBIdx = (int32_t) rangeProcObj->rxChanOffset;
        syncABCfg.srcCIdx = 0;
        syncABCfg.dstCIdx = (int16_t)((int32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCEST_PONG_IN] - 
                                      (int32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCEST_PING_IN]);

        errorCode = DPEDMA_configSyncAB(handle,
                                        &pHwConfig->edmaInCfg.dataIn,
                                        &chainingCfg,
                                        &syncABCfg,
                                        true,    /* isEventTriggered */
                                        /* Intermediate and Final transfer interrupts are enabled in case
                                         * the user wants to poll the IPR(H) register (for example,
                                         * in the range proc test case) or register an ISR for when a chirp
                                         * transfer from to HWA memory is complete */
                                        true,   /* isIntermediateTransferInterruptEnabled */
                                        true,   /*isFinalTransferInterruptEnabled */
                                        NULL,
                                        NULL,
                                        pHwConfig->edmaTransferCompleteIntrObj);
    }
    else
    {
        DPEDMA_syncACfg    syncACfg;

        syncACfg.srcAddress = (uint32_t)rangeProcObj->ADCdataBuf;
        syncACfg.destAddress = rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCEST_PING_IN];
        syncACfg.aCount = bytePerRxChan * DPParams->numRxAntennas;
        syncACfg.bCount = 2U; /* ping and pong */
        syncACfg.dstBIdx = ((int32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCEST_PONG_IN] - 
                            (int32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_DCEST_PING_IN]);
        syncACfg.srcBIdx = 0;

        errorCode = DPEDMA_configSyncA_singleFrame(handle,
                                        &pHwConfig->edmaInCfg.dataIn,
                                        &chainingCfg,
                                        &syncACfg,
                                        /* Intermediate and Final transfer interrupts are enabled in case
                                         * the user wants to poll the IPR(H) register (for example,
                                         * in the range proc test case) or register an ISR for when a chirp
                                         * transfer from to HWA memory is complete */
                                        true,    /* isEventTriggered */
                                        true,   /* isIntermediateTransferInterruptEnabled */
                                        true,   /* isFinalTransferInterruptEnabled */
                                        NULL,
                                        NULL,
                                        pHwConfig->edmaTransferCompleteIntrObj);
    }

    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

    /*************************************************/
    /* Generate Hot Signature to trigger Ping/Pong paramset   */
    /*************************************************/

    errorCode = DPEDMAHWA_configTwoHotSignature(handle,
                                                  &pHwConfig->edmaInCfg.dataInSignature,
                                                  rangeProcObj->initParms.hwaHandle,
                                                  rangeProcObj->dataInTrigger[0],
                                                  rangeProcObj->dataInTrigger[1],
                                                  false);

    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }
exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      rangeProc configuration in interleaved mode
 *
 *  @param[in]  rangeProcObj                 Pointer to rangeProc object
 *  @param[in]  DPParams                     Pointer to data path common params
 *  @param[in]  pHwConfig                    Pointer to rangeProc hardware resources
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigInterleaveMode
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
)
{
    int32_t             retVal = 0;
    uint8_t             destChanPing;
    uint8_t             destChanPong;
    HWA_Handle          hwaHandle;

    hwaHandle = rangeProcObj->initParms.hwaHandle;

    /* In interleave mode, only edmaOutCfgFmt is supported */
    retVal = HWA_getDMAChanIndex(hwaHandle, (uint8_t)pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channel, &destChanPing);
    if (retVal != 0)
    {
        goto exit;
    }

    /* In interleave mode, only edmaOutCfgFmt is supported */
    retVal = HWA_getDMAChanIndex(hwaHandle, (uint8_t)pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channel, &destChanPong);
    if (retVal != 0)
    {
        goto exit;
    }

    if(pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_ISOLATED)
    {
        /* Copy data from ADC buffer to HWA buffer */
        retVal = rangeProcHWA_ConfigEDMA_DataIn(rangeProcObj, DPParams, pHwConfig);

        if(retVal < 0)
        {
            goto exit;
        }
        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
            pHwConfig,
            destChanPing,
            destChanPong
        );
    }
	else if (pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_MAPPED)
    {
        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
                pHwConfig,
                destChanPing,
                destChanPong
        );
    }
    else if (pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
    {
        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
                pHwConfig,
                destChanPing,
                destChanPong
        );
    }
    else
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
    }
    if(retVal < 0)
    {
        goto exit;
    }

    /* EDMA configuration */
    retVal = rangeProcHWA_ConfigEDMA_DataOut_interleave(rangeProcObj,
                                                  DPParams,
                                                  pHwConfig,
                                                  (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_COMP_PING_OUT],
                                                  (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_COMP_PONG_OUT],
                                                  (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_FFT_PING_OUT],
                                                  (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWADDMA_MEM_BANK_FFT_PONG_OUT]);
exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Internal function to parse rangeProc configuration and save in internal rangeProc object
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  pConfigIn                 Pointer to rangeProcHWA configuration structure
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ParseConfig
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_Config  *pConfigIn
)
{
    int32_t                 retVal = 0;
    rangeProc_dpParams      *params;
    DPU_RangeProcHWA_StaticConfig   *pStaticCfg;
    rangeProcHWACompressionCfg      *compParams;
    float                           temp;

    /* Get configuration pointers */
    pStaticCfg = &pConfigIn->staticCfg;
    params    = &rangeProcObj->params;
    compParams = &rangeProcObj->compressionCfg;

    /* Save datapath parameters */
    params->numTxAntennas = pStaticCfg->numTxAntennas;
    params->numRxAntennas = pStaticCfg->ADCBufData.dataProperty.numRxAntennas;
    params->numVirtualAntennas = pStaticCfg->numVirtualAntennas;
    params->numChirpsPerChirpEvent = pStaticCfg->ADCBufData.dataProperty.numChirpsPerChirpEvent;
    params->numAdcSamples = pStaticCfg->ADCBufData.dataProperty.numAdcSamples;
    params->numRangeBins = pStaticCfg->numRangeBins;
    params->numFFTBins = pStaticCfg->numFFTBins;

    if((bool)pStaticCfg->isChirpDataReal)
    {
        params->isReal = 1;
        params->sizeOfInputSample = (uint16_t) sizeof(int16_t);
    }
    else
    {
        params->isReal = 0;
        params->sizeOfInputSample = (uint16_t) sizeof(cmplx16ImRe_t);
    }
    params->numChirpsPerFrame = pStaticCfg->numChirpsPerFrame;
    params->numDopplerChirps = pStaticCfg->numChirpsPerFrame/pStaticCfg->numTxAntennas;
    params->fftOutputDivShift = pStaticCfg->rangeFFTtuning.fftOutputDivShift;
    params->numLastButterflyStagesToScale = pStaticCfg->rangeFFTtuning.numLastButterflyStagesToScale;

    /* Save buffers */
    rangeProcObj->ADCdataBuf        = (cmplx16ImRe_t *)pStaticCfg->ADCBufData.data;

    rangeProcObj->radarCubebuf      = (cmplx16ImRe_t *)pConfigIn->hwRes.radarCube.data;

    /* Save interleave mode from ADCBuf configuraiton */
    rangeProcObj->interleave = pStaticCfg->ADCBufData.dataProperty.interleave;

    if((rangeProcObj->interleave == DPIF_RXCHAN_NON_INTERLEAVE_MODE) &&
        (rangeProcObj->params.numRxAntennas > 1U))
    {
        /* For rangeProcDPU needs rx channel has same offset from one channel to the next channel
           Use first two channel offset to calculate the BIdx for EDMA
         */
        rangeProcObj->rxChanOffset = pStaticCfg->ADCBufData.dataProperty.rxChanOffset[1] -
                                    pStaticCfg->ADCBufData.dataProperty.rxChanOffset[0];

        /* rxChanOffset should be 16 bytes aligned and should be big enough to hold numAdcSamples */
        if ((rangeProcObj->rxChanOffset < (rangeProcObj->params.numAdcSamples * rangeProcObj->params.sizeOfInputSample)) ||
            ((rangeProcObj->rxChanOffset & 0xFU) != 0U))
        {
            retVal = DPU_RANGEPROCHWA_EADCBUF_INTF;
            goto exit;
        }
    }

    /* Save RadarCube format */
    if (pConfigIn->hwRes.radarCube.datafmt == DPIF_RADARCUBE_FORMAT_2)
    {
        rangeProcObj->radarCubeLayout = rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt;
    }
    else if(pConfigIn->hwRes.radarCube.datafmt == DPIF_RADARCUBE_FORMAT_1)
    {
        rangeProcObj->radarCubeLayout = rangeProc_dataLayout_TxAnt_DOPPLER_RxAnt_RANGE;
    }
    else
    {
        retVal = DPU_RANGEPROCHWA_EINTERNAL;
        goto exit;
    }

    /* Save compression parameters */
    compParams->isEnabled = pStaticCfg->compressionCfg.isEnabled;
    compParams->compressionMethod = pStaticCfg->compressionCfg.compressionMethod;
    if(compParams->compressionMethod == HWA_COMPRESS_METHOD_BFP)
    {
        compParams->inputSamplesPerBlock = pStaticCfg->compressionCfg.rangeBinsPerBlock;
    }
    else
    {
        compParams->inputSamplesPerBlock = (pStaticCfg->compressionCfg.rangeBinsPerBlock)
                                        * (pStaticCfg->compressionCfg.numRxAntennaPerBlock);
    }
    compParams->bytesPerSample = 4; /* Complex FFT */

    compParams->inputBytesPerBlock = compParams->bytesPerSample * compParams->inputSamplesPerBlock;
    compParams->numBlocks = (pStaticCfg->ADCBufData.dataProperty.numRxAntennas * pStaticCfg->numRangeBins) /
                                compParams->inputSamplesPerBlock;
    temp = ((float)compParams->inputBytesPerBlock * pStaticCfg->compressionCfg.compressionRatio + 3.99F) / 4.0F;
    compParams->outputBytesPerBlock = (uint16_t)temp * 4U; /* Word aligned */

    compParams->achievedCompressionRatio = (float) compParams->outputBytesPerBlock / (float) compParams->inputBytesPerBlock;
    compParams->outputSamplesPerBlock = compParams->outputBytesPerBlock/compParams->bytesPerSample;
    compParams->rangeBinsPerBlock = pStaticCfg->compressionCfg.rangeBinsPerBlock;
    compParams->rxAntPerBlock = pStaticCfg->compressionCfg.numRxAntennaPerBlock;

    rangeProcObj->rangeProcChain = pStaticCfg->rangeProcCfg.rangeProcChain;
    rangeProcObj->isReal2XEnabled = (bool)pStaticCfg->rangeProcCfg.isReal2XEnabled;
    rangeProcObj->fftOutputScaling = pStaticCfg->rangeProcCfg.fftOutputScaling;
    if((pStaticCfg->isChirpDataReal == 0U) && (pStaticCfg->rangeProcCfg.isReal2XEnabled == 1U))
    {
        /* Real2X mode with Complex ADC Data is invalid */
        retVal = DPU_RANGEPROCHWA_EINTERNAL;
        goto exit;
    }

    /* Prepare internal hardware resouces = trigger source matchs its  paramset index */
    rangeProcObj->dataInTrigger[0]      = 1U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataInTrigger[1]      = (pConfigIn->hwRes.hwaCfg.numParamSet / 2U) + rangeProcObj->dataInTrigger[0];
    if(rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
    {
        /* Signature channel triggers the compression paramset after the EDMA out is done. */
        rangeProcObj->dataOutTrigger[0]     = 2U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
        rangeProcObj->dataOutTrigger[1]     = (pConfigIn->hwRes.hwaCfg.numParamSet / 2U) + rangeProcObj->dataOutTrigger[0];
    }
    else
    {
        /* Signature channel triggers the dummy paramset after the EDMA out is done. */
        rangeProcObj->dataOutTrigger[0]     = 0U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
        rangeProcObj->dataOutTrigger[1]     = (pConfigIn->hwRes.hwaCfg.numParamSet / 2U) + rangeProcObj->dataOutTrigger[0];
    }

    /* Save hardware resources that will be used at runtime */
    rangeProcObj->edmaHandle= pConfigIn->hwRes.edmaHandle;
    rangeProcObj->dataOutSignatureChan = pConfigIn->hwRes.edmaOutCfg.dataOutSignature.channel;
    rangeProcObj->dcRangeSigMean = pConfigIn->hwRes.dcRangeSigMean;
    rangeProcObj->dcRangeSigMeanSize = pConfigIn->hwRes.dcRangeSigMeanSize;
    (void)memcpy((void *)&rangeProcObj->hwaCfg, (void *)&pConfigIn->hwRes.hwaCfg, sizeof(DPU_RangeProcHWA_HwaConfig));

    /* DC Est shift and scale */
    rangeProcObj->dcEstShiftScaleCfg = pStaticCfg->dcEstShiftScaleCfg;

    /* Interf config */
    rangeProcObj->intfStatsMagShiftScaleCfg = pStaticCfg->intfStatsMagShiftScaleCfg;
    rangeProcObj->intfStatsMagDiffShiftScaleCfg = pStaticCfg->intfStatsMagDiffShiftScaleCfg;

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Function to program DC stats and interference threshold estimates in HWA SW register.
 *
 *  @param[in]  rangeHwaCfg                     Pointer to rangeProcHWA configuration structure
 *  @param[in]  saveRestoreDcEstDataSize        DC Estimation Bytes to load
 *  @param[in]  saveRestoreIntfStatDataSize     Interference Statistics Bytes to load
 *  @param[in]  index                           Index of the buffer array
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval     N/A
 *
 */
void rangeProcHWA_loadPreProcStats
(
    DPU_RangeProcHWA_Config*    rangeHwaCfg,
    uint32_t saveRestoreDcEstDataSize,
    uint32_t saveRestoreIntfStatDataSize,
    uint8_t index
)
{
    uint8_t chanIdx = 0;
    uint32_t intfStatOffset = (index * saveRestoreIntfStatDataSize) / 4U;
    DSSHWACCRegs *ctrlBaseAddr = (DSSHWACCRegs *)(((HWA_Object *)RangeObj.initParms.hwaHandle)->hwAttrs->ctrlBaseAddr);

    if(saveRestoreDcEstDataSize > 0U)
    {
        /* Program the DC Estimation Values */
        (void)memcpy((void*)&ctrlBaseAddr->DC_I_SW[0], (void*)(rangeHwaCfg->hwRes.dcEstIVal + ((index * saveRestoreDcEstDataSize) / 4U)), saveRestoreDcEstDataSize);
    
        if((bool)rangeHwaCfg->staticCfg.rangeProcCfg.isReal2XEnabled)
        {
            (void)memcpy((void*)&ctrlBaseAddr->DC_Q_SW[0], (void*)(rangeHwaCfg->hwRes.dcEstQVal + ((index * saveRestoreDcEstDataSize) / 4U)), saveRestoreDcEstDataSize);
        }
    }

    if(saveRestoreIntfStatDataSize > 0U)
    {
        for(chanIdx = 0; chanIdx < rangeHwaCfg->staticCfg.ADCBufData.dataProperty.numRxAntennas; chanIdx++)
        {
            if(*(rangeHwaCfg->hwRes.intfThresMagDiffVal + intfStatOffset + chanIdx) < rangeHwaCfg->staticCfg.rangeProcCfg.intfMitigMagDiffThresMinLim)
            {
                *(rangeHwaCfg->hwRes.intfThresMagDiffVal + intfStatOffset + chanIdx) = rangeHwaCfg->staticCfg.rangeProcCfg.intfMitigMagDiffThresMinLim;
            }
        }
    
        /* Program the Interf Stats MagDiff Values. Programming Mag Thresold is not required beacuse we perform only magdiff threshold based interf localization.*/
        (void)memcpy((void*)&ctrlBaseAddr->INTF_LOC_THRESH_MAGDIFF_SW[0], (void*)(rangeHwaCfg->hwRes.intfThresMagDiffVal + intfStatOffset), saveRestoreIntfStatDataSize);
    }
}

/**
 *  @b Description
 *  @n
 *      Function to keep a copy of previous chirp's DC stats and
 *      interference threshold estimates from HWA registers.
 *
 *  @param[in]  rangeHwaCfg                     Pointer to rangeProcHWA configuration structure
 *  @param[in]  saveRestoreDcEstDataSize        DC Estimation Bytes to load
 *  @param[in]  saveRestoreIntfStatDataSize     Interference Statistics Bytes to load
 *  @param[in]  index                           Index of the buffer array
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval     N/A
 *
 */
void rangeProcHWA_storePreProcStats
(
    DPU_RangeProcHWA_Config*    rangeHwaCfg,
    uint32_t saveRestoreDcEstDataSize,
    uint32_t saveRestoreIntfStatDataSize,
    uint8_t index
)
{
    DSSHWACCRegs *ctrlBaseAddr = (DSSHWACCRegs *)(((HWA_Object *)RangeObj.initParms.hwaHandle)->hwAttrs->ctrlBaseAddr);

    if(saveRestoreDcEstDataSize > 0U)
    {
        /* Read and store the DC Estimation values */
        (void)memcpy((void*)(rangeHwaCfg->hwRes.dcEstIVal + ((index * saveRestoreDcEstDataSize) / 4U)), (void*)&ctrlBaseAddr->DC_EST_I_VAL[0], saveRestoreDcEstDataSize);
        
        if((bool)rangeHwaCfg->staticCfg.rangeProcCfg.isReal2XEnabled)
        {
            (void)memcpy((void*)(rangeHwaCfg->hwRes.dcEstQVal + ((index * saveRestoreDcEstDataSize) / 4U)), (void*)&ctrlBaseAddr->DC_EST_Q_VAL[0], saveRestoreDcEstDataSize);
        }
    }

    if(saveRestoreIntfStatDataSize > 0U)
    {
        /* Read and store the Interf Stats MagDiff Values. Storing Mag Thresold is not required beacuse we perform magdiff threshold based interf localization.*/
        (void)memcpy((void*)(rangeHwaCfg->hwRes.intfThresMagDiffVal + ((index * saveRestoreIntfStatDataSize) / 4U)), (void*)&ctrlBaseAddr->INTF_LOC_THRESH_MAGDIFF_VAL[0], saveRestoreIntfStatDataSize);
    }
}


/**************************************************************************
 ************************RangeProcHWA External APIs **************************
 **************************************************************************/

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU init function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]  initParams              Pointer to DPU init parameters
 *  @param[in]  subframeCounter         subFrame index for dpu initialization
 *  @param[in]  errCode                 Pointer to errCode generates from the API
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid rangeProc handle
 *  @retval
 *      Error       - NULL
 */
DPU_RangeProcHWA_Handle DPU_RangeProcHWA_init
(
    DPU_RangeProcHWA_InitParams     *initParams,
    volatile uint8_t            subframeCounter,
    int32_t*                            errCode
)
{
    rangeProcHWAObj     *rangeProcObj = NULL;
    HWA_MemInfo         hwaMemInfo;
    uint16_t             index;
    int32_t             status = SystemP_SUCCESS;
    uint32_t            dcSubInitValues[12] = {0};

    *errCode = 0;

    if( (initParams == NULL) ||
       (initParams->hwaHandle == NULL) )
    {
        *errCode = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    DebugP_assert(subframeCounter < RL_MAX_SUBFRAMES);

    rangeProcObj = (rangeProcHWAObj*)&RangeObj;

    /* Initialize memory */
    (void)memset((void *)rangeProcObj, 0, sizeof(rangeProcHWAObj));

    (void)memcpy((void *)&rangeProcObj->initParms, (void *)initParams, sizeof(DPU_RangeProcHWA_InitParams));

    /* Set HWA bank memory address */
    *errCode =  HWA_getHWAMemInfo(initParams->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {
        goto exit;
    }

    for (index = 0; index < hwaMemInfo.numBanks; index++)
    {
        rangeProcObj->hwaMemBankAddr[index] = hwaMemInfo.baseAddress + ((uint32_t)index * hwaMemInfo.bankSize);
    }

    /* Initialize HWA DC SW programmable registers values */
    *errCode = HWA_initConfig(initParams->hwaHandle);
    if (*errCode < 0)
    {
        goto exit;
    }
    *errCode = HWA_configDCSubtractionSWValues(dcSubInitValues, dcSubInitValues);
    if (*errCode < 0)
    {
        goto exit;
    }

    /* Create semaphore for EDMA done */
    status = SemaphoreP_constructBinary(&rangeProcObj->edmaDoneSemaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_RANGEPROCHWA_ESEMA;
        goto exit;
    }

    /* Create semaphore for HWA done */
    status = SemaphoreP_constructBinary(&rangeProcObj->hwaDoneSemaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_RANGEPROCHWA_ESEMA;
        goto exit;
    }

exit:
    if(*errCode < 0)
    {
        rangeProcObj = (DPU_RangeProcHWA_Handle)NULL;
    }
    else
    {
        /* Fall through */
    }
    return ((DPU_RangeProcHWA_Handle)rangeProcObj);

}


/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU config function. It saves buffer pointer and configurations
 *  including system resources and configures HWA and EDMA for runtime range processing.
 *
 *  @pre    DPU_RangeProcHWA_init() has been called
 *
 *  @param[in]  handle                  rangeProc DPU handle
 *  @param[in]  pConfigIn               Pointer to rangeProc configuration data structure
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcHWA_config
(
    DPU_RangeProcHWA_Handle  handle,
    DPU_RangeProcHWA_Config  *pConfigIn
)
{
    rangeProcHWAObj                 *rangeProcObj;
    DPU_RangeProcHWA_StaticConfig   *pStaticCfg;
    HWA_Handle                      hwaHandle;
    int32_t                         retVal = 0;

    rangeProcObj = (rangeProcHWAObj *)handle;
    if(rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Get configuration pointers */
    pStaticCfg = &pConfigIn->staticCfg;
    hwaHandle = rangeProcObj->initParms.hwaHandle;

#if DEBUG_CHECK_PARAMS
    /* Validate params */
    if(!pConfigIn ||
      !(pConfigIn->hwRes.edmaHandle) ||
       (pConfigIn->hwRes.hwaCfg.numParamSet != (DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS_DDMA +
                                                pStaticCfg->compressionCfg.bfpCompExtraParamSets -
                                                (2U * pStaticCfg->rangeProcCfg.rangeProcChain)))
      )
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Parameter check: validate Adc data interface configuration
        Support:
            - 1 chirp per chirpEvent
            - Complex 16bit ADC data in IMRE format supported for AM273X+2243 Processing Chain
            - Real only 16bit ADC data supported for AWR294X Processing Chain
     */
    if( (pStaticCfg->ADCBufData.dataProperty.dataFmt != DPIF_DATAFORMAT_REAL16) ||
       (pStaticCfg->ADCBufData.dataProperty.numChirpsPerChirpEvent != 1U) )
    {
        retVal = DPU_RANGEPROCHWA_EADCBUF_INTF;
        goto exit;
    }

    /* Parameter check: windowing Size */
    {
        uint16_t expectedWinSize;

        if( pConfigIn->hwRes.hwaCfg.hwaWinSym == HWA_FFT_WINDOW_SYMMETRIC)
        {
            /* Only half of the windowing factor is needed for symmetric window */
            expectedWinSize = (uint16_t) ((pStaticCfg->ADCBufData.dataProperty.numAdcSamples + 1U) / 2U ) * (uint16_t) sizeof(uint32_t);
        }
        else
        {
            expectedWinSize = (uint16_t) pStaticCfg->ADCBufData.dataProperty.numAdcSamples * (uint16_t) sizeof(uint32_t);
        }

        if(pStaticCfg->windowSize != expectedWinSize)
        {
            retVal = DPU_RANGEPROCHWA_EWINDOW;
            goto exit;
        }
    }

    if(pConfigIn->hwRes.radarCube.data == NULL)
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }

#ifdef SUBSYS_MSS
    if(CSL_MEM_IS_NOT_ALIGN(pConfigIn->hwRes.radarCube.data,
                                 DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F))
#else
    if(CSL_MEM_IS_NOT_ALIGN(pConfigIn->hwRes.radarCube.data,
                                 DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP))
#endif
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }

    /* Refer to radar cube definition for FORMAT_x , the following are the only supported formats
        Following assumption is made upon radar cube FORMAT_x definition
           1. data type is complex in cmplx16ImRe_t format only
           2. It is always Format 2 for DDMA (interleaved)
     */
    if( (pConfigIn->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_2) )
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }

    /* Not supported input & output format combination */
    if ((pStaticCfg->ADCBufData.dataProperty.interleave == DPIF_RXCHAN_INTERLEAVE_MODE) &&
         (pConfigIn->hwRes.radarCube.datafmt == DPIF_RADARCUBE_FORMAT_1) )
    {
        retVal = DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }
    if (pStaticCfg->ADCBufData.dataProperty.numRxAntennas == 3U)
    {
        retVal = DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }

    /* Parameter check: Num butterfly stages to scale */
    if (pStaticCfg->rangeFFTtuning.numLastButterflyStagesToScale > mathUtils_ceilLog2(pStaticCfg->numRangeBins))
    {
        retVal = DPU_RANGEPROCHWA_EBUTTERFLYSCALE;
        goto exit;
    }
#endif

    retVal = rangeProcHWA_ParseConfig(rangeProcObj, pConfigIn);
    if (retVal < 0)
    {
        goto exit;
    }

    /* Parameter check: radarcube buffer Size */
    if (pConfigIn->hwRes.radarCube.dataSize != (((((pStaticCfg->numRangeBins * sizeof(cmplx16ImRe_t)) *
                                                    pStaticCfg->numChirpsPerFrame) *
                                                    pStaticCfg->ADCBufData.dataProperty.numRxAntennas) /
                                                    rangeProcObj->compressionCfg.inputBytesPerBlock) *
                                                    rangeProcObj->compressionCfg.outputBytesPerBlock))
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Windowing configuraiton in HWA */
    retVal = HWA_configRam(hwaHandle,
                            HWA_RAM_TYPE_WINDOW_RAM,
                            (uint8_t *)pStaticCfg->window,
                            pStaticCfg->windowSize,   /* size in bytes */
                            pConfigIn->hwRes.hwaCfg.hwaWinRamOffset * sizeof(uint32_t));
    if (retVal != 0)
    {
        goto exit;
    }

    /* Clear stats */
    rangeProcObj->numProcess = 0U;

    /* Initial configuration of rangeProc */
    retVal = rangeProcHWA_ConfigInterleaveMode(rangeProcObj, &rangeProcObj->params, &pConfigIn->hwRes);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU process function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @pre    DPU_RangeProcHWA_init() has been called
 *
 *  @param[in]  handle                  rangeProc DPU handle
 *  @param[in]  rangeHwaCfg             Pointer to rangeProcHWA configuration structure
 *  @param[in]  outParams               DPU output parameters
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcHWA_process
(
    DPU_RangeProcHWA_Handle     handle,
    DPU_RangeProcHWA_Config*    rangeHwaCfg,
    DPU_RangeProcHWA_OutParams  *outParams
)
{
    rangeProcHWAObj     *rangeProcObj;
    int32_t             retVal = 0;
    uint32_t index;
    uint8_t fftPingParam, fftPongParam;
    uint32_t chirpIdx = 0U;
    size_t saveRestoreDataSize;
    DSSHWACCRegs *ctrlBaseAddr = (DSSHWACCRegs *)(((HWA_Object *)RangeObj.initParms.hwaHandle)->hwAttrs->ctrlBaseAddr);
    uint32_t baseAddr, regionId;

    rangeProcObj = (rangeProcHWAObj *)handle;
    if ((rangeProcObj == NULL) ||
        (outParams == NULL))
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Set inProgress state */
    rangeProcObj->inProgress = true;
    outParams->endOfChirp = false;
    saveRestoreDataSize = (uint32_t)rangeProcObj->params.numRxAntennas * 4U; //bytes
    
    /* Only in DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE, 
     * software intervention in active frame is required. */
    if(rangeProcObj->rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
    {
        baseAddr = EDMA_getBaseAddr(rangeProcObj->edmaHandle);
        DebugP_assert(baseAddr != 0U);

        regionId = EDMA_getRegionId(rangeProcObj->edmaHandle);
        DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

        fftPingParam = rangeProcObj->hwaCfg.paramSetStartIdx + (uint8_t)1U;
        fftPongParam = fftPingParam + (rangeProcObj->hwaCfg.numParamSet / (uint8_t) 2U);

        for(chirpIdx = 0U; chirpIdx < rangeProcObj->params.numChirpsPerFrame; chirpIdx += 2U)
        {
            { /* PING */
            index = chirpIdx % rangeHwaCfg->staticCfg.numBandsTotal;

            if(chirpIdx != 0U)
            {
                /* Load Ping Chirp DC and Interference Estimates */
                rangeProcHWA_loadPreProcStats(rangeHwaCfg, saveRestoreDataSize, saveRestoreDataSize, (uint8_t)index);

                /* SW trigger to HWA for Ping Chirp */
                CSL_FINSR(ctrlBaseAddr->FW2HWA_TRIG_0, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START, 1U);
            }

            /* Wait for FFT paramset done interrupt from HWA - Ping */
            while(CSL_FEXTR(ctrlBaseAddr->PARAM_DONE_SET_STATUS[0], (uint32_t)fftPingParam, (uint32_t)fftPingParam) != 1U)
            {
                /* Wait */
            };
            CSL_FINSR(ctrlBaseAddr->PARAM_DONE_CLR[0], (uint32_t)fftPingParam, (uint32_t)fftPingParam, 1U);

            /* Store Ping Chirp DC and Interference Estimates */
            rangeProcHWA_storePreProcStats(rangeHwaCfg, saveRestoreDataSize, saveRestoreDataSize, (uint8_t)index);
            }


            { /* PONG */
            index = (chirpIdx + 1U) % rangeHwaCfg->staticCfg.numBandsTotal;

            /* Load Pong Chirp DC and Interference Estimates */
            rangeProcHWA_loadPreProcStats(rangeHwaCfg, saveRestoreDataSize, saveRestoreDataSize, (uint8_t)index);

            /* SW trigger to HWA for Pong Chirp */
            CSL_FINSR(ctrlBaseAddr->FW2HWA_TRIG_0, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_END, FW2HWA_TRIG_0_FW2HWA_TRIGGER_0_START, 1U);

            /* Wait for FFT paramset done interrupt from HWA - Pong */
            while(CSL_FEXTR(ctrlBaseAddr->PARAM_DONE_SET_STATUS[0], (uint32_t)fftPongParam, (uint32_t)fftPongParam) != 1U)
            {
                /* Wait */
            };
            CSL_FINSR(ctrlBaseAddr->PARAM_DONE_CLR[0], (uint32_t)fftPongParam, (uint32_t)fftPongParam, 1U);

            /* Store Pong Chirp DC and Interference Estimates */
            rangeProcHWA_storePreProcStats(rangeHwaCfg, saveRestoreDataSize, saveRestoreDataSize, (uint8_t)index);
            }
        }
    }

    /**********************************************/
    /* WAIT FOR HWA NUMLOOPS INTERRUPT            */
    /**********************************************/
    /* wait for the all paramSets done interrupt */
    (void)SemaphoreP_pend(&rangeProcObj->hwaDoneSemaHandle, SystemP_WAIT_FOREVER);

    /**********************************************/
    /* WAIT FOR EDMA INTERRUPT                    */
    /**********************************************/
    (void)SemaphoreP_pend(&rangeProcObj->edmaDoneSemaHandle, SystemP_WAIT_FOREVER);

    /* Range FFT is done, disable Done interrupt */
    (void)HWA_disableDoneInterrupt(rangeProcObj->initParms.hwaHandle, 0);

    /* Disable the HWA */
    retVal = HWA_enable(rangeProcObj->initParms.hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }
    /* Update stats and output parameters */
    rangeProcObj->numProcess++;

    /* Following stats is not available for rangeProcHWA */
    outParams->stats.processingTime = 0;
    outParams->stats.waitTime = 0;

    outParams->endOfChirp = true;

    /* Clear inProgress state */
    rangeProcObj->inProgress = false;

exit:

    return retVal;
}


/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU control function.
 *
 *  @pre    DPU_RangeProcHWA_init() has been called
 *
 *  @param[in]  handle           rangeProc DPU handle
 *  @param[in]  rangeHwaCfg      Pointer to rangeProcHWA configuration structure
 *  @param[in]  cmd              rangeProc DPU control command
 *  @param[in]  arg              rangeProc DPU control argument pointer
 *  @param[in]  argSize          rangeProc DPU control argument size
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcHWA_control
(
    DPU_RangeProcHWA_Handle handle,
    DPU_RangeProcHWA_Config* rangeHwaCfg,
    DPU_RangeProcHWA_Cmd    cmd,
    void*                   arg,
    uint32_t                argSize
)
{
    int32_t             retVal = 0;
    rangeProcHWAObj     *rangeProcObj;

    /* Get rangeProc data object */
    rangeProcObj = (rangeProcHWAObj *)handle;

    /* Sanity check */
    if (rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Check if control() is called during processing time */
    if(rangeProcObj->inProgress == true)
    {
        retVal = DPU_RANGEPROCHWA_EINPROGRESS;
        goto exit;
    }

    /* Control command handling */
    switch(cmd)
    {
        case DPU_RangeProcHWA_Cmd_triggerProc:
            /* Trigger rangeProc in HWA */
            retVal = rangeProcHWA_TriggerHWA(rangeProcObj, rangeHwaCfg);
            break;

        default:
            retVal = DPU_RANGEPROCHWA_ECMD;
            break;
    }
exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      The function is rangeProc DPU deinit function. It frees the resources used for the DPU.
 *
 *  @pre    DPU_RangeProcHWA_init() has been called
 *
 *  @param[in]  handle           rangeProc DPU handle
 *
 *  \ingroup    DPU_RANGEPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeProcHWA_deinit
(
    DPU_RangeProcHWA_Handle     handle
)
{
    rangeProcHWAObj     *rangeProcObj;
    int32_t             retVal = 0;

    /* Sanity Check */
    rangeProcObj = (rangeProcHWAObj *)handle;
    if(rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Delete Semaphores */
    SemaphoreP_destruct(&rangeProcObj->edmaDoneSemaHandle);
    SemaphoreP_destruct(&rangeProcObj->hwaDoneSemaHandle);
exit:

    return (retVal);
}
