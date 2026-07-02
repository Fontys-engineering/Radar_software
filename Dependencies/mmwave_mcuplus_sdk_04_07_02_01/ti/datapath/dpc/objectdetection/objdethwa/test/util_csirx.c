/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 *   @file  util_csirx.c
 *
 *   @brief
 *      CSIRX Configuration Utility functions
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* Standard Include Files. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <ti/datapath/dpc/objectdetection/objdethwa/test/dssgenerated/ti_drivers_open_close.h>
#include <kernel/dpl/SemaphoreP.h>
#include <ti/datapath/dpc/objectdetection/objdethwa/test/util_csirx.h>


/*! This is supplied at command line when application builds this file. This file
 * is owned by the application and contains all resource partitioning, an
 * application may include more than one DPC and also use resources outside of DPCs.
 * The resource definitions used by this object detection DPC are prefixed by DPC_OBJDET_ */
#include APP_RESOURCE_FILE

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*! In debug mode, the line end codes are not detected due to a silicon problem
 *  but generating two line ends instead of one would result in detection of one
 *  line end code. However silicon team has not determined if other logic
 *  is sound, so the define below has been provided to disable/enable it */
#define CSIRX_ENABLE_DEBUG_MODE_LINE_END_WORKAROUND

/*! short packet synthesis from virtual channel and data type */
#define TEST_DEBUG_MODE_SP(VC, DT) (0xFF000000U | (VC << 6U) | DT)

/*! Frame Start Code synthesis from virtual channel */
#define TEST_DEBUG_MODE_FSC(VC) TEST_DEBUG_MODE_SP(VC, 0x0)
/*! Frame End Code synthesis from virtual channel */
#define TEST_DEBUG_MODE_FEC(VC) TEST_DEBUG_MODE_SP(VC, 0x1)

/*! Line Start Code synthesis from virtual channel */
#define TEST_DEBUG_MODE_LSC(VC) TEST_DEBUG_MODE_SP(VC, 0x2)

/*! Line End Code synthesis from virtual channel */
#define TEST_DEBUG_MODE_LEC(VC) TEST_DEBUG_MODE_SP(VC, 0x3)

/*! Packet Header synthesis from virtual channel, data type and word count */
#define TEST_DEBUG_MODE_PH(VC, DT, WC) (0xFF000000U | ((WC) << 8) | (VC << 6U) | DT)

#define TEST_CONTEXT                 DPC_OBJDET_CSIRX_CHIRP_DATA_CONTEXT
#define TEST_FORMAT                  CSIRX_FORMAT_RAW8
#define TEST_VC                      3
#define TEST_USER_DEFINED_MAPPING    CSIRX_USER_DEFINED_FORMAT_RAW8
#define TEST_NUM_FRAMES              2  /* keep even */
#define TEST_NUM_LINES               4  /* keep even */
#define TEST_PAYLOAD_BYTES_PER_LINE  4  /* keep this multiple of 4 */

#ifndef DPC_SKIP_CSI_TRIGGER
    extern SemaphoreP_Object gCsirxEndOfLineSemHandle;
#endif

/*! holds configuration structures of config APIs */
typedef struct testConfig_s
{
    CSIRX_DphyConfig DPHYcfg;
    CSIRX_ComplexioConfig complexIOcfg;
    CSIRX_CommonConfig commonCfg;
    CSIRX_ContextConfig contextCfg;
} testConfig_t;

/*! holds IRQ status of various IRQs */
typedef struct TestIRQs_s
{
    CSIRX_ContextIntr context[CONFIG_CSIRX_NUM_INSTANCES];
    CSIRX_CommonIntr common;
    CSIRX_ComplexioLanesIntr  complexIOlanes;
} TestIRQs_t;

/*! holds call-back counts, incremented in individual callback functions */
typedef struct TestCallBackCounts_s
{
    uint32_t common;
    uint32_t combinedEOL;
    uint32_t combinedEOF;
    uint32_t EOF0;
    uint32_t EOF1;
    uint32_t SOF0;
    uint32_t SOF1;
    uint32_t contextEOL[CONFIG_CSIRX_NUM_INSTANCES];
} TestCallBackCounts_t;

/*! test state */
typedef struct TestState_s
{
    TestIRQs_t IRQ;
    TestCallBackCounts_t callbackCount;
    uint32_t receivedBuffer;
} TestState_t;

TestState_t testState;

/**
 *  @b Description
 *  @n
 *      CSIRX common call back function
 */
void Test_commonCallback(CSIRX_Handle handle, void *arg, struct CSIRX_CommonIntr_s *IRQ)
{
    uint8_t i;
    uint32_t errorCode;

    DebugP_assert(handle != NULL);
    testState.callbackCount.common++;

    testState.IRQ.common = *IRQ;

    if(IRQ->isComplexioError)
    {
        errorCode = CSIRX_complexioGetPendingIntr(handle, &testState.IRQ.complexIOlanes);
        /*errorCode = CSIRX_getComplexIOlanesIRQ(handle,
                                                &testState.IRQ.complexIOlanes);*/
        DebugP_assert(errorCode == SystemP_SUCCESS);

        errorCode = CSIRX_complexioClearAllIntr(handle);
        DebugP_assert(errorCode == SystemP_SUCCESS);
    }

    for(i = 0; i < CONFIG_CSIRX_NUM_INSTANCES; i++)
    {
        if(IRQ->isContextIntr[i] == (bool)true)
        {
            DebugP_assert(i == TEST_CONTEXT);
            errorCode = CSIRX_contextGetPendingIntr(handle, i, &testState.IRQ.context[i]);
            DebugP_assert(errorCode == SystemP_SUCCESS);

            errorCode = CSIRX_contextClearAllIntr(handle, i);
            DebugP_assert(errorCode == SystemP_SUCCESS);
        }
    }
}

void Test_combinedEOLcallback(CSIRX_Handle handle, uint32_t arg)
{
    DebugP_assert(handle != NULL);
    testState.callbackCount.combinedEOL++;
#ifndef DPC_SKIP_CSI_TRIGGER
    SemaphoreP_post(&gCsirxEndOfLineSemHandle);
#endif
}

void Test_SOF0callback(CSIRX_Handle handle, uint32_t arg, uint8_t contextId)
{
    DebugP_assert(handle != NULL);
    DebugP_assert(contextId == TEST_CONTEXT);
    testState.callbackCount.SOF0++;
}


/**
 *  @b Description
 *  @n
 *      Initializes  test state and test configuration.
 */
void Test_init(void)
{
    memset(&testState, 0, sizeof(testState));
    gCsirxCommonConfig[TEST_CONTEXT].enableIntr.isContextIntr[TEST_CONTEXT] = true;
}

/**
 *  @b Description
 *  @n
 *      Initializes  CSIRX.
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 *
 */
int32_t util_csirxInit()
{
    Test_init();

    return 0;
}

/**
 *  @b Description
 *  @n
 *      Open  CSIRX.
 *
 *  @param[in]  instanceId    CSIRX instance ID
 *
 *  @param[out]  errorCode    Error code: =0 - success, <0 - error
 *
 *  @retval CSIRX_Handle CSIRX handle
 *
 */
CSIRX_Handle util_csirxOpen(uint8_t instanceId, int32_t *errorCode)
{
    CSIRX_Handle         handle;

    /* Open Instance */
    handle = CSIRX_open(instanceId);

    printf("CSIRX instance opened");

    /* reset csi */
    *errorCode = CSIRX_reset(handle);
    if(*errorCode != SystemP_SUCCESS)
    {
        printf("CSIRX_reset failed, errorCode = %d\n", *errorCode);
        goto exit;
    }

exit:
    return handle;
}

/**
 *  @b Description
 *  @n
 *      Configure  CSIRX.
 *
 *  @param[in]  handle    CSIRX handle
 *
 *  @param[out]  instanceId    CSIRX instance ID
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 *
 */
int32_t util_csirxConfig(CSIRX_Handle handle, uint8_t instanceId)
{
    int32_t errorCode;

    /* config common */
    errorCode = CSIRX_commonSetConfig(handle, &gCsirxCommonConfig[0]);
    if(errorCode != SystemP_SUCCESS)
    {
        printf("CSIRX_configCommon failed, errorCode = %d\n", errorCode);
        goto exit;
    }

    /* enable interface */
    errorCode = CSIRX_commonEnable(handle);
    if(errorCode != SystemP_SUCCESS)
    {
        printf("CSIRX_enableInterface failed, errorCode = %d\n", errorCode);
        goto exit;
    }

    /* config contexts */
    gConfigCsirx0ContextConfig[0].pingPongConfig.pingAddress = CSL_DSS_HWA_DMA0_RAM_BANK0_BASE;
    gConfigCsirx0ContextConfig[0].pingPongConfig.pongAddress = CSL_DSS_HWA_DMA0_RAM_BANK1_BASE;
    errorCode = CSIRX_contextSetConfig(handle, 0, &gConfigCsirx0ContextConfig[0]);
    if(errorCode != SystemP_SUCCESS)
    {
        printf("CSIRX_configContext failed, errorCode = %d\n", errorCode);
        goto exit;
    }

    /* enable context */
    errorCode = CSIRX_contextEnable(handle, 0);
    if(errorCode != SystemP_SUCCESS)
    {
        printf("CSIRX_enableContext failed, errorCode = %d\n", errorCode);
        goto exit;
    }

    /* Debug mode, first flush FIFO - disable debug mode and enable interface */
    CSIRX_debugModeEnable(handle);

    /* enable interface */
    errorCode = CSIRX_commonEnable(handle);
    if(errorCode != SystemP_SUCCESS)
    {
        printf("CSIRX_enableInterface failed, errorCode = %d\n", errorCode);
        goto exit;
    }

    /* Enable Debug mode */
    CSIRX_debugModeEnable(handle);

exit:
    return(errorCode);
}

/**
 *  @b Description
 *  @n
 *      Push data through CSIRX in Debug mode
 *
 *  @param[in]  handle      Handle CSIRX object
 *  @param[in]  instanceId  Instance ID
 *  @param[in]  srcDataBuf  Pointer to data buffer with complex samples (complex 16)
 *                          from which the samples are pushed through CSIRX
 *  @param[in]  numWords    Number of 32-bit words (complex samples) in srcDataBuf, (number of samples per line)
 *  @param[in]  lineInd     Line index, valid values: 0 to numLines-1
 *  @param[in]  numLines    Number of lines
 *
 *  @retval
 *      Success     - 0
 *  @retval
 *      Error       - <0
 *
 */
int32_t util_csirxPushData(CSIRX_Handle handle,
                          uint8_t instanceId,
                          uint32_t *srcDataBuf,
                          uint32_t numWords,
                          uint32_t lineInd,
                          uint32_t numLines)
{
    uint32_t wordInd;
    uint32_t errorCode = SystemP_SUCCESS;

    if (lineInd == 0)
    {
        /* Send Frame start Code (short packet) */
        CSIRX_debugModeSetShortPacket(handle, TEST_DEBUG_MODE_FSC(TEST_VC));
    }

    /* Send Line start Code (short packet) */
    CSIRX_debugModeSetShortPacket(handle, TEST_DEBUG_MODE_LSC(TEST_VC));

    /* Send Long Packet Header (PH) */
    CSIRX_debugModeSetLongPacketHeader(handle,
        TEST_DEBUG_MODE_PH(TEST_VC, TEST_FORMAT, sizeof(uint32_t) * numWords));

    /* Send words */
    for (wordInd = 0; wordInd < numWords; wordInd++)
    {
        CSIRX_debugModeSetLongPacketPayload(handle, srcDataBuf[wordInd]);
    }

    /* Send Long Packet Footer (PF) */
    /* It seems in debug mode the PF is automatically generated */

    /* Send Line End Code (short packet) */
    CSIRX_debugModeSetShortPacket(handle, TEST_DEBUG_MODE_LEC(TEST_VC));
#ifdef CSIRX_ENABLE_DEBUG_MODE_LINE_END_WORKAROUND
    CSIRX_debugModeSetShortPacket(handle, TEST_DEBUG_MODE_LEC(TEST_VC));
#endif

    if (lineInd == (numLines-1))
    {
        /* Send Frame end Code (short packet) */
        CSIRX_debugModeSetShortPacket(handle, TEST_DEBUG_MODE_FEC(TEST_VC));
    }
    return errorCode;
}



