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
 *  Name        : cslr_isc_ctrl_defines.h
*/
#ifndef CSLR_ISC_CTRL_DEFINES_H_
#define CSLR_ISC_CTRL_DEFINES_H_

#include <drivers/hw_include/cslr.h>
#include <drivers/hw_include/tistdtypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/***********************************************************************
 * ISC CTRL total reconfigurable master's register count
 ***********************************************************************/
#define CSL_ISC_CTRL_REG_CNT 21

/***********************************************************************
 * ISC CTRL registers base addresses and IDs
 ***********************************************************************/

#define CSL_ISC_CTRL_REG_MSS_R5FA_AXI_ID                 (0U)
#define CSL_ISC_CTRL_REG_MSS_R5FA_AXI_ADDR               (0x40000800U)

#define CSL_ISC_CTRL_REG_MSS_R5FB_AXI_ID                 (1U)
#define CSL_ISC_CTRL_REG_MSS_R5FB_AXI_ADDR               (0x40000804U)

#define CSL_ISC_CTRL_REG_MSS_TPTC_A0_ID                  (2U)
#define CSL_ISC_CTRL_REG_MSS_TPTC_A0_ADDR                (0x40000808U)

#define CSL_ISC_CTRL_REG_MSS_TPTC_A1_ID                  (3U)
#define CSL_ISC_CTRL_REG_MSS_TPTC_A1_ADDR                (0x4000080CU)

#define CSL_ISC_CTRL_REG_MSS_TPTC_B0_ID                  (4U)
#define CSL_ISC_CTRL_REG_MSS_TPTC_B0_ADDR                (0x40000810U)

#define CSL_ISC_CTRL_REG_MSS_ETHERNET_DMA_ID             (5U)
#define CSL_ISC_CTRL_REG_MSS_ETHERNET_DMA_ADDR           (0x40000814U)

#define CSL_ISC_CTRL_REG_DSS_MDMA_ID                     (6U)
#define CSL_ISC_CTRL_REG_DSS_MDMA_ADDR                   (0x4000082CU)

#define CSL_ISC_CTRL_REG_DSS_TPTC_A0_ID                  (7U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_A0_ADDR                (0x40000830U)

#define CSL_ISC_CTRL_REG_DSS_TPTC_A1_ID                  (8U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_A1_ADDR                (0x40000834U)

#define CSL_ISC_CTRL_REG_DSS_TPTC_B0_ID                  (9U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_B0_ADDR                (0x40000838U)

#define CSL_ISC_CTRL_REG_DSS_TPTC_B1_ID                  (10U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_B1_ADDR                (0x4000083CU)

#define CSL_ISC_CTRL_REG_DSS_TPTC_C0_ID                  (11U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_C0_ADDR                (0x40000840U)

#define CSL_ISC_CTRL_REG_DSS_TPTC_C1_ID                  (12U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_C1_ADDR                (0x40000844U)

#define CSL_ISC_CTRL_REG_DSS_TPTC_C2_ID                  (13U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_C2_ADDR                (0x40000848U)

#define CSL_ISC_CTRL_REG_DSS_TPTC_C3_ID                  (14U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_C3_ADDR                (0x4000084CU)

#define CSL_ISC_CTRL_REG_DSS_TPTC_C4_ID                  (15U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_C4_ADDR                (0x40000850U)

#define CSL_ISC_CTRL_REG_DSS_TPTC_C5_ID                  (16U)
#define CSL_ISC_CTRL_REG_DSS_TPTC_C5_ADDR                (0x40000854U)

#define CSL_ISC_CTRL_REG_DSS_HWA_PROC_ID                 (17U)
#define CSL_ISC_CTRL_REG_DSS_HWA_PROC_ADDR               (0x40000858U)

#define CSL_ISC_CTRL_REG_RCSS_CSIA_ID                    (18U)
#define CSL_ISC_CTRL_REG_RCSS_CSIA_ADDR                  (0x4000085CU)

#define CSL_ISC_CTRL_REG_RCSS_CR4_ID                     (19U)
#define CSL_ISC_CTRL_REG_RCSS_CR4_ADDR                   (0x40000860U)

#define CSL_ISC_CTRL_REG_RCSS_TPTC_A0_ID                 (20U)
#define CSL_ISC_CTRL_REG_RCSS_TPTC_A0_ADDR               (0x40000864U)


/***********************************************************************
 * PRIV ID Defines
 ***********************************************************************/
#define CSL_ISC_CTRL_REG_PRIVID_MASK                     (0x00000F00U)
#define CSL_ISC_CTRL_REG_PRIVID_SHIFT                    (0x00000008U)

#define PRIV_ID_0         (0U)
#define PRIV_ID_1         (1U)
#define PRIV_ID_2         (2U)
#define PRIV_ID_3         (3U)
#define PRIV_ID_4         (4U)
#define PRIV_ID_5         (5U)
#define PRIV_ID_6         (6U)
#define PRIV_ID_7         (7U)
#define PRIV_ID_8         (8U)
#define PRIV_ID_9         (9U)
#define PRIV_ID_10        (10U)
#define PRIV_ID_11        (11U)
#define PRIV_ID_12        (12U)
#define PRIV_ID_13        (13U)
#define PRIV_ID_14        (14U)
#define PRIV_ID_15        (15U)


#ifdef __cplusplus
}
#endif

#endif  /* CSLR_ISC_CTRL_DEFINES_H_ */
