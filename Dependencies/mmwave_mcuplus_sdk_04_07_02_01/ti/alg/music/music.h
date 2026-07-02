/**
 *   @file  music.h
 *
 *   @brief
 *      Implements MUSIC Algorithm using DSP.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2024 Texas Instruments, Inc.
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
 * @mainpage MUSIC Algorithm
 *
 * The MUSIC (Multiple Signal Classification) algorithm is a high-resolution
 * eigen-based method to estimate the directions of arrival (DOA) of signals
 * impinging on an antenna array. It is commonly used in radar, sonar,
 * and wireless communication systems. MUSIC works by exploiting the
 * eigenstructure of the covariance matrix of the received signal.
 *
 * The covariance matrix of the received signal can be decomposed into two
 * orthogonal subspaces: the signal subspace (spanned by the eigenvectors
 * corresponding to the largest eigenvalues) and the noise subspace (spanned by
 * eigenvectors corresponding to the smallest eigenvalues). MUSIC creates a
 * pseudospectrum by projecting potential DOA vectors (steering vectors) onto
 * the noise subspace. Peaks in this pseudospectrum indicate the directions of
 * the incoming signals.
 *
 * This library implements unitary transformation method of MUSIC. Unitary transformation
 * method provides an ease of computation as it transforms complex covariance matrix into real
 * symmetric matrix, and complex search vector (steering vector) into real vector.
 *
 * There are 3 key steps involved in the MUSIC algorithm:
 *
 * Step 1: Covariance Matrix Computation:
 * With a signal \f$s\f$ having \f$N_{ant}\f$ number of antenna elements, a covariance matrix of
 * size \f$ N_s x N_s \f$ is formed after performing forward-backward spatial smoothing.
 * Here, \f$N_s\f$ is the size of subarray \f$= N_{ant}/2\f$. Then, we do unitarization of
 * covariance matrix \f$R\f$ using unitary matrix \f$U\f$ to get a real-symmetric matrix \f$C\f$.
 *
 * \f[
 *      C = URU^H
 * \f]
 *
 *
 * Step 2: Eigenvalue Decomposition:
 * We use Jacobi iteration method to find the eigen vectors (signal subspace and noise subspace)
 * of real-symmetric matrix \f$C\f$.
 *
 * Step 3: MUSIC Psuedo-spectrum Calculation:
 *
 * \f[
 *      P_{MUSIC}(\theta) = \frac{1}{|E_{N}^H Ua(\theta)|^2}
 * \f]
 *
 * \f$E_N\f$: Eigen vectors corresponding to the noise subspace \n
 * \f$U\f$: Unitarization Matrix \n
 * \f$a(\theta)\f$: Steering vector searching from -theta/2 to theta/2. We define steering vector as
 * symmetric around origin, to get computational advantage, as imaginary part cancels out on
 * multiplication with unitary matrix, giving real steering vector \f$Ua(\theta)\f$.
 *
 *
 * \a
 * Reference: Keh-Chiarng Huarng and Chien-Chung Yeh, "A unitary transformation method for a
 * angle-of-arrival estimation," in IEEE Transactions on Signal Processing, vol. 39, no. 4, pp.
 * 975-977, April 1991, doi: 10.1109/78.80927.
 *
 * Limitations:
 * 1. Assumes uniform linear array.
 * 2. Spatial smoothing subarray size is hard-coded to half of antenna length.
 * 3. Noise subspace size is hard-code to be half of subarray size.
 * 4. Limited testing: performance of MUSIC spectrum is verified with simulated test vectors only.
 *
 * @image html output.png "MUSIC vs FFT for AoA [0&deg; 5&deg;] for 16 antenna elements"
 *
 * @section Code Structures and API Documentation
 * @subsection struct Structures
 * The structures to be used by the user are well-commented in the music.h file.
 *
 * @subsection api APIs
 * @code
 * MUSIC_Handle MUSIC_init
 * (
 *      int32_t *errCode
 * )
 * @endcode
 *
 * *Description*: \n
 * MUSIC init function. It allocates memory to store its internal
 * data object and returns a handle if it executes successfully.
 *
 * *Parameters*: \n
 *      errCode = Pointer to errCode generates by the API.
 *
 * *Return Values*: \n
 *      Success = valid handle \n
 *      Error != 0: NULL
 *
 *
 * @code
 * int32_t MUSIC_config
 * (
 *      MUSIC_Handle handle,
 *      MUSIC_Config *cfg
 * )
 * @endcode
 *
 * *Description*: \n
 * MUSIC configuration function.
 *
 * *Parameters*: \n
 *      handle = MUSIC handle. \n
 *      cfg    = Pointer to configuration parameters.
 *
 * *Return Values*: \n
 *      Success = 0 \n
 *      Error != 0: MUSIC_ERROR_CODE
 *
 *
 * @code
 * int32_t MUSIC_process
 * (
 *      MUSIC_Handle handle,
 *      MUSIC_Config *cfg,
 *      MUSIC_OutParams *outParams
    )
 * @endcode
 *
 * *Description*: \n
 * MUSIC process function.
 *
 * *Parameters*: \n
 *      handle    = MUSIC handle. \n
 *      cfg       = Pointer to configuration parameters. \n
 *      outParams = Pointer to output parameters.
 *
 * *Return Values*: \n
 *      Success = 0 \n
 *      Error != 0: MUSIC_ERROR_CODE
 *
 *
 * @code
 * int32_t MUSIC_deinit
 * (
 *      MUSIC_Handle handle
 * )
 * @endcode
 *
 * *Description*: \n
 * MUSIC deinit function.
 *
 * *Parameters*: \n
 *      handle    = MUSIC handle.
 *
 * *Return Values*: \n
 *      Success = 0 \n
 *      Error != 0: MUSIC_ERROR_CODE
 *
*/

#ifndef MUSIC_H
#define MUSIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ========================================================================= */
/*                             Include Files                                 */
/* ========================================================================= */

/* Standard Include Files. */
#include <stdint.h>
#ifdef _TMS320C6X
#include "c6x.h"
#endif

/* ========================================================================= */
/*                           Macros & Typedefs                               */
/* ========================================================================= */

/**
 * @brief   Error Code: Invalid argument
 */
#define MUSIC_EINVAL                  (-1)

/**
 * @brief   Error Code: Out of memory
 */
#define MUSIC_ENOMEM                  (-2)


/*!
 *  @brief   Handle for MUSIC.
 */
typedef void * MUSIC_Handle;


/* ========================================================================= */
/*                         Structures and Enums                              */
/* ========================================================================= */

/**
 * @brief Structure to hold imag and real part of complex value
 *
 *  \ingroup MUSIC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct cplxf
{
    float32_t imag;
    float32_t real;
} cplxf_t;

/**
 * @brief Memory Configuration Parameters
 *
 *  \ingroup MUSIC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct MUSIC_MemCfg_t
{
    /*! @brief   Start address of memory provided by the application
     *           from which scratch buffers will allocate.
     */
    void *addr;

    /*! @brief   Size limit of memory allowed to be consumed */
    uint32_t size;

    /*! @brief   Size of memory consumed */
    uint32_t memUsedSize;
} MUSIC_MemCfg;

/**
 * @brief
 *  HW resources configuration parameters
 *
 * @details
 *  The structure is used to hold the HW configuration parameters
 *  for MUSIC Algorithm
 *
 *  \ingroup MUSIC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct MUSIC_HW_Resources_t
{
    /*! @brief  Input Data */
    cplxf_t *inputData;

    /*! @brief  MUSIC Spectrum */
    float32_t *musicSpectrum;

   /*! @brief Core Local RAM configuration (e.g DSS_L2 for DSP).
    *         MUSIC lib will allocate memory from this as needed and report
    *         the amount of memory consumed to the application */
    MUSIC_MemCfg coreLocalRamCfg;

} MUSIC_HW_Resources;

/**
 * @brief
 *  Jacobi EVD Configuration parameters
 *
 * @details
 *  The structure is used to hold the configuration parameters
 *  for Eigen Value Decomposition
 *
 *  \ingroup MUSIC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct MUSIC_JacobiEVDCfg_t
{
    /*! @brief  Tolerance for Jacobi iteration method.
        This tolerance defines one of the stopping criteria as
        off_diagonal_norm < initial_frobenius_norm * tolerance. */
    float32_t  tolerance;

    /*! @brief  zeroThrsldFactor * frobenius_norm decides if an element
        is sufficiently close to zero. */
    float32_t  zeroThrsldFactor;

    /*! @brief  Maximum Jacobi Iterations  */
    uint8_t maxIterations;

} MUSIC_JacobiEVDCfg;


/**
 * @brief
 *   Steering Matrix configuration parameters
 *
 * @details
 *  The structure is used to hold the configuration parameters
 *  for the steering matrix generation.
 *
 *  \ingroup MUSIC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct MUSIC_SteeringMatCfg_t
{
    /*! @brief  Number of angles to sample in MUSIC spectrum (Grid Size) */
    uint16_t  numAnglesToSample;

    /*! @brief  Start angle (in degree) to sample in MUSIC spectrum (-theta) */
    float32_t   startAngleDeg;

    /*! @brief  Difference between two search angle in degree
    (for e.g.: 1 degree or 0.5 degree) */
    float32_t   stepSize;

} MUSIC_SteeringMatCfg;

/**
 * @brief
 *   MUSIC static configuration parameters
 *
 * @details
 *  The structure is used to hold the static configuration parameters
 *  for the  MUSIC Algorithm.
 *
 *  \ingroup MUSIC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct MUSIC_StaticConfig_t
{
    /*! @brief  Number of virtual antennas (input angle array size) */
    uint16_t    numAntennas;

    /*! @brief  Configuration to generate steering matrix */
    MUSIC_SteeringMatCfg  steerMatCfg;

    /*! @brief  Configurations for Jacobi EVD */
    MUSIC_JacobiEVDCfg  jacobiEVDCfg;

} MUSIC_StaticConfig;

/**
 * @brief
 *   MUSIC configuration parameters
 *
 * @details
 *  The structure is used to hold the configuration parameters
 *  for the  MUSIC Algorithm
 *
 *  \ingroup MUSIC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct MUSIC_Config_t
{
    /*! @brief HW resources. */
    MUSIC_HW_Resources  hwRes;

    /*! @brief Static configuration. */
    MUSIC_StaticConfig  staticCfg;

}MUSIC_Config;

/**
 * @brief
 *   MUSIC output parameters
 *
 * @details
 *  The structure is used to hold the configuration parameters
 *  for the MUSIC Algorithm
 *
 *  \ingroup MUSIC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct MUSIC_OutParams_t
{
    /*! @brief in process flag to detect the state of execution */
    uint8_t  inProgress;

    /*! @brief Jacobi Iterations. */
    uint8_t  jacobiIterations;

}MUSIC_OutParams;


/* ========================================================================= */
/*                         Function Declarations                             */
/* ========================================================================= */
/**
  *  @b Description
  *  @n  MUSIC init function.
  *
  *  @param[out]   errCode    Pointer to errCode
  *
  *  \ingroup    MUSIC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success    - Valid Handle
  *  @retval
  *      Error      - NULL
  */
extern MUSIC_Handle MUSIC_init(int32_t *errCode);

/**
  *  @b Description
  *  @n  MUSIC configuration function.
  *
  *  @param[in]   handle     handle.
  *  @param[in]   cfg        Pointer to configuration parameters.
  *
  *  \ingroup    MUSIC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success     = 0
  *  @retval
  *      Error      != 0 MUSIC_ERROR_CODE
  */
extern int32_t MUSIC_config(MUSIC_Handle handle,
                        MUSIC_Config *cfg);

/**
  *  @b Description
  *  @n Unitary MUSIC process function.
  *
  *  @param[in]   handle     handle.
  *  @param[in]   cfg        Pointer to configuration parameters.
  *  @param[out]  outParams  Pointer to output parameters.
  *
  *  \ingroup   MUSIC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success     = 0
  *  @retval
  *      Error      !=0  MUSIC_ERROR_CODE
  */
extern int32_t MUSIC_process(MUSIC_Handle handle,
                        MUSIC_Config *cfg,
                        MUSIC_OutParams *outParams);


/**
  *  @b Description
  *  @n  MUSIC deinit function.
  *
  *  @param[in]    handle     handle
  *
  *  \ingroup    MUSIC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success     = 0
  *  @retval
  *      Error      != 0  MUSIC_ERROR_CODE
  */
extern int32_t MUSIC_deinit(MUSIC_Handle handle);

#ifdef __cplusplus
}
#endif

#endif /* end of _MUSIC_H_ definition */
