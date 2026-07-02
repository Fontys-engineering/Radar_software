/*
 *  NOTE:
 *      (C) Copyright 2023 Texas Instruments, Inc.
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
 *   @file  rangeprochwaReal2x.c
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

/* HWA_SOC Include files */
#include <drivers/soc.h>

/* Data Path Include files */
#include <ti/datapath/dpu/rangeprocReal2x/rangeprochwaReal2x.h>

/* MATH utils library Include files */
#include <ti/utils/mathutils/mathutils.h>

/* Internal include Files */
#include <ti/datapath/dpu/rangeprocReal2x/include/rangeprochwaReal2x_internal.h>

#include <ti/control/mmwavelink/mmwavelink.h>

rangeProcHWAObj RangeObj[RL_MAX_SUBFRAMES] __attribute__((aligned(HeapP_BYTE_ALIGNMENT)));

/* Flag to check input parameters */
#define DEBUG_CHECK_PARAMS   1

#define DCEST_PING_HWA_PARAMSET_RELATIVE_IDX        1
#define DCSUB_PING_HWA_PARAMSET_RELATIVE_IDX        2
#define FFT_PING_HWA_PARAMSET_RELATIVE_IDX          3
#define COMPRESS_PING_HWA_PARAMSET_RELATIVE_IDX     4

#define DPU_RANGEHWAREAL2X_MEM_BANK_DCEST_PING_IN     0
#define DPU_RANGEHWAREAL2X_MEM_BANK_DCSUB_PING_OUT    2
#define DPU_RANGEHWAREAL2X_MEM_BANK_FFT_PING_OUT      0
#define DPU_RANGEHWAREAL2X_MEM_BANK_COMP_PING_OUT     2

#define DPU_RANGEHWAREAL2X_MEM_BANK_DCEST_PONG_IN     1
#define DPU_RANGEHWAREAL2X_MEM_BANK_DCSUB_PONG_OUT    3
#define DPU_RANGEHWAREAL2X_MEM_BANK_FFT_PONG_OUT      1
#define DPU_RANGEHWAREAL2X_MEM_BANK_COMP_PONG_OUT     3

#define DPU_RANGEHWAREAL2X_ADDR_DCEST_PING_IN     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_DCEST_PING_IN])
/* DPU_RANGEHWAREAL2X_ADDR_DCEST_PING_OUT is not required */
#define DPU_RANGEHWAREAL2X_ADDR_DCSUB_PING_IN     DPU_RANGEHWAREAL2X_ADDR_DCEST_PING_IN
#define DPU_RANGEHWAREAL2X_ADDR_DCSUB_PING_OUT    HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_DCSUB_PING_OUT])
#define DPU_RANGEHWAREAL2X_ADDR_FFT_PING_IN       DPU_RANGEHWAREAL2X_ADDR_DCSUB_PING_OUT
#define DPU_RANGEHWAREAL2X_ADDR_FFT_PING_OUT      HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_FFT_PING_OUT])
#define DPU_RANGEHWAREAL2X_ADDR_COMP_PING_IN      DPU_RANGEHWAREAL2X_ADDR_FFT_PING_OUT
#define DPU_RANGEHWAREAL2X_ADDR_COMP_PING_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_COMP_PING_OUT])

#define DPU_RANGEHWAREAL2X_ADDR_DCEST_PONG_IN     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_DCEST_PONG_IN])
/* DPU_RANGEHWAREAL2X_ADDR_DCEST_PONG_OUT is not required */
#define DPU_RANGEHWAREAL2X_ADDR_DCSUB_PONG_IN     DPU_RANGEHWAREAL2X_ADDR_DCEST_PONG_IN
#define DPU_RANGEHWAREAL2X_ADDR_DCSUB_PONG_OUT    HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_DCSUB_PONG_OUT])
#define DPU_RANGEHWAREAL2X_ADDR_FFT_PONG_IN       DPU_RANGEHWAREAL2X_ADDR_DCSUB_PONG_OUT
#define DPU_RANGEHWAREAL2X_ADDR_FFT_PONG_OUT      HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_FFT_PONG_OUT])
#define DPU_RANGEHWAREAL2X_ADDR_COMP_PONG_IN      DPU_RANGEHWAREAL2X_ADDR_FFT_PONG_OUT
#define DPU_RANGEHWAREAL2X_ADDR_COMP_PONG_OUT     HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_COMP_PONG_OUT])


/**************************************************************************
 ************************ Internal Functions Prototype       **********************
 **************************************************************************/
static void rangeProcHWADoneIsrCallback(uint32_t threadIdx, void * arg);

static int32_t rangeProcHWA_ConfigHWA
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
);

static int32_t rangeProcHWA_TriggerHWA
(
    rangeProcHWAObj     *rangeProcObj
);
static int32_t rangeProcHWA_ConfigEDMA_DataOut
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
);

static int32_t rangeProcHWA_ConfigEDMA_CompCubeOut
(
    rangeProcHWAObj         *rangeProcHWAObj,
    DPU_RangeProcHWA_Config *pConfigIn,
    DPEDMA_3LinkChanCfg     *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    uint32_t                srcAddress,
    uint32_t                destAddress,
    bool                    isTransferCompletionEnabled,
    Edma_EventCallback      transferCompletionCallbackFxn,
    void*                   transferCompletionCallbackFxnArg,
    Edma_IntrObject         *intrObj
);

static int32_t rangeProcHWA_ConfigEDMA_PayloadHeader
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
);
static int32_t rangeProcHWA_ConfigEDMA_PayloadFooter
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
);
static int32_t rangeProcHWA_ConfigEDMA_DataIn
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
);
static int32_t rangeProcHWA_ConfigNonInterleaveMode
(
    rangeProcHWAObj          *rangeProcObj,
    DPU_RangeProcHWA_Config  *pConfigIn
);

/**************************************************************************
 **************************** Extern Declarations *************************
 **************************************************************************/
extern HWA_ParamConfig gHwaParamConfig[];
extern HWA_CommonConfig gHwaCommonConfig[];
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
 *      Function to calculate the DC Estimation scale and shift
 *      to be sent to the HWA.
 *
 *  @param[in]   numSamples              number of ADC samples
 *  @param[out]  scaleVal                scale to be fed to HWA
 *  @param[out]  shiftVal                shift value to be fed to HWA
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
uint32_t rangeProcHWAReal2x_findDCEstStaticParams(uint32_t numSamples, uint32_t * scaleVal, uint32_t * shiftVal){

    uint32_t scale_best = 0U;
    uint32_t shift_best = 6U;
    uint32_t shift, scale_curr;

    for(shift = 0U; shift <= 14U; shift++){
        scale_curr = (1U << (shift + 8U + 6U)) / numSamples;
        if((scale_curr > scale_best) && (scale_curr < (1U << 9U) - 1U)){
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
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
uint32_t rangeProcHWAReal2x_findIntfStatsStaticParams(uint32_t numSamples, uint32_t SNRdB, uint32_t * scaleVal, uint32_t * shiftVal){

    uint32_t scale_best = 0;
    uint32_t shift_best = 6;
    uint32_t shift, scale_curr;

    for(shift = 0; shift <= 6; shift++){
        scale_curr = MATHUTILS_ROUND_FLOAT((float)((pow(10, ((double)SNRdB/20.0))/numSamples*(256*2)*(1 << shift))));
        if((scale_curr > scale_best) && (scale_curr < (1 << 8) - 1)){
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
 *      Function to config a EDMA in linear format, when compression is enabled
 *
 *  @param[in]  rangeProcHWAObj         Pointer to rangeProc object
 *  @param[in]  pConfigIn               DPU Config
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
static int32_t rangeProcHWA_ConfigEDMA_CompCubeOut
(
    rangeProcHWAObj         *rangeProcHWAObj,
    DPU_RangeProcHWA_Config *pConfigIn,
    DPEDMA_3LinkChanCfg     *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    uint32_t                srcAddress,
    uint32_t                destAddress,
    bool                    isTransferCompletionEnabled,
    Edma_EventCallback      transferCompletionCallbackFxn,
    void*                   transferCompletionCallbackFxnArg,
    Edma_IntrObject         *intrObj
)
{
    int32_t                 retVal=0;
    DPEDMA_syncABCfg        syncABCfg, syncABCfg1, syncABCfg2;
    DPU_RangeProcHWA_HW_Resources *pHwConfig = &pConfigIn->hwRes;
    DPU_RangeProcHWA_StaticConfig *pStaticCfg = &pConfigIn->staticCfg;

    uint32_t baseAddr = EDMA_getBaseAddr(rangeProcHWAObj->edmaHandle);
    if (baseAddr == 0)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* dpedma configuration */
    syncABCfg.aCount = rangeProcHWAObj->compressionCfg.outputBytesPerBlock * rangeProcHWAObj->compressionCfg.numBlockPerPayload;
    syncABCfg.bCount = pStaticCfg->numPayloads;
    syncABCfg.cCount = pStaticCfg->numChirpsEachIter[0]/2U;
    syncABCfg.srcBIdx = rangeProcHWAObj->compressionCfg.outputBytesPerBlock * rangeProcHWAObj->compressionCfg.numBlockPerPayload;
    syncABCfg.srcCIdx = 0U;
    syncABCfg.dstBIdx = (rangeProcHWAObj->compressionCfg.outputBytesPerBlock * rangeProcHWAObj->compressionCfg.numBlockPerPayload) +\
                        pStaticCfg->appHeaderSize + pStaticCfg->appFooterSize;
    syncABCfg.dstCIdx = (pHwConfig->radarCube.dataSize/pStaticCfg->numChirpsPerFrame)*2U;

    syncABCfg.srcAddress = srcAddress;
    syncABCfg.destAddress= destAddress;

    syncABCfg1 = syncABCfg;
    syncABCfg1.cCount = pStaticCfg->numChirpsEachIter[1]/2U;

    syncABCfg2 = syncABCfg;
    syncABCfg2.cCount = pStaticCfg->numChirpsEachIter[2]/2U;

    retVal = DPEDMA_configSyncAB_ThreeLinks(rangeProcHWAObj->edmaHandle,
            chanCfg,
            chainingCfg,
            &syncABCfg,
            &syncABCfg1,
            &syncABCfg2,
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

    if(syncABCfg1.cCount == 0U)
    {
        /* No L3 Reuse case, only single iteration of L3. */
        /* Link main channel to the shadow channel and shadow to itself */
        EDMA_linkChannel(baseAddr, chanCfg->paramId, chanCfg->ShadowPramId[2]);
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[2], chanCfg->ShadowPramId[2]);
    }
    else if(syncABCfg2.cCount == 0U)
    {
        /* L3 Reuse two iterations */
        /* Link EDMA link1 to the shadow channel and shadow to link 1 */
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[0], chanCfg->ShadowPramId[2]);
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[2], chanCfg->ShadowPramId[0]);
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

    if (rangeProcObj == NULL)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }
    DPU_RangeProcHWA_HwaConfig *hwaCfg;
    hwaCfg = &rangeProcObj->hwaCfg;

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    gHwaCommonConfig[0].numLoops = rangeProcObj->numChirpsPerFrame/2U;

    /* DC Est shift and scale */
    gHwaCommonConfig[0].dcEstimateConfig.scale = rangeProcObj->dcEstShiftScaleCfg.scale;
    gHwaCommonConfig[0].dcEstimateConfig.shift = rangeProcObj->dcEstShiftScaleCfg.shift;

    /* Interf config */
    gHwaCommonConfig[0].interfConfig.sumMagScale = rangeProcObj->intfStatsMagShiftScaleCfg.scale;
    gHwaCommonConfig[0].interfConfig.sumMagShift = rangeProcObj->intfStatsMagShiftScaleCfg.shift;
    gHwaCommonConfig[0].interfConfig.sumMagDiffScale = rangeProcObj->intfStatsMagDiffShiftScaleCfg.scale;
    gHwaCommonConfig[0].interfConfig.sumMagDiffShift = rangeProcObj->intfStatsMagDiffShiftScaleCfg.shift;

    gHwaCommonConfig[0].interfConfig.mitigationWindowParam[0] = hwaCfg->hwaInterfMitigWindow[0];
    gHwaCommonConfig[0].interfConfig.mitigationWindowParam[1] = hwaCfg->hwaInterfMitigWindow[1];
    gHwaCommonConfig[0].interfConfig.mitigationWindowParam[2] = hwaCfg->hwaInterfMitigWindow[2];
    gHwaCommonConfig[0].interfConfig.mitigationWindowParam[3] = hwaCfg->hwaInterfMitigWindow[3];
    gHwaCommonConfig[0].interfConfig.mitigationWindowParam[4] = hwaCfg->hwaInterfMitigWindow[4];

    retVal = HWA_configCommon(rangeProcObj->initParms.hwaHandle, &gHwaCommonConfig[0]);
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
 *  @param[in]  pConfigIn                     Pointer to rangeProc config
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigHWA
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
)
{
    HWA_InterruptConfig     paramISRConfig;
    int32_t                 errCode = 0;
    uint32_t                paramsetIdx = 0;
    uint32_t                hwParamsetIdx;
    HWA_Handle              hwaHandle;
    rangeProcHWACompressionCfg  *pDPCompParams;
    uint8_t                      index;
    uint8_t                 destChanPing;
    uint8_t                 destChanPong;
    DPU_RangeProcHWA_HW_Resources *pHwConfig = &pConfigIn->hwRes;
    DPU_RangeProcHWA_StaticConfig *pStaticCfg = &pConfigIn->staticCfg;
    uint8_t numRxAntennas = pStaticCfg->ADCBufData.dataProperty.numRxAntennas;
    uint16_t numAdcSamples, sizeOfInputSample;
    hwaHandle = rangeProcObj->initParms.hwaHandle;
    pDPCompParams = &rangeProcObj->compressionCfg;

    hwParamsetIdx = rangeProcObj->hwaCfg.paramSetStartIdx;
    for(index = 0; index < rangeProcObj->hwaCfg.numParamSet; index++)
    {
        errCode = HWA_disableParamSetInterrupt(hwaHandle, index + rangeProcObj->hwaCfg.paramSetStartIdx,
                HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 |HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    if(pStaticCfg->isMode2x==1){
        /* Each real sample is treated as complex, complex input with 2x mode is not valid */
        numAdcSamples = pStaticCfg->ADCBufData.dataProperty.numAdcSamples/2U;
        sizeOfInputSample = pStaticCfg->sizeOfInputSample*2U;
    }
    else{
        /* Each real/complex sample is treated as it is */
        numAdcSamples = pStaticCfg->ADCBufData.dataProperty.numAdcSamples;
        sizeOfInputSample = pStaticCfg->sizeOfInputSample;
    }

    /***********************/
    /* PING DUMMY PARAMSET */
    /***********************/
    {
    gHwaParamConfig[paramsetIdx].triggerSrc = hwParamsetIdx;
    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }
    }

    /*******************************/
    /* PING DC ESTIMATION PARAMSET */
    /*******************************/
    {
    paramsetIdx++;
    hwParamsetIdx++;
    gHwaParamConfig[paramsetIdx].triggerSrc = hwParamsetIdx;

    if(pStaticCfg->isMode2x==1){
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.mode2X = HWA_FEATURE_BIT_ENABLE;
        gHwaParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        gHwaParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    }
    else{
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.mode2X = HWA_FEATURE_BIT_DISABLE;
        gHwaParamConfig[paramsetIdx].source.srcRealComplex = (pStaticCfg->isChirpDataReal ==1)? HWA_SAMPLES_FORMAT_REAL: HWA_SAMPLES_FORMAT_COMPLEX;
        gHwaParamConfig[paramsetIdx].dest.dstRealComplex = (pStaticCfg->isChirpDataReal ==1)? HWA_SAMPLES_FORMAT_REAL: HWA_SAMPLES_FORMAT_COMPLEX;
    }

    /* SRC CONFIG */
    gHwaParamConfig[paramsetIdx].source.srcAddr = DPU_RANGEHWAREAL2X_ADDR_DCEST_PING_IN;
    gHwaParamConfig[paramsetIdx].source.srcAcnt = numAdcSamples - 1; /* this is samples - 1 */
    gHwaParamConfig[paramsetIdx].source.srcAIdx = sizeOfInputSample;
    gHwaParamConfig[paramsetIdx].source.srcBcnt = numRxAntennas-1;
    gHwaParamConfig[paramsetIdx].source.srcBIdx = sizeOfInputSample*numAdcSamples;

    /* DEST CONFIG */
    gHwaParamConfig[paramsetIdx].dest.dstAddr = DPU_RANGEHWAREAL2X_ADDR_DCSUB_PING_OUT;
    gHwaParamConfig[paramsetIdx].dest.dstAcnt = numAdcSamples - 1; /* No FFT is being performed here */
    gHwaParamConfig[paramsetIdx].dest.dstAIdx = sizeOfInputSample;
    gHwaParamConfig[paramsetIdx].dest.dstBIdx = sizeOfInputSample*numAdcSamples;

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    }

    /********************************************************************/
    /* PING DC SUBTRACTION, INTERFERENCE STATISTICS ESTIMATION PARAMSET */
    /********************************************************************/
    {
    paramsetIdx++;
    hwParamsetIdx++;

    if(pStaticCfg->isMode2x==1){
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.mode2X = HWA_FEATURE_BIT_ENABLE;
        gHwaParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        gHwaParamConfig[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    }
    else{
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.mode2X = HWA_FEATURE_BIT_DISABLE;
        gHwaParamConfig[paramsetIdx].source.srcRealComplex = (pStaticCfg->isChirpDataReal ==1)? HWA_SAMPLES_FORMAT_REAL: HWA_SAMPLES_FORMAT_COMPLEX;
        gHwaParamConfig[paramsetIdx].dest.dstRealComplex = (pStaticCfg->isChirpDataReal ==1)? HWA_SAMPLES_FORMAT_REAL: HWA_SAMPLES_FORMAT_COMPLEX;
    }

    /* SRC CONFIG */
    gHwaParamConfig[paramsetIdx].source.srcAddr = DPU_RANGEHWAREAL2X_ADDR_DCSUB_PING_IN;
    gHwaParamConfig[paramsetIdx].source.srcAcnt = numAdcSamples - 1; /* this is samples - 1 */
    gHwaParamConfig[paramsetIdx].source.srcAIdx = sizeOfInputSample;
    gHwaParamConfig[paramsetIdx].source.srcBcnt = numRxAntennas-1;
    gHwaParamConfig[paramsetIdx].source.srcBIdx = sizeOfInputSample*numAdcSamples;

    /* DEST CONFIG */
    gHwaParamConfig[paramsetIdx].dest.dstAddr = DPU_RANGEHWAREAL2X_ADDR_DCSUB_PING_OUT;
    gHwaParamConfig[paramsetIdx].dest.dstAcnt = numAdcSamples - 1; /* No FFT is being performed here */
    gHwaParamConfig[paramsetIdx].dest.dstAIdx = sizeOfInputSample;
    gHwaParamConfig[paramsetIdx].dest.dstBIdx = sizeOfInputSample*numAdcSamples;

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }
    }

    /**********************************************/
    /* PING INTERFERENCE MITIGATION, FFT PARAMSET */
    /**********************************************/
    {
    paramsetIdx++;
    hwParamsetIdx++;

    if(pStaticCfg->isMode2x==1){
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.mode2X = HWA_FEATURE_BIT_ENABLE;
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.conjCombScaling = 2U;
        gHwaParamConfig[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.windowMode = HWA_WINDOW_MODE_16BITREAL;

    }
    else{
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.mode2X = HWA_FEATURE_BIT_DISABLE;
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.conjCombScaling = 0U;
        gHwaParamConfig[paramsetIdx].source.srcRealComplex = (pStaticCfg->isChirpDataReal ==1)? HWA_SAMPLES_FORMAT_REAL: HWA_SAMPLES_FORMAT_COMPLEX;
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.windowMode = HWA_WINDOW_MODE_18BITREAL;
    }

    /* ACCELMODE CONFIG */
    if(pStaticCfg->numFFTBins % 3 == 0){
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(pStaticCfg->numFFTBins/3);
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_ENABLE;
    }
    else{
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(pStaticCfg->numFFTBins);
        gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.fftSize3xEn = HWA_FEATURE_BIT_DISABLE;
    }

    gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.windowStart = rangeProcObj->hwaCfg.hwaWinRamOffset;
    gHwaParamConfig[paramsetIdx].accelModeArgs.fftMode.winSymm = rangeProcObj->hwaCfg.hwaWinSym;

    /* SRC CONFIG */
    gHwaParamConfig[paramsetIdx].source.srcAddr = DPU_RANGEHWAREAL2X_ADDR_FFT_PING_IN;
    gHwaParamConfig[paramsetIdx].source.srcAcnt = numAdcSamples - 1; /* this is samples - 1 */
    gHwaParamConfig[paramsetIdx].source.srcAIdx = sizeOfInputSample;
    gHwaParamConfig[paramsetIdx].source.srcBcnt = numRxAntennas-1;
    gHwaParamConfig[paramsetIdx].source.srcBIdx = sizeOfInputSample*numAdcSamples;

    /* DEST CONFIG */
    gHwaParamConfig[paramsetIdx].dest.dstAddr = DPU_RANGEHWAREAL2X_ADDR_FFT_PING_OUT;
    if(pDPCompParams->compressionMethod==HWA_COMPRESS_METHOD_BFP)
    {
        /* Store FFT output in Non-Interleaved Order */
        gHwaParamConfig[paramsetIdx].dest.dstAcnt = pStaticCfg->numRangeBins-1;
        gHwaParamConfig[paramsetIdx].dest.dstAIdx = sizeof(cmplx16ImRe_t);
        gHwaParamConfig[paramsetIdx].dest.dstBIdx = pStaticCfg->numRangeBins *sizeof(cmplx16ImRe_t);
    }
    else if(pDPCompParams->compressionMethod==HWA_COMPRESS_METHOD_EGE)
    {
        /* Store FFT output in Interleaved Order */
        gHwaParamConfig[paramsetIdx].dest.dstAcnt = pStaticCfg->numRangeBins-1;
        gHwaParamConfig[paramsetIdx].dest.dstAIdx = numRxAntennas * sizeof(cmplx16ImRe_t);
        gHwaParamConfig[paramsetIdx].dest.dstBIdx = sizeof(cmplx16ImRe_t);
    }

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }
    }

    /*****************************/
    /* PING COMPRESSION PARAMSET */
    /*****************************/
    {
    paramsetIdx++;
    hwParamsetIdx++;

    /* ACCELMODE CONFIG */
    gHwaParamConfig[paramsetIdx].accelModeArgs.compressMode.method = pDPCompParams->compressionMethod;

    if(pDPCompParams->compressionMethod==HWA_COMPRESS_METHOD_BFP)
    {
        gHwaParamConfig[paramsetIdx].accelModeArgs.compressMode.BFPMantissaBW = floor((pDPCompParams->outputBytesPerBlock*8U - \
                                                    gHwaParamConfig[paramsetIdx].accelModeArgs.compressMode.scaleFactorBW)\
                                                    /(pDPCompParams->inputSamplesPerBlock * 2U));
    }

    /* SRC CONFIG */
    gHwaParamConfig[paramsetIdx].source.srcAddr = DPU_RANGEHWAREAL2X_ADDR_COMP_PING_IN;
    gHwaParamConfig[paramsetIdx].source.srcAcnt = pDPCompParams->inputSamplesPerBlock - 1;
    gHwaParamConfig[paramsetIdx].source.srcAIdx = pDPCompParams->bytesPerSample;
    gHwaParamConfig[paramsetIdx].source.srcBcnt = pDPCompParams->numBlocks - 1;
    gHwaParamConfig[paramsetIdx].source.srcBIdx = pDPCompParams->inputBytesPerBlock;

    /* DEST CONFIG */
    gHwaParamConfig[paramsetIdx].dest.dstAddr = DPU_RANGEHWAREAL2X_ADDR_COMP_PING_OUT;
    gHwaParamConfig[paramsetIdx].dest.dstAcnt = pDPCompParams->outputSamplesPerBlock - 1;
    gHwaParamConfig[paramsetIdx].dest.dstAIdx = pDPCompParams->bytesPerSample;
    gHwaParamConfig[paramsetIdx].dest.dstBIdx = pDPCompParams->outputBytesPerBlock;

    errCode = HWA_configParamSet(hwaHandle,
                                hwParamsetIdx,
                                &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /* enable the DMA hookup to this paramset so that data gets copied out */
    errCode = HWA_getDMAChanIndex(hwaHandle, pHwConfig->edmaOutCfg.dataOutPing.channel, &destChanPing);
    if (errCode != 0)
    {
        goto exit;
    }
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
    paramISRConfig.dma.dstChannel = destChanPing;


    errCode = HWA_enableParamSetInterrupt(hwaHandle,hwParamsetIdx,&paramISRConfig);
    if (errCode != 0)
    {
        goto exit;
    }
    }

    /***********************/
    /* PONG DUMMY PARAMSET */
    /***********************/
    {
    paramsetIdx=0;
    hwParamsetIdx++;
    gHwaParamConfig[paramsetIdx].triggerSrc = hwParamsetIdx;
    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }
    }

    /*******************************/
    /* PONG DC ESTIMATION PARAMSET */
    /*******************************/
    {
    paramsetIdx++;
    hwParamsetIdx++;
    gHwaParamConfig[paramsetIdx].source.srcAddr = DPU_RANGEHWAREAL2X_ADDR_DCEST_PONG_IN;
    gHwaParamConfig[paramsetIdx].dest.dstAddr = DPU_RANGEHWAREAL2X_ADDR_DCSUB_PONG_OUT;
    gHwaParamConfig[paramsetIdx].triggerSrc = hwParamsetIdx;

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }
    }

    /********************************************************************/
    /* PONG DC SUBTRACTION, INTERFERENCE STATISTICS ESTIMATION PARAMSET */
    /********************************************************************/
    {
    paramsetIdx++;
    hwParamsetIdx++;
    gHwaParamConfig[paramsetIdx].source.srcAddr = DPU_RANGEHWAREAL2X_ADDR_DCSUB_PONG_IN;
    gHwaParamConfig[paramsetIdx].dest.dstAddr = DPU_RANGEHWAREAL2X_ADDR_DCSUB_PONG_OUT;

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }
    }

    /**********************************************/
    /* PONG INTERFERENCE MITIGATION, FFT PARAMSET */
    /**********************************************/
    {
    paramsetIdx++;
    hwParamsetIdx++;
    gHwaParamConfig[paramsetIdx].source.srcAddr = DPU_RANGEHWAREAL2X_ADDR_FFT_PONG_IN;
    gHwaParamConfig[paramsetIdx].dest.dstAddr = DPU_RANGEHWAREAL2X_ADDR_FFT_PONG_OUT;

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }
    }

    /*****************************/
    /* PONG COMPRESSION PARAMSET */
    /*****************************/
    {
    paramsetIdx++;
    hwParamsetIdx++;
    gHwaParamConfig[paramsetIdx].source.srcAddr = DPU_RANGEHWAREAL2X_ADDR_COMP_PONG_IN;
    gHwaParamConfig[paramsetIdx].dest.dstAddr = DPU_RANGEHWAREAL2X_ADDR_COMP_PONG_OUT;

    errCode = HWA_configParamSet(hwaHandle,
                                  hwParamsetIdx,
                                  &gHwaParamConfig[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }
    }
    /* only edmaOutCfgFmt is supported */
    errCode = HWA_getDMAChanIndex(hwaHandle, pHwConfig->edmaOutCfg.dataOutPong.channel, &destChanPong);
    if (errCode != 0)
    {
        goto exit;
    }
    /* Enable the DMA hookup to this paramset so that data gets copied out */
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
    paramISRConfig.dma.dstChannel = destChanPong;
    errCode = HWA_enableParamSetInterrupt(hwaHandle,
                                           hwParamsetIdx,
                                           &paramISRConfig);
    if (errCode != 0)
    {
        goto exit;
    }
exit:
    return(errCode);
}

/**
 *  @b Description
 *  @n
 *      Trigger HWA for range processing.
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
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
    rangeProcHWAObj     *rangeProcObj
)
{
    int32_t             retVal = 0;
    HWA_Handle          hwaHandle;

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
 *      EDMA configuration for payload header to be placed from bookkeep registers
 *      in L3 before the start of every data packet.
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  pConfigIn                 Pointer to rangeProc config
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_PayloadHeader
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
)
{
    int32_t             errorCode = SystemP_SUCCESS;
    EDMA_Handle         handle;
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncABCfg    syncABCfg, syncABCfg1, syncABCfg2;
    uint32_t            compDataSizePerPayload;
    DPU_RangeProcHWA_HW_Resources *pHwConfig = &pConfigIn->hwRes;
    DPU_RangeProcHWA_StaticConfig *staticCfg = &pConfigIn->staticCfg;

    /* Get rangeProc Configuration */
    handle = rangeProcObj->edmaHandle;
    compDataSizePerPayload = rangeProcObj->compressionCfg.outputBytesPerBlock * rangeProcObj->compressionCfg.numBlockPerPayload;

    /********************************/
    /* RSS BookKeep Registers -> L3 */
    /********************************/
    chainingCfg.chainingChannel = pHwConfig->edmaPacketCfg.appHeader.channel;
    chainingCfg.isFinalChainingEnabled = false;
    chainingCfg.isIntermediateChainingEnabled = false;

    syncABCfg.srcAddress = (uint32_t)(CSL_RSS_CTRL_U_BASE+CSL_RSS_CTRL_RSS_APP_GP);
    syncABCfg.destAddress = (uint32_t)rangeProcObj->radarCubebuf;
    syncABCfg.aCount = staticCfg->appHeaderSize;
    syncABCfg.bCount = staticCfg->numPayloads;
    syncABCfg.cCount = staticCfg->numChirpsEachIter[0];
    syncABCfg.srcBIdx = 0U;
    syncABCfg.srcCIdx = 0U;
    syncABCfg.dstBIdx = compDataSizePerPayload +\
                        staticCfg->appHeaderSize + staticCfg->appFooterSize;
    syncABCfg.dstCIdx = pHwConfig->radarCube.dataSize/staticCfg->numChirpsPerFrame;

    syncABCfg1 = syncABCfg;
    syncABCfg1.cCount = staticCfg->numChirpsEachIter[1];
    syncABCfg2 = syncABCfg;
    syncABCfg2.cCount = staticCfg->numChirpsEachIter[2];

    errorCode = DPEDMA_configSyncAB_ThreeLinks(handle,
                                    &pHwConfig->edmaPacketCfg.appHeader,
                                    &chainingCfg,
                                    &syncABCfg,
                                    &syncABCfg1,
                                    &syncABCfg2,
                                    true,   /* isEventTriggered */
                                    false,   /* isIntermediateTransferInterruptEnabled */
                                    false,   /* isFinalTransferInterruptEnabled */
                                    NULL,
                                    NULL,
                                    NULL);

    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

    uint32_t baseAddr = EDMA_getBaseAddr(handle);
    if (baseAddr == 0)
    {
        errorCode = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }
    DPEDMA_3LinkChanCfg     *chanCfg = &pHwConfig->edmaPacketCfg.appHeader;
    if(syncABCfg1.cCount == 0U)
    {
        /* No L3 Reuse case, only single iteration of L3. */
        /* Link main channel to the shadow channel and shadow to itself */
        EDMA_linkChannel(baseAddr, chanCfg->paramId, chanCfg->ShadowPramId[2]);
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[2], chanCfg->ShadowPramId[2]);
    }
    else if(syncABCfg2.cCount == 0U)
    {
        /* L3 Reuse two iterations */
        /* Link EDMA link1 to the shadow channel and shadow to link 1 */
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[0], chanCfg->ShadowPramId[2]);
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[2], chanCfg->ShadowPramId[0]);
    }


exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      EDMA configuration for payload footer to be placed from RTI Timer in L3 at the the end
 *      of every data packet .
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  pConfigIn                 Pointer to rangeProc config
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_PayloadFooter
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
)
{
    int32_t             errorCode = SystemP_SUCCESS;
    EDMA_Handle         handle;
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncABCfg    syncABCfg, syncABCfg1, syncABCfg2;
    uint32_t            compSamplesPerPayload;
    DPU_RangeProcHWA_HW_Resources *pHwConfig = &pConfigIn->hwRes;
    DPU_RangeProcHWA_StaticConfig *staticCfg = &pConfigIn->staticCfg;

    /* Get rangeProc Configuration */
    handle = rangeProcObj->edmaHandle;
    compSamplesPerPayload = rangeProcObj->compressionCfg.outputSamplesPerBlock * rangeProcObj->compressionCfg.numBlockPerPayload;

    chainingCfg.chainingChannel = pHwConfig->edmaOutCfg.dataOutSignature.channel;
    chainingCfg.isFinalChainingEnabled = true;
    chainingCfg.isIntermediateChainingEnabled = true;

    syncABCfg.srcAddress = (uint32_t)(CSL_MSS_RTIA_U_BASE + 0x10);
    syncABCfg.destAddress = (uint32_t)(rangeProcObj->radarCubebuf + compSamplesPerPayload + (staticCfg->appHeaderSize/sizeof(uint32_t)));
    syncABCfg.aCount = 8U;
    syncABCfg.bCount = staticCfg->numPayloads;
    syncABCfg.cCount = staticCfg->numChirpsEachIter[0];
    syncABCfg.srcBIdx = 0U;
    syncABCfg.srcCIdx = 0U;
    syncABCfg.dstBIdx = compSamplesPerPayload*rangeProcObj->compressionCfg.bytesPerSample + staticCfg->appHeaderSize + staticCfg->appFooterSize;
    syncABCfg.dstCIdx = pHwConfig->radarCube.dataSize/staticCfg->numChirpsPerFrame;

    syncABCfg1 = syncABCfg;
    syncABCfg1.cCount = staticCfg->numChirpsEachIter[1];
    syncABCfg2 = syncABCfg;
    syncABCfg2.cCount = staticCfg->numChirpsEachIter[2];

    errorCode = DPEDMA_configSyncAB_ThreeLinks(handle,
                                    &pHwConfig->edmaPacketCfg.appFooter,
                                    &chainingCfg,
                                    &syncABCfg,
                                    &syncABCfg1,
                                    &syncABCfg2,
                                    false,    /* isEventTriggered */
                                    true,   /* isIntermediateTransferInterruptEnabled */
                                    true,   /* isFinalTransferInterruptEnabled */
                                    pHwConfig->edmaPacketCfg.appFooterCallbackFxn,
                                    NULL,
                                    pHwConfig->edmaPacketCfg.edmaCompleteIntrObj);

    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

    uint32_t baseAddr = EDMA_getBaseAddr(handle);
    if (baseAddr == 0)
    {
        errorCode = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }
    DPEDMA_3LinkChanCfg     *chanCfg = &pHwConfig->edmaPacketCfg.appFooter;
    if(syncABCfg1.cCount == 0U)
    {
        /* No L3 Reuse case, only single iteration of L3. */
        /* Link main channel to the shadow channel and shadow to itself */
        EDMA_linkChannel(baseAddr, chanCfg->paramId, chanCfg->ShadowPramId[2]);
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[2], chanCfg->ShadowPramId[2]);
    }
    else if(syncABCfg2.cCount == 0U)
    {
        /* L3 Reuse two iterations */
        /* Link EDMA link1 to the shadow channel and shadow to link 1 */
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[0], chanCfg->ShadowPramId[2]);
        EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[2], chanCfg->ShadowPramId[0]);
    }

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      EDMA configuration for rangeProc data output to be stored in L3
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  pConfigIn                 Pointer to rangeProc config
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_DataOut
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
)
{
    int32_t             errorCode = SystemP_SUCCESS;
    DPEDMA_ChainingCfg       chainingCfg;
    uint32_t pingSrcAddress = (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_COMP_PING_OUT];
    uint32_t pongSrcAddress = (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_COMP_PONG_OUT];
    uint32_t pingDstAddress = (uint32_t)(rangeProcObj->radarCubebuf+ (uint32_t)(pConfigIn->staticCfg.appHeaderSize/sizeof(uint32_t)));
    uint32_t pongDstAddress = pingDstAddress + (pConfigIn->hwRes.radarCube.dataSize/rangeProcObj->numChirpsPerFrame);

    /* Setup Chaining configuration */
    chainingCfg.chainingChannel = pConfigIn->hwRes.edmaPacketCfg.appFooter.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled = true;

    /* Ping EDMA Out HWA->L3 Configuration */
    errorCode = rangeProcHWA_ConfigEDMA_CompCubeOut(rangeProcObj,
                                                    pConfigIn,
                                                    &pConfigIn->hwRes.edmaOutCfg.dataOutPing,
                                                    &chainingCfg,
                                                    pingSrcAddress,
                                                    pingDstAddress,
                                                    false,  /* isTransferCompletionEnabled */
                                                    NULL, /* transferCompletionCallbackFxn */
                                                    NULL,
                                                    NULL);
    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* Pong EDMA Out HWA->L3 Configuration */
    errorCode = rangeProcHWA_ConfigEDMA_CompCubeOut(rangeProcObj,
                                                    pConfigIn,
                                                    &pConfigIn->hwRes.edmaOutCfg.dataOutPong,
                                                    &chainingCfg,
                                                    pongSrcAddress,
                                                    pongDstAddress,
                                                    false,
                                                    NULL,
                                                    NULL,
                                                    NULL);
    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* Copy Header to the L3RAM */
    errorCode = rangeProcHWA_ConfigEDMA_PayloadHeader(rangeProcObj, pConfigIn);
    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }


    /* Copy Footer to the L3RAM */
    errorCode = rangeProcHWA_ConfigEDMA_PayloadFooter(rangeProcObj, pConfigIn);
    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

    /*****************************************************************
     *  HWA hot signature EDMA, chained to the Footer EDMA channels
     ****************************************************************/
    errorCode = DPEDMAHWA_configTwoHotSignature(rangeProcObj->edmaHandle,
                                                  &pConfigIn->hwRes.edmaOutCfg.dataOutSignature,
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
 *  @param[in]  pConfigIn                 Pointer to rangeProc configuration
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
    DPU_RangeProcHWA_Config *pConfigIn
)
{
    int32_t             errorCode = SystemP_SUCCESS;
    EDMA_Handle         handle ;
    uint16_t            bytePerRxChan;
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncACfg     syncACfg;

    /* Get rangeProc Configuration */
    handle = rangeProcObj->edmaHandle;

    bytePerRxChan = pConfigIn->staticCfg.ADCBufData.dataProperty.numAdcSamples * pConfigIn->staticCfg.sizeOfInputSample;

    /**********************************************/
    /* ADCBuf -> Ping/Pong Buffer(M0 and M1)           */
    /**********************************************/
    chainingCfg.chainingChannel = pConfigIn->hwRes.edmaInCfg.dataInSignature.channel;
    chainingCfg.isFinalChainingEnabled = true;
    chainingCfg.isIntermediateChainingEnabled = true;

    syncACfg.srcAddress = (uint32_t)rangeProcObj->ADCdataBuf;
    syncACfg.destAddress = rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_DCEST_PING_IN];
    syncACfg.aCount = bytePerRxChan *pConfigIn->staticCfg.ADCBufData.dataProperty.numRxAntennas;
    syncACfg.bCount = 2U; /* ping and pong */
    syncACfg.dstBIdx=((uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_DCEST_PONG_IN] - (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWAREAL2X_MEM_BANK_DCEST_PING_IN]);
    syncACfg.srcBIdx = 0U;

    errorCode = DPEDMA_configSyncA_singleFrame(handle,
                                    &pConfigIn->hwRes.edmaInCfg.dataIn,
                                    &chainingCfg,
                                    &syncACfg,
                                    true,    /* isEventTriggered */
                                    true,   /* isIntermediateTransferInterruptEnabled */
                                    true,   /* isFinalTransferInterruptEnabled */
                                    NULL,
                                    NULL,
                                    NULL);


    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }

    /*************************************************/
    /* Generate Hot Signature to trigger Ping/Pong paramset   */
    /*************************************************/

    errorCode = DPEDMAHWA_configTwoHotSignature(handle,
                                                  &pConfigIn->hwRes.edmaInCfg.dataInSignature,
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
 *      rangeProc configuration in non-interleaved mode
 *
 *  @param[in]  rangeProcObj                 Pointer to rangeProc object
 *  @param[in]  pConfigIn                 Pointer to rangeProc configuration
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigNonInterleaveMode
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_Config *pConfigIn
)
{
    int32_t             retVal = 0;

    /* Copy data from ADC buffer to HWA buffer */
    retVal = rangeProcHWA_ConfigEDMA_DataIn(rangeProcObj, pConfigIn);
    if(retVal != 0)
    {
        goto exit;
    }

    /* Range FFT configuration in HWA */
    retVal = rangeProcHWA_ConfigHWA(rangeProcObj, pConfigIn);
    if(retVal != 0)
    {
        goto exit;
    }

    /* EDMA configuration */
    retVal = rangeProcHWA_ConfigEDMA_DataOut(rangeProcObj, pConfigIn);
    if(retVal != 0)
    {
        goto exit;
    }

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
    DPU_RangeProcHWA_StaticConfig   *pStaticCfg;
    rangeProcHWACompressionCfg      *compParams;
    uint32_t                shift, scale;

    /* Get configuration pointers */
    pStaticCfg = &pConfigIn->staticCfg;
    compParams = &rangeProcObj->compressionCfg;

    /* Save buffers */
    rangeProcObj->ADCdataBuf        = (cmplx16ImRe_t *)pStaticCfg->ADCBufData.data;

    rangeProcObj->radarCubebuf      = (cmplx16ImRe_t *)pConfigIn->hwRes.radarCube.data;

    /* Save interleave mode from ADCBuf configuraiton */
    rangeProcObj->interleave = pStaticCfg->ADCBufData.dataProperty.interleave;
    /* Saven numbetr of chirps from static configuration */
    rangeProcObj->numChirpsPerFrame = pStaticCfg->numChirpsPerFrame;

    if((rangeProcObj->interleave ==DPIF_RXCHAN_NON_INTERLEAVE_MODE) &&
        (pStaticCfg->ADCBufData.dataProperty.numRxAntennas > 1) )
    {
        /* For rangeProcDPU needs rx channel has same offset from one channel to the next channel
           Use first two channel offset to calculate the BIdx for EDMA
         */
        rangeProcObj->rxChanOffset = pStaticCfg->ADCBufData.dataProperty.rxChanOffset[1] -
                                    pStaticCfg->ADCBufData.dataProperty.rxChanOffset[0];

        /* rxChanOffset should be 16 bytes aligned and should be big enough to hold numAdcSamples */
        if ((rangeProcObj->rxChanOffset < (pStaticCfg->ADCBufData.dataProperty.numAdcSamples * pStaticCfg->sizeOfInputSample)) ||
          ((rangeProcObj->rxChanOffset & 0xF) != 0))
        {
            retVal = DPU_RANGEPROCHWA_EADCBUF_INTF;
            goto exit;
        }
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
    compParams->numBlockPerPayload =  compParams->numBlocks/pStaticCfg->numPayloads;
    compParams->outputBytesPerBlock = (MATHUTILS_CEILING_POS_FLOAT(compParams->inputBytesPerBlock * pStaticCfg->compressionCfg.compressionRatio / 4.0))
                            * 4; /* Word aligned */

    compParams->achievedCompressionRatio = (float) compParams->outputBytesPerBlock / (float) compParams->inputBytesPerBlock;
    compParams->outputSamplesPerBlock = compParams->outputBytesPerBlock/compParams->bytesPerSample;
    compParams->rangeBinsPerBlock = pStaticCfg->compressionCfg.rangeBinsPerBlock;
    compParams->rxAntPerBlock = pStaticCfg->compressionCfg.numRxAntennaPerBlock;

    /* Prepare internal hardware resouces = trigger source matchs its  paramset index */
    rangeProcObj->dataInTrigger[0]      = 1U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataInTrigger[1]      = 6U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataOutTrigger[0]     = 0U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataOutTrigger[1]     = 5U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;

    /* Save hardware resources that will be used at runtime */
    rangeProcObj->edmaHandle= pConfigIn->hwRes.edmaHandle;
    rangeProcObj->dataOutSignatureChan = pConfigIn->hwRes.edmaOutCfg.dataOutSignature.channel;
    memcpy((void *)&rangeProcObj->hwaCfg, (void *)&pConfigIn->hwRes.hwaCfg, sizeof(DPU_RangeProcHWA_HwaConfig));

    /* DC Est shift and scale */
    retVal = rangeProcHWAReal2x_findDCEstStaticParams(pStaticCfg->ADCBufData.dataProperty.numAdcSamples,
                                                    &scale, &shift);
    if (retVal != 0)
    {
        goto exit;
    }
    rangeProcObj->dcEstShiftScaleCfg.scale = scale;
    rangeProcObj->dcEstShiftScaleCfg.shift = shift;

    /* Interf config */
    retVal = rangeProcHWAReal2x_findIntfStatsStaticParams(pStaticCfg->ADCBufData.dataProperty.numAdcSamples,
                                                        pStaticCfg->intfStatsCfgdB.intfMitgMagSNRdB,
                                                        &scale, &shift);
    if (retVal != 0)
    {
        goto exit;
    }
    rangeProcObj->intfStatsMagShiftScaleCfg.scale = scale;
    rangeProcObj->intfStatsMagShiftScaleCfg.shift = shift;

    retVal = rangeProcHWAReal2x_findIntfStatsStaticParams(pStaticCfg->ADCBufData.dataProperty.numAdcSamples,
                                                        pStaticCfg->intfStatsCfgdB.intfMitgMagDiffSNRdB,
                                                        &scale, &shift);
    if (retVal != 0)
    {
        goto exit;
    }
    rangeProcObj->intfStatsMagDiffShiftScaleCfg.scale = scale;
    rangeProcObj->intfStatsMagDiffShiftScaleCfg.shift = shift;

exit:
    return(retVal);
}

/**************************************************************************
 ************************RangeProcHWA External APIs ***********************
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
    uint8_t             index;
    int32_t             status = SystemP_SUCCESS;

    *errCode = 0;

    if( (initParams == NULL) ||
       (initParams->hwaHandle == NULL) )
    {
        *errCode = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    DebugP_assert(subframeCounter < RL_MAX_SUBFRAMES);

    rangeProcObj = (rangeProcHWAObj*)&RangeObj[subframeCounter];

    if(rangeProcObj == NULL)
    {
        *errCode = DPU_RANGEPROCHWA_ENOMEM;
        goto exit;
    }

    /* Initialize memory */
    memset((void *)rangeProcObj, 0, sizeof(rangeProcHWAObj));

    memcpy((void *)&rangeProcObj->initParms, initParams, sizeof(DPU_RangeProcHWA_InitParams));

    *errCode = HWA_initializeRAM(initParams->hwaHandle, HWA_APP_MEMINIT_MEMBANK_ALL);
    if (*errCode < 0)
    {
        goto exit;
    }

    /* Set HWA bank memory address */
    *errCode =  HWA_getHWAMemInfo(initParams->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {
        goto exit;
    }

    for (index = 0; index < hwaMemInfo.numBanks; index++)
    {
        rangeProcObj->hwaMemBankAddr[index] = hwaMemInfo.baseAddress + index * hwaMemInfo.bankSize;
    }

    /* Create semaphore for HWA done */
    status = SemaphoreP_constructBinary(&rangeProcObj->hwaDoneSemaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_RANGEPROCHWA_ESEMA;
        goto exit;
    }

    /* Reset the RSS Bookkeeping registers required for application header value */
    CSL_rss_ctrlRegs *ptrRssCtrlregs = (CSL_rss_ctrlRegs *)CSL_RSS_CTRL_U_BASE;

    /* Reset the RSS_BOOKKEEPING_SEQ_NUM */
    CSL_FINS(ptrRssCtrlregs->RSS_BOOKKEEPING_CTRL, RSS_CTRL_RSS_BOOKKEEPING_CTRL_RSS_BOOKKEEPING_CTRL_SEQ_NUM_RST, 1);

    /* Reset the RSS_BOOKKEEPING_FRM_CNT */
    CSL_FINS(ptrRssCtrlregs->RSS_BOOKKEEPING_CTRL, RSS_CTRL_RSS_BOOKKEEPING_CTRL_RSS_BOOKKEEPING_CTRL_FRM_CNT_RST, 1);

    /* Reset the RSS_BOOKKEEPING_CHRP_CNT */
    CSL_FINS(ptrRssCtrlregs->RSS_BOOKKEEPING_CTRL, RSS_CTRL_RSS_BOOKKEEPING_CTRL_RSS_BOOKKEEPING_CTRL_CHRP_CNT_RST, 1);

    /* Write 0: RSS_BOOKKEEPING_FRM_CNT is incremented on every FRAME_START */
    CSL_FINS(ptrRssCtrlregs->RSS_BOOKKEEPING_CTRL, RSS_CTRL_RSS_BOOKKEEPING_CTRL_RSS_BOOKKEEPING_CTRL_FRM_CNT_TRIG_SRC, 0);

    /* Write 0: RSS_BOOKKEEPING_CHRP_CNT is incremented on every CHIRP_START */
    CSL_FINS(ptrRssCtrlregs->RSS_BOOKKEEPING_CTRL, RSS_CTRL_RSS_BOOKKEEPING_CTRL_RSS_BOOKKEEPING_CTRL_CHRP_CNT_TRIG_SRC, 0);

    /* Write 0: RSS_BOOKKEEPING_CHRP_CNT is reset on every FRAME_START */
    CSL_FINS(ptrRssCtrlregs->RSS_BOOKKEEPING_CTRL, RSS_CTRL_RSS_BOOKKEEPING_CTRL_RSS_BOOKKEEPING_CTRL_CHRP_CNT_RST_SRC, 0);

    /* Set the Magic Number in RSS_APP_GP */
    CSL_FINS(ptrRssCtrlregs->RSS_APP_GP, RSS_CTRL_RSS_APP_GP_RSS_APP_GP_REG, 0x01234567);

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
       (pConfigIn->hwRes.hwaCfg.numParamSet != DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS)
      )
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }
    if(pConfigIn->hwRes.hwaCfg.dataInputMode != DPU_RangeProcHWA_InputMode_ISOLATED)
    {
        retVal = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    /* Parameter check: validate Adc data interface configuration
        Support:
            - 1 chirp per chirpEvent
            - Real only 16bit ADC data supported for AWR2544 Processing Chain
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
            expectedWinSize = ((pStaticCfg->ADCBufData.dataProperty.numAdcSamples + 1U) / 2U );
        }
        else
        {
            expectedWinSize = pStaticCfg->ADCBufData.dataProperty.numAdcSamples;
        }

        expectedWinSize *= (pStaticCfg->isMode2x)?sizeof(uint16_t):sizeof(uint32_t);

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

    if(CSL_MEM_IS_NOT_ALIGN(pConfigIn->hwRes.radarCube.data,
#ifdef SUBSYS_MSS
                                 DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F))
#else
                                 DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP))
#endif
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }

    /* Refer to radar cube definition for FORMAT_x , the following are the only supported formats
        Following assumption is made upon radar cube FORMAT_x definition
           1. data type is complex in cmplx16ImRe_t format only
           2. It is always Format 2 (interleaved)
     */
    if( (pConfigIn->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_2) )
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }

    /* Not supported input & output format combination */
    if (pStaticCfg->ADCBufData.dataProperty.interleave == DPIF_RXCHAN_INTERLEAVE_MODE)
    {
        retVal = DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }
    if (pStaticCfg->ADCBufData.dataProperty.numRxAntennas == 3U)
    {
        retVal = DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }

    /* Check if RTIA timer is enabled */
    if(((*((volatile uint32_t*)CSL_MSS_RTIA_U_BASE)) & 0x00000001U) == 0U)
    {
        retVal = DPU_RANGEPROCHWA_EFOOTERTIMER;
        goto exit;
    }
#endif

    retVal = rangeProcHWA_ParseConfig(rangeProcObj, pConfigIn);
    if (retVal < 0)
    {
        goto exit;
    }

    if (pConfigIn->hwRes.radarCube.dataSize != (((pStaticCfg->numRangeBins* sizeof(cmplx16ImRe_t) *
                                      pStaticCfg->ADCBufData.dataProperty.numRxAntennas
                                      * rangeProcObj->compressionCfg.achievedCompressionRatio)+
                                      ((pStaticCfg->appHeaderSize + pStaticCfg->appFooterSize)*pStaticCfg->numPayloads))*pStaticCfg->numChirpsPerFrame))
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_DATASIZE;
        goto exit;
    }

    /* Parameter check: radarcube buffer Size */
    if (pConfigIn->hwRes.radarCubeBufSizeL3 != (((pStaticCfg->numRangeBins* sizeof(cmplx16ImRe_t) *
                                      pStaticCfg->ADCBufData.dataProperty.numRxAntennas
                                      * rangeProcObj->compressionCfg.achievedCompressionRatio)+
                                      ((pStaticCfg->appHeaderSize + pStaticCfg->appFooterSize)*pStaticCfg->numPayloads))*pStaticCfg->numChirpsEachIter[0]))
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_BUF;
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
    if (rangeProcObj->interleave == DPIF_RXCHAN_NON_INTERLEAVE_MODE)
    {
        retVal = rangeProcHWA_ConfigNonInterleaveMode(rangeProcObj, pConfigIn);
        if (retVal != 0)
        {
            goto exit;
        }
    }
    else
    {
        retVal=DPU_RANGEPROCHWA_ENOTIMPL;
        goto exit;
    }

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
    DPU_RangeProcHWA_OutParams  *outParams
)
{
    rangeProcHWAObj     *rangeProcObj;
    int32_t             retVal = 0;

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

    /**********************************************/
    /* WAIT FOR HWA NUMLOOPS INTERRUPT            */
    /**********************************************/
    /* wait for the all paramSets done interrupt */
    SemaphoreP_pend(&rangeProcObj->hwaDoneSemaHandle, SystemP_WAIT_FOREVER);

    /* Range FFT is done, disable Done interrupt */
    HWA_disableDoneInterrupt(rangeProcObj->initParms.hwaHandle, 0);

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
    outParams->stats.waitTime= 0;

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
            retVal = rangeProcHWA_TriggerHWA( rangeProcObj);
            if(retVal != 0)
            {
                goto exit;
            }
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
    SemaphoreP_destruct(&rangeProcObj->hwaDoneSemaHandle);

exit:

    return (retVal);
}
