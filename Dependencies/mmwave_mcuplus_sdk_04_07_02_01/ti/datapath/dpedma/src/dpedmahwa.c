/**
 *   @file  dpedmahwa.c
 *
 *   @brief
 *      EDMA Configuration Utility API implementation for HWA related functions.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 - 2021 Texas Instruments, Inc.
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


/* PDK Include Files */
#include <drivers/hwa.h>
#include <drivers/soc.h>

/* mmWave SDK Include Files */
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/include/dpedmainternal.h>

/**
 *  @b Description
 *  @n
 *     Utility function for configuring a simple EDMA transfer used for transferring
 *     a specific  32-bit word (from a table of 16 words each containing a 1-hot signature)
 *     to the HWA's DMA completion register. The counts and addresses
 *     can be obtained from HWA APIs issued for configuring HWA.
 *     This is used to implement the DMA based trigger mode
 *     of the HWACC (search for "DMA based trigger" in the HWACC spec document)
 *
 *  @param[in]  edmaHandle          EDMA driver handle.
 *  @param[in]  chanCfg             pointer to EDMA Channel configuration
 *  @param[in]  hwaHandle           HWA driver handle.
 *  @param[in]  dmaTriggerSource    HWA paramset trigger source
 *  @param[in]  isEventTriggered    true if chId is event triggered else false
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMAHWA_configOneHotSignature
(
    EDMA_Handle         edmaHandle,
    DPEDMA_ChanCfg      *chanCfg,
    HWA_Handle          hwaHandle,
    uint8_t             dmaTriggerSource,
    bool                isEventTriggered
)
{
    uint32_t            baseAddr;
    EDMACCPaRAMEntry   edmaParam;
    int32_t             errorCode = DPEDMA_NO_ERROR;
    HWA_SrcDMAConfig        dmaConfig;

    if((chanCfg == NULL) || (hwaHandle == NULL))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    /* Get DMA trigger configuration from HWA driver */
    (void)HWA_getDMAconfig(hwaHandle, dmaTriggerSource, &dmaConfig);

    baseAddr = EDMA_getBaseAddr(edmaHandle);
    if(baseAddr == 0U)
    {
        errorCode = DPEDMA_EINVAL_EDMAADDR;
        goto exit;
    }

    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t) SOC_virtToPhy((void *)dmaConfig.srcAddr);
    edmaParam.destAddr      = (uint32_t) SOC_virtToPhy((void *)dmaConfig.destAddr);
    edmaParam.aCnt          = (uint16_t) dmaConfig.aCnt;
    edmaParam.bCnt          = (uint16_t) dmaConfig.bCnt;
    edmaParam.cCnt          = (uint16_t) dmaConfig.cCnt;
    edmaParam.bCntReload    = (uint16_t) dmaConfig.bCnt;
    edmaParam.srcBIdx       = (int16_t) 0;
    edmaParam.destBIdx      = (int16_t) 0;
    edmaParam.srcCIdx       = (int16_t) 0;
    edmaParam.destCIdx      = (int16_t) 0;
    edmaParam.linkAddr      = 0xFFFFU;
    /* 
     * (BIT-0, BIT-1)  -   Source Address Mode & destination Address Mode is by default Linear
     * (BIT-3)         -   Static Entry is by default normal
     * (BIT-8 - BIT-10)-   FIFO width is by default 8-bit
     * (BIT-11)        -   Early completion is by default false
     */
    edmaParam.opt          |= (EDMA_SYNC_A);

    EDMA_setPaRAM(baseAddr, chanCfg->paramId, &edmaParam);

    errorCode = DPEDMA_setup_shadow_link(edmaHandle, (uint8_t)chanCfg->paramId, (uint16_t)chanCfg->shadowPramId, &edmaParam);

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *     Utility function for configuring a simple EDMA transfer used for transferring
 *     two specific 32-bit word (from a table of 16 words each containing a 1-hot signature)
 *     to the HWA's DMA completion register. The counts and addresses
 *     can be obtained from HWA APIs issued for configuring HWA.
 *     This is used to implement the DMA based trigger mode
 *     of the HWACC (search for "DMA based trigger" in the HWACC spec document)
 *
 *  @param[in]  edmaHandle          EDMA driver handle.
 *  @param[in]  chanCfg             pointer to EDMA Channel configuration
 *  @param[in]  hwaHandle           HWA driver handle.
 *  @param[in]  dmaTriggerSource1   HWA paramset trigger source 1
 *  @param[in]  dmaTriggerSource2   HWA paramset trigger source 2
 *  @param[in]  isEventTriggered    true if chId is event triggered else false
 *
 *  \ingroup    DPEDMA_EXTERNAL_FUNCTION
 *
 *  @retval
 *      EDMA driver error code, see "EDMA_ERROR_CODES" in EDMA API.
 */
int32_t DPEDMAHWA_configTwoHotSignature
(
    EDMA_Handle         edmaHandle,
    DPEDMA_ChanCfg      *chanCfg,
    HWA_Handle          hwaHandle,
    uint8_t             dmaTriggerSource1,
    uint8_t             dmaTriggerSource2,
    bool                isEventTriggered
)
{
    uint32_t            baseAddr;
    EDMACCPaRAMEntry   edmaParam;
    int32_t             errorCode = DPEDMA_NO_ERROR;
    HWA_SrcDMAConfig        dmaConfig[2];

    if((chanCfg == NULL) || (hwaHandle == NULL))
    {
        errorCode = DPEDMA_EINVAL;
        goto exit;
    }

    /* Get DMA trigger configuration from HWA driver */
    (void)HWA_getDMAconfig(hwaHandle, dmaTriggerSource1, &dmaConfig[0]);
    (void)HWA_getDMAconfig(hwaHandle, dmaTriggerSource2, &dmaConfig[1]);

    baseAddr = EDMA_getBaseAddr(edmaHandle);
    if(baseAddr == 0U)
    {
        errorCode = DPEDMA_EINVAL_EDMAADDR;
        goto exit;
    }

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t) SOC_virtToPhy((void *)dmaConfig[0].srcAddr);
    edmaParam.destAddr      = (uint32_t) SOC_virtToPhy((void *)dmaConfig[0].destAddr);
    edmaParam.aCnt          = (uint16_t) dmaConfig[0].aCnt;
    edmaParam.bCnt          = (uint16_t) dmaConfig[0].bCnt * 2U;
    edmaParam.cCnt          = (uint16_t) dmaConfig[0].cCnt;
    edmaParam.bCntReload    = (uint16_t) edmaParam.bCnt;
    edmaParam.srcBIdx       = (int16_t)dmaConfig[1].srcAddr - (int16_t)dmaConfig[0].srcAddr;
    edmaParam.destBIdx      = (int16_t) 0;
    edmaParam.srcCIdx       = (int16_t) 0;
    edmaParam.destCIdx      = (int16_t) 0;
    edmaParam.linkAddr      = 0xFFFFU;
    /* 
     * (BIT-0, BIT-1)  -   Source Address Mode & destination Address Mode is by default Linear
     * (BIT-3)         -   Static Entry is by default normal
     * (BIT-8 - BIT-10)-   FIFO width is by default 8-bit
     * (BIT-11)        -   Early completion is by default false
     */
    edmaParam.opt          |= (EDMA_SYNC_A);

    EDMA_setPaRAM(baseAddr, chanCfg->paramId, &edmaParam);

    errorCode = DPEDMA_setup_shadow_link(edmaHandle, (uint8_t)chanCfg->paramId, (uint16_t)chanCfg->shadowPramId, &edmaParam);

exit:
    return(errorCode);
}

