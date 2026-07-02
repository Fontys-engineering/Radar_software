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
 *   @file  rangeprochwaDDMA_internal.h
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

/* mmwave SDK include files */
#include <ti/datapath/dpu/rangeprocDDMA/rangeprochwaDDMA.h>
#include <ti/datapath/dpu/rangeprocDDMA/rangeproc_common.h>
#include <ti/datapath/dpu/rangeprocDDMA/include/rangeprocDDMA_internal.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct rangeProcHWADDMAHwaAddressSingle_t
{
    uint32_t inAddr;
    uint32_t outAddr;
}rangeProcHWADDMAHwaAddressSingle;

typedef struct rangeProcHWADDMAHwaInOutAddress_t
{
    rangeProcHWADDMAHwaAddressSingle dcEstParamSet;
    rangeProcHWADDMAHwaAddressSingle dcSubIntfEstParamSet;
    rangeProcHWADDMAHwaAddressSingle intfEstFFTParamSet;
    rangeProcHWADDMAHwaAddressSingle compParamSet;

}rangeProcHWADDMAHwaInOutAddress;

/**
 * @brief
 *  RangeProcHWA Compression internal data object
 *
 *  \ingroup DPU_RANGEPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct rangeProcHWACompressionCfg_t
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

    /*! @brief     Data path common parameters used in rangeProc */
    rangeProc_dpParams              params;

    /*! @brief      EDMA Handle */
    EDMA_Handle                     edmaHandle;

    /*! @brief     RangeProc HWA configuration */
    DPU_RangeProcHWA_HwaConfig      hwaCfg;

    /*! @brief     RangeProc HWA data input paramset trigger */
    uint8_t                 dataInTrigger[2];

    /*! @brief     RangeProc HWA data output paramset trigger */
    uint8_t                 dataOutTrigger[2];

    /*! @brief     EDMA done semaphore */
    SemaphoreP_Object       edmaDoneSemaHandle;

    /*! @brief     HWA Processing Done semaphore Handle */
    SemaphoreP_Object       hwaDoneSemaHandle;

    /*! @brief      Data in interleave or non-interleave mode */
    DPIF_RXCHAN_INTERLEAVE  interleave;

    /*! @brief     Rada Cube layout */
    rangeProcRadarCubeLayoutFmt radarCubeLayout;

    /*! @brief     ADC data buffer RX channel offset - fixed for all channels */
    uint16_t                rxChanOffset;

    /*! @brief      Pointer to ADC buffer */
    cmplx16ImRe_t           *ADCdataBuf;

    /*! @brief      Pointer to Radar Cube buffer */
    cmplx16ImRe_t           *radarCubebuf;

    /*! @brief      Pointer to DC range signal mean buffer */
    cmplx32ImRe_t           *dcRangeSigMean;

    /*! @brief     DC range calibration scratch buffer size */
    uint32_t                dcRangeSigMeanSize;

    /*! @brief      HWA Memory address */
    uint32_t                hwaMemBankAddr[SOC_HWA_NUM_MEM_BANKS];

    /*! @brief      HWAInOutBanks Memory address */
    rangeProcHWADDMAHwaInOutAddress    hwaParamSetMemBankAddr;

    /*! @brief     DMA channel trigger after HWA processing is done */
    uint8_t                 calibDcNumLog2AvgChirps;

    /*! @brief     DMA data out Signature channel */
    uint32_t                 dataOutSignatureChan;

    /*! @brief     rangeProc DPU is in processing state */
    bool                    inProgress;

    /*! @brief     rangeProc DPU HWA Chain select*/
    DPU_RangeProcHWA_RangeProcChain rangeProcChain;

    /*! @brief     rangeProc DPU HWA Real 2X enable */
    bool                    isReal2XEnabled;

    /*! @brief     rangeProc DPU HWA FFT scaling applied to output */
    uint8_t                 fftOutputScaling;

    /*! @brief     Total number of rangeProc DPU processing */
    uint32_t                numProcess;

    /*! @brief     Total number of data output EDMA done interrupt */
    uint32_t                numEdmaDataOutCnt;

    /*! @brief     Compression Configuration */
    rangeProcHWACompressionCfg  compressionCfg;

    /*! @brief HWA Static Shift/Scale config for DC Est */
    DPU_RangeProc_shiftScaleCfg  dcEstShiftScaleCfg;

    /*! @brief HWA Static Shift/Scale config for Interf Stats Mag */
    DPU_RangeProc_shiftScaleCfg  intfStatsMagShiftScaleCfg;

    /*! @brief HWA Static Shift/Scale config for Interf Stats Mag Diff */
    DPU_RangeProc_shiftScaleCfg  intfStatsMagDiffShiftScaleCfg;

}rangeProcHWAObj;





#ifdef __cplusplus
}
#endif

#endif
