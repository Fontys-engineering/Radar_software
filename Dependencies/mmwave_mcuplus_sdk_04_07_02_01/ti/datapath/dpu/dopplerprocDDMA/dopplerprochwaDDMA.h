/**
 *   @file  dopplerprochwaDDMA.h
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
#ifndef DOPPLERPROC_HWA_H
#define DOPPLERPROC_HWA_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

/* mmWave SDK Driver/Common Include Files */
// #include <ti/drv/hwa/hwa.h>
#include <drivers/edma.h>
#include <drivers/hwa.h>

/* DPIF Components Include Files */
#include <ti/datapath/dpif/dpif_detmatrix.h>
#include <ti/datapath/dpif/dpif_radarcube.h>
#include <ti/datapath/dpif/dpif_pointcloud.h>

/* mmWave SDK Data Path Include Files */
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpu/dopplerprocDDMA/dopplerprocDDMAcommon.h>

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup DPU_DOPPLERPROC_ERROR_CODE
 *  Base error code for the dopplerProc DPU is defined in the
 *  \include ti/datapath/dpif/dp_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define DPU_DOPPLERPROCHWA_EINVAL                  (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define DPU_DOPPLERPROCHWA_ENOMEM                  (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-2)

/**
 * @brief   Error Code: DPU is in progress
 */
#define DPU_DOPPLERPROCHWA_EINPROGRESS             (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-3)

/**
 * @brief   Error Code: Out of HWA resources
 */
#define DPU_DOPPLERPROCHWA_EHWARES                 (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-4)

/**
 * @brief   Error Code: Semaphore creation failed
 */
#define DPU_DOPPLERPROCHWA_ESEMA                   (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-5)

/**
 * @brief   Error Code: Bad semaphore status
 */
#define DPU_DOPPLERPROCHWA_ESEMASTATUS             (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-6)

/**
 * @brief   Error Code: Configure parameters exceed HWA memory bank size
 */
#define DPU_DOPPLERPROCHWA_EEXCEEDHWAMEM           (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-7)

/**
 * @brief   Error Code: Unsupported radar cube format
 */
#define DPU_DOPPLERPROCHWA_ECUBEFORMAT             (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-8)

/**
 * @brief   Error Code: Unsupported detection matrix format
 */
#define DPU_DOPPLERPROCHWA_EDETMFORMAT             (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-9)

/**
 * @brief   Error Code: Insufficient detection matrix size
 */
#define DPU_DOPPLERPROCHWA_EDETMSIZE               (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-10)

/**
 * @brief   Error Code: Wrong window size
 */
#define DPU_DOPPLERPROCHWA_EWINDSIZE               (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-11)

/**
 * @brief   Error Code: Number of chirps per ping for decompression < 1
 */
#define DPU_DOPPLERPROCHWA_ERROR_NUMCHIRPSPERPING          (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-12)

/**
 * @brief   Error Code: Number of Rx antennas per block should be the same as the number
 *                      of Rx antennas
 */
#define DPU_DOPPLERPROCHWA_ERROR_NUMRXANTPERBLOCK_DECOMPRESSION        (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-13)

/**
 * @brief   Error Code: RangeBinsPerBlock for decompression should be a power of 2
 */
#define DPU_DOPPLERPROCHWA_ERROR_RANGEBINSPERBLOCK_DECOMPRESSION       (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-14)

/**
 * @brief   Error Code: decompression method 0 and 1 are supported
 */
#define DPU_DOPPLERPROCHWA_ERROR_METHOD_DECOMPRESSION                  (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-15)

/**
 * @brief   Error Code: CFAR Config error
 */
#define DPU_DOPPLERPROCHWA_ERROR_METHOD_CFAR                  (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-16)

/**
 * @brief   Error Code: Find Max Idx error
 */
#define DPU_DOPPLERPROCHWA_ERROR_FINDMAX                  (DP_ERRNO_DOPPLER_PROC_DDMA_BASE-17)

/**
@}
*/
/** @brief This limits the detected number of objects per range bin. It helps in detecting more objects at
 * long distance, with limited available memory.
 * With detObjList written over the radar cube, it is essential to keep this macro enabled to avoid
 * overflow and detect objects at far range-bin. */
#define LIMIT_DETECTED_OBJS_PER_RANGEBIN

#ifdef LIMIT_DETECTED_OBJS_PER_RANGEBIN
/** @brief This macro defines the maximum number of objects that can be stored per range bin. */
#define MAX_NUM_OBJ_PER_RANGE_BIN (40U)
#endif


/* Uncommenting the following macro enables the histogram based detection and
 * disables the doppler CFAR stage.
 * For bit-exact testing of this feature, user needs to generate test vectors from
 * Matlab model by enabling the 'useHistogram' option in main script. */

// #define ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
/* CDF Bin Number which crosses the CDF_CNT_THRES * dopSubBins mark is used for LM engine SW thresholds. */
#define CDF_CNT_THRES                      (0.9f)
#define HIST_SIZE_SELECT                   (6U)
#define HIST_SCALE_SELECT                  (10U)
#define HIST_DOPPLER_DIM_OFFSET_DB         (10U) /* dB */
#endif

/**
 * @brief   Number of HWA memory banks needed
 */
#define DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS  8U

#define DOPPLERPROCHWADDMA_NUM_EDMA_INTERRUPTS 3U

#define DOPPLERPROCHWA_DDMA_DECOMP_NUM_HWA_PARAMSETS         2U
#define DOPPLERPROCHWA_DDMA_BFP_DECOMP_NUM_HWA_PARAMSETS     8U
#define DOPPLERPROCHWA_DDMA_DOPPLER_NUM_HWA_PARAMSETS       12U

#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
#define DOPPLERPROCHWA_DDMA_AZIMCFAR_NUM_HWA_PARAMSETS       12U
#else
#define DOPPLERPROCHWA_DDMA_AZIMCFAR_NUM_HWA_PARAMSETS      14U
#endif

/**
 * @brief   16KB of HWA MemBank size for Decompression
 */
#define DECOMP_HWA_MEMBANK_SIZE 16384U


/*! @brief Alignment for memory allocation purpose of detection matrix.
 *         There is CPU access of detection matrix in the implementation.
 */
#define DPU_DOPPLER_DET_MATRIX_BYTE_ALIGNMENT CSL_CACHE_L1D_LINESIZE

#define NUM_DDMA_BANDS 6U
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
#define NUM_TXANT 4U
#define NUM_TXANT_AZIM 3U
#define NUM_TXANT_ELEV 1U
#elif defined(SOC_AWR2943)
#define NUM_TXANT 3U
#define NUM_TXANT_AZIM 2U
#define NUM_TXANT_ELEV 1U

#endif
#define NUM_RXANT 4U


/*!
 *  @brief   Handle for Doppler Processing DPU.
 */
typedef void*  DPU_DopplerProcHWA_Handle;

/**
 * @brief
 *  dopplerProc DPU initial configuration parameters
 *
 * @details
 *  The structure is used to hold the DPU initial configurations.
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_InitCfg_t
{
    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;

}DPU_DopplerProcHWA_InitParams;

typedef struct DPU_DopplerProcHWADDMA_HwaStateMachineCfg_t{
        /*! @brief Number of HWA paramsets reserved for the Doppler DPU.
         The DPU will use numParamSets consecutively, starting from paramSetStartIdx.\n
    */
    uint8_t     numParamSets;

    /*! @brief HWA paramset Start index.
         Application has to ensure that paramSetStartIdx is such that \n
        [paramSetStartIdx, paramSetStartIdx + 1, ... (paramSetStartIdx + numParamSets - 1)] \n
        is a valid set of HWA paramsets.\n
    */
    uint32_t    paramSetStartIdx;
}DPU_DopplerProcHWADDMA_HwaStateMachineCfg;


/**
 * @brief
 *  dopplerProc DPU HWA configuration parameters
 *
 * @details
 *  The structure is used to hold the HWA configuration parameters
 *  for the Doppler Processing DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
     */
typedef struct DPU_DopplerProcHWA_HwaCfg_t
{
    /*! @brief Indicates if HWA window is symmetric or non-symmetric.
        Use HWA macro definitions for symmetric/non-symmetric.
    */
    uint8_t     winSym;

    /*!  @brief Doppler FFT window size in bytes.
         This is the number of coefficients to be programmed in the HWA for the windowing
         functionality. The size is a function of numDopplerChirps as follows:\n
         If non-symmetric window is selected: windowSize = numDopplerChirps * sizeof(int32_t) \n
         If symmetric window is selected and numDopplerChirps is even:
         windowSize = numDopplerChirps * sizeof(int32_t) / 2 \n
         If symmetric window is selected and numDopplerChirps is odd:
         windowSize = (numDopplerChirps + 1) * sizeof(int32_t) / 2
    */
    uint32_t    windowSize;

    /*! @brief Pointer to Doppler FFT window coefficients. */
    int32_t     *window;

    /*! @brief HWA window RAM offset in number of samples. */
    uint32_t    winRamOffset;

    /*! @brief Number of HWA paramsets reserved for the Doppler DPU.
         The DPU will use numParamSets consecutively, starting from paramSetStartIdx.\n
    */
    uint8_t     numParamSets;

    /*! @brief HWA paramset Start index.
         Application has to ensure that paramSetStartIdx is such that \n
        [paramSetStartIdx, paramSetStartIdx + 1, ... (paramSetStartIdx + numParamSets - 1)] \n
        is a valid set of HWA paramsets.\n
    */
    uint32_t    paramSetStartIdx;

    DPU_DopplerProcHWADDMA_HwaStateMachineCfg   decompStageHwaStateMachineCfg;
    DPU_DopplerProcHWADDMA_HwaStateMachineCfg   dopplerStageHwaStateMachineCfg;
    DPU_DopplerProcHWADDMA_HwaStateMachineCfg   azimCfarStageHwaStateMachineCfg;

}DPU_DopplerProcHWA_HwaCfg;

/**
 * @brief
 *  dopplerProc DPU EDMA configuration parameters
 *
 * @details
 *  The structure is used to hold the EDMA configuration parameters
 *  for the Doppler Processing DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProc_DDMA_Decompression_EdmaCfg_t
{

    /*! @brief  EDMA configuration for Decompression input data (Radar cube -> HWA memory). */
    DPU_DopplerProc_Edma edmaIn;

    /*! @brief  EDMA configuration for Decompression input data (Radar cube -> HWA memory). */
    DPU_DopplerProc_Edma edmaInSignature;

    /*! @brief  EDMA configuration for Decompression Output data (HWA memory -> detection matrix). */
    DPU_DopplerProc_Edma edmaOut;

    /*! @brief  EDMA configuration for decompression out. */
    Edma_IntrObject *edmaIntrObjDecompOut;

}DPU_DopplerProc_DDMA_Decompression_EdmaCfg;

/**
 * @brief
 *  dopplerProc DPU EDMA configuration parameters
 *
 * @details
 *  The structure is used to hold the EDMA configuration parameters
 *  for the Doppler Processing DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProc_DDMA_Doppler_EdmaCfg_t
{

    /*! @brief  EDMA configuration for Doppler FFT input data (DecompressedBuf cube -> HWA memory). */
    DPU_DopplerProc_Edma edmaIn;

    /*! @brief  EDMA configuration for Doppler FFT input data Signature Channel. */
    DPU_DopplerProc_Edma edmaInSignature;

    /*! @brief  EDMA configuration for DDMA Metric Output data.
     *  This Channel brings the Max Subband Idx Output if
     *  DDMA demodulation is performed on HWA  */
    DPU_DopplerProc_Edma edmaMaxSubbandOut;

    /*! @brief Interrupt Object for DDMA Metric/ Max Subband Idx Out */
    DPU_DopplerProc_EdmaIntrObj edmaIntrObjMaxSubbandOut;

    /*! @brief  EDMA configuration for SumRX  + log Abs + SumTX Output Output data (HWA memory -> detection matrix). */
    DPU_DopplerProc_Edma edmaSumLogAbsOut;

}DPU_DopplerProc_DDMA_Doppler_EdmaCfg;

/**
 * @brief
 *  dopplerProc DPU EDMA configuration parameters
 *
 * @details
 *  The structure is used to hold the EDMA configuration parameters
 *  for the Doppler Processing DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_EdmaCfg_t
{

    /*! @brief  EDMA driver handle. */
    EDMA_Handle edmaHandle;

    /*! @brief  EDMA config for decompression stage. */
    DPU_DopplerProc_DDMA_Decompression_EdmaCfg decompEdmaCfg;

    /*! @brief  EDMA config for doppler demod stage. */
    DPU_DopplerProc_DDMA_Doppler_EdmaCfg dopplerEdmaCfg;

    /*! @brief  EDMA config for detected objects antenna samples copy. */
    DPEDMA_ChanCfg  edmaDetObjAntSamples;

}DPU_DopplerProcHWA_EdmaCfg;

typedef struct DetObjParams_t
{
    /*! @brief  Azim FFT Samples */
    cmplx32ImRe_t azimSamples[NUM_TXANT_AZIM * NUM_RXANT] __attribute__((aligned(8)));

    /*! @brief  Elev FFT Samples */
    cmplx32ImRe_t elevSamples[NUM_TXANT_ELEV * NUM_RXANT] __attribute__((aligned(8)));

    /*! @brief  Azimuth Index */
    uint32_t    azimIdx;

    /*! @brief  Doppler Idx relative to sub band */
    uint32_t    dopIdx;

    /*! @brief  Range Gate Idx */
    uint32_t    rangeIdx;

    /*! @brief  Actual doppler Idx */
    uint32_t    dopIdxActual;

    /*! @brief  CFAR noise value */
    uint32_t    dopCfarNoise;

    /*! @brief  Peak samples Azim */
    uint32_t    azimPeakSamples[3];

}DetObjParams;

/**
 * @brief
 *  Doppler DPU HW configuration parameters
 *
 * @details
 *  The structure is used to hold the  HW configuration parameters
 *  for the Doppler DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_HW_Resources_t
{
    /*! @brief  EDMA configuration */
    DPU_DopplerProcHWA_EdmaCfg edmaCfg;

    /*! @brief  HWA configuration */
    DPU_DopplerProcHWA_HwaCfg  hwaCfg;

    /*! @brief  Radar Cube (Compressed) */
    DPIF_RadarCube radarCube;

    /*! @brief  Detection matrix */
    DPIF_DetMatrix detMatrix;

    /*! @brief  Local scratch buffer for decompression (will store a
                decompressed radar cube block) */
    uint8_t * decompScratchBuf;

    /*! @brief  Local scratch buffer for decompression */
    uint32_t decompScratchBufferSizeBytes;

#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    /*! @brief  Maximum number of CFAR peaks that can be detected */
    uint32_t maxCfarPeaksToDetect;

    /*! @brief  Range-dependent CFAR threshold scaling Look-Up Table
     *
     * This LUT stores pre-computed threshold scale adjustments for each range bin when
     * variable CFAR threshold mode is enabled (variableThresholdMode = 1).
     *
     * Array size: numRangeBins elements
     * Units: Threshold adjustment value in log2 5.11 fixed-point format (scale factor: 2048)
     *
     * The LUT is populated by DPU_DopplerProcHWA_configCfarThreshScaleLUT() during
     * configuration and implements the following threshold profile:
     * - First 25% of range bins: Constant offset of +7dB above baseline
     * - Remaining 75% of range bins: Linear taper from +7dB to 0dB
     *
     * During processing, the threshold for range bin i is computed as:
     *   actualThreshold = thresholdScale + cfarThreshScaleLUT[i]
     * where thresholdScale is the baseline threshold set for the far-field.
     *
     * Memory allocation: Must be allocated by application before DPU configuration.
     * The array should persist throughout the lifetime of DPU operation.
     */
    uint32_t * cfarThreshScaleLUT;
#endif

    /*! @brief  Local scratch buffer storing Doppler Max SubBand Idx */
    uint8_t * dopMaxSubBandScratchBuf[2];

    /*! @brief  Size of local scratch buffer storing Doppler Max SubBand Idx (Ping + pong)*/
    uint32_t dopMaxSubBandScratchBufferSizeBytes;

    /*! @brief List of detected objects */
    DetObjParams * detObjList;

    /*! @brief Max size of detected objects list to be stored per range-gate after range-dop processing */
    uint32_t maxObjListPerRGateSize;

    /*! @brief Max number of final detected objects to be stored after dop and range cfar stage intersection. */
    uint32_t finalMaxNumDetObjs;

    /*! @brief List of final detected objects after intersection with rangecfar */
    DetObjParams * finalDetObjList;

    /*! @brief      Detected objects output data */
    DPIF_PointCloudCartesian  *  objOut;

    /*! @brief Max size of detected object output data in bytes */
    uint32_t objOutSizeInBytes;

}DPU_DopplerProcHWA_HW_Resources;


/**
 * @brief
 *  DopplerProc HWA DDMA Decompression hardware resources
 *
 * @details
 *  The structure is used to hold the hardware resources needed for decompression of Range FFT
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProc_DecompressionCfg_t
{
    /*! @brief Flag that indicates if compression is enabled */
    bool  isEnabled;

    /*! @brief Compression Method, 0 indicates EGE and 1 indicates BFP */
    uint8_t  compressionMethod;

    /*! @brief Compression ration, a value between 0 and 1 */
    float  compressionRatio;

    /*! @brief Indicates the number of range bins to be compressed in a single compression block */
    uint16_t  rangeBinsPerBlock;

    /*! @brief Can be greater than 1 only for DPIF_RADARCUBE_FORMAT_2.
               For DPIF_RADARCUBE_FORMAT_1 this should be set to 1 */
    uint16_t  numRxAntennaPerBlock;

    /*! @brief extra compression parameters for BFP, dependent on enabled number of RX antenna*/
    uint8_t bfpCompExtraParamSets;

}DPU_DopplerProc_DecompressionCfg;

/**
 * @brief
 *  CFAR Configuration
 *
 * @details
 *  The structure contains the cfar configuration used in data path
 */
typedef struct DPU_DopplerProc_CfarCfg_t
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
     *             0- disabled, 1-enabled
     *
     * When enabled, this feature implements range-dependent CFAR thresholds to improve
     * detection performance across different ranges. The threshold varies per range bin:
     * - Near-field bins (first 25% of range): Higher constant threshold (+7dB above far-field)
     * - Mid-to-far field bins: Linearly decreasing threshold from near to far
     * - Far-field bins: Baseline threshold (specified by thresholdScale parameter)
     *
     * This compensates for the higher clutter and SNR typically present in near-field bins
     * while maintaining sensitivity for far-field targets. The threshold adjustment is
     * computed during configuration and stored in cfarThreshScaleLUT, then applied
     * dynamically during processing on a per-range-bin basis.
     *
     * Note: The thresholdScale parameter should be set for the far-field (last range gate).
     * Near-field thresholds are automatically scaled higher based on the algorithm.
     */
    uint8_t        variableThresholdMode;

} DPU_DopplerProc_CfarCfg;

/**
 * @brief
 *  Local Max Configuration
 *
 * @details
 *  The structure contains the local max thresholds/config used in data path
 */
typedef struct DPU_DopplerProc_LocalMaxCfg_t
{
    /*! @brief    Azim threshold scale */
    uint16_t azimThreshold;

    /*! @brief    Azim threshold scale */
    uint16_t dopplerThreshold;

} DPU_DopplerProc_LocalMaxCfg;

/**
 * @brief
 *  Doppler DPU static configuration parameters
 *
 * @details
 *  The structure is used to hold the static configuration parameters
 *  for the Doppler DPU. The following conditions must be satisfied:
 *
 *    @verbatim
      numTxAntennas * numRxAntennas * numDopplerChirps * sizeof(cmplx16ImRe_t) <= X
         where X = 16 KB (one HWA memory bank) for HWA 1.0
               X = 32 KB (two HWA memory banks) for HWA 2.0

      numTxAntennas * numRxAntennas * numDopplerBins * sizeof(uint16_t) <= X
         where X = 16 KB (one HWA memory bank) for HWA 1.0
               X = 32 KB (two HWA memory banks) for HWA 2.0
      @endverbatim
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_StaticConfig_t
{
    /*! @brief  Number of transmit antennas */
    uint8_t     numTxAntennas;

    /*! @brief  Number of transmit antennas */
    uint8_t     numAzimTxAntennas;

    /*! @brief  Number of receive antennas */
    uint8_t     numRxAntennas;

    /*! @brief  Number of virtual antennas */
    uint8_t     numVirtualAntennas;

    /*! @brief  Number of range bins */
    uint16_t    numRangeBins;

    /*! @brief  Number of Doppler chirps. */
    uint16_t    numChirps;

    /*! @brief  Number of Doppler bins */
    uint16_t    numDopplerFFTBins;

    /*! @brief  Number of Azimuth FFT Bins */
    uint16_t     numAzimFFTBins;

    /*! @brief  Size of input samples (radarcube samples) in bytes */
    uint16_t    sizeOfInputSample;

	/*! @brief  maximum number of detected objects */
    uint32_t    maxNumObj;

    /*! @brief  Log2 of number of Doppler bins */
    uint8_t     log2NumDopplerBins;

    /*! @brief  Total number of subbands */
    uint8_t     numBandsTotal;

    /*! @brief  If range CFAR is disabled, we may disable Sum Tx calculation. */
    uint8_t     isSumTxEnabled;

    /* @brief   Zero Insertion Mask in Azimuth Antenna Array */
    uint64_t     zeroInsrtMaskAzim;

    /*! @brief Decompression Configuration */
    DPU_DopplerProc_DecompressionCfg decompCfg;

#ifndef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    /* CFAR Configuration */
    DPU_DopplerProc_CfarCfg cfarCfg;
#endif
    /* Local Max Configuration */
    DPU_DopplerProc_LocalMaxCfg localMaxCfg;

    /*! @brief Antenna Calibration Configuration, complex float value, hence the factor of 2, IM RE format */
    float antennaCalibParams[NUM_TXANT * NUM_RXANT * 2U];

    uint16_t antennaGeometryCfg[NUM_TXANT * NUM_RXANT];

}DPU_DopplerProcHWA_StaticConfig;

/**
 * @brief
 *  dopplerProc DPU configuration parameters
 *
 * @details
 *  The structure is used to hold the configuration parameters
 *  for the Doppler Processing removal DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_Config_t
{
    /*! @brief HW resources. */
    DPU_DopplerProcHWA_HW_Resources  hwRes;

    /*! @brief Static configuration. */
    DPU_DopplerProcHWA_StaticConfig  staticCfg;

}DPU_DopplerProcHWA_Config;


/**
 * @brief
 *  DPU processing output parameters
 *
 * @details
 *  The structure is used to hold the output parameters DPU processing
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_OutParams_t
{
    /*! @brief DPU statistics */
    DPU_DopplerProc_Stats  stats;

    /*! @brief Number of Objects Out */
    uint32_t  numObjOut;

}DPU_DopplerProcHWA_OutParams;


DPU_DopplerProcHWA_Handle DPU_DopplerProcHWA_init(
    DPU_DopplerProcHWA_InitParams *initCfg,
    volatile uint8_t subframeCounter,
    int32_t* errCode
)
#if defined(SUBSYS_M4) && SUBSYS_M4
__attribute__((section (".customCode")));
#else
;
#endif

int32_t DPU_DopplerProcHWA_config(
    DPU_DopplerProcHWA_Handle handle,
    DPU_DopplerProcHWA_Config *cfg, int32_t isFullConfig
);

int32_t DPU_DopplerProcHWA_process(
    DPU_DopplerProcHWA_Handle handle,
    DPU_DopplerProcHWA_Config *cfg,
    DPU_DopplerProcHWA_OutParams *outParams);

int32_t DPU_DopplerProcHWA_deinit(DPU_DopplerProcHWA_Handle handle);


#ifdef __cplusplus
}
#endif

#endif
