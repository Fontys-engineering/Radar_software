/**
 *   @file  objectdetection.h
 *
 *   @brief
 *      Object Detection DPC Header File
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 - 2026 Texas Instruments, Inc.
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
#ifndef DPC_OBJECTDETECTION__H
#define DPC_OBJECTDETECTION__H

/** Object Detection HWA DDMA
 *
 * This implements the DDMA Data processing chain.
 *
 */

/* MMWAVE Driver Include Files */
#include <ti/common/mmwave_error.h>
#ifdef INCLUDE_DPM
#include <ti/control/dpm/dpm.h>
#endif

#include <ti/datapath/dpu/rangeprocDDMA/rangeprochwaDDMA.h>
#include <ti/datapath/dpu/dopplerprocDDMA/dopplerprochwaDDMA.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/rangecfarprochwa.h>
#include <ti/datapath/dpif/dpif_pointcloud.h>
#include <ti/board/antenna_geometry.h>

#if defined(SOC_AWR2X44P)
#include <ti/control/mmwavelink/mmwavelink.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef SUBSYS_MSS
#define EDMA_NUM_CC EDMA_MSS_NUM_CC
#else
#define EDMA_NUM_CC EDMA_DSS_NUM_CC
#endif

/** @defgroup DPC_OBJDET_EXTERNAL        Object Detection DPC (Data-path Processing Chain) External
 *
 * This DPC performs processes ADC samples and generates detected object list.
 */
/**
@defgroup DPC_OBJDET__GLOBAL                             Object Detection DPC Globals
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the globals exposed by the Object detection DPC.
*/
/**
@defgroup DPC_OBJDET_IOCTL__DATA_STRUCTURES               Object Detection DPC Data Structures
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the data structures which are a part of the DPC module
*   and which are exposed to the application
*/
/**
@defgroup DPC_OBJDET_IOCTL__DEFINITIONS                   Object Detection DPC Definitions
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the definitions which are used by the Object Detection DPC.
*/
/**
@defgroup DPC_OBJDET_IOCTL__COMMAND                       Object Detection DPC Configuration Commands
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the commands which are supported by the DPC.
*   All commands of the type IOCTL__STATIC_<...> can only be issued either before the
*   first call to @ref DPM_start (DPC_ObjectDetection_start) or after the @ref DPM_stop (DPC_ObjectDetection_stop)
*   All commands of the type IOCTL__DYNAMIC_<...> can be issued between at
*   the inter-frame boundary i.e when the result is available from @ref DPM_execute (DPC_ObjectDetection_execute).
*   All commands of the type IOCTL__STATIC_<..> must be issued
*   before @ref DPM_start (DPC_ObjectDetection_start) because there are no defaults.
*/
/**
@defgroup DPC_OBJECTDETECTION_ERROR_CODE                   Object Detection DPC Error Codes
@ingroup DPC_OBJDET_EXTERNAL
@brief
*   This section has a list of all the error codes returned when calling Object Detection DPC functions
*   during error conditions.
*/


/** @addtogroup DPC_OBJDET_IOCTL__DEFINITIONS
 @{ */
/*! @brief Maximum Number of objects that can be detected in a frame */
#define DPC_OBJDET_MAX_NUM_OBJECTS                       1800U

#define DPC_OBJDET_RANGECFAR_MAX_NUM_OBJECTS             500U

/*! Maximum Number of CFAR Peaks can be detected per range bin */
#define DPC_OBJDET_MAX_NUM_CFAR_PEAKS                    256U


#define X_ANTENNA_SPACING_BY_LAMBDA  0.5f
#define Z_ANTENNA_SPACING_BY_LAMBDA  0.8f

#if defined(SOC_AWR2944) ||defined(SOC_AWR2X44P)
#define MAX_NUM_AZIM_TX 3U
#define MAX_NUM_ELEV_TX 1U
#define MAX_NUM_RX 4U
#elif defined(SOC_AWR2943)
#define MAX_NUM_AZIM_TX 2U
#define MAX_NUM_ELEV_TX 1U
#define MAX_NUM_RX 4U
#endif
#define MAX_NUM_AZIM_VIRT_ANT (MAX_NUM_RX * MAX_NUM_AZIM_TX)
#define MAX_NUM_ELEV_VIRT_ANT (MAX_NUM_RX * MAX_NUM_ELEV_TX)
#define MAX_NUM_VIRT_ANT      (MAX_NUM_ELEV_VIRT_ANT + MAX_NUM_AZIM_VIRT_ANT)

/**
@}
*/

#ifndef INCLUDE_DPM
/**
 * @brief
 *  This is the common definition for the maximum number of buffers which
 *  can be exchanged by the application with the DPC
 */
#define DPM_MAX_BUFFER                              3
/**
 * @brief
 *  DPM Processing Chain Handle
 */
typedef void*   DPM_DPCHandle;

/**
 * @brief
 *  DPM Buffer Interface
 *
 * @details
 *  The DPM processing chain receive data buffers which are to be
 *  processed. The DPC processes these buffers and generates
 *  the results which are then passed to the application. The structure
 *  encompasses this information.
 */
typedef struct DPM_Buffer_t
{
    /**
     * @brief   Pointer to the buffer
     */
    uint8_t*            ptrBuffer[DPM_MAX_BUFFER];

    /**
     * @brief   Size of the result buffer
     */
    uint32_t            size[DPM_MAX_BUFFER];
}DPM_Buffer;
#endif



/** @addtogroup DPC_OBJDET_IOCTL__DATA_STRUCTURES
 @{ */

/**
 * @brief
 *  CFAR Configuration
 *
 * @details
 *  The structure contains the cfar configuration used in data path
 */
typedef struct DPC_ObjectDetection_CfarCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
    uint8_t subFrameNum;

    /*! @brief   CFAR Configuration */
    DPU_DopplerProc_CfarCfg cfg;

}DPC_ObjectDetection_CfarCfg;


/**
 * @brief RX channel gain/phase measurement configuration.
 *
 */
typedef struct DPC_ObjectDetection_MeasureRxChannelBiasCfg_t
{
    /*! @brief  1-enabled 0-disabled */
    uint8_t enabled;

    /*! @brief  Target distance during measurement (in meters) */
    float targetDistance;

    /*! @brief  Search window size (in meters), the search is done in range
     *          [-searchWinSize/2 + targetDistance, targetDistance + searchWinSize/2] */
    float searchWinSize;
} DPC_ObjectDetection_MeasureRxChannelBiasCfg;


/*!
*  @brief      Call back function type for calling back during process
*  @param[out] subFrameIndx Sub-frame indx [0..(numSubFrames-1)]
*/
typedef void (*DPC_ObjectDetection_processCallBackFxn_t)(uint8_t subFrameIndx);

/*! @brief  Process call backs configuration */
typedef struct DPC_ObjectDetection_ProcessCallBackCfg_t
{
    /*! @brief  Call back function that will be called at the beginning of frame
     *          processing (beginning of 1D) */
    DPC_ObjectDetection_processCallBackFxn_t processFrameBeginCallBackFxn;

    /*! @brief  Call back function that will be called at the beginning of inter-frame
     *          processing (beginning of 2D) */
    DPC_ObjectDetection_processCallBackFxn_t processInterFrameBeginCallBackFxn;
} DPC_ObjectDetection_ProcessCallBackCfg;

/*
 * @brief Memory Configuration used during init API
 */
typedef struct DPC_ObjectDetection_MemCfg_t
{
    /*! @brief   Start address of memory provided by the application
     *           from which DPC will allocate.
     */
    void *addr;

    /*! @brief   Size limit of memory allowed to be consumed by the DPC */
    uint32_t size;
} DPC_ObjectDetection_MemCfg;

/*
 * @brief Bi-directional Memory Configuration used during init API
 */
typedef struct DPC_ObjectDetection_L3MemCfg_t
{
    /*! @brief   Start address of memory provided by the application
     *           from which DPC will allocate.
     */
    void *addr;

    /*! @brief   Size limit of memory allowed to be consumed by the DPC */
    uint32_t size;

    /*! @brief   Size limit of memory allowed to be consumed by the DPC from bottom of buffer */
    uint32_t endSize;
} DPC_ObjectDetection_BiDirMemCfg;

/*
 * @brief Configuration for DPM's init API.
 *        DPM_init's arg = pointer to this structure.
 *        DPM_init's argLen = size of this structure.
 *
 */
typedef struct DPC_ObjectDetection_InitParams_t
{
   /*! @brief   Handle to the hardware accelerator */
   HWA_Handle hwaHandle;

   /*! @brief   Handles of the EDMA driver. */
   EDMA_Handle edmaHandle[EDMA_NUM_CC];

   /*! @brief L3 RAM configuration. DPC will allocate memory from this
    *         as needed and report the amount of memory consumed through
    *         @ref DPC_ObjectDetection_PreStartCfg to application */
   DPC_ObjectDetection_BiDirMemCfg L3ramCfg;

   /*! @brief Core Local RAM configuration (e.g L2 for R5F).
    *         DPC will allocate memory from this as needed and report the amount
    *         of memory consumed through @ref DPC_ObjectDetection_PreStartCfg
    *         to the application */
   DPC_ObjectDetection_MemCfg CoreLocalRamCfg;

   /*! @brief   Process call back function configuration */
   DPC_ObjectDetection_ProcessCallBackCfg processCallBackCfg;
} DPC_ObjectDetection_InitParams;

/**
 * @brief
 *  Field of view configuration
 *
 * @details
 *  The structure contains the field of view
 *
 */
typedef struct DPC_ObjectDetection_FovAoaCfg_t
{
    /*! @brief    minimum azimuth angle (in degrees) exported to Host*/
    float        minAzimuthDeg;

    /*! @brief    maximum azimuth angle (in degrees) exported to Host*/
    float        maxAzimuthDeg;

    /*! @brief    minimum elevation angle (in degrees) exported to Host*/
    float        minElevationDeg;

    /*! @brief    maximum elevation angle (in degrees) exported to Host*/
    float        maxElevationDeg;
} DPC_ObjectDetection_FovAoaCfg;

/*!
 * @brief Configurations required to reconfigure
 *        ADC buffer (RX channel offset) during subFrame switching
 */
typedef struct DPC_ObjectDetection_ADCBufConfig_t{

    /*!
     * @ brief  RX Channel Bitmap \n
                b0 RX0 Channel Enable \n
                0   Disable RX Channel 0 \n
                1   Enable RX Channel 0 \n
                b1 RX1 Channel Enable \n
                0   Disable RX Channel 1 \n
                1   Enable RX Channel 1 \n
                b2 RX2 Channel Enable \n
                0   Disable RX Channel 2 \n
                1   Enable RX Channel 2 \n
                b3 RX3 Channel Enable \n
                0   Disable RX Channel 3 \n
                1   Enable RX Channel 3 \n
                b15:4 - RESERVED \n
    */
    uint16_t    rxChannelEn;

    /*! @brief  Number of bytes per RX channel */
    uint32_t    adcBufChanDataSize;

}DPC_ObjectDetection_ADCBufConfig;

#ifdef SOC_AWR2X44P
/*
 * @brief Configuration to enable/disable power optimixation hooks
 */
typedef struct DPC_ObjectDetection_PowerOptimize_t
{
    /*! @brief    Flag to enable HWA Dynamic Clock Gating. */
    uint32_t      isHwaDynamicClkGate;

    /*! @brief    Flag to enable HWA Clock gatting after Frame Proc. */
    uint32_t      hwaStateAfterFrameProc;

    /*! @brief    Flag to enable DSP Clock gating or Power Gating after Frame Proc. */
    uint32_t      dspStateAfterFrameProc;

}DPC_ObjectDetection_PowerOptimize;
#endif

/*
 * @brief Static Configuration that is part of the pre-start configuration.
 */
typedef struct DPC_ObjectDetection_StaticCfg_t
{
    /*! @brief      ADCBuf buffer interface */
    DPIF_ADCBufData  ADCBufData;

    /*! @brief      ADCBuf buffer configuration */
    DPC_ObjectDetection_ADCBufConfig ADCBufConfig;

    /*! @brief  Rx Antenna order */
    uint8_t     rxAntOrder[SYS_COMMON_NUM_RX_CHANNEL];

    /*! @brief  Tx Antenna order */
    uint8_t     txAntOrder[SYS_COMMON_NUM_TX_ANTENNAS];

    /*! @brief  Number of transmit antennas */
    uint8_t     numTxAntennas;

    /*! @brief  Number of virtual antennas */
    uint8_t     numVirtualAntennas;

    /*! @brief  Number of virtual azimuth antennas */
    uint8_t     numVirtualAntAzim;

    /*! @brief  Number of virtual elevation antennas */
    uint8_t     numVirtualAntElev;

    /*! @brief  Number of range FFT bins, this is at a minimum the next power of 2 of
               @ref DPIF_ADCBufProperty_t::numAdcSamples, in case of complex ADC data,
               and half that value in case of real only data.
                If range zoom is supported, this can be bigger than the minimum. */
    uint16_t    numRangeBins;

    /*! @brief  Number of bins used in Range FFT Calculation. In case of real only samples,
                this is twice the number of range bins. Else it is equal to the number
                of range bins. */
    uint16_t    numRangeFFTBins;

    /*! @brief  Number of chirps per frame */
    uint16_t    numChirpsPerFrame;

    /*! @brief Number of chirps for Doppler computation purposes. */
    uint16_t    numChirps;

    /*! @brief  Number of Doppler FFT bins, this is at a minimum the next power of 2 of
               @ref DPU_DopplerProcHWA_Obj.numDopplerChirps "numDopplerChirps". If Doppler zoom is supported, this can be bigger
               than the minimum. */
    uint16_t    numDopplerBins;

    /*! @brief  Range conversion factor for FFT range index to meters */
    float       rangeStep;

    /*! @brief  Doppler conversion factor for Doppler FFT index to m/s */
    float       dopplerStep;

    /*! @brief   1 if valid profile has one Tx per chirp else 0: 0 for DDMA cases */
    uint8_t      isValidProfileHasOneTxPerChirp;

    /*! @brief   Sum Tx must be enabled if range profile is to be sent out or
                 range CFAR is enabled */
    uint8_t      isSumTxEnabled;

    /*! @brief   Total Number of subbands used in DDMA demodulation */
    uint8_t      numBandsTotal;

    /*! @brief     Range FFT Tuning Params */
    DPU_RangeProcHWA_FFTtuning    rangeFFTtuning;

    /*! @brief     Data Input Mode, */
    DPU_RangeProcHWA_InputMode      dataInputMode;

    /*! @brief     Doppler CFAR Cfg */
    DPC_ObjectDetection_CfarCfg     cfarCfg;

    /*! @brief     Range CFAR Cfg */
    DPC_ObjectDetection_CfarCfg     rangeCfarCfg;

    /*! @brief     Compression Cfg */
    DPU_RangeProcHWA_CompressionCfg compressionCfg;

    /*! @brief Shift/Scale config for Interf Stats Mag Diff in range DPU */
    DPU_RangeProcHWADDMA_intfStatsdBCfg  intfStatsdBCfg;

    /*! @brief      LocalMax thresholds in doppler and azimuth dimensions */
    DPU_DopplerProc_LocalMaxCfg localMaxCfg;

    /*! @brief      Field of view config used in AoA estimation */
    DPC_ObjectDetection_FovAoaCfg   aoaFovCfg;

#ifdef SOC_AWR2X44P
    /*! @brief      Power Optimization hooks configuration */
    DPC_ObjectDetection_PowerOptimize powerOptCfg;
#endif

} DPC_ObjectDetection_StaticCfg;


/*
 * @brief Antenna Spacing
 */
typedef struct DPC_ObjectDetection_AntennaSpacing_t
{
    /* Spacing in X dimension */
    float xSpacingByLambda;

    /* Spacing in Z dimension */
    float zSpacingByLambda;
}DPC_ObjectDetection_AntennaSpacing;

/*
 * @brief Zero Insertion Mask configuration
 */
typedef struct DPC_ObjectDetection_ZeroInsrtMaskCfg_t
{
    /* Spacing in X dimension */
    uint64_t zeroInsrtMaskAzim;

    /* Spacing in Z dimension */
    uint64_t zeroInsrtMaskElev;
}DPC_ObjectDetection_ZeroInsrtMaskCfg;

/**
 * @brief Configuration related to IOCTL API for command
 *        @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG. This is independent
 *        of sub frame.
 */
typedef struct DPC_ObjectDetection_PreStartCommonCfg_t
{
    /*! @brief   Number of sub-frames */
    uint8_t numSubFrames;

    /*! @brief   rx channel gain/phase measurement configuration */
    DPC_ObjectDetection_MeasureRxChannelBiasCfg measureRxChannelBiasCfg;

    /*! @brief   Antenna Calbration parameters in Im/Re format */
    float antennaCalibParams[SYS_COMMON_NUM_RX_CHANNEL * SYS_COMMON_NUM_TX_ANTENNAS * 2U] __attribute__((aligned(8)));

    /*! @brief   Antenna Geometry Re-arrangement order */
    uint16_t antennaGeometryCfg[SYS_COMMON_NUM_RX_CHANNEL * SYS_COMMON_NUM_TX_ANTENNAS];

    /*! @brief Antenna Spacing in X and Z dim */
    DPC_ObjectDetection_AntennaSpacing antennaSpacing;

    /*! @brief Zero Insertion Mask configuration: to insert zeros in virtual antenna array */
    DPC_ObjectDetection_ZeroInsrtMaskCfg zeroInsrtMaskCfg;

    /*! @brief The structure is used to hold the config needed for Range Proc HWA Chain */
    DPU_RangeProcHWADDMA_rangeProcCfg rangeProcCfg;

    /*! @brief Maximum number of ADC Samples across all subframes */
    uint16_t maxAdcSamples;

} DPC_ObjectDetection_PreStartCommonCfg;

/*
 * @brief  Structure related to @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG
 *        IOCTL command. When the pre-start IOCTL is processed, it will report
 *        memory usage as part of @DPC_ObjectDetection_PreStartCfg.
 */
typedef struct DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage_t
{
    /*! @brief   Indicates number of bytes of L3 memory allocated to be used by DPC */
    uint32_t L3RamTotal;

    /*! @brief   Indicates number of bytes of L3 memory used by DPC from the allocated
     *           amount indicated through @ref DPC_ObjectDetection_InitParams */
    uint32_t L3RamUsage;

    /*! @brief   Indicates number of bytes of Core Local memory allocated to be used by DPC */
    uint32_t CoreLocalRamTotal;

    /*! @brief   Indicates number of bytes of Core Local memory used by DPC from the allocated
     *           amount indicated through @ref DPC_ObjectDetection_InitParams */
    uint32_t CoreLocalRamUsage;

    /*! @brief   Indicates number of bytes of system heap allocated */
    uint32_t SystemHeapTotal;

    /*! @brief   Indicates number of bytes of system heap used at the end of PreStartCfg */
    uint32_t SystemHeapUsed;

    /*! @brief   Indicates number of bytes of system heap used by DCP at the end of PreStartCfg */
    uint32_t SystemHeapDPCUsed;
} DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage;

/*
 * @brief Stats structure to convey to Application timing and related information.
 */
typedef struct DPC_ObjectDetection_Stats_t
{
    /*! @brief   interChirpProcess margin in CPU cyctes */
    uint32_t      interChirpProcessingMargin;

    /*! @brief   Counter which tracks the number of frame start interrupt */
    uint32_t      frameStartIntCounter;

    /*! @brief   Counter which tracks the number of subframe start interrupt */
    uint32_t      subframeStartIntCounter;

    /*! @brief   Frame start CPU time stamp */
    uint32_t      frameStartTimeStamp;

    /*! @brief   Inter-frame start CPU time stamp */
    uint32_t      interFrameStartTimeStamp;

    /*! @brief   Inter-frame end CPU time stamp */
    uint32_t      interFrameEndTimeStamp;

    /*! @brief Sub frame preparation cycles. Note when this is reported as part of
     *         the process result reporting, then it indicates the cycles that took
     *         place in the previous sub-frame/frame for preparing to switch to
     *         the sub-frame that is being reported because switching happens
     *         in the processing of DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED,
     *         which is after the DPC process. */
    uint32_t      subFramePreparationCycles;
} DPC_ObjectDetection_Stats;


/**
 * @brief rx channel gain/phase compensation configuration.
 */
typedef struct Measure_compRxChannelBiasCfg_t
{
    /* Range of the detected peak (corner reflector) */
    float targetRange;

    /* Peak Value */
    uint16_t peakVal;

    /*! @brief  Compensation for Rx channel phase bias in Q15 format.
     *          The order here is like x[rx][tx] where rx order is 0,1,....SYS_COMMON_NUM_RX_CHANNEL-1
     *          and tx order is MAX_NUM_AZIM_TX followed by MAX_NUM_ELEV_TX
     */
    cmplx16ImRe_t rxChPhaseComp[SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL];

} Measure_compRxChannelBiasCfg;


/*
 * @brief This is the result structure reported from DPC's registered processing function
 *        to the application through the DPM_Buffer structure. The DPM_Buffer's
 *        first fields will be populated as follows:
 *        pointer[0] = pointer to this structure.
 *        size[0] = size of this structure i.e sizeof(DPC_ObjectDetection_Result)
 *
 *        pointer[1..3] = NULL and size[1..3] = 0.
 */
typedef struct DPC_ObjectDetection_ExecuteResult_t
{
    /*! @brief      Sub-frame index, this is in the range [0..numSubFrames - 1] */
    uint8_t         subFrameIdx;

    /*! @brief      doppler Number of detected objects */
    uint32_t        dopNumObjOut;

    /*! @brief      Final Number of detected objects */
    uint32_t         numObjOut;

    /*! @brief      Detected objects output list of @ref numObjOut elements */
    DPIF_PointCloudCartesian    *objOut;

    /*! @brief      Detection Matrix structure */
    DPIF_DetMatrix  detMatrix;

    /*! @brief      Detected objects side information (snr + noise) output list,
     *              of @ref numObjOut elements  */
    DPIF_PointCloudSideInfo *objOutSideInfo;

    DetObjParams *detObjList;

    /*! @brief      Pointer to DPC stats structure */
    DPC_ObjectDetection_Stats *stats;

    /*! @brief   Pointer to rx channel gain/phase compensation measurement
     *           result. Note it is expected that when measurement is enabled,
     *           the number of sub-frames will be 1 (i.e advanced frame
     *           feature will be disabled). If measurement
     *           was not enabled, then this pointer will be NULL. */
    Measure_compRxChannelBiasCfg *compRxChanBiasMeasurement;

    /*! @brief These variables store the FFT clip count for range fft and
     *         doppler/azimuth fft. It is incremented everytime HWA FFT clip
     *         status register is non-zero to indicate that clipping has happened.
     *         Final value is printed on the console when sensorStop
     *         happens. */
    uint32_t    FFTClipCount[2];

} DPC_ObjectDetection_ExecuteResult;

/*
 * @brief This is the informational structure related to the IOCTL command
 *        @ref DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED.
 */
typedef struct DPC_ObjectDetection_ExecuteResultExportedInfo_t
{
    /*! @brief      Sub-frame index, this is in the range [0..numSubFrames - 1].
     *              This is the sub-frame whose results have been exported.
     *              Although this DPC implementation knows what sub-frame to expect as the exports
     *              are expected to be sequential in sub-frames, this field helps
     *              in error checking when for example the application could miss
     *              exporting/consuming a sub-frame in a timely manner or have out of order
     *              export/consumption. */
    uint8_t         subFrameIdx;
} DPC_ObjectDetection_ExecuteResultExportedInfo;

#if defined(SOC_AWR2X44P)

/**
 * @brief Structure related to @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG
 *        IOCTL command. When the pre-start IOCTL is processed, it will populate
 *        elevation estimation cfg.
 */
typedef struct DPC_ObjectDetection_ElevEstSubframeCfg_t
{
    /*! @brief  Number of bins for Azimuth (3D) FFT */
    uint16_t numAzimFFTBins;

    /*! @brief  Number of bins for Doppler (2D) FFT */
    uint16_t numDopplerBins;

    /*! @brief  Range conversion factor for FFT range index to meters */
    float       rangeStep;

    /*! @brief  Doppler conversion factor for Doppler FFT index to m/s */
    float       dopplerStep;

    /*! @brief      Field of view (in degree) used in AoA estimation */
    DPC_ObjectDetection_FovAoaCfg   aoaFovSinVal;
} DPC_ObjectDetection_ElevEstSubframeCfg;

/**
 * @brief Structure related to @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG
 *        IOCTL command. When the pre-start common IOCTL is processed, it will populate
 *        elevation estimation cfg.
 */
typedef struct DPC_ObjectDetection_ElevEstCommonCfg_t
{
    /*! @brief   Number of sub-frames */
    uint8_t numSubFrames;

    /*! @brief   Antenna Calbration parameters in Im/Re format */
    float antennaCalibParams[SYS_COMMON_NUM_RX_CHANNEL * SYS_COMMON_NUM_TX_ANTENNAS * 2U] __attribute__((aligned(8)));

    /*! @brief   Antenna Geometry Re-arrangement order */
    uint16_t antennaGeometryCfg[SYS_COMMON_NUM_RX_CHANNEL * SYS_COMMON_NUM_TX_ANTENNAS];

    /*! @brief Antenna Spacing in X and Z dim */
    DPC_ObjectDetection_AntennaSpacing antennaSpacing;

    /*! @brief Zero Insertion Mask configuration: to insert zeros in virtual antenna array */
    DPC_ObjectDetection_ZeroInsrtMaskCfg zeroInsrtMaskCfg;

    DPC_ObjectDetection_ExecuteResult *result;

#ifdef SOC_AWR2X44P
    /*! @brief Power Optimization config for DSP core */
    uint32_t dspStateAfterFrameProc;
#endif
} DPC_ObjectDetection_ElevEstCommonCfg;

/**
 * @brief Structure to store elevation estimation parameters on MSS/DSS
 */
typedef struct DPC_ObjectDetection_ElevEstCfg_t
{
    /*! @brief Subframe specific configuration for Elevation Estimation */
    DPC_ObjectDetection_ElevEstSubframeCfg subframeCfg[RL_MAX_SUBFRAMES];

    /*! @brief Subframe specific configuration for Elevation Estimation */
    DPC_ObjectDetection_ElevEstCommonCfg commonCfg;
    
} DPC_ObjectDetection_ElevEstCfg;
#endif

/*
 * @brief Configuration related to IOCTL API for command
 *        @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG.
 *
 */
typedef struct DPC_ObjectDetection_PreStartCfg_t
{
    /*! @brief   Subframe number for which this message is applicable. When
     *           advanced frame is not used, this should be set to
     *           0 (the 1st and only sub-frame) */
    uint8_t subFrameNum;

    /*! Static configuration */
    DPC_ObjectDetection_StaticCfg staticCfg;

    /*! Memory usage after the preStartCfg is applied */
    DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage memUsage;

} DPC_ObjectDetection_PreStartCfg;

/**
@}
*/

/** @addtogroup DPC_OBJDET_IOCTL__COMMAND
 @{ */

#ifndef INCLUDE_DPM
#define DPM_CMD_DPC_START_INDEX 100U
#endif

/**
 * @brief Command associated with @ref DPC_ObjectDetection_PreStartCfg_t. In this IOCTL, the sub-frame's
 *        configurations will be processed by configuring individual DPUs.
 *        The @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG must be issued
 *        before issuing this IOCTL.
 */
#define DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG                            (uint32_t)(DPM_CMD_DPC_START_INDEX + 0U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_PreStartCommonCfg_t. Must be issued before
 *        issuing @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG
 */
#define DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG                     (uint32_t)(DPM_CMD_DPC_START_INDEX + 1U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_CfarCfg_t for range dimension.
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_CFAR_RANGE_CFG                          (uint32_t)(DPM_CMD_DPC_START_INDEX + 2U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_CfarCfg_t for doppler dimension.
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_CFAR_DOPPLER_CFG                        (uint32_t)(DPM_CMD_DPC_START_INDEX + 3U)

/**
 * @brief Command associated with @ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_MEASURE_RANGE_BIAS_AND_RX_CHAN_PHASE   (uint32_t)(DPM_CMD_DPC_START_INDEX + 7U)

/**
 * @brief This commands indicates to the DPC that the results DPC provided to the application
 *        through its execute API (which application will access through DPM_execute API)
 *        have been exported/consumed. The purpose of this command is for DPC to
 *        reclaim the memory resources associated with the results. The DPC may
 *        also perform sub-frame switching, and do error-checking to see
 *        if export was later than expected e.g the DPC design may be such that
 *        the previous frame/sub-frame's export notification may need to come
 *        after a new frame/sub-frame (this is the case currently with this
 *        object detection DPC). The DPC will also consider this command as a
 *        signal from the application that all its processing for the current frame/sub-frame
 *        has been done and so if a new frame/sub-frame interrupt (DPC has registered
 *        a frame interrupt with the DPM) comes before the last step in the
 *        processing of this command (which could be sub-frame switching and
 *        preparing for next sub-frame/frame), then the DPC will signal an assert
 *        to the application from its frame interrupt. The expected sequence is
 *        the following:
 *
 *        1. App consumes the process result of the DPC (e.g sending output on UART).
 *        2. App performs any dynamic configuration command processing by issuing DPC's
 *           IOCTL APIs for the next frame/sub-frame.
 *        3. App issues this result-exported IOCTL.
 *        4. DPC does its processing related to this IOCTL in the following sequence:
 *            a. May do error checking and preparing for next sub-frame/frame.
 *            b. Do book-keeping related to marking this as end of sub-frame/frame processing
 *               by the app. The DPC's registered frame start interrupt performs
 *               check on this information to see if next frame/sub-frame came before
 *               this end of processing in which case it will issue an assert to app.
 *
 *        An informational structure @ref DPC_ObjectDetection_ExecuteResultExportedInfo_t
 *        is associated with this command.
 */
#define DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED                     (uint32_t)(DPM_CMD_DPC_START_INDEX + 14U)

/**
 * @brief This command is for non real-time (without RF) testing. When issued, it will simulate
 *        the trigger of frame start. No configuration structure is associated with this command.
 *        Must be issued between start and stop of DPC.
 */
#define DPC_OBJDET_IOCTL__TRIGGER_FRAME                                  (uint32_t) (DPM_CMD_DPC_START_INDEX + 15U)

/* Command associated with @ref DPC_ObjectDetection_ElevEstCfg. In this IOCTL, the sub-frame's
 *        configurations will be processed by configuring elevation estimation.
 *        The @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG must be issued
 *        before issuing this IOCTL.
 */
#define DPC_OBJDET_IOCTL__STATIC_PRE_START_ELEVEST_CFG                        (uint32_t)(DPM_CMD_DPC_START_INDEX + 16U)

/**
@}
*/

/** @addtogroup DPC_OBJECTDETECTION_ERROR_CODE
 *  Base error code for the objdethwa DPC is defined in the
 *  \include ti/datapath/dpif/dp_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument general (such as NULL argument pointer)
 */
#define DPC_OBJECTDETECTION_EINVAL                  (DP_ERRNO_OBJECTDETECTION_BASE-1)

/**
 * @brief   Error Code: Invalid argSize in DPM_InitCfg provided to @ref DPC_ObjectDetection_init,
 *          does not match the expected size of @ref DPC_ObjectDetection_InitParams_t
 */
#define DPC_OBJECTDETECTION_EINVAL__INIT_CFG_ARGSIZE   (DP_ERRNO_OBJECTDETECTION_BASE-2)

/**
 * @brief   Error Code: Invalid argument bad command argument in DPM_ProcChainIoctlFxn for
 *                      Object detection DPC.
 */
#define DPC_OBJECTDETECTION_EINVAL__COMMAND         (DP_ERRNO_OBJECTDETECTION_BASE-4)

/**
 * @brief   Error Code: Out of general heap memory
 */
#define DPC_OBJECTDETECTION_ENOMEM                  (DP_ERRNO_OBJECTDETECTION_BASE-10)

/**
 * @brief   Error Code: Out of L3 RAM during radar cube allocation.
 */
#define DPC_OBJECTDETECTION_ENOMEM__L3_RAM_RADAR_CUBE            (DP_ERRNO_OBJECTDETECTION_BASE-11)

/**
 * @brief   Error Code: Out of L3 RAM during detection matrix allocation.
 */
#define DPC_OBJECTDETECTION_ENOMEM__L3_RAM_DET_MATRIX            (DP_ERRNO_OBJECTDETECTION_BASE-12)

/**
 * @brief   Error Code: Out of HWA Window RAM
 */
#define DPC_OBJECTDETECTION_ENOMEM_HWA_WINDOW_RAM            (DP_ERRNO_OBJECTDETECTION_BASE-13)

/**
 * @brief   Error Code: Out of Core Local RAM for generating window coefficients
 *          for HWA when doing range DPU Config.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_RANGE_HWA_WINDOW    (DP_ERRNO_OBJECTDETECTION_BASE-14)

/**
 * @brief   Error Code: Out of Core Local RAM for generating window coefficients
 *          for HWA when doing doppler DPU Config.
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_HWA_WINDOW    (DP_ERRNO_OBJECTDETECTION_BASE-15)

/**
 * @brief   Error Code: When allocating decompression buffer for doppler DPU config
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_DECOMP_BUF     (DP_ERRNO_OBJECTDETECTION_BASE-16)

/**
 * @brief   Error Code: When allocating doppler FFT submat buffer for doppler DPU config
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_DOPFFT_SUBMAT     (DP_ERRNO_OBJECTDETECTION_BASE-17)

/**
 * @brief   Error Code: When allocating max subband buffer for doppler DPU config
 */
#define DPC_OBJECTDETECTION_ENOMEM__RAM_DOPPLER_MAXDOP_SUBBAND    (DP_ERRNO_OBJECTDETECTION_BASE-18)

/**
 * @brief   Error Code: When allocatid
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_DOPFFT_SCRATCH    (DP_ERRNO_OBJECTDETECTION_BASE-20)

/**
 * @brief   Error Code: When allocating Azim FFT scratch buffer for doppler DPU config
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_AZIMFFT_SCRATCH    (DP_ERRNO_OBJECTDETECTION_BASE-21)

/**
 * @brief   Error Code: When allocating CFAR scratch buffer for doppler DPU config
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_CFAR_SCRATCH     (DP_ERRNO_OBJECTDETECTION_BASE-22)

/**
 * @brief   Error Code: When allocating local max scratch buffer for doppler DPU config
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_DOPPLER_LOCALMAX_SCRATCH    (DP_ERRNO_OBJECTDETECTION_BASE-23)

/**
 * @brief   Error Code: Pre-start config was received before pre-start common config.
 */
#define DPC_OBJECTDETECTION_PRE_START_CONFIG_BEFORE_PRE_START_COMMON_CONFIG  (DP_ERRNO_OBJECTDETECTION_BASE-30)

/**
 * @brief   Error Code: When allocating obj list for doppler DPU config
 */
#define DPC_OBJECTDETECTION_ENOMEM__OBJ_PARAMS_RAM_DOPPLER_DECOMP_BUF       (DP_ERRNO_OBJECTDETECTION_BASE-31)

/**
 * @brief   Error Code: When allocating range CFAR L3 Buffer
 */
#define DPC_OBJECTDETECTION_ENOMEM__OBJ_PARAMS_RAM_RANGE_CFAR_BUF       (DP_ERRNO_OBJECTDETECTION_BASE-32)

/**
 * @brief   Error Code: When allocating range CFAR L2 Scratch Buffer
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_RANGECFAR_SCRATCH_BUF       (DP_ERRNO_OBJECTDETECTION_BASE-33)

/**
 * @brief   Error Code: When allocating range CFAR L2 Buffer
 */
#define DPC_OBJECTDETECTION_ENOMEM__CORE_LOCAL_RAM_RANGECFAR_NUMOBJ_PER_DOPPLER_BUF     (DP_ERRNO_OBJECTDETECTION_BASE-34)

/**
 * @brief   Error Code: In isObjInRangeAndDopplerList
 */
#define DPC_OBJECTDETECTION_RANGE_DOPPLER_NO_MATCH     (DP_ERRNO_OBJECTDETECTION_BASE-35)

/**
 * @brief   Error Code: In num range bins
 */
#define DPC_OBJECTDETECTION_RANGE_BINS_ERR     (DP_ERRNO_OBJECTDETECTION_BASE-36)

/**
 * @brief   Error Code: In window type
 */
#define DPC_OBJECTDETECTION_WIN_ERR     (DP_ERRNO_OBJECTDETECTION_BASE-37)

/**
 * @brief   Error Code: Too many objects
 */
#define DPC_OBJECTDETECTION_NUMOBJ_EXCEED_MAX_ERR     (DP_ERRNO_OBJECTDETECTION_BASE-38)

/**
 * @brief   Error Code: When allocating side info
 */
#define DPC_OBJECTDETECTION_ENOMEM__OBJ_PARAMS_SIDEINFO       (DP_ERRNO_OBJECTDETECTION_BASE-39)

/**
 * @brief   Error Code: Internal error
 */
#define DPC_OBJECTDETECTION_EINTERNAL               (DP_ERRNO_OBJECTDETECTION_BASE-40)

/**
 * @brief  Error Code: When allocating pre proc buffers
*/
#define DPC_OBJECTDETECTION_PREPROCBUF_ERR     (DP_ERRNO_OBJECTDETECTION_BASE-41)

/**
 * @brief  Error Code: Azim FFT Size should be <= 64 if histogram is enabled.
*/
#define DPC_OBJECTDETECTION_HIST_AZIMFFT_SIZE    (DP_ERRNO_OBJECTDETECTION_BASE-42)

/**
 * @brief   Error Code: Not implemented
 */
#define DPC_OBJECTDETECTION_ENOTIMPL                (DP_ERRNO_OBJECTDETECTION_BASE-50)

/**
@}
*/

#ifdef INCLUDE_DPM
/** @addtogroup DPC_OBJDET__GLOBAL
 @{ */

/*! Application developers: Use this configuration to load the Object Detection DPC
 *  within the DPM. */
extern DPM_ProcChainCfg  gDPC_ObjectDetectionCfg;

/**
@}
*/
#else
DPM_DPCHandle DPC_ObjectDetection_init(
    DPC_ObjectDetection_InitParams *dpcInitParams,
    int32_t *errCode) 
#ifdef SUBSYS_M4       
    __attribute__((section (".customCode")))
#endif    
;

int32_t DPC_ObjectDetection_start(DPM_DPCHandle handle) 
#ifdef SUBSYS_M4  
__attribute__((section (".customCode")))
#endif
;

int32_t DPC_ObjectDetection_execute(
    DPM_DPCHandle handle,
    DPM_Buffer *ptrResult);

int32_t DPC_ObjectDetection_ioctl(
    DPM_DPCHandle handle,
    uint32_t cmd,
    void *arg,
    uint32_t argLen) 
#ifdef SUBSYS_M4  
    __attribute__((section (".customCode")))
#endif
;

int32_t DPC_ObjectDetection_stop(DPM_DPCHandle handle);

int32_t DPC_ObjectDetection_deinit(DPM_DPCHandle handle);

void DPC_ObjectDetection_frameStart(DPM_DPCHandle handle);

#endif

#if (defined(SOC_AWR2X44P) && ((defined(SUBSYS_MSS) && MSS_AOA_ENABLED) || (defined(SUBSYS_DSS) && !MSS_AOA_ENABLED)))
int32_t DPC_ObjDet_estimateXYZ(DPC_ObjectDetection_ElevEstCfg * restrict elevEstCfg,
                               const DetObjParams * restrict detObjList,
                               DPIF_PointCloudCartesian * restrict objOut,
                               uint32_t numObjOut,
                               uint32_t * restrict finalNumObjOut);
#endif

#ifdef __cplusplus
}
#endif


#endif /* objectdetection.h */
