/*
 *  Copyright (C) 2021 Texas Instruments Incorporated
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

/*
 * Auto generated file
 */
#include "ti_drivers_config.h"
#include <drivers/pinmux.h>



static Pinmux_PerCfg_t gPinMuxMainDomainCfg1[] =
{
    /* Unused PAD registers configuration - Start */
    {
        PIN_PAD_BQ,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_BT,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_BX,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_BY,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_CS,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_DG,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    /* Unused PAD registers configuration - End */
    {PINMUX_END, PINMUX_END}
};

static Pinmux_PerCfg_t gPinMuxMainDomainCfg[] = {
    /* Unused PAD registers configuration - Start */
    {
        PIN_PAD_AD,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_AE,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_AF,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_AG,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_AR,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_BU,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_BV,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_BW,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_CT,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_CU,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_CV,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_CW,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_CX,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_CY,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    {
        PIN_PAD_CZ,
        ( PIN_FORCE_INPUT_DISABLE | PIN_FORCE_OUTPUT_DISABLE )
    },
    /* Unused PAD registers configuration - End */
            /* QSPI0 pin config */
    /* QSPI_0 -> PAD_AL (U11) */
    {
        PIN_PAD_AL,
        ( PIN_MODE(1) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
    /* QSPI0 pin config */
    /* QSPI_1 -> PAD_AM (V11) */
    {
        PIN_PAD_AM,
        ( PIN_MODE(1) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
    /* QSPI0 pin config */
    /* QSPI_2 -> PAD_AN (T11) */
    {
        PIN_PAD_AN,
        ( PIN_MODE(1) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
    /* QSPI0 pin config */
    /* QSPI_3 -> PAD_AO (R12) */
    {
        PIN_PAD_AO,
        ( PIN_MODE(1) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
    /* QSPI0 pin config */
    /* QSPI_CLK -> PAD_AP (R10) */
    {
        PIN_PAD_AP,
        ( PIN_MODE(1) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
    /* QSPI0 pin config */
    /* QSPI_CS -> PAD_AQ (U12) */
    {
        PIN_PAD_AQ,
        ( PIN_MODE(1) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },

            /* UARTB pin config */
    /* UARTB_RX -> PAD_BB (R17) */
    {
        PIN_PAD_BB,
        ( PIN_MODE(6) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
    /* UARTB pin config */
    /* UARTB_TX -> PAD_DE (A14) */
    {
        PIN_PAD_DE,
        ( PIN_MODE(6) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
            /* UARTA pin config */
    /* UARTA_RX -> PAD_DA (B16) */
    {
        PIN_PAD_DA,
        ( PIN_MODE(5) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },
    /* UARTA pin config */
    /* UARTA_TX -> PAD_DB (C16) */
    {
        PIN_PAD_DB,
        ( PIN_MODE(5) | PIN_PULL_DISABLE | PIN_SLEW_RATE_LOW )
    },

            /* MDIO0 pin config */
    /* MDIO_DATA -> PAD_CM (T5) */
    {
        PIN_PAD_CM,
        ( PIN_MODE(1) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* MDIO0 pin config */
    /* MDIO_CLK -> PAD_CN (R4) */
    {
        PIN_PAD_CN,
        ( PIN_MODE(1) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_TCTL -> PAD_CA (R6) */
    {
        PIN_PAD_CA,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_RCTL -> PAD_CB (T7) */
    {
        PIN_PAD_CB,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_TD3 -> PAD_CC (U4) */
    {
        PIN_PAD_CC,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_TD2 -> PAD_CD (U6) */
    {
        PIN_PAD_CD,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_TD1 -> PAD_CE (U5) */
    {
        PIN_PAD_CE,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_TD0 -> PAD_CF (U7) */
    {
        PIN_PAD_CF,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_TCLK -> PAD_CG (V3) */
    {
        PIN_PAD_CG,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_RCLK -> PAD_CH (T9) */
    {
        PIN_PAD_CH,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_RD3 -> PAD_CI (U10) */
    {
        PIN_PAD_CI,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_RD2 -> PAD_CJ (V5) */
    {
        PIN_PAD_CJ,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_RD1 -> PAD_CK (V4) */
    {
        PIN_PAD_CK,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },
    /* RGMII0 pin config */
    /* RGMII_RD0 -> PAD_CL (V6) */
    {
        PIN_PAD_CL,
        ( PIN_MODE(3) | PIN_PULL_DOWN | PIN_SLEW_RATE_HIGH )
    },

    {PINMUX_END, PINMUX_END}
};



/*
 * Pinmux
 */


void Pinmux_init(void)
{
    Pinmux_config(gPinMuxMainDomainCfg, PINMUX_DOMAIN_ID_MAIN);


    /* Check if additional PAD registers needs to be disabled for this device */
    if(SOC_rcmIsPadDisableRequired() == 0x1)
    {
        Pinmux_config(gPinMuxMainDomainCfg1, PINMUX_DOMAIN_ID_MAIN);
    }
}

