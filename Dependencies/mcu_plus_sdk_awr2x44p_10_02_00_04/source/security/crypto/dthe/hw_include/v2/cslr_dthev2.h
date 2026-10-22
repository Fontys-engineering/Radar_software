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
 *  Name        : cslr_hsm_dthev2.h
*/
#ifndef CSLR_HSM_DTHEV2_H_
#define CSLR_HSM_DTHEV2_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <drivers/hw_include/cslr.h>
#include <stdint.h>

/**************************************************************************
* Hardware Region  :
**************************************************************************/


/**************************************************************************
* Register Overlay Structure
**************************************************************************/

typedef struct {
    volatile uint32_t REVISION;
    volatile uint32_t CAPABILITY;
    volatile uint8_t  Resv_512[504];
    volatile uint32_t CLK_GATE_CFG;
    volatile uint8_t  Resv_4096[3580];
    volatile uint32_t S_CRC_CTRL;
    volatile uint8_t  Resv_4128[28];
    volatile uint32_t S_CRC_SEED;
    volatile uint8_t  Resv_4160[28];
    volatile uint32_t S_CRC_DIN;
    volatile uint8_t  Resv_4192[28];
    volatile uint32_t S_CRC_RSLT_PP;
    volatile uint8_t  Resv_8192[3996];
    volatile uint32_t P_CRC_CTRL;
    volatile uint8_t  Resv_8224[28];
    volatile uint32_t P_CRC_SEED;
    volatile uint8_t  Resv_8256[28];
    volatile uint32_t P_CRC_DIN;
    volatile uint8_t  Resv_8288[28];
    volatile uint32_t P_CRC_RSLT_PP;
} CSL_hsm_dthev2Regs;

/*These typedefs are added for API compatibility with DTHE_V1*/
typedef CSL_hsm_dthev2Regs CSL_DTHERegs;
typedef CSL_hsm_dthev2Regs CSL_DTHECfgRegs;

/**************************************************************************
* Register Macros
**************************************************************************/

#define CSL_HSM_DTHEV2_REVISION                                                (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY                                              (0x00000004U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG                                            (0x00000200U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL                                              (0x00001000U)
#define CSL_HSM_DTHEV2_S_CRC_SEED                                              (0x00001020U)
#define CSL_HSM_DTHEV2_S_CRC_DIN                                               (0x00001040U)
#define CSL_HSM_DTHEV2_S_CRC_RSLT_PP                                           (0x00001060U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL                                              (0x00002000U)
#define CSL_HSM_DTHEV2_P_CRC_SEED                                              (0x00002020U)
#define CSL_HSM_DTHEV2_P_CRC_DIN                                               (0x00002040U)
#define CSL_HSM_DTHEV2_P_CRC_RSLT_PP                                           (0x00002060U)

/**************************************************************************
* Field Definition Macros
**************************************************************************/


/* REVISION */

#define CSL_HSM_DTHEV2_REVISION_REVMIN_MASK                                    (0x0000003FU)
#define CSL_HSM_DTHEV2_REVISION_REVMIN_SHIFT                                   (0x00000000U)
#define CSL_HSM_DTHEV2_REVISION_REVMIN_RESETVAL                                (0x00000002U)
#define CSL_HSM_DTHEV2_REVISION_REVMIN_MAX                                     (0x0000003FU)

#define CSL_HSM_DTHEV2_REVISION_CUSTOM_MASK                                    (0x000000C0U)
#define CSL_HSM_DTHEV2_REVISION_CUSTOM_SHIFT                                   (0x00000006U)
#define CSL_HSM_DTHEV2_REVISION_CUSTOM_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_REVISION_CUSTOM_MAX                                     (0x00000003U)

#define CSL_HSM_DTHEV2_REVISION_REVMAJ_MASK                                    (0x00000700U)
#define CSL_HSM_DTHEV2_REVISION_REVMAJ_SHIFT                                   (0x00000008U)
#define CSL_HSM_DTHEV2_REVISION_REVMAJ_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_REVISION_REVMAJ_MAX                                     (0x00000007U)

#define CSL_HSM_DTHEV2_REVISION_REVRTL_MASK                                    (0x0000F800U)
#define CSL_HSM_DTHEV2_REVISION_REVRTL_SHIFT                                   (0x0000000BU)
#define CSL_HSM_DTHEV2_REVISION_REVRTL_RESETVAL                                (0x00000003U)
#define CSL_HSM_DTHEV2_REVISION_REVRTL_MAX                                     (0x0000001FU)

#define CSL_HSM_DTHEV2_REVISION_MODID_MASK                                     (0x0FFF0000U)
#define CSL_HSM_DTHEV2_REVISION_MODID_SHIFT                                    (0x00000010U)
#define CSL_HSM_DTHEV2_REVISION_MODID_RESETVAL                                 (0x00000FF0U)
#define CSL_HSM_DTHEV2_REVISION_MODID_MAX                                      (0x00000FFFU)

#define CSL_HSM_DTHEV2_REVISION_BU_MASK                                        (0x30000000U)
#define CSL_HSM_DTHEV2_REVISION_BU_SHIFT                                       (0x0000001CU)
#define CSL_HSM_DTHEV2_REVISION_BU_RESETVAL                                    (0x00000002U)
#define CSL_HSM_DTHEV2_REVISION_BU_MAX                                         (0x00000003U)

#define CSL_HSM_DTHEV2_REVISION_SCHEME_MASK                                    (0xC0000000U)
#define CSL_HSM_DTHEV2_REVISION_SCHEME_SHIFT                                   (0x0000001EU)
#define CSL_HSM_DTHEV2_REVISION_SCHEME_RESETVAL                                (0x00000001U)
#define CSL_HSM_DTHEV2_REVISION_SCHEME_MAX                                     (0x00000003U)

#define CSL_HSM_DTHEV2_REVISION_RESETVAL                                       (0x6FF01802U)

/* CAPABILITY */

#define CSL_HSM_DTHEV2_CAPABILITY_AES_COUNTERMEASURE_MASK                      (0x80000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_AES_COUNTERMEASURE_SHIFT                     (0x0000001FU)
#define CSL_HSM_DTHEV2_CAPABILITY_AES_COUNTERMEASURE_RESETVAL                  (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_AES_COUNTERMEASURE_MAX                       (0x00000001U)

#define CSL_HSM_DTHEV2_CAPABILITY_AES_ENABLED_MASK                             (0x01000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_AES_ENABLED_SHIFT                            (0x00000018U)
#define CSL_HSM_DTHEV2_CAPABILITY_AES_ENABLED_RESETVAL                         (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_AES_ENABLED_MAX                              (0x00000001U)

#define CSL_HSM_DTHEV2_CAPABILITY_SM4_ENABLED_MASK                             (0x00020000U)
#define CSL_HSM_DTHEV2_CAPABILITY_SM4_ENABLED_SHIFT                            (0x00000011U)
#define CSL_HSM_DTHEV2_CAPABILITY_SM4_ENABLED_RESETVAL                         (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_SM4_ENABLED_MAX                              (0x00000001U)

#define CSL_HSM_DTHEV2_CAPABILITY_SM3_ENABLED_MASK                             (0x00010000U)
#define CSL_HSM_DTHEV2_CAPABILITY_SM3_ENABLED_SHIFT                            (0x00000010U)
#define CSL_HSM_DTHEV2_CAPABILITY_SM3_ENABLED_RESETVAL                         (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_SM3_ENABLED_MAX                              (0x00000001U)

#define CSL_HSM_DTHEV2_CAPABILITY_SHA_ENABLED_MASK                             (0x00000100U)
#define CSL_HSM_DTHEV2_CAPABILITY_SHA_ENABLED_SHIFT                            (0x00000008U)
#define CSL_HSM_DTHEV2_CAPABILITY_SHA_ENABLED_RESETVAL                         (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_SHA_ENABLED_MAX                              (0x00000001U)

#define CSL_HSM_DTHEV2_CAPABILITY_TRNG_ENABLED_MASK                            (0x00000002U)
#define CSL_HSM_DTHEV2_CAPABILITY_TRNG_ENABLED_SHIFT                           (0x00000001U)
#define CSL_HSM_DTHEV2_CAPABILITY_TRNG_ENABLED_RESETVAL                        (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_TRNG_ENABLED_MAX                             (0x00000001U)

#define CSL_HSM_DTHEV2_CAPABILITY_PKA_ENABLED_MASK                             (0x00000001U)
#define CSL_HSM_DTHEV2_CAPABILITY_PKA_ENABLED_SHIFT                            (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_PKA_ENABLED_RESETVAL                         (0x00000000U)
#define CSL_HSM_DTHEV2_CAPABILITY_PKA_ENABLED_MAX                              (0x00000001U)

#define CSL_HSM_DTHEV2_CAPABILITY_RESETVAL                                     (0x00000000U)

/* CLK_GATE_CFG */

#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SHA_CG_CFG_MASK                            (0x00000001U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SHA_CG_CFG_SHIFT                           (0x00000000U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SHA_CG_CFG_RESETVAL                        (0x00000000U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SHA_CG_CFG_MAX                             (0x00000001U)

#define CSL_HSM_DTHEV2_CLK_GATE_CFG_AES_CG_CFG_MASK                            (0x00000002U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_AES_CG_CFG_SHIFT                           (0x00000001U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_AES_CG_CFG_RESETVAL                        (0x00000000U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_AES_CG_CFG_MAX                             (0x00000001U)

#define CSL_HSM_DTHEV2_CLK_GATE_CFG_TRNG_CG_CFG_MASK                           (0x00000008U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_TRNG_CG_CFG_SHIFT                          (0x00000003U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_TRNG_CG_CFG_RESETVAL                       (0x00000000U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_TRNG_CG_CFG_MAX                            (0x00000001U)

#define CSL_HSM_DTHEV2_CLK_GATE_CFG_PKA_CG_CFG_MASK                            (0x00000010U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_PKA_CG_CFG_SHIFT                           (0x00000004U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_PKA_CG_CFG_RESETVAL                        (0x00000000U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_PKA_CG_CFG_MAX                             (0x00000001U)

#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SM3_CG_CFG_MASK                            (0x00000040U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SM3_CG_CFG_SHIFT                           (0x00000006U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SM3_CG_CFG_RESETVAL                        (0x00000000U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SM3_CG_CFG_MAX                             (0x00000001U)

#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SM4_CG_CFG_MASK                            (0x00000080U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SM4_CG_CFG_SHIFT                           (0x00000007U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SM4_CG_CFG_RESETVAL                        (0x00000000U)
#define CSL_HSM_DTHEV2_CLK_GATE_CFG_SM4_CG_CFG_MAX                             (0x00000001U)

#define CSL_HSM_DTHEV2_CLK_GATE_CFG_RESETVAL                                   (0x00000000U)

/* S_CRC_CTRL */

#define CSL_HSM_DTHEV2_S_CRC_CTRL_TYPE_MASK                                    (0x0000000FU)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_TYPE_SHIFT                                   (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_TYPE_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_TYPE_MAX                                     (0x0000000FU)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_ENDIAN_MASK                                  (0x00000030U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_ENDIAN_SHIFT                                 (0x00000004U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_ENDIAN_RESETVAL                              (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_ENDIAN_MAX                                   (0x00000003U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES1_MASK                                    (0x00000040U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES1_SHIFT                                   (0x00000006U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES1_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES1_MAX                                     (0x00000001U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_IBR_MASK                                     (0x00000080U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_IBR_SHIFT                                    (0x00000007U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_IBR_RESETVAL                                 (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_IBR_MAX                                      (0x00000001U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_OBR_MASK                                     (0x00000100U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_OBR_SHIFT                                    (0x00000008U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_OBR_RESETVAL                                 (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_OBR_MAX                                      (0x00000001U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_OINV_MASK                                    (0x00000200U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_OINV_SHIFT                                   (0x00000009U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_OINV_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_OINV_MAX                                     (0x00000001U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES2_MASK                                    (0x00000C00U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES2_SHIFT                                   (0x0000000AU)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES2_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES2_MAX                                     (0x00000003U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_BYTE_MASK                                    (0x00001000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_BYTE_SHIFT                                   (0x0000000CU)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_BYTE_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_BYTE_MAX                                     (0x00000001U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_INIT_MASK                                    (0x00006000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_INIT_SHIFT                                   (0x0000000DU)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_INIT_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_INIT_MAX                                     (0x00000003U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_FLASH_ACC2CRC_MASK                           (0x00008000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_FLASH_ACC2CRC_SHIFT                          (0x0000000FU)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_FLASH_ACC2CRC_RESETVAL                       (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_FLASH_ACC2CRC_MAX                            (0x00000001U)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES3_MASK                                    (0xFFFF0000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES3_SHIFT                                   (0x00000010U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES3_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_CTRL_RES3_MAX                                     (0x0000FFFFU)

#define CSL_HSM_DTHEV2_S_CRC_CTRL_RESETVAL                                     (0x00000000U)

/* S_CRC_SEED */

#define CSL_HSM_DTHEV2_S_CRC_SEED_SEED_MASK                                    (0xFFFFFFFFU)
#define CSL_HSM_DTHEV2_S_CRC_SEED_SEED_SHIFT                                   (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_SEED_SEED_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_SEED_SEED_MAX                                     (0xFFFFFFFFU)

#define CSL_HSM_DTHEV2_S_CRC_SEED_RESETVAL                                     (0x00000000U)

/* S_CRC_DIN */

#define CSL_HSM_DTHEV2_S_CRC_DIN_DATA_IN_MASK                                  (0xFFFFFFFFU)
#define CSL_HSM_DTHEV2_S_CRC_DIN_DATA_IN_SHIFT                                 (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_DIN_DATA_IN_RESETVAL                              (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_DIN_DATA_IN_MAX                                   (0xFFFFFFFFU)

#define CSL_HSM_DTHEV2_S_CRC_DIN_RESETVAL                                      (0x00000000U)

/* S_CRC_RSLT_PP */

#define CSL_HSM_DTHEV2_S_CRC_RSLT_PP_RSLT_PP_MASK                              (0xFFFFFFFFU)
#define CSL_HSM_DTHEV2_S_CRC_RSLT_PP_RSLT_PP_SHIFT                             (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_RSLT_PP_RSLT_PP_RESETVAL                          (0x00000000U)
#define CSL_HSM_DTHEV2_S_CRC_RSLT_PP_RSLT_PP_MAX                               (0xFFFFFFFFU)

#define CSL_HSM_DTHEV2_S_CRC_RSLT_PP_RESETVAL                                  (0x00000000U)

/* P_CRC_CTRL */

#define CSL_HSM_DTHEV2_P_CRC_CTRL_TYPE_MASK                                    (0x0000000FU)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_TYPE_SHIFT                                   (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_TYPE_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_TYPE_MAX                                     (0x0000000FU)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_ENDIAN_MASK                                  (0x00000030U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_ENDIAN_SHIFT                                 (0x00000004U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_ENDIAN_RESETVAL                              (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_ENDIAN_MAX                                   (0x00000003U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES1_MASK                                    (0x00000040U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES1_SHIFT                                   (0x00000006U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES1_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES1_MAX                                     (0x00000001U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_IBR_MASK                                     (0x00000080U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_IBR_SHIFT                                    (0x00000007U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_IBR_RESETVAL                                 (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_IBR_MAX                                      (0x00000001U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_OBR_MASK                                     (0x00000100U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_OBR_SHIFT                                    (0x00000008U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_OBR_RESETVAL                                 (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_OBR_MAX                                      (0x00000001U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_OINV_MASK                                    (0x00000200U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_OINV_SHIFT                                   (0x00000009U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_OINV_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_OINV_MAX                                     (0x00000001U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES2_MASK                                    (0x00000C00U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES2_SHIFT                                   (0x0000000AU)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES2_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES2_MAX                                     (0x00000003U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_BYTE_MASK                                    (0x00001000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_BYTE_SHIFT                                   (0x0000000CU)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_BYTE_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_BYTE_MAX                                     (0x00000001U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_INIT_MASK                                    (0x00006000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_INIT_SHIFT                                   (0x0000000DU)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_INIT_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_INIT_MAX                                     (0x00000003U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_FLASH_ACC2CRC_MASK                           (0x00008000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_FLASH_ACC2CRC_SHIFT                          (0x0000000FU)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_FLASH_ACC2CRC_RESETVAL                       (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_FLASH_ACC2CRC_MAX                            (0x00000001U)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES3_MASK                                    (0xFFFF0000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES3_SHIFT                                   (0x00000010U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES3_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_CTRL_RES3_MAX                                     (0x0000FFFFU)

#define CSL_HSM_DTHEV2_P_CRC_CTRL_RESETVAL                                     (0x00000000U)

/* P_CRC_SEED */

#define CSL_HSM_DTHEV2_P_CRC_SEED_SEED_MASK                                    (0xFFFFFFFFU)
#define CSL_HSM_DTHEV2_P_CRC_SEED_SEED_SHIFT                                   (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_SEED_SEED_RESETVAL                                (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_SEED_SEED_MAX                                     (0xFFFFFFFFU)

#define CSL_HSM_DTHEV2_P_CRC_SEED_RESETVAL                                     (0x00000000U)

/* P_CRC_DIN */

#define CSL_HSM_DTHEV2_P_CRC_DIN_DATA_IN_MASK                                  (0xFFFFFFFFU)
#define CSL_HSM_DTHEV2_P_CRC_DIN_DATA_IN_SHIFT                                 (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_DIN_DATA_IN_RESETVAL                              (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_DIN_DATA_IN_MAX                                   (0xFFFFFFFFU)

#define CSL_HSM_DTHEV2_P_CRC_DIN_RESETVAL                                      (0x00000000U)

/* P_CRC_RSLT_PP */

#define CSL_HSM_DTHEV2_P_CRC_RSLT_PP_RSLT_PP_MASK                              (0xFFFFFFFFU)
#define CSL_HSM_DTHEV2_P_CRC_RSLT_PP_RSLT_PP_SHIFT                             (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_RSLT_PP_RSLT_PP_RESETVAL                          (0x00000000U)
#define CSL_HSM_DTHEV2_P_CRC_RSLT_PP_RSLT_PP_MAX                               (0xFFFFFFFFU)

#define CSL_HSM_DTHEV2_P_CRC_RSLT_PP_RESETVAL                                  (0x00000000U)

#ifdef __cplusplus
}
#endif
#endif
