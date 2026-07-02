/**
 *   @file  dpm_awr294x.c
 *
 *   @brief
 *      DPM Ported Layer for AWR294X Platform
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020-2021 Texas Instruments, Inc.
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
#include <ti/control/dpm/dpm.h>
#include <ti/control/dpm/include/dpm_internal.h>

/**
 *  \brief DPM OSAL ISR callback function prototype.
 *
 *  \param arg          [IN] App data
 */
typedef void (*Dpm_OsalIsrFxn)(uintptr_t arg);
/**************************************************************************
 *********************** Global Platform Functions ************************
 **************************************************************************/

/**
 * @brief
 *  DPM Hardware Attributes. No interrupt numbers for AWR294X.
 */
DPM_HWAttribute  gDPMHwAttrib =
{
#ifdef SUBSYS_DSS
    CSL_DSS_INTR_RSS_ADC_CAPTURE_COMPLETE_DITH,
    CSL_DSS_INTR_DFE_FRAME_START_TO_DSS
#else
    CSL_MSS_INTR_RSS_ADC_CAPTURE_COMPLETE,
    CSL_MSS_INTR_DFE_FRAME_START_TO_MSS
#endif
};

/**
 *  @b Description
 *  @n
 *      Chirp Available System ISR registered by the DPM module
 *
 *  @param[in]  arg
 *      DPM Module Handle
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void DPM_chirpISR (void *arg)
{
    DPM_MCB*    ptrDPM;

    /* Get the pointer to the DPM Module: */
    ptrDPM = (DPM_MCB*)arg;

    #if defined(_TMS320C6X)
    HwiP_clearInt(&ptrDPM->hwiChirpAvailableHwiObject);
    #else
    HwiP_clearInt((uint32_t)gDPMHwAttrib.chirpAvailableSysInterrupt);
    #endif

    if(ptrDPM->procChainCfg.chirpAvailableFxn != NULL){
        /* Pass the control back to the processing chain registered chirp available function: */
        ptrDPM->procChainCfg.chirpAvailableFxn (ptrDPM->dpcHandle);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      Frame Start System ISR registered by the DPM module
 *
 *  @param[in]  arg
 *      DPM Module Handle
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Not applicable
 */
static void DPM_frameStartISR (void *arg)
{
    DPM_MCB*    ptrDPM;

    /* Get the pointer to the DPM Module: */
    ptrDPM = (DPM_MCB*)arg;

    #if defined(_TMS320C6X)
    HwiP_clearInt(&ptrDPM->hwiFrameStartHwiObject);
    #else
    HwiP_clearInt((uint32_t)gDPMHwAttrib.frameStartSysInterrupt);
    #endif

    if(ptrDPM->procChainCfg.frameStartFxn != NULL){
        /* Pass the control back to the processing chain registered frame start function: */
        ptrDPM->procChainCfg.frameStartFxn (ptrDPM->dpcHandle);
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      Device specific functionality such as install chirp/frame ISR.
 *      No device specific feature needed for AWR294X.
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM Module
 *  @param[in]  ptrProcChainCfg
 *      Processing chain to be loaded
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_initDeviceHook(
        DPM_MCB*            ptrDPM,
        DPM_ProcChainCfg*   ptrProcChainCfg
    )
{
    int32_t           retVal = 0;
    int32_t           status = SystemP_SUCCESS;
    HwiP_Params       hwiPrms;

    if(ptrProcChainCfg->chirpAvailableFxn != NULL)
    {
        /* Register interrupt */
        HwiP_Params_init(&hwiPrms);
        hwiPrms.intNum      = gDPMHwAttrib.chirpAvailableSysInterrupt;
        hwiPrms.callback    = &DPM_chirpISR;
        hwiPrms.args        = (void *)ptrDPM;
        status              = HwiP_construct(&ptrDPM->hwiChirpAvailableHwiObject, &hwiPrms);

        if(SystemP_SUCCESS != status)
        {
            retVal = -1;
        }
        else
        {
            #if defined(_TMS320C6X)
            HwiP_enableInt(&ptrDPM->hwiChirpAvailableHwiObject);
            #else
            HwiP_enableInt((uint32_t)gDPMHwAttrib.chirpAvailableSysInterrupt);
            #endif
        }
    }

    if(ptrProcChainCfg->frameStartFxn != NULL)
    {
        /* Register interrupt */
        HwiP_Params_init(&hwiPrms);
        hwiPrms.intNum      = gDPMHwAttrib.frameStartSysInterrupt;
        hwiPrms.callback    = &DPM_frameStartISR;
        hwiPrms.args        = (void *)ptrDPM;
        status              = HwiP_construct(&ptrDPM->hwiFrameStartHwiObject, &hwiPrms);

        if(SystemP_SUCCESS != status)
        {
            retVal = -1;
        }
        else
        {
            #if defined(_TMS320C6X)
            HwiP_enableInt(&ptrDPM->hwiFrameStartHwiObject);
            #else
            HwiP_enableInt((uint32_t)gDPMHwAttrib.frameStartSysInterrupt);
            #endif
        }
    }

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      Device specific functionality such as deinstall chirp/frame ISR.
 *      No device specific feature needed for AWR294X.
 *
 *  @param[in]  ptrDPM
 *      Pointer to the DPM Module
 *
 *  \ingroup DPM_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   - one of @ref DPM_ERROR_CODE
 */
int32_t DPM_deinitDeviceHook(DPM_MCB* ptrDPM)
{
    if(ptrDPM->procChainCfg.chirpAvailableFxn!=NULL)
    {
        #if defined(_TMS320C6X)
        HwiP_disableInt(&ptrDPM->hwiChirpAvailableHwiObject);
        #else
        HwiP_disableInt((uint32_t)gDPMHwAttrib.chirpAvailableSysInterrupt);
        #endif
        HwiP_destruct(&ptrDPM->hwiChirpAvailableHwiObject);
    }

    if(ptrDPM->procChainCfg.frameStartFxn!=NULL)
    {
        #if defined(_TMS320C6X)
        HwiP_disableInt(&ptrDPM->hwiFrameStartHwiObject);
        #else
        HwiP_disableInt((uint32_t)gDPMHwAttrib.frameStartSysInterrupt);
        #endif
        HwiP_destruct(&ptrDPM->hwiFrameStartHwiObject);
    }
    return (0);
}

