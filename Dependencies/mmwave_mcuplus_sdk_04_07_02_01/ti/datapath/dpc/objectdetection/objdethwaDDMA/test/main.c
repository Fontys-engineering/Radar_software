/**
 *   @file  main.c
 *
 *   @brief
 *      Unit Test code for the object detection DDMA DPC.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2019 - 2026 Texas Instruments, Inc.
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
#include "ti_drivers_config.h"
#include "ti_board_config.h"
#include "ti_drivers_open_close.h"
#include "ti_board_open_close.h"

#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/CycleCounterP.h>
#ifndef SUBSYS_M4
#include "FreeRTOS.h"
#include "task.h"
#endif

/*! This is supplied at command line when application builds this file. This file
 * is owned by the application and contains all resource partitioning, an
 * application may include more than one DPC and also use resources outside of DPCs.
 * The resource definitions used by this object detection DPC are prefixed by DPC_OBJDET */
#include APP_RESOURCE_FILE

/* mmwave SDK include files */
#include <ti/common/syscommon.h>
#include <ti/datapath/dpc/objectdetection/objdethwaDDMA/objectdetection.h>
#include <ti/datapath/dpc/objectdetection/common/test_data_path.h>
#include <ti/utils/mathutils/mathutils.h>
#ifdef INCLUDE_DPM
#include <ti/control/dpm/dpm.h>
#else
#ifdef SOC_AWR2X44P
#include <ti/demo/awr2x44P/mmw_ddm/mmw_common.c>
#endif
#endif
#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/board/antenna_geometry.h>

/*!< maximum number of frames for running tests */
#define MAX_NUM_TEST_FRAME (10)

/* Max permissible error values */
#define MAX_XDIM_ERR 0.1
#define MAX_YDIM_ERR 0.1
#define MAX_ZDIM_ERR 2.0
#define MAX_VEL_ERR 0.1

#define MAX_NUM_ADCSAMPLE_PERCHIRP 384
#define MAX_NUM_RX_ANTENNA 4
#define OBJ_DET_DDMA_Test_MAX_NUM_OBJECTS 50

#ifndef SUBSYS_M4
/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (1U)
#define APP_DPM_TASK_PRI     (5U)
#define APP_TASK_STACK_SIZE  (2*1024U)
#define APP_DPM_TASK_STACK_SIZE (2*1024U)
#endif

#define OBJ_DETECTION_TEST_CFAR_THRESHOLD_ENCODING_FACTOR (100.0)

#define CONST_LOG2_10  (3.3219)

#ifdef INCLUDE_DPM
uint16_t gCurrTestId;
uint32_t gMaxL3RamUsage = 0;
uint16_t gMaxL3RamUsageTestId = 0;

uint32_t gMaxCoreLocalRamUsage = 0;
uint16_t gMaxCoreLocalRamUsageTestId = 0;
#endif

float antennaCalibParams[SYS_COMMON_NUM_RX_CHANNEL * SYS_COMMON_NUM_TX_ANTENNAS * 2] __attribute__((aligned(8)));
uint8_t finalResults = 1;

/******************************************************************
 *                      Internal data structures
 ******************************************************************/

 /**
  * @brief
  *  Unit test static configuration
  *
  */
typedef struct ObjectDetectionDDMA_Test_staticCfg_t
{
    /*!< @brief Number of TX antennas */
    uint32_t numTxAntennas;

    /*!< @brief Number of RX antennas */
    uint32_t numRxAntennas;

     /*!< @brief Number of ADC samples per chirp */
    uint32_t numAdcSamples;

    /*!< @brief Number of chirps per frame */
    uint32_t numChirps;

    /*!< @brief Compression Method */
    uint8_t compMethod;

    /*!< @brief Compression Ratio */
    float compRatio;

    /*!< @brief Number of compression range bons per block */
    uint16_t compRangeBinsPerBlock;

    /*!< @brief flag to read data again */
    uint8_t readDataAgain;

    /*!< @brief flag to enable range CFAR */
    uint8_t isRangeCFARenabled;

    /*!< @brief range resolution */
    float rangeStep;

    /*!< @brief velocity resolution */
    float dopplerStep;

} ObjectDetectionDDMA_Test_staticCfg;


typedef struct ObjectDetectionDDMA_Test_SubFrameObj_t
{
    ObjectDetection_Test_SubFrameParams params;

    /*! @brief  Static configuration */
    DPC_ObjectDetection_StaticCfg staticCfg;

    /*! @brief  Sub-frame unit test results */
    DPC_ObjectDetection_ExecuteResult *result;
}ObjectDetectionDDMA_Test_SubFrameObj;

/**
 * @brief
 *  Millimeter Wave Demo Data Path Information.
 *
 * @details
 *  The structure is used to hold all the relevant information for
 *  the data path.
 */
typedef struct ObjectDetectionDDMA_Test_DataPathObj_t
{
    /*! @brief   Handle to the DPM Module */
    DPM_Handle   dpmHandle;

    /*! @brief   Handle to the hardware accelerator */
    HWA_Handle   hwaHandle;

    /*! @brief   Handle of the EDMA driver. */
    EDMA_Handle  edmaHandle[EDMA_NUM_CC];

    /*! @brief For notification to test task that the frame processing is completed  */
    SemaphoreP_Object semaphoreHandle;

#ifndef INCLUDE_DPM
    DPM_DPCHandle dpcHandle;
#endif

    /*! @brief  Common configuration storage */
    DPC_ObjectDetection_PreStartCommonCfg commonCfg;

    ObjectDetectionDDMA_Test_SubFrameObj testSubFrmObj[RL_MAX_SUBFRAMES];

} ObjectDetectionDDMA_Test_DataPathObj;


/******************************************************************
 *                      Global variables
 ******************************************************************/

/* FreeRTOS Task declarations. */
#ifndef SUBSYS_M4
TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

TaskHandle_t    gDpmTask;
StaticTask_t    gDpmTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(64)));
StackType_t gDpmTskStack[APP_TASK_STACK_SIZE] __attribute__((aligned(64)));
#endif

FILE *testParamsFileId;
FILE *adcDataFileID;
FILE *pointCloudObjFileID;
/**
 * @brief
 *  Data Path Object
 */
ObjectDetectionDDMA_Test_DataPathObj gDataPathObj;

/**
 * @brief
 *  Test static cfg
 */
ObjectDetectionDDMA_Test_staticCfg gtestStaticCfg[RL_MAX_SUBFRAMES];

uint8_t gIdealObjFoundList[OBJ_DET_DDMA_Test_MAX_NUM_OBJECTS];
uint8_t gIsReal;

#ifdef SUBSYS_DSS
/* data memeory */
#pragma DATA_SECTION(adcDataIn, ".dataBufL3");
uint32_t adcDataIn[MAX_NUM_ADCSAMPLE_PERCHIRP* MAX_NUM_RX_ANTENNA]
    __attribute__((aligned(CSL_CACHE_L1D_LINESIZE))); /*adc buffer for one chirp*/

/*! L3 RAM buffer */
#define OBJECT_DETECTION_L3RAM_CODE_SIZE  (60U * 1024U)
#if defined(SOC_AWR2943)
#define OBJECT_DETECTION_L3RAM_HEAP_SIZE (CSL_DSS_L3_U_SIZE - 0x80000 - OBJECT_DETECTION_L3RAM_CODE_SIZE)
#else
#define OBJECT_DETECTION_L3RAM_HEAP_SIZE (CSL_DSS_L3_U_SIZE - OBJECT_DETECTION_L3RAM_CODE_SIZE)
#endif
uint8_t gObjectDetectionDDMA_Test_L3Heap[OBJECT_DETECTION_L3RAM_HEAP_SIZE];
#pragma DATA_ALIGN(gObjectDetectionDDMA_Test_L3Heap, 4096U);
#pragma DATA_SECTION(gObjectDetectionDDMA_Test_L3Heap, ".l3ram");

uint8_t gObjectDetectionDDMA_Test_coreLocalRamHeap[90U *1024U];
// #pragma DATA_ALIGN(gObjectDetectionDDMA_Test_coreLocalRamHeap, 4096U);
#pragma DATA_SECTION(gObjectDetectionDDMA_Test_coreLocalRamHeap, ".dpc_l2Heap");


/*! Ideal point cloud output */
DPIF_PointCloudCartesian gPointCloudObjIdealResult[OBJ_DET_DDMA_Test_MAX_NUM_OBJECTS];
#pragma DATA_ALIGN(gPointCloudObjIdealResult, 4096U);
#pragma DATA_SECTION(gPointCloudObjIdealResult, ".l3ram");
#endif

#ifdef SUBSYS_M4

/* data memeory */
uint32_t adcDataIn[MAX_NUM_ADCSAMPLE_PERCHIRP* MAX_NUM_RX_ANTENNA]
    __attribute__((aligned(CSL_CACHE_L1D_LINESIZE), section(".dpc_l2Heap"))); /*adc buffer for one chirp*/

/*! L3 RAM buffer */

uint8_t *gObjectDetectionDDMA_Test_L3Heap = (uint8_t *)0x88000000;
#ifdef SOC_AWR2X44P
#if defined(SOC_AWR2X44ECO)
#define OBJECT_DETECTION_L3RAM_CODE_END  (0x260000U)
/* In AWR2x44ECO, 2.5MB of DSS_L3 space is available */
#define OBJECT_DETECTION_L3RAM_HEAP_SIZE (0x27E000U)
#elif defined(SOC_AWR2X44LC)
#define OBJECT_DETECTION_L3RAM_CODE_END  (0x1A0000U)
/* In AWR2x44LC, 1.5MB of DSS_L3 space is available */
#define OBJECT_DETECTION_L3RAM_HEAP_SIZE (0x1BE000U)
#else
#define OBJECT_DETECTION_L3RAM_CODE_END  (0x2E0000U)
/* In AWR2x44P, 3.0MB of DSS_L3 space is available */
#define OBJECT_DETECTION_L3RAM_HEAP_SIZE (0x2FE000U)
#endif
#endif
uint8_t gObjectDetectionDDMA_Test_coreLocalRamHeap[90U *1024U]
                __attribute__((aligned(CSL_CACHE_L1D_LINESIZE), section(".dpc_l2Heap")));

/*! Ideal point cloud output */
DPIF_PointCloudCartesian gPointCloudObjIdealResult[OBJ_DET_DDMA_Test_MAX_NUM_OBJECTS]
                __attribute__((aligned(CSL_CACHE_L1D_LINESIZE), section(".dpc_l2Heap")));
#endif

#ifdef SUBSYS_M4
void ObjectDetectionDDMA_Test_setCommonProfile(ObjectDetectionDDMA_Test_DataPathObj *obj, uint8_t numSubFrames) __attribute__((section(".customCode")));

void ObjectDetectionDDMA_Test_configDPC(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj) __attribute__((section(".customCode")));

static uint16_t ObjectDetectionDDMA_Test_convertDopplerCfarToThresh(uint16_t codedCfarVal) __attribute__((section(".customCode")));

static uint16_t ObjectDetectionDDMA_Test_convertRangeCfarToThresh(uint16_t codedCfarVal, uint8_t numBands) __attribute__((section(".customCode")));

void ObjectDetectionDDMA_Test_setProfile(
    ObjectDetectionDDMA_Test_DataPathObj *datapathObj,
    ObjectDetectionDDMA_Test_SubFrameObj *subFrameObj,
    uint8_t subFrameIndx
) __attribute__((section(".customCode")));

void ObjectDetectionDDMA_Test_DPCInit(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj) __attribute__((section(".customCode")));

void ObjectDetectionDDMA_Test_initComponents(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj) __attribute__((section(".customCode")));
#endif

volatile uint32_t gChirpIdx = 0;
volatile uint32_t gNumSubFrames = 0;
volatile uint32_t gSubFrameIndx = 0;
volatile uint32_t gFrameIdx = 0;
volatile uint32_t gNumFrames = 0;

/**
 *  @b Description
 *  @n
 *      Print assert information to console.
 */
void _ObjectDetectionDDMA_Test_debugAssert(int32_t expression, const char *file, int32_t line)
{
    if (!expression) {
        DebugP_log ("Exception: %s, line %d.\n",file,line);
    }
}

#define ObjectDetectionDDMA_Test_debugAssert(expression) {                                      \
                     _ObjectDetectionDDMA_Test_debugAssert(expression,      \
                              __FILE__, __LINE__);         \
                     DebugP_assert(expression);             \
                    }

void DPU_RangeProcHWA_Test_adcDataIn(uint32_t intrIdx, uint32_t paramSet, void * arg)
{
    uint32_t j;
    uint8_t numBytesPerInputSample;
    uint32_t baseAddr, regionId;
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
    numBytesPerInputSample = 2;
#elif defined(SOC_AWR2943)
    numBytesPerInputSample = 2;
#else
    #error "Error: Unsupported deviceType"
    numBytesPerInputSample = 0;
#endif
    /* process chirp */
    if(gFrameIdx == gNumFrames)
    {
        /* This is to stop the ISR from running if more than the specified number of frames is running */
        return;
    }

    /* read in one chirp data */
    for (j = 0; j < gtestStaticCfg[gSubFrameIndx].numRxAntennas; j++)
    {
        if (numBytesPerInputSample == 2)
        {
            /* 16 byte aligned, 8 uint16 symbols */
            int32_t alignedAddress8Samples = j * (((gtestStaticCfg[gSubFrameIndx].numAdcSamples + 7) / 8) * 8);
            int32_t alignedAddress32Bits = alignedAddress8Samples/2;
            fread((uint16_t *)&adcDataIn[alignedAddress32Bits],  sizeof(uint16_t), gtestStaticCfg[gSubFrameIndx].numAdcSamples, adcDataFileID);
        }
        else
        {
            /* 16 byte aligned, 4 uint32 symbols */
            int32_t alignedAddress4Samples = j * ((gtestStaticCfg[gSubFrameIndx].numAdcSamples + 3) / 4 * 4);
            fread((uint32_t *)&adcDataIn[alignedAddress4Samples],  sizeof(uint32_t), gtestStaticCfg[gSubFrameIndx].numAdcSamples, adcDataFileID);
        }
    }

    gChirpIdx++;
    if(gChirpIdx % 10 == 0){
        printf("Reading and Range Processing... %d percent\n", gChirpIdx * 100 / gtestStaticCfg[gSubFrameIndx].numChirps);
    }

    if(gChirpIdx == gtestStaticCfg[gSubFrameIndx].numChirps)
    {
        gChirpIdx = 0;
        gSubFrameIndx++;
        if(gSubFrameIndx == gNumSubFrames)
        {
            gSubFrameIndx = 0;
            gFrameIdx++;
        }
    }

    if(gDataPathObj.commonCfg.rangeProcCfg.rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_NTH_CHIRP_ESTIMATES_MODE)
    {
        baseAddr = EDMA_getBaseAddr(gEdmaHandle[0]);
        DebugP_assert(baseAddr != 0);

        regionId = EDMA_getRegionId(gEdmaHandle[0]);
        DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

        EDMA_enableTransferRegion(baseAddr, regionId, DPC_OBJDET_DPU_RANGEPROC_EDMAIN_CH, EDMA_TRIG_MODE_MANUAL);
    }
    /* end of chirp */
}
/**
  *  @b Description
  *  @n
  *         Initializes pre-start common configuration structure for DPC
  *
  *  @retval None
  *
  */
void ObjectDetectionDDMA_Test_setCommonProfile(ObjectDetectionDDMA_Test_DataPathObj *obj, uint8_t numSubFrames)
{
    uint32_t i, idx, j = 0;
#if defined (SOC_AWR2944) || defined(SOC_AWR2X44P)
    uint16_t antennaGeometryCfg[SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL] = {0,1,4,5,8,2,9,3,6,7,10,11,0,1,2,3};
    uint64_t zeroInsrtMaskAzim = 0xB7ED;
    uint64_t zeroInsrtMaskElev = 0x50A00;
#elif defined (SOC_AWR2943)
    uint16_t antennaGeometryCfg[SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL] = {0,1,2,3,4,5,6,7,0,1,2,3};
    uint64_t zeroInsrtMaskAzim = 0xFF;
    uint64_t zeroInsrtMaskElev = 0x3C;
#endif

    /* Fill common config */
    obj->commonCfg.numSubFrames = numSubFrames;

    for (i = 0; i < SYS_COMMON_NUM_TX_ANTENNAS * SYS_COMMON_NUM_RX_CHANNEL * 2; i+=2)
    {
        if (i < MAX_NUM_AZIM_VIRT_ANT * 2)
            idx = 2* antennaGeometryCfg[j++];
        else
            idx = 2*(MAX_NUM_AZIM_VIRT_ANT + antennaGeometryCfg[j++]);
        obj->commonCfg.antennaCalibParams[i] = antennaCalibParams[idx];
        obj->commonCfg.antennaCalibParams[i+1] = antennaCalibParams[idx+1];
    }

    obj->commonCfg.measureRxChannelBiasCfg.enabled = 0;
    obj->commonCfg.measureRxChannelBiasCfg.targetDistance = 3;
    obj->commonCfg.measureRxChannelBiasCfg.searchWinSize = 0.5;

    obj->commonCfg.zeroInsrtMaskCfg.zeroInsrtMaskAzim = zeroInsrtMaskAzim;
    obj->commonCfg.zeroInsrtMaskCfg.zeroInsrtMaskElev = zeroInsrtMaskElev;

    memcpy((void*)obj->commonCfg.antennaGeometryCfg, (void*)antennaGeometryCfg, sizeof(antennaGeometryCfg));
}

/**
 *  @b Description
 *  @n
 *      Utility function to convert the CFAR threshold
 *      from a CLI encoded dB value to a log2 value
 *      as expected by the doppler CFAR DPU
 *
 *  @param[in] codedCfarVal CFAR SNR threshold in dB as encoded in the CLI
 *  @param[in] numBands Total number of subbands
 *
 *  @retval
 *      CFAR threshold in dB format
 */
static uint16_t ObjectDetectionDDMA_Test_convertDopplerCfarToThresh(uint16_t codedCfarVal)
{
    uint16_t linearVal;
    float    dbVal, linVal;

    /* dbVal is a float value from 0-100dB. It needs to
    be converted to linear scale..
    First, recover float dbVal that was encoded in CLI. */
    dbVal = (float)(codedCfarVal / OBJ_DETECTION_TEST_CFAR_THRESHOLD_ENCODING_FACTOR);

    /* Now convert it to linear value */
    linVal = (uint32_t)(dbVal/20.0 * CONST_LOG2_10 * 2048.0);

    linearVal = (uint16_t) linVal;
    return (linearVal);
}


/**
 *  @b Description
 *  @n
 *      Utility function to convert the CFAR threshold
 *      from a CLI encoded dB value to a log2 value
 *      as expected by the Range CFAR DPU
 *
 *  @param[in] codedCfarVal CFAR SNR threshold in dB as encoded in the CLI
 *  @param[in] numBands Total number of subbands
 *
 *  @retval
 *      CFAR threshold in dB format
 */
static uint16_t ObjectDetectionDDMA_Test_convertRangeCfarToThresh(uint16_t codedCfarVal, uint8_t numBands)
{
    uint16_t linearVal;
    float    dbVal, linVal;
    uint32_t defaultScaling = 1 << 11;
    float additionalScaling =  numBands / (float)(1 << mathUtils_ceilLog2(numBands));

    /* dbVal is a float value from 0-100dB. It needs to
    be converted to linear scale..
    First, recover float dbVal that was encoded in CLI. */
    dbVal = (float)(codedCfarVal / OBJ_DETECTION_TEST_CFAR_THRESHOLD_ENCODING_FACTOR);

    /* Now convert it to linear value */
    linVal = (uint32_t)(log2f(pow(10, (float)dbVal/20.0)) * additionalScaling * defaultScaling + 0.5);

    linearVal = (uint16_t) linVal;
    return (linearVal);
}


int32_t MmwDemo_getNumEmptySubBands(uint32_t numTxAntennas){

    int32_t numBandsEmpty;
    /* Empty subbands */
    switch (numTxAntennas)
    {
        case 2:
            numBandsEmpty = 1;
            break;
        case 3:
            numBandsEmpty = 1;
            break;
        case 4:
            numBandsEmpty = 2;
            break;
        default:
            numBandsEmpty = -1;
            goto exit;
    }

exit:
    return numBandsEmpty;
}

/**
  *  @b Description
  *  @n
  *     Initializes sub-frame test data structure and DPC static
  *     and dynamic configuration structures
  *
  *  @param[in] datapathObj pointer to data path test object
  *
  *  @param[out] subFrameObj pointer to subframe test object
  *
  *  @retval None
  *
  */
void ObjectDetectionDDMA_Test_setProfile
(
    ObjectDetectionDDMA_Test_DataPathObj *datapathObj,
    ObjectDetectionDDMA_Test_SubFrameObj *subFrameObj,
    uint8_t subFrameIndx
)
{
    ObjectDetection_Test_SubFrameParams *params;
    uint32_t i;

    params = &subFrameObj->params;

    datapathObj->commonCfg.rangeProcCfg.fftOutputScaling = 0U;

    /* Hard coded these two values */
    params->isInterleavedSampInAdcBuf = 1;
    params->isBpmEnabled = 0;

    params->numRxAntennas = gtestStaticCfg[subFrameIndx].numRxAntennas;

    params->numAdcSamples = gtestStaticCfg[subFrameIndx].numAdcSamples;
    params->rxChanOffsetInSamples = CSL_MEM_ALIGN(params->numAdcSamples, 4);

    if (gIsReal){
        params->numRangeBins = mathUtils_getValidFFTSize(params->numAdcSamples)/2 ;
    }
    else{
        params->numRangeBins = mathUtils_getValidFFTSize(params->numAdcSamples) ;
    }

    params->numTxAntennas = gtestStaticCfg[subFrameIndx].numTxAntennas;

    params->numChirpsPerFrame = gtestStaticCfg[subFrameIndx].numChirps;
    params->numDopplerChirps = gtestStaticCfg[subFrameIndx].numChirps;
    params->numDopplerBins = mathUtils_getValidFFTSize(params->numDopplerChirps);

    params->numVirtualAntAzim   = (params->numTxAntennas -1) * params->numRxAntennas;
    params->numVirtualAntElev   = 1 * params->numRxAntennas;
    params->numVirtualAntennas  = params->numVirtualAntAzim + params->numVirtualAntElev;
    params->numAngleBins = 4 * mathUtils_getValidFFTSize(params->numVirtualAntAzim);

    /* Workaround for range DPU limitation for FFT size 1024 and 12 virtual antennas case*/
    if ((params->numVirtualAntennas == 12) && (params->numRangeBins == 1024))
    {
        params->numRangeBins = 1022;
    }

    /* Fill static config */

    subFrameObj->staticCfg.isValidProfileHasOneTxPerChirp = 0;

    if(gIsReal){
        subFrameObj->staticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_REAL16;
        subFrameObj->staticCfg.numRangeFFTBins = params->numRangeBins*2;
    }
    else{
        subFrameObj->staticCfg.ADCBufData.dataProperty.dataFmt = DPIF_DATAFORMAT_COMPLEX16_IMRE;
        subFrameObj->staticCfg.numRangeFFTBins = params->numRangeBins;
    }

    subFrameObj->staticCfg.ADCBufData.dataProperty.interleave = DPIF_RXCHAN_INTERLEAVE_MODE;
    subFrameObj->staticCfg.ADCBufData.dataProperty.numAdcSamples = params->numAdcSamples;
    subFrameObj->staticCfg.ADCBufData.dataProperty.numChirpsPerChirpEvent = 1; // since this is HWA based chain
    subFrameObj->staticCfg.ADCBufData.dataProperty.numRxAntennas = params->numRxAntennas;
    subFrameObj->staticCfg.ADCBufData.dataProperty.adcBits = 2;

    for (i = 0; i < params->numRxAntennas; i++)
    {
        if(gIsReal){
            subFrameObj->staticCfg.ADCBufData.dataProperty.rxChanOffset[i] =  i *
                    CSL_MEM_ALIGN((params->numAdcSamples * sizeof(int16_t)), 16);
        }
        else{
            subFrameObj->staticCfg.ADCBufData.dataProperty.rxChanOffset[i] =  i *
                    CSL_MEM_ALIGN((params->numAdcSamples * sizeof(cmplx16ImRe_t)), 16);
        }
    }

    subFrameObj->staticCfg.ADCBufData.data = (void *)&adcDataIn[0];
    if(gIsReal){
        subFrameObj->staticCfg.ADCBufData.dataSize = params->numRxAntennas * params->numAdcSamples * sizeof(int16_t);
    }
    else{
        subFrameObj->staticCfg.ADCBufData.dataSize = params->numRxAntennas * params->numAdcSamples * sizeof(cmplx16ImRe_t);
    }
    subFrameObj->staticCfg.numTxAntennas = params->numTxAntennas;
    subFrameObj->staticCfg.numChirpsPerFrame = params->numChirpsPerFrame;
    subFrameObj->staticCfg.numChirps = params->numChirpsPerFrame;
    subFrameObj->staticCfg.numDopplerBins = mathUtils_getValidFFTSize(subFrameObj->staticCfg.numChirps);
    subFrameObj->staticCfg.numRangeBins = params->numRangeBins;
    subFrameObj->staticCfg.numVirtualAntAzim = params->numVirtualAntAzim;
    subFrameObj->staticCfg.numVirtualAntElev = params->numVirtualAntElev;
    subFrameObj->staticCfg.numVirtualAntennas = params->numVirtualAntennas;
    subFrameObj->staticCfg.isSumTxEnabled = 1;
    subFrameObj->staticCfg.numBandsTotal = params->numTxAntennas + MmwDemo_getNumEmptySubBands(params->numTxAntennas);
    subFrameObj->staticCfg.rangeStep = gtestStaticCfg[subFrameIndx].rangeStep;
    subFrameObj->staticCfg.dopplerStep = gtestStaticCfg[subFrameIndx].dopplerStep;

    subFrameObj->staticCfg.dataInputMode = DPU_RangeProcHWA_InputMode_ISOLATED;

#ifdef SOC_AWR294X
    subFrameObj->staticCfg.ADCBufConfig.rxChannelEn = params->numRxAntennas;

    if(gIsReal){
        subFrameObj->staticCfg.ADCBufConfig.adcBufChanDataSize = (params->numAdcSamples * sizeof(int16_t) + 15U) / 16U * 16U;
    }
    else{
        subFrameObj->staticCfg.ADCBufConfig.adcBufChanDataSize = (params->numAdcSamples * sizeof(cmplx16ImRe_t) + 15U) / 16U * 16U;
    }
#endif

    /* Cfar Cfg */
    subFrameObj->staticCfg.cfarCfg.subFrameNum = subFrameIndx;
    subFrameObj->staticCfg.cfarCfg.cfg.isEnabled = 1;
    subFrameObj->staticCfg.cfarCfg.cfg.thresholdScale = ObjectDetectionDDMA_Test_convertDopplerCfarToThresh(24*OBJ_DETECTION_TEST_CFAR_THRESHOLD_ENCODING_FACTOR);
    subFrameObj->staticCfg.cfarCfg.cfg.averageMode = 3;
    subFrameObj->staticCfg.cfarCfg.cfg.winLen = 16;
    subFrameObj->staticCfg.cfarCfg.cfg.guardLen = 0;
    subFrameObj->staticCfg.cfarCfg.cfg.noiseDivShift = 0;
    subFrameObj->staticCfg.cfarCfg.cfg.cyclicMode = 1;
    subFrameObj->staticCfg.cfarCfg.cfg.peakGroupingScheme = 0;
    subFrameObj->staticCfg.cfarCfg.cfg.peakGroupingEn = 0;
    subFrameObj->staticCfg.cfarCfg.cfg.osKvalue = 7;
    subFrameObj->staticCfg.cfarCfg.cfg.osEdgeKscaleEn = 0;
    subFrameObj->staticCfg.cfarCfg.cfg.variableThresholdMode = 1U;


    /* Range Cfar Cfg */
    subFrameObj->staticCfg.rangeCfarCfg.subFrameNum = subFrameIndx;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.isEnabled = gtestStaticCfg[subFrameIndx].isRangeCFARenabled;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.thresholdScale = ObjectDetectionDDMA_Test_convertRangeCfarToThresh(15*OBJ_DETECTION_TEST_CFAR_THRESHOLD_ENCODING_FACTOR, subFrameObj->staticCfg.numBandsTotal);
    subFrameObj->staticCfg.rangeCfarCfg.cfg.averageMode = 3;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.winLen = 16;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.guardLen = 0;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.noiseDivShift = 0;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.cyclicMode = 1;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.peakGroupingScheme = 0;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.peakGroupingEn = 0;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.osKvalue = 7;
    subFrameObj->staticCfg.rangeCfarCfg.cfg.osEdgeKscaleEn = 0;


    /* Compression Cfg */
    subFrameObj->staticCfg.compressionCfg.isEnabled = 1;
    subFrameObj->staticCfg.compressionCfg.compressionMethod = gtestStaticCfg[subFrameIndx].compMethod;
    subFrameObj->staticCfg.compressionCfg.compressionRatio = gtestStaticCfg[subFrameIndx].compRatio;
    subFrameObj->staticCfg.compressionCfg.rangeBinsPerBlock = gtestStaticCfg[subFrameIndx].compRangeBinsPerBlock;
    subFrameObj->staticCfg.compressionCfg.numRxAntennaPerBlock = 4;
    if(subFrameObj->staticCfg.compressionCfg.compressionMethod == HWA_COMPRESS_METHOD_BFP)
        subFrameObj->staticCfg.compressionCfg.bfpCompExtraParamSets = 2*(subFrameObj->staticCfg.compressionCfg.numRxAntennaPerBlock - 1);
    else
        subFrameObj->staticCfg.compressionCfg.bfpCompExtraParamSets = 0;

    /* Inteference Statistics Cfg */
    subFrameObj->staticCfg.intfStatsdBCfg.intfMitgMagSNRdB = 15;
    subFrameObj->staticCfg.intfStatsdBCfg.intfMitgMagDiffSNRdB = 18;

    /* Local Max Cfg */
    subFrameObj->staticCfg.localMaxCfg.azimThreshold = 6;
    subFrameObj->staticCfg.localMaxCfg.dopplerThreshold = 40;

    /* Aoa Fov Cfg */
    subFrameObj->staticCfg.aoaFovCfg.minAzimuthDeg = -90;
    subFrameObj->staticCfg.aoaFovCfg.maxAzimuthDeg = 90;
    subFrameObj->staticCfg.aoaFovCfg.minElevationDeg = -90;
    subFrameObj->staticCfg.aoaFovCfg.maxElevationDeg = 90;

}


/**
 *  @b Description
 *  @n
 *      Compares two PointCloud structures and returns 0 if within error limits
 */
int32_t comparePointCloudObjOut(DPIF_PointCloudCartesian * obj1, DPIF_PointCloudCartesian * obj2){

    if(fabsf(obj1->x - obj2->x) < MAX_XDIM_ERR){
        if(fabsf(obj1->y - obj2->y) < MAX_YDIM_ERR){
            if(fabsf(obj1->z - obj2->z) < MAX_ZDIM_ERR){
                if(fabsf(obj1->velocity - obj2->velocity) < MAX_VEL_ERR){
                    return 0;
                }
            }
        }
    }
    /* object not found */
    return 1;
}


/**
  *  @b Description
  *  @n
  *     Compare test data to reference data
  *
  *  @param[in] obj Pointer to subframe reference data
  *
  *  @retval    Comparison result: 1: Passed, 0: Failed
  *
  */

uint32_t ObjectDetectionDDMA_Test_ComparePointCloudObjsToRef(DPIF_PointCloudCartesian * obtainedPointCloudList, DPIF_PointCloudCartesian *idealPointCloudList, uint32_t numObjOut, uint32_t numObjOutIdeal)
{
    uint32_t retVal = 0;
    uint32_t i,j;
    uint8_t extraObjFound = 0;
    memset(&gIdealObjFoundList, 0, OBJ_DET_DDMA_Test_MAX_NUM_OBJECTS * sizeof(uint8_t));
    uint8_t noLog=1;

    DebugP_log("NumObjOut = %d, NumObjOutIdeal = %d\n", numObjOut, numObjOutIdeal);
    for(i=0; i< numObjOut; i++){
        extraObjFound = 1;
        for(j=0; j<numObjOutIdeal; j++){
            if(!comparePointCloudObjOut(&obtainedPointCloudList[i], &idealPointCloudList[j])){
                gIdealObjFoundList[j] = 1;
                extraObjFound = 0;
                // break;
            }
        }
        if(extraObjFound){
            noLog=0;
            DebugP_log("Extra Object Found. ObjIdx = %d, X = %f, Y = %f, Z = %f, Velocity = %f \n",
                    i, obtainedPointCloudList[i].x, obtainedPointCloudList[i].y, obtainedPointCloudList[i].z, obtainedPointCloudList[i].velocity);
            finalResults = 0;
        }

    }

    for(i = 0; i < numObjOutIdeal; i++){
        if(!gIdealObjFoundList[i]){
            noLog=0;
            DebugP_log("Object Not Found. ObjIdx = %d, X = %f, Y = %f, Z = %f, Velocity = %f \n",
                    i, idealPointCloudList[i].x, idealPointCloudList[i].y, idealPointCloudList[i].z, idealPointCloudList[i].velocity);
            finalResults = 0;
        }
    }
    if(noLog){
        DebugP_log("All objects matched!\n");
    }


    DebugP_log("\n");

    return retVal;
}


#ifdef INCLUDE_DPM
/*
    *  @b Description
    *  @n  DPM Registered Report Handler. The DPM Module uses this registered function to notify
    *      the application about DPM reports.
    *     Report function registered with DPM to report events
    *     and DPM processing status
    *  @param[in]  reportType
    *      Report Type
    *  @param[in]  instanceId
    *      Instance Identifier which generated the report
    *  @param[in]  errCode
    *      Error code if any.
    *  @param[in] arg0
    *      Argument 0 interpreted with the report type
    *  @param[in] arg1
    *      Argument 1 interpreted with the report type
    *
    *  @retval
    *      Not Applicable.
*/
static void ObjectDetectionDDMA_Test_reportFxn
(
    DPM_Report  reportType,
    uint32_t    instanceId,
    int32_t     errCode,
    uint32_t    arg0,
    uint32_t    arg1
)
{

    /* Only errors are logged on the console: */
    if (errCode != 0)
    {
        /* Error: Detected log on the console and die all errors are FATAL currently. */
        DebugP_log ("Error: DPM Report %d received with error:%d arg0:0x%x arg1:0x%x\n",
                        reportType, errCode, arg0, arg1);
        DebugP_assert (0);
    }

    /* Processing further is based on the reports received: This is the control of the profile
     * state machine: */
    switch (reportType)
    {
        case DPM_Report_IOCTL:
        {
            /*****************************************************************
             * DPC has been configured without an error:
             * - This is an indication that the profile configuration commands
             *   went through without any issues.
             *****************************************************************/

            if (arg0 == DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG)
            {
                DPC_ObjectDetection_PreStartCfg *cfg;
                DPC_ObjectDetection_DPC_IOCTL_preStartCfg_memUsage *memUsage;

                /* Get memory usage from preStartCfg */
                cfg = (DPC_ObjectDetection_PreStartCfg*)arg1;
                memUsage = &cfg->memUsage;

                if(memUsage->L3RamUsage > gMaxL3RamUsage)
                {
                    gMaxL3RamUsage = memUsage->L3RamUsage;
                    gMaxL3RamUsageTestId = gCurrTestId;
                }
                if(memUsage->CoreLocalRamUsage > gMaxCoreLocalRamUsage)
                {
                    gMaxCoreLocalRamUsage = memUsage->CoreLocalRamUsage;
                    gMaxCoreLocalRamUsageTestId = gCurrTestId;
                }
                DebugP_log("DPC ObjDet Memory usage (SubFrameIndx = %d): L3: %d Bytes, CoreLocalRam: %d Bytes\n",
                              cfg->subFrameNum,
                              memUsage->L3RamUsage,
                              memUsage->CoreLocalRamUsage);

            }
            break;
        }
        case DPM_Report_DPC_STARTED:
        {
            /*****************************************************************
             * DPC has been started without an error:
             * - we can start chirping
             *****************************************************************/

            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT:
        {
            /*****************************************************************
             * DPC Results have been passed:
             * - This implies that we have valid profile results which have
             *   been received from the profile.
             *****************************************************************/

            break;
        }
        case DPM_Report_DPC_ASSERT:
        {
            DPM_DPCAssert*  ptrAssert;

            /*****************************************************************
             * DPC Fault has been detected:
             * - This implies that the DPC has crashed.
             * - The argument0 points to the DPC assertion information
             *****************************************************************/
            ptrAssert = (DPM_DPCAssert*)arg0;
            DebugP_log ("DSS Exception: %s, line %d.\n", ptrAssert->fileName,
                       ptrAssert->lineNum);
            break;
        }
        case DPM_Report_DPC_STOPPED:
        {
            /*****************************************************************
             * DPC has been stopped without an error:
             * - This implies that the DPC can either be reconfigured or
             *   restarted.
             *****************************************************************/
            break;
        }
        case DPM_Report_NOTIFY_DPC_RESULT_ACKED:
        {
            /*****************************************************************
             * DPC Results have been acked:
             * - This implies that the results are received.
             *****************************************************************/
            break;
        }
        case DPM_Report_DPC_INFO:
        {
            /* Currently objDetHwa does not use this feature. */
            break;
        }
        default:
        {
            DebugP_assert (0);
            break;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      DPM Execution Task
 *
 *  @retval
 *      Not Applicable.
 */
static void ObjectDetectionDDMA_Test_dpmTask(void* args)
{
    int32_t     errCode;
    DPM_Buffer  resultBuffer;

    while (1)
    {
        /* Execute the DPM module: */
        errCode = DPM_execute (gDataPathObj.dpmHandle, &resultBuffer);
        if (errCode < 0) {
            DebugP_log ("Error: DPM execution failed %d\n", errCode);
            ObjectDetectionDDMA_Test_debugAssert (0);
        }
        else
        {
            if (resultBuffer.size[0] == sizeof(DPC_ObjectDetection_ExecuteResult))
            {
                DPC_ObjectDetection_ExecuteResult *res = (DPC_ObjectDetection_ExecuteResult *)resultBuffer.ptrBuffer[0];
                uint8_t subFrameIdx = res->subFrameIdx;
                gDataPathObj.testSubFrmObj[subFrameIdx].result = (DPC_ObjectDetection_ExecuteResult *)resultBuffer.ptrBuffer[0];

                /* Indicate to test task that the sub-frame is completed */
                SemaphoreP_post (&gDataPathObj.semaphoreHandle);
            }
        }
    }
}
#else
static void ObjectDetectionDDMA_Test_reportFxn (void *data, uint16_t dataLen)
{
    if(dataLen == sizeof(uint32_t))
    {
        /* start the execution */
        SemaphoreP_post (&gDataPathObj.semaphoreHandle);
    }
    return;
}
#endif

/**
 *  @b Description
 *  @n
 *      Close HWA instance.
 */
void ObjectDetectionDDMA_Test_hwaClose(ObjectDetectionDDMA_Test_DataPathObj *obj)
{
    int32_t             errCode;

    /* Close the HWA Instance */
    errCode = HWA_close(obj->hwaHandle);
    if (errCode != 0)
    {
        ObjectDetectionDDMA_Test_debugAssert (0);
        return;
    }
}


/**
 *  @b Description
 *  @n
 *      Close EDMA.
 */
void ObjectDetectionDDMA_Test_edmaClose(ObjectDetectionDDMA_Test_DataPathObj *obj)
{
    uint32_t            edmaCCIdx=0;

    for (edmaCCIdx=0;edmaCCIdx<EDMA_NUM_CC;edmaCCIdx++)
    {
        EDMA_close(obj->edmaHandle[edmaCCIdx]);
    }
}

/**
  *  @b Description
  *  @n     Initializes DPM, and launches DPM task
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  */
void ObjectDetectionDDMA_Test_DPCInit(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj)
{
    DPM_InitCfg                         dpmInitCfg;
    int32_t                             errCode;
    uint32_t                            edmaCCIdx;
    DPC_ObjectDetection_InitParams      objDetInitParams;
    static bool                         dpmInit = true;

    /*****************************************************************************
     * Initialization of the DPM Module:
     *****************************************************************************/
    memset ((void *)&objDetInitParams, 0, sizeof(DPC_ObjectDetection_InitParams));
    objDetInitParams.hwaHandle = dataPathObj->hwaHandle;
    for (edmaCCIdx = 0; edmaCCIdx < EDMA_NUM_CC; edmaCCIdx++)
    {
        objDetInitParams.edmaHandle[edmaCCIdx] = dataPathObj->edmaHandle[edmaCCIdx];
    }

    /* Memory related config */
#if defined(SOC_AWR2X44P)
    objDetInitParams.L3ramCfg.addr        = (void*)gObjectDetectionDDMA_Test_L3Heap;
    objDetInitParams.L3ramCfg.size        = OBJECT_DETECTION_L3RAM_HEAP_SIZE;
    objDetInitParams.L3ramCfg.endSize     = objDetInitParams.L3ramCfg.size - OBJECT_DETECTION_L3RAM_CODE_END;
#else
    objDetInitParams.L3ramCfg.addr        = &gObjectDetectionDDMA_Test_L3Heap;
    objDetInitParams.L3ramCfg.size        = sizeof(gObjectDetectionDDMA_Test_L3Heap);
    objDetInitParams.L3ramCfg.endSize     = sizeof(gObjectDetectionDDMA_Test_L3Heap);
#endif
    objDetInitParams.CoreLocalRamCfg.addr = &gObjectDetectionDDMA_Test_coreLocalRamHeap;
    objDetInitParams.CoreLocalRamCfg.size = sizeof(gObjectDetectionDDMA_Test_coreLocalRamHeap);


    memset ((void *)&dpmInitCfg, 0, sizeof(DPM_InitCfg));

#ifdef INCLUDE_DPM
    /* Setup the configuration: */
    dpmInitCfg.ptrProcChainCfg  = &gDPC_ObjectDetectionCfg;
    dpmInitCfg.instanceId       = 0xFEEDFEED;
    dpmInitCfg.domain           = DPM_Domain_LOCALIZED;
    dpmInitCfg.reportFxn        = ObjectDetectionDDMA_Test_reportFxn;
    dpmInitCfg.arg              = &objDetInitParams;
    dpmInitCfg.argSize          = sizeof(DPC_ObjectDetection_InitParams);
#else
    /* Setup the configuration: */
    dpmInitCfg.localEndPt  = gRemoteCoreEndPt[CSL_CORE_ID_M4SS0_1];
    dpmInitCfg.reportFxn   = ObjectDetectionDDMA_Test_reportFxn;
    dpmInitCfg.setBitPos   = DPM_DSS_CM4_BOOT_INFO_BIT_POS;
#endif

    if(dpmInit)
    {
        /* Initialize the DPM Module: */
        dataPathObj->dpmHandle = DPM_init (&dpmInitCfg, &errCode);
        if (dataPathObj->dpmHandle == NULL)
        {
            DebugP_log ("Error: DPM Init: %d\n", errCode);
            ObjectDetectionDDMA_Test_debugAssert (0);
            return;
        }
    }
#ifdef INCLUDE_DPM
    /* Launch the DPM Task */
    gDpmTask = xTaskCreateStatic( ObjectDetectionDDMA_Test_dpmTask,
                                      "test_dpm_task",
                                      APP_DPM_TASK_STACK_SIZE,
                                      NULL,
                                      APP_DPM_TASK_PRI,
                                      gDpmTskStack,
                                      &gDpmTaskObj );

    configASSERT(gDpmTask != NULL);
#else
    if(dpmInit)
    {
        dpmInit = false; /* DPM_init procedure should not happen in subsequent tests, but only the first test */
        while (1)
        {
            int32_t syncStatus;
            int32_t syncStatusRefVal = (uint32_t)1U << DPM_DSS_CM4_BOOT_INFO_BIT_POS;
#if MSS_AOA_ENABLED
            syncStatusRefVal += (uint32_t)1U << DPM_MSS_BOOT_INFO_BIT_POS;
#else
            syncStatusRefVal += (uint32_t)1U << DPM_DSS_BOOT_INFO_BIT_POS;
#endif
            /* Get the synchronization status: */
            syncStatus = DPM_synch (dataPathObj->dpmHandle, &errCode);
            if (syncStatus < 0)
            {
                /* Error: Unable to synchronize the framework */
                DebugP_log ("Error: DPM Synchronization %d\n", errCode);
                ObjectDetectionDDMA_Test_debugAssert (0);
                return;
            }
            if (syncStatus == syncStatusRefVal)
            {
                /* Synchronization acheived: */
                break;
            }
            /* Sleep and poll again: */
            ClockP_usleep(1 * 1000U);
        }
    }

    dataPathObj->dpcHandle = DPC_ObjectDetection_init (&objDetInitParams, &errCode);
    if (dataPathObj->dpcHandle == NULL)
    {
        DebugP_log ("Error: DPC Init: %d]\n", errCode);
        ObjectDetectionDDMA_Test_debugAssert (0);
        return;
    }
#endif
}

/**
  *  @b Description
  *  @n
  *     Initializes test components: EDMA, HWA, DPM/DPC
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  *
  */
void ObjectDetectionDDMA_Test_initComponents(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj)
{
    /* Initialize the HWA */
    int32_t             errCode;

    /* Open the HWA Instance */
    dataPathObj->hwaHandle = HWA_open(0, NULL, &errCode);
    if (dataPathObj->hwaHandle == NULL)
    {
        ObjectDetectionDDMA_Test_debugAssert (0);
        return;
    }
    errCode = HWA_reset(dataPathObj->hwaHandle);
    DebugP_assert(errCode==SystemP_SUCCESS);

    /* EDMA init */
    dataPathObj->edmaHandle[0] = gEdmaHandle[CONFIG_EDMA0];

    /* DPM/DPC init */
    ObjectDetectionDDMA_Test_DPCInit(dataPathObj);

    int32_t status = SystemP_SUCCESS;
    /* Initialize semaphore for sub-frame unit test completion indication */
    status = SemaphoreP_constructBinary(&dataPathObj->semaphoreHandle, 0);
    DebugP_assert(status==SystemP_SUCCESS);
}

/**
  *  @b Description
  *  @n
  *     DPM Deinitialization
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  *
  */
void ObjectDetectionDDMA_Test_DPCDeinit(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj)
{
    int32_t                             errCode;

    /* Deinitialize the DPM Module: */
#ifdef INCLUDE_DPM
    errCode = DPM_deinit (dataPathObj->dpmHandle);
#else
    errCode = DPC_ObjectDetection_deinit(dataPathObj->dpcHandle);
#endif
    if (errCode<0)
    {
        DebugP_log ("Error: Unable to de-initialize the DPM Module [Error: %d]\n", errCode);
        ObjectDetectionDDMA_Test_debugAssert (0);
        return;
    }

#ifdef INCLUDE_DPM
    vTaskDelete(gDpmTask);
#endif
}

/**
  *  @b Description
  *  @n
  *         DeInitializes test components: HWA, EDMA, DPC
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  *
  */
void ObjectDetectionDDMA_Test_deinitComponents(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj)
{
    /* HWA close*/
    ObjectDetectionDDMA_Test_hwaClose(dataPathObj);

    /* DPM/DPC deinit */
    ObjectDetectionDDMA_Test_DPCDeinit(dataPathObj);

    /* destruct semaphore */
    SemaphoreP_destruct(&(dataPathObj->semaphoreHandle));
}

/**
  *  @b Description
  *  @n
  *     Sends ioctl trigger start frame to DPC to simulate the frame interrupt.
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  *
  */
void ObjectDetectionDDMA_Test_trigFrameStart(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj)
{
#ifdef INCLUDE_DPM
    int32_t                         errCode;
    /* send DPC_OBJDET_IOCTL__TRIGGER_FRAME */
    errCode = DPM_ioctl (dataPathObj->dpmHandle,
                         DPC_OBJDET_IOCTL__TRIGGER_FRAME,
                         NULL,
                         0);

    if (errCode < 0)
    {
        DebugP_log ("Error: Unable to send DPC_OBJDET_IOCTL__TRIGGER_FRAME [Error:%d]\n", errCode);
        ObjectDetectionDDMA_Test_debugAssert (0);
        return;
    }
#else
    /* Simulate the trigger of frame start event for DPC*/
    DPC_ObjectDetection_frameStart (dataPathObj->dpcHandle);
#endif
}

/**
  *  @b Description
  *  @n
  *     Configures DPC for 4 subframes. Sends one ioctl for
  *     common configuration and one ioctal per each subframe.
  *
  *  @param[in] dataPathObj Pointer to main test data structure
  *
  *  @retval    None
  */
void ObjectDetectionDDMA_Test_configDPC(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj)
{
    int32_t                         errCode;
    union DPC_ObjectDetection_CommonCfgUnion {
        DPC_ObjectDetection_PreStartCommonCfg objDetPreStartCommonCfg;
#if defined(SOC_AWR2X44P)
        DPC_ObjectDetection_ElevEstCommonCfg elevEstCommonCfg;
#endif
    } commonCfg;
    union DPC_ObjectDetection_SubframeCfgUnion {
        DPC_ObjectDetection_PreStartCfg objDetPreStartCfg;
#if defined(SOC_AWR2X44P)
        DPC_ObjectDetection_ElevEstSubframeCfg elevEstSubframeCfg;
#endif
    } subframeCfg;
    int8_t subFrameIdx =0;

    memset(&commonCfg.objDetPreStartCommonCfg, 0, sizeof(DPC_ObjectDetection_PreStartCommonCfg));
    memset(&subframeCfg.objDetPreStartCfg, 0, sizeof(DPC_ObjectDetection_PreStartCfg));

    /* Reset the chirpIdx counter */
    gChirpIdx = 0;

    /**************************************/
    /* Prepare and send the pre start common configuration */
    /**************************************/

    /* Fill configuration */
    commonCfg.objDetPreStartCommonCfg = dataPathObj->commonCfg;

    /* send DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG */
#ifdef INCLUDE_DPM
    errCode = DPM_ioctl (dataPathObj->dpmHandle,
                         DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG,
                         &commonCfg.objDetPreStartCommonCfg,
                         sizeof (DPC_ObjectDetection_PreStartCommonCfg));
#else
    errCode = DPC_ObjectDetection_ioctl (dataPathObj->dpcHandle,
                         DPC_OBJDET_IOCTL__STATIC_PRE_START_COMMON_CFG,
                         &commonCfg.objDetPreStartCommonCfg,
                         sizeof (DPC_ObjectDetection_PreStartCommonCfg));
#endif
    if (errCode < 0)
    {
        DebugP_log ("Error: CommonCfg: %d\n", errCode);
        ObjectDetectionDDMA_Test_debugAssert (0);
        return;
    }
    else
    {
        DebugP_log ("TEST: CommonCfg Done\n");
    }

#if defined(SOC_AWR2X44P)
#if MSS_AOA_ENABLED
    errCode = DPM_send(dataPathObj->dpmHandle, (void*)(&commonCfg.elevEstCommonCfg), sizeof(DPC_ObjectDetection_ElevEstCommonCfg), CSL_CORE_ID_R5FSS0_0);
#else
    errCode = DPM_send(dataPathObj->dpmHandle, (void*)(&commonCfg.elevEstCommonCfg), sizeof(DPC_ObjectDetection_ElevEstCommonCfg), CSL_CORE_ID_C66SS0);
#endif
    if (errCode < 0)
    {
        DebugP_log ("Error: ElevEstCommonCfg: %d\n", errCode);
        ObjectDetectionDDMA_Test_debugAssert (0);
        return;
    }
    else
    {
        DebugP_log ("TEST: ElevEstCommonCfg Done\n");
    }
#endif

    /* Insted of configuring in normal order, we configure in reverse order
    *  so that subFrame 0 is configured at the last so that EDMA channel parameters
    *  of first subframe remain configured and not overwritten.
    */
    for (subFrameIdx = dataPathObj->commonCfg.numSubFrames-1; subFrameIdx >=0 ; subFrameIdx--)
    {
        /**************************************/
        /* Prepare and send the pre start cfg */
        /**************************************/
        subframeCfg.objDetPreStartCfg.subFrameNum = subFrameIdx;

        /* Fill static configuration */
        subframeCfg.objDetPreStartCfg.staticCfg = dataPathObj->testSubFrmObj[subFrameIdx].staticCfg;

        /* send DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG */
#ifdef INCLUDE_DPM
        errCode = DPM_ioctl (dataPathObj->dpmHandle,
                             DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG,
                             &subframeCfg.objDetPreStartCfg,
                             sizeof (DPC_ObjectDetection_PreStartCfg));
#else
        errCode = DPC_ObjectDetection_ioctl (dataPathObj->dpcHandle,
                        DPC_OBJDET_IOCTL__STATIC_PRE_START_CFG,
                        &subframeCfg.objDetPreStartCfg,
                        sizeof (DPC_ObjectDetection_PreStartCfg));
#endif
        if (errCode < 0)
        {
            DebugP_log ("Error: PreStartCfg: %d\n", errCode);
            ObjectDetectionDDMA_Test_debugAssert (0);
            return;
        }
        else
        {
            DebugP_log ("TEST: PreStartCfg Done subFrame %d\n", subFrameIdx);
        }

#if defined(SOC_AWR2X44P)
#if MSS_AOA_ENABLED
        errCode = DPM_send(dataPathObj->dpmHandle, (void *)&subframeCfg.elevEstSubframeCfg, (uint16_t)sizeof(DPC_ObjectDetection_ElevEstSubframeCfg), CSL_CORE_ID_R5FSS0_0);
#else
        errCode = DPM_send(dataPathObj->dpmHandle, (void *)&subframeCfg.elevEstSubframeCfg, (uint16_t)sizeof(DPC_ObjectDetection_ElevEstSubframeCfg), CSL_CORE_ID_C66SS0);
#endif
        if (errCode < 0)
        {
            DebugP_log ("Error: ElevEstCfg: %d\n", errCode);
            ObjectDetectionDDMA_Test_debugAssert (0);
            return;
        }
        else
        {
            DebugP_log ("TEST: ElevEstCfg Done, subFrame %d\n", subFrameIdx);
        }
#endif
    }
    DebugP_log("\n");
}


/**
  *  @b Description
  *  @n
  *     Invokes DPM_start
  *
  *  @retval None
  *
  */
void ObjectDetectionDDMA_Test_startDPC(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj)
{
    int32_t                         errCode;

#ifdef INCLUDE_DPM
    /* Start the DPM Profile: */
    errCode = DPM_start (dataPathObj->dpmHandle);
#else
    /* start DPC */
    errCode = DPC_ObjectDetection_start (dataPathObj->dpcHandle);
#endif
    if (errCode < 0)
    {
        /* Error: Unable to start the profile */
        DebugP_log ("Error: DPM/DPC Start: %d\n", errCode);
        ObjectDetectionDDMA_Test_debugAssert(0);
    }
}

/**
  *  @b Description
  *  @n
  *         Invokes DPM_stop
  *
  *  @retval    None
  *
  */
void ObjectDetectionDDMA_Test_stopDPC(ObjectDetectionDDMA_Test_DataPathObj *dataPathObj)
{
    int32_t                         errCode;

    /* Start the DPM Profile: */
#ifdef INCLUDE_DPM
    errCode = DPM_stop (dataPathObj->dpmHandle);
#else
    errCode = DPC_ObjectDetection_stop (dataPathObj->dpcHandle);
#endif
    if (errCode < 0)
    {
        /* Error: Unable to start the profile */
        DebugP_log ("Error: Unable to stop the DPM Profile [Error: %d]\n", errCode);
        ObjectDetectionDDMA_Test_debugAssert(0);
    }
}

void ObjectDetectionDDMA_Test_freeDmaChannels(void)
{
    uint32_t   index;
    uint32_t  dmaCh, tcc, pram, shadow;

    for(index = 0; index < 64; index++)
    {
        dmaCh = index;
        tcc = index;
        pram = index;
        shadow = index;

        DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &pram, &shadow);

    }

    for(index = 0; index < 128; index++)
    {
        shadow = index;
        DebugP_assert(EDMA_freeParam(gEdmaHandle[CONFIG_EDMA0], &shadow) == SystemP_SUCCESS);
    }

    return;
}

/**
 *  @b Description
 *  @n
 *      Test task - Main task that performs ObjectDetecion DPC unit test.
 *
 *  @retval
 *      Not Applicable.
 */

void ObjectDetectionDDMA_Test_task(void* args)
{
#ifdef ENABLE_HISTOGRAM_BASED_DOP_AZIM_DETECTION
    DebugP_log("Test vectors not available !\n");
    exit(0);
#endif
    ObjectDetectionDDMA_Test_DataPathObj *dataPathObj = &gDataPathObj;
    char fileName[100];
    uint32_t numTests, testIdx, numObjOutIdeal;
    uint8_t numSubFrames;
    uint8_t readDataAgain;
    uint32_t idx;
    int32_t retVal = 0;
#ifdef INCLUDE_DPM
    DPC_ObjectDetection_ExecuteResultExportedInfo exportInfo;
#else
    DPM_Buffer  resultBuffer;
#endif
    uint8_t subFrameIndx=0;
    uint8_t numFrames, frameIdx;
    uint32_t baseAddr, regionId;
    uint64_t testStartTime, testStopTime;

#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
    char deviceType[] = "awr2944";
    gIsReal =1;
#elif defined(SOC_AWR2943)
    char deviceType[] = "awr2943";
    gIsReal =1;
#else
    #error "Error: Unsupported deviceType"
#endif

    DebugP_log("\n");
    DebugP_log("...... Initialization ...... \n\n");
    memset((void*)dataPathObj, 0, sizeof(ObjectDetectionDDMA_Test_DataPathObj));

    Drivers_open();
    Board_driversOpen();

    baseAddr = EDMA_getBaseAddr(gEdmaHandle[CONFIG_EDMA0]);
    DebugP_assert(baseAddr != 0);

    regionId = EDMA_getRegionId(gEdmaHandle[CONFIG_EDMA0]);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    CycleCounterP_reset();
    testStartTime= ClockP_getTimeUsec();

    /* opening test configuration file */
    strcpy(fileName, "testdata\\objDetDDMA_testVectors_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    testParamsFileId = fopen(fileName, "rb");
    if (testParamsFileId == NULL)
    {
        DebugP_log("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    /* opening ADC data input file */
    strcpy(fileName, "testdata\\objDetDDMA_testAdcDataIn_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    adcDataFileID = fopen(fileName, "rb");
    if (adcDataFileID == NULL)
    {
        DebugP_log("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    /* opening point cloud output file */
    strcpy(fileName, "testdata\\objDetDDMA_testObjOutPointCloud_");
    strcat(fileName, deviceType);
    strcat(fileName, ".bin");
    pointCloudObjFileID = fopen(fileName, "rb");
    if (pointCloudObjFileID == NULL)
    {
        DebugP_log("Error:  Cannot open %s !\n", fileName);
        exit(0);
    }

    fread(&numTests, sizeof(uint32_t), 1, testParamsFileId);

    for(testIdx = 0; testIdx < numTests; testIdx++)
	{
        /* Initialize HWA, EDMA, DPM/DPC */
        ObjectDetectionDDMA_Test_initComponents(dataPathObj);

        DebugP_log("##################################\n");
        DebugP_log("\nTest #%d  start\r\n", testIdx);
        DebugP_log("------------\n");
        fread(&numSubFrames, sizeof(uint8_t), 1, testParamsFileId);
        fread(&numFrames, sizeof(uint8_t), 1, testParamsFileId);
        fread(&antennaCalibParams, sizeof(float), SYS_COMMON_NUM_RX_CHANNEL * SYS_COMMON_NUM_TX_ANTENNAS * 2, testParamsFileId);

        /* config common profile for every test, since numSubFrames can be different in every test */
        ObjectDetectionDDMA_Test_setCommonProfile(dataPathObj, numSubFrames);
        dataPathObj->commonCfg.maxAdcSamples = 0U;
        for(subFrameIndx =0; subFrameIndx< numSubFrames; subFrameIndx++)
        {
            /* read in test config */
            fread(&gtestStaticCfg[subFrameIndx].numTxAntennas, sizeof(uint32_t), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].numRxAntennas, sizeof(uint32_t), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].numAdcSamples, sizeof(uint32_t), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].numChirps, sizeof(uint32_t), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].compMethod, sizeof(uint8_t), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].compRatio, sizeof(float), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].compRangeBinsPerBlock, sizeof(uint16_t), 1, testParamsFileId);
            fread(&readDataAgain, sizeof(uint8_t), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].isRangeCFARenabled, sizeof(uint8_t), 1, testParamsFileId);
            fread(&dataPathObj->commonCfg.rangeProcCfg.rangeProcChain, sizeof(uint16_t), 1, testParamsFileId);
            fread(&dataPathObj->commonCfg.rangeProcCfg.intfMitigMagThresMinLim, sizeof(uint16_t), 1, testParamsFileId);
            fread(&dataPathObj->commonCfg.rangeProcCfg.intfMitigMagDiffThresMinLim, sizeof(uint16_t), 1, testParamsFileId);
            fread(&dataPathObj->commonCfg.rangeProcCfg.isReal2XEnabled, sizeof(uint16_t), 1, testParamsFileId);
            fread(&dataPathObj->commonCfg.antennaSpacing.xSpacingByLambda, sizeof(float), 1, testParamsFileId);
            fread(&dataPathObj->commonCfg.antennaSpacing.zSpacingByLambda, sizeof(float), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].rangeStep, sizeof(float), 1, testParamsFileId);
            fread(&gtestStaticCfg[subFrameIndx].dopplerStep, sizeof(float), 1, testParamsFileId);
            if(dataPathObj->commonCfg.maxAdcSamples < gtestStaticCfg[subFrameIndx].numAdcSamples)
            {
                dataPathObj->commonCfg.maxAdcSamples = gtestStaticCfg[subFrameIndx].numAdcSamples;
            }

            DebugP_log("\nSubFrame #%d\r\n", subFrameIndx);
            DebugP_log("------------\n");
            DebugP_log("NumRx %d, NumTx %d\n", gtestStaticCfg[subFrameIndx].numRxAntennas, gtestStaticCfg[subFrameIndx].numTxAntennas);
            DebugP_log("NumADCSamples %d, NumChirps %d\n", gtestStaticCfg[subFrameIndx].numAdcSamples, gtestStaticCfg[subFrameIndx].numChirps);
            DebugP_log("RangeProcChain %d, Real2XEnable %d\n", gDataPathObj.commonCfg.rangeProcCfg.rangeProcChain, gDataPathObj.commonCfg.rangeProcCfg.isReal2XEnabled);
            DebugP_log("Compression Method %d, Compression Ratio %.2f, RangeBinsPerBlock %d\n", gtestStaticCfg[subFrameIndx].compMethod, gtestStaticCfg[subFrameIndx].compRatio, gtestStaticCfg[subFrameIndx].compRangeBinsPerBlock);
            DebugP_log("isChirpDataReal %d, isRangeCFARenabled %d\n",gIsReal, gtestStaticCfg[subFrameIndx].isRangeCFARenabled);
            DebugP_log("------------\n\n");

            /* config datapathObj from testStaticCfg */
            ObjectDetectionDDMA_Test_setProfile(dataPathObj,
                                    &dataPathObj->testSubFrmObj[subFrameIndx], subFrameIndx);
        }

        /* config DPC from dataPathObj */
        ObjectDetectionDDMA_Test_configDPC(dataPathObj);

        gNumSubFrames = numSubFrames;
        gSubFrameIndx = 0;
        gFrameIdx = 0;
        gNumFrames = numFrames;
        /* start DPC */
        ObjectDetectionDDMA_Test_startDPC(dataPathObj);

        for (frameIdx = 0; frameIdx < numFrames; frameIdx++)
        {

            for(subFrameIndx =0; subFrameIndx< numSubFrames; subFrameIndx++){

                DebugP_log("Processing subFrame: %d for frame: %d\n", subFrameIndx, frameIdx);
                /* Simulate the trigger of frame start event for DPC */
                ObjectDetectionDDMA_Test_trigFrameStart(dataPathObj);

                for(idx=0; idx< gtestStaticCfg[subFrameIndx].numChirps; idx++)
                {
                    if((gDataPathObj.commonCfg.rangeProcCfg.rangeProcChain == DPU_RANGEPROCHWA_DEFAULT_MODE)
                        || (gDataPathObj.commonCfg.rangeProcCfg.rangeProcChain == DPU_RANGEPROCHWA_PREVIOUS_FRAME_DC_MODE))
                    {
                        while(gChirpIdx == idx)
                            ;
                        EDMA_enableTransferRegion(baseAddr, regionId, DPC_OBJDET_DPU_RANGEPROC_EDMAIN_CH, EDMA_TRIG_MODE_MANUAL);
                    }
                } /* end of chirp loop */

                DebugP_log("executing...\n");
#ifdef INCLUDE_DPM
                SemaphoreP_pend(&dataPathObj->semaphoreHandle, SystemP_WAIT_FOREVER);
#else
                DPC_ObjectDetection_execute(dataPathObj->dpcHandle, &resultBuffer);

                uint32_t msg = MMWDEMO_DPC_RESULT;
#if defined(SOC_AWR2X44P)
#if MSS_AOA_ENABLED
                retVal = DPM_send(dataPathObj->dpmHandle, (void*)(&msg), sizeof(uint32_t), CSL_CORE_ID_R5FSS0_0);
#else
                retVal = DPM_send(dataPathObj->dpmHandle, (void*)(&msg), sizeof(uint32_t), CSL_CORE_ID_C66SS0);
#endif
#endif
                DebugP_assert(0U == retVal);

                SemaphoreP_pend(&dataPathObj->semaphoreHandle, SystemP_WAIT_FOREVER);

                dataPathObj->testSubFrmObj[subFrameIndx].result = (DPC_ObjectDetection_ExecuteResult *)resultBuffer.ptrBuffer[0];

#endif
                fread(&numObjOutIdeal, sizeof(uint32_t), 1, pointCloudObjFileID);
                fread(&gPointCloudObjIdealResult, sizeof(DPIF_PointCloudCartesian), numObjOutIdeal, pointCloudObjFileID);
                retVal = ObjectDetectionDDMA_Test_ComparePointCloudObjsToRef(dataPathObj->testSubFrmObj[subFrameIndx].result->objOut, &gPointCloudObjIdealResult[0], dataPathObj->testSubFrmObj[subFrameIndx].result->numObjOut, numObjOutIdeal);
                if(retVal < 0){
                    DebugP_log("Error in compareResults!\n");
                }

#ifdef INCLUDE_DPM
                /* indicate result consumed and end of frame/sub-frame processing */
                exportInfo.subFrameIdx = dataPathObj->testSubFrmObj[subFrameIndx].result->subFrameIdx;

                retVal = DPM_ioctl(dataPathObj->dpmHandle,
                    DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED,
                    &exportInfo,
                    sizeof(DPC_ObjectDetection_ExecuteResultExportedInfo));
                if (retVal < 0) {
                    DebugP_log("Error: DPM DPC_OBJDET_IOCTL__DYNAMIC_EXECUTE_RESULT_EXPORTED failed [Error code %d]\n",
                        retVal);
                    ObjectDetectionDDMA_Test_debugAssert(0);
                }
#endif
            }/* end of subframe loop */
        } /* end of frame loop */

        /* stop DPC */
        ObjectDetectionDDMA_Test_stopDPC(dataPathObj);
        /* Deinitialize HWA, EDMA, DPM/DPC */
        ObjectDetectionDDMA_Test_deinitComponents(dataPathObj);
        DebugP_log("Test %d Completed!\n", testIdx);
        DebugP_log("----------------------------\n\n");

    } /* end of test while loop */

#ifdef INCLUDE_DPM
    DebugP_log("DPC ObjDet Maximum Memory usage: L3: %d Bytes (Test #%d), CoreLocalRam: %d Bytes (Test #%d)\n",
                gMaxL3RamUsage,
                gMaxL3RamUsageTestId,
                gMaxCoreLocalRamUsage,
                gMaxCoreLocalRamUsageTestId);
#endif

    DebugP_log("All %d Tests finished!\n\r", testIdx);
    testStopTime= ClockP_getTimeUsec();
    DebugP_log("Test Time Cycle : %lld \n", testStopTime-testStartTime);
    /* close file */
    fclose(testParamsFileId);
    fclose(adcDataFileID);
    fclose(pointCloudObjFileID);

    Board_driversClose();
    Drivers_close();
    if(finalResults)
    {
        DebugP_log ("All tests have passed!!\n");
    }
    else
    {
        DebugP_log ("Some tests have not passed!!\n");
    }
    DebugP_log ("--- Test Completed ---\n");

    while(1);
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

#ifdef SUBSYS_M4
    void *args = NULL;
    ObjectDetectionDDMA_Test_task(args);
#else
    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( ObjectDetectionDDMA_Test_task,
                                  "test_task_main",
                                  APP_TASK_STACK_SIZE,
                                  NULL,
                                  APP_TASK_PRI,
                                  gAppTskStackMain,
                                  &gAppTaskObj );

    configASSERT(gAppTask != NULL);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    /* The following line should never be reached because vTaskStartScheduler()
    will only return if there was not enough FreeRTOS heap memory available to
    create the Idle and (if configured) Timer tasks.  Heap management, and
    techniques for trapping heap exhaustion, are described in the book text. */
    DebugP_assertNoLog(0);
#endif
}
