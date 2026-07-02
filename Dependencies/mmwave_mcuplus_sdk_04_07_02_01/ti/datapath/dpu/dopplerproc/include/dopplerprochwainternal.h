/**
 *   @file  dopplerprochwainternal.h
 *
 *   @brief
 *      Implements Data path doppler processing functionality.
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

/**************************************************************************
 *************************** Include Files ********************************
 **************************************************************************/
#ifndef DOPPLERPROC_HWA_INTERNAL_H
#define DOPPLERPROC_HWA_INTERNAL_H

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

/* DPIF Components Include Files */
#include <ti/datapath/dpif/dpif_detmatrix.h>
#include <ti/datapath/dpif/dpif_radarcube.h>

/* mmWave SDK Data Path Include Files */
#include <ti/datapath/dpif/dp_error.h>
#include <ti/datapath/dpu/dopplerproc/dopplerprochwa.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief
 *  DC estimation parameters for HWA
 *
 * @details
 *  The structure holds DC estimation parameters to configure HWA.
 *  The HWA DC estimation consists of the following:
 *  - accumulate of input samples
 *  - multiply accumulated values by dcestScale, and right shift by (8 + 6 + dcestShift)
 *  
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_DC_estimParams_t
{
    /*! @brief DC estimation scale value in Q8 format, 9bit wide (scale range 0 to 511/256) */
    uint16_t dcestScale;

    /*! @brief Programming value for DC estimation shift. Total
               right shift of the accumulator after the multiplication by dcestScale is (8+6+dcestShift) */
    uint8_t dcestShift;

    /*! @brief programming value for the scale shift in the input 
               formater, of DC estimation param set */
    uint8_t preProcScaleShift;

} DPU_DopplerProcHWA_DC_estimParams;

/**
 * @brief
 *  dopplerProc DPU internal data Object
 *
 * @details
 *  The structure is used to hold dopplerProc internal data object
 *
 *  \ingroup DPU_DOPPLERPROC_INTERNAL_DATA_STRUCTURE
 */
typedef struct DPU_DopplerProcHWA_Obj_t
{
    /*! @brief HWA Handle */
    HWA_Handle  hwaHandle;
    
    /*! @brief  EDMA driver handle. */
    EDMA_Handle edmaHandle;

    /*! @brief  EDMA configuration for Input data (Radar cube -> HWA memory). */
    DPU_DopplerProc_Edma edmaIn;

    /*! @brief HWA Processing Done semaphore Handle */
    SemaphoreP_Object  hwaDoneSemaHandle;

    /*! @brief EDMA Done semaphore Handle */
    SemaphoreP_Object  edmaDoneSemaHandle;
    
    /*! @brief Flag to indicate if DPU is in processing state */
    bool inProgress;
          
    /*! @brief  DMA trigger source channel for Ping/Pong param set */
    uint8_t hwaDmaTriggerSourcePingPong[2];
    
    /*! @brief  HWA number of loops */
    uint16_t hwaNumLoops;
    
    /*! @brief  HWA start paramset index */
    uint8_t  hwaParamStartIdx;
    
    /*! @brief  HWA stop paramset index */
    uint8_t  hwaParamStopIdx;
    
    /*! @brief  HWA memory bank addresses */
    uint32_t hwaMemBankAddr[DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS];

    /*! @brief  Number of Doppler chirps. */
    uint16_t    numDopplerChirps;

    /*! @brief  Number of Doppler bins */
    uint16_t    numDopplerBins;

    /*! @brief  HWA translated memory addresses, first index for ping/pong, second index for source/destination */
    uint32_t    hwaLocMemAddr[2][2];

    DPU_DopplerProcHWA_DC_estimParams dcEstPar;
}DPU_DopplerProcHWA_Obj;


#ifdef __cplusplus
}
#endif

#endif
