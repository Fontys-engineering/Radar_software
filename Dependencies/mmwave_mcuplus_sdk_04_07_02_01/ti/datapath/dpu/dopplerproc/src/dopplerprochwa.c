/**
 *   @file  dopplerprochwa.c
 *
 *   @brief
 *      Implements Data path Doppler processing Unit using HWA.
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

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>


/* MCU+SDK Include files */
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/HeapP.h>
#include <kernel/dpl/ClockP.h>

/* Utils */
#include <ti/utils/mathutils/mathutils.h>

/* Data Path Include files */
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpu/dopplerproc/dopplerprochwa.h>
#include <ti/datapath/dpu/dopplerproc/include/dopplerprochwainternal.h>

#include <ti/control/mmwavelink/mmwavelink.h>

/* Flag to check input parameters */
#define DEBUG_CHECK_PARAMS   0

/* HWA ping/pong memory bank indices for the 1st param doing FFT + log + magnitude */
#define DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_SRC_PING   0
#define DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_SRC_PONG   2
#define DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_DST_PING   4
#define DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_DST_PONG   6

/* HWA ping/pong memory bank indices for the 2nd param doing sum of log magnitudes of 1st param,
 * the sum performed using FFT */
#define DPU_DOPPLERPROCHWA_MEM_BANK_INDX_SUM_SRC_PING         DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_DST_PING
#define DPU_DOPPLERPROCHWA_MEM_BANK_INDX_SUM_SRC_PONG         DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_DST_PONG
#define DPU_DOPPLERPROCHWA_MEM_BANK_INDX_SUM_DST_PING         DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_SRC_PING
#define DPU_DOPPLERPROCHWA_MEM_BANK_INDX_SUM_DST_PONG         DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_SRC_PONG

/* HWA ping/pong memory bank addresses for 1st param doing FFT + log + magnitude */
#define DPU_DOPPLERPROCHWA_FFTLOGMAG_SRCADDR_PING   HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_SRC_PING])
#define DPU_DOPPLERPROCHWA_FFTLOGMAG_SRCADDR_PONG   HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_SRC_PONG])
#define DPU_DOPPLERPROCHWA_FFTLOGMAG_DSTADDR_PING   HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_DST_PING])
#define DPU_DOPPLERPROCHWA_FFTLOGMAG_DSTADDR_PONG   HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaMemBankAddr[DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_DST_PONG])

/* HWA ping/pong memory bank addresses for 2nd param doing sum of log magnitudes of 1st param,
 * sum performed using FFT */
#define DPU_DOPPLERPROCHWA_SUM_SRCADDR_PING    DPU_DOPPLERPROCHWA_FFTLOGMAG_DSTADDR_PING
#define DPU_DOPPLERPROCHWA_SUM_SRCADDR_PONG    DPU_DOPPLERPROCHWA_FFTLOGMAG_DSTADDR_PONG
#define DPU_DOPPLERPROCHWA_SUM_DSTADDR_PING    DPU_DOPPLERPROCHWA_FFTLOGMAG_SRCADDR_PING
#define DPU_DOPPLERPROCHWA_SUM_DSTADDR_PONG    DPU_DOPPLERPROCHWA_FFTLOGMAG_SRCADDR_PONG

#define DPU_DOPPLERPROCHWA_PING 0
#define DPU_DOPPLERPROCHWA_PONG 1

/* User defined heap memory and handle */
DPU_DopplerProcHWA_Obj dopplerProcObjPool[RL_MAX_SUBFRAMES] __attribute__((aligned(HeapP_BYTE_ALIGNMENT)));

/*===========================================================
 *                    Internal Functions
 *===========================================================*/
void DPU_DopplerProcHWA_calcDCEstimParams
(
    uint32_t numChirps, 
    DPU_DopplerProcHWA_DC_estimParams *dcEstPar
);

/**
 *  @b Description
 *  @n
 *      HWA processing completion call back function.
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static void DPU_DopplerProcHWA_hwaDoneIsrCallback(uint32_t threadIdx, void * arg)
{
    if (arg != NULL)
    {
        SemaphoreP_post((SemaphoreP_Object*)arg);
    }
}

/**
 *  @b Description
 *  @n
 *      EDMA completion call back function.
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 */
static void DPU_DopplerProcHWA_edmaDoneIsrCallback(Edma_IntrHandle intrHandle,
   void *args)
{
    if (args != NULL)
    {
        SemaphoreP_post((SemaphoreP_Object*)args);
    }
}

/**
 *  @b Description
 *  @n
 *      Calculates DC estimation configuration parameters for HWA
 *
 *  @param[in]  numChirps    - Number of chirps (number of samples that are averaged)
 *  @param[out] dcEstPar     - Pointer to structure with DC estimation parameters
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval None
 */
void DPU_DopplerProcHWA_calcDCEstimParams(uint32_t numChirps, DPU_DopplerProcHWA_DC_estimParams *dcEstPar)
{
    uint32_t shiftCeil;
    uint32_t shiftActual;
    uint32_t prePorcLeftShift;
    uint16_t   dcEstScale;
    uint32_t   shiftedValue;
    float      temp;
#define ONEQ8F 256.0F

    shiftCeil = mathUtils_ceilLog2(numChirps);

    shiftActual = shiftCeil;
    if (shiftActual < 6U)
    {
        shiftActual = 6U;
    }
    /* Gain applied in input formatter during the DC estimation time */
    prePorcLeftShift = shiftActual -  shiftCeil;
    shiftedValue = (uint32_t)1U << shiftCeil;
    temp = ONEQ8F * (float) (shiftedValue) / (float) numChirps + 0.5F;
    dcEstScale = (uint16_t) (temp);

    /* Scale value in Q8 format */
    dcEstPar->dcestScale = (uint16_t)dcEstScale;
    /* HWA Programming value */
    dcEstPar->dcestShift = (uint8_t)shiftActual - 6U;
    /* HWA input formater scale programming value for the DC esimation stage */
    dcEstPar->preProcScaleShift = 8U - (uint8_t)prePorcLeftShift;

}

/**
 *  @b Description
 *  @n
 *      Configures HWA for Doppler processing.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval error code.
 */
static inline int32_t DPU_DopplerProcHWA_configHwa
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{
    HWA_ParamConfig         hwaParamCfg;
    HWA_InterruptConfig     paramISRConfig;
    uint32_t                paramsetIdx = 0U;
    int32_t                 retVal = 0;
    uint8_t                 destChan;
    uint32_t                pingPongIdx;
    uint32_t                txAntIdx;
    uint8_t                 triggerMode;
    uint32_t                addrIdx;


    /* Check if we have the correct number of paramsets.*/
    if(cfg->staticCfg.staticClutterCfg.isEnabled)
    {
        if(cfg->hwRes.hwaCfg.numParamSets !=
                DPU_DOPPLERPROCHWA_NUM_HWA_PARAMS_CLUTTER_REMOVAL_ENABLED(cfg->staticCfg.numTxAntennas))
        {
            retVal = DPU_DOPPLERPROCHWA_EHWARES;
            goto exit;
        }

        DPU_DopplerProcHWA_calcDCEstimParams(cfg->staticCfg.numDopplerChirps, &obj->dcEstPar);
    }
    else
    {
        if(cfg->hwRes.hwaCfg.numParamSets !=
                DPU_DOPPLERPROCHWA_NUM_HWA_PARAMS_CLUTTER_REMOVAL_DISABLED(cfg->staticCfg.numTxAntennas))
        {
            retVal = DPU_DOPPLERPROCHWA_EHWARES;
            goto exit;
        }
    }

    /********************************************************************************/
    for (pingPongIdx = 0U; pingPongIdx < 2U; pingPongIdx++)
    {
        triggerMode = HWA_TRIG_MODE_DMA;
        addrIdx = 0;

        if (cfg->staticCfg.staticClutterCfg.isEnabled)
        {
            /*************************************************************************/
            /******************** Clutter Removal ENABLED ****************************/
            /*************************************************************************/
            /*******************/
            /* Clutter Removal */
            /*******************/
            for (txAntIdx = 0; txAntIdx < cfg->staticCfg.numTxAntennas; txAntIdx++)
            {
                /*******************/
                /* DC Estimation   */
                /*******************/
                (void)memset((void*) &hwaParamCfg, 0, sizeof(HWA_ParamConfig));
                hwaParamCfg.triggerMode = triggerMode;
                triggerMode = HWA_TRIG_MODE_IMMEDIATE;
                hwaParamCfg.triggerSrc = obj->hwaDmaTriggerSourcePingPong[pingPongIdx];
                hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

                hwaParamCfg.source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaLocMemAddr[pingPongIdx][addrIdx]) + txAntIdx * sizeof(cmplx16ImRe_t) * cfg->staticCfg.numRxAntennas * cfg->staticCfg.numDopplerChirps;
                hwaParamCfg.source.srcAcnt = cfg->staticCfg.numDopplerChirps - 1U; //size in samples - 1

                hwaParamCfg.source.srcAIdx = (int32_t)cfg->staticCfg.numRxAntennas * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBcnt = (uint16_t)cfg->staticCfg.numRxAntennas - 1U;
                hwaParamCfg.source.srcBIdx = (int32_t)sizeof(cmplx16ImRe_t);

                hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
                hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
                hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.source.srcConjugate = 0;
                hwaParamCfg.source.srcScale = obj->dcEstPar.preProcScaleShift;

                hwaParamCfg.dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaLocMemAddr[pingPongIdx][addrIdx^1U]) + txAntIdx * sizeof(cmplx16ImRe_t) * cfg->staticCfg.numRxAntennas;
                hwaParamCfg.dest.dstAcnt = 1 - 1; //Zero samples output: Here specify one sample which will be skipped, see below  dstSkipInit=1
                hwaParamCfg.dest.dstAIdx = (int32_t)cfg->staticCfg.numVirtualAntennas * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstBIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
                hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
                hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.dest.dstConjugate = 0;
                hwaParamCfg.dest.dstScale = 0;
                hwaParamCfg.dest.dstSkipInit = 1;

                hwaParamCfg.accelModeArgs.fftMode.fftEn = 0;

                /* DC estimation ENABLED / DC subtraction DISABLED */
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCEST;

                hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
                hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;

                retVal = HWA_configParamSet(obj->hwaHandle,
                                            (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                            &hwaParamCfg, NULL);
                if (retVal != 0)
                {
                    goto exit;
                }
                retVal = HWA_disableParamSetInterrupt(obj->hwaHandle,
                                                      (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                                      HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
                if (retVal != 0)
                {
                    goto exit;
                }
                paramsetIdx++;

                /*****************************************************/
                /* DC Subtraction + Permute tx index and chirp index */
                /*****************************************************/
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
                hwaParamCfg.dest.dstAcnt = cfg->staticCfg.numDopplerChirps - 1U; //this is samples - 1
                hwaParamCfg.source.srcScale = 8;
                hwaParamCfg.dest.dstSkipInit = 0;
                /* DC estimation DISABLED / DC subtraction ENABLED */
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_ENABLE;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCEST;

                retVal = HWA_configParamSet(obj->hwaHandle,
                                            (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                            &hwaParamCfg, NULL);
                if (retVal != 0)
                {
                    goto exit;
                }
                retVal = HWA_disableParamSetInterrupt(obj->hwaHandle,
                                                      (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                                      HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
                if (retVal != 0)
                {
                    goto exit;
                }
                paramsetIdx++;

            }//End of txAntIdx for loop

            addrIdx ^= 1U;

            /*******************************/
            /********* Doppler FFT *********/
            /*******************************/
            (void)memset((void*) &hwaParamCfg, 0, sizeof(HWA_ParamConfig));
            hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            hwaParamCfg.triggerSrc = obj->hwaDmaTriggerSourcePingPong[pingPongIdx];
            hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

            hwaParamCfg.source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaLocMemAddr[pingPongIdx][addrIdx]);
            hwaParamCfg.source.srcAcnt = cfg->staticCfg.numDopplerChirps - 1U; //size in samples - 1

            hwaParamCfg.source.srcAIdx = (int32_t)cfg->staticCfg.numVirtualAntennas * (int32_t)sizeof(cmplx16ImRe_t);
            hwaParamCfg.source.srcBcnt = (uint16_t)cfg->staticCfg.numVirtualAntennas - 1U;
            hwaParamCfg.source.srcBIdx = (int32_t)sizeof(cmplx16ImRe_t);

            hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
            hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
            hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
            hwaParamCfg.source.srcConjugate = 0;
            hwaParamCfg.source.srcScale = 0;

            hwaParamCfg.dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaLocMemAddr[pingPongIdx][addrIdx^1U]);
            hwaParamCfg.dest.dstAcnt = cfg->staticCfg.numDopplerBins - 1U; //this is samples - 1
            hwaParamCfg.dest.dstAIdx = (int32_t)cfg->staticCfg.numVirtualAntennas * (int32_t)sizeof(uint16_t);
            hwaParamCfg.dest.dstBIdx = (int32_t)sizeof(uint16_t);
            hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
            hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
            hwaParamCfg.dest.dstSign = HWA_SAMPLES_UNSIGNED;
            hwaParamCfg.dest.dstConjugate = 0;
            hwaParamCfg.dest.dstScale = 0;
            hwaParamCfg.dest.dstSkipInit = 0;

            hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
            hwaParamCfg.accelModeArgs.fftMode.fftSize = cfg->staticCfg.log2NumDopplerBins;

            /* scaling is enabled in all stages except for the first stage which is defined by user*/
            if(cfg->hwRes.hwaCfg.firstStageScaling == DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED)
            {
                /* Enable scaling on all stages except first one.*/
                hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = (cfg->staticCfg.numDopplerBins - 1U) >> 1U;
            }
            else
            {
                /* Enable scaling on all stages.*/
                hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = (cfg->staticCfg.numDopplerBins - 1U);
            }


            hwaParamCfg.accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE;
            hwaParamCfg.accelModeArgs.fftMode.windowStart = (uint16_t)cfg->hwRes.hwaCfg.winRamOffset;
            hwaParamCfg.accelModeArgs.fftMode.winSymm = cfg->hwRes.hwaCfg.winSym;

            hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_ENABLED;
            hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
            hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

            retVal = HWA_configParamSet(obj->hwaHandle,
                                        (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                        &hwaParamCfg, NULL);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Make sure DMA interrupt/trigger is disabled for this paramset*/
            retVal = HWA_disableParamSetInterrupt(obj->hwaHandle,
                                                  (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                                  HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
            if (retVal != 0)
            {
                goto exit;
            }
            paramsetIdx++;
            addrIdx ^= 1U;
        }
        else
        {
            /*************************************************************************/
            /******************** Clutter Removal DISABLED ***************************/
            /*************************************************************************/
            /******************/
            /* DOPPLER FFT    */
            /******************/
            for (txAntIdx = 0; txAntIdx < cfg->staticCfg.numTxAntennas; txAntIdx++)
            {
                (void)memset((void*) &hwaParamCfg, 0, sizeof(HWA_ParamConfig));
                hwaParamCfg.triggerMode = triggerMode;
                triggerMode = HWA_TRIG_MODE_IMMEDIATE;
                hwaParamCfg.triggerSrc = obj->hwaDmaTriggerSourcePingPong[pingPongIdx];
                hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

                hwaParamCfg.source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaLocMemAddr[pingPongIdx][addrIdx]) + txAntIdx * sizeof(cmplx16ImRe_t) * cfg->staticCfg.numRxAntennas * cfg->staticCfg.numDopplerChirps;
                hwaParamCfg.source.srcAcnt = cfg->staticCfg.numDopplerChirps - 1U; //size in samples - 1

                hwaParamCfg.source.srcAIdx = (int32_t)cfg->staticCfg.numRxAntennas * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBcnt = (uint16_t)cfg->staticCfg.numRxAntennas - 1U;
                hwaParamCfg.source.srcBIdx = (int32_t)sizeof(cmplx16ImRe_t);

                hwaParamCfg.source.srcAcircShift = 0;
                hwaParamCfg.source.srcAcircShiftWrap = 0;

                hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX;
                hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
                hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.source.srcConjugate = 0; //no conjugate
                hwaParamCfg.source.srcScale = 0;

                hwaParamCfg.dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaLocMemAddr[pingPongIdx][addrIdx^1U]) + txAntIdx * sizeof(uint16_t) * cfg->staticCfg.numRxAntennas;
                hwaParamCfg.dest.dstAcnt = cfg->staticCfg.numDopplerBins - 1U; //this is samples - 1
                hwaParamCfg.dest.dstAIdx = (int32_t)cfg->staticCfg.numVirtualAntennas * (int32_t)sizeof(uint16_t);
                hwaParamCfg.dest.dstBIdx = (int32_t)sizeof(uint16_t);
                hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
                hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
                hwaParamCfg.dest.dstSign = HWA_SAMPLES_UNSIGNED;
                hwaParamCfg.dest.dstConjugate = 0; //no conjugate
                hwaParamCfg.dest.dstScale = 0;
                hwaParamCfg.dest.dstSkipInit = 0;

                hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
                hwaParamCfg.accelModeArgs.fftMode.fftSize = cfg->staticCfg.log2NumDopplerBins;

                /* scaling is enabled in all stages except for the first stage which is defined by user*/
                if(cfg->hwRes.hwaCfg.firstStageScaling == DPU_DOPPLERPROCHWA_FIRST_SCALING_DISABLED)
                {
                    /* Enable scaling on all stages except first one.*/
                    hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = (cfg->staticCfg.numDopplerBins - 1U) >> 1U;
                }
                else
                {
                    /* Enable scaling on all stages.*/
                    hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = (cfg->staticCfg.numDopplerBins - 1U);
                }


                hwaParamCfg.accelModeArgs.fftMode.windowEn = HWA_FEATURE_BIT_ENABLE;
                hwaParamCfg.accelModeArgs.fftMode.windowStart = (uint16_t)cfg->hwRes.hwaCfg.winRamOffset;
                hwaParamCfg.accelModeArgs.fftMode.winSymm = cfg->hwRes.hwaCfg.winSym;

                hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_ENABLED;
                hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

                retVal = HWA_configParamSet(obj->hwaHandle,
                                            (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                            &hwaParamCfg, NULL);
                if (retVal != 0)
                {
                    goto exit;
                }

                /* Make sure DMA interrupt/trigger is disabled for this paramset*/
                retVal = HWA_disableParamSetInterrupt(obj->hwaHandle,
                                                      (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                                      HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
                if (retVal != 0)
                {
                    goto exit;
                }
                paramsetIdx++;
            }//End of txAntIdx for loop
            addrIdx ^= 1U;
        }

        /*************************************************************************/
        /*******************  LOG2 MAGNITUDE SUMMATION ***************************/
        /*************************************************************************/
        (void)memset( (void*) &hwaParamCfg, 0, sizeof(HWA_ParamConfig));
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT;

        hwaParamCfg.source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaLocMemAddr[pingPongIdx][addrIdx]);
        hwaParamCfg.source.srcAcnt = (uint16_t)cfg->staticCfg.numVirtualAntennas - 1U; //size in samples - 1

        hwaParamCfg.source.srcAIdx = (int32_t)sizeof(uint16_t);
        hwaParamCfg.source.srcBcnt = cfg->staticCfg.numDopplerBins - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t)cfg->staticCfg.numVirtualAntennas * (int32_t)sizeof(uint16_t);

        hwaParamCfg.source.srcAcircShift = 0;
        hwaParamCfg.source.srcAcircShiftWrap = 0;

        hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_REAL;
        hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT;
        hwaParamCfg.source.srcSign = HWA_SAMPLES_UNSIGNED;
        hwaParamCfg.source.srcConjugate = 0; //no conjugate
        hwaParamCfg.source.srcScale = 3;

        hwaParamCfg.dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(obj->hwaLocMemAddr[pingPongIdx][addrIdx^1U]);

        hwaParamCfg.dest.dstAcnt = 1 - 1; //get only bin zero
        hwaParamCfg.dest.dstAIdx = (int32_t)sizeof(uint16_t);
        hwaParamCfg.dest.dstBIdx = (int32_t)sizeof(uint16_t); // size of one output sample
        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT;
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_UNSIGNED;
        hwaParamCfg.dest.dstConjugate = 0; //no conjugate
        hwaParamCfg.dest.dstScale = 8;
        hwaParamCfg.dest.dstSkipInit = 0; // no skipping

        if(cfg->staticCfg.numVirtualAntennas == 1U)
        {
            /*If number of virtual antennas is 1, do not use FFT to compute sum magnitude.*/
            hwaParamCfg.accelModeArgs.fftMode.fftEn = 0;
            hwaParamCfg.accelModeArgs.fftMode.fftSize = 1;
        }
        else
        {
            hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
            hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(cfg->staticCfg.numVirtualAntennas);
        }
        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0x3FF;
        hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
        hwaParamCfg.accelModeArgs.fftMode.windowStart = 0;
        hwaParamCfg.accelModeArgs.fftMode.winSymm = cfg->hwRes.hwaCfg.winSym;

        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        retVal = HWA_configParamSet(obj->hwaHandle,
                                    (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                    &hwaParamCfg,
                                    NULL);
        if (retVal != 0)
        {
            goto exit;
        }

        /************ Enable the DMA hookup to this paramset so that data gets copied out ***********/
        /* First, make sure all DMA interrupt/trigger are disabled for this paramset*/
        retVal = HWA_disableParamSetInterrupt(obj->hwaHandle,
                                              (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx,
                                              HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1);
        if (retVal != 0)
        {
            goto exit;
        }

        retVal = HWA_getDMAChanIndex(obj->hwaHandle,
                                     (uint8_t)cfg->hwRes.edmaCfg.edmaOut.pingPong[pingPongIdx].channel,
                                     &destChan);
        if (retVal != 0)
        {
            goto exit;
        }
        /* Now enable interrupt */
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
        paramISRConfig.dma.dstChannel = destChan;
        paramISRConfig.cpu.callbackArg = NULL;
        retVal = HWA_enableParamSetInterrupt(obj->hwaHandle, (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)paramsetIdx, &paramISRConfig);
        if (retVal != 0)
        {
            goto exit;
        }
        paramsetIdx++;

    }//End of Ping/Pong Index

    if (cfg->staticCfg.staticClutterCfg.isEnabled)
    {
        HWA_CommonConfig    hwaCommonConfig;
        (void)memset((void*) &hwaCommonConfig, 0, sizeof(HWA_CommonConfig));

        /* Config Common Registers */
        hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_DCEST_SCALESHIFT;

        hwaCommonConfig.dcEstimateConfig.scale = obj->dcEstPar.dcestScale;
        hwaCommonConfig.dcEstimateConfig.shift = obj->dcEstPar.dcestShift;

        retVal = HWA_configCommon(obj->hwaHandle, &hwaCommonConfig);
        if (retVal != 0)
        {
            goto exit;
        }

    }

exit:
    return(retVal);
}

/**
 *  @b Description
 *  @n
 *  Doppler DPU EDMA configuration.
 *  This implementation of doppler processing involves Ping/Pong 
 *  Mechanism, hence there are two sets of EDMA transfer.
 *
 *  @param[in] obj    - DPU obj
 *  @param[in] cfg    - DPU configuration
 *
 *  \ingroup    DPU_DOPPLERPROC_INTERNAL_FUNCTION
 *
 *  @retval EDMA error code, see EDMA API.
 */
static inline int32_t DPU_DopplerProcHWA_configEdma
(
    DPU_DopplerProcHWA_Obj      *obj,
    DPU_DopplerProcHWA_Config   *cfg
)
{
    int32_t             retVal = SystemP_SUCCESS;
    cmplx16ImRe_t       *radarCubeBase = (cmplx16ImRe_t *)cfg->hwRes.radarCube.data;
    uint16_t            *detMatrixBase = (uint16_t *)cfg->hwRes.detMatrix.data;
    int16_t             sampleLenInBytes = (int16_t)sizeof(cmplx16ImRe_t);
    uint32_t            sizeOfAbsTransfer = cfg->staticCfg.numDopplerBins;
    uint32_t            sizeOfAbsTransferBytes = sizeOfAbsTransfer * sizeof(uint16_t);
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncACfg     syncACfg;
    DPEDMA_syncABCfg    syncABCfg;
    uint32_t            pingPongIdx;
    uint32_t            addrInIdx = 0;
    uint32_t            addrOutIdx;
    Edma_EventCallback  doneCllbackFunc[2] = {NULL, DPU_DopplerProcHWA_edmaDoneIsrCallback};
    uint32_t            doneCllbackFuncArg[2] = {0, (uint32_t)&obj->edmaDoneSemaHandle};
    bool                doneTransferCompletionEnabled[2] = {false, true};

    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    
    if (cfg->staticCfg.staticClutterCfg.isEnabled)
    {
        addrOutIdx = 1; /*  mem[0] -> DC_subtract -> mem[1] -> Dop_FFT -> mem[0] -> Summation -> mem[addrOutIdx] -> EDMA out */
    }
    else
    {
        addrOutIdx = 0; /*  mem[0] -> Dop_FFT -> mem[1] -> Summation -> mem[addrOutIdx] -> EDMA out */
    }

    for (pingPongIdx = 0U; pingPongIdx < 2U; pingPongIdx++)
    {
        /***************************************************************************
         *  PROGRAM DMA channel  to transfer 2D abs data from accelerator output
         *  buffer (ping) to L3
         **************************************************************************/
        chainingCfg.chainingChannel               = (uint8_t)cfg->hwRes.edmaCfg.edmaIn.pingPong[pingPongIdx].channel;
        chainingCfg.isIntermediateChainingEnabled = true;
        chainingCfg.isFinalChainingEnabled        = false;

        syncACfg.srcAddress  = (uint32_t) obj->hwaLocMemAddr[pingPongIdx][addrOutIdx];
        syncACfg.destAddress = (uint32_t)(&detMatrixBase[pingPongIdx * sizeOfAbsTransfer]);
        syncACfg.aCount      = (uint16_t)sizeOfAbsTransferBytes;
        syncACfg.bCount      = cfg->staticCfg.numRangeBins / 2U; //factor of 2 due to ping/pong
        syncACfg.srcBIdx     = 0;
        syncACfg.dstBIdx     = (int32_t)sizeOfAbsTransferBytes * 2;//factor of 2 due to ping/pong

        retVal = DPEDMA_configSyncA_singleFrame(cfg->hwRes.edmaCfg.edmaHandle,
                                    &cfg->hwRes.edmaCfg.edmaOut.pingPong[pingPongIdx],
                                    &chainingCfg,
                                    &syncACfg,
                                    true, //isEventTriggered
                                    false,//isIntermediateTransferInterruptEnabled
                                    doneTransferCompletionEnabled[pingPongIdx],//isTransferCompletionEnabled
                                    doneCllbackFunc[pingPongIdx], //transferCompletionCallbackFxn
                                    (void*)doneCllbackFuncArg[pingPongIdx], //transferCompletionCallbackFxnArg
                                    cfg->hwRes.edmaCfg.intrObj);

        if (retVal != SystemP_SUCCESS)
        {
            goto exit;
        }

        /******************************************************************************************
        *  PROGRAM DMA channel  to transfer data from Radar cube to accelerator input buffer (ping/pong)
        ******************************************************************************************/
        chainingCfg.chainingChannel               = (uint8_t)cfg->hwRes.edmaCfg.edmaHotSig.pingPong[pingPongIdx].channel;
        chainingCfg.isIntermediateChainingEnabled = true;
        chainingCfg.isFinalChainingEnabled        = true;

        syncABCfg.srcAddress  = (uint32_t)(&radarCubeBase[pingPongIdx * 1U]);
        syncABCfg.destAddress = (uint32_t) obj->hwaLocMemAddr[pingPongIdx][addrInIdx];
        syncABCfg.aCount      = (uint16_t)sampleLenInBytes;
        syncABCfg.bCount      = cfg->staticCfg.numRxAntennas * cfg->staticCfg.numDopplerChirps *  cfg->staticCfg.numTxAntennas;
        syncABCfg.cCount      = cfg->staticCfg.numRangeBins / 2U;//factor of 2 due to ping/pong
        syncABCfg.srcBIdx     = (int32_t)cfg->staticCfg.numRangeBins * (int32_t)sampleLenInBytes;
        syncABCfg.dstBIdx     = (int32_t)sampleLenInBytes;
        syncABCfg.srcCIdx     = 2 * sampleLenInBytes;//factor of 2 due to ping/pong
        syncABCfg.dstCIdx     = 0;

        retVal = DPEDMA_configSyncAB(cfg->hwRes.edmaCfg.edmaHandle,
                                     &cfg->hwRes.edmaCfg.edmaIn.pingPong[pingPongIdx],
                                     &chainingCfg,
                                     &syncABCfg,
                                     false,//isEventTriggered
                                     true, //isIntermediateTransferCompletionEnabled
                                     false,//isTransferCompletionEnabled
                                     NULL, //transferCompletionCallbackFxn
                                     NULL, //transferCompletionCallbackFxnArg
                                     NULL);

        if (retVal != SystemP_SUCCESS)
        {
            goto exit;
        }

        /******************************************************************************************
        *  PROGRAM DMA channel to trigger HWA for processing of input (ping)
        ******************************************************************************************/
        retVal = DPEDMAHWA_configOneHotSignature(cfg->hwRes.edmaCfg.edmaHandle,
                                                 &cfg->hwRes.edmaCfg.edmaHotSig.pingPong[pingPongIdx],
                                                 obj->hwaHandle,
                                                 obj->hwaDmaTriggerSourcePingPong[pingPongIdx],
                                                 false);

        if (retVal != SystemP_SUCCESS)
        {
            goto exit;
        }
    }

exit:
    return(retVal);
} 

/*===========================================================
 *                    Doppler Proc External APIs
 *===========================================================*/

/**
 *  @b Description
 *  @n
 *      dopplerProc DPU init function. It allocates memory to store
 *  its internal data object and returns a handle if it executes successfully.
 *
 *  @param[in]   initCfg Pointer to initial configuration parameters
 *  @param[in]   subframeCounter Sub frame counter
 *  @param[out]  errCode Pointer to errCode generates by the API
 *
 *  \ingroup    DPU_DOPPLERPROC_EXTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - valid handle
 *  @retval
 *      Error       - NULL
 */
DPU_DopplerProcHWA_Handle DPU_DopplerProcHWA_init
(
    DPU_DopplerProcHWA_InitParams *initCfg,
    volatile uint8_t              subframeCounter,
    int32_t                       *errCode
)
{
    DPU_DopplerProcHWA_Obj  *obj = NULL;
    HWA_MemInfo             hwaMemInfo;
    uint32_t                i;
    int32_t                 status = SystemP_SUCCESS;

    *errCode       = 0;
    
    if((initCfg == NULL) || (initCfg->hwaHandle == NULL))
    {
        *errCode = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    DebugP_assert(subframeCounter < RL_MAX_SUBFRAMES);

    /* Allocate memory */
    obj = (DPU_DopplerProcHWA_Obj*)&dopplerProcObjPool[subframeCounter];

    /* Initialize memory */
    (void)memset((void *)obj, 0, sizeof(DPU_DopplerProcHWA_Obj));
    
    /* Save init config params */
    obj->hwaHandle   = initCfg->hwaHandle;

    /* Create DPU semaphores */
    status = SemaphoreP_constructBinary(&obj->edmaDoneSemaHandle, 0);
    if(SystemP_SUCCESS != status)
    {
        *errCode = DPU_DOPPLERPROCHWA_ESEMA;
        goto exit;
    }

    status = SemaphoreP_constructBinary(&obj->hwaDoneSemaHandle, 0);
    if(SystemP_SUCCESS != status)
    {
        *errCode = DPU_DOPPLERPROCHWA_ESEMA;
        goto exit;
    }

    /* Populate HWA base addresses and offsets. This is done only once, at init time.*/
    *errCode =  HWA_getHWAMemInfo(obj->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {       
        goto exit;
    }
    
    /* check if we have enough memory banks*/
    if(hwaMemInfo.numBanks < DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS)
    {    
        *errCode = DPU_DOPPLERPROCHWA_EHWARES;
        goto exit;
    }
    
    for (i = 0; i < DPU_DOPPLERPROCHWA_NUM_HWA_MEMBANKS; i++)
    {
        obj->hwaMemBankAddr[i] = hwaMemInfo.baseAddress + i * hwaMemInfo.bankSize;
    }

    obj->hwaLocMemAddr[0][0] = obj->hwaMemBankAddr[DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_SRC_PING];//DPU_DOPPLERPROCHWA_FFTLOGMAG_SRCADDR_PING;
    obj->hwaLocMemAddr[0][1] = obj->hwaMemBankAddr[DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_DST_PING];//DPU_DOPPLERPROCHWA_FFTLOGMAG_DSTADDR_PING;
    obj->hwaLocMemAddr[1][0] = obj->hwaMemBankAddr[DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_SRC_PONG];//DPU_DOPPLERPROCHWA_FFTLOGMAG_SRCADDR_PONG;
    obj->hwaLocMemAddr[1][1] = obj->hwaMemBankAddr[DPU_DOPPLERPROCHWA_MEM_BANK_INDX_FFTLOGMAG_DST_PONG];//DPU_DOPPLERPROCHWA_FFTLOGMAG_DSTADDR_PONG;
    
exit:    

    if(*errCode < 0)
    {
        if(obj != NULL)
        {
            obj = NULL;
        }
    }
   return ((DPU_DopplerProcHWA_Handle)obj);
}

/**
  *  @b Description
  *  @n
  *   Doppler DPU configuration 
  *
  *  @param[in]   handle     DPU handle.
  *  @param[in]   cfg        Pointer to configuration parameters.
  *
  *  \ingroup    DPU_DOPPLERPROC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success      = 0
  *  @retval
  *      Error       != 0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_config
(
    DPU_DopplerProcHWA_Handle    handle,
    DPU_DopplerProcHWA_Config    *cfg
)
{
    DPU_DopplerProcHWA_Obj   *obj;
    int32_t                  retVal = 0;
#if DEBUG_CHECK_PARAMS
    uint16_t                 expectedWinSamples;
#endif

    obj = (DPU_DopplerProcHWA_Obj *)handle;
    if(obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    
#if DEBUG_CHECK_PARAMS
    /* Validate params */
    if(!cfg ||
       !cfg->hwRes.edmaCfg.edmaHandle ||
       !cfg->hwRes.hwaCfg.window || 
       !cfg->hwRes.radarCube.data ||
       !cfg->hwRes.detMatrix.data
      )
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }

    /* Check if radar cube format is supported by DPU*/
    if(cfg->hwRes.radarCube.datafmt != DPIF_RADARCUBE_FORMAT_1)
    {
        retVal = DPU_DOPPLERPROCHWA_ECUBEFORMAT;
        goto exit;
    }

    /* Check if detection matrix format is supported by DPU*/
    if(cfg->hwRes.detMatrix.datafmt != DPIF_DETMATRIX_FORMAT_1)
    {
        retVal = DPU_DOPPLERPROCHWA_EDETMFORMAT;
        goto exit;
    }
    
/* number of ping or pong banks for HWA processing at each stage */
#define DPU_DOPPLERPROCHWA_NUM_PING_OR_PONG_HWA_BANKS   2

    /* Check if radar cube column fits into allowed HWA memory banks */
    if((cfg->staticCfg.numTxAntennas * cfg->staticCfg.numRxAntennas * 
        cfg->staticCfg.numDopplerChirps * sizeof(cmplx16ImRe_t)) >
       DPU_DOPPLERPROCHWA_NUM_PING_OR_PONG_HWA_BANKS * (SOC_HWA_MEM_SIZE/SOC_HWA_NUM_MEM_BANKS))
    {
        retVal = DPU_DOPPLERPROCHWA_EEXCEEDHWAMEM;
        goto exit;
    }

    /* Check if abs value of log2 of 2D FFT fits in allowed HWA memory banks */
    if((cfg->staticCfg.numTxAntennas * cfg->staticCfg.numRxAntennas * 
        cfg->staticCfg.numDopplerBins * sizeof(uint16_t)) >
       DPU_DOPPLERPROCHWA_NUM_PING_OR_PONG_HWA_BANKS * (SOC_HWA_MEM_SIZE/SOC_HWA_NUM_MEM_BANKS))
    {
        retVal = DPU_DOPPLERPROCHWA_EEXCEEDHWAMEM;
        goto exit;
    }

    /* Check if number of range bins is even*/
    if((cfg->staticCfg.numRangeBins % 2) != 0)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    
    /* Check if detection matrix size is sufficient*/
    if(cfg->hwRes.detMatrix.dataSize < (cfg->staticCfg.numRangeBins *
                                        cfg->staticCfg.numDopplerBins * sizeof(uint16_t)))
    {
        retVal = DPU_DOPPLERPROCHWA_EDETMSIZE;
        goto exit;
    }

    /* Check window Size */
    if(cfg->hwRes.hwaCfg.winSym == HWA_FFT_WINDOW_NONSYMMETRIC)
    {
        expectedWinSamples = cfg->staticCfg.numDopplerChirps;
    }
    else
    {
        /* odd samples have to be rounded up per HWA */
        expectedWinSamples = (cfg->staticCfg.numDopplerChirps + 1) / 2;
    }

    if (cfg->hwRes.hwaCfg.windowSize != expectedWinSamples * sizeof(int32_t)) 
    {
            retVal = DPU_DOPPLERPROCHWA_EWINDSIZE;
            goto exit;
    }        
#endif

    /* Save necessary parameters to DPU object that will be used during Process time */
    
    /* EDMA parameters needed to trigger first EDMA transfer*/
    obj->edmaHandle  = cfg->hwRes.edmaCfg.edmaHandle;
    (void)memcpy((void*)(&obj->edmaIn), (void *)(&cfg->hwRes.edmaCfg.edmaIn), sizeof(DPU_DopplerProc_Edma));
    
    /*HWA parameters needed for the HWA common configuration*/
    obj->hwaNumLoops      = cfg->staticCfg.numRangeBins / 2U;
    obj->hwaParamStartIdx = (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx;    
    obj->hwaParamStopIdx  = (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx + (uint8_t)cfg->hwRes.hwaCfg.numParamSets - 1U;
    
    /* Disable the HWA */
    retVal = HWA_enable(obj->hwaHandle, 0); 
    if (retVal != 0)
    {
        goto exit;
    }
    
    /* HWA window configuration */
    retVal = HWA_configRam(obj->hwaHandle,
                           HWA_RAM_TYPE_WINDOW_RAM,
                           (uint8_t *)cfg->hwRes.hwaCfg.window,
                           cfg->hwRes.hwaCfg.windowSize, //size in bytes
                           cfg->hwRes.hwaCfg.winRamOffset * sizeof(int32_t)); 
    if (retVal != 0)
    {
        goto exit;
    }
    
    obj->numDopplerChirps = cfg->staticCfg.numDopplerChirps;
    obj->numDopplerBins = cfg->staticCfg.numDopplerBins;

    /*******************************/
    /**  Configure HWA            **/
    /*******************************/
    /*Compute source DMA channels that will be programmed in both HWA and EDMA.   
      The DMA channels are set to be equal to the paramSetIdx used by HWA*/
    /* Ping DMA channel (Ping uses the first paramset)*/  
    obj->hwaDmaTriggerSourcePingPong[DPU_DOPPLERPROCHWA_PING] = (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx;
    /* Pong DMA channel*/  
    obj->hwaDmaTriggerSourcePingPong[DPU_DOPPLERPROCHWA_PONG] = (uint8_t)cfg->hwRes.hwaCfg.paramSetStartIdx +
                                                                (uint8_t)cfg->staticCfg.numTxAntennas + 1U;

    retVal = DPU_DopplerProcHWA_configHwa(obj, cfg);
    if (retVal != 0)
    {
        goto exit;
    }
                    
    /*******************************/
    /**  Configure EDMA           **/
    /*******************************/    
    retVal = DPU_DopplerProcHWA_configEdma(obj, cfg);
    if (retVal != 0)
    {
        goto exit;
    }

exit:
    return retVal;
}

 /**
  *  @b Description
  *  @n Doppler DPU process function. 
  *   
  *  @param[in]   handle     DPU handle.
  *  @param[out]  outParams  Output parameters.
  *
  *  \ingroup    DPU_DOPPLERPROC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success     =0
  *  @retval
  *      Error      !=0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_process
(
    DPU_DopplerProcHWA_Handle    handle,
    DPU_DopplerProcHWA_OutParams *outParams
)
{
    volatile uint64_t   startTime;
    DPU_DopplerProcHWA_Obj *obj;
    int32_t             retVal = 0;
    int32_t             status;
    HWA_CommonConfig    hwaCommonConfig;
    uint32_t            baseAddr, regionId;

    obj = (DPU_DopplerProcHWA_Obj *)handle;
    if (obj == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
        goto exit;
    }
    /* Set inProgress state */
    obj->inProgress = true;

    baseAddr = EDMA_getBaseAddr(obj->edmaHandle);
    DebugP_assert(baseAddr != 0U);

    regionId = EDMA_getRegionId(obj->edmaHandle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    startTime = ClockP_getTimeUsec();

    /**********************************************/
    /* ENABLE NUMLOOPS DONE INTERRUPT FROM HWA */
    /**********************************************/
    retVal = HWA_enableDoneInterrupt(obj->hwaHandle,
									 HWA_THREAD_BACKGROUNDCONTEXT,
                                     DPU_DopplerProcHWA_hwaDoneIsrCallback,
                                     (void *)&obj->hwaDoneSemaHandle);
    if (retVal != 0)
    {
        goto exit;
    }
    
    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    (void)memset((void*) &hwaCommonConfig, 0, sizeof(HWA_CommonConfig));

    /* Config Common Registers */
    hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG |/* numLoops, paramStartIdx, paramStopIdx combined here */
                               HWA_COMMONCONFIG_MASK_TWIDDITHERENABLE |
                               HWA_COMMONCONFIG_MASK_LFSRSEED;

    hwaCommonConfig.fftConfig.twidDitherEnable = HWA_FEATURE_BIT_ENABLE;
    hwaCommonConfig.fftConfig.lfsrSeed = 0x1234567; /*Some non-zero value*/
    hwaCommonConfig.numLoops      = obj->hwaNumLoops;     
    hwaCommonConfig.paramStartIdx = obj->hwaParamStartIdx;
    hwaCommonConfig.paramStopIdx  = obj->hwaParamStopIdx; 

    retVal = HWA_configCommon(obj->hwaHandle, &hwaCommonConfig);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Enable the HWA */
    retVal = HWA_enable(obj->hwaHandle,1); 
    if (retVal != 0)
    {
        goto exit;
    }

    (void)EDMA_enableTransferRegion(baseAddr, regionId, obj->edmaIn.pingPong[DPU_DOPPLERPROCHWA_PING].channel, EDMA_TRIG_MODE_MANUAL);

    (void)EDMA_enableTransferRegion(baseAddr, regionId, obj->edmaIn.pingPong[DPU_DOPPLERPROCHWA_PONG].channel, EDMA_TRIG_MODE_MANUAL);

    /**********************************************/
    /* WAIT FOR HWA NUMLOOPS INTERRUPT            */
    /**********************************************/
    status = SemaphoreP_pend(&obj->hwaDoneSemaHandle, SystemP_WAIT_FOREVER);
    if (status != SystemP_SUCCESS)
    {
        retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
        goto exit;
    }

    (void)HWA_disableDoneInterrupt(obj->hwaHandle, HWA_THREAD_BACKGROUNDCONTEXT);

    /* Disable the HWA */
    retVal = HWA_enable(obj->hwaHandle, 0); 
    if (retVal != 0)
    {
        goto exit;
    }
    
    /**********************************************/
    /* WAIT FOR EDMA DONE INTERRUPT            */
    /**********************************************/
    status = SemaphoreP_pend(&obj->edmaDoneSemaHandle, SystemP_WAIT_FOREVER);
    if (status != SystemP_SUCCESS)
    {
        retVal = DPU_DOPPLERPROCHWA_ESEMASTATUS;
        goto exit;
    }
    
    outParams->stats.numProcess++;
    outParams->stats.processingTime = ClockP_getTimeUsec() - startTime;
    
exit:
    if (obj != NULL)
    {
        obj->inProgress = false;
    }    
    
    return retVal;
}

/**
  *  @b Description
  *  @n
  *  Doppler DPU deinit 
  *
  *  @param[in]   handle   DPU handle.
  *
  *  \ingroup    DPU_DOPPLERPROC_EXTERNAL_FUNCTION
  *
  *  @retval
  *      Success      =0
  *  @retval
  *      Error       !=0 @ref DPU_DOPPLERPROC_ERROR_CODE
  */
int32_t DPU_DopplerProcHWA_deinit(DPU_DopplerProcHWA_Handle handle)
{
    int32_t     retVal = 0;
    
    /* Free memory */
    if(handle == NULL)
    {
        retVal = DPU_DOPPLERPROCHWA_EINVAL;
    }
    
    return retVal;
}

