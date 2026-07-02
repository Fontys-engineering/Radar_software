/**
 *   @file  objectdetectioninternal.h
 *
 *   @brief
 *      Object Detection DPC Header File
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 - 2025 Texas Instruments, Inc.
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

/** @mainpage Object Detection Data-path DDMA Processing Chain (DPC)
 *
 *  @section objdetddmaintro Introduction - DDMA
 *  This section describes the basics of Doppler division multiple access (DDMA) MIMO method,
 *  including the classical DDMA method and the empty-band DDMA method. The empty-band DDMA method is
 *  used in the current implementation of the DDMA processing chain.
 *
 *  @subsection classicalDDMA Classical DDMA
 *  Doppler division multiple access (DDMA) is a MIMO strategy that allows simultaneous transmission of
 *  all TX channels with orthogonality in the Doppler spectrum. The figure below illustrates the working of DDMA modulation.
 *  In time domain, a unique phase is applied to each TX channel. This phase value is constant per-chirp (slow time) and
 *   increases as the chirp index increases. If there are N transmitters, each transmitter is modulated with
 *  phase \f$\omega_{k} = (2\pi)(k-1)/N_{t}\f$ , k is the chirp index. The example in figure below, has N_t=3.
 *  The orthogonality happens in the Doppler frequency domain as shown in the figure below. Therefore, after Doppler FFT,
 *  the signals from different TX channels are naturally separated. However, due to Doppler frequency multiplexing,
 *  the allowed maximum target velocity is reduced by a factor N_t, similar to time division Multiplexing (TDM) MIMO.
 *   It is possible to recover the maximum velocity using hypothesis testing with zero padding or phase dithering.
 *  This hypothesis test requires cyclic memory shifting and assumes single target in the same range/Doppler bin.
 *
 *  @image html classicalDDMA1.png "Classical DDMA in case of 3 tx antennas"
 *   \n
 *   \n
 *
 *  @subsection esbDDMA Empty-SubBand DDMA
 *  To overcome the maximum velocity limitation reliably without the single target assumption, this section introduces an
 *  empty sub-band based DDMA strategy, the corresponding demodulation and maximum velocity extension algorithm will be
 *  covered in the next sections. To introduce the empty Doppler sub-band, the phase added to each TX channel is modified
 *  \f$\omega_{k} = (2\pi)(k-1)/(N_{t}+1)\f$. As shown in the following figures with N_t=3 and a single empty-band,
 *  and with N_t=4 and two empty-bands.
 *
 *  @image html emptyBandDDMA1.png "Empty SubBand DDMA in case of 3 tx antennas and 1 empty band"
 *   \n
 *   \n
 *
 *  @image html emptyBandDDMA2.png "Empty SubBand DDMA in case of 4 tx antennas and 2 empty bands"
 *   \n
 *   \n
 *  The location of the empty Doppler sub-band D in the figure corresponding to Empty SubBand DDMA in case of 3 tx antennas
 *  and 1 empty band is used for maximum velocity extension as shown in the figure below.
 *  In this case with N_t=3 as an odd number, 0 indicates the empty Doppler sub-band location, with the corresponding
 *  velocity hypothesis indicated on the left of the figure. The differentiation between [3Vmax/4Vmax] and [-Vmax -3Vmax/4]
 *   depends on if the bin is on the left half Doppler spectrum or the right half Doppler spectrum.
 *
 *  @image html dopplerSpec1.png "Max Velocity Extension in case of 3 tx antennas"
 *   \n
 *   \n
 *
 *  As another example, the figure below shows the velocity extension with N_t=4 as an even number.
 *  It is more straightforward to recover the maximum velocity with odd number of sub-bands.
 *  @image html dopplerSpec2.png "Max Velocity Extension in case of 4 tx antennas and single empty subband"
 *   \n
 *   \n
 *
 *  @section objdetddmaintro2 Introduction - DPC
 *
 *  The Object detection DPC provides the functionality of processing ADC samples
 *  to detected objects during the frame acquisition and inter-frame processing
 *  periods. It can be used by the application by registering with the DPM
 *  framework and invoked using DPM APIs. The external interface of Object detection
 *  DPC can be seen at @ref DPC_OBJDET_EXTERNAL
 *
 *  The processing chain utilized is the empty sub-band based DDMA.  The number of Doppler-bins
 *  (defined as the numDopplerBins/numTotalSubbands) within a sub-band ideally should
 *  be an integer. This puts constraints on the Doppler FFT size and the number of sub-bands
 *  that need to be employed.
 *
 *  In AWR2944 we have 4 TX channels and given the constraints of FFT size on the HWA (2^N or 3*2^N)
 *  we need to employ 2 empty sub-bands in AWR2944 thus making the total number of sub-bands equal
 *  to 6 (4-TX channels and 2 empty bands). For the case of 6 sub-bands, a Doppler FFT size of 3*2^N
 *  is used to satisfy this requirement. In the case of AWR2943, however, we have only 3 Tx antennas.
 *  Hence, we can do with a single empty subband and a Doppler FFT size of 2^N.
 *
 *  The order in which phases are stored for the Txs is {AzimTx1, .., AzimTxN, ElevTx1, .., ElevTxM}
 *  where N and M are the number of azimuth and elevation Tx antennas respectively.
 *
 *  @section objdetddmahwa_datapath Data Path
 *   @image html object_detection_datapath.png "Object Detection Data Path Processing Chain"
 *   \n
 *   \n
 *   The data path processing consists of:
 *   - Processing during the chirps as seen in the timing diagram:
 *     - This consists of 1D (range) FFT processing that takes input from multiple
 *     receive antennae from the ADC buffer for every chirp (corresponding to the
 *     chirping pattern on the transmit antennae). It also includes DC Subtraction and interference
 *     mitigation on the samples before FFT is performed. The output is compressed using the EGE/BFP compression
 *     algorithm and stored L3 RAM. For more details, see the doxygen
 *     documentation of range processing DDMA DPU (Data Path Unit). Hence, the processing, at a high level consists of
 *     - DC estimation and subtraction on ADC Samples
 *     - Interference estimation and mitigation on ADC Samples
 *     - 1D FFT on ADC samples.
 *     - Compression of 1D FFT Output to give out a compressed radar cube.
 *
 *   - Processing during the time between the end of chirps until the beginning of the
 *     next chirping period, shown as "Inter frame Period" in the timing diagram.
 *     This processing consists of:
 *     - Part-by-part decompression of the radar cube. (Doppler DDMA DPU)
 *     - 2D (velocity) FFT processing that takes input from partly decompressed 1D output in L3 RAM and performs
 *       FFT. (Doppler DDMA DPU)
 *     - Magnitude and Summation of 2D FFT Output to obtain a (range, doppler) matrix (detection matrix). (Doppler DDMA DPU)
 *     - DDMA Demodulation which disambiguates the 2D FFT Output samples into virtual antenna bins. (Doppler DDMA DPU)
 *     - Azimuth FFT on demodulated 2D FFT Output (3rd Dimension FFT on the azimuth virtual antenna samples
 *       obtained after the DDMA Demodulation step). (Doppler DDMA DPU)
 *     - CFAR along the doppler dimension, followed by local max peak-finding along the doppler as well as
 *       azimuth dimensions. The two lists obtained (after the CFAR and local max steps) are then merged to
 *       get an intermediate object list. (Note: this step is performed on the Azimuth FFT Output). (Doppler DDMA DPU)
 *     - CFAR along the range dimension on the detection matrix obtained after the doppler DDMA DPU.
 *       An intermediate object list is obtained. (Range CFAR DDMA DPU)
 *     - The intermediate object lists obtained after the Doppler CFAR-Local Max steps and the Range CFAR
 *       steps are merged to obtain a final object list which contains objects that were present in both lists.
 *     - Elevation processing and further Angle of Arrival processing to produce a final list of
 *       detected objects with position coordinates (x,y,z) and velocity. (Object Detection- XYZ Estimation)
 *
 *  @section appdpcFlowDDMA Application-DPC Execution Flow
 *   Following diagram shows the application-DPC execution Flow.
 *
 *  @image html dpc_flow.png "Application-DPC Execution Flow"
 *
 *   The flow above shows the sequencing of initialization, configuration, execution and
 *   dynamic control operations of the DPC and some level of detail of
 *   what happens under these operations. Note that after the DDMA demo optimizations, range processing trigger is
 *   given before the AoA estimation (elevation measurement and XYZ estimation) to ensure parallel processing of
 *   current and next subframe/frame.
 *   Most of the hardware resource (e.g HWA, EDMA related) configuration
 *   for the DPUs that is issued by the DPC as part of processing
 *   @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG commands is provided by the
 *   application at build time using a resource file (DPC sources are built
 *   as part of building the application, there is no separate DPC library object).
 *   This file is passed as a compiler command line define
 *   @verbatim --define=APP_RESOURCE_FILE="fileName" @endverbatim The "fileName"
 *   above includes the path as if to include the file when building the the DPC sources
 *   as part of building the application, and any DPC source that needs to refer
 *   to this file (currently objectdetection.c) has the following code
 *    @verbatim #include APP_RESOURCE_FILE @endverbatim
 *   One of the demos that uses this DPC is located at ti/demo/awr294x. The
 *   resource file in this demo is mmw_resDDM.h, this file shows all the definitions
 *   that are needed by the DPC from the application. This file is provided
 *   on compiler command line when building as follows:
 *   @verbatim --define=APP_RESOURCE_FILE="<ti/demo/awr294x/mmw_resDDM.h>" @endverbatim
 *
 *   The application which does not include DPM library, invokes the DPC APIs directly from the application.
 *   And a DPC state machine runs at the application level to handle the DPC configuration, start and execution states. The details
 *   of the state machine can be found in the device demo doxygen.
 *
 *  @section memAlloc Memory Allocation
 *   The partitioning of L3, Core Local RAM (hitherto referred in short as L2RAM) and Fast RAM
 *   memories provided by the application (in @ref DPC_ObjectDetection_InitParams_t::L3ramCfg
 *   and @ref DPC_ObjectDetection_InitParams_t::CoreLocalRamCfg) during
 *   @ref DPC_ObjectDetection_init (invoked by application through @ref DPM_init)
 *   happens during the processing of @ref DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG command
 *   and is shown in the following figure.
 *
 *   @image html memory_allocation_core_local_ram.png "Data memory allocation: Core Local RAM"
 * \n\n
 *   @image html memory_allocation_l3.png "Data memory allocation: L3"
 *
 *   The details of the respective L2 memories are as follows:
 *     - Doppler FFT Submat: Stores demodulated Doppler FFT output of one ping and pong range bin
 *       each. Needed by @ref DPU_DopplerProcHWA_extractObjectList function, and populated by Demodulation HWA paramset.
 *     - Max Subband Buf: Stores the band index which has the highest DDMA metric, for each of the
 *       doppler sub-bins, corresponding to one ping and pong range bin each. Needed by
 *       @ref DPU_DopplerProcHWA_extractObjectList function, and populated by Max Subband HWa paramset.
 *     - Azimuth FFT Out: Stores Azimuth FFT output of one ping and pong range bin
 *       each. Needed by @ref DPU_DopplerProcHWA_extractObjectList function.
 *     - CFAR Out: Stores doppler CFAR output (peak index and noise information) of one ping and pong range bin
 *       each. Needed by @ref DPU_DopplerProcHWA_extractObjectList function.
 *     - Local Max Out: Stores Local Max output of one ping and pong range bin
 *       each. Needed by @ref DPU_DopplerProcHWA_extractObjectList function.
 *     - Range CFAR Scratch Buf: Stores the output of the CFAR HWA paramsets (peak index and noise information).
 *       Needed by @ref dpuRangeCfarProcDDMA_createRangeCfarList function.
 *     - Final Detected Objects List: Stores the intermediate object list (azimuth Samples, elevation samples, doppler Index, range Index, etc).
 *       This is created after the intersection with range cfar detected objects on which AoA processing happens.
 *       This list is required to allow execution of AoA in parallel with range proc.
 *
 *   L3 Memory is a Bi-directional memory pool where the runtime objects are allocated from the top which are transient
 *   (are reset and repopulated during processing), and the objects allocated from the bottom which are persistent (their data 
 *   needs to be preserved throughout the running of the application). The details of the respective L3 memories are as follows:
 *     - Radar Cube (compressed): Stores the compressed output of the radar cube.
 *     - Decompressed Radar Cube Scratch Buf: Contains decompressed data corresponding to one outer-block
 *       (with numSamplesPerBlock = numRangeBinsPerBlock * numRxAntennas * numChirps) of the compressed radar cube.
 *     - Detection Matrix: Stores the Doppler-Range Heatmap. It is computed by the Doppler DPU and used by the Range CFAR DPU.
 *     - Object List: Stores the intermediate object list after the Doppler DDMA DPU. It is overwritten over the radar cube allowing 
 *       to store upto ((Radar Cube Size) / (numRangeBins * (size of @ref DetObjParams))) objects per Range bin (size of @ref DetObjParams is 160 bytes by default), which can be further reduced by changing the 
 *       @ref MAX_NUM_OBJ_PER_RANGE_BIN macro. It is computed by the @ref DPU_DopplerProcHWA_extractObjectList function.
 *     - Range CFAR List: Stores the intermediate object list after the Range CFAR DDMA DPU. It is computed by the
 *       @ref dpuRangeCfarProcDDMA_createRangeCfarList function.
 *     - Range CFAR Num Obj Per Doppler Bin Buf: Stores the cumulative sum of the number of objects found after the Range CFAR step,
 *       for every doppler bin. It is used for faster merger of Range CFAR and the Doppler CFAR-Local Max lists. It is used in the
 *       @ref DPC_ObjDet_estimateXYZ function.
 *     - Point CLoudt Buf: List of object coordinates and velocity. It is computed by the @ref DPC_ObjDet_estimateXYZ function.
 *     - Side Info Buf: List of side info (SNR and noise). It is computed by the @ref DPC_ObjDet_estimateXYZ function.
 *     - Window RAM buffer: The window RAM for Range FFT and Doppler FFT are calculated and stored here for all the subframes.
 *       This is a persistent buffer and hence is allocated from the bottom of memory pool.
 * 
 *    Fast RAM Buffer is also a Bi-directional memory pool. The details of the allocations inside it are:
 *     - This buffer is used in below rangeProcChain modes as persistent data as these need to be used across frames:
 *          1. DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE
 *             Size is 1(DC Estimation) * RL_MAX_SUBFRAMES (numSubframes) * 4 (numRx) * sizeof(uint32_t) (numBytesPerSample)
 *          2. DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE
 *             Size is (1(DC Estimation) + 2(Interference Statistics)) * RL_MAX_SUBFRAMES (numSubframes) 
 *                          * 6 (numBandsTotal) * 4 (numRx) * sizeof(uint32_t) (numBytesPerSample) 
 *          But, for DPU_RANGEPROCHWA_DEFAULT_MODE mode, this buffer is not required. 
 *     - Doppler Max Subband Buffer storage only used during Doppler DPU is used as transient data as its lifetime only lasts the
 *       Doppler DPU process function.
 *   @image html bidirmempool.png "Bidirectional Memory Pool: L3, Fast RAM"
 *
 *   @subsection chirpDataHandlingDDMA Handling Real-Only Chirp Data
 *   Refer to [this section](@ref real_only_DDM) to understand how real-only chirp data is handled.
 *
 *   @subsection reconfigDDMA DPU reconfiguration related to data path processing within and across sub-frames
 *
 *   Currently there are no special partial configuration APIs
 *   for DPUs to just reconfigure the desired overlapping param sets, so the
 *   full DPU configuration (xxx_config) must be issued (this includes for example FFT
 *   window configuration in relevant DPUs). Additionally, such a case of
 *   reconfiguration is also required when switching subframes because all HWA param
 *   sets and EDMA resources are overlapped across sub-frames. Note that the DPU's xxx_config API
 *   is a full configuration API beyond the HWA and EDMA resources configuration.
 *   After the DDMA optimizations (described in Section DDMA Optimizations of ti/datapath/dpc/objectdetection/objdethwaDDMA/docs/doxygen/html/index.html),
 *   parallel processing is done wherein AOA processing and UART data transfer of the current sub-frame/frame can run in
 *   parallel with the 1D and 2D processing of the next frame. So, subframe switching and next subframe reconfiguration must be done before
 *   AoA estimation so that range processing of the next incoming subframe can run parallelly. Next subframe reconfiguration includes
 *   reconfiguring ADC buffer for RX channel offset (depending on the channel datasize of the next subframe), Range and Doppler FFT window
 *   configuration.
 *
 *
 *  @section objdetddmahwa_calibration Rx Channel Gain/Offset Measurement and Compensation
 *
 *    Because of imperfections in antenna layouts on the board, RF delays in SOC, etc,
 *    there is need to calibrate the sensor to compensate for bias in the receive channel gain and phase imperfections.
 *    The DPC provides the ability to do the measurement and compensation.
 *
 *    @anchor Figure_calibration_DDMA
 *    @image html calibrationDDM.png "Rx Channel Gain/Offset Measurement and Compensation"
 *
 *   @subsection measurementDDMA Measurement
 *     If the measurement (@ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t::enabled) is enabled in pre-start
 *     common config (@ref DPC_ObjectDetection_PreStartCommonCfg_t::measureRxChannelBiasCfg),
 *     it is assumed that a strong target like a corner reflector is kept at boresight at
 *     a distance of X = @ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t::targetDistance.
 *
 *     For the total number of detected objects after doppler processing, peak search is done in the 0th Doppler bin
 *     of the detection matrix within distance D = @ref DPC_ObjectDetection_MeasureRxChannelBiasCfg_t::searchWinSize
 *     around X i.e peak is searched between -D/2 + X and X + D/2. The detection matrix is laid out as
 *     two dimensional matrix defined as @ref DPIF_DETMATRIX_FORMAT_1. The peak position is then used to obtain the
 *     virtual antenna samples from the detected object list (detObjList) obtained after doppler processing stage.
 *     For rx channel phase and gain estimation, minimum of the magnitude squared of the virtual antennas is computed and this minimum is used
 *     to scale the individual antennas so that the magnitude of the coefficients is
 *     always less than or equal to 1.
 *
 *     Rx channel compensation coefficients are calculated according to following equations.
 *
 *       \f[
 *           x(i_{antIdx}) = \mbox{detObjList}.azimSamples(i_{azimAntIdx}), \;\;\;\; i_{azimAntIdx}=0,...,N_{RxAnt}*N_{AzimTxAnt}-1, \;\;\;\; i_{antIdx}=0,...,N_{RxAnt}*N_{AzimTxAnt}-1
 *       \f]
 *
 *       \f[
 *           x(i_{antIdx}) = \mbox{detObjList}.elevSamples(i_{elevAntIdx}), \;\;\;\; i_{elevAntIdx}=0,...,N_{RxAnt}*N_{ElevTxAnt}-1, \;\;\;\; i_{antIdx}=N_{RxAnt}*N_{AzimTxAnt},...,N_{RxAnt}*N_{TxAnt}-1
 *       \f]
 *
 *     The coefficients are calculated as
 *
 *       \f[
 *           c(i_{antIdx}) = \frac{x^*(i_{antIdx})}{|x(i_{antIdx})|^2}\;x_{Min}, \;\;\;\; i_{antIdx}=0,...,N_{RxAnt}*N_{TxAnt}-1
 *       \f]
 *
 *     where
 *
 *       \f[
 *           x_{Min} = \min{|x(i_{antIdx})|}
 *       \f]
 *
 *     In the above calculations, the division by magnitude squared for each element
 *     achieves the gain compensation part i.e equalizes the different tx-rx path gains --
 *     during compensation (@ref compensationDDMA), when the input antenna samples
 *     are multiplied with their corresponding above coefficients,
 *     the result has the same magnitude for all virtual antennas. Similarly,
 *     the multiplication by complex conjugate achieves phase compensation. The use of minimum
 *     of the data set allows for maximal utilization of the 16-bits of fixed-point precision
 *     to represent the coefficients -- the compensation vector will have unity amplitude for the
 *     minimum element and less than unity for the rest.
 *     The compensation rx coefficients are in Q15 format.
 *     Refer to the function @ref DPC_ObjDet_RxChPhaseMeasure
 *     which performs the measurements, the measurement
 *     result (@ref Measure_compRxChannelBiasCfg) is given out of the DPC
 *     to the application as part of the result structure produced by DPC's execute
 *     API (@ref DPC_ObjectDetection_ExecuteResult_t::compRxChanBiasMeasurement).
 *
 *   @subsection compensationDDMA Compensation
 *     When compensation configuration is provided as part of
 *     pre-start common config (**antennaCalibParams** field of @ref DPC_ObjectDetection_PreStartCommonCfg_t)
 *     as seen in figure @ref Figure_calibration_DDMA, for each sub-frame the @ref DPU_DopplerProcHWA_config and @ref DPC_ObjDet_estimateXYZ
 *     will apply this contiguously stored vector on the virtual antenna array before performing angle computation.
 *
 *     @note During calibration care must be exercised to ensure that there are no moving objects in the search
 *       window range.
 *
 *   \n
 */

/** @defgroup DPC_OBJDET_INTERNAL       Object Detection DPC (Data Path Chain) Internal
 */
/**
@defgroup DPC_OBJDET_IOCTL__INTERNAL_DATA_STRUCTURES      Object Detection DPC Internal Data Structures
@ingroup DPC_OBJDET_INTERNAL
@brief
*   This section has a list of all the internal data structures which are a part of the DPC.
*/
/**
@defgroup DPC_OBJDET_IOCTL__INTERNAL_DEFINITIONS      Object Detection DPC Internal Definitions
@ingroup DPC_OBJDET_INTERNAL
@brief
*   This section has a list of all the internal defines which are a part of the DPC.
*/
/**
@defgroup DPC_OBJDET__INTERNAL_FUNCTION             Object Detection DPC Internal Functions
@ingroup DPC_OBJDET_INTERNAL
@brief
*   This section has a list of all the internal function which are a part of the DPC.
*   These are not exposed to the application.
*/

#ifndef DPC_OBJECTDETECTION_INTERNAL_H
#define DPC_OBJECTDETECTION_INTERNAL_H

/* MMWAVE Driver Include Files */
#include <ti/common/mmwave_error.h>
#ifdef INCLUDE_DPM
#include <ti/control/dpm/dpm.h>
#else
#include <kernel/dpl/SemaphoreP.h>
#endif
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/objectdetection.h>

#ifdef __cplusplus
extern "C" {
#endif

 /** @addtogroup DPC_OBJDET_IOCTL__INTERNAL_DATA_STRUCTURES
  @{ */

/**
 * @brief  The structure is used to hold all the dpu configurations that were
 *         constructed at the time of pre-start configuration, so that they
 *         do not need to be reconstructed when issuing for the purpose of
 *         reconfiguration within sub-frame due to overlapping h/w resources
 *         (e.g HWA resources) within sub-frame among dpus or across sub-frames
 *         due to overlapping h/w resources (e.g HWA, L3 and core Local memory).
 */
typedef struct DpuConfigs_t
{
    /*! @brief   Range DPU configuration storage */
    DPU_RangeProcHWA_Config      rangeCfg;

    /*! @brief   Doppler DPU configuration storage */
    DPU_DopplerProcHWA_Config    dopplerCfg;

    /*! @brief   Range CFAR configuration storage */
    DPU_RangeCfarProcHWA_Config    rangeCfarCfg;

} DpuConfigs;

/**
 * @brief
 *  Field of view configuration sin values
 *
 */
typedef struct ObjDetFovAoaSinVal_t
{
    /*! @brief    minimum azimuth angle (in degrees) exported to Host*/
    float        minAzimuthSinVal;

    /*! @brief    maximum azimuth angle (in degrees) exported to Host*/
    float        maxAzimuthSinVal;

    /*! @brief    minimum elevation angle (in degrees) exported to Host*/
    float        minElevationSinVal;

    /*! @brief    maximum elevation angle (in degrees) exported to Host*/
    float        maxElevationSinVal;
} ObjDetFovAoaSinVal;


/**
 * @brief  The structure is used to hold all the relevant information for
 *         each of the sub-frames for the object detection DPC.
 */
typedef struct SubFrameObj_t
{
    /*! @brief   Pointer to hold Range Proc DPU handle */
    DPU_RangeProcHWA_Handle dpuRangeObj;

    /*! @brief   Pointer to hold Doppler DPU handle */
    DPU_DopplerProcHWA_Handle dpuDopplerObj;

    /*! @brief   Static configuration */
    DPC_ObjectDetection_StaticCfg staticCfg;

    /*! @brief   Pointer to hold Range CFAR DPU handle */
    DPU_RangeCFARProcHWA_Handle dpuRangeCfarObj;

    /*! @brief  Log2 of number of doppler bins */
    uint8_t     log2NumDopplerBins;

    /*! @brief      Field of view config used in AoA estimation */
    ObjDetFovAoaSinVal   aoaFovSinVal;

    /*! @brief DPU configuration storage to use within and when switching
     * sub-frames due to shared resources among DPUs of same sub-frame and
     * sharing across sub-frames */
    DpuConfigs dpuCfg;

    /*! @brief      Detected objects side info output data */
    DPIF_PointCloudSideInfo  *  detObjOutSideInfo;

} SubFrameObj;

/*
 * @brief Memory pool object to manage memory based on @ref DPC_ObjectDetection_MemCfg_t.
 */
typedef struct MemPoolObj_t
{
    /*! @brief Memory configuration */
    DPC_ObjectDetection_MemCfg cfg;

    /*! @brief   Pool running adress.*/
    uintptr_t currAddr;

    /*! @brief   Pool max address. This pool allows setting address to desired
     *           (e.g for rewinding purposes), so having a running maximum
     *           helps in finding max pool usage
     */
    uintptr_t maxCurrAddr;
} MemPoolObj;

/*
 * @brief Bi-directional Memory pool object to manage memory based on @ref DPC_ObjectDetection_MemCfg_t.
 */
typedef struct BiDirMemPoolObj_t
{
    /*! @brief Memory configuration */
    DPC_ObjectDetection_BiDirMemCfg cfg;

    /*! @brief   Pool running top adress.*/
    uintptr_t currTopAddr;

    /*! @brief   Pool running bottom adress.*/
    uintptr_t currBottomAddr;

    /*! @brief   Pool max address. This pool allows setting address to desired
     *           (e.g for rewinding purposes), so having a running maximum
     *           helps in finding max pool usage
     */
    uintptr_t maxCurrAddr;
} BiDirMemPoolObj;

/**
 * @brief
 *  Millimeter Object Detection DPC object/instance
 *
 * @details
 *  The structure is used to hold all the relevant information for the
 *  object detection DPC.
 */
typedef struct ObjDetObj_t
{
#ifdef INCLUDE_DPM
    /*! @brief DPM Initialization Configuration */
    DPM_InitCfg   dpmInitCfg;

    /*! @brief Handle to the DPM Module */
    DPM_Handle    dpmHandle;
#endif
    /*! @brief   Handle to the hardware accelerator */
    HWA_Handle    hwaHandle;

    /*! @brief   Per sub-frame object */
    SubFrameObj   subFrameObj[RL_MAX_SUBFRAMES];

    /* Interrupt object for rangeProc */
    Edma_IntrObject         rangProcIntrObj[RANGEPROCHWADDMA_NUM_EDMA_INTERRUPTS];

    /* Interrupt object for dopplerProc */
    Edma_IntrObject         dopplerProcIntrObj[DOPPLERPROCHWADDMA_NUM_EDMA_INTERRUPTS];

    /* Interrupt object for rangeCfarProc */
    Edma_IntrObject         rangeCfarProcIntrObj[RANGECFARPROCHWADDMA_NUM_EDMA_INTERRUPTS];

    /*! @brief Sub-frame index */
    uint8_t       subFrameIndx;

#ifndef INCLUDE_DPM
        /*! @brief   Semaphore Object to pend DPC Execution */
    SemaphoreP_Object   dpcExecSemHandle;
#endif

    /*! @brief   Handle of the EDMA driver. */
    EDMA_Handle   edmaHandle[EDMA_NUM_CC];

    /*! @brief   edma channel to copy the final intersected objs with RAnge CFAR. */
    DPEDMA_ChanCfg edmaDetObjs;

    /*! @brief  Used for checking that inter-frame (inter-sub-frame) processing
     *          finished on time */
    int32_t       interSubFrameProcToken;

    /*! @brief  Indicates the number of times result exported ioctl was received */
    int32_t       numTimesResultExported;

    /*! @brief L3 ram memory pool object */
    BiDirMemPoolObj    L3RamObj;

    /*! @brief Core Local ram memory pool object */
    MemPoolObj    CoreLocalRamObj;

    /*! @brief Fast memory pool object */
    BiDirMemPoolObj    FastRamBufObj;

    /*! @brief  HWA memory bank addresses */
    uint32_t      hwaMemBankAddr[SOC_HWA_NUM_MEM_BANKS];

    /*! @brief  HWA memory single bank size in bytes */
    uint16_t      hwaMemBankSize;

    /*! @brief  Common configuration storage. */
    DPC_ObjectDetection_PreStartCommonCfg commonCfg;

    /*! @brief Flag to make sure pre start common config is received by the DPC
     *          before any pre-start (sub-frame specific) configs are received. */
    bool     isCommonCfgReceived;

    /*! @brief DPC's execute API result storage */
    DPC_ObjectDetection_ExecuteResult executeResult;

    /*! @brief   rx channel gain/phase compensation output of
     *           measurement to send to application in the result of processing
     *            */
    Measure_compRxChannelBiasCfg compRxChanCfgMeasureOut;

    /*! @brief   Stats structure to convey to Application timing and related information. */
    DPC_ObjectDetection_Stats stats;

    /*! @brief   Process call back function configuration */
    DPC_ObjectDetection_ProcessCallBackCfg processCallBackCfg;
} ObjDetObj;

/**
@}
*/

#ifdef INCLUDE_DPM
extern void _DPC_Objdet_Assert(DPM_Handle handle, int32_t expression, const char *file, int32_t line);
#define DPC_Objdet_Assert(handle, expression) {                \
              _DPC_Objdet_Assert(handle, expression,           \
                                 __FILE__, __LINE__);          \
               DebugP_assert(expression);                      \
                                      }
#endif
#ifdef __cplusplus
}
#endif


#endif /* objectdetectioninternal.h */
