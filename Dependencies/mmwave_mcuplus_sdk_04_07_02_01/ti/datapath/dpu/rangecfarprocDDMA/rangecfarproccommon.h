/**
 *   @file  rangecfarproccommon.h
 *
 *   @brief
 *      Implements Common definition across rangecfarProc DPU.
 *
 *  \par
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

/** @mainpage Range CFAR DPU
 * [TOC]
 *  @section rangecfar_intro_section RANGE CFAR DPU
 *
 * This DPU implements CFAR object detection from the Range/Doppler detection
 * matrix.
 *
 *
 *  CFAR DPU includes HWA implementations:
 *   Device          |  DPU runs on core | Uses HWA
 *  :----------------|:------------------|:--------
 *   awr294x         |  DSP              | Yes
 *   awr2x44p        | DSS_CM4           | Yes
 *
 *  @section rangecfar_hwa HWA-RANGECFAR DPU
 *
 * @subsection rangecfarhwa_intro_section Introduction
 * This DPU implements CFAR object detection from the Range detection
 * matrix using hardware accelerator (HWA).
 * CFAR detection is performed in Range domain. The DPU exports
 * the list of objects that are detected in range domains.
 *
 * The following figure depicts the DPU implementation:
 *
 * @image html rangeCFAR.png "HWA-CFAR implementation"
 *
 * The input is detection matrix of the format [rangeBins][dopplerSubBins].
 * To utilize the ping-pong mechanism efficiently and to meet the HWA Mem Bank Size requirements,
 * the total number of doppler subbins (number of doppler FFT bins / num total sub bands)
 * are first divided into macro-blocks each of which contain blocks worth numDopplerGatesToDecompressPerPing
 * doppler sub bins. In the CFAR stage, numDopplerGatesToDecompressPerPing chirps are processed on in a ping-pong manner,
 * with the total number of loops to run for a DPU being numDopSubBins/(numDopplerGatesToDecompressPerPing*2), the multiplication
 * factor of 2 being due to the fact that 2*numDopplerGatesToDecompressPerPing will be processed in a single
 * Range CFAR HWA Loop due to the ping/pong mechanism.
 *
 * The process is initiated by triggering input EDMA that copies a macro block of the detection matrix
 * from L3 memory to internal HWA memory. Signature EDMA (@ref DPU_RangeCFARProcHWA_HW_Resources::edmaInSignature)
 * then triggers HWA CFAR which is executed and on completion, this triggers the EDMA Out transfer which transfers
 * the CFAR output into the L2 RAM. Then, the DSP takes over and processes on this data and extracts the object list.
 * While the DSP carries out the ping stage object extraction, the HWA takes over for the pong stage and vice versa.
 *
 * In this output list, side information of noise (from the range CFAR HWA processing)
 * and SNR is also populated for each of the generated points in this list.
 *
 * The HWA-CFAR timing diagram is illustrated in figure below.
 *
 * @image html hwa_rangecfar_timing.png "HWA-CFAR timing" width=80%
 *
 * @subsection cfarhwaDDM_apis CFAR HWA APIs
 *
 * - @ref DPU_RangeCFARProcHWA_init DPU initialization function.
 * - @ref DPU_RangeCFARProcHWA_config DPU configuration function. The configuration can only be done after
 *   the DPU has been initialized using @ref DPU_RangeCFARProcHWA_init.
 * - @ref DPU_RangeCFARProcHWA_process DPU processing function. This will execute the CFAR detection
 *   algorithm using HWA and extraction using DSP.
 *   This processing can only be done after the DPU has been configured through.
 *
 */


/** @defgroup CFAR_PROC_DPU_EXTERNAL       cfarProc DPU External
 */

/**
@defgroup DPU_RANGECFARPROC_EXTERNAL_FUNCTION            cfarProc DPU External Functions
@ingroup CFAR_PROC_DPU_EXTERNAL
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the cfarProc DPU
*/
/**
@defgroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE      cfarProc DPU External Data Structures
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
@defgroup DPU_RangeCFARProcHWA_HwaCfarConfig                   cfarProc DPU Error Codes
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
typedef struct DPU_RangeCFARProc_Stats_t
{
    /*! @brief total number of calls of DPU processing */
    uint32_t            numProcess;

    /*! @brief total processing time during all chirps in a frame excluding EDMA waiting time*/
    uint64_t            processingTime;

    /*! @brief total wait time for EDMA data transfer during all chirps in a frame*/
    uint64_t            waitTime;

    uint64_t            timeDsp[2];
    uint64_t            timeLoop;
}DPU_RangeCFARProc_Stats;


#ifdef __cplusplus
}
#endif

#endif
