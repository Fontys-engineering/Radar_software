/**
 *   @file  dml.h
 *
 *   @brief
 *      Implements DML functionality.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2025 Texas Instruments, Inc.
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
 * @mainpage DML Algorithm
 *
 * @section Introduction
 * Deterministic Maximum Likelihood (DML) is a high-resolution algorithm based on
 * Maximum Likelihood Estimation, used to estimate Direction of Arrival (DOA)
 * of signals. In the current implementation, we perform 2 target DML. It assumes a
 * deterministic signal, where the waveform is known, and estimates the direction of
 * arrival (DOA) of two targets simultaneously by performing a two-dimensional search.
 *
 * Let \f$x\f$ be the signal that is observed at the virtual receiver array. We assume
 * there are at most two objects in the region of interest. Let these objects be at angles
 * \f$\theta_{i}\f$ and \f$\theta_{j}\f$ respectively. Let \f$A(\theta)\f$ be the matrix of
 * steering vectors looking in the directions of interest.
 *
 * \f[
 *      A(\theta) = [a(\theta_{i}) { } a(\theta_{j})]
 * \f]
 *
 * The signal \f$x\f$ can be modeled as \f$x = [a(\theta_{m}) a(\theta_{n})].s + n\f$ where \f$s\f$
 * is the signal from the objects and \f$n\f$ is Gaussian noise.
 *
 * Maximum Likelihood is a method of estimating the parameters of a function to maximize the given
 * function.
 * DML estimator in case of two targets is given by,
 * \f[
 *       argmax_{i,j} (C_{ij}) = \frac{|r_{i}|^{2} + |r_{j}|^2 - 2Re(r_{i}r_{j}^{*}b_{ij})}{1 - |b_{ij}|^{2}}
 * \f]
 * In this case, we want to maximize \f$C_{ij}\f$ and find \f$i\f$, \f$j\f$ that maximizes
 * \f$C_{ij}\f$.
 *
 * In case of two target DML,
 * * \f$r_{i} = x^{H}.a(\theta_{i})\f$
 * * \f$b_{ij} = a(\theta_{i})^{H}.a(\theta_{j})\f$, where \f$i,{ }j \in [1,N]\f$
 *      * \f$N\f$ is the number of bins we want to divide the region of interest
 * * \f$b_{ij} = b_{ab}\f$, if \f$i - j = a - b\f$
 * * \f$C_{ij}\f$ is a symmetric matrix
 *
 * Also, let's denote \f$b^1_{ij}\f$ as \f$\frac{2_{k}}{1 - |b_{ij}|^{2}}\f$ where \f$k\f$ is
 * a normalizing factor.
 *
 * \a
 * Reference: F. Meinl, M. Kunert and H. Blume, "Hardware acceleration of Maximum-Likelihood
 * angle estimation for automotive MIMO radars," 2016 Conference on Design and Architectures
 * for Signal and Image Processing (DASIP), Rennes, France, 2016, pp. 168-175,
 * doi: 10.1109/DASIP.2016.7853815.
 *
 * Limitations:
 * * We assume that there are at most two targets in the given region of interest.
 * * This implementation is tested with simulated targets.
 * * HWA processing and reconfiguration of few HWA parameters (processing on core) are not
 * parallelized. This impacts the performance of the algorithm.
 *
 * @section high_level_flow High Level Design
 * DML algorithm is implemented using HWA. Based on the input parameters to the library,
 * HWA is configured accordingly. Input to the library includes Azimuth Samples, zero insertion
 * mask, number of bins we want to divide the region of interest (Azimuth FFT size) and region of
 * interest where we want to perform DML (ROI). We will also need \f$b_{ij}\f$ and \f$b^1_{ij}\f$
 * to be loaded onto the pre-defined buffers, so that the library can directly load the values into
 * required memory locations for HWA to process. The output of the algorithm is the indices i, j of
 * \f$max(C_{ij})\f$ and the corresponding \f$C_{ij}\f$.
 *
 * To compute \f$C_{ij}\f$, 2D search has to be performed on the matrix \f$C_{ij} { }\forall i,j\f$.
 * However, either upper or lower triangular matrix has to be evaluated as DML function is symmetric
 * in the form of \f$C_{ij} = C_{ji}\f$. In the current implementation, we use the upper triangular
 * matrix.
 *
 * In DML, since we consider there are at most 2 targets, so X, Y axis in the comparison plot
 * determines the location of the targets, i.e. parameters \f$i\f$, \f$j\f$ that maximizes
 * DML\f$C_{ij}\f$. The Z axis in the plot is the maximum value of \f$C_{ij}\f$.
 * @note Location of targets in the plot is in degrees.
 *
 * @image html dml_vs_fft.png "FFT vs DML"
 *
 * In the above figure, two objects are simulated at 20 and 23 degrees.
 * In case of traditional FFT based angle estimation, we can see that the two objects that are
 * 3 degrees apart are not resolved. Whereas in case of DML, two objects that are very close to
 * each other can be resolved. Factors like the number of bins we want to divide the region of
 * interest and the size of each bin will determine how fine the DML resolution is. Maximum number
 * of bins we can divide the region of interest is 128 (size greater than 128 is not tested).
 *
 * @section Code Structures and API Documentation
 * @subsection struct Structures
 * The structures to be used by the user are well-commented in the dml.h file.
 *
 * @subsection api APIs
 * @code
 * DML_HWA_Handle DML_HWA_init
 * (
 *      HWA_Handle  hwaHandle,
 *      int32_t *errCode
 * )
 * @endcode
 *
 * *Description*: \n
 * DML init function. It allocates memory to store its internal
 * data object and returns a handle if it executes successfully.
 *
 * *Parameters*: \n
 *      hwaHandle -> HWA handle \n
 *      errCode -> Pointer to errCode generates by the API.
 *
 * *Return Values*: \n
 *      Success = valid handle \n
 *      Error != 0: NULL
 *
 *
 * @code
 * int32_t DML_HWA_config
 * (
 *      DML_HWA_Handle    handle,
 *      DML_HWA_cfg *cfg
 * )
 * @endcode
 *
 * *Description*: \n
 * DML algorithm configuration function.
 *
 * *Parameters*: \n
 *      handle -> DML handle \n
 *      cfg    -> Pointer to configuration parameters
 *
 * *Return Values*: \n
 *      Success = 0 \n
 *      Error != 0: DML_ERROR_CODE
 *
 *
 * @code
 * int32_t DML_HWA_process
 * (
 *      DML_HWA_Handle handle,
 *      DML_HWA_cfg *cfg,
 *      DML_HWA_OutParams *outParams
    )
 * @endcode
 *
 * *Description*: \n
 * DML process function.
 *
 * *Parameters*: \n
 *      handle    = DML handle \n
 *      cfg       = Pointer to configuration parameters \n
 *      outParams = Pointer to output parameters
 *
 * *Return Values*: \n
 *      Success = 0 \n
 *      Error != 0: DML_ERROR_CODE
 *
 *
 * @code
 * int32_t DML_HWA_deinit
 * (
 *      DML_HWA_Handle handle
 * )
 * @endcode
 *
 * *Description*: \n
 * DML deinit function.
 *
 * *Parameters*: \n
 *      handle    = DML handle.
 *
 * *Return Values*: \n
 *      Success = 0 \n
 *      Error != 0: DML_ERROR_CODE
 *
 * @note
 * To generate \f$b_{ij}\f$ and \f$b^1_{ij}\f$ which are required as the input to the
 * library, the user needs to either generate them in the application (provided in the
 * test) or generate using MATLAB, by running DML_config.m file. Based on the virtual
 * antenna array and the region of interest, \f$b_{ij}\f$ and \f$b^1_{ij}\f$ are generated
 * and stored in dml_test_gen.h file. The MATLAB script can also simulate the targets and
 * provide the azimuth samples needed for testing DML algorithm. To generate \f$b_{ij}\f$ and
 * \f$b^1_{ij}\f$ in the application, user can check DML_generate_bij_and_b1ij function
 * in dml_test.c file.
 *
*/

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DML_HWA_H
#define DML_HWA_H

/* Standard Include Files. */
#include <stdint.h>

/* mmWave SDK Driver/Common Include Files */
// #include <ti/drv/hwa/hwa.h>
#include <drivers/edma.h>
#include <drivers/hwa.h>

/* mmWave SDK Data Path Include Files */

#include <ti/datapath/dpedma/dpedma.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DML_NUM_HWA_PARAMSETS 8

/** @addtogroup DML_ERROR_CODE
 *  Base error code for the DML algorithm
 @{ */

/**
 * @brief   Error Code: Base
 *
 */
#define DP_ERRNO_DML_PROC_BASE          (-1000)

/**
 * @brief   Error Code: Invalid argument
 */
#define DML_HWA_EINVAL                  (DP_ERRNO_DML_PROC_BASE-1)

/**
 * @brief   Error Code: Out of memory
 */
#define DML_HWA_ENOMEM                  (DP_ERRNO_DML_PROC_BASE-2)

/**
 * @brief   Error Code: Out of HWA resources
 */
#define DML_HWA_EHWARES                 (DP_ERRNO_DML_PROC_BASE-3)

/**
 * @brief   Error Code: Wrong window size
 */
#define DML_HWA_EWINDSIZE               (DP_ERRNO_DML_PROC_BASE-4)

/**
 * @brief   Error Code: Find Max Idx error
 */
#define DML_HWA_ERROR_FINDMAX           (DP_ERRNO_DML_PROC_BASE-5)

/**
 * @brief   Error Code: Input parameters are incorrect
 */
#define DML_HWA_ERROR_INPDATA           (DP_ERRNO_DML_PROC_BASE-6)

/**
 * @brief   Error Code: Invalid number of samples
 * Virtual antenna array size should be equal to number of zeros inserted +
 * number of azimuth samples
 */
#define DML_HWA_ERROR_NUM_SAMPLES       (DP_ERRNO_DML_PROC_BASE-7)

/**
 * @brief   Error Code: Invalid number of azimuth FFT bins
 */
#define DML_HWA_ERROR_NUM_AZIMUTH_FFT_BINS   (DP_ERRNO_DML_PROC_BASE-8)

/**
 * @brief   Error Code: Insufficient space in Vector Multiplication RAM
 */
#define DML_HWA_ERROR_VECT_MULT_RAM   (DP_ERRNO_DML_PROC_BASE-9)

/**
 * @brief   Error Code: Insufficient space in Window RAM
 */
#define DML_HWA_ERROR_WIN_RAM   (DP_ERRNO_DML_PROC_BASE-10)

/**
@}
*/

/*!
 *  @brief   Handle for DML.
 */
typedef void*  DML_HWA_Handle;

/**
 * @brief
 *  DML HWA configuration parameters
 *
 * @details
 *  The structure is used to hold the HWA configuration parameters
 *  for the DML
*/
typedef struct DML_HWA_HwaCfg_t
{
    /*!  @brief DML window size in bytes */
    uint32_t    windowSize;

    /*! @brief HWA window RAM offset in number of samples. */
    uint16_t    winRamOffset;

    /*! @brief HWA Vector Multiplication RAM offset in number of samples. */
    uint16_t    vecMultRamOffset;

    /*! @brief Number of Samples to be loaded onto the Vector Multiplication RAM */
    uint32_t vectorMultRAMSamples;

    /*! @brief Number of HWA paramsets reserved for DML.
         The algorithm will use numParamSets consecutively, starting from paramSetStartIdx.\n
    */
    uint8_t     numParamSets;

    /*! @brief HWA paramset Start index.
         Application has to ensure that paramSetStartIdx is such that \n
        [paramSetStartIdx, paramSetStartIdx + 1, ... (paramSetStartIdx + numParamSets - 1)] \n
        is a valid set of HWA paramsets.\n
    */
    uint8_t    paramSetStartIdx;

}DML_HWA_HwaCfg;

/**
 * @brief
 * DML EDMA configuration for Azimuth FFT
 *
 * @details
 *  The structure is used to hold the EDMA configuration for Azimuth FFT
 *
 */
typedef struct DML_AzimFFT_EdmaCfg_t
{
    /**
     * @brief  EDMA configuration for Azimuth FFT input data
     * (Azimuth samples (DSS_L3) -> HWA memory).
     */
    DPEDMA_ChanCfg edmaIn;

    /**
     * @brief EDMA configuration for Azimuth FFT input data Signature Channel
     */
    DPEDMA_ChanCfg edmaInSignature;

    /**
     * @brief EDMA configuration to transfer scaled Azimuth FFT from HWA membank
     * to Vector Multiplication RAM.
     */
    DPEDMA_ChanCfg edmaOut;

}DML_AzimFFT_EdmaCfg;

/**
 * @brief
 *  DML EDMA configuration parameters
 *
 * @details
 *  The structure is used to hold the EDMA configuration parameters
 *  for the DML
 */
typedef struct DML_HWA_EdmaCfg_t
{
    /*! @brief  EDMA driver handle. */
    EDMA_Handle edmaHandle;

    /*! @brief  EDMA config for AZIM FFT stage. */
    DML_AzimFFT_EdmaCfg azimFFTEdmaCfg;

}DML_HWA_EdmaCfg;

/**
 * @brief
 * DML buffers interface
 *
 * @details
 * The structure holds size of the buffer and the data pointer.
 */
typedef struct DPIF_Buf_t
{
    /*! @brief  buffer size in bytes */
    uint32_t                dataSize;

    /*! @brief  data pointer */
    void                    *data;
}DPIF_Buf;

/**
 * @brief
 *  DML reconfiguration buffers
 *
 * @details
 *  The structure is used to hold the buffers used for reconfiguration
 *  of HWA paramsets and Common registers
 *
 */
typedef struct DML_Reconfig_Buf_t
{
    /*! @brief  buffer to store the pre-calculated Vector Multiplication RAM offset */
    uint32_t *preproc_buf;

    /**
     * @brief buffer to store pre-calculated source address of paramset which
     * computes -2*REAL(ri rj* bij)
     */
    uint32_t *src1_buf;

    /**
     * @brief buffer to store pre-calculated destination address of paramset which computes
     * -2*REAL(ri rj* bij)
     */
    uint32_t *dst1_buf;

    /**
     * @brief buffer to store pre-calculated source address of paramset which computes
     * rest of the CIJ
     */
    uint32_t *src2_buf;

    /**
     * @brief buffer to store pre-calculated destination address of paramset which
     * computes rest of the CIJ
     */
    uint32_t *dst2_buf;

    /*! @brief  buffer to store the channel combination size */
    uint32_t *chanComb_buf;
}DML_Reconfig_Buf;

/**
 * @brief
 *  DML HW configuration parameters
 *
 * @details
 *  The structure is used to hold the  HW configuration parameters
 *  for the DML
 *
 */
typedef struct DML_HWA_HW_Resources_t
{
    /*! @brief  EDMA configuration */
    DML_HWA_EdmaCfg edmaCfg;

    /*! @brief  HWA configuration */
    DML_HWA_HwaCfg  hwaCfg;

    /*! @brief  Buffer for storing Azim FFT input samples on L3 */
    DPIF_Buf azimFFTIn;

    /*! @brief  Buffer for storing -2bij */
    DPIF_Buf bijBuf;

    /*! @brief  Buffer for storing 1/1-|bij|^2 */
    DPIF_Buf b1ijBuf;

    /*! @brief  DML reconfiguration buffers */
    DML_Reconfig_Buf reconfigBuf;

}DML_HWA_HW_Resources;


/**
 * @brief
 *  DML HWA Azimuth FFT hardware resources
 *
 * @details
 *  The structure is used to hold the hardware resources needed for Azimuth FFT
 *
 */
typedef struct DML_AzimFFTCfg_t
{
    /*! @brief Number of input samples */
    uint16_t  numSamples;

    /*! @brief Number of FFT bins */
    uint16_t nFFTbins;

}DML_AzimFFTCfg;

/**
 * @brief
 *  DML static configuration parameters
 *
 * @details
 *  The structure is used to hold the static configuration parameters
 *  for DML The following conditions must be satisfied:
 *
 *  \ingroup DML_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DML_HWA_StaticConfig_t
{
    /*! @brief  Size of input samples (Azim FFT input samples) in bytes */
    uint16_t    sizeOfInputSamples;

    /*! @brief  Number of steering vectors*/
    uint16_t    numSteerVecs;

    /*! @brief  Start index of steering vectors*/
    uint16_t    n1;

    /*! @brief  End index of steering vectors*/
    uint16_t    n2;

    /*! @brief Spacing in X dimension */
    uint64_t zeroInsrtMaskAzim;

    /*! @brief Size of Virtual Antenna Array */
    uint32_t virtualAntennaArraySize;

    /*! @brief Azim FFT Configuration */
    DML_AzimFFTCfg azimFFTCfg;

}DML_HWA_StaticConfig;

/**
 * @brief
 *  DML configuration parameters
 *
 * @details
 *  The structure is used to hold the configuration parameters
 *  of DML
 *
 *  \ingroup DML_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DML_HWA_cfg_t
{
    /*! @brief HW resources. */
    DML_HWA_HW_Resources  hwRes;

    /*! @brief Static configuration. */
    DML_HWA_StaticConfig  staticCfg;

    /*! @brief  Number of PING/PONG loops for CIJ computation  */
    uint16_t maxLoopForCIJ;

}DML_HWA_cfg;

/**
 * @brief
 *  DML processing output parameters
 *
 * @details
 *  The structure is used to hold the output parameters of DML algorithm
 *
 *  \ingroup DML_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DML_HWA_OutParams_t
{
    /*! @brief total processing time including EDMA transfers.*/
    uint32_t            processingTime;

    /*! @brief Final result from HWA */
    cmplx32ImRe_t result_MaxCij;

    /*! @brief Peak location of target 1 */
    uint32_t peak_y;

    /*! @brief Peak location of target 2 */
    uint32_t peak_x;

    /*! @brief Peak location offset of target 2 in upper triangular matrix for a given row */
    uint32_t peak_x_b1ij;

    /*! @brief  Scaled Absolute Square of FFT samples at peak location x */
    uint32_t sqAbs_x;

    /*! @brief  Scaled Absolute Square of FFT samples at peak location y */
    uint32_t sqAbs_y;

}DML_HWA_OutParams;


/**
 *  @b Description
 *  @n
 *      DML init function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]   hwaHandle HWA handle
 *  @param[out]  errCode Pointer to errCode generates by the API
 *
 *  @retval
 *      Success     - valid handle
 *  @retval
 *      Error       - NULL
 */
DML_HWA_Handle DML_HWA_init(
    HWA_Handle  hwaHandle,
    int32_t* errCode
);

/**
  *  @b Description
  *  @n
  *   DML algorithm configuration
  *
  *  @param[in]   handle     DML handle.
  *  @param[in]   cfg        Pointer to configuration parameters.
  *
  *  @retval
  *      Success      = 0
  *  @retval
  *      Error       != 0 @ref DML_ERROR_CODE
  */
int32_t DML_HWA_config(
    DML_HWA_Handle handle,
    DML_HWA_cfg *cfg
);

/**
 *  @b Description
 *  @n DML process function.
 *
 * DSP : Some operation.
 * HWA : Another HWA operation.
 * EDMA: A data transfer operation.
 *
 *  @param[in]   handle     DML handle
 *  @param[in]   cfg        Pointer to configuration parameters
 *  @param[out]  outParams  Pointer to output parameters
 *
 *
 *  @retval
 *      Success     =0
 *  @retval
 *      Error      !=0 @ref DML_ERROR_CODE
 */
int32_t DML_HWA_process(
    DML_HWA_Handle handle,
    DML_HWA_cfg *cfg,
    DML_HWA_OutParams *outParams);

/**
  *  @b Description
  *  @n
  *  DML deinit
  *
  *  @param[in]   handle   DML handle.
  *
  *  @retval
  *      Success      =0
  *  @retval
  *      Error       !=0 @ref DML_ERROR_CODE
  */
int32_t DML_HWA_deinit(DML_HWA_Handle handle);

#ifdef __cplusplus
}
#endif

#endif
