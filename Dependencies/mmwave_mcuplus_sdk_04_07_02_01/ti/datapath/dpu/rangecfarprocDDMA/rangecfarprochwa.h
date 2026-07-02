/**
 *   @file  rangecfarprochwa.h
 *
 *   @brief
 *      Implements Range CFAR DPU (DDMA) using HWA.
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
#ifndef RANGECFAR_PROC_HWA_H
#define RANGECFAR_PROC_HWA_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* Datapath files */
#include <ti/datapath/dpif/dpif_detmatrix.h>
#include <ti/datapath/dpif/dpif_pointcloud.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpu/rangecfarprocDDMA/rangecfarproccommon.h>
#include <drivers/hwa.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ARM_ARCH) && (__ARM_ARCH == 7) && defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')
#define CSL_CACHE_L1D_LINESIZE 64U
#endif
/** @addtogroup DPU_CFARPROC_EXTERNAL_DEFINITIONS
 *
 @{ */

/*! @brief Number of HWA parameter sets */
#define DPU_RANGECFARPROCHWA_NUM_HWA_PARAM_SETS  2U

/*! @brief Number of HWA memory banks */
#define DPU_RANGECFARPROCHWA_NUM_HWA_MEMBANKS 8U

/*! @brief Number of HWA memory banks */
#define DPU_RANFECFARPROCHWADDMA_HWA_MEMBANK_SIZE_BYTES 16384U

/*! @brief Number of EDMA Interrupts */
#define RANGECFARPROCHWADDMA_NUM_EDMA_INTERRUPTS  2U

/*! @brief Alignment for memory allocation purpose of detection matrix.
 *         There is CPU access of detection matrix in the implementation.
 */
#define DPU_RANGECFARPROCHWA_DET_MATRIX_BYTE_ALIGNMENT CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for R5F for detection list memory allocation purpose. */
#define DPU_RANGECFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_R5F \
        CSL_MAX(CSL_CACHE_L1D_LINESIZE, DPIF_CFAR_DET_LIST_CPU_BYTE_ALIGNMENT)

/*! @brief Alignment for DSP for detection list memory allocation purpose. */
#define DPU_RANGECFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_DSP \
        DPIF_CFAR_DET_LIST_CPU_BYTE_ALIGNMENT

/*! @brief Alignment for memory allocation purpose. There is CPU access of this buffer
 *         in the implementation.
 */
#define DPU_RANGECFARPROCHWA_HWA_MEM_OUT_DOPPLER_BYTE_ALIGNMENT    (sizeof(uint32_t))

/*! @brief Alignment for memory allocation purpose. There is CPU access of thi buffers
 *         in the implementation.
 */
#define DPU_RANGECFARPROCHWA_HWA_MEM_OUT_RANGE_BYTE_ALIGNMENT    (sizeof(uint32_t))

/*! @brief Alignment for R5F for detection list memory allocation purpose. */
#define DPU_RANGECFARPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT_R5F \
        CSL_CACHE_L1D_LINESIZE

/*! @brief Alignment for DSP for detection list memory allocation purpose. */
#define DPU_RANGECFARPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT_DSP CSL_CACHE_L1D_LINESIZE

/**
@}
*/

/** @addtogroup DPU_RANGECFARPROC_ERROR_CODE
 *  Base error code for the cfarProc DPU is defined in the
 *  \include ti/datapath/dpif/dp_error.h
 @{ */

/**
 * @brief   Error Code: Invalid argument
 */
#define DPU_RANGECFARPROCHWA_EINVAL                  (DP_ERRNO_RANGECFAR_PROC_BASE-1)

/**
 * @brief   Error Code: Invalid detection matrix format argument
 */
#define DPU_RANGECFARPROCHWA_EINVAL__DET_MATRIX_FORMAT (DP_ERRNO_RANGECFAR_PROC_BASE-2)

/**
  * @brief   Error Code: Invalid number of param sets
  */
#define DPU_RANGECFARPROCHWA_EINVAL__NUM_PARAM_SETS    (DP_ERRNO_RANGECFAR_PROC_BASE-3)

/**
 * @brief   Error Code: Out of memory when allocating using MemoryP_osal
 */
#define DPU_RANGECFARPROCHWA_ENOMEM                  (DP_ERRNO_RANGECFAR_PROC_BASE-10)

/**
 * @brief   Error Code: HWA input memory for detection matrix is not sufficient.
 */
#define DPU_RANGECFARPROCHWA_ENOMEM__DET_MATRIX_EXCEEDS_HWA_INP_MEM  (DP_ERRNO_RANGECFAR_PROC_BASE-11)

 /**
  * @brief   Error Code: Memory not aligned for detection matrix (detMatrix.data)
  */
#define DPU_RANGECFARPROCHWA_ENOMEMALIGN_DET_MATRIX                  (DP_ERRNO_RANGECFAR_PROC_BASE-12)

/**
 * @brief   Error Code: Memory not aligned for @ref DPU_RangeCFARProcHWA_HW_Resources::hwaMemOutRange
 */
#define DPU_RANGECFARPROCHWA_ENOMEMALIGN_HWA_MEM_OUT_RANGE           (DP_ERRNO_RANGECFAR_PROC_BASE-15)

/**
 * @brief   Error Code: Internal error
 */
#define DPU_RANGECFARPROCHWA_EINTERNAL               (DP_ERRNO_RANGECFAR_PROC_BASE-20)

/**
 * @brief   Error Code: Not implemented
 */
#define DPU_RANGECFARPROCHWA_ENOTIMPL                (DP_ERRNO_RANGECFAR_PROC_BASE-30)

 /**
  * @brief   Error Code: Semaphore error
  */
 #define DPU_RANGECFARPROCHWA_ESEMA                   (DP_ERRNO_RANGECFAR_PROC_BASE-40)

 /**
  * @brief   Error Code: Only CFAR-OS can be used
  */
#define DPU_RANGECFARPROCHWA_ERROR_METHOD_CFAR         (DP_ERRNO_RANGECFAR_PROC_BASE-41)

 /**
  * @brief   Error Code: HW Resource Error
  */
#define DPU_RANGECFARPROCHWA_ERROR_HWRES         (DP_ERRNO_RANGECFAR_PROC_BASE-42)

 /**
  * @brief   Error Code: Error in calculating number of gates per ping
  */
#define DPU_RANGECFARPROCHWADDMA_NUM_GATES_PER_PING_ERR         (DP_ERRNO_RANGECFAR_PROC_BASE-43)

 /**
  * @brief   Error Code: Error in calculating CFAR In size
  */
#define DPU_RANGECFARPROCHWADDMA_CFARINSIZE_ERR         (DP_ERRNO_RANGECFAR_PROC_BASE-44)

 /**
 @}
 */


/**
 * @brief
 *  CFAR HWA configuration
 *
 * @details
 *  The structure is used to hold the HWA configuration needed for CFAR
 *
 *  \ingroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_RangeCFARProcHWA_HwaCfarConfig_t
{
    /*! @brief     HWA paramset Start index */
    uint8_t    paramSetStartIdx;

    /*! @brief     number of HWA paramset */
    uint8_t    numParamSet;
}DPU_RangeCFARProcHWA_HwaCfarConfig;

/**
 * @brief
 *  CFARProcHWA DPU initial configuration parameters
 *
 * @details
 *  The structure is used to hold the DPU initial configurations.
 *
 *  \ingroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_RangeCFARProcHWA_InitParams_t
{
    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;
}DPU_RangeCFARProcHWA_InitParams;


typedef struct DPU_RangeCfarProc_Edma_t
{
    /*! @brief  EDMA Ping/Pong channel. */
    DPEDMA_ChanCfg  pingPong[2];
}DPU_RangeCfarProc_Edma;


typedef struct DPU_RangeCfarProc_EdmaIntrObj_t
{
    /*! @brief  EDMA Ping/Pong channel. */
    Edma_IntrObject  *pingPong[2];
}DPU_RangeCfarProc_EdmaIntrObj;

typedef struct RangeCfarListObj_t
{

    /*! @brief  Doppler Idx */
    uint32_t    dopIdx;

    /*! @brief  Range Gate Idx */
    uint32_t    rangeIdx;

    /*! @brief  CFAR noise value */
    uint32_t    rangeCFARNoise;

}RangeCfarListObj;

/*!
 *  @brief    Detected object parameters filled by HWA CFAR
 *
 *  \ingroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef volatile struct DPU_RangeCFARProcHWA_CfarDetOutput_t
{
    uint32_t   noise;           /*!< @brief Noise energy in CFAR cell */
    uint32_t   cellIdx  : 12;   /*!< @brief Sample index (i.e. cell under test index) */
    uint32_t   iterNum  : 12;   /*!< @brief Iteration number (i.e. REG_BCNT counter value) */
    uint32_t   reserved :  8;   /*!< @brief Reserved */
} DPU_RangeCFARProcHWA_CfarDetOutput;


/**
 * @brief
 *  CFAR Hardware resources
 *
 * @details
 *  CFAR Hardware resources
 *
 *  \ingroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE
 *
 *
 */
typedef struct DPU_RangeCFARProcHWA_Resources_t
{
    /*! @brief     EDMA Handle */
    EDMA_Handle         edmaHandle;

    /*! @brief  EDMA configuration for DetMat In (DetMat -> HWA memory). */
    DPU_RangeCfarProc_Edma edmaIn;

    /*! @brief  EDMA configuration for DetMat In (Signature Channel) */
    DPU_RangeCfarProc_Edma edmaInSignature;

    /*! @brief  EDMA configuration for DetMat Out */
    DPU_RangeCfarProc_Edma edmaOut;

    /*! @brief  EDMA interrupt configuration */
    DPU_RangeCfarProc_EdmaIntrObj edmaIntrObj;

    /*! @brief     HWA Configuration */
    DPU_RangeCFARProcHWA_HwaCfarConfig   hwaCfg;

    /*! @brief Pointer to range/Doppler log2 magnitude detection matrix. The data buffer
     *         must be aligned to @ref DPU_RANGECFARPROCHWA_DET_MATRIX_BYTE_ALIGNMENT */
    DPIF_DetMatrix      detMatrix;

    /*! @brief Number of bytes per sample in detection matrix */
    uint32_t      detMatBytesPerSample;

    /*! @brief  Local scratch buffer storing intermediate range CFAR Output */
    uint8_t * rangeCfarScratchBuf[2];

    /*! @brief  Size of Local CFAR scratch buf (Ping + pong)*/
    uint32_t rangeCfarScratchBufSizeBytes;

    /*! @brief List of detected objects */
    RangeCfarListObj * rangeCfarList;

    /*! @brief  Size of Range CFAR scratch buf (Ping + pong)*/
    uint32_t rangeCfarListSizeBytes;

    /*! @brief Number of objects detected for every doppler bin */
    uint8_t * rangeCfarNumObjPerDopplerBinBuf;

    /*! @brief  Size of Number of objects detected for every doppler bin buffer */
    uint32_t rangeCfarNumObjPerDopplerBinSizeBytes;

    /*! @brief HWA scratch memory to page-in detection matrix. Note in HWA1.0, 2 contiguous
     *         memory banks of the 4 banks, in HWA2.0 4 contiguous memory banks of the 8 banks,
               could be allocated to this. */
    uint16_t  *hwaMemInp;

    /*! @brief Number of elements of type uint16_t of HWA memory to hold detection matrix
     *         (associated with @ref hwaMemInp) */
    uint32_t   hwaMemInpSize;

    uint32_t   maxNumCFARObj;

    /*! @brief HWA scratch memory for producing intermediate cfar detection list in Range domain,
     *         cannot be overlaid with other HWA scratch memory inputs for this DPU.
     *         Must be different memory bank than bank(s) of @ref hwaMemInp.
     *         Must be aligned to @ref DPU_RANGECFARPROCHWA_HWA_MEM_OUT_RANGE_BYTE_ALIGNMENT.
     *         Note this need not be the start of a HWA memory bank but typically it is,
     *         and is therefore naturally aligned to this alignment requirement */
    uint8_t *hwaMemOutRange;

    /*! @brief Number of elements of type @ref DPU_RangeCFARProcHWA_CfarDetOutput of
     *         HWA memory for cfar detection list in Range domain */
    uint32_t hwaMemOutRangeSize;

} DPU_RangeCFARProcHWA_HW_Resources;

/**
 * @brief
 *  HWA CFAR static configuration
 *
 * @details
 *  The structure is used to hold the static configuration used for CFAR.
 *
 *  \ingroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_RangeCFARProcHWA_StaticConfig_t
{
    /*! @brief  Num Subbands Total */
    uint8_t     numSubBandsTotal;

    /*! @brief  Number of range bins */
    uint16_t    numRangeBins;

    /*! @brief  Number of doppler bins */
    uint16_t    numDopplerBins;

    /*! @brief  CFAR Configuration */
    DPU_CFARProc_CfarCfg   cfarCfg;

} DPU_RangeCFARProcHWA_StaticConfig;

/**
 * @brief
 *  HWA CFAR configuration
 *
 * @details
 *  The structure is used to hold the HWA configuration used for CFAR
 *
 *  \ingroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE
 *
 *
 */
typedef struct DPU_RangeCfarProcHWA_Config_t
{
    /*! @brief  Hardware resources */
    DPU_RangeCFARProcHWA_HW_Resources res;

    /*! @brief  Static configuration */
    DPU_RangeCFARProcHWA_StaticConfig staticCfg;
}DPU_RangeCfarProcHWA_Config;

/**
 * @brief
 *  Output parameters populated during Processing time
 *
 * @details
 *  The structure is used to hold the output parameters
 *
 *
 *  \ingroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef struct DPU_RangeCFARProcHWA_OutParams_t
{
    /*! @brief     CFARProc statistics */
    DPU_RangeCFARProc_Stats stats;

    /*! @brief      Number of CFAR detected points*/
    uint32_t numCfarDetectedPoints;
}DPU_RangeCFARProcHWA_OutParams;

/**
 * @brief
 *  CFARHwa DPU Handle
 *
 *
 *  \ingroup DPU_RANGECFARPROC_EXTERNAL_DATA_STRUCTURE
 *
 */
typedef void* DPU_RangeCFARProcHWA_Handle;

/**
 *  @b Description
 *  @n
 *      The function is RangeCFARProcHWA DPU initialization function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]  initCfg                 Pointer to initialization configuration
 *  @param[in]  subframeCounter         subFrame index for dpu initialization
 *  @param[in]  errCode                 Pointer to errCode generates from the API
 *
 *  \ingroup    DPU_RANGECFARPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid RangeCFARProcHWA handle
 *  @retval
 *      Error       - NULL
 */
DPU_RangeCFARProcHWA_Handle DPU_RangeCFARProcHWA_init
(
    DPU_RangeCFARProcHWA_InitParams *initCfg,
    volatile uint8_t         subframeCounter,
    int32_t*            errCode
)
#if defined(SUBSYS_M4) && SUBSYS_M4
 __attribute__((section (".customCode")));
#else
;
#endif

/**
 *  @b Description
 *  @n
 *      The function is RangeCFARProcHWA DPU configuration function. It saves buffer pointer and configurations
 *  including system resources and configures EDMA for runtime range processing.
 *
 *  @pre    DPU_RangeCFARProcHWA_init() has been called
 *
 *  @param[in]  handle                  CFARProcHWA DPU handle
 *  @param[in]  cfarHwaCfg              Pointer to CFARProcHWA configuration data structure
 *
 *  \ingroup    DPU_RANGECFARPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeCFARProcHWA_config
(
    DPU_RangeCFARProcHWA_Handle           handle,
    DPU_RangeCfarProcHWA_Config      *cfarHwaCfg
);

/**
 *  @b Description
 *  @n
 *      The function is RangeCFARProcHWA DPU process function. It performs CFAR detection using HWA
 *
 *  @pre    DPU_RangeCFARProcHWA_init() has been called
 *
 *  @param[in]  handle                  RangeCFARProcHWA DPU handle
 *  @param[in]  cfg                     RangeCFARProcHWA configuration
 *  @param[in]  outParams               DPU output parameters
 *
 *  \ingroup    DPU_RANGECFARPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success = 0
 *  @retval
 *      Error  != 0
 */
int32_t DPU_RangeCFARProcHWA_process
(
    DPU_RangeCFARProcHWA_Handle            handle,
    DPU_RangeCfarProcHWA_Config       *cfg,
    DPU_RangeCFARProcHWA_OutParams         *outParams
);

/**
 *  @b Description
 *  @n
 *      The function is RangeCFARProcHWA DPU deinitialization function. It frees up the
 *   resources allocated during initialization.
 *
 *  @pre    DPU_RangeCFARProcHWA_init() has been called
 *
 *  @param[in]  handle           CFARProcHWA DPU handle
 *
 *  \ingroup    DPU_RANGECFARPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
int32_t DPU_RangeCFARProcHWA_deinit
(
    DPU_RangeCFARProcHWA_Handle handle
);

#ifdef __cplusplus
}
#endif

#endif
