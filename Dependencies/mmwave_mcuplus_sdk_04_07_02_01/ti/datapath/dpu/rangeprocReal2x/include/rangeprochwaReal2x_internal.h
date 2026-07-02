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
 *   @file  rangeprochwaReal2x_internal.h
 *
 *   @brief
 *      rangeProcHWA internal definitions.
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DPU_RANGEPROCHWA_INTERNAL_H
#define DPU_RANGEPROCHWA_INTERNAL_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  RangeProcHWA Compression internal data object
 *
 *  \ingroup DPU_RANGEPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct rangeProcHWACompressionCfg
{
    /*! @brief Flag that indicates if compression is enabled */
    bool  isEnabled;

    /*! @brief Compression Method, 0 indicates EGE, 1 indicates BFP */
    uint8_t  compressionMethod;

    /*! @brief Number of samples in a single input block to be compressed */
    uint16_t inputSamplesPerBlock;

    /*! @brief Number of samples in a single compressed output block */
    uint16_t outputSamplesPerBlock;

    /*! @brief Number of blocks */
    uint16_t numBlocks;

    /*! @brief Number of blocks per packet*/
    uint16_t numBlockPerPayload;

    /*! @brief Bytes per input/output sample */
    uint16_t bytesPerSample;

    /*! @brief Number of bytes per input block */
    uint16_t inputBytesPerBlock;

    /*! @brief Number of bytes per compressed output block */
    uint16_t outputBytesPerBlock;

    /*! @brief Number of Rx Antennas per block */
    uint16_t rxAntPerBlock;

    /*! @brief Number of Range Bins per block */
    uint16_t rangeBinsPerBlock;

    /*! @brief The actual compression ratio achieved */
    float achievedCompressionRatio;

}rangeProcHWACompressionCfg;

/**
 * @brief
 *  RangeProcHWA shift scale value for HWA
 *
 *  \ingroup DPU_RANGEPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct rangeProcHWAshiftScaleCfg_t{

    /*! @brief Shift Value */
    uint32_t shift;

    /*! @brief Scale Value */
    uint32_t scale;
}rangeProcHWAshiftScaleCfg;


/**
 * @brief
 *  RangeProcHWA DPU Object
 *
 * @details
 *  The structure is used to hold RangeProcHWA internal data object
 *
 *  \ingroup DPU_RANGEPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct rangeProcHWAObj_t
{
    DPU_RangeProcHWA_InitParams     initParms;

    /*! @brief      EDMA Handle */
    EDMA_Handle                     edmaHandle;

    /*! @brief     RangeProc HWA configuration */
    DPU_RangeProcHWA_HwaConfig      hwaCfg;

    /*! @brief     RangeProc HWA data input paramset trigger */
    uint8_t                 dataInTrigger[2];

    /*! @brief     RangeProc HWA data output paramset trigger */
    uint8_t                 dataOutTrigger[2];

    /*! @brief     HWA Processing Done semaphore Handle */
    SemaphoreP_Object       hwaDoneSemaHandle;

    /*! @brief      Data in interleave or non-interleave mode */
    DPIF_RXCHAN_INTERLEAVE  interleave;

    /*! @brief  Number of chirps per frame */
    uint16_t                numChirpsPerFrame;

    /*! @brief     ADC data buffer RX channel offset - fixed for all channels */
    uint16_t                rxChanOffset;

    /*! @brief      Pointer to ADC buffer */
    cmplx16ImRe_t           *ADCdataBuf;

    /*! @brief      Pointer to Radar Cube buffer */
    cmplx16ImRe_t           *radarCubebuf;

    /*! @brief      HWA Memory address */
    uint32_t                hwaMemBankAddr[SOC_HWA_NUM_MEM_BANKS];

    /*! @brief     DMA data out Signature channel */
    uint32_t                 dataOutSignatureChan;

    /*! @brief     rangeProc DPU is in processing state */
    bool                    inProgress;

    /*! @brief     Total number of rangeProc DPU processing */
    uint32_t                numProcess;

    /*! @brief     Total number of data output EDMA done interrupt */
    uint32_t                numEdmaDataOutCnt;

    /*! @brief     Compression Configuration */
    rangeProcHWACompressionCfg  compressionCfg;

    /*! @brief HWA Static Shift/Scale config for DC Est */
    rangeProcHWAshiftScaleCfg  dcEstShiftScaleCfg;

    /*! @brief HWA Static Shift/Scale config for Interf Stats Mag */
    rangeProcHWAshiftScaleCfg  intfStatsMagShiftScaleCfg;

    /*! @brief HWA Static Shift/Scale config for Interf Stats Mag Diff */
    rangeProcHWAshiftScaleCfg  intfStatsMagDiffShiftScaleCfg;

}rangeProcHWAObj;


#ifdef __cplusplus
}
#endif

#endif
