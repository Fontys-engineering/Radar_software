/**
 *   @file  dopplerprocDDMAcommon.h
 *
 *   @brief
 *      Implements Common definition across dopplerProc DPU.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DOPPLERPROC_COMMON_H
#define DOPPLERPROC_COMMON_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>
#include <ti/datapath/dpedma/dpedma.h>

/** @mainpage Doppler DDMA DPU
 *
 * This DPU implements the Doppler DDMA DPU, which performs 2D FFT, DDMA Demodulation,
 * Azimuth FFT, Doppler CFAR and object list extraction.
 *
 *  @section dopplerDDM_intro_section Introduction
 *
 *  Description
 *  ----------------
 *
 *  This DPU expects as input the compressed radar cube with 1D FFT data.
 *
 *  The Doppler DPU is available in HWA implementations:
 *
 *   Device          |  DPU runs on core | Uses HWA
 *  :----------------|:------------------|:--------
 *   awr294x         |  DSP              | Yes
 *   awr2x44p        | DSS_CM4      | Yes
 *\n
 *
 *  @section dopplerDDM_dpu1 DopplerProcHWA
 *
 * A list of resources required by this DPU is described in @ref DPU_DopplerProcHWA_HW_Resources_t.
 * In particular, the number of EDMA channels required is constant and does not depend on the DPU configuration.
 * On the other hand, the number of required HWA paramsets depends on the compression method and sumTX enabled/disabled in
 * the data path as described in @ref DPU_DopplerProcHWA_HwaCfg_t.
 *
 *
 * Besides the resources described above, other parameters required for the DPU configuration are listed
 * in @ref DPU_DopplerProcHWA_StaticConfig_t.
 *
 * The Doppler DDMA DPU performs the bulk of the remaining processing.
 *  This DPU has three stages- the Decompression stage, Doppler stage and Azim stage.
 * It requires both HWA and DSP/DSS_CM4 usage, and has 3 major stages:
 * -# Decompression stage: HWA performs decompression of a single outer block in a ping-pong
 *    fashion. No DSP/DSS_CM4 intervention is required for decompression of one outer block, after which
 *    it is processed by the next two DPU stages. After one outer block is processed by all the DPU stages, the DSP/DSS_CM4 triggers the
 *    decompression of the next outer block again. If the range bins per compressed block are less than 8, HWA performs decompression of merged number of outer blocks in a ping-pong fashion.
 * -# Doppler Stage: HWA performs Doppler FFT, log-mag summation, DDMA Metric calculation, max subband computation and range-doppler heatmap generation, while the DSP/DSS_CM4 programs the shuffle LUT to perform DDMA Demodulation on HWA.
 * -# Azim Stage: HWA performs re-arrangement/ demodulation, Azimuth FFT, Doppler CFAR-OS and Local Max calculation, while the DSP/ DSS_CM4 performs the list extraction.
 *
 * The figure below shows a high level overview of the DPU. In the shown diagram, numRangeBinsPerBlock is forced to 8 when actual number of range bins per compressed block are less than 8.
 * This is done to decompress a group of outer blocks together in case of lesser number of range bins per block, thus reducing the number of for loops for decompression and doppler-azimuth stage.
 *
 * @image html ddmaDpu.png "Doppler DDMA DPU at a high-level " width=50%
 *
 * The demodulation scheme that is used is empty subband based DDMA. The number of Doppler-bins (defined as the numDopplerBins/numTotalSubbands) within a sub-band ideally should
 *  be an integer. This puts constraints on the Doppler FFT size and the number of sub-bands that need to be employed.
 *
 *  In AWR2944 we have 4 TX channels and given the constraints of FFT size on the HWA (2^N or 3*2^N)
 *  we need to employ 2 empty sub-bands in AWR2944 thus making the total number of sub-bands equal
 *  to 6 (4-TX channels and 2 empty bands). For the case of 6 sub-bands, a Doppler FFT size of 3*2^N
 *  is used to satisfy this requirement. In the case of AWR2943, however, we have only 3 Tx antennas.
 *  Hence, we can do with a single empty subband and a Doppler FFT size of 2^N.
 *
 * **Decompression**\n
 * Decompression of the data is performed using the decompression engine of the HWA. Compressed data for a merged outer block is copied from L3 memory to the HWA memory.
 * This decompression is performed for merged number of outer blocks (@ref dopplerProcHWADDMADecompressionCfg::mergedNumOuterBlocks) in a ping-pong fashion, where the number of chirps to decompress per ping/pong
 * is calculated (based on the available HWA memory). Here, mergedNumOuterBlocks is a group of (8/rangeBinsPerBlock) number of outer blocks, if rangeBinsPerBlock < 8, else mergedNumOuterBlocks = 1 single outer block.
 * This decompression of mergedNumOuterBlocks at a time is done because for the cases when range bins per block is less than 8,
 * the size of one outer block would be much smaller than available HWA memory and the number of outer blocks (= numRangeBins/ rangeBinsPerBlock) would be
 * more. Thus, processor will need to reconfigure HWA common registers etc. for decompression
 * and doppler processing for numOuterBlocks times increasing the processing latency.
 *
 * Example, let the number of chirps is 96 and rangeBinsPerBlock is 2, the size of
 * a single outer block will be 96*(sizeOfCompressedBlock) in case of EGE and 96*(sizeOfCompressedBlock)*RxAnt in case of BFP.
 * mergedNumOuterBlocks = 8/rangeBinsPerBlock = 4.
 * We will calculate numChirpsToDecompressPerPing such that
 * numChirpsToDecompressPerPing *(sizeOfCompressedBlock) * mergedNumOuterBlocks < availableHWAMemBankMemory and will run the ping-pong loops
 * 96/numChirpsToDecompressPerPing times. The decompressed outer block is placed in a scratch L3 RAM and is used as the
 * input to the Doppler-FFT and subsequent processing steps. This implies that after decompression stage, next stages of doppler DPU
 * can process at least 8 decompressed range bins even when the range bins per compressed block are less than 8.
 *
 * \n
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  Decompression Paramset
 *  Trigger            |  Decompression EDMA In Signature channel transfer completion
 *  Input              |  numChirpsToDecompressPerPing blocks of compressed radar cube corresponding to the same range bins and rx antennas
 *  Input MemBank      |  M0/M2
 *  Output             |  Decompressed partial radar cube of numRangePerBlock * numRxAntennas * numChirpsToDecompressPerPing * mergedNumOuterBlocks size
 *  Output MemBank     |  M4/M6
 *  On Completion      |  Triggers EDMA Out transfer of partially decompressed radar cube to L3.
 *
 *
 * Associated EDMA transfers:
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  EDMA Param         |  Transfer of compressed radar cube (partial) from L3 to HWA Mem
 *  EDMA Source Mem    |  Compressed Radar Cube (L3)
 *  EDMA Source Fmt    |  [mergedNumOuterBlocks][numChirpsToDecompressPerPing][block]
 *  EDMA Dest Mem      |  M0/M2
 *  EDMA Dest Fmt      |  [mergedNumOuterBlocks][numChirpsToDecompressPerPing][block]
 *
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  EDMA Param         |  Transfer of decompressed radar cube (partial) from HWA Mem to L2
 *  EDMA Source Mem    |  M4/M6
 *  EDMA Source Fmt    |  [mergedNumOuterBlocks][numChirpsToDecompressPerPing][rangeBinsPerBlock][rxAntennas]
 *  EDMA Dest Mem      |  Decompressed Radar Cube Scratch Buf (L2)
 *  EDMA Dest Fmt      |  [mergedNumOuterBlocks][numChirpsToDecompressPerPing][rangeBinsPerBlock][rxAntennas]
 *
 * EDMA transfer happens one range bin at a time for ping, and similarly for pong. Two range bins (one ping, one pong)
 * after transfer are processed in one loop run of this stage (2D FFT + DDMA Demod + Azim FFT + Doppler CFAR).

 * **Windowing and Doppler FFT**\n
 * The number of Doppler-bins within a sub-band should be an integer. This puts constraints on the Doppler FFT
 * size and the number of sub-bands that need to be employed.
 * In AWR2944 we have 4 TX channels and given the constraints of FFT size on the HWA (2^N or 3*2^N)
 * we need to employ 2 empty sub-bands in AWR2944 thus making the total number of sub-bands equal
 * to 6 (4-TX channels and 2 empty bands). For the case of 6 sub-bands, a Doppler FFT size of 3*2^N
 * is used to satisfy this requirement. Before FFT operation, input samples are multiplied by a window function. Window size and coefficients are
 * defined in @ref DPU_DopplerProcHWA_HwaCfg_t.
 * Window coefficients must be provided by application.\n\n
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  Doppler FFT Paramset
 *  Trigger            |  Doppler FFT EDMA In Signature channel transfer completion
 *  Input              |  1D FFT of a single decompressed range gate
 *  Input MemBank      |  M0-M1/M2-M3
 *  Output             |  2D FFT of single range gate
 *  Output MemBank     |  M4-M5/M6-M7
 *  On Completion      |  -
 *
 * \n\n
 *
 * **LogAbs + SumRx**\n
 * Log2 of the absolute value of each sample of type cmplx32ImRe_t is computed.
 * Sum of the log2 values of all receive channels is done using the HWA statistics block configured to obtain the sumRx output.
 *
 * Note that this step is not combined with the previous step itself (even though HWA does offer capability for the same)
 * because we need the pre-logAbs value of the 2D FFT in the subsequent stages.
 *
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Trigger            |  Doppler FFT Paramset completion
 *  Input              |  2D FFT of a single decompressed range gate
 *  Input MemBank      |  M4-M5/M6-M7
 *  Output             |  RX sum of LogAbs of 2D FFT
 *  Output MemBank     |  M1/M3
 *  On Completion      |  -
 *
 * \n\n
 *
 * **A note on empty subband based demodulation**\n
 * Demodulation consists of associating each sub-band with the correct TX channel. We employ an empty-band
 * DDMA based demodulation where the TX channels are modulated such that an empty sub-band exists whose location allows the determination
 * of the correct circular shift for demodulation.
 * An algorithm for finding the empty sub-bands utilizing the resources available in the HWA is described in the box below:\n
 * @image html emptySubBand.jpg "Finding the empty subband" width=50%
 *
 *
 * **DDMA Metric**\n
 * The DDMA Metric HWA paramset would perform a sequence of moving summation operations to compute an energy
 * level [Z1, Z2, ..,  Z6] for all the possible hypothesis (the number of hypothesis is equivalent to the number of
 * sub-bands, for example, 6). \n
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  DDMA Metric Paramset
 *  Trigger            |  SumRx Paramset completion
 *  Input              |  SumRx Output
 *  Input MemBank      |  M1/M3
 *  Output             |  Energy level of all possible hypotheses
 *  Output MemBank     |  M0/M2
 *  On Completion      |  -
 * \n\n
 *
 *  **Max Subband** \n
 * The Max Subband HWA paramset finds the maximum value hypothesis i.e. Max [Z1, Z2, Z3, Z4, Z5, Z6] from which the empty sub-band indices can be identified. \n
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  Max Subband Paramset
 *  Trigger            |  DDMA Metric Paramset completion
 *  Input              |  DDMA Metric Output
 *  Input MemBank      |  M0/M2
 *  Output             |  Max suband indices
 *  Output MemBank     |  M1 + offset / M3 + offset
 *  On Completion      |  Triggers EDMA transfer of Max subband (current paramset) output if sumTx is disabled
 * \n
 * Associated EDMA transfers (This EDMA is triggered here if SumTx is disabled. If Sum TX is enabled, then EDMA Out here may result in Mem Access Error - as EDMA and HWA will try to access the same Membank (M1/M3). In such cases, this EDMA Out is chained to sumTXOut):
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  EDMA Param         |  Transfer of Max Subband from HWA Mem to L2
 *  EDMA Source Mem    |  M1 + offset / M3 + offset
 *  EDMA Source Fmt    |  [dopSubBins]
 *  EDMA Dest Mem      |  DDMA Max Subband Buf Ping/Pong (L2)
 *  EDMA Dest Fmt      |  [dopSubBins]
 *
 * **SumTx**\n
 * Summation of the output of SumRx paramset is further summed over all the Tx antennas.
 * The sum is done using FFT in HWA, the sum is obtained in the DC (0th) bin.
 * The output of this paramset would form the detection matrix. \n\n
 *
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  SumTx Paramset
 *  Trigger            |  DDMA Metric / Max Subband Paramset completion
 *  Input              |  SumRx Output
 *  Input MemBank      |  M1/M3
 *  Output             |  Range/Doppler (Detection) Matrix
 *  Output MemBank     |  M0 + offset / M2 + offset
 *  On Completion      |  Triggers EDMA transfer of Sum Tx output, which is chained to the EDMA transfer of Max Subband output
 * \n
 * Associated EDMA transfers:
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  EDMA Param         |  Transfer of SumTx data from HWA Mem to L3
 *  EDMA Source Mem    |  M0 + offset / M2 + offset
 *  EDMA Source Fmt    |  [rangeBins][dopSubBins]
 *  EDMA Dest Mem      |  Detection Matrix (L3)
 *  EDMA Dest Fmt      |  [rangeBins][dopSubBins]
 * \n\n
 *
 * **DDMA Demodulation**\n
 * A high-level picture of the demodulation scheme is detailed below. From the DDMA metric hypothesis, indices of max subband is computed. These max subband are
 * programmed in shuffled LUT by processor. Using the shuffle LUT values, demodulation HWA paramset picks the relevant antenna
 * samples and discard the empty bins, thus providing the virtual antenna array for doppler subbins of each range bin. On this
 * antenna array, then azimuth-CFAR stage is executed to detect the objects.
 *
 * A high level picture of the demodulation scheme is detailed below-
 * @image html ddmaDemodHWA.png "DDM MIMO Demodulation on HWA"
 *
 * The following image shows how the transfer of Doppler FFT takes place from the HWA dopFFTOut MemBank to the HWA
 * DemodulationIn MemBank. This EDMA transfer is SW triggered only after the completion of dopFFT stage, so that destination
 * membank are available.
 *
 * Consider a K-point Doppler FFT being performed. In actuality, as many K-point FFTs are being performed for
 * a single range gate as there are number of receiver antennae. The output of the HWA doppler FFT paramset is
 * stored in the HWA Mem Bank as shown, where the bins corresponding to each band are
 * together, with the second dimension being the bins, followed by the Rx antennas. In the case of 4 Tx antennae, the
 * number of bands for performing the empty subband based DDMA is taken to be 6. Hence the K bins forming the output
 * of the Doppler FFT can be divided into six bands having K/6 samples each. Samples corresponding to Bins nK/6 + d,
 * where d is a constant between 1 and K/6 (both inclusive) and n = 0, 1, 2, 3, 4, 5 would refer to the same object,
 * since we have divided the spectrum into 6 bands, out of which, for every object, in the corresponding range bin,
 * 4 bands would have peaks corresponding to the 4 Tx antennas and 2 bands would be empty. We call d here as the Doppler Sub Bin.
 * For example, for a fixed range bin x, samples corresponding to Doppler Sub Bin 1 would be Bins 1, K/6+1, 2K/6+1, 3K/6+1,
 * 4K/6+1 and 5K/6+1. If an object existed at this range bin x with a velocity corresponding to Doppler Sub Bin 1, we would
 * see peaks in 4 of the 6 aforementioned bins, and two would be empty. The bands which would actually
 * have the peaks depends on the velocity of the object. The 4 bins corresponding to an object that actually have the peak
 * values are relevant to us and the two samples corresponding to the bins in the empty subbands can be discarded.
 * The location of the empty subband would help us in the DDMA demodulation, i.e., the process of mapping the samples
 * corresponding to the 4 peak bins to the corresponding Tx antennas.
 *
 * Let us take an example, where two object A and B are at the same range but different velocities. For example,
 * one might see peaks at Doppler Sub Bin 1 (Bin 1, Bin K/6+1, Bin 2K/6+1 and Bin 3K/6+1) for one object
 * (call it Object A), or peaks at Doppler Sub Bin K-1 (Bin K-1, Bin K/6-1, Bin 2K/6-1 and Bin 3K/6-1) for
 * another object (call it Object B). Note that here we consider both objects in the same range bin, and hence
 * we'll see peaks for both objects in the same Doppler FFT Paramset run. The peaks in the spectrum indicate
 * that bands 1, 2, 3 and 4 are active bands for Object A and bands 5 and 6 are empty subbands. Similarly,
 * bands 6, 1, 2 and 3 are active bands for Object B and bands 4 and 5 are empty subbands. By design (i.e., by
 * the phase shifter configuration), we say that when bands are taken in rotation after the last empty subband, the
 * first three bands corresponding to Azimuth Tx antennae (Tx 0, 2 and 3) and the fourth band corresponds to the
 * Elevation Tx antenna (Tx 1). Hence, we have successfully demodulated the samples for Objects A and B. For Object A,
 * samples of Bin 1, Bin K/6+1, Bin 2K/6+1 and Bin 3K/6+1 correspond to Tx 0, 2, 3 and 1 respectively, and for
 * Object B, samples of Bin K-1, Bin K/6-1, Bin 2K/6-1 and Bin 3K/6-1 correspond to Tx 0, 2, 3 and 1 respectively.
 * Thus, we can separate out the Azimuth and Elevation samples for their corresponding processing.
 *
 *
 * @image html DDMDemodulate_Input.png "HWA stores dopFFT output in the format [rxAntennas][numBands][dopSubBins]."
 *
 * **Rxi Demodulation Paramset (i=1,2,3,4)** \n
 * This paramset identifies the active band samples based on max subband location programmed in shuffle LUT, 
 * rotates the array to move active bands to front of array and stores only the active bands in [dopSubbins][Tx antenna][Rx antenna] format by interleaving. \n
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  Demodulation Paramset
 *  Trigger            |  Max subband (pong) / SumTx (pong) / Rx(i-1) Demodulation / Local Max (ping) completion
 *  Input              |  Doppler FFT Output
 *  Input MemBank      |  M4-M5/M6-M7
 *  Output             |  Active bands Array
 *  Output MemBank     |  M0/M2
 *  On Completion      |  Triggers Rx(i+1) Demodulation / Azimuth FFT paramset
 * \n
 *
 * **Azimuth FFT**\n
 * An FFT is performed on the Azimuth samples from the output HWA membank of Demodulation paramsets. 
 * Before computing azimuth FFT, this paramset does the following operations as well:
 * - Rearrangement of virtual azimuth antenna array samples based on antenna pattern programmed in the shuffle LUT.
 * - Antenna Calibration by multiplication of azimuth samples with the antenna calibration parameters/coefficients.
 * - Zero insertion in the virtual antenna array based on the programmed zero insertion mask.
 *
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  Azimuth FFT Paramset
 *  Trigger            |  Rx4 Demodulation Paramset Completion
 *  Input              |  Demodulation Output
 *  Input MemBank      |  M0/M2
 *  Output             |  Azimuth FFT Data
 *  Output MemBank     |  M4/M6
 *  On Completion      |  Triggers CFAR (Doppler) paramset
 * \n
 *
 * **CFAR (Doppler)**\n
 * CFAR-OS is performed in the Doppler direction, on the Azimuth FFT samples. The maximum number of
 * peaks that the paramset will store is provided by the application. The output of this DPU is represented
 * as a (32 bit Real, 32 bit Imaginary) complex number where the real part of each sample is used to calculate the
 * peak index and the imaginary part indicates the noise level. \n
 * The threshold can be fixed for all range gates or be changed for each range gate, where the difference is provided 
 * by a function / LUT @ref DPU_DopplerProcHWA_log2CfarThreshScale. This helps to fine tune the thresholds to model the
 * SNR difference observed due to varying range of objects. The threshold for last range bin is fixed to be same as
 * the CLI input while that for other range gates is adjusted accordingly. \n\n
 *
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  Doppler CFAR Paramset
 *  Trigger            |  Azimuth FFT paramset completion
 *  Input              |  Azimuth FFT output
 *  Input MemBank      |  M4/M6
 *  Output             |  Doppler CFAR-OS data
 *  Output MemBank     |  M5/M7
 *  On Completion      |  Generate CPU interrupt (only for PING) and trigger Local Max paramset
 * \n
 *
 * **Variable CFAR Thresholds (Range-Dependent)**\n
 * The DPU supports range-dependent CFAR thresholding to improve detection performance across different ranges.
 * When enabled via @ref DPU_DopplerProc_CfarCfg::variableThresholdMode, the CFAR threshold is dynamically adjusted
 * on a per-range-bin basis during processing. This feature addresses the practical challenge that near-field
 * targets typically have higher SNR and are subject to more clutter, while far-field targets require maximum
 * sensitivity.\n\n
 *
 * The algorithm implements a two-region threshold profile:
 * - **Near-field region** (first 25% of range bins): Constant elevated threshold of +7dB above baseline
 * - **Mid-to-far field region** (remaining 75% of range bins): Linear taper from baseline+7dB down to baseline
 *
 * @image html Variable_CFAR_Thresholds.png "Range dependent CFAR Thresholds curve" width=60%
 * 
 * The threshold scaling values are pre-computed during configuration by @ref DPU_DopplerProcHWA_configCfarThreshScaleLUT()
 * and stored in the @ref DPU_DopplerProcHWA_HW_Resources_t::cfarThreshScaleLUT array. During processing, the HWA
 * CFAR threshold is updated for each range bin using @ref DPU_DopplerProcHWA_getRangeDependentCfarThreshold() which
 * computes and returns the final threshold value (baseline + range-dependent offset).\n\n
 *
 * The actual threshold applied to range bin i is computed internally as:
 * @verbatim
 *   threshold[i] = thresholdScale + cfarThreshScaleLUT[i]
 * @endverbatim
 *
 * where:
 * - thresholdScale is the baseline threshold configured in @ref DPU_DopplerProc_CfarCfg::thresholdScale
 *   (should be set for far-field sensitivity requirements)
 * - cfarThreshScaleLUT[i] is the range-dependent offset for range bin i (in log2 5.11 fixed-point format)
 *
 * The threshold scaling formula used in the LUT computation is:
 * @verbatim
 *   For i < 0.25 * numRangeBins:
 *       cfarThreshScaleLUT[i] = (7 dB / 20) * log2(10) * 2048
 *
 *   For i >= 0.25 * numRangeBins:
 *       linearRatio = (1 - (i+1)/numRangeBins) / (1 - 0.25)
 *       cfarThreshScaleLUT[i] = linearRatio * (7 dB / 20) * log2(10) * 2048
 * @endverbatim
 *
 * Note: The constants (25% region boundary and 7dB offset) are defined in @ref DPU_DopplerProcHWA_configCfarThreshScaleLUT()
 * and can be modified if different threshold profiles are desired.\n\n
 * \n
 *
 * **Local Max**\n
 * A local maxima is performed on the Azimuth FFT samples in both azimuth and doppler dimension. Row-wise as well
 * as column-wise comparison is performed, and "0 1 0 1 0 1 0 1", i.e., a "+" shaped comparison is utilized.
 * The output bit pattern is stored in the destination memory packed as 32-bit words,
 * with the LSB bit corresponding to column count of 0. \n\n
 *
 *   Parameter         |  Comment
 *  :------------------|:--------------
 *  Paramset           |  Local Max Paramset
 *  Trigger            |  Doppler CFAR-OS paramset completion
 *  Input              |  Azimuth FFT output
 *  Input MemBank      |  M4/M6
 *  Output             |  Doppler Local Max data
 *  Output MemBank     |  M1/M3
 *  On Completion      |  Generate CPU interrupt
 * \n
 *
 * **Extract Object List**\n
 * This step is performed in the DSP/DSS_CM4 with the input matrices (doppler FFT, azimuth FFT, cfar, local max outputs) retained in the HWA membanks.
 * A loop is run over all the detected CFAR peaks. If the peak position is also detected by the Local Max paramset, the following information is stored for the particular peak:
 * azimuth bin index, doppler index, CFAR noise, doppler FFT samples corresponding to the particular range/doppler indices, azimuth FFT output corresponding to the sample 
 * and the samples previous to and after the corresponding sample.
 *
 *
 * **Note** - It is observed that in some real-time scenarios more objects are generated than the limited available memory can store or device can process. This means that
 * most of the detected objects space will be consumed for objects that are close to the radar leaving no space for objects that are far away. This problem can be resolved by
 * enabling the macro @ref LIMIT_DETECTED_OBJS_PER_RANGEBIN, which limits the number of objects per range bin so that
 * objects further out can also be detected. Out of all the detected objects per range bin, only a limited number of objects having the highest SNR are stored and rest are discarded.
 * This is controlled by the space available per range gate @ref DPU_DopplerProcHWA_HW_Resources_t::maxObjListPerRGateSize which is populated by DPC, the number of cumulative objects
 * stored upto current range gate and a set limit @ref MAX_NUM_OBJ_PER_RANGE_BIN, which can be independently increased. 
 * 
 *
 * @subsection histogram Use of HWA's Histogram feature for detection
 *
 * This feature allows use of histogram for detection on doppler-azimuth heatmap and disables doppler CFAR operation for better estimation of noise floor.
 * SW does the following when the feature is enabled using the compile-time flag (macro) ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION.
 *
 * - HWA computes azimuth FFT size number of histograms per doppler-azimuth heatmap. Histogram is enabled in azimuth FFT paramset in HWA's mode 3 (HWA_HISTOGRAM_MODE_CDF_THRESHOLD).
 * - Size and scale of the histogram can be modified by changing the macro HIST_SIZE_SELECT and HIST_SCALE_SELECT respectively. These parameters are configured as HIST_SIZE_SELECT = 6 (64 bins - max possible) and HIST_SCALE_SELECT = 10 (each bin size of 1024) by default.
 * We would want max limit to go upto 2^16 ( = 65536) because of azim FFT 16-bit log-abs output. Thus, scale = 2^16/hist size = 1024.
 * - Processor waits for the completion of azimuth FFT paramset, and reads the CDF bin number which crossed the CDF_CNT_THRES (90% of doppler Subbins), populated by HWA.
 * - For better estimate of noise floor, we do linear interpolation of the CDF bin number based on the histogram value of the current and next bin.
 * - Processor programs the interpolated CDF bin value after scaling by bin size into the LOCAL MAX DIMB MAX VALUE RAM (CSL_CM4_DSS_HWA_2DSTAT_SMPL_VAL_RAM_U_BASE).
 * - Local Max Paramset is configured to be used in mode 3, that is to use the max value RAMs for threshold in both the dimensions.
 * - Max2Doffset for Dim2 is configured as HIST_DOPPLER_DIM_OFFSET_DB (10dB), scaled to log2 5.11 format.
 * - Thus, during local Max paramset run, sample is compared with the [max in azimuth dim - offset], and [cdf based doppler threshold + 10dB] for detection.
 *
 *
 * @subsection apis Exported APIs
 * DPU initialization is done through @ref DPU_DopplerProcHWA_init. \n
 * DPU configuration is done by @ref DPU_DopplerProcHWA_config. The configuration can only be done after
 * the DPU has been initialized. The configuration parameters are described in @ref DPU_DopplerProcHWA_Config. \n
 * The DPU is executed by calling @ref DPU_DopplerProcHWA_process. This function will control the entire processing of the Doppler proc DDMA DPU. It will loop over the
 * range bins, configure HWA common registers for each stage, trigger EDMA transfers and poll on EDMA
 * transfer completions.
 *
 * @subsection interrupts Use of polls vs interrupts for EDMA/HWA completion - trade-off between timing and DSP load
 * There are around 7 places in doppler DDMA DPU where EDMA/HWA completion is monitored before triggering the next operation - optimization here becomes significant. \n
 * Use of EDMA/HWA polls implies waiting for the completion of EDMA transfer in a while loop. Polling increases the DSP load ~ 80% (very
 * high). This hogs the DSP, limiting its scope of usage for further processing applications on top of OOB, like neural
 * network for classification, and other complex signal processing algorithms. \n
 * Using interrupts instead of polling as an alternate way to monitor the EDMA/HWA completion reduces the DSP utilization to ~ 23% for the
 * standard AWR2944 high range profile cfg (with 384 samples and 768 chirps). Thus, DSP becomes free enough to do some other computations but, some time is
 * wasted in context save and restore. \n
 *
 * Both the options are provided to the user via compile-time flag, to choose as per their target application. By default, interrupts are enabled. 
 * User can enable the interrupts by enabling (uncomment) the compile-time flag DOPPLERPROCHWADDMA_INTERRUPTS in
 * dopplerprochwaDDMAinternal.h. Note that, code is written such that only either interrupts or polls can be used at a time.
 *
 * @section dopDPU Detailed Block Diagram, Description and Timing
 *
 * @subsection dcmpStage Decompression Stage
 * The radar cube is compressed along the range bin and rx antenna dimension for every chirp. Suppose x range bins
 * and y rx antennas are compressed together, it results in a block. The blocks taken for these x range bins and y rx
 * antennas for every chirp is what is termed as an outer block. Thus, the decompression of an outer block would
 * result in x range bin worth radar cube data being decompressed for all chirps and rx antennas.
 * To utilize the ping-pong mechanism efficiently and to meet the HWA Mem Bank Size requirements, an outer block
 * which contains blocks worth all chirps is divided into smaller macro-blocks each of which contain blocks worth
 * numChirps To DecompressPerPing chirps. In the decompression stage, numChirps To DecompressPerPing chirps are
 * decompressed in a ping-pong manner, with the total number of loops to run for a single outer block being numChirps
 * Total/(num Chirps To DecompressPerPing*2), the multiplication factor of 2 being due to the fact that 2*numChirps
 * To DecompressPerPing will be processed in a single decompression HWA Loop due to the ping/pong mechanism.
 * After the complete decompression of a single outer block, the second stage of the Doppler DPU is triggered. After
 * the second stage of the Doppler DPU processes the entire decompressed outer block, the next outer block is
 * decompressed and this carries out for all outer blocks.
 *
 * @image html dopplerDPU.jpg "Doppler DDMA DPU implementation using HWA" width=150%
 *
 * @subsection dopDSP Doppler Stage
 * The decompression of a single outer block results in decompressed data worth numRange Bins PerBlock range bins.
 * Each of the range bins of a single outer block are processed by this second stage of the Doppler DPU, two range
 * bins at a time (ping/pong mechanism).
 * Data worth each range bin needs to be processed in 4 stages -
 * (i) The 2D FFT, DDMA Metric, and Max subband computation stage performed in HWA,
 * (ii) Configuration of Shuffle LUT by processor based on max indices for DDMA demodulation (to pick the active bands from doppler FFT output),
 * (iii) The Azimuth-Doppler CFAR stage performed in the HWA, and
 * (iv) Object List Extraction performed in the processor.
 * Each stage needs its input from the output of the previous stage. Thus, two range bins are operated upon in a
 * single loop, in a pipelined fashion such that the processor and HWA have the most efficient utilization.
 * A loop runs for num Range Bins PerBlock/2 range bins, wherein each loop run processes two range bins. While stage (i) of range bin
 * x is executed in the HWA, stage (iv) of range bin x-1 is executed on the processor. When the output of stage (i) of x is available and
 * stage (iv) of x-1 is complete, stage (ii) of x is performed on the processor and stage (i) of x+1 on the HWA. Then, stage (iii) of x is
 * executed on the HWA while stage (ii) of x+1 executes on the processor. Finally, stage (iv) of x is processed on the processor while processing
 * on the HWA, stage (iii) of x+1. Stage (iv) of x+1 is carried out while stage (i) of x+2 is executed in the next loop.
 *
 * This can be seen in the timing diagram below-
 *
 * @image html timing.jpg "Doppler DDMA DPU timing (stage II)" width=80%
 *
 * \n\n\n
 *
 */



/**
@defgroup DPU_DOPPLERPROC_EXTERNAL_FUNCTION            dopplerProc DPU External Functions
@ingroup DOPPLER_PROC_DPU
@brief
*   The section has a list of all the exported API which the applications need to
*   invoke in order to use the dopplerProc DPU
*/
/**
@defgroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE      dopplerProc DPU External Data Structures
@ingroup DOPPLER_PROC_DPU
@brief
*   The section has a list of all the data structures which are exposed to the application
*/
/**
@defgroup DPU_DOPPLERPROC_ERROR_CODE                   dopplerProc DPU Error Codes
@ingroup DOPPLER_PROC_DPU
@brief
*   The section has a list of all the error codes which are generated by the dopplerProc DPU
*/
/**
@defgroup DPU_DOPPLERPROC_INTERNAL_FUNCTION            dopplerProc DPU Internal Functions
@ingroup DOPPLER_PROC_DPU
@brief
*   The section has a list of all internal API which are not exposed to the external
*   applications.
*/
/**
@defgroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE      dopplerProc DPU Internal Data Structures
@ingroup DOPPLER_PROC_DPU
@brief
*   The section has a list of all internal data structures which are used internally
*   by the dopplerProc DPU module.
*/
/**
@defgroup DPU_DOPPLERPROC_INTERNAL_DEFINITION          dopplerProc DPU Internal Definitions
@ingroup DOPPLER_PROC_DPU
@brief
*   The section has a list of all internal definitions which are used internally
*   by the dopplerProc DPU.
*/



#ifdef __cplusplus
extern "C" {
#endif

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
typedef struct DPU_DopplerProc_Edma_t
{
    /*! @brief  EDMA Ping/Pong channel. */
    DPEDMA_ChanCfg  pingPong[2];
}DPU_DopplerProc_Edma;

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
typedef struct DPU_DopplerProc_EdmaIntrObj_t
{
    /*! @brief  EDMA Ping/Pong channel. */
    Edma_IntrObject  *pingPong[2];
}DPU_DopplerProc_EdmaIntrObj;


/**
 * @brief
 *  dopplerProc DPU statistics
 *
 * @details
 *  The structure is used to hold the statistics of the DPU
 *
 *  \ingroup DPU_DOPPLERPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProc_Stats_t
{
    /*! @brief total number of DPU processing */
    uint32_t            numProcess;

    /*! @brief For HWA version of the DPU: total processing time including EDMA transfers.\n
               For DSP version of the DPU: total processing time excluding EDMA transfers.*/
    uint64_t            processingTime;

    /*! @brief time spent waiting for EDMA transfers. Valid only for DSP version of DPU.*/
    uint64_t            waitTime;
}DPU_DopplerProc_Stats;

#ifdef __cplusplus
}
#endif

#endif
