/**
 *   @file  aoaprochwa_test_main.c
 *
 *   @brief
 *      Unit Test code for aoa processing.
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
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/* MCU Plus Include Files. */
#include <drivers/edma.h>
#ifdef SUBSYS_DSS
#include <ti/datapath/dpu/aoaproc/test/dssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/aoaproc/test/dssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/aoaproc/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/aoaproc/test/dssgenerated/ti_board_open_close.h>
#endif

#ifdef SUBSYS_MSS
#include <ti/datapath/dpu/aoaproc/test/mssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/aoaproc/test/mssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/aoaproc/test/mssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/aoaproc/test/mssgenerated/ti_board_open_close.h>
#endif
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmwave SDK include files */
#include <ti/utils/testlogger/logger.h>
#include <ti/datapath/dpu/aoaproc/aoaprochwa.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/utils/randomdatagenerator/gen_rand_data.h>

#define TEST_MAX_NUM_DOPPLER_BINS 512
#define TEST_MAX_NUM_RANGE_BIN  1024
#define TEST_MAX_RADAR_CUBE_SIZE  (64 * 1024 * 4)  //numchirps * numRange * numRx complex value
#define TEST_AOA_FFTSIZE 64         /* 3D fft size */
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
#define MAX_NUM_TX_ANTENNAS 4
#define MAX_NUM_AZIM_TX_ANTENNAS 3
#define ELEV_SCALE_FACTOR 1.6 /* By virtue of the fact that the elevation antenna is offset at 0.8 * lambda */
#endif
#ifdef SOC_AWR2943
#define MAX_NUM_TX_ANTENNAS 3
#define MAX_NUM_AZIM_TX_ANTENNAS 3
#define ELEV_SCALE_FACTOR 1.6 /* By virtue of the fact that the elevation antenna is offset at 0.8 * lambda */
#endif
#define MAX_NUM_RX_ANTENNAS 4

#ifdef SUBSYS_MSS
#define TEST_MAX_NUM_CFARDETOBJ_DPU_LIST 276
#define TEST_MAX_NUM_CFARDETOBJ_DPU  276  //for Tx=2, >=256, for Tx=3, >=64
#define TEST_MAX_NUM_AOAPOINTCLOUD_LIST 276
#define TEST_MAX_NUM_PEAKS 2
#endif
#ifdef SUBSYS_DSS
#define  TEST_MAX_NUM_CFARDETOBJ_DPU_LIST   6    //must be even
#define TEST_MAX_NUM_CFARDETOBJ_DPU  276
#define TEST_MAX_NUM_AOAPOINTCLOUD_LIST  5
#define TEST_MAX_NUM_PEAKS 1
#endif

#define TEST_MAX_NUM_CFARDETOBJ 5       //except for morepeak test, 3 tx 64, 2 tx 256
#define TEST_MAX_NUM_AZIMUTH_VIRTUAL_ANTENNAS (MAX_NUM_AZIM_TX_ANTENNAS * MAX_NUM_RX_ANTENNAS)
#define TEST_MAX_NUM_VIRTUAL_ANTENNAS (MAX_NUM_TX_ANTENNAS * MAX_NUM_RX_ANTENNAS)


#define MAX_PEAKIDX_ERROR 4
#define MAX_ELEVANGLE_ERROR 0.076f
#define MAX_LOCATION_ERROR 0.01f
#define MAX_HEATMAP_ERROR 2
#define TWOPEAK_POSITIVE_THRESHOLD 0.65f
#define TWOPEAK_NEGTIVE_THRESHOLD 0.95f
#define CUBADATA_PHASE_SHIFT 8.f
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
#define TEST_ANTENNA_LOOP 7   /* number of tx/rx combination */
#else
#define TEST_ANTENNA_LOOP 5   /* number of tx/rx combination */
#endif
#define TEST_DOPPLER_BIN_SIZE 9 // test doppler size covers from 2 to 512, tests for doppler bin = 1024 is added at the end */
#define TEST_RANGE_BIN_SIZE 7   //tested range bin size from 16 to 1024

#ifdef SUBSYS_MSS
#define TEST_AOA_DEBUG 1 /* Debug support for 4 Tx is not applicable as of yet */
#else
#define TEST_AOA_DEBUG 0 //for dsp, always disabled
#endif
#if TEST_AOA_DEBUG
uint32_t maxCubeSize = 0;
uint32_t testCaseId = 0;
#define TEST_AOA_DEBUG_RADARCUBEDATA 0 //print out the radar cube data
#define TEST_AOA_DEBUG_3DFFTIN 0
#define TEST_AOA_DEBUG_3DFFTOUT 0
#endif

#define MAX_NUM_AOA_HYPHOTHESES (MAX_NUM_TX_ANTENNAS)

#define PI 3.141592653589793

/***************** radar cube data ******************/
#ifdef SUBSYS_MSS
/* Note the AoA DPU does not require cache alignment, this requirement is for the test
 * because the test code uses CPU access to fill test data in the radar cube which
 * needs to be written back before AoA DPU processing */
#define TEST_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT   CSL_CACHE_L1D_LINESIZE
#else
#define TEST_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT   CSL_CACHE_L1D_LINESIZE
#endif

/* msb is real, lsb 16 is imag */
uint8_t gTestRadarCubeBuf[CSL_NEXT_MULTIPLE_OF(TEST_MAX_RADAR_CUBE_SIZE * sizeof(cmplx16ImRe_t), \
                                           TEST_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(TEST_RADAR_CUBE_DATABUF_BYTE_ALIGNMENT), section(".l3ram")));
cmplx16ImRe_t *gTestRadarCube = (cmplx16ImRe_t *) &gTestRadarCubeBuf[0];

/*********** Scratch Buffers ***************************************/
#ifdef SUBSYS_MSS
#define TEST_DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT_DSP
#endif

#define SCRATCH_SIZE_BYTES \
        CSL_NEXT_MULTIPLE_OF(MAX_NUM_AOA_HYPHOTHESES * DPU_AOAPROCHWA_NUM_ANGLE_BINS * sizeof(uint16_t), \
                             TEST_DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT)

uint8_t gScratchPingBuf[SCRATCH_SIZE_BYTES] \
        __attribute__ ((aligned(TEST_DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT), section(".coreLocalRam")));

uint8_t gScratchPongBuf[SCRATCH_SIZE_BYTES] \
        __attribute__ ((aligned(TEST_DPU_AOAPROCHWA_LOCAL_SCRATCH_BYTE_ALIGNMENT), section(".coreLocalRam")));

/********************** cfar det obj *****************************/

#ifdef SUBSYS_MSS
#define TEST_DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_DSP
#endif

uint8_t gCfarDetObjListBuf[CSL_NEXT_MULTIPLE_OF(TEST_MAX_NUM_CFARDETOBJ_DPU_LIST * sizeof(DPIF_CFARDetList), \
                                            TEST_DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(TEST_DPU_AOAPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT), section(".coreLocalRam")));
DPIF_CFARDetList * pCfarDetObjList;

/******************** 2D FFT windowing coefficients ********************/
#ifdef SUBSYS_MSS
int32_t gTestDopplerWindow[TEST_MAX_NUM_DOPPLER_BINS * 2]__attribute__((section(".coreLocalRam")));
#endif
#ifdef SUBSYS_DSS
/* if -o3 is disabled, memory not fit */
int32_t gTestDopplerWindow[TEST_MAX_NUM_DOPPLER_BINS]__attribute__((section(".coreLocalRam")));
#endif
int32_t * pTestDopplerWindow;

/********************* AOA DPU detected objects ***********************/
#ifdef SUBSYS_MSS
#define TEST_DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT_DSP
#endif

uint8_t gAoaDetObjBuf[CSL_NEXT_MULTIPLE_OF(TEST_MAX_NUM_AOAPOINTCLOUD_LIST * TEST_MAX_NUM_PEAKS \
                                       * sizeof(DPIF_PointCloudCartesian), \
                                       TEST_DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT)]
        __attribute__ ((aligned(TEST_DPU_AOAPROCHWA_POINT_CLOUD_CARTESIAN_BYTE_ALIGNMENT), section(".coreLocalRam")));
DPIF_PointCloudCartesian *pAoaDetObj;

/******************** AOA DPU output side info ************************/
#ifdef SUBSYS_MSS
#define TEST_DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT_DSP
#endif

uint8_t gAoaDetObjSideInfoBuf[CSL_NEXT_MULTIPLE_OF(TEST_MAX_NUM_CFARDETOBJ_DPU * TEST_MAX_NUM_PEAKS * \
                                               sizeof(DPIF_PointCloudSideInfo), \
                                               TEST_DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(TEST_DPU_AOAPROCHWA_POINT_CLOUD_SIDE_INFO_BYTE_ALIGNMENT), section(".coreLocalRam")));

/******************** AOA DPU heat map output *************************/
#ifdef SUBSYS_MSS
#define TEST_DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT_DSP
#endif

uint8_t gStaticHeatmapBuf[CSL_NEXT_MULTIPLE_OF(TEST_MAX_NUM_RANGE_BIN * \
                                           TEST_MAX_NUM_AZIMUTH_VIRTUAL_ANTENNAS * sizeof(cmplx16ImRe_t), \
                                           TEST_DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(TEST_DPU_AOAPROCHWA_AZIMUTH_STATIC_HEAT_MAP_BYTE_ALIGNMENT), section(".coreLocalRam")));
cmplx16ImRe_t *gStaticHeatmap = (cmplx16ImRe_t *) &gStaticHeatmapBuf[0];

/* heat map references */
/* maximum azimvirtual antenna is 2*4 */
cmplx16ImRe_t gStaticHeatmapRef[TEST_MAX_NUM_RANGE_BIN * TEST_MAX_NUM_AZIMUTH_VIRTUAL_ANTENNAS]__attribute__((aligned(64), section(".coreLocalRam")));

/******************* AOA DPU detected Azim peak index ******************/
#ifdef SUBSYS_MSS
#define TEST_DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT_DSP
#endif

uint8_t gAoaDetObjAzimIdx[CSL_NEXT_MULTIPLE_OF(TEST_MAX_NUM_CFARDETOBJ_DPU * TEST_MAX_NUM_PEAKS, \
                                              TEST_DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(TEST_DPU_AOAPROCHWA_DET_OBJ2_AZIM_IDX_BYTE_ALIGNMENT), section(".coreLocalRam")));

/******************* AOA DPU detects Elev angle ************************/
#ifdef SUBSYS_MSS
#define TEST_DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT \
             DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT_DSP
#endif

uint8_t gAoaDetObjElevAngleBuf[CSL_NEXT_MULTIPLE_OF(TEST_MAX_NUM_CFARDETOBJ_DPU * \
                                                TEST_MAX_NUM_PEAKS * sizeof(float), \
                                                TEST_DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(TEST_DPU_AOAPROCHWA_DET_OBJ_ELEVATION_ANGLE_BYTE_ALIGNMENT), section(".coreLocalRam")));

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (5U)
#define APP_TASK_STACK_SIZE  (8 * 1024u)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(CSL_CACHE_L1D_LINESIZE)));


/******************* EDMA resource partitioning **********************/
/* HWA can only be accessed through DSS*/
#define EDMA_NUM_DMA_CHANNELS                              64

#define EDMA_SHADOW_LNK_PARAM_BASE_ID                      EDMA_NUM_DMA_CHANNELS
#define DPU_AOA_PROC_EDMAIN_CH_PING                        EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ6
#define DPU_AOA_PROC_EDMAIN_CH_PONG                        EDMA_DSS_TPCC_A_EVT_DFE_FRAME_START_TO_DSS
#define DPU_AOA_PROC_EDMAIN_SHADOW_PING                    (EDMA_SHADOW_LNK_PARAM_BASE_ID + 0U)
#define DPU_AOA_PROC_EDMAIN_SHADOW_PONG                    (EDMA_SHADOW_LNK_PARAM_BASE_ID + 1U)
#define DPU_AOA_PROC_EDMAIN_EVENT_QUE                      0
#define DPU_AOA_PROC_EDMAIN_SIG_CH_PING                    EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ0
#define DPU_AOA_PROC_EDMAIN_SIG_CH_PONG                    EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ1
#define DPU_AOA_PROC_EDMAIN_SIG_SHADOW_PING                (EDMA_SHADOW_LNK_PARAM_BASE_ID + 2U)
#define DPU_AOA_PROC_EDMAIN_SIG_SHADOW_PONG                (EDMA_SHADOW_LNK_PARAM_BASE_ID + 3U)
#define DPU_AOA_PROC_EDMAIN_SIG_EVENT_QUE                  0
#define DPU_AOA_PROC_EDMAOUT_CH_PING                       EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ2
#define DPU_AOA_PROC_EDMAOUT_CH_PONG                       EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ3
#define DPU_AOA_PROC_EDMAOUT_SHADOW_PING                   (EDMA_SHADOW_LNK_PARAM_BASE_ID + 4U)
#define DPU_AOA_PROC_EDMAOUT_SHADOW_PONG                   (EDMA_SHADOW_LNK_PARAM_BASE_ID + 5U)
#define DPU_AOA_PROC_EDMAOUT_EVENT_QUE                     0

/* EDMA general physical channels */
#define DPC_OBJDET_DPU_AOA_PROC_EDMAIN_PING_EVENT_QUE                 0
#define DPC_OBJDET_DPU_AOA_PROC_EDMAOUT_PING_EVENT_QUE                0
/* Event queues PONG path */
#define DPC_OBJDET_DPU_AOA_PROC_EDMAIN_PONG_EVENT_QUE                 0
#define DPC_OBJDET_DPU_AOA_PROC_EDMAOUT_PONG_EVENT_QUE                0

/* EDMA physical channels tied to HWA output events */
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_HWA_OUTPUT_CH_0                  EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ4
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_HWA_OUTPUT_CH_1                  EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ5

#define DPC_OBJDET_EDMA_SHADOW_BASE                                   EDMA_NUM_DMA_CHANNELS

#define DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_0                             EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ2
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_1                             EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ3
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_2                             EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ4
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_3                             EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ5

/* EDMA Param sets */
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_0                        (DPC_OBJDET_EDMA_SHADOW_BASE + 27)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_1                        (DPC_OBJDET_EDMA_SHADOW_BASE + 28)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_2                        (DPC_OBJDET_EDMA_SHADOW_BASE + 29)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_3                        (DPC_OBJDET_EDMA_SHADOW_BASE + 30)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_4                        (DPC_OBJDET_EDMA_SHADOW_BASE + 31)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_5                        (DPC_OBJDET_EDMA_SHADOW_BASE + 32)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_6                        (DPC_OBJDET_EDMA_SHADOW_BASE + 33)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_7                        (DPC_OBJDET_EDMA_SHADOW_BASE + 34)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_8                        (DPC_OBJDET_EDMA_SHADOW_BASE + 35)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_9                        (DPC_OBJDET_EDMA_SHADOW_BASE + 36)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_10                       (DPC_OBJDET_EDMA_SHADOW_BASE + 37)
#define DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_11                       (DPC_OBJDET_EDMA_SHADOW_BASE + 38)

#if TEST_AOA_DEBUG    // DEBUG FOR MSS only

uint32_t fftDataInBuf[TEST_MAX_NUM_DOPPLER_BINS * 2]__attribute__((aligned(64), section(".coreLocalRam")));

int32_t fftDataWindow[TEST_MAX_NUM_DOPPLER_BINS * 4]__attribute__((aligned(64), section(".coreLocalRam")));

int32_t fftDataOutBuf[TEST_MAX_NUM_DOPPLER_BINS * 4]__attribute__((aligned(64), section(".coreLocalRam")));

int32_t fft3dDataInBuf[TEST_MAX_NUM_VIRTUAL_ANTENNAS * 2]__attribute__((aligned(64), section(".coreLocalRam")));

/* first TEST_AOA_FFTSIZE azim fft out, next elev fft out */
int32_t fft3dDataOutBuf[TEST_AOA_FFTSIZE * 4]__attribute__((aligned(64), section(".coreLocalRam")));
#endif

/**
  * @brief
  * AOA DPU unit test test configuration structure.
  *
*/
typedef struct aoaProcDpuTestConfig_t {
    uint16_t numTxAntennas;     /*!< @brief number of Tx antenna */
    uint16_t numTxAzimAntennas; /*!< @brief number of Azimuth Tx antenna */
    uint16_t numTxElevAntennas; /*!< @brief number of Elevation Tx antenna */
    uint16_t numRxAntennas;     /*!< @brief number of Rx antenna */
    uint16_t numRangeBins;      /*!< @brief number of range bins */
    uint16_t numChirps;         /*!< @brief number of chirps, <= numDopplerBins */
    uint16_t numDopplerBins;    /*!< @brief number of doppler bins */
    uint16_t numCfarDetObj;     /*!< @brief number of cfar detected obj */
    uint16_t numAoaOutObj;      /*!< @brief number of AOA output reference */
    uint16_t cfarDetObjRangeIdx[TEST_MAX_NUM_CFARDETOBJ];         /*!< @brief range index for CFAR det objects */
    uint16_t cfarDetObjdopplerIdx[TEST_MAX_NUM_CFARDETOBJ];       /*!< @brief doppler index for CFAR det objects */
    int16_t  cfarDetObjDopplerRefIdx[TEST_MAX_NUM_CFARDETOBJ];    /*!< @brief signed doppler index for CFAR det objects */
    uint8_t  twoPeaksFlag;     /*!< @brief if 0, single peak angle for azim, if 1 2 peak angles for azim */
    uint8_t  heatMapFlag;      /*!< @brief turn on/off heat map checking in the tests, always ON for numRx > 1 */
    uint8_t  minmaxAngleFlag;  /*!< @brief if 0, min/max azim/elev angles are set to +/- 80 degree, if 1, they are set to
                               min/max azim/elev based on the tests */
    int16_t minAzim;          /*!<  @brief minimun azim index in the test */
    int16_t maxAzim;          /*!<  @brief maximum azim index in the test */
    int16_t minElev, maxElev; /*!<  @brief minimum/maximum elev index in the test */

    uint8_t checkCfarDetObjFlag[TEST_MAX_NUM_CFARDETOBJ];   /*!<  @brief The cfar det object is detected or
                                                            not detected in aoa dpu*/

    uint8_t  aoaNum3DfftPeaks[TEST_MAX_NUM_CFARDETOBJ];   /*!<  @brief number of azim peaks for every cfar det objects */

   /*!<  @brief azim peak index reference for every cfar det objects */
    int16_t  aoaSignedAzimPeakIdxRef[TEST_MAX_NUM_CFARDETOBJ * TEST_MAX_NUM_PEAKS];
    /*!<  @brief elev peak index reference for every cfar det objects */
    int16_t  aoaSignedElevPeakIdxRef[TEST_MAX_NUM_CFARDETOBJ * TEST_MAX_NUM_PEAKS];

    uint8_t winType;   /*!<  @brief if 0, rectange windowing, if 1, hanning windowing */
    uint8_t morePeaksFlag;  /*!<  @brief if 1, generates more than 64 peaks if tx=3, and more than 256 peaks if tx=2
                                 if 0, maximum cfar det objects is defined as TEST_MAX_NUM_CFARDETOBJ*/
    uint8_t extVelocityFlag;  /*!<  @brief extension velocity flag, 0- extension velocity is disabled, 1 - enabled*/

    uint8_t staticClutterRemovalFlag; /*!<  @brief static clutter removal flag, 0 - disabled, 1 - enabled*/
} aoaProcDpuTestConfig_t;

HWA_Handle  hwaHandle;
volatile uint8_t    subframeCount = 0;

MCPI_LOGBUF_INIT(128);

uint8_t finalResults;

DPU_AoAProcHWA_Config        aoaCfg;
DPU_AoAProcHWA_Handle        aoaProcHandle;
DPU_AoAProc_FovAoaCfg        aoaDynFovCfg;
DPU_AoAProc_compRxChannelBiasCfg aoaDynOutCompRxCfg;
DPU_AoAProc_MultiObjBeamFormingCfg aoaDynBmCfg;
DPU_AoAProcHWA_OutParams outAoaProc;
DPU_AoAProc_ExtendedMaxVelocityCfg  extMaxVelCfg;

aoaProcDpuTestConfig_t    aoaTestCfg;
uint32_t testCount;

/**
*  @b Description
*  @n
*  calculate modula value
*
*  @retval None
*
*/
uint16_t test_modCalc(uint16_t value1, uint16_t value2)
{
    uint16_t result;
    result = value1 - value1 / value2 * value2;
    return result;
}

/**
*  @b Description
*  @n
*    Initialize the HWA.
*
*  @retval None
*
*/
void aoaProc_test_hwainit()
{
    int32_t             errorCode;

    hwaHandle = HWA_open(0, NULL, &errorCode);
    if (hwaHandle == NULL)
    {
        test_print("HWA failed to open\n");
        DebugP_assert(0);
        return;
    }
}

/**
*  @b Description
*  @n
*   Initialize the AOA DPU, Generates the AOA DPU handler.
*
*  @retval None
*
*/
void aoaProc_test_dpuInit()
{
    DPU_AoAProcHWA_InitParams   aoaInitParams;
    int32_t errorCode = 0;

    aoaInitParams.hwaHandle = hwaHandle;

    aoaProcHandle = DPU_AoAProcHWA_init(&aoaInitParams, subframeCount, &errorCode);
    if (aoaProcHandle == NULL)
    {
        test_print("AOA DPU failed to initialize. Error %d.\n", errorCode);
        return;
    }
}


/**
 *  @b Description
 *  @n
 *      Allocates Shawdow paramset
 */
static void allocateEDMAParam(uint32_t *param)
{
    int32_t             testStatus = SystemP_SUCCESS;

    testStatus = EDMA_allocParam(gEdmaHandle[CONFIG_EDMA0], param);
    DebugP_assert(testStatus == SystemP_SUCCESS);

    return;
}

/**
*  @b Description
*  @n
*   Initialize the AOA DPU configuration structure for the parameters NOT change for every test.
*
*  @retval None
*
*/
void aoaProc_test_dpuConfig()
{
    DPU_AoAProcHWA_HW_Resources   * resPtr;
    DPU_AoAProcHWA_hwaAoAConfig  *aoaHwaCfgPtr;
    uint16_t ii;
    float realTemp, imagTemp;
    uint32_t dmaCh, tcc, param;

    memset(&aoaCfg, 0, sizeof(aoaCfg));

    resPtr = &aoaCfg.res;

    resPtr->edmaHandle = gEdmaHandle[CONFIG_EDMA0];

    /* For Azimuth Heatmap ping/pong paths */
    dmaCh = DPU_AOA_PROC_EDMAIN_CH_PING;
    tcc   = DPU_AOA_PROC_EDMAIN_CH_PING;
    param = DPU_AOA_PROC_EDMAIN_CH_PING;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwa[0].in.channel = dmaCh;
    resPtr->edmaHwa[0].in.paramId = param;
    resPtr->edmaHwa[0].in.tcc     = tcc;

    param = DPU_AOA_PROC_EDMAIN_SHADOW_PING;
    allocateEDMAParam(&param);
    resPtr->edmaHwa[0].in.shadowPramId = param;
    resPtr->edmaHwa[0].in.eventQueue = DPU_AOA_PROC_EDMAIN_EVENT_QUE;

    dmaCh = DPU_AOA_PROC_EDMAIN_SIG_CH_PING;
    tcc   = DPU_AOA_PROC_EDMAIN_SIG_CH_PING;
    param = DPU_AOA_PROC_EDMAIN_SIG_CH_PING;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwa[0].inSignature.channel = dmaCh;
    resPtr->edmaHwa[0].inSignature.paramId = param;
    resPtr->edmaHwa[0].inSignature.tcc     = tcc;

    param = DPU_AOA_PROC_EDMAIN_SIG_SHADOW_PING;
    allocateEDMAParam(&param);
    resPtr->edmaHwa[0].inSignature.shadowPramId = param;
    resPtr->edmaHwa[0].inSignature.eventQueue = DPU_AOA_PROC_EDMAIN_SIG_EVENT_QUE;

    dmaCh = DPU_AOA_PROC_EDMAOUT_CH_PING;
    tcc   = DPU_AOA_PROC_EDMAOUT_CH_PING;
    param = DPU_AOA_PROC_EDMAOUT_CH_PING;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwa[0].out.channel = dmaCh;
    resPtr->edmaHwa[0].out.paramId = param;
    resPtr->edmaHwa[0].out.tcc     = tcc;

    param = DPU_AOA_PROC_EDMAOUT_SHADOW_PING;
    allocateEDMAParam(&param);
    resPtr->edmaHwa[0].out.shadowPramId = param;
    resPtr->edmaHwa[0].out.eventQueue = DPU_AOA_PROC_EDMAOUT_EVENT_QUE;

    dmaCh = DPU_AOA_PROC_EDMAIN_CH_PONG;
    tcc   = DPU_AOA_PROC_EDMAIN_CH_PONG;
    param = DPU_AOA_PROC_EDMAIN_CH_PONG;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwa[1].in.channel = dmaCh;
    resPtr->edmaHwa[1].in.paramId = param;
    resPtr->edmaHwa[1].in.tcc     = tcc;

    param = DPU_AOA_PROC_EDMAIN_SHADOW_PONG;
    allocateEDMAParam(&param);
    resPtr->edmaHwa[1].in.shadowPramId = param;
    resPtr->edmaHwa[1].in.eventQueue = DPU_AOA_PROC_EDMAIN_EVENT_QUE;

    dmaCh = DPU_AOA_PROC_EDMAIN_SIG_CH_PONG;
    tcc   = DPU_AOA_PROC_EDMAIN_SIG_CH_PONG;
    param = DPU_AOA_PROC_EDMAIN_SIG_CH_PONG;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwa[1].inSignature.channel = dmaCh;
    resPtr->edmaHwa[1].inSignature.paramId = param;
    resPtr->edmaHwa[1].inSignature.tcc     = tcc;

    param = DPU_AOA_PROC_EDMAIN_SIG_SHADOW_PONG;
    allocateEDMAParam(&param);
    resPtr->edmaHwa[1].inSignature.shadowPramId = param;
    resPtr->edmaHwa[1].inSignature.eventQueue = DPU_AOA_PROC_EDMAIN_SIG_EVENT_QUE;

    dmaCh = DPU_AOA_PROC_EDMAOUT_CH_PONG;
    tcc   = DPU_AOA_PROC_EDMAOUT_CH_PONG;
    param = DPU_AOA_PROC_EDMAOUT_CH_PONG;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwa[1].out.channel = dmaCh;
    resPtr->edmaHwa[1].out.paramId = param;
    resPtr->edmaHwa[1].out.tcc     = tcc;

    param = DPU_AOA_PROC_EDMAOUT_SHADOW_PONG;
    allocateEDMAParam(&param);
    resPtr->edmaHwa[1].out.shadowPramId = param;
    resPtr->edmaHwa[1].out.eventQueue = DPU_AOA_PROC_EDMAOUT_EVENT_QUE;

    /* For main data processing ping/pong paths */
    dmaCh = DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_0;
    tcc   = DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_0;
    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_0;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwaExt[0].chIn.channel =               dmaCh;
    resPtr->edmaHwaExt[0].chIn.eventQueue =            DPC_OBJDET_DPU_AOA_PROC_EDMAIN_PING_EVENT_QUE;

    dmaCh = DPC_OBJDET_DPU_AOA_PROC_EDMA_HWA_OUTPUT_CH_0;
    tcc   = DPC_OBJDET_DPU_AOA_PROC_EDMA_HWA_OUTPUT_CH_0;
    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_HWA_OUTPUT_CH_0;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwaExt[0].chOut.channel =              dmaCh;
    resPtr->edmaHwaExt[0].chOut.eventQueue =           DPC_OBJDET_DPU_AOA_PROC_EDMAOUT_PING_EVENT_QUE;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_0;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[0].stage[0].paramIn =           param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_1;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[0].stage[0].paramInSignature =  param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_2;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[0].stage[0].paramOut =          param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_3;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[0].stage[1].paramIn =           param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_4;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[0].stage[1].paramInSignature =  param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_5;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[0].stage[1].paramOut =          param;
    resPtr->edmaHwaExt[0].eventQueue = 0;

    dmaCh = DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_1;
    tcc   = DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_1;
    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_CH_1;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwaExt[1].chIn.channel =               dmaCh;
    resPtr->edmaHwaExt[1].chIn.eventQueue =            DPC_OBJDET_DPU_AOA_PROC_EDMAIN_PONG_EVENT_QUE;

    dmaCh = DPC_OBJDET_DPU_AOA_PROC_EDMA_HWA_OUTPUT_CH_1;
    tcc   = DPC_OBJDET_DPU_AOA_PROC_EDMA_HWA_OUTPUT_CH_1;
    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_HWA_OUTPUT_CH_1;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    resPtr->edmaHwaExt[1].chOut.channel =              dmaCh;
    resPtr->edmaHwaExt[1].chOut.eventQueue =           DPC_OBJDET_DPU_AOA_PROC_EDMAOUT_PONG_EVENT_QUE;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_6;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[1].stage[0].paramIn =           param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_7;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[1].stage[0].paramInSignature =  param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_8;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[1].stage[0].paramOut =          param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_9;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[1].stage[1].paramIn =           param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_10;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[1].stage[1].paramInSignature =  param;

    param = DPC_OBJDET_DPU_AOA_PROC_EDMA_VIRT_CH_11;
    allocateEDMAParam(&param);
    resPtr->edmaHwaExt[1].stage[1].paramOut =          param;
    resPtr->edmaHwaExt[1].eventQueue = 0;

    /* Allocate buffers for ping and pong paths: */
	resPtr->localScratchBuffer[0] = gScratchPingBuf;
	resPtr->localScratchBuffer[1] = gScratchPongBuf;

    aoaHwaCfgPtr = &resPtr->hwaCfg;

    aoaHwaCfgPtr->paramSetStartIdx = 0;
    aoaHwaCfgPtr->winRamOffset = 128;

    resPtr->radarCube.datafmt = DPIF_RADARCUBE_FORMAT_1;
    resPtr->radarCube.data = gTestRadarCube;

    /* aoa output buffer set*/
    aoaCfg.res.detObjOutSideInfo = (DPIF_PointCloudSideInfo *) gAoaDetObjSideInfoBuf;
    aoaCfg.res.detObjOutMaxSize = TEST_MAX_NUM_CFARDETOBJ_DPU * TEST_MAX_NUM_PEAKS;
    aoaCfg.res.azimuthStaticHeatMap = gStaticHeatmap;
    aoaCfg.res.detObj2dAzimIdx = gAoaDetObjAzimIdx;
    aoaCfg.res.detObjElevationAngle = (float *)gAoaDetObjElevAngleBuf;

    aoaCfg.dynCfg.fovAoaCfg = &aoaDynFovCfg;

    aoaDynOutCompRxCfg.rangeBias = 0.1;

    realTemp = cosf(CUBADATA_PHASE_SHIFT / 180.f * PI);
    realTemp = realTemp * 32767;
    imagTemp = sin(CUBADATA_PHASE_SHIFT / 180.f * PI);
    imagTemp = imagTemp * (-32767);
    for (ii = 0; ii < TEST_MAX_NUM_VIRTUAL_ANTENNAS; ii++)           //tx=3, rx =4, 3*4=12
    {
        aoaDynOutCompRxCfg.rxChPhaseComp[ii].real = (int16_t) (realTemp + 0.5);
        aoaDynOutCompRxCfg.rxChPhaseComp[ii].imag = (int16_t) (imagTemp - 0.5);

    }
    aoaCfg.dynCfg.compRxChanCfg = &aoaDynOutCompRxCfg;

    aoaDynBmCfg.enabled = false;        //default, will overwirte by each test
    aoaDynBmCfg.multiPeakThrsScal = TWOPEAK_POSITIVE_THRESHOLD;
    aoaCfg.dynCfg.multiObjBeamFormingCfg = &aoaDynBmCfg;

   // extMaxVelCfg.enabled = 1;
    aoaCfg.dynCfg.extMaxVelCfg = &extMaxVelCfg;

    // CacheP_wbInv((void*)&aoaCfg, sizeof(DPU_AoAProcHWA_Config), CacheP_TYPE_ALLD);

    return;
}


static int32_t aoaProc_test_elevFFTCfg
(
    DPU_AoAProcHWA_StaticConfig *DPParams
)
{
    int32_t retVal = 0;
    uint32_t zeroMask = DPParams->zeroInsrtMaskCfg.zeroInsrtMaskElev;
    uint8_t numElemVirtualAntELev = mathUtils_ceilLog2(DPParams->zeroInsrtMaskCfg.zeroInsrtMaskElev);
    uint8_t idx, k=0;
    uint8_t numParams = 0, elements = 0;

    if(numElemVirtualAntELev == DPParams->numVirtualAntElev)
    {
        /* zero insertion is not required */
        DPParams->numElevFFTParams = 0U;
        memset(DPParams->elevFFTParamCfg, 0, sizeof(DPParams->elevFFTParamCfg));
    }
    else{
        uint8_t bitPos[4] = {0};
        uint8_t bitPosDiff[3] = {0};

        for(idx= 0; idx < numElemVirtualAntELev; idx++){
            if((zeroMask >> idx) & 0x1U)
                bitPos[k++] = idx;
        }

        /* difference between two positions */
        for(k = 1; k < DPParams->numVirtualAntElev; k++)
        {
            bitPosDiff[k-1] = bitPos[k] - bitPos[k-1];
        }

        /* at least one param is required */
        DPParams->elevFFTParamCfg[0].srcAddOffset = 0;
        DPParams->elevFFTParamCfg[0].dstAddOffset = bitPos[0];
        DPParams->elevFFTParamCfg[0].acnt = (DPParams->numVirtualAntElev-elements) >= 2 ? (2 - 1) : (1-1);
        DPParams->elevFFTParamCfg[0].dstAidx = bitPosDiff[0];
        elements +=  DPParams->elevFFTParamCfg[0].acnt + 1;

        for(k=1; k < (DPParams->numVirtualAntElev-1); k++)
        {
            if(bitPosDiff[k] == bitPosDiff[k-1])
            {
                DPParams->elevFFTParamCfg[numParams].acnt++;
                elements++;
            }
            else
            {
                numParams++;
                DPParams->elevFFTParamCfg[numParams].srcAddOffset = (DPParams->elevFFTParamCfg[numParams-1].acnt+1);
                DPParams->elevFFTParamCfg[numParams].dstAddOffset = bitPos[elements];
                DPParams->elevFFTParamCfg[numParams].acnt = (DPParams->numVirtualAntElev-elements) >= 2 ? (2 - 1) : (1-1);
                DPParams->elevFFTParamCfg[numParams].dstAidx = bitPosDiff[k+1];
                elements+= DPParams->elevFFTParamCfg[numParams].acnt + 1;
            }

            if(numParams+1 > DPU_AOAPROCHWA_MAX_ELEVFFTPARAMS_FOR_ZERO_INSERTION)
            {
                retVal = DPU_AOAPROCHWA_EEXCEED_ELEVPARAMS;
                goto exit;
            }

            /* break the lopp if all params are exhausted */
            if(elements == DPParams->numVirtualAntElev)
                break;
        }
    }

    DPParams->numElevFFTParams = numParams+1;
exit:
    return (retVal);
}

/**
*  @b Description
*  @n
*  set up the AOA DPU configuration structure based on each test configurations.
*
*  @retval None
*
*/
void Test_setProfile(aoaProcDpuTestConfig_t * testConfig)
{
    DPU_AoAProcHWA_StaticConfig * staticCfgPtr;
    uint16_t ii;
    int16_t interval;

    aoaCfg.res.cfarRngDopSnrList = pCfarDetObjList;
    staticCfgPtr = &aoaCfg.staticCfg;

    staticCfgPtr->staticClutterCfg.isEnabled = testConfig->staticClutterRemovalFlag;

    staticCfgPtr->numTxAntennas = testConfig->numTxAntennas;
    staticCfgPtr->numRxAntennas = testConfig->numRxAntennas;
    staticCfgPtr->numVirtualAntAzim = testConfig->numTxAzimAntennas * testConfig->numRxAntennas;
    staticCfgPtr->numVirtualAntElev = testConfig->numTxElevAntennas * testConfig->numRxAntennas;
    staticCfgPtr->numRangeBins = testConfig->numRangeBins;
    staticCfgPtr->numDopplerChirps = testConfig->numChirps;
    staticCfgPtr->numDopplerBins = testConfig->numDopplerBins;

    staticCfgPtr->zeroInsrtMaskCfg.zeroInsrtMaskAzim = 0;
    for(ii=0; ii<staticCfgPtr->numVirtualAntAzim; ii++)
    {
        staticCfgPtr->zeroInsrtMaskCfg.zeroInsrtMaskAzim |= 1 <<ii;
    }

    staticCfgPtr->zeroInsrtMaskCfg.zeroInsrtMaskElev = 0;
    for(ii=0; ii<staticCfgPtr->numVirtualAntElev; ii++)
    {
        staticCfgPtr->zeroInsrtMaskCfg.zeroInsrtMaskElev |= 1 <<ii;
    }
    staticCfgPtr->zeroInsrtMaskCfg.zeroInsrtMaskElev = staticCfgPtr->zeroInsrtMaskCfg.zeroInsrtMaskElev<<2;

    for(ii=0; ii<staticCfgPtr->numVirtualAntAzim; ii++){
        staticCfgPtr->antennaGeometryCfg[ii] = ii;
    }

    for(ii=0; ii<staticCfgPtr->numVirtualAntElev; ii++){
        staticCfgPtr->antennaGeometryCfg[(SYS_COMMON_NUM_RX_CHANNEL * SYS_COMMON_NUM_AZIMTX_ANTENNAS) + ii] = ii;
    }

    staticCfgPtr->antennaSpacing.xSpacingByLambda = 0.5f;
    staticCfgPtr->antennaSpacing.zSpacingByLambda = 0.8f;
    if(staticCfgPtr->numVirtualAntElev > 0){
        aoaProc_test_elevFFTCfg(staticCfgPtr);
    }

    aoaCfg.res.hwaCfg.numParamSet =  DPU_AoAProcHWA_getNumHwaParamSets(staticCfgPtr->numTxAntennas,
                                                                       staticCfgPtr->numVirtualAntElev,
                                                                       staticCfgPtr->numElevFFTParams,
                                                                       staticCfgPtr->staticClutterCfg.isEnabled);

    /* hard code range step, and dopplerStep*/
    staticCfgPtr->rangeStep = 1.0f;
    staticCfgPtr->dopplerStep = 1.0f;
    aoaCfg.res.detObjOut = pAoaDetObj;

    aoaCfg.res.localScratchBufferSizeBytes = testConfig->numTxAntennas *
            DPU_AOAPROCHWA_NUM_ANGLE_BINS * sizeof(uint16_t);

    /* set up windowing for doppler fft*/
    aoaCfg.res.hwaCfg.winSym = HWA_FFT_WINDOW_SYMMETRIC;

    if (aoaCfg.res.hwaCfg.winSym == HWA_FFT_WINDOW_NONSYMMETRIC)
    {
        aoaCfg.res.hwaCfg.windowSize = testConfig->numChirps * sizeof(int32_t);
    }
    else
    {
        aoaCfg.res.hwaCfg.windowSize = (testConfig->numChirps + 1) / 2 * sizeof(int32_t);
    }

    aoaCfg.res.radarCube.dataSize = testConfig->numTxAntennas * testConfig->numRangeBins *
                                    testConfig->numDopplerBins * testConfig->numRxAntennas * 4;

    aoaCfg.res.azimuthStaticHeatMapSize = testConfig->numRangeBins *
                                      testConfig->numRxAntennas * testConfig->numTxAzimAntennas;

    if (!testConfig->morePeaksFlag)
    {
        for (ii = 0; ii < testConfig->numCfarDetObj; ii++)
        {
            pCfarDetObjList[ii].rangeIdx = testConfig->cfarDetObjRangeIdx[ii];

			if(testConfig->staticClutterRemovalFlag)
            {
                 if (testConfig->cfarDetObjDopplerRefIdx[ii] == 0)
                 {
                     testConfig->cfarDetObjDopplerRefIdx[ii]++;
                     testConfig->cfarDetObjdopplerIdx[ii]++;
                 }
            }

            pCfarDetObjList[ii].dopplerIdx = testConfig->cfarDetObjdopplerIdx[ii];
            pCfarDetObjList[ii].snr = 10;
            pCfarDetObjList[ii].noise = 10;
        }
    }
    else
    {
        interval = testConfig->numDopplerBins * (testConfig->numRangeBins - 1) / testConfig->numCfarDetObj;
        for (ii = 0; ii < testConfig->numCfarDetObj; ii++)
        {
            pCfarDetObjList[ii].rangeIdx = ii * interval / testConfig->numDopplerBins;
            pCfarDetObjList[ii].rangeIdx++;
            pCfarDetObjList[ii].dopplerIdx = test_modCalc(ii * interval, testConfig->numDopplerBins);

            pCfarDetObjList[ii].snr = 10;
            pCfarDetObjList[ii].noise = 10;
        }
    }
    aoaCfg.res.cfarRngDopSnrListSize = testConfig->numCfarDetObj;

    aoaDynBmCfg.multiPeakThrsScal = TWOPEAK_POSITIVE_THRESHOLD;
    if (testConfig->twoPeaksFlag)
    {
        aoaDynBmCfg.enabled = 1;
        if (testConfig->twoPeaksFlag == 2)
        {
            aoaDynBmCfg.multiPeakThrsScal = TWOPEAK_NEGTIVE_THRESHOLD;
        }
    }
    else
    {
        aoaDynBmCfg.enabled = 0;
    }

    /* dynamic set*/
    aoaCfg.dynCfg.prepareRangeAzimuthHeatMap = testConfig->heatMapFlag;

    /* min and max angle */
    aoaDynFovCfg.minAzimuthDeg = -80.;
    aoaDynFovCfg.maxAzimuthDeg = 80.;
    aoaDynFovCfg.minElevationDeg = -80.;
    aoaDynFovCfg.maxElevationDeg = 80.;

    /* add velocity extension */
    extMaxVelCfg.enabled = testConfig->extVelocityFlag;
}

/**
*  @b Description
*  @n
*   If test is for min/max azim/elev angles, update the azim/elev references and AOA DPU configurations
*   by setting the min/max azim/elev angles to mim/max azim/elev used to generate radar cuba data.
*
*  @retval None
*
*/
void Test_updateProfile(aoaProcDpuTestConfig_t * testConfig)
{
    uint16_t ii;
    int16_t aoaSignPeakIdx;
    uint16_t count;
    float Wz;
    float scale;

    /* set the angle limits*/
    if (testConfig->minmaxAngleFlag == 2)
    {
        aoaDynFovCfg.minAzimuthDeg = asin(2.f * testConfig->minAzim / TEST_AOA_FFTSIZE) * 180 / PI;
        aoaDynFovCfg.maxAzimuthDeg = asin(2.f * testConfig->maxAzim / TEST_AOA_FFTSIZE) * 180 / PI;
        aoaDynFovCfg.minElevationDeg = asin(2.f * testConfig->minElev / TEST_AOA_FFTSIZE) * 180 / PI;
        aoaDynFovCfg.maxElevationDeg = asin(2.f * testConfig->maxElev / TEST_AOA_FFTSIZE) * 180 / PI;
    }
    else
    {
        aoaDynFovCfg.minAzimuthDeg = asin(2.f * testConfig->minAzim / TEST_AOA_FFTSIZE) * 180 / PI;
        aoaDynFovCfg.maxAzimuthDeg = 80.;
        aoaDynFovCfg.minElevationDeg = -80;
        aoaDynFovCfg.maxElevationDeg = asin(2.f * testConfig->maxElev / TEST_AOA_FFTSIZE) * 180 / PI; //80; //38; //asin(2.f * testConfig->maxElev / TEST_AOA_FFTSIZE) * 180 / PI;

        testConfig->maxAzim = 32; // disable the max azim index
        testConfig->minElev = -32; //disalbe the min Elev index
    }

    /* update the reference */
    count = 0;
    for (ii = 0; ii < testConfig->numCfarDetObj; ii++)
    {
        Wz = 1.f;
        scale = 1.f;
        if (testConfig->numTxElevAntennas > 0)   //check the elevation first
        {
            Wz = ELEV_SCALE_FACTOR * aoaCfg.res.detObjElevationAngle[ii]; /* Since Wz = scalefactor * sin(phi) */

            if (( Wz < (2.f * testConfig->minElev / 64.f)) || (Wz > (2.f * testConfig->maxElev / 64.f)))
            {
                testConfig->checkCfarDetObjFlag[ii] = 0; //skip this one
                continue; //no need to check the azim
            }
            //else continue check the azim
            scale = sqrt(1.f - Wz * Wz);
        }

		if (aoaCfg.res.detObj2dAzimIdx[ii] >= TEST_AOA_FFTSIZE / 2)
		{
            aoaSignPeakIdx = aoaCfg.res.detObj2dAzimIdx[ii] - TEST_AOA_FFTSIZE;
		}
        else
        {
            aoaSignPeakIdx = aoaCfg.res.detObj2dAzimIdx[ii];
        }

        if ((aoaSignPeakIdx < scale * testConfig->minAzim) || (aoaSignPeakIdx > scale * testConfig->maxAzim))
        {
              testConfig->checkCfarDetObjFlag[ii] = 0; //skip this one
        }
        else
        {
            testConfig->checkCfarDetObjFlag[ii] = 1; //keep this one
            count++;
        }
    }
    testConfig->numAoaOutObj = count;
}

/**
*  @b Description
*  @n
*       Set up the CFAR det objects with random range, doppler index and azim, elev peak index
*
*  @retval None
*
*/
void Test_aoaDpu_cfarListGen(aoaProcDpuTestConfig_t * testConfig)
{
    uint16_t ii, count;
    uint16_t randTempValue;
    uint16_t step, step1;
    uint16_t rangeIdx, dopplerIdx, tempValue, tempValue1;
    uint16_t azimTemp, elevTemp;
    int16_t minAzim, maxAzim;
    int16_t minElev, maxElev;

   // int16_t minElev, maxElev;

   // float angle;

    randTempValue = RandSimple();
    if (testConfig->twoPeaksFlag)
    {
        testConfig->numCfarDetObj = test_modCalc(randTempValue, 2);
    }
    else
    {
        /* mod(rand, TEST_MAX_NUM_CFARDETOBJ)*/
        testConfig->numCfarDetObj = test_modCalc(randTempValue, TEST_MAX_NUM_CFARDETOBJ);
    }
    testConfig->numCfarDetObj++; //at least there is 1 cfar det peak
    testConfig->numAoaOutObj = 0;
    if (testConfig->twoPeaksFlag)
    {
        for (ii = 0; ii < testConfig->numCfarDetObj; ii++)
        {
            randTempValue = RandSimple();
            testConfig->aoaNum3DfftPeaks[ii] = (randTempValue & 0x1) + 1;
            testConfig->numAoaOutObj += testConfig->aoaNum3DfftPeaks[ii];
        }
    }
    else
    {
        for (ii = 0; ii < testConfig->numCfarDetObj; ii++)
        {
            testConfig->aoaNum3DfftPeaks[ii] = 1; //single peak
            testConfig->numAoaOutObj++;
        }
    }

    /* set up the range bin, and doppler bin */
    for (ii = 0; ii < testConfig->numCfarDetObj; ii++)
    {
        step = testConfig->numRangeBins * testConfig->numDopplerBins / testConfig->numCfarDetObj;
        step1 = step >> 1; //more seperate the peaks
        randTempValue = RandSimple();
        tempValue = step * ii + test_modCalc(randTempValue, step1);
        rangeIdx = test_modCalc(tempValue, testConfig->numRangeBins);
        if (rangeIdx == 0) //avoid range = 0;
            rangeIdx += 1;
        dopplerIdx = tempValue / testConfig->numRangeBins;
        testConfig->cfarDetObjRangeIdx[ii] = rangeIdx;
        testConfig->cfarDetObjdopplerIdx[ii] = dopplerIdx;
        testConfig->cfarDetObjDopplerRefIdx[ii] = testConfig->cfarDetObjdopplerIdx[ii];
        if (testConfig->cfarDetObjdopplerIdx[ii] >= testConfig->numDopplerBins/2)
        {
            testConfig->cfarDetObjDopplerRefIdx[ii] = testConfig->cfarDetObjDopplerRefIdx[ii] - testConfig->numDopplerBins;
        }

        testConfig->checkCfarDetObjFlag[ii] = 1;
    }

    /* set up the aoa azim phase */
    count = 0;
    minAzim = 32;
    maxAzim = -32;
    minElev = 32;
    maxElev = -32;
    for (ii = 0; ii < testConfig->numCfarDetObj; ii++)
    {
        randTempValue = RandSimple();
        azimTemp = test_modCalc(randTempValue, TEST_AOA_FFTSIZE); //dpu  uses 64 point fft for angle estimation

        if ((azimTemp <= 32) && (azimTemp >= 27))
        {
            azimTemp -= 5;
        }
        if ((azimTemp >= 32) && (azimTemp <= 37))
        {
            azimTemp += 5;
        }

        if (azimTemp >= TEST_AOA_FFTSIZE / 2)
        {
            testConfig->aoaSignedAzimPeakIdxRef[count] = azimTemp - TEST_AOA_FFTSIZE;
        }
        else
        {
            testConfig->aoaSignedAzimPeakIdxRef[count] = azimTemp;
        }

        if (testConfig->aoaSignedAzimPeakIdxRef[count] < minAzim)
        {
            minAzim = testConfig->aoaSignedAzimPeakIdxRef[count];
        }

        if (testConfig->aoaSignedAzimPeakIdxRef[count] > maxAzim)
        {
            maxAzim = testConfig->aoaSignedAzimPeakIdxRef[count];
        }

        if (testConfig->numTxElevAntennas > 0)    /* add the elevation angle*/
        {
            randTempValue = RandSimple();
            elevTemp = test_modCalc(randTempValue, TEST_AOA_FFTSIZE/2); //  -16<n<16, phase -pi/2<angle<pi/2

            //testConfig->aoaElevPeakIdxRef[count] = tempValue;
            if (elevTemp >= TEST_AOA_FFTSIZE / 4)
                testConfig->aoaSignedElevPeakIdxRef[count] = elevTemp - TEST_AOA_FFTSIZE / 2;
            else
                testConfig->aoaSignedElevPeakIdxRef[count] = elevTemp;

            if (testConfig->aoaSignedElevPeakIdxRef[count] < minElev)
            {
                minElev = testConfig->aoaSignedElevPeakIdxRef[count];
            }
            if (testConfig->aoaSignedElevPeakIdxRef[count] > maxElev)
            {
                maxElev = testConfig->aoaSignedElevPeakIdxRef[count];
            }

        }

        count++;

        if (testConfig->aoaNum3DfftPeaks[ii] > 1)   //two peask
        {
            tempValue1 = azimTemp + 23;
            tempValue1 = test_modCalc(tempValue1, TEST_AOA_FFTSIZE);

            if ((tempValue1 >= TEST_AOA_FFTSIZE / 2)  && (tempValue1 < TEST_AOA_FFTSIZE / 2 + 5))
                tempValue1 += 5;

            if (tempValue1 >= TEST_AOA_FFTSIZE / 2)
                testConfig->aoaSignedAzimPeakIdxRef[count] = tempValue1 - TEST_AOA_FFTSIZE;
            else
                testConfig->aoaSignedAzimPeakIdxRef[count] = tempValue1;


            if (testConfig->numTxElevAntennas > 0)
            {
                tempValue1 = elevTemp + 14;
                tempValue = test_modCalc(tempValue1, TEST_AOA_FFTSIZE/2);

                if (tempValue >= TEST_AOA_FFTSIZE / 4)
                    testConfig->aoaSignedElevPeakIdxRef[count] = tempValue - TEST_AOA_FFTSIZE/2;
                else
                    testConfig->aoaSignedElevPeakIdxRef[count] = tempValue;
            }
            count++;

        }
    }

    testConfig->minAzim = minAzim + 1;
    testConfig->maxAzim = maxAzim - 1;

    testConfig->minElev = minElev + 1;
    testConfig->maxElev = maxElev - 1;
}

/**
*  @b Description
*  @n
*   If test has more than 64/256 cfar det objects for tx=1,2/3, set up the azim peak index
*
*  @retval None
*
*/
int16_t Test_setAzim(uint16_t index)
{
    int16_t azimIdx;
    azimIdx = test_modCalc(index, 64);
    if ((azimIdx < 32) && (azimIdx > 27))
    {
        azimIdx = azimIdx - 5;
    }
    if ((azimIdx >= 32) && (azimIdx < 37))
    {
        azimIdx = azimIdx + 5;
    }

    if (azimIdx >= TEST_AOA_FFTSIZE / 2)
        azimIdx = azimIdx - TEST_AOA_FFTSIZE;
    return azimIdx;
}

volatile uint32_t gCleanSignalGen = 1;

/**
*  @b Description
*  @n
*   Generates the radar cuba data based on test configuration per test.
*
*  @retval None
*
*/
void Test_aoaDpu_cubaDataGen(aoaProcDpuTestConfig_t * testConfig)
{
    uint32_t ii, jj, kk, mm;
    uint32_t radarCubeIdx;
    float dopplerIncr, dopplerAngle;
    float real, imag, noiseRe, noiseIm;
    float realTemp, imagTemp;
    uint32_t txAntIdx;
    uint32_t maxTxAntAzimIdx;
    float var = 1;
    int32_t tempvalue;
    float amplitude; //signal amplitude
                     //float snr; //signal snr
    float noiseScale; //noiseScale^2 is noisevaraince
    uint16_t count;
    uint32_t rangeIdx;

    /* aoa azim phase */
    int32_t aoaAzimIdx1, aoaAzimIdx2;
    float aoaAzimAngle1, aoaAzimAngle2;

    /* aoa elev phase */
    int32_t aoaElevIdx1, aoaElevIdx2;
    float aoaElevAngle1, aoaElevAngle2;
    uint16_t numHeatmapAzimAnt;
    float winValue, realWin, imagWin;
    int32_t winTempValue;
    cmplx32ImRe_t  heatmapTemp;
    uint8_t heatmapScale;
    int16_t interval;
    int16_t dopplerIdx;

    amplitude = 840.f;
    noiseScale = 60.f;

    /* generate the cuba data*/
#if  TEST_AOA_DEBUG
    if (testConfig->numRangeBins * testConfig->numDopplerBins *
        testConfig->numTxAntennas * testConfig->numRxAntennas > maxCubeSize)
    {
        maxCubeSize = testConfig->numRangeBins * testConfig->numDopplerBins *
                      testConfig->numTxAntennas * testConfig->numRxAntennas;
        testCaseId =  testCount;
    }
#endif
    memset(gTestRadarCube, 0, sizeof(cmplx16ImRe_t) * testConfig->numRangeBins *
           testConfig->numDopplerBins * testConfig->numTxAntennas * testConfig->numRxAntennas);
    /* save the heatmap reference */
    numHeatmapAzimAnt = testConfig->numTxAzimAntennas * testConfig->numRxAntennas;

    memset(gStaticHeatmapRef, 0, sizeof(cmplx16ImRe_t) * testConfig->numRangeBins * numHeatmapAzimAnt);

 //   memset(heatmapTemp, 0, sizeof(cmplx32ImRe_t) * testConfig->numRangeBins* numHeatmapAzimAnt);

    /* generate the peak first */
    count = 0;
    interval = (testConfig->numRangeBins - 1) * testConfig->numDopplerBins / testConfig->numCfarDetObj;

    for (mm = 0; mm < testConfig->numCfarDetObj; mm++)
    {
        if (!testConfig->morePeaksFlag)
        {
            rangeIdx = testConfig->cfarDetObjRangeIdx[mm];
            dopplerIdx = testConfig->cfarDetObjDopplerRefIdx[mm];
            if (testConfig->extVelocityFlag)
            {
                if ((testConfig->numTxAntennas == 2) && (dopplerIdx != 0) && (mm&0x1))
                {
                   if (dopplerIdx > 0)
                   {
                        dopplerIdx = dopplerIdx - testConfig->numDopplerBins;
                   }
                   else
                   {
                        dopplerIdx = dopplerIdx + testConfig->numDopplerBins;
                   }
                }
                if ((testConfig->numTxAntennas == 3) && (dopplerIdx != 0))
                {
                    tempvalue = test_modCalc(mm, 3);  //0,1,2---> -1, 0, 1
                    dopplerIdx = dopplerIdx + (tempvalue - 1) * testConfig->numDopplerBins;
                }
                if ((testConfig->numTxAntennas == 4) && (dopplerIdx != 0) && (mm&0x1))
                {
                   if (dopplerIdx > 0)
                   {
                        dopplerIdx = dopplerIdx - 2*testConfig->numDopplerBins;
                   }
                   else
                   {
                        dopplerIdx = dopplerIdx + 2*testConfig->numDopplerBins;
                   }
                }
                if (testConfig->staticClutterRemovalFlag)
                {
                    if (dopplerIdx == 0)
                    {
                        dopplerIdx++;
                    }
                }
                testConfig->cfarDetObjDopplerRefIdx[mm] = dopplerIdx; //update the reference, no need to recalculate
            }
            aoaAzimIdx1 = testConfig->aoaSignedAzimPeakIdxRef[count];      //signed
            aoaElevIdx1 = 0;
            aoaAzimIdx2 = 0;
            aoaElevIdx2 = 0;
            if (testConfig->numTxElevAntennas > 0)
            {
                aoaElevIdx1 = testConfig->aoaSignedElevPeakIdxRef[count];
            }
            count++;
            if (testConfig->aoaNum3DfftPeaks[mm] == 2)
            {
                aoaAzimIdx2 = testConfig->aoaSignedAzimPeakIdxRef[count];

                if (testConfig->numTxElevAntennas > 0)
                {
                    aoaElevIdx2 = testConfig->aoaSignedElevPeakIdxRef[count];
                }
                count++;
            }
        }
        else
        {
            /* MUST MATCH WITH SETPROFILE*/
            rangeIdx = mm * interval / testConfig->numDopplerBins;
            rangeIdx++;

            dopplerIdx = test_modCalc(mm * interval, testConfig->numDopplerBins);
            if (dopplerIdx >= testConfig->numDopplerBins / 2){
                dopplerIdx -= testConfig->numDopplerBins;
            }

            aoaAzimIdx1 = Test_setAzim(mm);
            aoaElevIdx1 = aoaAzimIdx1 / 2;
        }
        for (ii = 0; ii < testConfig->numChirps; ii++) //chirp loop
        {
            for (txAntIdx = 0; txAntIdx < testConfig->numTxAntennas; txAntIdx++) //for tx antenna loop
            {
                for (jj = 0; jj < testConfig->numRxAntennas; jj++) //rx antenna loop
                {

                    dopplerIncr = 2.f * PI * dopplerIdx /testConfig->numDopplerBins/testConfig->numTxAntennas;
                    dopplerAngle = (ii * testConfig->numTxAntennas + txAntIdx) * dopplerIncr;  //increase every chirp

                    if(testConfig->numTxElevAntennas > 0){
                        if(testConfig->numTxAntennas == 4){
                            maxTxAntAzimIdx = 3; /* 3 Azim, 1 Elev Tx case */
                        }
                        else{
                            maxTxAntAzimIdx = 2; /* 2 Azim, 1 Elev Tx case */
                        }
                    }
                    else{
                        maxTxAntAzimIdx = 3; /* Possible 3 Azim Tx case */
                    }

                    /* add the aoa phase */
                    // if (txAntIdx < 2)   //for tx 0, 1, the delay is 0 1 2 3 4 5 6 7
                    if (txAntIdx < maxTxAntAzimIdx)   //for tx 0, 1, 2 the delay is 0 1 2 3 4 5 6 7 8 9 10 11
                    {
                        aoaAzimAngle1 = 2.f * PI * (txAntIdx * testConfig->numRxAntennas + jj) *
                                        (float)aoaAzimIdx1 / (float)TEST_AOA_FFTSIZE;
                        aoaAzimAngle2 = 0.f;
                        if ((testConfig->aoaNum3DfftPeaks[mm] == 2) && (!testConfig->morePeaksFlag))
                        {
                            aoaAzimAngle2 = 2.f * PI * (txAntIdx * testConfig->numRxAntennas + jj)  *
                            (float)aoaAzimIdx2 / (float)TEST_AOA_FFTSIZE;
                        }

                        /* for tx 0, 1, no elev delay */
                        aoaElevAngle1 = 0.f;
                        aoaElevAngle2 = 0.f;
                    }
                    else //for Elev Tx, azim delay is [ 2 3 4 5]
                    {
                        aoaAzimAngle1 = 2.f * PI * (2 + jj) * (float)aoaAzimIdx1 / (float)TEST_AOA_FFTSIZE;
                        aoaAzimAngle2 = 0.f;
                       if ((!testConfig->morePeaksFlag) && (testConfig->aoaNum3DfftPeaks[mm] == 2) )
                       {
                            aoaAzimAngle2 = 2.f * PI * (2 + jj)  * (float)aoaAzimIdx2 / (float)TEST_AOA_FFTSIZE;
                       }

                        aoaElevAngle1 = -2.f * PI * (float)aoaElevIdx1 / (float)TEST_AOA_FFTSIZE;
                        aoaElevAngle2 = 0.f;
                        if ((!testConfig->morePeaksFlag) && (testConfig->aoaNum3DfftPeaks[mm] == 2))
                        {
                            aoaElevAngle2 = -2.f * PI* (float)aoaElevIdx2 / (float)TEST_AOA_FFTSIZE;
                        }
                    }

                    real = amplitude * cosf(dopplerAngle + aoaAzimAngle1 + aoaElevAngle1);
                    imag = amplitude * sinf(dopplerAngle + aoaAzimAngle1 + aoaElevAngle1);

                    if ((!testConfig->morePeaksFlag) && (testConfig->aoaNum3DfftPeaks[mm] == 2))
                    {
                        real += (amplitude * 0.75f * cosf(dopplerAngle + aoaAzimAngle2 + aoaElevAngle2));
                        imag += (amplitude * 0.75f * sinf(dopplerAngle + aoaAzimAngle2 + aoaElevAngle2));
                    }

                    radarCubeIdx = txAntIdx * testConfig->numChirps * testConfig->numRxAntennas *  testConfig->numRangeBins;
                    radarCubeIdx += (ii * testConfig->numRxAntennas * testConfig->numRangeBins);
                    radarCubeIdx += (jj * testConfig->numRangeBins);
                    radarCubeIdx += rangeIdx;

                    tempvalue = (int32_t)MATHUTILS_ROUND_FLOAT(real);
                    MATHUTILS_SATURATE16(tempvalue);
                    gTestRadarCube[radarCubeIdx].real += (int16_t)tempvalue;
                    tempvalue = (int32_t)MATHUTILS_ROUND_FLOAT(imag);
                    MATHUTILS_SATURATE16(tempvalue);

                    /* some peaks have same range bin, with different doppler bin */
                    gTestRadarCube[radarCubeIdx].imag += (int16_t)tempvalue;
                }
            }
        }

    }

    heatmapScale = 0;
    if (testConfig->numDopplerBins == 2)
        heatmapScale = 1;      //reference right shift
    else if (testConfig->numDopplerBins == 4)
        heatmapScale = 2;
    else if (testConfig->numDopplerBins == 8)
        heatmapScale = 3;
    else if (testConfig->numDopplerBins == 16)
        heatmapScale = 4;
    else if (testConfig->numDopplerBins == 32)
        heatmapScale = 5;
    else if (testConfig->numDopplerBins == 64)
        heatmapScale = 6;
    else if (testConfig->numDopplerBins == 128)
        heatmapScale = 7;
    else if (testConfig->numDopplerBins == 256)
        heatmapScale = 8;
    else if (testConfig->numDopplerBins == 512)
        heatmapScale = 9;
    else if (testConfig->numDopplerBins == 1024)
        heatmapScale = 10;

    /* add the noise, and save the heatmap reference */
    for (txAntIdx = 0; txAntIdx < testConfig->numTxAntennas; txAntIdx++) //for tx antenna loop
    {
       for (jj = 0; jj < testConfig->numRxAntennas; jj++) //rx antenna loop
       {
           for (kk = 0; kk < testConfig->numRangeBins; kk++)
           {
            //    if ((txAntIdx == 0) || (txAntIdx == 1))
            if ((txAntIdx == 0) || (txAntIdx == 1) || (txAntIdx == 2 && testConfig->numTxAzimAntennas == 3))
               {
                   heatmapTemp.real = 0;
                   heatmapTemp.imag = 0;
               }

               for (ii = 0; ii < testConfig->numChirps; ii++) //chirp loop
               {
                   /* get window coefficients in float */
                   if (testConfig->winType == 1) //check window type
                   {
                       winValue = 1.f;    //rec
                   }
                   else
                   {
                       /* hard code to hanning window */
                       winValue = 0.5*(1.f - cos(2.f * PI_ * (float)ii / ((float)testConfig->numChirps - 1)));
                   }

                   GaussianNoise(var, &noiseRe, &noiseIm);
                   //noise only
                   if (gCleanSignalGen)
                   {
                       real = 0;
                       imag = 0;
                   }
                   else
                   {
                       real = noiseScale * noiseRe;
                       imag = noiseScale * noiseIm;
                   }

                   radarCubeIdx = txAntIdx * testConfig->numChirps * testConfig->numRxAntennas *  testConfig->numRangeBins;
                   radarCubeIdx += (ii * testConfig->numRxAntennas * testConfig->numRangeBins);
                   radarCubeIdx += (jj * testConfig->numRangeBins);
                   radarCubeIdx += kk;

                   real += gTestRadarCube[radarCubeIdx].real;
                   imag += gTestRadarCube[radarCubeIdx].imag;

                   /* add a phase shift*/
                   realTemp = real;
                   imagTemp = imag;
#ifdef SUBSYS_MSS
                   real = realTemp * cosf(CUBADATA_PHASE_SHIFT / 180.f * PI) -
                          imagTemp * sinf(CUBADATA_PHASE_SHIFT / 180.f * PI);
                   imag = realTemp * sinf(CUBADATA_PHASE_SHIFT / 180.f * PI) +
                          imagTemp * cosf(CUBADATA_PHASE_SHIFT / 180.f * PI);
#endif
#ifdef SUBSYS_DSS
                   if (CUBADATA_PHASE_SHIFT)
                   {
                       real = realTemp * cosf(CUBADATA_PHASE_SHIFT / 180.f * PI) -
                              imagTemp * sinf(CUBADATA_PHASE_SHIFT / 180.f * PI);
                       imag = realTemp * sinf(CUBADATA_PHASE_SHIFT / 180.f * PI) +
                              imagTemp * cosf(CUBADATA_PHASE_SHIFT / 180.f * PI);
                   }
#endif

                   tempvalue = (int32_t)MATHUTILS_ROUND_FLOAT(real);
                   MATHUTILS_SATURATE16(tempvalue);
                   gTestRadarCube[radarCubeIdx].real = (int16_t)tempvalue;
                   tempvalue = (int32_t)MATHUTILS_ROUND_FLOAT(imag);
                   MATHUTILS_SATURATE16(tempvalue);
                   gTestRadarCube[radarCubeIdx].imag = (int16_t)tempvalue;

                   /* sum over all chirps after windowing */
                  // if (testConfig->numTxAntennas == 3)    //only accumulate tx0 and tx 1 data

                if ((txAntIdx == 0) || (txAntIdx == 1)|| (txAntIdx == 2 && testConfig->numTxAzimAntennas == 3))
                   {
                       realWin = winValue * realTemp; //with no phase shift for heatmap, will compensated back in the aoa dpu
                       imagWin = winValue * imagTemp;

                       winTempValue = (int32_t)MATHUTILS_ROUND_FLOAT(realWin);
                       //MATHUTILS_SATURATE16(winTempValue);
                       heatmapTemp.real += (int16_t)winTempValue;
                       winTempValue = (int32_t)MATHUTILS_ROUND_FLOAT(imagWin);
                       //MATHUTILS_SATURATE16(winTempValue);
                       heatmapTemp.imag += (int16_t)winTempValue;
                   }
               } /* end of chirp sum */

                if ((txAntIdx == 0) || (txAntIdx == 1) || (txAntIdx == 2 && testConfig->numTxAzimAntennas == 3))
                {
                    gStaticHeatmapRef[kk * numHeatmapAzimAnt + txAntIdx * testConfig->numRxAntennas + jj].real =
                         (int16_t)(heatmapTemp.real >> heatmapScale);
                    gStaticHeatmapRef[kk * numHeatmapAzimAnt + txAntIdx * testConfig->numRxAntennas + jj].imag =
                         (int16_t)(heatmapTemp.imag >> heatmapScale);
                }
            }
        }
    }
}

#if TEST_AOA_DEBUG
/* input is 16I/16Q, output is 32 bits I and 32 bits Q*/
void Test_debug_2dWindowing(uint16_t numSamples)
{
    uint32_t ii;
    int32_t winCoef;
    uint32_t dataSample;
    int64_t realValue, imagValue;
    int64_t tempValue;

    for (ii = 0; ii < numSamples; ii++)
    {
        winCoef = (int32_t)gTestDopplerWindow[ii];
        dataSample = fftDataInBuf[ii];

        imagValue = (int64_t)((int16_t)(dataSample & 0x0000FFFF));      //lsb is imag
        tempValue = (imagValue * winCoef + 0x10000) >> 17;   //window coefficients is Q17 format
        fftDataWindow[ii * 2 + 1] = (int32_t)tempValue;       //imag is odd

        realValue = (int64_t)((int16_t)(dataSample >> 16));
        tempValue = (realValue * winCoef + 0x10000) >> 17;
        fftDataWindow[ii * 2] = (int32_t)tempValue;     //real is even
    }
}

void Test_debug_fft(int32_t * dataOut, int32_t * dataIn, uint32_t fftSize, uint32_t numAdcSamples)
{
    uint32_t ii, jj;
    float cosValue, sinValue;
    float cosStep, sinStep;
    float alpha;
    float realValueTemp, imagValueTemp;
    float realValue, imagValue;
    float inReal, inImag;
    int32_t outReal, outImag;
    float tempValue;
    uint16_t divValue;

    divValue = 0;

    //in 32 bits input, Real16/imag16
    for (ii = 0; ii < fftSize; ii++)
    {
        alpha = -2.f * ii * PI_ / (float)fftSize;
        cosValue = 1.f;
        sinValue = 0.f;
        cosStep = cos(alpha);
        sinStep = sin(alpha);
        //realValue = (float)((int16_t)(dataIn[0] >> 16));
        //imagValue = (float)((int16_t)(dataIn[0] & 0xFFFF));   //lsb at even
        realValue = (float)dataIn[0];
        imagValue = (float)dataIn[1];
        for (jj = 1; jj < numAdcSamples; jj++)
        {
            tempValue = cosValue;
            cosValue = cosValue * cosStep - sinValue * sinStep;
            sinValue = sinValue * cosStep + tempValue *sinStep;

            //inReal = (float)((int16_t)(dataIn[jj] >> 16));
            //inImag = (float)((int16_t)(dataIn[jj] & 0xFFFF));
            inReal = (float)dataIn[2 * jj];
            inImag = (float)dataIn[2 * jj + 1];

            realValueTemp = inReal * cosValue - inImag * sinValue;
            realValue += realValueTemp;
            imagValueTemp = inReal * sinValue + inImag * cosValue;
            imagValue += imagValueTemp;
        }
        /*match with hwa hardware */
        realValue = realValue / (float)(1 << divValue);
        outReal = (int32_t)MATHUTILS_ROUND_FLOAT(realValue);

        imagValue = imagValue / (float)(1 << divValue);
        outImag = (int32_t)MATHUTILS_ROUND_FLOAT(imagValue);

        dataOut[2 * ii] = outReal;    //out real is even
        dataOut[2 * ii + 1] = outImag; //out imag is odd
    }
}

void Test_debug_cubadataverify(aoaProcDpuTestConfig_t * testConfig)
{
    uint32_t ii, kk, jj, mm;
    uint16_t rangeIdx;
    uint32_t *cubePtr = (uint32_t *)gTestRadarCube;     //msb is real, lsb is imag
    uint32_t radarCubeIdx;
    float cosV, sinV, phase, cos2V, sin2V, cos3V, sin3V;
    float tempValue;

#if TEST_AOA_DEBUG_RADARCUBEDATA
    /* print out the cuba data, */
    for (kk = 0; kk < testConfig->numRxAntennas; kk++)     //first rx antenna
    {
        //for (ii = 0; ii < testConfig->numRangeBins; ii++) //then range bin
        ii = testConfig->cfarDetObjRangeIdx[1];
        {
            for (mm = 0; mm < testConfig->numChirps; mm++)   //then chirp index
            {
                for (jj = 0; jj < testConfig->numTxAntennas; jj++)     //then tx antenna
                {
                    radarCubeIdx = jj * testConfig->numChirps * testConfig->numRxAntennas *  testConfig->numRangeBins;
                    radarCubeIdx += (mm * testConfig->numRxAntennas * testConfig->numRangeBins);
                    radarCubeIdx += (kk * testConfig->numRangeBins);
                    radarCubeIdx += ii;
                    test_print("%d,%d,%d,", radarCubeIdx, gTestRadarCube[radarCubeIdx].real, gTestRadarCube[radarCubeIdx].imag);
                }
            }
            test_print("\n");
        }
    }
#endif

    if (testConfig->numRxAntennas > 1)
    {
        //for (ii = 0; ii < testConfig->numCfarDetObj; ii++)
        ii = 1;
        {
            rangeIdx = testConfig->cfarDetObjRangeIdx[ii];

            if (testConfig->numTxAntennas >= 2)
            {
                /* exp(-j * 2 * pi*dopplerIdx / numTx / dopplerbin) */
                phase = -2.f * PI * (float)testConfig->cfarDetObjDopplerRefIdx[ii] /
                        (float)testConfig->numTxAntennas / (float)testConfig->numDopplerBins;
                cosV = cosf(phase);
                sinV = sinf(phase);
                if (testConfig->numTxAntennas == 3)
                {
                    cos2V = cosf(2.f * phase);
                    sin2V = sinf(2.f * phase);
                }
                if (testConfig->numTxAntennas == 4)
                {
                    cos3V = cosf(3.f * phase);
                    sin3V = sinf(3.f * phase);
                }
            }

            for (kk = 0; kk < testConfig->numRxAntennas; kk++)    //all rx antenna
            {
                for (jj = 0; jj < testConfig->numTxAntennas; jj++) //all tx antenna
                {
                    for (mm = 0; mm < testConfig->numChirps; mm++) //all chirps
                    {
                        /* get the input data */

                        radarCubeIdx = jj * testConfig->numChirps * testConfig->numRxAntennas *  testConfig->numRangeBins;
                        radarCubeIdx += (mm * testConfig->numRxAntennas * testConfig->numRangeBins);
                        radarCubeIdx += (kk * testConfig->numRangeBins);
                        radarCubeIdx += rangeIdx;
                        fftDataInBuf[mm] = cubePtr[radarCubeIdx];

                    }
                    /* do the windowing fft */
                    Test_debug_2dWindowing(testConfig->numChirps); //16I/Q in, 32 I/Q out
                                                                   // if (testConfig->numDopplerBins < 8)
                                                                   // {
                                                                   //     Test_debug_fft(fftDataOutBuf, fftDataWindow, 8, testConfig->numChirps);   //minimum fft size is 8
                                                                   //     fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] = fftDataOutBuf[testConfig->cfarDetObjdopplerIdx[ii] *(8/ testConfig->numChirps) * 2];
                                                                   //     fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] = fftDataOutBuf[testConfig->cfarDetObjdopplerIdx[ii] * (8 / testConfig->numChirps) * 2 + 1];

                                                                   // }
                                                                   //else
                    {
                        Test_debug_fft(fftDataOutBuf, fftDataWindow, testConfig->numDopplerBins, testConfig->numChirps);

                        /* find the value corresponds the doppler index, should be the maximum */
                        fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] = fftDataOutBuf[testConfig->cfarDetObjdopplerIdx[ii] * 2];
                        fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] = fftDataOutBuf[testConfig->cfarDetObjdopplerIdx[ii] * 2 + 1];

                        if ((testConfig->numTxAntennas >= 2) && (jj == 1))   //numTx=2,3, tx=1 doppler compensation
                        {
                            /* compensate the second tx data by exp(-j*2*pi*dopplerIdx/numTx/dopplerbin) */
                            tempValue = (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] * cosV -
                                        (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] * sinV;
                            if (tempValue > 0)
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] = (int32_t)(tempValue + 0.5);
                            else
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] = (int32_t)(tempValue - 0.5);

                            tempValue = (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] * sinV +
                                        (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] * cosV;
                            if (tempValue > 0)
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] = (int32_t)(tempValue + 0.5);
                            else
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] = (int32_t)(tempValue - 0.5);
                        }

                        if ((testConfig->numTxAntennas >= 3) && (jj == 2)) //numtx=3, tx=2, compensation
                        {
                            tempValue = (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] * cos2V -
                                        (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] * sin2V;
                            if (tempValue > 0)
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] = (int32_t)(tempValue + 0.5);
                            else
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] = (int32_t)(tempValue - 0.5);

                            tempValue = (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] * sin2V +
                                        (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] * cos2V;
                            if (tempValue > 0)
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] = (int32_t)(tempValue + 0.5);
                            else
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] = (int32_t)(tempValue - 0.5);

                        }
                        if ((testConfig->numTxAntennas == 4) && (jj == 2)) //numtx=4, tx=3, compensation
                        {
                            tempValue = (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] * cos3V -
                                        (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] * sin3V;
                            if (tempValue > 0)
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] = (int32_t)(tempValue + 0.5);
                            else
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] = (int32_t)(tempValue - 0.5);

                            tempValue = (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2] * sin3V +
                                        (float)fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] * cos3V;
                            if (tempValue > 0)
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] = (int32_t)(tempValue + 0.5);
                            else
                                fft3dDataInBuf[(jj * testConfig->numRxAntennas + kk) * 2 + 1] = (int32_t)(tempValue - 0.5);

                        }
                    }
                }
            }

            /* do the 3d fft*/
#if TEST_AOA_DEBUG_3DFFTIN
            for (mm = 0; mm < testConfig->numTxAntennas *testConfig->numRxAntennas; mm++)
            {
                test_print("%d,%d\n", fft3dDataInBuf[2 * mm], fft3dDataInBuf[2 * mm + 1]);   //even is real, odd is imag
            }
            test_print("\n");
#endif
            if (testConfig->numTxElevAntennas == 0)
            {
                Test_debug_fft(fft3dDataOutBuf, fft3dDataInBuf, TEST_AOA_FFTSIZE,
                               testConfig->numTxAntennas*testConfig->numRxAntennas);
            }
            else if (testConfig->numTxAntennas == 3)
            {

                Test_debug_fft(fft3dDataOutBuf, fft3dDataInBuf, TEST_AOA_FFTSIZE, 8);

                Test_debug_fft(&fft3dDataOutBuf[TEST_AOA_FFTSIZE*2], &fft3dDataInBuf[8*2], TEST_AOA_FFTSIZE, 4);

            }
#if TEST_AOA_DEBUG_3DFFTOUT
            {
                uint64_t absValue;
                uint64_t maxAbsValue;
                uint16_t maxIdxAzim;

                maxAbsValue = 0;
                maxIdxAzim = 0;
                for (mm = 0; mm < TEST_AOA_FFTSIZE; mm++)
                {
                    // test_print("%d,%d\n", fft3dDataOutBuf[2 * mm], fft3dDataOutBuf[2 * mm + 1]);
                    absValue = (int64_t)fft3dDataOutBuf[2 * mm] *     fft3dDataOutBuf[2 * mm] +
                               (int64_t)fft3dDataOutBuf[2 * mm + 1] * fft3dDataOutBuf[2 * mm + 1];
                    if (absValue > maxAbsValue)
                    {
                        maxIdxAzim = mm;
                        maxAbsValue = absValue;
                    }
                }
                test_print("The azim peak idx is %d, ref = %d\n", maxIdxAzim, testConfig->aoaSignedAzimPeakIdxRef[ii]);
                if (testConfig->numTxAntennas == 3)
                {
                    /* find the elev * conj(azim)*/
                    int32_t elevReal, elevImag, azimReal, azimImag;
                    float realValue, imagValue;
                    float angleEst;
                    float angleCal;
                    azimReal = fft3dDataOutBuf[2 * maxIdxAzim];
                    azimImag = fft3dDataOutBuf[2 * maxIdxAzim + 1];

                    elevReal = fft3dDataOutBuf[2 * maxIdxAzim + 2 * TEST_AOA_FFTSIZE];
                    elevImag = fft3dDataOutBuf[2 * maxIdxAzim + 2 * TEST_AOA_FFTSIZE + 1];

                    realValue = (float) elevReal *  (float)azimReal + (float)elevImag * (float)azimImag;
                    imagValue = (float)azimReal *  (float)elevImag - (float)elevReal *(float)azimImag;

                    /* should be 2*2*pi*azimIdx/64 - 2*pi*elevIdx/32 */
                    angleEst = atan2(imagValue, realValue);
                    angleCal = 2.f * 2.f * PI * testConfig->aoaSignedAzimPeakIdxRef[ii] / 64.f -
                               2 * PI * testConfig->aoaSignedElevPeakIdxRef[ii] / 64.f;
                    if (angleCal < -2.f * PI)
                    {
                        angleCal += (2.f * PI);
                    }
                    if (angleCal > 2.f * PI)
                    {
                        angleCal -= (2.f * PI);
                    }

                    test_print("the elev angle diff est = %f, cal = %f, diff = %f\n",
                                  angleEst, angleCal, fabs(angleCal - angleEst));
                }
            }
#endif
        }
    }
}
#endif
/**
*  @b Description
*  @n
*   check the AOA DPU results with the reference.
*
*  @retval None
*
*/
void Test_printResults(aoaProcDpuTestConfig_t * testConfig)
{

    uint32_t ii, jj;
    uint32_t count, refCount;
    int16_t aoaSignPeakIdx;
    float x, y, z;
    uint8_t testPass = 1;
    float elevAngleRef;
    float elevAngleErr;
    uint16_t numAzimVirtualAnt;
    int16_t heapmapDiff;
    int16_t azimSignedPeakIdxRef;
    uint16_t rangeIdx;
    uint16_t interval;
    int16_t dopplerIdx;
    int16_t elevIdxRef;
    float range;

    interval = (testConfig->numRangeBins - 1) * testConfig->numDopplerBins / testConfig->numCfarDetObj;

    /* check the aoa output object number */
    if (testConfig->twoPeaksFlag == 2)
    {
        if ((outAoaProc.numAoADetectedPoints * 2)!= testConfig->numAoaOutObj)
        {
            test_print("\nAOA output number is : aoaOut=%d, ref=%d\n",
                          outAoaProc.numAoADetectedPoints, testConfig->numAoaOutObj);
            finalResults = 0;
            testPass = 0;
            if (testConfig->morePeaksFlag)
            {
                test_print(" FAIL \n");
                return;
            }
        }
    }
    else
    {
        if (outAoaProc.numAoADetectedPoints != testConfig->numAoaOutObj)
        {
            test_print("\nAOA output number is : aoaOut=%d, ref=%d\n",
                          outAoaProc.numAoADetectedPoints, testConfig->numAoaOutObj);
            finalResults = 0;
            testPass = 0;
            if (testConfig->morePeaksFlag)
            {
                test_print(" FAIL \n");
                return;
            }
        }
    }

    /* check the x,y,z*/
    if (testConfig->numTxAntennas * testConfig->numRxAntennas == 1)
    {
        for (ii = 0; ii < testConfig->numCfarDetObj; ii++) //no aoa, directly output the cfar result
        {
            if ((aoaCfg.res.detObjOut[ii].x != 0.f) || (aoaCfg.res.detObjOut[ii].z != 0.f) ||
                (aoaCfg.res.detObjOut[ii].y != (float)testConfig->cfarDetObjRangeIdx[ii]))
            {

                if (testPass)
                    test_print("\n");
                test_print("AOA %d out x/y/z is : aoaOut=%d,%d,%d, ref=%d\n", ii,
                              (int32_t) aoaCfg.res.detObjOut[ii].x, (int32_t) aoaCfg.res.detObjOut[ii].y,
                              (int32_t) aoaCfg.res.detObjOut[ii].z, (int32_t) testConfig->cfarDetObjRangeIdx[ii]);
                finalResults = 0;
                testPass = 0;
            }
        }
    }
    else
    {
        /* check the x.y.z, and peakIdx*/
        count = 0;
        refCount = 0;
        for (ii = 0; ii < testConfig->numCfarDetObj; ii++) //check the peak index
        {
            if (!testConfig->morePeaksFlag)
            {
                if (testConfig->checkCfarDetObjFlag[ii] == 0)
                {
                    refCount++;
                    if (testConfig->aoaNum3DfftPeaks[ii] == 2)
                    {
                        refCount++;
                    }
                    continue; //skip this obj, it is deleted by min/max angle
                }
            }
            //else, more peaks case, alwarys check, no deleted

			if (aoaCfg.res.detObj2dAzimIdx[count] >= TEST_AOA_FFTSIZE / 2)
			{
                aoaSignPeakIdx = aoaCfg.res.detObj2dAzimIdx[count] - TEST_AOA_FFTSIZE;
			}
            else
            {
                aoaSignPeakIdx = aoaCfg.res.detObj2dAzimIdx[count];
            }

            if (!testConfig->morePeaksFlag)
            {
                azimSignedPeakIdxRef = testConfig->aoaSignedAzimPeakIdxRef[refCount];
            }
            else
            {
                azimSignedPeakIdxRef = Test_setAzim(ii);
            }
            if (abs(aoaSignPeakIdx - azimSignedPeakIdxRef) > MAX_PEAKIDX_ERROR)
                // && (fabs(aoaCfg.res.detObj2dAzimIdx[ii] - testConfig->aoaSignedAzimPeakIdxRef[count])))
            {

                if (testPass)
                    test_print("\n");
                printf("Cfar Obj %d 1st peakIdx : aoaOut=%d, ref=%d\n", ii, aoaCfg.res.detObj2dAzimIdx[count],
                       azimSignedPeakIdxRef);
                finalResults = 0;
                testPass = 0;
            }

            if (testConfig->numTxElevAntennas > 0)
            {
                if (!testConfig->morePeaksFlag)
                {
                    elevIdxRef = testConfig->aoaSignedElevPeakIdxRef[refCount];
                }
                else
                {
                    elevIdxRef = azimSignedPeakIdxRef / 2;
                }
                elevAngleRef = 2.f * elevIdxRef / (float)TEST_AOA_FFTSIZE / ELEV_SCALE_FACTOR; /* Index to angle */
                elevAngleErr = fabs(elevAngleRef - aoaCfg.res.detObjElevationAngle[count]);

                if (elevAngleErr > MAX_ELEVANGLE_ERROR)
                {
                    if (testPass)
                        test_print("\n");
                    printf("Cfar obj %d 1st ElevAngle : aoaOut=%f, ref=%f, error=%f\n", ii,
                           aoaCfg.res.detObjElevationAngle[ii], elevAngleRef, elevAngleErr);
                    finalResults = 0;
                    testPass = 0;
                }
            }

            /* x is range * sin */
            if (!testConfig->morePeaksFlag)
            {
                rangeIdx = testConfig->cfarDetObjRangeIdx[ii];
            }
            else
            {
                rangeIdx = ii * interval / testConfig->numDopplerBins;
                rangeIdx++;
            }
            range = (float)rangeIdx - aoaDynOutCompRxCfg.rangeBias;
            x = range * 2.f * (float)aoaSignPeakIdx / (float)TEST_AOA_FFTSIZE;
            if (testConfig->numTxElevAntennas > 0)
            {
                z = range * aoaCfg.res.detObjElevationAngle[count];
                y = sqrtf(range * range - x * x - z * z);
            }
            else
            {
                z = 0.f;
                y = range * sqrtf(1.f - 4.f * (float)aoaSignPeakIdx * (float)aoaSignPeakIdx /
                    (float)TEST_AOA_FFTSIZE / (float)TEST_AOA_FFTSIZE);
            }

            if ( (fabs(aoaCfg.res.detObjOut[count].x - x) > MAX_LOCATION_ERROR) ||
                 (fabs(aoaCfg.res.detObjOut[count].y - y) > MAX_LOCATION_ERROR) ||
                 (fabs(aoaCfg.res.detObjOut[count].z - z) > MAX_LOCATION_ERROR) )
            {
                if (testPass)
                    test_print("\n");
                printf("cfar obj %d out 1st x/y/z is : aoaOut=%f,%f,%f, ref=%f,%f,%f\n",
                       ii, aoaCfg.res.detObjOut[count].x, aoaCfg.res.detObjOut[count].y,
                       aoaCfg.res.detObjOut[count].z, x,y,z);
                finalResults = 0;
                testPass = 0;
            }

            count++;
            refCount++;

            if ((!testConfig->morePeaksFlag) &&(testConfig->aoaNum3DfftPeaks[ii] == 2) )
            {
                if (testConfig->twoPeaksFlag == 1)
                {
                    if (aoaCfg.res.detObj2dAzimIdx[count] >= TEST_AOA_FFTSIZE / 2)
                        aoaSignPeakIdx = aoaCfg.res.detObj2dAzimIdx[count] - TEST_AOA_FFTSIZE;
                    else
                        aoaSignPeakIdx = aoaCfg.res.detObj2dAzimIdx[count];


                    if (abs(aoaSignPeakIdx - testConfig->aoaSignedAzimPeakIdxRef[refCount]) > MAX_PEAKIDX_ERROR)
                        // && (fabs(aoaCfg.res.detObj2dAzimIdx[ii] - testConfig->aoaSignedAzimPeakIdxRef[count])))
                    {

                        if (testPass)
                            test_print("\n");
                        printf("Cfar obj %d 2rd peakIdx : aoaOut=%d, ref=%d\n", ii,
                               aoaCfg.res.detObj2dAzimIdx[count], testConfig->aoaSignedAzimPeakIdxRef[refCount]);
                        finalResults = 0;
                        testPass = 0;
                    }

                    if (testConfig->numTxElevAntennas > 0)
                    {
                        elevAngleRef = 2.f * testConfig->aoaSignedElevPeakIdxRef[refCount] / (float)TEST_AOA_FFTSIZE / ELEV_SCALE_FACTOR;
                        elevAngleErr = fabs(elevAngleRef - aoaCfg.res.detObjElevationAngle[count]);

                        if (elevAngleErr > MAX_ELEVANGLE_ERROR)
                        {
                            if (testPass)
                                test_print("\n");
                            printf("Cfar obj %d 2rd ElevAngle : aoaOut=%f, ref=%f, error=%f\n",
                                   ii, aoaCfg.res.detObjElevationAngle[ii], elevAngleRef, elevAngleErr);
                            finalResults = 0;
                            testPass = 0;
                        }
                    }
                    /* x is range * sin */
                    range = (float)testConfig->cfarDetObjRangeIdx[ii] - aoaDynOutCompRxCfg.rangeBias;
                    x = range * 2.f * (float)aoaSignPeakIdx / (float)TEST_AOA_FFTSIZE;
                    if (testConfig->numTxElevAntennas > 0)
                    {
                        z = range * aoaCfg.res.detObjElevationAngle[count];
                        y = sqrtf(range * range - x * x - z * z);
                    }
                    else
                    {
                        z = 0.f;
                        y = range * sqrtf(1.f - 4.f * (float)aoaSignPeakIdx * (float)aoaSignPeakIdx /
                                          (float)TEST_AOA_FFTSIZE / (float)TEST_AOA_FFTSIZE);
                    }

                    if ((fabs(aoaCfg.res.detObjOut[count].x - x) > MAX_LOCATION_ERROR) ||
                        (fabs(aoaCfg.res.detObjOut[count].y - y) > MAX_LOCATION_ERROR) ||
                        (fabs(aoaCfg.res.detObjOut[count].z - z) > MAX_LOCATION_ERROR))
                    {
                        if (testPass)
                            test_print("\n");
                        printf("Cfar obj %d 2rd out x/y/z is : aoaOut=%f,%f,%f, ref=%f,%f,%f\n",
                               ii, aoaCfg.res.detObjOut[count].x, aoaCfg.res.detObjOut[count].y,
                               aoaCfg.res.detObjOut[count].z, x, y, z);
                        finalResults = 0;
                        testPass = 0;
                    }

                    count++;
                    refCount++;
                }
                else             //negtive two peak testing, one peak is not detected
                {
                    refCount++;
                }
            }
        }
    }

    count = 0;
    for (ii = 0; ii < testConfig->numCfarDetObj; ii++) //no aoa, directly output the cfar result
    {
        if ((!testConfig->morePeaksFlag) && (!testConfig->checkCfarDetObjFlag[ii]))
        {
            continue;
        }
        if (!testConfig->morePeaksFlag)
        {
            dopplerIdx = testConfig->cfarDetObjDopplerRefIdx[ii];
        }
        else
        {
            dopplerIdx = test_modCalc(ii * interval, testConfig->numDopplerBins);
            if (dopplerIdx >= testConfig->numDopplerBins / 2)
                dopplerIdx -= testConfig->numDopplerBins;
        }
        if (aoaCfg.res.detObjOut[count].velocity != (float)dopplerIdx)
        {
            if (testPass)
                test_print("\n");

            printf("Cfar obj %d 1st out vel is : aoaOut=%f, ref=%d\n", ii, aoaCfg.res.detObjOut[count].velocity, dopplerIdx);
            finalResults = 0;
            testPass = 0;
        }
        count++;
        if ((!testConfig->morePeaksFlag) && (testConfig->aoaNum3DfftPeaks[ii] == 2) && (testConfig->twoPeaksFlag == 1))
        {
            if (aoaCfg.res.detObjOut[count].velocity != (float)testConfig->cfarDetObjDopplerRefIdx[ii])
            {
                if (testPass)
                    test_print("\n");
                printf("Cfar obj %d 2rd out vel is : aoaOut=%f, ref=%d\n", ii,
                       aoaCfg.res.detObjOut[ii].velocity, testConfig->cfarDetObjDopplerRefIdx[ii]);
                finalResults = 0;
                testPass = 0;
            }
            count++;
        }
    }

    for (ii = 0; ii < outAoaProc.numAoADetectedPoints; ii++)
    {
       /* check some side information*/
       if ((aoaCfg.res.detObjOutSideInfo[ii].snr != 10.f) ||
           (aoaCfg.res.detObjOutSideInfo[ii].noise != 10.f))
       {
           if (testPass)
               test_print("\n");
            test_print("AOA %d sideInfo is : aoaOut=%d,%d\n", ii,
                          aoaCfg.res.detObjOutSideInfo[ii].snr,
                          aoaCfg.res.detObjOutSideInfo[ii].noise);
            finalResults = 0;
            testPass = 0;
        }
    }

    /* add heatmap checking */
    if (testConfig->heatMapFlag)
    {
        numAzimVirtualAnt = testConfig->numTxAzimAntennas * testConfig->numRxAntennas;

        for (ii = 0; ii < testConfig->numRangeBins; ii++)
        {
            for (jj = 0; jj < numAzimVirtualAnt; jj++)
            {

                heapmapDiff = abs(gStaticHeatmapRef[ii *numAzimVirtualAnt + jj].real -
                                  gStaticHeatmap[ii *numAzimVirtualAnt + jj].real );

                if (heapmapDiff > MAX_HEATMAP_ERROR)
                {
                    if (testPass)
                        test_print("\n");
                    test_print("heatmap bin %d, ant %d, ref=%d, aoa=%d, diff =%d real fail\n", ii, jj,
                                  gStaticHeatmapRef[ii *numAzimVirtualAnt + jj].real,
                                     gStaticHeatmap[ii *numAzimVirtualAnt + jj].real, heapmapDiff);
                    finalResults = 0;
                    testPass = 0;
                }

                heapmapDiff = abs(gStaticHeatmapRef[ii *numAzimVirtualAnt + jj].imag -
                                     gStaticHeatmap[ii *numAzimVirtualAnt + jj].imag );

                if (heapmapDiff > MAX_HEATMAP_ERROR)
                {
                    if (testPass)
                        test_print("\n");
                    test_print("heatmap bin %d, ant %d, ref=%d, aoa=%d diff = %d imag fail\n", ii, jj,
                                  gStaticHeatmapRef[ii *numAzimVirtualAnt + jj].imag,
                                     gStaticHeatmap[ii *numAzimVirtualAnt + jj].imag, heapmapDiff);
                    finalResults = 0;
                    testPass = 0;
                }
            }
        }
    }

    if (testPass)
    {
        test_print(" PASS \n");
    }
    else
    {
        test_print(" FAIL \n");
    }

    test_print("\n");
}

/**
*  @b Description
*  @n
*     Generate 2D windowing coefficients.
*
*  @retval None
*
*/
void Test_window2DCoef_gen(uint32_t numSamples, uint8_t winType, int32_t *winPtr)
{
    float ephyR, ephyI;
    float phi = 2 * PI_ / ((float)numSamples - 1);

    uint32_t ii;
    float a0;
    float winValue;
    int32_t winValueFixed;
    float cosValue, sinValue;
    float temp;

    cosValue = 1.f;
    sinValue = 0.f;

    ephyR = cos(phi);
    ephyI = sin(phi);
    a0 = 0.5f;

    if (winType == 1)
    {
        for (ii = 0; ii < numSamples; ii++)
        {
            winPtr[ii] = (1 << 17) - 1; //rec window, for debug
        }
    }
    else
    {
        /* hanning window */
        for (ii = 0; ii < numSamples; ii++)
        {
            winValue = a0 * (1.f - cosValue);       //0.5*(1-cos(2*pi*n/N-1))

            winValue = winValue * (1 << 17) + 0.5;     //Q17 format
            winValueFixed = (uint32_t)winValue;
            if (winValueFixed >= (1 << 17))
            {
                winValueFixed = (1 << 17) - 1;
            }
            temp = cosValue;
            cosValue = cosValue * ephyR - sinValue * ephyI;
            sinValue = temp * ephyI + sinValue * ephyR;

            //winPtr[ii] = (1<<17) - 1; //rec window, for debug
            winPtr[ii] = winValueFixed;
        }
    }
}

/**
*  @b Description
*  @n
*     test main task  : Generate the radar cuba data based on the test configuration, and generate the cfar detected
*                       object list. The cuba data and the detected object list are sent to AOA dpu. The DPU outputs
*                       the deteced object azimth and elevation angle, which are checked against the test configuration.
*                       If the differece is within the error threshold, the test is declared PASS, otherwise, it is declared
*                       FAIL. The tests cover 5 TX/RX antenna setups, 1/1, 1/2, 1/4, 2/4 and 3/4.
*
*  @retval None
*
*/
void Test_task(void* args)
{
    uint32_t ii, jj, kk, kk1;
    uint64_t taskStartTime, taskEndTime;

    char featureName[200];
    uint16_t numAntennas[TEST_ANTENNA_LOOP][3] = {
        /* numTxAzim, numTxElev, numRx*/
        {1, 0, 1}
       ,{1, 0, 2}
       ,{1, 0, 4}
       ,{2, 0, 4}
       ,{2, 1, 4}
#if defined(SOC_AWR2944) || defined(SOC_AWR2X44P)
       ,{3, 1, 4}
       ,{3, 0, 4}
#endif
    };

    uint32_t numDopplerBin[TEST_DOPPLER_BIN_SIZE] = {2, 4, 8, 16, 32, 64, 128, 256, 512};
    uint32_t numRangeBin[TEST_RANGE_BIN_SIZE] = { 16, 32, 64, 128, 256, 512, 1024 };
    uint32_t numTests;
    int32_t   retVal;
    uint16_t dopplerLoopStep;
    uint16_t numRangeBinSize;
    uint16_t idx;
    uint32_t maxNumChirps,mm;

    Drivers_open();
    Board_driversOpen();

    /* Initialize test logger */
    MCPI_Initialize();

    finalResults = 1;
    taskStartTime=ClockP_getTimeUsec();
    test_print("...... Initialization ...... \n");

    /* hwa, dpu inialization */
    aoaProc_test_hwainit();
    aoaProc_test_dpuInit();

    /* aoa dpu config*/
    aoaProc_test_dpuConfig();

    test_print("aoa Proc DPU Test start ...... \n");

    /*for tx/rx1,1, only run 5 doppler size, each with 1 range bin size */
    numTests = TEST_ANTENNA_LOOP;

    /* antenne combination * doppler size, 2 range bin size*/
    numTests += (TEST_ANTENNA_LOOP - 1) * TEST_DOPPLER_BIN_SIZE * 2;

    /* for each doppler bin size, chirp even, odd, maximum */
    numTests = numTests * 3;

    /* add doppler size = 1024 tests, for each tx/rx setting 1/2 and 1/4, add
       3 tests with different range bin, and even, odd, maximum chirp number if 2/4,
       3/4 maximum chirp is limited to <512 due to DPU_AOAPROCHWA_EEXCEEDHWAMEM error,
       can not get to doppler bin = 1024 */
    numTests += (2 * 3);

    printf("\n...... %d Tests to be run .......\n\n", numTests);

    testCount = 0;
    for (ii = 0; ii < TEST_ANTENNA_LOOP; ii++) /* antenna loop */
    {
        dopplerLoopStep = 1;
        numRangeBinSize = 2;
        if (ii == 0)
        {
            dopplerLoopStep = 2;
            numRangeBinSize = 1; //for 1tx, 1rx, 1 range bin size
        }

        for (jj = 0; jj < TEST_DOPPLER_BIN_SIZE; jj += dopplerLoopStep) /* doppler size loop*/
        {
            aoaTestCfg.numTxAzimAntennas = numAntennas[ii][0];
            aoaTestCfg.numTxElevAntennas = numAntennas[ii][1];
            aoaTestCfg.numTxAntennas = aoaTestCfg.numTxAzimAntennas + aoaTestCfg.numTxElevAntennas;
            aoaTestCfg.numRxAntennas = numAntennas[ii][2];
            aoaTestCfg.numDopplerBins = numDopplerBin[jj];       //NOT TEST OVERSAMPLING

            maxNumChirps = 16384 / aoaTestCfg.numTxAntennas / aoaTestCfg.numRxAntennas / 4;
            if (maxNumChirps > aoaTestCfg.numDopplerBins)
            {
                maxNumChirps = aoaTestCfg.numDopplerBins;
            }

            for (mm = 0; mm < 3; mm++)
            {
                for (kk = 0; kk < numRangeBinSize; kk++) /* range bin size loop, random pick range bin size */
                {
                    /* randomly find the range bin sizes*/
                    if (aoaTestCfg.numTxAntennas == 1)
                        //||( (aoaTestCfg.numTxAntennas == 2) &&(aoaTestCfg.numRangeBins==2)))
                    {
                        if (aoaTestCfg.numDopplerBins < 64)
                        {
                            kk1 = testCount + kk * 5;
                            kk1 = test_modCalc(kk1, TEST_RANGE_BIN_SIZE);
                        }
                        else
                        {
                            kk1 = testCount + kk * ((11 - jj) / 2);
                            kk1 = test_modCalc(kk1, (11 - jj));  //maximum range size is 512,256,... for doppler size >=64
                        }
                    }
                    else if (aoaTestCfg.numTxAntennas == 2)
                    {
                        if (aoaTestCfg.numDopplerBins < 32)
                        {
                            if (aoaTestCfg.numDopplerBins == 2)
                                kk1 = testCount;
                            else
                                kk1 = testCount + kk * 4;
                            kk1 = test_modCalc(kk1, TEST_RANGE_BIN_SIZE);
                        }
                        else if (aoaTestCfg.numDopplerBins < 256)
                        {
                            kk1 = testCount + kk * ((11 - jj) / 2);
                            kk1 = test_modCalc(kk1, (11 - jj));  //maximum range size is 512,256,... for doppler size >=64
                        }
                        else  //jj = 8, 512
                        {
                            kk1 = testCount;
                            kk1 = test_modCalc(kk1, (10 - jj));  //maximum range size is 256,... for doppler size >=256
                        }
                    }
                    else // (aoaTestCfg.numTxAntennas == 3)
                    {
                        if (aoaTestCfg.numDopplerBins < 32)
                        {
                            if (aoaTestCfg.numDopplerBins < 8)
                            {
                                kk1 = testCount;
                            }
                            else if (aoaTestCfg.numDopplerBins == 16)
                            {
                                kk1 = testCount + 2;
                            }
                            else
                            {
                                kk1 = testCount + kk * 3;
                            }

                            kk1 = test_modCalc(kk1, TEST_RANGE_BIN_SIZE);
                        }
                        else if (aoaTestCfg.numDopplerBins < 128)
                        {
                            kk1 = testCount + kk * ((11 - jj) / 2);
                            kk1 = test_modCalc(kk1, (10 - jj));  //maximum range size is 256,...
                        }
                        else if (aoaTestCfg.numDopplerBins < 512)
                        {
                            kk1 = testCount + kk * 13;
                            kk1 = test_modCalc(kk1, (10 - jj)); //maximum range size is 256,...
                        }
                        else
                        {
                            kk1 = testCount;
                            kk1 = test_modCalc(kk1, (10 - jj));;  //maximum range size is 256,...
                        }
                    }

                    aoaTestCfg.numRangeBins = numRangeBin[kk1];

                    /* set up the number of chirps <= doppler bin size*/
                    if (aoaTestCfg.numDopplerBins == 2)
                    {
                        aoaTestCfg.numChirps = 2;
                    }
                    else if (aoaTestCfg.numDopplerBins == 4)
                    {
                        aoaTestCfg.numChirps = 3 + (mm & 0x1);
                    }
                    else if (aoaTestCfg.numDopplerBins == 8)
                    {
                        aoaTestCfg.numChirps = 8 - (mm & 0x3);
                    }
                    else if (aoaTestCfg.numDopplerBins == 16)
                    {
                        aoaTestCfg.numChirps = 16 - ((mm + kk) & 0x7);
                    }
                    else
                    {
                        if (mm == 0)
                        {
                            aoaTestCfg.numChirps = maxNumChirps;
                        }
                        else
                        {
                            aoaTestCfg.numChirps = maxNumChirps - (mm + kk) * 2 - (mm - 1) * 3;
                        }

                    }

                    /* multiple peaks */
                    aoaTestCfg.twoPeaksFlag = 0;
                    if ((aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas == 2) ||
                        (aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas == 1))
                    {
                        aoaTestCfg.twoPeaksFlag = 0;
                    }
                    else if ((testCount & 0x7) == 0)
                    {
                        aoaTestCfg.twoPeaksFlag = 1;
                        /* add several tests for two peaks, but set threshold high, so only sigle peak is detected */
                        if ((testCount == 104) || (testCount == 128) || (testCount == 192) ||
                            (testCount == 200) || (testCount == 216))
                        {
                            aoaTestCfg.twoPeaksFlag = 2;
                        }
                    }

                    //heat map
                    if (aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas == 1)
                    {
                        aoaTestCfg.heatMapFlag = false;
                    }
                    else
                    {
                        aoaTestCfg.heatMapFlag = true;
                    }

                    /* generate the windowing coefficients */
                    if (aoaTestCfg.numChirps == 2)
                    {
                        aoaTestCfg.winType = 1; //rec
                    }
                    else
                    {
                        aoaTestCfg.winType = 0;
                    }

                    /* generate the cfar det obj list */
                    pCfarDetObjList = (DPIF_CFARDetList *) &gCfarDetObjListBuf[0];
                    pAoaDetObj = (DPIF_PointCloudCartesian *) &gAoaDetObjBuf[0];
                    Test_aoaDpu_cfarListGen(&aoaTestCfg);

                    /* check the min and max angle */
                    aoaTestCfg.minmaxAngleFlag = 0;
                    if ((aoaTestCfg.numCfarDetObj >= 4) && ((testCount & 0x3) <= 1) && (aoaTestCfg.numRxAntennas != 1))
                    {
                        aoaTestCfg.minmaxAngleFlag = aoaTestCfg.numCfarDetObj - 3;
                    }

                    /* check more peaks */
                    aoaTestCfg.morePeaksFlag = 0;
                    if ((!aoaTestCfg.minmaxAngleFlag) && (!aoaTestCfg.twoPeaksFlag))
                    {
                        if ((aoaTestCfg.numTxAntennas < 3) && (aoaTestCfg.numRxAntennas > 1) &&
                            (aoaTestCfg.numRangeBins * aoaTestCfg.numDopplerBins > 256) && ((testCount & 0x3) == 0))
                        {
                            if (aoaTestCfg.numRangeBins == 16)  //minimum range bin
                            {
                                if (aoaTestCfg.numDopplerBins > 32)
                                {
                                    aoaTestCfg.morePeaksFlag = 1;
                                }
                            }
                            else
                            {
                                aoaTestCfg.morePeaksFlag = 1;
                            }
                        }
                        if ((aoaTestCfg.numTxAntennas == 3) && (aoaTestCfg.numRangeBins *
                             aoaTestCfg.numDopplerBins > 64) && ((testCount & 0x3) == 0))
                        {
                            aoaTestCfg.morePeaksFlag = 1;
                        }
                    }
#ifdef SUBSYS_DSS
                    /* for running on dsp, not enough memory for L2, move cfarobj list, and aoa output cloud point into L3,
					   limit cuba data size to 65536 in L2 */
                    if (aoaTestCfg.numRangeBins * aoaTestCfg.numDopplerBins *
                        aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas > 65536)
                    {
                        aoaTestCfg.morePeaksFlag = 0;
                    }
#endif
                    if (aoaTestCfg.morePeaksFlag)
                    {
                        if (aoaTestCfg.numTxAntennas == 3 || aoaTestCfg.numTxAntennas == 4)
                        {
                            aoaTestCfg.numCfarDetObj = 64 + test_modCalc(testCount, 20);
                            aoaTestCfg.numAoaOutObj = aoaTestCfg.numCfarDetObj; //no multiple peaks, and no min/max angles
                        }
                        else
                        {
                            aoaTestCfg.numCfarDetObj = 256 + test_modCalc(testCount, 22);
                            aoaTestCfg.numAoaOutObj = aoaTestCfg.numCfarDetObj; //no multiple peaks, and no min/max angles
                        }
                    }

                    /* add the extion velocity test*/
                    aoaTestCfg.extVelocityFlag = 0;   //ext velocity is disabled
                    if (aoaTestCfg.numTxAntennas > 1)
                    {
                        if ((!aoaTestCfg.minmaxAngleFlag) && (!aoaTestCfg.twoPeaksFlag))
                        {
                            aoaTestCfg.extVelocityFlag = 1;   //ext velocity is enabled
                            if ((aoaTestCfg.numTxAntennas == 2) && (aoaTestCfg.morePeaksFlag) &&
                                (aoaTestCfg.numDopplerBins == 4))
                            {
                                aoaTestCfg.extVelocityFlag = 0;
                            }
                        }
                    }
                    if (!aoaTestCfg.morePeaksFlag)
                    {
                        if(aoaTestCfg.numDopplerBins > 2)
                        {
                            //every odd test number enable clutter removal
                            aoaTestCfg.staticClutterRemovalFlag = testCount & 0x1;
                        }
                        else
                        {
                            aoaTestCfg.staticClutterRemovalFlag = 0;
                        }
                    }
                    else
                    {
                         aoaTestCfg.staticClutterRemovalFlag = 0;
                    }

                    test_print("\nTest #%d  start\r\n", testCount);

                    /* generate 2D windowing coefficients */
                    pTestDopplerWindow = gTestDopplerWindow;
                    aoaCfg.res.hwaCfg.window = pTestDopplerWindow;
                    Test_window2DCoef_gen(aoaTestCfg.numChirps, aoaTestCfg.winType, pTestDopplerWindow);

#if defined SUBSYS_DSS
					//due to not enough memory for L2 on dsp, use L3 memory
                    if (aoaTestCfg.morePeaksFlag)
                    {
                        uint32_t tempSize;
                        pAoaDetObj = (DPIF_PointCloudCartesian *)&gTestRadarCube[65536 +4];
                        tempSize = sizeof(DPIF_PointCloudCartesian) *  TEST_MAX_NUM_CFARDETOBJ_DPU;
                        tempSize = (tempSize + 3) / 4 * 4;
                        pCfarDetObjList = (DPIF_CFARDetList *)&gTestRadarCube[65536 + 4 + tempSize];

                    }
#endif
                    /* set up the AOA dpu configuration structure */
                    Test_setProfile(&aoaTestCfg);

                    sprintf(featureName, "%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d",
                        ":TxAzim", aoaTestCfg.numTxAzimAntennas,
                        ":TxElev", aoaTestCfg.numTxElevAntennas,
                        ":Rx", aoaTestCfg.numRxAntennas,
                        ":Chp", aoaTestCfg.numChirps,
                        ":D", aoaTestCfg.numDopplerBins,
                        ":R", aoaTestCfg.numRangeBins,
                        ":extVel", aoaTestCfg.extVelocityFlag,
                        ":clutRem", aoaTestCfg.staticClutterRemovalFlag);

                    test_print("Test feature : %s\n", featureName);

                    if (!aoaTestCfg.morePeaksFlag)
                    {
                        test_print("CFAR Det Peaks: %d [", aoaTestCfg.numCfarDetObj);
                        for (idx = 0; idx < aoaTestCfg.numCfarDetObj - 1; idx++)
                        {
                            if (!aoaTestCfg.twoPeaksFlag)
                                test_print("%d,%d,%d  ",
                                              aoaTestCfg.cfarDetObjRangeIdx[idx],
                                              aoaTestCfg.cfarDetObjDopplerRefIdx[idx],
                                              aoaTestCfg.aoaSignedAzimPeakIdxRef[idx]);
                            else
                                test_print("%d,%d  ", aoaTestCfg.cfarDetObjRangeIdx[idx],
                                              aoaTestCfg.cfarDetObjDopplerRefIdx[idx]);
                        }
                        /* print the last one*/
                        if (!aoaTestCfg.twoPeaksFlag)
                            test_print("%d,%d,%d]\n",
                                          aoaTestCfg.cfarDetObjRangeIdx[aoaTestCfg.numCfarDetObj - 1],
                                          aoaTestCfg.cfarDetObjDopplerRefIdx[aoaTestCfg.numCfarDetObj - 1],
                                          aoaTestCfg.aoaSignedAzimPeakIdxRef[aoaTestCfg.numCfarDetObj - 1]);
                        else
                            test_print("%d,%d]\n",
                                          aoaTestCfg.cfarDetObjRangeIdx[aoaTestCfg.numCfarDetObj - 1],
                                          aoaTestCfg.cfarDetObjDopplerRefIdx[aoaTestCfg.numCfarDetObj - 1]);
                    }
                    else
                    {
                        test_print("CFAR Det Peaks: %d \n", aoaTestCfg.numCfarDetObj);
                    }

                    /* generates the 1D fft output, radar cuba data */
                    Test_aoaDpu_cubaDataGen(&aoaTestCfg);
//#if defined SUBSYS_MSS
					/* write back to L3 memory*/
					CacheP_wb(gTestRadarCube,
						sizeof(cmplx16ImRe_t) * aoaTestCfg.numRangeBins * aoaTestCfg.numDopplerBins *
						aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas, CacheP_TYPE_ALLD);
#if TEST_AOA_DEBUG
                    /* THIS IS FOR DEBUG */
                    Test_debug_cubadataverify(&aoaTestCfg);
#endif
//#endif

                    /* clear the heat map memory */
                    memset(gStaticHeatmap, 0, sizeof(cmplx16ImRe_t)*TEST_MAX_NUM_RANGE_BIN * 8);
#ifdef SUBSYS_MSS
					CacheP_wb(gStaticHeatmap,
						     sizeof(cmplx16ImRe_t) * TEST_MAX_NUM_RANGE_BIN * 8, CacheP_TYPE_ALLD);
#endif
                    /* call aoa dpu configuration */
                    retVal = DPU_AoAProcHWA_config(aoaProcHandle, &aoaCfg);
                    if (retVal < 0)
                    {
                        if ((DPU_AOAPROCHWA_EEXCEEDHWAMEM) && (aoaTestCfg.numTxAntennas==3) &&
                            (aoaTestCfg.numDopplerBins==512))
                        {
                            /* not support 3x4, doppler is 512, not fit into 16k*/
                            test_print("AOA DPU NOT SUPPORT, KNOWN ISSUE\n");
                            testCount++;
                            continue;
                        }
                        else
                        {
                            test_print("AOA DPU failed to configure. Error %d.\n", retVal);
                            DebugP_assert(0);
                            return;
                        }
                    }

                    /* call aoa dpu process */
                    retVal = DPU_AoAProcHWA_process(aoaProcHandle, aoaTestCfg.numCfarDetObj, &outAoaProc);
                    if (retVal < 0)
                    {
                        test_print("AOA DPU failed to run. Error %d.\n", retVal);
                        DebugP_assert(0);
                        return;
                    }

                    /* check the AOA DPU output */
#ifdef SUBSYS_MSS
                    /* Check if DPU is obeying design rules by invalidating  output buffers */
                    CacheP_inv(aoaCfg.res.azimuthStaticHeatMap,
                              sizeof(cmplx16ImRe_t) * aoaCfg.res.azimuthStaticHeatMapSize, CacheP_TYPE_ALLD);
                    CacheP_inv(&aoaCfg.res.detObjOut[0],
                              outAoaProc.numAoADetectedPoints * sizeof(DPIF_PointCloudCartesian), CacheP_TYPE_ALLD);
                    CacheP_inv(&aoaCfg.res.detObjOutSideInfo[0],
                              outAoaProc.numAoADetectedPoints * sizeof(DPIF_PointCloudSideInfo), CacheP_TYPE_ALLD);
                    CacheP_inv(&aoaCfg.res.detObjElevationAngle[0],
                              outAoaProc.numAoADetectedPoints * sizeof(float), CacheP_TYPE_ALLD);
                    CacheP_inv(&aoaCfg.res.detObj2dAzimIdx[0],
                              outAoaProc.numAoADetectedPoints * sizeof(uint8_t), CacheP_TYPE_ALLD);
#endif
                    test_print("Check the AOA output :  ");
                    Test_printResults(&aoaTestCfg);
                    test_print("Test #%d finished!\n\r", testCount);


                    testCount++;

                } /*end of number of chirps */
            } /* end of range bin loop*/
        } /* end of doppler bin loop */
    }   /* end of antenna loop */

    /* add doppler size = 1024 test */
    for (ii = 1; ii < 3; ii++) /* antenna loop, test 1/2, 1/4 */
    {
        aoaTestCfg.numTxAzimAntennas = numAntennas[ii][0];
        aoaTestCfg.numTxElevAntennas = numAntennas[ii][1];
        aoaTestCfg.numTxAntennas = aoaTestCfg.numTxAzimAntennas + aoaTestCfg.numTxElevAntennas;

        aoaTestCfg.numDopplerBins = 1024;
        //maxNumChirps = 16384 / aoaTestCfg.numTxAntennas / aoaTestCfg.numRxAntennas / 4;
        //if (maxNumChirps > aoaTestCfg.numDopplerBins)
            maxNumChirps = aoaTestCfg.numDopplerBins;
        for (mm = 0; mm < 3; mm++)
        {

            if (aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas == 2)
            {
                kk1 = test_modCalc(testCount, 4);    //1/2, maximum is 128 //max is 128
            }
            else if (aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas == 4)
            {
                kk1 = test_modCalc(testCount, 3);
            }
          //  else if (aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas == 8)
          //  {
           //     kk1 = test_modCalc(testCount, 2);
           // }
           // else
           //     kk1 = 0; //for tx*rx=12, max range bin size is 16
            aoaTestCfg.numRangeBins = numRangeBin[kk1];
            aoaTestCfg.numChirps = maxNumChirps - ((testCount & 0x3) * 13);
            aoaTestCfg.twoPeaksFlag = 0;
            aoaTestCfg.heatMapFlag = true;
            aoaTestCfg.winType = testCount & 0x1;
            /* generate the cfar det obj list */
            pCfarDetObjList = (DPIF_CFARDetList *) &gCfarDetObjListBuf[0];
            pAoaDetObj = (DPIF_PointCloudCartesian *) &gAoaDetObjBuf[0];
            Test_aoaDpu_cfarListGen(&aoaTestCfg);
            aoaTestCfg.minmaxAngleFlag = 0;
            aoaTestCfg.morePeaksFlag = 0;
            aoaTestCfg.extVelocityFlag = 0;    //only 1 tx,
            test_print("\nTest #%d  start\r\n", testCount);
            sprintf(featureName, "%s%d%s%d%s%d%s%d%s%d%s%d%s%d",
                ":TxAzim", aoaTestCfg.numTxAzimAntennas,
                ":TxElev", aoaTestCfg.numTxElevAntennas,
                ":Rx", aoaTestCfg.numRxAntennas,
                ":Chp", aoaTestCfg.numChirps,
                ":D", aoaTestCfg.numDopplerBins,
                ":R", aoaTestCfg.numRangeBins,
                ":extVel", aoaTestCfg.extVelocityFlag);
            test_print("Test feature : %s\n", featureName);
            test_print("CFAR Det Peaks: %d [", aoaTestCfg.numCfarDetObj);
            for (idx = 0; idx < aoaTestCfg.numCfarDetObj - 1; idx++)
            {
                if (!aoaTestCfg.twoPeaksFlag)
                    test_print("%d,%d,%d  ",
                                  aoaTestCfg.cfarDetObjRangeIdx[idx],
                                  aoaTestCfg.cfarDetObjDopplerRefIdx[idx],
                                  aoaTestCfg.aoaSignedAzimPeakIdxRef[idx]);
                else
                    test_print("%d,%d  ",
                                  aoaTestCfg.cfarDetObjRangeIdx[idx],
                                  aoaTestCfg.cfarDetObjDopplerRefIdx[idx]);
            }
            if (!aoaTestCfg.twoPeaksFlag)
                test_print("%d,%d,%d]\n",
                              aoaTestCfg.cfarDetObjRangeIdx[idx],
                              aoaTestCfg.cfarDetObjDopplerRefIdx[idx],
                              aoaTestCfg.aoaSignedAzimPeakIdxRef[idx]);
            else
                test_print("%d,%d]\n",
                              aoaTestCfg.cfarDetObjRangeIdx[idx],
                              aoaTestCfg.cfarDetObjDopplerRefIdx[idx]);
            /* generate 2D windowing coefficients */
#if defined SUBSYS_MSS
            pTestDopplerWindow = gTestDopplerWindow;

#endif
#if defined SUBSYS_DSS
            pTestDopplerWindow = (int32_t *)&gStaticHeatmapRef[128 * 4];
#endif
            aoaCfg.res.hwaCfg.window = pTestDopplerWindow;
            Test_window2DCoef_gen(aoaTestCfg.numChirps, aoaTestCfg.winType, pTestDopplerWindow);
            /* generates the 1D fft output, radar cuba data */
            Test_aoaDpu_cubaDataGen(&aoaTestCfg);
#ifdef SYBSYS_MSS
			/* write back to L3 memory*/
			CacheP_wb(gTestRadarCube,
				sizeof(cmplx16ImRe_t) * aoaTestCfg.numRangeBins * aoaTestCfg.numDopplerBins *
				aoaTestCfg.numTxAntennas * aoaTestCfg.numRxAntennas, CacheP_TYPE_ALLD);
#endif

            /* set up the AOA dpu configuration structure */
            Test_setProfile(&aoaTestCfg);
#if TEST_AOA_DEBUG
            /* THIS IS FOR DEBUG */
           Test_debug_cubadataverify(&aoaTestCfg);
#endif
            /* clear the heat map memory */
            memset(gStaticHeatmap, 0, sizeof(cmplx16ImRe_t)*TEST_MAX_NUM_RANGE_BIN * TEST_MAX_NUM_AZIMUTH_VIRTUAL_ANTENNAS);
#ifdef SUBSYS_MSS
			CacheP_wb(gStaticHeatmap,
				sizeof(cmplx16ImRe_t) * TEST_MAX_NUM_RANGE_BIN * TEST_MAX_NUM_AZIMUTH_VIRTUAL_ANTENNAS, CacheP_TYPE_ALLD);
#endif

            /* call aoa dpu configuration */
            retVal = DPU_AoAProcHWA_config(aoaProcHandle, &aoaCfg);
            if (retVal < 0)
            {
                test_print("AOA DPU failed to configure. Error %d.\n", retVal);
                DebugP_assert(0);
                return;
            }
			retVal = DPU_AoAProcHWA_process(aoaProcHandle, aoaTestCfg.numCfarDetObj, &outAoaProc);
            if (retVal < 0)
            {
                test_print("AOA DPU failed to run. Error %d.\n", retVal);
                DebugP_assert(0);
                return;
            }

            test_print("Check the AOA output :  ");
#ifdef SUBSYS_MSS
            /* Check if DPU is obeying design rules by invalidating  output buffers */
            CacheP_inv(aoaCfg.res.azimuthStaticHeatMap,
                       sizeof(cmplx16ImRe_t) * aoaCfg.res.azimuthStaticHeatMapSize, CacheP_TYPE_ALLD);
            CacheP_inv(&aoaCfg.res.detObjOut[0],
                       outAoaProc.numAoADetectedPoints * sizeof(DPIF_PointCloudCartesian), CacheP_TYPE_ALLD);
            CacheP_inv(&aoaCfg.res.detObjOutSideInfo[0],
                       outAoaProc.numAoADetectedPoints * sizeof(DPIF_PointCloudSideInfo), CacheP_TYPE_ALLD);
            CacheP_inv(&aoaCfg.res.detObjElevationAngle[0],
                      outAoaProc.numAoADetectedPoints * sizeof(float), CacheP_TYPE_ALLD);
            CacheP_inv(&aoaCfg.res.detObj2dAzimIdx[0],
                      outAoaProc.numAoADetectedPoints * sizeof(uint8_t), CacheP_TYPE_ALLD);
#endif
            Test_printResults(&aoaTestCfg);
            testCount++;
        }
    }

    HWA_close(hwaHandle);
    retVal = DPU_AoAProcHWA_deinit(aoaProcHandle);
    if (retVal != 0)
    {
        test_print("AOA DPU failed to deinit. Error %d.\n", retVal);
        DebugP_assert(0);
        return;
    }

#if  TEST_AOA_DEBUG
    test_print("max cube size is %d, test id is %d\n", maxCubeSize, testCaseId);
#endif

    test_print("----------------------------------------\n\n");
    if (finalResults == 1)
    {
        test_print ("All tests have passed!!\n");
        test_print("All Tests PASSED!\n");
        MCPI_setFeatureTestResult("AOA_DPU", MCPI_TestResult_PASS);
    }
    else
    {
        test_print("Test FAILED!\n");
        MCPI_setFeatureTestResult("AOA_DPU", MCPI_TestResult_FAIL);
    }

    taskEndTime=ClockP_getTimeUsec();
    test_print("Test Time Cycles: %lld\n",taskEndTime-taskStartTime);
    test_print("Test Finished\n");


    MCPI_setTestResult();

    Board_driversClose();
    Drivers_close();

    test_print ("--- Test Completed ---\n");

    vTaskDelete(NULL);
}

/**
*  @b Description
*  @n
*     test main function
*
*  @retval None
*
*/
int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( Test_task,   /* Pointer to the function that implements the task. */
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
