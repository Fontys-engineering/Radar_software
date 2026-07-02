/*
 *
 *  NOTE:
 *      (C) Copyright 2023-2025 Texas Instruments, Inc.
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
 *   @file  rangeprochwaReal2x.h
 *
 *   @brief
 *      Implements range processing functionality using HWA.
 */
/**
 * @mainpage RangeProc Real 2x DPU
 * [TOC]
 * This DPU implements range processing using hardware accelerator (HWA).
 * Range Processing Unit takes RF data in ADC buffer, performs DC estimation and subtraction,
 *  interference estimation and mitigation, and computes 1D FFT and saves results in L3 RAM in
 *  compressed format. This DPU supports both 1x (single-path) and 2x (two-parallel path) mode processing of data. This DPU is valid only for AWR2544.
 *
 *  Range Processing includes HWA implementations:
 *
 *   DPU         |  runs on cores
 *  :------------|:----------------
 *  rangeprochwaReal2x |  R5F
 *  @section toplevel_hwa Top Level Design
 *
 *  Range FFT processing is done by HWA hardware. Based on the RF parameters,
 *  rangeProcHWA configures hardware accelerator FFT ad Compression engine
 *  accordingly. It configures data input and output EDMA channels to
 *  bring data in and out of range processing memory. It also configures the
 *  header and footer EDMA channel to bring the payload header information
 *  (for example, chirp number, frame number) from bookkeeping registers and
 *  payload footer information (timestamp) from timer registers respectively to
 *  the radar cube memory (L3).
 *  First, samples are brought into the HWA memory from the ADC Buffer using EDMA. Then, DC Estimation is performed on
 *  the samples. This is followed by DC Subtraction and interference statistics estimation. Then, interference mitigation
 *  is performed on the samples. Then the FFT is performed on these samples followed by compression and storage into the
 *  radar cube.
 *
 *  HWA FFT process is triggered by hardware-based trigger - "chirp data available" which is hooked up to HWA internally in hardware.
 *
 *  After compression is done, HWA generates interrupt to rangeProcHWA DPU, at the same time triggers EDMA data
 *  output channel to copy FFT results to radarCube in linear format. EDMA interrupt done interrupt is triggered by EDMA hardware
 *  after the copy is completed.
 *
 *
 *
 *  @section config_hwa Data Interface Parameter Range
 *
 *  Here are supported ADCBuf and radarCube interface configurations:
 *
 *  ADCBuf Data Interface
 *----------------------
 *
 *   Parameter | Supported value
 *  :----------|:----------------:
 *   dataFmt | @ref DPIF_DATAFORMAT_REAL16 for AWR2544
 *   interleave|non-interleave (@ref DPIF_RXCHAN_NON_INTERLEAVE_MODE) ADC Data ([RxAntenna][AdcSample])
 *   numChirpsPerChirpEvent|1 ONLY
 *   numRxAntennas|4
 *   compressionMethod|EGE and BFP
 *   rxAntennaPerBlock|4 (only in EGE)
 *   rangeBinsPerBlock|Power of 2 (upto 16)
 *   compression dimension|Along RX and Range Bins in EGE, Along Range Bins in BFP
 *
 * How Real-only ADC Samples are handled {#real_only}
 *----------------------
 *  AWR2544 only supports real ADC data. Due to this, the memory requirement for ADC data is reduced by half (since
 *  in this case, the DPU uses 16 bit real-only data instead of "16 bit Im, 16 bit Re" complex data format). Hence, 2048 ADC samples
 *  can now be supported by the DPU. The 1-D FFT calculated on these samples results in a complex valued radar cube. However, since
 *  FFT is taken on real samples, the latter half of the FFT turns out to be the complex conjugate of the former half. Hence, the latter
 *  half of the FFT computed is rejected and not pushed into the HWA memory bank by the HWA output formatter directly. Thus, the radar
 *  cube memory requirement also reduces by half. So, in case of real only ADC samples, the FFT size and the number of Range Bins is
 *  taken as half the number of ADC samples.
 *
 * This DPU for AWR2544 also supports 2X mode operation for real-data. That is, FFT is computed in two parallel path using HWA. HWA sees real input data as complex (X_odd + j*X_even), for example, X[1] + jX[0], X[3] + jX[2]. Therefore, in 2X mode operation it is mandatory to have input data in non-interleaved format, HWA source configuration as complex, and FFT size to be half (for effective 256 point real fft, FFT size configured is 7 in 2X mode).
 *
 *  @section dpu_config Data Processing Configurations
 *
 *  @subsection config HWA Configuration
 *
 *  Range DPU has 10 paramsets (5 PING + 5 PONG). These parameter
 *  configuration and static (common) configuration structures are generated via SysConfig. Configurations
 *  dependent on the user input are updated during the run time.
 *  \n\n
 *  Parameter         |  Trigger | Input MemBank | Output MemBank  |  On Completion  |
 *  :-----------------|:-----------|:---------------|:---------------|:----------------|
 *  Dummy           | DMA | - | - | -
 *  DC Estimation   | DMA | M0/M1 | M2/M3 | -
 *  DC Subtraction and Interference Estimation | Immediate | M0/M1 | M2/M3 | -
 *  Interference Mitigation and FFT | Immediate | M2/M3 | M0/M1 | -
 *  Compression (EGE/BFP)   |  Immediate | M0/M1 | M2/M3 | Radar Cube Out EDMA Triggered
 *
 * In case of BFP compression, input and output format of all the paramsets
 * is non-interleaved. However, in case of EGE compression method, FFT
 * paramset output is stored in interleaved format. This helps in compressing
 * the range bins and RX together in one block. Rest of the paramsets input
 * and output format is non-interleaved only in case of EGE compression.
 *
 * @subsection input_hwa Data Input
 *
 *  Only single HWA input mode is supported (@ref DPU_RangeProcHWA_InputMode),
 *  -  @ref DPU_RangeProcHWA_InputMode_ISOLATED : \n
 *     ADCBuf buffer and HWA memory are isolated in physical memory space.
 *     Data input EDMA channel is configured to transfer data from ADCBuf to HWA M0/M1 memory
 *     in ping/pong alternate order.
 *
 *  @subsection output_hwa Data Output
 *
 *  Compressed radar cube stored in L3 is simultaneously transmitted via
 *  ethernet, enabling reuse of the limited available L3 RAM to store the
 *  entire compressed radar cube. Maximum three iterations of the L3 reuse is
 *  supported as now, which enables storing of radar cube of maximum size of approx. 2.
 *  8MB (L3 size (960KB) * 3 (inclusive of header and footer per payload)).
 *  Thus, three link EDMAs are configured to transfer the
 *  data from HWA to radarCube memory. Link EDMA resets the destination
 *  address back to start address of the radar cube memory. In the use cases,
 *  where less than three iterations of L3 are sufficient to store the
 *  compressed radar cube, three links are configured as two links / single link as per use.
 *  Note that in order to iterate L3 more than three times (to store radar cubes bigger than >2.8MB),
 *  link corresponding number of EDMA shadow channels. For example, to iterate L3 4 times,
 *  configure 4 linked EDMAs, i.e., one main EDMA channel, 3 link parameters, and one shadow parameter
 *  (copy of main channel parameter).
 *
 *  @image html L3_ReUse.png "L3 RAM Re-Use to Store Compressed Radar Cube"
 *
 *  Each chirp data needs to be split into packets as per maximum
 *  payload size (Application Header + Compressed 1-D FFT Blocks + Application
 *  Footer) of the ethernet buffer. Every packet should have application
 *  header and footer during transmission. This is taken care while storing
 *  the compressed radar cube from HWA to L3. That is, EDMA stores the
 *  compressed data from HWA to L3 keeping the offset for header (16 bytes)
 *  and footer (8 bytes) data.
 *
 *  @image html Packet_header_footer.png "Application Header and Footer"
 *
 *  EDMA to bring Header data from bookkeeping registers to L3 is triggered by - "chirp cycle start" event. It brings the magic number, sequence number,
 *  frame count, and chirp count for number of payloads times every chirp.
 *  Footer EDMA to store the timestamp in L3 is triggered by EDMA radar cube
 *  data out channel. This EDMA brings the RTI timestamp from RTIA timer
 *  registers for number of payloads times every chirp. Follow on to
 *  understand the output data format.
 *
 * \n\n
 *
 *  @section process_hwa Data Processing Implementation
 *  The following image shows the data processing chain implementation.
 *
 *  @image html rangeDPU.png "Range DPU Data Processing Flow"
 *
 *  DC Est/Sub, Intf Est/Sub, Range FFT processing and Compression is done by HWA hardware.
 *  As shown in the following diagram, after @ref DPU_RangeProcHWA_config is completed,
 *  in every frame, rangeProcHWA is triggered through @ref DPU_RangeProcHWA_control \n
 *  using command @ref DPU_RangeProcHWA_Cmd_triggerProc. If the hardware resources are overlaid with other modules,
 *  @ref DPU_RangeProcHWA_config can be called before the next frame start. \n
 *
 * @image html hwa_callflow.png "Range DPU Call Flow"
 *
 *  @subsection rpCompression Compression
 *  @subsubsection EGE EGE Compression
 *  The EGE (Exponential Goulomb Encoder) data compression algorithm is used
 * to compress data across range bins and rx antennas. FFT Output is stored in
 * interleaved format to enable EGE compression across range bins and rx
 * antennas. Parameters issued by the user are the number of range bins per
 * block and compression ratio. A block consisting of all samples
 * corresponding to all Rx Antennas and the number of range bins per block are
 * compressed together. Thus, the size of the block, which was of size (range
 * bins per block) * (rx antennas) * (bytes per sample) before compression,
 * would become a (range bins per block) * (rx antennas) * (bytes per sample) * (achieved compression ratio)
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
 *  @image html compressionNonInterleaveBFP.png "BFP Compression of single chirp FFT data (non-interleaved)"
 *
 *  Ultimately, the DPU would result in a compressed radar cube of the following format.
 *
 *  @image html radarCubeCompressed.png "Output Compressed Radar Cube"
 *
 *  Here, in case of EGE compression, each block consists of compressed range bins per block * RX antenna samples. And in case of BFP compression, each block consists of compressed range bins per block. Each payload starts with header and ends with footer. Each chirp can have x (>1) number of payloads depending upon chirp data size and payload buffer size.
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
#include <stdio.h>
#include <math.h>
/* MCU Plus SDK Drivers include files */
#include <drivers/hwa.h>

/* mmWave SDK Data Path Include Files */
#include <ti/datapath/dpif/dpif_adcdata.h>
#include <ti/datapath/dpif/dpif_radarcube.h>
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpedma/dpedma.h>

#ifdef __cplusplus
extern "C" {
#endif

/*! Number of HWA parameter sets */
#define DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS                             10U

/*! Alignment for DC range signal mean buffer - if DPU is running on DSP(C66) */
#define DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_DSP              8U

/*! Alignment for DC range signal mean buffer - if DPU is running on R5F */
#define DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_R5F              4U

/*! Alignment for radar cube on R5F */
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F                   CSL_CACHE_L1D_LINESIZE

/*! Alignment for radar cube on DSP */
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP                   CSL_CACHE_L1D_LINESIZE

#define DPU_RANGEPROCHWA_INTFMITIG_WIN_SIZE                       (5U)

#define RANGEPROCHWA_L3REUSE_MAX_ITERATIONS                                 3U

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
 * @brief   Error Code: Radar Cube Interface Error
 */
#define DPU_RANGEPROCHWA_ERADARCUBE_INTF         (DP_ERRNO_RANGE_PROC_BASE-10)

/**
 * @brief   Error Code: Radar Cube Datasize error
 */
#define DPU_RANGEPROCHWA_ERADARCUBE_DATASIZE     (DP_ERRNO_RANGE_PROC_BASE-11)

/**
 * @brief   Error Code: Radar Cube Buffer error
 */
#define DPU_RANGEPROCHWA_ERADARCUBE_BUF          (DP_ERRNO_RANGE_PROC_BASE-12)

/**
 * @brief   Error Code: HWA windowing configuration error
 */
#define DPU_RANGEPROCHWA_EWINDOW                 (DP_ERRNO_RANGE_PROC_BASE-13)

/**
 * @brief   Error Code: Footer Timer Not Enabled
 */
#define DPU_RANGEPROCHWA_EFOOTERTIMER            (DP_ERRNO_RANGE_PROC_BASE-15)

/**
@}
*/

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
                     ADC buffer is not mapped to HWA memory,
                     DMA data from ADCBuf to HWA memory is
                     needed in range processing */
#define DPU_RangeProcHWA_InputMode_ISOLATED            (uint32_t) 1U

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

    /*! @brief     Number of HWA param sets must be @ref DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS */
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
    uint8_t         hwaInterfMitigWindow[DPU_RANGEPROCHWA_INTFMITIG_WIN_SIZE];

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
 *  RangeProc output EDMA configuration
 *
 * @details
 *  The structure is used to hold the EDMA configuration needed for Range FFT
 *
 *  Fmt1: Generic EDMA ping/pong output mode with three links
 *       - 1 ping/pong EDMA channel,
 *       - 1 ping/pong HWA signature channel
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_EDMAOutputConfig_t
{
    /*! @brief     EDMA data output Signature */
    DPEDMA_ChanCfg        dataOutSignature;

    /*! @brief     EDMA configuration for rangeProc data Out- ping
                    It must be a HWACC triggered EDMA channel.
     */
    DPEDMA_3LinkChanCfg        dataOutPing;

    /*! @brief     EDMA configuration for rangeProc data Out- pong
                    It must be a HWACC triggered EDMA channel
     */
    DPEDMA_3LinkChanCfg        dataOutPong;
}DPU_RangeProcHWA_EDMAOutputConfig;

/**
 * @brief
 *  RangeProc EDMA Network Packet configuration
 *
 * @details
 *  The structure is used to hold the EDMA configuration needed for header and footer
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProcHWA_EDMAPacketConfig_t
{
    /*! @brief     EDMA configuration for application header */
    DPEDMA_3LinkChanCfg        appHeader;

    /*! @brief     EDMA configuration for application footer */
    DPEDMA_3LinkChanCfg        appFooter;

    /*! @brief     EDMA callback function application footer */
    Edma_EventCallback         appFooterCallbackFxn;

    /* EDMA Interrupt Object */
    Edma_IntrObject           *edmaCompleteIntrObj;
}DPU_RangeProcHWA_EDMAPacketConfig;

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

    /*! @brief     EDMA configuration for packet header and footer */
    DPU_RangeProcHWA_EDMAPacketConfig edmaPacketCfg;

    uint32_t            radarCubeBufSizeL3;

    /*! @brief      Radar cube data interface. Radar cube buffer (radarCube.data)
        For R5F:\n
        Byte alignment Requirement = @ref DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F \n
        For DSP (C66X):\n
        Byte alignment Requirement = @ref DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP \n
     */
    DPIF_RadarCube      radarCube;

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
typedef struct DPU_RangeProcHWA_CompressionCfg
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
typedef struct DPU_RangeProcHWA_intfStatsdBCfg_t
{

    /*! @brief Interference mitigation magnitude SNR in dB */
    uint32_t intfMitgMagSNRdB;

    /*! @brief Interference mitigation magdiff SNR in dB */
    uint32_t intfMitgMagDiffSNRdB;

}DPU_RangeProcHWA_intfStatsdBCfg;

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

    /*! @brief  Size of input sample */
    uint16_t    sizeOfInputSample;

    /*! @brief  1 if ADC Samples are real */
    uint16_t    isMode2x;

    /*! @brief  Number of chirps per frame */
    uint16_t    numChirpsPerFrame;

    /*! @brief  Number of Chirps Data to be Stored in L3RAM in ecah iteration */
    uint16_t    numChirpsEachIter[RANGEPROCHWA_L3REUSE_MAX_ITERATIONS];

    /*! @brief  Application header size per packet*/
    uint32_t    appHeaderSize;

    /*! @brief  Application footer size per packet*/
    uint32_t    appFooterSize;

    /*! @brief  Number of Payloads per chirp*/
    uint32_t    numPayloads;

    /*! @brief  Number of Payloads per Frame*/
    uint32_t    numPayloadsPerFrame;

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

    /*! @brief Compression Configuration */
    DPU_RangeProcHWA_CompressionCfg  compressionCfg;

    /*! @brief Interference mitigation Configuration */
    DPU_RangeProcHWA_intfStatsdBCfg intfStatsCfgdB;

}DPU_RangeProcHWA_StaticConfig;


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
 *  Data processing Unit statistics
 *
 * @details
 *  The structure is used to hold the statistics of the DPU
 *
 *  \ingroup DPU_RANGEPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeProc_stats_t
{
    /*! @brief total processing time during all chirps in a frame excluding EDMA waiting time*/
    uint32_t            processingTime;

    /*! @brief total wait time for EDMA data transfer during all chirps in a frame*/
    uint32_t            waitTime;
}DPU_RangeProc_stats;

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
);

int32_t DPU_RangeProcHWA_config
(
    DPU_RangeProcHWA_Handle     handle,
    DPU_RangeProcHWA_Config*    rangeHwaCfg
);

int32_t DPU_RangeProcHWA_process
(
    DPU_RangeProcHWA_Handle     handle,
    DPU_RangeProcHWA_OutParams* outParams
);

int32_t DPU_RangeProcHWA_control
(
    DPU_RangeProcHWA_Handle handle,
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
