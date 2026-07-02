/**
 *   @file  aoaprochwa.c
 *
 *   @brief
 *      Implements Data path processing Unit using HWA.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018-2025 Texas Instruments, Inc.
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
#include <stdbool.h>

/* MCU + SDK include files. */
#include <drivers/soc.h>
#include <drivers/hw_include/hw_types.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/SystemP.h>
#include <kernel/dpl/HeapP.h>
#include <drivers/hwa.h>
#include <drivers/edma.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/ClockP.h>


/* Data Path Include Files */
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpu/aoaproc/aoaprochwa.h>
#include <ti/datapath/dpu/aoaproc/include/aoaprochwa_internal.h>
#include <ti/utils/mathutils/mathutils.h>

#include <ti/control/mmwavelink/mmwavelink.h>


//#define DBG_AOA_HWA_OBJ_DPU
/* HWA ping/pong memory bank indices for the 1st param doing FFT + log + magnitude */
#define DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PING   0
#define DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PONG   2
#define DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PING   4
#define DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PONG   6
#define DPU_AOAPROCHWA_MEM_BANK_INDX_ELEV_IN_SRC 5
#define DPU_AOAPROCHWA_MEM_BANK_INDX_ELEV_IN_DST 7

/* HWA memory available for Radar Cube range column and 2DFFT input */
#define HWAMEM_FOR_CUBE_RANGE_COLUMN   (2U*SOC_HWA_MEM_SIZE/SOC_HWA_NUM_MEM_BANKS)
#define HWAMEM_FOR_2DFFT_INPUT         (2U*SOC_HWA_MEM_SIZE/SOC_HWA_NUM_MEM_BANKS)

#ifdef DBG_AOA_HWA_OBJ_DPU
volatile AOAHwaObj *gAoaHwaObj[RL_MAX_SUBFRAMES] = {NULL};
volatile uint32_t gAoaHwaObjInd = 0;
#endif

//#define PROFILE_AOA_HWA_OBJ_DPU

#ifdef PROFILE_AOA_HWA_OBJ_DPU
DPU_AoAProcHWA_OutParams       gAoAProcStats[16];
volatile uint32_t gAoAProcStatsIdx = 0;
#endif

/* AoA Proc objects */
AOAHwaObj aoaProcObjPool[RL_MAX_SUBFRAMES] __attribute__((aligned(HeapP_BYTE_ALIGNMENT)));

static uint32_t AoAProcHWA_angleEstimationAzimElev
(
    AOAHwaObj       *aoaHwaObj,
    uint32_t        objInIdx,
    uint32_t        pingPongIdx,
    uint32_t        objOutIdx
);


static int32_t HWAutil_configHWA_extendedVelocityScheme
(
    HWA_Handle      hwaHandle,
    DPU_AoAProcHWA_HW_Resources *res,
    DPU_AoAProcHWA_StaticConfig *DPParams,
    uint32_t        *hwaMemBankAddr,
    uint8_t         numHypotheses,
    uint32_t        numAzimuthBins
);

static void DPU_AoAProcHWA_calcDCEstimParams(uint32_t numChirps, DPU_AoAProcHWA_DC_estimParams *dcEstPar);

static int32_t AoAProcHWA_cfgAndTrigger_EDMA_2DFFT
(
    AOAHwaObj *aoaHwaObj,
    uint32_t detObjIdx,
    uint8_t pingPongIdx
);

static int32_t AoAProcHWA_dataPathWait2DFFTDetObj(SemaphoreP_Object semaHandle);

static uint32_t AoAProcHWA_XYZestimation
(
    AOAHwaObj   *aoaHwaObj,
    uint32_t    pingPongIdx,
    uint32_t    objInIdx,
    uint32_t    objOutIdx,
    uint32_t    maxIdx,
    int32_t     dopplerIdx,
    uint32_t    hypothesisIdx
);

static int32_t  AoAProcHWA_PrepareRangeAzimuthHeatmap
(
    AOAHwaObj *aoaHwaObj,
    volatile uint64_t   *waitTimeLocal
);

static void AoAProcHWA_ConvertFov
(
    AOAHwaObj *aoaHwaObj,
    DPU_AoAProc_FovAoaCfg *fovAoaCfg
);


/**
 *  @b Description
 *  @n
 *      HWA processing completion call back function as per HWA API.
 *      Depending on the programmed transfer completion codes,
 *      posts HWA done semaphore.
 *
 *  @param[in]  threadIdx           Thread id
 *
 *  @param[in]  arg                 Argument to the callback function
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *  @retval     None
 */
static void AOAProcHWADoneIsrCallback(uint32_t threadIdx, void * arg)
{
    AOAHwaObj   *aoaObj;

    if (arg != NULL)
    {
        aoaObj = (AOAHwaObj *)arg;
        SemaphoreP_post(&aoaObj->hwaDone_semaHandle);
    }
}

/**
 *   @b Description
 *   @n
 *      AoAProcHWA Rx channel phase/gain compensation. The function performs phase/gain
 *      compensation on the Rx virtual antenna symbols of the detected object. The
 *      function is also used to perform the phase/gain compensation on the two dimensional
 *      array of only azimuth antennas which is used for the azimuth heat map display
 *      on the host. In this case the two dimensional array is laid out as
 *      X[number of range bins][number of azimuth antennas].
 *
 * @param[in]      rxChComp Rx channel phase/gain compensation coefficients
 *
 * @param[in]      numObj Number of detected points
 *
 * @param[in]      numAnt number of antennas
 *
 * @param[in]      symbolsIn Pointer to array of input symbols
 *
 * @param[out]     symbolsOut Pointer to array of output symbols
 *
 * @return         void
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static void AoAProcHWA_rxChanPhaseBiasCompensation(cmplx16ImRe_t *rxChComp,
                                                   uint32_t numObj,
                                                   uint32_t numAnt,
                                                   cmplx16ImRe_t *symbolsIn,
                                                   cmplx16ImRe_t *symbolsOut)
{
    int32_t Re, Im;
    uint32_t i, j;
    uint32_t antIndx;
    uint32_t objIdx;

    for (objIdx = 0; objIdx < numObj; objIdx++)
    {
        j = 0;
        /* Compensation of azimuth antennas */
        for (antIndx = 0; antIndx < numAnt; antIndx++)
        {
            i = objIdx*numAnt + antIndx;
            Re = (int32_t) symbolsIn[i].real * (int32_t) rxChComp[j].real -
                 (int32_t) symbolsIn[i].imag * (int32_t) rxChComp[j].imag;

            Im = (int32_t) symbolsIn[i].real * (int32_t) rxChComp[j].imag +
                 (int32_t) symbolsIn[i].imag * (int32_t) rxChComp[j].real;
            MATHUTILS_ROUND_AND_SATURATE_Q15(Re);
            MATHUTILS_ROUND_AND_SATURATE_Q15(Im);

            symbolsOut[i].real = (int16_t) Re;
            symbolsOut[i].imag = (int16_t) Im;
            j++;
        }
    }
}

/**
 *  @b Description
 *  @n
 *      The function configures EDMA for data transfers to/from HWA in a two stage
 *      data processing where the first stage is 2D-FFT (Doppler FFT) computation,
 *      and the second stage is 3D-FFT (azimuth FFT) computation.
 *      The 2D + 3D processing is organized in a ping/pong manner to run in parallel
 *      with input/output data transfers. Each ping/pong path processes one
 *      detected object at a time. Total 4 physical EDMA channels are used, two  per ping/pong path.
 *      In each path one channel is used for input data transfers to both first and second stage,
 *      and the other channel is used for output data transfers from both first and second stage.
 *
 *  @param[in]  hwaHandle           Handle to HWA
 *  @param[in]  res                 Pointer to hardware resources structure
 *  @param[in]  DPParams            Pointer to profile parameters
 *  @param[in]  srcIn2DFFTBuffAddr  Source address of the 1D-FFT radar cube
 *                                  matrix, the address is set in every iteration
 *                                  by local core per detected object according
 *                                  to its index.
 *  @param[in]  dstIn2DFFTBuffAddr  Destination addresses in HWA memory of
 *                                  ping/pong paths, 0-ping, 1-pong, for 2D-FFT calculation.
 *  @param[in]  srcOut2DFFTBuffAddr Source 2DFFT output addresses in HWA memory
 *                                  of ping/pong paths, 0-ping, 1-pong, the address
 *                                  is set in every iteration by local core per
 *                                  detected object according to its doppler index
 *  @param[in]  dstOut2DFFTBuffAddr Destination 2DFFT output address in local
 *                                  core memory of ping/pong paths. Output data
 *                                  consists of array of 2D antenna symbols corresponding
 *                                  to detected object range/doppler index. The
 *                                  2D array format is X[numTxAntenna][numRxAntenna]
 *  @param[in]  srcIn3DFFTBuffAddr  Address of 2DFFT antenna symbols, (Doppler and
 *                                  phase/gain compensated), of ping/pong paths,
 *                                  0-ping, 1-pong. In each path, it is 3D array
 *                                  in the form X[numberHypotheses][numTxAntenna][numRxAntenna],
 *                                  where the number of hypotheses is equal to number
 *                                  of Tx antennas in TDM MIMO scheme when extended
 *                                  maximum velocity scheme is enabled, otherwise
 *                                  number of hypotheses is equal to one.
 *  @param[in]  dstIn3DFFTBuffAddr  Destination addresses in HWA memory of ping/pong
 *                                  paths, 0-ping, 1-pong, for 3D FFT calculation.
 *  @param[in]  srcOut3DFFTBuffAddr Source 3DFFT output addresses in HWA memory of
 *                                  ping/pong paths, 0-ping, 1-pong.
 *  @param[in]  dstOut3DFFTBuffAddr Destination 3DFFT output address in local core
 *                                  memory, 0-ping, 1-pong.
 *  @param[in]  extMaxVelEnabled    0 - extended maximum velocity feature enabled,
 *                                  1 - disabled
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t AoAProcHWA_config_EDMA
(
    HWA_Handle  hwaHandle,
    DPU_AoAProcHWA_HW_Resources        *res,
    DPU_AoAProcHWA_StaticConfig  *DPParams,
    uint32_t           srcIn2DFFTBuffAddr,
    uint32_t           *dstIn2DFFTBuffAddr,
    uint32_t           *srcOut2DFFTBuffAddr,
    uint32_t           *dstOut2DFFTBuffAddr,

    uint32_t           *srcIn3DFFTBuffAddr,
    uint32_t           *dstIn3DFFTBuffAddr,
    uint32_t           *srcOut3DFFTBuffAddr,
    uint32_t           *dstOut3DFFTBuffAddr,
    uint8_t            extMaxVelEnabled

)
{
    uint16_t numBlocks;
    uint32_t pingPongIdx;
    uint16_t numIterations = 1; //EDMA will be reloaded per target
    uint16_t numHypotheses;
    uint16_t numDopFftParams;
    EDMACCPaRAMEntry       edmaParam;
    EDMACCPaRAMEntry       paramCfg;
    int32_t                 errorCode = SystemP_SUCCESS;
    HWA_SrcDMAConfig        dmaConfig;
    uint32_t            baseAddr, regionId;

    baseAddr = EDMA_getBaseAddr(res->edmaHandle);
    DebugP_assert(baseAddr != 0U);

    regionId = EDMA_getRegionId(res->edmaHandle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    if(extMaxVelEnabled != 0U)
    {
        numHypotheses = DPParams->numTxAntennas;
    }
    else
    {
        numHypotheses = 1;
    }

    /* Program Param Set */
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.aCnt          = (uint16_t) 1;
    edmaParam.bCnt          = (uint16_t) 0;
    edmaParam.cCnt          = (uint16_t) 0;
    edmaParam.linkAddr      = 0xFFFFU;
    CacheP_wbInv((void*)&edmaParam, sizeof(EDMACCPaRAMEntry), CacheP_TYPE_ALLD);

    if (DPParams->staticClutterCfg.isEnabled)
    {
        /* One extra param for DC estimation, before each Doppler FFT param */
        numDopFftParams = 2U * (uint16_t)DPParams->numTxAntennas;
    }
    else
    {
        numDopFftParams = DPParams->numTxAntennas;
    }

    numBlocks = ((uint16_t)DPParams->numVirtualAntAzim + (uint16_t)DPParams->numVirtualAntElev) * DPParams->numDopplerChirps;
    for (pingPongIdx = 0; pingPongIdx < 2U; pingPongIdx++)
    {
        /************************************************************/
        /* Physical Input channel is initially set to dummy channel */
        /************************************************************/

        (void)EDMA_configureChannelRegion(baseAddr, regionId, EDMA_CHANNEL_TYPE_DMA,
                                    res->edmaHwaExt[pingPongIdx].chIn.channel,
                                    res->edmaHwaExt[pingPongIdx].chIn.channel,
                                    res->edmaHwaExt[pingPongIdx].chIn.channel,
                                    res->edmaHwaExt[pingPongIdx].eventQueue);

        EDMA_setPaRAM(baseAddr, res->edmaHwaExt[pingPongIdx].chIn.channel, &edmaParam);

        /************************************************************/
        /* Physical Output channel is initially set to dummy channel */
        /************************************************************/
        edmaParam.opt          |= (EDMA_OPT_TCINTEN_MASK | EDMA_OPT_ITCINTEN_MASK);

        (void)EDMA_configureChannelRegion(baseAddr, regionId, EDMA_CHANNEL_TYPE_DMA,
                                    res->edmaHwaExt[pingPongIdx].chOut.channel,
                                    res->edmaHwaExt[pingPongIdx].chOut.channel,
                                    res->edmaHwaExt[pingPongIdx].chOut.channel,
                                    res->edmaHwaExt[pingPongIdx].eventQueue);

        EDMA_setPaRAM(baseAddr, res->edmaHwaExt[pingPongIdx].chOut.channel, &edmaParam);

        (void)EDMA_enableTransferRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chOut.channel, EDMA_TRIG_MODE_EVENT);

        /* Set EDMA Transfer type to AB type */
        EDMA_ccPaRAMEntry_init(&paramCfg);
        paramCfg.opt          |= (EDMA_OPT_SYNCDIM_MASK);
        /************************/
        /* Param set: 2D-FFT In */
        /************************/
        paramCfg.srcAddr       = (uint32_t) SOC_virtToPhy((void *)srcIn2DFFTBuffAddr);
        paramCfg.destAddr      = (uint32_t) SOC_virtToPhy((void *)dstIn2DFFTBuffAddr[pingPongIdx]);
        paramCfg.aCnt          = (uint16_t) sizeof(cmplx16ImRe_t);
        paramCfg.bCnt          = (uint16_t) numBlocks;
        paramCfg.cCnt          = (uint16_t) numIterations;
        paramCfg.bCntReload    = (uint16_t) 0U;
        paramCfg.srcBIdx       = (int16_t)DPParams->numRangeBins * (int16_t)sizeof(cmplx16ImRe_t);
        paramCfg.destBIdx      = (int16_t) sizeof(cmplx16ImRe_t);
        paramCfg.srcCIdx       = (int16_t) sizeof(cmplx16ImRe_t);
        paramCfg.destCIdx      = (int16_t) 0;
        paramCfg.linkAddr      = 0xFFFFU;

        paramCfg.opt          |= ((((uint32_t)res->edmaHwaExt[pingPongIdx].chIn.channel) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK);
        paramCfg.opt          |= (EDMA_TPCC_OPT_TCCHEN_MASK | EDMA_TPCC_OPT_ITCCHEN_MASK);

        CacheP_wbInv((void*)&paramCfg, sizeof(EDMACCPaRAMEntry), CacheP_TYPE_ALLD);
        EDMA_setPaRAM(baseAddr, res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramIn, &paramCfg);

        /* Set EDMA Transfer type to A type */
        EDMA_ccPaRAMEntry_init(&paramCfg);
        /**********************************/
        /* Param set: 2D-FFT Signature In */
        /**********************************/
        (void)HWA_getDMAconfig(hwaHandle,
                         (uint8_t)res->hwaCfg.paramSetStartIdx + ((uint8_t)pingPongIdx * (uint8_t)numDopFftParams),
                         &dmaConfig);

        paramCfg.srcAddr       = (uint32_t) SOC_virtToPhy((void *)dmaConfig.srcAddr);
        paramCfg.destAddr      = (uint32_t) SOC_virtToPhy((void *)dmaConfig.destAddr);
        paramCfg.aCnt          = (uint16_t) dmaConfig.aCnt;
        paramCfg.bCnt          = (uint16_t) dmaConfig.bCnt;
        paramCfg.cCnt          = (uint16_t) dmaConfig.cCnt;
        paramCfg.bCntReload    = (uint16_t) dmaConfig.bCnt;
        paramCfg.srcBIdx       = (int16_t) 0;
        paramCfg.destBIdx      = (int16_t) 0;
        paramCfg.srcCIdx       = (int16_t) 0;
        paramCfg.destCIdx      = (int16_t) 0;
        paramCfg.linkAddr      = 0xFFFFU;

        CacheP_wbInv((void*)&paramCfg, sizeof(EDMACCPaRAMEntry), CacheP_TYPE_ALLD);
        EDMA_setPaRAM(baseAddr, res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramInSignature, &paramCfg);

        /************************/
        /* Param set: 3D-FFT In */
        /************************/
        EDMA_ccPaRAMEntry_init(&paramCfg);
        paramCfg.srcAddr       = (uint32_t) SOC_virtToPhy((void *)srcIn3DFFTBuffAddr[pingPongIdx]);
        paramCfg.destAddr      = (uint32_t) SOC_virtToPhy((void *)dstIn3DFFTBuffAddr[pingPongIdx]);
        paramCfg.aCnt          = (uint16_t) numHypotheses * ((uint16_t)DPParams->numVirtualAntAzim 
                                                                + (uint16_t)DPParams->numVirtualAntElev) * (uint16_t)sizeof(cmplx16ImRe_t);
        paramCfg.bCnt          = (uint16_t) 1;
        paramCfg.cCnt          = (uint16_t) 1;
        paramCfg.bCntReload    = (uint16_t) 0;
        paramCfg.srcBIdx       = (int16_t) 0;
        paramCfg.destBIdx      = (int16_t) 0;
        paramCfg.srcCIdx       = (int16_t) 0;
        paramCfg.destCIdx      = (int16_t) 0;
        paramCfg.linkAddr      = 0xFFFFU;

        paramCfg.opt          |= ((((uint32_t)res->edmaHwaExt[pingPongIdx].chIn.channel) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK);
        paramCfg.opt          |= (EDMA_TPCC_OPT_TCCHEN_MASK | EDMA_TPCC_OPT_ITCCHEN_MASK);

        CacheP_wbInv((void*)&paramCfg, sizeof(EDMACCPaRAMEntry), CacheP_TYPE_ALLD);
        EDMA_setPaRAM(baseAddr, res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramIn, &paramCfg);

        /**********************************/
        /* Param set: 3D-FFT Signature In */
        /**********************************/
        uint32_t num3DFFTParamSets = (DPParams->numVirtualAntElev > 0U ? 1U : 0U) 
                                        * (1U + (DPParams->numElevFFTParams > 0U ? 1U : 0U) * (2U + (uint32_t)DPParams->numElevFFTParams)) + 2U;
        (void)HWA_getDMAconfig(hwaHandle,
                         (uint8_t)res->hwaCfg.paramSetStartIdx 
                            + 2U * (uint8_t)numDopFftParams + ((uint8_t)pingPongIdx * (uint8_t)num3DFFTParamSets),
                         &dmaConfig);

        EDMA_ccPaRAMEntry_init(&paramCfg);
        paramCfg.srcAddr       = (uint32_t) SOC_virtToPhy((void *)dmaConfig.srcAddr);
        paramCfg.destAddr      = (uint32_t) SOC_virtToPhy((void *)dmaConfig.destAddr);
        paramCfg.aCnt          = (uint16_t) dmaConfig.aCnt;
        paramCfg.bCnt          = (uint16_t) dmaConfig.bCnt;
        paramCfg.cCnt          = (uint16_t) dmaConfig.cCnt;
        paramCfg.bCntReload    = (uint16_t) dmaConfig.bCnt;
        paramCfg.srcBIdx       = (int16_t) 0;
        paramCfg.destBIdx      = (int16_t) 0;
        paramCfg.srcCIdx       = (int16_t) 0;
        paramCfg.destCIdx      = (int16_t) 0;
        paramCfg.linkAddr      = 0xFFFFU;

        CacheP_wbInv((void*)&paramCfg, sizeof(EDMACCPaRAMEntry), CacheP_TYPE_ALLD);
        EDMA_setPaRAM(baseAddr, res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramInSignature, &paramCfg);

        /**********************************************/
        /* Link Input physical channel and param sets */
        /**********************************************/
        /* Do not use LinkChannel API, it changes to ParamId's TCC  */
        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)res->edmaHwaExt[pingPongIdx].chIn.channel), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramIn));


        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramIn), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramInSignature));


        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramInSignature), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramIn));


        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramIn), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramInSignature));


        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramInSignature), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramIn));

        /********************************/
        /* Bring in the first param set */
        /********************************/
        (void)EDMA_enableTransferRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chIn.channel, EDMA_TRIG_MODE_MANUAL);

        /*********************/
        /* 2D-FFT EDMA Out:  */
        /*********************/
        EDMA_ccPaRAMEntry_init(&paramCfg);
        paramCfg.srcAddr       = (uint32_t) SOC_virtToPhy((void *) srcOut2DFFTBuffAddr[pingPongIdx]);
        paramCfg.destAddr      = (uint32_t) SOC_virtToPhy((void *) dstOut2DFFTBuffAddr[pingPongIdx]);
        paramCfg.aCnt          = ((uint16_t)DPParams->numVirtualAntAzim + (uint16_t)DPParams->numVirtualAntElev) * (uint16_t)sizeof(cmplx16ImRe_t);
        paramCfg.bCnt          = (uint16_t) 1;
        paramCfg.cCnt          = (uint16_t) 1;
        paramCfg.bCntReload    = (uint16_t) 0;
        paramCfg.srcBIdx       = (int16_t) 0;
        paramCfg.destBIdx      = (int16_t) 0;
        paramCfg.srcCIdx       = (int16_t) 0;
        paramCfg.destCIdx      = (int16_t) 0;
        paramCfg.linkAddr      = 0xFFFFU;

        paramCfg .opt         |= (EDMA_OPT_TCINTEN_MASK | EDMA_OPT_ITCINTEN_MASK |
                                 ((((uint32_t)res->edmaHwaExt[pingPongIdx].chOut.channel) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

        CacheP_wbInv((void*)&paramCfg, sizeof(EDMACCPaRAMEntry), CacheP_TYPE_ALLD);
        EDMA_setPaRAM(baseAddr, res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramOut, &paramCfg);

        /*********************/
        /* 3D-FFT EDMA Out:  */
        /*********************/
        EDMA_ccPaRAMEntry_init(&paramCfg);
        paramCfg.srcAddr       = (uint32_t) SOC_virtToPhy((void *) srcOut3DFFTBuffAddr[pingPongIdx]);
        paramCfg.destAddr      = (uint32_t) SOC_virtToPhy((void *) dstOut3DFFTBuffAddr[pingPongIdx]);
        paramCfg.aCnt          = (uint16_t) numHypotheses * (uint16_t)DPU_AOAPROCHWA_NUM_ANGLE_BINS * (uint16_t)sizeof(uint16_t);
        paramCfg.bCnt          = (uint16_t) 1;
        paramCfg.cCnt          = (uint16_t) 1;
        paramCfg.bCntReload    = (uint16_t) 0;
        paramCfg.srcBIdx       = (int16_t) 0;
        paramCfg.destBIdx      = (int16_t) 0;
        paramCfg.srcCIdx       = (int16_t) 0;
        paramCfg.destCIdx      = (int16_t) 0;
        paramCfg.linkAddr      = 0xFFFFU;

        paramCfg .opt         |= (EDMA_OPT_TCINTEN_MASK | EDMA_OPT_ITCINTEN_MASK |
                                 ((((uint32_t)res->edmaHwaExt[pingPongIdx].chOut.channel) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

        CacheP_wbInv((void*)&paramCfg, sizeof(EDMACCPaRAMEntry), CacheP_TYPE_ALLD);
        EDMA_setPaRAM(baseAddr, res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramOut, &paramCfg);

        /***********************************************/
        /* Link Output physical channel and param sets */
        /***********************************************/
        /* Do not use LinkChannel API, it changes to ParamId's TCC  */
        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)res->edmaHwaExt[pingPongIdx].chOut.channel), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramOut));


        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramOut), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramOut));


        HW_WR_FIELD32(baseAddr + EDMA_TPCC_LNK((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_3DFFT_STAGE].paramOut), EDMA_TPCC_LNK_LINK,
            baseAddr + EDMA_TPCC_OPT((uint32_t)res->edmaHwaExt[pingPongIdx].stage[AOAHWA_2DFFT_STAGE].paramOut));

        /********************************/
        /* Bring in the first param set */
        /********************************/
        (void)EDMA_enableTransferRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chOut.channel, EDMA_TRIG_MODE_MANUAL);
    }

    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      The function configures EDMA for single bin 2D-FFT computation using HWA.
 *      The output result is 2D array of zero Doppler 2D-FFT symbols,
 *      (X[numVirtualAntAzim][numRangeBins]), used for azimuth heatmap display.
 *      It configures EDMA to copy data from 1D-FFT radar cube matrix to HWA memory for
 *      single bin 2D-FFT calculation. The radar cube matrix format must be defined
 *      as @ref DPIF_RADARCUBE_FORMAT_1. The 2D-FFT processing is organized in a
 *      ping/pong manner to run parallel with input/output data transfer.
 *      Two input EDMA sets are configured to copy 1D-FFT antenna symbols, ping
 *      set reading even and pong set reading odd range bins, into HWA memory and
 *      trigger HWA to execute 2D-FFT computation. The output EDMA sets are configured
 *      to copy zero Doppler symbols, ping set copying even and pong set copying
 *      odd range bins.
 *
 *  @param[in]  hwaHandle            Handle to HWA
 *  @param[in]  res                  Pointer to hardware resources structure
 *  @param[in]  DPParams             Pointer to profile parameters
 *  @param[in]  srcIn2DFFTBuffAddr   Address of the 1D-FFT radar cube matrix
 *  @param[in]  dstIn2DFFTBuffAddr   FFT input addresses in HWA memory of ping/pong
 *                                   paths, 0-ping, 1-pong
 *  @param[in]  srcOut2DFFTBuffAddr  FFT output addresses in HWA memory of ping/pong
 *                                   paths, 0-ping, 1-pong
 *  @param[in]  dstOut2DFFTBuffAddr  Final result address of zero Doppler array in
 *                                   local core memory, 2D array:
 *                                   X[numVirtualAntAzim][numRangeBins]
 *  @param[in]  numTxAnt             Number of Tx Azimuth antennas (this is equal
 *                                   to number of HWA param sets per ping/pong path)
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t AoAProcHWA_config_azimuthHeatMap_EDMA
(
    HWA_Handle  hwaHandle,
    DPU_AoAProcHWA_HW_Resources        *res,
    DPU_AoAProcHWA_StaticConfig  *DPParams,
    uint32_t           srcIn2DFFTBuffAddr,
    uint32_t           *dstIn2DFFTBuffAddr,
    uint32_t           *srcOut2DFFTBuffAddr,
    uint32_t           dstOut2DFFTBuffAddr,
    uint8_t            numTxAnt
)
{
    int32_t     retVal = 0;
    uint16_t numBlocks;
    DPEDMA_ChainingCfg  chainingCfg;
    DPEDMA_syncABCfg    syncABCfg;
    uint32_t pingPongIdx;
    uint16_t numIterations = (uint16_t)DPParams->numRangeBins/AOAHWA_NUM_PING_PONG_BUF;
    bool isIntermediateTransferCompletionEnabled = false;
    bool isFinalTransferCompletionEnabled = true;

    numBlocks = (DPParams->numVirtualAntAzim) * DPParams->numDopplerChirps;
    for (pingPongIdx = 0; pingPongIdx < AOAHWA_NUM_PING_PONG_BUF; pingPongIdx++)
    {
        /************************************************************/
        /* EDMA In: load to HWA all antennas, azimuth and elevation */
        /************************************************************/

        /* Copy one Range bin to HWA memory                         */
        chainingCfg.chainingChannel               = (uint8_t)res->edmaHwa[pingPongIdx].inSignature.channel;
        chainingCfg.isIntermediateChainingEnabled = true;
        chainingCfg.isFinalChainingEnabled        = true;

        syncABCfg.srcAddress  = srcIn2DFFTBuffAddr + pingPongIdx * sizeof(cmplx16ImRe_t);
        syncABCfg.destAddress = dstIn2DFFTBuffAddr[pingPongIdx];
        syncABCfg.aCount      = (uint16_t)sizeof(cmplx16ImRe_t);
        syncABCfg.bCount      = numBlocks;
        syncABCfg.cCount      = numIterations;
        syncABCfg.srcBIdx     = (int32_t)DPParams->numRangeBins * (int32_t)sizeof(cmplx16ImRe_t);
        syncABCfg.dstBIdx     = (int32_t)sizeof(cmplx16ImRe_t);
        syncABCfg.srcCIdx     = (int16_t)sizeof(cmplx16ImRe_t) * (int16_t)AOAHWA_NUM_PING_PONG_BUF;
        syncABCfg.dstCIdx     = 0;

        CacheP_wbInv(&syncABCfg, sizeof(DPEDMA_syncABCfg), CacheP_TYPE_ALLD);

        retVal = DPEDMA_configSyncAB(res->edmaHandle,
                                     &res->edmaHwa[pingPongIdx].in,
                                     &chainingCfg,
                                     &syncABCfg,
                                     false,//isEventTriggered
                                     false, //isIntermediateTransferCompletionEnabled
                                     false,//isTransferCompletionEnabled
                                     NULL, //transferCompletionCallbackFxn
                                     NULL,//transferCompletionCallbackFxnArg
                                     NULL);
        if (retVal != SystemP_SUCCESS)
        {
            goto exit;
        }

        /* Copy one hot signature to trigger HWA 2D FFT */
        retVal = DPEDMAHWA_configOneHotSignature(res->edmaHandle,
                                     &res->edmaHwa[pingPongIdx].inSignature,
                                     hwaHandle,
                                     (uint8_t)res->hwaCfg.paramSetStartIdx + ((uint8_t)pingPongIdx * (uint8_t)numTxAnt),
                                     false);
        if (retVal != SystemP_SUCCESS)
        {
            goto exit;
        }

        /**************/
        /* EDMA Out:  */
        /**************/

        /* Copy one Range bin to HWA memory */
        chainingCfg.chainingChannel               = (uint8_t)res->edmaHwa[pingPongIdx].in.channel;
        chainingCfg.isIntermediateChainingEnabled = true;
        chainingCfg.isFinalChainingEnabled        = false;

        syncABCfg.srcAddress  = srcOut2DFFTBuffAddr[pingPongIdx];
        syncABCfg.destAddress = dstOut2DFFTBuffAddr + pingPongIdx * DPParams->numVirtualAntAzim * sizeof(cmplx16ImRe_t) ;
        syncABCfg.aCount      = (uint16_t)DPParams->numVirtualAntAzim * (uint16_t)sizeof(cmplx16ImRe_t);
        syncABCfg.bCount      = 1U;
        syncABCfg.cCount      = numIterations;
        syncABCfg.srcBIdx     = 0;
        syncABCfg.dstBIdx     = 0;
        syncABCfg.srcCIdx     = 0;
        syncABCfg.dstCIdx     = (int16_t)DPParams->numVirtualAntAzim * (int16_t)sizeof(cmplx16ImRe_t) * (int16_t)AOAHWA_NUM_PING_PONG_BUF;

        retVal = DPEDMA_configSyncAB(res->edmaHandle,
                                     &res->edmaHwa[pingPongIdx].out,
                                     &chainingCfg,
                                     &syncABCfg,
                                     true,//isEventTriggered
                                     isIntermediateTransferCompletionEnabled,
                                     isFinalTransferCompletionEnabled,
                                     NULL, //transferCompletionCallbackFxn
                                     NULL,//transferCompletionCallbackFxnArg
                                     NULL);

        if (retVal != SystemP_SUCCESS)
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
 *      The function configures source address of the input EDMA (based on object's
 *      range index), and source address of the output EDMA (based on object's Doppler index).
 *      The function then triggers input EDMA to transfer data to HWA. This trigger initiates the
 *      first stage of cloud point processing, 2D-FFT.
 *
 *  @param[in] aoaHwaObj    Pointer to AoA DPU internal object
 *  @param[in] detObjIdx    Object index in detected object list
 *  @param[in] pingPongIdx  Data path index, 0-ping, 1-pong
 *
 *  @retval EDMA error code, see EDMA API.
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 */
static int32_t AoAProcHWA_cfgAndTrigger_EDMA_2DFFT(AOAHwaObj *aoaHwaObj,
                                 uint32_t detObjIdx,
                                 uint8_t pingPongIdx)
{
    int32_t retVal;
    DPU_AoAProcHWA_HW_Resources *res = &aoaHwaObj->res;
    DPU_AoAProcHWA_StaticConfig *DPParams = &aoaHwaObj->params;
    cmplx16ImRe_t *radarCubeBase = (cmplx16ImRe_t *)res->radarCube.data;
    uint32_t dopplerBinSize = (uint32_t)sizeof(cmplx16ImRe_t) * ((uint32_t)DPParams->numVirtualAntAzim + (uint32_t)DPParams->numVirtualAntElev);
    uint32_t srcBuffAddr;
    uint32_t baseAddr, regionId;

    baseAddr = EDMA_getBaseAddr(res->edmaHandle);
    DebugP_assert(baseAddr != 0U);

    regionId = EDMA_getRegionId(res->edmaHandle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* EDMA In Source address in radar cube */
    srcBuffAddr = (uint32_t) (&radarCubeBase[res->cfarRngDopSnrList[detObjIdx].rangeIdx]);

    retVal = DPEDMA_updateAddressAndTrigger(res->edmaHandle, srcBuffAddr, 0,
             res->edmaHwaExt[pingPongIdx].chIn.channel, false);
    if (retVal != 0)
    {
        goto exit;
    }

    /* EDMA Out Source address in M2 or M3 */
    srcBuffAddr =  aoaHwaObj->edmaSrcOut2DFFTBuffAddr[pingPongIdx] +
                   (res->cfarRngDopSnrList[detObjIdx].dopplerIdx *  dopplerBinSize);
    retVal = DPEDMA_updateAddressAndTrigger(res->edmaHandle, srcBuffAddr, 0,
             res->edmaHwaExt[pingPongIdx].chOut.channel, false);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Trigger single bin 2D FFT */
    (void)EDMA_enableTransferRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chIn.channel, EDMA_TRIG_MODE_MANUAL);

exit:
    return retVal;
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
static void DPU_AoAProcHWA_calcDCEstimParams(uint32_t numChirps, DPU_AoAProcHWA_DC_estimParams *dcEstPar)
{
    uint8_t shiftCeil;
    uint8_t shiftActual;
    uint8_t prePorcLeftShift;
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
    temp = (ONEQ8F * ((float) (shiftedValue) / (float) numChirps)) + 0.5F;
    dcEstScale = (uint16_t) (temp);

    /* Scale value in Q8 format */
    dcEstPar->dcestScale = dcEstScale;
    /* HWA Programming value */
    dcEstPar->dcestShift = shiftActual - 6U;
    /* HWA input formater scale programming value for the DC esimation stage */
    dcEstPar->preProcScaleShift = 8U - prePorcLeftShift;

}

/**
 *   @b Description
 *   @n
 *      The function configures HWA Param sets for 2D-FFT and 3D-FFT calculation.
 *      Processing is organized in a ping pong manner, to run in parallel with
 *      EDMA in/out transfers. THe PARAM sets are split in two groups, one for ping
 *      path and the other for pong path. Processing in each path is divided in
 *      two stages. In the first stage 2D-FFT is calculated, and in the second stage
 *      3D-FFT is calculated. One HWA loop iteration processes 2 cloud points.
 *      (If the number of cloud-points is odd, a dummy one is appended at the end
 *      of the list of detected points.) One loop runs in the following order: ping
 *      path 2D-FFT, pong path 2D-FFT, ping path 3D-FFTs. pong path 3D-FFTs. The
 *      number of HWA Param sets for 2D-FFT calculation is equal to number of Tx
 *      antennas in TDM MIMO scheme. The number of Param sets for 3D-FFT calculation
 *      depends on whether the elevation Tx antenna is enabled or disabled. If the
 *      Tx elevation antenna is disabled, the number of param sets is equal to 2, the
 *      first one calculates 3D-FFT complex output samples, the second one calculates
 *      magnitude square of the complex samples. If the Tx elevation antenna is enabled,
 *      the number of param sets is equal to 3, the additional param set calculates 3D-FFT
 *      complex samples of the elevation antennas.
 *
 *   @param[in] hwaHandle           HWA driver handle
 *   @param[in] res                 Hardware resources
 *   @param[in] DPParams            Static parameters
 *   @param[in] hwaMemBankAddr      Array of 8 HWA bank addresses
 *   @param[in] extMaxVelEnabled    0 - Extended maximum velocity disabled, 1 - enabled
 *   @param[in] numAzimuthBins      Size of 3D-FFT (angle FFT)
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *   \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t HWAutil_configHWA_extendedVelocityScheme
(
    HWA_Handle      hwaHandle,
    DPU_AoAProcHWA_HW_Resources *res,
    DPU_AoAProcHWA_StaticConfig *DPParams,
    uint32_t        *hwaMemBankAddr,
    uint8_t         extMaxVelEnabled,
    uint32_t        numAzimuthBins
)
{
    HWA_ParamConfig hwaParamCfg;
    HWA_InterruptConfig     paramISRConfig;
    int32_t retVal = 0;
    uint32_t i;
    uint8_t         pingPongidx;
    uint8_t         paramSetCurrentIdx;
    uint8_t         dmaTriggerDest;
    uint8_t         numHypotheses;
	DPU_AoAProcHWA_DC_estimParams dcEstPar;

    if(extMaxVelEnabled != 0U)
    {
        numHypotheses = DPParams->numTxAntennas;
    }
    else
    {
        numHypotheses = 1;
    }

    uint8_t         paramSetStartIdx = res->hwaCfg.paramSetStartIdx;
    uint32_t        windowOffset = res->hwaCfg.winRamOffset;
    uint8_t         winSym = res->hwaCfg.winSym;
    uint32_t        numDopplerChirps = DPParams->numDopplerChirps;
    uint32_t        numDopplerBins = DPParams->numDopplerBins;
    uint8_t         numRxAnt = DPParams->numRxAntennas;
    uint8_t         numTxAnt = DPParams->numTxAntennas;
    uint8_t         numVirtualAntAzim = DPParams->numVirtualAntAzim;
    uint8_t         numVirtualAntElev = DPParams->numVirtualAntElev;
    uint32_t        hwaMemAzimSource[2];
    uint32_t        hwaMemAzimDest[2];
    uint8_t         numElemVirtualAntELev = mathUtils_ceilLog2(DPParams->zeroInsrtMaskCfg.zeroInsrtMaskElev);
    uint8_t         elevFFTParamIdx;
    uint8_t         numElevFFTParams = DPParams->numElevFFTParams;
    DPU_AoAProc_elevParamCfg *elevFFTParamCfg = DPParams->elevFFTParamCfg;

    hwaMemAzimSource[0] = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PING]);
    hwaMemAzimSource[1] = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PONG]);
    hwaMemAzimDest[0] = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PING]);
    hwaMemAzimDest[1] = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PONG]);

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    if (DPParams->staticClutterCfg.isEnabled)
    {
        DPU_AoAProcHWA_calcDCEstimParams(numDopplerChirps, &dcEstPar);
    }


    /**********************************************************/
    /******************* Configure 2D-FFT *********************/
    /**********************************************************/
    paramSetCurrentIdx = paramSetStartIdx;
    for (pingPongidx = 0U; pingPongidx < 2U; pingPongidx++)
    {
        (void)memset( (void*) &hwaParamCfg, 0, sizeof(HWA_ParamConfig));


        /* Program 2D FFT */
        hwaParamCfg.triggerSrc = paramSetCurrentIdx;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT; //do FFT

        hwaParamCfg.source.srcAcnt = (uint16_t)numDopplerChirps - 1U; //size in samples - 1

        hwaParamCfg.source.srcAIdx = (int32_t)numRxAnt * (int32_t)sizeof(cmplx16ImRe_t); //
        hwaParamCfg.source.srcBcnt = (uint16_t)numRxAnt - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t)sizeof(cmplx16ImRe_t); //should be dont care
        hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
        hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
        hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED; //signed
        hwaParamCfg.source.srcConjugate = 0; //no conjugate
        hwaParamCfg.source.srcAcircShift = 0;
        hwaParamCfg.source.srcAcircShiftWrap = 0;

		/* We place the 16-bit sample to LSB position of 24-bit HWA core computational unit */
        hwaParamCfg.source.srcScale = 8;

        hwaParamCfg.dest.dstAcnt = (uint16_t)numDopplerBins - 1U;
        hwaParamCfg.dest.dstAIdx = ((int32_t)numVirtualAntAzim + (int32_t)numVirtualAntElev) * (int32_t)sizeof(cmplx16ImRe_t);
        hwaParamCfg.dest.dstBIdx = (int32_t)sizeof(cmplx16ImRe_t) ; //should be dont care
        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
        hwaParamCfg.dest.dstConjugate = 0; //no conjugate

        /* Output formatter scale in combination and radix scale configuration make overall scale = 1/dopplerFftSize */
        hwaParamCfg.dest.dstScale = mathUtils_floorLog2(numDopplerBins);
        if (hwaParamCfg.dest.dstScale > 8U)
        {
            hwaParamCfg.dest.dstScale = 8U;
        }

        hwaParamCfg.dest.dstSkipInit = 0; // no skipping

        hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
        hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_floorLog2(numDopplerBins);

        /* Output formatter scale in combination and radix scale configuration make overall scale = 1/dopplerFftSize */
        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = (uint16_t)numDopplerBins >> 8U;
        if (hwaParamCfg.accelModeArgs.fftMode.butterflyScaling > 0U)
        {
            hwaParamCfg.accelModeArgs.fftMode.butterflyScaling--;
        }

        hwaParamCfg.accelModeArgs.fftMode.windowEn = 1; //enabled
        hwaParamCfg.accelModeArgs.fftMode.windowStart = (uint16_t)windowOffset; //start of window RAM
        hwaParamCfg.accelModeArgs.fftMode.winSymm = winSym;

        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

		/* Unnecessary but added for clarity */
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCEST;

        for (i = 0; i < numTxAnt; i++)
        {
            if (i == 0U)
            {
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            }
            else
            {
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            }
            hwaParamCfg.source.srcAddr = hwaMemAzimSource[pingPongidx] + (i * numDopplerChirps * numRxAnt * sizeof(cmplx16ImRe_t)); // address is relative to start of MEM0
            hwaParamCfg.dest.dstAddr = hwaMemAzimDest[pingPongidx] + (i * numRxAnt * sizeof(cmplx16ImRe_t)); // address is relative to start of MEM0

            if (DPParams->staticClutterCfg.isEnabled)
            {
                /*******************/
                /*** DC Estimate ***/
                /*******************/
                hwaParamCfg.source.srcScale = dcEstPar.preProcScaleShift;  /* May need to shift left for DC estimate, if the number of chirps is less than 64 */
                hwaParamCfg.dest.dstAcnt = 1 - 1; /* No output during DC estimation */
                hwaParamCfg.dest.dstSkipInit = 1; /* No output during DC estimation */


                /* DC estimation ENABLED / DC subtraction DISABLED */
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_PARAMRESET;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_DISABLE;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCEST;
                hwaParamCfg.accelModeArgs.fftMode.fftEn = 0;

                retVal = HWA_configParamSet(hwaHandle, (uint8_t) paramSetCurrentIdx, &hwaParamCfg, NULL);
                if (retVal != 0)
                {
                    goto exit;
                }

                /* Disable DMA/interrupt hookup to all except the last one */
                retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                                      paramSetCurrentIdx,
                                                      HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
                if (retVal != 0)
                 {
                      goto exit;
                 }
                paramSetCurrentIdx++;

                /*********************************/
                /*** DC subtract & Doppler FFT ***/
                /*********************************/
                hwaParamCfg.dest.dstAcnt = (uint16_t)numDopplerBins - 1U;
                hwaParamCfg.source.srcScale = 8U;
                hwaParamCfg.dest.dstSkipInit = 0U;
                /* DC estimation DISABLED / DC subtraction ENABLED */
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcEstResetMode = HWA_DCEST_INTERFSUM_RESET_MODE_NOUPDATE;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubEnable = HWA_FEATURE_BIT_ENABLE;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.dcSubSelect = HWA_DCSUB_SELECT_DCEST;
                hwaParamCfg.accelModeArgs.fftMode.fftEn = 1U;
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            }

            retVal = HWA_configParamSet(hwaHandle, (uint8_t) paramSetCurrentIdx, &hwaParamCfg, NULL);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Disable DMA/interrupt hookup to all except the last one */
            retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                                  paramSetCurrentIdx,
                                                  HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
            if (retVal != 0)
             {
                  goto exit;
             }
             /* Output interrupt to DMA only by last param set */
            if (i == (numTxAnt-1U))
            {
                /* Last param set: enable the DMA hookup to this paramset so that data gets copied out */
                retVal = HWA_getDMAChanIndex(hwaHandle,
                                             res->edmaHwaExt[pingPongidx].chOut.channel,
                                             &dmaTriggerDest);
                if (retVal != 0)
                {
                    goto exit;
                }
                paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
                paramISRConfig.dma.dstChannel = dmaTriggerDest;  /* EDMA channel to trigger to copy the data out */
                paramISRConfig.cpu.callbackArg = NULL;
                retVal = HWA_enableParamSetInterrupt(hwaHandle,
                                                     paramSetCurrentIdx,
                                                     &paramISRConfig);
                if (retVal != 0)
                 {
                      goto exit;
                 }
            }
            paramSetCurrentIdx++;
        }
    }

    /**********************************************************/
    /******************* Configure 3D-FFT *********************/
    /**********************************************************/
    for (pingPongidx = 0U; pingPongidx < 2U; pingPongidx++)
    {
        {
        /************** Param set computes the complex values of the azimuth-FFT after shuffling and zero insertion **********/
        (void)memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
        hwaParamCfg.triggerSrc = paramSetCurrentIdx;
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;

        hwaParamCfg.source.srcAddr = hwaMemAzimSource[pingPongidx];
        hwaParamCfg.source.srcAcnt = (uint16_t)numVirtualAntAzim - 1U;
        hwaParamCfg.source.srcAIdx = (int32_t)sizeof(cmplx16ImRe_t);
        hwaParamCfg.source.srcBIdx = ((int32_t)numVirtualAntAzim + (int32_t)numVirtualAntElev) * (int32_t)sizeof(cmplx16ImRe_t);
        hwaParamCfg.source.srcBcnt = (uint16_t)numHypotheses - 1U;
        hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
        hwaParamCfg.source.srcScale = 8;
        hwaParamCfg.source.shuffleMode  = HWA_SRC_SHUFFLE_AB_MODE_ADIM;
        hwaParamCfg.source.shuffleStart = 0U;

        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.zeroInsertEn =  HWA_FEATURE_BIT_ENABLE;

        hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
        hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(numAzimuthBins);//assumes power of 2;
        hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

        hwaParamCfg.dest.dstAddr =  hwaMemAzimDest[pingPongidx];
        hwaParamCfg.dest.dstAcnt = (uint16_t)numAzimuthBins - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t)sizeof(cmplx16ImRe_t);
        hwaParamCfg.dest.dstBIdx = (int32_t)numAzimuthBins * (int32_t)sizeof(cmplx16ImRe_t);
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
        hwaParamCfg.dest.dstScale = 3;

        retVal = HWA_configParamSet(hwaHandle, paramSetCurrentIdx, &hwaParamCfg, NULL);
        if (retVal != 0)
        {
          goto exit;
        }
        retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                           paramSetCurrentIdx,
                                           HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
        if (retVal != 0)
        {
          goto exit;
        }

        paramSetCurrentIdx++;
        }

        if (numVirtualAntElev > 0U)
        {
            /* As Zero Insertion Mask RAM / fields can not be changed in between the paramsets,
               so for elevation FFT zero insertion is done using paramsets. */
            if( numElemVirtualAntELev != numVirtualAntElev)
            {
                /************** Param set shuffles the elevation-FFT samples **********/
                {
                (void)memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

                hwaParamCfg.source.srcAddr = hwaMemAzimSource[pingPongidx] +
                                                        (numVirtualAntAzim * sizeof(cmplx16ImRe_t));
                hwaParamCfg.source.srcAcnt = (uint16_t)numVirtualAntElev - 1U;
                hwaParamCfg.source.srcAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBIdx = ((int32_t)numVirtualAntAzim + (int32_t)numVirtualAntElev) * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBcnt = (uint16_t)numHypotheses - 1U;
                hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.source.srcScale = 8;
                hwaParamCfg.source.shuffleMode  = HWA_SRC_SHUFFLE_AB_MODE_ADIM;
                hwaParamCfg.source.shuffleStart = 1U;

                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
                hwaParamCfg.accelModeArgs.fftMode.fftEn = 0;
                hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
                hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

                hwaParamCfg.dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_ELEV_IN_SRC]);
                hwaParamCfg.dest.dstAcnt = (uint16_t)numVirtualAntElev - 1U;
                hwaParamCfg.dest.dstAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstBIdx = (int32_t)numVirtualAntElev * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.dest.dstScale = 0;

                retVal = HWA_configParamSet(hwaHandle, paramSetCurrentIdx, &hwaParamCfg, NULL);
                if (retVal != 0)
                {
                    goto exit;
                }
                retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                                paramSetCurrentIdx,
                                                HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
                if (retVal != 0)
                {
                goto exit;
                }

                paramSetCurrentIdx++;
                }

                /************** Param set fills the zeros for elevation-FFT input **********/
                {
                (void)memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

                hwaParamCfg.source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_ELEV_IN_SRC]);
                hwaParamCfg.source.srcAcnt = (uint16_t)numElemVirtualAntELev - 1U;
                hwaParamCfg.source.srcAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBIdx = (int32_t)numElemVirtualAntELev * (int32_t)sizeof(cmplx16ImRe_t); // doesn't matter as this param always writes 0
                hwaParamCfg.source.srcBcnt = (uint16_t)numHypotheses - 1U;
                hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.source.srcScale = 8;

                /* Multiply by 0 */
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_SCALAR_MULT;
                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.modeCfg.scalerMultiply.scaleCmultScaleEn = HWA_FEATURE_BIT_DISABLE;
                hwaParamCfg.accelModeArgs.fftMode.fftEn = 0;
                hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
                hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

                hwaParamCfg.dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_ELEV_IN_DST]);
                hwaParamCfg.dest.dstAcnt = (uint16_t)numElemVirtualAntELev - 1U;
                hwaParamCfg.dest.dstAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstBIdx = (int32_t)numElemVirtualAntELev * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.dest.dstScale = 0;

                retVal = HWA_configParamSet(hwaHandle, paramSetCurrentIdx, &hwaParamCfg, NULL);
                if (retVal != 0)
                {
                    goto exit;
                }
                retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                                paramSetCurrentIdx,
                                                HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
                if (retVal != 0)
                {
                goto exit;
                }

                paramSetCurrentIdx++;
                }

                /************** Param set writes the elevation-FFT samples **********/
                {
                    for (elevFFTParamIdx = 0; elevFFTParamIdx < numElevFFTParams; elevFFTParamIdx++)
                    {
                        (void)memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
                        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

                        hwaParamCfg.source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_ELEV_IN_SRC]) + \
                                                    elevFFTParamCfg[elevFFTParamIdx].srcAddOffset * sizeof(cmplx16ImRe_t);
                        hwaParamCfg.source.srcAcnt = elevFFTParamCfg[elevFFTParamIdx].acnt;
                        hwaParamCfg.source.srcAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                        hwaParamCfg.source.srcBIdx = (int32_t)numVirtualAntElev * (int32_t)sizeof(cmplx16ImRe_t);
                        hwaParamCfg.source.srcBcnt = (uint16_t)numHypotheses - 1U;
                        hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
                        hwaParamCfg.source.srcScale = 8;

                        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

                        hwaParamCfg.accelModeArgs.fftMode.fftEn = 0;
                        hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
                        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

                        hwaParamCfg.dest.dstAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_ELEV_IN_DST]) + \
                                                    elevFFTParamCfg[elevFFTParamIdx].dstAddOffset * sizeof(cmplx16ImRe_t);
                        hwaParamCfg.dest.dstAcnt = elevFFTParamCfg[elevFFTParamIdx].acnt;
                        hwaParamCfg.dest.dstAIdx = (int32_t)elevFFTParamCfg[elevFFTParamIdx].dstAidx * (int32_t)sizeof(cmplx16ImRe_t);
                        hwaParamCfg.dest.dstBIdx = (int32_t)numElemVirtualAntELev * (int32_t)sizeof(cmplx16ImRe_t);
                        hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
                        hwaParamCfg.dest.dstScale = 0;

                        retVal = HWA_configParamSet(hwaHandle, paramSetCurrentIdx, &hwaParamCfg, NULL);
                        if (retVal != 0)
                        {
                            goto exit;
                        }
                        retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                                        paramSetCurrentIdx,
                                                        HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
                        if (retVal != 0)
                        {
                        goto exit;
                        }

                        paramSetCurrentIdx++;
                    }
                }
                /************** Param set performs the elevation-FFT **********/
                {
                (void)memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

                hwaParamCfg.source.srcAddr = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_ELEV_IN_DST]);
                hwaParamCfg.source.srcAcnt = (uint16_t)numElemVirtualAntELev - 1U;
                hwaParamCfg.source.srcAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBIdx = (int32_t)numElemVirtualAntELev * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBcnt = (uint16_t)numHypotheses - 1U;
                hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.source.srcScale = 8;

                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

                hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
                hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(numAzimuthBins);//assumes power of 2;
                hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
                hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

                hwaParamCfg.dest.dstAddr =  hwaMemAzimDest[pingPongidx] +
                                                        (numHypotheses * numAzimuthBins * sizeof(cmplx16ImRe_t));
                hwaParamCfg.dest.dstAcnt = (uint16_t)numAzimuthBins - 1U;
                hwaParamCfg.dest.dstAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstBIdx = (int32_t)numAzimuthBins * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.dest.dstScale = 3;

                retVal = HWA_configParamSet(hwaHandle, paramSetCurrentIdx, &hwaParamCfg, NULL);
                if (retVal != 0)
                {
                    goto exit;
                }
                retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                                paramSetCurrentIdx,
                                                HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
                if (retVal != 0)
                {
                goto exit;
                }

                paramSetCurrentIdx++;
                }
            }
            else
            {
                /************** Param set performs the elevation-FFT after shuffling the elevation samples **********/
                (void)memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

                hwaParamCfg.source.srcAddr = hwaMemAzimSource[pingPongidx] +
                                                        (numVirtualAntAzim * sizeof(cmplx16ImRe_t));
                hwaParamCfg.source.srcAcnt = (uint16_t)numVirtualAntElev - 1U;
                hwaParamCfg.source.srcAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBIdx = ((int32_t)numVirtualAntAzim + (int32_t)numVirtualAntElev) * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.source.srcBcnt = (uint16_t)numHypotheses - 1U;
                hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.source.srcScale = 8;
                hwaParamCfg.source.shuffleMode  = HWA_SRC_SHUFFLE_AB_MODE_ADIM;
                hwaParamCfg.source.shuffleStart = 1U;

                hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

                hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
                hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(numAzimuthBins);//assumes power of 2;
                hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
                hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

                hwaParamCfg.dest.dstAddr =  hwaMemAzimDest[pingPongidx] +
                                                        (numHypotheses * numAzimuthBins * sizeof(cmplx16ImRe_t));
                hwaParamCfg.dest.dstAcnt = (uint16_t)numAzimuthBins - 1U;
                hwaParamCfg.dest.dstAIdx = (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstBIdx = (int32_t)numAzimuthBins * (int32_t)sizeof(cmplx16ImRe_t);
                hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED;
                hwaParamCfg.dest.dstScale = 3;

                retVal = HWA_configParamSet(hwaHandle, paramSetCurrentIdx, &hwaParamCfg, NULL);
                if (retVal != 0)
                {
                    goto exit;
                }
                retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                                paramSetCurrentIdx,
                                                HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
                if (retVal != 0)
                {
                goto exit;
                }

                paramSetCurrentIdx++;
            }
        }

        //This calculates magnitude
        /************** Param set computes magnitude of input complex values of the azimuth-FFT **********/
        (void)memset( (void*) &hwaParamCfg, 0, sizeof(hwaParamCfg));
        hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;

        hwaParamCfg.source.srcAddr = hwaMemAzimDest[pingPongidx];
        hwaParamCfg.source.srcAcnt = (uint16_t)numAzimuthBins - 1U;
        hwaParamCfg.source.srcAIdx = (int32_t)sizeof(cmplx16ImRe_t);
        hwaParamCfg.source.srcBIdx = (int32_t)numAzimuthBins * (int32_t)sizeof(cmplx16ImRe_t);
        hwaParamCfg.source.srcBcnt = (uint16_t)numHypotheses - 1U;
        hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED;
        hwaParamCfg.source.srcScale = 8;

        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_ONLY_ENABLED;
		hwaParamCfg.accelModeArgs.fftMode.fftEn = 0;
        hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_ceilLog2(numAzimuthBins);//Ignored
        hwaParamCfg.accelModeArgs.fftMode.windowEn = 0;
        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0; //no scaling

        hwaParamCfg.dest.dstAddr = hwaMemAzimSource[pingPongidx] + DPU_AOAPROCHWA_3DFFT_MAG_SQUARE_ADDRSS_OFFSET;
        hwaParamCfg.dest.dstAcnt = (uint16_t)numAzimuthBins - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t)sizeof(uint16_t);
        hwaParamCfg.dest.dstBIdx = (int32_t)numAzimuthBins * (int32_t)sizeof(uint16_t);
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_UNSIGNED;
        hwaParamCfg.dest.dstScale = 0;
        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_REAL;

        retVal = HWA_configParamSet(hwaHandle, paramSetCurrentIdx, &hwaParamCfg, NULL);
        if (retVal != 0)
        {
          goto exit;
        }

        /* Enable the DMA hookup to EDMA */
        retVal = HWA_getDMAChanIndex(hwaHandle,
                                     res->edmaHwaExt[pingPongidx].chOut.channel,
                                     &dmaTriggerDest);
        if (retVal != 0)
        {
             goto exit;
        }
        paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
        paramISRConfig.dma.dstChannel = dmaTriggerDest;  /* EDMA channel to trigger to copy the data out */
        paramISRConfig.cpu.callbackArg = NULL;
        retVal = HWA_enableParamSetInterrupt(hwaHandle,
                                             paramSetCurrentIdx,
                                             &paramISRConfig);
        if (retVal != 0)
        {
             goto exit;
        }

        paramSetCurrentIdx++;

    }

    if (DPParams->staticClutterCfg.isEnabled)
    {
        HWA_CommonConfig    hwaCommonConfig;
        (void)memset((void*) &hwaCommonConfig, 0, sizeof(HWA_CommonConfig));

        /* Config Common Registers */
        hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_DCEST_SCALESHIFT;

		/*DC estimation scaling */
        hwaCommonConfig.dcEstimateConfig.scale = dcEstPar.dcestScale;
        hwaCommonConfig.dcEstimateConfig.shift = dcEstPar.dcestShift;

        retVal = HWA_configCommon(hwaHandle, &hwaCommonConfig);
        if (retVal != 0)
        {
            goto exit;
        }
    }

exit:
    return (retVal);
}

/**
 *   @b Description
 *   @n
 *      The function configures HWA Param sets for 2D-FFT single range bin
 *      calculation from 1D-FFT data from radarCube. The 1D FFT symbols are
 *      arranged in HWA memory as three dimensional array as
 *      X[Number of Tx antennas][Number of virtual chirps][Number of Rx antennas]
 *      THe output symbols are arrange as three dimensional array as
 *      Y[Number of Doppler bins][Number of Tx antnnas][Number of Rx antennas]
 *
 *   @param[in] hwaHandle       HWA driver handle
 *   @param[in] res             Hardware resources
 *   @param[in] DPParams        Static parameters
 *   @param[in] hwaMemBankAddr  Array of 8 HWA bank addresses
 *   @param[in] numAzimTxAnt    Number of azimuth Tx antennas
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *   \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t HWAutil_configHWA_azimuthHeatMap
(
    HWA_Handle      hwaHandle,
    DPU_AoAProcHWA_HW_Resources *res,
    DPU_AoAProcHWA_StaticConfig *DPParams,
    uint32_t        *hwaMemBankAddr,
    uint8_t        numAzimTxAnt
)
{
    HWA_ParamConfig hwaParamCfg;
    HWA_InterruptConfig     paramISRConfig;
    int32_t retVal = 0;
    uint32_t i;
    uint8_t pingPongidx;
    uint8_t paramSetCurrentIdx;
    uint8_t         dmaTriggerDest;

    uint8_t         paramSetStartIdx = res->hwaCfg.paramSetStartIdx;
    uint32_t        windowOffset = res->hwaCfg.winRamOffset;
    uint8_t         winSym = res->hwaCfg.winSym;
    uint32_t        numDopplerChirps = DPParams->numDopplerChirps;
    uint32_t        numDopplerBins = DPParams->numDopplerBins;
    uint8_t         numRxAnt = DPParams->numRxAntennas;
    uint8_t         numVirtualAntAzim = DPParams->numVirtualAntAzim;
    uint32_t        hwaMemAzimSource[2];
    uint32_t        hwaMemAzimDest[2];

    hwaMemAzimSource[0] = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PING]);
    hwaMemAzimSource[1] = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PONG]);
    hwaMemAzimDest[0] = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PING]);
    hwaMemAzimDest[1] = HWADRV_ADDR_TRANSLATE_CPU_TO_HWA(hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PONG]);

    uint32_t        numOutSamplesPerFft = 1;



    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

    /**********************************************************/
    /******************* Configure 2D-FFT *********************/
    /**********************************************************/
    paramSetCurrentIdx = paramSetStartIdx;
    for (pingPongidx = 0U; pingPongidx < 2U; pingPongidx++)
    {
        (void)memset( (void*) &hwaParamCfg, 0, sizeof(HWA_ParamConfig));


        /* Program 2D FFT */
        hwaParamCfg.triggerSrc = paramSetCurrentIdx;
        hwaParamCfg.accelMode = HWA_ACCELMODE_FFT; //do FFT

        hwaParamCfg.source.srcAcnt = (uint16_t)numDopplerChirps - 1U; //size in samples - 1

        hwaParamCfg.source.srcAIdx = (int32_t)numRxAnt * (int32_t)sizeof(cmplx16ImRe_t); //
        hwaParamCfg.source.srcBcnt = (uint16_t)numRxAnt - 1U;
        hwaParamCfg.source.srcBIdx = (int32_t)sizeof(cmplx16ImRe_t); //should be dont care
		hwaParamCfg.source.srcAcircShift = 0;
        hwaParamCfg.source.srcAcircShiftWrap = 0;

        hwaParamCfg.source.srcRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //complex data
        hwaParamCfg.source.srcWidth = HWA_SAMPLES_WIDTH_16BIT; //16-bit
        hwaParamCfg.source.srcSign = HWA_SAMPLES_SIGNED; //signed
        hwaParamCfg.source.srcConjugate = 0; //no conjugate
        hwaParamCfg.source.srcScale = 0;

        hwaParamCfg.dest.dstAcnt = (uint16_t)numOutSamplesPerFft - 1U;
        hwaParamCfg.dest.dstAIdx = (int32_t)(numVirtualAntAzim) * (int32_t)sizeof(cmplx16ImRe_t);
        hwaParamCfg.dest.dstBIdx = (int32_t)sizeof(cmplx16ImRe_t) ; //should be dont care
        hwaParamCfg.dest.dstRealComplex = HWA_SAMPLES_FORMAT_COMPLEX; //same as input - complex
        hwaParamCfg.dest.dstWidth = HWA_SAMPLES_WIDTH_16BIT; //same as input - 16 bit
        hwaParamCfg.dest.dstSign = HWA_SAMPLES_SIGNED; //same as input - signed
        hwaParamCfg.dest.dstConjugate = 0; //no conjugate
        hwaParamCfg.dest.dstScale = 8 ;
        hwaParamCfg.dest.dstSkipInit = 0; // no skipping

        hwaParamCfg.accelModeArgs.fftMode.fftEn = 1;
        hwaParamCfg.accelModeArgs.fftMode.fftSize = mathUtils_floorLog2(numDopplerBins);
        hwaParamCfg.accelModeArgs.fftMode.butterflyScaling = 0x3FF; //LSB fftSize bits are relevant
        hwaParamCfg.accelModeArgs.fftMode.windowEn = 1; //enabled
        hwaParamCfg.accelModeArgs.fftMode.windowStart = (uint16_t)windowOffset; //start of window RAM
        hwaParamCfg.accelModeArgs.fftMode.winSymm = winSym;

        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.magLogEn = HWA_FFT_MODE_MAGNITUDE_LOG2_DISABLED;
        hwaParamCfg.accelModeArgs.fftMode.postProcCfg.fftOutMode = HWA_FFT_MODE_OUTPUT_DEFAULT;
        hwaParamCfg.accelModeArgs.fftMode.preProcCfg.complexMultiply.cmultMode = HWA_COMPLEX_MULTIPLY_MODE_DISABLE;

        for (i = 0U; i < numAzimTxAnt; i++)
        {
            if (i == 0U)
            {
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_DMA;
            }
            else
            {
                hwaParamCfg.triggerMode = HWA_TRIG_MODE_IMMEDIATE;
            }

            hwaParamCfg.source.srcAddr = hwaMemAzimSource[pingPongidx] + (i * numDopplerChirps * numRxAnt * sizeof(cmplx16ImRe_t)); // address is relative to start of MEM0
            hwaParamCfg.dest.dstAddr = hwaMemAzimDest[pingPongidx] + (i * numRxAnt * sizeof(cmplx16ImRe_t)); // address is relative to start of MEM0
            retVal = HWA_configParamSet(hwaHandle, (uint8_t) paramSetCurrentIdx, &hwaParamCfg, NULL);
            if (retVal != 0)
            {
                goto exit;
            }

            /* Disable DMA/interrupt hookup to all except the last one */
            retVal = HWA_disableParamSetInterrupt(hwaHandle,
                                                  paramSetCurrentIdx,
                                                  HWA_PARAMDONE_INTERRUPT_TYPE_DMA | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR1 | HWA_PARAMDONE_INTERRUPT_TYPE_CPU_INTR2);
            if (retVal != 0)
             {
                  goto exit;
             }
             /* Output interrupt to DMA only by last param set */
            if (i == (numAzimTxAnt - 1U))
            {
                /* Last param set: enable the DMA hookup to this paramset so that data gets copied out */
                retVal = HWA_getDMAChanIndex(hwaHandle,
                                             (uint8_t)res->edmaHwa[pingPongidx].out.channel,
                                             &dmaTriggerDest);
                if (retVal != 0)
                {
                    goto exit;
                }
                paramISRConfig.interruptTypeFlag = HWA_PARAMDONE_INTERRUPT_TYPE_DMA;
                paramISRConfig.dma.dstChannel = dmaTriggerDest;  /* EDMA channel to trigger to copy the data out */
                paramISRConfig.cpu.callbackArg = NULL;
                retVal = HWA_enableParamSetInterrupt(hwaHandle,
                                                     paramSetCurrentIdx,
                                                     &paramISRConfig);
                if (retVal != 0)
                 {
                      goto exit;
                 }
            }
            paramSetCurrentIdx++;
        }
    }

exit:
    return (retVal);
}



/**
 *  @b Description
 *  @n
 *      Configures HWA common register. It sets the Params start index and
 *      number of iteration loops.
 *
 *   @param[in] hwaHandle               HWA driver handle
 *   @param[in] numLoops                Number of HWA loops
 *   @param[in] paramStartIdx           Param start index
 *   @param[in] numParams               Number of params
 *   @param[in] aoaHwaObj               Pointer to AoA DPU internal object
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static int32_t AoAProc_HWAutil_configCommon (HWA_Handle hwaHandle,
                                                uint16_t numLoops,
                                                uint16_t paramStartIdx,
                                                uint16_t numParams,
                                                AOAHwaObj *aoaHwaObj)
{
    int32_t errCode = 0;
    HWA_CommonConfig    hwaCommonConfig;
    uint8_t idx = 0;

    /* Disable the HWA */
     errCode = HWA_enable(hwaHandle, 0);
     if (errCode != 0)
     {
         goto exit;
     }

    /***********************/
    /* HWA COMMON CONFIG   */
    /***********************/
    /* Config Common Registers */
    hwaCommonConfig.configMask = HWA_COMMONCONFIG_MASK_STATEMACHINE_CFG |/* numLoops, paramStartIdx, paramStopIdx combined here */
                               HWA_COMMONCONFIG_MASK_TWIDDITHERENABLE |
                               HWA_COMMONCONFIG_MASK_LFSRSEED |
                               HWA_COMMONCONFIG_MASK_ZEROINSERT_NUM_MASK |
                               HWA_COMMONCONFIG_MASK_COMPLEXMULT_SCALECONST;

    hwaCommonConfig.fftConfig.twidDitherEnable = HWA_FEATURE_BIT_ENABLE;
    hwaCommonConfig.fftConfig.lfsrSeed = 0x1234567; /*Some non-zero value*/
    hwaCommonConfig.numLoops = numLoops;
    hwaCommonConfig.paramStartIdx = paramStartIdx;
    hwaCommonConfig.paramStopIdx = paramStartIdx + (numParams - 1U);
    hwaCommonConfig.zeroInsertConfig.number = 0;
    for(idx=0; idx < mathUtils_ceilLog2(aoaHwaObj->params.zeroInsrtMaskCfg.zeroInsrtMaskAzim); idx++)
    {
        if(((aoaHwaObj->params.zeroInsrtMaskCfg.zeroInsrtMaskAzim >> idx) & 0x1U) == 0U)
        {
            hwaCommonConfig.zeroInsertConfig.number++;
        }
    }
    hwaCommonConfig.complexMultiplyConfig.Iscale[0] = 0;
    hwaCommonConfig.complexMultiplyConfig.Qscale[0] = 0;

    /* Configure LUT RAM for Zero insertion. */
    errCode = HWA_configRam(hwaHandle, HWA_RAM_TYPE_LUT_FREQ_DEROTATE_RAM, (uint8_t *)&aoaHwaObj->params.zeroInsrtMaskCfg.zeroInsrtMaskAzim, sizeof(uint32_t), 0);
    if (errCode != 0)
    {
        goto exit;
    }

    errCode = HWA_configCommon(hwaHandle, &hwaCommonConfig);
    if (errCode != 0)
    {
        goto exit;
    }

    errCode = HWA_enableDoneInterrupt(hwaHandle,
		                             HWA_THREAD_BACKGROUNDCONTEXT,
                                     AOAProcHWADoneIsrCallback,
                                     aoaHwaObj);

exit:
    return (errCode);
}

/**
 *  @b Description
 *  @n
 *      Wait for angle estimation to complete. This is a blocking function.
 *
 *  @param[in] semaHandle  Semaphore handle
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error code  - <0
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *
 */
static int32_t AoAProcHWA_dataPathWait2DFFTDetObj(SemaphoreP_Object semaHandle)
{
    return((int32_t) SemaphoreP_pend(&semaHandle, SystemP_WAIT_FOREVER));
}

/**
 *  @b Description
 *  @n
 *      This function is called per object and it calculates its x/y/z coordinates
 *      based on Azimuth FFT output. It stores the coordinates
 *      to the output list of type @ref DPIF_PointCloudCartesian_t and it also
 *      calculates object's SNR and the noise level and writes into the side
 *      information list of type @ref DPIF_PointCloudSideInfo_t
 *
 *  @param[in] aoaHwaObj Pointer to AoA DPU internal object
 *
 *  @param[in] pingPongIdx ping/pong index
 *
 *  @param[in] objInIdx Index of the current object in CFAR output list @ref DPIF_CFARDetList_t
 *
 *  @param[in] objOutIdx Index of the current object in the output lists @ref DPIF_PointCloudCartesian_t and @ref DPIF_PointCloudSideInfo_t
 *
 *  @param[in] maxIdx Peak position in the FFT output of azimuth antennas
 *
 *  @param[in] dopplerIdx Doppler index of detected object
 *
 *  @param[in] hypothesisIdx Hypothesis index related to extended maximum velocity feature.
 *              (If the feature is disabled, hypothesisIdx is passed as zero)
 *
 *  @retval objOutIdx Index for the next object in the output list
 *
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static uint32_t AoAProcHWA_XYZestimation
(
    AOAHwaObj   *aoaHwaObj,
    uint32_t    pingPongIdx,
    uint32_t    objInIdx,
    uint32_t    objOutIdx,
    uint32_t    maxIdx,
    int32_t     dopplerIdx,
    uint32_t    hypothesisIdx
)
{
    int32_t         sMaxIdx;
    float           temp;
    float           Wx, Wz;
    float           range;
    float           limitScale;
    float           x, y, z;
    float           peakAzimRe, peakAzimIm, peakElevRe, peakElevIm;
    uint32_t numAngleBins = DPU_AOAPROCHWA_NUM_ANGLE_BINS;
    DPU_AoAProcHWA_HW_Resources            *res = &aoaHwaObj->res;
    DPIF_CFARDetList        *objIn = res->cfarRngDopSnrList;
    DPIF_PointCloudCartesian *objOut = res->detObjOut;
    DPIF_PointCloudSideInfo *objOutSideInfo = res->detObjOutSideInfo;

    DPU_AoAProcHWA_StaticConfig *params = &aoaHwaObj->params;
    //uint16_t numDopplerBins = params->numDopplerBins;

    cmplx16ImRe_t *azimFFTPtr = (cmplx16ImRe_t *) (aoaHwaObj->hwaAzimuthFftCmplxOutBuffAddr[pingPongIdx] + (hypothesisIdx*numAngleBins)*sizeof(cmplx16ImRe_t));

    uint32_t numHypotheses;

    if (aoaHwaObj->dynLocalCfg.extMaxVelCfg.enabled != 0U)
    {
        numHypotheses = aoaHwaObj->params.numTxAntennas;
    }
    else
    {
        numHypotheses = 1;
    }




    range = (float)objIn[objInIdx].rangeIdx * params->rangeStep;

    /* Compensate for range bias */
    range -= aoaHwaObj->dynLocalCfg.compRxChanCfg.rangeBias;
    if (range < 0.0F)
    {
        range = 0.0F;
    }

    if(maxIdx > (numAngleBins / 2U - 1U))
    {
        sMaxIdx = (int32_t)maxIdx - (int32_t)numAngleBins;
    }
    else
    {
        sMaxIdx = (int32_t)maxIdx;
    }

    Wx = 2.0F * ((float) sMaxIdx / (float) numAngleBins);
    /* Check if it is within configured field of view */

    x = range * (Wx / (2.0F * aoaHwaObj->params.antennaSpacing.xSpacingByLambda));

    if (params->numVirtualAntElev > 0U)
    {
        peakAzimIm = (float) azimFFTPtr[maxIdx].imag;
        peakAzimRe = (float) azimFFTPtr[maxIdx].real;
        peakElevIm = (float) azimFFTPtr[maxIdx + numHypotheses*numAngleBins].imag;
        peakElevRe = (float) azimFFTPtr[maxIdx + numHypotheses*numAngleBins].real;

        Wz = (float)(atan2((double)peakAzimIm * (double)peakElevRe - (double)peakAzimRe * (double)peakElevIm,
                   (double)peakAzimRe * (double)peakElevRe + (double)peakAzimIm * (double)peakElevIm) / PI_);

        /* Note that division by pi has already been taken care of while calculating
        Wz. pi is divided in the first term, and it is not considered in the Wx calculation,
        effectively dividing by pi. */
        if (Wz > 1.0F)
        {
            Wz = Wz - 2.0F;
        }
        else if (Wz < -1.0F)
        {
            Wz = Wz + 2.0F;
        }
        else
        {
            ;
        }

        /* In case of AWR294X, the elevation antenna is offset at 0.8 * lambda, and not
        0.5 * lambda. In this case, Wz, instead of being pi * sin(phi), becomes ELEV_SCALE_FACTOR * sin(phi).
        Refer to the doxygen for aoa processing for more details. Hence this factor is needed */
        /* Check if it is within configured field of view */
        if((Wz / (2.0F * aoaHwaObj->params.antennaSpacing.zSpacingByLambda) < aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.minElevationSineVal) || 
           (Wz / (2.0F * aoaHwaObj->params.antennaSpacing.zSpacingByLambda) > aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.maxElevationSineVal))
        {
            goto exit;
        }
        z = range * Wz / (aoaHwaObj->params.antennaSpacing.zSpacingByLambda * 2.0F);
        res->detObjElevationAngle[objOutIdx] = Wz/(aoaHwaObj->params.antennaSpacing.zSpacingByLambda * 2.0F);
        /*record wz for debugging/testing*/
        limitScale = (float)sqrt(1.0 - (double)Wz*(double)Wz);
    }
    else
    {
        z = 0.0F;
        limitScale = 1.0F;
    }

    if((Wx < (limitScale * aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.minAzimuthSineVal)) ||
       (Wx > (limitScale * aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.maxAzimuthSineVal)))
    {
        goto exit;
    }

    temp = range*range -x*x -z*z;
    if (temp > 0.0F)
    {
        y = (float)sqrt(temp);
    }
    else
    {
        goto exit;
    }

    objOut[objOutIdx].x = x;
    objOut[objOutIdx].y = y;
    objOut[objOutIdx].z = z;

    objOut[objOutIdx].velocity = params->dopplerStep * (float)dopplerIdx;
    objOutSideInfo[objOutIdx].noise = objIn[objInIdx].noise;
    objOutSideInfo[objOutIdx].snr = objIn[objInIdx].snr;
    res->detObj2dAzimIdx[objOutIdx] = (uint8_t)maxIdx;

    objOutIdx++;

exit:
    return (objOutIdx);
}

/*!
 *  @b Description
 *  @n
 *      This function performs Doppler compensation on a single antenna symbol.
 *
 *  @param[in]  in           Pointer to the Input Symbol
 *
 *  @param[in]  out          Pointer to the Output Symbol
 *
 *  @param[in]  Cos          Cosine value depending on doppler index
 *
 *  @param[in]  Sin          Sine value depending on doppler index
 *
 *  @retval None
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static void aoaHwa_dopplerComp
(
    cmplx16ImRe_t *in,
    cmplx16ImRe_t *out,
    float  Cos,
    float  Sin
)
{
    float           yRe, yIm;

    /* Rotate symbol (correct the phase) */
    yRe = (float)in->real * Cos + (float)in->imag * Sin;
    yIm = (float)in->imag * Cos - (float)in->real * Sin;

    out->real = (int16_t) yRe;
    out->imag = (int16_t) yIm;
}

/**
 *   @b Description
 *   @n
 *      Function performs Doppler compensation on antenna symbols. Note: In order to support
 *      more than 4 Tx antennas, one needs to modify the Cosine/Sine calculation array
 *      accordingly.
 *
 *  @param[in]  srcPtr              Input pointer to antenna symbols
 *
 *  @param[in]  cfarOutList         CFAR detection list
 *
 *  @param[in]  dstPtr              Output pointer to antenna symbols
 *
 *  @param[in]  numTxAnt            Number of Tx antennas
 *
 *  @param[in]  numRxAnt            Number of physical Rx antennas
 *
 *  @param[in]  numVirtualAntAzim   Number of virtual azimuth Rx antennas
 *
 *  @param[in]  numVirtualAntElev   Number of virtual elevation Rx antennas
 *
 *  @param[in]  numDopplerBins      Number of Doppler bins
 *
 *  @param[in]  extendedMaxVelEnabled      0: extended maximum velocity feature disabled, 1: enabled
 *
 *  @return None
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static void aoaHwa_dopplerCompensation
(
    uint32_t *srcPtr,
    DPIF_CFARDetList *cfarOutList,
    uint32_t *dstPtr,
    uint32_t numTxAnt,
    uint32_t numRxAnt,
    uint32_t numVirtualAntAzim,
    uint32_t numVirtualAntElev,
    uint32_t numDopplerBins,
    uint32_t extendedMaxVelEnabled
)
{
    uint32_t    index;
    uint32_t    j;
    uint32_t    virtAntIdx;
    uint32_t    txAntIdx;
    uint16_t    dopplerIdx;
    int32_t     dopplerSignIdx;
    float       dopplerCompensationIdx;
    int32_t     wrapInd;
    uint32_t    numHypotheses;
    float       Cos, Sin;
    float       SinVal[3], CosVal[3]; /* Stores {SinV, Sin2V, Sin3V},  {CosV, Cos2V, Cos3V} */
    uint32_t    numVirtualAnt;
    uint32_t    tempIdx;

    dopplerIdx = cfarOutList->dopplerIdx;
    dopplerSignIdx = (int32_t) dopplerIdx;
    if(dopplerIdx >= (numDopplerBins / 2U))
    {
        dopplerSignIdx = dopplerSignIdx - (int32_t) numDopplerBins;
    }

    if(extendedMaxVelEnabled != 0U)
    {
        numHypotheses = numTxAnt;
        tempIdx = numHypotheses >> 1U;
        wrapInd = - ((int32_t) (tempIdx));
        if (((numTxAnt & 0x1U) == 0U) && (dopplerSignIdx < 0))
        {
            wrapInd++;
        }
    }
    else
    {
        numHypotheses = 1;
        wrapInd = 0;
    }

    numVirtualAnt = numVirtualAntAzim + numVirtualAntElev;

    for(index = 0; index < numHypotheses; index++)
    {
        virtAntIdx = 0;
        /* transfer data corresponding to azimuth virtual antennas (corresponding to chirp of antenna Tx0) */
        for(j = 0; j < numRxAnt; j++)
        {
            dstPtr[virtAntIdx + index*numVirtualAnt] = srcPtr[virtAntIdx];
            virtAntIdx++;
        }

        if(numTxAnt > 1U)
        {
            dopplerCompensationIdx = ((float)dopplerSignIdx + ((float)wrapInd * (float)numDopplerBins)) / (float)numTxAnt;
            Cos = (float)cos(2.0*PI_*(double)dopplerCompensationIdx/(double)numDopplerBins);
            Sin = (float)sin(2.0*PI_*(double)dopplerCompensationIdx/(double)numDopplerBins);
            CosVal[0] = Cos;
            SinVal[0] = Sin;
            CosVal[1] = Cos * Cos - Sin * Sin;
            SinVal[1] = 2.0F * Cos * Sin;
            CosVal[2] = 4.0F * Cos * Cos * Cos - 3.0F * Cos;
            SinVal[2] = 3.0F * Sin - 4.0F * Sin * Sin * Sin;

            for(txAntIdx=1; txAntIdx < numTxAnt; txAntIdx++)
            {
                for(j = 0; j < numRxAnt; j++)
                {
                    aoaHwa_dopplerComp((cmplx16ImRe_t *)&srcPtr[virtAntIdx],
                                       (cmplx16ImRe_t *)&dstPtr[virtAntIdx + index*numVirtualAnt],
                                       CosVal[txAntIdx-1U],
                                       SinVal[txAntIdx-1U]);
                    virtAntIdx++;
                }

            }
        }
        wrapInd++;
    }
}


/**
 *  @b  Description
 *  @n
 *      Function calculates x/y/z coordinates of the objects detected by CFAR DPU.
 *      The number of output objects may be larger than input if multiobject beam
 *      forming is enabled, and more than one objects are detected at the
 *      same range/doppler bin.
 *
 *  @param[in]  aoaHwaObj          Pointer to AoAProc DPU internal data Object
 *  @param[in]  objInIdx           Index to the next object in the output list
 *  @param[in]  pingPongIdx        Ping/pong index
 *  @param[in]  objOutIdx          Index to the next object in the output list
 *
 *  @retval    objOutIdx           Number of detected objects in the output list
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 */
static uint32_t AoAProcHWA_angleEstimationAzimElev
(
    AOAHwaObj       *aoaHwaObj,
    uint32_t        objInIdx,
    uint32_t        pingPongIdx,
    uint32_t        objOutIdx
)
{
    uint32_t j, maxVal = 0,maxIdx = 0, tempVal;
    DPU_AoAProcHWA_HW_Resources *res = &aoaHwaObj->res;
    DPU_AoAProcHWA_StaticConfig *DPParams = &aoaHwaObj->params;
    DPIF_CFARDetList        *objIn = res->cfarRngDopSnrList;


    uint32_t numAngleBins = DPU_AOAPROCHWA_NUM_ANGLE_BINS;
    uint16_t maxNumObj = (uint16_t)res->detObjOutMaxSize;
    uint32_t numHypotheses;
    uint32_t hypothesisMaxIdx;
    uint16_t *azimFFTAbsPtr = (uint16_t *)aoaHwaObj->azimuthFftOutMagBuf[pingPongIdx];
    int16_t  dopplerSignIdx;
    int32_t  wrapStartInd;
    uint8_t numTxAnt = DPParams->numTxAntennas;
    uint16_t numDopplerBins = DPParams->numDopplerBins;
    float    temp;
    uint32_t tempIdx = 0U;

    if (objOutIdx >= maxNumObj)
    {
        return objOutIdx;
    }

    dopplerSignIdx = AOA_DOPPLER_IDX_TO_SIGNED(objIn[objInIdx].dopplerIdx, numDopplerBins);

    if(aoaHwaObj->dynLocalCfg.extMaxVelCfg.enabled != 0U)
    {
        numHypotheses = aoaHwaObj->params.numTxAntennas;
        tempIdx = numHypotheses >> 1U;
        wrapStartInd = - ((int32_t) (tempIdx));
        if (((numTxAnt & 0x1U) == 0U) && (dopplerSignIdx < 0))
        {
            wrapStartInd++;
        }
    }
    else
    {
        numHypotheses = 1;
        wrapStartInd = 0;
    }



    maxVal = 0;
    for(j=0; j < numHypotheses * numAngleBins; j++)
    {
        tempVal = azimFFTAbsPtr[j];
        if(tempVal > maxVal)
        {
            maxVal = tempVal;
            maxIdx = j;
        }
    }

    hypothesisMaxIdx = maxIdx >> DPU_AOAPROCHWA_LOG2_NUM_ANGLE_BINS;
    maxIdx = maxIdx & (numAngleBins - 1U);

    dopplerSignIdx += ((int16_t)wrapStartInd + (int16_t)hypothesisMaxIdx) * (int16_t) numDopplerBins;

    /* Estimate x,y,z */
    objOutIdx = AoAProcHWA_XYZestimation(aoaHwaObj,
                                         pingPongIdx,
                                         objInIdx,
                                         objOutIdx,
                                         maxIdx,
                                         dopplerSignIdx,
                                         hypothesisMaxIdx);

    if (objOutIdx >= maxNumObj)
    {
        return objOutIdx;
    }

    /* Multi peak azimuzth search?*/
    if (aoaHwaObj->dynLocalCfg.multiObjBeamFormingCfg.enabled != 0U)
    {
        uint32_t leftSearchIdx;
        uint32_t rightSearchIdx;
        uint32_t secondSearchLen;
        uint32_t iModAzimLen;
        uint32_t maxVal2;
        int32_t k;
        uint32_t t;
        uint32_t azimIdx = maxIdx;
        uint16_t* azimuthMag = &azimFFTAbsPtr[hypothesisMaxIdx * numAngleBins];

        /* Find right edge of the first peak */
        t = azimIdx;
        leftSearchIdx = (t + 1U) & (numAngleBins-1U);
        k = (int32_t)numAngleBins;
        while ((azimuthMag[t] >= azimuthMag[leftSearchIdx]) && (k > 0))
        {
            t = (t + 1U) & (numAngleBins - 1U);
            leftSearchIdx = (leftSearchIdx + 1U) & (numAngleBins - 1U);
            k--;
        }

        /* Find left edge of the first peak */
        t = azimIdx;
        rightSearchIdx = (t - 1U) & (numAngleBins - 1U);
        k = (int32_t)numAngleBins;
        while ((azimuthMag[t] >= azimuthMag[rightSearchIdx]) && (k > 0))
        {
            t = (t - 1U) & (numAngleBins - 1U);
            rightSearchIdx = (rightSearchIdx - 1U) & (numAngleBins - 1U);
            k--;
        }

        secondSearchLen = ((rightSearchIdx - leftSearchIdx) & (numAngleBins - 1U)) + 1U;
        /* Find second peak */
        maxVal2 = azimuthMag[leftSearchIdx];
        azimIdx = leftSearchIdx;
        for (t = leftSearchIdx; t < (leftSearchIdx + secondSearchLen); t++)
        {
            iModAzimLen = t & (numAngleBins - 1U);
            if (azimuthMag[iModAzimLen] > maxVal2)
            {
                azimIdx = (uint16_t)iModAzimLen;
                maxVal2 = azimuthMag[iModAzimLen];
            }
        }
        temp = (float)maxVal * aoaHwaObj->dynLocalCfg.multiObjBeamFormingCfg.multiPeakThrsScal;
        /* Is second peak greater than threshold? */
        if ( (maxVal2 >( ((uint32_t)(temp)))) && (objOutIdx < maxNumObj) )
        {

            /* Estimate x,y,z for second peak */
            objOutIdx = AoAProcHWA_XYZestimation(aoaHwaObj,
                                                 pingPongIdx,
                                                 objInIdx,
                                                 objOutIdx,
                                                 azimIdx,
                                                 dopplerSignIdx,
                                                 hypothesisMaxIdx);
        }
    }
    return(objOutIdx);
}

/**
 *  @b Description
 *  @n
 *      The function prepares data for range-azimuth heat-map.
 *      It calculates 2D-FFT for all virtual antennas and saves
 *      zero Doppler values for all range bins.
 *
 *  @param[in]    aoaHwaObj     Pointer to internal AoAProcHWA data object
 *
 *  @param[inout] waitTimeLocal CPU waiting time
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 */
static int32_t AoAProcHWA_PrepareRangeAzimuthHeatmap(AOAHwaObj *aoaHwaObj,
                                               volatile uint64_t   *waitTimeLocal)
{
    volatile uint64_t   startTime1;
    int32_t retVal = 0;
    uint16_t numParams;
    HWA_Handle hwaHandle;
    cmplx16ImRe_t     *radarCubeBase;
    DPU_AoAProcHWA_HW_Resources *res;
    DPU_AoAProcHWA_StaticConfig *DPParams;
    uint8_t numTxAzimAnt;
    uint32_t baseAddr, regionId;

    res = &aoaHwaObj->res;
    DPParams = &aoaHwaObj->params;
    radarCubeBase = (cmplx16ImRe_t *)res->radarCube.data;
    hwaHandle = aoaHwaObj->hwaHandle;

    baseAddr = EDMA_getBaseAddr(res->edmaHandle);
    DebugP_assert(baseAddr != 0U);

    regionId = EDMA_getRegionId(res->edmaHandle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    numTxAzimAnt = (uint8_t) (DPParams->numVirtualAntAzim / DPParams->numRxAntennas);

    retVal = HWAutil_configHWA_azimuthHeatMap(hwaHandle,
                                              res,
                                              DPParams,
                                              aoaHwaObj->hwaMemBankAddr,
                                              numTxAzimAnt);
    if (retVal != 0)
    {
        goto exit;
    }

    numParams = (uint16_t)numTxAzimAnt * AOAHWA_NUM_PING_PONG_BUF;
    retVal =  AoAProc_HWAutil_configCommon (hwaHandle,
                                            DPParams->numRangeBins / AOAHWA_NUM_PING_PONG_BUF, //numLoops,
                                            res->hwaCfg.paramSetStartIdx, //paramStartIdx,
                                            numParams,
                                            aoaHwaObj);
    if (retVal != 0)
    {
        goto exit;
    }

    retVal = AoAProcHWA_config_azimuthHeatMap_EDMA(hwaHandle,
                                                   res,
                                                   DPParams,
                                                   (uint32_t) radarCubeBase,
                                                   aoaHwaObj->edmaDstIn2DFFTBuffAddr,
                                                   aoaHwaObj->edmaSrcOut2DFFTBuffAddr,
                                                   (uint32_t) res->azimuthStaticHeatMap,
                                                   numTxAzimAnt);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Enable the HWA */
    retVal = HWA_enable(hwaHandle, 1);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Trigger first two range bins 2D FFT */
    /* Ping */
    (void)EDMA_enableTransferRegion(baseAddr, regionId, res->edmaHwa[0].in.channel, EDMA_TRIG_MODE_MANUAL);

    /* Pong */
    (void)EDMA_enableTransferRegion(baseAddr, regionId, res->edmaHwa[1].in.channel, EDMA_TRIG_MODE_MANUAL);

    /* Wait until HWA done with FFT */
    startTime1 = ClockP_getTimeUsec();
    (void)AoAProcHWA_dataPathWait2DFFTDetObj(aoaHwaObj->hwaDone_semaHandle);
    *waitTimeLocal += ClockP_getTimeUsec() - startTime1;

    /* Wait until EDMA output is done */
    startTime1 = ClockP_getTimeUsec();

    while(EDMA_readIntrStatusRegion(baseAddr, regionId, res->edmaHwa[0].in.channel) != 1U)
    {
        /* Wait until EDMA transfer is done */
    }
    EDMA_clrIntrRegion(baseAddr, regionId, res->edmaHwa[0].in.channel);

    while(EDMA_readIntrStatusRegion(baseAddr, regionId, res->edmaHwa[1].in.channel) != 1U)
    {
        /* Wait until EDMA transfer is done */
    }
    EDMA_clrIntrRegion(baseAddr, regionId, res->edmaHwa[1].in.channel);
    *waitTimeLocal += ClockP_getTimeUsec() - startTime1;

    /* Disable the HWA */
    retVal = HWA_enable(hwaHandle, 0);
    if (retVal != 0)
    {
        goto exit;
    }

	/* if azimuthStaticHeatMap is in L2 or L3, need to invalidate the cache before the compensation */
	CacheP_wbInv(res->azimuthStaticHeatMap, sizeof (cmplx16ImRe_t) *
	           DPParams->numRangeBins * DPParams->numVirtualAntAzim, CacheP_TYPE_ALLD);

    /* Rx channel gain/phase offset compensation */
    AoAProcHWA_rxChanPhaseBiasCompensation(&aoaHwaObj->dynLocalCfg.compRxChanCfg.rxChPhaseComp[0],
                                        DPParams->numRangeBins,
                                        DPParams->numVirtualAntAzim,
                                        res->azimuthStaticHeatMap,
                                        res->azimuthStaticHeatMap);

    /* Cache operation required per DPU design rules */
    CacheP_wbInv(res->azimuthStaticHeatMap, sizeof (cmplx16ImRe_t) * DPParams->numRangeBins * DPParams->numVirtualAntAzim, CacheP_TYPE_ALLD);

exit:
    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function converts angle of arrival field of view values specified
 *      in degrees to the values appropriate for internal DPU comparison.
 *
 *  @param[in]  aoaHwaObj    Pointer to internal AoAProcHWA data object
 *
 *  @param[in]  fovAoaCfg    Pointer to field of view configuration
 *
 *  \ingroup    DPU_AOAPROC_INTERNAL_FUNCTION
 *
 *  @retval  None
 *
 */
static void AoAProcHWA_ConvertFov(AOAHwaObj *aoaHwaObj,
                           DPU_AoAProc_FovAoaCfg *fovAoaCfg)
{
    aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.minAzimuthSineVal = (float)sin(fovAoaCfg->minAzimuthDeg / 180. * PI_);
    aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.maxAzimuthSineVal = (float)sin(fovAoaCfg->maxAzimuthDeg / 180. * PI_);
    aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.minElevationSineVal = (float)sin(fovAoaCfg->minElevationDeg / 180. * PI_);
    aoaHwaObj->dynLocalCfg.fovAoaLocalCfg.maxElevationSineVal = (float)sin(fovAoaCfg->maxElevationDeg  / 180. * PI_);
}


uint16_t DPU_AoAProcHWA_getNumHwaParamSets
(
    uint16_t numTxAnt,
    uint16_t numRxVirtAntElevation,
    uint8_t  numElevFFTParamsZerIns,
    bool     isStaticClutterRemovalEnabled
)
{
    uint16_t numParams;
    uint16_t numElevParams;

    if(numRxVirtAntElevation > 0U)
    {
        if(numElevFFTParamsZerIns > 0U)
        {
            // shuffle + zero fill + numElevFFTParamsZerIns + elevFFT
            numElevParams = 2U + (uint16_t)numElevFFTParamsZerIns + 1U;
        }
        else
        {
            // elev FFT
            numElevParams = 1U;
        }
    }
    else
    {
        numElevParams = 0U;
    }
    if (isStaticClutterRemovalEnabled)
    {
        numParams = (2U * ((2U * numTxAnt) + 2U + numElevParams));
    }
    else
    {
        numParams = (2U * ((numTxAnt) + 2U + numElevParams));
    }
    return  numParams;
}

DPU_AoAProcHWA_Handle DPU_AoAProcHWA_init
(
    DPU_AoAProcHWA_InitParams *initParams,
    volatile uint8_t          subframeCounter,
    int32_t*            errCode
)
{
    AOAHwaObj     *aoaHwaObj = NULL;
    HWA_MemInfo   hwaMemInfo;
    uint16_t      index;
    int32_t       status = SystemP_SUCCESS;

    if ((initParams == NULL) || (initParams->hwaHandle == NULL))
    {
        *errCode = DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }

    DebugP_assert(subframeCounter < RL_MAX_SUBFRAMES);

    /* Allocate Memory for AoAProc */
    aoaHwaObj = (AOAHwaObj*)&aoaProcObjPool[subframeCounter];

    /* Save for debugging */
#ifdef DBG_AOA_HWA_OBJ_DPU
    if (gAoaHwaObjInd < RL_MAX_SUBFRAMES)
    {
        gAoaHwaObj[gAoaHwaObjInd++] = aoaHwaObj;
    }
#endif

    /* Set HWA bank memory address */
    *errCode =  HWA_getHWAMemInfo(initParams->hwaHandle, &hwaMemInfo);
    if (*errCode < 0)
    {
        goto exit;
    }

    /* Initialize memory */
    (void)memset((void *)aoaHwaObj, 0, sizeof(AOAHwaObj));

    /* Save init config params */
    aoaHwaObj->hwaHandle   = initParams->hwaHandle;

    for (index = 0; index < hwaMemInfo.numBanks; index++)
    {
        aoaHwaObj->hwaMemBankAddr[index] = hwaMemInfo.baseAddress + (uint32_t)index * (uint32_t)hwaMemInfo.bankSize;
    }

    /* Create semaphore for HWA done */
    status = SemaphoreP_constructBinary(&aoaHwaObj->hwaDone_semaHandle, 0);
    if(status != SystemP_SUCCESS)
    {
        *errCode = DPU_AOAPROCHWA_ESEMA;
        goto exit;
    }

    aoaHwaObj->edmaDstIn2DFFTBuffAddr[0] = aoaHwaObj->hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PING];  //HWA M0
    aoaHwaObj->edmaDstIn2DFFTBuffAddr[1] = aoaHwaObj->hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PONG];  //HWA M1(HWA1.0) M2(HWA2.0)

    aoaHwaObj->edmaSrcOut2DFFTBuffAddr[0] = aoaHwaObj->hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PING];  //HWA M2(HWA1.0) M4(HWA2.0)
    aoaHwaObj->edmaSrcOut2DFFTBuffAddr[1] = aoaHwaObj->hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PONG];  //HWA M3(HWA1.0) M6(HWA2.0)

    aoaHwaObj->edmaDstIn3DFFTBuffAddr[0] = aoaHwaObj->hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PING];  //HWA M0
    aoaHwaObj->edmaDstIn3DFFTBuffAddr[1] = aoaHwaObj->hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_SRC_PONG];  //HWA M1(HWA1.0) M2(HWA2.0)

    aoaHwaObj->edmaSrcOut3DFFTBuffAddr[0] = aoaHwaObj->edmaDstIn3DFFTBuffAddr[0] + DPU_AOAPROCHWA_3DFFT_MAG_SQUARE_ADDRSS_OFFSET;
    aoaHwaObj->edmaSrcOut3DFFTBuffAddr[1] = aoaHwaObj->edmaDstIn3DFFTBuffAddr[1] + DPU_AOAPROCHWA_3DFFT_MAG_SQUARE_ADDRSS_OFFSET;

    aoaHwaObj->hwaAzimuthFftCmplxOutBuffAddr[0] = aoaHwaObj->hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PING];  //HWA M2(HWA1.0) M4(HWA2.0)
    aoaHwaObj->hwaAzimuthFftCmplxOutBuffAddr[1] = aoaHwaObj->hwaMemBankAddr[DPU_AOAPROCHWA_MEM_BANK_INDX_DST_PONG];  //HWA M3(HWA1.0) M6(HWA2.0)

exit:
    return ((DPU_AoAProcHWA_Handle)aoaHwaObj);
}

int32_t DPU_AoAProcHWA_config
(
    DPU_AoAProcHWA_Handle    handle,
    DPU_AoAProcHWA_Config    *aoaHwaCfg
)
{
    int32_t   retVal = 0;
    AOAHwaObj *aoaHwaObj = (AOAHwaObj *)handle;
    int32_t i;

    if(aoaHwaObj == NULL)
    {
        retVal = DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }

    /* Check if radar cube formats are supported. */
    if (!(aoaHwaCfg->res.radarCube.datafmt == DPIF_RADARCUBE_FORMAT_1))
    {
        retVal = DPU_AOAPROCHWA_EINVAL__RADARCUBE_DATAFORMAT;
        goto exit;
    }


#ifdef SUBSYS_MSS
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.cfarRngDopSnrList,
                        DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_R5F)
#else
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.cfarRngDopSnrList,
                        DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_DSP)
#endif
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_CFAR_DET_LIST;
        goto exit;
    }

#ifdef SUBSYS_MSS
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjOut,
                            DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT_R5F)
#else
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjOut,
                            DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT_DSP)
#endif
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_POINT_CLOUD_CARTESIAN;
        goto exit;
    }

#ifdef SUBSYS_MSS
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjOutSideInfo,
                            DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT_R5F)
#else
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjOutSideInfo,
                            DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT_DSP)
#endif
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_POINT_CLOUD_SIDE_INFO;
        goto exit;
    }

#ifdef SUBSYS_MSS
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.azimuthStaticHeatMap,
                        DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT_R5F)
#else
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.azimuthStaticHeatMap,
                        DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT_DSP)
#endif
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_AZIMUTH_STATIC_HEAT_MAP;
        goto exit;
    }

    /* Check if radar cube range column fits into two memory banks(32KB) */
    if(((uint16_t)aoaHwaCfg->staticCfg.numTxAntennas * (uint16_t)aoaHwaCfg->staticCfg.numRxAntennas *
            aoaHwaCfg->staticCfg.numDopplerChirps * (uint16_t)sizeof(cmplx16ImRe_t)) > HWAMEM_FOR_CUBE_RANGE_COLUMN)
    {
        retVal = DPU_AOAPROCHWA_EEXCEEDHWAMEM;
        goto exit;
    }

    /* Check if complex values of 2D FFT fit in two memory banks(32KB) */
    /* HWA is running in ping/pong manner, so only 2 banks is available */
    if(((uint16_t)aoaHwaCfg->staticCfg.numTxAntennas * (uint16_t)aoaHwaCfg->staticCfg.numRxAntennas *
            aoaHwaCfg->staticCfg.numDopplerBins * (uint16_t)sizeof(cmplx16ImRe_t)) > HWAMEM_FOR_2DFFT_INPUT)
    {
        retVal = DPU_AOAPROCHWA_EEXCEEDHWAMEM;
        goto exit;
    }

    /* Check Heapmap configuration */
    if (aoaHwaCfg->dynCfg.prepareRangeAzimuthHeatMap)
    {
        if (aoaHwaCfg->res.azimuthStaticHeatMapSize !=
          (aoaHwaCfg->staticCfg.numRangeBins * aoaHwaCfg->staticCfg.numVirtualAntAzim))
        {
            retVal = DPU_AOAPROCHWA_ENOMEM__AZIMUTH_STATIC_HEAT_MAP;
            goto exit;
        }

        if (aoaHwaCfg->res.azimuthStaticHeatMap == NULL)
        {
            retVal = DPU_AOAPROCHWA_EINVAL;
            goto exit;
        }
    }

#ifdef SUBSYS_MSS
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObj2dAzimIdx,
                            DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT_R5F)
#else
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObj2dAzimIdx,
                            DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT_DSP)
#endif
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_DET_OBJ2_AZIM_IDX;
        goto exit;
    }

#ifdef SUBSYS_MSS
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjElevationAngle,
                            DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT_R5F)
#else
    if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.detObjElevationAngle,
                            DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT_DSP)
#endif
    {
        retVal = DPU_AOAPROCHWA_ENOMEMALIGN_DET_OBJ_ELEVATION_ANGLE;
        goto exit;
    }

    if (aoaHwaCfg->dynCfg.prepareRangeAzimuthHeatMap && (aoaHwaCfg->staticCfg.numVirtualAntAzim == 1U))
    {
        retVal = DPU_AOAPROCHWA_EINVALID_NUM_VIRT_ANT_AND_AZIMUTH_STATIC_HEAT_MAP;
        goto exit;
    }

    if ((aoaHwaCfg->res.detObjOutMaxSize & 0x1U) != 0U)
    {
        retVal = DPU_AOAPROCHWA_EDETECTED_OBJECT_LIST_SIZE_ODD_NUMBER;
        goto exit;
    }

    for(i = 0; i < DPU_AOAPROCHWA_NUM_LOCAL_SCRATCH_BUFFERS; i++)
    {
#ifdef SUBSYS_MSS
        if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.localScratchBuffer[i],
                            DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT_R5F)
#else
        if CSL_MEM_IS_NOT_ALIGN(aoaHwaCfg->res.localScratchBuffer[i],
                            DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT_DSP)
#endif
        {
            retVal = DPU_AOAPROCHWA_ENOMEMALIGN_LOCAL_SCRATCH_BUF;
            goto exit;
        }
        if (aoaHwaCfg->res.localScratchBuffer[i] == NULL)
        {
            retVal = DPU_AOAPROCHWA_EINVAL;
            goto exit;
        }
    }
    if (aoaHwaCfg->res.localScratchBufferSizeBytes !=
      (DPU_AOAPROCHWA_NUM_LOCAL_SCRATCH_BUFFER_SIZE_BYTES((uint32_t)aoaHwaCfg->staticCfg.numTxAntennas)))
    {
        retVal = DPU_AOAPROCHWA_ENOMEM_LOCAL_SCRATCH_BUF;
        goto exit;
    }

#if defined(DEBUG_CHECK_PARAMS) && DEBUG_CHECK_PARAMS
    /* Validate params */
    if(!aoaHwaCfg->edmaHandle ||
      !aoaHwaCfg->hwaHandle ||
      !aoaHwaCfg->params ||
      !aoaHwaCfg->radarCube ||
      !aoaHwaCfg->detObjOut ||
      (aoaHwaCfg->numParamSet != 1)
      )
    {
        retVal= DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }
#endif
    aoaHwaObj->res = aoaHwaCfg->res;
    aoaHwaObj->params = aoaHwaCfg->staticCfg;

    aoaHwaObj->dynLocalCfg.compRxChanCfg  = *aoaHwaCfg->dynCfg.compRxChanCfg;
    aoaHwaObj->dynLocalCfg.multiObjBeamFormingCfg = *aoaHwaCfg->dynCfg.multiObjBeamFormingCfg;
    aoaHwaObj->dynLocalCfg.prepareRangeAzimuthHeatMap  = aoaHwaCfg->dynCfg.prepareRangeAzimuthHeatMap;
    AoAProcHWA_ConvertFov(aoaHwaObj, aoaHwaCfg->dynCfg.fovAoaCfg);

    aoaHwaObj->dynLocalCfg.extMaxVelCfg = *aoaHwaCfg->dynCfg.extMaxVelCfg;

    /* Allocate buffers for ping and pong paths: */
    for (i = 0; i < DPU_AOAPROCHWA_NUM_LOCAL_SCRATCH_BUFFERS; i++)
    {
		uint32_t byteAlignment;
        aoaHwaObj->azimuthFftOutMagBuf[i] = (uint16_t *) aoaHwaCfg->res.localScratchBuffer[i];
		aoaHwaObj->azimElevLocalBuf[i] = (uint32_t *)aoaHwaCfg->res.localScratchBuffer[i];
#ifdef SUBSYS_MSS
		byteAlignment = CSL_CACHE_L1D_LINESIZE;
#endif
#ifdef SUBSYS_DSS
		byteAlignment = 1U;
#endif
		aoaHwaObj->azimElevLocalHypothesesBuf[i] =
			(uint32_t *)CSL_MEM_ALIGN((uint32_t)&aoaHwaObj->azimElevLocalBuf[i][aoaHwaCfg->staticCfg.numTxAntennas *
				                       aoaHwaCfg->staticCfg.numRxAntennas],
				                       byteAlignment);
    }

    aoaHwaObj->edmaDstOut2DFFTBuffAddr[0] = (uint32_t) &aoaHwaObj->azimElevLocalBuf[0][0];
    aoaHwaObj->edmaDstOut2DFFTBuffAddr[1] = (uint32_t) &aoaHwaObj->azimElevLocalBuf[1][0];
    aoaHwaObj->edmaSrcIn3DFFTBuffAddr[0] = (uint32_t) &aoaHwaObj->azimElevLocalHypothesesBuf[0][0];
    aoaHwaObj->edmaSrcIn3DFFTBuffAddr[1] = (uint32_t) &aoaHwaObj->azimElevLocalHypothesesBuf[1][0];
    aoaHwaObj->edmaDstOut3DFFTBuffAddr[0] = (uint32_t) &aoaHwaObj->azimuthFftOutMagBuf[0][0];
    aoaHwaObj->edmaDstOut3DFFTBuffAddr[1] = (uint32_t) &aoaHwaObj->azimuthFftOutMagBuf[1][0];


    /* Windowing configuraiton in HWA */
    retVal = HWA_configRam(aoaHwaObj->hwaHandle,
                           HWA_RAM_TYPE_WINDOW_RAM,
                           (uint8_t *)aoaHwaCfg->res.hwaCfg.window,
                           aoaHwaCfg->res.hwaCfg.windowSize, /* size in bytes */
                           aoaHwaCfg->res.hwaCfg.winRamOffset * sizeof(uint32_t));
    if (retVal != 0)
    {
        goto exit;
    }

    /* Populate Shuffle LUT RAM in HWA - Azimuth */
    retVal = HWA_configRam(aoaHwaObj->hwaHandle,
                            HWA_RAM_TYPE_SHUFFLE_RAM,
                            (uint8_t *)aoaHwaCfg->staticCfg.antennaGeometryCfg, \
                            sizeof(uint16_t) * aoaHwaCfg->staticCfg.numVirtualAntAzim, 0);

    if(aoaHwaCfg->staticCfg.numVirtualAntElev > 0U)
    {
        /* Populate Shuffle LUT RAM in HWA - Elevation*/
        retVal = HWA_configRam(aoaHwaObj->hwaHandle,
                                HWA_RAM_TYPE_SHUFFLE_RAM,
                                (uint8_t *)&aoaHwaCfg->staticCfg.antennaGeometryCfg[SYS_COMMON_NUM_RX_CHANNEL * SYS_COMMON_NUM_AZIMTX_ANTENNAS], \
                                sizeof(uint16_t) * aoaHwaCfg->staticCfg.numVirtualAntElev, 16U * sizeof(uint16_t));
    }

exit:
    return retVal;
}


int32_t DPU_AoAProcHWA_process
(
    DPU_AoAProcHWA_Handle    handle,
    uint32_t        numObjsIn,
    DPU_AoAProcHWA_OutParams  *outParams
)
{
    volatile uint64_t   startTime;
    volatile uint64_t   startTime1;
    volatile uint64_t   waitTimeLocal = 0;
    int32_t             retVal = 0;
    uint32_t            idx;
    uint16_t            detObjIdxIn;
    uint16_t            detObjIdxProc;
    uint8_t             pingPongIdx;
    uint32_t            trueNumObjsIn;
    float               range;
    uint32_t            numObjsOut = 0;
    uint16_t numParams;
    HWA_Handle hwaHandle;
    uint32_t baseAddr, regionId;
    int16_t             temp;

#ifdef SUBSYS_MSS
    uint32_t cfarListWritePosition;
    bool isCfarListUpdated = false;
#endif

    AOAHwaObj *aoaHwaObj;
    DPU_AoAProcHWA_HW_Resources *res;
    DPU_AoAProcHWA_StaticConfig *DPParams;
    if (handle == NULL)
    {
        retVal = DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }
    aoaHwaObj = (AOAHwaObj *)handle;
    res = &aoaHwaObj->res;
    DPParams = &aoaHwaObj->params;
    hwaHandle = aoaHwaObj->hwaHandle;

    baseAddr = EDMA_getBaseAddr(res->edmaHandle);
    DebugP_assert(baseAddr != 0U);

    regionId = EDMA_getRegionId(res->edmaHandle);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);


    startTime = ClockP_getTimeUsec();

    if (numObjsIn == 0U)
    {
        outParams->numAoADetectedPoints = numObjsIn;
        outParams->stats.waitTime = 0;
        outParams->stats.processingTime = 0;
        retVal = 0;
        goto exit;
    }

    /**********************************************/
    /* ENABLE NUMLOOPS DONE INTERRUPT FROM HWA */
    /**********************************************/
    retVal = HWA_enableDoneInterrupt(hwaHandle,
		                             HWA_THREAD_BACKGROUNDCONTEXT,
                                     AOAProcHWADoneIsrCallback,
                                     aoaHwaObj);
    if (retVal != 0)
    {
        goto exit;
    }

    /* Azimuth heap doppler compensation */
    if (aoaHwaObj->dynLocalCfg.prepareRangeAzimuthHeatMap)
    {
        /* Perform 2D FFT for all range bins and save zero Doppler bin */
        retVal = AoAProcHWA_PrepareRangeAzimuthHeatmap(aoaHwaObj,
                                                       &waitTimeLocal);
        if (retVal != 0)
        {
            goto exit;
        }
    }

    /* Angle estimation */
    if(DPParams->numVirtualAntAzim == 1U)
    {
       /*If there is only one virtual antenna, there is no
         need of azimuth FFT as azimuth can not be estimated.*/

        /* Limit number of input samples */
        if (numObjsIn > res->detObjOutMaxSize)
        {
          numObjsIn = res->detObjOutMaxSize;
        }

        /* Fill the output list */
        for(idx=0; idx < numObjsIn; idx++)
        {
            range = (float)res->cfarRngDopSnrList[idx].rangeIdx * DPParams->rangeStep;
            res->detObjOut[idx].y = range;
            res->detObjOut[idx].x = 0.0F;
            res->detObjOut[idx].z = 0.0F;
            temp = AOA_DOPPLER_IDX_TO_SIGNED(res->cfarRngDopSnrList[idx].dopplerIdx, DPParams->numDopplerBins);
            res->detObjOut[idx].velocity = DPParams->dopplerStep * (float)temp;
            res->detObjOutSideInfo[idx].snr = res->cfarRngDopSnrList[idx].snr;
            res->detObjOutSideInfo[idx].noise = res->cfarRngDopSnrList[idx].noise;
        }
        numObjsOut = numObjsIn;
    }
    else
    {
        /* Limit number of input samples */
        if (numObjsIn > res->detObjOutMaxSize)
        {
          numObjsIn = res->detObjOutMaxSize;
        }
        trueNumObjsIn = numObjsIn;
        /* Make number of detected objects EVEN */
        if((numObjsIn & 0x1U) != 0U)
        {
            /* If odd number of input objects, add dummy as duplicated last object */
            res->cfarRngDopSnrList[numObjsIn] = res->cfarRngDopSnrList[numObjsIn-1U];
#ifdef SUBSYS_MSS
            cfarListWritePosition = numObjsIn;
            isCfarListUpdated = true;
#endif
            numObjsIn++;
        }

        /* Configure HWA Params*/
        retVal = HWAutil_configHWA_extendedVelocityScheme(hwaHandle,
                                        res,
                                        DPParams,
                                        aoaHwaObj->hwaMemBankAddr,
                                        aoaHwaObj->dynLocalCfg.extMaxVelCfg.enabled,
                                        DPU_AOAPROCHWA_NUM_ANGLE_BINS);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Configure Common HWA registers */
        numParams = DPU_AoAProcHWA_getNumHwaParamSets(DPParams->numTxAntennas,
                                                      DPParams->numVirtualAntElev,
                                                      DPParams->numElevFFTParams,
                                                      DPParams->staticClutterCfg.isEnabled);
        retVal =  AoAProc_HWAutil_configCommon (hwaHandle,
                                                (uint16_t)numObjsIn / 2U, //numLoops,
                                                res->hwaCfg.paramSetStartIdx, //paramStartIdx,
                                                numParams,
                                                aoaHwaObj);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Configure EDMA */
        retVal = AoAProcHWA_config_EDMA(hwaHandle,
                                        res,
                                        DPParams,
                                        0,//srcIn2DFFTBuffAddr
                                        aoaHwaObj->edmaDstIn2DFFTBuffAddr,
                                        aoaHwaObj->edmaSrcOut2DFFTBuffAddr,
                                        aoaHwaObj->edmaDstOut2DFFTBuffAddr,
                                        aoaHwaObj->edmaSrcIn3DFFTBuffAddr,
                                        aoaHwaObj->edmaDstIn3DFFTBuffAddr,
                                        aoaHwaObj->edmaSrcOut3DFFTBuffAddr,
                                        aoaHwaObj->edmaDstOut3DFFTBuffAddr,
                                        aoaHwaObj->dynLocalCfg.extMaxVelCfg.enabled);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Enable the HWA */
        retVal = HWA_enable(hwaHandle, 1);
        if (retVal != 0)
        {
            goto exit;
        }

        /* Trigger first two objects */
        detObjIdxIn = 0;
        pingPongIdx = 0;
        (void)AoAProcHWA_cfgAndTrigger_EDMA_2DFFT(aoaHwaObj,
                                      detObjIdxIn,
                                      pingPongIdx);

        detObjIdxIn = 1;
        pingPongIdx = 1;
        (void)AoAProcHWA_cfgAndTrigger_EDMA_2DFFT(aoaHwaObj,
                                      detObjIdxIn,
                                      pingPongIdx);


        /* Loop through the list two objects per loop */
        detObjIdxIn = 2;
        detObjIdxProc = 0;
        while(detObjIdxProc < numObjsIn)
        {
            for (pingPongIdx = 0; pingPongIdx < 2U; pingPongIdx++)
            {
                /* Wait until EDMA output is done */
                startTime1 = ClockP_getTimeUsec();
                while(EDMA_readIntrStatusRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chOut.channel) != 1U)
                {
                    /* Wait for EDMA completion */
                }
                EDMA_clrIntrRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chOut.channel);

                waitTimeLocal += ClockP_getTimeUsec() - startTime1;
#ifdef SUBSYS_MSS
				/* if azimElevLocalBuf in L2 or L3, invalidate the cache before compensation  */
				CacheP_inv(aoaHwaObj->azimElevLocalBuf[pingPongIdx],
					(uint32_t)sizeof(cmplx16ImRe_t) * ((uint32_t)DPParams->numVirtualAntAzim + (uint32_t)DPParams->numVirtualAntElev), CacheP_TYPE_ALLD);
#endif

                /* Rx channel gain/phase offset compensation */
                AoAProcHWA_rxChanPhaseBiasCompensation(&aoaHwaObj->dynLocalCfg.compRxChanCfg.rxChPhaseComp[0],
                                                    1,
                                                    (uint32_t)DPParams->numVirtualAntAzim + (uint32_t)DPParams->numVirtualAntElev,
                                                    (cmplx16ImRe_t *) aoaHwaObj->azimElevLocalBuf[pingPongIdx],
                                                    (cmplx16ImRe_t *) aoaHwaObj->azimElevLocalBuf[pingPongIdx]);


                /* Doppler Compensation - */
                aoaHwa_dopplerCompensation((uint32_t *) aoaHwaObj->azimElevLocalBuf[pingPongIdx],
                                           &res->cfarRngDopSnrList[detObjIdxProc + pingPongIdx],
                                           (uint32_t *) aoaHwaObj->azimElevLocalHypothesesBuf[pingPongIdx],
                                           DPParams->numTxAntennas,
                                           DPParams->numRxAntennas,
                                           DPParams->numVirtualAntAzim,
                                           DPParams->numVirtualAntElev,
                                           DPParams->numDopplerBins,
                                           aoaHwaObj->dynLocalCfg.extMaxVelCfg.enabled);
#ifdef SUBSYS_MSS
				{
					/* the hypothesesBuf is written back before EDMA transfer the data to HWA for 3D FFT */
					uint32_t numHypotheses;
					if (aoaHwaObj->dynLocalCfg.extMaxVelCfg.enabled == 0U)
					{
						numHypotheses = 1;
					}
					else
					{
						numHypotheses = DPParams->numTxAntennas;
					}
					/* Buffer azimElevLocalBuf is not used any more down the processing path and we have to
					 * ensure that cache is invalidated on this part of scratch memory and no dirty cache lines are left.
					 * This part of scratch memory is reused later on in the processing path */
					CacheP_inv(aoaHwaObj->azimElevLocalBuf[pingPongIdx],
					   (uint32_t)sizeof(cmplx16ImRe_t) * ((uint32_t)DPParams->numVirtualAntAzim + (uint32_t)DPParams->numVirtualAntElev), CacheP_TYPE_ALLD);
                    CacheP_wb(aoaHwaObj->azimElevLocalHypothesesBuf[pingPongIdx],
                       (uint32_t)sizeof(cmplx16ImRe_t) * (uint32_t)numHypotheses * ((uint32_t)DPParams->numVirtualAntAzim + (uint32_t)DPParams->numVirtualAntElev), CacheP_TYPE_ALLD);
				}
#endif

                /* Trigger 3D-FFT */
                (void)EDMA_enableTransferRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chIn.channel, EDMA_TRIG_MODE_MANUAL);
            }

            for (pingPongIdx = 0U; pingPongIdx < 2U; pingPongIdx++)
            {
                /* Wait until EDMA outputs 3D-FFTs and Maximum peaks */
                startTime1 = ClockP_getTimeUsec();
                while(EDMA_readIntrStatusRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chOut.channel) != 1U)
                {
                    /* Wait for EDMA completion */
                }
                EDMA_clrIntrRegion(baseAddr, regionId, res->edmaHwaExt[pingPongIdx].chOut.channel);

                waitTimeLocal += ClockP_getTimeUsec() - startTime1;
				if (detObjIdxProc < trueNumObjsIn)
				{
#ifdef SUBSYS_MSS
					{
						uint32_t numHypotheses;
						if (aoaHwaObj->dynLocalCfg.extMaxVelCfg.enabled == 0U)
						{
							numHypotheses = 1;
						}
						else
						{
							numHypotheses = DPParams->numTxAntennas;
			            }
						CacheP_inv(aoaHwaObj->azimuthFftOutMagBuf[pingPongIdx],
							       sizeof(uint16_t) * numHypotheses * DPU_AOAPROCHWA_NUM_ANGLE_BINS, CacheP_TYPE_ALLD);
					}
#endif
                    numObjsOut = AoAProcHWA_angleEstimationAzimElev(aoaHwaObj,
                                                                    detObjIdxProc,
                                                                    pingPongIdx,
                                                                    numObjsOut);
                }
                if (detObjIdxIn < numObjsIn)
                {
                    /* Trigger 2D-FFT - next target */
                    (void)AoAProcHWA_cfgAndTrigger_EDMA_2DFFT(aoaHwaObj,
                                                         detObjIdxIn,
                                                         pingPongIdx);
                }
                detObjIdxProc++;
                detObjIdxIn++;
            }

        } /* Loop per two detected points */

        /* Disable the HWA */
        retVal = HWA_enable(hwaHandle, 0);
        if (retVal != 0)
        {
            goto exit;
        }

    }

#ifdef SUBSYS_MSS
    /* Write back output lists as per DPU design rules */
    CacheP_wb(&res->detObjOut[0], numObjsOut * sizeof(DPIF_PointCloudCartesian), CacheP_TYPE_ALLD);
    CacheP_wb(&res->detObjOutSideInfo[0], numObjsOut * sizeof(DPIF_PointCloudSideInfo), CacheP_TYPE_ALLD);
    if(isCfarListUpdated == true)
    {
        CacheP_wb(&res->cfarRngDopSnrList[cfarListWritePosition], sizeof(DPIF_CFARDetList), CacheP_TYPE_ALLD);
    }
    CacheP_wb(&res->detObjElevationAngle[0], numObjsOut * sizeof(float), CacheP_TYPE_ALLD);
    CacheP_wb(&res->detObj2dAzimIdx[0], numObjsOut * sizeof(uint8_t), CacheP_TYPE_ALLD);
#endif

    outParams->numAoADetectedPoints = numObjsOut;
    outParams->stats.waitTime = waitTimeLocal;
    outParams->stats.processingTime = ClockP_getTimeUsec() - startTime - waitTimeLocal;
#ifdef PROFILE_AOA_HWA_OBJ_DPU
    gAoAProcStats[gAoAProcStatsIdx] = *outParams;
    gAoAProcStatsIdx = (gAoAProcStatsIdx +1) & 0xf;
#endif

    /* Process is done, disable Done interrupt */
    (void)HWA_disableDoneInterrupt(hwaHandle, HWA_THREAD_BACKGROUNDCONTEXT);
exit:
    return retVal;
}

int32_t DPU_AoAProcHWA_control
(
   DPU_AoAProcHWA_Handle handle,
   DPU_AoAProcHWA_Cmd cmd,
   void *arg,
   uint32_t argSize
)
{
   int32_t retVal = 0;
   AOAHwaObj *aoaHwaObj = (AOAHwaObj *)handle;

   /* Get rangeProc data object */
   if (aoaHwaObj == NULL)
   {
       retVal = DPU_AOAPROCHWA_EINVAL;
       goto exit;
   }

   switch(cmd)
   {
       case DPU_AoAProcHWA_Cmd_FovAoACfg:
       {
           if((argSize != sizeof(DPU_AoAProc_FovAoaCfg)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
               /* Save configuration */
               AoAProcHWA_ConvertFov(aoaHwaObj, (DPU_AoAProc_FovAoaCfg *) arg);

           }
       }
       break;
       case DPU_AoAProcHWA_Cmd_MultiObjBeamFormingCfg:
       {
           if((argSize != sizeof(DPU_AoAProc_MultiObjBeamFormingCfg)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
               /* Save configuration */
               (void)memcpy((void *)&aoaHwaObj->dynLocalCfg.multiObjBeamFormingCfg, arg, argSize);
           }
       }
       break;
       case DPU_AoAProcHWA_Cmd_ExtMaxVelocityCfg:
       {
           if((argSize != sizeof(DPU_AoAProc_ExtendedMaxVelocityCfg)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
               /* Save configuration */
               (void)memcpy((void *)&aoaHwaObj->dynLocalCfg.extMaxVelCfg, arg, argSize);
           }
       }
       break;
       case DPU_AoAProcHWA_Cmd_CompRxChannelBiasCfg:
       {
           if((argSize != sizeof(DPU_AoAProc_compRxChannelBiasCfg)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
               /* Save configuration */
               (void)memcpy((void *)&aoaHwaObj->dynLocalCfg.compRxChanCfg, arg, argSize);
           }
       }
       break;
       case DPU_AoAProcHWA_Cmd_PrepareRangeAzimuthHeatMap:
       {
           if((argSize != sizeof(bool)) ||
              (arg == NULL))
           {
               retVal = DPU_AOAPROCHWA_EINVAL;
               goto exit;
           }
           else
           {
                /* Sanity check on saved heapmap ptr and size */
                if ((aoaHwaObj->res.azimuthStaticHeatMapSize !=
                    (aoaHwaObj->params.numRangeBins * aoaHwaObj->params.numVirtualAntAzim)) ||
                    (aoaHwaObj->res.azimuthStaticHeatMap == NULL))
                {
                   retVal = DPU_AOAPROCHWA_EINVAL;
                   goto exit;
                }

                /* Save configuration */
                (void)memcpy((void *)&aoaHwaObj->dynLocalCfg.prepareRangeAzimuthHeatMap, arg, argSize);
           }
       }
       break;
       default:
           retVal = DPU_AOAPROCHWA_EINVAL;
           break;
   }
exit:
   return (retVal);
}

int32_t DPU_AoAProcHWA_deinit(DPU_AoAProcHWA_Handle handle)
{
    int32_t retVal = 0;

    if (handle == NULL)
    {
        retVal = DPU_AOAPROCHWA_EINVAL;
        goto exit;
    }

exit:
    return (retVal);
}
