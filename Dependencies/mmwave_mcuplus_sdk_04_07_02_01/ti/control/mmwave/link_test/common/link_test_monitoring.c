/*
 *   @file  link_test_monitoring.c
 *
 *   @brief
 *      Implements mmwavelink tests for monitoring and calibration APIs
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

#include <ti/common/syscommon.h>
#include <kernel/dpl/AddrTranslateP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/HwiP.h>
#include <drivers/edma.h>
#include <drivers/hw_include/hw_types.h>

/* mmWave SDK Include Files: */
#include <ti/control/mmwavelink/mmwavelink.h>
#include <drivers/crc.h>
#include <ti/control/mmwavelink/include/rl_driver.h>
#include <ti/utils/testlogger/logger.h>
/* ADCBUF Driver: */
#if defined(SOC_AWR294X) || defined(SOC_AWR2X44P)
#include <drivers/adcbuf.h>
#endif
#include <ti/control/mmwave/link_test/common/link_testcase_setting.h>
#include <ti/control/mmwave/link_test/common/link_test.h>
#include <ti/control/mmwave/link_test/common/link_test_monitoring.h>
#include <ti/control/mmwave/link_test/common/link_testcase_config.h>




/**************************************************************************
 *************************** Global Variables *****************************
 **************************************************************************/
uint16_t monAeCnt [MMWAVELINK_TEST_MAX_NUM_MON] = { 0 };
uint16_t monFailRepCheck [MMWAVELINK_TEST_MAX_NUM_MON] = { 0 };
uint32_t anaMonEn = 0;
monitoringDataAe_t gMonDataAeStrct = { 0 };
extern uint32_t gRunTimeCalibStatus;


/**************************************************************************
 ************************** mmWave Link Functions *************************
 **************************************************************************/
/**
 *  @b Description
 *  @n
 *      Initialize monitoring report counters.
 *
 *  @retval
 *      Not Applicable.
 */
void MmwaveLink_monitorReportCountInit (void)
{
    int32_t i;

    /* Reset Monitoring report counts & error report counts */
    for(i = 0; i < MMWAVELINK_TEST_MAX_NUM_MON; i++)
    {
        monFailRepCheck[i] = 0;
        monAeCnt[i] = 0;
    }
}

/**
 *  @b Description
 *  @n
 *      The function Sets the consolidated configuration of all analog monitoring excluding
 *      CQ & CP monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfAnaMonConfig (uint8_t platformType, bool useInputMask, rlUInt32_t inputMask)
{
    int32_t  retVal;
    rlMonAnaEnables_t data;

    if(useInputMask)
    {
        data.enMask1 = inputMask;
    }
    else
    {
        /* Each bit(0-31) represents different analog monitor enable configuration */
        data.enMask1 = (1 << 0)   | \
            (1 << 1)   | \
            (0 << 2)   | \
            (1 << 3)   | \
            (1 << 4)   | \
            (1 << 5)   | \
            (1 << 6)   | \
            (1 << 7)   | \
            (1 << 8)   | \
            (1 << 9)   | \
            (1 << 10)  | \
            (1 << 11)  | \
            (1 << 12)  | \
            (1 << 13)  | \
            (1 << 14)  | \
            (0 << 15)  | \
            (1 << 16)  | \
            (1 << 17)  | \
            (1 << 18)  | \
            (1 << 19)  | \
            (1 << 20)  | \
            (1 << 21)  | \
            (1 << 22)  | \
            (1 << 23)  | \
            (0 << 24)  | \
            (0 << 25)  | \
            (0 << 26)  | \
            (0 << 27)  | \
            (0 << 28)  | \
            (1 << 29)  | \
            (1 << 30)  | \
            (1 << 31);
    }

    anaMonEn = data.enMask1;

    /* Analog monitors enable */
    retVal = rlRfAnaMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,&data);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfAnaMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlRfAnaMonConfig configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to temperature monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfTempMonConfig (void)
{
    int32_t         retVal;

    /* Set Temperature monitor configuration */
    retVal = rlRfTempMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlTempMonConf_t*)&tempMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfTempMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished temperature monitoring configurations to BSS\n");

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets RX gain and phase monitoring config to device.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfRxGainPhaMonConfig (void)
{
    int32_t         retVal;
    /* Set RX Gain Phase monitor configuration */
    retVal = rlRfRxGainPhMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlRxGainPhaseMonConf_t*)&rxGainPhaseMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfRxGainPhMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished RX gain and phase monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to RX IF filter attenuation monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfRxIfStageMonConfig (void)
{
    int32_t         retVal;
    /* Set RX IF Stage monitor configuration */
    retVal = rlRfRxIfStageMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlRxIfStageMonConf_t*)&monRxIfStageCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfRxIfStageMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished RX IF filter attenuation monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to TX power monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfTxPowMonConfig (void)
{
    int32_t         retVal;
    /* Set TX Power monitor configuration */
    retVal = rlRfTxnPowrMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlTxnPowMonConf_t*)&allTxPowMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfTxnPowrMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to TX Power monitor configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to TX ball break detection.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfTxBallbreakMonConfig (void)
{
    int32_t         retVal;
    /* Set TX Ballbreak monitor configuration */
    retVal = rlRfTxnBallbreakMonConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlTxnBallbreakMonConf_t*)&allTxBallbreakMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfTxBallbreakMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to TX ball break configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to TX gain and phase mismatch monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfTxGainPhaseMismatchMonConfig (void)
{
    int32_t         retVal;
    /* Set TX Gain Phase monitor configuration */
    retVal = rlRfTxGainPhaseMismatchMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlTxGainPhaseMismatchMonConf_t*)&txGainPhMisMatchMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfTxGainPhaseMismatchMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to TX gain and phase mismatch monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to TX gain and phase mismatch monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfAdvTxGainPhaseMismatchMonConfig (void)
{
    int32_t         retVal;
    /* Set TX Gain Phase monitor configuration */
    retVal = rlRfAdvTxGainPhaseMismatchMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlAdvTxGainPhaseMismatchMonConf_t*)&AdvtxGainPhMisMatchMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfTxGainPhaseMismatchMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to TX gain and phase mismatch monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to synthesizer frequency.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfSynthFreqMonConfig (void)
{
    int32_t         retVal;

    /* Set Synth Freq monitor configuration */
    retVal = rlRfSynthFreqMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlSynthFreqMonConf_t*)&synthFreqMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfSynthFreqMonConfig retVal=%d\n", retVal);
        return -1;
    }

    printf("Debug: Finished information related to synthesizer frequency configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to synthesizer frequency non live monitor.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfSynthFreqNonLiveMonConfig ()
{
    int32_t         retVal;

    /* Set Synth Freq monitor configuration */
    retVal = rlRfSynthFreqMonConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlSynthFreqMonConf_t *)&synthFreqNonLiveMonCfg[0U]);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfSynthFreqNonLiveMonConfig 1 retVal=%d\n", retVal);
        return -1;
    }

    /* Set Synth Freq monitor configuration */
    retVal = rlRfSynthFreqMonConfig(RL_DEVICE_MAP_INTERNAL_BSS, ( rlSynthFreqMonConf_t *)&synthFreqNonLiveMonCfg[1U]);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfSynthFreqNonLiveMonConfig 2 retVal=%d\n", retVal);
        return -1;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to TX Internal Analog Signals monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfTxIntAnaSignalMonConfig (void)
{
    int32_t         retVal;

    /* Set TX Internal analog signal monitor configuration */
    retVal = rlRfTxnIntAnaSignalsMonConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlTxnIntAnaSignalsMonConf_t*)&allTxanaSigMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfTxIntAnaSignalsMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to TX Internal Analog Signals configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to RX Internal Analog Signals monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfRxIntAnaSignalMonConfig (void)
{
    int32_t         retVal;
    rlRxIntAnaSignalsMonConf_t rxIntAnaSgnlMonCfg = {0,2,0,0};

    /* Set RX Internal analog signal monitor configuration */
    retVal = rlRfRxIntAnaSignalsMonConfig(RL_DEVICE_MAP_INTERNAL_BSS, &rxIntAnaSgnlMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfRxIntAnaSignalsMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to RX Internal Analog Signals configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to Power Management, Clock generation and LO distribution.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfPmClkLoIntAnaSignalsMonConfig (void)
{
    int32_t         retVal;
    /* Set PMCLKLO monitor configuration */
    retVal = rlRfPmClkLoIntAnaSignalsMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlPmClkLoIntAnaSignalsMonConf_t*)&pmClkLoIntAnaSigMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfPmClkLoIntAnaSignalsMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to Power Management, Clock generation and LO distributioncon configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to GPADC Internal Analog Signals monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfGpadcIntAnaSignalsMonConfig (void)
{
    int32_t         retVal;
    /* Set GPADC internal analog signal monitor configuration */
    retVal = rlRfGpadcIntAnaSignalsMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlGpadcIntAnaSignalsMonConf_t*) &gpadcIntAnaSigMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfGpadcIntAnaSignalsMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to GPADC Internal Analog Signals monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to APLL and Synthesizer's control voltage signals monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfPllContrlVoltMonConfig (void)
{
    int32_t         retVal;
    /* Set PLL control voltage monitor configuration */
    retVal = rlRfPllContrlVoltMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlPllContrVoltMonConf_t*)&pllConVoltMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfPllContrlVoltMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to APLL and Synthesizer's control voltage signals monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to the DCC based clock frequency monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfDualClkCompMonConfig (void)
{
    int32_t         retVal;
    /* Set DCC monitor configuration */
    retVal = rlRfDualClkCompMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,(rlDualClkCompMonConf_t*)&dualClkCompMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfDualClkCompMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to the DCC based clock frequency monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets information related to  RX saturation detector monitoring.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfRxIfSatMonConfig (void)
{
    int32_t         retVal;

    /* Set RX IF Stage monitor configuration */
    retVal = rlRfRxIfSatMonConfig(RL_DEVICE_MAP_INTERNAL_BSS, (rlRxSatMonConf_t*)&rxSatMonCfg);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfRxIfSatMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to  RX saturation detector monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to Configure digital monitors
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t mmwaveLink_digMonEnableConfig(bool useInputMask, rlUInt32_t inputMask)
{
    int32_t  retVal;
    rlMonDigEnables_t data = { 0 };

    if(useInputMask)
    {
        data.enMask = inputMask;
    }
    else
    {
        /* Each bit(0-31) represents different digital monitor enable configuration */
        data.enMask = (0 << 0)   | \
                      (1 << 1)   | \
                      (0 << 2)   | \
                      (1 << 3)   | \
                      (0 << 4)   | \
                      (0 << 5)   | \
                      (1 << 6)   | \
                      (1 << 7)   | \
                      (1 << 8)   | \
                      (1 << 9)   | \
                      (1 << 10)  | \
                      (1 << 11)  | \
                      (0 << 12)  | \
                      (0 << 13)  | \
                      (0 << 14)  | \
                      (0 << 15)  | \
                      (1 << 16)  | \
                      (1 << 17)  | \
                      (1 << 18)  | \
                      (1 << 19)  | \
                      (1 << 20)  | \
                      (0 << 21)  | \
                      (0 << 22)  | \
                      (0 << 23)  | \
                      (1 << 24)  | \
                      (1 << 25)  | \
                      (1 << 26)  | \
                      (0 << 27)  | \
                      (0 << 28)  | \
                      (0 << 29)  | \
                      (0 << 30)  | \
                      (0 << 31);
    }

    data.testMode = 0;
    /* Digital monitoring configuration */
    retVal = rlRfDigMonEnableConfig(RL_DEVICE_MAP_INTERNAL_BSS,&data);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfDigMonEnableConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlRfDigMonEnableConfig configurations to BSS\n");
    return 0;
};

/**
 *  @b Description
 *  @n
 *      The function is used to Configure digital monitors
 *      for periodicity.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_rlRfDigMonPeriodicConfig (void)
{
    int32_t         retVal;
    rlDigMonPeriodicConf_t data = {0x0};
    data.reportMode = 0;
    data.periodicEnableMask = 0xD;

    /* Set Digital periodic monitor configuration */
    retVal = rlRfDigMonPeriodicConfig(RL_DEVICE_MAP_INTERNAL_BSS, &data);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfDigMonPeriodicConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished information related to  rlRfDigMonPeriodicConfig monitoring configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function Sets the consolidated configuration of CQ & CP monitoring If analog monitors
 *      already enabled then function enable analog monitors(those enabled), CQ & CP monitors .
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfAnaMonConfigCq (void)
{
    int32_t  retVal;

    /* Each bit(0-31) represents different analog monitor enable configuration */
    rlMonAnaEnables_t data = { (0 << 0)   | \
                               (0 << 1)   | \
                               (0 << 2)   | \
                               (0 << 3)   | \
                               (0 << 4)   | \
                               (0 << 5)   | \
                               (0 << 6)   | \
                               (0 << 7)   | \
                               (0 << 8)   | \
                               (0 << 9)   | \
                               (0 << 10)  | \
                               (0 << 11)  | \
                               (0 << 12)  | \
                               (0 << 13)  | \
                               (0 << 14)  | \
                               (0 << 15)  | \
                               (0 << 16)  | \
                               (0 << 17)  | \
                               (0 << 18)  | \
                               (0 << 19)  | \
                               (0 << 20)  | \
                               (0 << 21)  | \
                               (0 << 22)  | \
                               (0 << 23)  | \
                               (1 << 24)  | \
                               (0 << 25)  | \
                               (0 << 26)  | \
                               (0 << 27)  | \
                               (0 << 28)  | \
                               (0 << 29)  | \
                               (0 << 30)  | \
                               (0 << 31)
                                ,0x0};
     data.enMask1 |= anaMonEn;
     anaMonEn = data.enMask1;
    /* Set analog monitor enable configuration */
    retVal = rlRfAnaMonConfig(RL_DEVICE_MAP_INTERNAL_BSS,&data);
    /* Check for mmWaveLink API call status */
    if(retVal != 0)
    {
        /* Error: Link reported an issue. */
        printf("Error: rlRfAnaMonConfig retVal=%d\n", retVal);
        return -1;
    }
    printf("Debug: Finished rlRfAnaMonConfig configurations to BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to check the monitoring reports are being received or not.
 *      For some monitors we also check the status flags to verify correct reports.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_CheckMonitoringReport(uint32_t frameCnt)
{
    int32_t failedMonRepCnt = 0;
    printf ("----------------------------------------------------\n");
    if(((anaMonEn >> MMWAVELINK_TEST_MON_TEMP) & 1U) == 1U)
    {
        if(monAeCnt[MMWAVELINK_TEST_MON_TEMP] < frameCnt)
        {
            printf("Missing: Temp monitor report missed, received %d out of %d \n", monAeCnt[MMWAVELINK_TEST_MON_TEMP],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> MMWAVELINK_TEST_MON_RX_GAIN_PH) & 1U) == 1U)
    {
        if(monAeCnt[MMWAVELINK_TEST_MON_RX_GAIN_PH] < frameCnt)
        {
            printf("Missing: Rx Gain Phase monitor report missed, received %d out of %d \n", monAeCnt[MMWAVELINK_TEST_MON_RX_GAIN_PH],frameCnt);
            failedMonRepCnt = -1;
        }
    }

    if(((anaMonEn >> 3U) & 1U) == 1U)
    {
        if(monAeCnt[3] < frameCnt)
        {
            printf("Missing: If stage monitor report missed, received %d out of %d \n", monAeCnt[3],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 4U) & 1U) == 1U)
    {
        if(monAeCnt[4] < frameCnt)
        {
            printf("Missing: TX0 pow monitor report missed, received %d out of %d \n", monAeCnt[4],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 5U) & 1U) == 1U)
    {
        if(monAeCnt[5] < frameCnt)
        {
            printf("Missing: TX1 pow monitor report missed, received %d out of %d \n", monAeCnt[5],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 6U) & 1U) == 1U)
    {
            if(monAeCnt[6] < frameCnt)
            {
                printf("Missing: TX2 pow monitor report missed, received %d out of %d \n", monAeCnt[6],frameCnt);
                failedMonRepCnt = -1;
            }
    }
    if(((anaMonEn >> 7U) & 1U) == 1U)
    {
        if(monAeCnt[7] < frameCnt)
        {
            printf("Missing: TX0 ball break monitor report missed, received %d out of %d \n", monAeCnt[7],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 8U) & 1U) == 1U)
    {
        if(monAeCnt[8] < frameCnt)
        {
            printf("Missing: TX1 ball break monitor report missed, received %d out of %d \n", monAeCnt[8],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 9U) & 1U) == 1U)
    {
            if(monAeCnt[9] < frameCnt)
            {
                printf("Missing: TX2 ball break monitor report missed, received %d out of %d \n", monAeCnt[9],frameCnt);
                failedMonRepCnt = -1;
            }
    }
    if(((anaMonEn >> 14U) & 1U) == 1U)
    {
        if(monAeCnt[14] < frameCnt)
        {
            printf("Missing: Synth Freq monitor report missed, received %d out of %d \n", monAeCnt[14],frameCnt);
            failedMonRepCnt = -1;
        }
    }

    if(((anaMonEn >> 16U) & 1U) == 1U)
    {
        if(monAeCnt[16] < frameCnt)
        {
            printf("Missing: Int TX0 Sig monitor report missed, received %d out of %d \n", monAeCnt[16],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 17U) & 1U) == 1U)
    {
        if(monAeCnt[17] < frameCnt)
        {
            printf("Missing: Int TX1 Sig monitor report missed, received %d out of %d \n", monAeCnt[17],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 18U) & 1U) == 1U)
    {
            if(monAeCnt[18] < frameCnt)
            {
                printf("Missing: Int TX2 Sig monitor report missed, received %d out of %d \n", monAeCnt[18],frameCnt);
                failedMonRepCnt = -1;
            }
    }
    if(((anaMonEn >> 19U) & 1U) == 1U)
    {
        if(monAeCnt[19] < frameCnt)
        {
            printf("Missing: Int RX Sig monitor report missed, received %d out of %d \n", monAeCnt[19],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 20U) & 1U) == 1U)
    {
        if(monAeCnt[20] < frameCnt)
        {
            printf("Missing: PMCLKLO Sig monitor report missed, received %d out of %d \n", monAeCnt[20],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 21U) & 1U) == 1U)
    {
        if(monAeCnt[21] < frameCnt)
        {
            printf("Missing: Int Gpadc Sig monitor report missed, received %d out of %d \n", monAeCnt[21],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 22U) & 1U) == 1U)
    {
        if(monAeCnt[22] < frameCnt)
        {
            printf("Missing: Pll Ctrl Volt monitor report missed, received %d out of %d \n", monAeCnt[22],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 23U) & 1U) == 1U)
    {
        if(monAeCnt[23] < frameCnt)
        {
            printf("Missing: Dcc Clk Freq monitor report missed, received %d out of %d \n", monAeCnt[23],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 28U) & 1U) == 1U)
    {
        if(monAeCnt[28] < frameCnt)
        {
            printf("Missing: Synth Non-lIve monitor report missed, received %d out of %d \n", monAeCnt[28],frameCnt);
            failedMonRepCnt = -1;
        }
    }
    if(((anaMonEn >> 29U) & 1U) == 1U)
    {
        if(monAeCnt[29] < frameCnt)
        {
            printf("Missing: TX3 pow monitor report missed, received %d out of %d \n", monAeCnt[29],frameCnt);
            failedMonRepCnt = -1;
        }
    }
     if(((anaMonEn >> 30U) & 1U) == 1U)
    {
        if(monAeCnt[30] < frameCnt)
        {
        printf("Missing: TX3 ballbreak monitor report missed, received %d out of %d \n", monAeCnt[30],frameCnt);
        failedMonRepCnt = -1;
        }
    }
     if(((anaMonEn >> 31U) & 1U) == 1U)
    {
        if(monAeCnt[31] < frameCnt)
        {
        printf("Missing: TX3 int signal monitor report missed, received %d out of %d \n", monAeCnt[31],frameCnt);
        failedMonRepCnt = -1;
        }
    }
    printf("\n");

    if(monFailRepCheck[MMWAVELINK_TEST_MON_TEMP] > 0)
    {
        printf ("Error: Temperature monitor report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[MMWAVELINK_TEST_MON_TEMP]);
        failedMonRepCnt = -1;
    }

    if(monFailRepCheck[3] > 0)
    {
        printf ("Error: Rx IF Stage Report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[3]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[4] > 0)
    {
        printf ("Error: Rx TX0 Power report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[4]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[5] > 0)
    {
        printf ("Error: Rx TX1 Power report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[5]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[6] > 0)
    {
            printf ("Error: Rx TX2 Power report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[6]);
            failedMonRepCnt = -1;
    }

    if(monFailRepCheck[7] > 0)
    {
        printf ("Error: Rx TX0 Ballbreak report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[7]);
        failedMonRepCnt = -1;
        printf("Rx TX0 Ballbreak report: errCode:0x%x, txReflCoefVal: %d \n",
                    gMonDataAeStrct.monTx0BallbreakRep.errorCode,
                    gMonDataAeStrct.monTx0BallbreakRep.txReflCoefVal);
    }
    if(monFailRepCheck[8] > 0)
    {
        printf ("Error: Rx TX1 Ballbreak report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[8]);
        failedMonRepCnt = -1;
        printf("Rx TX1 Ballbreak report: errCode:0x%x, txReflCoefVal: %d \n",
                    gMonDataAeStrct.monTx1BallbreakRep.errorCode,
                    gMonDataAeStrct.monTx1BallbreakRep.txReflCoefVal);
    }
    if(monFailRepCheck[9] > 0)
    {
            printf ("Error: Rx TX2 Ballbreak report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[9]);
            failedMonRepCnt = -1;
            printf("Rx TX2 Ballbreak report: errCode:0x%x, txReflCoefVal: %d \n",
                    gMonDataAeStrct.monTx2BallbreakRep.errorCode,
                    gMonDataAeStrct.monTx2BallbreakRep.txReflCoefVal);
    }

    if(monFailRepCheck[14] > 0)
    {
        printf ("Error: Synthesizeer freq report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[14]);
        failedMonRepCnt = -1;
    }

    if(monFailRepCheck[16] > 0)
    {
        printf ("Error: TX0 Internal Analog Signal report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[16]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[17] > 0)
    {
        printf ("Error: TX1 Internal Analog Signal report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[17]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[18] > 0)
    {
            printf ("Error: TX2 Internal Analog Signal report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[18]);
            failedMonRepCnt = -1;
    }

    if(monFailRepCheck[19] > 0)
    {
        printf ("Error: RX Internal Analog Signal report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[19]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[20] > 0)
    {
        printf ("Error: PMCLKLO Internal Analog Signal report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[20]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[21] > 0)
    {
        printf ("Error: GPADC Internal Analog Signal report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[21]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[22] > 0)
    {
        printf ("Error: PLL Control Volt report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[22]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[23] > 0)
    {
        printf ("Error: DCC CLK Freq report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[23]);
        failedMonRepCnt = -1;
    }

    if(monFailRepCheck[28] > 0)
    {
        printf ("Error: Run Time Calibration report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[28]);
        failedMonRepCnt = -1;
    }
    if(monFailRepCheck[29] > 0)
    {
        printf ("Error: Digital Periodic Monitoring report %d times Failed. Status Flag : 0x0 \n",monFailRepCheck[26]);
        failedMonRepCnt = -1;
    }

    return failedMonRepCnt;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set calib/monitoring configuration
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setCalMonConfig (void)
{
    int32_t         retVal;

    /* Set calib/monitoring time unit configuration */
    retVal = rlRfSetCalMonTimeUnitConfig(RL_DEVICE_MAP_INTERNAL_BSS,
                                     (rlRfCalMonTimeUntConf_t*)&calMonTimeUnitConf);
    if (retVal != 0)
    {
        printf ("Error: Unable to rlRfSetCalMonTimeUnitConfig [Error %d]\n", retVal);
        return -1;
    }
    /* Set calib/monitoring freq limits configuration */
    retVal = rlRfSetCalMonFreqLimitConfig(RL_DEVICE_MAP_INTERNAL_BSS,
                                     (rlRfCalMonFreqLimitConf_t*)&freqLimit);

    if (retVal != 0)
    {
        printf ("Error: Unable to rlRfSetCalMonFreqLimitConfig[Error %d]\n", retVal);
        return -1;
    }

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Init time calibration.
 *      using the mmWave link API.
 *
 *  @param  Enable/Disable Calibrations.
 *          1 - Enable all calibrations. 0 - Disable all calibrations
 *  @note Enable/disable calibrations based on your factory calibration config.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setInitTimeCalibConfig (int8_t enAllCalib, uint8_t platformType)
{
    int32_t         retVal;
    rlRfInitCalConf_t data = { 0 };
    if (enAllCalib == 1)
    {
            data.calibEnMask = 0x1FF0;

    }
    else
    {
        data.calibEnMask = 0x0;
    }
    /* Enable/Disable calibrations */
    retVal = rlRfInitCalibConfig(RL_DEVICE_MAP_INTERNAL_BSS, &data);

    if (retVal != 0)
    {
        printf ("Error: Unable to rlRfInitCalibConfig [Error %d]\n", retVal);
        return -1;
    }

    return 0;
}


/**
 *  @b Description
 *  @n
 *      The function is used to set run time calibration.
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRunTimeCalibConfig (void)
{
    int32_t         retVal;

    /* Set Run time calibration configuration */
    retVal = rlRfRunTimeCalibConfig(RL_DEVICE_MAP_INTERNAL_BSS,
                                (rlRunTimeCalibConf_t*)&runTimeCalib);

    if (retVal != 0)
    {
        printf ("Error: Unable to rlRfRunTimeCalibConfig [Error %d]\n", retVal);
        return -1;
    }

    while(gRunTimeCalibStatus == 0U)
    {
        /* Sleep and poll again: */
        ClockP_usleep(1);
    }
    gRunTimeCalibStatus = 0U;

    return 0;
}

/**
 *  @b Description
 *  @n
 *      The function is used to set Tx phase shifter DAC monitor
 *      using the mmWave link API.
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_setRfTXPhShifterDacMonConfig(void)
{
    int32_t retVal;
    rlMonTxPhShifterDacConf_t psDacData = {0};
    psDacData.profileId = 0;
    psDacData.reportMode = 2;
    psDacData.txSelMask = 0xF;
    psDacData.txPsDacMonThresh[0] = 5;
    psDacData.txPsDacMonThresh[1] = 5;
    psDacData.txPsDacMonThresh[2] = 5;
    psDacData.txPsDacMonThresh[3] = 5;
    retVal = rlRfMontxPhShiftDacConfig(RL_DEVICE_MAP_INTERNAL_BSS, &psDacData);

    if(retVal != 0)
    {
        printf ("Error: Unable to rlRfMontxPhShiftDacConfig[Error %d]\n", retVal);
        return -1;
    }

    printf ("Debug: Finished rlRfMontxPhShiftDacConfig configuration in BSS\n");
    return 0;
}

/**
 *  @b Description
 *  @n
 *      Print RF Monitoring Results
 *
 *  @retval
 *      Not applicable
 */
void MmwaveLink_printMonitoringResult()
{
    printf ("--------------Debug: Monitoring Report--------------\n");
    printf ("RL_RF_AE_MON_TEMPERATURE_REPORT_SB [0x%x] \n", gMonDataAeStrct.monTempReport.statusFlags);
    printf ("RL_RF_AE_MON_RX_GAIN_PHASE_REPORT [0x%x] \n", gMonDataAeStrct.monRxGainPhRep.statusFlags);
    printf ("RL_RF_AE_MON_RX_IF_STAGE_REPORT [0x%x] \n", gMonDataAeStrct.monRxIfStageRep.statusFlags);

    printf ("RL_RF_AE_MON_TX0_POWER_REPORT [0x%x] \n", gMonDataAeStrct.monTx0powRep.statusFlags);
    printf ("RL_RF_AE_MON_TX1_POWER_REPORT [0x%x] \n", gMonDataAeStrct.monTx1powRep.statusFlags);
    printf ("RL_RF_AE_MON_TX2_POWER_REPORT [0x%x] \n", gMonDataAeStrct.monTx2powRep.statusFlags);
    printf ("RL_RF_AE_MON_TX3_POWER_REPORT [0x%x] \n", gMonDataAeStrct.monTx3powRep.statusFlags);

    printf ("RL_RF_AE_MON_TX0_BALLBREAK_REPORT [0x%x] \n", gMonDataAeStrct.monTx0BallbreakRep.statusFlags);
    printf ("RL_RF_AE_MON_TX1_BALLBREAK_REPORT [0x%x] \n", gMonDataAeStrct.monTx1BallbreakRep.statusFlags);
    printf ("RL_RF_AE_MON_TX2_BALLBREAK_REPORT [0x%x] \n", gMonDataAeStrct.monTx2BallbreakRep.statusFlags);
    printf ("RL_RF_AE_MON_TX3_BALLBREAK_REPORT [0x%x] \n", gMonDataAeStrct.monTx3BallbreakRep.statusFlags);

    printf ("RL_RF_AE_MON_Adv_TX_GAIN_MISMATCH_REPORT [0x%x] \n", gMonDataAeStrct.monAdvTxGainPhMisMatchRep.statusFlags);

    printf ("RL_RF_AE_MON_SYNTHESIZER_FREQ_REPORT [0x%x] \n", gMonDataAeStrct.monSynthFreqRep.statusFlags);

    printf ("RL_RF_AE_MON_TX0_INT_ANA_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monTx0IntAnaSigRep.statusFlags);
    printf ("RL_RF_AE_MON_TX1_INT_ANA_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monTx1IntAnaSigRep.statusFlags);
    printf ("RL_RF_AE_MON_TX2_INT_ANA_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monTx2IntAnaSigRep.statusFlags);
    printf ("RL_RF_AE_MON_TX2_INT_ANA_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monTx3IntAnaSigRep.statusFlags);

    printf ("RL_RF_AE_MON_RX_INT_ANALOG_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monRxIntAnaSigRep.statusFlags);
    printf ("RL_RF_AE_MON_PMCLKLO_INT_ANA_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monPmClkIntAnaSigRep.statusFlags);
    printf ("RL_RF_AE_MON_GPADC_INT_ANA_SIG_REPORT [0x%x] \n", gMonDataAeStrct.monGpadcIntAnaSigRep.statusFlags);
    printf ("RL_RF_AE_MON_PLL_CONTROL_VOLT_REPORT [0x%x] \n", gMonDataAeStrct.monPllConvVoltRep.statusFlags);
    printf ("RL_RF_AE_MON_DCC_CLK_FREQ_REPORT [0x%x] \n", gMonDataAeStrct.monDccClkFreqRep.statusFlags);
    printf ("----------------------------------------------------\n");
}

/**
 *  @b Description
 *  @n
 *      RF Monitoring Enable/Configuration function. Enables all monitoring in
 *      Radar SS
 *
 *  @retval
 *      Success -   0
 *  @retval
 *      Error   -   <0
 */
int32_t MmwaveLink_monitoring(uint8_t platformType)
{
    /* Initialize monitoring counters */
    MmwaveLink_monitorReportCountInit();
    printf ("----------Debug: Monitoring Configuration-----------\n");
    /* Set mmWave digital monitor enable to the BSS */
    if (mmwaveLink_digMonEnableConfig(false, 0) < 0)
    {
        PRINT_LOG ("Set mmWave Link digital monitor Enable\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link digital monitor Enable\n", RESULT_PASS);
    /* Set mmWave digital monitor periodic config to the BSS */
    if (MmwaveLink_rlRfDigMonPeriodicConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link digital monitor periodic Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link digital monitor periodic Config\n", RESULT_PASS);

    /* Set mmWave temperature monitor config to the BSS */
    if (MmwaveLink_setRfTempMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link RF temperature monitor Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link RF temperature monitor Config\n", RESULT_PASS);

    /* Set mmWave RX gain and phase monitoring config to the BSS */
    if (MmwaveLink_setRfRxGainPhaMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link RF RX gain and phase monitor Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link RF RX gain and phase monitor Config\n", RESULT_PASS);

    /* Set mmWave RX If stage monitoring config to the BSS */
    if (MmwaveLink_setRfRxIfStageMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link RF RX IF stage monitor Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link RF RX IF stage monitor Config\n", RESULT_PASS);

    /* Set mmWave TX Power monitoring config to the BSS */
    if (MmwaveLink_setRfTxPowMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link Tx power monitor Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link RF Tx power monitor Config\n", RESULT_PASS);

    /* Set mmWave TX ball break monitoring config to the BSS */
    if (MmwaveLink_setRfTxBallbreakMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link Tx ballbreak monitor Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link RF Tx ballbreak monitor Config\n", RESULT_PASS);

    /* Set mmWave TX Gain Phase mismatch monitoring config to the BSS */ //not working
    if(platformType != RL_AR_DEVICETYPE_68XX)
    {
        if (MmwaveLink_setRfAdvTxGainPhaseMismatchMonConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link TX gain and phase mismatch monitor Config\n", RESULT_FAIL);
            return -1;
        }
        PRINT_LOG ("Set mmWave Link RF TX gain and phase mismatch monitor Config\n", RESULT_PASS);
    }

    if (platformType != RL_AR_DEVICETYPE_68XX)
    {
        /* Set mmWave Synth Frequency monitoring config to the BSS */
        if (MmwaveLink_setRfSynthFreqMonConfig() < 0)
        {
            PRINT_LOG ("Set mmWave Link Rf synth freq Config\n", RESULT_FAIL);
            return -1;
        }
        PRINT_LOG ("Set mmWave Link Rf synth freq Config\n", RESULT_PASS);
    }

    /* Set mmWave TX internal analog signal config to the BSS */
    if (MmwaveLink_setRfTxIntAnaSignalMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link Rf TX internal analog signal Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link Rf TX internal analog signal Config\n", RESULT_PASS);

    /* Set mmWave RXinternal analog signal config to the BSS */
    if (MmwaveLink_setRfRxIntAnaSignalMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link Rf RX internal analog signal Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link Rf RX internal analog signal Config\n", RESULT_PASS);

    /* Set mmWave PMCLKLO internal analog signal monitoring config to the BSS */
    if (MmwaveLink_setRfPmClkLoIntAnaSignalsMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link Rf pm,clock  Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link Rf pm,clock Config\n", RESULT_PASS);

    /* Set mmWave GPADC internal analog signal monitoring config to the BSS */
    if (MmwaveLink_setRfGpadcIntAnaSignalsMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link GPADC Internal Analog Signals Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link GPADC Internal Analog Signals Config\n", RESULT_PASS);

    /* Set mmWave Pll control volt monitoring config to the BSS */
    if (MmwaveLink_setRfPllContrlVoltMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link MmwaveLink_setRfPllContrlVoltMonConfig Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link MmwaveLink_setRfPllContrlVoltMonConfig Config\n", RESULT_PASS);

    /* Set mmWave Dual Clock monitoring config to the BSS */
    if (MmwaveLink_setRfDualClkCompMonConfig() < 0)
    {
        PRINT_LOG ("Set mmWave Link Dual Clk Config\n", RESULT_FAIL);
        return -1;
    }
    PRINT_LOG ("Set mmWave Link Dual Clk Config\n", RESULT_PASS);

    /* Set mmWave Analog monitor config to the BSS */
    if (MmwaveLink_setRfAnaMonConfig(platformType, false, 0) < 0)
    {
        PRINT_LOG ("Set mmWave Link RF analog monitor Config\n", RESULT_FAIL);
        return -1;
    }

    printf ("----------------------------------------------------\n");
    return 0;

}
