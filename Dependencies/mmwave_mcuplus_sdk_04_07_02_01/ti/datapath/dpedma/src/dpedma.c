/**
 *   @file  dpedma.c
 *
 *   @brief
 *      EDMA Configuration Utility API implementation.
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

#include <kernel/dpl/CacheP.h>
#include <drivers/soc.h>
#include <ti/datapath/dpedma/dpedma.h>

/**
 *  @b Description
 *  @n
 *     Utility function for linking EDMA channel with a shadow link paramset
 *
 *  @param[in]  handle          EDMA handle.
 *  @param[in]  paramId         EDMA param id
 *  @param[in]  shadowParamId   EDMA channel shadow id
 *  @param[in]  config          Pointer to EDMA paramset configuration
 *
 *  \ingroup    DPEDMA_INTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMA_setup_shadow_link
(
    EDMA_Handle     handle,
    uint8_t         paramId,
    uint16_t        shadowParamId,
    EDMACCPaRAMEntry  *config
)
{
    int32_t             errorCode = DPEDMA_NO_ERROR;
    uint32_t            baseAddr;
    EDMACCPaRAMEntry   edmaParam;

    baseAddr = EDMA_getBaseAddr(handle);
    if(baseAddr == 0U)
    {
        errorCode = DPEDMA_EINVAL_EDMAADDR;
        goto exit;
    }
    DebugP_assert(baseAddr != 0U);

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam = *config;

    EDMA_setPaRAM(baseAddr, shadowParamId, &edmaParam);

    EDMA_linkChannel(baseAddr, paramId, shadowParamId);

    EDMA_linkChannel(baseAddr, shadowParamId, shadowParamId);

exit:
    return(errorCode);
}


/**
 *  @b Description
 *  @n
 *     EDMA utility function for sync AB type transfers.
 *
 *  @param[in]  handle          EDMA handle.
 *  @param[in]  chanCfg         Pointer to datapath EDMA channel configuration
 *  @param[in]  chainingCfg     Pointer to datapath EDMA channel chaining configuration
 *  @param[in]  syncABCfg       Pointer to syncAB type configuration
 *  @param[in]  isEventTriggered  Flag indicates if the channel is event triggered
 *  @param[in]  isIntermediateTransferCompletionEnabled Set to 'true' if intermedate transfer completion
 *                 indication is to be enabled.
 *  @param[in]  isTransferCompletionEnabled Set to 'true' if final transfer completion
 *                              indication is to be enabled.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *  @param[in]  intrObj pointer to EDMA interrupt object of type Edma_IntrObject.
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
 int32_t DPEDMA_configSyncAB
(
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    DPEDMA_syncABCfg        *syncABCfg,
    bool                    isEventTriggered,
    bool                    isIntermediateTransferCompletionEnabled,
    bool                    isTransferCompletionEnabled,
    Edma_EventCallback      transferCompletionCallbackFxn,
    void*                   transferCompletionCallbackFxnArg,
    Edma_IntrObject         *intrObj
)
{
    volatile uint32_t   baseAddr, regionId;
    EDMACCPaRAMEntry   edmaParam;
    int32_t             errorCode = DPEDMA_NO_ERROR;
    uint32_t            signBits = 0;

    if((chanCfg == NULL) || (syncABCfg == NULL))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    /* Ensuring that the BIDX value is a signed integer in the limits of 24 bits */
    signBits = (uint32_t)syncABCfg->srcBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    signBits = (uint32_t)syncABCfg->dstBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    baseAddr = EDMA_getBaseAddr(handle);
    if(baseAddr == 0U)
    {
        errorCode = DPEDMA_EINVAL_EDMAADDR;
        goto exit;
    }

    regionId = EDMA_getRegionId(handle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t) SOC_virtToPhy((void *)syncABCfg->srcAddress);
    edmaParam.destAddr      = (uint32_t) SOC_virtToPhy((void *)syncABCfg->destAddress);
    edmaParam.aCnt          = (uint16_t) syncABCfg->aCount;
    edmaParam.bCnt          = (uint16_t) syncABCfg->bCount;
    edmaParam.cCnt          = (uint16_t) syncABCfg->cCount;
    edmaParam.bCntReload    = (uint16_t) 0U;
    edmaParam.srcBIdx       = (int16_t) EDMA_PARAM_BIDX(syncABCfg->srcBIdx);
    edmaParam.destBIdx      = (int16_t) EDMA_PARAM_BIDX(syncABCfg->dstBIdx);
    edmaParam.srcCIdx       = (int16_t) syncABCfg->srcCIdx;
    edmaParam.destCIdx      = (int16_t) syncABCfg->dstCIdx;
    edmaParam.srcBIdxExt    = (int8_t) EDMA_PARAM_BIDX_EXT(syncABCfg->srcBIdx);
    edmaParam.destBIdxExt   = (int8_t) EDMA_PARAM_BIDX_EXT(syncABCfg->dstBIdx);
    edmaParam.linkAddr      = 0xFFFFU;

    /*
     * (BIT-0, BIT-1)  -   Source Address Mode & destination Address Mode is by default Linear
     * (BIT-3)         -   Static Entry is by default normal
     * (BIT-8 - BIT-10)-   FIFO width is by default 8-bit
     * (BIT-11)        -   Early completion is by default false
     */
    edmaParam.opt          |= (EDMA_OPT_SYNCDIM_MASK);

    if(isTransferCompletionEnabled)
    {
        edmaParam.opt          |= (EDMA_OPT_TCINTEN_MASK);
    }

    if(isIntermediateTransferCompletionEnabled)
    {
        edmaParam.opt          |= (EDMA_OPT_ITCINTEN_MASK);
    }

    /* Chaining channel Cfg. */
    if(chainingCfg != NULL)
    {
        edmaParam.opt          |= ((((uint32_t)chainingCfg->chainingChannel) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK);
        if(chainingCfg->isFinalChainingEnabled)
        {
            edmaParam.opt          |= (EDMA_TPCC_OPT_TCCHEN_MASK);
        }

        if(chainingCfg->isIntermediateChainingEnabled)
        {
            edmaParam.opt          |= (EDMA_TPCC_OPT_ITCCHEN_MASK);
        }
    }
    EDMA_setPaRAM(baseAddr, chanCfg->paramId, &edmaParam);

    if(isEventTriggered)
    {
        (void)EDMA_enableTransferRegion(baseAddr, regionId, chanCfg->channel, EDMA_TRIG_MODE_EVENT);
    }

    if((transferCompletionCallbackFxn != NULL) && (chainingCfg != NULL) && (intrObj!=NULL))
    {
        /* Register interrupt */
        intrObj->tccNum = chainingCfg->chainingChannel;
        intrObj->cbFxn  = transferCompletionCallbackFxn;
        intrObj->appData = (void *) transferCompletionCallbackFxnArg;
        errorCode = EDMA_registerIntr(handle, intrObj);
        DebugP_assert(errorCode == SystemP_SUCCESS);
    }

    errorCode = DPEDMA_setup_shadow_link(handle, (uint8_t)chanCfg->paramId, (uint16_t)chanCfg->shadowPramId, &edmaParam);

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *     EDMA utility function for sync AB type transfers with 3 shadow links.
 *
 *  @param[in]  handle          EDMA handle.
 *  @param[in]  chanCfg         Pointer to datapath EDMA channel configuration
 *  @param[in]  chainingCfg     Pointer to datapath EDMA channel chaining configuration
 *  @param[in]  syncABCfg       Pointer to syncAB type configuration of 1st shadow
 *  @param[in]  syncABCfg1      Pointer to syncAB type configuration of 2nd shadow
 *  @param[in]  syncABCfg2      Pointer to syncAB type configuration of 3rd shadow
 *  @param[in]  isEventTriggered  Flag indicates if the channel is event triggered
 *  @param[in]  isIntermediateTransferCompletionEnabled Set to 'true' if intermedate transfer completion
 *                 indication is to be enabled.
 *  @param[in]  isTransferCompletionEnabled Set to 'true' if final transfer completion
 *                              indication is to be enabled.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *  @param[in]  intrObj pointer to EDMA interrupt object of type Edma_IntrObject.
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
 int32_t DPEDMA_configSyncAB_ThreeLinks
(
    EDMA_Handle             handle,
    DPEDMA_3LinkChanCfg     *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    DPEDMA_syncABCfg        *syncABCfg,
    DPEDMA_syncABCfg        *syncABCfg1,
    DPEDMA_syncABCfg        *syncABCfg2,
    bool                    isEventTriggered,
    bool                    isIntermediateTransferCompletionEnabled,
    bool                    isTransferCompletionEnabled,
    Edma_EventCallback      transferCompletionCallbackFxn,
    void*                   transferCompletionCallbackFxnArg,
    Edma_IntrObject         *intrObj
)
{
    volatile uint32_t   baseAddr, regionId;
    EDMACCPaRAMEntry    edmaParam, edmaParam1, edmaParam2;
    int32_t             errorCode = DPEDMA_NO_ERROR;
    uint32_t            signBits = 0;

    if((chanCfg == NULL) || (syncABCfg == NULL) || (syncABCfg1 == NULL) || (syncABCfg2 == NULL))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    /* Ensuring that the BIDX value is a signed integer in the limits of 24 bits */
    signBits = (uint32_t)syncABCfg->srcBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    signBits = (uint32_t)syncABCfg->dstBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    signBits = (uint32_t)syncABCfg1->srcBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    signBits = (uint32_t)syncABCfg1->dstBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    signBits = (uint32_t)syncABCfg2->srcBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    signBits = (uint32_t)syncABCfg2->dstBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    baseAddr = EDMA_getBaseAddr(handle);
    if(baseAddr == 0U)
    {
        errorCode = DPEDMA_EINVAL_EDMAADDR;
        goto exit;
    }

    regionId = EDMA_getRegionId(handle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t) SOC_virtToPhy((void *)syncABCfg->srcAddress);
    edmaParam.destAddr      = (uint32_t) SOC_virtToPhy((void *)syncABCfg->destAddress);
    edmaParam.aCnt          = (uint16_t) syncABCfg->aCount;
    edmaParam.bCnt          = (uint16_t) syncABCfg->bCount;
    edmaParam.cCnt          = (uint16_t) syncABCfg->cCount;
    edmaParam.bCntReload    = (uint16_t) 0U;
    edmaParam.srcBIdx       = (int16_t) EDMA_PARAM_BIDX(syncABCfg->srcBIdx);
    edmaParam.destBIdx      = (int16_t) EDMA_PARAM_BIDX(syncABCfg->dstBIdx);
    edmaParam.srcCIdx       = (int16_t) syncABCfg->srcCIdx;
    edmaParam.destCIdx      = (int16_t) syncABCfg->dstCIdx;
    edmaParam.srcBIdxExt    = (int8_t) EDMA_PARAM_BIDX_EXT(syncABCfg->srcBIdx);
    edmaParam.destBIdxExt   = (int8_t) EDMA_PARAM_BIDX_EXT(syncABCfg->dstBIdx);
    edmaParam.linkAddr      = 0xFFFFU;

    /*
     * (BIT-0, BIT-1)  -   Source Address Mode & destination Address Mode is by default Linear
     * (BIT-3)         -   Static Entry is by default normal
     * (BIT-8 - BIT-10)-   FIFO width is by default 8-bit
     * (BIT-11)        -   Early completion is by default false
     */
    edmaParam.opt          |= (EDMA_OPT_SYNCDIM_MASK);

    if(isTransferCompletionEnabled)
    {
        edmaParam.opt          |= (EDMA_OPT_TCINTEN_MASK);
    }

    if(isIntermediateTransferCompletionEnabled)
    {
        edmaParam.opt          |= (EDMA_OPT_ITCINTEN_MASK);
    }

    /* Chaining channel Cfg. */
    if(chainingCfg != NULL)
    {
        edmaParam.opt          |= ((((uint32_t)chainingCfg->chainingChannel) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK);
        if(chainingCfg->isFinalChainingEnabled)
        {
            edmaParam.opt          |= (EDMA_TPCC_OPT_TCCHEN_MASK);
        }

        if(chainingCfg->isIntermediateChainingEnabled)
        {
            edmaParam.opt          |= (EDMA_TPCC_OPT_ITCCHEN_MASK);
        }
    }
    EDMA_setPaRAM(baseAddr, chanCfg->paramId, &edmaParam);

    if(isEventTriggered)
    {
        (void)EDMA_enableTransferRegion(baseAddr, regionId, chanCfg->channel, EDMA_TRIG_MODE_EVENT);
    }

    if((transferCompletionCallbackFxn != NULL) && (chainingCfg != NULL) && (intrObj!=NULL))
    {
        /* Register interrupt */
        intrObj->tccNum = chainingCfg->chainingChannel;
        intrObj->cbFxn  = transferCompletionCallbackFxn;
        intrObj->appData = (void *) transferCompletionCallbackFxnArg;
        errorCode = EDMA_registerIntr(handle, intrObj);
        DebugP_assert(errorCode == SystemP_SUCCESS);
    }

    /* Link Main channel to Link 1*/
    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam1);
    edmaParam1 = edmaParam;
    edmaParam1.srcAddr       = (uint32_t) SOC_virtToPhy((void *)syncABCfg1->srcAddress);
    edmaParam1.destAddr      = (uint32_t) SOC_virtToPhy((void *)syncABCfg1->destAddress);
    edmaParam1.aCnt          = (uint16_t) syncABCfg1->aCount;
    edmaParam1.bCnt          = (uint16_t) syncABCfg1->bCount;
    edmaParam1.cCnt          = (uint16_t) syncABCfg1->cCount;
    edmaParam1.bCntReload    = (uint16_t) 0U;
    edmaParam1.srcBIdx       = (int16_t) EDMA_PARAM_BIDX(syncABCfg1->srcBIdx);
    edmaParam1.destBIdx      = (int16_t) EDMA_PARAM_BIDX(syncABCfg1->dstBIdx);
    edmaParam1.srcCIdx       = (int16_t) syncABCfg1->srcCIdx;
    edmaParam1.destCIdx      = (int16_t) syncABCfg1->dstCIdx;
    edmaParam1.srcBIdxExt    = (int8_t) EDMA_PARAM_BIDX_EXT(syncABCfg1->srcBIdx);
    edmaParam1.destBIdxExt   = (int8_t) EDMA_PARAM_BIDX_EXT(syncABCfg1->dstBIdx);
    edmaParam1.linkAddr      = 0xFFFFU;
    errorCode = DPEDMA_setup_shadow_link(handle, (uint8_t)chanCfg->paramId, (uint16_t)chanCfg->ShadowPramId[0], &edmaParam1);
    if(errorCode != DPEDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Link Link1 to Link2 */
    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam2);
    edmaParam2 = edmaParam;
    edmaParam2.srcAddr       = (uint32_t) SOC_virtToPhy((void *)syncABCfg2->srcAddress);
    edmaParam2.destAddr      = (uint32_t) SOC_virtToPhy((void *)syncABCfg2->destAddress);
    edmaParam2.aCnt          = (uint16_t) syncABCfg2->aCount;
    edmaParam2.bCnt          = (uint16_t) syncABCfg2->bCount;
    edmaParam2.cCnt          = (uint16_t) syncABCfg2->cCount;
    edmaParam2.bCntReload    = (uint16_t) 0U;
    edmaParam2.srcBIdx       = (int16_t) EDMA_PARAM_BIDX(syncABCfg2->srcBIdx);
    edmaParam2.destBIdx      = (int16_t) EDMA_PARAM_BIDX(syncABCfg2->dstBIdx);
    edmaParam2.srcCIdx       = (int16_t) syncABCfg2->srcCIdx;
    edmaParam2.destCIdx      = (int16_t) syncABCfg2->dstCIdx;
    edmaParam2.srcBIdxExt    = (int8_t) EDMA_PARAM_BIDX_EXT(syncABCfg2->srcBIdx);
    edmaParam2.destBIdxExt   = (int8_t) EDMA_PARAM_BIDX_EXT(syncABCfg2->dstBIdx);
    edmaParam2.linkAddr      = 0xFFFFU;
    errorCode = DPEDMA_setup_shadow_link(handle, (uint8_t)chanCfg->ShadowPramId[0], (uint16_t)chanCfg->ShadowPramId[1], &edmaParam2);
    if(errorCode != DPEDMA_NO_ERROR)
    {
        goto exit;
    }

    /* Link Link2 to Shadow to reflect the Main Channel Params */
    errorCode = DPEDMA_setup_shadow_link(handle, (uint8_t)chanCfg->ShadowPramId[1], (uint16_t)chanCfg->ShadowPramId[2], &edmaParam);
    if(errorCode != DPEDMA_NO_ERROR)
    {
        goto exit;
    }

    EDMA_linkChannel(baseAddr, chanCfg->ShadowPramId[2], chanCfg->ShadowPramId[0]);

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *     EDMA utility function for sync A type transfers.
 *     Here single frame means (EDMA) C count is 1.
 *
 *  @param[in]  handle          EDMA handle.
 *  @param[in]  chanCfg         Pointer to datapath EDMA channel configuration
 *  @param[in]  chainingCfg     Pointer to datapath EDMA channel chaining configuration
 *  @param[in]  syncACfg        Pointer to syncA type configuration
 *  @param[in]  isEventTriggered  Flag indicates if the channel is event triggered
 *  @param[in]  isIntermediateTransferInterruptEnabled Set to 'true' if intermediate transfer completion
 *                 indication is to be enabled.
 *  @param[in]  isTransferCompletionEnabled Set to 'true' if final transfer completion
 *                              indication is to be enabled.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *  @param[in]  intrObj pointer to EDMA interrupt object of type Edma_IntrObject.
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMA_configSyncA_singleFrame
(
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    DPEDMA_syncACfg         *syncACfg,
    bool                    isEventTriggered,
    bool                    isIntermediateTransferInterruptEnabled,
    bool                    isTransferCompletionEnabled,
    Edma_EventCallback      transferCompletionCallbackFxn,
    void*                   transferCompletionCallbackFxnArg,
    Edma_IntrObject         *intrObj
)
{
    uint32_t            baseAddr, regionId;
    EDMACCPaRAMEntry   edmaParam;
    int32_t             errorCode = DPEDMA_NO_ERROR;
    uint32_t            signBits = 0;

    if((chanCfg == NULL) || (syncACfg == NULL))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    /* Ensuring that the BIDX value is a signed integer in the limits of 24 bits */
    signBits = (uint32_t)syncACfg->srcBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    signBits = (uint32_t)syncACfg->dstBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    baseAddr = EDMA_getBaseAddr(handle);
    if(baseAddr == 0U)
    {
        errorCode = DPEDMA_EINVAL_EDMAADDR;
        goto exit;
    }

    regionId = EDMA_getRegionId(handle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t) SOC_virtToPhy((void *)syncACfg->srcAddress);
    edmaParam.destAddr      = (uint32_t) SOC_virtToPhy((void *)syncACfg->destAddress);
    edmaParam.aCnt          = (uint16_t) syncACfg->aCount;
    edmaParam.bCnt          = (uint16_t) syncACfg->bCount;
    edmaParam.cCnt          = (uint16_t) 1U;
    edmaParam.bCntReload    = (uint16_t) edmaParam.bCnt;
    edmaParam.srcBIdx       = (int16_t) EDMA_PARAM_BIDX(syncACfg->srcBIdx);
    edmaParam.destBIdx      = (int16_t) EDMA_PARAM_BIDX(syncACfg->dstBIdx);
    edmaParam.srcBIdxExt    = (int8_t) EDMA_PARAM_BIDX_EXT(syncACfg->srcBIdx);
    edmaParam.destBIdxExt   = (int8_t) EDMA_PARAM_BIDX_EXT(syncACfg->dstBIdx);
    edmaParam.linkAddr      = 0xFFFFU;
    /*
     * (BIT-0, BIT-1)  -   Source Address Mode & destination Address Mode is by default Linear
     * (BIT-3)         -   Static Entry is by default normal
     * (BIT-8 - BIT-10)-   FIFO width is by default 8-bit
     * (BIT-11)        -   Early completion is by default false
     */
    edmaParam.opt          |= (EDMA_SYNC_A);

    if(isTransferCompletionEnabled)
    {
        edmaParam.opt          |= (EDMA_OPT_TCINTEN_MASK);
    }

    if(isIntermediateTransferInterruptEnabled)
    {
        edmaParam.opt          |= (EDMA_OPT_ITCINTEN_MASK);
    }

    /* Chaining Channel Config. */
    if(chainingCfg != NULL)
    {
        edmaParam.opt          |= ((((uint32_t)chainingCfg->chainingChannel) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK);

        if(chainingCfg->isFinalChainingEnabled)
        {
            edmaParam.opt          |= (EDMA_TPCC_OPT_TCCHEN_MASK);
        }

        if(chainingCfg->isIntermediateChainingEnabled)
        {
            edmaParam.opt          |= (EDMA_TPCC_OPT_ITCCHEN_MASK);
        }
    }

    EDMA_setPaRAM(baseAddr, chanCfg->paramId, &edmaParam);

    if(isEventTriggered)
    {
        (void)EDMA_enableTransferRegion(baseAddr, regionId, chanCfg->channel, EDMA_TRIG_MODE_EVENT);
    }

    if((transferCompletionCallbackFxn != NULL) && (chainingCfg != NULL) && (intrObj!=NULL))
    {
        /* Register interrupt */
        intrObj->tccNum = chainingCfg->chainingChannel;
        intrObj->cbFxn  = transferCompletionCallbackFxn;
        intrObj->appData = (void *) transferCompletionCallbackFxnArg;
        errorCode = EDMA_registerIntr(handle, intrObj);
        DebugP_assert(errorCode == SystemP_SUCCESS);
    }

    errorCode = DPEDMA_setup_shadow_link(handle, (uint8_t)chanCfg->paramId, (uint16_t)chanCfg->shadowPramId, &edmaParam);

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *     EDMA utility function for sync A type transfers.
 *
 *  @param[in]  handle          EDMA handle.
 *  @param[in]  chanCfg         Pointer to datapath EDMA channel configuration
 *  @param[in]  chainingCfg     Pointer to datapath EDMA channel chaining configuration
 *  @param[in]  syncACfg        Pointer to syncA type configuration
 *  @param[in]  isEventTriggered  Flag indicates if the channel is event triggered
 *  @param[in]  isIntermediateTransferInterruptEnabled Set to 'true' if intermediate transfer completion
 *                 indication is to be enabled.
 *  @param[in]  isTransferCompletionEnabled Set to 'true' if final transfer completion
 *                              indication is to be enabled.
 *  @param[in]  transferCompletionCallbackFxn Transfer completion call back function.
 *  @param[in]  transferCompletionCallbackFxnArg Argument for transfer completion call back function.
 *  @param[in]  intrObj pointer to EDMA interrupt object of type Edma_IntrObject.
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMA_configSyncA
(
    EDMA_Handle             handle,
    DPEDMA_ChanCfg          *chanCfg,
    DPEDMA_ChainingCfg      *chainingCfg,
    DPEDMA_syncACfg         *syncACfg,
    bool                    isEventTriggered,
    bool                    isIntermediateTransferInterruptEnabled,
    bool                    isTransferCompletionEnabled,
    Edma_EventCallback      transferCompletionCallbackFxn,
    void*                   transferCompletionCallbackFxnArg,
    Edma_IntrObject         *intrObj
)
{
    uint32_t            baseAddr, regionId;
    EDMACCPaRAMEntry   edmaParam;
    int32_t             errorCode = DPEDMA_NO_ERROR;
    uint32_t            signBits = 0;

    if((chanCfg == NULL) || (syncACfg == NULL))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    /* Ensuring that the BIDX value is a signed integer in the limits of 24 bits */
    signBits = (uint32_t)syncACfg->srcBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    signBits = (uint32_t)syncACfg->dstBIdx & 0xFF800000U;
    if((signBits != 0U) && (signBits != 0xFF800000U))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    baseAddr = EDMA_getBaseAddr(handle);
    if(baseAddr == 0U)
    {
        errorCode = DPEDMA_EINVAL_EDMAADDR;
        goto exit;
    }

    regionId = EDMA_getRegionId(handle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t) SOC_virtToPhy((void *)syncACfg->srcAddress);
    edmaParam.destAddr      = (uint32_t) SOC_virtToPhy((void *)syncACfg->destAddress);
    edmaParam.aCnt          = (uint16_t) syncACfg->aCount;
    edmaParam.bCnt          = (uint16_t) syncACfg->bCount;
    edmaParam.cCnt          = (uint16_t) syncACfg->cCount;
    edmaParam.srcCIdx       = (int16_t) syncACfg->srcCIdx;
    edmaParam.destCIdx      = (int16_t) syncACfg->dstCIdx;
    edmaParam.bCntReload    = (uint16_t) edmaParam.bCnt;
    edmaParam.srcBIdx       = (int16_t) EDMA_PARAM_BIDX(syncACfg->srcBIdx);
    edmaParam.destBIdx      = (int16_t) EDMA_PARAM_BIDX(syncACfg->dstBIdx);
    edmaParam.srcBIdxExt    = (int8_t) EDMA_PARAM_BIDX_EXT(syncACfg->srcBIdx);
    edmaParam.destBIdxExt   = (int8_t) EDMA_PARAM_BIDX_EXT(syncACfg->dstBIdx);
    edmaParam.linkAddr      = 0xFFFFU;
    /*
     * (BIT-0, BIT-1)  -   Source Address Mode & destination Address Mode is by default Linear
     * (BIT-3)         -   Static Entry is by default normal
     * (BIT-8 - BIT-10)-   FIFO width is by default 8-bit
     * (BIT-11)        -   Early completion is by default false
     */
    edmaParam.opt          |= (EDMA_SYNC_A);

    if(isTransferCompletionEnabled)
    {
        edmaParam.opt          |= (EDMA_OPT_TCINTEN_MASK);
    }

    if(isIntermediateTransferInterruptEnabled)
    {
        edmaParam.opt          |= (EDMA_OPT_ITCINTEN_MASK);
    }

    /* Chaining Channel Config. */
    if(chainingCfg != NULL)
    {
        edmaParam.opt          |= ((((uint32_t)chainingCfg->chainingChannel) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK);

        if(chainingCfg->isFinalChainingEnabled)
        {
            edmaParam.opt          |= (EDMA_TPCC_OPT_TCCHEN_MASK);
        }

        if(chainingCfg->isIntermediateChainingEnabled)
        {
            edmaParam.opt          |= (EDMA_TPCC_OPT_ITCCHEN_MASK);
        }
    }

    EDMA_setPaRAM(baseAddr, chanCfg->paramId, &edmaParam);

    if(isEventTriggered)
    {
        (void)EDMA_enableTransferRegion(baseAddr, regionId, chanCfg->channel, EDMA_TRIG_MODE_EVENT);
    }

    if((transferCompletionCallbackFxn != NULL) && (chainingCfg != NULL) && (intrObj!=NULL))
    {
        /* Register interrupt */
        intrObj->tccNum = chainingCfg->chainingChannel;
        intrObj->cbFxn  = transferCompletionCallbackFxn;
        intrObj->appData = (void *) transferCompletionCallbackFxnArg;
        errorCode = EDMA_registerIntr(handle, intrObj);
        DebugP_assert(errorCode == SystemP_SUCCESS);
    }

    errorCode = DPEDMA_setup_shadow_link(handle, (uint8_t)chanCfg->paramId, (uint16_t)chanCfg->shadowPramId, &edmaParam);

exit:
    return(errorCode);
}


/**
 *  @b Description
 *  @n
 *    Utility function that reconfigures source and destination addresses of a given channel Id
 *    (whose param Id is assumed to be already configured to be same as channel Id)
 *    and then starts a transfer on that channel.
 *
 *  @param[in]  handle         EDMA handle.
 *  @param[in]  srcAddress     Source Address. If NULL, does not update.
 *  @param[in]  destAddress    Destination Address. If NULL, does not update.
 *  @param[in]  paramId        EDMA Param Id.
 *  @param[in]  triggerEnabled =1: trigger EDMA, =0: does not trigger EDMA
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 *
 */
int32_t DPEDMA_updateAddressAndTrigger(EDMA_Handle handle,
    uint32_t  srcAddress,
    uint32_t  destAddress,
    uint8_t   paramId,
    bool   triggerEnabled)
{
    int32_t errorCode = DPEDMA_NO_ERROR;
    uint32_t            baseAddr, regionId;

    baseAddr = EDMA_getBaseAddr(handle);
    if(baseAddr == 0U)
    {
        errorCode = DPEDMA_EINVAL_EDMAADDR;
        goto exit;
    }

    if(srcAddress != (uint32_t)NULL)
    {
        EDMA_dmaSetPaRAMEntry(baseAddr, paramId, EDMACC_PARAM_ENTRY_SRC, (uint32_t) SOC_virtToPhy((void *)srcAddress));
    }

    if(destAddress != (uint32_t)NULL)
    {
        EDMA_dmaSetPaRAMEntry(baseAddr, paramId, EDMACC_PARAM_ENTRY_DST, (uint32_t) SOC_virtToPhy((void *)destAddress));
    }

    if(triggerEnabled)
    {
        regionId = EDMA_getRegionId(handle);

        if (EDMA_enableTransferRegion(baseAddr, regionId, paramId, EDMA_TRIG_MODE_MANUAL) == 0U)
        {
            errorCode = DPEDMA_EDMA_TRIGGER_FAIL;
            goto exit;
        }
    }

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *    Utility function that allocated DMA resources
 *
 */
void DPEDMA_allocateEDMAChannel(EDMA_Handle handle,
    uint32_t *dmaCh,
    uint32_t *tcc,
    uint32_t *param
)
{
    int32_t             testStatus = SystemP_SUCCESS;
    uint32_t            baseAddr, regionId;
    EDMA_Config        *config;
    EDMA_Object        *object;

    config = (EDMA_Config *) handle;
    object = config->object;

    if(*dmaCh < 64U)
    {
        if((object->allocResource.dmaCh[*dmaCh/32U] & ((uint32_t)1U << *dmaCh%32U)) != ((uint32_t)1U << *dmaCh%32U))
        {
            testStatus = EDMA_allocDmaChannel(handle, dmaCh);
            DebugP_assert(testStatus == SystemP_SUCCESS);

            testStatus = EDMA_allocTcc(handle, tcc);
            DebugP_assert(testStatus == SystemP_SUCCESS);

            testStatus = EDMA_allocParam(handle, param);
            DebugP_assert(testStatus == SystemP_SUCCESS);

            baseAddr = EDMA_getBaseAddr(handle);
            DebugP_assert(baseAddr != 0U);

            regionId = EDMA_getRegionId(handle);
            DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

            /* Request channel */
            (void)EDMA_configureChannelRegion(baseAddr, regionId, EDMA_CHANNEL_TYPE_DMA,
                *dmaCh, *tcc, *param, 0);
        }
    }

    return;
}

/**
 *  @b Description
 *  @n
 *    Utility function that frees DMA resource
 *
 */
void DPEDMA_freeEDMAChannel(EDMA_Handle handle,
    uint32_t *dmaCh,
    uint32_t *tcc,
    uint32_t *param,
    uint32_t *shadowParam
)
{
    int32_t             testStatus = SystemP_SUCCESS;
    uint32_t            baseAddr, regionId;

    baseAddr = EDMA_getBaseAddr(handle);
    DebugP_assert(baseAddr != 0U);

    regionId = EDMA_getRegionId(handle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    (void)EDMA_freeChannelRegion(baseAddr, regionId, EDMA_CHANNEL_TYPE_DMA,
        *dmaCh, EDMA_TRIG_MODE_MANUAL, *tcc, 0);

    /* Free the EDMA resources managed by driver. */
    testStatus = EDMA_freeDmaChannel(handle, dmaCh);
    DebugP_assert(testStatus == SystemP_SUCCESS);
    *dmaCh  = 0;

    testStatus = EDMA_freeTcc(handle, tcc);
    DebugP_assert(testStatus == SystemP_SUCCESS);
    *tcc  = 0;

    testStatus = EDMA_freeParam(handle, param);
    DebugP_assert(testStatus == SystemP_SUCCESS);
    *param  = 0;

    testStatus = EDMA_freeParam(handle, shadowParam);
    DebugP_assert(testStatus == SystemP_SUCCESS);
    *shadowParam  = 0;

    return;
}

/**
 *  @b Description
 *  @n
 *    Utility function to perform manual trigger of EDMA
 *
 */
int32_t DPEDMA_edmaStartTransferManualTrigger(EDMA_Handle handle,
                                                uint32_t channel)
{

    int32_t retVal = 0;
    uint32_t baseAddr, regionId;

    baseAddr = EDMA_getBaseAddr(handle);
    if (baseAddr == 0U){
        retVal = -1;
        goto exit;
    }

    regionId = EDMA_getRegionId(handle);
    if (regionId > SOC_EDMA_NUM_REGIONS){
        retVal = -1;
        goto exit;
    }

    if(EDMA_enableTransferRegion(baseAddr, regionId, channel, EDMA_TRIG_MODE_MANUAL) == 0U){
        retVal = -1;
    }

exit:
    return retVal;

}

