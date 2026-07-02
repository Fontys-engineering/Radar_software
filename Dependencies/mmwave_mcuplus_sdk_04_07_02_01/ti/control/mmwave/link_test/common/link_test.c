/*
 *   @file  link_test.c
 *
 *   @brief
 *      The file contains common functions which test the mmWave Link API
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2016 Texas Instruments, Inc.
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


/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/HwiP.h>
#include <drivers/edma.h>
#include <drivers/adcbuf.h>
#include <drivers/hw_include/hw_types.h>
#include <drivers/crc.h>

#ifdef SOC_AWR2544
#include "FreeRTOS.h"
#include "task.h"
#include <drivers/mailbox.h>
#include <drivers/adcbuf.h>
#ifdef SUBSYS_MSS
#include "../awr2544/mssgenerated/ti_drivers_config.h"
#include "../awr2544/mssgenerated/ti_drivers_open_close.h"
#include "../awr2544/mssgenerated/ti_board_open_close.h"
#endif
#endif

#ifdef SOC_AWR294X
/* SOC_AWR294X */
#include "FreeRTOS.h"
#include "task.h"
#include <drivers/mailbox.h>
#include <drivers/adcbuf.h>
#ifdef SUBSYS_MSS
#include "../awr294x/mssgenerated/ti_drivers_config.h"
#include "../awr294x/mssgenerated/ti_drivers_open_close.h"
#include "../awr294x/mssgenerated/ti_board_open_close.h"
#elif SUBSYS_DSS
#include "../awr294x/dssgenerated/ti_drivers_config.h"
#include "../awr294x/dssgenerated/ti_drivers_open_close.h"
#include "../awr294x/dssgenerated/ti_board_open_close.h"
#endif
#endif

#ifdef SOC_AWR2X44P
/* SOC_AWR2X44P */
#include "FreeRTOS.h"
#include "task.h"
#include <drivers/mailbox.h>
#include <drivers/adcbuf.h>
#ifdef SUBSYS_MSS
#include "../awr2x44P/mssgenerated/ti_drivers_config.h"
#include "../awr2x44P/mssgenerated/ti_drivers_open_close.h"
#include "../awr2x44P/mssgenerated/ti_board_open_close.h"
#elif SUBSYS_DSS
#include "../awr2x44P/dssgenerated/ti_drivers_config.h"
#include "../awr2x44P/dssgenerated/ti_drivers_open_close.h"
#include "../awr2x44P/dssgenerated/ti_board_open_close.h"
#endif
#endif


#include <ti/control/mmwavelink/mmwavelink.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/control/mmwave/link_test/common/mmwavelink_test_common.h>
/* Test include files: */
#include <ti/control/mmwave/link_test/common/link_test.h>
#include <ti/control/mmwave/link_test/common/link_testcase_setting.h>
#include <ti/control/mmwave/link_test/common/link_testcase_config.h>
#include <ti/control/mmwave/link_test/common/link_test_monitoring.h>

#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
#define EDMA_CHANNEL_CQ_DATA  16 /*EDMA_TPCC0_REQ_FREE_0 */
#define CQ2_RAM_ADDRESS       (0xA5010400U)
#define L3_MEM_ADDRESS        (0x88000000U)
#define EDMA_NULL_LINK_ADDRESS 0xFFFFU
#define EDMA_NUM_DMA_CHANNELS (64U)
#endif

#define MMWAVELINK_CRC_TYPE     RL_CRC_TYPE_16BIT_CCITT

#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
#define MMW_LINK_MGMNT_TASK_STACK_SIZE  (5*1024U)
StackType_t linkMgmntStack[MMW_LINK_MGMNT_TASK_STACK_SIZE] __attribute__((aligned(32)));
#endif

/**************************************************************************
 *************************** Local Structures *****************************
 **************************************************************************/
/**
 * @brief
 *  mmWave debug stats
 *
 * @details
 *  The structure is used to store relevant debug stats.
 */
typedef struct MMWave_debugStats_t
{
    /**
     * @brief   Number of received IRQs
     */
    uint16_t   numIRQReceived;

    /**
     * @brief   Number of control channel writes
     */
    uint16_t   numControlChannelWrite;

    /**
     * @brief   Number of control channel reads
     */
    uint16_t   numControlChannelRead;

    /**
     * @brief   Number of Async events received
     */
    uint16_t   numAsyncEvents;

    /**
     * @brief   Number of Internal error Async events received
     */
    uint16_t   numErrAsyncEvents;

}MMWave_debugStats;


/**
 * @brief
 *  Mmwave Link Master Control Block
 *
 * @details
 *  The structure is used to hold all the relevant information for the
 *  Mmwave Link.
 */
typedef struct MmwaveLink_MCB
{

    /**
     * @brief   Semaphore handle for the mmWave Link
     */
    SemaphoreP_Object            linkSemaphore;

    /**
     * @brief   mmWave Link Spawning function
     */
    RL_P_OSI_SPAWN_ENTRY        spawnFxn;

    /**
     * @brief   Status of the BSS:
     */
    volatile uint32_t           bssStatus;

    /**
     * @brief   Counter which tracks of the number of times the spawn function was
     * overrun.
     */
    uint32_t                    spawnOverrun;
    /**
     * @brief   Handle to the CRC Channel
     */
    //CRC_Handle                  crcHandle;

    /**
    * @brief   Debug stats.
    */
    MMWave_debugStats           debugStats;

}MmwaveLink_MCB;

/**************************************************************************
 *************************** Extern Definitions ***************************
 **************************************************************************/

extern uint16_t monAeCnt [MMWAVELINK_TEST_MAX_NUM_MON];
extern uint16_t monFailRepCheck [MMWAVELINK_TEST_MAX_NUM_MON];
extern monitoringDataAe_t gMonDataAeStrct;
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
extern ADCBuf_Handle    adcbufHandle;
#endif
extern bool gLinkContModeTest;
extern uint16_t TxEnLutOffset;
extern rlUInt16_t lutOffsetInNBytes;
extern rlInt8_t AdvChirpLUTData[LUT_ADVCHIRP_TABLE_SIZE];
extern EDMA_Handle gEdmaHandle[CONFIG_EDMA_NUM_INSTANCES];
extern ADCBuf_Config gADCBufConfig[CONFIG_ADCBUF_NUM_INSTANCES];

/**************************************************************************
 *************************** Global Definitions ***************************
 **************************************************************************/

/* Global Variable for tracking information required by the mmWave Link */
MmwaveLink_MCB    gMmwaveLinkMCB;
uint32_t gMonitorHdrCnt = 0U;
uint32_t gInitTimeCalibStatus = 0U;
uint32_t gRunTimeCalibStatus = 0U;
volatile uint32_t gFrameStartStatus = 0U;
volatile uint32_t gMmwlChecksumFailAsyncEvent = 0U;
volatile uint32_t gMmwlCrcFailAsyncEvent = 0U;
rlUInt16_t gMonitoringStatus = 0U;
rlUInt8_t isGetGpAdcMeasData = 0U;
rlCpuFault_t cpufault;
RL_P_EVENT_HANDLER g_CommInterruptFunc = NULL;
volatile uint8_t eDMADataTransferDone = 0U;
uint32_t            baseAddr, regionId;
int32_t             Status = SystemP_SUCCESS;
uint32_t            dmaCh0, tcc0, param0;
volatile int8_t cpTestStatus = -10;
volatile int8_t cqTestStatus = -10;



/* received GPAdc Data over Async Event */
rlRecvdGpAdcData_t rcvGpAdcData = {0};


/* Negative test configuration structure */
typedef struct retryNegativeTest
{
    /* Type of corruption
       0 - No corruption
       1 - Checksum corruption
       2 - CRC corruption  */
    uint16_t corruptType;
    /* Event to corrupt
       0 - Command
       1 - Response
       2 - Async Event  */
    uint16_t corruptEvent;
}retryNegativeTest_t;


/* Async Event Enable and Direction configuration */
rlRfDevCfg_t rfDevCfg = {0};

/* Calibration Data storage */
rlCalibrationData_t calibData = { 0 };

/* Phase Shifter Calibration Data storage */
rlPhShiftCalibrationData_t phShiftCalibData = { 0 };
rlRxGainTempLutData_t rxGainTempLutData = { 0 };
rlAdvTxGainTempLutData_t txGainTempLutData0 = { 0 };
rlAdvTxGainTempLutData_t txGainTempLutData1 = { 0 };
rlAdvTxGainTempLutData_t txGainTempLutData2 = { 0 };
rlAdvTxGainTempLutData_t txGainTempLutData3 = { 0 };
retryNegativeTest_t   retryNegativeTestCfg = { 0 };
uint16_t hit_count = 0;
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
uint32_t mailbox_dummy_handle = 0;
#endif
/**************************************************************************
 *************************** Extern Definitions ***************************
 **************************************************************************/
extern rlInt32_t Osal_mutexCreate(rlOsiMutexHdl_t* mutexHdl, rlInt8_t* name);
extern rlInt32_t Osal_mutexLock(rlOsiMutexHdl_t* mutexHdl, rlOsiTime_t timeout);
extern rlInt32_t Osal_mutexUnlock(rlOsiMutexHdl_t* mutexHdl);
extern rlInt32_t Osal_mutexDelete(rlOsiMutexHdl_t* mutexHdl);
extern rlInt32_t Osal_semCreate(rlOsiSemHdl_t* semHdl, rlInt8_t* name);
extern rlInt32_t Osal_semWait(rlOsiSemHdl_t* semHdl, rlOsiTime_t timeout);
extern rlInt32_t Osal_semSignal(rlOsiSemHdl_t* semHdl);
extern rlInt32_t Osal_semDelete(rlOsiSemHdl_t* semHdl);

/**************************************************************************
 ************************* Link Unit Test Functions ***********************
 **************************************************************************/
 /**
 *  @b Description
 *  @n
 *  @param[in] time
 *          Sleep time in 1us units
 *
 *   Sleep function. 1LSB = 1uS.
 */
void MMWave_sleep(uint32_t time)
{
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
    ClockP_usleep(time);
#else
    TaskP_sleepInMsecs(time);
#endif
}

/**************************************************************************
 *    xWR294x device *
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      Mailbox registered function which is invoked on the reception of data
 *
 *  @retval
 *      Success - Communicate Interface Channel Handle
 *  @retval
 *      Error   - NULL
 */
 #if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
static void MmwaveLink_mboxCallbackFxn (uint32_t remoteCoreId, void *args)
#else
static void MmwaveLink_mboxCallbackFxn (Mbox_Handle handle, Mailbox_Instance remoteEndpoint)
#endif
{


    /* Indicate to the Radar Link that a message has been received. */
    g_CommInterruptFunc(0, NULL);

}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to open the communication
 *      interface channel
 *
 *  @retval
 *      Success - Communicate Interface Channel Handle
 *  @retval
 *      Error   - NULL
 */
static rlComIfHdl_t MmwaveLink_mboxOpen(rlUInt8_t deviceIndex, uint32_t flags)
{
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
    Mailbox_setReadCallback(MmwaveLink_mboxCallbackFxn, NULL);

    return (rlComIfHdl_t)&mailbox_dummy_handle;
#else
    Mailbox_initParams initParam;
    Mailbox_openParams openParam;
    int32_t errCode;

    /* Mailbox driver init parameters */
    Mailbox_initParams_init(&initParam);
#ifdef SUBSYS_MSS
    initParam.localEndpoint = MAILBOX_INST_MSS_CR5A;
#elif SUBSYS_DSS
    initParam.localEndpoint = MAILBOX_INST_DSP;
#endif
    /* Initialize the Mailbox driver */
    Mailbox_init(&initParam);

    /* Mailbox driver open parameters for BSS */
    Mailbox_openParams_init(&openParam);
    openParam.remoteEndpoint = MAILBOX_INST_RSS;
    openParam.cfg.readMode = MAILBOX_MODE_CALLBACK;
    openParam.cfg.readCallback = MmwaveLink_mboxCallbackFxn;
	openParam.cfg.writeMode = MAILBOX_MODE_BLOCKING;
	openParam.cfg.readTimeout = MAILBOX_WAIT_FOREVER;

    /* Open the mailbox driver instance for BSS */
    gMmwaveLinkMCB.commHandle = Mailbox_open(&openParam, &errCode);
    if ((gMmwaveLinkMCB.commHandle == NULL) || (errCode != 0))
    {
        printf("Error: Unable to open the Mailbox Instance [Error code %d]\n", errCode);
        return NULL;
    }

    return (rlComIfHdl_t)gMmwaveLinkMCB.commHandle;
#endif
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to close the communication
 *      interface channel
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static int32_t MmwaveLink_mboxClose(rlComIfHdl_t fd)
{
#if defined(SOC_AWR294X) || defined(SOC_AWR2544)|| defined(SOC_AWR2X44P)
    return 0;
#else
    int32_t errCode;

    /* Close the Mailbox */
    errCode = Mailbox_close ((Mbox_Handle)fd);
    if (errCode < 0)
        printf ("Error: Unable to close the BSS Mailbox [Error code %d]\n", errCode);

    return errCode;
#endif
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to read data from the communication
 *      interface channel
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static int32_t MmwaveLink_mboxRead(rlComIfHdl_t fd, uint8_t* pBuff, uint16_t len)
{

    int32_t    status;

#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
    status = Mailbox_read(CSL_CORE_ID_RSS_R4, pBuff, len, SystemP_WAIT_FOREVER);

    if (status != SystemP_SUCCESS)
    {
        DebugP_logInfo("Mailbox Read Error. \n");
    }
    else
    {
        status = len;
    }
#else
    status = Mailbox_read((Mbox_Handle)fd, pBuff, len);
#endif
    if (retryNegativeTestCfg.corruptType == 1)
    {
        if (retryNegativeTestCfg.corruptEvent == 1)
        {
            hit_count++;
            if (hit_count == 2)
            {
                pBuff[10] = ~(pBuff[14]);
                pBuff[11] = ~(pBuff[15]);
                retryNegativeTestCfg.corruptType = 0;
                hit_count = 0;
            }
        }
    }
    if (retryNegativeTestCfg.corruptType == 1)
    {
        if (retryNegativeTestCfg.corruptEvent == 2)
        {
            hit_count++;
            if (hit_count == 5)
            {
                pBuff[10] = ~(pBuff[14]);
                pBuff[11] = ~(pBuff[15]);
                retryNegativeTestCfg.corruptType = 0;
                hit_count = 0;
            }
        }
    }

    return status;


}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to write data to the communication
 *      interface channel
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static int32_t MmwaveLink_mboxWrite(rlComIfHdl_t fd, uint8_t* pBuff, uint16_t len)
{


    int32_t    status;

    if (retryNegativeTestCfg.corruptType == 1)
    {
        if (retryNegativeTestCfg.corruptEvent == 0)
        {
            hit_count++;
            if (hit_count == 1)
            {
                pBuff[14] = ~(pBuff[14]);
                pBuff[15] = ~(pBuff[15]);
                retryNegativeTestCfg.corruptType = 0;
                hit_count = 0;
            }
        }
    }
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
    status = Mailbox_write(CSL_CORE_ID_RSS_R4, pBuff, len, SystemP_WAIT_FOREVER);

    if (status != SystemP_SUCCESS)
    {
        DebugP_logInfo("Mailbox Write Error. \n");
    }
    else
    {
        status = len;
    }
#else
    /*
      Currently, the mmwavelink can not detect the error condition where it did not receive a mailbox layer ACK from BSS.

      For instance:
      - The mmwavelink may try to send a message before an ACK was received for the previous message.
      - The mmwavelink may try to resend a message that did not receive a mmwavelink layer ACK back from BSS. It is possible that the
      message did not receive a mailbox layer ACK as well from BSS.

      In either case, Mailbox_writeReset() has to be called before another message is sent to BSS.

      The mmwavelink has no hooks to call the Mailbox_writeReset().
      Therefore, a write reset is done if it is detected that a mailbox layer ACK was not received for the
      previous message (MAILBOX_ETXFULL).
     */
    do
    {
        status = Mailbox_write((Mbox_Handle)fd, pBuff, len);
        if(status == MAILBOX_ETXFULL)
        {
            DebugP_log0("MSS: Error. Write timed out. Rewriting msg \n");
        }
    } while (status == MAILBOX_ETXACKTIMEDOUT);
#endif
    return status;


}


/**
 *  @b Description
 *  @n
 *      ADC Buffer configuration to receive chirp interrupt from mmWave
 *      front end
 *
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 *
 */
int32_t MmwaveLink_adcBufConfig()
{
    ADCBuf_dataFormat       dataFormat;
    ADCBuf_RxChanConf       rxChanConf;
    int32_t retVal;
    uint8_t      numBytePerSample, numRxChannel = 0, channel;
    uint32_t bufSizePerChan;
    extern const rlChanCfg_t chCfg;
    extern const rlProfileCfg_t  profileCfg[4];
    extern const rlAdcOutCfg_t adcOutCfgArgs;

    /* Configure ADC buffer data format */
    if (adcOutCfgArgs.fmt.b2AdcOutFmt == ADC_FORMAT_REAL)
    {
        dataFormat.adcOutFormat       = 1U; /* 0 - Complex Data mode, 1 - Real data mode */
    }
    else
    {
        dataFormat.adcOutFormat       = 0U; /* 0 - Complex Data mode, 1 - Real data mode */
    }
    dataFormat.sampleInterleave   = 0U;
    dataFormat.channelInterleave  = 1U;//0;

    retVal = ADCBuf_control(adcbufHandle, ADCBufMMWave_CMD_CONF_DATA_FORMAT, (void *)&dataFormat);
    if (retVal < 0)
    {
        printf("Error: ADCBufMMWave_CMD_CONF_DATA_FORMAT failed with [Error %d]\n", retVal);
    }

    /* Calculate the DMA transfer parameters */
    if (dataFormat.adcOutFormat == 0)
    {
        /* Complex dataFormat has 4 bytes */
        numBytePerSample =  4 ;
    }
    else
    {
        /* Real dataFormat has 2 bytes */
        numBytePerSample =  2 ;
    }

    /* Enable the receive channels */
    for (channel = 0; channel < SYS_COMMON_NUM_RX_CHANNEL; channel++)
    {
        /* Is the channel enabled? */
        if (chCfg.rxChannelEn & (0x1 << channel))
        {
            /* YES: Populate the receive channel configuration: */
            rxChanConf.channel = channel;

            /* Are we operating in continuous mode? */
            if (gLinkContModeTest == false)
            {
                /* Calculate ADCBuf size per channel */
                bufSizePerChan = profileCfg[0].numAdcSamples * numBytePerSample;

                /* Chirp Mode: */
                rxChanConf.offset = bufSizePerChan * numRxChannel;

                /* Offset is used in non-interleaved mode and it must be 16 bytes aligned. */
                if(rxChanConf.offset % 16)
                {
                   // DebugP_assert(0);
                }
            }
            else
            {
                /* Continuous Mode: */
                rxChanConf.offset = (PROFILE_ADC_SAMPLE_VAL*4) * numBytePerSample * numRxChannel;
            }

            /* Enable the channel: ADCBuf channel offset address is used for non-interleaved mode, the address must be 16 bytes aligned.*/
            retVal = ADCBuf_control(adcbufHandle, ADCBufMMWave_CMD_CHANNEL_ENABLE, (void *)&rxChanConf);
            if(retVal < 0)
            {
                printf("Error: ADCBufMMWave_CMD_CHANNEL_ENABLE failed for channel %d , offset = %x, with [Error %d]\n",
                               channel, rxChanConf.offset, retVal);
            }

            /* Track the number of receive channels: */
            numRxChannel++;
        }
    }
    printf("Debug: ADCbuf configuration is done!!\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to power on the AR1XX Device
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MmwaveLink_enableDevice(rlUInt8_t deviceIndex)
{
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to power off the AR1XX Device
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MmwaveLink_disableDevice(rlUInt8_t deviceIndex)
{
    printf("Debug: Disabling the device\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to mask the interrupts.
 *      In the case of Mailbox communication interface the driver will
 *      handle the interrupt management. This function is a dummy stub
 *
 *  @retval
 *      Not applicable
 */
static void MmwaveLink_maskHostIRQ(rlComIfHdl_t fd)
{
    return;
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to umask the interrupts.
 *      In the case of the mailbox driver we will flush out and close the
 *      read buffer.
 *
 *  @retval
 *      Not applicable
 */
void MmwaveLink_unmaskHostIRQ(rlComIfHdl_t fd)
{
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
    /* The Mailbox driver indicates to the remote endpoint that the message
     * have been processed. The mmWave is now capable of receiving another message */
    Mailbox_readDone(CSL_CORE_ID_RSS_R4);
#else
    Mailbox_readFlush((Mbox_Handle)fd);
#endif
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to register the Interrupt Handler.
 *      In the case of the Mailbox the driver does the interrupt registeration and
 *      so this function is a dummy stub.
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MmwaveLink_registerInterruptHandler(rlUInt8_t deviceIndex, RL_P_EVENT_HANDLER pHandler, void* pValue)
{
    g_CommInterruptFunc = pHandler;
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Used to configure the ADCBUF registers to receive the CP/CQ data
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
int32_t MMwaveLink_AdcBufConfigCQ()
{
    int status=0;
    ADCBuf_close(adcbufHandle);
    PRINT_LOG("Closing the adcbufHandle %d for CQ ADC Open \n",adcbufHandle);
    ADCBuf_Handle adchandle;
    adchandle=ADCBuf_open(0,NULL);
    if(adchandle == NULL)
    {
        PRINT_LOG("Error: CQ ADC Handle NULL\n");
    }

    ADCBuf_CQConf cqconfig;
    ADCBuf_dataFormat dataformat;
    ADCBuf_RxChanConf chanconfig={0};
    chanconfig.channel=0;
    dataformat.adcOutFormat=1;
    dataformat.sampleInterleave=0;
    dataformat.channelInterleave=0;//1
    cqconfig.cqDataWidth=1;
    cqconfig.cq96BitPackEn=0;
    cqconfig.cq0AddrOffset=0x0U;
    cqconfig.cq1AddrOffset=0x200U;
    cqconfig.cq2AddrOffset=0x400U;
    status=ADCBuf_control(adchandle,ADCBufMMWave_CMD_CONF_CQ,(void*)&cqconfig);
    status=ADCBuf_control(adchandle,ADCBufMMWave_CMD_CONF_DATA_FORMAT,(void*)&dataformat);
    status=ADCBuf_control(adchandle,ADCBufMMWave_CMD_CHANNEL_ENABLE,(void*)&chanconfig);
    chanconfig.channel=1;
    status=ADCBuf_control(adchandle,ADCBufMMWave_CMD_CHANNEL_ENABLE,(void*)&chanconfig);
    chanconfig.channel=2;
    status=ADCBuf_control(adchandle,ADCBufMMWave_CMD_CHANNEL_ENABLE,(void*)&chanconfig);
    chanconfig.channel=3;
    status=ADCBuf_control(adchandle,ADCBufMMWave_CMD_CHANNEL_ENABLE,(void*)&chanconfig);
    ADCBuf_close(adchandle);

    ADCBuf_Params     ADCBufparams;
    /* ADCBUF Params initialize */
    ADCBuf_Params_init(&ADCBufparams);
    ADCBufparams.chirpThresholdPing = 1;
    ADCBufparams.chirpThresholdPong = 1;
    ADCBufparams.continousMode  = 0;
    adcbufHandle = ADCBuf_open(0, &ADCBufparams);
    if (adcbufHandle == NULL)
    {
        PRINT_LOG("Error: Unable to re-open the ADCBUF Instance after CQ ADC open/close\n");
        return -1;
    }
    PRINT_LOG("Debug: ADCBUF Instance %p has been reopened successfully after CQ ADC open/close\n", adcbufHandle);
    return status;
}

/**
 *  @b Description
 *  @n
 *      Trigger eDMA transfer
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
void MmwaveLink_triggerEdmaTransfer()
{

    EDMA_enableTransferRegion(baseAddr, regionId, dmaCh0,
             EDMA_TRIG_MODE_MANUAL);
     while(EDMA_readIntrStatusRegion(baseAddr, regionId, tcc0) != 1)
                    ;

        EDMA_clrIntrRegion(baseAddr, regionId, tcc0);
        eDMADataTransferDone++;
}

/**
 *  @b Description
 *  @n
 *     Intialize and configure eDMA to copy CQ Data to L3 memory
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
int32_t MmwaveLink_CQedmaSetup(void)
{

    baseAddr = EDMA_getBaseAddr(gEdmaHandle[0]);
    DebugP_assert(baseAddr != 0);

    regionId = EDMA_getRegionId(gEdmaHandle[0]);
    DebugP_assert(regionId < SOC_EDMA_NUM_REGIONS);

    dmaCh0 = EDMA_RESOURCE_ALLOC_ANY;
    Status = EDMA_allocDmaChannel(gEdmaHandle[0], &dmaCh0);
    DebugP_assert(Status == SystemP_SUCCESS);

    tcc0 = EDMA_RESOURCE_ALLOC_ANY;
    Status = EDMA_allocTcc(gEdmaHandle[0], &tcc0);
    DebugP_assert(Status == SystemP_SUCCESS);

    param0 = EDMA_RESOURCE_ALLOC_ANY;
    Status = EDMA_allocParam(gEdmaHandle[0], &param0);
    DebugP_assert(Status == SystemP_SUCCESS);

    /* Request channel */
    EDMA_configureChannelRegion(baseAddr, regionId, EDMA_CHANNEL_TYPE_DMA,
         dmaCh0, tcc0, param0, 0);

    /* Program Param Set */
    EDMACCPaRAMEntry   edmaParam;
    EDMA_ccPaRAMEntry_init(&edmaParam);
    edmaParam.srcAddr       = (uint32_t)CQ2_RAM_ADDRESS;
    edmaParam.destAddr      = (uint32_t)L3_MEM_ADDRESS;
    edmaParam.aCnt          = (uint16_t) 64U; //CQ2 data is 128 max
    edmaParam.bCnt          = (uint16_t) 1U;
    edmaParam.cCnt          = (uint16_t) 501U;
    edmaParam.bCntReload    = (uint16_t) 1U;
    edmaParam.srcBIdx       = (int16_t) 0U;
    edmaParam.destBIdx      = (int16_t) 0U;
    edmaParam.srcCIdx       = (int16_t) 0U;
    edmaParam.destCIdx      = (int16_t) 64U;
    edmaParam.linkAddr      = 0xFFFFU;
    edmaParam.opt          |=
        (EDMA_OPT_TCINTEN_MASK | EDMA_OPT_ITCINTEN_MASK |
         ((((uint32_t)tcc0) << EDMA_OPT_TCC_SHIFT) & EDMA_OPT_TCC_MASK));

    EDMA_setPaRAM(baseAddr, param0, &edmaParam);
    return 0;
}


/**
 *  @b Description
 *  @n
 *      This function is to verify CQ2 RAM data for RX signal and image band monitor
 *      and saturation monitor for each slices which device has been configured.
 *
 *  @param : coreType- MSS or DSS core
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int8_t MmwaveLink_VerifyCQData(uint8_t coreType)
{
    unsigned char numSlices, i;
    volatile unsigned char *cq2MemLoc;
    if (coreType == RL_PLATFORM_MSS)
    {
        cq2MemLoc=(unsigned char*)(L3_MEM_ADDRESS);
    }
    else
    {
        cq2MemLoc=(unsigned char*)(L3_MEM_ADDRESS);
    }

    /* CQ data will be available in CQ RAM which is a ping-pong memory when the CQ monitors are
       enabled. Currently supported CQ monitors are RL_RF_RX_SATURATION_MON_CONF_SB for CQ2.
       CQ data will be refreshed every chirp by the hardware.
       User has to ensure that before the next chirp finishes, the current chirps’ CQ data is either
       processed or transferred to a local memory for further processing.
     */

    /* check for timeSlice value for CQ2 as configured to device via
       rlRfRxIfSatMonConfig API */
    if (CQ_NUM_SLICES != cq2MemLoc[0])
    {
        cqTestStatus = -1;
    }
    else
    {
        numSlices = cq2MemLoc[0];
        for (i = 1; i < numSlices; i++)
        {
            /* check if any signal band data is less than image band data for all
               primary and secondary slices, then set error  */
            if (cq2MemLoc[i] != 0)
            {
                cqTestStatus = -2;
                break;
            }
        }

        /* if conditions are not matched for all slices then set success */
        if (i == numSlices)
        {
            cqTestStatus = 0;
        }

    }
    return cqTestStatus;
}


/**
 *  @b Description
 *  @n
 *      Verify Chirp parameters
 *      CP is Chirp Parameter information which is defined for each RX as follows
        Bit     Description
        11:0    Chirp number
                In legacy frame configuration, chirp number starts from 1 and increments
                for each chirp within the frame and resets to 0 for the next frame.
                In advanced frame configuration chirp number starts from 1 and increments for
                each chirp within the burst and resets to 0 for the next burst.
        15:12    Reserved
        17:16    Channel number. The receive channel number which is encoded as
                00 – RX0
                01 – RX1
                10 – RX2
                11 – RX3
        21:18    Profile number. The profile number to which the chirp belongs
        31:22    Reserved
 *
 *  @param : coreType- MSS or DSS core
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int8_t MmwaveLink_VerifyCPData(uint8_t coreType)
{
    uint8_t cp0DataBuff[16] = {0};
    volatile uint16_t chirpNum, channelNum, profileId;

    if (coreType == RL_PLATFORM_MSS)
    {
        /* copy CP0 data to local buffer */
        memcpy((void*)&cp0DataBuff[0], (void*)0x05020C08U, 16U);
    }
    else
    {
        /* copy CP0 data to local buffer */
       memcpy((void*)&cp0DataBuff[0], (void*)0x05020C08U, 16U);
    }

    /* Please refer mmWaveLink or ICD for Chirp parameter (CP) information fields */
    chirpNum =  (((cp0DataBuff[3] & 0x0F)<< 8) | (cp0DataBuff[2]));
    channelNum = (cp0DataBuff[0] & 0x03);
    profileId =  ((cp0DataBuff[0] >> 2) & 0x0F);
    /* verify CP-0 data for channel 0 */
    if ((channelNum != 0U) || (profileId != chirpCfg[0].profileId) || \
         ((chirpNum + 1) != ((frameCfg.chirpEndIdx - frameCfg.chirpStartIdx + 1) * frameCfg.numLoops)))
    {
        cpTestStatus = -1;
    }
    else
    {
        cpTestStatus = 0;
    }

    chirpNum =  (((cp0DataBuff[7] & 0x0F)<< 8) | (cp0DataBuff[6]));
    channelNum = (cp0DataBuff[4] & 0x03);
    profileId =  ((cp0DataBuff[4] >> 2) & 0x0F);
    /* verify CP-0 data for channel 1 */
    if ((channelNum != 1U) || (profileId != chirpCfg[0].profileId) || \
        ((chirpNum + 1) != ((frameCfg.chirpEndIdx - frameCfg.chirpStartIdx + 1) * frameCfg.numLoops)))
    {
        cpTestStatus -=1;
    }
    else
    {
        cpTestStatus = 0;
    }

    chirpNum =  (((cp0DataBuff[11] & 0x0F)<< 8) | (cp0DataBuff[10]));
    channelNum = (cp0DataBuff[8] & 0x03);
    profileId =  ((cp0DataBuff[8] >> 2) & 0x0F);
    /* verify CP-0 data for channel 2 */
    if ((channelNum != 2U) || (profileId != chirpCfg[0].profileId) || \
        ((chirpNum + 1) != ((frameCfg.chirpEndIdx - frameCfg.chirpStartIdx + 1) * frameCfg.numLoops)))
    {
        cpTestStatus -=1;
    }
    else
    {
        cpTestStatus = 0;
    }

    chirpNum =  (((cp0DataBuff[15] & 0x0F)<< 8) | (cp0DataBuff[14]));
    channelNum = (cp0DataBuff[12] & 0x03);
    profileId =  ((cp0DataBuff[12] >> 2) & 0x0F);
    /* verify CP-0 data for channel 3 */
    if ((channelNum != 3U) || (profileId != chirpCfg[0].profileId) || \
        ((chirpNum + 1) != ((frameCfg.chirpEndIdx - frameCfg.chirpStartIdx + 1) * frameCfg.numLoops)))
    {
        cpTestStatus -=1;
    }
    else
    {
        cpTestStatus = 0;
    }

    return cpTestStatus;
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to handle asynchronous events
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
 uint16_t errorStatusCnt[5] = {0};

static void MmwaveLink_asyncEventHandler(uint8_t devIndex, uint16_t sbId, uint16_t sbLen, uint8_t *payload)
{
    uint16_t asyncSB = RL_GET_SBID_FROM_UNIQ_SBID(sbId);
    uint16_t msgId   = RL_GET_MSGID_FROM_SBID(sbId);

    /* Process the received message: */
    switch (msgId)
    {

        case RL_RF_ASYNC_EVENT_MSG:
        {
            /* Received Asychronous Message: */
            switch (asyncSB)
            {
                case RL_RF_AE_CPUFAULT_SB:
                {
                    rlCpuFault_t* rfCpuFault = (rlCpuFault_t*)payload;
                    printf ("Debug: CPU Fault has been detected\n");
                    printf ("Debug: Fault \n type: %d, lineNum: %d, LR: 0x%x \n"
                                    "PrevLR: 0x%x, spsr: 0x%x, sp: 0x%x, PC: 0x%x \n"
                                    "Status: 0x%x, Source: %d, AxiErrType: %d, AccType: %d, Recovery Type: %d \n",
                                    rfCpuFault->faultType,
                                    rfCpuFault->lineNum,
                                    rfCpuFault->faultLR,
                                    rfCpuFault->faultPrevLR,
                                    rfCpuFault->faultSpsr,
                                    rfCpuFault->faultSp,
                                    rfCpuFault->faultAddr,
                                    rfCpuFault->faultErrStatus,
                                    rfCpuFault->faultErrSrc,
                                    rfCpuFault->faultAxiErrType,
                                    rfCpuFault->faultAccType,
                                    rfCpuFault->faultRecovType);
                    break;
                }
                case RL_RF_AE_ESMFAULT_SB:
                {
                    printf ("Debug: ESM Fault. Group1:[0x%x] Group2:[0x%x]\n",
                    ((rlBssEsmFault_t*)payload)->esmGrp1Err, ((rlBssEsmFault_t*)payload)->esmGrp2Err);
                    break;
                }
                case RL_RF_AE_INITCALIBSTATUS_SB:
                {
                    gInitTimeCalibStatus = ((rlRfInitComplete_t*)payload)->calibStatus;
                    if(gInitTimeCalibStatus != 0U)
                    {
                        printf ("Debug: Init time calibration status [0x%x] \n", gInitTimeCalibStatus);
                    }
                    else
                    {
                        printf ("Error: All Init time calibrations Failed:\n");
                    }
                    break;
                }
                case RL_RF_AE_FRAME_TRIGGER_RDY_SB:
                {
                    gFrameStartStatus = 1U;
                    break;
                }
                case RL_RF_AE_MON_TIMING_FAIL_REPORT_SB:
                {
                    printf ("Debug: Monitoring FAIL Report received \n");
                    break;
                }
                case RL_RF_AE_RUN_TIME_CALIB_REPORT_SB:
                {
                    monAeCnt[28]++;
                    gRunTimeCalibStatus = ((rlRfRunTimeCalibReport_t*)payload)->calibErrorFlag;
                    if(gRunTimeCalibStatus == 0U)
                    {
                        monFailRepCheck[28]++;
                        printf ("Error: All Run time calibrations Failed:\n");
                    }
                    break;
                }
                case RL_RF_AE_MON_DIG_PERIODIC_REPORT_SB:
                {
                    monAeCnt[29]++;
                    extern volatile uint32_t gLinkFrameCnt;
                    errorStatusCnt[2] = gLinkFrameCnt;
                    gMonitoringStatus = ((rlDigPeriodicReportData_t*)payload)->digMonPeriodicStatus;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[29]++;
                    }
                    break;
                }
                case RL_RF_AE_MON_TEMPERATURE_REPORT_SB:
                {
                    extern volatile uint32_t gLinkFrameCnt;
                    errorStatusCnt[1] = gLinkFrameCnt;
                    memcpy(&gMonDataAeStrct.monTempReport, payload, sizeof(rlMonTempReportData_t));

                    gMonitoringStatus = ((rlMonTempReportData_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[MMWAVELINK_TEST_MON_TEMP]++;
                    }
                    monAeCnt[MMWAVELINK_TEST_MON_TEMP]++;
                    break;
                }
                case RL_RF_AE_MON_RX_GAIN_PHASE_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monRxGainPhRep, payload, sizeof(rlMonRxGainPhRep_t));
                    monAeCnt[MMWAVELINK_TEST_MON_RX_GAIN_PH]++;
                    break;
                }
                case RL_RF_AE_MON_RX_NOISE_FIG_REPORT:
                {
                    ;
                }
                case RL_RF_AE_MON_RX_IF_STAGE_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monRxIfStageRep, payload, sizeof(rlMonRxIfStageRep_t));
                    gMonitoringStatus = ((rlMonRxIfStageRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[3]++;
                    }
                    monAeCnt[3]++;
                    break;
                }
                case RL_RF_AE_MON_TX0_POWER_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monTx0powRep, payload, sizeof(rlMonTxPowRep_t));
                    gMonitoringStatus = ((rlMonTxPowRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[4]++;
                    }
                    monAeCnt[4]++;
                    break;
                }
                case RL_RF_AE_MON_TX1_POWER_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monTx1powRep, payload, sizeof(rlMonTxPowRep_t));
                    gMonitoringStatus = ((rlMonTxPowRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[5]++;
                    }
                    monAeCnt[5]++;
                    break;
                }
                case RL_RF_AE_MON_TX2_POWER_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monTx2powRep, payload, sizeof(rlMonTxPowRep_t));
                    gMonitoringStatus = ((rlMonTxPowRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[6]++;
                    }
                    monAeCnt[6]++;
                    break;
                }

                case RL_RF_AE_MON_TX0_BALLBREAK_REPORT:
                {
                    /* stop updating the global cached value after first error so that we can dump the first failed case valus */
                    if (monFailRepCheck[7]==0) {
                        memcpy(&gMonDataAeStrct.monTx0BallbreakRep, payload, sizeof(rlMonTxBallBreakRep_t));
                    }
                    gMonitoringStatus = ((rlMonTxBallBreakRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[7]++;
                    }
                    monAeCnt[7]++;
                    break;
                }
                case RL_RF_AE_MON_TX1_BALLBREAK_REPORT:
                {
                    /* stop updating the global cached value after first error so that we can dump the first failed case valus */
                    if (monFailRepCheck[8]==0) {
                        memcpy(&gMonDataAeStrct.monTx1BallbreakRep, payload, sizeof(rlMonTxBallBreakRep_t));
                    }
                    gMonitoringStatus = ((rlMonTxBallBreakRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[8]++;
                    }
                    monAeCnt[8]++;
                    break;
                }
                case RL_RF_AE_MON_REPORT_HEADER_SB:
                {
                    gMonitorHdrCnt++;
                    monAeCnt[27]++;
                    break;
                }
                case RL_RF_AE_GPADC_MEAS_DATA_SB:
                {
                    isGetGpAdcMeasData = 1U;
                    memcpy(&rcvGpAdcData, payload, sizeof(rlRecvdGpAdcData_t));
                    break;
                }
                case RL_RF_AE_DIG_LATENTFAULT_REPORT_SB:
                {
                    printf ("Debug:  Digital Monitoring Latent Fault Report Received \n");
                    rlDigLatentFaultReportData_t *data = (rlDigLatentFaultReportData_t*)payload;
                    printf("Dig Latent Fault report [0x%x] Async event\n\n", data->digMonLatentFault);
                    break;
                }
                case RL_RF_AE_FRAME_END_SB:
                {
                    gFrameStartStatus = 0U;
                    printf ("Debug:  Frame Stop Async Event \n");
                    break;
                }
                case RL_RF_AE_ANALOG_FAULT_SB:
                {
                    printf ("Debug:  Analog Fault Async Event \n");
                    break;
                }
                default:
                {
                    printf ("Error: Asynchronous Event SB Id %d not handled with msg ID [0x%x] \n", asyncSB,msgId);
                    break;
                }
            }
            break;
        }
        case RL_RF_ASYNC_EVENT_1_MSG:
        {
            switch (asyncSB)
            {
                case RL_RF_AE_MON_TX2_BALLBREAK_REPORT:
                {
                    /* stop updating the global cached value after first error so that we can dump the first failed case valus */
                    if (monFailRepCheck[9]==0) {
                        memcpy(&gMonDataAeStrct.monTx2BallbreakRep, payload, sizeof(rlMonTxBallBreakRep_t));
                    }
                    gMonitoringStatus = ((rlMonTxBallBreakRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[9]++;
                    }
                    monAeCnt[9]++;
                    break;
                }
                case RL_RF_AE_MON_TX3_BALLBREAK_REPORT:
                {
                    /* stop updating the global cached value after first error so that we can dump the first failed case valus */
                    if (monFailRepCheck[30]==0) {
                        memcpy(&gMonDataAeStrct.monTx3BallbreakRep, payload, sizeof(rlMonTxBallBreakRep_t));
                    }
                    gMonitoringStatus = ((rlMonTxBallBreakRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[30]++;
                    }
                    monAeCnt[30]++;
                    break;
                }
                  case RL_RF_AE_MON_TX3_POWER_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monTx3powRep, payload, sizeof(rlMonTxPowRep_t));
                    gMonitoringStatus = ((rlMonTxPowRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[29]++;
                    }
                    monAeCnt[29]++;
                    break;
                }
                case RL_RF_AE_MON_TX_GAIN_MISMATCH_REPORT:
                {
                    ;
                }

                case RL_RF_AE_MON_ADV_TX_GAIN_PHASE_MIS_REPORT:
                {
                     memcpy(&gMonDataAeStrct.monAdvTxGainPhMisMatchRep, payload, sizeof(rlMonAdvTxGainPhaMisRep_t));
                    gMonitoringStatus = ((rlMonAdvTxGainPhaMisRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[10]++;
                    }
                    monAeCnt[10]++;
                    break;
                }

                case RL_RF_AE_MON_SYNTHESIZER_FREQ_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monSynthFreqRep, payload, sizeof(rlMonSynthFreqRep_t));
                    gMonitoringStatus = ((rlMonSynthFreqRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[14]++;
                    }
                    monAeCnt[14]++;
                    break;
                }
                case RL_RF_AE_MON_EXT_ANALOG_SIG_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monExtAnaSigRep, payload, sizeof(rlMonExtAnaSigRep_t));
                    gMonitoringStatus = ((rlMonExtAnaSigRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[15]++;
                    }
                    monAeCnt[15]++;
                    break;
                }
                case RL_RF_AE_MON_TX0_INT_ANA_SIG_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monTx0IntAnaSigRep, payload, sizeof(rlMonTxIntAnaSigRep_t));
                    gMonitoringStatus = ((rlMonTxIntAnaSigRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[16]++;
                    }
                    monAeCnt[16]++;
                    break;
                }
                case RL_RF_AE_MON_TX1_INT_ANA_SIG_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monTx1IntAnaSigRep, payload, sizeof(rlMonTxIntAnaSigRep_t));
                    gMonitoringStatus = ((rlMonTxIntAnaSigRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[17]++;
                    }
                    monAeCnt[17]++;
                    break;
                }
                case RL_RF_AE_MON_TX2_INT_ANA_SIG_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monTx2IntAnaSigRep, payload, sizeof(rlMonTxIntAnaSigRep_t));
                    gMonitoringStatus = ((rlMonTxIntAnaSigRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[18]++;
                    }
                    monAeCnt[18]++;
                    break;
                }
                  case RL_RF_AE_MON_TX3_INT_ANA_SIG_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monTx3IntAnaSigRep, payload, sizeof(rlMonTxIntAnaSigRep_t));
                    gMonitoringStatus = ((rlMonTxIntAnaSigRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[31]++;
                    }
                    monAeCnt[31]++;
                    break;
                }
                case RL_RF_AE_MON_RX_INT_ANALOG_SIG_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monRxIntAnaSigRep, payload, sizeof(rlMonRxIntAnaSigRep_t));
                    gMonitoringStatus = ((rlMonRxIntAnaSigRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[19]++;
                    }
                    monAeCnt[19]++;
                    break;
                }
                case RL_RF_AE_MON_PMCLKLO_INT_ANA_SIG_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monPmClkIntAnaSigRep, payload, sizeof(rlMonPmclkloIntAnaSigRep_t));
                    gMonitoringStatus = ((rlMonPmclkloIntAnaSigRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[20]++;
                    }
                    monAeCnt[20]++;
                    break;
                }
                case RL_RF_AE_MON_GPADC_INT_ANA_SIG_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monGpadcIntAnaSigRep, payload, sizeof(rlMonGpadcIntAnaSigRep_t));
                    gMonitoringStatus = ((rlMonGpadcIntAnaSigRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[21]++;
                    }
                    monAeCnt[21]++;
                    break;
                }
                case RL_RF_AE_MON_PLL_CONTROL_VOLT_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monPllConvVoltRep, payload, sizeof(rlMonPllConVoltRep_t));
                    gMonitoringStatus = ((rlMonPllConVoltRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[22]++;
                    }
                    monAeCnt[22]++;
                    break;
                }
                case RL_RF_AE_MON_DCC_CLK_FREQ_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monDccClkFreqRep, payload, sizeof(rlMonDccClkFreqRep_t));
                    gMonitoringStatus = ((rlMonDccClkFreqRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[23]++;
                    }
                    monAeCnt[23]++;
                    break;
                }
                case RL_RF_AE_MON_RX_MIXER_IN_PWR_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monRxMixrInPwrRep, payload, sizeof(rlMonRxMixrInPwrRep_t));
                    gMonitoringStatus = ((rlMonRxMixrInPwrRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[26]++;
                    }
                    monAeCnt[26]++;
                    break;
                }
                case RL_RF_AE_MON_SYNTH_FREQ_NONLIVE_REPORT:
                {
                    memcpy(&gMonDataAeStrct.monSynthFreqNonLiveRep, payload, sizeof(rlMonSynthFreqNonLiveRep_t));
                    gMonitoringStatus = ((rlMonSynthFreqNonLiveRep_t*)payload)->statusFlags;
                    if(gMonitoringStatus == 0U)
                    {
                        monFailRepCheck[28]++;
                    }
                    monAeCnt[28]++;
                    break;
                }
                default:
                {
                    printf ("Error: Asynchronous Event SB Id %d not handled with msg ID 0x%x\n", asyncSB,msgId);
                    break;
                }
            }
            break;
        }
        /* Async Event from MMWL */
        case RL_MMWL_ASYNC_EVENT_MSG:
        {
            switch (asyncSB)
            {
                case RL_MMWL_AE_MISMATCH_REPORT:
                {
                    int errTemp = *(int32_t*)payload;
                    /* CRC mismatched in the received Async-Event msg */
                    if (errTemp == RL_RET_CODE_CRC_FAILED)
                    {
                        gMmwlCrcFailAsyncEvent = 1U;
                    }
                    /* Checksum mismatched in the received msg */
                    else if (errTemp == RL_RET_CODE_CHKSUM_FAILED)
                    {
                        gMmwlChecksumFailAsyncEvent = 1U;
                    }
                    /* Polling to HostIRQ is timed out,
                    i.e. Device didn't respond to CNYS from the Host */
                    else if (errTemp == RL_RET_CODE_HOSTIRQ_TIMEOUT)
                    {
                        printf("HostIRQ polling timed out \n");
                    }
                    else
                    {
                        printf("mmWaveLink error \n");
                    }
                    break;
                }
                default:
                {
                    printf ("Error: Asynchronous Event SB Id %d not handled with msg ID 0x%x\n", asyncSB,msgId);
                    break;
                }
            }
            break;
        }
        default:
        {
            printf ("Error: Asynchronous message %d is NOT handled\n", msgId);
            break;
        }
    }
    return;
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to call the function in a different context
 *      This function is invoked from the Interrupt context.
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MmwaveLink_spawn (RL_P_OSI_SPAWN_ENTRY pEntry, const void* pValue, uint32_t flags)
{
    /* Record the function which is to be spawned. */
    if (gMmwaveLinkMCB.spawnFxn != NULL)
        gMmwaveLinkMCB.spawnOverrun++;

    /* Record the entry to be spawned. */
    gMmwaveLinkMCB.spawnFxn = pEntry;

    /* Post the semaphore and wake up the link management task */
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
    SemaphoreP_post (&gMmwaveLinkMCB.linkSemaphore);
#else
    Semaphore_post (gMmwaveLinkMCB.linkSemaphore);
#endif
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Radar Link Registered Callback function to compute the CRC.
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
static rlInt32_t MmwaveLink_computeCRC(rlUInt8_t* data, rlUInt32_t dataLen, rlUInt8_t crcType, rlUInt8_t* crc)
{
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
    uint64_t        signature;
    uint32_t        index;
    uint8_t*        ptrSignature;
    int32_t         retVal    = 0;
    uint32_t        crcLength = 0U;
    uint32_t        loopCnt;
    uint32_t        patternCnt;
    CRC_SignatureRegAddr   psaSignRegAddr;
    uint16_t*       ptrData16 = (uint16_t*)data;
    uint32_t*       ptrData32 = (uint32_t*)data;
    CRC_Config     config;

    /* Sanity Check: Validate the parameters; determine the length of the CRC */
    switch (crcType)
    {
        case RL_CRC_TYPE_16BIT_CCITT:
        {
            /* CRC is 2 bytes */
            crcLength = 2U;
            patternCnt = dataLen/2;
            break;
        }
        case RL_CRC_TYPE_32BIT:
        {
            /* CRC is 4 bytes */
            crcLength = 4U;
            patternCnt = dataLen/4;
            break;
        }
        case RL_CRC_TYPE_64BIT_ISO:
        {
            /* CRC is 8 bytes */
            crcLength = 8U;
            patternCnt = dataLen/4;
            break;
        }
        default:
        {
            /* Error: The mmWave link has passed an invalid CRC Type */
            retVal = MINUS_ONE;
            break;
        }
    }

    /* Sanity Check: Did the link pass a valid CRC Type? */
    if (retVal == MINUS_ONE)
    {
        /* NO: Invalid CRC Type has been detected */
        DebugP_assert (0);
        goto exit;
    }

    config.mode         = CRC_OPERATION_MODE_FULLCPU;
    config.type         = CRC_TYPE_16BIT;
    config.dataLen      = CRC_DATALENGTH_16BIT;
    config.bitSwap      = CRC_BITSWAP_MSB;
    config.byteSwap     = CRC_BYTESWAP_ENABLE;
    config.patternCount = patternCnt;
    config.sectorCount  = 1;

    retVal = CRC_configure((uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR),
                          CRC_CHANNEL_1, &config);

    if(retVal < 0)
    {
        /* Error: Unable to configure CRC moduel. */
        DebugP_logInfo ("Error: Configure of CRC failed [Error code %d]\n", retVal);
        DebugP_assert (0);
        goto exit;
    }

    /* Get CRC PSA signature register address */
    CRC_getPSASigRegAddr((uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR),
                        CRC_CHANNEL_1, &psaSignRegAddr);

    /* Reset the CRC channel*/
    CRC_channelReset((uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR),
                    CRC_CHANNEL_1);

    /* compute the CRC by writing the data buffer on which CRC computation is needed */
    for (loopCnt = 0; loopCnt < patternCnt; loopCnt++)
    {
        if(crcType == RL_CRC_TYPE_16BIT_CCITT)
        {
            HW_WR_REG16(psaSignRegAddr.regL, (uint16_t)ptrData16[loopCnt]);
        }
        else
        {
            HW_WR_REG32(psaSignRegAddr.regL, (uint32_t)ptrData32[loopCnt]);
        }
    }

    /* Fetch CRC signature value */
    retVal = CRC_getPSASig((uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR),
                          CRC_CHANNEL_1, (CRC_Signature*)&signature);

    /* Get the pointer to the CRC Signature: */
    ptrSignature = (uint8_t*)&signature;
#else
    CRC_SigGenCfg   signGenCfg;
    int32_t         errCode;
    uint64_t        signature;
    uint32_t        index;
    uint8_t*        ptrSignature;
    uint8_t         crcLength;

    /* Initialize the signature generation configuration */
    memset ((void *)&signGenCfg, 0, sizeof(CRC_SigGenCfg));

    /* Allocate a unique transaction id: */
    if (CRC_getTransactionId (gMmwaveLinkMCB.crcHandle, &signGenCfg.transactionId, &errCode) < 0)
    {
        printf ("Error: CRC Driver Get transaction id failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Populate the signature generation configuration: */
    signGenCfg.ptrData = (uint8_t*)data;
    signGenCfg.dataLen = dataLen;

    /* Compute the signature for the specific data on Channel-1 */
    if (CRC_computeSignature (gMmwaveLinkMCB.crcHandle, &signGenCfg, &errCode) < 0)
    {
        printf ("Error: CRC Driver compute signature failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Get the Signature for Channel */
    if (CRC_getSignature (gMmwaveLinkMCB.crcHandle, signGenCfg.transactionId, &signature, &errCode) < 0)
    {
        printf ("Error: CRC Driver get signature failed [Error code %d]\n", errCode);
        return -1;
    }

    /* Get the pointer to the CRC Signature: */
    ptrSignature = (uint8_t*)&signature;

    /* Determine the length of the CRC: */
    switch (crcType)
    {
        case RL_CRC_TYPE_16BIT_CCITT:
        {
            crcLength = 2;
            break;
        }
        case RL_CRC_TYPE_32BIT:
        {
            crcLength = 4;
            break;
        }
        case RL_CRC_TYPE_64BIT_ISO:
        {
            crcLength = 8;
            break;
        }
        default:
        {
            printf ("Error: Unknown CRC Type passed from mmWave Link: %d\n", crcType);
            return -1;
        }
    }

#endif
    /* Copy the CRC signature into CRC output array*/
    for(index = 0U; index < crcLength; index++)
    {
        *(crc + index) = *(ptrSignature + index);
    }

    /* CRC signature has been computed correctly */
    retVal = 0;

exit:
    return (rlInt32_t)retVal;
}

/**
 *  @b Description
 *  @n
 *      This is the task which handles the mmWave Link communication
 *      messages between the BSS and MSS.
 *
 *  @retval
 *      Not Applicable.
 */
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
static void MmwaveLink_mmwaveLinkMgmtTask (void* arg)
#else
static void MmwaveLink_mmwaveLinkMgmtTask (UArg arg0, UArg arg1)
#endif
{
    RL_P_OSI_SPAWN_ENTRY    spawnFxn;
    uintptr_t               key;
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)

    /* Initialize the mmWave Link Semaphore: */
    SemaphoreP_constructBinary(&gMmwaveLinkMCB.linkSemaphore, 0);

    /* Execute forever: */
    while (1)
    {
        /* Pending on the link semaphore */
        SemaphoreP_pend (&gMmwaveLinkMCB.linkSemaphore, SystemP_WAIT_FOREVER);

        /* Critical Section: We record the spawn function which is to be executed */
        key = HwiP_disable();
        spawnFxn = gMmwaveLinkMCB.spawnFxn;
        gMmwaveLinkMCB.spawnFxn = NULL;
        HwiP_restore (key);
#else
    Semaphore_Params        semParams;

    /* Initialize the mmWave Link Semaphore: */
    Semaphore_Params_init(&semParams);
    semParams.mode  = Semaphore_Mode_BINARY;
    gMmwaveLinkMCB.linkSemaphore = Semaphore_create(0, &semParams, NULL);

    /* Execute forever: */
    while (1)
    {
        /* Pending on the link semaphore */
        Semaphore_pend (gMmwaveLinkMCB.linkSemaphore, BIOS_WAIT_FOREVER);

        /* Critical Section: We record the spawn function which is to be executed */
        key = Hwi_disable();
        spawnFxn = gMmwaveLinkMCB.spawnFxn;
        gMmwaveLinkMCB.spawnFxn = NULL;
        Hwi_restore (key);
#endif
        /* Execute the spawn function: */
        spawnFxn (NULL);
    }
}


/**
 *  @b Description
 *  @n
 *      The function is used to get and display the version information
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_getVersion (void)
{
    rlVersion_t verArgs;
    int32_t     retVal;
    /* currently patch binaries are available for AWR16 ES2.0 & IWR16 ES2.0 only*/
    int8_t rfPatchBuildVer, rfPatchDebugVer;

    /* Get the version string: */
    retVal = rlDeviceGetVersion(RL_DEVICE_MAP_INTERNAL_BSS, &verArgs);
    if (retVal != 0)
    {
        printf ("Error: Unable to get the device version from mmWave link [Error %d]\n", retVal);
        return -1;
    }

    /* Display the version information */
    printf ("RF H/W Version    : %02d.%02d\n",
                    verArgs.rf.hwMajor, verArgs.rf.hwMinor);
    printf ("RF F/W Version    : %02d.%02d.%02d.%02d.%02d.%02d.%02d\n",
                    verArgs.rf.fwMajor, verArgs.rf.fwMinor, verArgs.rf.fwBuild, verArgs.rf.fwDebug,
                    verArgs.rf.fwYear, verArgs.rf.fwMonth, verArgs.rf.fwDay);
    rfPatchDebugVer = ((verArgs.rf.patchBuildDebug) & 0x0F);
    rfPatchBuildVer = (((verArgs.rf.patchBuildDebug) & 0xF0) >> 4);

    printf ("RF F/W Patch Version : %02d.%02d.%02d.%02d.%02d.%02d.%02d\n",
                    verArgs.rf.patchMajor, verArgs.rf.patchMinor, rfPatchBuildVer, rfPatchDebugVer,
                    verArgs.rf.patchYear, verArgs.rf.patchMonth, verArgs.rf.patchDay);
    printf ("mmWaveLink Version: %02d.%02d.%02d.%02d\n",
                    verArgs.mmWaveLink.major, verArgs.mmWaveLink.minor,
                    verArgs.mmWaveLink.build, verArgs.mmWaveLink.debug);
    return 0;
}

StaticTask_t        gStaticTask;
/**
 *  @b Description
 *  @n
 *      The function is used to initialize and setup the mmWave link
 *
 *  @retval
 *      Success - 0
 *  @retval
 *      Error   - <0
 */
int32_t MmwaveLink_initLink (rlUInt8_t deviceType, rlUInt8_t platform)
{
    rlClientCbs_t       RlApp_ClientCtx;
    TaskHandle_t        taskHandle;

    int32_t             retVal;

    /* Initialize and populate the Mmwave Link MCB */
    memset ((void*)&gMmwaveLinkMCB, 0, sizeof(MmwaveLink_MCB));

    /*****************************************************************************
     * Start CRC driver:
     *****************************************************************************/

    /* Configure CRC Module */
    retVal = CRC_channelReset((uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR),
            CRC_CHANNEL_1);
    DebugP_assert(retVal == SystemP_SUCCESS);

    retVal = CRC_initialize((uint32_t) AddrTranslateP_getLocalAddr(CONFIG_CRC0_BASE_ADDR),
                           CRC_CHANNEL_1, 0, 0);

    if (retVal != SystemP_SUCCESS)
    {
        printf ("Error: Unable to open the CRC Channel [Error Code %d]\n", retVal);
        return -1;
    }
    printf("Debug: CRC Channel %d has been opened successfully\n", CRC_CHANNEL_1);

    /*****************************************************************************
     * Launch the Mmwave Link Tasks:
     *****************************************************************************/

    /* Initialize and Launch the mmWave Link Management Task: */

    taskHandle = xTaskCreateStatic( MmwaveLink_mmwaveLinkMgmtTask,
                                      "link_mgnt_task",
                                      MMW_LINK_MGMNT_TASK_STACK_SIZE,
                                      NULL,
                                      4,
                                      linkMgmntStack,
                                      &gStaticTask);

    configASSERT(taskHandle != NULL);

    /*****************************************************************************
     * Initialize the mmWave Link: We need to have the link management task
     * operational to be able to process the SPAWN function.
     *****************************************************************************/

    /* Reset the client context: */
    memset ((void *)&RlApp_ClientCtx, 0, sizeof(rlClientCbs_t));

    RlApp_ClientCtx.ackTimeout  = 1000U;

    /* Setup the crc Type in the mmWave link and synchronize this with the
     * created CRC Channel. */
    RlApp_ClientCtx.crcType = RL_CRC_TYPE_16BIT_CCITT;

    /* Setup the platform on which the mmWave Link executes */
    RlApp_ClientCtx.platform  = platform;
    RlApp_ClientCtx.arDevType = deviceType;

    /* xWR294x device */

    /* Initialize the Communication Interface API: */
    RlApp_ClientCtx.comIfCb.rlComIfOpen     = MmwaveLink_mboxOpen;
    RlApp_ClientCtx.comIfCb.rlComIfClose    = MmwaveLink_mboxClose;
    RlApp_ClientCtx.comIfCb.rlComIfRead     = MmwaveLink_mboxRead;
    RlApp_ClientCtx.comIfCb.rlComIfWrite    = MmwaveLink_mboxWrite;


    /* Initialize OSI Mutex Interface */
    RlApp_ClientCtx.osiCb.mutex.rlOsiMutexCreate = Osal_mutexCreate;
    RlApp_ClientCtx.osiCb.mutex.rlOsiMutexLock   = Osal_mutexLock;
    RlApp_ClientCtx.osiCb.mutex.rlOsiMutexUnLock = Osal_mutexUnlock;
    RlApp_ClientCtx.osiCb.mutex.rlOsiMutexDelete = Osal_mutexDelete;

    /* Initialize OSI Semaphore Interface */
    RlApp_ClientCtx.osiCb.sem.rlOsiSemCreate    = Osal_semCreate;
    RlApp_ClientCtx.osiCb.sem.rlOsiSemWait      = Osal_semWait;
    RlApp_ClientCtx.osiCb.sem.rlOsiSemSignal    = Osal_semSignal;
    RlApp_ClientCtx.osiCb.sem.rlOsiSemDelete    = Osal_semDelete;

    /* Initialize OSI Queue Interface */
    RlApp_ClientCtx.osiCb.queue.rlOsiSpawn      = MmwaveLink_spawn;

    /* Initialize OSI Timer Interface */
    RlApp_ClientCtx.timerCb.rlDelay             = NULL;

    /* Initialize the CRC Interface */
    RlApp_ClientCtx.crcCb.rlComputeCRC          = MmwaveLink_computeCRC;

    /* Initialize Device Control Interface */
    RlApp_ClientCtx.devCtrlCb.rlDeviceDisable            = MmwaveLink_disableDevice;
    RlApp_ClientCtx.devCtrlCb.rlDeviceEnable             = MmwaveLink_enableDevice;
    RlApp_ClientCtx.devCtrlCb.rlDeviceMaskHostIrq        = MmwaveLink_maskHostIRQ;
    RlApp_ClientCtx.devCtrlCb.rlDeviceUnMaskHostIrq      = MmwaveLink_unmaskHostIRQ;

    RlApp_ClientCtx.devCtrlCb.rlRegisterInterruptHandler = MmwaveLink_registerInterruptHandler;

    /* Initialize the Asynchronous Event Handler: */
    RlApp_ClientCtx.eventCb.rlAsyncEvent    = MmwaveLink_asyncEventHandler;

    /* Power on the Device */
    if (rlDevicePowerOn(1U, RlApp_ClientCtx) != 0)
    {
        printf("Error: Power on request to the BSS failed\n");
        return -1;
    }

    return 0;
}

int32_t MmwaveLink_getRfBootupStatus (void)
{
    int32_t         retVal;
    rlRfBootStatusCfg_t statusCfg = {0};

    /* Get RF Boot status */
    retVal = rlGetRfBootupStatus(RL_DEVICE_MAP_INTERNAL_BSS, &statusCfg);
    printf("Debug: BootupStatus = 0x%x\n", statusCfg.bssSysStatus);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlGetRfBootupStatus retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished get radarSS bootup status to BSS\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set/send mmWave Link Channel Configuration to the BSS.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setChannelConfig (bool useInputRxConfig, rlUInt16_t rxChannelEn)
{
    int32_t         retVal;
    rlChanCfg_t     localChCfg = chCfg;

    if(useInputRxConfig)
    {
        localChCfg.rxChannelEn = rxChannelEn;
    }

    /* Set channel configuration */
    retVal = rlSetChannelConfig(RL_DEVICE_MAP_INTERNAL_BSS, &localChCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: setChannelConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished set channel configurations to BSS\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets Loop Back burst configuration.
 *
 *  @param Type of loop back
 *         1 - IF Loop Backm, 2 - PS Loop Back, 3 - PA Loop Back, Others - No Loop Back
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_rlSetLoopBackBurstCfg(uint8_t loopBackType)
{
    int32_t         retVal=0;
    return retVal;
}
/**
 *  @b Description
 *  @n
 *      The function is used for Advance Chirp Configuration.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */

int32_t MmwaveLink_rlSetAdvChirpCfg()
{
    int32_t retVal = 0;
    rlRfMiscConf_t MiscCfg = { 0 };
    rlProfileCfg_t profileCfg[4] = {0};
    int i;
    rlAdvChirpCfg_t AdvChirpCfgArgs = { 0 };
	rlFillLUTParams_t rlFillLUTParamsArgs = { 0 };
	/* profile ID LUT dither local buffer */
	rlInt8_t ProfileCfgData[6] = { 0 };
	/* Frequency start LUT dither local buffer */
	rlInt16_t StartFreqData[CHIRPS_PER_CYCLE*2] = { 0 }; /* Change this based on the chirp param size*/
	rlInt8_t StartFreqSlopeData[128] = { 0 };

	rlAdvChirpLUTCfg_t rlAdvChirpLUTCfgArgs = { 0 };

    /* Enable Adv chirp feature
    b0: PERCHIRP_PHASESHIFTER_EN
    b1: ADVANCE_CHIRP_CONFIG_EN  */
    MiscCfg.miscCtl = 0x7; //03
    /* Enable advChirp in miscConfig */
    retVal = rlRfSetMiscConfig(RL_DEVICE_MAP_INTERNAL_BSS, &MiscCfg);


    /* two profile configurations */
    double gCLI_mmwave_freq_scale_factor = 3.6;

    /*****  PROFILE-0 **********/
    /* Populate the profile configuration: */
    profileCfg[0].profileId             = 0;
    profileCfg[0].pfVcoSelect           = 0x3; /* VCO_SEL : VCO2 for xWR294x to use 76-81 GHz */
    /* Translate from GHz to [1 LSB = gCLI_mmwave_freq_scale_factor * 1e9 / 2^26 Hz] units
     * of mmwavelink format */
    profileCfg[0].startFreqConst        = (uint32_t) ((float)PROFILE_0_START_FREQ * (1U << 26) / 3.6); //77GHz
    /* Translate below times from us to [1 LSB = 10 ns] units of mmwavelink format */
    profileCfg[0].idleTimeConst         = (uint32_t)((float)PROFILE_0_IDLE_TIME * 1000 / 10);
    profileCfg[0].adcStartTimeConst     = (uint32_t)((float)PROFILE_0_ADC_START_TIME * 1000 / 10);
    profileCfg[0].rampEndTime           = (uint32_t)((float)PROFILE_0_RAMP_END_TIME * 1000 / 10);
    profileCfg[0].txOutPowerBackoffCode = 0;
    profileCfg[0].txPhaseShifter        = 0;
    /* Translate from MHz/us to [1 LSB = (gCLI_mmwave_freq_scale_factor * 1e6 * 900) / 2^26 kHz/uS]
     * units of mmwavelink format */
    profileCfg[0].freqSlopeConst        =  (int16_t)(PROFILE_0_FREQ_SLOPE * (1U << 26) /
                                              ((gCLI_mmwave_freq_scale_factor * 1e3) * 900.0));
    /* Translate from us to [1 LSB = 10 ns] units of mmwavelink format */
    profileCfg[0].txStartTime           = (int32_t)((float)PROFILE_0_TX_START_TIME * 1000 / 10);
    profileCfg[0].numAdcSamples         = PROFILE_0_NUM_SAMPLE;
    profileCfg[0].digOutSampleRate      = PROFILE_0_SAMPLE_RATE;
    profileCfg[0].hpfCornerFreq1        = 0;
    profileCfg[0].hpfCornerFreq2        = 0;
    profileCfg[0].rxGain                = PROFILE_0_RX_GAIN;
    profileCfg[0].miscFeatureEn         = 7;
    profileCfg[0].pfCalLutUpdate        = 0x03;

     /*****  PROFILE-1 **********/
    /* Populate the profile configuration: */
    profileCfg[1].profileId             = 1;
    profileCfg[1].pfVcoSelect           = 0x3; /* VCO_SEL : VCO2 for xWR294x to use 76-81 GHz */
    profileCfg[1].startFreqConst        = (uint32_t) ((float)PROFILE_1_START_FREQ * (1U << 26) / 3.6); //77GHz
    profileCfg[1].idleTimeConst         = (uint32_t)((float)PROFILE_1_IDLE_TIME * 1000 / 10);
    profileCfg[1].adcStartTimeConst     = (uint32_t)((float)PROFILE_1_ADC_START_TIME * 1000 / 10);
    profileCfg[1].rampEndTime           = (uint32_t)((float)PROFILE_1_RAMP_END_TIME * 1000 / 10);
    profileCfg[1].txOutPowerBackoffCode = 0;
    profileCfg[1].txPhaseShifter        = 0;
    /* Translate from MHz/us to [1 LSB = (gCLI_mmwave_freq_scale_factor * 1e6 * 900) / 2^26 kHz/uS]
     * units of mmwavelink format */
    profileCfg[1].freqSlopeConst        = (int16_t)(PROFILE_1_FREQ_SLOPE * (1U << 26) /
                                              ((gCLI_mmwave_freq_scale_factor * 1e3) * 900.0));
    /* Translate from us to [1 LSB = 10 ns] units of mmwavelink format */
    profileCfg[1].txStartTime           = (int32_t)((float)PROFILE_1_TX_START_TIME * 1000 / 10);
    profileCfg[1].numAdcSamples         = PROFILE_1_NUM_SAMPLE;
    profileCfg[1].digOutSampleRate      = PROFILE_1_SAMPLE_RATE;
    profileCfg[1].hpfCornerFreq1        = 0;
    profileCfg[1].hpfCornerFreq2        = 0;
    profileCfg[1].rxGain                = PROFILE_1_RX_GAIN;
    profileCfg[1].pfCalLutUpdate        = 0x03;
    /*****  PROFILE-2 **********/
    /* Populate the profile configuration: */
    profileCfg[2].profileId             = 2;
    profileCfg[2].pfVcoSelect           = 0x3; /* VCO_SEL : VCO2 for xWR294x to use 76-81 GHz */
    profileCfg[2].startFreqConst        = (uint32_t) ((float)PROFILE_2_START_FREQ * (1U << 26) / 3.6); //77GHz
    profileCfg[2].idleTimeConst         = (uint32_t)((float)PROFILE_2_IDLE_TIME * 1000 / 10);
    profileCfg[2].adcStartTimeConst     = (uint32_t)((float)PROFILE_2_ADC_START_TIME * 1000 / 10);
    profileCfg[2].rampEndTime           = (uint32_t)((float)PROFILE_2_RAMP_END_TIME * 1000 / 10);
    profileCfg[2].txOutPowerBackoffCode = 0;
    profileCfg[2].txPhaseShifter        = 0;
    /* Translate from MHz/us to [1 LSB = (gCLI_mmwave_freq_scale_factor * 1e6 * 900) / 2^26 kHz/uS]
     * units of mmwavelink format */
    profileCfg[2].freqSlopeConst        = (int16_t)(PROFILE_2_FREQ_SLOPE * (1U << 26) /
                                              ((gCLI_mmwave_freq_scale_factor * 1e3) * 900.0));
    /* Translate from us to [1 LSB = 10 ns] units of mmwavelink format */
    profileCfg[2].txStartTime           = (int32_t)((float)PROFILE_2_TX_START_TIME * 1000 / 10);
    profileCfg[2].numAdcSamples         = PROFILE_2_NUM_SAMPLE;
    profileCfg[2].digOutSampleRate      = PROFILE_2_SAMPLE_RATE;
    profileCfg[2].hpfCornerFreq1        = 0;
    profileCfg[2].hpfCornerFreq2        = 0;
    profileCfg[2].rxGain                = PROFILE_2_RX_GAIN;
    profileCfg[2].pfCalLutUpdate        = 0x03;
    /*****  PROFILE-3 **********/
    /* Populate the profile configuration: */
    profileCfg[3].profileId             = 3;
    profileCfg[3].pfVcoSelect           = 0x3; /* VCO_SEL : VCO2 for xWR294x to use 76-81 GHz */
    profileCfg[3].startFreqConst        = (uint32_t) ((float)PROFILE_3_START_FREQ * (1U << 26) / 3.6); //77GHz
    profileCfg[3].idleTimeConst         = (uint32_t)((float)PROFILE_3_IDLE_TIME * 1000 / 10);
    profileCfg[3].adcStartTimeConst     = (uint32_t)((float)PROFILE_3_ADC_START_TIME * 1000 / 10);
    profileCfg[3].rampEndTime           = (uint32_t)((float)PROFILE_3_RAMP_END_TIME * 1000 / 10);
    profileCfg[3].txOutPowerBackoffCode = 0;
    profileCfg[3].txPhaseShifter        = 0;
    /* Translate from MHz/us to [1 LSB = (gCLI_mmwave_freq_scale_factor * 1e6 * 900) / 2^26 kHz/uS]
     * units of mmwavelink format */
    profileCfg[3].freqSlopeConst        = (int16_t)(PROFILE_3_FREQ_SLOPE * (1U << 26) /
                                              ((gCLI_mmwave_freq_scale_factor * 1e3) * 900.0));
    /* Translate from us to [1 LSB = 10 ns] units of mmwavelink format */
    profileCfg[3].txStartTime           = (int32_t)((float)PROFILE_3_TX_START_TIME * 1000 / 10);
    profileCfg[3].numAdcSamples         = PROFILE_3_NUM_SAMPLE;
    profileCfg[3].digOutSampleRate      = PROFILE_3_SAMPLE_RATE;
    profileCfg[3].hpfCornerFreq1        = 0;
    profileCfg[3].hpfCornerFreq2        = 0;
    profileCfg[3].rxGain                = PROFILE_3_RX_GAIN;
    profileCfg[3].pfCalLutUpdate        = 0x03;
    /* send all four profiles */
    retVal = rlSetProfileConfig(RL_DEVICE_MAP_INTERNAL_BSS, 4, &profileCfg[0]);



    /* Configure Profile (Param Index = 0) */
	/* Fixed delta dither is not supported for profile parameter */
	/* Fixed Profile ID:0 for all the chirps */
	/*   Chirp    Profile   */
	/*   0-N       0 */
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_CHIRP_PROFILE_VAR;
	AdvChirpCfgArgs.lutResetPeriod = CHIRPS_PER_CYCLE*2; /* reset back to original offset after 2nd burst (512*2) chirps */
	AdvChirpCfgArgs.lutParamUpdatePeriod = CHIRPS_PER_CYCLE/2; /* change profile at 256 chirp (at every half cyle/burst) */
	AdvChirpCfgArgs.lutPatternAddressOffset = 0;
	AdvChirpCfgArgs.numOfPatterns = 4;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

    /* profile index */
    ProfileCfgData[0] = 0;
    ProfileCfgData[1] = 1;
    ProfileCfgData[2] = 2;
    ProfileCfgData[3] = 3;

    /* fill up the Chirp LUT buffer which is used later for rlSetAdvChirpLUTConfig API */
	rlFillLUTParamsArgs.chirpParamIndex = RL_LUT_CHIRP_PROFILE_VAR;
	rlFillLUTParamsArgs.inputSize = AdvChirpCfgArgs.numOfPatterns;
	rlFillLUTParamsArgs.lutGlobalOffset = lutOffsetInNBytes;
    rlFillLUTParamsArgs.chirpParamSize = AdvChirpCfgArgs.lutChirpParamSize;
	retVal = rlDevSetFillLUTBuff(&rlFillLUTParamsArgs, &ProfileCfgData[0], &AdvChirpLUTData[lutOffsetInNBytes], &lutOffsetInNBytes);

    /* reserved starting 16 WORD for param type with Fixed dither */
    //lutOffsetInNBytes += RESERVED_DITHER_LUT_SIZE * 4; /* convert that into bytes */

	/* Start Frequency (Param Index = 1) */
	/* For given modulation scheme (increase and decrease startFreq dither), we can't use delta dither but LUT dither.
    */
	/* Configuring 512 *2 unique start frequency LUT dither (+0.0014, +0.0014*2...., +0.0014*511, -0.0014,-0.0014*2...., -0.0014*511) GHz :  1.4MHz increment at every chirp till 512, then decrement by -1.4MHz at every chirp till next 512 chirps */
	/* The new start frequency LUT dither is picked every chirp (update period = 1) and it will reset every [512 * 2] chirps [= at 2 bursts] */
	/*   Chirp    Start Freq (from Profile) + LUT dither + Fixed delta dither
		   0         77 GHz + 0.0014 GHz + 0
		   1         77 GHz + 0.0014 *2 GHz + 0
		   2         77 GHz + 0.0014 *3 GHz + 0
		  ........
          ........
           511       77 GHz + 0.0014 *511 GHz + 0

           512       77 GHz - 0.0014 GHz + 0
		   513       77 GHz - 0.0014 *2 GHz + 0
		   514       77 GHz - 0.0014 *3 GHz + 0
		  ........
          ........
		   1023      77 GHz - 0.0014 *511 GHz + 0

           (LUT dither reset period = 1024) and so on */
	/* LUT start address offset for the start frequency chirp parameter is made 4 */
	/* AdvChirpLUTData[4] is the start address offset (Offset = 4), Each data parameter is 2 bytes (used lutChirpParamSize = 1 and lutChirpParamScale = 0) */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_CHIRP_FREQ_START_VAR;
	AdvChirpCfgArgs.resetMode = 0; /* reset at the end of frame */
	AdvChirpCfgArgs.deltaResetPeriod = 0;
	AdvChirpCfgArgs.deltaParamUpdatePeriod = 0; /* Delta Dither is disabled */
	AdvChirpCfgArgs.lutResetPeriod = CHIRPS_PER_CYCLE*2; /* reset LUT dither to origin offset after 256*2*2 chirps */
	AdvChirpCfgArgs.lutParamUpdatePeriod = 1;
	AdvChirpCfgArgs.lutPatternAddressOffset =lutOffsetInNBytes;//4; /* start address offset (4 bytes boundary) for this chirp_param_type */
	AdvChirpCfgArgs.numOfPatterns = CHIRPS_PER_CYCLE *2;
	AdvChirpCfgArgs.lutChirpParamSize = 1; /* 2 byte size of each freqStart dither var */
	AdvChirpCfgArgs.lutChirpParamScale = 0;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);

	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

	/* Dividing the input param data by 1 LSB . 1 LSB = ((3.6 * 10^9)/2^26) * 2^Scale Hz = (3.6 * 2^Scale)/2^26 GHz */
    for(i=0; i < CHIRPS_PER_CYCLE; i++)
    {
        StartFreqData[i] = (rlInt16_t) (INCR_START_FREQ_DITH *(i));

    }

    /* for freq decrement dither */
    for(i=0; i < CHIRPS_PER_CYCLE; i++)
    {
        StartFreqData[CHIRPS_PER_CYCLE+i] = (rlInt16_t) (DCR_START_FREQ_DITH *(i));

    }

    /* fill up the Chirp LUT buffer which is used later for rlSetAdvChirpLUTConfig API */
	rlFillLUTParamsArgs.chirpParamIndex = RL_LUT_CHIRP_FREQ_START_VAR;
	rlFillLUTParamsArgs.chirpParamSize = AdvChirpCfgArgs.lutChirpParamSize;
	rlFillLUTParamsArgs.inputSize = AdvChirpCfgArgs.numOfPatterns;
	rlFillLUTParamsArgs.lutGlobalOffset = lutOffsetInNBytes;

	retVal = rlDevSetFillLUTBuff(&rlFillLUTParamsArgs, (rlInt8_t*)&StartFreqData[0], &AdvChirpLUTData[lutOffsetInNBytes], &lutOffsetInNBytes);


    /* Frequency Slope (Param Index = 2) */
	/* LUT Dither to this chirp param type */
	/* LUT start address offset for the slope chirp parameter is made 2056 */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_CHIRP_FREQ_SLOPE_VAR;
    AdvChirpCfgArgs.deltaResetPeriod =0;
	AdvChirpCfgArgs.deltaParamUpdatePeriod =0;
    AdvChirpCfgArgs.lutResetPeriod = 512*2;
	AdvChirpCfgArgs.lutParamUpdatePeriod = 8;
	AdvChirpCfgArgs.lutPatternAddressOffset =lutOffsetInNBytes;;
	AdvChirpCfgArgs.numOfPatterns = 128;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

    for(i=0; i < 64; i++)
    {
        StartFreqSlopeData[i] = (rlInt8_t) (1 *(i));

    }

    /* for freq decrement dither */
    for(i=0; i < 64; i++) //check
    {
        StartFreqSlopeData[64+i] = (rlInt8_t) (-1 *(i));

    }

    /* fill up the Chirp LUT buffer which is used later for rlSetAdvChirpLUTConfig API */
	rlFillLUTParamsArgs.chirpParamIndex = RL_LUT_CHIRP_FREQ_SLOPE_VAR;
	rlFillLUTParamsArgs.inputSize = AdvChirpCfgArgs.numOfPatterns;
	rlFillLUTParamsArgs.lutGlobalOffset = lutOffsetInNBytes;
	retVal = rlDevSetFillLUTBuff(&rlFillLUTParamsArgs, (rlInt8_t*)&StartFreqSlopeData[0], &AdvChirpLUTData[lutOffsetInNBytes], &lutOffsetInNBytes);


    /* Idle time (Param Index = 3) */
	/* Fixed idle time delta dither 2 us at every 512 chirp (update period = 512) and it will reset every 512*3 chirps.
    NO LUT DITHER applied here. */
	/*   Chirp    Idle time (from Profile) + LUT dither + Fixed delta dither
		   0-511         100 us + 0       + 0
		   512-1023      100 us + 0       + 2 us
		   1024-1535     100 us + 0       + 2*2 us
           (Delta dither reset period = 512*3 ) and so on */
	/* Number of unique LUT dither parameters (4) */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_CHIRP_IDLE_TIME_VAR;
	AdvChirpCfgArgs.deltaResetPeriod = CHIRPS_PER_CYCLE*3; /* Reset delta at 512 * 3 */
	AdvChirpCfgArgs.deltaParamUpdatePeriod = CHIRPS_PER_CYCLE; /* add delta value after 512 chirps */
	AdvChirpCfgArgs.sf0ChirpParamDelta = (uint32_t)((float)CHIRP_IDLE_TIME_DITH * 1000.0 / 10.0);
    AdvChirpCfgArgs.lutResetPeriod = 1;
	AdvChirpCfgArgs.lutParamUpdatePeriod = 0;
	AdvChirpCfgArgs.lutPatternAddressOffset = DUMMY_LUT_ADDR_OFFSET;
	AdvChirpCfgArgs.numOfPatterns = 1;
	AdvChirpCfgArgs.lutChirpParamScale = 0;
	AdvChirpCfgArgs.lutChirpParamSize = 0;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

	/* ADC start time (Param Index = 4) */
	/* No Dither to this chirp param type */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_CHIRP_ADC_START_TIME_VAR;
    AdvChirpCfgArgs.lutResetPeriod = 1;
	AdvChirpCfgArgs.lutParamUpdatePeriod = 0;
	AdvChirpCfgArgs.lutPatternAddressOffset = DUMMY_LUT_ADDR_OFFSET; /* Fixed LUT pattern to zero offset */
	AdvChirpCfgArgs.numOfPatterns = 1;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

	/* BPM Enable (Param Index = 6) */
	/* No LUT or Delta Dither for BPM Enable parameter */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_CHIRP_BPM_VAL_VAR;
    AdvChirpCfgArgs.lutResetPeriod = 1;
	AdvChirpCfgArgs.lutParamUpdatePeriod = 0;
	AdvChirpCfgArgs.lutPatternAddressOffset = DUMMY_LUT_ADDR_OFFSET; /* Fixed LUT pattern to offset */
	AdvChirpCfgArgs.numOfPatterns = 1;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

	/* TX0 Phase shifter (Param Index = 7) */
	/* No Delta or LUT dither for TX0 phase shifter */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_TX0_PHASE_SHIFT_VAR;
    AdvChirpCfgArgs.lutResetPeriod = 1;
	AdvChirpCfgArgs.lutParamUpdatePeriod = 0;
	AdvChirpCfgArgs.lutPatternAddressOffset = DUMMY_LUT_ADDR_OFFSET; /* Fixed LUT pattern to offset */
	AdvChirpCfgArgs.numOfPatterns = 1;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}
	/* TX1 Phase shifter (Param Index = 8) */
	/* No Delta or LUT dither for TX0 phase shifter */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_TX1_PHASE_SHIFT_VAR;
     AdvChirpCfgArgs.lutResetPeriod = 1;
	AdvChirpCfgArgs.lutParamUpdatePeriod = 0;
	AdvChirpCfgArgs.lutPatternAddressOffset = DUMMY_LUT_ADDR_OFFSET; /* Fixed LUT pattern to offset */
	AdvChirpCfgArgs.numOfPatterns = 1;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

    /* TX2 Phase shifter (Param Index = 9) */
	/* No Delta or LUT dither for TX2 phase shifter */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_TX2_PHASE_SHIFT_VAR;
    AdvChirpCfgArgs.lutResetPeriod = 1;
	AdvChirpCfgArgs.lutParamUpdatePeriod = 0;
	AdvChirpCfgArgs.lutPatternAddressOffset = DUMMY_LUT_ADDR_OFFSET; /* Fixed LUT pattern to offset */
	AdvChirpCfgArgs.numOfPatterns = 1;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

    /* TX3 Phase shifter (Param Index = 10) */
	/* No Delta or LUT dither for TX0 phase shifter */
	memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
	AdvChirpCfgArgs.chirpParamIdx = RL_LUT_TX3_PHASE_SHIFT_VAR;
    AdvChirpCfgArgs.lutResetPeriod = 1;
	AdvChirpCfgArgs.lutParamUpdatePeriod = 0;
	AdvChirpCfgArgs.lutPatternAddressOffset = DUMMY_LUT_ADDR_OFFSET; /* Fixed LUT pattern to zero offset */
	AdvChirpCfgArgs.numOfPatterns = 1;

	retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);
	if (retVal != RL_RET_CODE_OK)
	{
		printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
		return retVal;
	}

     /* Tx Enable (Param Index = 5) */
    /* Fixed delta dither is not supported for Tx Enable parameter */
    /* Configuring 4 unique Tx enable mask (7,3,1,2) in the generic SW LUT - LUT Reset period (4) */
    /* The new Tx enable mask is picked every chirp (update period = 1) and it will reset every 4 chirps */
    /*   Chirp (burst)   Tx enable mask
          0-511          1
          512-1023       2
          1024-1535      4
          1536-2047      8 and then reset */
    /* Each data parameter is 4 bits */
    memset((void *)&AdvChirpCfgArgs, 0, sizeof(rlAdvChirpCfg_t));
    AdvChirpCfgArgs.chirpParamIdx = RL_LUT_CHIRP_TX_EN_VAR;
    AdvChirpCfgArgs.lutResetPeriod = 1;
    AdvChirpCfgArgs.lutParamUpdatePeriod = 0;
    AdvChirpCfgArgs.lutPatternAddressOffset =DUMMY_LUT_ADDR_OFFSET;//2188;
    AdvChirpCfgArgs.numOfPatterns = 1;


    retVal = rlSetAdvChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvChirpCfgArgs);

    if (retVal != RL_RET_CODE_OK)
    {
        printf("*** Failed - rlSetAdvChirpConfig failed with error code %d*** \n\n", retVal);
        return retVal;
    }

    while(lutOffsetInNBytes%4 != 0)
    {
        lutOffsetInNBytes+=1;
    }

    /* Send the locally programmed LUT data to the device */
    rlAdvChirpLUTCfgArgs.lutAddressOffset = 0;
    rlAdvChirpLUTCfgArgs.numBytes = lutOffsetInNBytes;

    retVal = rlSetMultiAdvChirpLUTConfig(RL_DEVICE_MAP_INTERNAL_BSS, &rlAdvChirpLUTCfgArgs, &AdvChirpLUTData[0]);

    if (retVal != RL_RET_CODE_OK)
    {
        printf("rlSetMultiAdvChirpLUTConfig for deviceMap %u failed with error code %d \n\n", RL_DEVICE_MAP_INTERNAL_BSS, retVal);

    }
    else
    {
        printf("rlSetMultiAdvChirpLUTConfig success for deviceMap %u with lutAddressOffset = %d and numBytes = %d\n\n", RL_DEVICE_MAP_INTERNAL_BSS, rlAdvChirpLUTCfgArgs.lutAddressOffset, rlAdvChirpLUTCfgArgs.numBytes);
    }

    return retVal;
}

/**
 *  @b Description
 *  @n
 *      The function Configure chirps dynamically.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_rlSetDynChirpCfg(void)
{
    int32_t         retVal;

    rlDynChirpCfg_t *dynChirpData[32] = {(rlDynChirpCfg_t*)&dynChirpCfg[0],(rlDynChirpCfg_t*)&dynChirpCfg[1],(rlDynChirpCfg_t*)&dynChirpCfg[2],
                                        (rlDynChirpCfg_t*)&dynChirpCfg[3],(rlDynChirpCfg_t*)&dynChirpCfg[4],(rlDynChirpCfg_t*)&dynChirpCfg[5],(rlDynChirpCfg_t*)&dynChirpCfg[6],
                                        (rlDynChirpCfg_t*)&dynChirpCfg[7],(rlDynChirpCfg_t*)&dynChirpCfg[8],(rlDynChirpCfg_t*)&dynChirpCfg[9],(rlDynChirpCfg_t*)&dynChirpCfg[10],
                                        (rlDynChirpCfg_t*)&dynChirpCfg[11],(rlDynChirpCfg_t*)&dynChirpCfg[12],(rlDynChirpCfg_t*)&dynChirpCfg[13],(rlDynChirpCfg_t*)&dynChirpCfg[14],
                                        (rlDynChirpCfg_t*)&dynChirpCfg[15],(rlDynChirpCfg_t*)&dynChirpCfg[16],(rlDynChirpCfg_t*)&dynChirpCfg[17],(rlDynChirpCfg_t*)&dynChirpCfg[18],
                                        (rlDynChirpCfg_t*)&dynChirpCfg[19],(rlDynChirpCfg_t*)&dynChirpCfg[20],(rlDynChirpCfg_t*)&dynChirpCfg[21],(rlDynChirpCfg_t*)&dynChirpCfg[22],
                                        (rlDynChirpCfg_t*)&dynChirpCfg[23],(rlDynChirpCfg_t*)&dynChirpCfg[24],(rlDynChirpCfg_t*)&dynChirpCfg[25],(rlDynChirpCfg_t*)&dynChirpCfg[26],
                                        (rlDynChirpCfg_t*)&dynChirpCfg[27],(rlDynChirpCfg_t*)&dynChirpCfg[28],(rlDynChirpCfg_t*)&dynChirpCfg[29],(rlDynChirpCfg_t*)&dynChirpCfg[30],
                                        (rlDynChirpCfg_t*)&dynChirpCfg[31]};

    /* Set Dynamic chirp configuration */
    retVal = rlSetDynChirpCfg(RL_DEVICE_MAP_INTERNAL_BSS, 32U, (rlDynChirpCfg_t**)&dynChirpData[0U]);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetDynChirpCfg retVal=%d\n", retVal);
        return -1;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to enable the dynamic chirp configuration
 *
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_rlSetDynChirpEn(void)
{
    int32_t retVal;
    rlDynChirpEnCfg_t dynChirpEnCfg = {0};

    /* Dynamic chirp enable */
    retVal = rlSetDynChirpEn(RL_DEVICE_MAP_INTERNAL_BSS, &dynChirpEnCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetDynChirpEn retVal=%d\n", retVal);
        return -1;
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Configure per chirp phase shifter dynamically
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_rlSetDynPerChirpPhShifterCfg(void)
{
    int32_t         retVal;
    rlAdvDynPerChirpPhShftCfg_t *data[32] ={(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[0],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[1],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[2],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[3],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[4],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[5],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[6],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[7],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[8],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[9],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[10],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[11],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[12],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[13],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[14],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[15],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[16],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[17],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[18],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[19],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[20],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[21],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[22],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[23],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[24],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[25],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[26],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[27],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[28],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[29],
                                            (rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[30],(rlAdvDynPerChirpPhShftCfg_t*)&dynChirpPhaseCfg[31]};

    /* Set Dynamic per chirp phase shifter configuration */
    retVal = rlSetAdvDynPerChirpPhShifterCfg(RL_DEVICE_MAP_INTERNAL_BSS, 32U, (rlAdvDynPerChirpPhShftCfg_t**)&data[0U]);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetAdvDynPerChirpPhShifterCfg retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished information related to rlSetAdvDynPerChirpPhShifterCfg configurations to BSS\n");
    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to set/send the Adc out configuration to the BSS.
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setAdcOutConfig (void)
{
    int32_t         retVal;

    /* Set ADC out configuration */
    retVal = rlSetAdcOutConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdcOutCfg_t*)&adcOutCfgArgs);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: setAdcOutConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished setAdcOutConfig to BSS\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set low power mode.
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setLowPowerModeConfig (void)
{
    int32_t         retVal;

    /* Set Low power mode configuration */
    retVal = rlSetLowPowerModeConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlLowPowerModeCfg_t*)&lowPowerModeCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: setLowPowerMode retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished setLowPowerMode\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test Set HSI clock API.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setHsiClk (void)
{
    int32_t     retVal;

    /* Set HSI clock */
    retVal = rlDeviceSetHsiClk(RL_DEVICE_MAP_INTERNAL_BSS, (rlDevHsiClk_t*)&deviceHsiClk);
    if (retVal != 0)
    {
        printf ("Error: Unable to set HSI clock [Error %d]\n", retVal);
        return -1;
    }
    printf("Debug: Set HSI clock successfully\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test RF Init/Calibration API.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_rfCalibration (void)
{
    int32_t     retVal;

    /* RF Initialization */
    retVal = rlRfInit(RL_DEVICE_MAP_INTERNAL_BSS);
    if (retVal != 0)
    {
        printf ("Error: Unable to start RF [Error %d]\n", retVal);
        return -1;
    }
    while(gInitTimeCalibStatus == 0U)
    {
        /* Sleep and poll again: */
        ClockP_usleep(1);
    }
    gInitTimeCalibStatus = 0U;
    printf("Debug: RF start successfully\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Profile configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setProfileConfig (void)
{
    int32_t         retVal;

    /* Set Profile configuration */
    retVal = rlSetProfileConfig(RL_DEVICE_MAP_INTERNAL_BSS, 7U, (rlProfileCfg_t*)&profileCfg[0U]);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
    printf("Error: rlSetProfileConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetProfileConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Dummy Profile configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setDummyProfileConfig (void)
{
    int32_t         retVal;

    /* Set Profile configuration */
    retVal = rlSetProfileConfig(RL_DEVICE_MAP_INTERNAL_BSS, 1U, (rlProfileCfg_t*)&dummyProfileCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
    printf("Error: Dummy profile configuration failure with retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished dummy profile configuration\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get Profile configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_getProfileConfig (void)
{
    int32_t         retVal, i;
    rlProfileCfg_t  getProfileData = { 0 };

    for(i = 0; i< 7U; i++)
    {
        /* Get profile configuration */
        retVal = rlGetProfileConfig(RL_DEVICE_MAP_INTERNAL_BSS, i, (rlProfileCfg_t*)&getProfileData);

        /* Check for mmWaveLink API call status */
        if(retVal != 0)
        {
            /* Error: Link reported an issue. */
            printf("Error: rlGetProfileConfig retVal=%d\n", retVal);
            return -1;
        }
        if((getProfileData.adcStartTimeConst != profileCfg[i].adcStartTimeConst) || \
           (getProfileData.digOutSampleRate != profileCfg[i].digOutSampleRate) || \
           (getProfileData.freqSlopeConst != profileCfg[i].freqSlopeConst) || \
           (getProfileData.hpfCornerFreq1 != profileCfg[i].hpfCornerFreq1) || \
           (getProfileData.hpfCornerFreq2 != profileCfg[i].hpfCornerFreq2) || \
           (getProfileData.idleTimeConst != profileCfg[i].idleTimeConst) || \
           (getProfileData.numAdcSamples != profileCfg[i].numAdcSamples) || \
           (getProfileData.profileId != profileCfg[i].profileId) || \
           (getProfileData.rampEndTime != profileCfg[i].rampEndTime) || \
           (getProfileData.rxGain != profileCfg[i].rxGain) || \
           (getProfileData.startFreqConst != profileCfg[i].startFreqConst) || \
           (getProfileData.txOutPowerBackoffCode != profileCfg[i].txOutPowerBackoffCode) || \
           (getProfileData.txPhaseShifter != profileCfg[i].txPhaseShifter) || \
           (getProfileData.txStartTime != profileCfg[i].txStartTime))
        {
            printf("Error: rlGetProfileConfig parameters not matched with configured parameters [%d]\n", retVal);
            retVal = -1;
        }
    }
    if(retVal == 0)
    {
        printf("Debug: rlGetProfileConfig parameters matched with configured parameters\n");
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Chirp configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setChirpConfig (void)
{
    int32_t         retVal;

    /* Set chirp configuration */
    retVal = rlSetChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, 2U, (rlChirpCfg_t*)&chirpCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetChirpConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetChirpConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Dummy Chirp configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setDummyChirpConfig (void)
{
    int32_t         retVal;

    /* Set chirp configuration */
    retVal = rlSetChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, 1U, (rlChirpCfg_t*)&dummyChirpCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: Dummy chirp configuration failure with retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished dummy chirp configuration\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Multiple Chirp configurations.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setMultiChirpConfig (void)
{
    int32_t         retVal;
    rlChirpCfg_t * multiChirpData[2] = {(rlChirpCfg_t*)&chirpCfg[0], (rlChirpCfg_t*)&chirpCfg[1]};

    /* Set multiple chirp configuration */
    retVal = rlSetMultiChirpCfg(RL_DEVICE_MAP_INTERNAL_BSS, 2U, &multiChirpData[0]);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetMultiChirpCfg retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetMultiChirpCfg\n");

    return 0;
}

rlChirpCfg_t getChirpCfg[10] = {0};
/**
 *  @b Description
 *  @n
 *      The function is used to Get Chirp configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_getChirpConfig (uint16_t startIdx, uint16_t endIdx)
{
    int32_t retVal;

    /* Get chirp configuration */
    retVal = rlGetChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, startIdx, endIdx, &getChirpCfg[0]);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlGetChirpConfig retVal=%d\n", retVal);
        return -1;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Get Chirp configuration.
 *      using the mmWave link API.
 *
 *  @param Configure type. 0 - Old configuration, 1 - New configuration
 *
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_getDynChirpConfig (void)
{
    int32_t retVal, i = 0,j = 0;
    uint16_t startIdx;

    if (dynChirpCfg[0].chirpRowSelect == 0x00)
    {
        startIdx = dynChirpCfg[0].chirpSegSel*16;
    }
    else
    {
        startIdx = dynChirpCfg[0].chirpSegSel*48;
    }

    retVal = MmwaveLink_getChirpConfig(startIdx, startIdx + 9);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlGetChirpConfig retVal=%d\n", retVal);
        return -1;
    }
    /* compare each chirpConfig parameters to lastly configured via rlDynChirpConfig API */
    while (i <= 9)
    {
        if (dynChirpCfg[0].chirpRowSelect == 0x00)
        {
            if ((getChirpCfg[i].profileId != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 4, 0)) || \
                (getChirpCfg[i].freqSlopeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 6, 8)) || \
                (getChirpCfg[i].txEnable != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 3, 16)) || \
                (getChirpCfg[i].startFreqVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR2, 23, 0)) || \
                (getChirpCfg[i].idleTimeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR3, 12, 0)) || \
                (getChirpCfg[i].adcStartTimeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR3, 12, 16)))
            {
                break;
            }
            i++;
        }
        else if (dynChirpCfg[0].chirpRowSelect == 0x10)
        {
            if ((getChirpCfg[i].profileId != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 4, 0)) || \
                (getChirpCfg[i].freqSlopeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 6, 8)) || \
                (getChirpCfg[i].txEnable != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 3, 16)))
            {
                break;
            }
            i++;
            if ((getChirpCfg[i].profileId != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR2, 4, 0)) || \
                (getChirpCfg[i].freqSlopeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR2, 6, 8)) || \
                (getChirpCfg[i].txEnable != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR2, 3, 16)))
            {
                break;
            }
            i++;
            if ((getChirpCfg[i].profileId != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR3, 4, 0)) || \
                (getChirpCfg[i].freqSlopeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR3, 6, 8)) || \
                (getChirpCfg[i].txEnable != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR3, 3, 16)))
            {
                break;
            }
            i++;
        }
        else if (dynChirpCfg[0].chirpRowSelect == 0x20)
        {
            if (getChirpCfg[i].startFreqVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 23, 0))
            {
                break;
            }
            i++;
            if (getChirpCfg[i].startFreqVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR2, 23, 0))
            {
                break;
            }
            i++;
            if (getChirpCfg[i].startFreqVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR3, 23, 0))
            {
                break;
            }
            i++;
        }
        else if (dynChirpCfg[0].chirpRowSelect == 0x30)
        {
            if ((getChirpCfg[i].idleTimeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 12, 0)) || \
                (getChirpCfg[i].adcStartTimeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR1, 12, 16)))
            {
                break;
            }
            i++;
            if ((getChirpCfg[i].idleTimeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR2, 12, 0)) || \
                (getChirpCfg[i].adcStartTimeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR2, 12, 16)))
            {
                break;
            }
            i++;
            if ((getChirpCfg[i].idleTimeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR3, 12, 0)) || \
                (getChirpCfg[i].adcStartTimeVar != GET_BIT_VALUE(dynChirpCfg[0].chirpRow[j].chirpNR3, 12, 16)))
            {
                break;
            }
            i++;
        }
        j++;
    }
    if (i <= 9)
    {
        retVal = -1;
        printf("Error: Parameters are mismatched GetChirpConfig compare to dynChirpConfig\n");
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Frame configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setFrameConfig (bool useInputNumFrames, rlUInt16_t numFrames)
{
    int32_t         retVal;
    rlFrameCfg_t    localFrameCfg = frameCfg;

    if(useInputNumFrames)
    {
        localFrameCfg.numFrames = numFrames;
    }

    /* Set frame configuration */
    retVal = rlSetFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &localFrameCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetFrameConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetFrameConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Dummy Frame configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setDummyFrameConfig (bool useInputNumFrames, rlUInt16_t numFrames)
{
    int32_t         retVal;
    rlFrameCfg_t    localFrameCfg = dummyFrameCfg;

    if(useInputNumFrames)
    {
        localFrameCfg.numFrames = numFrames;
    }

    /* Set frame configuration */
    retVal = rlSetFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &localFrameCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: Dummy frame configuration failure with retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished dummy frame configuration\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get Frame configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_getFrameConfig (void)
{
    int32_t         retVal;
    rlFrameCfg_t    frameData = { 0 };
    /* Get Frame configuration */
    retVal = rlGetFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &frameData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlGetFrameConfig retVal=%d\n", retVal);
        return -1;
    }

    if((frameData.chirpEndIdx != frameCfg.chirpEndIdx) || \
       (frameData.chirpStartIdx != frameCfg.chirpStartIdx) || \
       (frameData.framePeriodicity != frameCfg.framePeriodicity) || \
       (frameData.frameTriggerDelay != frameCfg.frameTriggerDelay) || \
       (frameData.numFrames != frameCfg.numFrames) || \
       (frameData.numLoops != frameCfg.numLoops) || \
       (frameData.triggerSelect != frameCfg.triggerSelect))
    {
        printf("Error: rlGetFrameConfig parameters not matched with configured parameters [%d]\n", retVal);
        retVal = -1;
    }
    if(retVal == 0)
    {
        printf("Debug: rlGetFrameConfig parameters matched wilh configured parameters [%d]\n", retVal);
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to get Advance Frame configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_getAdvFrameConfig (void)
{
    int32_t            retVal;
    rlAdvFrameCfg_t    advFrameData = { 0 };
    /* Get Advance frame configuration */
    retVal = rlGetAdvFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &advFrameData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlGetAdvFrameConfig retVal=%d\n", retVal);
        return -1;
    }

    if((advFrameData.frameSeq.forceProfile != advFrameCfg.frameSeq.forceProfile) || \
       (advFrameData.frameSeq.frameTrigDelay != advFrameCfg.frameSeq.frameTrigDelay) || \
       (advFrameData.frameSeq.numFrames != advFrameCfg.frameSeq.numFrames) || \
       (advFrameData.frameSeq.numOfSubFrames != advFrameCfg.frameSeq.numOfSubFrames))
    {
        printf("Error: rlGetAdvFrameConfig parameters not matched with configured parameters [%d]\n", retVal);
        retVal = -1;
    }
    if(retVal == 0)
    {
        printf("Debug: rlGetAdvFrameConfig parameters matched wilh configured parameters [%d]\n", retVal);
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test start sensor API.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_sensorStart (void)
{
    int32_t     retVal;

    /* Trigger the frame */
    retVal = rlSensorStart(RL_DEVICE_MAP_INTERNAL_BSS);
    if (retVal != 0)
    {
        printf ("Error: Unable to start Sensor [Error %d]\n", retVal);
        return -1;
    }
    /* if frame start async event is enable by rlRfSetDeviceCfg API,
       then wait for Frame start async event */
    if ((rfDevCfg.aeControl & 0x1) == 0x0)
    {
        while(gFrameStartStatus == 0U)
        {
            /* Sleep and poll again: */
            ClockP_usleep(1);
        }
    }
    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to test stop sensor API.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_sensorStop (void)
{
    int32_t     retVal;

    /* Stop the frame */
    retVal = rlSensorStop(RL_DEVICE_MAP_INTERNAL_BSS);
    if (retVal != 0)
    {
        if(retVal == RL_RET_CODE_FRAME_ALREADY_ENDED)
        {
            printf ("Debug: Frames are already stopped  [%d]\n", retVal);
            return 0;
        }
        printf ("Error: Unable to stop Sensor [Error %d]\n", retVal);
        return -1;
    }

    /* if frame stop async event is enable by rlRfSetDeviceCfg API,
        then wait for Frame stop async event */
    if ((rfDevCfg.aeControl & 0x2) == 0x0)
    {
        while(gFrameStartStatus == 1U)
        {
            /* Sleep and poll again: */
            ClockP_usleep(1);
        }
    }
    printf("Debug: Sensor stop successfully\n");

    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to set advance Frame configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setAdvFrameConfig (void)
{
    int32_t         retVal;

    /* Set advance frame configuration */
    retVal = rlSetAdvFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvFrameCfg_t*)&advFrameCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetAdvFrameConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetAdvFrameConfig\n");

    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to set Binary Phase Modulation Common configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setBpmCommonConfig (void)
{
    int32_t         retVal;

    /* Set BPM common configuration */
    retVal = rlSetBpmCommonConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlBpmCommonCfg_t*)&bpmCommonCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
    printf("Error: rlSetBpmCommonConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetBpmCommonConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Binary Phase Modulation Chirp configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setBpmChirpConfig (void)
{
    int32_t         retVal;
    rlBpmChirpCfg_t *bpmCfg[3] = {(rlBpmChirpCfg_t*)&bpmChirpCfg[0U],
                    (rlBpmChirpCfg_t*)&bpmChirpCfg[1U], (rlBpmChirpCfg_t*)&bpmChirpCfg[2U]};

    /* Set BPM chirp configuration */
    retVal = rlSetBpmChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlBpmChirpCfg_t*)&bpmChirpCfg[0U]);

    /* configure multiple BPM configuration via one API call */
    retVal = rlSetMultiBpmChirpConfig(RL_DEVICE_MAP_INTERNAL_BSS, 3U, &bpmCfg[0U]);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetBpmChirpConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetBpmChirpConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Continous mode Configuration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setContModeConfig (void)
{
    int32_t         retVal;

    /* Set continue mode configuration */
    retVal = rlSetContModeConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlContModeCfg_t*)&contModeCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
    printf("Error: rlSetContModeConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetContModeConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Enable/Disable Continous mode
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_enableContMode (uint8_t bEnable)
{
    int32_t         retVal;
    rlContModeEn_t contModeEnable;

    contModeEnable.contModeEn = bEnable;

    /* Enable Continous mode */
    retVal = rlEnableContMode(RL_DEVICE_MAP_INTERNAL_BSS, (rlContModeEn_t*)&contModeEnable);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlEnableContMode retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlEnableContMode\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Configure dynamic power saving feature
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_rfDynamicPowerSave (void)
{
    int32_t         retVal;

    /* Set Dynamic power save configuration */
    retVal = rlRfDynamicPowerSave(RL_DEVICE_MAP_INTERNAL_BSS, (rlDynPwrSave_t*)&dynPwrSave);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
    printf("Error: rlRfDynamicPowerSave retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlRfDynamicPowerSave\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Configure asynchronous event direction for device
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfDevCfg (rlUInt32_t dirData)
{
    int32_t         retVal;
    rfDevCfg.aeDirection = dirData; /* 0xa: BSS to DSS, 0: BSS to MSS */

    /* Set Device configuration */
    retVal = rlRfSetDeviceCfg(RL_DEVICE_MAP_INTERNAL_BSS, &rfDevCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfSetDeviceCfg retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlRfSetDeviceCfg\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Configure GP ADC data parameters
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setGpAdcConfig (void)
{
    int32_t         retVal;

    /* Set GPADC configuration */
    retVal = rlSetGpAdcConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlGpAdcCfg_t*)&gpAdcCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetGpAdcConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlSetGpAdcConfig\n");

    while(isGetGpAdcMeasData == 0U)
    {
        /* Sleep and poll again: */
        ClockP_usleep(1);
    }
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Enables/Disables LDO bypass mode
 *      using the mmWave link API.
 *  @Note - This API is required to bypass LDO for specific board/PMIC configuration.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_rfSetLdoBypassConfig (void)
{
    int32_t         retVal;

    /* Set LDO bypass configuration */
    retVal = rlRfSetLdoBypassConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlRfLdoBypassCfg_t*)&rfLdoBypassCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
    printf("Error: rlRfSetLdoBypassConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlRfSetLdoBypassConfig\n");

    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to Enable External Programmable filter
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_progFilterConfig (void)
{
    int32_t         retVal;

    /* Set Programmable filter RAM coeffecients */
    retVal = rlRfSetProgFiltCoeffRam(RL_DEVICE_MAP_INTERNAL_BSS, (rlRfProgFiltCoeff_t*)&progFiltCoef);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfSetProgFiltCoeffRam retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlRfSetProgFiltCoeffRam\n");

    /* Set Programmable filter configuration */
    retVal = rlRfSetProgFiltConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlRfProgFiltConf_t*)&progFiltCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfSetProgFiltConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlRfSetProgFiltConfig\n");


    return 0;
}

int32_t mmwaveLink_triggerSubframe (void)
{
    int32_t         retVal;

    /* trigger subframe */
    retVal = rlSetSubFrameStart(RL_DEVICE_MAP_INTERNAL_BSS, (rlSubFrameStartCfg_t*)&subFrameTrigCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetSubFrameStart retVal=%d\n", retVal);
        return -1;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Enable Misc feature such as per chirp phase shifter
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_miscConfig(void)
{
    int32_t         retVal;

    /* Set RF Misc configuration */
    retVal = rlRfSetMiscConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlRfMiscConf_t*)&miscCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfSetMiscConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlRfSetMiscConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Configure static per chirp phase shifter
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_perChirpPhaseShifterConfig(void)
{
    int32_t         retVal;

    /* Set Phase shifter configuration */
    retVal = rlRfSetPhaseShiftConfig(RL_DEVICE_MAP_INTERNAL_BSS, 2U, (rlRfPhaseShiftCfg_t*)&phaseShiftCfg[0]);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfSetPhaseShiftConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlRfSetPhaseShiftConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to induce different gain/phase offsets on the different RXs, for
 *      inter-RX mismatch compensation using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_interRxGainConfig(void)
{
    int32_t         retVal;

    /* Set Inter RX Gain phase configuration */
    retVal = rlRfInterRxGainPhaseConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlInterRxGainPhConf_t*)&interRxGainPhCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfInterRxGainPhaseConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished rlRfInterRxGainPhaseConfig\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to reads Temperature information from all temperature
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_getTemperatureReport(void)
{
    int32_t         retVal;
    rlRfTempData_t tempData = { 0 };

    /* Get Temerature report */
    retVal = rlRfGetTemperatureReport(RL_DEVICE_MAP_INTERNAL_BSS, (rlRfTempData_t*)&tempData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfGetTemperatureReport retVal=%d\n", retVal);
        return -1;
    }
    /* Display the temperature readings */
    printf ("Debug : Temerature readings\n");
    printf ("RX0 : %3d degree C, RX1 : %3d degree C, RX2 : %3d degree C, RX3 : %3d degree C\n",
                   tempData.tmpRx0Sens, tempData.tmpRx1Sens, tempData.tmpRx2Sens, tempData.tmpRx3Sens);
    printf ("TX0 : %3d degree C, TX1 : %3d degree C, TX2 : %3d degree C, TX3 : %3d degree C\n",
                   tempData.tmpTx0Sens, tempData.tmpTx1Sens, tempData.tmpTx2Sens,tempData.tmpDig0Sens);
    printf ("PM  : %3d degree C ",
                   tempData.tmpPmSens);

    printf ("\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure Test Source
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_testSourceConfig(void)
{
    int32_t         retVal;

    /* Set Test source configuration */
    retVal = rlSetTestSourceConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlTestSource_t*)&testSourceCfg);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlSetTestSourceConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlSetTestSourceConfig\n");

    retVal = rlTestSourceEnable(RL_DEVICE_MAP_INTERNAL_BSS, (rlTestSourceEnable_t*)&testSourceEn);
        if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTestSourceEnable retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTestSourceEnable\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to read Die ID Status
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_getDieIdStaus(void)
{

  int32_t         retVal;
    rlRfDieIdCfg_t dieIdStatus = { 0 };

    /* Get Device DIE ID */
    retVal = rlGetRfDieId(RL_DEVICE_MAP_INTERNAL_BSS, &dieIdStatus);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlGetRfdieId retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Die ID Status\n");
    printf("Val0 : %d, Val1: %d, Val2: %d, Val3: %d\n",
                  dieIdStatus.dieIDHexVal0, dieIdStatus.dieIDHexVal1, dieIdStatus.dieIDHexVal2, dieIdStatus.dieIDHexVal3);

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to RX Gain Temerature LUT Configuration
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_RxGainTempLutConfig(void)
{
    int32_t         retVal;

    /* Set RX Gain LUT */
    retVal = rlRxGainTempLutSet(RL_DEVICE_MAP_INTERNAL_BSS, (rlRxGainTempLutData_t*)&rxGainTempLutData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRxGainTempLutSet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlRxGainTempLutSet\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to read RX Gain Temerature LUT data
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_RxGainTempLutRead(void)
{
    int32_t         retVal;

    /* Get RX Gain LUT */
    retVal = rlRxGainTempLutGet(RL_DEVICE_MAP_INTERNAL_BSS, (rlRxGainTempLutReadReq_t*)&rxGainTempLutReadCfg,
                                     (rlRxGainTempLutData_t*)&rxGainTempLutData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRxGainTempLutGet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlRxGainTempLutGet\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to TX Gain Temerature LUT Configuration
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_TxGainTempLutConfig(void)
{
    int32_t         retVal;

    /* Set TX Gain LUT */
    retVal = rlAdvTxGainTempLutSet(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvTxGainTempLutData_t*)&txGainTempLutData0);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTxGainTempLutSet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTxGainTempLutSet\n");

    /* Set TX Gain LUT */
    retVal = rlAdvTxGainTempLutSet(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvTxGainTempLutData_t*)&txGainTempLutData1);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTxGainTempLutSet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTxGainTempLutSet\n");

    /* Set TX Gain LUT */
    retVal = rlAdvTxGainTempLutSet(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvTxGainTempLutData_t*)&txGainTempLutData2);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTxGainTempLutSet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTxGainTempLutSet\n");

    /* Set TX Gain LUT */
    retVal = rlAdvTxGainTempLutSet(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvTxGainTempLutData_t*)&txGainTempLutData3);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTxGainTempLutSet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTxGainTempLutSet\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to read TX Gain Temerature LUT data
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_TxGainTempLutRead(void)
{
    int32_t         retVal;

    /* Get TX Gain LUT */
    retVal = rlAdvTxGainTempLutGet(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvTxGainTempLutReadReq_t*)&txGainTempLutReadCfg0,
                                                            &txGainTempLutData0);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTxGainTempLutGet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTxGainTempLutGet\n");

    /* Get TX Gain LUT */
    retVal = rlAdvTxGainTempLutGet(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvTxGainTempLutReadReq_t*)&txGainTempLutReadCfg1,
                                                            &txGainTempLutData1);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTxGainTempLutGet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTxGainTempLutGet\n");

    /* Get TX Gain LUT */
    retVal = rlAdvTxGainTempLutGet(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvTxGainTempLutReadReq_t*)&txGainTempLutReadCfg2,
                                                            &txGainTempLutData2);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTxGainTempLutGet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTxGainTempLutGet\n");

    /* Get TX Gain LUT */
    retVal = rlAdvTxGainTempLutGet(RL_DEVICE_MAP_INTERNAL_BSS, (rlAdvTxGainTempLutReadReq_t*)&txGainTempLutReadCfg3,
                                                            &txGainTempLutData3);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlTxGainTempLutGet retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlTxGainTempLutGet\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to read Calibration Data from device
 *      using the mmWave link API.
 *
 *  @Note : It is suggested to store this calibration data to sFlash connected with device.
 *          then later reload this calibration data in the field using rlRfCalibDataRestore.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_calibDataStore(void)
{
    int32_t         retVal;

    /* Store calibration data */
    retVal = rlRfCalibDataStore(RL_DEVICE_MAP_INTERNAL_BSS, &calibData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfCalibDataStore retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlRfCalibDataStore\n");

    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to configure link CRC type.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_configLinkCrc(rlCrcType_t    crcType)
{
    int32_t         retVal;

    retVal = rlDeviceConfigureCrc(crcType);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlDeviceConfigureCrc=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlDeviceConfigureCrc\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to configure link ACK timeout value.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_configAckTimeout(rlUInt32_t ackTimeout)
{
    int32_t         retVal;

    retVal = rlDeviceConfigureAckTimeout(ackTimeout);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlDeviceConfigureAckTimeout=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlDeviceConfigureAckTimeout\n");

    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to write Calibration Data to device using the mmWave link API
 *      which was stored previously using mmwaveLink_calibDataStore.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_calibDataRestore(void)
{
    int32_t         retVal;

    /* Restore calibration data */
    retVal = rlRfCalibDataRestore(RL_DEVICE_MAP_INTERNAL_BSS, &calibData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfCalibDataRestore retVal=%d\n", retVal);
        return -1;
    }
    while(gInitTimeCalibStatus == 0U)
    {
        /* Sleep and poll again: */
        ClockP_usleep(1);
    }
    gInitTimeCalibStatus = 0U;
    printf("Debug: Finished rlRfCalibDataRestore\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to read Phase Shifter Calibration Data from device
 *      using the mmWave link API.
 *
 * @Note: For the actual application user may need to store this (factory) calibration [phShiftCalibData]
 *   to sFlash connected with device and in the application flow, it needs to read that calibration data
 *   [phShiftCalibData] from sFlash and reload to BSS/RadarSS via rlRfPhShiftCalibDataRestore.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_phShiftCalibDataStore(void)
{
    int32_t         retVal;
    phShiftCalibData.PhShiftcalibChunk[0].txIndex = 0x0;
    phShiftCalibData.PhShiftcalibChunk[1].txIndex = 0x1;
    phShiftCalibData.PhShiftcalibChunk[2].txIndex = 0x2;
#if defined(SOC_AWR294X) || defined(SOC_AWR2544) || defined(SOC_AWR2X44P)
    phShiftCalibData.PhShiftcalibChunk[3].txIndex = 0x3;
#endif

    /* Store Phase shifter calibration data */
    retVal = rlRfPhShiftCalibDataStore(RL_DEVICE_MAP_INTERNAL_BSS, &phShiftCalibData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfPhShiftCalibDataStore retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlRfPhShiftCalibDataStore\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to write Phase Shifter Calibration Data to device using the
 *      mmWave link API which was stored previously using mmwaveLink_phShiftCalibDataStore.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_phShiftCalibDataRestore(void)
{
    int32_t         retVal;

    /* Restore Phase shifter calibration data */
    retVal = rlRfPhShiftCalibDataRestore(RL_DEVICE_MAP_INTERNAL_BSS, &phShiftCalibData);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfPhShiftCalibDataRestore retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlRfPhShiftCalibDataRestore\n");

    return 0;
}
/**
 *  @b Description
 *  @n
 *      The function is used to get RF Digital Front End Statistics
 *      using mmwaveLink_phShiftCalibDataStore.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_getRealChDfeRxStatisticsReport(void)
{
    int32_t         retVal;
    rlRealChDfeStatReport_t RealChdfeStatReport = { 0 };

    /* Get RX DFE statistics report */
    retVal = rlRfRealChDfeRxStatisticsReport(RL_DEVICE_MAP_INTERNAL_BSS, &RealChdfeStatReport);

    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfDfeRxStatisticsReport retVal=%d\n", retVal);
        return -1;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to test analog fault monitoring system
 *      using rlRfAnaFaultInjConfig API from rl_monitoring.c
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_anaFaultInjection(rlAnaFaultInj_t anaFaultCfg)
{
    int32_t retVal;
    rlAnaFaultInj_t anaFaultConfig = anaFaultCfg;

    // anaFaultConfig.rxGainDrop = 0x1U;
    retVal = rlRfAnaFaultInjConfig(RL_DEVICE_MAP_INTERNAL_BSS, &anaFaultConfig);

    if(retVal != 0){
        printf("Error: rlRfAnaFaultInjConfig retVal=%d\n", retVal);
        return -1;
    }

    return 0;
}
