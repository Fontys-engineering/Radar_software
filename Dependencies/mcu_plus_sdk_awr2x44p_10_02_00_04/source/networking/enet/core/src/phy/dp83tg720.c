/*
 *  Copyright (c) Texas Instruments Incorporated 2023
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

/*!
 * \file  dp83tg720.c
 *
 * \brief This file contains the implementation of the DP83TG720 PHY.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <include/core/enet_utils.h>
#include <priv/core/enet_trace_priv.h>
#include <include/phy/enetphy.h>
#include <include/phy/dp83tg720.h>
#include "enetphy_priv.h"
#include "generic_phy.h"
#include "dp83tg720_priv.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DP83TG720_OUI                         (0x080028U)
#define DP83TG720_MODEL                       (0x28U)
#define DP83TG720_REV_CS1                     (3U)
#define DP83TG720_REV_CS1_1                   (4U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

enum dp83tg720_chip_type {
    DP83TG720_CS1 = 3,
    DP83TG720_CS1_1 = 4,
};

struct dp83tg720_privParams {
    int chip;
    bool is_controller;
};

struct dp83tg720_init_reg {
    uint8_t mmd;
    uint32_t reg;
    uint16_t val;
};

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static bool Dp83tg720_isPhyDevSupported(EnetPhy_Handle hPhy,
                                      const EnetPhy_Version *version);

static bool Dp83tg720_isMacModeSupported(EnetPhy_Handle hPhy,
                                       EnetPhy_Mii mii);

static int32_t Dp83tg720_config(EnetPhy_Handle hPhy,
                              const EnetPhy_Cfg *cfg,
                              EnetPhy_Mii mii);

static void Dp83tg720_setLoopbackCfg(EnetPhy_Handle hPhy,
                                   bool enable);

static void Dp83tg720_reset(EnetPhy_Handle hPhy);

static void Dp83tg720_resetHw(EnetPhy_Handle hPhy);

static bool Dp83tg720_isResetComplete(EnetPhy_Handle hPhy);

static int32_t Dp83tg720_readMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t *val);

static void Dp83tg720_writeMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t val);

static void Dp83tg720_setBitsMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t val);

static void Dp83tg720_readStraps(EnetPhy_Handle hPhy, Dp83tg720_operationMode msMode);

static void Dp83tg720_writeSeq(EnetPhy_Handle hPhy, const struct dp83tg720_init_reg *init_data, int size);

static void Dp83tg720_chipInit(EnetPhy_Handle hPhy);

static void Dp83tg720_setMiiMode(EnetPhy_Handle hPhy, EnetPhy_Mii mii);

static void Dp83tg720_configAutoNeg(EnetPhy_Handle hPhy, bool sgmiiAutoNegEn);

static void Dp83tg720_configClkShift(EnetPhy_Handle hPhy, bool txClkShiftEn, bool rxClkShiftEn);

static int32_t Dp83tg720_setClkDelay(EnetPhy_Handle hPhy, uint32_t txDelay, uint32_t rxDelay);

static void Dp83tg720_configIntr(EnetPhy_Handle hPhy, bool intrEn);

static void Dp83tg720_printRegs(EnetPhy_Handle hPhy);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

EnetPhy_Drv gEnetPhyDrvDp83tg720 =
{
    .name               = "dp83tg720",
    .isPhyDevSupported  = Dp83tg720_isPhyDevSupported,
    .isMacModeSupported = Dp83tg720_isMacModeSupported,
    .config             = Dp83tg720_config,
    .reset              = Dp83tg720_reset,
    .isResetComplete    = Dp83tg720_isResetComplete,
    .readExtReg         = GenericPhy_readExtReg,
    .writeExtReg        = GenericPhy_writeExtReg,
    .printRegs          = Dp83tg720_printRegs,
};

/* PHY Device Attributes */
static struct dp83tg720_privParams dp83tg720_params = {
    .chip = -1,
    .is_controller = false,
};

/* PHY Device Rx Delay config values supported */
static const uint32_t dp83tg720_rxSkew[] = {650U, 900U, 1150U, 1400U, 1650U,
                                        1990U, 2150U, 2400U, 2650U,2900U};

/* PHY Device Rx Delay config values supported */
static const uint32_t dp83tg720_txSkew[] = {250U, 490U, 730U, 970U, 1210U,
                                        1450U, 1690U, 1930U, 2170U, 2420U,
                                        2650U, 2900U};

/*! \brief Chip specific init scripts */
static const struct dp83tg720_init_reg dp83tg720_cs1_controller_init[] = {
    {0x1F,0x0001,0x0940},
    {0x1F,0x0001,0x0140},
    {0x1F,0x0573,0x0101},
    {0x01,0x0834,0xC001},
    {0x1F,0x08A1,0x0DFF},
    {0x1F,0x0802,0x0422},
    {0x1F,0x0865,0x0002},
    {0x1F,0x08A8,0xE080},
    {0x1F,0x08A9,0x03F0},
    {0x1F,0x088D,0x2800},
    {0x1F,0x0899,0x2800},
    {0x1F,0x050B,0x7E7C},
    {0x1F,0x056A,0x5F41},
    {0x1F,0x056B,0xFFB4},
    {0x1F,0x056C,0xFFB4},
    {0x1F,0x0412,0x0044},
    {0x1F,0x0403,0x1E00},
    {0x1F,0x041B,0x02F7},
    {0x1F,0x0424,0x0040},
    {0x1F,0x08A3,0x24E9},
    {0x1F,0x0824,0x15E0},
    {0x1F,0x0800,0x2090},
    {0x1F,0x0894,0x5057},
    {0x1F,0x085D,0x6405},
    {0x1F,0x0892,0x01B0},
    {0x1F,0x0840,0x4120},
    {0x1F,0x0841,0x6151},
    {0x1F,0x0843,0x327A},
    {0x1F,0x0842,0x6652},
    {0x1F,0x0852,0x327A},
    {0x1F,0x0851,0x6652},
    {0x1F,0x0844,0x3F10},
    {0x1F,0x0879,0xE480},
    {0x1F,0x089F,0x0001},
    {0x1F,0x0853,0x0632},
    {0x1F,0x080B,0x0016},
    {0x1F,0x0864,0x1FD0},
    {0x1F,0x086A,0x0006},
    {0x1F,0x0878,0x6032},
    {0x1F,0x08AE,0x0128},
    {0x1F,0x08A0,0x09E7},
    {0x1F,0x088A,0x001F},
    {0x1F,0x018C,0x0001},
};

static const struct dp83tg720_init_reg dp83tg720_cs1_peripheral_init[] = {
    {0x1F,0x0001,0x0940},
    {0x1F,0x0001,0x0140},
    {0x1F,0x0573,0x0101},
    {0x01,0x0834,0x8001},
    {0x1F,0x08A1,0x0DFF},
    {0x1F,0x0802,0x0422},
    {0x1F,0x0865,0x0002},
    {0x1F,0x08A8,0xE080},
    {0x1F,0x08A9,0x03F0},
    {0x1F,0x088D,0x2800},
    {0x1F,0x0899,0x2800},
    {0x1F,0x050B,0x7E7C},
    {0x1F,0x056A,0x5F41},
    {0x1F,0x056B,0xFFB4},
    {0x1F,0x056C,0xFFB4},
    {0x1F,0x0412,0x0044},
    {0x1F,0x0403,0x1E00},
    {0x1F,0x041B,0x02F7},
    {0x1F,0x0424,0x0040},
    {0x1F,0x08A3,0x24E9},
    {0x1F,0x0824,0x15E0},
    {0x1F,0x0800,0x2090},
    {0x1F,0x0894,0x5057},
    {0x1F,0x085D,0x6405},
    {0x1F,0x0892,0x01B0},
    {0x1F,0x0840,0x4120},
    {0x1F,0x0841,0x6151},
    {0x1F,0x0843,0x327A},
    {0x1F,0x0842,0x6652},
    {0x1F,0x0852,0x327A},
    {0x1F,0x0851,0x6652},
    {0x1F,0x0844,0x3F10},
    {0x1F,0x0879,0xE480},
    {0x1F,0x089F,0x0001},
    {0x1F,0x0853,0x0632},
    {0x1F,0x080B,0x0016},
    {0x1F,0x0864,0x1F90},
    {0x1F,0x086A,0x0006},
    {0x1F,0x0878,0x6032},
    {0x1F,0x08AE,0x0128},
    {0x1F,0x08A0,0x09E7},
    {0x1F,0x088A,0x001F},
    {0x1F,0x018C,0x0001},
};

static const struct dp83tg720_init_reg dp83tg720_cs1_1_controller_init[] = {
    {0x1F,0x0573,0x0101},
    {0x01,0x0834,0xC001},
    {0x1F,0x0405,0x5800},
    {0x1F,0x08AD,0x3C51},
    {0x1F,0x0894,0x5DF7},
    {0x1F,0x08A0,0x09E7},
    {0x1F,0x08C0,0x4000},
    {0x1F,0x0814,0x4800},
    {0x1F,0x080D,0x2EBF},
    {0x1F,0x08C1,0x0B00},
    {0x1F,0x087D,0x0001},
    {0x1F,0x082E,0x0000},
    {0x1F,0x0837,0x00F4},
    {0x1F,0x08BE,0x0200},
    {0x1F,0x08C5,0x4000},
    {0x1F,0x08C7,0x2000},
    {0x1F,0x08B3,0x005A},
    {0x1F,0x08B4,0x005A},
    {0x1F,0x08B0,0x0202},
    {0x1F,0x08B5,0x00EA},
    {0x1F,0x08BA,0x2828},
    {0x1F,0x08BB,0x6828},
    {0x1F,0x08BC,0x0028},
    {0x1F,0x08BF,0x0000},
    {0x1F,0x08B1,0x0014},
    {0x1F,0x08B2,0x0008},
    {0x1F,0x08EC,0x0000},
    {0x1F,0x08C8,0x0003},
    {0x1F,0x08BE,0x0201},
    {0x1F,0x018C,0x0001},
};

static const struct dp83tg720_init_reg dp83tg720_cs1_1_peripheral_init[] = {
    {0x1F,0x0573,0x0101},
    {0x01,0x0834,0x8001},
    {0x1F,0x0894,0x5DF7},
    {0x1F,0x056A,0x5F40},
    {0x1F,0x0405,0x5800},
    {0x1F,0x08AD,0x3C51},
    {0x1F,0x0894,0x5DF7},
    {0x1F,0x08A0,0x09E7},
    {0x1F,0x08C0,0x4000},
    {0x1F,0x0814,0x4800},
    {0x1F,0x080D,0x2EBF},
    {0x1F,0x08C1,0x0B00},
    {0x1F,0x087D,0x0001},
    {0x1F,0x082E,0x0000},
    {0x1F,0x0837,0x00F4},
    {0x1F,0x08BE,0x0200},
    {0x1F,0x08C5,0x4000},
    {0x1F,0x08C7,0x2000},
    {0x1F,0x08B3,0x005A},
    {0x1F,0x08B4,0x005A},
    {0x1F,0x08B0,0x0202},
    {0x1F,0x08B5,0x00EA},
    {0x1F,0x08BA,0x2828},
    {0x1F,0x08BB,0x6828},
    {0x1F,0x08BC,0x0028},
    {0x1F,0x08BF,0x0000},
    {0x1F,0x08B1,0x0014},
    {0x1F,0x08B2,0x0008},
    {0x1F,0x08EC,0x0000},
    {0x1F,0x08C8,0x0003},
    {0x1F,0x08BE,0x0201},
    {0x1F,0x056A,0x5F40},
    {0x1F,0x018C,0x0001},
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Dp83tg720_initCfg(Dp83tg720_Cfg *cfg)
{
    cfg->txClkShiftEn = true;
    cfg->rxClkShiftEn = true;
    cfg->txDelayInPs  = 2000U;  /* 2.00 ns */
    cfg->rxDelayInPs  = 2000U;  /* 0.00 ns */
    cfg->interruptEn = false;
    cfg->sgmiiAutoNegEn = true;
    cfg->OpeMode = DP83TG720_CONTROLLER_MODE;
}

static uint8_t Dp83tg720_getDelayIndex(uint32_t delay,
                                       const uint32_t *ptrSkew,
                                       uint8_t size)
{
    uint8_t index = 0;

    if (delay == ptrSkew[0])
    {
        index = 0;
    }
    else
    {
        for(uint8_t loopIdx = 1U; loopIdx < size; loopIdx++) {
            if((delay > ptrSkew[loopIdx - 1]) && (delay < ptrSkew[loopIdx]))
            {
                if((delay - ptrSkew[loopIdx - 1]) < (ptrSkew[loopIdx] - delay))
                    index = loopIdx - 1;
                else
                    index = loopIdx;
            }
        }
    }

    return index;
}

static bool Dp83tg720_isPhyDevSupported(EnetPhy_Handle hPhy,
                                      const EnetPhy_Version *version)
{
    bool supported = false;

    if ((version->oui == DP83TG720_OUI) &&
        (version->model == DP83TG720_MODEL))
    {
        supported = true;
    }

    /* Determine phy version */
    if (version->revision == DP83TG720_REV_CS1)
        dp83tg720_params.chip = DP83TG720_CS1;
    else if (version->revision == DP83TG720_REV_CS1_1)
        dp83tg720_params.chip = DP83TG720_CS1_1;

    return supported;
}

static bool Dp83tg720_isMacModeSupported(EnetPhy_Handle hPhy,
                                         EnetPhy_Mii mii)
{
    bool supported = false;

    switch (mii)
    {
        case ENETPHY_MAC_MII_MII:
            break;
        case ENETPHY_MAC_MII_GMII:
            break;
        case ENETPHY_MAC_MII_RGMII:
            supported = true;
            break;
        case ENETPHY_MAC_MII_SGMII:
            supported = true;
            break;
        default:
            supported = false;
            break;
    }

    return supported;
}

static int32_t Dp83tg720_config(EnetPhy_Handle hPhy,
                                const EnetPhy_Cfg *cfg,
                                EnetPhy_Mii mii)
{
    const Dp83tg720_Cfg *extendedCfg = (const Dp83tg720_Cfg *)cfg->extendedCfg;
    uint32_t extendedCfgSize = cfg->extendedCfgSize;
    int32_t status = ENETPHY_SOK;

    if ((extendedCfg == NULL) ||
        (extendedCfgSize != sizeof(*extendedCfg)))
    {
        ENETTRACE_ERR("PHY %u: invalid config params (cfg=%p, size=%u)\n",
                         hPhy->addr, extendedCfg, extendedCfgSize);
        status = ENETPHY_EINVALIDPARAMS;
    }

    /* Read strap register */
    if (status == ENETPHY_SOK)
    {
        Dp83tg720_readStraps(hPhy, extendedCfg->OpeMode);
    }

    /* Set controller/peripheral mode - through PHY config */
    if(extendedCfg->OpeMode == DP83TG720_CONTROLLER_MODE)
    {
        dp83tg720_params.is_controller = true;
        ENETTRACE_DBG("PHY %u: controller Mode enabled\n", hPhy->addr);
    }
    else if(extendedCfg->OpeMode == DP83TG720_PERIPHERAL_MODE)
    {
        dp83tg720_params.is_controller = false;
        ENETTRACE_DBG("PHY %u: peripheral Mode enabled\n", hPhy->addr);
    }

    /* Init specific chip */
    if (status == ENETPHY_SOK)
    {
        Dp83tg720_chipInit(hPhy);
    }

    /* Configure MII interface */
    if (status == ENETPHY_SOK)
    {
        Dp83tg720_setMiiMode(hPhy, mii);
    }

    /* Configure SGMII auto negotiation */
    if (status == ENETPHY_SOK &&
        mii == ENETPHY_MAC_MII_SGMII)
    {
        Dp83tg720_configAutoNeg(hPhy, extendedCfg->sgmiiAutoNegEn);
    }

    /* Configure RGMII clock shift */
    if (status == ENETPHY_SOK &&
        mii == ENETPHY_MAC_MII_RGMII)
    {
        Dp83tg720_configClkShift(hPhy,
                                 extendedCfg->txClkShiftEn,
                                 extendedCfg->rxClkShiftEn);
    }

    /* Configure Delay */
    if (status == ENETPHY_SOK)
    {
        status = Dp83tg720_setClkDelay(hPhy, extendedCfg->txDelayInPs,
                                       extendedCfg->rxDelayInPs);
    }

    /* Configure interrupts */
    if (status == ENETPHY_SOK)
    {
        Dp83tg720_configIntr(hPhy, extendedCfg->interruptEn);
    }

    /* Set loopback configuration: enable or disable */
    if (status == ENETPHY_SOK)
    {
        Dp83tg720_setLoopbackCfg(hPhy, cfg->loopbackEn);
    }

    return status;
}

static void Dp83tg720_setLoopbackCfg(EnetPhy_Handle hPhy,
                                   bool enable)
{
    bool complete;
    int32_t status;
    uint16_t val;

    ENETTRACE_DBG("PHY %u: %s loopback\n", hPhy->addr, enable ? "enable" : "disable");

    status = EnetPhy_readReg(hPhy, PHY_BMCR, &val);
    if(status != ENETPHY_SOK && enable)
    {
        ENETTRACE_ERR_IF(status != ENETPHY_SOK,
                         "PHY %u: failed to set loopback mode: could not read reg %u\n", hPhy->addr, PHY_BMCR);
        return;
    }
    if (enable)
    {
        /* xMII Loopback Mode */
        val |= BMCR_LOOPBACK;
    }
    else
    {
        /* Normal Mode */
        val &= ~BMCR_LOOPBACK;
    }

    /* Specific predefined loopback configuration values are required for
     * normal mode or loopback mode */
    EnetPhy_writeReg(hPhy, PHY_BMCR, val);

    /* Software restart is required after changing LOOPCR register */
    Dp83tg720_reset(hPhy);

    do
    {
        complete = Dp83tg720_isResetComplete(hPhy);
    }
    while (!complete);
}


static void Dp83tg720_reset(EnetPhy_Handle hPhy)
{
    /* Global software reset */
    ENETTRACE_DBG("PHY %u: global soft-reset\n", hPhy->addr);
    EnetPhy_rmwReg(hPhy, MII_DP83TG720_RESET_CTRL, DP83TG720_SW_RESET, DP83TG720_SW_RESET);
}

static void Dp83tg720_resetHw(EnetPhy_Handle hPhy)
{
    /* Global hardware reset */
    ENETTRACE_DBG("PHY %u: global hard-reset\n", hPhy->addr);
    EnetPhy_rmwReg(hPhy, MII_DP83TG720_RESET_CTRL, DP83TG720_HW_RESET, DP83TG720_HW_RESET);
}

static bool Dp83tg720_isResetComplete(EnetPhy_Handle hPhy)
{
    int32_t status;
    uint16_t val;
    bool complete = false;

    /* Reset is complete when RESET bits have self-cleared */
    status = EnetPhy_readReg(hPhy, MII_DP83TG720_RESET_CTRL, &val);
    if (status == ENETPHY_SOK)
    {
        complete = ((val & (DP83TG720_SW_RESET | DP83TG720_HW_RESET)) == 0U);
    }

    ENETTRACE_DBG("PHY %u: global reset is %s complete\n", hPhy->addr, complete ? "" : "not");

    return complete;
}

static int32_t Dp83tg720_readMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t *val)
{
    int32_t status;

    status = EnetPhy_writeReg(hPhy, PHY_MMD_CR, devad | MMD_CR_ADDR);

    if (status == ENETPHY_SOK)
    {
        status = EnetPhy_writeReg(hPhy, PHY_MMD_DR, reg);
    }

    if (status == ENETPHY_SOK)
    {
        EnetPhy_writeReg(hPhy, PHY_MMD_CR, devad | MMD_CR_DATA_NOPOSTINC);
    }

    if (status == ENETPHY_SOK)
    {
        status = EnetPhy_readReg(hPhy, PHY_MMD_DR, val);
    }

    ENETTRACE_VERBOSE_IF(status == ENETPHY_SOK,
                         "PHY %u: read reg %u val 0x%04x\n", hPhy->addr, reg, *val);
    ENETTRACE_ERR_IF(status != ENETPHY_SOK,
                     "PHY %u: failed to read reg %u\n", hPhy->addr, reg);

    return status;
}

static void Dp83tg720_writeMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t val)
{
    int32_t status;

    ENETTRACE_VERBOSE("PHY %u: write %u val 0x%04x\n", hPhy->addr, reg, val);

    status = EnetPhy_writeReg(hPhy, PHY_MMD_CR, devad | MMD_CR_ADDR);
    if (status == ENETPHY_SOK)
    {
        EnetPhy_writeReg(hPhy, PHY_MMD_DR, reg);
    }

    if (status == ENETPHY_SOK)
    {
        EnetPhy_writeReg(hPhy, PHY_MMD_CR, devad | MMD_CR_DATA_NOPOSTINC);
    }

    if (status == ENETPHY_SOK)
    {
        EnetPhy_writeReg(hPhy, PHY_MMD_DR, val);
    }

    ENETTRACE_ERR_IF(status != ENETPHY_SOK,
                     "PHY %u: failed to write reg %u val 0x%04x\n", hPhy->addr, reg, val);
}

static void Dp83tg720_setBitsMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t val)
{
    uint16_t value;
    int32_t status;

    status = Dp83tg720_readMmd(hPhy, devad, reg, &value);
    if (status == ENETPHY_SOK)
    {
        value = value | val;
        Dp83tg720_writeMmd(hPhy, devad, reg, value);
    }
}

static void Dp83tg720_readStraps(EnetPhy_Handle hPhy, Dp83tg720_operationMode operationMode)
{
    uint16_t strap;
    int32_t status;

    status = Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_STRAP, &strap);
    if (status != ENETPHY_SOK)
        return;

    ENETTRACE_DBG("PHY %u: Strap register is 0x%X\n", hPhy->addr, strap);

    if(operationMode == DP83TG720_STRAP_MODE)
    {
        if (strap & DP83TG720_MASTER_MODE_EN)
        {
            dp83tg720_params.is_controller = true;
            ENETTRACE_DBG("PHY %u: Strap: controller Mode enabled\n", hPhy->addr);
        }
        else
        {
            dp83tg720_params.is_controller = false;
           ENETTRACE_DBG("PHY %u: Strap: peripheral Mode enabled\n", hPhy->addr);
        }
    }

    if (strap & DP83TG720_RGMII_IS_EN)
    {
        ENETTRACE_DBG("PHY %u: Strap: RGMII Mode enabled\n", hPhy->addr);
        if (strap & DP83TG720_TX_SHIFT_EN)
            ENETTRACE_DBG("PHY %u: Strap: TX Clock Shift enabled\n", hPhy->addr);
        if (strap & DP83TG720_RX_SHIFT_EN)
            ENETTRACE_DBG("PHY %u: Strap: RX Clock Shift enabled\n", hPhy->addr);
    }
    else if (strap & DP83TG720_SGMII_IS_EN)
    {
        ENETTRACE_DBG("PHY %u: Strap: SGMII Mode enabled\n", hPhy->addr);
    }
};

static void Dp83tg720_writeSeq(EnetPhy_Handle hPhy, const struct dp83tg720_init_reg *init_data, int size)
{
    int index;
    for (index = 0; index < size; index++) {
        Dp83tg720_writeMmd(hPhy, init_data[index].mmd, init_data[index].reg, init_data[index].val);
    }
}

static void Dp83tg720_chipInit(EnetPhy_Handle hPhy)
{
    bool complete = false;

    /* Perform a hardware reset prior to configuration */
    Dp83tg720_resetHw(hPhy);
    do
    {
        complete = Dp83tg720_isResetComplete(hPhy);
    }while (!complete);

    /* Apply chip-specific configuration */
    switch (dp83tg720_params.chip) {
        case DP83TG720_CS1:
            if (dp83tg720_params.is_controller){
                Dp83tg720_writeSeq(hPhy, dp83tg720_cs1_controller_init,
                                   sizeof(dp83tg720_cs1_controller_init)/sizeof(dp83tg720_cs1_controller_init[0]));
                ENETTRACE_DBG("PHY %u: Applying configuration for DP83TG720 CS1.0 controller\n", hPhy->addr);}
            else{
                Dp83tg720_writeSeq(hPhy, dp83tg720_cs1_peripheral_init,
                                   sizeof(dp83tg720_cs1_peripheral_init)/sizeof(dp83tg720_cs1_peripheral_init[0]));
                ENETTRACE_DBG("PHY %u: Applying configuration for DP83TG720 CS1.0 peripheral\n", hPhy->addr);}
            break;
        case DP83TG720_CS1_1:
            if (dp83tg720_params.is_controller){
                Dp83tg720_writeSeq(hPhy, dp83tg720_cs1_1_controller_init,
                                   sizeof(dp83tg720_cs1_1_controller_init)/sizeof(dp83tg720_cs1_1_controller_init[0]));
                ENETTRACE_DBG("PHY %u: Applying configuration for DP83TG720 CS1.1 controller\n", hPhy->addr);}
            else{
                Dp83tg720_writeSeq(hPhy, dp83tg720_cs1_1_peripheral_init,
                                   sizeof(dp83tg720_cs1_1_peripheral_init)/sizeof(dp83tg720_cs1_1_peripheral_init[0]));
                ENETTRACE_DBG("PHY %u: Applying configuration for DP83TG720 CS1.1 peripheral\n", hPhy->addr);}
            break;
        default:
            ENETTRACE_DBG("PHY %u: No supported DP83TG720 Chip. Skipping chip-specific configuration!\n", hPhy->addr);
            break;
    };

    /* Perform a software reset to restart the PHY with the updated configuration */
    Dp83tg720_reset(hPhy);
    do
    {
        complete = Dp83tg720_isResetComplete(hPhy);
    }while (!complete);

    /* Let the PHY start link-up procedure */
    Dp83tg720_writeMmd(hPhy, DP83TG720_DEVADDR, 0x0573U, 0x0001U);

    /* Start send-s detection during link-up sequence */
    Dp83tg720_writeMmd(hPhy, DP83TG720_DEVADDR, 0x056AU, 0x5F41U);
}

static void Dp83tg720_setMiiMode(EnetPhy_Handle hPhy, EnetPhy_Mii mii)
{
    uint16_t rgmii_val = 0U;
    uint16_t sgmii_val = 0U;
    int32_t status = ENETPHY_SOK;

    status = Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_RGMII_CTRL, &rgmii_val);
    if(status != ENETPHY_SOK)
        return;
    status = Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_SGMII_CTRL, &sgmii_val);
    if(status != ENETPHY_SOK)
        return;

    if (mii == ENETPHY_MAC_MII_RGMII)
    {
        rgmii_val |= DP83TG720_RGMII_EN;
        sgmii_val &= ~DP83TG720_SGMII_EN;
        ENETTRACE_DBG("PHY %u: RGMII Mode enabled\n", hPhy->addr);
    }
    else if (mii == ENETPHY_MAC_MII_SGMII)
    {
        rgmii_val &= ~DP83TG720_RGMII_EN;
        sgmii_val |= DP83TG720_SGMII_EN;
        ENETTRACE_DBG("PHY %u: SGMII Mode enabled\n", hPhy->addr);
    }

    Dp83tg720_writeMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_RGMII_CTRL, rgmii_val);
    Dp83tg720_writeMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_SGMII_CTRL, sgmii_val);
}

static void Dp83tg720_configAutoNeg(EnetPhy_Handle hPhy, bool sgmiiAutoNegEn)
{
    uint16_t val = 0U;
    int32_t status = ENETPHY_SOK;

    status = Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_SGMII_CTRL, &val);

    if(status != ENETPHY_SOK)
        return;

    if(sgmiiAutoNegEn)
    {
        val |= DP83TG720_SGMII_AUTO_NEG_EN;
        ENETTRACE_DBG("PHY %u: SGMII Auto Negotiation enabled\n", hPhy->addr);
    }
    else
    {
        val &= ~DP83TG720_SGMII_AUTO_NEG_EN;
        ENETTRACE_DBG("PHY %u: SGMII Auto Negotiation disabled\n", hPhy->addr);
    }

    Dp83tg720_writeMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_SGMII_CTRL, val);
}

static void Dp83tg720_configClkShift(EnetPhy_Handle hPhy, bool txClkShiftEn, bool rxClkShiftEn)
{
    uint16_t val = 0U;
    int32_t status = ENETPHY_SOK;

    status = Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_RGMII_ID_CTRL, &val);

    if(status != ENETPHY_SOK)
        return;

    if (!txClkShiftEn)
        val &= ~DP83TG720_TX_CLK_SHIFT;
    else
        val |= DP83TG720_TX_CLK_SHIFT;
    if (!rxClkShiftEn)
        val &= ~DP83TG720_RX_CLK_SHIFT;
    else
        val |= DP83TG720_RX_CLK_SHIFT;

    Dp83tg720_writeMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_RGMII_ID_CTRL, val);

    ENETTRACE_DBG("PHY %u: RGMII TX Clock Shift %s\n", hPhy->addr, txClkShiftEn ? "enabled" : "disabled");
    ENETTRACE_DBG("PHY %u: RGMII RX Clock Shift %s\n", hPhy->addr, rxClkShiftEn ? "enabled" : "disabled");
}

static int32_t Dp83tg720_setClkDelay(EnetPhy_Handle hPhy,
                                     uint32_t txDelay,
                                     uint32_t rxDelay)
{
    uint8_t txDelaySize, rxDelaySize;
    uint8_t txIndex, rxIndex;
    uint16_t val;
    int32_t status = ENETPHY_SOK;

    txDelaySize = ENET_ARRAYSIZE(dp83tg720_txSkew);
    rxDelaySize = ENET_ARRAYSIZE(dp83tg720_rxSkew);

    if (((txDelay > dp83tg720_txSkew[txDelaySize - 1]) || (rxDelay > dp83tg720_rxSkew[rxDelaySize - 1]))) {
        ENETTRACE_DBG("PHY %u: Invalid RGMII TX Clock skew %d\n", hPhy->addr, txDelay);
        status = ENETPHY_EINVALIDPARAMS;
    }

    if (status == ENETPHY_SOK) {
        ENETTRACE_DBG("PHY %u: set delay %u ps TX, %u ps RX\n", hPhy->addr, txDelay, rxDelay);

        /* Avoids wrong value of delayCtrl if txDelay is 0 */
        txIndex   = Dp83tg720_getDelayIndex(txDelay, &dp83tg720_txSkew[0], txDelaySize);
        val       = (uint16_t)(((txIndex + 1) << DP83TG720_TXDLYCTRL_OFFSET) & DP83TG720_TXDLYCTRL_MASK);

        /* Avoids wrong value of delayCtrl if rxDelay is 0 */
        rxIndex = Dp83tg720_getDelayIndex(rxDelay, &dp83tg720_rxSkew[0], rxDelaySize);
        val      |= (uint16_t)((rxIndex << DP83TG720_RXDLYCTRL_OFFSET) & DP83TG720_RXDLYCTRL_MASK);

        Dp83tg720_writeMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_A2D_REG48, val);
    }

    return status;
}

static void Dp83tg720_configIntr(EnetPhy_Handle hPhy, bool intrEn)
 {
    uint16_t reg_val;
    int32_t status;

    if (intrEn) {
        ENETTRACE_DBG("PHY %u: Enable interrupts\n", hPhy->addr);
        status = EnetPhy_readReg(hPhy, MII_DP83TG720_INT_STAT1, &reg_val);
        if (status != ENETPHY_SOK)
            return;

        reg_val |= (DP83TG720_ANEG_COMPLETE_INT_EN |
                    DP83TG720_ESD_EVENT_INT_EN |
                    DP83TG720_LINK_STAT_INT_EN |
                    DP83TG720_ENERGY_DET_INT_EN |
                    DP83TG720_LINK_QUAL_INT_EN);

        EnetPhy_writeReg(hPhy, MII_DP83TG720_INT_STAT1, reg_val);

        status = EnetPhy_readReg(hPhy, MII_DP83TG720_INT_STAT2, &reg_val);
        if (status != ENETPHY_SOK)
            return;

        reg_val |= (DP83TG720_SLEEP_MODE_INT_EN |
                    DP83TG720_OVERTEMP_INT_EN |
                    DP83TG720_OVERVOLTAGE_INT_EN |
                    DP83TG720_UNDERVOLTAGE_INT_EN);

        EnetPhy_writeReg(hPhy, MII_DP83TG720_INT_STAT2, reg_val);

        status = EnetPhy_readReg(hPhy, MII_DP83TG720_INT_STAT3, &reg_val);
        if (status != ENETPHY_SOK)
            return;

        reg_val |= (DP83TG720_LPS_INT_EN |
                    DP83TG720_WAKE_REQ_EN |
                    DP83TG720_NO_FRAME_INT_EN |
                    DP83TG720_POR_DONE_INT_EN);

        EnetPhy_writeReg(hPhy, MII_DP83TG720_INT_STAT3, reg_val);
    }
    else {
        ENETTRACE_DBG("PHY %u: Disable interrupts\n", hPhy->addr);
        EnetPhy_writeReg(hPhy, MII_DP83TG720_INT_STAT1, 0U);

        EnetPhy_writeReg(hPhy, MII_DP83TG720_INT_STAT2, 0U);

        EnetPhy_writeReg(hPhy, MII_DP83TG720_INT_STAT3, 0U);
    }
}

static void Dp83tg720_printRegs(EnetPhy_Handle hPhy)
{
    uint32_t phyAddr = hPhy->addr;
    uint16_t val;

    EnetPhy_readReg(hPhy, PHY_BMCR, &val);
    EnetUtils_printf("PHY %u: BMCR    = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_BMSR, &val);
    EnetUtils_printf("PHY %u: BMSR    = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_PHYIDR1, &val);
    EnetUtils_printf("PHY %u: PHYIDR1 = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, PHY_PHYIDR2, &val);
    EnetUtils_printf("PHY %u: PHYIDR2 = 0x%04x\n", phyAddr, val);

    Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_SGMII_CTRL, &val);
    EnetUtils_printf("PHY %u: SGMII_CTRL = 0x%04x\n", hPhy->addr, val);
    Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_RGMII_CTRL, &val);
    EnetUtils_printf("PHY %u: RGMII_CTRL = 0x%04x\n", hPhy->addr, val);
    Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_RGMII_ID_CTRL, &val);
    EnetUtils_printf("PHY %u: RGMII_DELAY_CTRL = 0x%04x\n", hPhy->addr, val);
    Dp83tg720_readMmd(hPhy, DP83TG720_DEVADDR, DP83TG720_A2D_REG48, &val);
    EnetUtils_printf("PHY %u: A2D_REG_48 = 0x%04x\n", hPhy->addr, val);
}
