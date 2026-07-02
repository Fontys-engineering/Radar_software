/**
 *   @file  aoaprochwa.h
 *
 *   @brief
 *      Implements Data path processing functionality.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018-2025 Texas Instruments, Inc.
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
#ifndef AOAPROC_HWA_H
#define AOAPROC_HWA_H

#ifdef __cplusplus
extern "C" {
#endif
/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

/* PDK drivers include files */
#include <drivers/hwa.h>

/* mmwave SDK board (EVM) specific files */
#ifdef SOC_AWR2943
#include <ti/common/sys_common_awr2943.h>
#endif

#ifdef SOC_AWR2944
#include <ti/common/sys_common_awr2944.h>
#endif

#ifdef SOC_AWR2X44P
#include <ti/common/sys_common_awr2x44P.h>
#endif

#include <ti/board/antenna_geometry.h>

/* Datapath files */
#include <ti/datapath/dpif/dpif_radarcube.h>
#include <ti/datapath/dpif/dpif_pointcloud.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpu/aoaproc/aoaproc_common.h>


/** @addtogroup DPU_AOAPROC_EXTERNAL_DEFINITIONS
 *
 @{ */


/*! @brief Alignment for memory allocation purpose for R5F. There is CPU access
 *         of this buffer in the implementation.
 */
#define DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_R5F    CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for memory allocation purpose for DSP. There is CPU access
 *         of this buffer in the implementation.
 */
#define DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_DSP    DPIF_CFAR_DET_LIST_CPU_BYTE_ALIGNMENT

/*! @brief Alignment for memory allocation purpose for R5F. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT_R5F  CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for memory allocation purpose for DSP. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT_DSP  DPIF_POINT_CLOUD_CARTESIAN_CPU_BYTE_ALIGNMENT

/*! @brief Alignment for memory allocation purpose for R5F. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT_R5F  CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for memory allocation purpose for DSP. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT_DSP  DPIF_POINT_CLOUD_SIDE_INFO_CPU_BYTE_ALIGNMENT

/*! @brief Alignment for memory allocation purpose for R5F. There is CPU access
 *         of this buffer in the implementation.
 */
#define DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT_R5F CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for memory allocation purpose for DSP. There is CPU access
 *         of this buffer in the implementation.
 */
#define DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT_DSP (sizeof(int16_t))

/*! @brief   Log2 of the size of azimuth FFT */
#define DPU_AOAPROCHWA_LOG2_NUM_ANGLE_BINS 6U /* FFT Size = 64 */

/*! @brief   Size of azimuth FFT */
#define DPU_AOAPROCHWA_NUM_ANGLE_BINS (1U << DPU_AOAPROCHWA_LOG2_NUM_ANGLE_BINS)

/*! @brief Alignment for memory allocation purpose for R5F. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT_R5F  CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for memory allocation purpose for DSP. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT_DSP (sizeof(uint8_t))

/*! @brief Alignment for memory allocation purpose for R5F. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT_R5F  CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for memory allocation purpose for DSP. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT_DSP (sizeof(float))

/*! @brief Alignment for R5F for local scratch memory allocation purpose. */
#define DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT_R5F   CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for DSP for local scratch memory allocation purpose. */
#define DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT_DSP (sizeof(uint32_t))

/*! @brief Number of local scratch buffers */
#define DPU_AOAPROCHWA_NUM_LOCAL_SCRATCH_BUFFERS (2)

/*! @brief Local scratch buffer size. The size maximum of the two buffers that are overlayed:
 *         Buffer 1 (azimElevLocalBuf and azimElevLocalHypothesesBuf) size:
 *                    (NTxAnt + 1) * NTxAnt * NRxAnt * sizeof(uint32_t)
 *         Buffer 2 (azimuthFftOutMagBuf) size:
 *                    NTxAnt * @ref DPU_AOAPROCHWA_NUM_ANGLE_BINS * sizeof(uint16_t)
 *         The size of the second buffer is greater than  the first one as long as
 *         (NTxAnt + 1) * NRxAnt < 32, which is true, therefore the size of the
 *         second buffer is defined below.
          Note: For nTx*nTx*nRx*sizeof(uint32_t) == 64, this is the case where
                only combination that is valid is nTx = 2, nRx = 4,
                we do the EDMA 4K speculative correction for some devices
                which advances the azimElevLocalHypothesesBuf by a maximum of 64.
                In this case, buffer1 size = 32 + (64 + 64) = 160
                buffer2 size = 256, so condition above is still true */
#define DPU_AOAPROCHWA_NUM_LOCAL_SCRATCH_BUFFER_SIZE_BYTES(numTxAntennas) \
    ((numTxAntennas) * DPU_AOAPROCHWA_NUM_ANGLE_BINS * sizeof(uint16_t))

#define DPU_AOAPROCHWA_MAX_ELEVFFTPARAMS_FOR_ZERO_INSERTION (2U)
/**
@}
*/

/** @addtogroup DPU_AOAPROC_ERROR_CODE
 *  Base error code for the aoaProc DPU is defined in the
 *  \include ti/datapath/dpif/dp_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument (general e.g NULL pointers)
 */
#define DPU_AOAPROCHWA_EINVAL                  (DP_ERRNO_AOA_PROC_BASE-1)

/**
 * @brief   Error Code: Invalid argument - radar cube format
 */
#define DPU_AOAPROCHWA_EINVAL__RADARCUBE_DATAFORMAT  (DP_ERRNO_AOA_PROC_BASE-2)

/**
 * @brief   Error Code: Out of memory when allocating using MemoryP_osal
 */
#define DPU_AOAPROCHWA_ENOMEM                               (DP_ERRNO_AOA_PROC_BASE-20)

/**
 * @brief   Error Code: Memory not aligned for @ref DPU_AoAProcHWA_HW_Resources::cfarRngDopSnrList
 */
#define DPU_AOAPROCHWA_ENOMEMALIGN_CFAR_DET_LIST            (DP_ERRNO_AOA_PROC_BASE-21)

/**
 * @brief   Error Code: Memory not aligned for @ref DPU_AoAProcHWA_HW_Resources::detObjOut
 */
#define DPU_AOAPROCHWA_ENOMEMALIGN_POINT_CLOUD_CARTESIAN    (DP_ERRNO_AOA_PROC_BASE-22)

/**
 * @brief   Error Code: Memory not aligned for @ref DPU_AoAProcHWA_HW_Resources::detObjOutSideInfo
 */
#define DPU_AOAPROCHWA_ENOMEMALIGN_POINT_CLOUD_SIDE_INFO    (DP_ERRNO_AOA_PROC_BASE-23)

/**
 * @brief   Error Code: Insufficient memory allocated for azimuth static heat map
 */
#define DPU_AOAPROCHWA_ENOMEM__AZIMUTH_STATIC_HEAT_MAP      (DP_ERRNO_AOA_PROC_BASE-24)

/**
 * @brief   Error Code: Memory not aligned for @ref DPU_AoAProcHWA_HW_Resources::azimuthStaticHeatMap
 */
#define DPU_AOAPROCHWA_ENOMEMALIGN_AZIMUTH_STATIC_HEAT_MAP  (DP_ERRNO_AOA_PROC_BASE-25)

/**
 * @brief   Error Code: Memory not aligned for @ref DPU_AoAProcHWA_HW_Resources::detObjElevationAngle
 */
#define DPU_AOAPROCHWA_ENOMEMALIGN_DET_OBJ_ELEVATION_ANGLE  (DP_ERRNO_AOA_PROC_BASE-26)

/**
 * @brief   Error Code: azimuth heat-map flag enabled and single virtual antenna not valid combination
 */
#define DPU_AOAPROCHWA_EINVALID_NUM_VIRT_ANT_AND_AZIMUTH_STATIC_HEAT_MAP (DP_ERRNO_AOA_PROC_BASE-27)

/**
 * @brief   Error Code: Configure parameters exceed HWA memory bank size
 */
#define DPU_AOAPROCHWA_EEXCEEDHWAMEM (DP_ERRNO_AOA_PROC_BASE-28)

/**
 * @brief   Error Code: Detected object list size is not even number
 */
#define DPU_AOAPROCHWA_EDETECTED_OBJECT_LIST_SIZE_ODD_NUMBER (DP_ERRNO_AOA_PROC_BASE-29)

/**
 * @brief   Error Code: Memory not aligned for @ref DPU_AoAProcHWA_HW_Resources::localScratchBuffer
 */
#define DPU_AOAPROCHWA_ENOMEMALIGN_LOCAL_SCRATCH_BUF  (DP_ERRNO_AOA_PROC_BASE-30)

/**
 * @brief   Error Code: Insufficient memory allocated for @ref DPU_AoAProcHWA_HW_Resources::localScratchBuffer
 */
#define DPU_AOAPROCHWA_ENOMEM_LOCAL_SCRATCH_BUF      (DP_ERRNO_AOA_PROC_BASE-31)

/**
 * @brief   Error Code: Memory not aligned for @ref DPU_AoAProcHWA_HW_Resources::detObj2dAzimIdx
 */
#define DPU_AOAPROCHWA_ENOMEMALIGN_DET_OBJ2_AZIM_IDX  (DP_ERRNO_AOA_PROC_BASE-32)

/**
 * @brief   Error Code: NUmber of params exceeded for elevation FFT
 */
#define DPU_AOAPROCHWA_EEXCEED_ELEVPARAMS       (DP_ERRNO_AOA_PROC_BASE-33)

/**
 * @brief   Error Code: Internal error
 */
#define DPU_AOAPROCHWA_EINTERNAL               (DP_ERRNO_AOA_PROC_BASE-40)

/**
 * @brief   Error Code: Not implemented
 */
#define DPU_AOAPROCHWA_ENOTIMPL                (DP_ERRNO_AOA_PROC_BASE-50)

/**
 * @brief   Error Code: Semaphore error
 */
#define DPU_AOAPROCHWA_ESEMA                   (DP_ERRNO_AOA_PROC_BASE-60)

/**
@}
*/

/**
* @brief
*  AoAProcHWA DPU control command
*
* @details
*  Defines the AoAProcHWA supported run time command
*
*  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
*/
typedef uint32_t DPU_AoAProcHWA_Cmd;

/**
 * @brief   Command to update field of view configuration, azimuth and elevation selected range 
 */
#define DPU_AoAProcHWA_Cmd_FovAoACfg              ((uint32_t) 0) 
/**
 * @brief   Command to update multiobject beam forming configuration 
 */
#define DPU_AoAProcHWA_Cmd_MultiObjBeamFormingCfg ((uint32_t) 1)
/**
 * @brief   Command to update rx channel phase compensation 
 */
#define DPU_AoAProcHWA_Cmd_CompRxChannelBiasCfg   ((uint32_t) 2)
/**
 * @brief   Command to update Azimuth heat-map configuration 
 */
#define DPU_AoAProcHWA_Cmd_PrepareRangeAzimuthHeatMap  ((uint32_t) 3)
/**
 * @brief   Command to update field of extended maximum velocity 
 */
#define DPU_AoAProcHWA_Cmd_ExtMaxVelocityCfg           ((uint32_t) 4)


/**
 * @brief
 *  AoAProcHWA DPU configuration
 *
 * @details
 *  The structure is used to hold the HWA configuration needed for AOA
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_AoAProcHWA_hwaAoAConfig_t
{
    /** @brief     HWA paramset Start index */
    uint8_t    paramSetStartIdx;

    /** @brief     number of HWA paramset*/
    uint8_t    numParamSet;

    /*! @brief     Flag to indicate if HWA windowing is symmetric
                    see HWA_WINDOW_SYMM definitions in HWA driver's doxygen documentation
     */
    uint8_t   winSym;

    /*! @brief Doppler FFT window size in bytes.
         This is the number of coefficients to be programmed in the HWA for the windowing
         functionality. The size is a function of @ref DPU_AoAProcHWA_StaticConfig::numDopplerChirps as follows:\n
         If non-symmetric window is selected: windowSize = numDopplerChirps * sizeof(int32_t) \n
         If symmetric window is selected and numDopplerChirps is even:
         windowSize = numDopplerChirps * sizeof(int32_t) / 2 \n
         If symmetric window is selected and numDopplerChirps is odd:
         windowSize = (numDopplerChirps + 1) * sizeof(int32_t) / 2
    */
    uint32_t    windowSize;

    /*! @brief Pointer to doppler FFT window coefficients. */
    int32_t     *window;

    /*! @brief   Offset in HWA window RAM for singleBin doppler FFT
     *            in number of samples */
    uint32_t     winRamOffset;
}DPU_AoAProcHWA_hwaAoAConfig;

/**
 * @brief
 *  AoAProcHWA DPU initial configuration parameters
 *
 * @details
 *  The structure is used to hold the DPU initial configurations parameters.
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_AoAProcHWA_InitParams_t
{
    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;
}DPU_AoAProcHWA_InitParams;

/**
 * @brief
 *  AoAProcHWA DPU EDMA configuration
 *
 * @details
 *  EDMA configuration for input/output to HWA for calculation of zero-Doppler 2D-FFT used for azimuth heatmap
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProcHWA_EdmaHwaInOut_t
{
    /*! @brief     EDMA configuration for AOA data In */
    DPEDMA_ChanCfg       in;

    /*! @brief     EDMA configuration for AOA data In signaure */
    DPEDMA_ChanCfg       inSignature;

    /*! @brief     EDMA configuration for AOA data Out */
    DPEDMA_ChanCfg       out;

} DPU_AoAProcHWA_EdmaHwaInOut;

/**
 * @brief
 *  AoAProcHWA DPU EDMA channel configuration
 *
 * @details
 *  DMA physical channel configuration
 *
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProcHWA_ChanCfg_t
{
    /*! @brief     EDMA channel id */
    uint8_t             channel;

    /*! @brief     EDMA event Queue used for the transfer */
    uint8_t             eventQueue;
} DPU_AoAProcHWA_ChanCfg;

/**
 * @brief
 *  AoAProcHWA DPU EDMA configuration for HWA input/output
 *
 * @details
 *  EDMA Param sets used for data transfer to and from HWA
 *
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProcHWA_EdmaHwaInOutParamId_t
{
    /*! @brief     EDMA configuration for AOA data In */
    uint16_t       paramIn;

    /*! @brief     EDMA configuration for AOA data In signaure */
    uint16_t       paramInSignature;

    /*! @brief     EDMA configuration for AOA data Out */
    uint16_t       paramOut;
    /*! @brief     EDMA to read CFAR peak counts. This is for future use. */
    uint16_t       paramPeakCnt;

    /*! @brief     EDMA to trigger HWA to continue running. This is for future use. */
    uint16_t       paramHwaContinue;


} DPU_AoAProcHWA_EdmaHwaInOutParamId;

/**
 * @brief
 *  AoAProcHWA DPU EDMA configuration
 *
 * @details
 *  EDMA configuration for input/output to HWA for 2D-FFT and 3D-FFT stages
 *
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProcHWA_EdmaHwaDataInOut_t
{
    /*! @brief     EDMA physical channel In */
    DPU_AoAProcHWA_ChanCfg       chIn;

    /*! @brief     EDMA physical channel Out */
    DPU_AoAProcHWA_ChanCfg       chOut;

    /*! @brief     EDMA configuration in/out per stage: 0: 2D-FFT and 1: 3D-FFT */
    DPU_AoAProcHWA_EdmaHwaInOutParamId stage[2];

    /*! @brief     EDMA event Queue used for the transfer */
    uint8_t             eventQueue;
} DPU_AoAProcHWA_EdmaHwaDataInOut;

/**
 * @brief
 *  AoAProcHWA DPU Hardware resources
 *
 * @details
 *  AoAProcHWA DPU Hardware resources
 *
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_AoAProcHWA_Resources_t
{
    /*! @brief     EDMA Handle */
    EDMA_Handle         edmaHandle;

    /*! @brief     EDMA configuration for azimuth heatmap computation (zero-Doppler 2D-FFT calculation for all range bins), 0-ping, 1-pong */
    DPU_AoAProcHWA_EdmaHwaInOut edmaHwa[2];

    /*! @brief     EDMA configuration, for point-cloud computation including 2D-FFT and 3D-FFT,  0-ping, 1-pong*/
    DPU_AoAProcHWA_EdmaHwaDataInOut edmaHwaExt[2];

    /*! @brief     HWA Configuration */
    DPU_AoAProcHWA_hwaAoAConfig    hwaCfg;

    /*! @brief     Radar Cube structure */
    DPIF_RadarCube      radarCube;

    /*! @brief      Input list of CFAR detected objects of @ref cfarRngDopSnrListSize elements,
     *              must be aligned to:\n
     *              For R5F: @ref DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_R5F \n
     *              For DSP: @ref DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_DSP \n
     *              Although this is an input list, this list may be updated
     *              (written) during AoA processing */
    DPIF_CFARDetList *cfarRngDopSnrList;

    /*! @brief      CFAR detected objects list size */
    uint16_t        cfarRngDopSnrListSize;

    /*! @brief      Detected objects output list sized to @ref detObjOutMaxSize elements,
     *              must be aligned to:\n
     *              For R5F: @ref DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT_R5F \n
     *              For DSP: @ref DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT_DSP \n
     *              Although this is an input list, this list may be updated
     *              (written) during AoA processing */
    DPIF_PointCloudCartesian *detObjOut;

    /*! @brief      Detected objects side information (snr + noise) output list,
     *              sized to @ref detObjOutMaxSize elements,
     *              must be aligned to:\n
     *              For R5F: @ref DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT_R5F \n
     *              For DSP: @ref DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT_DSP */
    DPIF_PointCloudSideInfo *detObjOutSideInfo;

    /*! @brief      This field dimensions several other fields in this structure as
     *              referred in their descriptions. It is determined by the dpc/application based
     *              on balancing between maximum number of objects expected to be
     *              detected in the scene (this can depend on configuration like cfar thresholds,
     *              static clutter removal etc) and memory and MIPS limitations.
     *              Must be even. */
    uint32_t        detObjOutMaxSize;

    /*! @brief      Pointer to range-azimuth static heat map, this is a 2D FFT
     *              array in range direction (x[numRangeBins][numVirtualAntAzim]),
     *              at doppler index 0, sized to @ref azimuthStaticHeatMapSize elements of
     *              type cplx16ImRe_t.
     *              must be aligned to:\n
     *              For R5F: @ref DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT_R5F \n
     *              For DSP: @ref DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT_DSP */
    cmplx16ImRe_t   *azimuthStaticHeatMap;

    /*! @brief      Number of elements of azimuthStaticHeatMap, this should be
     *              numVirtualAntAzim * numRangeBins */
    uint32_t        azimuthStaticHeatMapSize;

    /*! @brief      Detected objects azimuth index for debugging,
     *              sized to @ref detObjOutMaxSize elements, must be aligned to:\n
     *              For R5F: @ref DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT_R5F \n
     *              For DSP: @ref DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT_DSP */
    uint8_t         *detObj2dAzimIdx;

    /*! @brief      Detected object elevation angle for debugging,
     *              sized to @ref detObjOutMaxSize elements, must be aligned to:\n
     *              For R5F: @ref DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT_R5F \n
     *              For DSP: @ref DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT_DSP */
    float           *detObjElevationAngle;

	/*! @brief      Local scratch buffers, each buffer must be aligned to \n
	 *              For R5F: @ref DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT_R5F \n
	 *              For DSP: @ref DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT_DSP */
	uint8_t         *localScratchBuffer[DPU_AOAPROCHWA_NUM_LOCAL_SCRATCH_BUFFERS];

    /*! @brief      Local scratch buffer size = numTxAntennas * DPU_AOAPROCHWA_NUM_ANGLE_BINS * sizeof(uint16_t).
                    A convenient macro @ref DPU_AOAPROCHWA_NUM_LOCAL_SCRATCH_BUFFER_SIZE_BYTES has
                    been provided for calculating this size */
    uint32_t        localScratchBufferSizeBytes;
} DPU_AoAProcHWA_HW_Resources;

/**
 * @brief
 *  Static clutter removal configuration
 *
 * @details
 *  The structure contains the configuration for static clutter removal.
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_AoAProcHWA_StaticClutterRemovalCfg_t
{
    /*! @brief Flag that indicates if static clutter removal is enabled.*/
    bool  isEnabled;
}DPU_AoAProcHWA_StaticClutterRemovalCfg;

/*
 * @brief Antenna Spacing
 */
typedef struct DPU_AoAProcHWA_AntennaSpacing_t
{
    /* Spacing in X dimension */
    float xSpacingByLambda;

    /* Spacing in Z dimension */
    float zSpacingByLambda;
}DPU_AoAProcHWA_AntennaSpacing;

/*
 * @brief Zero Insertion Mask configuration
 */
typedef struct DPU_AoAProcHWA_ZeroInsrtMaskCfg_t
{
    /* Spacing in X dimension */
    uint32_t zeroInsrtMaskAzim;

    /* Spacing in Z dimension */
    uint32_t zeroInsrtMaskElev;
}DPU_AoAProcHWA_ZeroInsrtMaskCfg;


/**
 * @brief Structure to hold the elevation FFT param configuration
 */
typedef struct DPU_AoAProc_elevParamCfg_t{
    /* Src Address Offset */
    uint8_t srcAddOffset;
    /* Dst Address */
    uint8_t dstAddOffset;
    /* Acnt */
    uint8_t acnt;
    /* Dst AIdx */
    uint8_t dstAidx;
} DPU_AoAProc_elevParamCfg;

/**
 * @brief
 *  AoAProcHWA DPU static configuration parameters
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 * @details
 * The following conditions must be satisfied:
 *    @verbatim
      numTxAntennas * numRxAntennas * numDopplerChirps * sizeof(cmplx16ImRe_t) <= 16 KB (one HWA memory bank)
      numTxAntennas * numRxAntennas * numDopplerBins * sizeof(cmplx16ImRe_t) <= 32 KB (two HWA memory banks)
      @endverbatim
 *
 */
typedef struct DPU_AoAProcHWA_StaticConfig_t
{
    /*! @brief  Number of transmit antennas */
    uint8_t     numTxAntennas;

    /*! @brief  Number of receive antennas */
    uint8_t     numRxAntennas;

    /*! @brief  Number of virtual azimuth antennas */
    uint8_t     numVirtualAntAzim;

    /*! @brief  Number of virtual elevation antennas */
    uint8_t     numVirtualAntElev;

    /*! @brief  Number of range bins */
    uint16_t    numRangeBins;

    /*! @brief Number of chirps for Doppler computation purposes.
               For example, in TDM/BPM-MIMO scheme, this is the physical chirps
               in a frame per transmit antenna
               i.e numDopplerChirps = numChirpsPerFrame / numTxAntennas */
    uint16_t    numDopplerChirps;

    /*! @brief  Number of doppler bins */
    uint16_t    numDopplerBins;

    /*! @brief  Range conversion factor for range FFT index to meters */
    float       rangeStep;

    /*! @brief  Doppler conversion factor for Doppler FFT index to m/s */
    float       dopplerStep;

    /*! @brief  umParams Based on the antenna pattern, and zero insertion Mask*/
    uint8_t     numElevFFTParams;

    /*! @brief  ELveation FFT Paramsets Configuration */
    DPU_AoAProc_elevParamCfg elevFFTParamCfg[DPU_AOAPROCHWA_MAX_ELEVFFTPARAMS_FOR_ZERO_INSERTION];

    /*! @brief  Pointer to antenna geometry definition. This is for future use */
    ANTDEF_AntGeometry *antDef;

    uint16_t antennaGeometryCfg[SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL];

    /*! @brief Antenna Spacing in X and Z dim */
    DPU_AoAProcHWA_AntennaSpacing antennaSpacing;

    /*! @brief Zero Insertion Mask configuration: to insert zeros in virtual antenna array */
    DPU_AoAProcHWA_ZeroInsrtMaskCfg zeroInsrtMaskCfg;

    /*! @brief     Static clutter removal configuration */
    DPU_AoAProcHWA_StaticClutterRemovalCfg staticClutterCfg;

} DPU_AoAProcHWA_StaticConfig;

/**
 * @brief
 *  AoAProcHWA DPU configuration
 *
 * @details
 *  The structure is used to hold the AoAProcHWA configuration
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_AoAProcHWA_Config_t
{
    /*! @brief     Data path common parameters */
    DPU_AoAProcHWA_StaticConfig   staticCfg;

    /*! @brief     Hardware resources */
    DPU_AoAProcHWA_HW_Resources res;

    /*! @brief     Dynamic configuration */
    DPU_AoAProc_DynamicConfig dynCfg;

}DPU_AoAProcHWA_Config;

/**
 * @brief
 *  Output parameters populated during Processing time
 *
 * @details
 *  The structure is used to hold the output parameters
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_AoAProcHWA_OutParams_t
{

    /*! @brief     AoAProcHWA stats */
    DPU_AoAProc_Stats       stats;

    /*! @brief      Number of AoA DPU detected points*/
    uint32_t numAoADetectedPoints;

}DPU_AoAProcHWA_OutParams;

/**
 * @brief
 *  AoAProcHWA DPU Handle
 *
 *
 *  \ingroup DPU_AOAPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef void* DPU_AoAProcHWA_Handle;

/**
 *  @b Description
 *  @n
 *      The function returns number of used HWA Param sets
 *
 *  @param[in]  numTxAnt               Number of Tx antennas in TDM MIMO scheme
 *
 *  @param[in]  numRxVirtAntElevation  Number of Rx vitual elevation antennas
 * 
 *  @param[in]  numElevFFTParamsZerIns Number of Elevation FFT Zero Insertion Params 
 *
 *  @param[in]  isStaticClutterRemovalEnabled  Static clutter removal flag
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Number of used HWA Param sets
 */
uint16_t DPU_AoAProcHWA_getNumHwaParamSets
(
    uint16_t numTxAnt,
    uint16_t numRxVirtAntElevation,
    uint8_t numElevFFTParamsZerIns,
    bool     isStaticClutterRemovalEnabled
);

/**
 *  @b Description
 *  @n
 *      The function is AoAProcHWA DPU initialization function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]  initParams              Pointer to initialization parameters
 *
 *  @param[in]  subframeCounter         sub frame counter max is RL_MAX_SUBFRAMES
 *
 *  @param[in]  errCode                 Pointer to errCode generates from the API
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid AoAProcHWA handle
 *  @retval
 *      Error       - NULL
 */
DPU_AoAProcHWA_Handle DPU_AoAProcHWA_init
(
    DPU_AoAProcHWA_InitParams *initParams,
    volatile uint8_t    subframeCounter,
    int32_t*            errCode
);

/**
 *  @b Description
 *  @n
 *      The function is AoAProcHWA DPU configuration function. It saves buffer pointer and configurations
 *  including system resources and configures EDMA for runtime range processing.
 *
 *  @pre    DPU_AoAProcHWA_init() has been called
 *
 *  @param[in]  handle                  AoAProcHWA DPU handle
 *  @param[in]  aoaHwaCfg              Pointer to AoAProcHWA configuration data structure
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_AoAProcHWA_config
(
    DPU_AoAProcHWA_Handle    handle,
    DPU_AoAProcHWA_Config    *aoaHwaCfg
);

/**
 *  @b Description
 *  @n
 *      The function is AoAProcHWA DPU process function. It performs CFAR detection using HWA
 *
 *  @pre    DPU_AoAProcHWA_init() has been called
 *
 *  @param[in]  handle                  AoAProcHWA DPU handle
 *
 *  @param[in]  numObjsIn               Number of detected objects by CFAR DPU
 *
 *  @param[in]  outParams               DPU output parameters
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
int32_t DPU_AoAProcHWA_process
(
    DPU_AoAProcHWA_Handle    handle,
    uint32_t        numObjsIn,
    DPU_AoAProcHWA_OutParams  *outParams
);

/**
 *  @b Description
 *  @n
 *      The function is AoAProcHWA DPU control function.
 *
 *  @pre     DPU_AoAProcHWA_init() has been called
 *
 *  @param[in]  handle           AoAProcHWA DPU handle
 *  @param[in]  cmd              AoAProcHWA DPU control command
 *  @param[in]  arg              AoAProcHWA DPU control argument pointer
 *  @param[in]  argSize          AoAProcHWA DPU control argument size
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_AoAProcHWA_control
(
   DPU_AoAProcHWA_Handle handle,
   DPU_AoAProcHWA_Cmd cmd,
   void *arg,
   uint32_t argSize
);

/**
 *  @b Description
 *  @n
 *      The function is AoAProcHWA DPU deinitialization function. It frees up the
 *   resources allocated during initialization.
 *
 *  @pre    DPU_AoAProcHWA_init() has been called
 *
 *  @param[in]  handle           AoAProcHWA DPU handle
 *
 *  \ingroup    DPU_AOAPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_AoAProcHWA_deinit
(
    DPU_AoAProcHWA_Handle handle
);

#ifdef __cplusplus
}
#endif

#endif
