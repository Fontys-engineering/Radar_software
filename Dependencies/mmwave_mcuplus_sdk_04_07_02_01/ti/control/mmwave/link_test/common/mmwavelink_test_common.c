/*
 *   @file  mmwavelink_test_common.c
 *
 *   @brief
 *      Unit Test common code for the mmWave Link
 *
 *  \par
 *  NOTE:
 *      (C) Copyright 2019 Texas Instruments, Inc.
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

/* mmWave PDK Include Files: */
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/CycleCounterP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/SemaphoreP.h>
#include "FreeRTOS.h"
#include "task.h"

/* mmWave SDK Include Files: */
#include <ti/common/syscommon.h>
#include <ti/control/mmwavelink/mmwavelink.h>
#include <drivers/crc.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/utils/testlogger/logger.h>
#include <ti/control/mmwave/link_test/common/mmwavelink_test_common.h>
#include <ti/control/mmwave/link_test/common/link_testcase_setting.h>
#include <ti/control/mmwave/link_test/common/link_test.h>
#include <ti/control/mmwave/link_test/common/link_test_monitoring.h>

/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/
/*RF Monitoring test, this doesn't include cq & cp test*/
extern bool gLinkMonitoringTest;

/*RF calibration test*/
extern bool gLinkCalibTest;

/* RF Advance frame test
   Note: If gLinkContModeTest is enabled then gLinkMonitoringTest and gLinkCalibTest must be disabled
*/
extern bool gLinkContModeTest;

/*RF Advance frame test*/
extern bool gLinkAdvanceFrameTest;

/*Advance Chirp Configuration test*/
extern bool gAdvChirpTest ;

/* Dynaic Chirp configuration test */
extern bool gDynamicChirpTest;

/*Binary phase modulation test*/
extern bool gLinkBpmEnableTest;

/*RF program filter test*/
extern bool gLinkProgFiltTest;

/* Sub frame trigger test. Valid when gLinkAdvanceFrameTest flag is enabled */
extern bool gLinkSubFrameTrigTest;

/*CQ2 data extraction and verification test
    Here is the the flow to verify the CQ2 RAM data
   1. For every chirp the application triggers eDMA to transfer CQ2
      data of that chirp to L3 memory.
   2. CQ2 data verification is not covered in this application, but user can verify this by putting another
      EVM running countinous mode of frequency range which cause interference on EVM under test. In case
      of any interference recieved on EVM under test, CQ2 data will have non-zero values.
*/
/* enable this flag to verify CQ data in this application */
extern bool cqDataVerifyTest;

/*enable this variable to define mutiple chirps*/
extern bool multichirpen;

/* enable this variable to use the test source*/
extern bool testsrcen;

/* enable this variable for analog fault injection testing */
extern bool gAnalogFaultInjTest;

/* enable this variable for digital latent fault testing */
extern bool gDigLatentFaultTest;


#ifdef SOC_AWR294X
extern volatile uint8_t eDMADataTransferDone;
#endif

/*frame interrupt Semaphore  */
extern SemaphoreP_Object gLinkFrameSem;

/*Frame Configuration*/
extern rlFrameCfg_t  frameCfg;

/* Advance Frame Configuration */
extern rlAdvFrameCfg_t  advFrameCfg;

extern monitoringDataAe_t gMonDataAeStrct;

/* Array to hold average chirp interval for Dynamic chirps*/
uint32_t avgChirpTimeInterval[2];
uint32_t lastChirpIntTimeStamp;

/* Variables to capture time stamp during Adv Config*/
uint32_t AdvChirptimeStamp1;
uint32_t AdvChirptimeStamp12;
uint32_t AdvChirptimeStamp2;
uint32_t AdvChirptimeStamp21;

/*Counter for number of chirps*/
volatile uint32_t gLinkChirpCnt = 0U;

/*Counter for number of frames*/
volatile uint32_t gLinkFrameCnt = 0U;

volatile uint32_t dynChirpTestStatus = 0;

/* Store CQ verification test result */
int32_t gCqTestResult = 0xFF;
/* Store CP data verification result */
int32_t gCpTestResult  = 0xFF;

/* Dynamic Chirp configuration Number, Initially 0, After Dynamic chirp configuration, Set to 1*/
uint8_t gDynChirpConfigNum = 0U;

extern uint32_t gInitTimeCalibStatus;
int32_t retVal;

/* set to True if chirp interrupt is missed in a whole frame */
bool chirpMissedError = false;

/* fetching frame count and periodicity from configuration */
int32_t framePeriodicity = 0;
int32_t frameCount = 0;
int32_t chirpCount = 0;
uint32_t activFrameTime = 0;
rlAdvFrameCfg_t AdvframeCfgArgs = { 0 };

// LUT related definations for AdvChirpConfig
rlUInt16_t lutOffsetInNBytes = 0;

/* Advanced Chirp LUT data */
/* Max size of the LUT is 12KB.
   Maximum of 212 bytes per chunk can be present per SPI message. */
/* This array is created to store the LUT RAM values from the user programmed parameters or config file.
   The populated array is sent over SPI to populate the RadarSS LUT RAM.
 * The chirp paramters start address offset should be 4 byte aligned */
rlInt8_t AdvChirpLUTData[LUT_ADVCHIRP_TABLE_SIZE] = { 0 };
/**************************************************************************
 ************************** mmWave Link Functions *************************
 **************************************************************************/

void PRINT_LOG(const char* format, ...)
{
    va_list     arg;
    char        logMessage[256] = {0};
    //int32_t     sizeMessage;

    /* Format the message: */
    va_start (arg, format);
    //sizeMessage = 
    vsnprintf (&logMessage[0], sizeof(logMessage), format, arg);
    va_end (arg);

    printf("%s",&logMessage[0]);
    
}

int32_t MmwaveLink_subframeStart()
{
        uint32_t i;
        int32_t  result = 0;
        volatile uint16_t loop_cnt = 0, sFrameCnt = 0;
        uint32_t noSubFramesPerFrame = advFrameCfg.frameSeq.numOfSubFrames;
        uint16_t triggeredSubFrameCnt = 0;

        printf ("----------------------------------------------------\n");
        /* mmWave Link set Advance Frame configuration */
        if (MmwaveLink_setAdvFrameConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Advance Frame config Fail\n");
            return -1;
        }
        else
        {
             PRINT_LOG ("Set mmWave Link Advance Frame config Pass\n");
        }
        while(loop_cnt < 2)
        {
            /* mmWave Link start sensor */
            if (MmwaveLink_sensorStart() < 0)
            {
                PRINT_LOG ("Start sensor Fail\n");
                return -1;
            }
            else
            {
                PRINT_LOG ("Start sensor Pass\n");
            }

            for(i = 1; i <= noSubFramesPerFrame*4 ; i++)
            {
                if (mmwaveLink_triggerSubframe() < 0)
                {
                    PRINT_LOG ("Subframe Trigger  Fail\n");
                    result = -1;
                    break;
                }
                else
                {
                    triggeredSubFrameCnt++;
                }
                while(gLinkChirpCnt != advFrameCfg.frameSeq.subFrameCfg[advFrameCfg.frameSeq.numOfSubFrames-1U].numOfChirps);
                sFrameCnt++;
                if (i < noSubFramesPerFrame*4)
                {
                    ClockP_usleep(500); /* don't trigger next sub frame, until current subframe sent out */
                }
            }
            loop_cnt++;
            if (MmwaveLink_sensorStop() < 0)
            {
                PRINT_LOG ("Sensor Stop Fail\n");
            }
            PRINT_LOG ("Sensor Stop Pass\n");

            if(result < 0)
            {
                return -1;
            }
        }
        printf(" %d subframes triggerd \n",triggeredSubFrameCnt);

        return 0;
}


/**
 *  @b Description
 *  @n
 *      System Initialization Task which initializes the various
 *      components in the system.
 *
 *  @retval
 *      Not Applicable.
 */
void MmwaveLink_startTest(uint8_t platformType, uint8_t platformCore)
{
    /* Status variables */
    int32_t resultSensorStart = -1;
    int32_t resultDynamicChirpCfg = -1;
    int32_t resultDynamicChirpEnable = -1;
    int32_t resultMonitoringReport = -1;
    int32_t resultAdvChirpCfg=-1;
    
    if(gLinkAdvanceFrameTest == true && gAdvChirpTest==true)
    {
        /* for advFrameCfg frame count is total number of subframes */
        frameCount = (advFrameCfg.frameSeq.numFrames)*(advFrameCfg.frameSeq.numOfSubFrames);
        framePeriodicity = ((advFrameCfg.frameSeq.subFrameCfg[0].subFramePeriodicity)*5)/1000000;
        chirpCount = chirpCount + advFrameCfg.frameSeq.subFrameCfg[0].numOfChirps;
    }
    else
    {
        frameCount =frameCfg.numFrames;
        framePeriodicity = (FRAME_PRIODICITY_VAL*5)/1000000;
        chirpCount = (frameCfg.chirpEndIdx - frameCfg.chirpStartIdx + 1U) * frameCfg.numLoops;
    }

 /*Configure the EDMA if CQ2 test is enabled*/
    if (cqDataVerifyTest)
    {   
        /* setup edma to copy CQ2 RAM data to L3 memory */
        if(MmwaveLink_CQedmaSetup() < 0)
        {
            PRINT_LOG ("mmWave Link CQ EDMA setup Fail\n");
            return;
        }
    }

    /*Configure EDMA if ADC data trasfer to L3 memory is required*/
    

    /* Setup and initialize the mmWave Link: */
    if (MmwaveLink_initLink (platformType, platformCore ) < 0)
    {
        PRINT_LOG ("mmWave Link Initialization Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link Initialization Pass\n");

    /*Get Rf version*/
    rlFwVersionParam_t rfVersionData = { 0 };
    if (rlDeviceGetRfVersion(RL_DEVICE_MAP_INTERNAL_BSS, &rfVersionData) < 0)
    {
        PRINT_LOG ("Get Rf version status Fail\n");
        return ;
    }
    PRINT_LOG ("Get Rf version status Pass \n");

    /* Get radarSS bootup status */
    if (MmwaveLink_getRfBootupStatus () < 0)
    {
        PRINT_LOG ("Get radarSS bootup status Fail\n");
        return ;
    }
    PRINT_LOG ("Get radarSS bootup status Pass\n");

 #ifdef SUBSYS_DSS
    /* mmWave Link set Async event configuration */
    if (MmwaveLink_setRfDevCfg(0xA) < 0)
    {
        PRINT_LOG ("Set Async event  status Fail\n");
        return;
    }
    PRINT_LOG ("Set Async event config status Pass\n");
 #else
    /* mmWave Link set Async event configuration */
    if (MmwaveLink_setRfDevCfg(0x0) < 0)
    {
        PRINT_LOG ("Set Async event config Fail\n");
        return;
    }
    PRINT_LOG ("Set Async event config status Pass\n");
 #endif

    /* Set device HSI clock */
    if (MmwaveLink_setHsiClk() < 0)
    {
        PRINT_LOG ("Set HSI clock Fail\n");
        return;
    }
    PRINT_LOG ("Set HSI clock status Pass\n");

    //Use any one of the three below to configure AckTimeout
    /* Set device ACK timeout to min value */
    if (mmwaveLink_configAckTimeout(ACK_TIMEOUT_MIN_VAL) < 0)
    {
        PRINT_LOG ("Set ACK timeout to min value Fail\n");
        return;
    }
    PRINT_LOG ("Set ACK timeout to min value status Pass\n");

    /* Set device ACK timeout to max value */
    if (mmwaveLink_configAckTimeout(ACK_TIMEOUT_MAX_VAL) < 0)
    {
        PRINT_LOG ("Set ACK timeout to max value Fail\n");
        return;
    }
    PRINT_LOG ("Set ACK timeout to max value status Pass\n");

    /* Set device ACK timeout to any value beteen min and max */
    if (mmwaveLink_configAckTimeout(ACK_TIMEOUT_VAL) < 0)
    {
        PRINT_LOG ("Set ACK timeout Fail\n");
        return;
    }
    PRINT_LOG ("Set ACK timeout status Pass\n");

    /* Get Device Die ID Status */
    if (mmwaveLink_getDieIdStaus() < 0)
    {
        PRINT_LOG ("Get Device Die ID Status Fail\n");
        return;
    }
    PRINT_LOG ("Get Device Die ID Status Pass\n");

    /* Set mmWave Link Channel Configuration to the BSS */
    if (MmwaveLink_setChannelConfig(false, 0) < 0)
    {
        PRINT_LOG ("Set mmWave Link Channel Config Fail\n");
        return;
    }
    PRINT_LOG ("Set mmWave Link Channel Config Pass\n");

    /* Set mmWave Link ADC Out Configuration to the BSS */
    if (MmwaveLink_setAdcOutConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link ADC Out Config Fail\n");
        return;
    }
    PRINT_LOG ("Set mmWave Link ADC Out Config Pass\n");

    /* Set mmWave Link low power mode Configuration to the BSS */ 
    if((platformType == RL_AR_DEVICETYPE_16XX) || (platformType == RL_AR_DEVICETYPE_14XX) || (platformType == RL_AR_DEVICETYPE_294X) || (platformType == RL_AR_DEVICETYPE_2544))
    {
        /* Set mmWave Link low power mode config*/
        if (MmwaveLink_setLowPowerModeConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Low Power Mode config Fail\n");
            return;
        }
        PRINT_LOG ("Set mmWave Link Low Power Mode config Pass\n");
    }
    #ifdef SOC_AWR294X
    /* configure AdcBuffer based on sample size and rx-channels */
    MmwaveLink_adcBufConfig();
    #endif

    /*For CQ2 Data format*/
    if (cqDataVerifyTest)
    {
        if(MMwaveLink_AdcBufConfigCQ()<0)
        {
            PRINT_LOG ("Set mmWave Link ADC Out Config Fail\n");
            return;
        }
         PRINT_LOG ("Set mmWave Link ADCBuf Config Pass\n");
    }
   

    /* mmWave Link Enable/Disable Calibration */
    if (MmwaveLink_setInitTimeCalibConfig(1, platformType) < 0)
    {
        PRINT_LOG ("mmWave Link Enable/Disable Calibration Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link Enable/Disable Calibration Pass\n");

    /* mmWave Link RF Init/Calibration */
    if (MmwaveLink_rfCalibration() < 0)
    {
        PRINT_LOG ("mmWave Link RF Calibration Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link RF Calibration Pass\n");

    /* mmWave Link Calibration Data Store configuration */
    if (mmwaveLink_calibDataStore() < 0)
    {
        PRINT_LOG ("mmWave Link Calibration Data Store config Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link Calibration Data Store config Pass\n");

    if(((platformType == RL_AR_DEVICETYPE_18XX) && (!gLinkBpmEnableTest)) || (platformType == RL_AR_DEVICETYPE_68XX) || ((platformType == RL_AR_DEVICETYPE_294X) && (!gLinkBpmEnableTest)) || ((platformType == RL_AR_DEVICETYPE_2544) && (!gLinkBpmEnableTest)))
    {
        /* mmWave Link Phase Shifter Calibration Data Store configuration */
        if (mmwaveLink_phShiftCalibDataStore() < 0)
        {
            PRINT_LOG ("mmWave Link Phase Shifter Calibration Data Store config Fail\n");
            return;
        }
        PRINT_LOG ("mmWave Link Phase Shifter Calibration Data Store config Pass\n");
    }

    /* mmWave Link Enable/Disable Calibration */
    if (MmwaveLink_setInitTimeCalibConfig(0, platformType) < 0)
    {
        PRINT_LOG ("mmWave Link Enable/Disable Calibration Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link Enable/Disable Calibration Pass\n");

    if(((platformType == RL_AR_DEVICETYPE_18XX) && (!gLinkBpmEnableTest)) || (platformType == RL_AR_DEVICETYPE_68XX)|| ((platformType == RL_AR_DEVICETYPE_294X) && (!gLinkBpmEnableTest)) || ((platformType == RL_AR_DEVICETYPE_2544) && (!gLinkBpmEnableTest)))
    {
        /* mmWave Link Phase Shifter Calibration Data Restore configuration */
        if (mmwaveLink_phShiftCalibDataRestore() < 0)
        {
            PRINT_LOG ("mmWave Link Phase Shifter Calibration Data Restore config Fail\n");
            return;
        }
        PRINT_LOG ("mmWave Link Phase Shifter Calibration Data Restore config Pass\n");
    }

    /* mmWave Link Calibration Data Restore configuration */
    if (mmwaveLink_calibDataRestore() < 0)
    {
        PRINT_LOG ("mmWave Link Calibration Data Restore config Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link Calibration Data Restore config Pass\n");

    /* mmWave Link RF Init/Calibration */
    if (MmwaveLink_rfCalibration() < 0)
    {
        PRINT_LOG ("mmWave Link RF Calibration Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link RF Calibration Pass\n");

    /* Set GPADC configuration */
    if (MmwaveLink_setGpAdcConfig() < 0)
    {
        PRINT_LOG ("mmWave Link Set GPAdc Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link Set GPAdc Pass\n");

    /* Set LDO bypass configuration */
    //LdoBypass not to be enabled for CQ2 data extraction and verification
    if (cqDataVerifyTest)
    {
        if (MmwaveLink_rfSetLdoBypassConfig() < 0)
        {
            PRINT_LOG ("mmWave Link Set LDO bypass Fail\n");
            return;
        }
        PRINT_LOG ("mmWave Link Set LDO bypass Pass\n");
    }
    /* mmWave Link Calib/Monitoring config */
    if (MmwaveLink_setCalMonConfig() < 0)
    {
        PRINT_LOG ("mmWave Link Calib/Monitoring config Fail\n");
        return;
    }
    PRINT_LOG ("mmWave Link Calib/Monitoring config Pass\n");

    /* Check for Programmable Filter Test */
    if(gLinkProgFiltTest == true) 
    {
        if (platformType != RL_AR_DEVICETYPE_68XX)
        {
            /* mmWave Link Programmable filter configuration */
            if (MmwaveLink_progFilterConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link Programmable filter config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Programmable filter config Pass\n");
        }
    }

    /* Analog Fault Injection Test */
    if(gAnalogFaultInjTest == true)
    {
        PRINT_LOG("Analog Fault Injection Test Started...\n");

        if(MmwaveLink_setDummyProfileConfig() < 0)
        {
            printf("Debug: Dummy Profile Configuration Failed\n");
            return;
        }
        printf("Debug: Dummy Profile Configuration Successful\n");
        
        if(MmwaveLink_setDummyChirpConfig() < 0)
        {
            printf("Debug: Dummy Chirp Configuration Failed\n");
            return;
        }
        printf("Debug: Dummy Chirp Configuration Successful\n");

        uint8_t faultIdx, retVal = 0;
        rlAnaFaultInj_t anaFaultCfg = {0};
        for(faultIdx=0; faultIdx<NUM_ANALOG_FAULT_TEST; faultIdx++)
        {
            switch(faultIdx)
            {
                case ANALOG_FAULT_RX_GAIN_DROP:
                    anaFaultCfg.rxGainDrop = 0xF;
                    retVal = MmwaveLink_setRfRxGainPhaMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_RX_PHASE_INV:
                    anaFaultCfg.rxPhInv = 0x1;
                    retVal = MmwaveLink_setRfRxGainPhaMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_RX_IF_STAGE:
                    anaFaultCfg.rxIfStagesFault = 0xF;
                    retVal = MmwaveLink_setRfRxIfStageMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_TX_GAIN_DROP:
                    anaFaultCfg.txGainDrop = 0xF;
                    retVal = MmwaveLink_setRfTxPowMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_TX_PHASE_INV:
                    anaFaultCfg.txPhInv = 0x51;
                    retVal = MmwaveLink_setRfAdvTxGainPhaseMismatchMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_SYNTH_FREQ:
                    anaFaultCfg.synthFault = 0x2;
                    retVal = MmwaveLink_setRfPllContrlVoltMonConfig() < 0 ? -1 : 0;
                    retVal = MmwaveLink_setRfSynthFreqMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_SUPPLY_LDO:
                    anaFaultCfg.supplyLdoFault = 0x1;
                    retVal = MmwaveLink_setRfPmClkLoIntAnaSignalsMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_MISC:
                    anaFaultCfg.miscFault = 0x1;
                    retVal = MmwaveLink_setRfDualClkCompMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_MISC_THRESHOLD:
                    anaFaultCfg.miscThreshFault = 0x3;
                    retVal = MmwaveLink_setRfGpadcIntAnaSignalsMonConfig() < 0 ? -1 : 0;
                    break;
                case ANALOG_FAULT_TX_PS_DAC:
                    anaFaultCfg.txPsDacFault = 0xF;
                    retVal = MmwaveLink_setRfTXPhShifterDacMonConfig() < 0 ? -1 : 0;
                    break;
            }

            if(retVal != 0)
            {
                printf("Debug: Analog Fault Monitor Configuration Failure with retVal = %d\n", retVal);
                return;
            }
            printf("Debug: Analog Fault Monitor Configuration Successful\n");

            retVal = mmwaveLink_anaFaultInjection(anaFaultCfg);
            if(retVal != 0)
            {
                PRINT_LOG("Set Analog Fault Injection Fail\n");
                return;
            }
            PRINT_LOG("Set Analog Fault Injection Pass\n");
            
            /* Enable Monitors */
            if (MmwaveLink_setRfAnaMonConfig(platformType, false, 0) < 0)
            {
                PRINT_LOG ("Set mmWave Link RF analog monitor Config\n", RESULT_FAIL);
                return;
            }

            if(MmwaveLink_setDummyFrameConfig(false, 0) < 0)
            {
                printf("Debug: Dummy Frame Configuration Failed\n");
                return;
            }
            printf("Debug: Dummy Frame Configuration Successful\n");

            /* Sensor Start */
            if (MmwaveLink_sensorStart() < 0)
            {
                PRINT_LOG ("Sensor Start Fail");
                return;
            }
            PRINT_LOG ("Sensor Start");

            /* Wait for X frames as per code requirment*/
            ClockP_usleep(framePeriodicity*1U*2);
            
            /* sensor Stop */
            if (MmwaveLink_sensorStop() < 0)
            {
                PRINT_LOG ("Sensor Stop Fail");
                return;
            }
            PRINT_LOG ("Sensor Stop");
            
            /* Print monitoring results */
            printf("\n----------------------------------------------------\n");
            switch(faultIdx)
            {
                case ANALOG_FAULT_RX_GAIN_DROP:
                    printf ("\nRL_RF_AE_MON_RX_GAIN_PHASE_REPORT [0x%x] \n", gMonDataAeStrct.monRxGainPhRep.statusFlags);
                    break;
                case ANALOG_FAULT_RX_PHASE_INV:
                    printf ("RL_RF_AE_MON_RX_GAIN_PHASE_REPORT [0x%x] \n", gMonDataAeStrct.monRxGainPhRep.statusFlags);
                    break;
                case ANALOG_FAULT_RX_IF_STAGE:
                    printf ("RL_RF_AE_MON_RX_IF_STAGE_REPORT [0x%x] \n", gMonDataAeStrct.monRxIfStageRep.statusFlags);
                    break;
                case ANALOG_FAULT_TX_GAIN_DROP:
                    printf ("RL_RF_AE_MON_TX0_POWER_REPORT [0x%x] \n", gMonDataAeStrct.monTx0powRep.statusFlags);
                    printf ("RL_RF_AE_MON_TX1_POWER_REPORT [0x%x] \n", gMonDataAeStrct.monTx1powRep.statusFlags);
                    printf ("RL_RF_AE_MON_TX2_POWER_REPORT [0x%x] \n", gMonDataAeStrct.monTx2powRep.statusFlags);
                    printf ("RL_RF_AE_MON_TX3_POWER_REPORT [0x%x] \n", gMonDataAeStrct.monTx3powRep.statusFlags);
                    printf ("RL_RF_AE_MON_TX0_BALLBREAK_REPORT [0x%x] \n", gMonDataAeStrct.monTx0BallbreakRep.statusFlags);
                    printf ("RL_RF_AE_MON_TX1_BALLBREAK_REPORT [0x%x] \n", gMonDataAeStrct.monTx1BallbreakRep.statusFlags);
                    printf ("RL_RF_AE_MON_TX2_BALLBREAK_REPORT [0x%x] \n", gMonDataAeStrct.monTx2BallbreakRep.statusFlags);
                    printf ("RL_RF_AE_MON_TX3_BALLBREAK_REPORT [0x%x] \n", gMonDataAeStrct.monTx3BallbreakRep.statusFlags);
                    break;
                case ANALOG_FAULT_TX_PHASE_INV:
                    printf ("RL_RF_AE_MON_Adv_TX_GAIN_MISMATCH_REPORT [0x%x] \n", gMonDataAeStrct.monAdvTxGainPhMisMatchRep.statusFlags);
                    break;
                case ANALOG_FAULT_SYNTH_FREQ:
                    printf ("RL_RF_AE_MON_PLL_CONTROL_VOLT_REPORT [0x%x] \n", gMonDataAeStrct.monPllConvVoltRep.statusFlags);
                    printf ("RL_RF_AE_MON_SYNTHESIZER_FREQ_REPORT [0x%x] \n", gMonDataAeStrct.monSynthFreqRep.statusFlags);
                    break;
                case ANALOG_FAULT_SUPPLY_LDO:
                    printf ("RL_RF_AE_MON_PMCLKLO_INT_ANA_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monPmClkIntAnaSigRep.statusFlags);
                    break;
                case ANALOG_FAULT_MISC:
                    printf ("RL_RF_AE_MON_DCC_CLK_FREQ_REPORT [0x%x] \n", gMonDataAeStrct.monDccClkFreqRep.statusFlags);
                    break;
                case ANALOG_FAULT_MISC_THRESHOLD:
                    printf ("RL_RF_AE_MON_GPADC_INT_ANA_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monGpadcIntAnaSigRep.statusFlags);
                    break;
                case ANALOG_FAULT_TX_PS_DAC:
                    printf ("AWR_MONITOR_TX_PHSHIFTER_DAC_REPORT_AE_SB [0x%x] \n", gMonDataAeStrct.monTxPhShifterDacRep.statusFlags);
                    break;
            }
            printf("----------------------------------------------------\n");

            /* clear the fault */
            anaFaultCfg = (rlAnaFaultInj_t){0};
        }
        return;
    }

    if(gDigLatentFaultTest == true)
    {
        /* Refer to rl_monitoring.h for inputMask bitfields in structure rlMonDigEnables::enMask */ 
        rlUInt32_t inputMask =  (1 << 6) | \
                                (1 << 7) | \
                                (1 << 9) | \
                                (1 << 16) | \
                                (1 << 17) | \
                                (1 << 25);
        printf("Input Mask For Latent Fault Test [0x%x]\n", inputMask);
        
        int32_t retVal = 0;
        retVal = mmwaveLink_digMonEnableConfig(true, inputMask);
        
        if(retVal < 0)
        {
            printf("Monitoring Configuration Failed\n");
            return;
        }
        printf("Monitoring Configuration Successful\n");
        return;
    }

    /* Check for If Continous Streaming mode Test is enabled */
    if(gLinkContModeTest == false)
    {
        /* mmWave Link set profile configuration */
        if (MmwaveLink_setProfileConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Profile config Fail\n");
            return;
        }
        PRINT_LOG ("Set mmWave Link Profile config Pass\n");

        /* mmWave Link Get RX Gain Temperature LUT configuration */
        if (mmwaveLink_RxGainTempLutRead() < 0)
        {
            PRINT_LOG ("Get mmWave Link RX Gain Temperature LUT config Fail\n");
            return;
        }
        PRINT_LOG ("Get mmWave Link RX Gain Temperature LUT config Pass\n");

        /* mmWave Link Get TX Gain Temperature LUT configuration */
        if (mmwaveLink_TxGainTempLutRead() < 0)
        {
            PRINT_LOG ("Get mmWave Link TX Gain Temperature LUT config Fail\n");
            return;
        }
        PRINT_LOG ("Get mmWave Link TX Gain Temperature LUT config Pass\n");

        if (platformType != RL_AR_DEVICETYPE_68XX)
        {
            /* mmWave Link Set RX Gain Temperature LUT configuration */
            if (mmwaveLink_RxGainTempLutConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link RX Gain Temperature LUT config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link RX Gain Temperature LUT config Pass\n");

            /* mmWave Link Set TX Gain Temperature LUT configuration */
            if (mmwaveLink_TxGainTempLutConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link TX Gain Temperature LUT config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link TX Gain Temperature LUT config Pass\n");
        }

        if (MmwaveLink_getProfileConfig() < 0)
        {
            PRINT_LOG ("Get mmWave Link Profile config Fail\n");
            return;
        }
        PRINT_LOG ("Get mmWave Link Profile config Pass\n");

        /* mmWave Link set Chirp configuration */
        if (MmwaveLink_setChirpConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Chirp config Fail\n");
            return;
        }
        PRINT_LOG ("Set mmWave Link Chirp config Pass\n");

        /* mmWave Link set Multi Chirp configuration. Use if reuiqred  */
        if(multichirpen)
        {
            if (MmwaveLink_setMultiChirpConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link Multi Chirp config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Multi Chirp config Pass\n");
        }
        if(((platformType == RL_AR_DEVICETYPE_18XX) && (!gLinkBpmEnableTest)) || (platformType == RL_AR_DEVICETYPE_68XX) || ((platformType == RL_AR_DEVICETYPE_294X) && (!gLinkBpmEnableTest)) || ((platformType == RL_AR_DEVICETYPE_2544) && (!gLinkBpmEnableTest)))
		{
			/* mmWave Link set Misc configuration */
            if (mmwaveLink_miscConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link Misc config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Misc config Pass\n");

            /* mmWave Link set Static Per Chirp Phase Shift configuration */
            if (mmwaveLink_perChirpPhaseShifterConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link Static Per Chirp Phase Shift config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Static Per Chirp Phase Shift config Pass\n");
        }

        if(platformType != RL_AR_DEVICETYPE_68XX)
        {
            /* Set Inter RX Gain/Phase configuration */
            if (mmwaveLink_interRxGainConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link Inter RX Gain/Phase config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Inter RX Gain/Phase config Pass\n");
        }

        /* mmWave Link Get Temperature report        */
        if (mmwaveLink_getTemperatureReport() < 0)
        {
            PRINT_LOG ("Set mmWave Link Get Temperature Report Fail\n");
            return;
        }
        PRINT_LOG ("Set mmWave Link Get Temperature Report Pass\n");

        /*Test source enable.*/
        if(gLinkMonitoringTest == false && testsrcen)
        {
            /* mmWave Link set Test Source configuration */
            if (mmwaveLink_testSourceConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link Test Source config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Test Source config Pass\n");
        }

        /* Check for If Advance Frame Test is enabled */
        if(gLinkAdvanceFrameTest == false)
        {   
            if (cqDataVerifyTest)
            {
                /* Rx Saturation monitoring configuration for CQ2 Data Extraction */
                if (MmwaveLink_setRfRxIfSatMonConfig() < 0)
                {
                    PRINT_LOG ("Rx Saturation Monitoring configuration Fail\n");
                    return;
                }
                PRINT_LOG ("Rx Saturation Monitoring configuration Pass\n");

                /* Enabling the Monitor for CQ2 Data*/
                if (MmwaveLink_setRfAnaMonConfigCq() < 0)
                {
                    PRINT_LOG ("Monitoring Enable for CQ2 test Fail\n");
                    return;
                }
                PRINT_LOG ("Monitoring Enable for CQ2 test Pass\n"); 
            }
            /* mmWave Link set Frame configuration */
            if (MmwaveLink_setFrameConfig(false, 0) < 0)
            {
                PRINT_LOG ("Set mmWave Link Frame config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Frame config Pass\n");

            /* mmWave Link get Frame configuration */
            if (MmwaveLink_getFrameConfig() < 0)
            {
                PRINT_LOG ("Get mmWave Link Frame config Fail\n");
                return;
            }
            PRINT_LOG ("Get mmWave Link Frame config Pass\n");
        
            /* Sensor Start */
            if (MmwaveLink_sensorStart() < 0)
            {
                PRINT_LOG ("Sensor Start Fail");
                return;
            }
            PRINT_LOG ("Sensor Start");

            /* Wait for X frames as per code requirment*/
            ClockP_usleep(framePeriodicity*1000U*2);
            
            /* sensor Stop */
            if (MmwaveLink_sensorStop() < 0)
            {
                PRINT_LOG ("Sensor Stop Fail");
                return;
            }
            PRINT_LOG ("Sensor Stop");

            /* If CQ test is enable then verify the CQ2 data which has been copied to
            L3 memory using eDMA at the last chirp of last frame */
            if (cqDataVerifyTest)
            {   
                //complete the last trasfer for last chirp
                MmwaveLink_triggerEdmaTransfer();
                /* verify  CQ2 data and store the test result */
                gCqTestResult = MmwaveLink_VerifyCQData(platformCore);
                /* verify CP-data and store the test result */
                gCpTestResult = MmwaveLink_VerifyCPData(platformCore);
                if (gCqTestResult == 0)
                {
                    PRINT_LOG ("CQ test result Pass ");
                }
                else
                {
                    PRINT_LOG ("CQ test result Fail ");
                }

                if (gCpTestResult == 0)
                {
                    PRINT_LOG ("CP test result Pass");
                }
                else
                {
                    PRINT_LOG ("CP test result Fail");
                }
                cqDataVerifyTest=false;
            }

            /*ADC Raw Data Trasfer to L3 memory*/

        }
    }
    else
    {
        /* mmWave Link set Continous Mode configuration */
        if (MmwaveLink_setContModeConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Continous Mode config Fail\n");
            return;
        }
        PRINT_LOG ("Set mmWave Link Continous Mode config Pass\n");

        /* mmWave Link Enable/Disable Continous mode */
        if (MmwaveLink_enableContMode(1) < 0)
        {
            PRINT_LOG ("Enable/Disable mmWave Link Continous mode Fail\n");
            return;
        }
        PRINT_LOG ("Enable/Disable mmWave Link Continous modeg Pass\n");
    }


    /* Check If Binary Phase Modulation test is enabled */
    if(gLinkBpmEnableTest == true)
    {
        /* mmWave Link Sets Binary Phase Modulation Common Configuration */
        if (MmwaveLink_setBpmCommonConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Binary Phase Modulation Common Config Fail\n");
            return;
        }
        PRINT_LOG ("Set mmWave Link Binary Phase Modulation Common Config Pass\n");

        /* mmWave Link Sets Binary Phase Modulation Chirp Configuration */
        if (MmwaveLink_setBpmChirpConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Binary Phase Modulation Chirp Config Fail\n");
            return;
        }
        PRINT_LOG ("Set mmWave Link Binary Phase Modulation Chirp Config Pass\n");
    }

    /* Check If Run Time calibration test is enabled */
    if(gLinkCalibTest == true)
    {
        /* mmWave Link Run time calibration configuration */
        if (MmwaveLink_setRunTimeCalibConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Run time Monitoring config Fail\n");
            return;
        }
        PRINT_LOG ("Set mmWave Link Run time Monitoring config Pass\n");
    }

    /* Check If Analog Monitoring is enabled */
    if(gLinkMonitoringTest == true)
        {
            /* mmWave RF Monitoring */
            if (MmwaveLink_monitoring(platformType) < 0)
            {
                PRINT_LOG ("Start Monitoring Fail\n");
                return;
            }
            PRINT_LOG ("Start Monitoring Pass\n");

            /* Re-initialize chirp & frame interrupts count */
            gLinkFrameCnt = 0;
            gLinkChirpCnt = 0;

            /* Sensor Start */
            if (MmwaveLink_sensorStart() < 0)
            {
                PRINT_LOG ("Sensor Start Fail");
                return;
            }
            PRINT_LOG ("Sensor Start");

            /* Wait for X frames as per code requirment*/
            ClockP_usleep(framePeriodicity*1000U*2);
            
            /* sensor Stop */
            if (MmwaveLink_sensorStop() < 0)
            {
                PRINT_LOG ("Sensor Stop Fail");
                return;
            }
            PRINT_LOG ("Sensor Stop");

            /* Print Monitoring Results  */
            if(gLinkAdvanceFrameTest == false)
            {
                resultMonitoringReport = MmwaveLink_CheckMonitoringReport(gLinkFrameCnt);
            }
            else
            {
                /* for advace frame, we get frame interrupt for each sub frame but monitoring report for each frame */
                resultMonitoringReport = MmwaveLink_CheckMonitoringReport(gLinkFrameCnt/(advFrameCfg.frameSeq.numOfSubFrames));
            }
            if(resultMonitoringReport < 0)
            {
                PRINT_LOG ("Monitoring results Fail\n");
            }
            else
            {
                PRINT_LOG ("Monitoring results Pass\n");
            }
            MmwaveLink_printMonitoringResult();
        }

 /***************************************************************************************************************
 * Advance Frame and Advance Chirp Config Test
 * Dynamic Chirp Test
 * ***********************************************************************************************************/  
    
    if(gLinkContModeTest == false) 
    {   
        /*Advace Chirp Config Test*/
        if(gAdvChirpTest == true && gLinkAdvanceFrameTest==true)
        {
            /* Re-initialize chirp & frame interrupts count */
            gLinkFrameCnt = 0;
            gLinkChirpCnt = 0;

            resultAdvChirpCfg = MmwaveLink_rlSetAdvChirpCfg();
            if (resultAdvChirpCfg < 0)
            {
                PRINT_LOG ("Advance Chirp config Fail\n");
            }

            /* Configure Advanced Frame config where SF0 is set with 6 burst */
            activFrameTime = ((PROFILE_0_IDLE_TIME + PROFILE_0_RAMP_END_TIME) * CHIRPS_PER_CYCLE);
            AdvframeCfgArgs.frameSeq.numOfSubFrames = 1;
            AdvframeCfgArgs.frameSeq.subFrameCfg[0].chirpStartIdx = 0;
            AdvframeCfgArgs.frameSeq.subFrameCfg[0].numOfChirps = 1;
            AdvframeCfgArgs.frameSeq.subFrameCfg[0].numLoops = CHIRPS_PER_CYCLE; /* 512 chirps per burst */
            AdvframeCfgArgs.frameSeq.subFrameCfg[0].burstPeriodicity = (activFrameTime/1000 * 1.5)* 1000000 / 5; /* duty cycle: 25% */
           
            AdvframeCfgArgs.frameSeq.subFrameCfg[0].chirpStartIdxOffset = 0; //with advChirpConfig this param is not applicable
            AdvframeCfgArgs.frameSeq.subFrameCfg[0].numOfBurst = NUM_UNIQ_CYCLES;
            AdvframeCfgArgs.frameSeq.subFrameCfg[0].numOfBurstLoops = 1;
            AdvframeCfgArgs.frameSeq.subFrameCfg[0].subFramePeriodicity = AdvframeCfgArgs.frameSeq.subFrameCfg[0].burstPeriodicity * AdvframeCfgArgs.frameSeq.subFrameCfg[0].numOfBurst;;
            
            AdvframeCfgArgs.frameSeq.numFrames = 0;// infinite frames 
            AdvframeCfgArgs.frameSeq.triggerSelect = 1;
           retVal = rlSetAdvFrameConfig(RL_DEVICE_MAP_INTERNAL_BSS, &AdvframeCfgArgs);
             if (retVal < 0)
            {
                PRINT_LOG ("Advance Frame config fail\n");
                return;
            }
            PRINT_LOG ("Advance Frame config Pass\n");

             /* Sensor Start */
            retVal = rlSensorStart(RL_DEVICE_MAP_INTERNAL_BSS); 
            if (retVal < 0)
            {
                PRINT_LOG ("Sensor Start Fail");
                return;
            }
            PRINT_LOG ("Sensor Start Pass\n");

            /* Wait for 1 frame */
            ClockP_usleep(framePeriodicity*1000U);
            /* sensor Stop */
            retVal=  rlSensorStop(RL_DEVICE_MAP_INTERNAL_BSS);
            if (retVal < 0)
            {   
                PRINT_LOG ("Sensor Stop Fail\n");
                return;
            }
            PRINT_LOG ("Sensor Stop Pass\n");

            //Implement the check for correct functionality
            if((AdvChirptimeStamp21 -AdvChirptimeStamp2) <= (AdvChirptimeStamp12-AdvChirptimeStamp1))
            {
                PRINT_LOG ("Advance Chirp Config test failed\n");
                return;
            }
            PRINT_LOG ("Advance Chirp Config Test Passed\n");
        }

        /* Re-initialize chirp & frame interrupts count */
        gLinkFrameCnt = 0;
        gLinkChirpCnt = 0;
        printf ("Debug: Starting Sensor.. \n");
        /* mmWave Link start sensor */
        resultSensorStart = MmwaveLink_sensorStart();
        if (resultSensorStart < 0)
        {
            PRINT_LOG ("Start sensor Fail\n");
            return;
        }

        /* Dynamically update chirp config while frame is ongoing */
        /* @Note - In case dynamic chirp test is ON in link_test application, then before downloading
           debug image from CCS to device, please disable CIO feature in CCS.
           This change required to avoid any Chirp/Frame interrupt in this application.
           Steps to disable CIO function in CCS for specific target configuration
           1. Open properties window of your target configuration file (*.ccxml) from 'Target Configuration'
           mini-window.
           2. Choose 'Program/Memory Load Options'
           3. Un-check 'Enable CIO function use' checkbox
           4. Apply this change and press OK.
           5. Launch Target Configuration and download MSS/DSS test application.

           After running this application wait for few minutes and then halt to check that if
           any error occured.
           dynChirpTestStatus == 0x0FFF -> no error occured while dynamic chirp config and new parameters are effective.
           dynChirpTestStatus == 0x0000 -> Either rlSetDynChirpCfg or rlSetDynChirpEn failed
           dynChirpTestStatus == 0x0AFF -> DynChirp config is not effective.
        */
        if(gDynamicChirpTest == true)
        {
            /* wait till few (~half) frame interrupts are received */
            ClockP_usleep((framePeriodicity*frameCount*1000)/2);
            resultDynamicChirpCfg = MmwaveLink_rlSetDynChirpCfg();
            if (resultDynamicChirpCfg < 0)
            {
                PRINT_LOG ("Dynamic Chirp config Fail");
                return;
            }

            /*Dynamic Phase change*/
            int8_t resultDynamicPhaseCfg = MmwaveLink_rlSetDynPerChirpPhShifterCfg();
            if (resultDynamicPhaseCfg < 0)
            {
                PRINT_LOG ("Dynamic Chirp Phase config Fail");
                return;
            }

            resultDynamicChirpEnable = MmwaveLink_rlSetDynChirpEn();
            if (resultDynamicChirpEnable < 0)
            {
                PRINT_LOG ("Dynamic Chirp Enable Fail\n");
                return;
            }

            /* wait till few (1/4 th) frame interrupts are received until configurations applied by H/W */
            ClockP_usleep((framePeriodicity*frameCount*1000)/4);
            
            if (MmwaveLink_getDynChirpConfig() < 0)
            {
                PRINT_LOG ("Get Chirp Config Fail\n");
                return;
            }
        }
        else
        {
            /* wait till few (3/4th) frame interrupts are received */
            ClockP_usleep((framePeriodicity*frameCount*1000)*3/4);
        }

        /* wait till all frame interrupts are received */
        ClockP_usleep((framePeriodicity*1000*(frameCount + 1))/4);

        if(gDynamicChirpTest == true)
        {   //check
            if(resultDynamicChirpCfg == 0)
            {
                PRINT_LOG ("Dynamic Chirp config Pass\n");
            }
            if(resultDynamicChirpEnable == 0)
            {
                PRINT_LOG ("Dynamic Chirp Enable Pass\n");
            }
            /* set the status value to notify that dynamic chirp configuration has been done correctly */
            dynChirpTestStatus |= 0x000F;
        }
        else
        {
            /* for Legacy frame config test */
            if(gLinkAdvanceFrameTest == false)
            {
                /* wait for all chirp interrupts of current frame */
                while(gLinkChirpCnt != ((frameCfg.chirpEndIdx - frameCfg.chirpStartIdx + 1U)* frameCfg.numLoops));
            }
            else
            {
                /* In case of advaced frame config, by this time last frame should be over.
                   Check if we recieved same number of chirp interrupts as given in last
                   sub-frame of Advanced frame configuration */
                while(gLinkChirpCnt != advFrameCfg.frameSeq.subFrameCfg[advFrameCfg.frameSeq.numOfSubFrames-1U].numOfChirps);
            }
        }

        if((gLinkFrameCnt == frameCfg.numFrames) || (gLinkFrameCnt == (advFrameCfg.frameSeq.numFrames *
                        advFrameCfg.frameSeq.numOfSubFrames)))
        {
           PRINT_LOG ("All Frames Received Pass\n");
        }
        else
        {
            PRINT_LOG ("All Frames are not Received Pass\n");
        }

        /* To verify that new Chirp configurations have been taken effect on RF-front end,
        we need to take time comparison of chirp interrupt arrival before and after dynamic
        chirp config. As in DynChirp Config chirp-Idle Time has increased so after this come
        in effect, every chirp interrupt arrival will have extra time delay */

        if(gDynamicChirpTest == true)
        {
            /* Set the status value to notify that sensor stop is passed */
            dynChirpTestStatus |= 0x00F0;

            /* Average interval post dynamic chirp should be more than old chirp avg interval */
            if(avgChirpTimeInterval[1U] <= avgChirpTimeInterval[0U])
            {
                /* Set the status value to notify that Dynamic chirp config is not affective */
                dynChirpTestStatus |= 0x0A00;
                PRINT_LOG ("Dynamic Chirp config Fail\n");
            }
            else
            {
                /* Set the status value to notify that Dynamic chirp config is affective */
                dynChirpTestStatus |= 0x0F00;
                PRINT_LOG ("Dynamic Chirp config Pass\n");
            }
        }
    }
     /* sensor Stop */
            if (MmwaveLink_sensorStop() < 0)
            {
                PRINT_LOG ("Sensor Stop Fail\n");
                return;
            }
            PRINT_LOG ("Sensor Stop Pass\n");

    if(gLinkContModeTest == false)
    {
        /* Infinite framing mode test, configuring numFrames to 0(infinite frames) */
        printf("-----------Debug: Infinite Frame mode test-----------\n");
        frameCfg.numFrames = 0U;
        advFrameCfg.frameSeq.numFrames = 0;
        if(gLinkAdvanceFrameTest == true)
        {
            /* mmWave Link set Advance Frame configuration */
            if (MmwaveLink_setAdvFrameConfig() < 0)
            {
                PRINT_LOG ("Set mmWave Link Advance Frame config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Advance Frame config Pass\n");
        }
        else
        {
            /* mmWave Link set Frame configuration */
            if (MmwaveLink_setFrameConfig(false, 0) < 0)
            {
                PRINT_LOG ("Set mmWave Link Frame config Fail\n");
                return;
            }
            PRINT_LOG ("Set mmWave Link Frame config Pass\n");
        }

        /* mmWave Link start sensor */
        resultSensorStart = MmwaveLink_sensorStart();
        if (resultSensorStart < 0)
        {
            PRINT_LOG ("Start sensor Fail\n");
            return;
        }
        PRINT_LOG ("Start sensor Pass\n");

        /* wait till 10 frame interrupts received */
        ClockP_usleep(framePeriodicity*10*1000U);

        /* mmWave Link stop sensor */
        if (MmwaveLink_sensorStop() < 0)
        {
            PRINT_LOG ("Stop sensor Fail\n");
        }
        else
        {
            PRINT_LOG ("Stop sensor Pass\n");
        }
    }
    if(gLinkSubFrameTrigTest == true)
    {
        advFrameCfg.frameSeq.subFrameTrigger = 1;
        if(MmwaveLink_subframeStart() < 0)
        {
            PRINT_LOG ("sub frame trigger Fail\n");
        }
        /* Disable subframe trigger variable once subframe trigger API done.
           Advance frame configuration may used further */
        advFrameCfg.frameSeq.subFrameTrigger = 0;
    }
}


/**
 *  @b Description
 *  @n
 *      Interrupt handler callback for frame start ISR.
 *
 *  @retval
 *      Not Applicable.
 */
void Mmwavelink_frameInterrupCallBackFunc(void* arg)
{
    /* check if number of chirps in last frame is same as configured numbers */
    if((gLinkFrameCnt > 0) &&
       (gLinkChirpCnt != (((frameCfg.chirpEndIdx - frameCfg.chirpStartIdx) + 1U)*frameCfg.numLoops))&&
       (gLinkChirpCnt != advFrameCfg.frameSeq.subFrameCfg[0].numOfChirps))
    {
        chirpMissedError = true;
    }

     /* increment Frame count */
    gLinkFrameCnt++;

        /* if Dynamic Chirp test is enabled then signal the main thread after receiving ~half
       the number of configured frames, so that application can configure and enable
       new chirps using Dynamic chirp configuration APIs */
        if((gDynamicChirpTest == true) && (gDynChirpConfigNum == 0U))
        {
            /* assuming frameCfg.numFrames is more than double of MAX_CHIRP_TIME_INTERVAL_STORE */
            if(((gLinkFrameCnt >= frameCfg.numFrames/2U) || (gLinkFrameCnt >= (advFrameCfg.frameSeq.numFrames *
                    advFrameCfg.frameSeq.numOfSubFrames)/2U))  && (gLinkChirpCnt == 0))
            {
                /* set tag to false to notify that by this time Dynamic chirp configuration is done */
                gDynChirpConfigNum = 1U;
                SemaphoreP_post (&gLinkFrameSem);
            }
        }
        else
        {
            if((gLinkFrameCnt >= (frameCfg.numFrames)) || (gLinkFrameCnt >= (advFrameCfg.frameSeq.numFrames *
                    advFrameCfg.frameSeq.numOfSubFrames)))
            {
                SemaphoreP_post (&gLinkFrameSem);
            }
        }
    gLinkChirpCnt = 0;
}

/**
 *  @b Description
 *  @n
 *      Interrupt handler callback for chirp available ISR.
 *
 *  @retval
 *      Not Applicable.
 */
void Mmwavelink_chirpAvailInterrupCallBackFunc(void* arg)
{   

    /* if Dynamic Chirp test is enabled then profile the time taken by chirps before and
       and after dynamic chirp configuration to ensure that new chirps are configured correctly */
    if(gDynamicChirpTest == true)
    {
        if(gLinkChirpCnt == 0)
        {
            lastChirpIntTimeStamp =  ClockP_getTimeUsec();
        }
        else if(gLinkChirpCnt == (NUM_OF_DYNAMIC_CHIRP_CONFIG_SEG * 16U) - 1U)
        {
            /* Compare the average of Dynamic chirp programmed
            avgChirpTimeInterval[0] = Before Dynamic chirps,
            avgChirpTimeInterval[1] = After Dynamic chirps */
            avgChirpTimeInterval[gDynChirpConfigNum] = ClockP_getTimeUsec() - lastChirpIntTimeStamp;
        }
    }

    if(gAdvChirpTest==true)
    {
        if(gLinkChirpCnt == 5)
        {
            AdvChirptimeStamp1 = ClockP_getTimeUsec();
        }
        else if(gLinkChirpCnt==6)
        {
            AdvChirptimeStamp12 = ClockP_getTimeUsec();
        }   
        else if(gLinkChirpCnt==520)
        {
            AdvChirptimeStamp2 = ClockP_getTimeUsec();
             
        }
        else if(gLinkChirpCnt==521)
        {
            AdvChirptimeStamp21 = ClockP_getTimeUsec();
        }
    }

    /* CQ data verification test: trigger eDMA to copy data from CQ RAM to L3 mem.
     * Note - this test has been defined for legacy frame scenario, for advanced frame config user
     *        needs to write seperate logic.
     */
    if (cqDataVerifyTest && gLinkChirpCnt>0)
    {
        /* Trigger eDMA to trasnfer CQ2 RAM data to L3 memory for each chirp */
        MmwaveLink_triggerEdmaTransfer();
    }
    /* increment the Chirp count for current frame */
    gLinkChirpCnt++;
}

