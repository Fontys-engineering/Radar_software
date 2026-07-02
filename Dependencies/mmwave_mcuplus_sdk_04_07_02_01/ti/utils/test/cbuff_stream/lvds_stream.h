/**
 *   @file  lvds_stream.h
 *
 *   @brief
 *      LVDS stream header file.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2020 - 2021 Texas Instruments, Inc.
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
 *   @file  lvds_stream.h
 *
 *   @brief
 *      CBUFF Stream application code
 */

 /** @mainpage MSS/DSS CBUFF Stream Application (AWR294X, AWR2544 and AWR2x44P)
 *
 * ## Introduction #
 *
 * CBUFF Stream: This application can be executed on:
 *   - MSS or DSS for AWR294X and AWR2x44P
 *   - MSS for AWR2544
 *
 *  The CBUFF Stream application is responsible for following:
 *   - Setup the control path through MMWave module.
 *   - Setup data path to stream ADC data over LVDS interface.
 *
 *  The application initializes the modules for the control & datapath. This
 *  would imply the configurations of the following modules:-
 *   1. Control path - Communication with BSS over mailbox to configure the
 *      sensor with hardcoded configuration to open mmWave control module.
 *   2. Data Path - Configuration of ADCBUF and CBUFF  module to stream
 *      ADC data received from BSS on to LVDS interface.
 *
 * ## Configuration #
 *
 * The application uses hardcoded configuration for the chirp, profile and frame.
 * This configuration is then passed to the BSS via the mmWave control module.
 *
 * Below are the configuration details for control path:
 *
 *  \ref TESTSOURCE_NUM_RX_ANTENNAS
 *
 *  \ref TESTSOURCE_NUM_ADC_SAMPLES
 *
 *  \ref TEST_SOURCE_NUM_LOOPS
 *
 *  \ref TESTSOURCE_CHIRP_START_INDEX
 *
 *  \ref TESTSOURCE_CHIRP_END_INDEX
 *
 *  \ref TESTSOURCE_NUM_FRAMES
 *
 */
/* ============================================================================
 */

#ifndef MSS_LVDS_STREAM_H
#define MSS_LVDS_STREAM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <kernel/dpl/SemaphoreP.h>
#include <ti/utils/hsiheader/hsiheader.h>

#include <ti/control/mmwave/mmwave.h>

/**
 * @brief   This is the maximum number of EDMA Channels which is used by
 * the HW Session
 */
#define LVDS_STREAM_HW_SESSION_MAX_EDMA_CHANNEL             (11U)

/*************************LVDS streaming EDMA resources*******************************/
/*EDMA instance used*/
#define LVDS_STREAM_EDMA_SHADOW_BASE         64

#if defined (SOC_AWR294X) || defined(SOC_AWR2X44P)
/* CBUFF EDMA trigger channels - Select DSS_TPCCA in sysconfig. For other TPCCs, modify HW triggered channels accordingly, */
#define LVDS_STREAM_CBUFF_EDMA_CH_0          EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ0
#define LVDS_STREAM_CBUFF_EDMA_CH_1          EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ1

/* HW Session*/
#define LVDS_STREAM_HW_SESSION_EDMA_CH_0     EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ0
#define LVDS_STREAM_HW_SESSION_EDMA_CH_1     EDMA_DSS_TPCC_A_EVT_HWA_DMA_REQ1
#define LVDS_STREAM_HW_SESSION_EDMA_CH_2     EDMA_DSS_TPCC_A_EVT_RTIA_DMA_REQ0
#define LVDS_STREAM_HW_SESSION_EDMA_CH_3     EDMA_DSS_TPCC_A_EVT_RTIA_DMA_REQ1
#define LVDS_STREAM_HW_SESSION_EDMA_CH_4     EDMA_DSS_TPCC_A_EVT_RTIA_DMA_REQ2
#define LVDS_STREAM_HW_SESSION_EDMA_CH_5     EDMA_DSS_TPCC_A_EVT_WDT_DMA_REQ2
#define LVDS_STREAM_HW_SESSION_EDMA_CH_6     EDMA_DSS_TPCC_A_EVT_WDT_DMA_REQ3
#define LVDS_STREAM_HW_SESSION_EDMA_CH_7     EDMA_DSS_TPCC_A_EVT_MCRC_DMA_REQ0
#define LVDS_STREAM_HW_SESSION_EDMA_CH_8     EDMA_DSS_TPCC_A_EVT_MCRC_DMA_REQ1
#define LVDS_STREAM_HW_SESSION_EDMA_CH_9     EDMA_DSS_TPCC_A_EVT_SCIA_RX_DMA_REQ
#define LVDS_STREAM_HW_SESSION_EDMA_CH_10    EDMA_DSS_TPCC_A_EVT_SCIA_TX_DMA_REQ

/* SW Session*/
#define LVDS_STREAM_SW_SESSION_EDMA_CH_0     EDMA_DSS_TPCC_A_EVT_DFE_FRAME_START_TO_DSS
#define LVDS_STREAM_SW_SESSION_EDMA_CH_1     EDMA_DSS_TPCC_A_EVT_RSS_DATA_CAPTURE_ENABLE_FALL
#define LVDS_STREAM_SW_SESSION_EDMA_CH_2     EDMA_DSS_TPCC_A_EVT_CBUFF_DMA_REQ0

/*shadow*/
/*shadow CBUFF trigger channels*/
#define LVDS_STREAM_CBUFF_EDMA_SHADOW_CH_0   (LVDS_STREAM_EDMA_SHADOW_BASE + 0U)
#define LVDS_STREAM_CBUFF_EDMA_SHADOW_CH_1   (LVDS_STREAM_EDMA_SHADOW_BASE + 1U)

/* HW Session*/
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_0   (LVDS_STREAM_EDMA_SHADOW_BASE + 2U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_1   (LVDS_STREAM_EDMA_SHADOW_BASE + 3U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_2   (LVDS_STREAM_EDMA_SHADOW_BASE + 4U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_3   (LVDS_STREAM_EDMA_SHADOW_BASE + 5U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_4   (LVDS_STREAM_EDMA_SHADOW_BASE + 6U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_5   (LVDS_STREAM_EDMA_SHADOW_BASE + 7U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_6   (LVDS_STREAM_EDMA_SHADOW_BASE + 8U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_7   (LVDS_STREAM_EDMA_SHADOW_BASE + 9U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_8   (LVDS_STREAM_EDMA_SHADOW_BASE + 10U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_9   (LVDS_STREAM_EDMA_SHADOW_BASE + 11U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_10  (LVDS_STREAM_EDMA_SHADOW_BASE + 12U)
#endif

#if defined (SOC_AWR2544)
/* CBUFF EDMA trigger channels */
#define LVDS_STREAM_CBUFF_EDMA_CH_0          EDMA_RSS_TPCC_A_EVT_DSS_CBUFF_DMA_REQ0
#define LVDS_STREAM_CBUFF_EDMA_CH_1          EDMA_RSS_TPCC_A_EVT_DSS_CBUFF_DMA_REQ1

/* HW Session*/
#define LVDS_STREAM_HW_SESSION_EDMA_CH_0     EDMA_RSS_TPCC_A_EVT_FREE_0
#define LVDS_STREAM_HW_SESSION_EDMA_CH_1     EDMA_RSS_TPCC_A_EVT_FREE_1
#define LVDS_STREAM_HW_SESSION_EDMA_CH_2     EDMA_RSS_TPCC_A_EVT_FREE_2
#define LVDS_STREAM_HW_SESSION_EDMA_CH_3     EDMA_RSS_TPCC_A_EVT_FREE_3
#define LVDS_STREAM_HW_SESSION_EDMA_CH_4     EDMA_RSS_TPCC_A_EVT_FREE_4
#define LVDS_STREAM_HW_SESSION_EDMA_CH_5     EDMA_RSS_TPCC_A_EVT_FREE_5
#define LVDS_STREAM_HW_SESSION_EDMA_CH_6     EDMA_RSS_TPCC_A_EVT_FREE_6
#define LVDS_STREAM_HW_SESSION_EDMA_CH_7     EDMA_RSS_TPCC_A_EVT_FREE_7
#define LVDS_STREAM_HW_SESSION_EDMA_CH_8     EDMA_RSS_TPCC_A_EVT_FREE_8
#define LVDS_STREAM_HW_SESSION_EDMA_CH_9     EDMA_RSS_TPCC_A_EVT_FREE_9
#define LVDS_STREAM_HW_SESSION_EDMA_CH_10    EDMA_RSS_TPCC_A_EVT_FREE_10

/* SW Session*/
#define LVDS_STREAM_SW_SESSION_EDMA_CH_0     EDMA_RSS_TPCC_A_EVT_FREE_11
#define LVDS_STREAM_SW_SESSION_EDMA_CH_1     EDMA_RSS_TPCC_A_EVT_FREE_12
#define LVDS_STREAM_SW_SESSION_EDMA_CH_2     EDMA_RSS_TPCC_A_EVT_FREE_13

/*shadow*/
/*shadow CBUFF trigger channels*/
#define LVDS_STREAM_CBUFF_EDMA_SHADOW_CH_0   (LVDS_STREAM_EDMA_SHADOW_BASE + 0U)
#define LVDS_STREAM_CBUFF_EDMA_SHADOW_CH_1   (LVDS_STREAM_EDMA_SHADOW_BASE + 1U)

/* HW Session*/
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_0   (LVDS_STREAM_EDMA_SHADOW_BASE + 2U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_1   (LVDS_STREAM_EDMA_SHADOW_BASE + 3U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_2   (LVDS_STREAM_EDMA_SHADOW_BASE + 4U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_3   (LVDS_STREAM_EDMA_SHADOW_BASE + 5U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_4   (LVDS_STREAM_EDMA_SHADOW_BASE + 6U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_5   (LVDS_STREAM_EDMA_SHADOW_BASE + 7U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_6   (LVDS_STREAM_EDMA_SHADOW_BASE + 8U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_7   (LVDS_STREAM_EDMA_SHADOW_BASE + 9U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_8   (LVDS_STREAM_EDMA_SHADOW_BASE + 10U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_9   (LVDS_STREAM_EDMA_SHADOW_BASE + 11U)
#define LVDS_STREAM_HW_SESSION_EDMA_SHADOW_CH_10  (LVDS_STREAM_EDMA_SHADOW_BASE + 12U)
#endif

/** @addtogroup TEST_CONTROL_PATH_CONFIG_DEFINITIONS
 @{ */

 /**
 * @brief
 * This macro defines the number of receive antennas.
 */
#define TESTSOURCE_NUM_RX_ANTENNAS         (4U)

 /**
 * @brief
 * This macro defines the number of ADC Samples.
 */
#define TESTSOURCE_NUM_ADC_SAMPLES         (256U)

 /**
 * @brief
 * This macro defines the number of chirps.
 */
#define TEST_SOURCE_NUM_LOOPS              (1U)

 /**
 * @brief
 * This macro defines the chirp start index.
 */
#define TESTSOURCE_CHIRP_START_INDEX       (0U)

 /**
 * @brief
 * This macro defines the chirp end index.
 */
#define TESTSOURCE_CHIRP_END_INDEX         (0U)

 /**
 * @brief
 * This macro defines the number of frame.
 */
#define TESTSOURCE_NUM_FRAMES              (2U)

/**
@}
*/

/**
 * @brief
 *  LVDS streaming user data header
 *
 * @details
 *  The LVDS SW streaming user data header.
 */
typedef struct LVDSUserDataHeader
{
    /**
     * @brief   Frame number.
     */
    uint32_t     frameNum;

    /**
     * @brief   Sub-Frame number. Always 0 when advanced frame is not enabled.
     *          Note although the subFrameNum does not need to be 16-bits (it needs to be
     *          only 8-bits), we keep it 16-bit for parsing convenience as compiler
     *          does not insert holes in this case
     */
    uint16_t     subFrameNum;

    /**
     * @brief   Number of detected objects.
     */
    uint16_t     detObjNum;
} LVDSUserDataHeader_t;


/**
 * @brief
 *  LVDS streaming MCB
 *
 * @details
 *  The LVDS streaming MCB.
 */
typedef struct LVDSStream_MCB
{
    /**
    * @brief   Handle to the CBUFF Driver
    */
    CBUFF_Handle             cbuffHandle;

    /**
     * @brief   EDMA Channel Allocator Index for the HW Session
     */
    uint8_t                  hwSessionEDMAChannelAllocatorIndex;

    /**
     * @brief   EDMA Channel Resource Table: This is used for creating the CBUFF Session.
     */
    CBUFF_EDMAChannelCfg     hwSessionEDMAChannelTable[LVDS_STREAM_HW_SESSION_MAX_EDMA_CHANNEL];

    /**
     * @brief   HW session HSI header.
     */
    HSIHeader                hwSessionHSIHeader;

    /**
     * @brief   True if hw session HSI header is allocated, false otherwise
     */
    bool                     isHwSessionHSIHeaderAllocated;

    /**
     * @brief   Handle to the HW CBUFF Session Handle.
     */
    CBUFF_SessionHandle      hwSessionHandle;

    /**
     * @brief   Number of HW frame done interrupt received.
     */
    uint16_t                 hwFrameDoneCount;

    /**
     * @brief   Semaphore handle to signal hw session done.
     */
    SemaphoreP_Object        hwFrameDoneSemHandle;

    /**
     * @brief   Pointer to user data header.
     */
    LVDSUserDataHeader_t    *userDataHeader;
} LVDSStream_MCB_t;

/**
 * @brief
 *  ADCBUF configuration (meant for CLI configuration)
 *
 * @details
 *  The structure is used to hold all the relevant configuration
 *  which is used to configure ADCBUF.
 *
 */
typedef struct LVDSStream_ADCBufCfg_t
{
    /*! ADCBUF out format:
        0-Complex,
        1-Real */
    uint8_t     adcFmt;

    /*! ADCBUF IQ swap selection:
        0-I in LSB, Q in MSB,
        1-Q in LSB, I in MSB */
    uint8_t     iqSwapSel;

    /*! ADCBUF channel interleave configuration:
        0-interleaved(not supported on XWR16xx),
        1- non-interleaved */
    uint8_t     chInterleave;

    /**
     * @brief   Chirp Threshold configuration used for ADCBUF buffer
     */
    uint8_t     chirpThreshold;
}LVDSStream_ADCBufCfg;

/**
 * @brief
 *  LVDS streaming configuration
 *
 * @details
 *  The structure is used to hold all the relevant configuration
 *  for the LVDS streaming.
 */
typedef struct LvdsStream_Cfg_t
{
    /**
     * @brief  HSI Header enabled/disabled flag. Only applicable for HW streaming.
     *         Will be ignored for SW streaming which will always have HSI header.
     */
    bool        isHeaderEnabled;

    /*! HW STREAMING DISABLED */
#define LVDS_STREAM_CFG_DATAFMT_DISABLED   0

    /*! ADC */
#define LVDS_STREAM_CFG_DATAFMT_ADC        1

    /*! CP_ADC_CQ */
#define LVDS_STREAM_CFG_DATAFMT_CP_ADC_CQ  4

    /*! HW streaming data format:
        0-HW STREAMING DISABLED
        1-ADC
        2-Reserved
        3-Reserved
        4-CP_ADC_CQ
    */
    uint8_t     dataFmt;

    /**
     * @brief  SW enabled/disabled flag
     */
    bool        isSwEnabled;
} LvdsStream_Cfg;

/**
 * @brief
 *  Millimeter Wave Demo Data Path Information.
 *
 * @details
 *  The structure is used to hold all the relevant information for
 *  the data path.
 */
typedef struct LVDSStream_SubFrameCfg_t
{
    /*! @brief ADC buffer configuration storage */
    LVDSStream_ADCBufCfg adcBufCfg;

    /*! @brief  LVDS stream configuration */
    LvdsStream_Cfg lvdsStreamCfg;

    /*! @brief CQ signal & image band monitor buffer size */
    uint32_t    sigImgMonTotalSize;

    /*! @brief CQ RX Saturation monitor buffer size */
    uint32_t    satMonTotalSize;

    /*! @brief  Number of ADC samples */
    uint16_t    numAdcSamples;

    /*! @brief  Number of chirps per sub-frame */
    uint16_t    numChirpsPerSubFrame;

} LVDSStream_SubFrameCfg;


/**
 * @brief
 *  LVDS Steam MCB
 *
 * @details
 *  The structure is used to hold all the relevant information for the
 *  LVDS Stream test.
 */
typedef struct LVDSStream_MSS_MCB_t
{
    /*! @brief      ADCBuf driver handle */
    ADCBuf_Handle               adcBufHandle;

    /*! @brief   Handle of the EDMA driver, used for CBUFF */
    EDMA_Handle                 edmaHandle;

    /*! @brief   Handle of the MMWave. */
    MMWave_Handle               mmWaveHandle;

    /*! @brief   Number of EDMA event Queues (tc) */
    uint8_t                     numEdmaEventQueues;

    /*! @brief      Object Detection DPC subFrame configuration */
    LVDSStream_SubFrameCfg         subFrameCfg[RL_MAX_SUBFRAMES];

    /*! @brief   this structure is used to hold all the relevant information
        for the mmw demo LVDS stream*/
    LVDSStream_MCB_t             lvdsStream;

} LVDSStream_MCB;

int32_t LVDSStream_Init (void);
int32_t LVDSStream_HwConfig (uint8_t subFrameIndx);
void LVDSStream_configLVDSHwData(uint8_t subFrameIndx);
void LVDSStream_DeleteHwSession (void);


#ifdef __cplusplus
}
#endif

#endif
