/*
 * Copyright (C) 2023-24 Texas Instruments Incorporated
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
#include <ti/utils/test/pmic_read_write/awr2544/mssgenerated/ti_drivers_open_close.h>

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* PMIC device I2C Address */
#define PMIC_I2C_ADDRESS         (0x60U)

/* PMIC Register LOCK offset */
#define PMIC_REG_LOCK_OFFSET     (0x9U)

/* PMIC Register unlock value */
#define PMIC_REG_UNLOCK_VALUE    (0x9BU)

/* PMIC Register Lock value: Other then 9B value */
#define PMIC_REG_LOCK_VALUE      (0x0U)

typedef struct {
    const char* regName;
    uint32_t address;
} PMIC_reg;

const PMIC_reg PMIC_regData[99] =
{
    {"DEV_REV",                      0x01},
    {"NVM_CODE_1",                   0x02},
    {"NVM_CODE_2",                   0x03},
    {"MANUFACTURING_VER",            0x04},
    {"FSM_COMMAND_REG",              0x05},
    {"BLOCK_EN_CTRL",                0x06},
    {"RECOV_CNT_CONTROL",            0x07},
    {"ESM_START_REG",                0x08},
    {"REGISTER_LOCK",                0x09},
    {"SCRATCH_PAD_REG_1",            0x0A},
    {"SCRATCH_PAD_REG_2",            0x0B},
    {"SCRATCH_PAD_REG_3",            0x0C},
    {"SCRATCH_PAD_REG_4",            0x0D},
    {"WD_ANSWER_REG",                0x0E},
    {"WD_ENABLE_REG",                0x0F},
    {"WD_MODE_REG",                  0x10},
    {"BUCK1_VOUT",                   0x11},
    {"BUCK2_VOUT",                   0x12},
    {"BUCK3_VOUT",                   0x13},
    {"LDO_LS1_VMON1_PG_LEVEL",       0x14},
    {"LS2_VMON2_PG_LEVEL",           0x15},
    {"BUCK1_MON_CONF",               0x16},
    {"BUCK2_MON_CONF",               0x17},
    {"BUCK3_MON_CONF",               0x18},
    {"LDO_LS1_VMON1_MON_CONF",       0x19},
    {"LS2_VMON2_MON_CONF",           0x1A},
    {"CLK_CONF",                     0x1B},
    {"INTERFACE_CONF",               0x1C},
    {"FUNC_CONF",                    0x1D},
    {"VCCA_MON_CONF",                0x1E},
    {"BUCK_LDO_LS1_VMON1_DEGLIT",    0x1F},
    {"BUCK1_SEQUENCE",               0x20},
    {"BUCK2_SEQUENCE",               0x21},
    {"BUCK3_SEQUENCE",               0x22},
    {"LDO_LS1_VMON1_SEQUENCE",       0x23},
    {"LS2_VMON2_SEQUENCE",           0x24},
    {"GPO_SEQUENCE",                 0x25},
    {"NRSTOUT_SEQUENCE",             0x26},
    {"REG_OV_CONF",                  0x27},
    {"REG_UV_CONF",                  0x28},
    {"REG_SC_CONF",                  0x29},
    {"VCCA_LS2_VMON2_OV_CONF",       0x2A},
    {"VCCA_LS2_VMON2_UV_CONF",       0x2B},
    {"MASK_BUCK1_2",                 0x2C},
    {"MASK_BUCK3_LDO_LS1_VMON1",     0x2D},
    {"MASK_LS2_VMON2",               0x2E},
    {"MASK_VCCA",                    0x2F},
    {"MASK_STARTUP",                 0x30},
    {"MASK_MISC",                    0x31},
    {"MASK_MODERATE_ERR",            0x32},
    {"MASK_COMM_ERR",                0x33},
    {"MASK_ESM",                     0x34},
    {"CONFIG_1",                     0x35},
    {"RECOV_CNT_REG_2",              0x36},
    {"ESM_DELAY1_REG",               0x37},
    {"ESM_DELAY2_REG",               0x38},
    {"ESM_MODE_CFG",                 0x39},
    {"ESM_HMAX_REG",                 0x3A},
    {"ESM_HMIN_REG",                 0x3B},
    {"ESM_LMAX_REG",                 0x3C},
    {"ESM_LMIN_REG",                 0x3D},
    {"WD_WIN1_CFG",                  0x3E},
    {"WD_WIN2_CFG",                  0x3F},
    {"WD_LONGWIN_CFG",               0x40},
    {"WD_QA_CFG",                    0x41},
    {"WD_THR_CFG",                   0x42},
    {"SPREAD_SPECTRUM_CONFIG_1",     0x43},
    {"SPREAD_SPECTRUM_CONFIG_2",     0x43},
    {"CONFIG_CRC_REG_1",             0x45},
    {"CONFIG_CRC_REG_2",             0x46},
    {"INT_TOP",                      0x47},
    {"INT_BUCK_LS2_VMON2",           0x48},
    {"INT_BUCK1_2",                  0x49},
    {"INT_BUCK3_LDO_LS1_VMON1",      0x4A},
    {"INT_LS2_VMON2",                0x4B},
    {"INT_VCCA",                     0x4C},
    {"INT_STARTUP",                  0x4D},
    {"INT_MISC",                     0x4E},
    {"INT_MODERATE_ERR",             0x4F},
    {"INT_SEVERE_ERR",               0x50},
    {"INT_FSM_ERR",                  0x51},
    {"INT_COMM_ERR",                 0x52},
    {"INT_ESM",                      0x53},
    {"STAT_BUCK1_2",                 0x54},
    {"STAT_BUCK3_LDO_LS1_VMON1",     0x55},
    {"STAT_LS2_VMON2",               0x56},
    {"STAT_VCCA",                    0x57},
    {"STAT_STARTUP",                 0x58},
    {"STAT_MISC",                    0x59},
    {"STAT_MODERATE_ERR",            0x5A},
    {"STAT_SEVERE_ERR",              0x5B},
    {"RECOV_CNT_REG_1",              0x5C},
    {"ESM_ERR_CNT_REG",              0x5D},
    {"WD_QUESTION_ANSW_CNT",         0x5E},
    {"WD_ERR_STATUS",                0x5F},
    {"WD_FAIL_CNT_REG",              0x60},
    {"CONFIG_CRC_CONFIG",            0x61},
    {"CALCUL_CONFIG_CRC_1",          0x62},
    {"CALCUL_CONFIG_CRC_2",          0x63},
};

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static void PMIC_selectTestType(void);
static void PMIC_setupTransfer(I2C_Handle handle,  uint32_t targetAddr,
                      uint8_t *writeData, uint32_t numWriteBytes,
                      uint8_t *readData,  uint32_t numReadBytes);
static int32_t PMIC_write (int32_t argc, char* argv[]);
static void PMIC_writeReg(uint32_t address, uint32_t val);

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

static void PMIC_setupTransfer(I2C_Handle handle,  uint32_t targetAddr,
                      uint8_t *writeData, uint32_t numWriteBytes,
                      uint8_t *readData,  uint32_t numReadBytes)
{
    int32_t status;
    I2C_Transaction i2cTransaction;

    /* Enable transaction */
    I2C_Transaction_init(&i2cTransaction);
    i2cTransaction.targetAddress = targetAddr;
    i2cTransaction.writeBuf = (uint8_t *)&writeData[0];
    i2cTransaction.writeCount = numWriteBytes;
    i2cTransaction.readBuf = (uint8_t *)&readData[0];
    i2cTransaction.readCount = numReadBytes;
    status = I2C_transfer(handle, &i2cTransaction);
    DebugP_assert(SystemP_SUCCESS == status);
}

static void PMIC_writeReg(uint32_t address, uint32_t val)
{
    I2C_Handle i2cHandle = gI2cHandle[PMICHANDLE];
    uint8_t deviceAddress = PMIC_I2C_ADDRESS;
    uint8_t regRead = 0;
    uint8_t writeBuf[3] = {0};

    /* Write to PMIC register  */
    writeBuf[0] = (uint8_t)address;
    writeBuf[1] = (uint8_t)val;
    PMIC_setupTransfer(i2cHandle, deviceAddress, &writeBuf[0], 2, NULL, 0);

    /* Readback PMIC register  */
    writeBuf[0] = (uint8_t)address;
    writeBuf[1] = 0;
    PMIC_setupTransfer(i2cHandle, deviceAddress, &writeBuf[0], 1, &regRead, 1);

    if(regRead != val)
    {
        DebugP_log("\tERROR: Readback of Address failed\r\n");
        DebugP_assert(0);
    }

    return;
}

void PMIC_readAllRegs(void)
{
    uint8_t deviceAddress = PMIC_I2C_ADDRESS;
    uint8_t regValue = 0;
    uint8_t writeBuf[3] = {0};
    I2C_Handle i2cHandle = gI2cHandle[PMICHANDLE];

    for(uint8_t loopIndex = 0; (loopIndex < sizeof(PMIC_regData)/sizeof(PMIC_reg)); loopIndex++)
    {
        writeBuf[0] = PMIC_regData[loopIndex].address;
        regValue    = 0U;
        PMIC_setupTransfer(i2cHandle, deviceAddress, &writeBuf[0], 1, &regValue, 1);
        DebugP_log("\t[PMIC] RegName = %s, Address = 0x%X, \t RegVal = 0x%X \r\n", PMIC_regData[loopIndex].regName, PMIC_regData[loopIndex].address, regValue);
    }
}

void PMIC_writeTest(void)
{
    uint32_t userRegAddress = 0;
    uint32_t userRegVal = 0;
    I2C_Handle i2cHandle = gI2cHandle[PMICHANDLE];
    uint8_t deviceAddress = PMIC_I2C_ADDRESS;
    uint8_t regRead = 0;
    uint8_t writeBuf[3] = {0};

    DebugP_log("\tRegister Address (Valid Range: 0x01...0x63): \t");
    DebugP_scanf("%i", &userRegAddress);

    DebugP_log("\tValue to be written                   : \t");
    DebugP_scanf("%i", &userRegVal);

    /* Return if invalid address. */
    if(userRegAddress > 0x63U)
    {
        DebugP_log("\tERROR: Invalid PMIC Address\r\n");
    }
    else
    {
        /* check if PMIC registers are locked */
        writeBuf[0] = PMIC_REG_LOCK_OFFSET;
        regRead = 0;
        PMIC_setupTransfer(i2cHandle, deviceAddress, &writeBuf[0], 1, &regRead, 1);

        if(regRead == 1)
        {
            /* unlock PMIC registers  */
            writeBuf[0] = PMIC_REG_LOCK_OFFSET;
            writeBuf[1] = PMIC_REG_UNLOCK_VALUE;
            PMIC_setupTransfer(i2cHandle, deviceAddress, &writeBuf[0], 2, NULL, 0);

            /* Readback LOCK status of PMIC registers  */
            writeBuf[0] = PMIC_REG_LOCK_OFFSET;
            writeBuf[1] = 0;
            PMIC_setupTransfer(i2cHandle, deviceAddress, &writeBuf[0], 1, &regRead, 1);
            if(regRead == 0)
            {
                PMIC_writeReg(userRegAddress, userRegVal);
            }
            else
            {
                DebugP_log("\tERROR: Unable to unlock register\r\n");
                DebugP_assert(0);
            }
        }
        else
        {
            PMIC_writeReg(userRegAddress, userRegVal);
        }
    }

    return;
}

void PMIC_readTest(void)
{
    uint32_t userRegAddress = 0;
    I2C_Handle i2cHandle = gI2cHandle[PMICHANDLE];
    uint8_t deviceAddress = PMIC_I2C_ADDRESS;
    uint8_t regRead = 0;
    uint8_t writeBuf[3] = {0};

    DebugP_log("\tRegister Address (Valid Range: 0x01...0x63): \t");
    DebugP_scanf("%i", &userRegAddress);

    /* Read PMIC register */
    writeBuf[0] = (uint8_t)userRegAddress;
    regRead = 0;
    PMIC_setupTransfer(i2cHandle, deviceAddress, &writeBuf[0], 1, &regRead, 1);
    DebugP_log("\tValue at PMIC Register(0x%02x) : 0x%02x\t\r\n", userRegAddress, regRead);
}

int32_t PMIC_isDeviceAvailable(void)
{
    int32_t status = SystemP_SUCCESS;
    uint32_t deviceAddress = 0U;
    I2C_Handle      i2cHandle;

    i2cHandle = gI2cHandle[PMICHANDLE];

    /* Check if PMIC is present at 0x60 */
    deviceAddress = PMIC_I2C_ADDRESS;
    status = I2C_probe(i2cHandle, deviceAddress);
    if(status == SystemP_SUCCESS)
    {
        DebugP_log("[PMIC] device found at address 0x%02x \r\n", deviceAddress);
    }
    else
    {
        DebugP_log("[PMIC] device not found at device address 0x%02x \r\n", deviceAddress);
    }

    return status;
}
