/*
 *
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
 *   @file  rangeprochwaDDMA.h
 *
 *   @brief
 *      Implements range processing functionality using HWA.
 */
/**
 * @page dpu_rangehwaDDMA RangeProcHWADDMA
 * [TOC]
 *  @section toplevel_hwa_DDM Top Level Design
 *
 *  Range FFT processing is done by HWA hardware. Based on the RF parameters, rangeProcHWADDMA configures
 *  hardware accelerator FFT engine accordingly. It also configures data input and output EDMA channels to
 *  bring data in and out of range processing memory.
 *  First, samples are brought into the HWA memory from the ADC Buffer using EDMA. Then, DC Estimation is performed on
 *  the samples. This is followed by DC Subtraction and interference statistics estimation. Then, interference mitigation
 *  is performed on the samples. Then the FFT is performed on these samples followed by compression and storage into the
 *  radar cube.
 *
 *  HWA FFT process is triggered by hardware-based trigger -"chirp data available" which is hooked up to HWA internally in hardware.
 *
 *  After compression is done, HWA generates interrupt to rangeProcHWA DPU, at the same time triggers EDMA data
 *  output channel to copy FFT results to radarCube in configured format. EDMA interrupt done interrupt is triggered by EDMA hardware
 *  after the copy is completed.
 *
 *  @section config_hwa_DDM Data Interface Parameter Range
 *
 *  Here are supported ADCBuf and radarCube interface configurations:
 *
 *  ADCBuf Data Interface
 *----------------------
 *
 *   Parameter | Supported value
 *  :----------|:----------------:
 *   dataFmt | @ref DPIF_DATAFORMAT_REAL16 only for AWR294X
 *   interleave|interleave (@ref DPIF_RXCHAN_INTERLEAVE_MODE) ADC Data ([AdcSample][RxAntenna])
 *   numChirpsPerChirpEvent|1 ONLY
 *   numRxAntennas|4
 *   compressionMethod|EGE and BFP
 *   rxAntennaPerBlock|4 (only in EGE)
 *   rangeBinsPerBlock|Power of 2 (upto 16)
 *   compression dimension|Along RX and Range Bins in EGE, Along Range Bins in BFP
 *
 * How Real-only ADC Samples are handled {#real_only_DDM}
 *----------------------
 *  AWR294X only supports real ADC data. Due to this, the memory requirement for ADC data is reduced by half (since
 *  in this case, the DPU uses 16 bit real-only data instead of "16 bit Im, 16 bit Re" complex data format.). Hence, 2048 ADC samples
 *  can now be supported by the DPU. The 1-D FFT calculated on these samples results in a complex valued radar cube. However, since
 *  FFT is taken on real samples, the latter half of the FFT turns out to be the complex conjugate of the former half. Hence, the latter
 *  half of the FFT computed is rejected and not pushed into the HWA memory bank by the HWA output formatter directly. Thus, the radar
 *  cube memory requirement also reduces by half. So, in case of real only ADC samples, the FFT Size and the number of Range Bins is
 *  taken as half the number of ADC samples. Post the calculation of radar cube, the rest of the chain remains similar to the case of
 *  complex ADC samples.
 *
 *  @section input_hwa_DDM Data Input
 *
 *  There are three HWA input modes supported (@ref DPU_RangeProcHWA_InputMode),
 *  -  @ref DPU_RangeProcHWA_InputMode_ISOLATED : \n
 *     ADCBuf buffer and HWA memory are isolated in physical memory space.
 *     Data input EDMA channel is configured to transfer data from ADCBuf to HWA M0/M1 memory
 *     in ping/pong alternate order.
 *  -  @ref DPU_RangeProcHWA_InputMode_MAPPED :\n
 *     ADCBuf buffer and HWA memory are mapped. HWA can read ADC data directly. No copy is needed. This mode is obsolete for HWA 2.0.
 *  -  @ref DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM :\n
 *     ADC data are streamed directly to HWA internal memory in a ping/pong manner. No copy is needed.
 *
 *  @section output_hwa_DDM Data Output
 *
 *  RangeProcHWA configures data output EDMA channels to transfer compressed FFT results from HWA M2/M3 to radarCube memory.
 *  Follow on to understand the output data format.
 *
 *  @section process_hwa_DDM Data Processing Implementation
 *  The following image shows the data processing chain implementation.
 *
 *  @image html rangeDPU.png "rangeProcHWADDMA Data processing flow"
 *
 *  DC Est/Sub, Intf Est/Sub, Range FFT processing and Compression is done by HWA hardware.
 *  As shown in the following diagram, after @ref DPU_RangeProcHWA_config is completed,
 *  in every frame, rangeProcHWA is triggered through @ref DPU_RangeProcHWA_control \n
 *  using command @ref DPU_RangeProcHWA_Cmd_triggerProc. If the hardware resources are overlaid with other modules,
 *  @ref DPU_RangeProcHWA_config can be called before the next frame start. \n
 *
 * @image html hwa_callflow.png "rangeProcHWA call flow"
 *
 *  @subsection rpDDMACompression Compression
 *  @subsubsection EGE EGE Compression
 *  The EGE (Exponential Goulomb Encoder) data compression algorithm is used to compress data across range bins and rx antennas. Parameters issued
 *  by the user are the number of range bins per block and compression ratio. A block consisting of
 *  all samples corresponding to all Rx Antennas and the number of range bins per block are compressed
 *  together. Thus, the size of the block, which was of size (range bins per block) * (rx antennas) * (bytes per sample)
 *  before compression, would become a (range bins per block) * (rx antennas) * (bytes per sample) * (achieved compression ratio)
 *  sized block post compression. Hence, all samples in a single block will be taken into consideration for a single compression operation.
 *  For example, if a block with samples worth 64 bytes is compressed with a compression ratio of 0.5, the resultant
 *  block would have a size of 32 bytes. Note that since blocks must be word aligned, the compression ratio
 *  achieved might be higher than the compression ratio desired.
 *
 *  The following image shows the EGE compression operation for a single chirp-
 *
 *  @image html compressionInterleaveEGE.png "EGE Compression of single chirp FFT data (interleaved)"
 *
 *  @subsubsection BFP BFP Compression
 *  The BFP (Block Floating Point) data compression algorithm is used to compress data across range bins. Parameters issued
 *  by the user are the number of range bins per block and compression ratio. A block consisting of
 *  all samples corresponding to the number of range bins per block are compressed
 *  together. Thus, the size of the block, which was of size (range bins per block) * (bytes per sample)
 *  before compression, would become a (range bins per block) * (bytes per sample) * (achieved compression ratio)
 *  sized block post compression. Hence, all samples in a single block will be taken into consideration for a single compression operation.
 *  For example, if a block with samples worth 64 bytes is compressed with a compression ratio of 0.5, the resultant
 *  block would have a size of 32 bytes. Note that since blocks must be word aligned, the compression ratio
 *  achieved might be higher than the compression ratio desired.
 *
 *  The following image shows the BFP compression operation for a single chirp-
 *
 *  @image html compressionInterleaveBFP.png "BFP Compression of single chirp FFT data (interleaved)"
 *
 *  Ultimately, the DPU would result in a compressed radar cube of the following format.
 *
 *  @image html radarCubeCompressed.png "Output Radar Cube"
 *
 *  Here, in case of EGE compression, each Block consists of compressed range bins per block * RX antenna samples. And in case of BFP compression,
 *  each block represents RX antenna number of compressed blocks, each consisting range bins per block of a receive antenna.
 *
 *  @subsection dcEst DC Estimation/Subtraction
 *  The DC estimation block estimates the time-domain average of the stream of samples along the ADC Sample dimension.
 *  DC estimation is based on accumulation followed by a fine scaling and a right shift, which can be programmed by the application.
 *  It must be noted that while the support for disabling DC Estimation/Subtraction is not available as of yet, the DC subtraction step
 *  can easily be bypassed by disabling the same in the "PING DC SUBTRACTION, INTERFERENCE STATISTICS ESTIMATION PARAMSET" section
 *  of @ref rangeProcHWA_ConfigHWA function.
 *
 *  @subsection intf Interference Localization and Mitigation
 *  In an FMCW radar transceiver, interference from another radar typically manifests itself as a time-domain spike in a few samples.
 *  This spike corresponds to the time duration when the chirping frequency of both radars overlap with each other.
 *  Such a time-domain spike caused by interference can lead to degradation in the noise floor at the FFT output,
 *  causing degradation in detection performance. In order to mitigate the impact of interference, the DPU, using the HWA,
 *  block provides capability to perform interference localization to identify samples corrupted by interference,
 *  followed by interference mitigation to repair those samples. It must be noted that while the support for disabling Interference
 *  Localization/Mitigation is not available as of yet, the mitigation step can easily be bypassed by disabling the same in
 *  the "PING INTERFERENCE MITIGATION, FFT PARAMSET" section of @ref rangeProcHWA_ConfigHWA function.
 *
 * @section rangeProcCfg Alternate Range Processing Chains
 * @subsection prevFrameDcRangeProc Range Processing using average DC estimate from previous frame
 * In this mode of execution (can be enabled by rangeProcCfg CLI as @ref DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE), we use previous frame's average DC estimates,
 * instead of computing them inline. Thus, saving the execution time of one paramset (DC Estimation).
 * 
 * This mode of range processing works as follows:
 * -# Before triggering (SW trigger) the next subframe's range processing, processor loads back the corresponding DC Estimate into the SW DC Estimate
 * register. If there are N Rx, then N such registers are populated.
 * -# DC Estimation paramset is removed and instead, "DC Subtraction + Interference Statistics" paramset computes the current chirp DC estimate. 
 * These DC estimates are accumulated over all the chirps in the current subframe and their average is stored at the end of the subframe range processing. 
 * -# After the subframe's range processing is complete, processor stores a copy of DC estimates in an array at the position corresponding to the subframe index.
 * -# In legacy frame mode, above processor interventions don't happen, but HWA gets the DC estimates directly from internal registers.
 * 
 * @subsection fastRangeProc Fast Range Processing
 * As range processing happens inline with the chirp acquisition, if the range processing is fast, chirp repetition rate could also be faster, which implies improved
 * maximum measurable velocity.
 * In this mode of execution (can be enabled by rangeProcCfg CLI as @ref DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE), we use previous chirp's DC stats and interference thresholds estimates,
 * instead of computing them inline. Thus, saving the execution time of two paramsets (DC Estimation and DC Subtraction + Interference Statistics).
 *
 * This mode of range processing works as follows:
 * -# HWA polls for the completion of range processing of every chirp.
 * -# At completion, processor stores a copy of DC estimates and interference stats estimates in an array at the position corresponding to the chirpCfg index.
 * The number of elements in the array is given by the number of distinct per-chirp phase shifter configuration (which is also the number of DDMA bands). For example in AWR2944, the
 * number of DDMA bands is 6, the number of phase configurations is also 6.
 * -# Before triggering (SW trigger) the next chirp's range processing, processor loads back the corresponding DC Estimate and Interference Statistics into the SW DC Estimate and Interference Statistic
 * register. If there are N Rx, then N such registers are populated.
 * -# DC Estimation and Interference Stats paramsets are removed and instead, FFT paramset computes the current chirp statistics.
 * -# HWA Range FFT paramset uses SW programmed DC and interference Stats for data pre-processing (DC subtraction and interference mitigation) before FFT.
 *
 * Above steps repeat for all the chirps in a frame. \n
 * **Note** - In HWA, since interference stats estimation happens after interference mitigation, so it might happen that thresholds get decrease below a certain level.
 * In such a case, HWA may mark non-corrupted ADC data as interference affected and mitigate it as well. To avoid this,
 * before loading into the HWA registers, processor ensures that magThreshold and magDiffThreshold are above @ref DPU_RangeProcHWADDMA_rangeProcCfg.intfMitigMagThresMinLim "intfMitigMagThresMinLim" and  @ref
 * DPU_RangeProcHWADDMA_rangeProcCfg.intfMitigMagDiffThresMinLim "intfMitigMagDiffThresMinLim" respectively. These minimum threshold limits are programmable via CLI and can be changed based on real-time scenario.
 *
 * @section Real 2X Mode
 * When real ADC data is used, we can utilize the ability of HWA to do DC estimation and Interference Statistics on complex data to effectively process the samples at
 * twice the speed by considering two adjacent adc samples to become one complex sample. This can be enabled via rangeProcCfg CLI using @ref DPU_RangeProcHWADDMA_rangeProcCfg.isReal2XEnabled "isReal2XEnabled". 
 * But, this comes with a restriction that only one interference threshold is obtained rather than one for each Rx channel.
 * 
 * @section FFT Output Scaling
 * For objects with high SNR, the peak value can sometimes be high enough that it's value is lost during compression (observed in case EGE compression with dithering enabled). For this, a configurable parameter is 
 * provided @ref DPU_RangeProcHWADDMA_rangeProcCfg.fftOutputScaling "fftOutputScaling", which scales down the FFT output by the programmed number of bits.
 * 
 */

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef RANGEPROCHWA_H
#define RANGEPROCHWA_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* MCU Plus SDK Drivers include files */
#include <drivers/hwa.h>

/* mmWave SDK Data Path Include Files */
#include <ti/datapath/dpif/dpif_adcdata.h>
#include <ti/datapath/dpif/dpif_radarcube.h>
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpu/rangeprocDDMA/rangeproc_common.h>

/* Cache is not availble for M4 Core. This macro is added for aligned memory access. */
#if defined(__ARM_ARCH) && defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'M')
#define CSL_CACHE_L1D_LINESIZE 64U
#endif
#ifdef __cplusplus
extern "C" {
#endif

/*! Number of HWA parameter sets */
/*! @brief Number of HWA parameter sets for DDMA saved with Fast Range Proc*/
#define DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS_DDMA_SAVED_FASTDPU   4U
/*! @brief Number of HWA parameter sets for DDMA */
#define DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS_DDMA            10U
/*! @brief Number of HWA parameter sets for DDMA with BFP compression */
#define DPU_RANGEPROCHWA_MAX_NUM_HWA_PARAM_SETS_BFP_DDMA    16U

/*! Alignment for DC range signal mean buffer - if DPU is running on DSP(C66) */
#define DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_DSP 8U

/*! Alignment for DC range signal mean buffer - if DPU is running on R5F */
#define DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_R5F 4U

/*! Alignment for radar cube on R5F */
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F    CSL_CACHE_L1D_LINESIZE

/*! Alignment for radar cube on DSP */
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP     CSL_CACHE_L1D_LINESIZE

#define DPU_RANGEPROCHWADDMA_NUM_INTFMITIG_WIN_HWACOMMONCFG_SIZE (5U)

#define RANGEPROCHWADDMA_NUM_EDMA_INTERRUPTS 1U

/** @addtogroup DPU_RANGEPROC_ERROR_CODE
 *  Base error code for the rangeProc DPU is defined in the
 *  \include ti/datapath/dpif/dp_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define DPU_RANGEPROCHWA_EINVAL                  (DP_ERRNO_RANGE_PROC_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define DPU_RANGEPROCHWA_ENOMEM                  (DP_ERRNO_RANGE_PROC_BASE-2)

/**
 * @brief   Error Code: Internal error
 */
#define DPU_RANGEPROCHWA_EINTERNAL               (DP_ERRNO_RANGE_PROC_BASE-3)

/**
 * @brief   Error Code: Not implemented
 */
#define DPU_RANGEPROCHWA_ENOTIMPL                (DP_ERRNO_RANGE_PROC_BASE-4)

/**
 * @brief   Error Code: In Progress
 */
#define DPU_RANGEPROCHWA_EINPROGRESS             (DP_ERRNO_RANGE_PROC_BASE-5)

/**
 * @brief   Error Code: Invalid control command
 */
#define DPU_RANGEPROCHWA_ECMD                    (DP_ERRNO_RANGE_PROC_BASE-6)

/**
 * @brief   Error Code: Semaphore error
 */
#define DPU_RANGEPROCHWA_ESEMA                   (DP_ERRNO_RANGE_PROC_BASE-7)

/**
 * @brief   Error Code: DC range signal removal configuration error
 */
#define DPU_RANGEPROCHWA_EDCREMOVAL              (DP_ERRNO_RANGE_PROC_BASE-8)

/**
 * @brief   Error Code: ADCBuf data interface configuration error
 */
#define DPU_RANGEPROCHWA_EADCBUF_INTF            (DP_ERRNO_RANGE_PROC_BASE-9)

/**
 * @brief   Error Code: ADCBuf data interface configuration error
 */
#define DPU_RANGEPROCHWA_ERADARCUBE_INTF         (DP_ERRNO_RANGE_PROC_BASE-10)

/**
 * @brief   Error Code: HWA windowing configuration error
 */
#define DPU_RANGEPROCHWA_EWINDOW                 (DP_ERRNO_RANGE_PROC_BASE-11)

/**
 * @brief   Error Code: Incorrect number of butterfly stages specified for scaling
 */
#define DPU_RANGEPROCHWA_EBUTTERFLYSCALE         (DP_ERRNO_RANGE_PROC_BASE-12)



/**
@}
*/

/**
 * @brief
 *  RangeProc HWA Processing Chain Mode
 *
 * @details
 *  This enum defines which HWA Processing Chain is used.
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef uint8_t DPU_RangeProcHWA_RangeProcChain;
/*! @brief     3 HWA Paramsets (except Dummy and Compression)
                are used. DC Estimates and Interference Statistics
                are computed for current chirp and used in the same chirp.
                      */
#define DPU_RANGEPROCHWA_DEFAULT_MODE 0U
/*! @brief     2 HWA Paramsets (except Dummy and Compression)
                are used. DC Estimates is computed and stored for next
                frame while interference statistics is computed for current
                chirp and used in the same chirp. Thus, every chirp's DC compensation
                happens with the DC estimate of previous frame.
                      */
#define DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE 1U
/*! @brief     1 HWA Paramset (except Dummy and Compression)
                is used. DC Estimates and Interference Statistics 
                is computed and stored for next nth chirp. Thus, every chrips's
                DC Compensation and Interference Mitigation happens with the estimates
                from previous n'th chirp where n is total number of subbands.
                      */
#define DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE 2U

/**
 * @brief
 *  RangeProc data input mode
 *
 * @details
 *  This enum defines if the rangeProc input data is from RF front end or it is in M0 but
 *  standalone from RF.
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef uint32_t DPU_RangeProcHWA_InputMode;
/*! @brief     Range input is integrated with DFE input
                     ADC buffer is mapped to HWA memory
                     DMA data from ADC buffer to HWA is NOT required
                      */
#define DPU_RangeProcHWA_InputMode_MAPPED              (uint32_t) 0U

/*! @brief     Range input is integrated with DFE input
                     ADC buffer is not mapped to HWA memory,
                     DMA data from ADCBuf to HWA memory is
                     needed in range processing */
#define DPU_RangeProcHWA_InputMode_ISOLATED            (uint32_t) 1U

/*! @brief      Range input is stored into HWA internal memory
                     in ping/pong manner*/
#define DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM    (uint32_t) 2U

/**
 * @brief
 *  rangeProc control command
 *
 * @details
 *  The enum defines the rangeProc supported run time command
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef uint32_t DPU_RangeProcHWA_Cmd;
/*! @brief     Command to update DC Signature removal configuration */
#define DPU_RangeProcHWA_Cmd_dcRangeCfg         (uint32_t) 0
/*! @brief     Command to trigger rangeProcHWA process */
#define DPU_RangeProcHWA_Cmd_triggerProc        (uint32_t) 1

/**
 * @brief
 *  rangeProc FFT tuning parameters for HWA based Range FFT
 *
 * @details
 *  This structure allows users to tune the scaling factors for HWA based Range FFTs
 *  (currently unused)
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_FFTtuning_t{
    /*! @brief  Specify amount of right (divide) shift to apply
           to convert HWA internal 24-bit Range FFT output to 16-bit RadarCube.
           User should adjust this based on the setup where sensor is deployed and
           sensors setting for Tx O/P power/RX gain and their application needs */
    uint16_t    fftOutputDivShift;

    /*! @brief  Specify number of Last butterfly stages to scale to avoid clipping within
           HWA FFT stages. Given the ADC data bit width of 16-bits and internal 24-bit width
           of HWA, user has around 8-bits to grow Range FFT output and should not need to use butterfly scaling
           for FFT sizes upto 256. Beyond that fft size, user should adjust this based on the setup
           where sensor is deployed and sensors setting for Tx O/P power/RX gain*/
    uint16_t    numLastButterflyStagesToScale;

}DPU_RangeProcHWA_FFTtuning;

/**
 * @brief
 *  RangeProc HWA configuration
 *
 * @details
 *  The structure is used to hold the HWA configuration needed for Range FFT
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_HwaConfig_t
{
    /*! @brief     HWA paramset Start index */
    uint8_t         paramSetStartIdx;

    /*! @brief     Number of HWA param sets must be @ref DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS_DDMA */
    uint8_t         numParamSet;

    /*! @brief     Flag to indicate if HWA windowing is symmetric
                    see HWA_WINDOW_SYMM definitions in HWA driver's doxygen documentation
     */
    uint8_t         hwaWinSym;

    /*! @brief     HWA windowing RAM offset in number of samples */
    uint16_t        hwaWinRamOffset;

    /*! @brief     Data Input Mode, */
    DPU_RangeProcHWA_InputMode      dataInputMode;

    /*! @brief Pointer to HWA Interference Mitigation window */
    uint8_t         hwaInterfMitigWindow[DPU_RANGEPROCHWADDMA_NUM_INTFMITIG_WIN_HWACOMMONCFG_SIZE];

    /*! @brief     HWA hardware trigger source. This is used only in @ref DPU_RangeProcHWA_InputMode_HWA_INTERNAL_MEM mode */
    uint8_t         hardwareTrigSrc;

}DPU_RangeProcHWA_HwaConfig;

/**
 * @brief
 *  RangeProc EDMA configuration
 *
 * @details
 *  The structure is used to hold the EDMA configuration needed for Range FFT
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_EDMAInputConfig_t
{
    /*! @brief     EDMA configuration for rangeProc data Input
                    This is needed only in @ref DPU_RangeProcHWA_InputMode_ISOLATED
     */
    DPEDMA_ChanCfg        dataIn;

    /*! @brief     EDMA configuration for rangeProc data Input Signature */
    DPEDMA_ChanCfg        dataInSignature;
}DPU_RangeProcHWA_EDMAInputConfig;

/**
 * @brief
 *  RangeProc EDMA configuration
 *
 * @details
 *  The structure is used to hold the EDMA configuration needed for Range FFT
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_EDMAOutputConfigFmt1_t
{
    /*! @brief     EDMA configuration for rangeProc data Out- ping
                    It must be a HWACC triggered EDMA channel.
     */
    DPEDMA_ChanCfg        dataOutPing;

    /*! @brief     EDMA configuration for rangeProc data Out- pong
                    It must be a HWACC triggered EDMA channel
     */
    DPEDMA_ChanCfg        dataOutPong;
}DPU_RangeProcHWA_EDMAOutputConfigFmt1;

/**
 * @brief
 *  RangeProc EDMA configuration
 *
 * @details
 *  The structure is used to hold the EDMA configuration needed for Range FFT
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_EDMAOutputConfigFmt2_t
{
    /*! @brief     EDMA configuration for rangeProc data Out- ping
                    It must be a HWACC triggered EDMA channel
     */
    DPEDMA_3LinkChanCfg   dataOutPing;
    DPEDMA_ChanCfg        dataOutPingData[3];

    /*! @brief     EDMA configuration for rangeProc data Out- pong
                    It must be a HWACC triggered EDMA channel
     */
    DPEDMA_3LinkChanCfg   dataOutPong;
    DPEDMA_ChanCfg        dataOutPongData[3];
}DPU_RangeProcHWA_EDMAOutputConfigFmt2;

/**
 * @brief
 *  RangeProc output EDMA configuration
 *
 * @details
 *  The structure is used to hold the EDMA configuration needed for Range FFT
 *
 *  Fmt1: Generic EDMA ping/pong output mode
 *       - 1 ping/pong EDMA channel,
 *       - 1 ping/pong HWA signature channel
 *
 *  Fmt2: Specific EDMA ping/pong output mode used ONLY for 3 TX anntenna for radar cube
 *        layout format: @ref DPIF_RADARCUBE_FORMAT_1, ADCbuf interleave mode
 *        @ref DPIF_RXCHAN_NON_INTERLEAVE_MODE
 *       - 1 ping/pong dummy EDMA channel with 3 shadow channels
         - 3 ping/pong dataOut channel
 *       - 1 ping/pong HWA signature channel
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_EDMAOutputConfig_t
{
    /*! @brief     EDMA data output Signature */
    DPEDMA_ChanCfg        dataOutSignature;

    union
    {
        /*! @brief     EDMA data output fmt1 @ref DPU_RangeProcHWA_EDMAOutputConfigFmt1 */
        DPU_RangeProcHWA_EDMAOutputConfigFmt1     fmt1;

        /*! @brief     EDMA data output fmt2 @ref DPU_RangeProcHWA_EDMAOutputConfigFmt2 */
        DPU_RangeProcHWA_EDMAOutputConfigFmt2     fmt2;
    }u;
}DPU_RangeProcHWA_EDMAOutputConfig;

/**
 * @brief
 *  RangeProcHWA hardware resources
 *
 * @details
 *  The structure is used to hold the hardware resources needed for Range FFT
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_HW_Resources_t
{
    /*! @brief     EDMA Handle */
    EDMA_Handle         edmaHandle;

    /*! @brief     HWA configuration */
    DPU_RangeProcHWA_HwaConfig      hwaCfg;

    /*! @brief     EDMA configuration for rangeProc data Input */
    DPU_RangeProcHWA_EDMAInputConfig edmaInCfg;

    /*! @brief     EDMA configuration for rangeProc data Output */
    DPU_RangeProcHWA_EDMAOutputConfig edmaOutCfg;

    /*! @brief     Pointer to Calibrate DC Range signature buffer
                    The size of the buffer = DPU_RANGEPROC_SIGNATURE_COMP_MAX_BIN_SIZE *
                                        numTxAntenna * numRxAntenna * sizeof(cmplx32ImRe_t)
        For R5F:\n
        Byte alignment Requirement = @ref DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_R5F \n
        For DSP (C66X):\n
        Byte alignment Requirement = @ref DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_DSP \n
     */
    cmplx32ImRe_t       *dcRangeSigMean;

    /*! @brief     DC range calibration scratch buffer size */
    uint32_t            dcRangeSigMeanSize;

    /*! @brief      Radar cube data interface. Radar cube buffer (radarCube.data)
        For R5F:\n
        Byte alignment Requirement = @ref DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F \n
        For DSP (C66X):\n
        Byte alignment Requirement = @ref DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP \n
     */
    DPIF_RadarCube      radarCube;

    /* EDMA Interrupt Object */
    Edma_IntrObject     *edmaTransferCompleteIntrObj;

    /* DC Estimation (I) Values Buffer */
    uint32_t *dcEstIVal;

    /* DC Estimation (Q) Values Buffer */
    uint32_t *dcEstQVal;

    /* Interf Threshold Magnitude Buffer */
    uint32_t *intfThresMagVal;

    /* Interf Threshold Magnitude Difference Buffer */
    uint32_t *intfThresMagDiffVal;

}DPU_RangeProcHWA_HW_Resources;

/**
 * @brief
 *  RangeProcHWA Compression hardware resources
 *
 * @details
 *  The structure is used to hold the hardware resources needed for compression of Range FFT
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_CompressionCfg_t
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

    /*! @brief extra compression parameters for BFP, dependent on number of enabled RX antenna*/
    uint8_t bfpCompExtraParamSets;

}DPU_RangeProcHWA_CompressionCfg;

/**
 * @brief
 *  RangeProcHWA Interference mitigation configuration
 *
 * @details
 *  The structure is used to hold the hardware resources needed for interference mitigation in Range DPU
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWADDMA_intfStatsdBCfg_t
{

    /*! @brief Interference mitigation magnitude SNR in dB */
    uint32_t intfMitgMagSNRdB;

    /*! @brief Interference mitigation magdiff SNR in dB */
    uint32_t intfMitgMagDiffSNRdB;

}DPU_RangeProcHWADDMA_intfStatsdBCfg;

/**
 * @brief
 *  RangeProcHWA Range Processing HWA Chain configuration
 *
 * @details
 *  The structure is used to hold the config needed for Range Processing HWA Chain.
 *  Refer DPU doxygen to know more about this feature and meaning of these inputs.
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWADDMA_rangeProcCfg_t
{
    /*! @brief HWA Chain select for Range Processing
        0 -> 3 Paramsets 
		1 -> 2 Paramsets (dc estimation from previous frame)
		2 -> 1 Paramset  (dc estimation and interference stats from previous nth chirp where n = number of subbands) */
    DPU_RangeProcHWA_RangeProcChain  rangeProcChain;

    /*! @brief Real 2X mode for DC estimation and Interference Statistics */
    uint32_t isReal2XEnabled;

    /*! @brief Interference mitigation mag threshold minimum limit */
    uint32_t intfMitigMagThresMinLim;

    /*! @brief Interference mitigation mag diff threshold minimum limit */
    uint32_t intfMitigMagDiffThresMinLim;

    /*! @brief Range FFT scaling applied to output */
    uint8_t fftOutputScaling;

}DPU_RangeProcHWADDMA_rangeProcCfg;

/**
 * @brief
 *  RangeProcHWA static configuration
 *
 * @details
 *  The structure is used to hold the static configuraiton used by rangeProcHWA
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_StaticConfig_t
{
    /*! @brief  Number of transmit antennas */
    uint8_t     numTxAntennas;

    /*! @brief  Number of virtual antennas */
    uint8_t     numVirtualAntennas;

    /*! @brief  Number of range bins */
    uint16_t    numRangeBins;

    /*! @brief  Number of bins used in Range FFT Calculation */
    uint16_t    numFFTBins;

    /*! @brief  1 if ADC Samples are real */
    uint16_t    isChirpDataReal;

    /*! @brief  Number of chirps per frame */
    uint16_t    numChirpsPerFrame;

    /*! @brief  Range FFT window coefficients, Appliation provided windows coefficients
                After @ref DPU_RangeProcHWA_config(), windowing buffer is not used by rangeProcHWA DPU,
                Hence memory can be released
     */
    int32_t    *window;

    /*! @brief     Range FFT window coefficients size in bytes
                    non-symmetric window, size = sizeof(uint32_t) * numADCSamples
                    symmetric window, size = sizeof(uint32_t)*(numADCSamples round up to even number )/2
     */
    uint32_t    windowSize;

    /*! @brief      ADCBuf buffer interface */
    DPIF_ADCBufData     ADCBufData;

    /*! @brief      Flag to reset dcRangeSigMean buffer
                     1 - to reset the dcRangeSigMean buffer and counter
                     0 - do not reset
     */
    uint8_t     resetDcRangeSigMeanBuffer;

    /*! @brief   Total Number of subbands used in DDMA demodulation */
    uint8_t     numBandsTotal;

    /*! @brief     Range FFT Tuning Params */
    DPU_RangeProcHWA_FFTtuning    rangeFFTtuning;

    /*! @brief Compression Configuration */
    DPU_RangeProcHWA_CompressionCfg  compressionCfg;

    /*! @brief HWA Static Shift/Scale config for DC Est */
    DPU_RangeProc_shiftScaleCfg  dcEstShiftScaleCfg;

    /*! @brief HWA Static Shift/Scale config for Interf Stats Mag */
    DPU_RangeProc_shiftScaleCfg  intfStatsMagShiftScaleCfg;

    /*! @brief HWA Static Shift/Scale config for Interf Stats Mag Diff */
    DPU_RangeProc_shiftScaleCfg  intfStatsMagDiffShiftScaleCfg;

    /* @brief Range proc DPU HWA Chain Configuration */
    DPU_RangeProcHWADDMA_rangeProcCfg rangeProcCfg;

}DPU_RangeProcHWA_StaticConfig;

/**
 * @brief
 *  RangeProcHWA dynamic configuration
 *
 * @details
 *  The structure is used to hold the dynamic configuraiton used by rangeProcHWA
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_DynamicConfig_t
{
    /*! @brief      Pointer to Calibrate DC Range signature configuration */
    DPU_RangeProc_CalibDcRangeSigCfg *calibDcRangeSigCfg;
}DPU_RangeProcHWA_DynamicConfig;

/**
 * @brief
 *  Range FFT configuration
 *
 * @details
 *  The structure is used to hold the configuration needed for Range FFT
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_Config_t
{
    /*! @brief     rangeProc hardware resources */
    DPU_RangeProcHWA_HW_Resources   hwRes;

    /*! @brief     rangeProc static configuration */
    DPU_RangeProcHWA_StaticConfig   staticCfg;

    /*! @brief     rangeProc dynamic configuration */
    DPU_RangeProcHWA_DynamicConfig  dynCfg;
}DPU_RangeProcHWA_Config;

/**
 * @brief
 *  rangeProcHWA DPU initial configuration parameters
 *
 * @details
 *  The structure is used to hold the DPU initial configurations.
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_InitParams_t
{
    /*! @brief     HWA Handle */
    HWA_Handle          hwaHandle;
}DPU_RangeProcHWA_InitParams;

/**
 * @brief
 *  rangeProcHWA output parameters populated during rangeProc Processing time
 *
 * @details
 *  The structure is used to hold the output parameters for rangeProcHWA
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_OutParams_t
{
    /*! @brief      End of Chirp indication for rangeProcHWA */
    bool                endOfChirp;

    /*! @brief     rangeProcHWA stats */
    DPU_RangeProc_stats  stats;
}DPU_RangeProcHWA_OutParams;

/**
 * @brief
 *  rangeProc DPU Handle
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef void* DPU_RangeProcHWA_Handle ;

/*================================================================
               rangeProcHWA DPU exposed APIs
 ================================================================*/
DPU_RangeProcHWA_Handle DPU_RangeProcHWA_init
(
    DPU_RangeProcHWA_InitParams     *initParams,
    volatile uint8_t            subframeCounter,
    int32_t*                        errCode
)
#if defined(SUBSYS_M4) && SUBSYS_M4
 __attribute__((section (".customCode")));
#else
;
#endif

int32_t DPU_RangeProcHWA_findDCEstStaticParams
(
    uint32_t numSamples, 
    uint32_t *scaleVal, 
    uint32_t *shiftVal
)
#if defined(SUBSYS_M4) && SUBSYS_M4
 __attribute__((section (".customCode")));
#else
;
#endif

int32_t DPU_RangeProcHWA_findIntfStatsStaticParams
(
    uint32_t numSamples, 
    uint32_t SNRdB, 
    uint32_t *scaleVal, 
    uint32_t *shiftVal
)
#if defined(SUBSYS_M4) && SUBSYS_M4
 __attribute__((section (".customCode")));
#else
;
#endif

void rangeProcHWA_loadPreProcStats
(
    DPU_RangeProcHWA_Config*    rangeHwaCfg,
    size_t                      saveRestoreDcEstDataSize,
    size_t                      saveRestoreIntfStatDataSize,
    uint8_t                     index
);

void rangeProcHWA_storePreProcStats
(
    DPU_RangeProcHWA_Config*    rangeHwaCfg,
    size_t                      saveRestoreDcEstDataSize,
    size_t                      saveRestoreIntfStatDataSize,
    uint8_t                     index
);

int32_t DPU_RangeProcHWA_config
(
    DPU_RangeProcHWA_Handle     handle,
    DPU_RangeProcHWA_Config*    rangeHwaCfg
);

int32_t DPU_RangeProcHWA_process
(
    DPU_RangeProcHWA_Handle     handle,
    DPU_RangeProcHWA_Config*    rangeHwaCfg,
    DPU_RangeProcHWA_OutParams* outParams
);

int32_t DPU_RangeProcHWA_control
(
    DPU_RangeProcHWA_Handle handle,
    DPU_RangeProcHWA_Config* rangeHwaCfg,
    DPU_RangeProcHWA_Cmd    cmd,
    void*                   arg,
    uint32_t                argSize
);

int32_t DPU_RangeProcHWA_deinit
(
    DPU_RangeProcHWA_Handle     handle
);

#ifdef __cplusplus
}
#endif

#endif
