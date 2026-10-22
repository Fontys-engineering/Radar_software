/********************************************************************
 * Copyright (C) 2024 Texas Instruments Incorporated.
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
 *
 *  Name        : cslr_dss_ctrl.h
*/
#ifndef CSLR_DSS_CTRL_H_
#define CSLR_DSS_CTRL_H_

#include <drivers/hw_include/cslr.h>
#include <drivers/hw_include/tistdtypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t PID;
    volatile uint32_t HW_REG0;
    volatile uint32_t HW_REG1;
    volatile uint32_t HW_REG2;
    volatile uint32_t HW_REG3;
    volatile uint32_t DSS_SW_INT;
    volatile uint32_t DSS_TPCC_A_ERRAGG_MASK;
    volatile uint32_t DSS_TPCC_A_ERRAGG_STATUS;
    volatile uint32_t DSS_TPCC_A_ERRAGG_STATUS_RAW;
    volatile uint32_t DSS_TPCC_A_INTAGG_MASK;
    volatile uint32_t DSS_TPCC_A_INTAGG_STATUS;
    volatile uint32_t DSS_TPCC_A_INTAGG_STATUS_RAW;
    volatile uint32_t DSS_TPCC_B_ERRAGG_MASK;
    volatile uint32_t DSS_TPCC_B_ERRAGG_STATUS;
    volatile uint32_t DSS_TPCC_B_ERRAGG_STATUS_RAW;
    volatile uint32_t DSS_TPCC_B_INTAGG_MASK;
    volatile uint32_t DSS_TPCC_B_INTAGG_STATUS;
    volatile uint32_t DSS_TPCC_B_INTAGG_STATUS_RAW;
    volatile uint32_t DSS_TPCC_C_ERRAGG_MASK;
    volatile uint32_t DSS_TPCC_C_ERRAGG_STATUS;
    volatile uint32_t DSS_TPCC_C_ERRAGG_STATUS_RAW;
    volatile uint32_t DSS_TPCC_C_INTAGG_MASK;
    volatile uint32_t DSS_TPCC_C_INTAGG_STATUS;
    volatile uint32_t DSS_TPCC_C_INTAGG_STATUS_RAW;
    volatile uint32_t DSS_TPCC_MEMINIT_START;
    volatile uint32_t DSS_TPCC_MEMINIT_STATUS;
    volatile uint32_t DSS_TPCC_MEMINIT_DONE;
    volatile uint8_t  Resv_128[20];
    volatile uint32_t DSS_DSP_L2RAM_MEMINIT_START;
    volatile uint32_t DSS_DSP_L2RAM_MEMINIT_STATUS;
    volatile uint32_t DSS_DSP_L2RAM_MEMINIT_DONE;
    volatile uint8_t  Resv_152[12];
    volatile uint32_t DSS_L3RAM_MEMINIT_START;
    volatile uint32_t DSS_L3RAM_MEMINIT_STATUS;
    volatile uint32_t DSS_L3RAM_MEMINIT_DONE;
    volatile uint8_t  Resv_176[12];
    volatile uint32_t DSS_MAILBOX_MEMINIT_START;
    volatile uint32_t DSS_MAILBOX_MEMINIT_STATUS;
    volatile uint32_t DSS_MAILBOX_MEMINIT_DONE;
    volatile uint32_t DSS_TPCC_A_PARITY_CTRL;
    volatile uint32_t DSS_TPCC_B_PARITY_CTRL;
    volatile uint32_t DSS_TPCC_C_PARITY_CTRL;
    volatile uint32_t DSS_TPCC_A_PARITY_STATUS;
    volatile uint32_t DSS_TPCC_B_PARITY_STATUS;
    volatile uint32_t DSS_TPCC_C_PARITY_STATUS;
    volatile uint32_t TPTC_DBS_CONFIG;
    volatile uint32_t DSS_DSP_BOOTCFG;
    volatile uint32_t DSS_DSP_NMI_GATE;
    volatile uint32_t DSS_PBIST_KEY_RESET;
    volatile uint32_t DSS_PBIST_REG0;
    volatile uint32_t DSS_PBIST_REG1;
    volatile uint32_t DSS_TPTC_BOUNDARY_CFG0;
    volatile uint32_t DSS_TPTC_BOUNDARY_CFG1;
    volatile uint8_t  Resv_248[4];
    volatile uint32_t DSS_TPTC_XID_REORDER_CFG0;
    volatile uint32_t DSS_TPTC_XID_REORDER_CFG1;
    volatile uint8_t  Resv_264[8];
    volatile uint32_t ESM_GATING0;
    volatile uint32_t ESM_GATING1;
    volatile uint32_t ESM_GATING2;
    volatile uint32_t ESM_GATING3;
    volatile uint8_t  Resv_1376[1096];
    volatile uint32_t DSS_PERIPH_ERRAGG_MASK0;
    volatile uint32_t DSS_PERIPH_ERRAGG_STATUS0;
    volatile uint32_t DSS_PERIPH_ERRAGG_STATUS_RAW0;
    volatile uint32_t DSS_DSP_MBOX_WRITE_DONE;
    volatile uint32_t DSS_DSP_MBOX_READ_REQ;
    volatile uint32_t DSS_DSP_MBOX_READ_DONE;
    volatile uint32_t DSS_WDT_EVENT_CAPTURE_SEL;
    volatile uint32_t DSS_RTIA_EVENT_CAPTURE_SEL;
    volatile uint32_t DSS_RTIB_EVENT_CAPTURE_SEL;
    volatile uint32_t DBG_ACK_CPU_CTRL;
    volatile uint32_t DBG_ACK_CTL0;
    volatile uint32_t DBG_ACK_CTL1;
    volatile uint32_t DSS_DSP_INT_SEL;
    volatile uint32_t DSS_CBUFF_TRIGGER_SEL;
    volatile uint8_t  Resv_2048[616];
    volatile uint32_t DSS_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_BUS_SAFETY_FI;
    volatile uint32_t DSS_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2068[8];
    volatile uint32_t DSS_DSP_MDMA_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_DSP_MDMA_BUS_SAFETY_FI;
    volatile uint32_t DSS_DSP_MDMA_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2104[24];
    volatile uint32_t DSS_L3_BANKA_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_L3_BANKA_BUS_SAFETY_FI;
    volatile uint32_t DSS_L3_BANKA_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2140[24];
    volatile uint32_t DSS_L3_BANKB_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_L3_BANKB_BUS_SAFETY_FI;
    volatile uint32_t DSS_L3_BANKB_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2176[24];
    volatile uint32_t DSS_L3_BANKC_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_L3_BANKC_BUS_SAFETY_FI;
    volatile uint32_t DSS_L3_BANKC_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2212[24];
    volatile uint32_t DSS_L3_BANKD_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_L3_BANKD_BUS_SAFETY_FI;
    volatile uint32_t DSS_L3_BANKD_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2248[24];
    volatile uint32_t DSS_DSP_SDMA_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_DSP_SDMA_BUS_SAFETY_FI;
    volatile uint32_t DSS_DSP_SDMA_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2280[20];
    volatile uint32_t DSS_TPTC_A0_RD_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_A0_RD_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_A0_RD_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2304[12];
    volatile uint32_t DSS_TPTC_A1_RD_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_A1_RD_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_A1_RD_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2328[12];
    volatile uint32_t DSS_TPTC_B0_RD_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_B0_RD_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_B0_RD_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2352[12];
    volatile uint32_t DSS_TPTC_B1_RD_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_B1_RD_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_B1_RD_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2376[12];
    volatile uint32_t DSS_TPTC_C0_RD_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_C0_RD_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_C0_RD_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2400[12];
    volatile uint32_t DSS_TPTC_C1_RD_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_C1_RD_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_C1_RD_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2520[108];
    volatile uint32_t DSS_TPTC_A0_WR_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_A0_WR_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_A0_WR_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2548[16];
    volatile uint32_t DSS_TPTC_A1_WR_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_A1_WR_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_A1_WR_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2576[16];
    volatile uint32_t DSS_TPTC_B0_WR_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_B0_WR_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_B0_WR_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2604[16];
    volatile uint32_t DSS_TPTC_B1_WR_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_B1_WR_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_B1_WR_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2632[16];
    volatile uint32_t DSS_TPTC_C0_WR_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_C0_WR_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_C0_WR_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2660[16];
    volatile uint32_t DSS_TPTC_C1_WR_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_TPTC_C1_WR_BUS_SAFETY_FI;
    volatile uint32_t DSS_TPTC_C1_WR_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2800[128];
    volatile uint32_t DSS_MDO_FIFO_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_MDO_FIFO_BUS_SAFETY_FI;
    volatile uint32_t DSS_MDO_FIFO_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_2832[20];
    volatile uint32_t DSS_CBUFF_FIFO_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_CBUFF_FIFO_BUS_SAFETY_FI;
    volatile uint32_t DSS_CBUFF_FIFO_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3024[180];
    volatile uint32_t DSS_MCRC_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_MCRC_BUS_SAFETY_FI;
    volatile uint32_t DSS_MCRC_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3056[20];
    volatile uint32_t DSS_PCR_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_PCR_BUS_SAFETY_FI;
    volatile uint32_t DSS_PCR_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3088[20];
    volatile uint32_t DSS_HWA_DMA0_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_HWA_DMA0_BUS_SAFETY_FI;
    volatile uint32_t DSS_HWA_DMA0_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3120[20];
    volatile uint32_t DSS_HWA_DMA1_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_HWA_DMA1_BUS_SAFETY_FI;
    volatile uint32_t DSS_HWA_DMA1_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3152[20];
    volatile uint32_t DSS_CM4_M_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_CM4_M_BUS_SAFETY_FI;
    volatile uint32_t DSS_CM4_M_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3184[20];
    volatile uint32_t DSS_CM4_S_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_CM4_S_BUS_SAFETY_FI;
    volatile uint32_t DSS_CM4_S_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3216[20];
    volatile uint32_t DSS_MBOX_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_MBOX_BUS_SAFETY_FI;
    volatile uint32_t DSS_MBOX_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3248[20];
    volatile uint32_t DSS_DSS2RSS_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_DSS2RSS_BUS_SAFETY_FI;
    volatile uint32_t DSS_DSS2RSS_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3280[20];
    volatile uint32_t DSS_RSS2DSS_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_RSS2DSS_BUS_SAFETY_FI;
    volatile uint32_t DSS_RSS2DSS_BUS_SAFETY_ERR;
    volatile uint8_t  Resv_3312[20];
    volatile uint32_t DSS_DSS2MSS_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_DSS2MSS_BUS_SAFETY_FI;
    volatile uint32_t DSS_DSS2MSS_BUS_SAFETY_ERR;
    volatile uint32_t DSS_MSS2DSS_BUS_SAFETY_CTRL;
    volatile uint32_t DSS_MSS2DSS_BUS_SAFETY_FI;
    volatile uint32_t DSS_MSS2DSS_BUS_SAFETY_ERR;
    volatile uint32_t DSS_BUS_SAFETY_LOOPBACK_SLV_SEL;
    volatile uint32_t DSS_BUS_SAFETY_LOOPBACK_MST_SEL;
    volatile uint8_t  Resv_3356[12];
    volatile uint32_t DSS_DMA_REQ_SEL;
    volatile uint32_t DSS_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG;
    volatile uint32_t DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG;
    volatile uint8_t  Resv_4048[648];
    volatile uint32_t HW_SPARE_RW0;
    volatile uint32_t HW_SPARE_RW1;
    volatile uint32_t HW_SPARE_RW2;
    volatile uint32_t HW_SPARE_RW3;
    volatile uint32_t HW_SPARE_RO0;
    volatile uint32_t HW_SPARE_RO1;
    volatile uint32_t HW_SPARE_RO2;
    volatile uint32_t HW_SPARE_RO3;
    volatile uint32_t DSS_DSP_MBOX_READ_DONE_ACK;
    volatile uint32_t HW_SPARE_REC;
    volatile uint8_t  Resv_4104[16];
    volatile uint32_t LOCK0_KICK0;
    volatile uint32_t LOCK0_KICK1;
    volatile uint32_t INTR_RAW_STATUS;
    volatile uint32_t INTR_ENABLED_STATUS_CLEAR;
    volatile uint32_t INTR_ENABLE;
    volatile uint32_t INTR_ENABLE_CLEAR;
    volatile uint32_t EOI;
    volatile uint32_t FAULT_ADDRESS;
    volatile uint32_t FAULT_TYPE_STATUS;
    volatile uint32_t FAULT_ATTR_STATUS;
    volatile uint32_t FAULT_CLEAR;
} CSL_dss_ctrlRegs;


/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_DSS_CTRL_PID                                                       (0x00000000U)
#define CSL_DSS_CTRL_HW_REG0                                                   (0x00000004U)
#define CSL_DSS_CTRL_HW_REG1                                                   (0x00000008U)
#define CSL_DSS_CTRL_HW_REG2                                                   (0x0000000CU)
#define CSL_DSS_CTRL_HW_REG3                                                   (0x00000010U)
#define CSL_DSS_CTRL_DSS_SW_INT                                                (0x00000014U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK                                    (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS                                  (0x0000001CU)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW                              (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK                                    (0x00000024U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS                                  (0x00000028U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW                              (0x0000002CU)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK                                    (0x00000030U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS                                  (0x00000034U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW                              (0x00000038U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK                                    (0x0000003CU)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS                                  (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW                              (0x00000044U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK                                    (0x00000048U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS                                  (0x0000004CU)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW                              (0x00000050U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK                                    (0x00000054U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS                                  (0x00000058U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW                              (0x0000005CU)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START                                    (0x00000060U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS                                   (0x00000064U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE                                     (0x00000068U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START                               (0x00000080U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS                              (0x00000084U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE                                (0x00000088U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START                                   (0x00000098U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS                                  (0x0000009CU)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE                                    (0x000000A0U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_START                                 (0x000000B0U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_STATUS                                (0x000000B4U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_DONE                                  (0x000000B8U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL                                    (0x000000BCU)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL                                    (0x000000C0U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL                                    (0x000000C4U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_STATUS                                  (0x000000C8U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_STATUS                                  (0x000000CCU)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_STATUS                                  (0x000000D0U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG                                           (0x000000D4U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG                                           (0x000000D8U)
#define CSL_DSS_CTRL_DSS_DSP_NMI_GATE                                          (0x000000DCU)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET                                       (0x000000E0U)
#define CSL_DSS_CTRL_DSS_PBIST_REG0                                            (0x000000E4U)
#define CSL_DSS_CTRL_DSS_PBIST_REG1                                            (0x000000E8U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0                                    (0x000000ECU)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1                                    (0x000000F0U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0                                 (0x000000F8U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1                                 (0x000000FCU)
#define CSL_DSS_CTRL_ESM_GATING0                                               (0x00000108U)
#define CSL_DSS_CTRL_ESM_GATING1                                               (0x0000010CU)
#define CSL_DSS_CTRL_ESM_GATING2                                               (0x00000110U)
#define CSL_DSS_CTRL_ESM_GATING3                                               (0x00000114U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0                                   (0x00000560U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0                                 (0x00000564U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0                             (0x00000568U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE                                   (0x0000056CU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ                                     (0x00000570U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE                                    (0x00000574U)
#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL                                 (0x00000578U)
#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL                                (0x0000057CU)
#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL                                (0x00000580U)
#define CSL_DSS_CTRL_DBG_ACK_CPU_CTRL                                          (0x00000584U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0                                              (0x00000588U)
#define CSL_DSS_CTRL_DBG_ACK_CTL1                                              (0x0000058CU)
#define CSL_DSS_CTRL_DSS_DSP_INT_SEL                                           (0x00000590U)
#define CSL_DSS_CTRL_DSS_CBUFF_TRIGGER_SEL                                     (0x00000594U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL                                       (0x00000800U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI                                         (0x00000804U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR                                        (0x00000808U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL                              (0x00000814U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI                                (0x00000818U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR                               (0x0000081CU)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL                              (0x00000838U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI                                (0x0000083CU)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR                               (0x00000840U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL                              (0x0000085CU)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI                                (0x00000860U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR                               (0x00000864U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL                              (0x00000880U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI                                (0x00000884U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR                               (0x00000888U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL                              (0x000008A4U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI                                (0x000008A8U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR                               (0x000008ACU)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL                              (0x000008C8U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI                                (0x000008CCU)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR                               (0x000008D0U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL                            (0x000008E8U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI                              (0x000008ECU)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR                             (0x000008F0U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL                            (0x00000900U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI                              (0x00000904U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR                             (0x00000908U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL                            (0x00000918U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI                              (0x0000091CU)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR                             (0x00000920U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL                            (0x00000930U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI                              (0x00000934U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR                             (0x00000938U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL                            (0x00000948U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI                              (0x0000094CU)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR                             (0x00000950U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL                            (0x00000960U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI                              (0x00000964U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR                             (0x00000968U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL                            (0x000009D8U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI                              (0x000009DCU)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR                             (0x000009E0U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL                            (0x000009F4U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI                              (0x000009F8U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR                             (0x000009FCU)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL                            (0x00000A10U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI                              (0x00000A14U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR                             (0x00000A18U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL                            (0x00000A2CU)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI                              (0x00000A30U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR                             (0x00000A34U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL                            (0x00000A48U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI                              (0x00000A4CU)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR                             (0x00000A50U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL                            (0x00000A64U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI                              (0x00000A68U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR                             (0x00000A6CU)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL                              (0x00000AF0U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI                                (0x00000AF4U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR                               (0x00000AF8U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL                            (0x00000B10U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI                              (0x00000B14U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR                             (0x00000B18U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL                                  (0x00000BD0U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI                                    (0x00000BD4U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR                                   (0x00000BD8U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL                                   (0x00000BF0U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI                                     (0x00000BF4U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR                                    (0x00000BF8U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL                              (0x00000C10U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI                                (0x00000C14U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR                               (0x00000C18U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL                              (0x00000C30U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI                                (0x00000C34U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR                               (0x00000C38U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL                                 (0x00000C50U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI                                   (0x00000C54U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR                                  (0x00000C58U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL                                 (0x00000C70U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI                                   (0x00000C74U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR                                  (0x00000C78U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL                                  (0x00000C90U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI                                    (0x00000C94U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR                                   (0x00000C98U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL                               (0x00000CB0U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI                                 (0x00000CB4U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR                                (0x00000CB8U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL                               (0x00000CD0U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI                                 (0x00000CD4U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR                                (0x00000CD8U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL                               (0x00000CF0U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI                                 (0x00000CF4U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR                                (0x00000CF8U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL                               (0x00000CFCU)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI                                 (0x00000D00U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR                                (0x00000D04U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL                           (0x00000D08U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL                           (0x00000D0CU)
#define CSL_DSS_CTRL_DSS_DMA_REQ_SEL                                           (0x00000D1CU)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG                                      (0x00000D20U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG                          (0x00000D24U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG                           (0x00000D28U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG                           (0x00000D2CU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG                           (0x00000D30U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG                           (0x00000D34U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG                        (0x00000D38U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG                        (0x00000D3CU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG                        (0x00000D40U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG                        (0x00000D44U)
#define CSL_DSS_CTRL_HW_SPARE_RW0                                              (0x00000FD0U)
#define CSL_DSS_CTRL_HW_SPARE_RW1                                              (0x00000FD4U)
#define CSL_DSS_CTRL_HW_SPARE_RW2                                              (0x00000FD8U)
#define CSL_DSS_CTRL_HW_SPARE_RW3                                              (0x00000FDCU)
#define CSL_DSS_CTRL_HW_SPARE_RO0                                              (0x00000FE0U)
#define CSL_DSS_CTRL_HW_SPARE_RO1                                              (0x00000FE4U)
#define CSL_DSS_CTRL_HW_SPARE_RO2                                              (0x00000FE8U)
#define CSL_DSS_CTRL_HW_SPARE_RO3                                              (0x00000FECU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_ACK                                (0x00000FF0U)
#define CSL_DSS_CTRL_HW_SPARE_REC                                              (0x00000FF4U)
#define CSL_DSS_CTRL_LOCK0_KICK0                                               (0x00001008U)
#define CSL_DSS_CTRL_LOCK0_KICK1                                               (0x0000100CU)
#define CSL_DSS_CTRL_INTR_RAW_STATUS                                           (0x00001010U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR                                 (0x00001014U)
#define CSL_DSS_CTRL_INTR_ENABLE                                               (0x00001018U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR                                         (0x0000101CU)
#define CSL_DSS_CTRL_EOI                                                       (0x00001020U)
#define CSL_DSS_CTRL_FAULT_ADDRESS                                             (0x00001024U)
#define CSL_DSS_CTRL_FAULT_TYPE_STATUS                                         (0x00001028U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS                                         (0x0000102CU)
#define CSL_DSS_CTRL_FAULT_CLEAR                                               (0x00001030U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* PID */

#define CSL_DSS_CTRL_PID_PID_MINOR_MASK                                        (0x0000003FU)
#define CSL_DSS_CTRL_PID_PID_MINOR_SHIFT                                       (0x00000000U)
#define CSL_DSS_CTRL_PID_PID_MINOR_RESETVAL                                    (0x00000015U)
#define CSL_DSS_CTRL_PID_PID_MINOR_MAX                                         (0x0000003FU)

#define CSL_DSS_CTRL_PID_PID_CUSTOM_MASK                                       (0x000000C0U)
#define CSL_DSS_CTRL_PID_PID_CUSTOM_SHIFT                                      (0x00000006U)
#define CSL_DSS_CTRL_PID_PID_CUSTOM_RESETVAL                                   (0x00000000U)
#define CSL_DSS_CTRL_PID_PID_CUSTOM_MAX                                        (0x00000003U)

#define CSL_DSS_CTRL_PID_PID_MAJOR_MASK                                        (0x00000700U)
#define CSL_DSS_CTRL_PID_PID_MAJOR_SHIFT                                       (0x00000008U)
#define CSL_DSS_CTRL_PID_PID_MAJOR_RESETVAL                                    (0x00000002U)
#define CSL_DSS_CTRL_PID_PID_MAJOR_MAX                                         (0x00000007U)

#define CSL_DSS_CTRL_PID_PID_MISC_MASK                                         (0x0000F800U)
#define CSL_DSS_CTRL_PID_PID_MISC_SHIFT                                        (0x0000000BU)
#define CSL_DSS_CTRL_PID_PID_MISC_RESETVAL                                     (0x00000000U)
#define CSL_DSS_CTRL_PID_PID_MISC_MAX                                          (0x0000001FU)

#define CSL_DSS_CTRL_PID_PID_MSB16_MASK                                        (0xFFFF0000U)
#define CSL_DSS_CTRL_PID_PID_MSB16_SHIFT                                       (0x00000010U)
#define CSL_DSS_CTRL_PID_PID_MSB16_RESETVAL                                    (0x00006180U)
#define CSL_DSS_CTRL_PID_PID_MSB16_MAX                                         (0x0000FFFFU)

#define CSL_DSS_CTRL_PID_RESETVAL                                              (0x61800215U)

/* HW_REG0 */

#define CSL_DSS_CTRL_HW_REG0_HW_REG0_HWREG0_MASK                               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_REG0_HW_REG0_HWREG0_SHIFT                              (0x00000000U)
#define CSL_DSS_CTRL_HW_REG0_HW_REG0_HWREG0_RESETVAL                           (0x00000000U)
#define CSL_DSS_CTRL_HW_REG0_HW_REG0_HWREG0_MAX                                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_REG0_RESETVAL                                          (0x00000000U)

/* HW_REG1 */

#define CSL_DSS_CTRL_HW_REG1_HW_REG1_HWREG1_MASK                               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_REG1_HW_REG1_HWREG1_SHIFT                              (0x00000000U)
#define CSL_DSS_CTRL_HW_REG1_HW_REG1_HWREG1_RESETVAL                           (0x00000000U)
#define CSL_DSS_CTRL_HW_REG1_HW_REG1_HWREG1_MAX                                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_REG1_RESETVAL                                          (0x00000000U)

/* HW_REG2 */

#define CSL_DSS_CTRL_HW_REG2_HW_REG2_HWREG2_MASK                               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_REG2_HW_REG2_HWREG2_SHIFT                              (0x00000000U)
#define CSL_DSS_CTRL_HW_REG2_HW_REG2_HWREG2_RESETVAL                           (0x00000000U)
#define CSL_DSS_CTRL_HW_REG2_HW_REG2_HWREG2_MAX                                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_REG2_RESETVAL                                          (0x00000000U)

/* HW_REG3 */

#define CSL_DSS_CTRL_HW_REG3_HW_REG3_HWREG3_MASK                               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_REG3_HW_REG3_HWREG3_SHIFT                              (0x00000000U)
#define CSL_DSS_CTRL_HW_REG3_HW_REG3_HWREG3_RESETVAL                           (0x00000000U)
#define CSL_DSS_CTRL_HW_REG3_HW_REG3_HWREG3_MAX                                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_REG3_RESETVAL                                          (0x00000000U)

/* DSS_SW_INT */

#define CSL_DSS_CTRL_DSS_SW_INT_DSS_SW_INT_DSS_SWINT_MASK                      (0x0000000FU)
#define CSL_DSS_CTRL_DSS_SW_INT_DSS_SW_INT_DSS_SWINT_SHIFT                     (0x00000000U)
#define CSL_DSS_CTRL_DSS_SW_INT_DSS_SW_INT_DSS_SWINT_RESETVAL                  (0x00000000U)
#define CSL_DSS_CTRL_DSS_SW_INT_DSS_SW_INT_DSS_SWINT_MAX                       (0x0000000FU)

#define CSL_DSS_CTRL_DSS_SW_INT_RESETVAL                                       (0x00000000U)

/* DSS_TPCC_A_ERRAGG_MASK */

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPCC_A_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_DSS_TPCC_A_ERRAGG_MASK_TPTC_A1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_MASK_RESETVAL                           (0x00000000U)

/* DSS_TPCC_A_ERRAGG_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPCC_A_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_DSS_TPCC_A_ERRAGG_STATUS_TPTC_A1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RESETVAL                         (0x00000000U)

/* DSS_TPCC_A_ERRAGG_STATUS_RAW */

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPCC_A_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_DSS_TPCC_A_ERRAGG_STATUS_RAW_TPTC_A1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_ERRAGG_STATUS_RAW_RESETVAL                     (0x00000000U)

/* DSS_TPCC_A_INTAGG_MASK */

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPCC_A_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPTC_A0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPTC_A0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPTC_A0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPTC_A0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPTC_A1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPTC_A1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPTC_A1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_DSS_TPCC_A_INTAGG_MASK_TPTC_A1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_MASK_RESETVAL                           (0x00000000U)

/* DSS_TPCC_A_INTAGG_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPCC_A_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPTC_A0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPTC_A0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPTC_A0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPTC_A0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPTC_A1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPTC_A1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPTC_A1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_DSS_TPCC_A_INTAGG_STATUS_TPTC_A1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RESETVAL                         (0x00000000U)

/* DSS_TPCC_A_INTAGG_STATUS_RAW */

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPCC_A_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPTC_A0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPTC_A0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPTC_A0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPTC_A0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPTC_A1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPTC_A1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPTC_A1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_DSS_TPCC_A_INTAGG_STATUS_RAW_TPTC_A1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_INTAGG_STATUS_RAW_RESETVAL                     (0x00000000U)

/* DSS_TPCC_B_ERRAGG_MASK */

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPCC_B_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_DSS_TPCC_B_ERRAGG_MASK_TPTC_B1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK_RESETVAL                           (0x00000000U)

/* DSS_TPCC_B_ERRAGG_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPCC_B_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_DSS_TPCC_B_ERRAGG_STATUS_TPTC_B1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RESETVAL                         (0x00000000U)

/* DSS_TPCC_B_ERRAGG_STATUS_RAW */

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPCC_B_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_DSS_TPCC_B_ERRAGG_STATUS_RAW_TPTC_B1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS_RAW_RESETVAL                     (0x00000000U)

/* DSS_TPCC_B_INTAGG_MASK */

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPCC_B_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPTC_B0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPTC_B0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPTC_B0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPTC_B0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPTC_B1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPTC_B1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPTC_B1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_DSS_TPCC_B_INTAGG_MASK_TPTC_B1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK_RESETVAL                           (0x00000000U)

/* DSS_TPCC_B_INTAGG_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPCC_B_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPTC_B0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPTC_B0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPTC_B0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPTC_B0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPTC_B1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPTC_B1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPTC_B1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_DSS_TPCC_B_INTAGG_STATUS_TPTC_B1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RESETVAL                         (0x00000000U)

/* DSS_TPCC_B_INTAGG_STATUS_RAW */

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPCC_B_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPTC_B0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPTC_B0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPTC_B0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPTC_B0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPTC_B1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPTC_B1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPTC_B1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_DSS_TPCC_B_INTAGG_STATUS_RAW_TPTC_B1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS_RAW_RESETVAL                     (0x00000000U)

/* DSS_TPCC_C_ERRAGG_MASK */

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPCC_C_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_DSS_TPCC_C_ERRAGG_MASK_TPTC_C1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_MASK_RESETVAL                           (0x00000000U)

/* DSS_TPCC_C_ERRAGG_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPCC_C_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_DSS_TPCC_C_ERRAGG_STATUS_TPTC_C1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RESETVAL                         (0x00000000U)

/* DSS_TPCC_C_ERRAGG_STATUS_RAW */

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_ERRINT_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_ERRINT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_ERRINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_ERRINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_MPINT_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_MPINT_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_MPINT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_MPINT_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_ERR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_ERR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_ERR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_ERR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_PARITY_ERR_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_PARITY_ERR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_PARITY_ERR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_PARITY_ERR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_WRITE_ACCESS_ERROR_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_WRITE_ACCESS_ERROR_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_WRITE_ACCESS_ERROR_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_WRITE_ACCESS_ERROR_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_WRITE_ACCESS_ERROR_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_WRITE_ACCESS_ERROR_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_WRITE_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_WRITE_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_READ_ACCESS_ERROR_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_READ_ACCESS_ERROR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPCC_C_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_READ_ACCESS_ERROR_MASK (0x02000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_READ_ACCESS_ERROR_SHIFT (0x00000019U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C0_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_READ_ACCESS_ERROR_MASK (0x04000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_READ_ACCESS_ERROR_SHIFT (0x0000001AU)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_READ_ACCESS_ERROR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_DSS_TPCC_C_ERRAGG_STATUS_RAW_TPTC_C1_READ_ACCESS_ERROR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_ERRAGG_STATUS_RAW_RESETVAL                     (0x00000000U)

/* DSS_TPCC_C_INTAGG_MASK */

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPCC_C_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPTC_C0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPTC_C0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPTC_C0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPTC_C0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPTC_C1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPTC_C1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPTC_C1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_DSS_TPCC_C_INTAGG_MASK_TPTC_C1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_MASK_RESETVAL                           (0x00000000U)

/* DSS_TPCC_C_INTAGG_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPCC_C_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPTC_C0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPTC_C0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPTC_C0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPTC_C0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPTC_C1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPTC_C1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPTC_C1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_DSS_TPCC_C_INTAGG_STATUS_TPTC_C1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RESETVAL                         (0x00000000U)

/* DSS_TPCC_C_INTAGG_STATUS_RAW */

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INTG_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INTG_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INTG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INTG_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT0_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT0_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT1_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT1_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT2_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT2_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT3_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT3_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT4_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT4_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT5_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT5_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT6_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT6_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT7_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT7_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPCC_C_INT7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPTC_C0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPTC_C0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPTC_C0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPTC_C0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPTC_C1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPTC_C1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPTC_C1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_DSS_TPCC_C_INTAGG_STATUS_RAW_TPTC_C1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_INTAGG_STATUS_RAW_RESETVAL                     (0x00000000U)

/* DSS_TPCC_MEMINIT_START */

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_A_MEMINIT_START_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_A_MEMINIT_START_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_A_MEMINIT_START_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_A_MEMINIT_START_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_B_MEMINIT_START_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_B_MEMINIT_START_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_B_MEMINIT_START_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_B_MEMINIT_START_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_C_MEMINIT_START_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_C_MEMINIT_START_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_C_MEMINIT_START_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_DSS_TPCC_MEMINIT_START_TPCC_C_MEMINIT_START_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_START_RESETVAL                           (0x00000000U)

/* DSS_TPCC_MEMINIT_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_A_MEMINIT_STATUS_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_A_MEMINIT_STATUS_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_A_MEMINIT_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_A_MEMINIT_STATUS_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_B_MEMINIT_STATUS_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_B_MEMINIT_STATUS_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_B_MEMINIT_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_B_MEMINIT_STATUS_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_C_MEMINIT_STATUS_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_C_MEMINIT_STATUS_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_C_MEMINIT_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_DSS_TPCC_MEMINIT_STATUS_TPCC_C_MEMINIT_STATUS_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_STATUS_RESETVAL                          (0x00000000U)

/* DSS_TPCC_MEMINIT_DONE */

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_A_MEMINIT_DONE_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_A_MEMINIT_DONE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_A_MEMINIT_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_A_MEMINIT_DONE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_B_MEMINIT_DONE_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_B_MEMINIT_DONE_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_B_MEMINIT_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_B_MEMINIT_DONE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_C_MEMINIT_DONE_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_C_MEMINIT_DONE_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_C_MEMINIT_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_DSS_TPCC_MEMINIT_DONE_TPCC_C_MEMINIT_DONE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_MEMINIT_DONE_RESETVAL                            (0x00000000U)

/* DSS_DSP_L2RAM_MEMINIT_START */

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB00_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB00_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB00_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB00_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB01_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB01_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB01_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB01_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB10_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB10_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB10_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB10_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB11_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB11_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB11_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB11_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB20_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB20_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB20_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB20_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB21_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB21_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB21_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB21_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB30_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB30_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB30_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB30_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB31_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB31_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB31_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_DSS_DSP_L2RAM_MEMINIT_START_VB31_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_START_RESETVAL                      (0x00000000U)

/* DSS_DSP_L2RAM_MEMINIT_STATUS */

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB00_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB00_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB00_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB00_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB01_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB01_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB01_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB01_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB10_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB10_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB10_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB10_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB11_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB11_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB11_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB11_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB20_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB20_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB20_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB20_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB21_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB21_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB21_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB21_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB30_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB30_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB30_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB30_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB31_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB31_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB31_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_DSS_DSP_L2RAM_MEMINIT_STATUS_VB31_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_STATUS_RESETVAL                     (0x00000000U)

/* DSS_DSP_L2RAM_MEMINIT_DONE */

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB00_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB00_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB00_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB00_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB01_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB01_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB01_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB01_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB10_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB10_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB10_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB10_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB11_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB11_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB11_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB11_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB20_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB20_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB20_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB20_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB21_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB21_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB21_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB21_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB30_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB30_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB30_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB30_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB31_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB31_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB31_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_DSS_DSP_L2RAM_MEMINIT_DONE_VB31_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_L2RAM_MEMINIT_DONE_RESETVAL                       (0x00000000U)

/* DSS_L3RAM_MEMINIT_START */

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM0_MEMINIT_START_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM0_MEMINIT_START_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM0_MEMINIT_START_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM0_MEMINIT_START_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM1_MEMINIT_START_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM1_MEMINIT_START_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM1_MEMINIT_START_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM1_MEMINIT_START_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM2_MEMINIT_START_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM2_MEMINIT_START_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM2_MEMINIT_START_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM2_MEMINIT_START_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM3_MEMINIT_START_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM3_MEMINIT_START_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM3_MEMINIT_START_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_DSS_L3RAM_MEMINIT_START_L3RAM3_MEMINIT_START_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_START_RESETVAL                          (0x00000000U)

/* DSS_L3RAM_MEMINIT_STATUS */

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM0_MEMINIT_STATUS_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM0_MEMINIT_STATUS_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM0_MEMINIT_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM0_MEMINIT_STATUS_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM1_MEMINIT_STATUS_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM1_MEMINIT_STATUS_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM1_MEMINIT_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM1_MEMINIT_STATUS_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM2_MEMINIT_STATUS_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM2_MEMINIT_STATUS_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM2_MEMINIT_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM2_MEMINIT_STATUS_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM3_MEMINIT_STATUS_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM3_MEMINIT_STATUS_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM3_MEMINIT_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_DSS_L3RAM_MEMINIT_STATUS_L3RAM3_MEMINIT_STATUS_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_STATUS_RESETVAL                         (0x00000000U)

/* DSS_L3RAM_MEMINIT_DONE */

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM0_MEMINIT_DONE_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM0_MEMINIT_DONE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM0_MEMINIT_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM0_MEMINIT_DONE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM1_MEMINIT_DONE_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM1_MEMINIT_DONE_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM1_MEMINIT_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM1_MEMINIT_DONE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM2_MEMINIT_DONE_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM2_MEMINIT_DONE_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM2_MEMINIT_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM2_MEMINIT_DONE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM3_MEMINIT_DONE_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM3_MEMINIT_DONE_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM3_MEMINIT_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_DSS_L3RAM_MEMINIT_DONE_L3RAM3_MEMINIT_DONE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_L3RAM_MEMINIT_DONE_RESETVAL                           (0x00000000U)

/* DSS_MAILBOX_MEMINIT_START */

#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_START_DSS_MAILBOX_MEMINIT_START_MEMINIT_START_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_START_DSS_MAILBOX_MEMINIT_START_MEMINIT_START_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_START_DSS_MAILBOX_MEMINIT_START_MEMINIT_START_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_START_DSS_MAILBOX_MEMINIT_START_MEMINIT_START_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_START_RESETVAL                        (0x00000000U)

/* DSS_MAILBOX_MEMINIT_STATUS */

#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_STATUS_DSS_MAILBOX_MEMINIT_STATUS_MEMINIT_STATUS_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_STATUS_DSS_MAILBOX_MEMINIT_STATUS_MEMINIT_STATUS_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_STATUS_DSS_MAILBOX_MEMINIT_STATUS_MEMINIT_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_STATUS_DSS_MAILBOX_MEMINIT_STATUS_MEMINIT_STATUS_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_STATUS_RESETVAL                       (0x00000000U)

/* DSS_MAILBOX_MEMINIT_DONE */

#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_DONE_DSS_MAILBOX_MEMINIT_DONE_MEMINIT_DONE_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_DONE_DSS_MAILBOX_MEMINIT_DONE_MEMINIT_DONE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_DONE_DSS_MAILBOX_MEMINIT_DONE_MEMINIT_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_DONE_DSS_MAILBOX_MEMINIT_DONE_MEMINIT_DONE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_MAILBOX_MEMINIT_DONE_RESETVAL                         (0x00000000U)

/* DSS_TPCC_A_PARITY_CTRL */

#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_EN_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_EN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_EN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_EN_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_TESTEN_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_TESTEN_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_TESTEN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_TESTEN_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_ERR_CLR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_ERR_CLR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_ERR_CLR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_DSS_TPCC_A_PARITY_CTRL_PARITY_ERR_CLR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_CTRL_RESETVAL                           (0x00000000U)

/* DSS_TPCC_B_PARITY_CTRL */

#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_EN_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_EN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_EN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_EN_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_TESTEN_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_TESTEN_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_TESTEN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_TESTEN_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_ERR_CLR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_ERR_CLR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_ERR_CLR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_DSS_TPCC_B_PARITY_CTRL_PARITY_ERR_CLR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_CTRL_RESETVAL                           (0x00000000U)

/* DSS_TPCC_C_PARITY_CTRL */

#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_EN_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_EN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_EN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_EN_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_TESTEN_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_TESTEN_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_TESTEN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_TESTEN_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_ERR_CLR_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_ERR_CLR_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_ERR_CLR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_DSS_TPCC_C_PARITY_CTRL_PARITY_ERR_CLR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_CTRL_RESETVAL                           (0x00000000U)

/* DSS_TPCC_A_PARITY_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_STATUS_DSS_TPCC_A_PARITY_STATUS_PARITY_ADDR_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_STATUS_DSS_TPCC_A_PARITY_STATUS_PARITY_ADDR_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_STATUS_DSS_TPCC_A_PARITY_STATUS_PARITY_ADDR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_STATUS_DSS_TPCC_A_PARITY_STATUS_PARITY_ADDR_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPCC_A_PARITY_STATUS_RESETVAL                         (0x00000000U)

/* DSS_TPCC_B_PARITY_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_STATUS_DSS_TPCC_B_PARITY_STATUS_PARITY_ADDR_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_STATUS_DSS_TPCC_B_PARITY_STATUS_PARITY_ADDR_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_STATUS_DSS_TPCC_B_PARITY_STATUS_PARITY_ADDR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_STATUS_DSS_TPCC_B_PARITY_STATUS_PARITY_ADDR_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPCC_B_PARITY_STATUS_RESETVAL                         (0x00000000U)

/* DSS_TPCC_C_PARITY_STATUS */

#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_STATUS_DSS_TPCC_C_PARITY_STATUS_PARITY_ADDR_MASK (0x000001FFU)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_STATUS_DSS_TPCC_C_PARITY_STATUS_PARITY_ADDR_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_STATUS_DSS_TPCC_C_PARITY_STATUS_PARITY_ADDR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_STATUS_DSS_TPCC_C_PARITY_STATUS_PARITY_ADDR_MAX (0x000001FFU)

#define CSL_DSS_CTRL_DSS_TPCC_C_PARITY_STATUS_RESETVAL                         (0x00000000U)

/* TPTC_DBS_CONFIG */

#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_A0_MASK              (0x00000003U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_A0_SHIFT             (0x00000000U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_A0_RESETVAL          (0x00000002U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_A0_MAX               (0x00000003U)

#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_A1_MASK              (0x0000000CU)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_A1_SHIFT             (0x00000002U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_A1_RESETVAL          (0x00000002U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_A1_MAX               (0x00000003U)

#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_B0_MASK              (0x00000030U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_B0_SHIFT             (0x00000004U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_B0_RESETVAL          (0x00000002U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_B0_MAX               (0x00000003U)

#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_B1_MASK              (0x000000C0U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_B1_SHIFT             (0x00000006U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_B1_RESETVAL          (0x00000002U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_B1_MAX               (0x00000003U)

#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_C0_MASK              (0x00000300U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_C0_SHIFT             (0x00000008U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_C0_RESETVAL          (0x00000002U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_C0_MAX               (0x00000003U)

#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_C1_MASK              (0x00000C00U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_C1_SHIFT             (0x0000000AU)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_C1_RESETVAL          (0x00000002U)
#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_TPTC_DBS_CONFIG_TPTC_C1_MAX               (0x00000003U)

#define CSL_DSS_CTRL_TPTC_DBS_CONFIG_RESETVAL                                  (0x00000AAAU)

/* DSS_DSP_BOOTCFG */

#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_ISTP_RST_VAL_MASK         (0x003FFFFFU)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_ISTP_RST_VAL_SHIFT        (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_ISTP_RST_VAL_RESETVAL     (0x00002000U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_ISTP_RST_VAL_MAX          (0x003FFFFFU)

#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_L1D_CACHE_MODE_MASK       (0x01000000U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_L1D_CACHE_MODE_SHIFT      (0x00000018U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_L1D_CACHE_MODE_RESETVAL   (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_L1D_CACHE_MODE_MAX        (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_L1P_CACHE_MODE_MASK       (0x02000000U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_L1P_CACHE_MODE_SHIFT      (0x00000019U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_L1P_CACHE_MODE_RESETVAL   (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_DSS_DSP_BOOTCFG_L1P_CACHE_MODE_MAX        (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_BOOTCFG_RESETVAL                                  (0x00002000U)

/* DSS_DSP_NMI_GATE */

#define CSL_DSS_CTRL_DSS_DSP_NMI_GATE_DSS_DSP_NMI_GATE_GATE_MASK               (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_NMI_GATE_DSS_DSP_NMI_GATE_GATE_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_NMI_GATE_DSS_DSP_NMI_GATE_GATE_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_NMI_GATE_DSS_DSP_NMI_GATE_GATE_MAX                (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSP_NMI_GATE_RESETVAL                                 (0x00000000U)

/* DSS_PBIST_KEY_RESET */

#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_PBIST_ST_KEY_MASK (0x0000000FU)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_PBIST_ST_KEY_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_PBIST_ST_KEY_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_PBIST_ST_KEY_MAX (0x0000000FU)

#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_PBIST_ST_RESET_MASK (0x000000F0U)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_PBIST_ST_RESET_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_PBIST_ST_RESET_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_PBIST_ST_RESET_MAX (0x0000000FU)

#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_L2_PBIST_ST_KEY_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_L2_PBIST_ST_KEY_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_L2_PBIST_ST_KEY_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_DSS_PBIST_KEY_RESET_DSS_L2_PBIST_ST_KEY_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PBIST_KEY_RESET_RESETVAL                              (0x00000000U)

/* DSS_PBIST_REG0 */

#define CSL_DSS_CTRL_DSS_PBIST_REG0_DSS_PBIST_REG0_DSS_PBIST_REG0_MASK         (0xFFFFFFFFU)
#define CSL_DSS_CTRL_DSS_PBIST_REG0_DSS_PBIST_REG0_DSS_PBIST_REG0_SHIFT        (0x00000000U)
#define CSL_DSS_CTRL_DSS_PBIST_REG0_DSS_PBIST_REG0_DSS_PBIST_REG0_RESETVAL     (0x00000000U)
#define CSL_DSS_CTRL_DSS_PBIST_REG0_DSS_PBIST_REG0_DSS_PBIST_REG0_MAX          (0xFFFFFFFFU)

#define CSL_DSS_CTRL_DSS_PBIST_REG0_RESETVAL                                   (0x00000000U)

/* DSS_PBIST_REG1 */

#define CSL_DSS_CTRL_DSS_PBIST_REG1_DSS_PBIST_REG1_DSS_PBIST_REG1_MASK         (0xFFFFFFFFU)
#define CSL_DSS_CTRL_DSS_PBIST_REG1_DSS_PBIST_REG1_DSS_PBIST_REG1_SHIFT        (0x00000000U)
#define CSL_DSS_CTRL_DSS_PBIST_REG1_DSS_PBIST_REG1_DSS_PBIST_REG1_RESETVAL     (0x00000000U)
#define CSL_DSS_CTRL_DSS_PBIST_REG1_DSS_PBIST_REG1_DSS_PBIST_REG1_MAX          (0xFFFFFFFFU)

#define CSL_DSS_CTRL_DSS_PBIST_REG1_RESETVAL                                   (0x00000000U)

/* DSS_TPTC_BOUNDARY_CFG0 */

#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_A0_SIZE_MASK (0x0000003FU)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_A0_SIZE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_A0_SIZE_RESETVAL (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_A0_SIZE_MAX (0x0000003FU)

#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_A1_SIZE_MASK (0x00003F00U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_A1_SIZE_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_A1_SIZE_RESETVAL (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_A1_SIZE_MAX (0x0000003FU)

#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_B0_SIZE_MASK (0x003F0000U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_B0_SIZE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_B0_SIZE_RESETVAL (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_B0_SIZE_MAX (0x0000003FU)

#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_B1_SIZE_MASK (0x3F000000U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_B1_SIZE_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_B1_SIZE_RESETVAL (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_DSS_TPTC_BOUNDARY_CFG0_TPTC_B1_SIZE_MAX (0x0000003FU)

#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG0_RESETVAL                           (0x12121212U)

/* DSS_TPTC_BOUNDARY_CFG1 */

#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_DSS_TPTC_BOUNDARY_CFG1_TPTC_C0_SIZE_MASK (0x0000003FU)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_DSS_TPTC_BOUNDARY_CFG1_TPTC_C0_SIZE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_DSS_TPTC_BOUNDARY_CFG1_TPTC_C0_SIZE_RESETVAL (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_DSS_TPTC_BOUNDARY_CFG1_TPTC_C0_SIZE_MAX (0x0000003FU)

#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_DSS_TPTC_BOUNDARY_CFG1_TPTC_C1_SIZE_MASK (0x00003F00U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_DSS_TPTC_BOUNDARY_CFG1_TPTC_C1_SIZE_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_DSS_TPTC_BOUNDARY_CFG1_TPTC_C1_SIZE_RESETVAL (0x00000012U)
#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_DSS_TPTC_BOUNDARY_CFG1_TPTC_C1_SIZE_MAX (0x0000003FU)

#define CSL_DSS_CTRL_DSS_TPTC_BOUNDARY_CFG1_RESETVAL                           (0x00001212U)

/* DSS_TPTC_XID_REORDER_CFG0 */

#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_A0_DISABLE_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_A0_DISABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_A0_DISABLE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_A0_DISABLE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_A1_DISABLE_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_A1_DISABLE_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_A1_DISABLE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_A1_DISABLE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_B0_DISABLE_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_B0_DISABLE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_B0_DISABLE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_B0_DISABLE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_B1_DISABLE_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_B1_DISABLE_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_B1_DISABLE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_DSS_TPTC_XID_REORDER_CFG0_TPTC_B1_DISABLE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG0_RESETVAL                        (0x00000000U)

/* DSS_TPTC_XID_REORDER_CFG1 */

#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_DSS_TPTC_XID_REORDER_CFG1_TPTC_C0_DISABLE_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_DSS_TPTC_XID_REORDER_CFG1_TPTC_C0_DISABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_DSS_TPTC_XID_REORDER_CFG1_TPTC_C0_DISABLE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_DSS_TPTC_XID_REORDER_CFG1_TPTC_C0_DISABLE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_DSS_TPTC_XID_REORDER_CFG1_TPTC_C1_DISABLE_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_DSS_TPTC_XID_REORDER_CFG1_TPTC_C1_DISABLE_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_DSS_TPTC_XID_REORDER_CFG1_TPTC_C1_DISABLE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_DSS_TPTC_XID_REORDER_CFG1_TPTC_C1_DISABLE_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_TPTC_XID_REORDER_CFG1_RESETVAL                        (0x00000000U)

/* ESM_GATING0 */

#define CSL_DSS_CTRL_ESM_GATING0_ESM_GATING0_ESM_GATING_MASK                   (0xFFFFFFFFU)
#define CSL_DSS_CTRL_ESM_GATING0_ESM_GATING0_ESM_GATING_SHIFT                  (0x00000000U)
#define CSL_DSS_CTRL_ESM_GATING0_ESM_GATING0_ESM_GATING_RESETVAL               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_ESM_GATING0_ESM_GATING0_ESM_GATING_MAX                    (0xFFFFFFFFU)

#define CSL_DSS_CTRL_ESM_GATING0_RESETVAL                                      (0xFFFFFFFFU)

/* ESM_GATING1 */

#define CSL_DSS_CTRL_ESM_GATING1_ESM_GATING1_ESM_GATING_MASK                   (0xFFFFFFFFU)
#define CSL_DSS_CTRL_ESM_GATING1_ESM_GATING1_ESM_GATING_SHIFT                  (0x00000000U)
#define CSL_DSS_CTRL_ESM_GATING1_ESM_GATING1_ESM_GATING_RESETVAL               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_ESM_GATING1_ESM_GATING1_ESM_GATING_MAX                    (0xFFFFFFFFU)

#define CSL_DSS_CTRL_ESM_GATING1_RESETVAL                                      (0xFFFFFFFFU)

/* ESM_GATING2 */

#define CSL_DSS_CTRL_ESM_GATING2_ESM_GATING2_ESM_GATING_MASK                   (0xFFFFFFFFU)
#define CSL_DSS_CTRL_ESM_GATING2_ESM_GATING2_ESM_GATING_SHIFT                  (0x00000000U)
#define CSL_DSS_CTRL_ESM_GATING2_ESM_GATING2_ESM_GATING_RESETVAL               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_ESM_GATING2_ESM_GATING2_ESM_GATING_MAX                    (0xFFFFFFFFU)

#define CSL_DSS_CTRL_ESM_GATING2_RESETVAL                                      (0xFFFFFFFFU)

/* ESM_GATING3 */

#define CSL_DSS_CTRL_ESM_GATING3_ESM_GATING3_ESM_GATING_MASK                   (0xFFFFFFFFU)
#define CSL_DSS_CTRL_ESM_GATING3_ESM_GATING3_ESM_GATING_SHIFT                  (0x00000000U)
#define CSL_DSS_CTRL_ESM_GATING3_ESM_GATING3_ESM_GATING_RESETVAL               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_ESM_GATING3_ESM_GATING3_ESM_GATING_MAX                    (0xFFFFFFFFU)

#define CSL_DSS_CTRL_ESM_GATING3_RESETVAL                                      (0xFFFFFFFFU)

/* DSS_PERIPH_ERRAGG_MASK0 */

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_RCM_RD_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_RCM_RD_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_RCM_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_RCM_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_RCM_WR_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_RCM_WR_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_RCM_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_RCM_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CTRL_RD_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CTRL_RD_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CTRL_WR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CTRL_WR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CM4_CTRL_RD_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CM4_CTRL_RD_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CM4_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CM4_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CM4_CTRL_WR_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CM4_CTRL_WR_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CM4_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_CM4_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_HWA_CFG_RD_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_HWA_CFG_RD_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_HWA_CFG_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_HWA_CFG_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_HWA_CFG_WR_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_HWA_CFG_WR_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_HWA_CFG_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_DSS_HWA_CFG_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_RCM_RD_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_RCM_RD_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_RCM_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_RCM_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_RCM_WR_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_RCM_WR_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_RCM_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_RCM_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_CTRL_RD_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_CTRL_RD_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_CTRL_WR_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_CTRL_WR_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_DSS_PERIPH_ERRAGG_MASK0_RCSS_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_MASK0_RESETVAL                          (0x00000000U)

/* DSS_PERIPH_ERRAGG_STATUS0 */

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_RCM_RD_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_RCM_RD_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_RCM_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_RCM_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_RCM_WR_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_RCM_WR_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_RCM_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_RCM_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CTRL_RD_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CTRL_RD_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CTRL_WR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CTRL_WR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CM4_CTRL_RD_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CM4_CTRL_RD_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CM4_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CM4_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CM4_CTRL_WR_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CM4_CTRL_WR_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CM4_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_CM4_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_HWA_CFG_RD_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_HWA_CFG_RD_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_HWA_CFG_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_HWA_CFG_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_HWA_CFG_WR_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_HWA_CFG_WR_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_HWA_CFG_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_DSS_HWA_CFG_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_RCM_RD_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_RCM_RD_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_RCM_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_RCM_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_RCM_WR_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_RCM_WR_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_RCM_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_RCM_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_CTRL_RD_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_CTRL_RD_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_CTRL_WR_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_CTRL_WR_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_DSS_PERIPH_ERRAGG_STATUS0_RCSS_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS0_RESETVAL                        (0x00000000U)

/* DSS_PERIPH_ERRAGG_STATUS_RAW0 */

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_RCM_RD_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_RCM_RD_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_RCM_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_RCM_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_RCM_WR_MASK (0x00000002U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_RCM_WR_SHIFT (0x00000001U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_RCM_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_RCM_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CTRL_RD_MASK (0x00000004U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CTRL_RD_SHIFT (0x00000002U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CTRL_WR_MASK (0x00000008U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CTRL_WR_SHIFT (0x00000003U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CM4_CTRL_RD_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CM4_CTRL_RD_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CM4_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CM4_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CM4_CTRL_WR_MASK (0x00000020U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CM4_CTRL_WR_SHIFT (0x00000005U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CM4_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_CM4_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_HWA_CFG_RD_MASK (0x00000040U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_HWA_CFG_RD_SHIFT (0x00000006U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_HWA_CFG_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_HWA_CFG_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_HWA_CFG_WR_MASK (0x00000080U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_HWA_CFG_WR_SHIFT (0x00000007U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_HWA_CFG_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_HWA_CFG_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_RCM_RD_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_RCM_RD_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_RCM_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_RCM_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_RCM_WR_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_RCM_WR_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_RCM_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_RCM_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_CTRL_RD_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_CTRL_RD_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_CTRL_RD_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_CTRL_RD_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_CTRL_WR_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_CTRL_WR_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_CTRL_WR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_DSS_PERIPH_ERRAGG_STATUS_RAW0_RCSS_CTRL_WR_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_PERIPH_ERRAGG_STATUS_RAW0_RESETVAL                    (0x00000000U)

/* DSS_DSP_MBOX_WRITE_DONE */

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_0_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_0_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_1_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_1_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_2_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_2_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_3_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_3_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_4_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_4_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_5_MASK (0x00100000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_5_SHIFT (0x00000014U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_6_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_6_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_7_MASK (0x10000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_7_SHIFT (0x0000001CU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_DSS_DSP_MBOX_WRITE_DONE_PROC_7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_WRITE_DONE_RESETVAL                          (0x00000000U)

/* DSS_DSP_MBOX_READ_REQ */

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_0_MASK   (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_0_SHIFT  (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_0_MAX    (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_1_MASK   (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_1_SHIFT  (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_1_MAX    (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_2_MASK   (0x00000100U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_2_SHIFT  (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_2_MAX    (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_3_MASK   (0x00001000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_3_SHIFT  (0x0000000CU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_3_MAX    (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_4_MASK   (0x00010000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_4_SHIFT  (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_4_MAX    (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_5_MASK   (0x00100000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_5_SHIFT  (0x00000014U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_5_MAX    (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_6_MASK   (0x01000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_6_SHIFT  (0x00000018U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_6_MAX    (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_7_MASK   (0x10000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_7_SHIFT  (0x0000001CU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_DSS_DSP_MBOX_READ_REQ_PROC_7_MAX    (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_REQ_RESETVAL                            (0x00000000U)

/* DSS_DSP_MBOX_READ_DONE */

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_0_MASK (0x00000001U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_0_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_0_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_1_MASK (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_1_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_1_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_2_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_2_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_2_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_3_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_3_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_3_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_4_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_4_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_4_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_5_MASK (0x00100000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_5_SHIFT (0x00000014U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_5_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_6_MASK (0x01000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_6_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_6_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_7_MASK (0x10000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_7_SHIFT (0x0000001CU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_DSS_DSP_MBOX_READ_DONE_PROC_7_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_RESETVAL                           (0x00000000U)

/* DSS_WDT_EVENT_CAPTURE_SEL */

#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_DSS_WDT_EVENT_CAPTURE_SEL_CAP0_MASK (0x0000007FU)
#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_DSS_WDT_EVENT_CAPTURE_SEL_CAP0_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_DSS_WDT_EVENT_CAPTURE_SEL_CAP0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_DSS_WDT_EVENT_CAPTURE_SEL_CAP0_MAX (0x0000007FU)

#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_DSS_WDT_EVENT_CAPTURE_SEL_CAP1_MASK (0x00007F00U)
#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_DSS_WDT_EVENT_CAPTURE_SEL_CAP1_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_DSS_WDT_EVENT_CAPTURE_SEL_CAP1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_DSS_WDT_EVENT_CAPTURE_SEL_CAP1_MAX (0x0000007FU)

#define CSL_DSS_CTRL_DSS_WDT_EVENT_CAPTURE_SEL_RESETVAL                        (0x00000000U)

/* DSS_RTIA_EVENT_CAPTURE_SEL */

#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_DSS_RTIA_EVENT_CAPTURE_SEL_CAP0_MASK (0x0000007FU)
#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_DSS_RTIA_EVENT_CAPTURE_SEL_CAP0_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_DSS_RTIA_EVENT_CAPTURE_SEL_CAP0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_DSS_RTIA_EVENT_CAPTURE_SEL_CAP0_MAX (0x0000007FU)

#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_DSS_RTIA_EVENT_CAPTURE_SEL_CAP1_MASK (0x00007F00U)
#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_DSS_RTIA_EVENT_CAPTURE_SEL_CAP1_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_DSS_RTIA_EVENT_CAPTURE_SEL_CAP1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_DSS_RTIA_EVENT_CAPTURE_SEL_CAP1_MAX (0x0000007FU)

#define CSL_DSS_CTRL_DSS_RTIA_EVENT_CAPTURE_SEL_RESETVAL                       (0x00000000U)

/* DSS_RTIB_EVENT_CAPTURE_SEL */

#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_DSS_RTIB_EVENT_CAPTURE_SEL_CAP0_MASK (0x0000007FU)
#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_DSS_RTIB_EVENT_CAPTURE_SEL_CAP0_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_DSS_RTIB_EVENT_CAPTURE_SEL_CAP0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_DSS_RTIB_EVENT_CAPTURE_SEL_CAP0_MAX (0x0000007FU)

#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_DSS_RTIB_EVENT_CAPTURE_SEL_CAP1_MASK (0x00007F00U)
#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_DSS_RTIB_EVENT_CAPTURE_SEL_CAP1_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_DSS_RTIB_EVENT_CAPTURE_SEL_CAP1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_DSS_RTIB_EVENT_CAPTURE_SEL_CAP1_MAX (0x0000007FU)

#define CSL_DSS_CTRL_DSS_RTIB_EVENT_CAPTURE_SEL_RESETVAL                       (0x00000000U)

/* DBG_ACK_CPU_CTRL */

#define CSL_DSS_CTRL_DBG_ACK_CPU_CTRL_DBG_ACK_CPU_CTRL_SEL_MASK                (0x00000001U)
#define CSL_DSS_CTRL_DBG_ACK_CPU_CTRL_DBG_ACK_CPU_CTRL_SEL_SHIFT               (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CPU_CTRL_DBG_ACK_CPU_CTRL_SEL_RESETVAL            (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CPU_CTRL_DBG_ACK_CPU_CTRL_SEL_MAX                 (0x00000001U)

#define CSL_DSS_CTRL_DBG_ACK_CPU_CTRL_RESETVAL                                 (0x00000000U)

/* DBG_ACK_CTL0 */

#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_DCCA_MASK                   (0x00000007U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_DCCA_SHIFT                  (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_DCCA_RESETVAL               (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_DCCA_MAX                    (0x00000007U)

#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_DCCB_MASK                   (0x00000070U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_DCCB_SHIFT                  (0x00000004U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_DCCB_RESETVAL               (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_DCCB_MAX                    (0x00000007U)

#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_RTIA_MASK                   (0x00000700U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_RTIA_SHIFT                  (0x00000008U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_RTIA_RESETVAL               (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_RTIA_MAX                    (0x00000007U)

#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_RTIB_MASK                   (0x00007000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_RTIB_SHIFT                  (0x0000000CU)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_RTIB_RESETVAL               (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_RTIB_MAX                    (0x00000007U)

#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_SCIA_MASK                   (0x00070000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_SCIA_SHIFT                  (0x00000010U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_SCIA_RESETVAL               (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_SCIA_MAX                    (0x00000007U)

#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_WDT_MASK                    (0x00700000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_WDT_SHIFT                   (0x00000014U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_WDT_RESETVAL                (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL0_DBG_ACK_CTL0_DSS_WDT_MAX                     (0x00000007U)

#define CSL_DSS_CTRL_DBG_ACK_CTL0_RESETVAL                                     (0x00000000U)

/* DBG_ACK_CTL1 */

#define CSL_DSS_CTRL_DBG_ACK_CTL1_DBG_ACK_CTL1_DSS_MCRC_MASK                   (0x07000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL1_DBG_ACK_CTL1_DSS_MCRC_SHIFT                  (0x00000018U)
#define CSL_DSS_CTRL_DBG_ACK_CTL1_DBG_ACK_CTL1_DSS_MCRC_RESETVAL               (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL1_DBG_ACK_CTL1_DSS_MCRC_MAX                    (0x00000007U)

#define CSL_DSS_CTRL_DBG_ACK_CTL1_DBG_ACK_CTL1_DSS_HWA_MASK                    (0x70000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL1_DBG_ACK_CTL1_DSS_HWA_SHIFT                   (0x0000001CU)
#define CSL_DSS_CTRL_DBG_ACK_CTL1_DBG_ACK_CTL1_DSS_HWA_RESETVAL                (0x00000000U)
#define CSL_DSS_CTRL_DBG_ACK_CTL1_DBG_ACK_CTL1_DSS_HWA_MAX                     (0x00000007U)

#define CSL_DSS_CTRL_DBG_ACK_CTL1_RESETVAL                                     (0x00000000U)

/* DSS_DSP_INT_SEL */

#define CSL_DSS_CTRL_DSS_DSP_INT_SEL_DSS_DSP_INT_SEL_RCSS_CSI2_ICSSM_MASK      (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_INT_SEL_DSS_DSP_INT_SEL_RCSS_CSI2_ICSSM_SHIFT     (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_INT_SEL_DSS_DSP_INT_SEL_RCSS_CSI2_ICSSM_RESETVAL  (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_INT_SEL_DSS_DSP_INT_SEL_RCSS_CSI2_ICSSM_MAX       (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSP_INT_SEL_RESETVAL                                  (0x00000000U)

/* DSS_CBUFF_TRIGGER_SEL */

#define CSL_DSS_CTRL_DSS_CBUFF_TRIGGER_SEL_DSS_CBUFF_TRIGGER_SEL_SEL_MASK      (0x0000007FU)
#define CSL_DSS_CTRL_DSS_CBUFF_TRIGGER_SEL_DSS_CBUFF_TRIGGER_SEL_SEL_SHIFT     (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_TRIGGER_SEL_DSS_CBUFF_TRIGGER_SEL_SEL_RESETVAL  (0x00000028U)
#define CSL_DSS_CTRL_DSS_CBUFF_TRIGGER_SEL_DSS_CBUFF_TRIGGER_SEL_SEL_MAX       (0x0000007FU)

#define CSL_DSS_CTRL_DSS_CBUFF_TRIGGER_SEL_RESETVAL                            (0x00000028U)

/* DSS_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_ENABLE_MASK       (0x00000007U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_ENABLE_SHIFT      (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_ENABLE_RESETVAL   (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_ENABLE_MAX        (0x00000007U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_CLK_DISABLE_MASK  (0x00000070U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_CLK_DISABLE_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_CLK_DISABLE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_CLK_DISABLE_MAX   (0x00000007U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_ERR_CLEAR_MASK    (0x00000700U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT   (0x00000008U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_ERR_CLEAR_MAX     (0x00000007U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_TRIG_MASK (0x00007000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_TRIG_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_TRIG_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_TRIG_MAX  (0x00000007U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_DONE_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_DONE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_DONE_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_DONE_MAX  (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_BUS_IDLE_MASK     (0x00020000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_BUS_IDLE_SHIFT    (0x00000011U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_BUS_IDLE_RESETVAL (0x00000001U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_BUS_IDLE_MAX      (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_EN_MASK   (0x00700000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_EN_SHIFT  (0x00000014U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_EN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_EN_MAX    (0x00000007U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_CLR_MASK  (0x01000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_CLR_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_CLR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_DSS_BUS_SAFETY_CTRL_PATTERN_CLR_MAX   (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_CTRL_RESETVAL                              (0x00020000U)

/* DSS_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_MAIN_MASK             (0x00000007U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_MAIN_SHIFT            (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_MAIN_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_MAIN_MAX              (0x00000007U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_INTERNAL_FLOP_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_INTERNAL_FLOP_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_INTERNAL_FLOP_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_INTERNAL_FLOP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_PATTERN_MASK_MASK     (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_PATTERN_MASK_SHIFT    (0x00000010U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_PATTERN_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_DSS_BUS_SAFETY_FI_PATTERN_MASK_MAX      (0x000000FFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_FI_RESETVAL                                (0x00000000U)

/* DSS_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_INTERNAL_FLOP_MASK  (0x000000FFU)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_INTERNAL_FLOP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_INTERNAL_FLOP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_INTERNAL_FLOP_MAX   (0x000000FFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_PATTERN_MASK        (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_PATTERN_SHIFT       (0x00000008U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_PATTERN_RESETVAL    (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_PATTERN_MAX         (0x000000FFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_INTERNAL_FLOP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_INTERNAL_FLOP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_INTERNAL_FLOP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_INTERNAL_FLOP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_PATTERN_MASK_MASK   (0xFF000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_PATTERN_MASK_SHIFT  (0x00000018U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_PATTERN_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_DSS_BUS_SAFETY_ERR_PATTERN_MASK_MAX    (0x000000FFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_ERR_RESETVAL                               (0xFFFF0000U)

/* DSS_DSP_MDMA_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x0000000CU)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_CTRL_RESETVAL                     (0x000C0007U)

/* DSS_DSP_MDMA_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_DSS_DSP_MDMA_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_DSP_MDMA_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_DSS_DSP_MDMA_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_MDMA_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_L3_BANKA_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_CTRL_RESETVAL                     (0x00030007U)

/* DSS_L3_BANKA_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_DSS_L3_BANKA_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_L3_BANKA_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_DSS_L3_BANKA_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKA_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_L3_BANKB_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_CTRL_RESETVAL                     (0x00030007U)

/* DSS_L3_BANKB_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_DSS_L3_BANKB_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_L3_BANKB_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_DSS_L3_BANKB_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKB_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_L3_BANKC_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_CTRL_RESETVAL                     (0x00030007U)

/* DSS_L3_BANKC_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_DSS_L3_BANKC_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_L3_BANKC_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_DSS_L3_BANKC_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKC_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_L3_BANKD_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_CTRL_RESETVAL                     (0x00030007U)

/* DSS_L3_BANKD_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_DSS_L3_BANKD_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_L3_BANKD_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_DSS_L3_BANKD_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_L3_BANKD_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_DSP_SDMA_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_CTRL_RESETVAL                     (0x00030007U)

/* DSS_DSP_SDMA_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_DSS_DSP_SDMA_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_DSP_SDMA_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_DSS_DSP_SDMA_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSP_SDMA_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_TPTC_A0_RD_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_CTRL_RESETVAL                   (0x00080007U)

/* DSS_TPTC_A0_RD_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DSS_TPTC_A0_RD_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_A0_RD_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_RD_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_A1_RD_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_CTRL_RESETVAL                   (0x00080007U)

/* DSS_TPTC_A1_RD_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DSS_TPTC_A1_RD_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_A1_RD_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_RD_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_B0_RD_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_CTRL_RESETVAL                   (0x00080007U)

/* DSS_TPTC_B0_RD_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DSS_TPTC_B0_RD_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_B0_RD_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_RD_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_B1_RD_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_CTRL_RESETVAL                   (0x00080007U)

/* DSS_TPTC_B1_RD_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DSS_TPTC_B1_RD_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_B1_RD_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_RD_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_C0_RD_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_CTRL_RESETVAL                   (0x00080007U)

/* DSS_TPTC_C0_RD_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DSS_TPTC_C0_RD_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_C0_RD_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_RD_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_C1_RD_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_CTRL_RESETVAL                   (0x00080007U)

/* DSS_TPTC_C1_RD_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DSS_TPTC_C1_RD_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_C1_RD_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_RD_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_A0_WR_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_CTRL_RESETVAL                   (0x00040007U)

/* DSS_TPTC_A0_WR_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DSS_TPTC_A0_WR_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_A0_WR_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A0_WR_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_A1_WR_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_CTRL_RESETVAL                   (0x00040007U)

/* DSS_TPTC_A1_WR_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DSS_TPTC_A1_WR_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_A1_WR_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_A1_WR_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_B0_WR_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_CTRL_RESETVAL                   (0x00040007U)

/* DSS_TPTC_B0_WR_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DSS_TPTC_B0_WR_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_B0_WR_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B0_WR_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_B1_WR_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_CTRL_RESETVAL                   (0x00040007U)

/* DSS_TPTC_B1_WR_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DSS_TPTC_B1_WR_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_B1_WR_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_B1_WR_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_C0_WR_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_CTRL_RESETVAL                   (0x00040007U)

/* DSS_TPTC_C0_WR_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DSS_TPTC_C0_WR_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_C0_WR_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C0_WR_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_TPTC_C1_WR_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000004U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_CTRL_RESETVAL                   (0x00040007U)

/* DSS_TPTC_C1_WR_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DSS_TPTC_C1_WR_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_TPTC_C1_WR_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_TPTC_C1_WR_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_MDO_FIFO_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_CTRL_RESETVAL                     (0x00030007U)

/* DSS_MDO_FIFO_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_DSS_MDO_FIFO_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_MDO_FIFO_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_DSS_MDO_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MDO_FIFO_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_CBUFF_FIFO_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_CTRL_RESETVAL                   (0x00030007U)

/* DSS_CBUFF_FIFO_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DSS_CBUFF_FIFO_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_FI_RESETVAL                     (0x00000000U)

/* DSS_CBUFF_FIFO_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CBUFF_FIFO_BUS_SAFETY_ERR_RESETVAL                    (0xFFFF0000U)

/* DSS_MCRC_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_CTRL_RESETVAL                         (0x00030007U)

/* DSS_MCRC_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_MAIN_MASK   (0x00000007U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_MAIN_SHIFT  (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_MAIN_MAX    (0x00000007U)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_DSS_MCRC_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_FI_RESETVAL                           (0x00000000U)

/* DSS_MCRC_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_DSS_MCRC_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MCRC_BUS_SAFETY_ERR_RESETVAL                          (0xFFFF0000U)

/* DSS_PCR_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_DSS_PCR_BUS_SAFETY_CTRL_TYPE_MAX  (0x000000FFU)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_CTRL_RESETVAL                          (0x00030007U)

/* DSS_PCR_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_MAIN_MASK     (0x00000007U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_MAIN_SHIFT    (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_MAIN_MAX      (0x00000007U)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_DSS_PCR_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_FI_RESETVAL                            (0x00000000U)

/* DSS_PCR_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_DSS_PCR_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_PCR_BUS_SAFETY_ERR_RESETVAL                           (0xFFFF0000U)

/* DSS_HWA_DMA0_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_CTRL_RESETVAL                     (0x00030007U)

/* DSS_HWA_DMA0_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_DSS_HWA_DMA0_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_HWA_DMA0_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_DSS_HWA_DMA0_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA0_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_HWA_DMA1_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_CTRL_RESETVAL                     (0x00030007U)

/* DSS_HWA_DMA1_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_DSS_HWA_DMA1_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_FI_RESETVAL                       (0x00000000U)

/* DSS_HWA_DMA1_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_DSS_HWA_DMA1_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_HWA_DMA1_BUS_SAFETY_ERR_RESETVAL                      (0xFFFF0000U)

/* DSS_CM4_M_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x0000000CU)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_CTRL_RESETVAL                        (0x000C0707U)

/* DSS_CM4_M_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_MAIN_MAX  (0x00000007U)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_DSS_CM4_M_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_FI_RESETVAL                          (0x00000000U)

/* DSS_CM4_M_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_DSS_CM4_M_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_M_BUS_SAFETY_ERR_RESETVAL                         (0xFFFF0000U)

/* DSS_CM4_S_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_CTRL_RESETVAL                        (0x00030007U)

/* DSS_CM4_S_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_MAIN_MAX  (0x00000007U)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_DSS_CM4_S_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_FI_RESETVAL                          (0x00000000U)

/* DSS_CM4_S_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_DSS_CM4_S_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_CM4_S_BUS_SAFETY_ERR_RESETVAL                         (0xFFFF0000U)

/* DSS_MBOX_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_CTRL_RESETVAL                         (0x00030007U)

/* DSS_MBOX_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_MAIN_MASK   (0x00000007U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_MAIN_SHIFT  (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_MAIN_MAX    (0x00000007U)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_DSS_MBOX_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_FI_RESETVAL                           (0x00000000U)

/* DSS_MBOX_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_DSS_MBOX_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MBOX_BUS_SAFETY_ERR_RESETVAL                          (0xFFFF0000U)

/* DSS_DSS2RSS_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_CTRL_RESETVAL                      (0x00030007U)

/* DSS_DSS2RSS_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_DSS_DSS2RSS_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_FI_RESETVAL                        (0x00000000U)

/* DSS_DSS2RSS_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_DSS_DSS2RSS_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2RSS_BUS_SAFETY_ERR_RESETVAL                       (0xFFFF0000U)

/* DSS_RSS2DSS_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x0000000CU)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_CTRL_RESETVAL                      (0x000C0007U)

/* DSS_RSS2DSS_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_DSS_RSS2DSS_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_FI_RESETVAL                        (0x00000000U)

/* DSS_RSS2DSS_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_DSS_RSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_RSS2DSS_BUS_SAFETY_ERR_RESETVAL                       (0xFFFF0000U)

/* DSS_DSS2MSS_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x00000003U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_CTRL_RESETVAL                      (0x00030007U)

/* DSS_DSS2MSS_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_DSS_DSS2MSS_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_FI_RESETVAL                        (0x00000000U)

/* DSS_DSS2MSS_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_DSS_DSS2MSS_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_DSS2MSS_BUS_SAFETY_ERR_RESETVAL                       (0xFFFF0000U)

/* DSS_MSS2DSS_BUS_SAFETY_CTRL */

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_ENABLE_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_ENABLE_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_ENABLE_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_ENABLE_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_ERR_CLEAR_MASK (0x00000700U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_ERR_CLEAR_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_ERR_CLEAR_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_ERR_CLEAR_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_TYPE_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_TYPE_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_TYPE_RESETVAL (0x0000000CU)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_TYPE_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_CTRL_RESETVAL                      (0x000C0007U)

/* DSS_MSS2DSS_BUS_SAFETY_FI */

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_MAIN_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_MAIN_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_MAIN_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_MAIN_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_CTRL_MASK_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_CTRL_MASK_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_CTRL_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_CTRL_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_DATA_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_DATA_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_DATA_MASK_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_DSS_MSS2DSS_BUS_SAFETY_FI_DATA_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_FI_RESETVAL                        (0x00000000U)

/* DSS_MSS2DSS_BUS_SAFETY_ERR */

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_DATA_COMP_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_DATA_COMP_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_MASK (0x00FF0000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_CTRL_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK_MASK (0xFF000000U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK_SHIFT (0x00000018U)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK_RESETVAL (0x000000FFU)
#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_DSS_MSS2DSS_BUS_SAFETY_ERR_DATA_COMP_MASK_MAX (0x000000FFU)

#define CSL_DSS_CTRL_DSS_MSS2DSS_BUS_SAFETY_ERR_RESETVAL                       (0xFFFF0000U)

/* DSS_BUS_SAFETY_LOOPBACK_SLV_SEL */

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_SELECT_MASK (0xFFFFFFFFU)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_SELECT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_SELECT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_SELECT_MAX (0xFFFFFFFFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_SLV_SEL_RESETVAL                  (0x00000000U)

/* DSS_BUS_SAFETY_LOOPBACK_MST_SEL */

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_SELECT_MASK (0xFFFFFFFFU)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_SELECT_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_SELECT_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_SELECT_MAX (0xFFFFFFFFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_LOOPBACK_MST_SEL_RESETVAL                  (0x00000000U)

/* DSS_DMA_REQ_SEL */

#define CSL_DSS_CTRL_DSS_DMA_REQ_SEL_DSS_DMA_REQ_SEL_SELECT_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_DSS_DMA_REQ_SEL_DSS_DMA_REQ_SEL_SELECT_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_DSS_DMA_REQ_SEL_DSS_DMA_REQ_SEL_SELECT_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_DSS_DMA_REQ_SEL_DSS_DMA_REQ_SEL_SELECT_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_DSS_DMA_REQ_SEL_RESETVAL                                  (0x00000000U)

/* DSS_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_MMR_RST_MASK    (0x00000007U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_MMR_RST_SHIFT   (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_MMR_RST_MAX     (0x00000007U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_MASK (0x0000FF00U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_MAX  (0x000000FFU)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00010000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000010U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00020000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000011U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00040000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x00000012U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00080000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x00000013U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00100000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x00000014U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00200000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x00000015U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00400000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x00000016U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00800000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x00000017U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_DSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_BUS_SAFETY_DEBUG_RESETVAL                             (0x00000070U)

/* DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_DSS2MSS_BUS_SAFETY_DEBUG_RESETVAL                 (0x00000070U)

/* DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM0_BUS_SAFETY_DEBUG_RESETVAL                  (0x00000070U)

/* DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM1_BUS_SAFETY_DEBUG_RESETVAL                  (0x00000070U)

/* DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM2_BUS_SAFETY_DEBUG_RESETVAL                  (0x00000070U)

/* DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2M_L3RAM3_BUS_SAFETY_DEBUG_RESETVAL                  (0x00000070U)

/* DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM0_BUS_SAFETY_DEBUG_RESETVAL               (0x00000070U)

/* DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM1_BUS_SAFETY_DEBUG_RESETVAL               (0x00000070U)

/* DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM2_BUS_SAFETY_DEBUG_RESETVAL               (0x00000070U)

/* DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG */

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_MASK (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SEL_MASK (0x00000070U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SEL_SHIFT (0x00000004U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SEL_RESETVAL (0x00000007U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_MMR_RST_SEL_MAX (0x00000007U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MASK (0x00000100U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_SHIFT (0x00000008U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC0_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MASK (0x00000200U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_SHIFT (0x00000009U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC1_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MASK (0x00000400U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_SHIFT (0x0000000AU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC2_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MASK (0x00000800U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_SHIFT (0x0000000BU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC3_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MASK (0x00001000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_SHIFT (0x0000000CU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC4_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MASK (0x00002000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_SHIFT (0x0000000DU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC5_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MASK (0x00004000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_SHIFT (0x0000000EU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC6_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MASK (0x00008000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_SHIFT (0x0000000FU)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_FSM_STATUS_REC7_MAX (0x00000001U)

#define CSL_DSS_CTRL_DSS_M2SRAM_L3RAM3_BUS_SAFETY_DEBUG_RESETVAL               (0x00000070U)

/* HW_SPARE_RW0 */

#define CSL_DSS_CTRL_HW_SPARE_RW0_HW_SPARE_RW0_HW_SPARE_RW0_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_SPARE_RW0_HW_SPARE_RW0_HW_SPARE_RW0_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RW0_HW_SPARE_RW0_HW_SPARE_RW0_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RW0_HW_SPARE_RW0_HW_SPARE_RW0_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_SPARE_RW0_RESETVAL                                     (0x00000000U)

/* HW_SPARE_RW1 */

#define CSL_DSS_CTRL_HW_SPARE_RW1_HW_SPARE_RW1_HW_SPARE_RW1_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_SPARE_RW1_HW_SPARE_RW1_HW_SPARE_RW1_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RW1_HW_SPARE_RW1_HW_SPARE_RW1_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RW1_HW_SPARE_RW1_HW_SPARE_RW1_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_SPARE_RW1_RESETVAL                                     (0x00000000U)

/* HW_SPARE_RW2 */

#define CSL_DSS_CTRL_HW_SPARE_RW2_HW_SPARE_RW2_HW_SPARE_RW2_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_SPARE_RW2_HW_SPARE_RW2_HW_SPARE_RW2_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RW2_HW_SPARE_RW2_HW_SPARE_RW2_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RW2_HW_SPARE_RW2_HW_SPARE_RW2_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_SPARE_RW2_RESETVAL                                     (0x00000000U)

/* HW_SPARE_RW3 */

#define CSL_DSS_CTRL_HW_SPARE_RW3_HW_SPARE_RW3_HW_SPARE_RW3_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_SPARE_RW3_HW_SPARE_RW3_HW_SPARE_RW3_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RW3_HW_SPARE_RW3_HW_SPARE_RW3_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RW3_HW_SPARE_RW3_HW_SPARE_RW3_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_SPARE_RW3_RESETVAL                                     (0x00000000U)

/* HW_SPARE_RO0 */

#define CSL_DSS_CTRL_HW_SPARE_RO0_HW_SPARE_RO0_HW_SPARE_RO0_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_SPARE_RO0_HW_SPARE_RO0_HW_SPARE_RO0_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RO0_HW_SPARE_RO0_HW_SPARE_RO0_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RO0_HW_SPARE_RO0_HW_SPARE_RO0_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_SPARE_RO0_RESETVAL                                     (0x00000000U)

/* HW_SPARE_RO1 */

#define CSL_DSS_CTRL_HW_SPARE_RO1_HW_SPARE_RO1_HW_SPARE_RO1_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_SPARE_RO1_HW_SPARE_RO1_HW_SPARE_RO1_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RO1_HW_SPARE_RO1_HW_SPARE_RO1_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RO1_HW_SPARE_RO1_HW_SPARE_RO1_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_SPARE_RO1_RESETVAL                                     (0x00000000U)

/* HW_SPARE_RO2 */

#define CSL_DSS_CTRL_HW_SPARE_RO2_HW_SPARE_RO2_HW_SPARE_RO2_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_SPARE_RO2_HW_SPARE_RO2_HW_SPARE_RO2_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RO2_HW_SPARE_RO2_HW_SPARE_RO2_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RO2_HW_SPARE_RO2_HW_SPARE_RO2_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_SPARE_RO2_RESETVAL                                     (0x00000000U)

/* HW_SPARE_RO3 */

#define CSL_DSS_CTRL_HW_SPARE_RO3_HW_SPARE_RO3_HW_SPARE_RO3_MASK               (0xFFFFFFFFU)
#define CSL_DSS_CTRL_HW_SPARE_RO3_HW_SPARE_RO3_HW_SPARE_RO3_SHIFT              (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RO3_HW_SPARE_RO3_HW_SPARE_RO3_RESETVAL           (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_RO3_HW_SPARE_RO3_HW_SPARE_RO3_MAX                (0xFFFFFFFFU)

#define CSL_DSS_CTRL_HW_SPARE_RO3_RESETVAL                                     (0x00000000U)

/* DSS_DSP_MBOX_READ_DONE_ACK */

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_ACK_DSS_DSP_MBOX_READ_DONE_ACK_PROC_MASK (0xFFFFFFFFU)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_ACK_DSS_DSP_MBOX_READ_DONE_ACK_PROC_SHIFT (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_ACK_DSS_DSP_MBOX_READ_DONE_ACK_PROC_RESETVAL (0x00000000U)
#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_ACK_DSS_DSP_MBOX_READ_DONE_ACK_PROC_MAX (0xFFFFFFFFU)

#define CSL_DSS_CTRL_DSS_DSP_MBOX_READ_DONE_ACK_RESETVAL                       (0x00000000U)

/* HW_SPARE_REC */

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC0_MASK              (0x00000001U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC0_SHIFT             (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC0_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC0_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC1_MASK              (0x00000002U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC1_SHIFT             (0x00000001U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC1_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC1_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC2_MASK              (0x00000004U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC2_SHIFT             (0x00000002U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC2_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC2_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC3_MASK              (0x00000008U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC3_SHIFT             (0x00000003U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC3_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC3_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC4_MASK              (0x00000010U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC4_SHIFT             (0x00000004U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC4_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC4_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC5_MASK              (0x00000020U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC5_SHIFT             (0x00000005U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC5_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC5_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC6_MASK              (0x00000040U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC6_SHIFT             (0x00000006U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC6_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC6_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC7_MASK              (0x00000080U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC7_SHIFT             (0x00000007U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC7_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC7_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC8_MASK              (0x00000100U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC8_SHIFT             (0x00000008U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC8_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC8_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC9_MASK              (0x00000200U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC9_SHIFT             (0x00000009U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC9_RESETVAL          (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC9_MAX               (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC10_MASK             (0x00000400U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC10_SHIFT            (0x0000000AU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC10_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC10_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC11_MASK             (0x00000800U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC11_SHIFT            (0x0000000BU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC11_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC11_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC12_MASK             (0x00001000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC12_SHIFT            (0x0000000CU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC12_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC12_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC13_MASK             (0x00002000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC13_SHIFT            (0x0000000DU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC13_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC13_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC14_MASK             (0x00004000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC14_SHIFT            (0x0000000EU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC14_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC14_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC15_MASK             (0x00008000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC15_SHIFT            (0x0000000FU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC15_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC15_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC16_MASK             (0x00010000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC16_SHIFT            (0x00000010U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC16_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC16_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC17_MASK             (0x00020000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC17_SHIFT            (0x00000011U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC17_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC17_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC18_MASK             (0x00040000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC18_SHIFT            (0x00000012U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC18_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC18_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC19_MASK             (0x00080000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC19_SHIFT            (0x00000013U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC19_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC19_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC20_MASK             (0x00100000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC20_SHIFT            (0x00000014U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC20_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC20_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC21_MASK             (0x00200000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC21_SHIFT            (0x00000015U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC21_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC21_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC22_MASK             (0x00400000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC22_SHIFT            (0x00000016U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC22_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC22_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC23_MASK             (0x00800000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC23_SHIFT            (0x00000017U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC23_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC23_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC24_MASK             (0x01000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC24_SHIFT            (0x00000018U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC24_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC24_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC25_MASK             (0x02000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC25_SHIFT            (0x00000019U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC25_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC25_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC26_MASK             (0x04000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC26_SHIFT            (0x0000001AU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC26_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC26_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC27_MASK             (0x08000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC27_SHIFT            (0x0000001BU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC27_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC27_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC28_MASK             (0x10000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC28_SHIFT            (0x0000001CU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC28_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC28_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC29_MASK             (0x20000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC29_SHIFT            (0x0000001DU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC29_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC29_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC30_MASK             (0x40000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC30_SHIFT            (0x0000001EU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC30_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC30_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC31_MASK             (0x80000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC31_SHIFT            (0x0000001FU)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC31_RESETVAL         (0x00000000U)
#define CSL_DSS_CTRL_HW_SPARE_REC_HW_SPARE_REC_HW_SPARE_REC31_MAX              (0x00000001U)

#define CSL_DSS_CTRL_HW_SPARE_REC_RESETVAL                                     (0x00000000U)

/* LOCK0_KICK0 */

#define CSL_DSS_CTRL_LOCK0_KICK0_LOCK0_KICK0_MASK                              (0xFFFFFFFFU)
#define CSL_DSS_CTRL_LOCK0_KICK0_LOCK0_KICK0_SHIFT                             (0x00000000U)
#define CSL_DSS_CTRL_LOCK0_KICK0_LOCK0_KICK0_RESETVAL                          (0x00000000U)
#define CSL_DSS_CTRL_LOCK0_KICK0_LOCK0_KICK0_MAX                               (0xFFFFFFFFU)

#define CSL_DSS_CTRL_LOCK0_KICK0_RESETVAL                                      (0x00000000U)

/* LOCK0_KICK1 */

#define CSL_DSS_CTRL_LOCK0_KICK1_LOCK0_KICK1_MASK                              (0xFFFFFFFFU)
#define CSL_DSS_CTRL_LOCK0_KICK1_LOCK0_KICK1_SHIFT                             (0x00000000U)
#define CSL_DSS_CTRL_LOCK0_KICK1_LOCK0_KICK1_RESETVAL                          (0x00000000U)
#define CSL_DSS_CTRL_LOCK0_KICK1_LOCK0_KICK1_MAX                               (0xFFFFFFFFU)

#define CSL_DSS_CTRL_LOCK0_KICK1_RESETVAL                                      (0x00000000U)

/* INTR_RAW_STATUS */

#define CSL_DSS_CTRL_INTR_RAW_STATUS_PROT_ERR_MASK                             (0x00000001U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_PROT_ERR_SHIFT                            (0x00000000U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_PROT_ERR_RESETVAL                         (0x00000000U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_PROT_ERR_MAX                              (0x00000001U)

#define CSL_DSS_CTRL_INTR_RAW_STATUS_ADDR_ERR_MASK                             (0x00000002U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_ADDR_ERR_SHIFT                            (0x00000001U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_ADDR_ERR_RESETVAL                         (0x00000000U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_ADDR_ERR_MAX                              (0x00000001U)

#define CSL_DSS_CTRL_INTR_RAW_STATUS_KICK_ERR_MASK                             (0x00000004U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_KICK_ERR_SHIFT                            (0x00000002U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_KICK_ERR_RESETVAL                         (0x00000000U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_KICK_ERR_MAX                              (0x00000001U)

#define CSL_DSS_CTRL_INTR_RAW_STATUS_PROXY_ERR_MASK                            (0x00000008U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_PROXY_ERR_SHIFT                           (0x00000003U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_PROXY_ERR_RESETVAL                        (0x00000000U)
#define CSL_DSS_CTRL_INTR_RAW_STATUS_PROXY_ERR_MAX                             (0x00000001U)

#define CSL_DSS_CTRL_INTR_RAW_STATUS_RESETVAL                                  (0x00000000U)

/* INTR_ENABLED_STATUS_CLEAR */

#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_PROT_ERR_MASK           (0x00000001U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_PROT_ERR_SHIFT          (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_PROT_ERR_RESETVAL       (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_PROT_ERR_MAX            (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_ADDR_ERR_MASK           (0x00000002U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_ADDR_ERR_SHIFT          (0x00000001U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_ADDR_ERR_RESETVAL       (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_ADDR_ERR_MAX            (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_KICK_ERR_MASK           (0x00000004U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_KICK_ERR_SHIFT          (0x00000002U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_KICK_ERR_RESETVAL       (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_KICK_ERR_MAX            (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_PROXY_ERR_MASK          (0x00000008U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_PROXY_ERR_SHIFT         (0x00000003U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_PROXY_ERR_RESETVAL      (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_ENABLED_PROXY_ERR_MAX           (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLED_STATUS_CLEAR_RESETVAL                        (0x00000000U)

/* INTR_ENABLE */

#define CSL_DSS_CTRL_INTR_ENABLE_PROT_ERR_EN_MASK                              (0x00000001U)
#define CSL_DSS_CTRL_INTR_ENABLE_PROT_ERR_EN_SHIFT                             (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_PROT_ERR_EN_RESETVAL                          (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_PROT_ERR_EN_MAX                               (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLE_ADDR_ERR_EN_MASK                              (0x00000002U)
#define CSL_DSS_CTRL_INTR_ENABLE_ADDR_ERR_EN_SHIFT                             (0x00000001U)
#define CSL_DSS_CTRL_INTR_ENABLE_ADDR_ERR_EN_RESETVAL                          (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_ADDR_ERR_EN_MAX                               (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLE_KICK_ERR_EN_MASK                              (0x00000004U)
#define CSL_DSS_CTRL_INTR_ENABLE_KICK_ERR_EN_SHIFT                             (0x00000002U)
#define CSL_DSS_CTRL_INTR_ENABLE_KICK_ERR_EN_RESETVAL                          (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_KICK_ERR_EN_MAX                               (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLE_PROXY_ERR_EN_MASK                             (0x00000008U)
#define CSL_DSS_CTRL_INTR_ENABLE_PROXY_ERR_EN_SHIFT                            (0x00000003U)
#define CSL_DSS_CTRL_INTR_ENABLE_PROXY_ERR_EN_RESETVAL                         (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_PROXY_ERR_EN_MAX                              (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLE_RESETVAL                                      (0x00000000U)

/* INTR_ENABLE_CLEAR */

#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_PROT_ERR_EN_CLR_MASK                    (0x00000001U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_PROT_ERR_EN_CLR_SHIFT                   (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_PROT_ERR_EN_CLR_RESETVAL                (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_PROT_ERR_EN_CLR_MAX                     (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_ADDR_ERR_EN_CLR_MASK                    (0x00000002U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_ADDR_ERR_EN_CLR_SHIFT                   (0x00000001U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_ADDR_ERR_EN_CLR_RESETVAL                (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_ADDR_ERR_EN_CLR_MAX                     (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_KICK_ERR_EN_CLR_MASK                    (0x00000004U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_KICK_ERR_EN_CLR_SHIFT                   (0x00000002U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_KICK_ERR_EN_CLR_RESETVAL                (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_KICK_ERR_EN_CLR_MAX                     (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_PROXY_ERR_EN_CLR_MASK                   (0x00000008U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_PROXY_ERR_EN_CLR_SHIFT                  (0x00000003U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_PROXY_ERR_EN_CLR_RESETVAL               (0x00000000U)
#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_PROXY_ERR_EN_CLR_MAX                    (0x00000001U)

#define CSL_DSS_CTRL_INTR_ENABLE_CLEAR_RESETVAL                                (0x00000000U)

/* EOI */

#define CSL_DSS_CTRL_EOI_EOI_VECTOR_MASK                                       (0x000000FFU)
#define CSL_DSS_CTRL_EOI_EOI_VECTOR_SHIFT                                      (0x00000000U)
#define CSL_DSS_CTRL_EOI_EOI_VECTOR_RESETVAL                                   (0x00000000U)
#define CSL_DSS_CTRL_EOI_EOI_VECTOR_MAX                                        (0x000000FFU)

#define CSL_DSS_CTRL_EOI_RESETVAL                                              (0x00000000U)

/* FAULT_ADDRESS */

#define CSL_DSS_CTRL_FAULT_ADDRESS_FAULT_ADDR_MASK                             (0xFFFFFFFFU)
#define CSL_DSS_CTRL_FAULT_ADDRESS_FAULT_ADDR_SHIFT                            (0x00000000U)
#define CSL_DSS_CTRL_FAULT_ADDRESS_FAULT_ADDR_RESETVAL                         (0x00000000U)
#define CSL_DSS_CTRL_FAULT_ADDRESS_FAULT_ADDR_MAX                              (0xFFFFFFFFU)

#define CSL_DSS_CTRL_FAULT_ADDRESS_RESETVAL                                    (0x00000000U)

/* FAULT_TYPE_STATUS */

#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_FAULT_TYPE_MASK                         (0x0000003FU)
#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_FAULT_TYPE_SHIFT                        (0x00000000U)
#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_FAULT_TYPE_RESETVAL                     (0x00000000U)
#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_FAULT_TYPE_MAX                          (0x0000003FU)

#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_FAULT_NS_MASK                           (0x00000040U)
#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_FAULT_NS_SHIFT                          (0x00000006U)
#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_FAULT_NS_RESETVAL                       (0x00000000U)
#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_FAULT_NS_MAX                            (0x00000001U)

#define CSL_DSS_CTRL_FAULT_TYPE_STATUS_RESETVAL                                (0x00000000U)

/* FAULT_ATTR_STATUS */

#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_PRIVID_MASK                       (0x000000FFU)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_PRIVID_SHIFT                      (0x00000000U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_PRIVID_RESETVAL                   (0x00000000U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_PRIVID_MAX                        (0x000000FFU)

#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_ROUTEID_MASK                      (0x000FFF00U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_ROUTEID_SHIFT                     (0x00000008U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_ROUTEID_RESETVAL                  (0x00000000U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_ROUTEID_MAX                       (0x00000FFFU)

#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_XID_MASK                          (0xFFF00000U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_XID_SHIFT                         (0x00000014U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_XID_RESETVAL                      (0x00000000U)
#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_FAULT_XID_MAX                           (0x00000FFFU)

#define CSL_DSS_CTRL_FAULT_ATTR_STATUS_RESETVAL                                (0x00000000U)

/* FAULT_CLEAR */

#define CSL_DSS_CTRL_FAULT_CLEAR_FAULT_CLR_MASK                                (0x00000001U)
#define CSL_DSS_CTRL_FAULT_CLEAR_FAULT_CLR_SHIFT                               (0x00000000U)
#define CSL_DSS_CTRL_FAULT_CLEAR_FAULT_CLR_RESETVAL                            (0x00000000U)
#define CSL_DSS_CTRL_FAULT_CLEAR_FAULT_CLR_MAX                                 (0x00000001U)

#define CSL_DSS_CTRL_FAULT_CLEAR_RESETVAL                                      (0x00000000U)

#ifdef __cplusplus
}
#endif
#endif
