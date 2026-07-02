/*
 * Copyright (C) 2024-25 Texas Instruments Incorporated
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the
 *   distribution.
 *
 *   Neither the name of Texas Instruments Incorporated nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 *   @file  pmic_test.c
 *
 *   @brief
 *      PMIC Read and Write example code
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* MCU+SDK Include Files: */
#include <ti/utils/test/pmic_read_write/awr294x/mssgenerated/ti_drivers_open_close.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* Standard Include Files. */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* PMIC configuration message size used using MIBSPIB interface */
#define PMIC_APP_MSGSIZE         (0x3U)

/* PMIC Register LOCK offset */
#define PMIC_REG_LOCK_OFFSET     (0xAU)

/* PMIC Register unlock value */
#define PMIC_REG_UNLOCK_VALUE    (0x9BU)

/* PMIC Register Lock value: Other then 9B value */
#define PMIC_REG_LOCK_VALUE      (0x0U)

typedef struct {
    const char* regName;
    uint32_t address;
} PMIC_reg;

const PMIC_reg PMIC_regData[] =
{
    {"DEV_REV",                      0x01},
    {"NVM_CODE_1",                   0x02},
    {"NVM_CODE_2",                   0x03},
    {"MANUFACTURING_VER",            0x04},
    {"FSM_COMMAND_REG",              0x05},
    {"BLOCK_EN_CTRL",                0x06},
    {"GPO_CTRL",                     0x07},
    {"RECOV_CNT_CONTROL",            0x08},
    {"ESM_START_REG",                0x09},
    {"REGISTER_LOCK",                0x0A},
    {"SCRATCH_PAD_REG_1",            0x0B},
    {"SCRATCH_PAD_REG_2",            0x0C},
    {"SCRATCH_PAD_REG_3",            0x0D},
    {"SCRATCH_PAD_REG_4",            0x0E},
    {"WD_ANSWER_REG",                0x0F},
    {"WD_ENABLE_REG",                0x10},
    {"WD_MODE_REG",                  0x11},
    {"BUCK1_VOUT",                   0x12},
    {"BUCK2_VOUT",                   0x13},
    {"BUCK3_VOUT",                   0x14},
    {"VMON1_PG_LEVEL",               0x15},
    {"BUCK1_MON_CONF",               0x16},
    {"BUCK2_MON_CONF",               0x17},
    {"BUCK3_MON_CONF",               0x18},
    {"BOOST_MON_CONF",               0x19},
    {"CLK_CONF",                     0x1A},
    {"INTERFACE_CONF",               0x1B},
    {"GPO_CONF",                     0x1C},
    {"VCCA_MON_CONF",                0x1D},
    {"VMON1_MON_CONF",               0x1E},
    {"BUCK_BOOST_DEGLIT",            0x1F},
    {"VIO_LDO_MON_CONF",             0x20},
    {"BUCK1_SEQUENCE",               0x21},
    {"BUCK2_SEQUENCE",               0x22},
    {"BUCK3_SEQUENCE",               0x23},
    {"BOOST_SEQUENCE",               0x24},
    {"VIO_LDO_SEQUENCE",             0x25},
    {"VMON1_GPO1_SEQUENCE",          0x26},
    {"GPO2_SEQUENCE",                0x27},
    {"NRSTOUT_SEQUENCE",             0x28},
    {"REG_OV_CONF",                  0x29},
    {"REG_UV_CONF",                  0x2A},
    {"REG_SC_CONF",                  0x2B},
    {"VCCA_VMON_OV_CONF",            0x2C},
    {"VCCA_VMON_UV_CONF",            0x2D},
    {"MASK_BUCK1_2",                 0x2E},
    {"MASK_BUCK3_BOOST",             0x2F},
    {"MASK_VIOLDO",                  0x30},
    {"MASK_VCCA_VMON1",              0x31},
    {"MASK_STARTUP",                 0x32},
    {"MASK_MISC",                    0x33},
    {"MASK_MODERATE_ERR",            0x34},
    {"MASK_COMM_ERR",                0x35},
    {"MASK_ESM",                     0x36},
    {"CONFIG_1",                     0x37},
    {"RECOV_CNT_REG_2",              0x38},
    {"ESM_DELAY1_REG",               0x39},
    {"ESM_DELAY2_REG",               0x3A},
    {"ESM_MODE_CFG",                 0x3B},
    {"ESM_HMAX_REG",                 0x3C},
    {"ESM_HMIN_REG",                 0x3D},
    {"ESM_LMAX_REG",                 0x3E},
    {"ESM_LMIN_REG",                 0x3F},
    {"WD_WIN1_CFG",                  0x40},
    {"WD_WIN2_CFG",                  0x41},
    {"WD_LONGWIN_CFG",               0x42},
    {"WD_QA_CFG",                    0x43},
    {"WD_THR_CFG",                   0x44},
    {"CONFIG_CRC_REG_1",             0x45},
    {"CONFIG_CRC_REG_2",             0x46},
    {"INT_TOP",                      0x47},
    {"INT_BUCK_BOOST",               0x48},
    {"INT_BUCK1_2",                  0x49},
    {"INT_BUCK3_BOOST",              0x4A},
    {"INT_VIOLDO",                   0x4B},
    {"INT_VCCA_VMON1",               0x4C},
    {"INT_STARTUP",                  0x4D},
    {"INT_MISC",                     0x4E},
    {"INT_MODERATE_ERR",             0x4F},
    {"INT_SEVERE_ERR",               0x50},
    {"INT_FSM_ERR",                  0x51},
    {"INT_COMM_ERR",                 0x52},
    {"INT_ESM",                      0x53},
    {"STAT_BUCK1_2",                 0x54},
    {"STAT_BUCK3_BOOST",             0x55},
    {"STAT_VIOLDO",                  0x56},
    {"STAT_VCCA_VMON1",              0x57},
    {"STAT_STARTUP",                 0x58},
    {"STAT_MISC",                    0x59},
    {"STAT_MODERATE_ERR",            0x5A},
    {"STAT_SEVERE_ERR",              0x5B},
    {"RECOV_CNT_REG_1",              0x5C},
    {"ESM_ERR_CNT_REG",              0x5D},
    {"INT_SPI_STATUS",               0x5E},
    {"WD_QUESTION_ANSW_CNT",         0x5F},
    {"WD_ERR_STATUS",                0x60},
    {"WD_FAIL_CNT_REG",              0x61},
    {"CONFIG_CRC_CONFIG",            0x62},
    {"CALCUL_CONFIG_CRC_1",          0x63},
    {"CALCUL_CONFIG_CRC_2",          0x64},
};

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void PMIC_write_reg(MIBSPI_Handle handle, uint8_t regAddr, uint8_t val);
static uint8_t PMIC_Read_reg(MIBSPI_Handle handle, uint8_t regOffset);

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */


static void PMIC_write_reg(MIBSPI_Handle handle, uint8_t regAddr, uint8_t val)
{
    uint8_t tx[PMIC_APP_MSGSIZE];
    MIBSPI_Transaction transaction;

    memset(&transaction, 0, sizeof(transaction));

    /* Configure Data Transfer */
    transaction.count = PMIC_APP_MSGSIZE;
    transaction.txBuf = tx;
    transaction.rxBuf = NULL;
    transaction.peripheralIndex = 0;
    /* Single write transmissions consists of 24bit:
     * Bits 0-7  : Register Address
     * Bits 8-10 : Page address for the register
     * Bit  11   : For Write, value should be 0
     * Bits 12-15: Reserved
     * Bits 16-23: Value to be written to the register
     */
    tx[0] = regAddr;
    tx[1] = 0;
    tx[2] = val;

    CacheP_wbInv((void *) tx, PMIC_APP_MSGSIZE, CacheP_TYPE_ALL);
    /* Start Data Transfer */
    MIBSPI_transfer(handle, &transaction);
}

static uint8_t PMIC_Read_reg(MIBSPI_Handle handle, uint8_t regOffset)
{
    uint8_t tx[PMIC_APP_MSGSIZE];
    uint8_t rx[PMIC_APP_MSGSIZE];
    MIBSPI_Transaction transaction;

    memset(&transaction, 0, sizeof(transaction));

    /* Configure Data Transfer */
    transaction.count = PMIC_APP_MSGSIZE;
    transaction.txBuf = tx;
    transaction.rxBuf = rx;
    transaction.peripheralIndex = 0;
    /* Single read transmissions consists of 24bit:
     * Bits 0-7  : Register Address
     * Bits 8-10 : Page address for the register
     * Bit  11   : For Read, value should be 1
     * Bits 12-15: Reserved
     * Bits 16-23: Value Read from the PMIC
     */
    tx[0] = regOffset;
    tx[1] = 0x10;
    tx[2] = 0;

    CacheP_wbInv((void *) tx, PMIC_APP_MSGSIZE, CacheP_TYPE_ALL);

    /* Start Data Transfer */
    MIBSPI_transfer(handle, &transaction);
    /* Invalidate the receive buffer */
    CacheP_inv((void *) rx, PMIC_APP_MSGSIZE, CacheP_TYPE_ALL);

    return rx[2];
}

void PMIC_readAllRegs(void)
{
    uint8_t regValue = 0;
    uint8_t regOffset = 0;

    for(uint8_t loopIndex = 0; (loopIndex < sizeof(PMIC_regData)/sizeof(PMIC_reg)); loopIndex++)
    {
        regOffset = PMIC_regData[loopIndex].address;
        regValue    = 0U;
        regValue = PMIC_Read_reg(gMibspiHandle[0], regOffset);
        DebugP_log("\t[PMIC] RegName = %s, Address = 0x%X, \t RegVal = 0x%X \r\n", PMIC_regData[loopIndex].regName, regOffset, regValue);
    }
}

void PMIC_writeTest(void)
{
    uint8_t userRegOffset = 0;
    uint8_t userRegVal = 0;
    uint8_t regAddr = 0;
    uint8_t regValue = 0;

    DebugP_log("\tRegister Address (Valid Range: 0x01...0x63): \t");
    DebugP_scanf("%i", &userRegOffset);

    DebugP_log("\tValue to be written                   : \t");
    DebugP_scanf("%i", &userRegVal);

    /* Return if invalid address. */
    if(userRegOffset > 0x64U)
    {
        DebugP_log("\tERROR: Invalid PMIC Address\r\n");
    }
    else
    {
        /* check if PMIC registers are locked */
        regAddr = PMIC_REG_LOCK_OFFSET;
        regValue = PMIC_Read_reg(gMibspiHandle[0], regAddr);

        if(regValue == 1)
        {
            /* unlock PMIC registers  */
            regAddr = PMIC_REG_LOCK_OFFSET;
            regValue = PMIC_REG_UNLOCK_VALUE;
            PMIC_write_reg(gMibspiHandle[0], regAddr, regValue);

            /* Readback LOCK status of PMIC registers  */
            regAddr = PMIC_REG_LOCK_OFFSET;
            regValue = PMIC_Read_reg(gMibspiHandle[0], regAddr);
            if(regValue == 0)
            {
                PMIC_write_reg(gMibspiHandle[0], userRegOffset, userRegVal);
            }
            else
            {
                DebugP_log("\tERROR: Unable to unlock register\r\n");
                DebugP_assert(0);
            }
        }
        else
        {
            PMIC_write_reg(gMibspiHandle[0], userRegOffset, userRegVal);
        }
    }

    return;
}

void PMIC_readTest(void)
{
    uint32_t regOffset = 0;
    uint8_t regVal = 0;

    DebugP_log("\tRegister Address (Valid Range: 0x01...0x64): \t");
    DebugP_scanf("%i", &regOffset);

    regVal = PMIC_Read_reg(gMibspiHandle[0], regOffset);

    DebugP_log("\tValue at PMIC Register(0x%02x) : 0x%02x\t\r\n", regOffset, regVal);

    return;
}

int32_t PMIC_isDeviceAvailable(void)
{
    return SystemP_SUCCESS;
}
