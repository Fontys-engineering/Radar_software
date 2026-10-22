/********************************************************************
*
* MCU_M4 memory map header file
*
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
*/
#ifndef CSLR_SOC_M4_BASEADDRESS_H
#define CSLR_SOC_M4_BASEADDRESS_H

#include <drivers/hw_include/cslr.h>
#include <drivers/hw_include/tistdtypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define CSL_CM4_HWA_M4_RAM_U_BASE                   (0x00000000U)
#define CSL_CM4_HWA_M4_RAM_U_SIZE                   (0x00010000U)
#define CSL_CM4_MSS_SPIA_RAM_U_BASE                 (0x52000000U)
#define CSL_CM4_MSS_SPIA_RAM_U_SIZE                 (0x0000100U)
#define CSL_CM4_MSS_SPIB_RAM_U_BASE                 (0x52020000U)
#define CSL_CM4_MSS_SPIB_RAM_U_SIZE                 (0x0000100U)
#define CSL_CM4_MSS_MCANA_MSG_RAM_U_BASE            (0x52040000U)
#define CSL_CM4_MSS_MCANA_MSG_RAM_U_SIZE            (0x0011000U)
#define CSL_CM4_MSS_VIM_R5A_U_BASE                  (0x52080000U)
#define CSL_CM4_MSS_VIM_R5B_U_BASE                  (0x520A0000U)
#define CSL_CM4_MSS_IOMUX_U_BASE                    (0x520C0000U)
#define CSL_CM4_MSS_RCM_U_BASE                      (0x52100000U)
#define CSL_CM4_MSS_CTRL_U_BASE                     (0x52120000U)
#define CSL_CM4_MSS_TOPRCM_U_BASE                   (0x52140000U)
#define CSL_CM4_MSS_DEBUGSS_U_BASE                  (0x52A00000U)
#define CSL_CM4_MSS_PCR1_U_BASE                     (0x52F78000U)
#define CSL_CM4_TOP_PBIST_U_BASE                    (0x52F79400U)
#define CSL_CM4_MSS_R5SS_STC_U_BASE                 (0x52F79800U)
#define CSL_CM4_MSS_DCCA_U_BASE                     (0x52F79C00U)
#define CSL_CM4_MSS_DCCB_U_BASE                     (0x52F79D00U)
#define CSL_CM4_MSS_DCCC_U_BASE                     (0x52F79E00U)
#define CSL_CM4_MSS_DCCD_U_BASE                     (0x52F79F00U)
#define CSL_CM4_MSS_RTIA_U_BASE                     (0x52F7A000U)
#define CSL_CM4_MSS_RTIB_U_BASE                     (0x52F7A100U)
#define CSL_CM4_MSS_RTIC_U_BASE                     (0x52F7A200U)
#define CSL_CM4_MSS_WDT_U_BASE                      (0x52F7A300U)
#define CSL_CM4_MSS_ESM_U_BASE                      (0x52F7A400U)
#define CSL_CM4_TOP_EFUSE_FARM_U_BASE               (0x52F7A800U)
#define CSL_CM4_MSS_CCMR_U_BASE                     (0x52F7AC00U)
#define CSL_CM4_MSS_I2C_U_BASE                      (0x52F7B000U)
#define CSL_CM4_MSS_GIO_U_BASE                      (0x52F7B400U)
#define CSL_CM4_MSS_ECC_AGG_R5A_U_BASE              (0x52F7B800U)
#define CSL_CM4_MSS_ECC_AGG_R5B_U_BASE              (0x52F7BC00U)
#define CSL_CM4_MSS_ECC_AGG_MSS_U_BASE              (0x52F7C000U)
#define CSL_CM4_MSS_SPIA_U_BASE                     (0x52F7E800U)
#define CSL_CM4_MSS_SPIB_U_BASE                     (0x52F7EA00U)
#define CSL_CM4_MSS_SCIA_U_BASE                     (0x52F7EC00U)
#define CSL_CM4_MSS_SCIB_U_BASE                     (0x52F7ED00U)
#define CSL_CM4_MSS_MCANA_ECC_U_BASE                (0x52F7F800U)
#define CSL_CM4_MSS_MCANA_CFG_U_BASE                (0x52F7FC00U)
#define CSL_CM4_MSS_MCANB_MSG_RAM_U_BASE            (0x53040000U)
#define CSL_CM4_MSS_MCANB_MSG_RAM_U_SIZE            (0x0011000U)
#define CSL_CM4_TOP_AURORA_TX_U_BASE                (0x53060000U)
#define CSL_CM4_TOP_MDO_INFRA_U_BASE                (0x53080000U)
#define CSL_CM4_MSS_GPADC_PKT_RAM_U_BASE            (0x530C0000U)
#define CSL_CM4_MSS_GPADC_PKT_RAM_U_SIZE            (0x0000800U)
#define CSL_CM4_TOP_CTRL_U_BASE                     (0x530E0000U)
#define CSL_CM4_MSS_TPCC_A_U_BASE                   (0x53100000U)
#define CSL_CM4_MSS_TPCC_B_U_BASE                   (0x53120000U)
#define CSL_CM4_MSS_TPTC_A0_U_BASE                  (0x53140000U)
#define CSL_CM4_MSS_TPTC_A1_U_BASE                  (0x53160000U)
#define CSL_CM4_MSS_TPTC_B0_U_BASE                  (0x53180000U)
#define CSL_CM4_MSS_PCR2_U_BASE                     (0x53F78000U)
#define CSL_CM4_MSS_ETPWMA_U_BASE                   (0x53F78C00U)
#define CSL_CM4_MSS_ETPWMB_U_BASE                   (0x53F78D00U)
#define CSL_CM4_MSS_ETPWMC_U_BASE                   (0x53F78E00U)
#define CSL_CM4_MSS_GPADC_REG_U_BASE                (0x53F79800U)
#define CSL_CM4_MSS_DMM_A_U_BASE                    (0x53F79C00U)
#define CSL_CM4_MSS_DMM_B_U_BASE                    (0x53F79E00U)
#define CSL_CM4_MSS_MCANB_ECC_U_BASE                (0x53F7F800U)
#define CSL_CM4_MSS_MCANB_CFG_U_BASE                (0x53F7FC00U)
#define CSL_CM4_RSS_RCM_U_BASE                      (0x55000000U)
#define CSL_CM4_RSS_CTRL_U_BASE                     (0x55020000U)
#define CSL_CM4_RSS_CSI2A_U_BASE                    (0x55080000U)
#define CSL_CM4_RSS_PROG_FILT_U_BASE                (0x550A0000U)
#define CSL_CM4_MPU_RSS_DSS2RSS_U_BASE              (0x550C0000U)
#define CSL_CM4_MPU_RSS_MSS2RSS_U_BASE              (0x550E0000U)
#define CSL_CM4_RSS_TPCC_A_U_BASE                   (0x55100000U)
#define CSL_CM4_RSS_TPTC_A0_U_BASE                  (0x55160000U)
#define CSL_CM4_RSS_ECC_AGG_U_BASE                  (0x551C0000U)
#define CSL_CM4_RSS_FIR_COEFF_U_BASE                (0x551E0000U)
#define CSL_CM4_RSS_FIR_DMEM_U_BASE                 (0x551E0400U)
#define CSL_CM4_RSS_PROC_CTRL_U_BASE                (0x55200000U)
#define CSL_CM4_RSS_PROC_ECC_AGG_U_BASE             (0x55220000U)
#define CSL_CM4_RSS_PCR_U_BASE                      (0x55F78000U)
#define CSL_CM4_DSS_RCM_U_BASE                      (0x56000000U)
#define CSL_CM4_DSS_CTRL_U_BASE                     (0x56020000U)
#define CSL_CM4_DSS_CBUFF_U_BASE                    (0x56040000U)
#define CSL_CM4_DSS_HWA_PARAM_U_BASE                (0x56060000U)
#define CSL_CM4_DSS_HWA_CFG_U_BASE                  (0x56062000U)
#define CSL_CM4_DSS_HWA_WINDOW_RAM_U_BASE           (0x56064000U)
#define CSL_CM4_DSS_HWA_WINDOW_RAM_U_SIZE           (0x0002000U)
#define CSL_CM4_DSS_HWA_MULT_RAM_U_BASE             (0x56068000U)
#define CSL_CM4_DSS_HWA_MULT_RAM_U_SIZE             (0x0002000U)
#define CSL_CM4_DSS_HWA_DEROT_RAM_U_BASE            (0x5606C000U)
#define CSL_CM4_DSS_HWA_DEROT_RAM_U_SIZE            (0x0000100U)
#define CSL_CM4_DSS_HWA_SHUFFLE_RAM_U_BASE          (0x5606E000U)
#define CSL_CM4_DSS_HWA_SHUFFLE_RAM_U_SIZE          (0x0000200U)
#define CSL_CM4_DSS_HWA_2DSTAT_ITER_VAL_RAM_U_BASE  (0x56080000U)
#define CSL_CM4_DSS_HWA_2DSTAT_ITER_VAL_RAM_U_SIZE  (0x0001000U)
#define CSL_CM4_DSS_HWA_2DSTAT_ITER_IDX_RAM_U_BASE  (0x56082000U)
#define CSL_CM4_DSS_HWA_2DSTAT_ITER_IDX_RAM_U_SIZE  (0x0000800U)
#define CSL_CM4_DSS_HWA_2DSTAT_SMPL_VAL_RAM_U_BASE  (0x56084000U)
#define CSL_CM4_DSS_HWA_2DSTAT_SMPL_VAL_RAM_U_SIZE  (0x0000400U)
#define CSL_CM4_DSS_HWA_2DSTAT_SMPL_IDX_RAM_U_BASE  (0x56086000U)
#define CSL_CM4_DSS_HWA_2DSTAT_SMPL_IDX_RAM_U_SIZE  (0x0000200U)
#define CSL_CM4_DSS_HWA_HIST_RAM_U_BASE             (0x56088000U)
#define CSL_CM4_DSS_HWA_HIST_RAM_U_SIZE             (0x0002000U)
#define CSL_CM4_DSS_HWA_HIST_THRESH_RAM_U_BASE      (0x5608C000U)
#define CSL_CM4_DSS_HWA_HIST_THRESH_RAM_U_SIZE      (0x0000100U)
#define CSL_CM4_DSS_ECC_AGG_U_BASE                  (0x560A0000U)
#define CSL_CM4_DSS_TPCC_A_U_BASE                   (0x56100000U)
#define CSL_CM4_DSS_TPCC_B_U_BASE                   (0x56120000U)
#define CSL_CM4_DSS_TPCC_C_U_BASE                   (0x56140000U)
#define CSL_CM4_DSS_TPTC_A0_U_BASE                  (0x56160000U)
#define CSL_CM4_DSS_TPTC_A1_U_BASE                  (0x56180000U)
#define CSL_CM4_DSS_TPTC_B0_U_BASE                  (0x561A0000U)
#define CSL_CM4_DSS_TPTC_B1_U_BASE                  (0x561C0000U)
#define CSL_CM4_DSS_TPTC_C0_U_BASE                  (0x561E0000U)
#define CSL_CM4_DSS_TPTC_C1_U_BASE                  (0x56200000U)
#define CSL_CM4_DSS_TPTC_C2_U_BASE                  (0x56220000U)
#define CSL_CM4_DSS_TPTC_C3_U_BASE                  (0x56240000U)
#define CSL_CM4_DSS_TPTC_C4_U_BASE                  (0x56260000U)
#define CSL_CM4_DSS_TPTC_C5_U_BASE                  (0x56280000U)
#define CSL_CM4_DSS_PCR_U_BASE                      (0x56F78000U)
#define CSL_CM4_DSS_DSP_PBIST_U_BASE                (0x56F79000U)
#define CSL_CM4_DSS_DSP_STC_U_BASE                  (0x56F79200U)
#define CSL_CM4_DSS_CM4_STC_U_BASE                  (0x56F79400U)
#define CSL_CM4_DSS_DCCA_U_BASE                     (0x56F79C00U)
#define CSL_CM4_DSS_DCCB_U_BASE                     (0x56F79D00U)
#define CSL_CM4_DSS_RTIA_U_BASE                     (0x56F7A000U)
#define CSL_CM4_DSS_RTIB_U_BASE                     (0x56F7A100U)
#define CSL_CM4_DSS_WDT_U_BASE                      (0x56F7A200U)
#define CSL_CM4_DSS_SCIA_U_BASE                     (0x56F7B000U)
#define CSL_CM4_DSS_CMC_CFG_U_BASE                  (0x56F7C000U)
#define CSL_CM4_DSS_ESM_U_BASE                      (0x56F7D000U)

#ifdef __cplusplus
}
#endif
#endif /* CSLR_SOC_M4_BASEADDRESS_H_ */
