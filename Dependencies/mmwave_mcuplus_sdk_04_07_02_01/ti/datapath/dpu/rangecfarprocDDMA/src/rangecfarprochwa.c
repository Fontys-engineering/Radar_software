/**
 *   @file  rangecfarprochwa.c
 *
 *   @brief
 *      Implements Data path processing Unit using HWA.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* MCU+SDK Include files */
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/HeapP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/CycleCounterP.h>

/* mmWave SDK drivers/common Include Files */
#include <ti/common/syscommon.h>

/* Data Path Include Files */
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/include/rangecfarprochwainternal.h>

#include <ti/control/mmwavelink/mmwavelink.h>
//#define DBG_CFAR_HWA_OBJ_DPU

#ifdef DBG_CFAR_HWA_OBJ_DPU
DPU_RangeCfarProcHWA_Obj         *DPU_RangeCfarProcHWA_Obj;
volatile DPU_RangeCfarProcHWA_Obj *gDPU_RangeCfarProcHWA_Obj[RL_MAX_SUBFRAMES] = {NULL};
volatile uint32_t gDPU_RangeCfarProcHWA_ObjInd = 0;
#endif

DPU_RangeCfarProcHWA_Obj cfarProcObjPool __attribute__((aligned(HeapP_BYTE_ALIGNMENT)));

#define DPU_RANGECFARPROCHWA_CEILDIV(a, b) ((a % b == 0) ? (a / b) : (a / b) + 1)

#define DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PING_IN      0
#define DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PING_OUT     4
#define DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PONG_IN      1
#define DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PONG_OUT     6

#define DPU_RANGECFARPROCHWADDMA_ADDR_CFAR_PING_IN              HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PING_IN])
#define DPU_RANGECFARPROCHWADDMA_ADDR_CFAR_PING_OUT             HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PING_OUT])
#define DPU_RANGECFARPROCHWADDMA_ADDR_CFAR_PONG_IN              HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PONG_IN])
#define DPU_RANGECFARPROCHWADDMA_ADDR_CFAR_PONG_OUT             HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PONG_OUT])

#define DPU_RANGECFARPROCHWADDMA_CFAR_PING_HWA_PARAMSET_RELATIVE_IDX          0

#define DPU_RANGECFARPROCHWADDMA_CFAR_PONG_HWA_PARAMSET_RELATIVE_IDX          1

static int32_t DPU_RangeCfarProcHWA_configEdmaDetMatIn
(
    DPU_RangeCfarProcHWA_Obj      *obj,
    DPU_RangeCfarProcHWA_Config   *cfg
);

static void RangeCFARHWA_saveConfiguration
(
    DPU_RangeCfarProcHWA_Obj *obj,
    DPU_RangeCfarProcHWA_Config *cfarHwaCfg
);

static int32_t dpuRangeCfarProcDDMA_cfarObjCumSum
(
    DPU_RangeCfarProcHWA_Obj          *obj,
    DPU_RangeCfarProcHWA_Config       *cfg
);

static int32_t dpuRangeCfarProcDDMA_createRangeCfarList
(   
    DPU_RangeCfarProcHWA_Obj          *obj,
    DPU_RangeCfarProcHWA_Config       *cfg,
    uint32_t blockIdx
);

#ifdef DBG_CFAR_HWA_OBJ_DPU
uint32_t gedmacall = 0;
#endif
/**
 *  @b Description
 *  @n
 *      EDMA completion call back function.
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static void DPU_RangeCfarProcHWA_edmaDoneIsrCallback(Edma_IntrHandle intrHandle,
   void *args)
{

#ifdef DBG_CFAR_HWA_OBJ_DPU
    gedmacall++;
#endif
    if (args != NULL)
    {
        SemaphoreP_post((SemaphoreP_Object*)args);
    }

}

/**
 *  @b Description
 *  @n
 *      Calculates the number of doppler gates per ping
 *
 *  @param[in]  numRangeBins           number of range bins
 *  @param[in]  numDopFFTBins             number of doppler FFT bins
 *  @param[in]  bytesPerSample            bytes per sample
 *  @param[in]  bankSizeBytes             HWA membank size in bytes
 *  @param[out]  numDopplerGatesPerPing   doppler gates to process per ping
 *  @param[out]  cfarInSize               CFAR in size
 *  @param[out]  numLoops                 number of HWA loops
 *
 *  \ingroup    DPU_RANGECFARPROC_INTERNAL_FUNCTION
 *
 *  @retval     error code
 */
static int32_t dpuRangeCfarProcDDMA_numDopplerGatesPerPing(uint32_t numRangeBins,
                                                                uint32_t numDopFFTBins,
                                                                uint32_t bytesPerSample,
                                                                uint32_t bankSizeBytes,
                                                                uint32_t *numDopplerGatesPerPing,
                                                                uint32_t *cfarInSize,
                                                                uint32_t *numLoops)
{
    uint32_t dopplerGateSize = numRangeBins * bytesPerSample;
    int32_t retVal = 0;

    *numDopplerGatesPerPing = bankSizeBytes / dopplerGateSize;

    *numLoops = ((numDopFFTBins + *numDopplerGatesPerPing - 1U) / *numDopplerGatesPerPing) / 2U; /* divide by 2 for ping and pong */

    if(*numLoops < 1U)
    {
        *numLoops = 1U;
    }

    while((numDopFFTBins % (*numLoops)) != 0U)
    {
        numLoops++;
    }

    *numDopplerGatesPerPing = numDopFFTBins / ((*numLoops) * 2U);
    *cfarInSize = dopplerGateSize * (*numDopplerGatesPerPing);
    if(*cfarInSize == 0U)
    {
        retVal = DPU_RANGECFARPROCHWADDMA_CFARINSIZE_ERR;
        goto exit;
    }

    if ((numDopFFTBins % ((*numLoops) * 2U)) != 0U)
    {
        retVal = DPU_RANGECFARPROCHWADDMA_NUM_GATES_PER_PING_ERR;
        goto exit;
    }

exit:
    return retVal;

}


/** @brief Configures ParameterSet for CFAR detection (The CFAR detection runs along range bins).
 *
 *  \ingroup    DPU_RANGECFARPROC_INTERNAL_FUNCTION
 *
 *   @param[in] obj    Range CFAR internal object
 *
 *   @param[in] cfg    Range CFAR configuration
 *
 */
static inline int32_t DPU_RangeCfarProcHWA_configHwa
(
    DPU_RangeCfarProcHWA_Obj      *obj,
    DPU_RangeCfarProcHWA_Config   *cfg
)
{
    HWA_ParamConfig hwaParamCfg;
    HWA_InterruptConfig paramISRConfig;
    int32_t errCode;
    uint8_t destChan;
    uint8_t pingHwParamsetIdx = cfg->res.hwaCfg.paramSetStartIdx,
            pongHwParamsetIdx = pingHwParamsetIdx + 1U;
    uint8_t cfarAvgRight, cfarAvgLeft, cfarGuardCells;
    uint32_t index;

    for (index = 0; index < cfg->res.hwaCfg.numParamSet; index++)
    {
        errCode = HWA_disableParamSetInterrupt(obj->hwaHandle, (uint8_t)index + pingHwParamsetIdx,
                                               HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    (void)memset((void *)&hwaParamCfg, 0, sizeof(hwaParamCfg));

    /*******************************/
    /* PING RANGE CFAR PARAMSET    */
    /*******************************/
    {
        {

            hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            hwaParamCfg.triggerSrc = obj->hwaDmaTriggerSourcePingPongIn[0];

            hwaParamCfg.accelMode = HWA_ACCELMODE_CFAR;

            cfarAvgRight = obj->rangeCfarCfg->winLen >> 1;
            cfarAvgLeft = obj->rangeCfarCfg->winLen >> 1;
            cfarGuardCells = obj->rangeCfarCfg->guardLen;

            hwaParamCfg.accelModeArgs.cfarMode.peakGroupEn = obj->rangeCfarCfg->peakGroupingEn;
            hwaParamCfg.accelModeArgs.cfarMode.operMode = HWA_CFAR_OPER_MODE_LOG_INPUT_REAL; /* cfarInpMode = 1, cfarLogMode = 1, cfarAbsMode = 00b */
            hwaParamCfg.accelModeArgs.cfarMode.numGuardCells = cfarGuardCells;
            hwaParamCfg.accelModeArgs.cfarMode.nAvgDivFactor = obj->rangeCfarCfg->noiseDivShift; // not applicable in CFAR_OS
            hwaParamCfg.accelModeArgs.cfarMode.cyclicModeEn = obj->rangeCfarCfg->cyclicMode;
            hwaParamCfg.accelModeArgs.cfarMode.nAvgMode = obj->rangeCfarCfg->averageMode;
            hwaParamCfg.accelModeArgs.cfarMode.numNoiseSamplesRight = cfarAvgRight;
            hwaParamCfg.accelModeArgs.cfarMode.numNoiseSamplesLeft = cfarAvgLeft;
            hwaParamCfg.accelModeArgs.cfarMode.outputMode = HWA_CFAR_OUTPUT_MODE_I_PEAK_IDX_Q_NEIGHBOR_NOISE_VAL;
            if (obj->rangeCfarCfg->averageMode == HWA_NOISE_AVG_MODE_CFAR_OS)
            {
                hwaParamCfg.accelModeArgs.cfarMode.cfarOsKvalue = obj->rangeCfarCfg->osKvalue;
                hwaParamCfg.accelModeArgs.cfarMode.cfarOsEdgeKScaleEn = obj->rangeCfarCfg->osEdgeKscaleEn;
            }

            hwaParamCfg.source.srcAddr = DPU_RANGECFARPROCHWADDMA_ADDR_CFAR_PING_IN;

            if(obj->rangeCfarCfg->cyclicMode == 1U)
            {
                /* ACNT is equal to the number of samples to be loaded into the sliding buffer including the cyclic rotation */
                hwaParamCfg.source.srcAcnt = (uint16_t)obj->numCfarSamples - 1U + (2U * (uint16_t)cfarAvgRight) +
                                         (2U * (uint16_t)cfarGuardCells) + (2U * (uint16_t)cfarAvgLeft);
                /* Since for the first sample the window starts from the end due to cyclic rotation, this gives offset of first sample of the window for first cell under test*/
                hwaParamCfg.source.srcAcircShift = (uint16_t)obj->numCfarSamples - ((2U * (uint16_t)cfarAvgRight) + (uint16_t)cfarGuardCells);

                /* Finally this tells the size of the array, around which cyclic rotation has to be done */
                if ((obj->numCfarSamples % 3U) == 0U)
                {                                             /* If numSamples % 3 == 0 */
                    hwaParamCfg.source.srcCircShiftWrap3 = 1; /* 'b001, means wrap in A dim */
                    hwaParamCfg.source.srcAcircShiftWrap = mathUtils_ceilLog2(obj->numCfarSamples / 3U);
                }
                else
                {
                    hwaParamCfg.source.srcCircShiftWrap3 = 0;
                    hwaParamCfg.source.srcAcircShiftWrap = mathUtils_ceilLog2(obj->numCfarSamples);
                }
            }
            else
            {
                hwaParamCfg.source.srcAcnt = (uint16_t)obj->numCfarSamples - 1U;
            }
            
            hwaParamCfg.source.srcAIdx = (int32_t)obj->numDopplerGatesPerPing * (int32_t)obj->rangeCfarIOCfg.input.bytesPerSample;
            hwaParamCfg.source.srcBIdx = (int32_t)obj->rangeCfarIOCfg.input.bytesPerSample;
            hwaParamCfg.source.srcBcnt = (uint16_t)obj->numDopplerGatesPerPing - 1U;
            hwaParamCfg.source.srcRealComplex = obj->rangeCfarIOCfg.input.isReal;
            hwaParamCfg.source.srcScale = 8U;
            if ((obj->rangeCfarIOCfg.input.bytesPerSample == 2U) ||
                ((obj->rangeCfarIOCfg.input.bytesPerSample == 4U) &&
                 (obj->rangeCfarIOCfg.input.isReal == 0U)))
            {
                hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
            }
            else
            {
                hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_32BIT;
            }
            hwaParamCfg.source.srcSign = obj->rangeCfarIOCfg.input.isSigned;
            hwaParamCfg.source.srcConjugate = 0;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAddr = DPU_RANGECFARPROCHWADDMA_ADDR_CFAR_PING_OUT;

            hwaParamCfg.dest.dstAcnt = ((uint16_t)obj->cfarOutSize / (uint16_t)obj->rangeCfarIOCfg.output.bytesPerSample) - 1U;
            hwaParamCfg.dest.dstAIdx = (int32_t)obj->rangeCfarIOCfg.output.bytesPerSample;
            hwaParamCfg.dest.dstBIdx = (int32_t)obj->numCfarSamples * (int32_t)obj->rangeCfarIOCfg.output.bytesPerSample;

            hwaParamCfg.dest.dstRealComplex = obj->rangeCfarIOCfg.output.isReal;
            if ((obj->rangeCfarIOCfg.output.bytesPerSample == 2U) ||
                ((obj->rangeCfarIOCfg.output.bytesPerSample == 4U) &&
                 (obj->rangeCfarIOCfg.output.isReal == 0U)))
            {
                hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
            }
            else
            {
                hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_32BIT;
            }
            hwaParamCfg.dest.dstSign = obj->rangeCfarIOCfg.output.isSigned;
            hwaParamCfg.dest.dstConjugate = HWA_FEATURE_BIT_DISABLE;
            hwaParamCfg.dest.dstScale = 8U;
            hwaParamCfg.dest.dstSkipInit = 0;

            errCode = HWA_configParamSet(obj->hwaHandle,
                                         pingHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            errCode = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->res.edmaOut.pingPong[0].channel, &destChan);
            if (errCode != 0)
            {
                goto exit;
            }
            /* enable the DMA hookup to this paramset so that data gets copied out */
            paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
            paramISRConfig.dma.dstChannel = destChan;
            errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pingHwParamsetIdx, &paramISRConfig);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

    /*******************************/
    /* PONG RANGE CFAR PARAMSET */
    /*******************************/
    {
        {

            /* adcbuf not mapped, HWA is triggered after edma copy is done */
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            hwaParamCfg.triggerSrc = obj->hwaDmaTriggerSourcePingPongIn[1];

            /* SRC CONFIG */
            hwaParamCfg.source.srcAddr = DPU_RANGECFARPROCHWADDMA_ADDR_CFAR_PONG_IN;

            /* DEST CONFIG */
            hwaParamCfg.dest.dstAddr = DPU_RANGECFARPROCHWADDMA_ADDR_CFAR_PONG_OUT;

            errCode = HWA_configParamSet(obj->hwaHandle,
                                         pongHwParamsetIdx,
                                         &hwaParamCfg, NULL);
            if (errCode != 0)
            {
                goto exit;
            }

            errCode = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->res.edmaOut.pingPong[1].channel, &destChan);
            if (errCode != 0)
            {
                goto exit;
            }
            /* enable the DMA hookup to this paramset so that data gets copied out */
            paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
            paramISRConfig.dma.dstChannel = destChan;
            errCode = HWA_enableParamSetInterrupt(obj->hwaHandle, pongHwParamsetIdx, &paramISRConfig);
            if (errCode != 0)
            {
                goto exit;
            }
        }
    }

exit:
    return (errCode);
}

/**
 *  @b Description
 *  @n
 *  Doppler DPU EDMA configuration that sends Detection Matrix data from L3
 *  to HWA memory
 *  This implementation of doppler processing involves Ping/Pong
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static int32_t DPU_RangeCfarProcHWA_configEdmaDetMatIn
(
    DPU_RangeCfarProcHWA_Obj      *obj,
    DPU_RangeCfarProcHWA_Config   *cfg
)
{

    DPEDMA_syncABCfg            syncABCfg;
    DPEDMA_ChainingCfg          chainingCfg;
    int32_t                     retVal;

    if(obj == NULL)
    {
        retVal = DPU_RANGECFARPROCHWA_EINVAL;
        goto exit;
    }

    /* Program EDMA Data in from Decompressed Radar Cube scratch buffer to HWA Memory */
    /* PING */
    {{
    chainingCfg.chainingChannel               = (uint8_t)cfg->res.edmaInSignature.pingPong[0].channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled        = true;

    syncABCfg.srcAddress  = (uint32_t)(cfg->res.detMatrix.data);
    syncABCfg.destAddress = (uint32_t)(obj->hwaMemBankAddr[DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PING_IN]);
    syncABCfg.aCount      = (uint16_t)obj->numDopplerGatesPerPing * (uint16_t)obj->rangeCfarIOCfg.input.bytesPerSample;
    syncABCfg.bCount      = (uint16_t)obj->numRangeBins;
    syncABCfg.cCount      = (uint16_t)obj->numLoops;
    syncABCfg.srcBIdx     = ((int32_t)obj->numDopplerBins / (int32_t)obj->numSubBandsTotal) * (int32_t)obj->rangeCfarIOCfg.input.bytesPerSample;
    syncABCfg.dstBIdx     = (int32_t)obj->numDopplerGatesPerPing * (int32_t)obj->rangeCfarIOCfg.input.bytesPerSample;
    syncABCfg.srcCIdx     = (int16_t)obj->numDopplerGatesPerPing * (int16_t)obj->rangeCfarIOCfg.input.bytesPerSample * 2;
    syncABCfg.dstCIdx     = 0;

    retVal = DPEDMA_configSyncAB(   cfg->res.edmaHandle,
                                    &cfg->res.edmaIn.pingPong[0],
                                    &chainingCfg,
                                    &syncABCfg,
                                    false,//isEventTriggered
                                    false, //isIntermediateTransferCompletionEnabled
                                    false,//isTransferCompletionEnabled
                                    NULL, //transferCompletionCallbackFxn
                                    NULL,
                                    NULL);//transferCompletionCallbackFxnArg
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* One Hot Signature to trigger the HWA */
    retVal = DPEDMAHWA_configOneHotSignature(     cfg->res.edmaHandle,
                                                    &cfg->res.edmaInSignature.pingPong[0],
                                                    obj->hwaHandle,
                                                    obj->hwaDmaTriggerSourcePingPongIn[0],
                                                    false);
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }
    }}

    /* PONG */
    {{
    chainingCfg.chainingChannel  = (uint8_t)cfg->res.edmaInSignature.pingPong[1].channel;
    syncABCfg.srcAddress  = (uint32_t)((uint8_t *)cfg->res.detMatrix.data +
                                        (obj->numDopplerGatesPerPing * obj->rangeCfarIOCfg.input.bytesPerSample));
    syncABCfg.destAddress = (uint32_t)(obj->hwaMemBankAddr[DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PONG_IN]);
    retVal = DPEDMA_configSyncAB(cfg->res.edmaHandle,
                                    &cfg->res.edmaIn.pingPong[1],
                                    &chainingCfg,
                                    &syncABCfg,
                                    false,//isEventTriggered
                                    false, //isIntermediateTransferCompletionEnabled
                                    false,//isTransferCompletionEnabled
                                    NULL, //transferCompletionCallbackFxn
                                    NULL,//transferCompletionCallbackFxnArg
                                    NULL);
    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* One Hot Signature to trigger the HWA */
    retVal = DPEDMAHWA_configOneHotSignature(cfg->res.edmaHandle,
                                                    &cfg->res.edmaInSignature.pingPong[1],
                                                    obj->hwaHandle,
                                                    obj->hwaDmaTriggerSourcePingPongIn[1],
                                                    false);

    }}

exit:
    return(retVal);

}

/**
 *  @b Description
 *  @n
 *  EDMA Configuration to send CFAR data out to L2 from HWA
 *  This implementation of doppler processing involves Ping/Pong
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static inline int32_t DPU_RangeCfarProcHWA_configEdmaCfarOut
(
    DPU_RangeCfarProcHWA_Obj      *obj,
    DPU_RangeCfarProcHWA_Config   *cfg
)
{

    DPEDMA_syncABCfg            syncABCfg;
    DPEDMA_ChainingCfg          chainingCfg;
    int32_t                     retVal;
    Edma_EventCallback doneCllbackFunc[2] = {DPU_RangeCfarProcHWA_edmaDoneIsrCallback, DPU_RangeCfarProcHWA_edmaDoneIsrCallback};
    uint32_t            doneCllbackFuncArg[2];
    
    if(obj == NULL)
    {
        retVal = DPU_RANGECFARPROCHWA_EINVAL;
        goto exit;
    }

    {
        doneCllbackFuncArg[0U] = (uint32_t)&obj->cfarPingEdmaOutDoneSemaHandle;
        doneCllbackFuncArg[1U] = (uint32_t)&obj->cfarPongEdmaOutDoneSemaHandle;
    }

    {{

    /* PING */
    chainingCfg.chainingChannel               = (uint8_t)cfg->res.edmaOut.pingPong[0].channel;
    chainingCfg.isIntermediateChainingEnabled = false;
    chainingCfg.isFinalChainingEnabled        = false;

    syncABCfg.srcAddress  = (uint32_t)(obj->hwaMemBankAddr[DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PING_OUT]);
    syncABCfg.destAddress = (uint32_t)(cfg->res.rangeCfarScratchBuf[0]);
    syncABCfg.aCount      = (uint16_t)obj->cfarOutSize;
    syncABCfg.bCount      = 1U;
    syncABCfg.cCount      = (uint16_t)obj->numLoops;
    syncABCfg.srcBIdx     = 0;
    syncABCfg.dstBIdx     = 0;
    syncABCfg.srcCIdx     = 0;
    syncABCfg.dstCIdx     = 0;

    retVal = DPEDMA_configSyncAB(   cfg->res.edmaHandle,
                                    &cfg->res.edmaOut.pingPong[0],
                                    &chainingCfg,
                                    &syncABCfg,
                                    true,//isEventTriggered // UPON HAWA COMPLETION
                                    true, //isIntermediateTransferCompletionEnabled
                                    true,//isTransferCompletionEnabled
                                    DPU_RangeCfarProcHWA_edmaDoneIsrCallback, //transferCompletionCallbackFxn
                                    (void *)((uint32_t)&obj->cfarPingEdmaOutDoneSemaHandle), //transferCompletionCallbackFxnArg
                                    cfg->res.edmaIntrObj.pingPong[0]); /* intrObj */

    if (retVal != SystemP_SUCCESS)
    {
        goto exit;
    }

    /* PONG */
    chainingCfg.chainingChannel = (uint8_t)cfg->res.edmaOut.pingPong[1].channel;
    syncABCfg.srcAddress  = (uint32_t)(obj->hwaMemBankAddr[DPU_RANGECFARPROCHWADDMA_HWA_MEM_BANK_CFAR_PONG_OUT]);
    syncABCfg.destAddress = (uint32_t)((uint8_t *)cfg->res.rangeCfarScratchBuf[1]);
    retVal = DPEDMA_configSyncAB(   cfg->res.edmaHandle,
                                    &cfg->res.edmaOut.pingPong[1],
                                    &chainingCfg,
                                    &syncABCfg,
                                    true,//isEventTriggered
                                    true, //isIntermediateTransferCompletionEnabled
                                    true,//isTransferCompletionEnabled
                                    doneCllbackFunc[1], //transferCompletionCallbackFxn
                                    (void *)doneCllbackFuncArg[1], //transferCompletionCallbackFxnArg
                                    cfg->res.edmaIntrObj.pingPong[1]); /* intrObj */

    }}

exit:
    return(retVal);

}

/**
 *  @b Description
 *  @n  Saves configuration parameters to CFAR instance
 *
 *  \ingroup    DPU_RANGECFARPROC_INTERNAL_FUNCTION
 *
 *
 * @param[in]  obj Pointer to CFAR instance
 *
 * @param[in]  cfarHwaCfg Pointer to configuration
 *
 *
 */
static void RangeCFARHWA_saveConfiguration(DPU_RangeCfarProcHWA_Obj *obj, DPU_RangeCfarProcHWA_Config *cfarHwaCfg)
{
    obj->rangeCfarCfg = &cfarHwaCfg->staticCfg.cfarCfg;
    obj->numRangeBins = cfarHwaCfg->staticCfg.numRangeBins;
    obj->numDopplerBins = cfarHwaCfg->staticCfg.numDopplerBins;
    obj->numSubBandsTotal = cfarHwaCfg->staticCfg.numSubBandsTotal;
}

DPU_RangeCFARProcHWA_Handle DPU_RangeCFARProcHWA_init
(
    DPU_RangeCFARProcHWA_InitParams *initCfg,
    volatile uint8_t         subframeCounter,
    int32_t* errCode
)
{
    DPU_RangeCfarProcHWA_Obj         *obj = NULL;
    int32_t                          status = SystemP_SUCCESS;
    HWA_MemInfo                      hwaMemInfo;
    uint8_t                          memBankIdx;

    *errCode = 0;

    if ((initCfg == NULL) || (initCfg->hwaHandle == NULL))
    {
        *errCode = DPU_RANGECFARPROCHWA_EINVAL;
        goto exit;
    }

    DebugP_assert(subframeCounter < RL_MAX_SUBFRAMES);

    obj = (DPU_RangeCfarProcHWA_Obj*)&cfarProcObjPool;

    /* Save for debugging */
#ifdef DBG_CFAR_HWA_OBJ_DPU
    if (gDPU_RangeCfarProcHWA_ObjInd < RL_MAX_SUBFRAMES)
    {
        gDPU_RangeCfarProcHWA_Obj[gDPU_RangeCfarProcHWA_ObjInd++] = obj;
    }
#endif

    /* Create semaphore for EDMA Out done */
    status = SemaphoreP_constructBinary(&obj->cfarPingEdmaOutDoneSemaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_RANGECFARPROCHWA_ESEMA;
        goto exit;
    }

    /* Create semaphore for EDMA Out done */
    status = SemaphoreP_constructBinary(&obj->cfarPongEdmaOutDoneSemaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_RANGECFARPROCHWA_ESEMA;
        goto exit;
    }

    /* Save init config params */
    obj->hwaHandle   = initCfg->hwaHandle;

    /* Populate HWA base addresses and offsets. This is done only once, at init time.*/
    *errCode =  HWA_getHWAMemInfo(obj->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {
        goto exit;
    }

    /* check if we have enough memory banks*/
    if(hwaMemInfo.numBanks < DPU_RANGECFARPROCHWA_NUM_HWA_MEMBANKS)
    {
        *errCode = DPU_RANGECFARPROCHWA_ERROR_HWRES;
        goto exit;
    }

    for (memBankIdx = 0; memBankIdx < DPU_RANGECFARPROCHWA_NUM_HWA_MEMBANKS; memBankIdx++)
    {
        obj->hwaMemBankAddr[memBankIdx] = hwaMemInfo.baseAddress + (uint32_t)memBankIdx * (uint32_t)hwaMemInfo.bankSize;
    }

exit:

    if(*errCode < 0)
    {
        /* Free the object if it was allocated */
        if(obj != NULL)
        {
            obj = NULL;
        }
    }

    return ((DPU_RangeCFARProcHWA_Handle)obj);
}

int32_t DPU_RangeCFARProcHWA_config
(
   DPU_RangeCFARProcHWA_Handle       handle,
   DPU_RangeCfarProcHWA_Config       *cfg
)
{
   int32_t  retVal = 0;

   DPU_RangeCfarProcHWA_Obj *obj = (DPU_RangeCfarProcHWA_Obj *)handle;
   DPU_RangeCFARProcHWA_HW_Resources *pRes = &cfg->res;

   if(obj == NULL)
   {
       retVal = DPU_RANGECFARPROCHWA_EINVAL;
       goto exit;
   }


   if(pRes->hwaCfg.numParamSet != DPU_RANGECFARPROCHWA_NUM_HWA_PARAM_SETS)
   {
       retVal = DPU_RANGECFARPROCHWA_EINVAL__NUM_PARAM_SETS;
       goto exit;
   }

   if(CSL_MEM_IS_NOT_ALIGN(pRes->detMatrix.data,
                       DPU_RANGECFARPROCHWA_DET_MATRIX_BYTE_ALIGNMENT))
   {
       retVal = DPU_RANGECFARPROCHWA_ENOMEMALIGN_DET_MATRIX;
       goto exit;
   }

   if(CSL_MEM_IS_NOT_ALIGN(pRes->hwaMemOutRange,
                       DPU_RANGECFARPROCHWA_HWA_MEM_OUT_RANGE_BYTE_ALIGNMENT))
   {
       retVal = DPU_RANGECFARPROCHWA_ENOMEMALIGN_HWA_MEM_OUT_RANGE;
       goto exit;
   }

   RangeCFARHWA_saveConfiguration(obj, cfg);

    obj->rangeCfarIOCfg.input.isReal = 1U;
    obj->rangeCfarIOCfg.input.bytesPerSample = (uint8_t)pRes->detMatBytesPerSample;
    obj->rangeCfarIOCfg.input.isSigned = 0U;
    obj->rangeCfarIOCfg.output.isReal = 0U;
    obj->rangeCfarIOCfg.output.bytesPerSample = (uint8_t)sizeof(cmplx32ImRe_t);
    obj->rangeCfarIOCfg.output.isSigned = 0U;

    obj->numCfarSamples = cfg->staticCfg.numRangeBins;
    retVal = dpuRangeCfarProcDDMA_numDopplerGatesPerPing(cfg->staticCfg.numRangeBins,
                                                        (uint32_t)cfg->staticCfg.numDopplerBins / (uint32_t)obj->numSubBandsTotal,
                                                        obj->rangeCfarIOCfg.input.bytesPerSample,
                                                        DPU_RANFECFARPROCHWADDMA_HWA_MEMBANK_SIZE_BYTES,
                                                        &obj->numDopplerGatesPerPing,
                                                        &obj->cfarInSize,
                                                        &obj->numLoops);
    if(retVal < 0)
    {
        retVal = DPU_RANGECFARPROCHWADDMA_NUM_GATES_PER_PING_ERR;
        goto exit;
    }

    obj->cfarOutSize = cfg->res.maxNumCFARObj * obj->rangeCfarIOCfg.output.bytesPerSample / (2U * obj->numLoops);

    retVal = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->res.edmaInSignature.pingPong[0].channel, &obj->hwaDmaTriggerSourcePingPongIn[0]);
    if (retVal != 0)
    {
        goto exit;
    }
    retVal = HWA_getDMAChanIndex(obj->hwaHandle, (uint8_t)cfg->res.edmaInSignature.pingPong[1].channel, &obj->hwaDmaTriggerSourcePingPongIn[1]);
    if (retVal != 0)
    {
        goto exit;
    }

    if(obj->rangeCfarCfg->averageMode == HWA_NOISE_AVG_MODE_CFAR_OS)
    {
        /* Guard Window is not required in case of CFAR OS */
        if(obj->rangeCfarCfg->guardLen != 0U)
        {
            retVal = DPU_RANGECFARPROCHWA_ERROR_METHOD_CFAR;
            goto exit;
        }
    }

    /* Only CFAR_OS and CFAR_CASO are supported in SDK */
    if((obj->rangeCfarCfg->averageMode != HWA_NOISE_AVG_MODE_CFAR_OS)
        && (obj->rangeCfarCfg->averageMode != HWA_NOISE_AVG_MODE_CFAR_CASO))
    {
        retVal = DPU_RANGECFARPROCHWA_ERROR_METHOD_CFAR;
        goto exit;
    }


   /**************************************/
   /* CFAR RANGE DOMAIN                  */
   /**************************************/

   /* Disable the HWA */
    retVal = HWA_enable(obj->hwaHandle,0);
    if (retVal != 0)
    {
        goto exit;
    }

   retVal = DPU_RangeCfarProcHWA_configHwa(obj, cfg);
   if (retVal != 0)
   {
     goto exit;
   }

   /* Configure EDMA */
    retVal = DPU_RangeCfarProcHWA_configEdmaDetMatIn(obj, cfg);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Configure EDMA */
    retVal = DPU_RangeCfarProcHWA_configEdmaCfarOut(obj, cfg);
    if (retVal != 0)
    {
        goto exit;
    }

exit:
   return retVal;
}

static int32_t dpuRangeCfarProcDDMA_cfarObjCumSum(DPU_RangeCfarProcHWA_Obj *obj, DPU_RangeCfarProcHWA_Config *cfg)
{
    uint16_t *cfarNumObjPerDopList = (uint16_t *) cfg->res.rangeCfarNumObjPerDopplerBinBuf;
    uint16_t numLoops = (obj->numDopplerBins / obj->numSubBandsTotal);
    uint16_t loopIdx;
    int32_t retVal = 0;

    for(loopIdx = 1; loopIdx < numLoops; loopIdx++)
    {
        cfarNumObjPerDopList[loopIdx] += cfarNumObjPerDopList[loopIdx - 1U];
    }

    CacheP_wbInv(cfarNumObjPerDopList, cfg->res.rangeCfarNumObjPerDopplerBinSizeBytes, CacheP_TYPE_ALLD);

    return retVal;

}

/**
 * @brief   Create Range CFAR list
 *  
 *  @param   obj       Range CFAR internal object
 *  @param   cfg       Range CFAR configuration
 *  @param   blockIdx  Block index
 * 
 *  \ingroup    DPU_RANGECFARPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t dpuRangeCfarProcDDMA_createRangeCfarList(DPU_RangeCfarProcHWA_Obj          *obj,
                                                        DPU_RangeCfarProcHWA_Config       *cfg,
                                                        uint32_t blockIdx)
{

    uint8_t * cfarMat;
    uint32_t numCfarPeaks, CFARNoiseCurr;
    uint32_t loopIdx, cfarResReal, cfarResImag;
    uint32_t rangeIdx, dopIdx;
    RangeCfarListObj * currObjParams;
    uint16_t * cfarNumObjPerDopList;
    int32_t retVal = 0;
    uint32_t numLoops;

    if((blockIdx % 2U) == 0U)
    {
        numCfarPeaks = obj->numCfarPeaksPing;
    }
    else
    {
        numCfarPeaks = obj->numCfarPeaksPong;
    }

    /* Assign local matrix pointers based on ping/pong */
    cfarMat = (uint8_t *) cfg->res.rangeCfarScratchBuf[blockIdx % 2U];
    cfarNumObjPerDopList = (uint16_t *) cfg->res.rangeCfarNumObjPerDopplerBinBuf;

    if(numCfarPeaks > cfg->res.maxNumCFARObj / (2U * obj->numLoops))
    {
        numLoops = cfg->res.maxNumCFARObj / (2U * obj->numLoops);
    }
    else
    {
        numLoops = numCfarPeaks;
    }
    /* Loop through CFAR peaks and check whether an object is present for a particular peak. If it is present,
       store its parameters in the output list */
    for(loopIdx = 0U; loopIdx < numLoops; loopIdx++)
    {
        currObjParams = &cfg->res.rangeCfarList[obj->numObjOut++];

        /* To get the real and imag part */
        cfarResImag = *(uint32_t *)(&cfarMat[loopIdx * sizeof(cmplx32ImRe_t)]);
        cfarResReal = *(uint32_t *)(&cfarMat[(loopIdx * sizeof(cmplx32ImRe_t)) + (sizeof(cmplx32ImRe_t) / 2U)]);
        CFARNoiseCurr = cfarResImag;

        dopIdx   = (cfarResReal) >> 12;
        rangeIdx = (cfarResReal) - (dopIdx << 12);

        currObjParams->rangeIdx = rangeIdx;
        currObjParams->dopIdx = dopIdx + (obj->numDopplerGatesPerPing * blockIdx);
        currObjParams->rangeCFARNoise = CFARNoiseCurr;

        if(currObjParams->dopIdx < (obj->numDopplerBins / obj->numSubBandsTotal))
        {
           cfarNumObjPerDopList[dopIdx + (obj->numDopplerGatesPerPing * blockIdx)]++;
        }
        else
        {
            DebugP_log("Wrong Object! RIdx = %d, DIdx = %d\n", currObjParams->rangeIdx, currObjParams->dopIdx);
            retVal = -1;
            goto exit;
        }
    }

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Internal function to config HWA common
 *
 *  @param[in]  obj                  Pointer to rangeCfarProc object
 *  @param[in]  cfg                  Pointer to rangeCfarProc cfg
 *
 *  \ingroup    DPU_RANGECFARPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t dpuRangeCfarProcHwaDDMA_ConfigHWACommon
(
    DPU_RangeCfarProcHWA_Obj          *obj,
    DPU_RangeCfarProcHWA_Config       *cfg
)
{
    int32_t retVal;

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    retVal = HWA_initConfig(obj->hwaHandle);
    if (retVal != 0)
    {
        goto exit;
    }
    retVal = HWA_configStateMachine((uint16_t)obj->numLoops, (uint16_t)cfg->res.hwaCfg.paramSetStartIdx,
            (uint16_t)cfg->res.hwaCfg.paramSetStartIdx + (uint16_t)cfg->res.hwaCfg.numParamSet - 1U);
    if (retVal != 0)
    {
        goto exit;
    }
    retVal = HWA_configCFARThresholdScale(obj->rangeCfarCfg->thresholdScale);
    if (retVal != 0)
    {
        goto exit;
    }
exit:
    return(retVal);
}

int32_t DPU_RangeCFARProcHWA_process
(
    DPU_RangeCFARProcHWA_Handle       handle,
    DPU_RangeCfarProcHWA_Config       *cfg,
    DPU_RangeCFARProcHWA_OutParams    *outParams
)
{

    int32_t             retVal = 0;
    int32_t             status;
    uint32_t            loopIdx;

    DPU_RangeCfarProcHWA_Obj *obj;

    if (handle == NULL)
    {
        retVal = DPU_RANGECFARPROCHWA_EINVAL;
        goto exit;
    }

    obj = (DPU_RangeCfarProcHWA_Obj *)handle;

    obj->numObjOut = 0;
    outParams->stats.processingTime = CycleCounterP_getCount32();

    CacheP_inv(cfg->res.rangeCfarScratchBuf[0], cfg->res.rangeCfarScratchBufSizeBytes / 2U, CacheP_TYPE_ALLD);
    CacheP_inv(cfg->res.rangeCfarScratchBuf[1], cfg->res.rangeCfarScratchBufSizeBytes / 2U, CacheP_TYPE_ALLD);
    /* Initialize memory */
    (void)memset((void *)cfg->res.rangeCfarNumObjPerDopplerBinBuf, 0, cfg->res.rangeCfarNumObjPerDopplerBinSizeBytes);

    /* Disable the HWA */
    retVal = HWA_enable(obj->hwaHandle,0);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Config HWA Common */
    retVal = dpuRangeCfarProcHwaDDMA_ConfigHWACommon(obj, cfg);
    if(retVal < 0)
    {
        goto exit;
    }

    /* Enable the HWA */
    retVal = HWA_enable(obj->hwaHandle,1);
    if (retVal != 0)
    {
        goto exit;
    }

    for(loopIdx = 0; loopIdx < obj->numLoops; loopIdx++)
    {

        outParams->stats.timeLoop = CycleCounterP_getCount32();

        /* Trigger ping EDMA */
        retVal = DPEDMA_edmaStartTransferManualTrigger(cfg->res.edmaHandle, (uint32_t)cfg->res.edmaIn.pingPong[0].channel);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Pong CFAR Extract Objects*/
        if(loopIdx != 0U)
        {
            outParams->stats.timeDsp[1] = CycleCounterP_getCount32();
            retVal = dpuRangeCfarProcDDMA_createRangeCfarList(obj, cfg, (2U * loopIdx) - 1U);
            if (retVal != 0)
            {
                goto exit;
            }
            outParams->stats.timeDsp[1] = CycleCounterP_getCount32() - outParams->stats.timeDsp[1];
        }

        /* Wait for EDMA Out (ping) */
        status = SemaphoreP_pend(&obj->cfarPingEdmaOutDoneSemaHandle, SystemP_WAIT_FOREVER);
        if (status != SystemP_SUCCESS)
        {
            retVal = DPU_RANGECFARPROCHWA_ESEMA;
            goto exit;
        }

        /* Read CFAR Ping Peak Regs */
        retVal = HWA_readCFARPeakCountReg(obj->hwaHandle, (uint8_t *)&obj->numCfarPeaksPing, (uint8_t)sizeof(uint32_t));
        if (retVal != 0)
        {
            goto exit;
        }

        /* Trigger pong EDMA */
        retVal = DPEDMA_edmaStartTransferManualTrigger(cfg->res.edmaHandle, (uint32_t)cfg->res.edmaIn.pingPong[1].channel);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Ping CFAR Extract Objects*/
        outParams->stats.timeDsp[0] = CycleCounterP_getCount32();
        retVal = dpuRangeCfarProcDDMA_createRangeCfarList(obj, cfg, 2U * loopIdx);
        if (retVal != 0)
        {
            goto exit;
        }
        outParams->stats.timeDsp[0] = CycleCounterP_getCount32() - outParams->stats.timeDsp[0];

        /* Wait for EDMA Out (pong) */
        status = SemaphoreP_pend(&obj->cfarPongEdmaOutDoneSemaHandle, SystemP_WAIT_FOREVER);
        if (status != SystemP_SUCCESS)
        {
            retVal = DPU_RANGECFARPROCHWA_ESEMA;
            goto exit;
        }

        /* Read CFAR Pong Peak Regs */
        retVal = HWA_readCFARPeakCountReg(obj->hwaHandle, (uint8_t *)&obj->numCfarPeaksPong, (uint8_t)sizeof(uint32_t));
        if (retVal != 0)
        {
            goto exit;
        }

        outParams->stats.timeLoop = CycleCounterP_getCount32() - outParams->stats.timeLoop;

    }

    /* Final Pong CFAR Extract Object */
    retVal = dpuRangeCfarProcDDMA_createRangeCfarList(obj, cfg, (2U * loopIdx) - 1U);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Cumulative sum of CFAR Object List */
    retVal = dpuRangeCfarProcDDMA_cfarObjCumSum(obj, cfg);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Disable the HWA */
    retVal = HWA_enable(obj->hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    CacheP_inv(cfg->res.rangeCfarScratchBuf[0], cfg->res.rangeCfarScratchBufSizeBytes / 2U, CacheP_TYPE_ALLD);
    CacheP_inv(cfg->res.rangeCfarScratchBuf[1], cfg->res.rangeCfarScratchBufSizeBytes / 2U, CacheP_TYPE_ALLD);

    outParams->stats.processingTime = CycleCounterP_getCount32() - outParams->stats.processingTime;

    outParams->numCfarDetectedPoints = obj->numObjOut;

exit:
    return (retVal);
}

int32_t DPU_RangeCFARProcHWA_deinit(DPU_RangeCFARProcHWA_Handle handle)
{
    int32_t retVal = 0;

    if (handle == NULL)
    {
        retVal = DPU_RANGECFARPROCHWA_EINVAL;
        goto exit;
    }
exit:
    return (retVal);
}
