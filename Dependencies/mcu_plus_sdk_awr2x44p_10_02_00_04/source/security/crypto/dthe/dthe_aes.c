/*
 *  Copyright (C) 2022-25 Texas Instruments Incorporated
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
 *  \file   dthe_aes.c
 *
 *  \brief  This file contains the implementation of Dthe aes driver
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <security/crypto/dthe/dthe_aes.h>
#include <security/crypto/dthe/dthe_edma.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */
/** This is the max value of datasize taken in case of streaming mode */
#define     MAX_VALUE                   (0xFFFFFFFFU)

/** This is the state of on ongoing stream state */
#define     AES_STATE_NEW               (0x00U)

/** This is the state of a stream in progress */
#define     AES_STATE_IN_PROGRESS       (0xA5U)

/** Max length for AAD data in CCM Mode */
#define     AES_CCM_AAD_MAX_LEN         (0xFF00U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

static uint8_t gStreamState = AES_STATE_NEW;

/* ========================================================================== */
/*                 Internal Function Declarations                             */
/* ========================================================================== */

static void DTHE_AES_setDMAContextStatus(CSL_AesRegs *ptrAesRegs, uint8_t dmaStatus);
static void DTHE_AES_setDMAOutputRequestStatus(CSL_AesRegs *ptrAesRegs, uint8_t dmaStatus);
static void DTHE_AES_setDMAInputRequestStatus(CSL_AesRegs *ptrAesRegs, uint8_t dmaStatus);
static void DTHE_AES_setKeySize(CSL_AesRegs *ptrAesRegs, uint8_t size);
static void DTHE_AES_set256BitKey1(CSL_AesRegs *ptrAesRegs, const uint32_t* ptrKey);
static void DTHE_AES_setIV(CSL_AesRegs *ptrAesRegs, uint32_t ivSize, const uint32_t* ptrIV);
static void DTHE_AES_set128BitKey2Part1(CSL_AesRegs *ptrAesRegs, const uint32_t* ptrKey);
static void DTHE_AES_set128BitKey2Part2(CSL_AesRegs *ptrAesRegs, const uint32_t* ptrKey);
static void DTHE_AES_pollInputReady(CSL_AesRegs *ptrAesRegs);
static void DTHE_AES_writeDataBlock(CSL_AesRegs *ptrAesRegs, const uint32_t* ptrData);
static void DTHE_AES_pollOutputReady(CSL_AesRegs *ptrAesRegs);
static void DTHE_AES_pollSaveContextReady(CSL_AesRegs *ptrAesRegs);
static void DTHE_AES_readDataBlock(CSL_AesRegs *ptrAesRegs, uint32_t* ptrData);
static void DTHE_AES_resetModule(CSL_AesRegs *ptrAesRegs);
static void DTHE_AES_controlMode(CSL_AesRegs *ptrAesRegs, uint32_t algoType);
static void DTHE_AES_setOpType(CSL_AesRegs *ptrAesRegs, uint32_t opType);
static void DTHE_AES_saveContextTopFunction(CSL_AesRegs *ptrAesRegs,const DTHE_AES_Params* ptrParams);
static void DTHE_AES_CTRWidth(CSL_AesRegs *ptrAesRegs, uint32_t ctrWidth);
static void DTHE_AES_xtsUpdateIv(CSL_AesRegs *ptrAesRegs,const DTHE_AES_Params* ptrParams, uint32_t *newIv, Bool isFirstBlock);
static inline void DTHE_AES_setDataLengthBytes(CSL_AesRegs *ptrAesRegs, uint32_t dataLenBytes);
static inline void DTHE_AES_setCcmTagLen(CSL_AesRegs *ptrAesRegs, uint32_t len);
static inline void DTHE_AES_setCcmLenWidth(CSL_AesRegs *ptrAesRegs, uint32_t width);
static inline void DTHE_AES_setAADLengthBytes(CSL_AesRegs *ptrAesRegs, uint32_t aadLenBytes);
static inline void DTHE_AES_readIV(CSL_AesRegs *ptrAesRegs, uint32_t* ivReg);
static inline void DTHE_AES_readTag(CSL_AesRegs *ptrAesRegs, uint32_t* ptrTag);
static inline void DTHE_AES_clearKey2Part1(CSL_AesRegs *ptrAesRegs);
static inline void DTHE_AES_clearKey2Part2(CSL_AesRegs *ptrAesRegs);
static inline void DTHE_AES_clearIV(CSL_AesRegs *ptrAesRegs);
static inline void DTHE_AES_clearAllInterrupts(CSL_AesRegs *ptrAesRegs);

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void DTHE_AES_resetModule(CSL_AesRegs *ptrAesRegs)
{
	/* Reset the AES and ensure it is out of reset */
    CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG, AES_S_SYSCONFIG_SOFTRESET, 1U);
    /* TODO: Timeout implementation */
    while (CSL_REG32_FEXT(&ptrAesRegs->SYSSTS, AES_S_SYSSTS_RESETDONE) == 0U)
    {
    }
}

static void DTHE_AES_controlMode(CSL_AesRegs *ptrAesRegs, uint32_t algoType)
{
	if(algoType == DTHE_AES_ECB_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_ECB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_CBC_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_CBC);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_CTR_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_CTR);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_ECB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_ICM_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_ICM);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_ECB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_CFB_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_CFB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_ECB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_F8_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_F8);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_ECB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_F9_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_F9);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_ECB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_XTS_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_NOOP);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_ECB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if((algoType == DTHE_AES_CBC_MAC_MODE)||(algoType == DTHE_AES_CMAC_MODE))
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_CBCMAC);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_ECB);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_GCM_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_NOOP);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_CTR);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_RESETVAL);
    }
    else if(algoType == DTHE_AES_CCM_MODE)
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CCM, CSL_AES_S_CTRL_CCM_CCM);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR, CSL_AES_S_CTRL_CTR_CTR);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CBCMAC, CSL_AES_S_CTRL_CBCMAC_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_MODE, CSL_AES_S_CTRL_MODE_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_ICM, CSL_AES_S_CTRL_ICM_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CFB, CSL_AES_S_CTRL_CFB_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F8, CSL_AES_S_CTRL_F8_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_F9, CSL_AES_S_CTRL_F9_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, CSL_AES_S_CTRL_XTS_RESETVAL);
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_GCM, CSL_AES_S_CTRL_GCM_RESETVAL);
    }
}

static void DTHE_AES_setIVTopFunction(CSL_AesRegs *ptrAesRegs, const DTHE_AES_Params *ptrParams)
{
    /* Clear the IV value in registers */
    DTHE_AES_clearIV(ptrAesRegs);

    /* Configure the Initialization Vector: */
    if ((ptrParams->algoType == DTHE_AES_CBC_MODE)||
        (ptrParams->algoType == DTHE_AES_CFB_MODE)||
        (ptrParams->algoType == DTHE_AES_XTS_MODE))
    {
        DTHE_AES_setIV(ptrAesRegs, 128U, ptrParams->ptrIV);
    }

    else if(ptrParams->algoType == DTHE_AES_CTR_MODE)
    {
        DTHE_AES_CTRWidth(ptrAesRegs, ptrParams->counterWidth);
        DTHE_AES_setIV(ptrAesRegs, 128U, ptrParams->ptrIV);
    }

    else if (ptrParams->algoType == DTHE_AES_ICM_MODE)
    {
        DTHE_AES_CTRWidth(ptrAesRegs, DTHE_AES_CTR_WIDTH_16);
        DTHE_AES_setIV(ptrAesRegs, 128U, ptrParams->ptrIV);
        ptrAesRegs->IV_IN_3 &= 0x0100FFFFU;
    }

    else if(ptrParams->algoType == DTHE_AES_CCM_MODE)
    {
        DTHE_AES_CTRWidth(ptrAesRegs, DTHE_AES_CTR_WIDTH_32);
        DTHE_AES_setIV(ptrAesRegs, 96U, ptrParams->ptrIV);
    }

    else if(ptrParams->algoType == DTHE_AES_GCM_MODE)
    {
        DTHE_AES_CTRWidth(ptrAesRegs, DTHE_AES_CTR_WIDTH_32);
        DTHE_AES_setIV(ptrAesRegs, 96U, ptrParams->ptrIV);
        ptrAesRegs->IV_IN_3 = 0x01000000U;
    }

}

static void DTHE_AES_saveContextTopFunction(CSL_AesRegs *ptrAesRegs,const DTHE_AES_Params *ptrParams)
{
    if((ptrParams->algoType == DTHE_AES_GCM_MODE)||
       (ptrParams->algoType == DTHE_AES_CCM_MODE)||
       (ptrParams->algoType == DTHE_AES_CBC_MAC_MODE)||
       (ptrParams->algoType == DTHE_AES_CMAC_MODE) ||
       (ptrParams->algoType == DTHE_AES_F9_MODE))
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_SAVE_CONTEXT, 1U);
    }
    else
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_SAVE_CONTEXT, 0U);
    }
}

/**
 * \brief                   The function is clear interrupts
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers.
 *
 * \param   status          Enable Interrupts
 *
 */
static void DTHE_AES_clearAllInterrupts(CSL_AesRegs *ptrAesRegs)
{
    ptrAesRegs->IRQEN = 0x0;
}

static void DTHE_AES_CTRWidth(CSL_AesRegs *ptrAesRegs, uint32_t ctrWidth)
{
    uint32_t ctrWidthRegValue = 3U;

	if(ctrWidth == DTHE_AES_CTR_WIDTH_32)
    {
        ctrWidthRegValue = CSL_AES_S_CTRL_CTR_WIDTH_COUNTER32;
    }
    else if(ctrWidth == DTHE_AES_CTR_WIDTH_64)
    {
        ctrWidthRegValue = CSL_AES_S_CTRL_CTR_WIDTH_COUNTER64;
    }
    else if(ctrWidth == DTHE_AES_CTR_WIDTH_96)
    {
        ctrWidthRegValue = CSL_AES_S_CTRL_CTR_WIDTH_COUNTER96;
    }
    else if(ctrWidth == DTHE_AES_CTR_WIDTH_128)
    {
        ctrWidthRegValue = CSL_AES_S_CTRL_CTR_WIDTH_COUNTER128;
    }

    CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_CTR_WIDTH, ctrWidthRegValue);
}

static void DTHE_AES_setOpType(CSL_AesRegs *ptrAesRegs, uint32_t opType)
{
	/* Operation Mode: Encryption/Decryption */
   if (opType == DTHE_AES_ENCRYPT)
   {
       /* Encryption: */
       CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_DIRECTION, 1U);
   }
   else
   {
       /* Decryption: */
       CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_DIRECTION, 0U);
   }

}

static inline void DTHE_AES_setDataLengthBytes(CSL_AesRegs *ptrAesRegs, uint32_t dataLenBytes)
{
	/* Setup the data length: */
    CSL_REG32_FINS(&ptrAesRegs->C_LENGTH_0, AES_S_C_LENGTH_0_LENGTH, dataLenBytes);
}

static inline void DTHE_AES_setAADLengthBytes(CSL_AesRegs *ptrAesRegs, uint32_t aadLenBytes)
{
	/* Setup the AAD data length: */
    CSL_REG32_FINS(&ptrAesRegs->AUTH_LENGTH, AES_S_AUTH_LENGTH_AUTH, aadLenBytes);
}

static inline void  DTHE_AES_setCcmTagLen(CSL_AesRegs *ptrAesRegs, uint32_t len)
{
    /* Set Authentification field length (CCM-M) for CCM operation*/
    CSL_REG32_FINS(&ptrAesRegs->CTRL,AES_S_CTRL_CCM_M,((len/2)-1));
}

static inline void  DTHE_AES_setCcmLenWidth(CSL_AesRegs *ptrAesRegs, uint32_t width)
{
    /* Set width of the length field (CCM-L) for CCM operations*/
    CSL_REG32_FINS(&ptrAesRegs->CTRL,AES_S_CTRL_CCM_L,(width-1));
}

static inline void DTHE_AES_clearKey2Part1(CSL_AesRegs *ptrAesRegs)
{
    /* Clear Key2 [3:0] register fields*/
    ptrAesRegs->KEY2_0 = 0U;
    ptrAesRegs->KEY2_1 = 0U;
    ptrAesRegs->KEY2_2 = 0U;
    ptrAesRegs->KEY2_3 = 0U;
}

static inline void DTHE_AES_clearKey2Part2(CSL_AesRegs *ptrAesRegs)
{
    /* Clear Key3/ Key2[7:4] register fields*/
    ptrAesRegs->KEY2_4 = 0U;
    ptrAesRegs->KEY2_5 = 0U;
    ptrAesRegs->KEY2_6 = 0U;
    ptrAesRegs->KEY2_7 = 0U;
}

static void DTHE_AES_xtsUpdateIv(CSL_AesRegs *ptrAesRegs,const DTHE_AES_Params* ptrParams, uint32_t *newIv, Bool isFirstBlock)
{
    if(isFirstBlock == TRUE)
    {
       DTHE_AES_set256BitKey1(ptrAesRegs, ptrParams->ptrKey);

       if( ptrParams->ptrKey1 != NULL)
       {
           DTHE_AES_set128BitKey2Part1(ptrAesRegs, ptrParams->ptrKey1);
       }

       if( ptrParams->ptrKey2 != NULL)
       {
           DTHE_AES_set128BitKey2Part2(ptrAesRegs, ptrParams->ptrKey2);
       }

        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, ptrParams->xtsModeSelect);
    }
    else
    {
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_XTS, DTHE_AES_XTS_MODE_1);
    }

    DTHE_AES_setIV(ptrAesRegs, 128U, newIv);

}

DTHE_AES_Return_t DTHE_AES_open(DTHE_Handle handle)
{
    DTHE_AES_Return_t status  = DTHE_AES_RETURN_FAILURE;
    DTHE_Config       *config = NULL;
    DTHE_Attrs        *attrs  = NULL;
    CSL_AesRegs       *ptrAesRegs;

    if(NULL != handle)
    {
        status  = DTHE_AES_RETURN_SUCCESS;
    }

    if(status  == DTHE_AES_RETURN_SUCCESS)
    {
        config          = (DTHE_Config *) handle;
        attrs           = config->attrs;
        ptrAesRegs      = (CSL_AesRegs *)attrs->aesBaseAddr;

        gStreamState = AES_STATE_NEW;

        /* Soft-Reset AES Module */
		DTHE_AES_resetModule(ptrAesRegs);

        /* Disable the DMA for the AES */
        DTHE_AES_setDMAContextStatus(ptrAesRegs, 0);
        DTHE_AES_setDMAOutputRequestStatus(ptrAesRegs, 0);
        DTHE_AES_setDMAInputRequestStatus(ptrAesRegs, 0);

        /* Disable Save Context */
        CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_SAVE_CONTEXT, 0U);

        /*Clear all interrupts*/
        DTHE_AES_clearAllInterrupts(ptrAesRegs);

    }

    return (status);
}

DTHE_AES_Return_t DTHE_AES_execute(DTHE_Handle handle, const DTHE_AES_Params* ptrParams)
{
    DTHE_AES_Return_t status  = DTHE_AES_RETURN_FAILURE;
    DTHE_Config       *config = NULL;
    DTHE_Attrs        *attrs  = NULL;
    CSL_AesRegs     *ptrAesRegs;
    uint32_t*       ptrWordInputBuffer;
    uint32_t*       ptrWordOutputBuffer;
    uint32_t        dataLenWords;
    uint32_t        numBlocks;
    uint32_t        partialDataSize;
    uint32_t        index;
    uint32_t        numBytes = 0U;
    uint8_t         inPartialBlock[32U];
    uint8_t         outPartialBlock[32U];
    uint8_t         tempData[16] = {0};
    uint32_t        tempIV[4] = {0};
    uint8_t         *ptrByteBuf = NULL;

    if (NULL != handle)
    {
        status  = DTHE_AES_RETURN_SUCCESS;
    }

    /*AES Stream is not supported for CCM, GCM, F8, F9 and XTS modes*/
    if((ptrParams->streamState != DTHE_AES_ONE_SHOT_SUPPORT) && \
       ((ptrParams->algoType == DTHE_AES_CCM_MODE) || (ptrParams->algoType == DTHE_AES_GCM_MODE)|| \
       (ptrParams->algoType == DTHE_AES_F8_MODE) || (ptrParams->algoType == DTHE_AES_F9_MODE)|| \
       (ptrParams->algoType == DTHE_AES_XTS_MODE)))
    {
        status  = DTHE_AES_RETURN_FAILURE;
    }

    if (status  == DTHE_AES_RETURN_SUCCESS)
    {
        config          = (DTHE_Config *) handle;
        attrs           = config->attrs;
        ptrAesRegs      = (CSL_AesRegs *)attrs->aesBaseAddr;

        /* This flow is for One-Shot mode and Stream Mode as INIT only */
        if(((ptrParams->streamState == DTHE_AES_ONE_SHOT_SUPPORT)||(ptrParams->streamState == DTHE_AES_STREAM_INIT))&&\
            (gStreamState == AES_STATE_NEW))
        {
            DTHE_AES_controlMode(ptrAesRegs, ptrParams->algoType);

            /*Update mode selection for GCM if provided, else set to default mode-3*/
            if(ptrParams->algoType == DTHE_AES_GCM_MODE)
            {
                if(ptrParams->gcmModeSelect != 0)
                {
                    CSL_REG32_FINS(&ptrAesRegs->CTRL,AES_S_CTRL_GCM,ptrParams->gcmModeSelect);
                }
                else
                {
                    /*Default mode value set to Mode-3*/
                    CSL_REG32_FINS(&ptrAesRegs->CTRL,AES_S_CTRL_GCM,DTHE_AES_GCM_MODE_3);
                }
            }

            /*Update additional mode selection for XTS if provided, else set to default value*/
            if(ptrParams->algoType == DTHE_AES_XTS_MODE)
            {
                if(ptrParams->xtsModeSelect != 0)
                {
                    CSL_REG32_FINS(&ptrAesRegs->CTRL,AES_S_CTRL_XTS,ptrParams->xtsModeSelect);
                }
                else
                {
                    /*Default mode value set to Mode-3*/
                    CSL_REG32_FINS(&ptrAesRegs->CTRL,AES_S_CTRL_XTS,DTHE_AES_XTS_MODE_3);
                }
            }

            /* Update CMM-L (Length of width of CCM operation) and CCM-M (Tag Length) for CCM operation*/
            if(ptrParams->algoType == DTHE_AES_CCM_MODE)
            {
                /*Sanity Check: Ensure Valid width of the length field */
                if((ptrParams->ccmLenWidth != 2U)&&
                   (ptrParams->ccmLenWidth != 4U)&&
                   (ptrParams->ccmLenWidth != 8U))
                {
                    status = DTHE_AES_RETURN_FAILURE;
                }
                /*Sanity Check: Ensure Valid Tag Length*/
                else if((ptrParams->ccmTagLen < 4U)||
                        (ptrParams->ccmTagLen > 16U)||
                        (ptrParams->ccmTagLen % 2U) != 0U)
                {
                    status = DTHE_AES_RETURN_FAILURE;
                }
                else
                {
                    /*Update CMM-L (Length of width of CCM operation) for CCM operation*/
                    DTHE_AES_setCcmLenWidth(ptrAesRegs, ptrParams->ccmLenWidth);

                    /*Update CCM-M (Tag Length) for CCM operation*/
                    DTHE_AES_setCcmTagLen(ptrAesRegs, ptrParams->ccmTagLen);
                }
            }

            /* Key Size setting */
            DTHE_AES_setKeySize(ptrAesRegs, ptrParams->keyLen);

            if((ptrParams->streamState == DTHE_AES_ONE_SHOT_SUPPORT)&&(ptrParams->dataLenBytes == 0U))
            {
                if(((ptrParams->algoType == DTHE_AES_CCM_MODE) || (ptrParams->algoType == DTHE_AES_GCM_MODE)) && (ptrParams->aadLenBytes != 0U))
                {
                    /*Valid option*/
                }
                else
                {
                    status = DTHE_AES_RETURN_FAILURE;
                }
            }

            /* Sanity Check: For Decryption data length always needs to be aligned to 16byte for block cipher algo*/
            if ((ptrParams->opType == DTHE_AES_DECRYPT) &&
                ((ptrParams->opType == DTHE_AES_ECB_MODE)||
                 (ptrParams->opType == DTHE_AES_CBC_MODE)))
            {
                if ((ptrParams->dataLenBytes % 16U) != 0U)
                {
                    status = DTHE_AES_RETURN_FAILURE;
                }
            }

            /* Sanity Check: Key Validation */
            if (status == DTHE_AES_RETURN_SUCCESS)
            {
                /* KEK Mode or Normal Key Mode: */
                if (ptrParams->useKEKMode == TRUE)
                {
                    /* If KEKMode is set, configure Muxes for KEK
                    * to be passed to AES Engine */
                    /* KEK Mode: Enable Direct Bus */
                    CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG,AES_S_SYSCONFIG_DIRECTBUSEN,1U);
                }
                else
                {
                    /* Normal Mode: Key should always be specified */
                    if (ptrParams->ptrKey == NULL)
                    {
                        status = DTHE_AES_RETURN_FAILURE;
                    }
                }

                if((ptrParams->algoType == DTHE_AES_CBC_MODE)\
                    ||(ptrParams->algoType == DTHE_AES_CTR_MODE)\
                    ||(ptrParams->algoType == DTHE_AES_ICM_MODE)\
                    ||(ptrParams->algoType == DTHE_AES_CFB_MODE)\
                    ||(ptrParams->algoType == DTHE_AES_GCM_MODE)\
                    ||(ptrParams->algoType == DTHE_AES_CCM_MODE)\
                    ||(ptrParams->algoType == DTHE_AES_XTS_MODE))

                {
                    if (ptrParams->ptrIV == NULL)
                    {
                        status = DTHE_AES_RETURN_FAILURE;
                    }
                }

                /* Select the key input: */
                if (ptrParams->useKEKMode == FALSE)
                {
                    /* Normal Key Mode: */
                    CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG,AES_S_SYSCONFIG_DIRECTBUSEN,0U);

                    /* Clear KEY2 (KEY2_PART1) and KEY3 (KEY2_PART2) registers*/
                    DTHE_AES_clearKey2Part1(ptrAesRegs);
                    DTHE_AES_clearKey2Part2(ptrAesRegs);

                    /* Normal Mode: Key should always be specified */
                    if (ptrParams->ptrKey != NULL)
                    {
                        /* Configure the key which is to be used: */
                        DTHE_AES_set256BitKey1 (ptrAesRegs, ptrParams->ptrKey);
                    }

                    if ((ptrParams->algoType == DTHE_AES_CMAC_MODE)
                        ||(ptrParams->algoType == DTHE_AES_F8_MODE))
                    {
                        DTHE_AES_set128BitKey2Part1(ptrAesRegs, ptrParams->ptrKey1);
                        DTHE_AES_set128BitKey2Part2(ptrAesRegs, ptrParams->ptrKey2);
                    }

                    if(ptrParams->algoType == DTHE_AES_XTS_MODE)
                    {
                        if((ptrParams->xtsModeSelect == DTHE_AES_XTS_MODE_2)
                           ||(ptrParams->xtsModeSelect == DTHE_AES_XTS_MODE_3))
                        {
                            DTHE_AES_set128BitKey2Part1(ptrAesRegs, ptrParams->ptrKey1);
                            if( ptrParams->ptrKey2 != NULL)
                            {
                                DTHE_AES_set128BitKey2Part2(ptrAesRegs, ptrParams->ptrKey2);
                            }
                        }
                    }

                    if((ptrParams->algoType == DTHE_AES_F9_MODE) ||
                      ((ptrParams->algoType == DTHE_AES_GCM_MODE) &&
                      ((ptrParams->gcmModeSelect == DTHE_AES_GCM_MODE_1)||
                       (ptrParams->gcmModeSelect == DTHE_AES_GCM_MODE_2 ))))

                    {
                        DTHE_AES_set128BitKey2Part1(ptrAesRegs, ptrParams->ptrKey1);
                    }

                }

                if(ptrParams->algoType == DTHE_AES_F9_MODE)
                {
                    CSL_REG32_FINS(&ptrAesRegs->CTRL, AES_S_CTRL_DIRECTION, ptrParams->f9Direction);
                }
                else
                {
                    DTHE_AES_setOpType(ptrAesRegs, ptrParams->opType);
                }

                /*Sanity Check: Ensure Valid aad length field*/
                if((ptrParams->algoType == DTHE_AES_CCM_MODE)\
                    ||(ptrParams->algoType == DTHE_AES_GCM_MODE)\
                    ||(ptrParams->algoType == DTHE_AES_XTS_MODE))
                {
                    if((ptrParams->algoType == DTHE_AES_CCM_MODE)\
                    &&(ptrParams->aadLenBytes > AES_CCM_AAD_MAX_LEN))
                    {
                        status = DTHE_AES_RETURN_FAILURE;
                    }
                }


                /* Configure the Initialization Vector and CTR for counter modes*/
                DTHE_AES_setIVTopFunction(ptrAesRegs,ptrParams);

                DTHE_AES_saveContextTopFunction(ptrAesRegs,ptrParams);

                /*
                - DataLength is sent by user, then set the same here.
                - DataLength is not sent by user, then set the length as maximum. */
                if((ptrParams->streamState != DTHE_AES_ONE_SHOT_SUPPORT)&&(ptrParams->dataLenBytes == 0))
                {
                    /* Setup the data length as 0xFFFFFFFF */
                    DTHE_AES_setDataLengthBytes(ptrAesRegs, MAX_VALUE);
                }
                else
                {
                    /* Setup the data length: */
                    DTHE_AES_setDataLengthBytes(ptrAesRegs, ptrParams->dataLenBytes);
                }

                if(ptrParams->algoType == DTHE_AES_XTS_MODE)
                {
                    /* Setup the AAD length: */
                    DTHE_AES_setAADLengthBytes(ptrAesRegs, ptrParams->aadLenBytes);
                }

                gStreamState = AES_STATE_IN_PROGRESS;
            }
        }
        /* Stream Mode Update should support streamSize aligned to 16B only */
        else if((gStreamState == AES_STATE_IN_PROGRESS)&&\
                ((ptrParams->streamState == DTHE_AES_STREAM_UPDATE)||(ptrParams->streamState == DTHE_AES_STREAM_FINISH)))
        {
            if ((ptrParams->streamState == DTHE_AES_STREAM_UPDATE)&&((ptrParams->streamSize % 16U) != 0U))
            {
                status = DTHE_AES_RETURN_FAILURE;
            }
        }
        else
        {
            status = DTHE_AES_RETURN_FAILURE;
        }

        /* Execute the AES Driver: */
        if ((status == DTHE_AES_RETURN_SUCCESS)&&(gStreamState == AES_STATE_IN_PROGRESS) )
        {
            /* This flow is for one-shot in continuation to the above flow
               In case of Update and Finish start execution from here */
            if((ptrParams->streamState == DTHE_AES_ONE_SHOT_SUPPORT)||(ptrParams->streamState == DTHE_AES_STREAM_UPDATE)||(ptrParams->streamState == DTHE_AES_STREAM_FINISH))
            {
                if((ptrParams->streamState == DTHE_AES_STREAM_FINISH)&&(ptrParams->dataLenBytes == 0U))
                {
                    /* Setup the data length: */
                    DTHE_AES_setDataLengthBytes(ptrAesRegs,  ptrParams->streamSize);
                }

                /*Setup Aad data*/
                if((ptrParams->algoType == DTHE_AES_GCM_MODE)||(ptrParams->algoType == DTHE_AES_CCM_MODE))
                {
                    /*Send AAD Data*/
                    if((ptrParams->aadLenBytes%4U)==0U)
                    {
                        DTHE_AES_setAADLengthBytes(ptrAesRegs, ptrParams->aadLenBytes);
                        if(ptrParams->aadLenBytes>0U)
                        {
                            ptrWordInputBuffer  = &ptrParams->ptrAadData[0];
                            numBlocks = (ptrParams->aadLenBytes)/16;
                            partialDataSize = (ptrParams->aadLenBytes)%16;

#if (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'M')
                            /* DTHE-EDMA is only supported by HSM (M4) core*/
                            if((gDtheConfig->attrs->dmaEnable == TRUE) && (gDtheConfig->attrs->aesdmaEnable == TRUE) && (numBlocks > 0U))
                            {
                                int32_t             edmaStatus = SystemP_FAILURE;
                                DTHE_AES_clearAllInterrupts(ptrAesRegs);
                                ptrWordInputBuffer = (uint32_t*)((uint32_t)0x20020000 + (uint32_t)ptrWordInputBuffer);
                                edmaStatus = DTHE_edmaChannelTxConfig((DTHE_Handle)config, gDtheParams.edmaInst);

                                edmaStatus = DTHE_edmaChannelTxParam((DTHE_Handle)config, ptrWordInputBuffer, (uint32_t *)&ptrAesRegs->DATA_IN_3, numBlocks, 0U, 1);
                                edmaStatus = DTHE_edmaChannelTxEnableTransferRegion((DTHE_Handle)config);
                                DTHE_AES_setDMAInputRequestStatus(ptrAesRegs, 1);
                                edmaStatus = DTHE_edmaChannelWaitForTxTransfer((DTHE_Handle)config);

                                DTHE_AES_setDMAInputRequestStatus(ptrAesRegs, 0);
                                edmaStatus = DTHE_edmaDisableTxChannel((DTHE_Handle)config);
                                if(edmaStatus == SystemP_FAILURE)
                                {
                                    status = DTHE_AES_RETURN_FAILURE;
                                }
                            }
                            else
#endif
                            {
                                /* Cycle through and write all the full blocks: */
                                for (index = 0U; index < (numBlocks); index++)
                                {
                                    /* Wait for the AES IP to be ready to receive the data: */
                                    DTHE_AES_pollInputReady(ptrAesRegs);

                                    /* Write the data: */
                                    DTHE_AES_writeDataBlock(ptrAesRegs, &ptrWordInputBuffer[index << 2U]);
                                }
                            }
                            if(partialDataSize != 0)
                            {
                                memset(inPartialBlock,0u,sizeof(inPartialBlock));
                                memcpy(inPartialBlock,&ptrWordInputBuffer[numBlocks*4],partialDataSize);

                                /* Wait for the AES IP to be ready to receive the data: */
                                DTHE_AES_pollInputReady(ptrAesRegs);
                                /* Write the data: */
                                DTHE_AES_writeDataBlock(ptrAesRegs, (uint32_t*)&inPartialBlock[0]);
                            }
                        }
                    }
                }

                /* Setup the input & output: */
                if (ptrParams->algoType == DTHE_AES_F9_MODE)
                {
                    /* F9 Tag generation: Plain Text is the Input Buffer
                     * Output is stored in temporay buffer which is discarded later as only tag needed
                     */
                    ptrWordInputBuffer  = &ptrParams->ptrPlainTextData[0];
                    ptrWordOutputBuffer = (uint32_t*)&tempData[0];
                }
                else
                {
                    if (ptrParams->opType == DTHE_AES_ENCRYPT)
                    {
                        /* Encryption: Plain Text is the Input & Encrypted Data is the Output */
                        ptrWordInputBuffer  = &ptrParams->ptrPlainTextData[0];
                        ptrWordOutputBuffer = &ptrParams->ptrEncryptedData[0];
                    }
                    else
                    {
                        /* Decryption: Encrypted Data is the Input & Plain Text is the Output */
                        ptrWordInputBuffer  = &ptrParams->ptrEncryptedData[0];
                        ptrWordOutputBuffer = &ptrParams->ptrPlainTextData[0];
                    }
                }

                if(ptrParams->streamState == DTHE_AES_ONE_SHOT_SUPPORT)
                {
                    /* Determine the data length in words: */
                    dataLenWords = ptrParams->dataLenBytes / 4U;
                    /* Compute the number of bytes which need to be handled seperately */
                    partialDataSize = ptrParams->dataLenBytes % 16U;
                }
                else
                {
                    /* Determine the data length in words: */
                    dataLenWords = ptrParams->streamSize / 4U;
                    /* Compute the number of bytes which need to be handled seperately */
                    partialDataSize = ptrParams->streamSize % 16U;
                }

                /* Compute the number of full blocks which can be written: Each block is 4words long*/
                numBlocks = (dataLenWords / 4U);

#if (__ARM_ARCH == 7) && (__ARM_ARCH_PROFILE == 'M')
                /* DTHE-EDMA is only supported by HSM (M4) core*/
                if((gDtheConfig->attrs->dmaEnable == TRUE) && (gDtheConfig->attrs->aesdmaEnable == TRUE) && (numBlocks > 0U))
                {
                    DTHE_AES_clearAllInterrupts(ptrAesRegs);
                    DTHE_edmaChannelFree(handle);
                    DTHE_Config     *config = NULL;
                    config = &gDtheConfig[0];
                    int32_t   edmaStatus = SystemP_FAILURE;
                    uint32_t xtsNumBlock = numBlocks;

                    if((ptrParams->algoType == DTHE_AES_XTS_MODE) && (ptrParams->opType == DTHE_AES_DECRYPT) && (partialDataSize != 0U))
                    {
                        /*CTS is required in XTS decryption if partial data present, so process with edma till second-last complete block only
                         */
                        numBlocks-- ;
                    }

                    if(numBlocks > 0U)  /*Ensure numblock is non-zero*/
                    {
                        edmaStatus = DTHE_edmaChannelTxConfig((DTHE_Handle)config, gDtheParams.edmaInst);

                        if((ptrParams->algoType != DTHE_AES_CBC_MAC_MODE)&&(ptrParams->algoType != DTHE_AES_CMAC_MODE)&&(ptrParams->algoType != DTHE_AES_F9_MODE))
                        {
                            edmaStatus = DTHE_edmaChannelRxConfig((DTHE_Handle)config, gDtheParams.edmaInst);
                        }

                        DTHE_AES_clearAllInterrupts(ptrAesRegs);

                        edmaStatus = DTHE_edmaChannelTxParam((DTHE_Handle)config, ptrWordInputBuffer, (uint32_t *)&ptrAesRegs->DATA_IN_3, numBlocks, 0U, 1);

                        if((ptrParams->algoType != DTHE_AES_CBC_MAC_MODE)&&(ptrParams->algoType != DTHE_AES_CMAC_MODE)&&(ptrParams->algoType != DTHE_AES_F9_MODE))
                        {
                            edmaStatus = DTHE_edmaChannelRxParam((DTHE_Handle)config, (uint32_t *)&ptrAesRegs->DATA_IN_3, ptrWordOutputBuffer, numBlocks,  0U, 1);
                        }

                        if((ptrParams->algoType != DTHE_AES_CBC_MAC_MODE)&&(ptrParams->algoType != DTHE_AES_CMAC_MODE)&&(ptrParams->algoType != DTHE_AES_F9_MODE))
                        {
                            DTHE_AES_setDMAOutputRequestStatus(ptrAesRegs, 1);
                            edmaStatus = DTHE_edmaChannelRxEnableTransferRegion((DTHE_Handle)config);
                        }

                        edmaStatus = DTHE_edmaChannelTxEnableTransferRegion((DTHE_Handle)config);
                        DTHE_AES_setDMAInputRequestStatus(ptrAesRegs, 1);


                        if((ptrParams->algoType != DTHE_AES_CBC_MAC_MODE)&&(ptrParams->algoType != DTHE_AES_CMAC_MODE)&&(ptrParams->algoType != DTHE_AES_F9_MODE))
                        {
                            edmaStatus = DTHE_edmaChannelWaitForRxTransfer((DTHE_Handle)config);
                        }

                        edmaStatus = DTHE_edmaChannelWaitForTxTransfer((DTHE_Handle)config);

                        DTHE_AES_setDMAInputRequestStatus(ptrAesRegs, 0);
                        if((ptrParams->algoType != DTHE_AES_CBC_MAC_MODE)&&(ptrParams->algoType != DTHE_AES_CMAC_MODE)&&(ptrParams->algoType != DTHE_AES_F9_MODE))
                        {
                            DTHE_AES_setDMAOutputRequestStatus(ptrAesRegs, 0);
                        }

                        edmaStatus = DTHE_edmaDisableTxChannel((DTHE_Handle)config);

                        if((ptrParams->algoType != DTHE_AES_CBC_MAC_MODE)&&(ptrParams->algoType != DTHE_AES_CMAC_MODE)&&(ptrParams->algoType != DTHE_AES_F9_MODE))
                        {
                            edmaStatus =DTHE_edmaDisableRxChannel((DTHE_Handle)config);
                        }

                        if(edmaStatus == SystemP_FAILURE)
                        {
                            status = DTHE_AES_RETURN_FAILURE;
                        }
                    }

                    /*Process the last complete block for XTS decryption with CTS*/
                    if(numBlocks == (xtsNumBlock - 1U))
                    {
                        /*Store IV (Tn-1) for XTS as CTS in decryption is required*/
                        DTHE_AES_readIV(ptrAesRegs,tempIV);

                        /* Wait for the AES IP to be ready to receive the data: */
                        DTHE_AES_pollInputReady(ptrAesRegs);
                        /* Write the data: */
                        DTHE_AES_writeDataBlock(ptrAesRegs, &ptrWordInputBuffer[numBlocks << 2U]);

                        /* Wait for the AES IP to be ready with the output data */
                        DTHE_AES_pollOutputReady(ptrAesRegs);

                        /* Read the decrypted data into the decrypted block: */
                        DTHE_AES_readDataBlock(ptrAesRegs, &ptrWordOutputBuffer[numBlocks << 2U]);

                        /*update numBlock back to total no. of complete blocks*/
                        numBlocks++ ;
                    }

                    /* Compute the number of bytes which have been processed: */
                    numBytes = numBytes + ((numBlocks) * 4 * sizeof(uint32_t));
                    index = numBlocks;

                }
                else
#endif
                {
                    /* Cycle through and write all the full blocks: */
                    for (index = 0U; index < numBlocks; index++)
                    {
                        /*Store IV (Tn-1) for XTS if CTS in decryption is required*/
                        if((index == numBlocks-1) && (ptrParams->algoType == DTHE_AES_XTS_MODE) && (ptrParams->opType == DTHE_AES_DECRYPT))
                        {
                            DTHE_AES_readIV(ptrAesRegs,tempIV);
                        }

                        /* Wait for the AES IP to be ready to receive the data: */
                        DTHE_AES_pollInputReady(ptrAesRegs);

                        /* Write the data: */
                        DTHE_AES_writeDataBlock(ptrAesRegs, &ptrWordInputBuffer[index << 2U]);

                        if((ptrParams->algoType != DTHE_AES_CBC_MAC_MODE)&&(ptrParams->algoType != DTHE_AES_CMAC_MODE)&& (ptrParams->algoType != DTHE_AES_F9_MODE))
                        {
                            /* Wait for the AES IP to be ready with the output data */
                            DTHE_AES_pollOutputReady(ptrAesRegs);

                            /* Read the decrypted data into the decrypted block: */
                            DTHE_AES_readDataBlock(ptrAesRegs, &ptrWordOutputBuffer[index << 2U]);
                        }

                        /* Compute the number of bytes which have been processed: */
                        numBytes = numBytes + (4U * sizeof(uint32_t));
                    }
                }

                /* - This flow is for one-shot in continuation to the above flow
                   - In case of Finish continue execution from here
                   - Update should not execute this because this is for partial block
                   handling which is not supported by Update CALL */
                if((ptrParams->streamState == DTHE_AES_ONE_SHOT_SUPPORT)||(ptrParams->streamState == DTHE_AES_STREAM_FINISH))
                {
                    /* Process any left over data: */
                    if(partialDataSize != 0U)
                    {
                        /* Initialize the partial block: */
                        (void)memset ((void *)&inPartialBlock, 0, sizeof(inPartialBlock));
                        (void)memset ((void *)&outPartialBlock, 0, sizeof(outPartialBlock));

                        /* Copy the data into the partial block: */
                        (void)memcpy ((void *)&inPartialBlock,
                                (void *)&ptrWordInputBuffer[index << 2U],
                                partialDataSize);

                        if (ptrParams->algoType == DTHE_AES_CMAC_MODE)
                        {
                            inPartialBlock[partialDataSize] = 0x80;
                        }
                        /* For AES XTS: Implement Cipher Text Stealing (CTS) [Step 1] for partial block if it's not the first block*/
                        if((ptrParams->algoType == DTHE_AES_XTS_MODE) && (numBlocks != 0))
                        {
                            if(ptrParams->opType == DTHE_AES_DECRYPT)
                            {
                                /* Wait for the AES IP to be ready to receive the data: */
                                DTHE_AES_pollInputReady(ptrAesRegs);
                                /* Write the data: */
                                DTHE_AES_writeDataBlock(ptrAesRegs, &ptrWordInputBuffer[(numBlocks-1) << 2U]);

                                /* Wait for the AES IP to be ready with the output data */
                                DTHE_AES_pollOutputReady(ptrAesRegs);
                                /* Read the decrypted data into the decrypted block: */
                                DTHE_AES_readDataBlock(ptrAesRegs, &ptrWordOutputBuffer[(numBlocks-1) << 2U]);

                                /*Update Iv to load tweak value*/
                                if(numBlocks == 1U)
                                {
                                    DTHE_AES_xtsUpdateIv(ptrAesRegs,ptrParams, tempIV, TRUE);
                                }
                                else
                                {
                                    DTHE_AES_xtsUpdateIv(ptrAesRegs,ptrParams, tempIV, FALSE);
                                }
                            }

                            memset(tempData,0,sizeof(tempData));

                            /*Copy last "complete 16-byte block" output data to tempData buffer*/
                            ptrByteBuf = (uint8_t*)&ptrWordOutputBuffer[(numBlocks-1)<<2U];
                            memcpy(&tempData[0],ptrByteBuf, 16);

                           /*Update pointer to last valid byte of inPartialBlock and fill remaing data from last
                             output data block at same index to make inPartialBlock 128 bit aligned*/
                            ptrByteBuf = (uint8_t*)&inPartialBlock[0];
                            memcpy(&ptrByteBuf[partialDataSize],&tempData[partialDataSize],16-partialDataSize);
                        }

                        /* Wait for the AES IP to be ready to receive the data: */
                        DTHE_AES_pollInputReady(ptrAesRegs);

                        /* Write the data: */
                        DTHE_AES_writeDataBlock(ptrAesRegs, (uint32_t *)&inPartialBlock[0U]);

                        if((ptrParams->algoType != DTHE_AES_CBC_MAC_MODE) &&
                           (ptrParams->algoType != DTHE_AES_CMAC_MODE) &&
                           (ptrParams->algoType != DTHE_AES_F9_MODE))
                        {
                            /* Wait for the AES IP to be ready with the output data */
                            DTHE_AES_pollOutputReady(ptrAesRegs);

                            /* Read the decrypted data into the decrypted block: */
                            DTHE_AES_readDataBlock(ptrAesRegs, (uint32_t *)&outPartialBlock[0U]);

                            if((ptrParams->algoType == DTHE_AES_ECB_MODE)||(ptrParams->algoType == DTHE_AES_CBC_MODE))
                            {
                                /* Copy the data into the output buffer, always is going to be 16U */
                                (void)memcpy ((void *)&ptrWordOutputBuffer[index << 2U],
                                        (void *)&outPartialBlock[0U],
                                        16U);
                            }
                            else if((ptrParams->algoType == DTHE_AES_XTS_MODE) && (numBlocks != 0))
                            {
                                /* XTS: Implement Cipher Text Stealing (CTS) [Step 2] for partial block*/
                                /*Replace 2nd last block of output buffer with current AES output*/
                                (void)memcpy ((void *)&ptrWordOutputBuffer[(index-1) << 2U],
                                        (void *)&outPartialBlock[0U],
                                        16U);

                                /*tempData has output of last result, copy this to last out block buffer upto partialDataSize*/
                                (void)memcpy ((void *)&ptrWordOutputBuffer[(index) << 2U],
                                        (void *)&tempData[0U],
                                        partialDataSize);
                            }
                            else
                            {
                                /* Copy the data into the output buffer, always is going to be 16U */
                                (void)memcpy ((void *)&ptrWordOutputBuffer[index << 2U],
                                        (void *)&outPartialBlock[0U],
                                        partialDataSize);
                            }
                        }

                        /* Compute the number of bytes which have been processed: */
                        numBytes = numBytes + partialDataSize;
                    }

                    if((ptrParams->algoType == DTHE_AES_CBC_MAC_MODE)
                       ||(ptrParams->algoType == DTHE_AES_CMAC_MODE)
                       ||(ptrParams->algoType == DTHE_AES_GCM_MODE)
                       ||(ptrParams->algoType == DTHE_AES_CCM_MODE)
                       ||(ptrParams->algoType == DTHE_AES_F9_MODE))
                    {
                        DTHE_AES_pollSaveContextReady(ptrAesRegs);
                        DTHE_AES_readTag(ptrAesRegs, &ptrParams->ptrTag[0]);
                    }
                }

                if(ptrParams->streamState == DTHE_AES_ONE_SHOT_SUPPORT)
                {
                    if(numBytes != ptrParams->dataLenBytes)
                    {
                        status = DTHE_AES_RETURN_FAILURE;
                    }

                    gStreamState = AES_STATE_NEW;
                }
                else if(ptrParams->streamState == DTHE_AES_STREAM_FINISH)
                {
                    gStreamState = AES_STATE_NEW;
                }
            }
        }
    }
    return (status);
}

DTHE_AES_Return_t DTHE_AES_close(DTHE_Handle handle)
{
    DTHE_AES_Return_t  status  = DTHE_AES_RETURN_FAILURE;
    DTHE_Config        *config = NULL;
    DTHE_Attrs         *attrs  = NULL;
    CSL_AesRegs        *ptrAesRegs;

    if(NULL != handle)
    {
        status  = DTHE_AES_RETURN_SUCCESS;
    }
    if(status  == DTHE_AES_RETURN_SUCCESS)
    {
        config          = (DTHE_Config *) handle;
        attrs           = config->attrs;
        ptrAesRegs      = (CSL_AesRegs *)attrs->aesBaseAddr;

        DTHE_AES_resetModule(ptrAesRegs);

    }
    return (status);
}

/* ========================================================================== */
/*                 Internal Function Definations                              */
/* ========================================================================== */
/**
 * \brief                   The function is used to set the DMA context (input and output) status of the AES module.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   dmaStatus       Flag which is used to enable(1)/disable(0) the DMA status.
 *
 */
static void DTHE_AES_setDMAContextStatus(CSL_AesRegs *ptrAesRegs, uint8_t dmaStatus)
{
    if (dmaStatus == 1U)
    {
        CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG, AES_S_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN, 1U);
        CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG, AES_S_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN, 1U);
    }
    else
    {
        CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG, AES_S_SYSCONFIG_DMA_REQ_CONTEXT_OUT_EN, 0U);
        CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG, AES_S_SYSCONFIG_DMA_REQ_CONTEXT_IN_EN, 0U);
    }

    return;
}

/**
 * \brief                   The function is used to set the DMA Output request status
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   dmaStatus       Flag which is used to enable(1)/disable(0) the DMA status.
 *
 */
static void DTHE_AES_setDMAOutputRequestStatus(CSL_AesRegs *ptrAesRegs, uint8_t dmaStatus)
{
    if (dmaStatus == 1U)
    {
        CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG,AES_S_SYSCONFIG_DMA_REQ_DATA_OUT_EN, 1U);
    }
    else
    {
        CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG,AES_S_SYSCONFIG_DMA_REQ_DATA_OUT_EN, 0U);
    }

    return;
}

/**
 * \brief                   The function is used to set the DMA Input request status
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   dmaStatus       Flag which is used to enable(1)/disable(0) the DMA status.
 *
 */
static void DTHE_AES_setDMAInputRequestStatus(CSL_AesRegs *ptrAesRegs, uint8_t dmaStatus)
{
    if (dmaStatus == 1U)
    {
        CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG,AES_S_SYSCONFIG_DMA_REQ_DATA_IN_EN, 1U);
    }
    else
    {
        CSL_REG32_FINS(&ptrAesRegs->SYSCONFIG,AES_S_SYSCONFIG_DMA_REQ_DATA_IN_EN, 0U);
    }

    return;
}

/**
 * \brief                   The function is used to set the key size
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   size            Size of the key to be configured.
 *
 */
static void DTHE_AES_setKeySize(CSL_AesRegs *ptrAesRegs, uint8_t size)
{
    uint8_t keySize;

    if (size == CSL_AES_S_CTRL_KEY_SIZE_KEY128)
    {
        keySize = 1U;
    }
    else if (size == CSL_AES_S_CTRL_KEY_SIZE_KEY192)
    {
        keySize = 2U;
    }
    else if (size == CSL_AES_S_CTRL_KEY_SIZE_KEY256)
    {
        keySize = 3U;
    }
    else
    {
        keySize = 0U;
    }
    CSL_REG32_FINS(&ptrAesRegs->CTRL,AES_S_CTRL_KEY_SIZE, keySize);

    return;
}

/**
 * \brief                   The function is used to configure the key in the AES module This will only configure the 256bit keys.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   ptrKey          Pointer to the 256bit key to be used.
 *
 */
static void DTHE_AES_set256BitKey1(CSL_AesRegs *ptrAesRegs, const uint32_t* ptrKey)
{
    ptrAesRegs->KEY1_0 = ptrKey[0U];
    ptrAesRegs->KEY1_1 = ptrKey[1U];
    ptrAesRegs->KEY1_2 = ptrKey[2U];
    ptrAesRegs->KEY1_3 = ptrKey[3U];
    ptrAesRegs->KEY1_4 = ptrKey[4U];
    ptrAesRegs->KEY1_5 = ptrKey[5U];
    ptrAesRegs->KEY1_6 = ptrKey[6U];
    ptrAesRegs->KEY1_7 = ptrKey[7U];

    return;
}

/**
 * \brief                   The function is used to configure the key in the AES module This will only configure the 128bit keys.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   ptrKey          Pointer to the 128bit key to be used.
 *
 */
static void DTHE_AES_set128BitKey2Part1(CSL_AesRegs *ptrAesRegs, const uint32_t* ptrKey)
{
    ptrAesRegs->KEY2_0 = ptrKey[0U];
    ptrAesRegs->KEY2_1 = ptrKey[1U];
    ptrAesRegs->KEY2_2 = ptrKey[2U];
    ptrAesRegs->KEY2_3 = ptrKey[3U];

    return;
}

/**
 * \brief                   The function is used to configure the key in the AES module This will only configure the 128bit keys.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   ptrKey          Pointer to the 128bit key to be used.
 *
 */
static void DTHE_AES_set128BitKey2Part2(CSL_AesRegs *ptrAesRegs, const uint32_t* ptrKey)
{
    ptrAesRegs->KEY2_4 = ptrKey[0U];
    ptrAesRegs->KEY2_5 = ptrKey[1U];
    ptrAesRegs->KEY2_6 = ptrKey[2U];
    ptrAesRegs->KEY2_7 = ptrKey[3U];

    return;
}

/**
 * \brief                   The function is used to clear the Initialization Vector (IV) in the AES module.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers.
 *
 */
static inline void DTHE_AES_clearIV(CSL_AesRegs *ptrAesRegs)
{
    ptrAesRegs->IV_IN_0 = 0U;
    ptrAesRegs->IV_IN_1 = 0U;
    ptrAesRegs->IV_IN_2 = 0U;
    ptrAesRegs->IV_IN_3 = 0U;
}

/**
 * \brief                   The function is used to set the Initialization Vector (IV) in the AES module.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers.
 *
 * \param   ptrIV           Pointer to the IV data to be used.
 *
 */
static void DTHE_AES_setIV(CSL_AesRegs *ptrAesRegs, uint32_t IvSize, const uint32_t* ptrIV)
{
    if(IvSize < 32U)
    {
        ptrAesRegs->IV_IN_0 = (ptrIV[0U] & 0x0000FFFFU);
    }

    if(IvSize >= 32U)
    {
        ptrAesRegs->IV_IN_0 = ptrIV[0U];
    }

    if((IvSize > 32U)&&(IvSize >= 64U))
    {
        ptrAesRegs->IV_IN_1 = ptrIV[1U];
    }

    if((IvSize > 64U)&&(IvSize >= 96U))
    {
        ptrAesRegs->IV_IN_2 = ptrIV[2U];
    }

    if((IvSize > 96U)&&(IvSize == 128U))
    {
        ptrAesRegs->IV_IN_3 = ptrIV[3U];
    }

    if(IvSize>128U)
    {
        ptrAesRegs->IV_IN_0 = 0U;
        ptrAesRegs->IV_IN_1 = 0U;
        ptrAesRegs->IV_IN_2 = 0U;
        ptrAesRegs->IV_IN_3 = 0U;
    }

    return;
}

/**
 * \brief                   The function is used to read the IV from the AES engine
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   ivReg           Pointer to the IV buffer populated by the API
 *
 */
static inline void DTHE_AES_readIV(CSL_AesRegs *ptrAesRegs, uint32_t* ivReg)
{
    ivReg[0] = ptrAesRegs->IV_IN_0;
    ivReg[1] = ptrAesRegs->IV_IN_1 ;
    ivReg[2] = ptrAesRegs->IV_IN_2 ;
    ivReg[3] = ptrAesRegs->IV_IN_3 ;
}

/**
 * \brief                   The function is used to poll until the AES IP block is ready to receive data.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 */
static void DTHE_AES_pollInputReady(CSL_AesRegs *ptrAesRegs)
{
    uint8_t     done = 0U;

    /* Loop around till the condition is met: */
    while (done == 0U)
    {
        done = CSL_REG32_FEXT(&ptrAesRegs->CTRL, AES_S_CTRL_INPUT_READY);
    }
    return;
}

/**
 * \brief                   The function is used to write the data and pass this to the AES engine.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers.
 *
 * \param   ptrData         Pointer to the data to be written.
 *
 */
static void DTHE_AES_writeDataBlock(CSL_AesRegs *ptrAesRegs, const uint32_t* ptrData)
{
    ptrAesRegs->DATA_IN_3 = ptrData[0U];
    ptrAesRegs->DATA_IN_2 = ptrData[1U];
    ptrAesRegs->DATA_IN_1 = ptrData[2U];
    ptrAesRegs->DATA_IN_0 = ptrData[3U];

    return;
}

/**
 * \brief                   The function is used to poll until the AES IP block has available data which can be read out.
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 */
static void DTHE_AES_pollOutputReady(CSL_AesRegs *ptrAesRegs)
{
    uint8_t     done = 0U;

    /* Loop around till the condition is met: */
    while (done == 0U)
    {
        done = CSL_REG32_FEXT(&ptrAesRegs->CTRL, AES_S_CTRL_OUTPUT_READY);
    }
    return;
}

/**
 * \brief                   The function is used to poll until the AES IP block has available save context ready register
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 */
static void DTHE_AES_pollSaveContextReady(CSL_AesRegs *ptrAesRegs)
{
    uint8_t     done = 0U;

    /* Loop around till the condition is met: */
    while (done == 0U)
    {
        done = CSL_REG32_FEXT(&ptrAesRegs->CTRL, AES_S_CTRL_SAVE_CONTEXT_READY);
    }
    return;
}

/**
 * \brief                   The function is used to read the data from the AES engine
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   ptrData         Pointer to the data buffer populated by the API
 *
 */
static void DTHE_AES_readDataBlock(CSL_AesRegs *ptrAesRegs, uint32_t* ptrData)
{
    ptrData[0] = ptrAesRegs->DATA_IN_3;
    ptrData[1] = ptrAesRegs->DATA_IN_2;
    ptrData[2] = ptrAesRegs->DATA_IN_1;
    ptrData[3] = ptrAesRegs->DATA_IN_0;

    return;
}

/**
 * \brief                   The function is used to read the TAG value from the AES engine
 *
 * \param   ptrAesRegs      Pointer to the EIP38T AES Registers
 *
 * \param   ptrTag          Pointer to the TAG buffer populated by the API
 *
 */
static inline void DTHE_AES_readTag(CSL_AesRegs *ptrAesRegs, uint32_t* ptrTag)
{
    ptrTag[0] = ptrAesRegs->TAG_OUT_0;
    ptrTag[1] = ptrAesRegs->TAG_OUT_1;
    ptrTag[2] = ptrAesRegs->TAG_OUT_2;
    ptrTag[3] = ptrAesRegs->TAG_OUT_3;
}
