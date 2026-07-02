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
 *   @file  rangeprochwa.c
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
#include <ti/control/mmwavelink/mmwavelink.h>

/* Data Path Include files */
#include <ti/datapath/dpu/rangeproc/rangeprochwa.h>

/* MATH utils library Include files */
#include <ti/utils/mathutils/mathutils.h>

/* Internal include Files */
#include <ti/datapath/dpu/rangeproc/include/rangeprochwa_internal.h>

/* User defined heap memory and handle */
rangeProcHWAObj RangeObj[RL_MAX_SUBFRAMES] __attribute__((aligned(HeapP_BYTE_ALIGNMENT)));

/* Flag to check input parameters */
#define DEBUG_CHECK_PARAMS   1

#define DPU_RANGEHWA_MEM_BANK_INDX_SRC_PING   0
#define DPU_RANGEHWA_MEM_BANK_INDX_SRC_PONG   1
#define DPU_RANGEHWA_MEM_BANK_INDX_DST_PING   2
#define DPU_RANGEHWA_MEM_BANK_INDX_DST_PONG   3

#define DPU_RANGEHWA_SRCADDR_PING   HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_SRC_PING])
#define DPU_RANGEHWA_SRCADDR_PONG   HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_SRC_PONG])
#define DPU_RANGEHWA_DSTADDR_PING   HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_DST_PING])
#define DPU_RANGEHWA_DSTADDR_PONG   HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_DST_PONG])

/**************************************************************************
 ************************ Internal Functions Prototype       **********************
 **************************************************************************/
static void rangeProcHWADoneIsrCallback(uint32_t threadIdx, void * arg);
/*static void rangeProcHWA_EDMA_transferCompletionCallbackFxn(uintptr_t arg,
    uint8_t transferCompletionCode);*/
void rangeProcHWA_EDMA_transferCompletionCallbackFxn(Edma_IntrHandle intrHandle,
   void *args);

static int32_t rangeProcHWA_ConfigEDMATranspose
(
    rangeProc_dpParams      *dpParams,
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    uint32_t                srcAddress,
    uint32_t                destAddress,
    bool                    isTransferCompletionEnabled,
    Edma_EventCallback      transferCompletionCallbackFxn,
    void*                   transferCompletionCallbackFxnArg,
    Edma_IntrObject         *intrObj
);

static int32_t rangeProcHWA_ConfigHWA
(
    rangeProcHWAObj     *rangeProcObj,
    uint8_t     destChanPing,
    uint8_t     destChanPong,
    uint32_t    hwaMemSrcPingOffset,
    uint32_t    hwaMemSrcPongOffset,
    uint32_t    hwaMemDestPingOffset,
    uint32_t    hwaMemDestPongOffset
);

static int32_t rangeProcHWA_TriggerHWA
(
    rangeProcHWAObj     *rangeProcObj
);
static int32_t rangeProcHWA_ConfigEDMA_DataOut_interleave
(
    rangeProcHWAObj     *rangeProcObj,
    rangeProc_dpParams  *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t            hwaOutPingOffset,
    uint32_t            hwaOutPongOffset
);
static int32_t rangeProcHWA_ConfigEDMA_DataOut_nonInterleave
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t                hwaOutPingOffset,
    uint32_t                hwaOutPongOffset
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
static int32_t rangeProcHWA_ConfigNonInterleaveMode
(
    rangeProcHWAObj          *rangeProcObj,
    rangeProc_dpParams       *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
);
static int32_t rangeProcHWA_dcRangeSignatureCompensation_init
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProc_CalibDcRangeSigCfg *calibDcRangeSigCfg,
    uint8_t             resetMeanBuffer
);

static void rangeProcHWA_dcRangeSignatureCompensation
(
    rangeProcHWAObj     *obj
);
static int32_t rangeProcHWA_ConfigEDMADummyThreeLinks
(
    EDMA_Handle             handle,
    DPEDMA_3LinkChanCfg     *chanCfg,
    uint8_t                 chainChId0,
    uint8_t                 chainChId1,
    uint8_t                 chainChId2,
    uint16_t                numIter
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
 *  @param[in]  arg                     Argument to the callback function
 *  @param[in]  transferCompletionCode  EDMA transfer complete code
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
volatile uint32_t EdmaCallbackcnt = 0;
void rangeProcHWA_EDMA_transferCompletionCallbackFxn(Edma_IntrHandle intrHandle,
   void *args)
{
    rangeProcHWAObj     *rangeProcObj;

    /* Get rangeProc object */
    rangeProcObj = (rangeProcHWAObj *)args;

    EdmaCallbackcnt++;
    if (intrHandle->tccNum == rangeProcObj->dataOutSignatureChan)
    {
        rangeProcObj->numEdmaDataOutCnt++;
        SemaphoreP_post(&rangeProcObj->edmaDoneSemaHandle);
    }
}

/**
 *  @b Description
 *  @n
 *      Function to config a dummy channel with 2 linked paramset. Each paramset is linked
 *   to a EDMA data copy channel
 *
 *  @param[in]  handle                  EDMA handle
 *  @param[in]  chanCfg                 EDMA channel configuraton
 *  @param[in]  chainChId0              linked EDMA channel 1
 *  @param[in]  chainChId1              linked EDMA channel 2
 *  @param[in]  numIter                 Number of iterations the dummy channel will be excuted.
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static int32_t rangeProcHWA_ConfigEDMADummyTwoLinks
(
    EDMA_Handle             handle,
    DPEDMA_3LinkChanCfg     *chanCfg,
    uint8_t                 chainChId0,
    uint8_t                 chainChId1,
    uint16_t                numIter
)
{
    EDMACCPaRAMEntry   edmaParam;
    int32_t             errorCode = SystemP_SUCCESS;
    uint16_t            linkChId0;
    uint16_t            linkChId1;
    uint32_t            baseAddr, regionId;

    baseAddr = EDMA_getBaseAddr(handle);
    if(baseAddr == 0U)
    {
        goto exit;
    }

    regionId = EDMA_getRegionId(handle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* Get LinkChan from configuraiton */
    linkChId0 = (uint16_t)chanCfg->ShadowPramId[0];
    linkChId1 = (uint16_t)chanCfg->ShadowPramId[1];

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t) NULL;
    edmaParam.destAddr      = (uint32_t) NULL;
    edmaParam.aCnt          = (uint16_t) 0u;
    edmaParam.bCnt          = (uint16_t) numIter;
    edmaParam.cCnt          = (uint16_t) 0u;
    edmaParam.bCntReload    = (uint16_t) 0u;
    edmaParam.srcBIdx       = (int16_t) 0u;
    edmaParam.destBIdx      = (int16_t) 0u;
    edmaParam.srcCIdx       = (int16_t) 0u;
    edmaParam.destCIdx      = (int16_t) 0u;
    edmaParam.linkAddr      = 0xFFFFU;
    edmaParam.opt          |= (EDMA_OPT_TCCHEN_MASK | EDMA_OPT_ITCCHEN_MASK |
         ((((uint32_t)chainChId0) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

    EDMA_setPaRAM(baseAddr, chanCfg->paramId, &edmaParam);

    (void)EDMA_enableTransferRegion(baseAddr, regionId, chanCfg->channel, EDMA_TRIG_MODE_EVENT);

    CacheP_wbAll(CacheP_TYPE_ALLD);

    /* Change the parameter set to use different transferCompletionCode */
    {
        EDMACCPaRAMEntry paramConfig;

        EDMA_ccPaRAMEntry_init(&paramConfig);
        (void)memcpy((void *)&paramConfig, (void *)&edmaParam, sizeof(EDMACCPaRAMEntry));

        /* to #1 EDMA channel */
        paramConfig.opt = 0;
        paramConfig.opt |= (EDMA_OPT_TCCHEN_MASK | EDMA_OPT_ITCCHEN_MASK |
         ((((uint32_t)chainChId1) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

         EDMA_setPaRAM(baseAddr, linkChId1, &paramConfig);

        /* to #0 EDMA channel */
        paramConfig.opt = 0u;
        paramConfig.opt |= (EDMA_OPT_TCCHEN_MASK | EDMA_OPT_ITCCHEN_MASK |
         ((((uint32_t)chainChId0) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

         EDMA_setPaRAM(baseAddr, linkChId0, &paramConfig);

        /* Link 2 shadow links */
        /* Do not use LinkChannel API, it changes to ParamId's TCC  */
        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)linkChId0), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)linkChId1));

        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)linkChId1), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)linkChId0));

        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)chanCfg->paramId), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)linkChId1));
    }
exit:
    return(errorCode);
}


/**
 *  @b Description
 *  @n
 *      Function to config a dummy channel with 3 linked paramset. Each paramset is linked
 *   to a EDMA data copy channel
 *
 *  @param[in]  handle                  EDMA handle
 *  @param[in]  chanCfg                 EDMA channel configuraton
 *  @param[in]  chainChId0              linked EDMA channel 1
 *  @param[in]  chainChId1              linked EDMA channel 2
 *  @param[in]  chainChId2              linked EDMA channel 3
 *  @param[in]  numIter                 Number of iterations the dummy channel will be excuted.
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static int32_t rangeProcHWA_ConfigEDMADummyThreeLinks
(
    EDMA_Handle             handle,
    DPEDMA_3LinkChanCfg     *chanCfg,
    uint8_t                 chainChId0,
    uint8_t                 chainChId1,
    uint8_t                 chainChId2,
    uint16_t                numIter
)
{
    EDMACCPaRAMEntry   edmaParam;
    int32_t             errorCode = SystemP_SUCCESS;
    uint16_t            linkChId0;
    uint16_t            linkChId1;
    uint16_t            linkChId2;
    uint32_t            baseAddr, regionId;

    baseAddr = EDMA_getBaseAddr(handle);
    if(baseAddr == 0U)
    {
        goto exit;
    }

    regionId = EDMA_getRegionId(handle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* Get LinkChan from configuraiton */
    linkChId0 = (uint16_t)chanCfg->ShadowPramId[0];
    linkChId1 = (uint16_t)chanCfg->ShadowPramId[1];
    linkChId2 = (uint16_t)chanCfg->ShadowPramId[2];

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t) NULL;
    edmaParam.destAddr      = (uint32_t) NULL;
    edmaParam.aCnt          = (uint16_t) 0u;
    edmaParam.bCnt          = (uint16_t) numIter;
    edmaParam.cCnt          = (uint16_t) 0u;
    edmaParam.bCntReload    = (uint16_t) 0u;
    edmaParam.srcBIdx       = (int16_t) 0u;
    edmaParam.destBIdx      = (int16_t) 0u;
    edmaParam.srcCIdx       = (int16_t) 0u;
    edmaParam.destCIdx      = (int16_t) 0u;
    edmaParam.linkAddr      = 0xFFFFU;
    edmaParam.opt          |= (EDMA_OPT_TCCHEN_MASK | EDMA_OPT_ITCCHEN_MASK |
         ((((uint32_t)chainChId0) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

    EDMA_setPaRAM(baseAddr, chanCfg->paramId, &edmaParam);

    (void)EDMA_enableTransferRegion(baseAddr, regionId, chanCfg->channel, EDMA_TRIG_MODE_EVENT);

    CacheP_wbAll(CacheP_TYPE_ALLD);

    /* Change the parameter set to use different transferCompletionCode */
    {
        EDMACCPaRAMEntry paramConfig;

        EDMA_ccPaRAMEntry_init(&paramConfig);

        (void)memcpy((void *)&paramConfig, (void *)&edmaParam, sizeof(EDMACCPaRAMEntry));

        /* to TX1 EDMA channel */
        paramConfig.opt = 0;
        paramConfig.opt |= (EDMA_OPT_TCCHEN_MASK | EDMA_OPT_ITCCHEN_MASK |
         ((((uint32_t)chainChId2) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

         EDMA_setPaRAM(baseAddr, linkChId2, &paramConfig);

        /* to TX2 EDMA channel */
        paramConfig.opt  = 0U;
        paramConfig.opt |= (EDMA_OPT_TCCHEN_MASK | EDMA_OPT_ITCCHEN_MASK |
         ((((uint32_t)chainChId1) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

         EDMA_setPaRAM(baseAddr, linkChId1, &paramConfig);

        /* to TX0 EDMA channel */
        paramConfig.opt  = 0U;
        paramConfig.opt |= (EDMA_OPT_TCCHEN_MASK | EDMA_OPT_ITCCHEN_MASK |
         ((((uint32_t)chainChId0) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

         EDMA_setPaRAM(baseAddr, linkChId0, &paramConfig);

        /* Link 3 shadow links */
        /* Do not use LinkChannel API, it changes toParamId's TCC  */
        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)linkChId0), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)linkChId1));
        // EDMA_linkChannel(baseAddr, linkChId0, linkChId1);

        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)linkChId1), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)linkChId2));
        // EDMA_linkChannel(baseAddr, linkChId1, linkChId2);

        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)linkChId2), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)linkChId0));
        // EDMA_linkChannel(baseAddr, linkChId2, linkChId0);

        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)chanCfg->paramId), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)linkChId1));
        // EDMA_linkChannel(baseAddr, chanCfg->paramId, linkChId1);
    }

exit:
    return(errorCode);
}


/**
 *  @b Description
 *  @n
 *      Function to config a dummy channel with 3 linked paramset. Each paramset is linked
 *   to a EDMA data copy channel
 *
 *  @param[in]  dpParams                Pointer to data path parameters
 *  @param[in]  handle                  EDMA handle
 *  @param[in]  chanCfg                 EDMA channel configuraton
 *  @param[in]  chainingCfg             EDMA chaining configuration
 *  @param[in]  srcAddress              EDMA copy source address
 *  @param[in]  destAddress             EDMA copy destination address
 *  @param[in]  isTransferCompletionEnabled Number of iterations the dummy channel will be excuted.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *  @param[in]  intrObj                 Interrupt object
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval     N/A
 */
static int32_t rangeProcHWA_ConfigEDMATranspose
(
    rangeProc_dpParams      *dpParams,
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
    syncABCfg.aCount = (uint16_t)dpParams->numRxAntennas * (uint16_t)sizeof(cmplx16ImRe_t); //dpParams->sizeOfInputSample; //
    syncABCfg.bCount = (uint16_t)dpParams->numRangeBins;
    syncABCfg.cCount = (uint16_t)dpParams->numChirpsPerFrame / 2U;
    syncABCfg.srcBIdx = (int32_t)dpParams->numRxAntennas * (int32_t)sizeof(cmplx16ImRe_t);
    syncABCfg.srcCIdx = 0;
    syncABCfg.dstBIdx = (int32_t)dpParams->numRxAntennas * (int32_t)dpParams->numChirpsPerFrame * (int32_t)sizeof(cmplx16ImRe_t);
    syncABCfg.dstCIdx = (int16_t)dpParams->numRxAntennas * 2 * (int16_t)sizeof(cmplx16ImRe_t);

    syncABCfg.srcAddress = srcAddress;
    syncABCfg.destAddress = destAddress;

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
    rangeProcHWAObj     *rangeProcObj
)
{
    HWA_CommonConfig    hwaCommonConfig;
    rangeProc_dpParams  *DPParams;
    int32_t             retVal;

    DPParams = &rangeProcObj->params;

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG |/* numLoops, paramStartIdx, paramStopIdx combined here */
                               HWA_COMMONCONFIG_MASK_TWIDDITHERENABLE |
                               HWA_COMMONCONFIG_MASK_LFSRSEED;

    hwaCommonConfig.fftConfig.twidDitherEnable = HWA_FEATURE_BIT_ENABLE;
    hwaCommonConfig.fftConfig.lfsrSeed = 0x1234567; /*Some non-zero value*/
    hwaCommonConfig.numLoops = DPParams->numChirpsPerFrame/2U;
    hwaCommonConfig.paramStartIdx = rangeProcObj->hwaCfg.paramSetStartIdx;
    hwaCommonConfig.paramStopIdx = rangeProcObj->hwaCfg.paramSetStartIdx + (uint16_t)rangeProcObj->hwaCfg.numParamSet - 1U;

    retVal = HWA_configCommon(rangeProcObj->initParms.hwaHandle, &hwaCommonConfig);
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
 *  @param[in]  destChanPing                  Destination channel id for PING
 *  @param[in]  destChanPong                  Destination channel id for PONG
 *  @param[in]  hwaMemSrcPingOffset           Source Address offset for Ping input
 *  @param[in]  hwaMemSrcPongOffset           Source Address offset for Pong input
 *  @param[in]  hwaMemDestPingOffset          Destination address offset for Ping output
 *  @param[in]  hwaMemDestPongOffset          Destination address offset for Pong output
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
    uint8_t     destChanPing,
    uint8_t     destChanPong,
    uint32_t    hwaMemSrcPingOffset,
    uint32_t    hwaMemSrcPongOffset,
    uint32_t    hwaMemDestPingOffset,
    uint32_t    hwaMemDestPongOffset
)
{
    HWA_InterruptConfig     paramISRConfig;
    int32_t                 errCode = 0;
    uint32_t                paramsetIdx = 0;
    uint32_t                hwParamsetIdx;
    uint32_t                pingParamSetIdx = 0;
    HWA_ParamConfig         hwaParamCfg[DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS];
    HWA_Handle                      hwaHandle;
    rangeProc_dpParams             *pDPParams;
    uint8_t                         index;

    hwaHandle = rangeProcObj->initParms.hwaHandle;
    pDPParams = &rangeProcObj->params;

    (void)memset(hwaParamCfg,0,sizeof(hwaParamCfg));

    hwParamsetIdx = rangeProcObj->hwaCfg.paramSetStartIdx;
    for(index = 0; index < DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS; index++)
    {
        errCode = HWA_disableParamSetInterrupt(hwaHandle, index + rangeProcObj->hwaCfg.paramSetStartIdx,
                HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 |HWA_PARAMDONE_INTERRUPT_TYPE_DMA);
        if (errCode != 0)
        {
            goto exit;
        }
    }

    /***********************/
    /* PING DUMMY PARAMSET */
    /***********************/
    hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA;
    hwaParamCfg[paramsetIdx].triggerSrc = (uint8_t)hwParamsetIdx;
    hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_NONE;
    errCode = HWA_configParamSet(hwaHandle,
                                  (uint8_t)hwParamsetIdx,
                                  &hwaParamCfg[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /***********************/
    /* PING PROCESS PARAMSET */
    /***********************/
    paramsetIdx++;
    hwParamsetIdx++;
    pingParamSetIdx = paramsetIdx;


    if(rangeProcObj->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
    {
        /* At a HWA trigger time adc samples are already in HWA memory */
        hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_HARDWARE;
        hwaParamCfg[paramsetIdx].triggerSrc = rangeProcObj->hwaCfg.hardwareTrigSrc;
    }
    else
    {
        /* adcbuf not mapped, HWA is triggered after edma copy is done */
        hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA;
        hwaParamCfg[paramsetIdx].triggerSrc = (uint8_t)hwParamsetIdx;
    }

    hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_FFT;
    hwaParamCfg[paramsetIdx].source.srcAddr = hwaMemSrcPingOffset;

    if(pDPParams->isReal != 0U)
    {
        hwaParamCfg[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL;
    }
    else
    {
        hwaParamCfg[paramsetIdx].source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    }

    hwaParamCfg[paramsetIdx].source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
    hwaParamCfg[paramsetIdx].source.srcSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg[paramsetIdx].source.srcConjugate = 0;
    hwaParamCfg[paramsetIdx].source.srcScale = 8;

    hwaParamCfg[paramsetIdx].dest.dstAddr = hwaMemDestPingOffset;

    hwaParamCfg[paramsetIdx].dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
    hwaParamCfg[paramsetIdx].dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
    hwaParamCfg[paramsetIdx].dest.dstSign = HWA_SAMPLES_SIGNED;
    hwaParamCfg[paramsetIdx].dest.dstConjugate = 0;
    hwaParamCfg[paramsetIdx].dest.dstScale = (uint8_t)pDPParams->fftOutputDivShift;
    hwaParamCfg[paramsetIdx].dest.dstSkipInit = 0;

    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftEn = 1;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(pDPParams->numFFTBins);
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.butterflyScaling =
                                    ((uint16_t)1U << pDPParams->numLastButterflyStagesToScale) - 1U;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.windowEn = 1;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.windowStart = rangeProcObj->hwaCfg.hwaWinRamOffset;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.winSymm = rangeProcObj->hwaCfg.hwaWinSym;

    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
    hwaParamCfg[paramsetIdx].accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

    /* HWA range FFT src/dst configuration*/
    if(rangeProcObj->interleave == DPIF_RXCHAN_INTERLEAVE_MODE)
    {
        if(rangeProcObj->radarCubeLayout == rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt)
        {
            hwaParamCfg[paramsetIdx].source.srcAcnt = pDPParams->numAdcSamples - 1U; /* this is samples - 1 */
            hwaParamCfg[paramsetIdx].source.srcAIdx = (int32_t)pDPParams->numRxAntennas * (int32_t)pDPParams->sizeOfInputSample;
            hwaParamCfg[paramsetIdx].source.srcBcnt = (uint16_t)pDPParams->numRxAntennas - 1U;
            hwaParamCfg[paramsetIdx].source.srcBIdx = (int32_t)pDPParams->sizeOfInputSample;

            hwaParamCfg[paramsetIdx].dest.dstAcnt = pDPParams->numRangeBins - 1U;
            hwaParamCfg[paramsetIdx].dest.dstAIdx = (int32_t)pDPParams->numRxAntennas * (int32_t)sizeof(uint32_t);
            hwaParamCfg[paramsetIdx].dest.dstBIdx = (int32_t)sizeof(uint32_t);
        }
        else
        {
            /* Other radarCube layout format is not supported */
            errCode = DPU_RANGEPROCHWA_ENOTIMPL;
            goto exit;
        }
    }
    else
    {
        if(rangeProcObj->radarCubeLayout == rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt)
        {
            hwaParamCfg[paramsetIdx].source.srcAcnt = pDPParams->numAdcSamples - 1U;
            hwaParamCfg[paramsetIdx].source.srcAIdx = (int32_t)pDPParams->sizeOfInputSample;
            hwaParamCfg[paramsetIdx].source.srcBcnt = (uint16_t)pDPParams->numRxAntennas - 1U;
            hwaParamCfg[paramsetIdx].source.srcBIdx = (int32_t)rangeProcObj->rxChanOffset;

            hwaParamCfg[paramsetIdx].dest.dstAcnt = pDPParams->numRangeBins - 1U;
            hwaParamCfg[paramsetIdx].dest.dstAIdx = (int32_t)sizeof(uint32_t) * (int32_t)pDPParams->numRxAntennas;
            hwaParamCfg[paramsetIdx].dest.dstBIdx = (int32_t)sizeof(uint32_t);
        }
        else
        {
            hwaParamCfg[paramsetIdx].source.srcAcnt = pDPParams->numAdcSamples - 1U;
            hwaParamCfg[paramsetIdx].source.srcAIdx = (int32_t)pDPParams->sizeOfInputSample;
            hwaParamCfg[paramsetIdx].source.srcBcnt = (uint16_t)pDPParams->numRxAntennas - 1U;
            hwaParamCfg[paramsetIdx].source.srcBIdx = (int32_t)rangeProcObj->rxChanOffset;
            hwaParamCfg[paramsetIdx].dest.dstAcnt = pDPParams->numRangeBins - 1U;
            hwaParamCfg[paramsetIdx].dest.dstAIdx = (int32_t)sizeof(uint32_t);
            hwaParamCfg[paramsetIdx].dest.dstBIdx = (int32_t)pDPParams->numRangeBins * (int32_t)sizeof(uint32_t);
        }
    }

    errCode = HWA_configParamSet(hwaHandle,
                                  (uint8_t)hwParamsetIdx,
                                  &hwaParamCfg[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /* enable the DMA hookup to this paramset so that data gets copied out */
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
    paramISRConfig.dma.dstChannel = destChanPing;

    errCode = HWA_enableParamSetInterrupt(hwaHandle,(uint8_t)hwParamsetIdx,&paramISRConfig);
    if (errCode != 0)
    {
        goto exit;
    }

    /***********************/
    /* PONG DUMMY PARAMSET */
    /***********************/
    paramsetIdx++;
    hwParamsetIdx++;

    hwaParamCfg[paramsetIdx].triggerMode = HWA_TRIG_MODE_DMA;
    hwaParamCfg[paramsetIdx].triggerSrc = (uint8_t)hwParamsetIdx;
    hwaParamCfg[paramsetIdx].accelMode = HWA_ACCELMODE_NONE;
    errCode = HWA_configParamSet(hwaHandle,
                                  (uint8_t)hwParamsetIdx,
                                  &hwaParamCfg[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /***********************/
    /* PONG PROCESS PARAMSET */
    /***********************/
    paramsetIdx++;
    hwParamsetIdx++;
    hwaParamCfg[paramsetIdx] = hwaParamCfg[pingParamSetIdx];
    hwaParamCfg[paramsetIdx].source.srcAddr = hwaMemSrcPongOffset;
    hwaParamCfg[paramsetIdx].dest.dstAddr = hwaMemDestPongOffset;

    if(rangeProcObj->hwaCfg.dataInputMode != DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
    {
        hwaParamCfg[paramsetIdx].triggerSrc = (uint8_t)hwParamsetIdx;
    }

    errCode = HWA_configParamSet(hwaHandle,
                                  (uint8_t)hwParamsetIdx,
                                  &hwaParamCfg[paramsetIdx],NULL);
    if (errCode != 0)
    {
        goto exit;
    }

    /* Enable the DMA hookup to this paramset so that data gets copied out */
    paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
    paramISRConfig.dma.dstChannel = destChanPong;
    errCode = HWA_enableParamSetInterrupt(hwaHandle,
                                           (uint8_t)hwParamsetIdx,
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
 *      EDMA configuration for rangeProc data output in interleave mode
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  DPParams                  Pointer to datapath parameter
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *  @param[in]  hwaOutPingOffset          Ping HWA memory address offset
 *  @param[in]  hwaOutPongOffset          Pong HWA memory address offset
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
    uint32_t            hwaOutPingOffset,
    uint32_t            hwaOutPongOffset
)
{
    int32_t             errorCode = SystemP_SUCCESS;
    EDMA_Handle         handle ;
    DPEDMA_ChainingCfg       chainingCfg;

    /* Get rangeProc hardware resources pointer */
    handle = rangeProcObj->edmaHandle;

    /* Setup Chaining configuration */
    chainingCfg.chainingChannel = (uint8_t)pHwConfig->edmaOutCfg.dataOutSignature.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled = true;

    errorCode = rangeProcHWA_ConfigEDMATranspose(DPParams,
                                        handle,
                                        &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                                        &chainingCfg,
                                        hwaOutPingOffset,
                                        (uint32_t)rangeProcObj->radarCubebuf,
                                        false,  /* isTransferCompletionEnabled */
                                        NULL,   /* transferCompletionCallbackFxn */
                                        NULL,
                                        pHwConfig->intrObj);
    if (errorCode != SystemP_SUCCESS)
    {
        goto exit;
    }
    errorCode = rangeProcHWA_ConfigEDMATranspose(DPParams,
                                        handle,
                                        &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                                        &chainingCfg,
                                        hwaOutPongOffset,
                                        (uint32_t)(rangeProcObj->radarCubebuf + DPParams->numRxAntennas),
                                        true,
                                        rangeProcHWA_EDMA_transferCompletionCallbackFxn,
                                        (void*)rangeProcObj,
                                        pHwConfig->intrObj);
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
 *      EDMA configuration for rangeProc data output in non-interleave mode
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  DPParams                  Pointer to datapath parameter
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *  @param[in]  hwaOutPingOffset          Ping HWA memory address offset
 *  @param[in]  hwaOutPongOffset          Pong HWA memory address offset
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_ConfigEDMA_DataOut_nonInterleave
(
    rangeProcHWAObj         *rangeProcObj,
    rangeProc_dpParams      *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig,
    uint32_t                hwaOutPingOffset,
    uint32_t                hwaOutPongOffset
)
{
    int32_t                 errorCode = SystemP_SUCCESS;
    EDMA_Handle             handle;
    DPEDMA_syncABCfg        syncABCfg;
    DPEDMA_ChainingCfg      chainingCfg;

    /* Get rangeProc Configuration */
    handle = rangeProcObj->edmaHandle;

    /* Chaining configuration for all cases -> chaining to the data out signature channel */
    chainingCfg.chainingChannel = (uint8_t)pHwConfig->edmaOutCfg.dataOutSignature.channel;
    chainingCfg.isIntermediateChainingEnabled = true;
    chainingCfg.isFinalChainingEnabled = true;

     /**************************************************************************
      *  Configure EDMA to copy from HWA memory to radar cube
      *************************************************************************/
    if(rangeProcObj->radarCubeLayout == rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt)
    {
        /* Ping/Pong common configuration */
        syncABCfg.aCount = (uint16_t)DPParams->numRxAntennas * (uint16_t)sizeof(uint32_t);
        syncABCfg.bCount = (uint16_t)DPParams->numRangeBins;
        syncABCfg.cCount = (uint16_t)DPParams->numChirpsPerFrame / 2U;
        syncABCfg.srcBIdx = (int32_t)DPParams->numRxAntennas * (int32_t)sizeof(uint32_t);
        syncABCfg.srcCIdx = 0;
        syncABCfg.dstBIdx = (int32_t)DPParams->numRxAntennas * (int32_t)sizeof(uint32_t) * (int32_t)DPParams->numChirpsPerFrame;
        syncABCfg.dstCIdx = (int16_t)DPParams->numRxAntennas * 2 * (int16_t)sizeof(uint32_t);

        /* Ping specific config */
        syncABCfg.srcAddress = hwaOutPingOffset;
        syncABCfg.destAddress= (uint32_t)rangeProcObj->radarCubebuf;

        errorCode = DPEDMA_configSyncAB(handle,
                &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                &chainingCfg,
                &syncABCfg,
                true,    /* isEventTriggered */
                false,   /* isIntermediateTransferCompletionEnabled */
                false,   /* isTransferCompletionEnabled */
                NULL,
                NULL,
                pHwConfig->intrObj);

        if (errorCode != SystemP_SUCCESS)
        {
            goto exit;
        }

        /* Pong specific configuration */
        syncABCfg.srcAddress = hwaOutPongOffset;
        syncABCfg.destAddress= (uint32_t)(rangeProcObj->radarCubebuf + DPParams->numRxAntennas);

        errorCode = DPEDMA_configSyncAB(handle,
                &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                &chainingCfg,
                &syncABCfg,
                true,   /* isEventTriggered */
                false,  /* isIntermediateTransferCompletionEnabled */
                true,   /* isTransferCompletionEnabled */
                rangeProcHWA_EDMA_transferCompletionCallbackFxn,
                (void*)rangeProcObj,
                pHwConfig->intrObj);
        if (errorCode != SystemP_SUCCESS)
        {
            goto exit;
        }
    }
    else
    {
        uint32_t    numSamplePerTx;

        numSamplePerTx = (uint32_t)DPParams->numRangeBins * (uint32_t)DPParams->numRxAntennas * (uint32_t)DPParams->numDopplerChirps;

        if (DPParams->numTxAntennas == 4U)
        {

            /**************************************************************************
            *  Configure EDMA to copy HWA results to radar cube
            *  For cases with 3 TX Antenna
            *************************************************************************/
            DPEDMA_syncACfg        syncACfg;
            uint32_t    numSamplePerChirp;
            uint32_t    numBytesPerChirp;
            uint32_t    destAddr[2][2];
            uint8_t     index;
            bool        lastChan = false;

            numSamplePerChirp = (uint32_t)DPParams->numRangeBins * (uint32_t)DPParams->numRxAntennas;
            numBytesPerChirp = numSamplePerChirp * sizeof(uint32_t);

            destAddr[0][0] = (uint32_t)rangeProcObj->radarCubebuf;
            destAddr[0][1] = (uint32_t)(rangeProcObj->radarCubebuf + (2U * numSamplePerTx));
            destAddr[1][0] = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerTx);
            destAddr[1][1] = (uint32_t)(rangeProcObj->radarCubebuf + (3U * numSamplePerTx));

            /* Desitnation EDMA */
            /* Ping configuration to transfer 1D FFT output from HWA to L3 RAM transposed */
            errorCode = rangeProcHWA_ConfigEDMADummyTwoLinks(handle,
                &pHwConfig->edmaOutCfg.u.fmt2.dataOutPing,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[0].channel,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[1].channel,
                DPParams->numChirpsPerFrame / 2U
                );
            if (errorCode != SystemP_SUCCESS)
            {
                goto exit;
            }

            /* Desitnation EDMA */
            /* Pong configuration to transfer 1D FFT output from HWA to L3 RAM transposed */
            errorCode = rangeProcHWA_ConfigEDMADummyTwoLinks(handle,
                &pHwConfig->edmaOutCfg.u.fmt2.dataOutPong,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[0].channel,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[1].channel,
                DPParams->numChirpsPerFrame / 2U
                );
            if (errorCode != SystemP_SUCCESS)
            {
                goto exit;
            }

            /* Ping/Pong common configuration */
            syncACfg.aCount = (uint16_t)numBytesPerChirp;
            syncACfg.bCount = DPParams->numChirpsPerFrame / 4U;
            syncACfg.srcBIdx = 0;
            syncACfg.dstBIdx = (int32_t)numBytesPerChirp;

            for(index = 0; index < 2U; index++)
            {
                if(index == 1U)
                {
                    /* Set last channel flag to enable completion flag */
                    lastChan = true;
                }

                /* Configure 3 EDMA channels to copy data from M2 to one of TX antenna radar Cube */
                /* PING specific config
                   M2 - >Txi (i=0,2) */
                syncACfg.srcAddress = hwaOutPingOffset;
                syncACfg.destAddress= destAddr[0][index];

                errorCode = DPEDMA_configSyncA_singleFrame(handle,
                            &pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index],
                            &chainingCfg,
                            &syncACfg,
                            false,      /* isEventTriggered */
                            false,      /* isIntermediateTransferInterruptEnabled */
                            false,      /* isTransferCompletionEnabled */
                            NULL,
                            NULL,
                            pHwConfig->intrObj);

                if (errorCode != SystemP_SUCCESS)
                {
                    goto exit;
                }

                /* PONG specific config
                   M3 - >Txi (i=1,3) */
                syncACfg.srcAddress = hwaOutPongOffset;
                syncACfg.destAddress= destAddr[1][index];

                errorCode = DPEDMA_configSyncA_singleFrame(handle,
                            &pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index],
                            &chainingCfg,
                            &syncACfg,
                            false,      /* isEventTriggered */
                            false,      /* isIntermediateTransferInterruptEnabled */
                            lastChan,   /* isTransferCompletionEnabled */
                            (lastChan == true)? rangeProcHWA_EDMA_transferCompletionCallbackFxn : (Edma_EventCallback)NULL,
                            (lastChan == true)? (void*)rangeProcObj : (void*)NULL,
                            pHwConfig->intrObj);
                if (errorCode != SystemP_SUCCESS)
                {
                    goto exit;
                }

            }
        }

        else if (DPParams->numTxAntennas == 3U)
        {

            /**************************************************************************
            *  Configure EDMA to copy HWA results to radar cube
            *  For cases with 3 TX Antenna
            *************************************************************************/
            DPEDMA_syncACfg        syncACfg;
            uint32_t    numSamplePerChirp;
            uint32_t    numBytesPerChirp;
            uint32_t    destAddr[2][3];
            uint8_t     index;
            bool        lastChan = false;

            numSamplePerChirp = (uint32_t)DPParams->numRangeBins * (uint32_t)DPParams->numRxAntennas;
            numBytesPerChirp = numSamplePerChirp * sizeof(uint32_t);

            destAddr[0][0] = (uint32_t)rangeProcObj->radarCubebuf;
            destAddr[0][1] = (uint32_t)(rangeProcObj->radarCubebuf + (2U * numSamplePerTx));
            destAddr[0][2] = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerTx + numSamplePerChirp);
            destAddr[1][0] = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerTx);
            destAddr[1][1] = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerChirp);
            destAddr[1][2] = (uint32_t)(rangeProcObj->radarCubebuf + (2U * numSamplePerTx) + numSamplePerChirp);

            /* Desitnation EDMA */
            /* Ping configuration to transfer 1D FFT output from HWA to L3 RAM transposed */
            errorCode = rangeProcHWA_ConfigEDMADummyThreeLinks(handle,
                &pHwConfig->edmaOutCfg.u.fmt2.dataOutPing,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[0].channel,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[1].channel,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[2].channel,
                DPParams->numChirpsPerFrame/2U
                );
            if (errorCode != SystemP_SUCCESS)
            {
                goto exit;
            }

            /* Desitnation EDMA */
            /* Pong configuration to transfer 1D FFT output from HWA to L3 RAM transposed */
            errorCode = rangeProcHWA_ConfigEDMADummyThreeLinks(handle,
                &pHwConfig->edmaOutCfg.u.fmt2.dataOutPong,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[0].channel,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[1].channel,
                (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[2].channel,
                DPParams->numChirpsPerFrame/2U
                );
            if (errorCode != SystemP_SUCCESS)
            {
                goto exit;
            }

            /* Ping/Pong common configuration */
            syncACfg.aCount = (uint16_t)numBytesPerChirp;
            syncACfg.bCount = DPParams->numChirpsPerFrame/6U;
            syncACfg.srcBIdx = 0;
            syncACfg.dstBIdx = (int32_t)numBytesPerChirp * 2;

            for(index = 0; index < 3U; index++)
            {
                if(index == 2U)
                {
                    /* Set last channel flag to enable completion flag */
                    lastChan = true;
                }

                /* Configure 3 EDMA channels to copy data from M2 to one of TX antenna radar Cube */
                /* PING specific config
                   M2 - >Txi (i=0,2, 1) */
                syncACfg.srcAddress = hwaOutPingOffset;
                syncACfg.destAddress= destAddr[0][index];

                errorCode = DPEDMA_configSyncA_singleFrame(handle,
                            &pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index],
                            &chainingCfg,
                            &syncACfg,
                            false,      /* isEventTriggered */
                            false,      /* isIntermediateTransferInterruptEnabled */
                            false,      /* isTransferCompletionEnabled */
                            NULL,
                            NULL,
                            pHwConfig->intrObj);
                if (errorCode != SystemP_SUCCESS)
                {
                    goto exit;
                }

                /* PONG specific config
                   M3 - >Txi (i=1,0, 2) */
                syncACfg.srcAddress = hwaOutPongOffset;
                syncACfg.destAddress= destAddr[1][index];

                errorCode = DPEDMA_configSyncA_singleFrame(handle,
                            &pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index],
                            &chainingCfg,
                            &syncACfg,
                            false,      /* isEventTriggered */
                            false,      /* isIntermediateTransferInterruptEnabled */
                            lastChan,   /* isTransferCompletionEnabled */
                            (lastChan == true)? rangeProcHWA_EDMA_transferCompletionCallbackFxn : (Edma_EventCallback)NULL,
                            (lastChan == true)? (void*)rangeProcObj : (void*)NULL,
                            pHwConfig->intrObj);
                if (errorCode != SystemP_SUCCESS)
                {
                    goto exit;
                }
            }
        }
        else if(DPParams->numTxAntennas == 2U)
        {
            /**************************************************************************
            *  Configure EDMA to copy HWA results to radar cube
            *  For cases with 2 TX Antenna
            *************************************************************************/
            /* Ping/Pong common configuration */
            syncABCfg.aCount = (uint16_t)DPParams->numRangeBins * (uint16_t)sizeof(uint32_t);
            syncABCfg.bCount = DPParams->numRxAntennas;
            syncABCfg.cCount = DPParams->numChirpsPerFrame/2U;
            syncABCfg.srcBIdx = (int32_t)DPParams->numRangeBins * (int32_t)sizeof(uint32_t);
            syncABCfg.srcCIdx = 0;
            syncABCfg.dstBIdx = (int32_t)DPParams->numRangeBins * (int32_t)sizeof(uint32_t);
            syncABCfg.dstCIdx = (int16_t)DPParams->numRxAntennas * (int16_t)DPParams->numRangeBins * (int16_t)sizeof(uint32_t);

            /* Ping specific config */
            syncABCfg.srcAddress = hwaOutPingOffset;
            syncABCfg.destAddress = (uint32_t)rangeProcObj->radarCubebuf;

            errorCode = DPEDMA_configSyncAB(handle,
                    &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                    &chainingCfg,
                    &syncABCfg,
                    true,    /* isEventTriggered */
                    false,   /* isIntermediateTransferCompletionEnabled */
                    false,   /* isTransferCompletionEnabled */
                    NULL,
                    NULL,
                    pHwConfig->intrObj);

            if (errorCode != SystemP_SUCCESS)
            {
                goto exit;
            }

            /* Pong specific configuration */
            syncABCfg.srcAddress = hwaOutPongOffset;
            syncABCfg.destAddress = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerTx);

            errorCode = DPEDMA_configSyncAB(handle,
                    &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                    &chainingCfg,
                    &syncABCfg,
                    true,   /* isEventTriggered */
                    false,  /* isIntermediateTransferCompletionEnabled */
                    true,   /*isTransferCompletionEnabled */
                    rangeProcHWA_EDMA_transferCompletionCallbackFxn,
                    (void*)rangeProcObj,
                    pHwConfig->intrObj);
            if (errorCode != SystemP_SUCCESS)
            {
                goto exit;
            }
        }
        else if(DPParams->numTxAntennas == 1U)
        {
            uint32_t    numSamplePerChirp;

            numSamplePerChirp = (uint32_t)DPParams->numRangeBins * (uint32_t)DPParams->numRxAntennas;

            /**************************************************************************
            *  Configure EDMA to copy HWA results to radar cube
            *  For cases with 1 TX Antenna
            *************************************************************************/
            DPEDMA_syncACfg        syncACfg;

            /* Ping/Pong common configuration */
            syncACfg.aCount = (uint16_t)numSamplePerChirp * (uint16_t)sizeof(cmplx16ImRe_t);
            syncACfg.bCount = DPParams->numChirpsPerFrame/2U;
            syncACfg.srcBIdx = 0;
            syncACfg.dstBIdx = (int32_t)numSamplePerChirp * (int32_t)sizeof(cmplx16ImRe_t) * 2;

            /* Ping specific config */
            syncACfg.srcAddress = hwaOutPingOffset;
            syncACfg.destAddress= (uint32_t)rangeProcObj->radarCubebuf;

            errorCode = DPEDMA_configSyncA_singleFrame(handle,
                    &pHwConfig->edmaOutCfg.u.fmt1.dataOutPing,
                    &chainingCfg,
                    &syncACfg,
                    true,    /*isEventTriggered */
                    false,   /*isIntermediateTransferCompletionEnabled */
                    false,   /*isTransferCompletionEnabled */
                    NULL,
                    NULL,
                    pHwConfig->intrObj);

            if (errorCode != SystemP_SUCCESS)
            {
                goto exit;
            }

            /* Pong specific configuration */
            syncACfg.srcAddress = hwaOutPongOffset;
            syncACfg.destAddress = (uint32_t)(rangeProcObj->radarCubebuf + numSamplePerChirp);

            errorCode = DPEDMA_configSyncA_singleFrame(handle,
                    &pHwConfig->edmaOutCfg.u.fmt1.dataOutPong,
                    &chainingCfg,
                    &syncACfg,
                    true,   /*isEventTriggered */
                    false,  /* isIntermediateTransferCompletionEnabled */
                    true,   /*isTransferCompletionEnabled */
                    rangeProcHWA_EDMA_transferCompletionCallbackFxn,
                    (void *)rangeProcObj,
                    pHwConfig->intrObj);
            if (errorCode != SystemP_SUCCESS)
            {
                goto exit;
            }
        }
        else
        {
            /* Fall through */
        }
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
    uint32_t            temp;

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
        syncABCfg.destAddress = rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_SRC_PING];

        syncABCfg.aCount = bytePerRxChan;
        syncABCfg.bCount = DPParams->numRxAntennas;
        syncABCfg.cCount =2U; /* ping and pong */

        syncABCfg.srcBIdx = (int32_t)rangeProcObj->rxChanOffset;
        syncABCfg.dstBIdx = (int32_t)rangeProcObj->rxChanOffset;
        syncABCfg.srcCIdx = 0;
        temp              = (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_SRC_PONG] - (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_SRC_PING];
        syncABCfg.dstCIdx = (int16_t)(temp);

        errorCode = DPEDMA_configSyncAB(handle,
                                        &pHwConfig->edmaInCfg.dataIn,
                                        &chainingCfg,
                                        &syncABCfg,
                                        true,    /* isEventTriggered */
                                        /* Intermediate and Final transfer interrupts are enabled in case
                                         * the user wants to poll the IPR(H) register (for example,
                                         * in the range proc test case) or register an ISR for when a chirp
                                         * transfer from to HWA memory is complete */
                                        true,  /* isIntermediateTransferCompletionEnabled */
                                        true,   /* isTransferCompletionEnabled */
                                        NULL,
                                        NULL,
                                        pHwConfig->intrObj);
    }
    else
    {
        DPEDMA_syncACfg    syncACfg;

        syncACfg.srcAddress = (uint32_t)rangeProcObj->ADCdataBuf;
        syncACfg.destAddress = rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_SRC_PING];
        syncACfg.aCount = bytePerRxChan * DPParams->numRxAntennas;
        syncACfg.bCount = 2U; /* ping and pong */
        syncACfg.srcBIdx = 0;
        syncACfg.dstBIdx = ((int32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_SRC_PONG] - (int32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_SRC_PING]);

        errorCode = DPEDMA_configSyncA_singleFrame(handle,
                                        &pHwConfig->edmaInCfg.dataIn,
                                        &chainingCfg,
                                        &syncACfg,
                                        /* Intermediate and Final transfer interrupts are enabled in case
                                         * the user wants to poll the IPR(H) register (for example,
                                         * in the range proc test case) or register an ISR for when a chirp
                                         * transfer from to HWA memory is complete */
                                        true,    /* isEventTriggered */
                                        true,  /* isIntermediateTransferCompletionEnabled */
                                        true,   /* isTransferCompletionEnabled */
                                        NULL,
                                        NULL,
                                        pHwConfig->intrObj);
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
        retVal = rangeProcHWA_ConfigEDMA_DataIn(rangeProcObj,    DPParams, pHwConfig);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
            destChanPing,
            destChanPong,
            DPU_RANGEHWA_SRCADDR_PING,
            DPU_RANGEHWA_SRCADDR_PONG,
            DPU_RANGEHWA_DSTADDR_PING,
            DPU_RANGEHWA_DSTADDR_PONG
        );
    }
	else if (pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_MAPPED)
    {
        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
                destChanPing,
                destChanPong,
                DPU_RANGEHWA_SRCADDR_PING,
                DPU_RANGEHWA_SRCADDR_PING,
                DPU_RANGEHWA_DSTADDR_PING,
                DPU_RANGEHWA_DSTADDR_PONG
        );
    }
    else if (pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
    {
        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
                destChanPing,
                destChanPong,
                DPU_RANGEHWA_SRCADDR_PING,
                DPU_RANGEHWA_SRCADDR_PONG,
                DPU_RANGEHWA_DSTADDR_PING,
                DPU_RANGEHWA_DSTADDR_PONG
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
                                                  (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_DST_PING],
                                                  (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_DST_PONG]);
exit:
    return (retVal);
}



/**
 *  @b Description
 *  @n
 *      rangeProc configuration in non-interleaved mode
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
static int32_t rangeProcHWA_ConfigNonInterleaveMode
(
    rangeProcHWAObj          *rangeProcObj,
    rangeProc_dpParams       *DPParams,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
)
{
    HWA_Handle          hwaHandle;
    int32_t             retVal = 0;
    uint8_t             destChanPing;
    uint8_t             destChanPong;
    uint8_t             edmaChanPing;
    uint8_t             edmaChanPong;

    hwaHandle = rangeProcObj->initParms.hwaHandle;


    if((rangeProcObj->radarCubeLayout == rangeProc_dataLayout_TxAnt_DOPPLER_RxAnt_RANGE) &&
        (DPParams->numTxAntennas == 3U || DPParams->numTxAntennas == 4U) )
    {
        edmaChanPing = (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.channel;
        edmaChanPong = (uint8_t)pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.channel;
    }
    else
    {
        edmaChanPing = (uint8_t)pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channel;
        edmaChanPong = (uint8_t)pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channel;
    }

    /* Get HWA destination channel id */
    retVal = HWA_getDMAChanIndex(hwaHandle, (uint8_t)edmaChanPing, &destChanPing);
    if (retVal != 0)
    {
        goto exit;
    }
    /* In interleave mode, only edmaOutCfgFmt is supported */
    retVal = HWA_getDMAChanIndex(hwaHandle, (uint8_t)edmaChanPong, &destChanPong);
    if (retVal != 0)
    {
        goto exit;
    }

    /* In ADCBuf and HWA memory isolated mode,
       - copy data from ADCBuf to HWA memory by EDMA
       - trigger HWA */
    if(pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_ISOLATED)
    {
        /* Copy data from ADC buffer to HWA buffer */
        retVal = rangeProcHWA_ConfigEDMA_DataIn(rangeProcObj, DPParams, pHwConfig);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
            destChanPing,
            destChanPong,
            DPU_RANGEHWA_SRCADDR_PING,
            DPU_RANGEHWA_SRCADDR_PONG,
            DPU_RANGEHWA_DSTADDR_PING,
            DPU_RANGEHWA_DSTADDR_PONG
        );
    }
    else if(pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_MAPPED)
    {
        /* EDMA copy is not needed */

        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
            destChanPing,
            destChanPong,
            DPU_RANGEHWA_SRCADDR_PING,
            DPU_RANGEHWA_SRCADDR_PING,
            DPU_RANGEHWA_DSTADDR_PING,
            DPU_RANGEHWA_DSTADDR_PONG
        );
    }
    else if(pHwConfig->hwaCfg.dataInputMode == DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM)
    {
        /* EDMA copy is not needed */

        /* Range FFT configuration in HWA */
        retVal = rangeProcHWA_ConfigHWA(rangeProcObj,
            destChanPing,
            destChanPong,
            DPU_RANGEHWA_SRCADDR_PING,
            DPU_RANGEHWA_SRCADDR_PONG,
            DPU_RANGEHWA_DSTADDR_PING,
            DPU_RANGEHWA_DSTADDR_PONG
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

    /* Data output EDMA configuration */
    retVal = rangeProcHWA_ConfigEDMA_DataOut_nonInterleave(rangeProcObj,
                                             DPParams,
                                             pHwConfig,
                                             (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_DST_PING],
                                             (uint32_t)rangeProcObj->hwaMemBankAddr[DPU_RANGEHWA_MEM_BANK_INDX_DST_PONG]);
    if (retVal < 0)
    {
        goto exit;
    }

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Compensation of DC range antenna signature Init function
 *
 *  @param[in]  rangeProcObj                 Pointer to rangeProc object
 *  @param[in]  calibDcRangeSigCfg           Pointer DC range compensation configuration
 *  @param[in]  resetMeanBuffer              Flag to indicate if buffer need to be reset
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_dcRangeSignatureCompensation_init
(
    rangeProcHWAObj     *rangeProcObj,
    DPU_RangeProc_CalibDcRangeSigCfg *calibDcRangeSigCfg,
    uint8_t             resetMeanBuffer
)
{
    int32_t                 retVal = 0;
    uint32_t                meanbufSize;

    meanbufSize = DPU_RANGEPROC_SIGNATURE_COMP_MAX_BIN_SIZE * (uint32_t)rangeProcObj->params.numVirtualAntennas
                 * (uint32_t)sizeof(cmplx32ImRe_t);

    /* Validate DC removal configuration */
    if(calibDcRangeSigCfg->enabled != 0U)
    {
#if defined(DEBUG_CHECK_PARAMS) && DEBUG_CHECK_PARAMS
        if(rangeProcObj->dcRangeSigMean == (cmplx32ImRe_t*)NULL)
        {
            /* Check DC range average buffer pointer */
            retVal = DPU_RANGEPROCHWA_EDCREMOVAL;
            goto exit;
        }

        if(
#ifdef SUBSYS_MSS
        CSL_MEM_IS_NOT_ALIGN(rangeProcObj->dcRangeSigMean,
                              DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_R5F))
#else
        CSL_MEM_IS_NOT_ALIGN(rangeProcObj->dcRangeSigMean,
                              DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_DSP))

#endif
        {
            /* Check DC range average buffer pointer alignment */
            retVal = DPU_RANGEPROCHWA_EDCREMOVAL;
            goto exit;
        }

        if(meanbufSize > rangeProcObj->dcRangeSigMeanSize)
        {
            /* Check DC range average buffer size */
            retVal = DPU_RANGEPROCHWA_EDCREMOVAL;
            goto exit;
        }
#endif

        /* Initialize memory */
        if (resetMeanBuffer == 1U)
        {
            (void)memset((void *)rangeProcObj->dcRangeSigMean, 0, meanbufSize);
            rangeProcObj->dcRangeSigCalibCntr = 0;
        }
        rangeProcObj->calibDcNumLog2AvgChirps = mathUtils_floorLog2(calibDcRangeSigCfg->numAvgChirps);
    }
    else
    {
        /* Feature is disabled , nothing needs to done here */
    }

exit:
    return (retVal);
}

/**
 *  @b Description
 *  @n
 *      Compensation of DC range antenna signature
 *
 *  @param[in]  obj                 Pointer to rangeProc object
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not Applicable.
 */
static void rangeProcHWA_dcRangeSignatureCompensation
(
    rangeProcHWAObj     *obj
)
{
    uint32_t    antIdx;
    int32_t     rngBinIdx;
    uint32_t    ind;
    uint32_t    dcRangeSigMeanSize;
    uint32_t    doppIdx;
    cmplx16ImRe_t *fftOut1D;
    DPU_RangeProc_CalibDcRangeSigCfg *calibDc = &obj->calibDcRangeSigCfg;
    int32_t     negativeBinIdx = calibDc->negativeBinIdx;
    int32_t     positiveBinIdx = calibDc->positiveBinIdx;
    cmplx32ImRe_t *dcRangeSigMean = obj->dcRangeSigMean;
    rangeProc_dpParams   *dpParams;

    dpParams    = &obj->params;
    fftOut1D    = (cmplx16ImRe_t *) obj->radarCubebuf;

    dcRangeSigMeanSize = (uint32_t)dpParams->numVirtualAntennas * (uint32_t)(positiveBinIdx - negativeBinIdx + 1);
    if (obj->dcRangeSigCalibCntr == 0U)
    {
        (void)memset(dcRangeSigMean, 0, (uint32_t)dcRangeSigMeanSize * sizeof(cmplx32ImRe_t));
    }

    /* RadarCube FMT2 */
    if(obj->radarCubeLayout == rangeProc_dataLayout_RANGE_DOPPLER_TxAnt_RxAnt)
    {
        uint32_t     rngOffset;
        uint32_t     numVirtAnt;

        numVirtAnt = (uint32_t)dpParams->numVirtualAntennas;

        rngOffset = (uint32_t)dpParams->numVirtualAntennas * (uint32_t)dpParams->numDopplerChirps;
        /* Calibration */
        if (obj->dcRangeSigCalibCntr < calibDc->numAvgChirps)
        {
#ifdef SUBSYS_MSS
            /* radarCube in L3, need to invalidate the cache before use the data */
            CacheP_inv(fftOut1D, sizeof(cmplx16ImRe_t) * ((uint32_t)positiveBinIdx+1U) * rngOffset, CacheP_TYPE_ALLD);
            CacheP_inv(&fftOut1D[((int32_t)dpParams->numRangeBins + negativeBinIdx) * (int32_t)rngOffset],
                       (uint32_t)sizeof(cmplx16ImRe_t) * (uint32_t)(-negativeBinIdx) * rngOffset, CacheP_TYPE_ALLD);
#endif
            for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
            {
                /* Accumulate */
                ind = 0;
                for (rngBinIdx = 0; rngBinIdx <= positiveBinIdx; rngBinIdx++)
                {
                    for (antIdx  = 0; antIdx < numVirtAnt; antIdx++)
                    {
                        uint32_t binIdx = (uint32_t)rngBinIdx * rngOffset + doppIdx * numVirtAnt + antIdx;
                        dcRangeSigMean[ind].real += fftOut1D[binIdx].real;
                        dcRangeSigMean[ind].imag += fftOut1D[binIdx].imag;
                        ind++;
                    }
                }
                for (rngBinIdx = 0; rngBinIdx < -negativeBinIdx; rngBinIdx++)
                {
                    for (antIdx  = 0; antIdx < numVirtAnt; antIdx++)
                    {
                        uint32_t binIdx = (uint32_t)((int32_t)dpParams->numRangeBins + negativeBinIdx + rngBinIdx) * rngOffset +
                                          doppIdx * numVirtAnt + antIdx;
                        dcRangeSigMean[ind].real += fftOut1D[binIdx].real;
                        dcRangeSigMean[ind].imag += fftOut1D[binIdx].imag;
                        ind++;
                    }
                }
                obj->dcRangeSigCalibCntr++;
            }

            if (obj->dcRangeSigCalibCntr == calibDc->numAvgChirps)
            {
                /* Average */
                for (ind  = 0; ind < dcRangeSigMeanSize; ind++)
                {
                    dcRangeSigMean[ind].real = dcRangeSigMean[ind].real >> obj->calibDcNumLog2AvgChirps;
                    dcRangeSigMean[ind].imag = dcRangeSigMean[ind].imag >> obj->calibDcNumLog2AvgChirps;
                }
            }
        }
        else /*after numAvgChirps, do compensation*/
        {
            /* fftOut1D -= dcRangeSigMean */
            for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
            {
                ind = 0;
                for (rngBinIdx = 0; rngBinIdx <= positiveBinIdx; rngBinIdx++)
                {
                    for (antIdx = 0; antIdx < numVirtAnt; antIdx++)
                    {
                        uint32_t binIdx = (uint32_t)rngBinIdx * rngOffset + doppIdx * numVirtAnt + antIdx;

                        fftOut1D[binIdx].real -= (int16_t)dcRangeSigMean[ind].real;
                        fftOut1D[binIdx].imag -= (int16_t)dcRangeSigMean[ind].imag;
                        ind++;
                    }
                }
                for (rngBinIdx = 0; rngBinIdx < -negativeBinIdx; rngBinIdx++)
                {
                    for (antIdx = 0; antIdx < numVirtAnt; antIdx++)
                    {
                        uint32_t binIdx = (uint32_t)((int32_t)dpParams->numRangeBins + negativeBinIdx + rngBinIdx) * rngOffset +
                                         doppIdx * numVirtAnt + antIdx;
                        fftOut1D[binIdx].real -= (int16_t)dcRangeSigMean[ind].real;
                        fftOut1D[binIdx].imag -= (int16_t)dcRangeSigMean[ind].imag;
                        ind++;
                    }
                }
            }
#ifdef SUBSYS_MSS
            /* radarCube in L3, need to do cache write back after modify the data */
            CacheP_wb(fftOut1D, sizeof(cmplx16ImRe_t) * ((uint32_t)positiveBinIdx+1U) * rngOffset, CacheP_TYPE_ALLD);
            CacheP_wb(&fftOut1D[(uint32_t)((int32_t)dpParams->numRangeBins + negativeBinIdx) * rngOffset],
                      (uint32_t)sizeof(cmplx16ImRe_t) * (uint32_t)(-negativeBinIdx) * rngOffset, CacheP_TYPE_ALLD);
#endif
        }
    }
    else
    {
        /* RadarCube FMT1 */
        uint32_t    txAntIdx;
        uint32_t    rxAntIdx;
        uint32_t    txAntOffset;
        uint32_t    dopOffset;

        /* FMT1: cmplx16ImRe_t x[numTXPatterns][numDopplerChirps][numRX][numRangeBins] |1D Range FFT output*/
        txAntOffset = (uint32_t)dpParams->numRxAntennas * (uint32_t)dpParams->numDopplerChirps * (uint32_t)dpParams->numRangeBins;
        dopOffset  = (uint32_t)dpParams->numRxAntennas * (uint32_t)dpParams->numRangeBins;

        /* Calibration */
        if (obj->dcRangeSigCalibCntr < calibDc->numAvgChirps)
        {
#ifdef SUBSYS_MSS
            /* radarCube in L3, need to invalidate the cache before use the data */
            for (txAntIdx = 0; txAntIdx< dpParams->numTxAntennas; txAntIdx++)
            {
                for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
                {
                    for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                    {
                        CacheP_inv(&fftOut1D[txAntIdx*txAntOffset + doppIdx*dopOffset + rxAntIdx*dpParams->numRangeBins],
                                   (uint32_t)sizeof(cmplx16ImRe_t) * ((uint32_t)positiveBinIdx+1U), CacheP_TYPE_ALLD);
                        CacheP_inv(&fftOut1D[txAntIdx*txAntOffset + doppIdx*dopOffset + rxAntIdx*dpParams->numRangeBins
                                             + (uint32_t)((int32_t)dpParams->numRangeBins + negativeBinIdx)],
                                   (uint32_t)sizeof(cmplx16ImRe_t) * (uint32_t)(-negativeBinIdx), CacheP_TYPE_ALLD);
                    }
                }
            }
#endif
            for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
            {
                /* Accumulate for all doppler bins */
                ind = 0;

                for(txAntIdx = 0; txAntIdx< dpParams->numTxAntennas; txAntIdx++)
                {
                    for (rngBinIdx = 0; rngBinIdx <= positiveBinIdx; rngBinIdx++)
                    {
                        for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                        {
                            uint32_t binIdx = txAntIdx * txAntOffset + doppIdx * dopOffset +
                                          dpParams->numRangeBins * rxAntIdx +
                                          (uint32_t)rngBinIdx;
                            dcRangeSigMean[ind].real += fftOut1D[binIdx].real;
                            dcRangeSigMean[ind].imag += fftOut1D[binIdx].imag;
                            ind++;
                        }
                    }
                    for (rngBinIdx = 0; rngBinIdx < -negativeBinIdx; rngBinIdx++)
                    {
                        for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                        {
                            uint32_t binIdx = txAntIdx * txAntOffset + doppIdx * dopOffset +
                                          dpParams->numRangeBins * rxAntIdx +
                                          (uint32_t)((int32_t)dpParams->numRangeBins + negativeBinIdx + rngBinIdx);

                            dcRangeSigMean[ind].real += fftOut1D[binIdx].real;
                            dcRangeSigMean[ind].imag += fftOut1D[binIdx].imag;
                            ind++;
                        }
                    }
                }
                obj->dcRangeSigCalibCntr++;
            }

            if (obj->dcRangeSigCalibCntr == calibDc->numAvgChirps)
            {
                /* Average */
                for (ind = 0; ind < dcRangeSigMeanSize; ind++)
                {
                    dcRangeSigMean[ind].real = dcRangeSigMean[ind].real >> obj->calibDcNumLog2AvgChirps;
                    dcRangeSigMean[ind].imag = dcRangeSigMean[ind].imag >> obj->calibDcNumLog2AvgChirps;
                }
            }
        }
        else /*after numAvgChirps, do compensation*/
        {
            /* fftOut1D -= dcRangeSigMean */
            for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
            {
                ind = 0;
                for(txAntIdx = 0; txAntIdx< dpParams->numTxAntennas; txAntIdx++)
                {
                    for (rngBinIdx = 0; rngBinIdx <= positiveBinIdx; rngBinIdx++)
                    {
                        for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                        {
                            uint32_t binIdx = txAntIdx * txAntOffset + doppIdx * dopOffset +
                                          dpParams->numRangeBins * rxAntIdx +
                                          (uint32_t)rngBinIdx;
                            fftOut1D[binIdx].real -= (int16_t)dcRangeSigMean[ind].real;
                            fftOut1D[binIdx].imag -= (int16_t)dcRangeSigMean[ind].imag;
                            ind++;
                        }
                    }
                    for (rngBinIdx = 0; rngBinIdx < -negativeBinIdx; rngBinIdx++)
                    {
                        for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                        {
                            uint32_t binIdx = txAntIdx * txAntOffset + doppIdx * dopOffset +
                                          dpParams->numRangeBins * rxAntIdx +
                                          (uint32_t)((int32_t)dpParams->numRangeBins + negativeBinIdx + rngBinIdx);
                            fftOut1D[binIdx].real -= (int16_t)dcRangeSigMean[ind].real;
                            fftOut1D[binIdx].imag -= (int16_t)dcRangeSigMean[ind].imag;
                            ind++;
                        }
                    }
                }
            }
#ifdef SUBSYS_MSS
            /* radarCube in L3, need to do cache write back after modify the data */
            for (txAntIdx = 0; txAntIdx< dpParams->numTxAntennas; txAntIdx++)
            {
                for (doppIdx = 0; doppIdx < dpParams->numDopplerChirps; doppIdx++)
                {
                    for (rxAntIdx = 0; rxAntIdx < dpParams->numRxAntennas; rxAntIdx++)
                    {
                        CacheP_wb(&fftOut1D[txAntIdx*txAntOffset + doppIdx*dopOffset + rxAntIdx*dpParams->numRangeBins],
                                   (uint32_t)sizeof(cmplx16ImRe_t) * ((uint32_t)positiveBinIdx+1U), CacheP_TYPE_ALLD);
                        CacheP_wb(&fftOut1D[txAntIdx*txAntOffset + doppIdx*dopOffset + rxAntIdx*dpParams->numRangeBins
                                             + (uint32_t)((int32_t)dpParams->numRangeBins + negativeBinIdx)],
                                   (uint32_t)sizeof(cmplx16ImRe_t) * (uint32_t)(-negativeBinIdx), CacheP_TYPE_ALLD);
                    }
                }
            }
#endif
        }
    }
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

    /* Get configuration pointers */
    pStaticCfg = &pConfigIn->staticCfg;
    params    = &rangeProcObj->params;

    /* Save datapath parameters */
    params->numTxAntennas = pStaticCfg->numTxAntennas;
    params->numRxAntennas = pStaticCfg->ADCBufData.dataProperty.numRxAntennas;
    params->numVirtualAntennas = pStaticCfg->numVirtualAntennas;
    params->numChirpsPerChirpEvent = pStaticCfg->ADCBufData.dataProperty.numChirpsPerChirpEvent;
    params->numAdcSamples = pStaticCfg->ADCBufData.dataProperty.numAdcSamples;
    params->numRangeBins = pStaticCfg->numRangeBins;
    params->numFFTBins = pStaticCfg->numFFTBins;

    if(pStaticCfg->isChirpDataReal != 0U)
    {
        params->isReal = 1;
        params->sizeOfInputSample = (uint16_t)sizeof(int16_t);
    }
    else
    {
        params->isReal = 0;
        params->sizeOfInputSample = (uint16_t)sizeof(cmplx16ImRe_t);
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
        (rangeProcObj->params.numRxAntennas > 1U) )
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

    /* Prepare internal hardware resouces = trigger source matchs its  paramset index */
    rangeProcObj->dataInTrigger[0]      = 1U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataInTrigger[1]      = 3U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataOutTrigger[0]     = 0U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;
    rangeProcObj->dataOutTrigger[1]     = 2U + pConfigIn->hwRes.hwaCfg.paramSetStartIdx;

    /* Save hardware resources that will be used at runtime */
    rangeProcObj->edmaHandle= pConfigIn->hwRes.edmaHandle;
    rangeProcObj->dataOutSignatureChan = (uint8_t)pConfigIn->hwRes.edmaOutCfg.dataOutSignature.tcc;
    rangeProcObj->dcRangeSigMean = pConfigIn->hwRes.dcRangeSigMean;
    rangeProcObj->dcRangeSigMeanSize = pConfigIn->hwRes.dcRangeSigMeanSize;
    (void)memcpy((void *)&rangeProcObj->hwaCfg, (void *)&pConfigIn->hwRes.hwaCfg, sizeof(DPU_RangeProcHWA_HwaConfig));

exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *      Internal function to config HWA/EDMA to perform range FFT
 *
 *  @param[in]  rangeProcObj              Pointer to rangeProc object
 *  @param[in]  pHwConfig                 Pointer to rangeProc hardware resources
 *
 *  \ingroup    DPU_RANGEPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t rangeProcHWA_HardwareConfig
(
    rangeProcHWAObj         *rangeProcObj,
    DPU_RangeProcHWA_HW_Resources *pHwConfig
)
{
    int32_t                 retVal = 0;
    rangeProc_dpParams      *DPParams;
    DPParams    = &rangeProcObj->params;

    if (rangeProcObj->interleave == DPIF_RXCHAN_INTERLEAVE_MODE)
    {
        retVal = rangeProcHWA_ConfigInterleaveMode(rangeProcObj, DPParams, pHwConfig);
        if (retVal != 0)
        {
            goto exit;
        }
    }
    else
    {
        retVal =rangeProcHWA_ConfigNonInterleaveMode(rangeProcObj, DPParams, pHwConfig);
        if (retVal != 0)
        {
            goto exit;
        }
    }
exit:
    return(retVal);
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
 *  @param[in]  subframeCounter         sub frame counter
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
    volatile uint8_t                subframeCounter,
    int32_t*                        errCode
)
{
    rangeProcHWAObj     *rangeProcObj = NULL;
    HWA_MemInfo         hwaMemInfo;
    uint16_t            index;
    int32_t             status = SystemP_SUCCESS;

    *errCode = 0;

    if( (initParams == NULL) ||
       (initParams->hwaHandle == NULL) )
    {
        *errCode = DPU_RANGEPROCHWA_EINVAL;
        goto exit;
    }

    DebugP_assert(subframeCounter < RL_MAX_SUBFRAMES);

    /* Allocate Memory for rangeProc */
    rangeProcObj = (rangeProcHWAObj*)&RangeObj[subframeCounter];

    /* Initialize memory */
    (void)memset((void *)rangeProcObj, 0, sizeof(rangeProcHWAObj));

    (void)memcpy((void *)&rangeProcObj->initParms, (void*)initParams, sizeof(DPU_RangeProcHWA_InitParams));

    /* Set HWA bank memory address */
    *errCode =  HWA_getHWAMemInfo(initParams->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {
        goto exit;
    }

    for (index = 0; index < hwaMemInfo.numBanks; index++)
    {
        rangeProcObj->hwaMemBankAddr[index] = hwaMemInfo.baseAddress + (uint32_t)index * (uint32_t)hwaMemInfo.bankSize;
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
       (pConfigIn->hwRes.hwaCfg.numParamSet != DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS)
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
            expectedWinSize = ((pStaticCfg->ADCBufData.dataProperty.numAdcSamples + 1U) / 2U ) * (uint16_t)sizeof(uint32_t);
        }
        else
        {
            expectedWinSize = pStaticCfg->ADCBufData.dataProperty.numAdcSamples * (uint16_t)sizeof(uint32_t);
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
           2. It is always 1D range output.
     */
    if( (pConfigIn->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_1) &&
        (pConfigIn->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_2) )
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

    /* Parameter check: radarcube buffer Size */
    if (pConfigIn->hwRes.radarCube.dataSize != (pStaticCfg->numRangeBins* sizeof(cmplx16ImRe_t) *
                                      pStaticCfg->numChirpsPerFrame *
                                      pStaticCfg->ADCBufData.dataProperty.numRxAntennas) )
    {
        retVal = DPU_RANGEPROCHWA_ERADARCUBE_INTF;
        goto exit;
    }

    /* Parameter check: Num butterfly stages to scale */
    if (pStaticCfg->rangeFFTtuning.numLastButterflyStagesToScale > mathUtils_ceilLog2(pStaticCfg->numRangeBins))
    {
        retVal = DPU_RANGEPROCHWA_EBUTTERFLYSCALE;
        goto exit;
    }
#endif

    /* Save hardware resources */
    (void)memcpy((void *)&rangeProcObj->calibDcRangeSigCfg,
           (void *)pConfigIn->dynCfg.calibDcRangeSigCfg, sizeof(DPU_RangeProc_CalibDcRangeSigCfg));

    retVal = rangeProcHWA_ParseConfig(rangeProcObj, pConfigIn);
    if (retVal < 0)
    {
        goto exit;
    }

    /* DC calibration and compensation init */
    retVal = rangeProcHWA_dcRangeSignatureCompensation_init(rangeProcObj,
                pConfigIn->dynCfg.calibDcRangeSigCfg, pStaticCfg->resetDcRangeSigMeanBuffer);
    if (retVal < 0)
    {
        goto exit;
    }

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Reset the internal state of the HWA */
    retVal = HWA_reset(hwaHandle);
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
    retVal = rangeProcHWA_HardwareConfig(rangeProcObj, &pConfigIn->hwRes);

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

    /* calib DC processing if enabled */
    if(rangeProcObj->calibDcRangeSigCfg.enabled != 0U)
    {
        rangeProcHWA_dcRangeSignatureCompensation(rangeProcObj);
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
        case DPU_RangeProcHWA_Cmd_dcRangeCfg:
        {
            DPU_RangeProc_CalibDcRangeSigCfg   *calibDc;

            if((argSize != sizeof(DPU_RangeProc_CalibDcRangeSigCfg)) ||
               (arg == NULL))
            {
                retVal = DPU_RANGEPROCHWA_EDCREMOVAL;
                goto exit;
            }
            else
            {
                calibDc = (DPU_RangeProc_CalibDcRangeSigCfg *)arg;

                if(memcmp((void *)&rangeProcObj->calibDcRangeSigCfg,
                           (void *)calibDc, sizeof(DPU_RangeProc_CalibDcRangeSigCfg)) == 0)
                {
                    /* NO configuration change, nothing needs to be done here */
                }
                else
                {
                    /* If dcRangeSignature configuration is changed , always reset buffer and counter */
                    retVal = rangeProcHWA_dcRangeSignatureCompensation_init(rangeProcObj, calibDc, 1U);
                    if(retVal < 0)
                    {
                        goto exit;
                    }
                    else
                    {
                        /* Save configuration */
                        (void)memcpy((void *)&rangeProcObj->calibDcRangeSigCfg, (void *)arg, argSize);
                    }
                }
            }
        }
        break;

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
    SemaphoreP_destruct(&rangeProcObj->edmaDoneSemaHandle);
    SemaphoreP_destruct(&rangeProcObj->hwaDoneSemaHandle);

exit:

    return (retVal);
}
