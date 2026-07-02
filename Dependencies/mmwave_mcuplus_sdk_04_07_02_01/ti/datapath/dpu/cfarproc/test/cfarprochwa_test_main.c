/**
 *   @file  cfarprochwa_test_main.c
 *
 *   @brief
 *      Unit Test code for cfar hwa dpu processing.
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
#include <kernel/dpl/CacheP.h>
#include <drivers/edma.h>
#ifdef SUBSYS_DSS
#include <ti/datapath/dpu/cfarproc/test/dssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/cfarproc/test/dssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/cfarproc/test/dssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/cfarproc/test/dssgenerated/ti_board_open_close.h>
#endif

#ifdef SUBSYS_MSS
#include <ti/datapath/dpu/cfarproc/test/mssgenerated/ti_drivers_config.h>
#include <ti/datapath/dpu/cfarproc/test/mssgenerated/ti_board_config.h>
#include <ti/datapath/dpu/cfarproc/test/mssgenerated/ti_drivers_open_close.h>
#include <ti/datapath/dpu/cfarproc/test/mssgenerated/ti_board_open_close.h>
#endif
#include <kernel/dpl/DebugP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmwave SDK include files */
#include <ti/common/syscommon.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/datapath/dpu/cfarproc/cfarprochwa.h>
#include <ti/utils/mathutils/mathutils.h>
#include <ti/utils/randomdatagenerator/gen_rand_data.h>

#define TEST_MAX_DETMATRIX_SIZE   (1024 * 32)    //numRangeBin*numDopplerBin*sizeof(uint16_t)<=64KB
#define TEST_CFAR_DATAIN_LEN 1024
#define TEST_MAX_NUM_CFARDETOBJ_DPU_LIST 300
#define TEST_DOPPLER_BIN_SIZE 10 // test doppler size covers from 2 to 1024
#define TEST_RANGE_BIN_SIZE 7   //tested range bin size from 16 to 1024
#define TEST_CFAR_WIN_LEN 6
#define TEST_CFAR_GUARD_LEN 7
#define TEST_NOISE_THRESHOLD  800

/**
* @brief
*  CFAR Detected object configuration structure for reference.
*
*/
typedef struct cfarDetObjRef_t
{
    uint16_t rangeIdx;      /*!< @brief detected object range index */
    uint16_t dopplerIdx;    /*!< @brief detected object doppler index */
    uint16_t peakAbs;       /*!< @brief detected object signal power */
    uint16_t noise;         /*!< @brief detected object noise power */
} cfarDetObjRef_t;

/*************** radar det matrix ************************/
uint16_t testDetMatrix[TEST_MAX_DETMATRIX_SIZE]__attribute__((aligned(64), section(".l3ram")));

/*************** cfar dpu detected object output ****************/
#ifdef SUBSYS_MSS
#define TEST_DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT \
             DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT \
             DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT_DSP
#endif

uint8_t gCfarDetObjListBuf[CSL_NEXT_MULTIPLE_OF(TEST_MAX_NUM_CFARDETOBJ_DPU_LIST * sizeof(DPIF_CFARDetList), \
                                                TEST_DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(TEST_DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT), section(".l3ram")));
DPIF_CFARDetList *gCfarDetObjList = (DPIF_CFARDetList *) &gCfarDetObjListBuf[0];

/* cfar DPU internal buffer for doppler cfar is enabled,  to save the detected objects */
#ifdef SUBSYS_MSS
#define TEST_DPU_CFARPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT \
             DPU_CFARPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT_R5F
#else
#define TEST_DPU_CFARPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT \
             DPU_CFARPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT_DSP
#endif


uint8_t gCfarDopplerDetOutBitMaskBuf[CSL_NEXT_MULTIPLE_OF(
             TEST_MAX_DETMATRIX_SIZE/32 * sizeof(uint32_t), \
             TEST_DPU_CFARPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT)] \
        __attribute__ ((aligned(TEST_DPU_CFARPROCHWA_DOPPLER_DET_OUT_BIT_MASK_BYTE_ALIGNMENT), section(".l3ram")));
uint32_t *gCfarDopplerDetOutBitMask = (uint32_t *) &gCfarDopplerDetOutBitMaskBuf[0];

/* the reference doppler cfar outputs*/
uint8_t gCfarDopplerDetOutRef[TEST_MAX_DETMATRIX_SIZE]__attribute__((aligned(64), section(".l3ram")));

/* the reference cfar outputs*/
cfarDetObjRef_t gCfarDetObjRef[TEST_MAX_NUM_CFARDETOBJ_DPU_LIST]__attribute__((aligned(64), section(".l3ram")));

/* for doppler and range cfar, fov commands*/
uint8_t gCfarDetObjCheck[TEST_MAX_NUM_CFARDETOBJ_DPU_LIST]__attribute__((aligned(64), section(".l3ram")));
uint32_t numDetPeaksRef;
uint32_t numCheckPeaksRef;

uint16_t gCfarInputDataTemp[TEST_CFAR_DATAIN_LEN]__attribute__((aligned(64), section(".coreLocalRam")));

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (5U)
#define APP_TASK_STACK_SIZE  (8*1024)

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

/* Task Stack variables.*/
StackType_t gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(64)));

HWA_Handle  hwaHandle;
volatile uint8_t subFrameCount = 0;

/**
* @brief
*  CFAR reference configuration.
*
*/
typedef struct cfarConfig_Test
{
    uint8_t aveMode;      /*!< @brief CFAR mode =0, average, =1: SO, =2: GO, and in HWA 2.0: =3: OS */
    uint8_t winLen;       /*!< @brief CFAR window length */
    uint8_t guardLen;     /*!< @brief CFAR guard length */
    uint8_t cyclicMode;   /*!< @brief CFAR cyclic mode =0: non-cyclic, = 1: cyclic mode */

    uint8_t osKvalue;     /*!< @brief Only for CFAR OS, value of the ordered statistic K */
    uint8_t osEdgeKscaleEnable;     /*!< @brief Only for CFAR OS non-cyclic mode, scale K value for edge samples */

}cfarConfig_Test;

/**
* @brief
*  CFAR DPU unit test test configuration structure.
*
*/
typedef struct cfarProcDpuTestConfig_t
{
    uint16_t numRangeBins;       /*!< @brief number of range bin */
    uint16_t numDopplerBins;     /*!< @brief number of doppler bin */
    uint8_t dopplerCfarEnabled;  /*!< @brief Flag for dppler cfar enabled  or disabled */
    cfarConfig_Test cfarRangeCfg;  /*!< @brief cfar configuration for range bin */
    cfarConfig_Test cfarDopplerCfg; /*!< @brief cfar configuration for doppler bin */
    uint8_t cfarRangePeakPruning;   /*!< @brief peak group for range bin cfar */
    uint8_t cfarDopplerPeakPruning;  /*!< @brief peak group for doppler bin cfar */
    uint8_t dpuControlFlag;          /*!< @brief CFAR dpu command 0-rangefov, 1-dopplerfov, 2-doppler cfar, 3-range cfar */
}cfarProcDpuTestConfig_t;

#define EDMA_NUM_DMA_CHANNELS            64

#define DPU_CFAR_PROC_EDMAIN_CH          EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ4
#define DPU_CFAR_PROC_EDMAIN_SIG_CH      EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ5
#define DPU_CFAR_PROC_EDMAIN_SHADOW      (EDMA_NUM_DMA_CHANNELS + 25U)
#define DPU_CFAR_PROC_EDMAIN_EVENT_QUE   0
#define DPU_CFAR_PROC_EDMAIN_SIG_SHADOW      (EDMA_NUM_DMA_CHANNELS + 26U)
#define DPU_CFAR_PROC_EDMAIN_SIG_EVENT_QUE   0


DPU_CFARProcHWA_Config cfarDpuCfg;
DPU_CFARProcHWA_Handle cfarDpuHandle;
DPU_CFARProc_CfarCfg cfarRangeCfg;
DPU_CFARProc_CfarCfg cfarDopplerCfg;
DPU_CFARProc_FovCfg fovRangeCfg;
DPU_CFARProc_FovCfg fovDopplerCfg;
int32_t hwaMemBankSize;
uint32_t hwaMemBankAddr[SOC_HWA_NUM_MEM_BANKS];
int32_t numBanksForInput;

cfarProcDpuTestConfig_t cfarTestCfg;
uint32_t numDetObjRef;
DPU_CFARProcHWA_OutParams cfarDpuOut;

MCPI_LOGBUF_INIT(512);

uint8_t finalResults;
uint32_t testCount;

/**
*  @b Description
*  @n
*    Initialize the HWA and EDMA.
*
*  @retval None
*
*/
void cfarProc_test_hwaInit()
{
    int32_t             errorCode;
    HWA_MemInfo         hwaMemInfo;
    uint32_t ii;

    hwaHandle = HWA_open(0, NULL, &errorCode);
    if (hwaHandle == NULL)
    {
        test_print("HWA failed to open\n");
        DebugP_assert(0);
        return;
    }

    errorCode = HWA_getHWAMemInfo(hwaHandle, &hwaMemInfo);
    if (errorCode != 0)
    {
        test_print("HWA MEMINFOR get fails\n");
        DebugP_assert(0);
        return;
    }
    hwaMemBankSize = hwaMemInfo.bankSize;

    for (ii = 0; ii < hwaMemInfo.numBanks; ii++)
    {
        hwaMemBankAddr[ii] = hwaMemInfo.baseAddress + ii * hwaMemBankSize;
    }

    return;
}


/**
 *  @b Description
 *  @n
 *      Allocates Shawdow paramset
 */
static void allocateEDMAShadowChannel(uint32_t *param)
{
    int32_t testStatus = SystemP_SUCCESS;

    testStatus = EDMA_allocParam(gEdmaHandle[0], param);
    DebugP_assert(testStatus == SystemP_SUCCESS);

    return;
}

/**
*  @b Description
*  @n
*   Initialize the CFAR DPU configuration structure for the parameters NOT change for every test.
*
*  @retval None
*
*/
void cfarProc_test_dpuConfig()
{
    DPU_CFARProcHWA_HW_Resources *pHwRes;
    uint32_t dmaCh, tcc, param;

    /* static config*/
    cfarDpuCfg.staticCfg.rangeStep = 1.f;
    cfarDpuCfg.staticCfg.dopplerStep = 1.f;

    /* cfar range configuration */
    fovRangeCfg.min = 0.f;
    fovRangeCfg.max = 1025.f;
    fovDopplerCfg.min = -512.f;      //signed min, and max
    fovDopplerCfg.max = 512.f;

    cfarDpuCfg.dynCfg.cfarCfgDoppler = &cfarDopplerCfg;
    cfarDpuCfg.dynCfg.cfarCfgRange = &cfarRangeCfg;
    cfarDpuCfg.dynCfg.fovRange = &fovRangeCfg;
    cfarDpuCfg.dynCfg.fovDoppler = &fovDopplerCfg;

    /* hw resource*/
    pHwRes = &cfarDpuCfg.res;
    pHwRes->edmaHandle = gEdmaHandle[CONFIG_EDMA0]; //edmaHandle;

    dmaCh = DPU_CFAR_PROC_EDMAIN_CH;
    tcc   = DPU_CFAR_PROC_EDMAIN_CH;
    param = DPU_CFAR_PROC_EDMAIN_CH;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    pHwRes->edmaHwaIn.channel = dmaCh;
    pHwRes->edmaHwaIn.paramId = param;
    pHwRes->edmaHwaIn.tcc     = tcc;

    param = DPU_CFAR_PROC_EDMAIN_SHADOW;
    allocateEDMAShadowChannel(&param);
    pHwRes->edmaHwaIn.shadowPramId = param;
    pHwRes->edmaHwaIn.eventQueue = DPU_CFAR_PROC_EDMAIN_EVENT_QUE;

    dmaCh = DPU_CFAR_PROC_EDMAIN_SIG_CH;
    tcc   = DPU_CFAR_PROC_EDMAIN_SIG_CH;
    param = DPU_CFAR_PROC_EDMAIN_SIG_CH;
    DPEDMA_allocateEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &dmaCh, &tcc, &param);
    pHwRes->edmaHwaInSignature.channel = dmaCh;
    pHwRes->edmaHwaInSignature.paramId = param;
    pHwRes->edmaHwaInSignature.tcc     = tcc;

    param = DPU_CFAR_PROC_EDMAIN_SIG_SHADOW;
    allocateEDMAShadowChannel(&param);
    pHwRes->edmaHwaInSignature.shadowPramId = param;
    pHwRes->edmaHwaInSignature.eventQueue = DPU_CFAR_PROC_EDMAIN_SIG_EVENT_QUE;

    pHwRes->hwaCfg.numParamSet = 2U;
    pHwRes->hwaCfg.paramSetStartIdx = 0U;

    pHwRes->detMatrix.datafmt = DPIF_DETMATRIX_FORMAT_1;
    pHwRes->detMatrix.data = testDetMatrix;

    pHwRes->cfarRngDopSnrList = gCfarDetObjList;
    pHwRes->cfarRngDopSnrListSize = TEST_MAX_NUM_CFARDETOBJ_DPU_LIST;

    numBanksForInput = TEST_MAX_DETMATRIX_SIZE*sizeof(uint16_t)/hwaMemBankSize;
    pHwRes->hwaMemInp = (uint16_t *)hwaMemBankAddr[0];
    pHwRes->hwaMemInpSize = TEST_MAX_DETMATRIX_SIZE;//(hwaMemBankSize * numBanksForInput) / sizeof(uint16_t);
    pHwRes->hwaMemOutDoppler = (DPU_CFARProcHWA_CfarDetOutput *)hwaMemBankAddr[numBanksForInput];
    pHwRes->hwaMemOutDopplerSize = hwaMemBankSize / sizeof(DPU_CFARProcHWA_CfarDetOutput);
    pHwRes->hwaMemOutRange = (DPU_CFARProcHWA_CfarDetOutput *)hwaMemBankAddr[numBanksForInput+1];
    pHwRes->hwaMemOutRangeSize = hwaMemBankSize / sizeof(DPU_CFARProcHWA_CfarDetOutput);
    pHwRes->cfarDopplerDetOutBitMask = gCfarDopplerDetOutBitMask;

    CacheP_wbInv((void*)&cfarDpuCfg, sizeof(DPU_CFARProcHWA_Config), CacheP_TYPE_ALLD);
}


/**
*  @b Description
*  @n
*       Implements CFAR detection, based on hwa CFAR engine described in Radar hardware accelerator
*
*  @retval the number of detected objets
*
*/
uint16_t test_cfar_ref(cfarConfig_Test * cfarConfig, int16_t length, uint16_t * inputData, uint16_t * outList, uint32_t *peakNoise)
{
    uint16_t count;
    uint16_t ii;
    uint16_t leftSide, rightSide;
    uint32_t rightSum, leftSum;
    uint32_t maxSum, minSum;
    uint16_t minScale, maxScale;
    uint32_t noiseValue;
    uint16_t totalWin;
    uint16_t guardLen;
    uint8_t  numShift;

    numShift = mathUtils_floorLog2(cfarConfig->winLen);

    count = 0;
    if (cfarConfig->aveMode == HWA_NOISE_AVG_MODE_CFAR_CA)
    {
        minScale = 1;  //average both left and right
        maxScale = 1;
    }
    else if (cfarConfig->aveMode == HWA_NOISE_AVG_MODE_CFAR_CAGO)
    {
        minScale = 0;
        maxScale = 1;  //average over max
    }
    else //HWA_NOISE_AVE_MODE_CFAR_CASO
    {
        minScale = 1;  //average over min
        maxScale = 0;
    }

    guardLen = cfarConfig->guardLen;
    totalWin = cfarConfig->winLen + cfarConfig->guardLen;

    leftSide = cfarConfig->winLen + cfarConfig->guardLen;
    rightSide = length - leftSide;

    rightSum = 0;
    leftSum = 0;
    for (ii = guardLen + 1; ii < totalWin + 1; ii++)
    {
        rightSum += inputData[ii];
    }

    /* check point 0*/
    if (!cfarConfig->cyclicMode)
    {
        noiseValue = rightSum >> numShift;
        if (inputData[0] > (noiseValue + TEST_NOISE_THRESHOLD))  //threshold
        {
            outList[count] = 0; //detect peak
            peakNoise[count] = noiseValue;
            count++;
        }

    }
    else
    {
        for (ii = length - totalWin; ii < length - guardLen; ii++)
        {
            leftSum += inputData[ii];
        }

        if (rightSum > leftSum)
        {
            maxSum = rightSum;
            minSum = leftSum;
        }
        else
        {
            maxSum = leftSum;
            minSum = rightSum;
        }

        noiseValue = (maxSum * maxScale + minSum * minScale) / (minScale + maxScale) >> numShift;
        if (inputData[0] >(noiseValue + TEST_NOISE_THRESHOLD))  //threshold
        {
            outList[count] = 0; //detect peak
            peakNoise[count] = noiseValue;
            count++;
        }

    }

    /* other points */
    if (!cfarConfig->cyclicMode)
    {
        for (ii = 1; ii < leftSide; ii++)
        {
            rightSum -= inputData[ii + guardLen];
            rightSum += inputData[ii + totalWin];

            noiseValue = rightSum >> numShift;
            if (inputData[ii] > (noiseValue + TEST_NOISE_THRESHOLD))  //threshold
            {
                outList[count] = ii; //detect peak
                peakNoise[count] = noiseValue;
                count++;
            }
        }

        /* prepare the left sum for point leftSide */
        for (ii = 0; ii < cfarConfig->winLen; ii++)
        {
             leftSum += inputData[ii];
        }

        if (leftSide < rightSide)
        {
             rightSum -= inputData[leftSide + guardLen];
             rightSum += inputData[leftSide + totalWin];
             if (rightSum > leftSum)
             {
                 maxSum = rightSum;
                 minSum = leftSum;
             }
             else
             {
                 maxSum = leftSum;
                 minSum = rightSum;
             }

             noiseValue = (maxSum * maxScale + minSum * minScale) / (minScale + maxScale) >> numShift;
             if (inputData[leftSide] > (noiseValue + TEST_NOISE_THRESHOLD))  //threshold
             {
                 outList[count] = leftSide; //detect peak
                 peakNoise[count] = noiseValue;
                 count++;
             }
        }

    }
    else // cyclic mode
    {
        for (ii = 1; ii < guardLen + 1; ii++)
        {
            rightSum -= inputData[ii + guardLen];
            rightSum += inputData[ii + totalWin];
            leftSum -= inputData[length - totalWin + ii - 1];
            leftSum += inputData[length - guardLen + ii - 1];

            if(rightSum > leftSum)
            {
                maxSum = rightSum;
                minSum = leftSum;
            }
            else
            {
                maxSum = leftSum;
                minSum = rightSum;
            }

            noiseValue = (maxSum * maxScale + minSum * minScale) / (minScale + maxScale) >> numShift;
            if (inputData[ii] >(noiseValue + TEST_NOISE_THRESHOLD))  //threshold
            {
                outList[count] = ii; //detect peak
                peakNoise[count] = noiseValue;
                count++;
            }

        }

        for (ii = guardLen + 1; ii < leftSide + 1; ii++)
        {
            rightSum -= inputData[ii + guardLen];
            rightSum += inputData[ii + totalWin];
            leftSum -= inputData[length - totalWin + ii - 1];
            leftSum += inputData[ii - guardLen - 1];

            if (rightSum > leftSum)
            {
                maxSum = rightSum;
                minSum = leftSum;
            }
            else
            {
                maxSum = leftSum;
                minSum = rightSum;
            }

            noiseValue = (maxSum * maxScale + minSum * minScale) / (minScale + maxScale) >> numShift;
            if (inputData[ii] >(noiseValue + TEST_NOISE_THRESHOLD))  //threshold
            {
                outList[count] = ii; //detect peak
                peakNoise[count] = noiseValue;
                count++;
            }

        }
    }

    /* middle part*/
    for (ii = leftSide + 1; ii < rightSide ; ii++)
    {
        rightSum -= inputData[ii + guardLen];
        rightSum += inputData[ii + totalWin];
        leftSum -= inputData[ii - totalWin - 1];
        leftSum += inputData[ii - guardLen - 1];

        if (rightSum > leftSum)
        {
            maxSum = rightSum;
            minSum = leftSum;
        }
        else
        {
            maxSum = leftSum;
            minSum = rightSum;
        }

        noiseValue = (maxSum * maxScale + minSum * minScale) / (minScale + maxScale) >> numShift;
        if (inputData[ii] >(noiseValue + TEST_NOISE_THRESHOLD))  //threshold
        {
            outList[count] = ii; //detect peak
            peakNoise[count] = noiseValue;
            count++;
            if (count>=TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
                return count;
        }

    }

    /* right edge */
    if (!cfarConfig->cyclicMode)
    {

        if (leftSide < rightSide)
        {
            for (ii = rightSide; ii < length; ii++)
            {
                leftSum -= inputData[ii - totalWin - 1];
                leftSum += inputData[ii - guardLen - 1];

                noiseValue = leftSum >> numShift;
                if (inputData[ii] > (noiseValue + TEST_NOISE_THRESHOLD))  //threshold
                {
                    outList[count] = ii; //detect peak
                    peakNoise[count] = noiseValue;
                    count++;
                    if (count>=TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
                        return count;
                }
             }
        }
        else if (leftSide == rightSide)
        {

            noiseValue = leftSum >> numShift;
            if (inputData[leftSide] > (noiseValue + TEST_NOISE_THRESHOLD))  //threshold
            {
                outList[count] = leftSide; //detect peak
                peakNoise[count] = noiseValue;
                count++;
                if (count>=TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
                    return count;
            }

            for (ii = rightSide + 1; ii < length; ii++)
            {
                leftSum -= inputData[ii - totalWin - 1];
                leftSum += inputData[ii - guardLen - 1];

                noiseValue = leftSum >> numShift;
                if (inputData[ii] >(noiseValue + TEST_NOISE_THRESHOLD))  //threshold
                {
                    outList[count] = ii; //detect peak
                    peakNoise[count] = noiseValue;
                    count++;
                    if (count>=TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
                        return count;
                }

            }
        }
    }
    else
    {
        for (ii = rightSide; ii < length - guardLen; ii++)
        {
            leftSum -= inputData[ii - totalWin - 1];
            leftSum += inputData[ii - guardLen - 1];
            rightSum -= inputData[ii + guardLen];
            rightSum += inputData[ii + totalWin - length];

            if (rightSum > leftSum)
            {
                maxSum = rightSum;
                minSum = leftSum;
            }
            else
            {
                maxSum = leftSum;
                minSum = rightSum;
            }

            noiseValue = (maxSum * maxScale + minSum * minScale) / (minScale + maxScale) >> numShift;
            if (inputData[ii] >(noiseValue + TEST_NOISE_THRESHOLD))  //threshold
            {
                outList[count] = ii; //detect peak
                peakNoise[count] = noiseValue;
                count++;
                if (count>=TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
                    return count;
            }

        }

        for (ii = length - guardLen; ii < length ; ii++)
        {
            leftSum -= inputData[ii - totalWin - 1];
            leftSum += inputData[ii - guardLen - 1];
            rightSum -= inputData[ii + guardLen - length];
            rightSum += inputData[ii + totalWin - length];

            if (rightSum > leftSum)
            {
                maxSum = rightSum;
                minSum = leftSum;
            }
            else
            {
                maxSum = leftSum;
                minSum = rightSum;
            }

            noiseValue = (maxSum * maxScale + minSum * minScale) / (minScale + maxScale) >> numShift;
            if (inputData[ii] >(noiseValue + TEST_NOISE_THRESHOLD))  //threshold
            {
                outList[count] = ii; //detect peak
                peakNoise[count] = noiseValue;
                count++;
                if (count>=TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
                    return count;
            }
        }
    }
    return count;

}

/**
*  @b Description
*  @n
*       Implements peak grouping, based on hwa CFAR engine described in Radar hardware accelerator,
*       A peak is declared only if the cell under test is greater than or equal to its most immediate
*       neighboring cells to its left and right
*
*  @retval none
*
*/
void test_hwaPeakgroup_ref(uint16_t *inputData, uint16_t length, uint16_t numDetPeaks, uint16_t * peakIdx, uint8_t wrapFlag, uint8_t *peakAddFlag)
{
    uint16_t ii;
    uint16_t index;
    for (ii = 0; ii < numDetPeaks; ii++)
    {
        peakAddFlag[ii] = 0;
        index = peakIdx[ii];
        //no wrap around
        if (index == 0)
        {
            if (wrapFlag)
            {
                if ((inputData[index] >= inputData[index + 1]) & (inputData[index] >= inputData[length - 1]))
                {
                    peakAddFlag[ii] = 1;
                }
            }
            else  //no wrap around
            {
                if (inputData[index] >= inputData[index + 1]) //to match with hwa, only if > neighbour
                {
                    peakAddFlag[ii] = 1;
                }
            }

        }
        else if (index == length - 1)
        {
            if (wrapFlag)
            {
                if ((inputData[index] >= inputData[index - 1]) & (inputData[index] >= inputData[0]))
                {
                    peakAddFlag[ii] = 1;
                }
            }
            else
            {
                if (inputData[index] >= inputData[index - 1])
                {
                    peakAddFlag[ii] = 1;
                }
            }
        }
        else
        {
            if ((inputData[index] >= inputData[index + 1]) & (inputData[index] >= inputData[index - 1]))
            {
                peakAddFlag[ii] = 1;
            }

        }
    }
}

uint16_t ref_cfaros(uint16_t input[restrict],
                uint32_t inplen,
                uint32_t winlen,
                uint16_t k,
                uint32_t thresh,
                uint8_t  cyclic,
                uint16_t detlist[restrict],
                uint32_t koslist[restrict]);

/**
*  @b Description
*  @n
*       Implement the range bin cfar and both range bin + doppler bin cfar for generating test references.
*
*  @retval the number of detected objets
*
*/
void Test_gen_detobj_ref(cfarProcDpuTestConfig_t * testConfig)
{
    uint32_t ii, jj;
    uint16_t numDetPeaks;
    uint16_t peakIdx[TEST_MAX_NUM_CFARDETOBJ_DPU_LIST];
    uint32_t peakNoise[TEST_MAX_NUM_CFARDETOBJ_DPU_LIST];
    uint8_t peakAddFlag[TEST_MAX_NUM_CFARDETOBJ_DPU_LIST];
    uint16_t * dataInPtr;

    numDetPeaksRef = 0;

    if (!testConfig->dopplerCfarEnabled)  //doppler cfar is disabled
    {
        for (ii = 0; ii < testConfig->numDopplerBins; ii++)
        {
            for (jj = 0; jj < testConfig->numRangeBins; jj++)
            {
                gCfarInputDataTemp[jj] = testDetMatrix[jj * testConfig->numDopplerBins + ii];
             }
            /* call cfar reference */
            if (testConfig->cfarRangeCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_OS)
            {
                numDetPeaks = ref_cfaros(gCfarInputDataTemp,
                                         testConfig->numRangeBins,
                                         testConfig->cfarRangeCfg.winLen*2,
                                         testConfig->cfarRangeCfg.osKvalue,
                                         TEST_NOISE_THRESHOLD,
                                         testConfig->cfarRangeCfg.cyclicMode,
                                         peakIdx,
                                         peakNoise);
            }
            else
            {
                numDetPeaks = test_cfar_ref(&testConfig->cfarRangeCfg, testConfig->numRangeBins, gCfarInputDataTemp, peakIdx, peakNoise);
            }

            if ((testConfig->cfarRangePeakPruning)  && (numDetPeaks))
            {
                /* call peak pruning ref */
                test_hwaPeakgroup_ref(gCfarInputDataTemp, testConfig->numRangeBins, numDetPeaks, peakIdx, testConfig->cfarRangeCfg.cyclicMode, peakAddFlag);
            }

            if (numDetPeaksRef < TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
            {
                for (jj = 0; jj < numDetPeaks; jj++)
                {
                    if ((!testConfig->cfarRangePeakPruning) || ((testConfig->cfarRangePeakPruning) && peakAddFlag[jj]))
                    {
                        gCfarDetObjRef[numDetPeaksRef].dopplerIdx = ii;
                        gCfarDetObjRef[numDetPeaksRef].rangeIdx = peakIdx[jj];
                        gCfarDetObjRef[numDetPeaksRef].noise = peakNoise[jj];
                        gCfarDetObjRef[numDetPeaksRef].peakAbs = gCfarInputDataTemp[peakIdx[jj]];
                        gCfarDetObjCheck[numDetPeaksRef] = 1;
                        numDetPeaksRef++;
                        if (numDetPeaksRef >= TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
                        {
                            break;
                        }
                    }
                }
            }
        }
    }
    else  //doppler cfar is enabled
    {
        memset(gCfarDopplerDetOutRef, 0, testConfig->numDopplerBins * testConfig->numRangeBins * sizeof(uint8_t));
        for (ii = 0; ii < testConfig->numRangeBins; ii++)
        {
            dataInPtr = &testDetMatrix[ii * testConfig->numDopplerBins];
            /* call the cfar ref */
            if (testConfig->cfarDopplerCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_OS)
            {
                numDetPeaks = ref_cfaros(dataInPtr,
                                         testConfig->numDopplerBins,
                                         testConfig->cfarDopplerCfg.winLen*2,
                                         testConfig->cfarDopplerCfg.osKvalue,
                                         TEST_NOISE_THRESHOLD,
                                         testConfig->cfarDopplerCfg.cyclicMode,
                                         peakIdx,
                                         peakNoise);
            }
            else
            {
                numDetPeaks = test_cfar_ref(&testConfig->cfarDopplerCfg, testConfig->numDopplerBins, dataInPtr, peakIdx, peakNoise);

            }
            if ((testConfig->cfarDopplerPeakPruning) && (numDetPeaks))
            {
                /* call peak pruning ref */
                test_hwaPeakgroup_ref(dataInPtr, testConfig->numDopplerBins, numDetPeaks, peakIdx, testConfig->cfarDopplerCfg.cyclicMode, peakAddFlag);
            }
            for (jj = 0; jj < numDetPeaks; jj++)
            {
                if ((!testConfig->cfarDopplerPeakPruning) || ((testConfig->cfarDopplerPeakPruning) && peakAddFlag[jj]))
                {
                    gCfarDopplerDetOutRef[ii * testConfig->numDopplerBins + peakIdx[jj]] = 1;
                }
            }

        }

        /* do the range cfar */
        for (ii = 0; ii < testConfig->numDopplerBins; ii++)
        {
            for (jj = 0; jj < testConfig->numRangeBins; jj++)
            {
                gCfarInputDataTemp[jj] = testDetMatrix[jj * testConfig->numDopplerBins + ii];
            }
            /* call cfar reference */
            if (testConfig->cfarRangeCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_OS)
            {
                numDetPeaks = ref_cfaros(gCfarInputDataTemp,
                                         testConfig->numRangeBins,
                                         testConfig->cfarRangeCfg.winLen*2,
                                         testConfig->cfarRangeCfg.osKvalue,
                                         TEST_NOISE_THRESHOLD,
                                         testConfig->cfarRangeCfg.cyclicMode,
                                         peakIdx,
                                         peakNoise);
            }
            else
            {
                numDetPeaks = test_cfar_ref(&testConfig->cfarRangeCfg, testConfig->numRangeBins, gCfarInputDataTemp, peakIdx, peakNoise);
            }

            if ((testConfig->cfarRangePeakPruning) && (numDetPeaks))
            {
                /* call peak pruning ref */
                test_hwaPeakgroup_ref(gCfarInputDataTemp, testConfig->numRangeBins, numDetPeaks, peakIdx, testConfig->cfarRangeCfg.cyclicMode, peakAddFlag);
            }

            if (numDetPeaksRef < TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
            {
                for (jj = 0; jj < numDetPeaks; jj++)
                {
                    if ((!testConfig->cfarRangePeakPruning) || ((testConfig->cfarRangePeakPruning) && peakAddFlag[jj]))
                    {
                        if (gCfarDopplerDetOutRef[peakIdx[jj] * testConfig->numDopplerBins + ii])
                        {
                            gCfarDetObjRef[numDetPeaksRef].dopplerIdx = ii;
                            gCfarDetObjRef[numDetPeaksRef].rangeIdx = peakIdx[jj];
                            gCfarDetObjRef[numDetPeaksRef].noise = peakNoise[jj];
                            gCfarDetObjRef[numDetPeaksRef].peakAbs = gCfarInputDataTemp[peakIdx[jj]];
                            gCfarDetObjCheck[numDetPeaksRef] = 1;
                            numDetPeaksRef++;
                            if (numDetPeaksRef > TEST_MAX_NUM_CFARDETOBJ_DPU_LIST)
                            {
                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}

/**
*  @b Description
*  @n
*  set up the CFAR DPU configuration structure based on each test configurations.
*
*  @retval None
*
*/
void Test_setProfile(cfarProcDpuTestConfig_t * testConfig)
{
    /* cfar range configuration */
    fovRangeCfg.min = 0.f;
    fovRangeCfg.max = 1025.f;
    fovDopplerCfg.min = -512.f;      //signed min, and max
    fovDopplerCfg.max = 512.f;

    cfarDpuCfg.staticCfg.numDopplerBins = testConfig->numDopplerBins;
    cfarDpuCfg.staticCfg.numRangeBins = testConfig->numRangeBins;
    cfarDpuCfg.staticCfg.log2NumDopplerBins = mathUtils_floorLog2(testConfig->numDopplerBins);

    /* cfar range config*/
    cfarRangeCfg.thresholdScale = TEST_NOISE_THRESHOLD; // noiseVar + thresholdScale
    cfarRangeCfg.averageMode = testConfig->cfarRangeCfg.aveMode;
    cfarRangeCfg.winLen = testConfig->cfarRangeCfg.winLen;
    cfarRangeCfg.guardLen = testConfig->cfarRangeCfg.guardLen;
#if DBG_CEIL_LOG
    if (cfarRangeCfg.averageMode == HWA_NOISE_AVG_MODE_CFAR_CA)
    {
        cfarRangeCfg.noiseDivShift = mathUtils_ceilLog2(testConfig->cfarRangeCfg.winLen * 2);  //the sum of noise divided by 2^noiseDivShift
    }
    else      //HWA_NOISE_AVE_MODE_CFAR_CAGO, HWA_NOISE_AVE_MODE_CFAR_CASO. Note this is ignored in case of CFAR_OS.
    {
        cfarRangeCfg.noiseDivShift = mathUtils_ceilLog2(testConfig->cfarRangeCfg.winLen);  //the sum of noise divided by 2^noiseDivShift
    }
#else
    if (cfarRangeCfg.averageMode == HWA_NOISE_AVG_MODE_CFAR_CA)
    {
        cfarRangeCfg.noiseDivShift = mathUtils_floorLog2(testConfig->cfarRangeCfg.winLen * 2);  //the sum of noise divided by 2^noiseDivShift
    }
    else      //HWA_NOISE_AVE_MODE_CFAR_CAGO, HWA_NOISE_AVE_MODE_CFAR_CASO. Note this is ignored in case of CFAR_OS.
    {
        cfarRangeCfg.noiseDivShift = mathUtils_floorLog2(testConfig->cfarRangeCfg.winLen);  //the sum of noise divided by 2^noiseDivShift
    }
#endif
    if (cfarRangeCfg.averageMode == HWA_NOISE_AVG_MODE_CFAR_OS)
    {
        cfarRangeCfg.osKvalue = testConfig->cfarRangeCfg.osKvalue;
        cfarRangeCfg.osEdgeKscaleEn = testConfig->cfarRangeCfg.osEdgeKscaleEnable;
    }
    cfarRangeCfg.cyclicMode = testConfig->cfarRangeCfg.cyclicMode;
    if (testConfig->cfarRangePeakPruning) //peak group is enabled
    {
        cfarRangeCfg.peakGroupingEn = 1;
        cfarRangeCfg.peakGroupingScheme = 1;
    }
    else
    {
        cfarRangeCfg.peakGroupingEn = 0;
        cfarRangeCfg.peakGroupingScheme = 0;
    }

    /* cfar doppler config */
    if (testConfig->dopplerCfarEnabled) //enabled the doppler cfar
        cfarDopplerCfg.thresholdScale = TEST_NOISE_THRESHOLD;
    else
        cfarDopplerCfg.thresholdScale = 0; //disable the doppler cfar

    cfarDopplerCfg.averageMode = testConfig->cfarDopplerCfg.aveMode;
    cfarDopplerCfg.winLen = testConfig->cfarDopplerCfg.winLen;
    cfarDopplerCfg.guardLen = testConfig->cfarDopplerCfg.guardLen;

#if DBG_CEIL_LOG
    if (cfarDopplerCfg.averageMode == HWA_NOISE_AVG_MODE_CFAR_CA)
    {
        cfarDopplerCfg.noiseDivShift = mathUtils_ceilLog2(testConfig->cfarDopplerCfg.winLen * 2);  //the sum of noise divided by 2^noiseDivShift
    }
    else
    {
        cfarDopplerCfg.noiseDivShift = mathUtils_ceilLog2(testConfig->cfarDopplerCfg.winLen);  //the sum of noise divided by 2^noiseDivShift
    }
#else
    if (cfarDopplerCfg.averageMode == HWA_NOISE_AVG_MODE_CFAR_CA)
    {
        cfarDopplerCfg.noiseDivShift = mathUtils_floorLog2(testConfig->cfarDopplerCfg.winLen * 2);  //the sum of noise divided by 2^noiseDivShift
    }
    else
    {
        cfarDopplerCfg.noiseDivShift = mathUtils_floorLog2(testConfig->cfarDopplerCfg.winLen);  //the sum of noise divided by 2^noiseDivShift
    }
#endif

    if (cfarDopplerCfg.averageMode == HWA_NOISE_AVG_MODE_CFAR_OS)
    {
        cfarDopplerCfg.osKvalue = testConfig->cfarDopplerCfg.osKvalue;
        cfarDopplerCfg.osEdgeKscaleEn = testConfig->cfarDopplerCfg.osEdgeKscaleEnable;
    }

    cfarDopplerCfg.cyclicMode = testConfig->cfarDopplerCfg.cyclicMode;
    if (testConfig->cfarDopplerPeakPruning) //peak group is enabled
    {
        cfarDopplerCfg.peakGroupingEn = 1;
        cfarDopplerCfg.peakGroupingScheme = 1;
    }
    else
    {
        cfarDopplerCfg.peakGroupingEn = 0;
        cfarDopplerCfg.peakGroupingScheme = 0;
    }

    cfarDpuCfg.res.detMatrix.dataSize = testConfig->numDopplerBins * testConfig->numRangeBins * sizeof(uint16_t);
    cfarDpuCfg.res.cfarDopplerDetOutBitMaskSize = (testConfig->numRangeBins * testConfig->numDopplerBins) / 32;
}

/**
*  @b Description
*  @n
*  calculate modula value
*
*  @retval the modula value
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
*   randomly generate a table index, and returns the table elemenment based on the index
*
*  @retval one of table elemenet, with randonly generated index
*
*/
uint16_t test_setParam(uint16_t modValue, uint8_t * pTbl)
{
    uint16_t tempValue, index;
    uint16_t value;

    tempValue = RandSimple();
    index = test_modCalc(tempValue, modValue);
    if (pTbl == NULL)
    {
        return index;
    }
    else
    {
        value = pTbl[index];
        return value;
    }
}

/**
*  @b Description
*  @n
*   check the CFAR DPU results with the reference.
*
*  @retval None
*
*/
void Test_printResults()
{
    uint8_t testPass = 1;
    uint32_t ii;
    float noiseInDbRef;
    float snrInDbRef;
    int16_t temp;
    uint32_t peakIdx;

    if (numCheckPeaksRef != cfarDpuOut.numCfarDetectedPoints)
    {
        test_print("\nCFAR output number is : cfarOut=%d, ref=%d\n", cfarDpuOut.numCfarDetectedPoints, numDetPeaksRef);
        finalResults = 0;
        testPass = 0;
    }
    //when match, also print numbers here:
    else
    {
        test_print("\nNum detected: cfarOut=%d, ref=%d\n", cfarDpuOut.numCfarDetectedPoints, numCheckPeaksRef);
        test_print("detObj 0 ref rangeIdx=%d, dopplerIdx=%d\n", gCfarDetObjRef[0].rangeIdx, gCfarDetObjRef[0].dopplerIdx);
    }

    peakIdx = 0;
    /* Check each object's details when there are detected objects  */
    if (cfarDpuOut.numCfarDetectedPoints > 0)
    for (ii = 0; ii < cfarDpuOut.numCfarDetectedPoints; ii++)
    {
        if (!gCfarDetObjCheck[ii])
            continue;
        /* check range idx and doppler idx*/
        if ((gCfarDetObjRef[ii].rangeIdx != gCfarDetObjList[peakIdx].rangeIdx) ||
            (gCfarDetObjRef[ii].dopplerIdx != gCfarDetObjList[peakIdx].dopplerIdx))
        {
            test_print("\ndetObj %d index mismatch: cfarOut doppler=%d range=%d;  ref doppler=%d, range=%d\n", ii,
                          gCfarDetObjList[peakIdx].dopplerIdx, gCfarDetObjList[peakIdx].rangeIdx,
                          gCfarDetObjRef[ii].dopplerIdx, gCfarDetObjRef[ii].rangeIdx);
            finalResults = 0;
            testPass = 0;
        }

        /* check the noise */
        noiseInDbRef = (float)gCfarDetObjRef[ii].noise / 256.f * 6.f; //20/log10(2)=6
        temp = (int16_t)(noiseInDbRef * 10.f);

        if (temp != gCfarDetObjList[peakIdx].noise)
        {
            test_print("\ndetObj %d noiseVar mismatch : cfarOut=%d, ref=%d\n", ii, gCfarDetObjList[peakIdx].noise, temp);
            finalResults = 0;
            testPass = 0;
            //dbg print more:
            test_print("detObj %d noise value: cfarOut=%d, ref=%d\n", ii, (int16_t)((float)gCfarDetObjList[peakIdx].noise*256.f/60.f),gCfarDetObjRef[ii].noise);
        }

        /* check the snr */
        snrInDbRef = ((float)gCfarDetObjRef[ii].peakAbs - (float)gCfarDetObjRef[ii].noise) / 256.f * 6.f;
        temp = (int16_t)(snrInDbRef * 10.f);

        if (temp != gCfarDetObjList[peakIdx].snr)
        {
            test_print("\ndetObj %d snr is : cfarOut=%d, ref=%d\n", ii, gCfarDetObjList[peakIdx].snr, temp);
            finalResults = 0;
            testPass = 0;
        }

        peakIdx++;
    }/* end of det peak*/

    if (testPass)
    {
        test_print(" PASS ");
    }
    else
    {
         test_print(" FAIL ");
    }
}


/**
*  @b Description
*  @n
*   if either Cmd_FovRangeCfg, or Cmd_FovDopplerCfg command is tested, update the cfar detected objects by removing
*   the objectes outside of the range or dopple range.
*
*  @retval None
*
*/
void Test_update_detobj_ref(cfarProcDpuTestConfig_t * testConfig)
{
    uint32_t ii;
    int16_t signedDopplerIdx;

    if (testConfig->dpuControlFlag == 0) //range fov
    {
        for (ii = 0; ii < numDetPeaksRef; ii++)
        {
            if ((gCfarDetObjRef[ii].rangeIdx < fovRangeCfg.min) || (gCfarDetObjRef[ii].rangeIdx > fovRangeCfg.max))
            {
                gCfarDetObjCheck[ii] = 0;
                numCheckPeaksRef --;
            }
        }

    }
    else if (testConfig->dpuControlFlag == 1)
    {
        for (ii = 0; ii < numDetPeaksRef; ii++)
        {
            signedDopplerIdx = gCfarDetObjRef[ii].dopplerIdx;
            if (signedDopplerIdx >= (testConfig->numDopplerBins / 2))
            {
                signedDopplerIdx = signedDopplerIdx - testConfig->numDopplerBins;
            }
            if ((signedDopplerIdx < fovDopplerCfg.min) || (signedDopplerIdx > fovDopplerCfg.max))
            {
                gCfarDetObjCheck[ii] = 0;
                numCheckPeaksRef--;
            }
        }
    }
}


/**
*  @b Description
*  @n
*   based on the dpu commands, test the DPU_CFARProcHWA_control function.
*
*  @retval None
*
*/
void test_dpu_control_cmd(cfarProcDpuTestConfig_t * testConfig)
{
    int32_t retVal;
    if (testConfig->dpuControlFlag == 0)
    {
        fovRangeCfg.min = testConfig->numRangeBins / 8.f;
        fovRangeCfg.max = testConfig->numRangeBins / 2.f;
        retVal = DPU_CFARProcHWA_control(cfarDpuHandle, DPU_CFARProcHWA_Cmd_FovRangeCfg, &fovRangeCfg, sizeof(DPU_CFARProc_FovCfg));
        if (retVal != 0)
        {
            test_print("CFAR DPU FovRangeCfg control error \n");
            DebugP_assert(0);
            return;
        }
    }
    else if (testConfig->dpuControlFlag == 1)
    {
        fovDopplerCfg.min = -testConfig->numDopplerBins / 8.f;      //signed min, and max
        fovDopplerCfg.max = testConfig->numDopplerBins / 2.f;
        retVal = DPU_CFARProcHWA_control(cfarDpuHandle, DPU_CFARProcHWA_Cmd_FovDopplerCfg, &fovDopplerCfg, sizeof(DPU_CFARProc_FovCfg));
        if (retVal != 0)
        {
            test_print("CFAR DPU FovDopplerCfg control error \n");
            DebugP_assert(0);
            return;
        }
    }
    else if (testConfig->dpuControlFlag == 2)
    {
        testConfig->cfarDopplerCfg.aveMode = test_modCalc(testConfig->cfarDopplerCfg.aveMode + 1, 3);
        testConfig->cfarDopplerCfg.cyclicMode = test_modCalc(testConfig->cfarDopplerCfg.cyclicMode + 1, 2);
        if (testConfig->cfarDopplerCfg.guardLen != 1)
            testConfig->cfarDopplerCfg.guardLen = testConfig->cfarDopplerCfg.guardLen - 1;
        if (testConfig->cfarDopplerCfg.winLen > 4) //hwa2.0 winLen 2 not valid
            testConfig->cfarDopplerCfg.winLen = testConfig->cfarDopplerCfg.winLen / 2;
        printf("New mode=%d, guard=%d, W=%d", testConfig->cfarDopplerCfg.aveMode, testConfig->cfarDopplerCfg.guardLen, testConfig->cfarDopplerCfg.winLen);
        Test_setProfile(testConfig);
        retVal = DPU_CFARProcHWA_control(cfarDpuHandle, DPU_CFARProcHWA_Cmd_CfarDopplerCfg, &cfarDopplerCfg, sizeof(DPU_CFARProc_CfarCfg));

        if (retVal != 0)
        {
            test_print("CFAR DPU CfarDopplerCfg control error \n");
            DebugP_assert(0);
            return;
        }
    }
    else if (testConfig->dpuControlFlag == 3)
    {
        testConfig->cfarRangeCfg.aveMode = test_modCalc(testConfig->cfarRangeCfg.aveMode + 1, 3);
        if (testConfig->cfarRangeCfg.guardLen > 1)
            testConfig->cfarRangeCfg.guardLen = testConfig->cfarRangeCfg.guardLen - 1;
        testConfig->cfarRangeCfg.winLen = testConfig->cfarRangeCfg.winLen + 2; //test values not power of 2
        if (testConfig->numRangeBins ==16)
            testConfig->cfarRangeCfg.winLen = 4; //HWA requires winLen < #Bins/2
        printf("New mode=%d, guard=%d, W=%d", testConfig->cfarRangeCfg.aveMode, testConfig->cfarRangeCfg.guardLen, testConfig->cfarRangeCfg.winLen);
        Test_setProfile(testConfig);
        retVal = DPU_CFARProcHWA_control(cfarDpuHandle, DPU_CFARProcHWA_Cmd_CfarRangeCfg, &cfarRangeCfg, sizeof(DPU_CFARProc_CfarCfg));

        if (retVal != 0)
        {
            test_print("CFAR DPU CfarRangeCfg control error %d\n", retVal);
            DebugP_assert(0);
            return;
        }
    }
 }


/**
*  @b Description
*  @n
*     test main task  : readin the det matrix from the binary test file, which is generated using matlab.
*                      a cfar reference generation function is developed to generate cfar detected objects,
*                      The reference detected objects are compared with the CFAR DPU outputs, if they are same,
*                      the test is declared PASS, otherwise, it is declared FAIL.
*
*  @retval None
*
*/
void Test_task(void* args)
{
    uint32_t ii, jj, kk;
    char featureName[200];
    uint32_t numDopplerBin[TEST_DOPPLER_BIN_SIZE] = { 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };
    uint32_t numRangeBin[TEST_RANGE_BIN_SIZE] = { 16, 32, 64, 128, 256, 512, 1024 };
    uint8_t cfarWinLen[TEST_CFAR_WIN_LEN] = { 2, 4, 8, 16, 32, 64 };
    uint8_t cfarGuardLen[TEST_CFAR_GUARD_LEN] = {1, 2, 3, 4, 5, 6, 7 };
    uint8_t cfarosWinLen[TEST_RANGE_BIN_SIZE] = {8, 12, 16, 8, 8, 8, 8}; /*number of samples on one side*/
    uint32_t numTests;
    uint32_t numTestsRange;
    DPU_CFARProcHWA_InitParams cfarDpuInitParams;
    int32_t errorCode;
    FILE * fileId;
    uint64_t taskStartTime, taskEndTime;

    finalResults = 1;

    Drivers_open();
    Board_driversOpen();

    /* Initialize test logger */
    MCPI_Initialize();

    taskStartTime=ClockP_getTimeUsec();
    test_print("...... Initialization ...... \n");

    /* hwa, inialization */
    cfarProc_test_hwaInit();

    /* configure cfar dpu */
    cfarProc_test_dpuConfig();

    test_print("CFAR Proc DPU Test start ...... \n");
    /* for doppler size = 2, 4, 8, 16 test range bin <=1024 */
    numTests = TEST_RANGE_BIN_SIZE * 4;
    /* for doppler size = 32, test range bin <= 512 */
    numTests += (TEST_RANGE_BIN_SIZE - 1);
    /* for doppler size = 64, test range bin <= 256 */
    numTests += (TEST_RANGE_BIN_SIZE - 2);
    /* for doppler size = 128, test range bin <= 128 */
    numTests += (TEST_RANGE_BIN_SIZE - 3);
    /* for doppler size = 256, test range bin <= 64 */
    numTests += (TEST_RANGE_BIN_SIZE - 4);
    /* for doppler size = 512, test range bin <= 32 */
    numTests += (TEST_RANGE_BIN_SIZE - 5);
    /* for doppler size = 1024, test range bin <= 16 */
    numTests += (TEST_RANGE_BIN_SIZE - 6);

    numTests ++; /* add tests for Doppler size 32 Range size 1024 */
    /* for every rangebinxdopplerbin combination, run 4 tests*/
    numTests = numTests * 4;

    printf("\n...... %d Tests to be run .......\n\n", numTests);

    fileId = fopen("cfarteststart.txt","w");
    if(fileId != NULL)
    {
        fclose(fileId);
        fileId = fopen("testdata\\cfardpu_test_data.bin", "rb");
        if (fileId == NULL)
        {
            test_print("Error:  Cannot open cfardpu_test_data.bin !\n");
            exit(0);
        }
    }

    testCount = 0;
    cfarDpuInitParams.hwaHandle = hwaHandle;
    cfarDpuHandle = DPU_CFARProcHWA_init(&cfarDpuInitParams, subFrameCount, &errorCode);
    if (errorCode != 0)
    {
        test_print("CFAR DPU INIT fail\n");
        DebugP_assert(0);
        goto exit;
    }

    for (ii = 0; ii < TEST_DOPPLER_BIN_SIZE; ii++)/* doppler bin loop*/
    {

        if (ii < 5) /* if D<=32, test up to R=1024  */
        {
            numTestsRange = TEST_RANGE_BIN_SIZE;
        }
        else
        {
            numTestsRange = TEST_RANGE_BIN_SIZE - (ii - 3);
        }

        for (jj = 0; jj < numTestsRange; jj++) /* range bin loop */
        {
            cfarTestCfg.numDopplerBins = numDopplerBin[ii];
            cfarTestCfg.numRangeBins = numRangeBin[jj];
            /* read in the det matrix data*/
            fread(testDetMatrix, sizeof(uint16_t), cfarTestCfg.numDopplerBins * cfarTestCfg.numRangeBins, fileId);

            CacheP_wbInv(testDetMatrix, cfarTestCfg.numDopplerBins * cfarTestCfg.numRangeBins * sizeof(uint16_t), CacheP_TYPE_ALLD);
            CacheP_wbInv(gCfarDetObjListBuf, (CSL_NEXT_MULTIPLE_OF(TEST_MAX_NUM_CFARDETOBJ_DPU_LIST * sizeof(DPIF_CFARDetList), \
                                                TEST_DPU_CFARPROCHWA_CFAR_DET_LIST_BYTE_ALIGNMENT))*sizeof(uint8_t), CacheP_TYPE_ALLD);

            for (kk = 0; kk < 4; kk++)
            {
                test_print("\nTest #%d : ", testCount);//("\nTest #%d  start\r\n", testCount);

                /* range config*/
                /* cfar mode*/
                cfarTestCfg.cfarRangeCfg.aveMode = (uint8_t )test_setParam(3, NULL);
                /*cyclic mode */
                cfarTestCfg.cfarRangeCfg.cyclicMode = 0; //for range cfar, NOT support cyclic mode = 1

                /* win length */
                if (cfarTestCfg.numRangeBins == 16)
                    cfarTestCfg.cfarRangeCfg.winLen = test_setParam(2, cfarWinLen);
                else if (cfarTestCfg.numRangeBins == 32)
                    cfarTestCfg.cfarRangeCfg.winLen = test_setParam(2, cfarWinLen);
                else if (cfarTestCfg.numRangeBins == 64)
                    cfarTestCfg.cfarRangeCfg.winLen = test_setParam(3, cfarWinLen);
                else if (cfarTestCfg.numRangeBins == 128)
                    cfarTestCfg.cfarRangeCfg.winLen = test_setParam(4, cfarWinLen);
                else if (cfarTestCfg.numRangeBins >= 256)
                    cfarTestCfg.cfarRangeCfg.winLen = test_setParam(TEST_CFAR_WIN_LEN, cfarWinLen);

                 /*  guard length*/
                if (cfarTestCfg.numRangeBins == 16)
                    cfarTestCfg.cfarRangeCfg.guardLen = test_setParam(3,cfarGuardLen);
                else if (cfarTestCfg.numRangeBins == 32)
                    cfarTestCfg.cfarRangeCfg.guardLen = test_setParam(4, cfarGuardLen);
                else
                    cfarTestCfg.cfarRangeCfg.guardLen = test_setParam(TEST_CFAR_GUARD_LEN, cfarGuardLen);

                /* peak group*/
                cfarTestCfg.cfarRangePeakPruning = test_setParam(2, NULL);


                /* doppler cfar enabled flag*/
                if (cfarTestCfg.numDopplerBins < 16)
                    cfarTestCfg.dopplerCfarEnabled = 0;
                else
                    cfarTestCfg.dopplerCfarEnabled = (uint8_t)test_setParam(2, NULL);

                //hwa 2.0 not allow W2, so replace some old W2 ca cases with new os cases
                if ((cfarTestCfg.cfarRangeCfg.winLen==2) && (cfarTestCfg.numDopplerBins < 32))
                {
                    cfarTestCfg.cfarRangeCfg.aveMode = HWA_NOISE_AVG_MODE_CFAR_OS;
                    cfarTestCfg.cfarRangeCfg.winLen = cfarosWinLen[jj];
                    cfarTestCfg.cfarRangeCfg.osKvalue = testCount&3; //k=0,1,2 or 3;
                    if (cfarTestCfg.cfarRangeCfg.winLen>=24)
                        cfarTestCfg.cfarRangeCfg.osKvalue = cfarTestCfg.cfarRangeCfg.winLen - (testCount&3); //test some larger k. if W=24, then k=24,23,22,21
                    cfarTestCfg.cfarRangeCfg.guardLen = 0;
                    cfarTestCfg.cfarRangeCfg.osEdgeKscaleEnable = 0;
                    cfarTestCfg.cfarRangePeakPruning = 0;
                }
                else if (cfarTestCfg.cfarRangeCfg.winLen==2)
                { cfarTestCfg.cfarRangeCfg.winLen=4;}


                /* doppler config*/
                if (!cfarTestCfg.dopplerCfarEnabled)   //disable the doppler cfar
                {
                    cfarTestCfg.cfarDopplerCfg.aveMode = 0;
                    cfarTestCfg.cfarDopplerCfg.cyclicMode = 0;
                    cfarTestCfg.cfarDopplerCfg.winLen = 0;
                    cfarTestCfg.cfarDopplerCfg.guardLen = 0;
                    cfarTestCfg.cfarDopplerPeakPruning = 0;

                }
                else
                {
                    /* cfar mode*/
                    cfarTestCfg.cfarDopplerCfg.aveMode = (uint8_t)test_setParam(3, NULL);
                    /*cyclic mode */
                    cfarTestCfg.cfarDopplerCfg.cyclicMode = (uint8_t)test_setParam(2, NULL);

                    /* win length */
                    if (cfarTestCfg.numDopplerBins == 16)
                        cfarTestCfg.cfarDopplerCfg.winLen = test_setParam(2, cfarWinLen);
                    else if (cfarTestCfg.numDopplerBins == 32)
                        cfarTestCfg.cfarDopplerCfg.winLen = test_setParam(3, cfarWinLen);
                    else if (cfarTestCfg.numDopplerBins == 64)
                        cfarTestCfg.cfarDopplerCfg.winLen = test_setParam(4, cfarWinLen);
                    else if (cfarTestCfg.numDopplerBins == 128)
                        cfarTestCfg.cfarDopplerCfg.winLen = test_setParam(5, cfarWinLen);
                    else //if (cfarTestCfg.numDopplerBins == 256)
                        cfarTestCfg.cfarDopplerCfg.winLen = test_setParam(TEST_CFAR_WIN_LEN, cfarWinLen);

                    /*  guard length*/
                    if (cfarTestCfg.numDopplerBins == 16)
                        cfarTestCfg.cfarDopplerCfg.guardLen = test_setParam(3, cfarGuardLen);
                    else if (cfarTestCfg.numDopplerBins == 32)
                        cfarTestCfg.cfarDopplerCfg.guardLen = test_setParam(4, cfarGuardLen);
                    else
                        cfarTestCfg.cfarDopplerCfg.guardLen = test_setParam(TEST_CFAR_GUARD_LEN, cfarGuardLen);
                    /* peak group*/
                    cfarTestCfg.cfarDopplerPeakPruning = test_setParam(2, NULL);

                    //hwa 2.0 not allow W2, so replace some old W2 ca cases with new os cases, smaller num bins for shorter time
                    if ((cfarTestCfg.cfarDopplerCfg.winLen==2) && (cfarTestCfg.numDopplerBins<64) && (cfarTestCfg.numRangeBins<64))
                    {
                    cfarTestCfg.cfarDopplerCfg.aveMode = HWA_NOISE_AVG_MODE_CFAR_OS;
                    cfarTestCfg.cfarDopplerCfg.winLen = cfarosWinLen[ii-3];
                    cfarTestCfg.cfarDopplerCfg.guardLen = 0;
                    cfarTestCfg.cfarDopplerCfg.osKvalue = testCount&3; //k=0,1,2 or 3;
                    if (cfarTestCfg.cfarDopplerCfg.winLen>=32)
                        cfarTestCfg.cfarDopplerCfg.osKvalue = cfarTestCfg.cfarDopplerCfg.winLen*2-1 - (testCount&3);
                    cfarTestCfg.cfarDopplerCfg.osEdgeKscaleEnable = 0;
                    }

                    if(cfarTestCfg.cfarDopplerCfg.winLen==2) ///hwa 2.0 not allow W2
                    {cfarTestCfg.cfarDopplerCfg.winLen=4;}

                }

                sprintf(featureName, "%s%d%s%d",
                    ":D ", cfarTestCfg.numDopplerBins,
                    ":R ", cfarTestCfg.numRangeBins);

                test_print("Test feature : %s", featureName);

                if (cfarTestCfg.cfarRangeCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_CA)
                {
                    test_print("  RangeCfar:: CA");
                }
                else if (cfarTestCfg.cfarRangeCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_CAGO)
                {
                    test_print("  RangeCfar:: CAGO");
                }
                else if (cfarTestCfg.cfarRangeCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_CASO)
                {
                    test_print("  RangeCfar:: CASO");
                }

                if (cfarTestCfg.cfarRangeCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_OS)
                {
                    test_print("  RangeCfar:: CFAROS");

                    sprintf(featureName, "%s%d%s%d%s%d%s%d%s%d",
                    ":G", cfarTestCfg.cfarRangeCfg.guardLen,
                    ":W", cfarTestCfg.cfarRangeCfg.winLen,
                    ":Cyc", cfarTestCfg.cfarRangeCfg.cyclicMode,
                    ":grp", cfarTestCfg.cfarRangePeakPruning,
                    ":k", cfarTestCfg.cfarRangeCfg.osKvalue);
                }
                else
                {
                sprintf(featureName, "%s%d%s%d%s%d%s%d",
                    ":G", cfarTestCfg.cfarRangeCfg.guardLen,
                    ":W", cfarTestCfg.cfarRangeCfg.winLen,
                    ":Cyc", cfarTestCfg.cfarRangeCfg.cyclicMode,
                    ":grp", cfarTestCfg.cfarRangePeakPruning );
                }

                test_print("%s", featureName);


                if (cfarTestCfg.dopplerCfarEnabled)
                {
                    if (cfarTestCfg.cfarDopplerCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_CA)
                    {
                        test_print("  DopplerCfar:: CA");
                    }
                    else if (cfarTestCfg.cfarDopplerCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_CAGO)
                    {
                        test_print("  DopplerCfar:: CAGO");
                    }
                    else if (cfarTestCfg.cfarDopplerCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_CASO)
                    {
                        test_print("  DopplerCfar:: CASO");
                    }

                    if (cfarTestCfg.cfarDopplerCfg.aveMode == HWA_NOISE_AVG_MODE_CFAR_OS)
                    {
                        test_print("  DopplerCfar:: CFAROS");

                        sprintf(featureName, "%s%d%s%d%s%d%s%d%s%d",
                            ":G", cfarTestCfg.cfarDopplerCfg.guardLen,
                            ":W", cfarTestCfg.cfarDopplerCfg.winLen,
                            ":Cyc", cfarTestCfg.cfarDopplerCfg.cyclicMode,
                            ":grp", cfarTestCfg.cfarDopplerPeakPruning,
                            ":k", cfarTestCfg.cfarDopplerCfg.osKvalue);
                    }
                    else
                    {
                        sprintf(featureName, "%s%d%s%d%s%d%s%d",
                        ":G", cfarTestCfg.cfarDopplerCfg.guardLen,
                        ":W", cfarTestCfg.cfarDopplerCfg.winLen,
                        ":Cyc", cfarTestCfg.cfarDopplerCfg.cyclicMode,
                        ":grp", cfarTestCfg.cfarDopplerPeakPruning);
                    }
                    test_print(" %s", featureName);
                }
                test_print("\n");

                Test_setProfile(&cfarTestCfg);

                errorCode = DPU_CFARProcHWA_config(cfarDpuHandle, &cfarDpuCfg);
                if (errorCode != 0)
                {
                    test_print("CFAR DPU configure error %d\n", errorCode);
                    DebugP_assert(0);
                    goto exit;;
                }

                errorCode = DPU_CFARProcHWA_process(cfarDpuHandle, &cfarDpuOut);
                if (errorCode != 0)
                {
                    test_print("CFAR DPU process error %d\n", errorCode);
                    DebugP_assert(0);
                    goto exit;;
                }
                /* check the results */
                test_print("Check DPU Process output: ");

                Test_gen_detobj_ref(&cfarTestCfg);
                numCheckPeaksRef = numDetPeaksRef;

                Test_printResults();
                test_print("\n");

                /* check the cfar command */
                if (((testCount & 0x3) == 0)  & (numCheckPeaksRef<TEST_MAX_NUM_CFARDETOBJ_DPU_LIST))
                {
                    if (cfarTestCfg.numDopplerBins > 16)
                    {
                        cfarTestCfg.dpuControlFlag = 1;   //doppler fov
                    }
                    else
                    {
                        cfarTestCfg.dpuControlFlag = 0;  //range fov
                    }
                }
                else if (((testCount & 0x3) == 1) & (numCheckPeaksRef<TEST_MAX_NUM_CFARDETOBJ_DPU_LIST))
                {
                    cfarTestCfg.dpuControlFlag = 0; //range fov
                }
                else if ((testCount & 0x3) == 2)
                {
                    cfarTestCfg.dpuControlFlag = 3; //range cfar
                }
                else
                {
                    if (cfarTestCfg.dopplerCfarEnabled)
                    {
                        cfarTestCfg.dpuControlFlag = 2;//doppler cfar
                    }
                    else
                    {
                        cfarTestCfg.dpuControlFlag = 3; //range cfar
                    }
                }

                if (cfarTestCfg.dpuControlFlag == 0)
                {
                    test_print("Check DPU Cmd_FovRangeCfg output: ");
                }
                else if (cfarTestCfg.dpuControlFlag == 1)
                {
                    test_print("Check DPU Cmd_FovDopplerCfg output: ");
                }
                else if (cfarTestCfg.dpuControlFlag == 2)
                {
                    test_print("Check DPU Cmd_CfarDopplerCfg output: ");
                }
                else
                {
                    test_print("Check DPU Cmd_CfarRangeCfg output: ");
                }
                test_dpu_control_cmd(&cfarTestCfg);
                if (cfarTestCfg.dpuControlFlag > 1) //cfar change, need to recalute the reference
                {
                    Test_gen_detobj_ref(&cfarTestCfg);
                    numCheckPeaksRef = numDetPeaksRef;
                }
                else   //update the previous results only
                {
                    Test_update_detobj_ref(&cfarTestCfg);
                }
                errorCode = DPU_CFARProcHWA_process(cfarDpuHandle, &cfarDpuOut);
                if (errorCode != 0)
                {
                    test_print("CFAR DPU process error %d\n", errorCode);
                    DebugP_assert(0);
                    goto exit;
                }

                Test_printResults();

                test_print("\n\n");

                testCount++;
            }  /* end of kk loop */

        }   /* end of range bin loop */
    } /* end of doppler bin loop */

    /* Free EDMA Channels. */
    /* DPEDMA_freeEDMAChannel(uint32_t *dmaCh, uint32_t *tcc, uint32_t *param, uint32_t *shadowParam)*/
    DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &cfarDpuCfg.res.edmaHwaIn.channel, &cfarDpuCfg.res.edmaHwaIn.tcc,
                &cfarDpuCfg.res.edmaHwaIn.paramId, &cfarDpuCfg.res.edmaHwaIn.shadowPramId );

    DPEDMA_freeEDMAChannel(gEdmaHandle[CONFIG_EDMA0], &cfarDpuCfg.res.edmaHwaInSignature.channel, &cfarDpuCfg.res.edmaHwaInSignature.tcc,
                &cfarDpuCfg.res.edmaHwaInSignature.paramId, &cfarDpuCfg.res.edmaHwaInSignature.shadowPramId );


    test_print("----------------------------------------\n\n");
    if (finalResults == 1)
    {
        test_print ("All tests have passed!!\n");
        test_print("All Tests PASSED!\n");
        MCPI_setFeatureTestResult("CFAR_DPU", MCPI_TestResult_PASS);
    }
    else
    {
        test_print("Test FAILED!\n");
        MCPI_setFeatureTestResult("CFAR_DPU", MCPI_TestResult_FAIL);
    }

    taskEndTime=ClockP_getTimeUsec();
    test_print("Test Time Cycles: %lld\n",taskEndTime-taskStartTime);
    test_print("Test Finished\n");

    MCPI_setTestResult();

exit:
    if (fileId != NULL)
    {
        fclose(fileId);
    }
    Board_driversClose();
    Drivers_close();
    HWA_close(hwaHandle);
    errorCode = DPU_CFARProcHWA_deinit(cfarDpuHandle);
    if (errorCode != 0)
    {
        test_print("CFAR DPU DE_INIT fail\n");
        DebugP_assert(0);
    }
    fileId = fopen("cfartestdone.txt","w");
    if(fileId != NULL)
    {
        fclose(fileId);
    }

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


/* Sorting function used by reference CFAR-OS function */
void sort(uint16_t inp[restrict], uint16_t output[restrict], int32_t len)
{
    int32_t i,j;
    uint16_t tmp;

    for (i = 0; i< len; i++)
    {
        output[i] = inp[i];
    }

    for (i = 0; i< len-1; i++)
    {
        /*i=0, first round the largest goes to output[len-1]
        i=1, second round the 2nd-largest goes to output[len-2], ...
        i=30, 31th round the 31th-largest i.e. 2nd smallest goes to out[1] if len=32*/
        for (j = 0; j< len-1-i; j++)
        {
            if (output[j] > output[j+1])
            {
                tmp         = output[j];
                output[j]   = output[j+1];
                output[j+1] = tmp;
            }
        }
    }
} /* sort */

#define WINDOW_SIZE 128
/* Reference CFAR-OS function */
uint16_t ref_cfaros(uint16_t input[restrict],
                uint32_t inplen,
                uint32_t winlen,
                uint16_t k,
                uint32_t thresh,
                uint8_t  cyclic,
                uint16_t detlist[restrict],
                uint32_t koslist[restrict])
{
    volatile uint32_t i, j;
    uint16_t numDet = 0;
    uint16_t kth_os;
    uint16_t search_window[WINDOW_SIZE+1];
    uint16_t sorted[WINDOW_SIZE+1];


    if (cyclic) /* cyclic mode, wrap around the edge */
    {
        for (i = 0; i < winlen/2; i++)
        {
            /*use winlen/2 samples at the end of input buffer to fill the first half of window */
            search_window[i] = input[inplen-winlen/2+i];
            /*input[0] in the middle of window, input[1] to input[winlen/2-1] fill the second half */
            search_window[winlen/2 +i] = input[i];
        }
    }
    else /* non-cyclic mode */
    {
        for (i = 0; i < winlen/2; i++)
        {
            search_window[i] = 0xFFFF; //use max uint16 to prefill the first half of window.
            search_window[winlen/2 +i] = input[i];
        }
    }

    for (i = 0; i < inplen; i++)
    {
        search_window[winlen] = input[i+ (winlen/2)];
        if ( i+ (winlen/2) >= inplen )
        {
            if (cyclic)
                search_window[winlen] = input[i+ (winlen/2) - inplen];
            else
                search_window[winlen] = 0xFFFF;
        }
        sort(search_window, sorted, winlen+1);
        kth_os = sorted[k]; //sorted[0] is the smallest of all, sorted[0]<sorted[1]..., sorted[k] is the k-th smallest

        //if (((uint32_t)input[i]*64) > (thresh*kth_os))
        if (((uint32_t)input[i]) > (thresh+kth_os))
        {
            detlist[numDet] = i;
            koslist[numDet] = kth_os;
            numDet++;
        }

        /* update search window: slide right by one sample */
        for (j=0; j<winlen; j++)
        {
            search_window[j] = search_window[j+1];
        }
    }

    return(numDet);

}/* ref_cfaros */
