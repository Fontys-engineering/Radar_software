/**
 *   @file  dopplerprochwaDDMAinternal.h
 *
 *   @brief
 *      Implements Data path doppler processing functionality.
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
#ifndef DOPPLERPROC_HWA_INTERNAL_H
#define DOPPLERPROC_HWA_INTERNAL_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

/* MCU SDK files */
#include <drivers/hwa.h>
#include <kernel/dpl/CycleCounterP.h>

/* DPIF Components Include Files */
#include <ti/datapath/dpif/dpif_detmatrix.h>
#include <ti/datapath/dpif/dpif_radarcube.h>

/* mmWave SDK Data Path Include Files */
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpu/dopplerprocDDMA/dopplerprochwaDDMA.h>

/* Uncomment this to use interrupts for EDMA completion instead of polls */
#define DOPPLERPROCHWADDMA_INTERRUPTS

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  DopplerProc HWA DDMA Decompression internal data object
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct dopplerProcHWADDMADecompressionCfg_t
{
    /*! @brief Flag that indicates if decompression is enabled */
    bool  isEnabled;

    /*! @brief Compression Method, 0 indicates EGE, 1 indicates BFP */
    uint8_t  compressionMethod;

    /*! @brief Number of samples in a single input block to be decompressed */
    uint16_t inputSamplesPerBlock;

    /*! @brief Number of samples in a single decompressed output block */
    uint16_t outputSamplesPerBlock;

    /*! @brief Number of blocks */
    uint16_t numBlocks;

    /*! @brief Bytes per input/output sample */
    uint16_t bytesPerSample;

    /*! @brief Number of bytes per input block */
    uint16_t inputBytesPerBlock;

    /*! @brief Number of bytes per decompressed output block */
    uint16_t outputBytesPerBlock;

    /*! @brief Number of Rx Antennas per block */
    uint16_t rxAntPerBlock;

    /*! @brief Number of Range Bins per block */
    uint16_t rangeBinsPerBlock;

    /*! @brief The actual compression ratio achieved */
    float achievedCompressionRatio;

    /*! @brief Number of chirps to process per ping or pong */
    uint16_t numChirpsPerPing;

    /*! @brief Number of blocks to process per ping or pong */
    uint16_t numBlocksPerPing;

    /*! @brief Number of loops to run the input EDMA for */
    uint16_t numLoops;

    /*! @brief Decompression matrix start address */
    int32_t * decompEdmaToHwaStartAddress;

    /*! @brief Number of outer blocks to decompress together */
    uint32_t mergedNumOuterBlocks;

    /*! @brief Number of outer blocks (decompression stage will run for these many stages) */
    uint32_t numOuterBlocks;

    /*! @brief Size of decompression per ping */
    uint32_t decompSizePerPingPong;

    /*! @brief Size of single compressed outer block */
    uint32_t outerBlockSizeCompressed;

    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPongIn[2];

    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPongOut[2];

}dopplerProcHWADDMADecompressionCfg;

/**
 * @brief
 *  DopplerProc HWA DDMA Decompression IO configuration internal data object
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct dopplerProcHWADDMAIODataCfg_t
{
    /*! @brief  Is data real */
    uint8_t isReal;

    /*! @brief  Number of bytes per sample */
    uint8_t bytesPerSample;

    /*! @brief  Is data signed*/
    uint8_t isSigned;

}dopplerProcHWADDMAIODataCfg;

/**
 * @brief
 *  DopplerProc HWA DDMA Decompression IO internal data object
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct dopplerProcHWADDMADataCfg_t
{
    /*! @brief  Input configuration */
    dopplerProcHWADDMAIODataCfg    input;

    /*! @brief  Output configuration */
    dopplerProcHWADDMAIODataCfg    output;

}dopplerProcHWADDMADataCfg;

/**
 * @brief
 *  DopplerProc HWA DDMA Doppler/Demodulation stage internal data object
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct dopplerProcHWADDMADopplerDemodCfg_t
{

    /*! @brief  Total number of subbands */
    uint16_t    numBandsTotal;

    /*! @brief  Number of empty subbands */
    uint16_t    numBandsEmpty;

    /*! @brief  Total number of active subbands */
    uint16_t    numBandsActive;

    /*! @brief Number of loops to run the input EDMA for */
    uint16_t numLoops;

    /*! @brief Number of loops to run the input EDMA for */
    int32_t * decompEdmaToHwaStartAddress;

    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPongIn[2];

    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPongOut[2];

}dopplerProcHWADDMADopplerDemodCfg;

#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
/**
 * @brief
 *  CFAR Configuration
 *
 * @details
 *  The structure contains the cfar configuration used in data path
 */
typedef struct dopplerProcHWADDMAcfarCfg_t
{
    /*! @brief    CFAR Enabled */
    uint8_t       isEnabled;
    
    /*! @brief    CFAR threshold scale */
    uint16_t       thresholdScale;

    /*! @brief    CFAR averagining mode 0-CFAR_CA, 1-CFAR_CAGO, 2-CFAR_CASO, 3-CFAR_OS(HWA2.0 only) */
    uint8_t        averageMode;

    /*! @brief    CFAR noise averaging one sided window length */
    uint8_t        winLen;

    /*! @brief    CFAR one sided guard length*/
    uint8_t        guardLen;

    /*! @brief    CFAR cumulative noise sum divisor
                  CFAR_CA:
                        noiseDivShift should account for both left and right noise window
                        ex: noiseDivShift = ceil(log2(2 * winLen))
                  CFAR_CAGO/_CASO:
                        noiseDivShift should account for only one sided noise window
                        ex: noiseDivShift = ceil(log2(winLen))
     */
    uint8_t        noiseDivShift;

    /*! @brief    CFAR 0-cyclic mode disabled, 1-cyclic mode enabled */
    uint8_t        cyclicMode;

    /*! @brief    Peak grouping scheme 1-based on neighboring peaks from detection matrix
     *                                 2-based on on neighboring CFAR detected peaks.
     *            Scheme 2 is not supported on the HWA version (cfarprochwa.h) */
    uint8_t        peakGroupingScheme;

    /*! @brief     Peak grouping, 0- disabled, 1-enabled */
    uint8_t        peakGroupingEn;

    /*! @brief     The ordered statistic K in CFAR_OS */
    uint8_t        osKvalue;

    /*! @brief     Only used in CFAR_OS non-cyclic mode, scaling of K value for edge samples,
     *             0- disabled, 1-enabled */
    uint8_t        osEdgeKscaleEn;

    /*! @brief     Variable CFAR Thresholds based on Range Index,
     *             0- disabled, 1-enabled */
    uint8_t        variableThresholdMode;

} dopplerProcHWADDMAcfarCfg;
#endif

/**
 * @brief
 *  DopplerProc HWA DDMA Local Max internal data object
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct dopplerProcHWADDMAlocalMaxCfg_t
{
    /*! @brief    Azim threshold scale */
    uint16_t azimThreshold;

    /*! @brief    Azim threshold scale */
    uint16_t dopplerThreshold;

} dopplerProcHWADDMAlocalMaxCfg;

/**
 * @brief
 *  DopplerProc HWA DDMA Azim stage internal data object
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct dopplerProcHWADDMAcfarAzimFFTCfg_t
{
#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    /*! @brief    CFAR Config */
    dopplerProcHWADDMAcfarCfg   *cfarCfg;
#endif

    /*! @brief    Local Max Config */
    dopplerProcHWADDMAlocalMaxCfg   localMaxCfg;

    /*! @brief    Number of Azim FFT Bins */
    uint32_t    numAzimFFTBins;

    /*! @brief Antenna calibration parameters in Im/Re format */
    int32_t antennaCalibParamsQuantized[NUM_TXANT * NUM_RXANT * 2U];

    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPongIn[2];

    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPongOut[2];

}dopplerProcHWADDMAcfarAzimFFTCfg;

/**
 * @brief
 *  dopplerProc DPU internal data Object
 *
 * @details
 *  The structure is used to hold dopplerProc internal data object
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_Obj_t
{
    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;

    /*! @brief  EDMA driver handle. */
    EDMA_Handle edmaHandle;

    /*! @brief  EDMA configuration for Input data (Radar cube -> HWA memory). */
    DPU_DopplerProc_Edma edmaIn;

#ifdef DOPPLERPROCHWADDMA_INTERRUPTS
    /*! @brief Doppler Decompression EDMA Out Done semaphore object */
    SemaphoreP_Object  decompEdmaOutDoneSemaHandle;

    /*! @brief DDMA Metric EDMA Out Done semaphore object */
    SemaphoreP_Object  maxSubbandEdmaOutDoneSemaHandle[2];

    /*! @brief Demodulation HWA Done semaphore object */
    SemaphoreP_Object  demodHwaDoneSemaHandle;

#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    /*! @brief Azimuth FFT HWA Done semaphore object */
    SemaphoreP_Object  azimFFTHwaDoneSemaHandle[2];
#else
    /*! @brief CFAR HWA Done semaphore object */
    SemaphoreP_Object  cfarHwaDoneSemaHandle;
#endif

    /*! @brief Local Max HWA Done semaphore object */
    SemaphoreP_Object  localMaxHwaDoneSemaHandle[2];
#endif
    /*! @brief Flag to indicate if DPU is in processing state */
    bool inProgress;

    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPong[2];

    /*! @brief  HWA memory bank addresses */
    uint32_t hwaMemBankAddr[DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS];

    /*! @brief  Number of Doppler chirps. */
    uint16_t    numDopplerChirps;

    /*! @brief  Number of Doppler bins */
    uint16_t    numDopplerBins;

    /*! @brief  HWA translated memory addresses, first index for ping/pong, second index for source/destination */
    uint32_t    hwaLocMemAddr[2][2];

    /*! @brief  Decompression config */
    dopplerProcHWADDMADecompressionCfg decompCfg;

    /*! @brief  DopplerDemod config */
    dopplerProcHWADDMADopplerDemodCfg dopplerDemodCfg;

    /*! @brief CFAR-AzimFFT config */
    dopplerProcHWADDMAcfarAzimFFTCfg cfarAzimFFTCfg;

#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    /*! @brief Number of ping peaks detected by CFAR */
    uint32_t    numCfarPeaksPing;

    /*! @brief Number of pong peaks detected by CFAR */
    uint32_t    numCfarPeaksPong;
#endif

    /*! @brief Number of objects out */
    uint32_t    numObjOut;

}DPU_DopplerProcHWA_Obj;


#ifdef __cplusplus
}
#endif

#endif
