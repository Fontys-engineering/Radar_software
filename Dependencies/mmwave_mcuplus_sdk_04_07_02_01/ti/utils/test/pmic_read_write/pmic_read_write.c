/*
 * Copyright (C) 2023-25 Texas Instruments Incorporated
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
 *   @file  pmic_read_write.c
 *
 *   @brief
 *      PMIC Read and Write example code
 */

 /** @mainpage PMIC Read and Write Application (AWR2544, AWR294x and AWR2x44P)
 *
 * ### Introduction
 *  The Power Management IC is designed to meet the power management requirements of the radar device.
 *
 * #### AWR2544
 *  AWR2544 EVM is populated with LP8772x-Q1 PMIC which is controlled by I2C communication interface. Refer https://www.ti.com/lit/gpn/LP87725-Q1 for more details.
 *
 * #### AWR294X/AWR2x44P
 *  AWR294X and AWR2x44P EVMs are populated with LP87745-Q1 PMIC which is controlled by SPI serial interface. Refer https://www.ti.com/lit/gpn/lp87745-q1 for more details.
 *
 *  This example is a generic application which can be used to read/write to PMIC registers.
 *  It has three cases which is selected based on user input.
 *      - Read all registers: Reads and prints all PMIC register contents on UART interface
 *      - Write to a register: Writes to user provided register address and reads back register content to verify written value.
 *      - Read a register: Read and print user provided register address on UART interface
 *
 * ### Procedure:
 * Connect to application port on any serial terminal. Run pmic_read_write application on MSS core.
 * The following statements are printed on the terminal and it waits for the user input.
 *
 * @image html input.png "Serial Terminal"
 *
 *
 * ### Sample Output
 *
 * @code
 *
 * [PMIC] device found at address 0x60
 * [PMIC] Select Test type:
 *       0: Read all registers
 *       1: Write to register
 *       2: Read register
 *       0
 *       [PMIC] RegName = DEV_REV, Address = 0x1,         RegVal = 0x96
 *       [PMIC] RegName = NVM_CODE_1, Address = 0x2,      RegVal = 0x1
 *       [PMIC] RegName = NVM_CODE_2, Address = 0x3,      RegVal = 0x0
 *       [PMIC] RegName = MANUFACTURING_VER, Address = 0x4,       RegVal = 0x0
 *       [PMIC] RegName = FSM_COMMAND_REG, Address = 0x5,         RegVal = 0x0
 *       [PMIC] RegName = BLOCK_EN_CTRL, Address = 0x6,   RegVal = 0x7F
 *       [PMIC] RegName = RECOV_CNT_CONTROL, Address = 0x7,       RegVal = 0x0
 *       [PMIC] RegName = ESM_START_REG, Address = 0x8,   RegVal = 0x0
 *       [PMIC] RegName = REGISTER_LOCK, Address = 0x9,   RegVal = 0x1
 *       [PMIC] RegName = SCRATCH_PAD_REG_1, Address = 0xA,       RegVal = 0x0
 *       [PMIC] RegName = SCRATCH_PAD_REG_2, Address = 0xB,       RegVal = 0x0
 *       [PMIC] RegName = SCRATCH_PAD_REG_3, Address = 0xC,       RegVal = 0x0
 *       [PMIC] RegName = SCRATCH_PAD_REG_4, Address = 0xD,       RegVal = 0x0
 *       [PMIC] RegName = WD_ANSWER_REG, Address = 0xE,   RegVal = 0x0
 *       [PMIC] RegName = WD_ENABLE_REG, Address = 0xF,   RegVal = 0x3
 *       [PMIC] RegName = WD_MODE_REG, Address = 0x10,    RegVal = 0x4
 *       [PMIC] RegName = BUCK1_VOUT, Address = 0x11,     RegVal = 0x2D
 *       [PMIC] RegName = BUCK2_VOUT, Address = 0x12,     RegVal = 0x5
 *       [PMIC] RegName = BUCK3_VOUT, Address = 0x13,     RegVal = 0xF
 *       [PMIC] RegName = LDO_LS1_VMON1_PG_LEVEL, Address = 0x14,         RegVal = 0x10
 *       [PMIC] RegName = LS2_VMON2_PG_LEVEL, Address = 0x15,     RegVal = 0x70
 *       [PMIC] RegName = BUCK1_MON_CONF, Address = 0x16,         RegVal = 0x70
 *       [PMIC] RegName = BUCK2_MON_CONF, Address = 0x17,         RegVal = 0x57
 *       [PMIC] RegName = BUCK3_MON_CONF, Address = 0x18,         RegVal = 0x57
 *       [PMIC] RegName = LDO_LS1_VMON1_MON_CONF, Address = 0x19,         RegVal = 0x57
 *       [PMIC] RegName = LS2_VMON2_MON_CONF, Address = 0x1A,     RegVal = 0xF4
 *       [PMIC] RegName = CLK_CONF, Address = 0x1B,       RegVal = 0xA4
 *       [PMIC] RegName = INTERFACE_CONF, Address = 0x1C,         RegVal = 0x85
 *       [PMIC] RegName = FUNC_CONF, Address = 0x1D,      RegVal = 0x2D
 *       [PMIC] RegName = VCCA_MON_CONF, Address = 0x1E,          RegVal = 0x40
 *       [PMIC] RegName = BUCK_LDO_LS1_VMON1_DEGLIT, Address = 0x1F,      RegVal = 0xFB
 *       [PMIC] RegName = BUCK1_SEQUENCE, Address = 0x20,         RegVal = 0xE8
 *       [PMIC] RegName = BUCK2_SEQUENCE, Address = 0x21,         RegVal = 0x0
 *       [PMIC] RegName = BUCK3_SEQUENCE, Address = 0x22,         RegVal = 0x3
 *       [PMIC] RegName = LDO_LS1_VMON1_SEQUENCE, Address = 0x23,         RegVal = 0x2
 *       [PMIC] RegName = LS2_VMON2_SEQUENCE, Address = 0x24,     RegVal = 0x4
 *       [PMIC] RegName = GPO_SEQUENCE, Address = 0x25,   RegVal = 0x1
 *       [PMIC] RegName = NRSTOUT_SEQUENCE, Address = 0x26,       RegVal = 0x0
 *       [PMIC] RegName = REG_OV_CONF, Address = 0x27,    RegVal = 0x9
 *       [PMIC] RegName = REG_UV_CONF, Address = 0x28,    RegVal = 0xEE
 *       [PMIC] RegName = REG_SC_CONF, Address = 0x29,    RegVal = 0x1D
 *       [PMIC] RegName = VCCA_LS2_VMON2_OV_CONF, Address = 0x2A,         RegVal = 0xEA
 *       [PMIC] RegName = VCCA_LS2_VMON2_UV_CONF, Address = 0x2B,         RegVal = 0xA0
 *       [PMIC] RegName = MASK_BUCK1_2, Address = 0x2C,   RegVal = 0xA0
 *       [PMIC] RegName = MASK_BUCK3_LDO_LS1_VMON1, Address = 0x2D,       RegVal = 0x44
 *       [PMIC] RegName = MASK_LS2_VMON2, Address = 0x2E,         RegVal = 0x44
 *       [PMIC] RegName = MASK_VCCA, Address = 0x2F,      RegVal = 0x4
 *       [PMIC] RegName = MASK_STARTUP, Address = 0x30,   RegVal = 0x0
 *       [PMIC] RegName = MASK_MISC, Address = 0x31,      RegVal = 0x0
 *       [PMIC] RegName = MASK_MODERATE_ERR, Address = 0x32,      RegVal = 0xC0
 *       [PMIC] RegName = MASK_COMM_ERR, Address = 0x33,          RegVal = 0x0
 *       [PMIC] RegName = MASK_ESM, Address = 0x34,       RegVal = 0x0
 *       [PMIC] RegName = CONFIG_1, Address = 0x35,       RegVal = 0x0
 *       [PMIC] RegName = RECOV_CNT_REG_2, Address = 0x36,        RegVal = 0x27
 *       [PMIC] RegName = ESM_DELAY1_REG, Address = 0x37,         RegVal = 0xFF
 *       [PMIC] RegName = ESM_DELAY2_REG, Address = 0x38,         RegVal = 0x0
 *       [PMIC] RegName = ESM_MODE_CFG, Address = 0x39,   RegVal = 0x0
 *       [PMIC] RegName = ESM_HMAX_REG, Address = 0x3A,   RegVal = 0x0
 *       [PMIC] RegName = ESM_HMIN_REG, Address = 0x3B,   RegVal = 0x0
 *       [PMIC] RegName = ESM_LMAX_REG, Address = 0x3C,   RegVal = 0x0
 *       [PMIC] RegName = ESM_LMIN_REG, Address = 0x3D,   RegVal = 0x0
 *       [PMIC] RegName = WD_WIN1_CFG, Address = 0x3E,    RegVal = 0x0
 *       [PMIC] RegName = WD_WIN2_CFG, Address = 0x3F,    RegVal = 0x7F
 *       [PMIC] RegName = WD_LONGWIN_CFG, Address = 0x40,         RegVal = 0x7F
 *       [PMIC] RegName = WD_QA_CFG, Address = 0x41,      RegVal = 0xFF
 *       [PMIC] RegName = WD_THR_CFG, Address = 0x42,     RegVal = 0xA
 *       [PMIC] RegName = SPREAD_SPECTRUM_CONFIG_1, Address = 0x43,       RegVal = 0x3F
 *       [PMIC] RegName = SPREAD_SPECTRUM_CONFIG_2, Address = 0x43,       RegVal = 0x3F
 *       [PMIC] RegName = CONFIG_CRC_REG_1, Address = 0x45,       RegVal = 0x0
 *       [PMIC] RegName = CONFIG_CRC_REG_2, Address = 0x46,       RegVal = 0x99
 *       [PMIC] RegName = INT_TOP, Address = 0x47,        RegVal = 0x8
 *       [PMIC] RegName = INT_BUCK_LS2_VMON2, Address = 0x48,     RegVal = 0x0
 *       [PMIC] RegName = INT_BUCK1_2, Address = 0x49,    RegVal = 0x10
 *       [PMIC] RegName = INT_BUCK3_LDO_LS1_VMON1, Address = 0x4A,        RegVal = 0x0
 *       [PMIC] RegName = INT_LS2_VMON2, Address = 0x4B,          RegVal = 0x0
 *       [PMIC] RegName = INT_VCCA, Address = 0x4C,       RegVal = 0x2
 *       [PMIC] RegName = INT_STARTUP, Address = 0x4D,    RegVal = 0x4
 *       [PMIC] RegName = INT_MISC, Address = 0x4E,       RegVal = 0x0
 *       [PMIC] RegName = INT_MODERATE_ERR, Address = 0x4F,       RegVal = 0x0
 *       [PMIC] RegName = INT_SEVERE_ERR, Address = 0x50,         RegVal = 0x18
 *       [PMIC] RegName = INT_FSM_ERR, Address = 0x51,    RegVal = 0x0
 *       [PMIC] RegName = INT_COMM_ERR, Address = 0x52,   RegVal = 0x0
 *       [PMIC] RegName = INT_ESM, Address = 0x53,        RegVal = 0x44
 *       [PMIC] RegName = STAT_BUCK1_2, Address = 0x54,   RegVal = 0x44
 *       [PMIC] RegName = STAT_BUCK3_LDO_LS1_VMON1, Address = 0x55,       RegVal = 0x4
 *       [PMIC] RegName = STAT_LS2_VMON2, Address = 0x56,         RegVal = 0x0
 *       [PMIC] RegName = STAT_VCCA, Address = 0x57,      RegVal = 0xF6
 *       [PMIC] RegName = STAT_STARTUP, Address = 0x58,   RegVal = 0x40
 *       [PMIC] RegName = STAT_MISC, Address = 0x59,      RegVal = 0x2
 *       [PMIC] RegName = STAT_MODERATE_ERR, Address = 0x5A,      RegVal = 0x0
 *       [PMIC] RegName = STAT_SEVERE_ERR, Address = 0x5B,        RegVal = 0x0
 *       [PMIC] RegName = RECOV_CNT_REG_1, Address = 0x5C,        RegVal = 0x0
 *       [PMIC] RegName = ESM_ERR_CNT_REG, Address = 0x5D,        RegVal = 0xBC
 *       [PMIC] RegName = WD_QUESTION_ANSW_CNT, Address = 0x5E,   RegVal = 0x0
 *       [PMIC] RegName = WD_ERR_STATUS, Address = 0x5F,          RegVal = 0x0
 *       [PMIC] RegName = WD_FAIL_CNT_REG, Address = 0x60,        RegVal = 0x1
 *       [PMIC] RegName = CONFIG_CRC_CONFIG, Address = 0x61,      RegVal = 0x0
 *       [PMIC] RegName = CALCUL_CONFIG_CRC_1, Address = 0x62,    RegVal = 0x0
 *       [PMIC] RegName = CALCUL_CONFIG_CRC_2, Address = 0x63,    RegVal = 0x7F
 *
 * [PMIC] Select Test type:
 *       0: Read all registers
 *       1: Write to register
 *       2: Read register
 *       1
 *       Register Address (Valid Range: 0x01...0x63):    0x11
 *       Value to be written                   :         0x2E
 *
 * [PMIC] Select Test type:
 *       0: Read all registers
 *       1: Write to register
 *       2: Read register
 *       2
 *       Register Address (Valid Range: 0x01...0x63):    0x11
 *       Value at PMIC Register(0x11) : 0x2e
 *
 * [PMIC] Select Test type:
 *       0: Read all registers
 *       1: Write to register
 *       2: Read register
 * @endcode
 *
 *
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

/* MCU+SDK Include Files: */
#if defined (SOC_AWR2544)
#include <ti/utils/test/pmic_read_write/awr2544/mssgenerated/ti_drivers_config.h>
#include <ti/utils/test/pmic_read_write/awr2544/mssgenerated/ti_board_config.h>
#include <ti/utils/test/pmic_read_write/awr2544/mssgenerated/ti_drivers_open_close.h>
#include <ti/utils/test/pmic_read_write/awr2544/mssgenerated/ti_board_open_close.h>
#endif

#if defined (SOC_AWR294X)
#include <ti/utils/test/pmic_read_write/awr294x/mssgenerated/ti_drivers_config.h>
#include <ti/utils/test/pmic_read_write/awr294x/mssgenerated/ti_board_config.h>
#include <ti/utils/test/pmic_read_write/awr294x/mssgenerated/ti_drivers_open_close.h>
#include <ti/utils/test/pmic_read_write/awr294x/mssgenerated/ti_board_open_close.h>
#endif

#if defined (SOC_AWR2X44P)
#include <ti/utils/test/pmic_read_write/awr2x44p/mssgenerated/ti_drivers_config.h>
#include <ti/utils/test/pmic_read_write/awr2x44p/mssgenerated/ti_board_config.h>
#include <ti/utils/test/pmic_read_write/awr2x44p/mssgenerated/ti_drivers_open_close.h>
#include <ti/utils/test/pmic_read_write/awr2x44p/mssgenerated/ti_board_open_close.h>
#endif

#include "FreeRTOS.h"
#include "task.h"

/* Application headers */
#include "pmic_read_write.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI             (5U)
#define APP_TASK_STACK_SIZE      (2*1024U)

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* PMIC test selection function */
static void PMIC_selectTestType(void);

/* Pmic read write task function */
void Pmic_read_write_test(void* args);

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */
TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

StackType_t     gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void PMIC_selectTestType(void)
{
    Bool retry;
    int32_t choice = -1;

    do
    {
        DebugP_log("[PMIC] Select Test type:\r\n");
        DebugP_log("\t0: Read all registers\r\n");
        DebugP_log("\t1: Write to register\r\n");
        DebugP_log("\t2: Read register\r\n\t");

        DebugP_scanf("%i", &choice);

        switch (choice)
        {
            case 0:
                PMIC_readAllRegs();
                retry = FALSE;
                break;
            case 1:
                PMIC_writeTest();
                retry = FALSE;
                break;
            case 2:
                PMIC_readTest();
                retry = FALSE;
                break;
            default:
                DebugP_log("\t[PMIC] Wrong option, try again...\r\n\n");
                retry = TRUE;
                break;
        }
    }while (retry);
}

void Pmic_read_write_test(void* args)
{
    Drivers_open();
    Board_driversOpen();

    if(PMIC_isDeviceAvailable() == SystemP_SUCCESS)
    {
        while(1)
        {
            PMIC_selectTestType();
            DebugP_log("\r\n");
        }
    }

    Board_driversClose();
    Drivers_close();

    vTaskDelete(NULL);
}

int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( Pmic_read_write_test,
                                  "Pmic_read_write_test",
                                  APP_TASK_STACK_SIZE,
                                  NULL,
                                  APP_TASK_PRI,
                                  gAppTskStackMain,
                                  &gAppTaskObj );
    configASSERT(gAppTask != NULL);

    /* Start the scheduler to start the tasks executing. */
    vTaskStartScheduler();

    /* The following line should never be reached because vTaskStartScheduler()
    will only return if there was not enough FreeRTOS heap memory available to
    create the Idle and (if configured) Timer tasks.  Heap management, and
    techniques for trapping heap exhaustion, are described in the book text. */
    DebugP_assertNoLog(0);
}

