/**
 *   @file  csitx_stream.h
 *
 *   @brief
 *      CSI-TX stream header file.
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2025 Texas Instruments, Inc.
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
 *   @file  csitx_stream.h
 *
 *   @brief
 *      CSITX Stream application code
 */

 /** @mainpage MSS CSITX Stream Application (AWR2544)
 *
 * ## Introduction #
 *
 * CSITX Stream: This application can be executed on:
 *   - MSS for AWR2544
 *
 *  The CSITX Stream application is responsible for following:
 *   - Setup the control path through MMWave module.
 *   - Setup data path to stream ADC data from CBUFF over CSITX.
 *
 *  The application initializes the modules for the control & datapath. This
 *  would imply the configurations of the following modules:-
 *   1. Control path - Communication with BSS over mailbox to configure the
 *      sensor with hardcoded configuration to open mmWave control module.
 *   2. Data Path - Configuration of ADCBUF and CBUFF  module to stream
 *      ADC data received from BSS on to CSITX.
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
 * @note 
 *   1.  Enable CSI-TX for Cbuff driver in Sysconfig, and add required
 *       configurations for CSI-TX driver in the Sysconfig to use Cbuff with CSI-TX.
 *   2.  CSI-TX will not work on AWR2544 EVM board by default as LVDS is enabled.
 *       HW changes are required on AWR2544 EVM board to enable CSI-TX. Refer EVM Schematic
 *       and EVM Userguide for more details on the HW changes need to be done.
 *
 */
/* ============================================================================
 */

#ifndef MSS_CSITX_STREAM_H
#define MSS_CSITX_STREAM_H

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
#define CSITX_STREAM_HW_SESSION_MAX_EDMA_CHANNEL             (11U)

/*************************CBUFF with CSI-TX streaming EDMA resources*******************************/
/*EDMA instance used*/
#define CSITX_STREAM_EDMA_SHADOW_BASE         64

/* CSITX_STREAM - CBUFF EDMA trigger channels */
#define CSITX_STREAM_CBUFF_EDMA_CH_0          EDMA_RSS_TPCC_A_EVT_DSS_CBUFF_DMA_REQ0
#define CSITX_STREAM_CBUFF_EDMA_CH_1          EDMA_RSS_TPCC_A_EVT_DSS_CBUFF_DMA_REQ1

/* HW Session*/
#define CSITX_STREAM_HW_SESSION_EDMA_CH_0     EDMA_RSS_TPCC_A_EVT_FREE_0
#define CSITX_STREAM_HW_SESSION_EDMA_CH_1     EDMA_RSS_TPCC_A_EVT_FREE_1
#define CSITX_STREAM_HW_SESSION_EDMA_CH_2     EDMA_RSS_TPCC_A_EVT_FREE_2
#define CSITX_STREAM_HW_SESSION_EDMA_CH_3     EDMA_RSS_TPCC_A_EVT_FREE_3
#define CSITX_STREAM_HW_SESSION_EDMA_CH_4     EDMA_RSS_TPCC_A_EVT_FREE_4
#define CSITX_STREAM_HW_SESSION_EDMA_CH_5     EDMA_RSS_TPCC_A_EVT_FREE_5
#define CSITX_STREAM_HW_SESSION_EDMA_CH_6     EDMA_RSS_TPCC_A_EVT_FREE_6
#define CSITX_STREAM_HW_SESSION_EDMA_CH_7     EDMA_RSS_TPCC_A_EVT_FREE_7
#define CSITX_STREAM_HW_SESSION_EDMA_CH_8     EDMA_RSS_TPCC_A_EVT_FREE_8
#define CSITX_STREAM_HW_SESSION_EDMA_CH_9     EDMA_RSS_TPCC_A_EVT_FREE_9
#define CSITX_STREAM_HW_SESSION_EDMA_CH_10    EDMA_RSS_TPCC_A_EVT_FREE_10

/* SW Session*/
#define CSITX_STREAM_SW_SESSION_EDMA_CH_0     EDMA_RSS_TPCC_A_EVT_FREE_11
#define CSITX_STREAM_SW_SESSION_EDMA_CH_1     EDMA_RSS_TPCC_A_EVT_FREE_12
#define CSITX_STREAM_SW_SESSION_EDMA_CH_2     EDMA_RSS_TPCC_A_EVT_FREE_13

/*shadow*/
/*CSITX_STREAM -shadow CBUFF trigger channels*/
#define CSITX_STREAM_CBUFF_EDMA_SHADOW_CH_0   (CSITX_STREAM_EDMA_SHADOW_BASE + 0U)
#define CSITX_STREAM_CBUFF_EDMA_SHADOW_CH_1   (CSITX_STREAM_EDMA_SHADOW_BASE + 1U)

/* HW Session*/
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_0   (CSITX_STREAM_EDMA_SHADOW_BASE + 2U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_1   (CSITX_STREAM_EDMA_SHADOW_BASE + 3U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_2   (CSITX_STREAM_EDMA_SHADOW_BASE + 4U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_3   (CSITX_STREAM_EDMA_SHADOW_BASE + 5U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_4   (CSITX_STREAM_EDMA_SHADOW_BASE + 6U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_5   (CSITX_STREAM_EDMA_SHADOW_BASE + 7U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_6   (CSITX_STREAM_EDMA_SHADOW_BASE + 8U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_7   (CSITX_STREAM_EDMA_SHADOW_BASE + 9U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_8   (CSITX_STREAM_EDMA_SHADOW_BASE + 10U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_9   (CSITX_STREAM_EDMA_SHADOW_BASE + 11U)
#define CSITX_STREAM_HW_SESSION_EDMA_SHADOW_CH_10  (CSITX_STREAM_EDMA_SHADOW_BASE + 12U)

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

 /* @brief
 *  CSI-TX streaming user data header
 *
 * @details
 *  The CSI-TX SW streaming user data header.
 */
typedef struct csiUserDataHeader
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
} csiUserDataHeader_t;


/**
 * @brief
 *  CSI-TX streaming MCB
 *
 * @details
 *  The CSI-TX streaming MCB.
 */
typedef struct csitxStream_MCB
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
    CBUFF_EDMAChannelCfg     hwSessionEDMAChannelTable[CSITX_STREAM_HW_SESSION_MAX_EDMA_CHANNEL];

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
    csiUserDataHeader_t    *userDataHeader;
} csitxStream_MCB_t;

/**
 * @brief
 *  ADCBUF configuration (meant for CLI configuration)
 *
 * @details
 *  The structure is used to hold all the relevant configuration
 *  which is used to configure ADCBUF.
 *
 */
typedef struct csitxStream_ADCBufCfg_t
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
}csitxStream_ADCBufCfg;

/**
 * @brief
 *  CSI-TX streaming configuration
 *
 * @details
 *  The structure is used to hold all the relevant configuration
 *  for the CSI-TX streaming.
 */
typedef struct csitxStream_Cfg_t
{
    /**
     * @brief  HSI Header enabled/disabled flag. Only applicable for HW streaming.
     *         Will be ignored for SW streaming which will always have HSI header.
     */
    bool        isHeaderEnabled;

    /*! HW STREAMING DISABLED */
#define CSITX_STREAM_CFG_DATAFMT_DISABLED   0

    /*! ADC */
#define CSITX_STREAM_CFG_DATAFMT_ADC        1

    /*! CP_ADC_CQ */
#define CSITX_STREAM_CFG_DATAFMT_CP_ADC_CQ  4

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
} csitxStream_Cfg;

/**
 * @brief
 *  Millimeter Wave Demo Data Path Information.
 *
 * @details
 *  The structure is used to hold all the relevant information for
 *  the data path.
 */
typedef struct csitxStream_SubFrameCfg_t
{
    /*! @brief ADC buffer configuration storage */
    csitxStream_ADCBufCfg adcBufCfg;

    /*! @brief  CSI-TX stream configuration */
    csitxStream_Cfg csitxStreamCfg;

    /*! @brief CQ signal & image band monitor buffer size */
    uint32_t    sigImgMonTotalSize;

    /*! @brief CQ RX Saturation monitor buffer size */
    uint32_t    satMonTotalSize;
    
    /*! @brief  Number of ADC samples */
    uint16_t    numAdcSamples;

    /*! @brief  Number of chirps per sub-frame */
    uint16_t    numChirpsPerSubFrame;
    
} csitxStream_SubFrameCfg;


/**
 * @brief
 *  CSI-TX Steam MCB
 *
 * @details
 *  The structure is used to hold all the relevant information for the
 *  CSI-TX Stream test.
 */
typedef struct csitxStream_MSS_MCB_t
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
    csitxStream_SubFrameCfg         subFrameCfg[RL_MAX_SUBFRAMES];
    
    /*! @brief   this structure is used to hold all the relevant information
        for the mmw demo csi-tx stream*/
    csitxStream_MCB_t             csitxStream;

    /*! @brief   Cbuff interface (LVDS or CSI-TX) */
    CBUFF_Interface              cbuffInterface;

} csitxStream_MCB;

int32_t csitxStream_Init (void);
int32_t csitxStream_HwConfig (uint8_t subFrameIndx);
void csitxStream_configCbuffHwData(uint8_t subFrameIndx);
void csitxStream_DeleteHwSession (void);


#ifdef __cplusplus
}
#endif

#endif
