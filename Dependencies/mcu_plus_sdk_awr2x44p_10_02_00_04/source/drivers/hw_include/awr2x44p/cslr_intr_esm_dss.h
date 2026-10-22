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
*/
#ifndef CSLR_INTR_ESM_DSS_H_
#define CSLR_INTR_ESM_DSS_H_

#ifdef __cplusplus
extern "C"
{
#endif


/* List of intr sources for receiver: esm group 1 */
#define CSL_DSS_ESMG1_DSS_TPCC_A_INTAGG_ERR           0
#define CSL_DSS_ESMG1_DSS_TPCC_B_INTAGG_ERR           1
#define CSL_DSS_ESMG1_DSS_TPCC_C_INTAGG_ERR           2
#define CSL_DSS_ESMG1_DSS_DSP_L1P_PARITY              3
#define CSL_DSS_ESMG1_DSS_DSP_L2_SEC_ERR              4
#define CSL_DSS_ESMG1_DSS_DSP_EDC_SEC_ERR             5
#define CSL_DSS_ESMG1_DSS_DSP_L2_DED_ERR              6
#define CSL_DSS_ESMG1_DSS_DSP_EDC_DED_ERR             7
#define CSL_DSS_ESMG1_DSS_CM4_STC_ERR DSS_            9
#define CSL_DSS_ESMG1_DSS_DSP_STC_ERR                10
#define CSL_DSS_ESMG1_DSS_CBUFF_SBE_ERR              11
#define CSL_DSS_ESMG1_DSS_CBUFF_DBE_ERR              12
#define CSL_DSS_ESMG1_DSS_CBUFF_SAFETY_ERR           13
#define CSL_DSS_ESMG1_DSS_DSP_PBIST_ERR              14
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_SEC_AGG         15
#define CSL_DSS_ESMG1_DSS_L3RAM0_MPU_ADDR_ERR        32
#define CSL_DSS_ESMG1_DSS_L3RAM1_MPU_ADDR_ERR        33
#define CSL_DSS_ESMG1_DSS_L3RAM2_MPU_ADDR_ERR        34
#define CSL_DSS_ESMG1_DSS_L3RAM3_MPU_ADDR_ERR        35
#define CSL_DSS_ESMG1_DSS_HWA_DMA0_MPU_ADDR_ERR      36
#define CSL_DSS_ESMG1_DSS_HWA_DMA1_MPU_ADDR_ERR      37
#define CSL_DSS_ESMG1_DSS_HWA_PROC_MPU_ADDR_ERR      38
#define CSL_DSS_ESMG1_DSS_DSS_MAILBOX_MPU_ADDR_ERR   39
#define CSL_DSS_ESMG1_DSS_L3RAM0_MPU_PROT_ERR        40
#define CSL_DSS_ESMG1_DSS_L3RAM1_MPU_PROT_ERR        41
#define CSL_DSS_ESMG1_DSS_L3RAM2_MPU_PROT_ERR        42
#define CSL_DSS_ESMG1_DSS_L3RAM3_MPU_PROT_ERR        43
#define CSL_DSS_ESMG1_DSS_HWA_DMA0_MPU_PROT_ERR      44
#define CSL_DSS_ESMG1_DSS_HWA_DMA1_MPU_PROT_ERR      45
#define CSL_DSS_ESMG1_DSS_HWA_PROC_MPU_PROT_ERR      46
#define CSL_DSS_ESMG1_DSS_DSS_MAILBOX_MPU_PROT_ERR   47
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_DSP_MDMA        48
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_L3RAM0          49
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_L3RAM1          50
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_L3RAM2          51
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_L3RAM3          52
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_DSP_SDMA        53
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCA0_RD       54
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCA1_RD       55
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCB0_RD       56
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCB1_RD       57
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCC0_RD       58
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCC1_RD       59
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCA0_WR       64
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCA1_WR       65
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCB0_WR       66
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCB1_WR       67
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCC0_WR       68
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_TPTCC1_WR       69
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_MCRC            75
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_PCR             76
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_CBUFF           77
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_HWA_DMA0        78
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_HWA_DMA1        79
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_HWA_CM4M        80
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_HWA_CM4S        81
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_MAILBOX         82
#define CSL_DSS_ESMG1_RSS_TPCC_A_INTAGG_ERR         85
#define CSL_DSS_ESMG1_RSS_CSI2A_CTX_MEM_PARITY_ERR  87
#define CSL_DSS_ESMG1_RSS_CSI2A_FIFO_MEM_PARITY_ERR 88
#define CSL_DSS_ESMG1_DSS_ECC_AGG_UERR               91
#define CSL_DSS_ESMG1_DSS_ECC_AGG_SERR               92
#define CSL_DSS_ESMG1_DSS_HWA_GRP1_ERR               93
#define CSL_DSS_RSS_ECC_AGG_SERR                     94
#define CSL_DSS_RSS_ECC_AGG_UERR                     95
#define CSL_DSS_ESMG1_DSS_ECC_L3_BANK0_DED           97
#define CSL_DSS_ESMG1_DSS_ECC_L3_BANK1_DED           98
#define CSL_DSS_ESMG1_DSS_ECC_L3_BANK2_DED           99
#define CSL_DSS_ESMG1_DSS_ECC_L3_BANK3_DED           100
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_DSS2RSS 101
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_RSS2DSS  102
#define CSL_DSS_ESMG1_DSS_DSS_BUS_SAFETY_DSS2MSS 103
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_MSS2DSS  104
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_PATTERN 105
#define CSL_DSS_ESMG1_DSS_BUS_SAFETY_INT_FLOP 106
#define CSL_DSS_ESMG1_DSS_L3BANKA_ACCESS_ERR        113
#define CSL_DSS_ESMG1_DSS_L3BANKB_ACCESS_ERR        114
#define CSL_DSS_ESMG1_DSS_L3BANKC_ACCESS_ERR        115
#define CSL_DSS_ESMG1_DSS_L3BANKD_ACCESS_ERR        116
#define CSL_BSS_ESMG1_DSS_ESM_LO                    117
#define CSL_BSS_ESMG1_DSS_ESM_HI                    118
#define CSL_RSS_ESMG1_DSS_MPU_DSS2RSS_ADDR_ERR      119
#define CSL_RSS_ESMG1_DSS_MPU_DSS2RSS_PROT_ERR      120
#define CSL_RSS_ESMG1_DSS_MPU_MSS2RSS_ADDR_ERR      121
#define CSL_RSS_ESMG1_DSS_MPU_MSS2RSS_PROT_ERR      122

/* List of intr sources for receiver: esm group 2 */
#define CSL_DSS_ESMG2_DSS_WDT_NMI_REQ                0
#define CSL_DSS_ESMG2_DSS_DCCA_ERR                   1
#define CSL_DSS_ESMG2_DSS_DCCB_ERR                   2
#define CSL_DSS_ESMG2_DSS_HWA_GRP2_ERR               3
#define CSL_DSS_ESMG2_DSS_DSP_L2_PARITY_ERR_VB0_EVEN 4
#define CSL_DSS_ESMG2_DSS_DSP_L2_PARITY_ERR_VB0_ODD  5
#define CSL_DSS_ESMG2_DSS_DSP_L2_PARITY_ERR_VB1_EVEN 6
#define CSL_DSS_ESMG2_DSS_DSP_L2_PARITY_ERR_VB1_ODD  7
#define CSL_DSS_ESMG2_DSS_DSP_L2_PARITY_ERR_VB2_EVEN 8
#define CSL_DSS_ESMG2_DSS_DSP_L2_PARITY_ERR_VB2_ODD  9
#define CSL_DSS_ESMG2_DSS_DSP_L2_PARITY_ERR_VB3_EVEN 10
#define CSL_DSS_ESMG2_DSS_DSP_L2_PARITY_ERR_VB3_ODD  11
#define CSL_DSS_ESMG2_DSS_DSP_L2_DED_ERR             12
#define CSL_DSS_ESMG2_DSS_DSP_EDC_DED_ERR            13
#define CSL_DSS_BSS_ESM_HI                           14

/* List of intr sources for receiver: esm group 3 */

#ifdef __cplusplus
}
#endif
#endif /* CSLR_INTR_ESM_DSS_H_*/
