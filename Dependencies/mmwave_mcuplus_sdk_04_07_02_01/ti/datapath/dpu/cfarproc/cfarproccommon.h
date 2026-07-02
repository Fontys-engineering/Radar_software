/**
 *   @file  cfarproccommon.h
 *
 *   @brief
 *      Implements Common definition across cfarProc DPU.
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

/** @mainpage CFAR DPU
 * [TOC]
 *  @section cfar_intro_section CFAR DPU
 *
 * This DPU implements CFAR object detection from the Range/Doppler detection
 * matrix.
 *
 *
 *  CFAR DPU includes HWA implementations:
 *
 *   DPU         |  runs on cores
 *  :------------|:----------------
 *  cfarProcHWA|  R5F or DSP
 *
 *  @section cfar_hwa HWA-CFAR DPU
 *
 * @subsection cfarhwa_intro_section Introduction
 * This DPU implements CFAR object detection from the Range/Doppler detection
 * matrix using hardware accelerator (HWA).
 * CFAR detection is performed in both Range and Doppler domains. The DPU exports
 * the list of objects that are detected both in range and in Doppler domains.
 * A high level data flow is illustrated in figure below.
 *
 * @image html hwa_cfar_top_level.png "HWA-CFAR high level data flow"
 *
 * The following figure depicts the DPU implementation:
 *
 * @image html hwa_cfar.png "HWA-CFAR implementation"
 *
 * The input is detection matrix
 * as per the format @ref DPIF_DetMatrix @ref DPIF_DETMATRIX_FORMAT_1, the elements 
 * of this matrix are sum  of log2 magnitudes across received antennas of the 2D FFT.
 * This matrix must be no bigger than what can fit in M0+M1 memory (error checking
 * is done during @ref DPU_CFARProcHWA_config and violation will generate error code
 * @ref DPU_CFARPROCHWA_ENOMEM__DET_MATRIX_EXCEEDS_HWA_INP_MEM).
 *
 * The detection process is initiated by triggering input EDMA (@ref DPU_CFARProcHWA_HW_Resources::edmaHwaIn)
 * that copies detection matrix(@ref DPIF_DetMatrix)
 * from L3 memory to internal HWA memory (@ref DPU_CFARProcHWA_HW_Resources::hwaMemInp, typically
 * (M0+M1)). Signature EDMA (@ref DPU_CFARProcHWA_HW_Resources::edmaHwaInSignature)
 * then triggers HWA CFAR which is executed
 * with one param set that performs CFAR in Doppler domain.
 * The results are stored in HWA memory
 * (@ref DPU_CFARProcHWA_HW_Resources::hwaMemOutDoppler, typically M2) as a list with each element represented
 * as @ref DPU_CFARProcHWA_CfarDetOutput_t.
 * Upon completion, the CPU triggers the HWA CFAR in range domain with the results
 * directed to HWA memory (@ref DPU_CFARProcHWA_HW_Resources::hwaMemOutRange, typically in M3)
 * and then in parallel calculates
 * (@ref CFARHWA_convHwaCfarDetListToDetMask) the bit mask of detected points
 * and puts it in to the @ref DPU_CFARProcHWA_HW_Resources::cfarDopplerDetOutBitMask
 * that is allocated by the application (typically in core local memory).
 * The bit position in the array corresponding to element x[rangeIdx][dopplerIdx]
 * is calculated as shown in the figure above (expressions "word = , bit = ").
 *
 * After the HWA CFAR detection is completed, the CPU (@ref CFARHWA_cfarRange_AND_cfarDoppler)
 * takes detected objects from the range CFAR detection list (M3 memory) and
 * populates in the output list (@ref DPIF_CFARDetList_t)
 * if the following conditions are satisfied:
 * 1. The object is also detected in the Doppler domain (i.e. if the corresponding
 *    bit is set in the Doppler detection bit mask).
 * 2. The range and doppler indices are within those determined from the
 *    FOV configuration given by the application when issuing any of the following:
 *    - @ref DPU_CFARProcHWA_config (@ref DPU_CFARProcHWA_DynamicConfig::fovRange, @ref DPU_CFARProcHWA_DynamicConfig::fovDoppler)
 *    - @ref DPU_CFARProcHWA_control (@ref DPU_CFARProcHWA_Cmd_FovRangeCfg, @ref DPU_CFARProcHWA_Cmd_FovDopplerCfg).
 *
 * In this output list, side information of noise (from the range CFAR HWA processing)
 * and SNR is also populated for each of the generated points in this list. The
 * SNR is calculated using the noise (from range CFAR HWA) and the detection matrix
 * value corresponding to the detected range,doppler position.
 *
 * The HWA CFAR function can also perform peak grouping functionality. It (enable/disable)
 * is configured from the application when issuing @ref DPU_CFARProcHWA_config (@ref DPU_CFARProcHWA_DynamicConfig::cfarCfgRange,
 * @ref DPU_CFARProcHWA_DynamicConfig::cfarCfgDoppler) or when issuing @ref DPU_CFARProcHWA_control (@ref DPU_CFARProcHWA_Cmd_CfarRangeCfg,
 * @ref DPU_CFARProcHWA_Cmd_CfarDopplerCfg). These configurations also allow CFAR parameters
 * like threshold scale and window/guard lengths to be set. The CFAR detection processing in
 * the Doppler domain can be bypassed by setting doppler direction's detection threshold to zero.
 * In this case the peak grouping in the Doppler domain is unavailable.
 *
 *
 * The HWA-CFAR timing diagram is illustrated in figure below.
 *
 * @image html hwa_cfar_timing.png "HWA-CFAR timing"
 *
 * @subsection cfarhwa_apis CFAR HWA APIs
 *
 * - @ref DPU_CFARProcHWA_init DPU initialization initialization function.
 * - @ref DPU_CFARProcHWA_config DPU configuration function. The configuration can only be done after
 *   the DPU has been initialized using @ref DPU_CFARProcHWA_init.
 * - @ref DPU_CFARProcHWA_process DPU processing function. This will execute the CFAR detection
 *   algorithm using HWA. This processing can only be done after the DPU has been configured through.
 * - @ref DPU_CFARProcHWA_config. If the parameters used by this DPU do not
 *   change from one frame to the next, @ref DPU_CFARProcHWA_config can be called only once for the first
 *   frame and for every frame @ref DPU_CFARProcHWA_process can be executed without the need of reconfiguring the DPU.
 * - @ref DPU_CFARProcHWA_control DPU control function. It processes the following messages received from DPC:
 *      - @ref DPU_CFARProcHWA_Cmd_CfarRangeCfg,
 *      - @ref DPU_CFARProcHWA_Cmd_CfarDopplerCfg,
 *      - @ref DPU_CFARProcHWA_Cmd_FovRangeCfg,
 *      - @ref DPU_CFARProcHWA_Cmd_FovDopplerCfg
 *
 *  Full API details can be seen at @ref CFAR_PROC_DPU_EXTERNAL
 *
 *
 *  Full API details can be seen at @ref CFAR_PROC_DPU_EXTERNAL */


/** @defgroup CFAR_PROC_DPU_EXTERNAL       cfarProc DPU External
 */

/**
@defgroup DPU_CFARPROC_EXTERNAL_FUNCTION            cfarProc DPU External Functions
@ingroup CFAR_PROC_DPU_EXTERNAL
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the cfarProc DPU
*/
/**
@defgroup DPU_CFARPROC_EXTERNAL_DATA_STRUCTURE      cfarProc DPU External Data Structures
@ingroup CFAR_PROC_DPU_EXTERNAL
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup DPU_CFARPROC_EXTERNAL_DEFINITIONS      cfarProc DPU External Definitions
@ingroup CFAR_PROC_DPU_EXTERNAL
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup DPU_CFARPROC_ERROR_CODE                   cfarProc DPU Error Codes
@ingroup CFAR_PROC_DPU_EXTERNAL
@brief
*   The section has a list of all the error codes which are generated by the sampleProc DPU
*/

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef CFARROC_COMMON_H
#define CFARROC_COMMON_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>


#ifdef __cplusplus
extern "C" {
#endif


/*! @brief   CFAR detection in range domain */
#define DPU_CFAR_RANGE_DOMAIN   0U

/*! * @brief   CFAR detection in Doppler domain */
#define DPU_CFAR_DOPPLER_DOMAIN 1U

/*! @brief Peak grouping scheme of CFAR detected objects based on peaks of neighboring cells taken from detection matrix */
#define DPU_CFAR_PEAK_GROUPING_DET_MATRIX_BASED 1U

/*! @brief Peak grouping scheme of CFAR detected objects based only on peaks of neighboring cells that are already detected by CFAR */
#define DPU_CFAR_PEAK_GROUPING_CFAR_PEAK_BASED  2U

/*! @brief  Convert peak/noise value to log10 value in 0.1dB
       Since, val = log2(|.|)* 2^Qformat = log10(|.|) / log10(2) * 2^Qformat
       Equation: output = 1/0.1 * 10log10(|.|^2) = 10 * [ val * 20log10(2) / 2^Qformat ] = val * 6.0 / 2^Qformat * 10
 */
#define CFARDSP_CONV_PEAK_TO_LOG(val, QFormat)        (val * 6.0F /(float)((uint32_t)1<<QFormat) * 10.0F)

/**
 * @brief
 *  CFAR Configuration
 *
 * @details
 *  The structure contains the cfar configuration used in data path
 */
typedef struct DPU_CFARProc_CfarCfg_t
{
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

} DPU_CFARProc_CfarCfg;

/**
 * @brief
 *  Data processing Unit statistics
 *
 * @details
 *  The structure is used to hold the statistics of the DPU 
 *
 *  \ingroup INTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_CFARProc_Stats_t
{
    /*! @brief total number of calls of DPU processing */
    uint32_t            numProcess;

    /*! @brief total processing time during all chirps in a frame excluding EDMA waiting time*/
    uint64_t            processingTime;

    /*! @brief total wait time for EDMA data transfer during all chirps in a frame*/
    uint64_t            waitTime;
}DPU_CFARProc_Stats;


/*!
 *  @brief    Field of view indices
 *
 *  \ingroup DPU_CFARPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_CFARProc_FovCfg_t
{
    /*! @brief Minimum value in meters for range, or meters/sec for doppler */
    float min;

    /*! @brief Maximum value in meters for range, or meters/sec for doppler */
    float max;
} DPU_CFARProc_FovCfg;


#ifdef __cplusplus
}
#endif

#endif 
