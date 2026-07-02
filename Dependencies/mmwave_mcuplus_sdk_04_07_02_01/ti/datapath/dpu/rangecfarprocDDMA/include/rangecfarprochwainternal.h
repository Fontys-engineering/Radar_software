/**
 *   @file  rangecfarprochwainternal.h
 *
 *   @brief
 *      Implements internal data structure for CFAR Processing with HWA.
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

/** @defgroup CFAR_PROC_DPU_INTERNAL       cfarProc DPU Internal
 */

/**
@defgroup DPU_CFARPROC_INTERNAL_FUNCTION            cfarProc DPU Internal Functions
@ingroup CFAR_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup DPU_CFARPROC_INTERNAL_DATA_STRUCTURE      cfarProc DPU Internal Data Structures
@ingroup CFAR_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal data structures which are used internally
*   by the cfarProc DPU module.
*/
/**
@defgroup DPU_CFARPROC_INTERNAL_DEFINITION          cfarProc DPU Internal Definitions
@ingroup CFAR_PROC_DPU_INTERNAL
@brief
*   The section has a list of all internal definitions which are used internally
*   by the cfarProc DPU.
*/


/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DPU_CFAR_HWA_H
#define DPU_CFAR_HWA_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* MCU + SDK include files. */
#include <kernel/dpl/SemaphoreP.h>

/* mmWave SDK Include Files */
#include <ti/datapath/dpu/rangecfarprocDDMA/rangecfarprochwa.h>
#include <ti/utils/mathutils/mathutils.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rangeCfarProcHWADDMAIODataCfg_t
{
    /*! @brief  1 if real, 0 otherwise */
    uint8_t isReal;

    /*! @brief Number of bytes per sample */
    uint8_t bytesPerSample;

    /*! @brief  1 if input is signed, 0 otherwise */
    uint8_t isSigned;

}rangeCfarProcHWADDMAIODataCfg;

typedef struct rangeCfarProcHWADDMADataCfg_t
{
    /*! @brief  Range CFAR Proc Intput config */
    rangeCfarProcHWADDMAIODataCfg    input;

    /*! @brief  Range CFAR Proc Output config */
    rangeCfarProcHWADDMAIODataCfg    output;

}rangeCfarProcHWADDMADataCfg;


/**
 * @brief
 *  HWA CFAR configuration
 *
 * @details
 *  The structure is used to hold the HWA configuration used for CFAR
 *
 *  \ingroup DPU_CFARPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeCfarProcHWA_Obj_t
{
    /*! @brief      number of detected objection from HWA */
    uint16_t        numObjOut;

    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;

    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPongIn[2];

    /*! @brief CFAR EDMA Out (ping) Done semaphore object */
    SemaphoreP_Object  cfarPingEdmaOutDoneSemaHandle;

    /*! @brief CFAR EDMA Out (pong) Done semaphore object */
    SemaphoreP_Object  cfarPongEdmaOutDoneSemaHandle;

    /*! @brief      CFAR configuration in range direction */
    DPU_CFARProc_CfarCfg   *rangeCfarCfg;

    /*! @brief      CFAR IO configuration in range direction */
    rangeCfarProcHWADDMADataCfg rangeCfarIOCfg;

    /*! @brief  Number of range bins */
    uint16_t    numRangeBins;

    /*! @brief  Number of doppler bins */
    uint16_t    numDopplerBins;

    /*! @brief total number of calls of DPU processing */
    uint32_t numProcess;

    /*! @brief number of doppler gates per ping processing */
    uint32_t numDopplerGatesPerPing;

    /*! @brief CFAR input size */
    uint32_t cfarInSize;

    /*! @brief CFAR output size */
    uint32_t cfarOutSize;

    /*! @brief num HWA loops */
    uint32_t numLoops;

    /*! @brief number of CFAR samples */
    uint32_t numCfarSamples;

    /*! @brief HWA Memory Bank addresses */
    uint32_t  hwaMemBankAddr[DPU_RANGECFARPROCHWA_NUM_HWA_MEMBANKS];

    /*! @brief Num Ping CFAR Peaks */
    uint16_t numCfarPeaksPing;

    /*! @brief Num Pong CFAR Peaks */
    uint16_t numCfarPeaksPong;

    /*! @brief Num Subbands */
    uint16_t numSubBandsTotal;

}DPU_RangeCfarProcHWA_Obj;



#ifdef __cplusplus
}
#endif

#endif /* DPU_CFAR_HWA_H */
