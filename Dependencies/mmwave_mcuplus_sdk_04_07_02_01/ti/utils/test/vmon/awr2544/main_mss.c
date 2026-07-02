/*
 * Copyright (C) 2023 Texas Instruments Incorporated
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

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
/* Standard Include Files. */
#include <string.h>
#include <stdio.h>

/* MCU+SDK include files. */
#include <drivers/hw_include/cslr_soc.h>
#include <kernel/dpl/ClockP.h>
#include <ti/utils/test/vmon/awr2544/mssgenerated/ti_drivers_config.h>
#include <ti/utils/test/vmon/awr2544/mssgenerated/ti_board_config.h>
#include <ti/utils/test/vmon/awr2544/mssgenerated/ti_drivers_open_close.h>
#include <ti/utils/test/vmon/awr2544/mssgenerated/ti_board_open_close.h>
#include "FreeRTOS.h"
#include "task.h"

/* MmwaveSDK include files. */
#include <ti/common/syscommon.h>


/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/* FreeRTOS Task declarations. */
#define APP_TASK_PRI         (5U)
#define APP_TASK_STACK_SIZE  (2*1024U)

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

TaskHandle_t    gAppTask;
StaticTask_t    gAppTaskObj;

/* Stack for main task. */
StackType_t    gAppTskStackMain[APP_TASK_STACK_SIZE] __attribute__((aligned(32)));

/* Interrupt counter. */
volatile uint32_t gVMONMssIntCount = 0U;

/* VMON VDD 1.2V UV Status flag. */
volatile uint32_t gVMONVDD1p2vUVflag = 0U;

/* VMON VDD 1.2V OV Status flag. */
volatile uint32_t gVMONVDD1p2vOVflag = 0U;

/* VMON VDD OSC 1.8V UV Status flag. */
volatile uint32_t gVMONVDDAOsc1p8vUVflag = 0U;

/* VMON VDD 3.3V UV Status flag. */
volatile uint32_t gVMONVDDs3p3UVflag = 0U;
/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

/**
 *  @b Description
 *  @n
 *      The function is used to get the base address to the MSS TOP RCM register space
 *
 *  @retval
 *      pointer to the MSS CTRL Register space.
 */
static CSL_mss_toprcmRegs* CSL_MSS_TOP_RCM_getBaseAddress (void)
{
    return (CSL_mss_toprcmRegs*) CSL_MSS_TOPRCM_U_BASE;
}

/**
 *  @b Description
 *  @n
 *      Application implemented callback function for ANA_WU_AND_CLK_STATUS_ERR 
 *      ESM errors. When the watchdog timeout causes a NMI to occur on DSS, that event inturn triggers a group1 ESM error on the MSS. The application can register
 *      a callback using the ESM notifier to handle these errors.
 *
 *   @param[in] arg
 *      Argument which is registered with the ESM module while registering the notify function.
 *  @retval
 *      Not Applicable.
 */
void mssVMONCallback(void* arg)
{
    CSL_mss_toprcmRegs *ptrMssTopRcmReg = CSL_MSS_TOP_RCM_getBaseAddress();

    gVMONMssIntCount = 0;

    if(CSL_FEXT(ptrMssTopRcmReg->ANA_REG_WU_STATUS_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_STATUS_REG_LOWV_CORE_UVDET_LAT))
    {
        CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDD_UV_VMON_EN, 0);
        gVMONVDD1p2vUVflag = 1;
    }
    else if (CSL_FEXT(ptrMssTopRcmReg->ANA_REG_WU_STATUS_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_STATUS_REG_LOWV_CORE_OVDET_LAT))
    {
        /* Disable VMON. */
        CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDD_OV_VMON_EN, 0);
        gVMONVDD1p2vOVflag = 1;
    }
    else if(CSL_FEXT(ptrMssTopRcmReg->ANA_REG_WU_STATUS_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_STATUS_REG_LOWV_VDDA_OSC_UVDET_LAT))
    {
        /* Disable VMON. */
        CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDDA_OSC_UV_VMON_EN, 0);
        gVMONVDDAOsc1p8vUVflag = 1;
    }
    else if(CSL_FEXT(ptrMssTopRcmReg->ANA_REG_WU_STATUS_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_STATUS_REG_LOWV_VDDS_3P3V_UVDET_LAT))
    {
        /* Disable VMON. */
        CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDDS_3P3V_UV_VMON_EN, 0);
        gVMONVDDs3p3UVflag = 1;
    }
    gVMONMssIntCount++;
    return;
}


static void mssVDD1p2vUVSelfTest(void)
{
    CSL_mss_toprcmRegs *ptrMssTopRcmReg = CSL_MSS_TOP_RCM_getBaseAddress();

    /* Set Reference for Self Test Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_SR_SEL, 0);

    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_OV_IR_DROP_COMP_SEL, 3);

    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_UV_SELF_TEST_SEL, 1);

    /* Allow reference to change */
    ClockP_usleep(1 * 1000);

    /* Enable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDD_UV_VMON_EN, 1);

    /* Allow VMON to trip */
    ClockP_usleep(1 * 1000);

    /* Verify that VMON has toggled and corresponding ESM interrupt is generated. */
    if((gVMONMssIntCount != 0) && (gVMONVDD1p2vUVflag == 1))
    {
        test_print("VDD1.2 UVtest Passed\n");
    }
    else
    {
        test_print("VDD1.2 UV test Failed - ESM Interrupt not triggered\n");
    }

    /* Disable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDD_UV_VMON_EN, 0);

    /* Disables Self Test Mode. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_UV_SELF_TEST_SEL, 0);

    /* Set Reference for Normal Mode. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_IR_DROP_COMP_SEL, 3);

    /* Set Reference for Normal Mode. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_SR_SEL, 1);

    /* Allow VMON to reset. */
    ClockP_usleep(1 * 1000);

    /* Re-enable VMON */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDD_UV_VMON_EN, 1);

    return;
}


static void mssVDD1p2vOVSelfTest(void)
{
    CSL_mss_toprcmRegs *ptrMssTopRcmReg = CSL_MSS_TOP_RCM_getBaseAddress();

    /* Set Reference for Self Test Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_IR_DROP_COMP_SEL, 3);

    /* Set Reference for Self Test Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_OV_SR_SEL, 1);

    /* Enable Self test mode. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_OV_SELF_TEST_SEL, 1);

    /* Allow reference to change. */
    ClockP_usleep(1 * 1000);

    /* Enable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDD_OV_VMON_EN, 1);

    /* Allow VMON to trip. */
    ClockP_usleep(1 * 1000);

    /* Verifies that VMON has toggled and corresponding ESM interrupt is generated. */
    if((gVMONMssIntCount != 0) && (gVMONVDD1p2vOVflag == 1))
    {
        test_print("VDD1.2 OVtest Passed\n");
    }
    else
    {
        test_print("VDD1.2 OVtest Failed - ESM Interrupt not triggered\n");
    }

    /* Disable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDD_OV_VMON_EN, 0);

    /* Set Reference for Normal Mode. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_OV_SR_SEL, 0);

    /* Set Reference for Normal Mode. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDD_OV_IR_DROP_COMP_SEL, 3);

    /* Allow VMON to reset. */
    ClockP_usleep(1 * 1000);

    /* Re-enable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDD_OV_VMON_EN, 1);

    return;
}

static void mssVDDAOsc1p8vUVSelfTest(void)
{
    CSL_mss_toprcmRegs *ptrMssTopRcmReg = CSL_MSS_TOP_RCM_getBaseAddress();

    /* Set Reference for Self Test Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDDA_OSC_IR_DROP_COMP_SEL, 0);

    /* Enable Self Test Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDDA_OSC_UV_SELF_TEST_SEL, 1);

    /* Allow reference to change. */
    ClockP_usleep(1 * 1000);

    /* Enable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDDA_OSC_UV_VMON_EN, 1);

    /* Allow VMON to trip. */
    ClockP_usleep(1 * 1000);

    /* Verify that VMON has toggled and corresponding ESM interrupt is generated. */
    if((gVMONMssIntCount != 0) && (gVMONVDDAOsc1p8vUVflag == 1))
    {
        test_print("VDDA_OSC 1.8V UV Test Passed\n");
    }
    else
    {
        test_print("VDDA_OSC 1.8V UV Test Failed - ESM Interrupt not triggered\n");
    }

    /* Disable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDDA_OSC_UV_VMON_EN, 0);

    /* Disable Self Test Mode. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDDA_OSC_UV_SELF_TEST_SEL, 0);

    /* Set Reference for Normal Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDDA_OSC_IR_DROP_COMP_SEL, 2);

    /* Allow VMON to reset. */
    ClockP_usleep(1 * 1000);

    /* Re-enable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDDA_OSC_UV_VMON_EN, 1);

    return;
}

static void mssVDDs3p3UVSelfTest(void)
{
    CSL_mss_toprcmRegs *ptrMssTopRcmReg = CSL_MSS_TOP_RCM_getBaseAddress();

    /* Set Reference for Self Test Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDDS_3P3V_IR_DROP_COMP_SEL, 0);

    /* Set Reference for Self Test Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDDS_3P3V_UV_SELF_TEST_SEL, 1);

    /* Allow reference to change. */
    ClockP_usleep(1 * 1000);

    /* Enable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDDS_3P3V_UV_VMON_EN, 1);

    /* Allow VMON to trip. */
    ClockP_usleep(1 * 1000);

    /* Verify that VMON has toggled and corresponding ESM interrupt is generated. */
    if((gVMONMssIntCount != 0) && (gVMONVDDs3p3UVflag == 1))
    {
        test_print("VDDS33 3.3V UV Test Passed\n");
    }
    else
    {
        test_print("VDDS33 3.3V UV Test Failed - ESM Interrupt not triggered\n");
    }

    /* Disable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDDS_3P3V_UV_VMON_EN, 0);

    /* Disable Self Test Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDDS_3P3V_UV_SELF_TEST_SEL, 0);

    /* Set Reference for Normal Mode */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_REFSYS_SPARE_REG_LOWV, MSS_TOPRCM_ANA_REG_REFSYS_SPARE_REG_LOWV_VDDS_3P3V_IR_DROP_COMP_SEL, 2);

    /* Allow VMON to reset. */
    ClockP_usleep(1 * 1000);

    /* Re-enable VMON. */
    CSL_FINS(ptrMssTopRcmReg->ANA_REG_WU_CTRL_REG_LOWV, MSS_TOPRCM_ANA_REG_WU_CTRL_REG_LOWV_WU_VDDS_3P3V_UV_VMON_EN, 1);

    return;
}


void Test_initTask(void* args)
{
    Drivers_open();
    Board_driversOpen();

    test_print ("\n******************************************\n");
    test_print ("Debug: MSS VMON Test Case.\n");
    test_print ("******************************************\n");

    /* unmask Group#2 bits for VMON ESM Interrupt*/
    CSL_REG32_WR((volatile uint32_t *)(CSL_MSS_CTRL_U_BASE + CSL_MSS_CTRL_ANALOG_WU_STATUS_REG_GRP2_MASK), 0xFFF9FFFCUL);

    /* MSS VMON Tests*/

    /* VDD 1.2V Under voltage test */
    mssVDD1p2vUVSelfTest();

    /* VDD 1.2V Over voltage test */
    mssVDD1p2vOVSelfTest();

    /* VDDA_OSC 1.8V Under Voltage test */
    mssVDDAOsc1p8vUVSelfTest();

    /* VDDS33 3.3V Under Voltage test */
    mssVDDs3p3UVSelfTest();

    Board_driversClose();
    Drivers_close();

    test_print("------ end of test --------\n");
    test_print ("All tests have passed!!\n");

    vTaskDelete(NULL);
}

int main (void)
{
    /* init SOC specific modules */
    System_init();
    Board_init();

    /* This task is created at highest priority, it should create more tasks and then delete itself */
    gAppTask = xTaskCreateStatic( Test_initTask,
                                  "test_task_main",
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
