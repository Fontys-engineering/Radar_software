/**
 *   @file  hwa_main.c
 *
 *   @brief
 *      Unit Test code for range hwa dpu processing.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2018 -2025 Texas Instruments, Inc.
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
#include <stdio.h>
#include <math.h>

/* MCU Plus Include Files. */
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/CacheP.h>
#include <kernel/dpl/ClockP.h>
#include <drivers/edma.h>
#ifdef SUBSYS_DSS
#include <ti/datapath/dpu/rangeproc/test/dssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/rangeproc/test/dssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/rangeproc/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/rangeproc/test/dssgenerated/ti_board_open_close.h>
#endif
#ifdef SUBSYS_MSS
#include <ti/datapath/dpu/rangeproc/test/mssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/rangeproc/test/mssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/rangeproc/test/mssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/rangeproc/test/mssgenerated/ti_board_open_close.h>
#endif
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmwave SDK files */
#include <ti/datapath/dpedma/dpedmahwa.h>
#include <ti/datapath/dpedma/dpedma.h>
#include <ti/datapath/dpu/rangeproc/rangeprochwa.h>
#include <ti/utils/testlogger/logger.h>

#define  MAX_NUM_RANGEBIN   2048
#define  MAX_NUM_RX_ANTENNA 4
#define  MAX_NUM_ADCSAMPLE_PERCHIRP    (MAX_NUM_RANGEBIN *  MAX_NUM_RX_ANTENNA)


/* Event queue to be used  */
#define EDMA_TEST_EVT_QUEUE_NO      (0U)

#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
#define  MAX_NUM_TX_ANTENNA 4
#else
#define  MAX_NUM_TX_ANTENNA 3
#endif

#define  MAX_NUM_CHIRPS_PERFRAME  6 //matlab generate input: tx=3, number of chirps = 6, otherwise = 4,
#define  NUM_REPEAT_PERFRAME 2      //for testing, repeat same data per frame
#define  MAX_ALLOWED_ERROR 13


#ifdef SUBSYS_MSS
/* data memeory */
uint32_t  dataInBuffer[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME] \
        __attribute__ ((aligned(CSL_CACHE_L1D_LINESIZE), section(".dataBuf")));; /*to save all adc buffer data */

uint32_t  adcDataIn[MAX_NUM_ADCSAMPLE_PERCHIRP] \
          __attribute__ ((aligned(CSL_CACHE_L1D_LINESIZE), section(".dataBuf"))); /*adc buffer for one chirp*/
#endif
#ifdef SUBSYS_DSS
/* data memeory */
#pragma DATA_SECTION(dataInBuffer, ".dataBuf");
uint32_t  dataInBuffer[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME]; /*to save all adc buffer data */
#pragma DATA_SECTION(adcDataIn, ".dataBufL2");
uint32_t  adcDataIn[MAX_NUM_ADCSAMPLE_PERCHIRP] \
          __attribute__ ((aligned(CSL_CACHE_L1D_LINESIZE))); /*adc buffer for one chirp*/
#endif

#ifdef SUBSYS_MSS
/* hwa 18 bits, whole length */
uint32_t  window1DCoef[MAX_NUM_RANGEBIN]__attribute__((aligned(8), section(".dataBuf")));
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(window1DCoef, ".dataBufL2");
#pragma DATA_ALIGN(window1DCoef,8);
uint32_t  window1DCoef[MAX_NUM_RANGEBIN];  //hwa 18 bits, whole length
#endif


/************** dc range sig mean *******************************/
#ifdef SUBSYS_MSS
#define DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT \
        DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_R5F
uint8_t gDcRangeSigMeanBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_TX_ANTENNA * MAX_NUM_RX_ANTENNA \
                                                * 32 * sizeof(cmplx32ImRe_t), \
                                                DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT), section(".dataBuf")));
#else
#define DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT \
        DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT_DSP
#pragma DATA_SECTION(gDcRangeSigMeanBuf, ".dataBufL2");
uint8_t gDcRangeSigMeanBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_TX_ANTENNA * MAX_NUM_RX_ANTENNA \
                                                * 32 * sizeof(cmplx32ImRe_t), \
                                                DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(DPU_RANGEPROCHWA_DCRANGESIGMEAN_BYTE_ALIGNMENT)));
#endif

cmplx32ImRe_t *gDcRangeSigMean = (cmplx32ImRe_t *) &gDcRangeSigMeanBuf[0];

/************** radar cube *******************************/
#ifdef SUBSYS_MSS
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT \
        DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_R5F
#else
#define DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT \
        DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT_DSP
#endif

#ifdef SUBSYS_MSS
uint8_t gRadarCubeBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_ADCSAMPLE_PERCHIRP * \
                                MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME * \
                                sizeof(cmplx16ImRe_t), \
                                DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)] \
              __attribute__ ((aligned(DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT), section(".l3data")));
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(gRadarCubeBuf, ".l3data");
uint8_t gRadarCubeBuf[CSL_NEXT_MULTIPLE_OF(MAX_NUM_ADCSAMPLE_PERCHIRP * \
                                MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME * \
                                sizeof(cmplx16ImRe_t), \
                                DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)] \
              __attribute__ ((aligned(DPU_RANGEPROCHWA_RADARCUBE_BYTE_ALIGNMENT)));
#endif


cmplx16ImRe_t *gRadarCube = (cmplx16ImRe_t *) &gRadarCubeBuf[0];

#ifdef SUBSYS_MSS
uint32_t fft1DOut_ref[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME]__attribute__((aligned(64), section(".l3data")));
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(fft1DOut_ref, ".l3data");
/* fixed to 16I and 16Q */
uint32_t fft1DOut_ref[MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME];
#endif

#ifdef SUBSYS_MSS
/* per rx antenna, per chirp for windowing results */
uint32_t dataWinOut[MAX_NUM_RANGEBIN]__attribute__((aligned(64), section(".dataBuf")));
#endif
#ifdef SUBSYS_DSS
#pragma DATA_SECTION(dataWinOut, ".dataBufL2");
uint32_t dataWinOut[MAX_NUM_RANGEBIN]; /* per rx antenna, per chirp for windowing results */
#endif

typedef struct rangeProcTestConfig_t_ {
    uint32_t numTxAntennas;
    uint32_t numRxAntennas;
    bool     isReal;
    uint32_t numAdcSamples;
    /* The number of range bins in this structure will not be half the number of ADC samples
    if isReal is 1. However, the number of range bins sent to the rangeproc DPU will
    be half the number of range bins, in case of real only samples. */
    uint32_t numRangeBins;
    uint32_t dpuNumRangeBins;
    uint32_t numChirpsPerFrame;
    uint32_t numChirpsPerFrameRef;
   // uint32_t numDopplerBins;
    uint8_t  cmplxIQswapFlag;
    uint8_t  rxChanInterleave;
    uint8_t  radarCubeLayoutFmt;
} rangeProcTestConfig_t;

#define  MMW_HWA_WINDOWRAM_1D_OFFSET         0

#define EDMA_NUM_DMA_CHANNELS               64

/* EDMA resource partitioning */
#define EDMA_SHADOW_LNK_PARAM_BASE_ID       EDMA_NUM_DMA_CHANNELS

#define EDMA_INSTANCE_ID                    EDMA_DRV_INST_DSS_A

#define EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL      EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ3///temp

/* 1D Input */
#define MMW_EDMA_1DIN_SHADOW_LINK_CH_ID (EDMA_SHADOW_LNK_PARAM_BASE_ID + 4)
#define MMW_EDMA_1DINSIGNATURE_CH_ID (EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ2)
#define MMW_EDMA_1DINSIGNATURE_PING_SHADOW_LINK_CH_ID (EDMA_SHADOW_LNK_PARAM_BASE_ID + 5)

/* 1D -ping */
#define MMW_EDMA_1D_PING_CH_ID              EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ0
#define MMW_EDMA_1D_PING_CHAIN_CH_ID        EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ4
#define MMW_EDMA_1D_PING_SHADOW_LINK_CH_ID  EDMA_SHADOW_LNK_PARAM_BASE_ID
#define MMW_EDMA_1D_PING_ONE_HOT_SHADOW_LINK_CH_ID (EDMA_SHADOW_LNK_PARAM_BASE_ID + 2)

/* 1D - pong */
#define MMW_EDMA_1D_PONG_CH_ID               EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ1
#define MMW_EDMA_1D_PONG_SHADOW_LINK_CH_ID  (EDMA_SHADOW_LNK_PARAM_BASE_ID + 1)

uint32_t     pingChan[3] =
{
    EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ0,
    EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ1,
    EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ2
};
uint32_t     pongChan[3] =
{
    EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ3,
    EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ4,
    EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ5
};

uint32_t     pingDummyChanShadow[3]=
{
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 7,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 8,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 9
};
uint32_t     pingChanShadow[3]=
{
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 10,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 11,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 12
};

uint32_t     pongDummyChanShadow[3]=
{
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 13,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 14,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 15
};
uint32_t     pongChanShadow[3]=
{
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 16,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 17,
    EDMA_SHADOW_LNK_PARAM_BASE_ID + 18
};

#define PI_ 3.14159265

DPU_RangeProc_CalibDcRangeSigCfg gCalibDcConfig=
{
    0,  //disabled
    -5, // negativeBinIdx
    5,  //positiveBinIdx
    128 //number of average chirps
};

/* Is input chirp data real? Can be 0 (i.e., complex data) only for AM273X */
bool gIsChirpDataReal = 0;

HWA_Handle          hwaHandle;
volatile uint8_t    subframeCount = 0;

DPU_RangeProcHWA_Handle rangeProcDpuHandle;
DPU_RangeProcHWA_Config rangeProcDpuCfg;
/*! @brief     EDMA interrupt object */
Edma_IntrObject intrObj;

uint8_t testFeatureStatus;

uint8_t checkResultFlag;

uint8_t finalResults ;

/**
 * @brief
 *  Initialize the MCPI Log Message Buffer
 */
MCPI_LOGBUF_INIT(2*9216);

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (5U)
#define APP_TASK_STACK_SIZE  ( 6*MAX_NUM_RANGEBIN)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));

FILE * fileId;

/**
 *  @b Description
 *  @n
 *      Allocates Shawdow paramset
 */
static void allocateEDMAShadowChannel(uint32_t *param)
{
    int32_t             testStatus = SystemP_SUCCESS;

    testStatus = EDMA_allocParam(gEdmaHandle[0], param);
    DebugP_assert(testStatus == SystemP_SUCCESS);

    return;
}

/**
*  @b Description
*  @n
*    Based on the test configuration, set up the range processing DPU configurations
*/
void Test_setProfile(rangeProcTestConfig_t * testConfig)
{
    DPU_RangeProcHWA_HW_Resources  *pHwConfig;
    DPU_RangeProcHWA_StaticConfig  * params;
    uint32_t index;
    uint32_t bytesPerRxChan;
    uint16_t numBytesPerInputSample;
    uint32_t            dmaCh, tcc, param;

    pHwConfig = &rangeProcDpuCfg.hwRes;
    params = &rangeProcDpuCfg.staticCfg;
    rangeProcDpuCfg.hwRes.intrObj = &intrObj;

    /* overwrite the DPIF_commonParams with the test configuration*/
    params->numTxAntennas = testConfig->numTxAntennas;
    params->numVirtualAntennas = testConfig->numTxAntennas * testConfig->numRxAntennas;
    params->numChirpsPerFrame = testConfig->numChirpsPerFrame;
    params->isChirpDataReal = testConfig->isReal;
    if(params->isChirpDataReal){
        numBytesPerInputSample = sizeof(int16_t);
        params->numRangeBins = testConfig->numRangeBins/2;
        params->numFFTBins = testConfig->numRangeBins;
    }
    else{
        numBytesPerInputSample = sizeof(uint32_t); /* Complex (16 bit real, 16 bit imag) */
        params->numRangeBins = testConfig->numRangeBins;
        params->numFFTBins = testConfig->numRangeBins;
    }

    /* windowing */
    params->windowSize = sizeof(uint32_t) * ((testConfig->numAdcSamples +1 ) / 2); //hwa is 18 bits, whole length
    params->ADCBufData.dataSize = testConfig->numAdcSamples * testConfig->numRxAntennas * numBytesPerInputSample ;
    params->ADCBufData.dataProperty.numAdcSamples = testConfig->numAdcSamples;
    params->ADCBufData.dataProperty.numRxAntennas = testConfig->numRxAntennas;

    if (params->numRangeBins == 2048)
    {
        params->rangeFFTtuning.fftOutputDivShift = 0;
        params->rangeFFTtuning.numLastButterflyStagesToScale = 3; /* scale only 2 stages */
    } else if (params->numRangeBins == 1024)
    {
        params->rangeFFTtuning.fftOutputDivShift = 0;
        params->rangeFFTtuning.numLastButterflyStagesToScale = 2; /* scale only 2 stages */
    } else if (params->numRangeBins == 512)
    {
        params->rangeFFTtuning.fftOutputDivShift = 1;
        params->rangeFFTtuning.numLastButterflyStagesToScale = 1; /* scale last stage */
    } else
    {
        params->rangeFFTtuning.fftOutputDivShift = 2;
        params->rangeFFTtuning.numLastButterflyStagesToScale = 0; /* no scaling needed as ADC is 16-bit and we have 8 bits to grow */
    }

    if (testConfig->rxChanInterleave)
    {
        //bytesPerRxChan = testConfig->numAdcSamples * sizeof(uint32_t);
        //bytesPerRxChan = (bytesPerRxChan + 15) / 16 * 16;
        bytesPerRxChan = 0;
        for (index = 0; index < testConfig->numRxAntennas; index++)
        {
            params->ADCBufData.dataProperty.rxChanOffset[index] = index * bytesPerRxChan;
        }

        params->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_INTERLEAVE_MODE;

        /* Data Input EDMA */
        dmaCh = EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL;
        tcc   = EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL;
        param = EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL;
        DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
        pHwConfig->edmaInCfg.dataIn.channel         = dmaCh;
        pHwConfig->edmaInCfg.dataIn.paramId         = param;
        pHwConfig->edmaInCfg.dataIn.tcc             = tcc;

        param = MMW_EDMA_1DIN_SHADOW_LINK_CH_ID;
        allocateEDMAShadowChannel(&param);
        pHwConfig->edmaInCfg.dataIn.shadowPramId   = param;
        pHwConfig->edmaInCfg.dataIn.eventQueue      = 0;

        dmaCh = MMW_EDMA_1DINSIGNATURE_CH_ID;
        tcc   = MMW_EDMA_1DINSIGNATURE_CH_ID;
        param = MMW_EDMA_1DINSIGNATURE_CH_ID;
        DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
        pHwConfig->edmaInCfg.dataInSignature.channel         = dmaCh;
        pHwConfig->edmaInCfg.dataInSignature.paramId         = param;
        pHwConfig->edmaInCfg.dataInSignature.tcc             = tcc;

        param = MMW_EDMA_1DINSIGNATURE_PING_SHADOW_LINK_CH_ID;
        allocateEDMAShadowChannel(&param);
        pHwConfig->edmaInCfg.dataInSignature.shadowPramId   = param;
        pHwConfig->edmaInCfg.dataInSignature.eventQueue      = 0;

        /* edma set up*/
        /* Output Ping*/
        dmaCh = MMW_EDMA_1D_PING_CH_ID;
        tcc   = MMW_EDMA_1D_PING_CH_ID;
        param = MMW_EDMA_1D_PING_CH_ID;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channel  = dmaCh;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.paramId   = param;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.tcc       = tcc;

        param = MMW_EDMA_1D_PING_SHADOW_LINK_CH_ID;
        allocateEDMAShadowChannel(&param);
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.shadowPramId   = param;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.eventQueue= 0;

        /* Output Pong*/
        dmaCh = MMW_EDMA_1D_PONG_CH_ID;
        tcc   = MMW_EDMA_1D_PONG_CH_ID;
        param = MMW_EDMA_1D_PONG_CH_ID;
        DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channel         = dmaCh;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.paramId          = param;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.tcc              = tcc;

        param = MMW_EDMA_1D_PONG_SHADOW_LINK_CH_ID;
        allocateEDMAShadowChannel(&param);
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.shadowPramId    = param;
        pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.eventQueue       = 0;

        /* Output signature channel */
        dmaCh = MMW_EDMA_1D_PING_CHAIN_CH_ID;
        tcc   = MMW_EDMA_1D_PING_CHAIN_CH_ID;
        param = MMW_EDMA_1D_PING_CHAIN_CH_ID;
        DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
        pHwConfig->edmaOutCfg.dataOutSignature.channel        = dmaCh;
        pHwConfig->edmaOutCfg.dataOutSignature.paramId         = param;
        pHwConfig->edmaOutCfg.dataOutSignature.tcc             = tcc;

        param = MMW_EDMA_1D_PING_ONE_HOT_SHADOW_LINK_CH_ID;
        allocateEDMAShadowChannel(&param);
        pHwConfig->edmaOutCfg.dataOutSignature.shadowPramId = param;
        pHwConfig->edmaOutCfg.dataOutSignature.eventQueue = 0;

    }
    else
    {
        bytesPerRxChan = testConfig->numAdcSamples * numBytesPerInputSample;
        bytesPerRxChan = (bytesPerRxChan + 15) / 16 * 16;

        for (index = 0; index < testConfig->numRxAntennas; index++)
        {
            params->ADCBufData.dataProperty.rxChanOffset[index] = index * bytesPerRxChan;
        }

        params->ADCBufData.dataProperty.interleave = DPIF_RXCHAN_NON_INTERLEAVE_MODE;
        /* Data Input EDMA */
        dmaCh = EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL;
        tcc   = EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL;
        param = EDMA_TPCC0_REQ_DFE_CHIRP_AVAIL;
        DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
        pHwConfig->edmaInCfg.dataIn.channel         = dmaCh;
        pHwConfig->edmaInCfg.dataIn.paramId         = param;
        pHwConfig->edmaInCfg.dataIn.tcc             = tcc;

        param = MMW_EDMA_1DIN_SHADOW_LINK_CH_ID;
        allocateEDMAShadowChannel(&param);
        pHwConfig->edmaInCfg.dataIn.shadowPramId   = param;
        pHwConfig->edmaInCfg.dataIn.eventQueue      = 0;

        dmaCh = MMW_EDMA_1DINSIGNATURE_CH_ID;
        tcc   = MMW_EDMA_1DINSIGNATURE_CH_ID;
        param = MMW_EDMA_1DINSIGNATURE_CH_ID;
        DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
        pHwConfig->edmaInCfg.dataInSignature.channel         = dmaCh;
        pHwConfig->edmaInCfg.dataInSignature.paramId         = param;
        pHwConfig->edmaInCfg.dataInSignature.tcc             = tcc;

        param = MMW_EDMA_1DINSIGNATURE_PING_SHADOW_LINK_CH_ID;
        allocateEDMAShadowChannel(&param);
        pHwConfig->edmaInCfg.dataInSignature.shadowPramId   = param;
        pHwConfig->edmaInCfg.dataInSignature.eventQueue      = 0;

        if((testConfig->numTxAntennas == 3)&& (testConfig->radarCubeLayoutFmt == 1))
        {

            /* PING */
            dmaCh = MMW_EDMA_1D_PING_CH_ID;
            tcc   = MMW_EDMA_1D_PING_CH_ID;
            param = MMW_EDMA_1D_PING_CH_ID;
            DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.channel         = dmaCh;
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.paramId         = param;
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.tcc             = tcc;
            for(index = 0; index< 3; index++)
            {
                param = pingDummyChanShadow[index];
                allocateEDMAShadowChannel(&param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.ShadowPramId[index]   = param;
            }
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.eventQueue= 0;

            for(index = 0; index< 3; index++)
            {
                dmaCh = pingChan[index];
                tcc   = pingChan[index];
                param = pingChan[index];
                DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].channel = dmaCh;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].paramId  = param;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].tcc      = tcc;

                param = pingChanShadow[index];
                allocateEDMAShadowChannel(&param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].shadowPramId = param;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].eventQueue = 0;
            }

            /* PONG */
            dmaCh = MMW_EDMA_1D_PONG_CH_ID;
            tcc   = MMW_EDMA_1D_PONG_CH_ID;
            param = MMW_EDMA_1D_PONG_CH_ID;
            DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.channel         = dmaCh;
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.paramId         = param;
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.tcc             = tcc;
            for(index = 0; index< 3; index++)
            {
                param = pongDummyChanShadow[index];
                allocateEDMAShadowChannel(&param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.ShadowPramId[index]   = param;
            }
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.eventQueue       = 0;

            for(index = 0; index< 3; index++)
            {
                dmaCh = pongChan[index];
                tcc   = pongChan[index];
                param = pongChan[index];
                DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].channel = dmaCh;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].paramId = param;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].tcc     = tcc;

                param = pongChanShadow[index];
                allocateEDMAShadowChannel(&param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].shadowPramId = param;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].eventQueue = 0;
            }
        }
        else if((testConfig->numTxAntennas == 4)&& (testConfig->radarCubeLayoutFmt == 1))
        {

            /* PING */
            dmaCh = MMW_EDMA_1D_PING_CH_ID;
            tcc   = MMW_EDMA_1D_PING_CH_ID;
            param = MMW_EDMA_1D_PING_CH_ID;
            DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.channel         = dmaCh;
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.paramId         = param;
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.tcc             = tcc;
            for(index = 0; index< 2; index++)
            {
                param = pingDummyChanShadow[index];
                allocateEDMAShadowChannel(&param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.ShadowPramId[index]   = param;
            }
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPing.eventQueue= 0;

            for(index = 0; index< 2; index++)
            {
                dmaCh = pingChan[index];
                tcc   = pingChan[index];
                param = pingChan[index];
                DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].channel = dmaCh;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].paramId = param;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].tcc     = tcc;

                param = pingChanShadow[index];
                allocateEDMAShadowChannel(&param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].shadowPramId = param;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPingData[index].eventQueue = 0;
            }

            /* PONG */
            dmaCh = MMW_EDMA_1D_PONG_CH_ID;
            tcc   = MMW_EDMA_1D_PONG_CH_ID;
            param = MMW_EDMA_1D_PONG_CH_ID;
            DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.channel         = dmaCh;
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.paramId         = param;
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.tcc             = tcc;
            for(index = 0; index< 2; index++)
            {
                param = pongDummyChanShadow[index];
                allocateEDMAShadowChannel(&param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.ShadowPramId[index]   = param;
            }
            pHwConfig->edmaOutCfg.u.fmt2.dataOutPong.eventQueue       = 0;

            for(index = 0; index< 2; index++)
            {
                dmaCh = pongChan[index];
                tcc   = pongChan[index];
                param = pongChan[index];
                DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].channel = dmaCh;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].paramId = param;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].tcc     = tcc;

                param = pongChanShadow[index];
                allocateEDMAShadowChannel(&param);
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].shadowPramId = param;
                pHwConfig->edmaOutCfg.u.fmt2.dataOutPongData[index].eventQueue = 0;
            }
        }
        else
        {
            /* Output Ping*/
            dmaCh = MMW_EDMA_1D_PING_CH_ID;
            tcc   = MMW_EDMA_1D_PING_CH_ID;
            param = MMW_EDMA_1D_PING_CH_ID;
            DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.channel         = dmaCh;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.paramId         = param;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.tcc             = tcc;

            param = MMW_EDMA_1D_PING_SHADOW_LINK_CH_ID;
            allocateEDMAShadowChannel(&param);
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.shadowPramId   = param;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPing.eventQueue= 0;

            /* Output Pong*/
            dmaCh = MMW_EDMA_1D_PONG_CH_ID;
            tcc   = MMW_EDMA_1D_PONG_CH_ID;
            param = MMW_EDMA_1D_PONG_CH_ID;
            DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.channel         = dmaCh;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.paramId         = param;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.tcc             = tcc;

            param = MMW_EDMA_1D_PONG_SHADOW_LINK_CH_ID;
            allocateEDMAShadowChannel(&param);
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.shadowPramId   = param;
            pHwConfig->edmaOutCfg.u.fmt1.dataOutPong.eventQueue       = 0;

        }
        /* Output signature channel */
        dmaCh = MMW_EDMA_1D_PING_CHAIN_CH_ID;
        tcc   = MMW_EDMA_1D_PING_CHAIN_CH_ID;
        param = MMW_EDMA_1D_PING_CHAIN_CH_ID;
        DPEDMA_allocateEDMAChannel(gEdmaHandle[0], &dmaCh, &tcc, &param);
        pHwConfig->edmaOutCfg.dataOutSignature.channel  = dmaCh;
        pHwConfig->edmaOutCfg.dataOutSignature.paramId  = param;
        pHwConfig->edmaOutCfg.dataOutSignature.tcc      = tcc;

        param = MMW_EDMA_1D_PING_ONE_HOT_SHADOW_LINK_CH_ID;
        allocateEDMAShadowChannel(&param);
        pHwConfig->edmaOutCfg.dataOutSignature.shadowPramId = param;
        pHwConfig->edmaOutCfg.dataOutSignature.eventQueue = 0;
    }

    /* radar cube*/
    /* only 1 tx is transmitted at one chirp */
    pHwConfig->radarCube.dataSize = params->numRangeBins * testConfig->numRxAntennas * sizeof(uint32_t) * testConfig->numChirpsPerFrame;


    if (testConfig->radarCubeLayoutFmt == 0)
    {
        pHwConfig->radarCube.datafmt = DPIF_RADARCUBE_FORMAT_2;
    }
    else
    {
        pHwConfig->radarCube.datafmt = DPIF_RADARCUBE_FORMAT_1;
    }

    if (gCalibDcConfig.enabled == 1)     //set the dc calibration buffer size
    {
        params->resetDcRangeSigMeanBuffer = 1;   //reset the buffer, first call
    }
}

/**
*  @b Description
*  @n
*    For each test, check the radar cube data with the references
*/
void Test_printResults(rangeProcTestConfig_t * testConfig, uint8_t flag) //, char* feature)
{
    uint32_t testStatus;
    uint32_t jj, kk, ii;
    uint32_t chirpIdx, chirpIdxRef;
    uint32_t radarCubeIdx;
    uint32_t * ptrCubeRef;
    int16_t realValueRef, imagValueRef;
    uint32_t txAntIdx;
    int32_t sumReal, sumImag;
    int16_t tempReal, tempImag;
    uint32_t tempValue;


    testStatus = 1;

    /* compare the output */
    for (chirpIdx = 0; chirpIdx < testConfig->numChirpsPerFrame; chirpIdx++)
    {
        /* repeat the same input data per frame*/
        chirpIdxRef = chirpIdx - (chirpIdx / testConfig->numChirpsPerFrameRef * testConfig->numChirpsPerFrameRef);

        for (jj = 0; jj < testConfig->numRxAntennas; jj++)
        {
            ptrCubeRef = &fft1DOut_ref[chirpIdxRef * testConfig->numRangeBins * MAX_NUM_RX_ANTENNA + jj * testConfig->numRangeBins];

            for (kk = 0; kk < testConfig->dpuNumRangeBins; kk++)
            {

                if (testConfig->radarCubeLayoutFmt==0)  //rx, tx, chirp, range
                {
                    //if (testConfig->numTxAntennas == 1)
                    radarCubeIdx = kk * testConfig->numRxAntennas *
                        testConfig->numChirpsPerFrame + chirpIdx * testConfig->numRxAntennas + jj;

                }
                else if (testConfig->radarCubeLayoutFmt==1)
                {
                    if (testConfig->numTxAntennas == 1)
                        radarCubeIdx = chirpIdx * testConfig->dpuNumRangeBins *
                           testConfig->numRxAntennas + jj * testConfig->dpuNumRangeBins + kk;
                    else //if (testConfig->numTxAntennas == 2)
                    {
                        //txAntIdx = chirpIdx & 0x1;  // chirpIdx mod(2)=0, tx = 0, mod(2)=1, tx = 1
                        txAntIdx = chirpIdx - chirpIdx/testConfig->numTxAntennas * testConfig->numTxAntennas; //mod(numTxAnt)
                        radarCubeIdx = txAntIdx * (testConfig->numChirpsPerFrame/testConfig->numTxAntennas ) *
                                       testConfig->numRxAntennas * testConfig->dpuNumRangeBins;
                        radarCubeIdx +=  (chirpIdx/testConfig->numTxAntennas) * testConfig->numRxAntennas *
                                         testConfig->dpuNumRangeBins + jj * testConfig->dpuNumRangeBins + kk;
                    }
                }

               // test_print("%d,%d\n",radarCube[radarCubeIdx].real, radarCube[radarCubeIdx].imag );
                realValueRef = (int16_t)(ptrCubeRef[kk] & 0x0000FFFF);   //fixed the reference output as Imag16/Real16
                imagValueRef = (int16_t)(ptrCubeRef[kk] >> 16);

                if (flag == 1)
                {
                    bool negBinIdxCondition;
                    if(testConfig->isReal){
                        negBinIdxCondition = (kk>= (testConfig->numRangeBins/2 + gCalibDcConfig.negativeBinIdx)) && (kk <= testConfig->numRangeBins/2 );
                    }
                    else{
                        negBinIdxCondition = (kk>= (testConfig->numRangeBins + gCalibDcConfig.negativeBinIdx));
                    }
                    if ((kk <= gCalibDcConfig.positiveBinIdx) || negBinIdxCondition)
                    {
                        //remove the mean from previous frame
                        sumReal = 0;
                        sumImag = 0;
                        for (ii = 0; ii < testConfig->numChirpsPerFrameRef / testConfig->numTxAntennas; ii++)
                        {

                            tempValue = fft1DOut_ref[(ii * testConfig->numTxAntennas +
                                (chirpIdxRef - chirpIdxRef / testConfig->numTxAntennas * testConfig->numTxAntennas)) *
                                testConfig->numRangeBins * MAX_NUM_RX_ANTENNA + jj * (testConfig->numRangeBins) + kk];
                            tempReal = (int16_t)(tempValue & 0x0000FFFF);
                            tempImag = (int16_t)(tempValue >> 16);
                            sumReal += tempReal;
                            sumImag += tempImag;
                        }

                        realValueRef -= (sumReal / testConfig->numChirpsPerFrameRef * testConfig->numTxAntennas);
                        imagValueRef -= (sumImag / testConfig->numChirpsPerFrameRef * testConfig->numTxAntennas);
                    }

                }
                if (((gRadarCube[radarCubeIdx].real - realValueRef) > MAX_ALLOWED_ERROR) ||
                    ((gRadarCube[radarCubeIdx].real - realValueRef) < -MAX_ALLOWED_ERROR))
                {
                    testStatus = 0;
                    testFeatureStatus = 0;
                    finalResults = 0;
                    // test_print("radar cube index %d\n", radarCubeIdx);
                    test_print(" test fail at chirp %d, rx antenna %d, bin %d, cube=%d, ref=%d\n ",
                                  chirpIdx, jj, kk, gRadarCube[radarCubeIdx].real, realValueRef );
                }
                if (((gRadarCube[radarCubeIdx].imag - imagValueRef) > MAX_ALLOWED_ERROR) ||
                    ((gRadarCube[radarCubeIdx].imag - imagValueRef) < -MAX_ALLOWED_ERROR))
                {
                    testStatus = 0;
                    testFeatureStatus = 0;
                    finalResults = 0;
                    test_print(" test fail at chirp %d, rx antenna %d, bin %d, cube=%d, ref=%d\n ",
                                  chirpIdx, jj, kk,gRadarCube[radarCubeIdx].imag,imagValueRef);
                }
            }
        }
    }

    if(testStatus == 1)
    {
        test_print("Test PASSED!\n");
    }
    else
    {
        test_print("Test FAILED!\n");
    }


}
/**
*  @b Description
*  @n
*    Generate the windowing coefficients, only support BLACKMAN
*/
void Test_1DwindowCoef_gen(uint32_t numAdcSamples)
{
    float ephyR, ephyI;
    float phi = 2 * PI_ / ((float) numAdcSamples - 1);


    uint32_t ii;
    float ephyR2, ephyI2;
    float a0, a1, a2;
    float winValue;
    int32_t winValueFixed;
    float cosValue, sinValue, cosValue2, sinValue2;
    float temp;
    float initR, initI;

    a0 = 0.42;
    a1 = 0.5;
    a2 = 0.08;

    cosValue = 1.f;
    sinValue = 0.f;
    cosValue2 = 1.f;
    sinValue2 = 0.f;

    initR  = cos(phi);
    initI  = sin(phi);
    ephyR = initR;
    ephyI = initI;
    ephyR2 = 1.f - (initI * initI) * 2.f; /* cos(2a)=1-2*sin(a)^2 */
    ephyI2 = 2.f * initR * initI; /* sin(2a)=2*sin(a)*cos(a) */

    /* window is whole length for hwa*/
    for (ii = 0; ii < numAdcSamples; ii++)
    {
        winValue = a0 - a1 * cosValue + a2 * cosValue2 ;
        winValue = winValue * (1<<17) + 0.5;
        winValueFixed = (uint32_t) winValue;
        if (winValueFixed >= (1<<17))
        {
            winValueFixed = (1<<17) - 1;
        }
        temp = cosValue;
        cosValue = cosValue * ephyR - sinValue * ephyI;
        sinValue = temp * ephyI + sinValue * ephyR;

        temp = cosValue2;
        cosValue2 = cosValue2 * ephyR2 - sinValue2 * ephyI2;
        sinValue2 = temp * ephyI2 + sinValue2 * ephyR2;
        //window1DCoef[ii] = (1<<17) - 1; //rec window, for debug
        window1DCoef[ii] = winValueFixed;
    }
}

/**
*  @b Description
*  @n
*    FFT calculation, with output fixed to 32 bits, with Imag16/Real16
*/
void Test_fft_ref(uint32_t * dataOut, uint32_t * dataIn, uint32_t fftSize, uint32_t numAdcSamples, bool isReal)
{
    uint32_t ii,jj;
    float cosValue, sinValue;
    float cosStep, sinStep;
    float alpha;
    float realValueTemp, imagValueTemp;
    float realValue, imagValue;
    float inReal, inImag;
    int16_t outReal, outImag;
    float tempValue;
    float divValue;

    /* hard code all the scale, math with hwa fft results  */
    if (fftSize == 64)
       divValue = 4.f;
    else if (fftSize==128)
        divValue = 4.f;
    else if (fftSize == 256)
        divValue = 4.f;  //scale with hwa fft
    else if (fftSize == 512)
        divValue = 4.f;
    else if (fftSize == 1024)
        divValue = 4.f;
    else if (fftSize == 2048)
        divValue = 4.f;
    else
        divValue = 4.f;
    //in 32 bits input, Real16/imag16
    for (ii = 0; ii < fftSize; ii++)
    {
        alpha = -2.f * ii * PI_ /(float) fftSize;
        cosValue = 1.f;
        sinValue = 0.f;
        cosStep = cos(alpha);
        sinStep = sin(alpha);

        if(isReal){
            realValue = (float)  ((int16_t)(dataIn[0] &0xFFFF));
            imagValue = (float)  ((int16_t)(dataIn[0] >> 16));
        }
        else{
            realValue = (float)  ((int16_t)(dataIn[0] >> 16));
            imagValue = (float)  ((int16_t)(dataIn[0] &0xFFFF));
        }

        for (jj = 1; jj < numAdcSamples; jj++)
        {
            tempValue = cosValue;
            cosValue = cosValue * cosStep - sinValue * sinStep;
            sinValue = sinValue * cosStep + tempValue *sinStep;

            if(isReal){
                inReal = (float)  ((int16_t)(dataIn[jj] &0xFFFF));
                inImag = (float)  ((int16_t)(dataIn[jj] >> 16));
            }
            else{
                inReal = (float)  ((int16_t)(dataIn[jj] >> 16));
                inImag = (float)  ((int16_t)(dataIn[jj] &0xFFFF));
            }

            realValueTemp = inReal * cosValue - inImag * sinValue;
            realValue += realValueTemp;
            imagValueTemp = inReal * sinValue + inImag * cosValue;
            imagValue += imagValueTemp;
        }
        /*match with hwa hardware */
        realValue = realValue /divValue;
        if (realValue > 0)
            outReal = (int16_t) (realValue + 0.5);
        else
            outReal = (int16_t) (realValue - 0.5);
        imagValue = imagValue /divValue;
        if (imagValue > 0)
        {
            outImag = (int16_t) (imagValue + 0.5);
        }
        else
            outImag = (int16_t) (imagValue - 0.5);

        dataOut[ii] = outReal;
        dataOut[ii] += (outImag << 16);
    }
}

/**
*  @b Description
*  @n
*    prepare the adc samples with interleave format
*/
void Test_channelInterleave(uint32_t * dataArray, uint32_t numRxAnt, uint32_t numAdcSamples, bool isReal)
{

    uint32_t antIdx;
    uint32_t leftIdx, rightIdx, index, rightIdx1[3]; //maximum is 4 antenna
    uint32_t temp;
    uint16_t temp16;

    leftIdx = 1;
    rightIdx1[0] = 0;
    rightIdx1[1] = 0;
    rightIdx1[2] = 0;
    while (leftIdx < rightIdx1[0] + numAdcSamples)
    {
        for (antIdx = 1; antIdx < numRxAnt; antIdx++)
        {
            rightIdx = antIdx * numAdcSamples + rightIdx1[antIdx - 1]  ;
            for ( index = rightIdx; index > leftIdx; index--)
            {
                if(isReal){
                    temp16 = *(((uint16_t* ) dataArray) + index);
                    *(((uint16_t* ) dataArray) + index) = *(((uint16_t* ) dataArray) +index - 1);
                    *(((uint16_t* ) dataArray) +index - 1) = temp16;
                }
                else{
                    temp = *(((uint32_t* ) dataArray) + index);
                    *(((uint32_t* ) dataArray) + index) = *(((uint32_t* ) dataArray) +index - 1);
                    *(((uint32_t* ) dataArray) +index - 1) = temp;
                }

            }

            leftIdx++;
        }
        leftIdx++; /* skip antenna 0 */
        for (antIdx = 1; antIdx < numRxAnt; antIdx++)
        {
            rightIdx1[antIdx - 1] += (numRxAnt - antIdx);
        }
    }

}

void Test_1Dfft_refGen( uint32_t numAdcSamples, uint32_t numRxAnt, uint32_t fftSize, uint32_t * dataInPtr, uint32_t * fftOutPtr, uint8_t symWin, bool isReal)
{
    uint32_t kk, jj;
    int32_t winCoef;

    uint32_t dataSampleCplx;
    uint16_t dataSampleReal;
    int16_t realValue, imagValue;
    int32_t tempValueReal, tempValueImag;
    uint32_t * ptrFftRef;

    ptrFftRef = fftOutPtr;
    for (jj = 0; jj < numRxAnt; jj++)
    {
        if(symWin == 1)
        {
            /* windowing */
            for (kk = 0; kk < numAdcSamples/2; kk++)
            {
                winCoef = (int32_t) window1DCoef[kk];

                if(isReal){
                    dataSampleReal = *(((int16_t *)dataInPtr) + kk);
                    realValue = dataSampleReal;
                    imagValue = (int16_t) 0;
                }
                else{
                    dataSampleCplx = *(((uint32_t *)dataInPtr) + kk);
                    realValue =  (int16_t) (dataSampleCplx&0x0000FFFF);
                    imagValue = (int16_t) (dataSampleCplx>>16);
                }

                tempValueReal = (realValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
                if (tempValueReal >= (1<<15))
                    dataWinOut[kk] = 0x7fff;
                else
                    dataWinOut[kk] = (uint16_t) tempValueReal;

                tempValueImag = (imagValue * winCoef + 0x10000) >> 17;
                if (tempValueImag >= (1<<15))
                    dataWinOut[kk] += ( 0x7fff <<16);
                else
                    dataWinOut[kk] +=  ( ((uint16_t)tempValueImag) << 16);

            }
            for (kk = numAdcSamples/2; kk < numAdcSamples; kk++)
            {
                winCoef = (int32_t) window1DCoef[numAdcSamples - 1 - kk ];

                if(isReal){
                    dataSampleReal = *(((int16_t *)dataInPtr) + kk);
                    realValue = dataSampleReal;
                    imagValue = (int16_t) 0;
                }
                else{
                    dataSampleCplx = *(((uint32_t *)dataInPtr) + kk);
                    realValue =  (int16_t) (dataSampleCplx&0x0000FFFF);
                    imagValue = (int16_t) (dataSampleCplx>>16);
                }

                tempValueReal = (realValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
                if (tempValueReal >= (1<<15))
                    dataWinOut[kk] = 0x7fff;
                else
                    dataWinOut[kk] = (uint16_t) tempValueReal;

                tempValueImag = (imagValue * winCoef + 0x10000) >> 17;
                if (tempValueImag >= (1<<15))
                     dataWinOut[kk] += ( 0x7fff <<16);
                else
                     dataWinOut[kk] +=  ( ((uint16_t)tempValueImag) << 16);
            }

        }
        else
        {
            /* windowing */
            for (kk = 0; kk < numAdcSamples; kk++)
            {
                winCoef = (int32_t) window1DCoef[kk];

                if(isReal){
                    dataSampleReal = *(((int16_t *)dataInPtr) + kk);
                    realValue = dataSampleReal;
                    imagValue = (int16_t) 0;
                }
                else{
                    dataSampleCplx = *(((uint32_t *)dataInPtr) + kk);
                    realValue =  (int16_t) (dataSampleCplx&0x0000FFFF);
                    imagValue = (int16_t) (dataSampleCplx>>16);
                }

                tempValueReal = (realValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
                if (tempValueReal >= (1<<15))
                    dataWinOut[kk] = 0x7fff;
                else
                    dataWinOut[kk] = (uint16_t) tempValueReal;

                tempValueImag = (imagValue * winCoef + 0x10000) >> 17;
                if (tempValueImag >= (1<<15))
                     dataWinOut[kk] += ( 0x7fff <<16);
                else
                     dataWinOut[kk] +=  ( ((uint16_t)tempValueImag) << 16);

            }
        }

        //must be fft size, if adc samples < fft size,
        if(isReal){
            dataInPtr += fftSize / 2;   /* 16 bit jump only */
        }
        else{
            dataInPtr += fftSize ;
        }
        /* append the zero */
        for ( kk = numAdcSamples; kk < fftSize; kk++)
        {
            dataWinOut[kk] = 0;
        }

        /* fft */
        Test_fft_ref(ptrFftRef, dataWinOut, fftSize, numAdcSamples, isReal);
        ptrFftRef += fftSize;

    }
}


void rangeProcDpuTest_hwaInit()
{
    int32_t             errorCode = 0;

    hwaHandle = HWA_open(0, NULL, &errorCode);
    if (hwaHandle == NULL)
    {
        test_print("Error: Unable to open the HWA Instance err:%d\n",errorCode);
        DebugP_assert (0);
        return;
    }
}

void rangeProcDpuTest_dpuInit()
{
    int32_t  errorCode = 0;
    DPU_RangeProcHWA_InitParams initParams;
    initParams.hwaHandle =  hwaHandle;
    /* generate the dpu handler*/
    rangeProcDpuHandle =  DPU_RangeProcHWA_init (&initParams, subframeCount, &errorCode);
    if (rangeProcDpuHandle == NULL)
    {
        test_print ("Debug: RangeProc DPU initialization returned error %d\n", errorCode);
        DebugP_assert (0);
        return;
    }
}

/**
*  @b Description
*  @n
*        set up the range processing DPU configurations, do not change for each test
*/
void rangeProcDpuTest_dpuCfg()
{
    DPU_RangeProcHWA_HW_Resources * pHwConfig;

    memset((void *)&rangeProcDpuCfg, 0, sizeof(DPU_RangeProcHWA_Config));


    /* hwi configuration */
    pHwConfig = &rangeProcDpuCfg.hwRes;

    /* HWA configurations, not related to per test, common to all test */
    pHwConfig->hwaCfg.paramSetStartIdx = 5;
    pHwConfig->hwaCfg.numParamSet = DPU_RANGEPROCHWA_NUM_HWA_PARAM_SETS;
    pHwConfig->hwaCfg.hwaWinRamOffset  = MMW_HWA_WINDOWRAM_1D_OFFSET;
    pHwConfig->hwaCfg.hwaWinSym = 1;
    pHwConfig->hwaCfg.dataInputMode = DPU_RangeProcHWA_InputMode_ISOLATED;

    /* edma configuration */
    pHwConfig->edmaHandle  = gEdmaHandle[CONFIG_EDMA0];

    /* windowing buffer is fixed, size will change*/
    rangeProcDpuCfg.staticCfg.window =  (int32_t *)&window1DCoef[0];

    if(gIsChirpDataReal){
        rangeProcDpuCfg.staticCfg.isChirpDataReal = 1;
    }
    else{
    rangeProcDpuCfg.staticCfg.isChirpDataReal = 0;
    }

    /* adc buffer buffer, format fixed, interleave, size will change */
    if(rangeProcDpuCfg.staticCfg.isChirpDataReal){
        rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_REAL16;
    }
    else{
        rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_COMPLEX16_IMRE;
    }

    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.adcBits = 2U;
    rangeProcDpuCfg.staticCfg.ADCBufData.dataProperty.numChirpsPerChirpEvent = 1U;
    rangeProcDpuCfg.staticCfg.ADCBufData.data = (void *) adcDataIn;

    /* radar cube */
    rangeProcDpuCfg.hwRes.radarCube.data  = (cmplx16ImRe_t *) &gRadarCube[0];

    /* FFT1D calibration buffers */
    rangeProcDpuCfg.hwRes.dcRangeSigMean  = (cmplx32ImRe_t *) &gDcRangeSigMean[0];
    rangeProcDpuCfg.dynCfg.calibDcRangeSigCfg = &gCalibDcConfig;

    /* complex is 32bitsI/32bitsQ, maximum 32 bits */
    pHwConfig->dcRangeSigMeanSize = MAX_NUM_TX_ANTENNA * MAX_NUM_RX_ANTENNA * 32 * 8;
}

/**
*  @b Description
*  @n
*        Testing Task
*/
void rangeProcDpuTest_Task(void* args)
{
    uint32_t i,j;
    uint32_t numDataReadIn;
    uint32_t numTests;
    uint32_t testIndx, testCount;
    uint32_t numBytesPerInputSample;
    char featureName[200];
    char testConfigInfo[200];
    uint32_t testLoop; //outputfmt and interleave loop
    rangeProcTestConfig_t testConfig;
    int32_t  retVal = 0;
    DPU_RangeProcHWA_OutParams outParms;
    int16_t rxIdx, rxIdxStart;
    uint16_t numRX[3]={1,2,4};
    uint16_t adcSampleLoop;
    uint16_t numAdcSampleLoop;
    uint16_t chirpIdxRef;
    uint8_t numDataType, testLoopDataType;
    uint32_t baseAddr, regionId;
    int32_t  testStatus = SystemP_SUCCESS;
    uint32_t index = 0;
    uint64_t taskStartTime, taskEndTime;
    Drivers_open();
    Board_driversOpen();

    baseAddr = EDMA_getBaseAddr(gEdmaHandle[CONFIG_EDMA0]);
    DebugP_assert(baseAddr != 0);

    regionId = EDMA_getRegionId(gEdmaHandle[CONFIG_EDMA0]);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    /* Initialize test logger */
    MCPI_Initialize();

    numDataType = 1; /* Real Only */

    finalResults = 1;
    taskStartTime=ClockP_getTimeUsec();
    test_print("...... Initialization ...... \n");

    /* hwa, edma, and rangeprocDPU initialization*/
    rangeProcDpuTest_hwaInit();
    rangeProcDpuTest_dpuInit();

    for (testLoopDataType = 0; testLoopDataType < numDataType; testLoopDataType++){
        gIsChirpDataReal = !(gIsChirpDataReal);

        if(gIsChirpDataReal){
            testConfig.isReal = 1;
        }
        else{
            testConfig.isReal = 0;
        }

        /* config the range proc DPU*/
        rangeProcDpuTest_dpuCfg();


    /* Data in files are different for 29xx/am273x */
        if(gIsChirpDataReal){
            /* start the test */
            #if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
            fileId = fopen("testdata\\rangeprochwa_test_data_2944_real.bin", "rb");
            if (fileId == NULL)
            {
                printf("Error:  Cannot open rangeprochwa_test_data_2944_real.bin !\n");
                exit(0);
            }
            test_print("Platform = AWR2944, Chirp Data = Real\n\n");
            #else
            fileId = fopen("testdata\\rangeprochwa_test_data_2943_real.bin", "rb");
            if (fileId == NULL)
            {
                printf("Error:  Cannot open rangeprochwa_test_data_2943_real.bin !\n");
                exit(0);
            }
            test_print("Platform = AWR2943, Chirp Data = Real\n\n");
            #endif
        }
        else{
            printf("Complex chirp data is not supported for AWR294X !\n");
            exit(0);
        }

        /* read in number of tests to be run */
        fread(&numDataReadIn, sizeof(uint32_t),1,fileId);
        /* 1Tx, insert 3 test between fft size,mod(4)=1,2,3,  (1,2,4) rx */
        numTests = ((numDataReadIn / MAX_NUM_TX_ANTENNA) + (((numDataReadIn / MAX_NUM_TX_ANTENNA) - 1) * 3)) * 3;
        /* 2Tx, insert 3 test between fft size, mod(4)=1,2,3, (2,4) rx */
        numTests += ((numDataReadIn / MAX_NUM_TX_ANTENNA) + (((numDataReadIn / MAX_NUM_TX_ANTENNA) - 1) * 3)) * 2;
        /* 3Tx, insert 3 test between fft size, mod(4)=1,2,3, 4 rx*/
        numTests += ((numDataReadIn / MAX_NUM_TX_ANTENNA) + (((numDataReadIn / MAX_NUM_TX_ANTENNA) - 1) * 3)) * 1;
        if(MAX_NUM_TX_ANTENNA == 4){
            /* 4Tx, insert 3 test between fft size, mod(4)=1,2,3, 4 rx*/
            numTests += ((numDataReadIn / MAX_NUM_TX_ANTENNA) + (((numDataReadIn / MAX_NUM_TX_ANTENNA) - 1) * 3)) * 1;
        }

        numTests = numTests * 3; // noninterleave, fmt0,1, interleave fmt 0
        if(gIsChirpDataReal){
            printf("\n...... %d Tests to be run for dataType Real.......\n\n", numTests);
        }
        else{
            printf("\n...... %d Tests to be run for dataType Complex.......\n\n", numTests);
        }

        testCount = 0;
        /* process each test */
        for (testIndx = 0; testIndx < numDataReadIn;  testIndx++)
        {

            /* read in test config */
            fread(&testConfig.numTxAntennas, sizeof(uint32_t),1,fileId);
            fread(&testConfig.numRangeBins, sizeof(uint32_t),1,fileId);
            fread(&testConfig.numChirpsPerFrameRef, sizeof(uint32_t),1,fileId);

            testConfig.cmplxIQswapFlag = 0; /* for arm, not support IQ swap */
            testConfig.numChirpsPerFrame = testConfig.numChirpsPerFrameRef * NUM_REPEAT_PERFRAME;

            test_print("numTxAntennas = %d\r", testConfig.numTxAntennas);
            test_print("numRangeBins = %d\r", testConfig.numRangeBins);
            test_print("numChirpsPerFrame = %d\n", testConfig.numChirpsPerFrame);
            test_print("\n");

            if(testConfig.isReal){
                numBytesPerInputSample = sizeof(int16_t);
            }
            else{
                numBytesPerInputSample = sizeof(uint32_t); /* Complex (16 bit imag, 16 bit real) */
            }

            /* read in all the adc samples MAX_NUM_RX_ANTENNA * testConfig.numRangeBins*testConfig.numChirpsPerFrame per test*/
            for(i=0; i<testConfig.numChirpsPerFrameRef; i++){
                if(numBytesPerInputSample == 2){
                    fread( (((uint16_t *)dataInBuffer) + i * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins),
                    sizeof( uint16_t ), MAX_NUM_RX_ANTENNA *  testConfig.numRangeBins, fileId );
                }
                else{
                    fread( (uint32_t *)&dataInBuffer[i * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins],
                        sizeof( uint32_t ), MAX_NUM_RX_ANTENNA *  testConfig.numRangeBins, fileId );

                }
            }

            if (testConfig.numRangeBins == 64)
                numAdcSampleLoop = 1;  //no insert
            else
                numAdcSampleLoop = 4; //insert 3 + fft size

            test_print("\nTest #%d  start\r\n", testCount);
            for (adcSampleLoop = 0; adcSampleLoop<numAdcSampleLoop; adcSampleLoop++)
            {
                if (numAdcSampleLoop == 1)
                    testConfig.numAdcSamples = testConfig.numRangeBins;
                else
                {   /* mod(4)=1,2,3, numRangeBins*/
                    testConfig.numAdcSamples = testConfig.numRangeBins -
                        (numAdcSampleLoop - 1 - adcSampleLoop) * 4 + (numAdcSampleLoop - 1 - adcSampleLoop);
                }
                /* generate the 1D window coefficients */
                Test_1DwindowCoef_gen(testConfig.numAdcSamples);
                /* generate the reference for all rx antenna  */
                for(i=0; i<testConfig.numChirpsPerFrameRef; i++)
                {
                    if(numBytesPerInputSample == 2){
                        Test_1Dfft_refGen(testConfig.numAdcSamples, MAX_NUM_RX_ANTENNA,
                                    testConfig.numRangeBins, &dataInBuffer[(i * MAX_NUM_RX_ANTENNA *  testConfig.numRangeBins)/2],
                                    &fft1DOut_ref[i * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins], 1, testConfig.isReal);
                    }
                    else{
                        Test_1Dfft_refGen(testConfig.numAdcSamples, MAX_NUM_RX_ANTENNA,
                                    testConfig.numRangeBins, &dataInBuffer[(i * MAX_NUM_RX_ANTENNA *  testConfig.numRangeBins)],
                                    &fft1DOut_ref[i * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins], 1, testConfig.isReal);
                    }
                }

                /* For 3, 4 Tx, numRx = 4 only */
                if(testConfig.numTxAntennas == 4){
                    rxIdxStart = -1;
                }
                else{
                    rxIdxStart = 0;
                }

                for (rxIdx = rxIdxStart; rxIdx < (int16_t)(4 - testConfig.numTxAntennas); rxIdx++)
                {
                    testConfig.numRxAntennas = numRX[(uint32_t)((int16_t)(testConfig.numTxAntennas - 1) + rxIdx)];

                    testFeatureStatus = 1;
                    sprintf(featureName, "%s%d%s%d%s%d%s%d",
                        ":Tx", testConfig.numTxAntennas,
                        " Rx", testConfig.numRxAntennas,
                        " R", testConfig.numRangeBins,
                        " D", testConfig.numChirpsPerFrame);

                    for (testLoop = 0; testLoop < 3; testLoop++) //for output layout, and interleave,noninterleave loop
                    {

                        testConfig.rxChanInterleave = testLoop >> 1;
                        testConfig.radarCubeLayoutFmt = testLoop&0x1;

                        /* set up the dpu configuration per test */
                        //add the dc calibartion test here
                        gCalibDcConfig.enabled = 1;
                        gCalibDcConfig.negativeBinIdx = -15; //-15;
                        gCalibDcConfig.positiveBinIdx = 16; //16;          //maximum is 32
                        gCalibDcConfig.numAvgChirps = testConfig.numChirpsPerFrame/testConfig.numTxAntennas;

                        test_print("\nTest #%d  start\r\n", testCount);

                        sprintf(testConfigInfo, "%s%d%s%d%s%d%s%d%s%d%s%d%s%d",
                            ":Tx", testConfig.numTxAntennas,
                            " Rx", testConfig.numRxAntennas,
                            " ADC", testConfig.numAdcSamples,
                            " Chps", testConfig.numChirpsPerFrame,
                        // " IQSwap", testConfig.cmplxIQswapFlag,
                            " IntLv", testConfig.rxChanInterleave,
                            " OutFmt", testConfig.radarCubeLayoutFmt,
                            " Real", testConfig.isReal);

                        test_print("Configuration : %s\n", testConfigInfo);

                        Test_setProfile(&testConfig);

                        checkResultFlag = 1;

                        retVal = DPU_RangeProcHWA_config (rangeProcDpuHandle,
                                                        &rangeProcDpuCfg);

                        if(retVal < 0)
                        {
                            if ((retVal == DPU_RANGEPROCHWA_ENOTIMPL) &&(rangeProcDpuCfg.staticCfg.numRangeBins == 1024 && (!testConfig.isReal)))
                            {
                                /* use the work around */
                                rangeProcDpuCfg.staticCfg.numRangeBins = 1022;
                                rangeProcDpuCfg.hwRes.radarCube.dataSize = rangeProcDpuCfg.staticCfg.numRangeBins * testConfig.numRxAntennas * sizeof(uint32_t) * testConfig.numChirpsPerFrame;
                                gCalibDcConfig.enabled = 0; //and disable the calibration
                                retVal = DPU_RangeProcHWA_config(rangeProcDpuHandle,
                                                                &rangeProcDpuCfg);
                                if (retVal < 0)
                                {
                                    test_print("DEBUG: RANGE DPU config return error:%d \n", retVal);
                                    DebugP_assert(0);
                                }
                            }
                            else if ((retVal == DPU_RANGEPROCHWA_ENOTIMPL) &&(rangeProcDpuCfg.staticCfg.numRangeBins == 2048 && (testConfig.isReal)))
                            {
                                /* use the work around */
                                rangeProcDpuCfg.staticCfg.numRangeBins = 2044;
                                rangeProcDpuCfg.hwRes.radarCube.dataSize = rangeProcDpuCfg.staticCfg.numRangeBins * testConfig.numRxAntennas * sizeof(uint32_t) * testConfig.numChirpsPerFrame;
                                gCalibDcConfig.enabled = 0; //and disable the calibration
                                retVal = DPU_RangeProcHWA_config(rangeProcDpuHandle,
                                                                &rangeProcDpuCfg);
                                if (retVal < 0)
                                {
                                    test_print("DEBUG: RANGE DPU config return error:%d \n", retVal);
                                    DebugP_assert(0);
                                }
                            }
                            else
                            {
                                test_print("DEBUG: RANGE DPU config return error:%d \n", retVal);
                                DebugP_assert(0);
                            }
                        }

                        /* control the rangeproc hwa*/
                    retVal = DPU_RangeProcHWA_control(rangeProcDpuHandle, DPU_RangeProcHWA_Cmd_triggerProc, NULL, 0);
                    if(retVal < 0)
                    {
                        /* Not Expected */
                        test_print("RangeProc DPU control error %d\n", retVal);
                        DebugP_assert(0);
                    }

                        /* process chirps loop in the frame*/
                        for(i=0; i< testConfig.numChirpsPerFrame ; i++)
                        {
                            /* i % testConfig.numChirpsPerFrameRef*/
                            chirpIdxRef = i - (i / testConfig.numChirpsPerFrameRef * testConfig.numChirpsPerFrameRef);

                            /* read in one chirp data */
                            for (j = 0; j < testConfig.numRxAntennas; j++)
                            {
                                    if(numBytesPerInputSample == 2){
                                        //16 byte aligned, 8 uint32 symbols
                                        // memcpy((void *)&adcDataIn[j * ((testConfig.numAdcSamples + 7) / 8 * 8)],
                                        memcpy((void *)&adcDataIn[(j * ((testConfig.numAdcSamples + 7) / 8 * 8))/2],
                                            (void *)&dataInBuffer[(chirpIdxRef * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins + j *testConfig.numRangeBins)/2], numBytesPerInputSample*testConfig.numAdcSamples);
                                    }
                                    else{
                                        //16 byte aligned, 4 uint32 symbols
                                        memcpy((void *)&adcDataIn[j * ((testConfig.numAdcSamples + 3) / 4 * 4)],
                                            (void *)&dataInBuffer[chirpIdxRef * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins + j *testConfig.numRangeBins], numBytesPerInputSample*testConfig.numAdcSamples);
                                    }
                            }

                            if (testConfig.rxChanInterleave)
                            {
                                /* end of rx antenna, data is aligned at 4 samples */
                                    if(numBytesPerInputSample == 2){
                                        Test_channelInterleave(adcDataIn, testConfig.numRxAntennas, ((testConfig.numAdcSamples + 7) / 8 * 8), testConfig.isReal);
                                    }
                                    else{
                                        Test_channelInterleave(adcDataIn, testConfig.numRxAntennas, (testConfig.numAdcSamples + 3) / 4 * 4, testConfig.isReal);
                                    }
                            }
#ifdef SUBSYS_MSS
                            if(!gIsChirpDataReal){
                                    CacheP_wb(adcDataIn, (numBytesPerInputSample * testConfig.numRxAntennas * ((testConfig.numAdcSamples+3)/4*4)), CacheP_TYPE_ALLD);
                                }
                                else{
                                    CacheP_wb(adcDataIn, (numBytesPerInputSample * testConfig.numRxAntennas * ((testConfig.numAdcSamples+7)/8*8)), CacheP_TYPE_ALLD);
                                }
#endif
							CacheP_wbInv(gRadarCubeBuf,sizeof(cmplx16ImRe_t)*(MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME),CacheP_TYPE_ALL);
                            EDMA_enableTransferRegion(baseAddr, regionId, rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.channel, EDMA_TRIG_MODE_MANUAL);

                            while(EDMA_readIntrStatusRegion(baseAddr, regionId, rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.channel) != 1);

                            ClockP_usleep(1 * 5U);

                            EDMA_clrIntrRegion(baseAddr, regionId, rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.channel);

                            //Task_sleep(1);
                        }/* end of chirp loop */
                        memset((void *)&outParms, 0, sizeof(DPU_RangeProcHWA_OutParams));

                        retVal = DPU_RangeProcHWA_process(rangeProcDpuHandle, &outParms);
                    //test_print("DPU processing time %d, wait time %d\n", outParms.stats.processingTime, outParms.stats.waitTime);

#ifdef SUBSYS_MSS
                        CacheP_inv(gRadarCube, (sizeof(cmplx16ImRe_t)*MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME), CacheP_TYPE_ALLD);
#endif

                        /* check the result */
                    	test_print("... DPU Finish, Check Cube data ....  : \n");
                        if (checkResultFlag == 1)
                        {
                            testConfig.dpuNumRangeBins = rangeProcDpuCfg.staticCfg.numRangeBins;
                            Test_printResults(&testConfig, 0); // , (char*)&testConfigInfo);
                        }
                        else  //hard code to fail, if running into edma run time error
                        {
                            test_print("Test Cube Data FAILED!\n");
                            testFeatureStatus = 0;
                            finalResults = 0;
                        }


                        if (gCalibDcConfig.enabled)
                        {
                            /* control the rangeproc hwa*/
                            retVal = DPU_RangeProcHWA_control(rangeProcDpuHandle, DPU_RangeProcHWA_Cmd_triggerProc, NULL, 0);
                            if (retVal < 0)
                            {
                                /* Not Expected */
                            	test_print("RangeProc DPU control error %d\n", retVal);
                                DebugP_assert(0);
                            }

                            for (i = 0; i< testConfig.numChirpsPerFrame; i++)
                            {
                                /* i % testConfig.numChirpsPerFrameRef*/
                                chirpIdxRef = i - (i / testConfig.numChirpsPerFrameRef * testConfig.numChirpsPerFrameRef);

                                /* read in one chirp data */
                                for (j = 0; j < testConfig.numRxAntennas; j++)
                                {
                                    if(numBytesPerInputSample == 2){
                                        //16 byte aligned, 8 uint32 symbols
                                        memcpy((void *)&adcDataIn[(j * ((testConfig.numAdcSamples + 7) / 8 * 8))/2],
                                            (void *)&dataInBuffer[(chirpIdxRef * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins + j *testConfig.numRangeBins)/2], numBytesPerInputSample*testConfig.numAdcSamples);
                                    }
                                    else{
                                        //16 byte aligned, 4 uint32 symbols
                                        memcpy((void *)&adcDataIn[j * ((testConfig.numAdcSamples + 3) / 4 * 4)],
                                            (void *)&dataInBuffer[chirpIdxRef * MAX_NUM_RX_ANTENNA * testConfig.numRangeBins + j *testConfig.numRangeBins], numBytesPerInputSample*testConfig.numAdcSamples);
                                    }
                                }
                                if (testConfig.rxChanInterleave)
                                {
                                    /* end of rx antenna, data is aligned at 4 samples */
                                    if(numBytesPerInputSample == 2){
                                        Test_channelInterleave(adcDataIn, testConfig.numRxAntennas, (testConfig.numAdcSamples + 7) / 8 * 8, testConfig.isReal);
                                    }
                                    else{
                                        Test_channelInterleave(adcDataIn, testConfig.numRxAntennas, (testConfig.numAdcSamples + 3) / 4 * 4, testConfig.isReal);
                                    }
                                }
#ifdef SUBSYS_MSS
                                if(!gIsChirpDataReal){
                                    CacheP_wb(adcDataIn, (numBytesPerInputSample * testConfig.numRxAntennas * ((testConfig.numAdcSamples+3)/4*4)), CacheP_TYPE_ALLD);
                                }
                                else{
                                    CacheP_wb(adcDataIn, (numBytesPerInputSample * testConfig.numRxAntennas * ((testConfig.numAdcSamples+7)/8*8)), CacheP_TYPE_ALLD);
                                }
#endif
                                EDMA_enableTransferRegion(baseAddr, regionId, rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.channel, EDMA_TRIG_MODE_MANUAL);

                                while(EDMA_readIntrStatusRegion(baseAddr, regionId, rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.channel) != 1);
                                ClockP_usleep(1*5U);
                                EDMA_clrIntrRegion(baseAddr, regionId, rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.channel);

                            }/* end of chirp loop */
                            memset((void *)&outParms, 0, sizeof(DPU_RangeProcHWA_OutParams));

                            retVal = DPU_RangeProcHWA_process(rangeProcDpuHandle, &outParms);
#ifdef SUBSYS_MSS
                            CacheP_inv(gRadarCube, (sizeof(cmplx16ImRe_t)*MAX_NUM_ADCSAMPLE_PERCHIRP * MAX_NUM_CHIRPS_PERFRAME * NUM_REPEAT_PERFRAME), CacheP_TYPE_ALLD);
#endif

                            test_print("... DPU Finish, Check Calibration data ....  : \n");
                            if (checkResultFlag == 1)
                            {
                                testConfig.dpuNumRangeBins = rangeProcDpuCfg.staticCfg.numRangeBins;
                                Test_printResults(&testConfig,1); // , (char*)&testConfigInfo);
                            }
                            else  //hard code to fail, if running into edma run time error
                            {
                            	test_print("Test Calibration Data FAILED!\n");
                                testFeatureStatus = 0;
                                finalResults = 0;
                            }

                        }

                    /* Free EDMA Channels. */
                    /* DPEDMA_freeEDMAChannel(uint32_t *dmaCh, uint32_t *tcc, uint32_t *param, uint32_t *shadowParam)*/
                    DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.channel, &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.tcc,
                                &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.paramId, &rangeProcDpuCfg.hwRes.edmaInCfg.dataIn.shadowPramId );

                    DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.channel, &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.tcc,
                                    &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.paramId, &rangeProcDpuCfg.hwRes.edmaInCfg.dataInSignature.shadowPramId );


                    if((testConfig.radarCubeLayoutFmt == 1) && ((testConfig.numTxAntennas == 4) || (testConfig.numTxAntennas == 3)))
                    {
                        /* Ping */
                        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPing.channel, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPing.tcc,
                        &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPing.paramId, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPing.ShadowPramId[0] );

                        /* Pong */
                        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPong.channel, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPong.tcc,
                        &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPong.paramId, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPong.ShadowPramId[0] );

                        for(index = 0; index < (6 - testConfig.numTxAntennas); index++)
                        {
                            /* Ping */
                            DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPingData[index].channel, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPingData[index].tcc,
                            &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPingData[index].paramId, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPingData[index].shadowPramId );

                            /* Pong */
                            DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPongData[index].channel, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPongData[index].tcc,
                            &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPongData[index].paramId, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPongData[index].shadowPramId );
                        }

                        for(index = 1; index <  (6 - testConfig.numTxAntennas); index++)
                        {
                            testStatus = EDMA_freeParam(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPing.ShadowPramId[index] );
                            DebugP_assert(testStatus == SystemP_SUCCESS);

                            testStatus = EDMA_freeParam(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt2.dataOutPong.ShadowPramId[index] );
                            DebugP_assert(testStatus == SystemP_SUCCESS);
                        }
                    }
                    else
                    {
                        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPing.channel, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPing.tcc,
                                        &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPing.paramId, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPing.shadowPramId );

                        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPong.channel, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPong.tcc,
                                        &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPong.paramId, &rangeProcDpuCfg.hwRes.edmaOutCfg.u.fmt1.dataOutPong.shadowPramId );
                    }

                    DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &rangeProcDpuCfg.hwRes.edmaOutCfg.dataOutSignature.channel, &rangeProcDpuCfg.hwRes.edmaOutCfg.dataOutSignature.tcc,
                                    &rangeProcDpuCfg.hwRes.edmaOutCfg.dataOutSignature.paramId, &rangeProcDpuCfg.hwRes.edmaOutCfg.dataOutSignature.shadowPramId );

                    testStatus = EDMA_unregisterIntr(gEdmaHandle[CONFIG_EDMA0], rangeProcDpuCfg.hwRes.intrObj);
                    DebugP_assert(testStatus == SystemP_SUCCESS);

                    test_print("Test #%d finished!\n\r", testCount);
                    testCount++;
                    }/* end of fmt, interleave loop*/

                    if (testFeatureStatus == 1)
                    {
                        MCPI_setFeatureTestResult(featureName, MCPI_TestResult_PASS);
                    }
                    else
                    {
                        MCPI_setFeatureTestResult(featureName, MCPI_TestResult_FAIL);
                    }

                    test_print("\n");

                }    /* end of rx antenna number loop*/
            }/* end of number adc samples loop*/

        }/* end of number of tx, and fft size loop */

        fclose(fileId);
        test_print("%d Tests finished!\n\r", testCount);
        }

    taskEndTime=ClockP_getTimeUsec();
    test_print("Test Time Cycles: %lld\n",taskEndTime-taskStartTime);
    test_print("All Tests finished!\n\r");

    /*delete the handler*/
    HWA_close(hwaHandle);
    DPU_RangeProcHWA_deinit(rangeProcDpuHandle);
    MCPI_setTestResult ();

    Board_driversClose();
    Drivers_close();

    test_print ("--- Test Completed ---\n");
    if (finalResults == 0)
    {
        test_print ("tests fails!!\n");
    }
    else
    {
        test_print ("All tests have passed!!\n");
    }

    vTaskDelete(NULL);
}

/**
 *  @b Description
 *  @n
 *      Entry point into the test code.
 *
 *  @retval
 *      Not Applicable.
 */
int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( rangeProcDpuTest_Task,   /* Pointer to the function that implements the task. */
                                  "test_task_main", /* Text name for the task.  This is to facilitate debugging only. */
                                  APP_TASK_STACK_SIZE,  /* Stack depth in units of StackType_t typically uint32_t on 32b CPUs */
                                  NULL,              /* We are not using the task parameter. */
                                  APP_TASK_PRI,      /* task priority, 0 is lowest priority, configMAX_PRIORITIES-1 is highest */
                                  gAppTskStackMain,  /* pointer to stack base */
                                  &gAppTaskObj );    /* pointer to statically allocated task object memory */
    configASSERT(gAppTask != NULL);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    /* The following line should never be reached because vTaskStartScheduler()
    will only return if there was not enough FreeRTOS heap memory available to
    create the Idle and (if configured) Timer tasks.  Heap management, and
    techniques for trapping heap exhaustion, are described in the book text. */
    DebugP_assertNoLog(0);
}
