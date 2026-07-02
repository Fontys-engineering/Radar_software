/*
 *  Copyright (c) Texas Instruments Incorporated 2021
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
 * \file  dp83tc812.c
 *
 * \brief This file contains the implementation of the DP83TC812 PHY.
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <stdint.h>
#include <include/core/enet_utils.h>
#include <priv/core/enet_trace_priv.h>
#include <include/phy/enetphy.h>
#include <include/phy/dp83tc812.h>
#include "enetphy_priv.h"
#include "generic_phy.h"
#include "dp83tc812_priv.h"

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define DP83TC812_OUI                         (0x080028U)
#define DP83TC812_MODEL                       (0x27U)
#define DP83TC812_REV_CS1					  (0U)
#define DP83TC812_REV_CS2			          (1U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

enum dp83720_chip_type {
         DP83TC812_CS1 = 0,
         DP83TC812_CS2 = 1,
};

struct dp83tc812_privParams {
         int chip;
		 bool is_master;
};

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static bool Dp83tc812_isPhyDevSupported(EnetPhy_Handle hPhy,
                                      const EnetPhy_Version *version);

static bool Dp83tc812_isMacModeSupported(EnetPhy_Handle hPhy,
                                       EnetPhy_Mii mii);

static int32_t Dp83tc812_config(EnetPhy_Handle hPhy,
                              const EnetPhy_Cfg *cfg,
                              EnetPhy_Mii mii);

static void Dp83tc812_setLoopbackCfg(EnetPhy_Handle hPhy,
                                   bool enable);

static void Dp83tc812_reset(EnetPhy_Handle hPhy);

static void Dp83tc812_resetHw(EnetPhy_Handle hPhy);

static bool Dp83tc812_isResetComplete(EnetPhy_Handle hPhy);

static int32_t Dp83tc812_readMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t *val);

static void Dp83tc812_writeMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t val);

static void Dp83tc812_setBitsMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t val);

static void Dp83tc812_readStraps(EnetPhy_Handle hPhy, Dp83tc812_operationMode msMode);

static void Dp83tc812_writeSeq(EnetPhy_Handle hPhy, const struct dp83tc812_init_reg *init_data, int size);

static void Dp83tc812_chipInit(EnetPhy_Handle hPhy);

static void Dp83tc812_setMiiMode(EnetPhy_Handle hPhy, EnetPhy_Mii mii);

static void Dp83tc812_configAutoNeg(EnetPhy_Handle hPhy, bool sgmiiAutoNegEn);

static void Dp83tc812_configClkShift(EnetPhy_Handle hPhy, bool txClkShiftEn, bool rxClkShiftEn);

static void Dp83tc812_configIntr(EnetPhy_Handle hPhy, bool intrEn);

static void Dp83tc812_printRegs(EnetPhy_Handle hPhy);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

EnetPhy_Drv gEnetPhyDrvDp83tc812 =
{
    .name               = "dp83tc812",
    .isPhyDevSupported  = Dp83tc812_isPhyDevSupported,
    .isMacModeSupported = Dp83tc812_isMacModeSupported,
    .config             = Dp83tc812_config,
    .reset              = Dp83tc812_reset,
    .isResetComplete    = Dp83tc812_isResetComplete,
    .readExtReg         = GenericPhy_readExtReg,
    .writeExtReg        = GenericPhy_writeExtReg,
    .printRegs          = Dp83tc812_printRegs,
};

/* PHY Device Attributes */
static struct dp83tc812_privParams dp83tc812_params = {
	.chip = -1,
	.is_master = false,
};


/*! \brief Chip specific init scripts */
static const struct dp83tc812_init_reg dp83tc812_cs1_master_init[] = {
		{0x01,0x0834,0xC001},
		{0x1F,0x0523,0x0001},
		{0x1F,0x0800,0xF864},
		{0x1F,0x0803,0x1552},
		{0x1F,0x0804,0x1A66},
		{0x1F,0x0805,0x1F7B},
		{0x1F,0x081F,0x2A88},
		{0x1F,0x0825,0x40E5},
		{0x1F,0x082B,0x7F3F},
		{0x1F,0x0830,0x0543},
		{0x1F,0x0836,0x5008},
		{0x1F,0x083A,0x08E0},
		{0x1F,0x083B,0x0845},
		{0x1F,0x083E,0x0445},
		{0x1F,0x0855,0x9B9A},
		{0x1F,0x085F,0x2010},
		{0x1F,0x0860,0x6040},
		{0x1F,0x086C,0x1333},
		{0x1F,0x086B,0x3E10},
		{0x1F,0x0872,0x88C0},
		{0x1F,0x0873,0x0003},
		{0x1F,0x0879,0x000F},
		{0x1F,0x087B,0x0070},
		{0x1F,0x087C,0x0002},
		{0x1F,0x0897,0x003F},
		{0x1F,0x089E,0x00AA},
		{0x1F,0x0510,0x000F},
};

static const struct dp83tc812_init_reg dp83tc812_cs1_slave_init[] = {
		{0x1F,0x0523,0x0001},
		{0x01,0x0834,0x8001},
		{0x1F,0x0803,0x1B52},
		{0x1F,0x0804,0x2166},
		{0x1F,0x0805,0x277B},
		{0x1F,0x0827,0x3000},
		{0x1F,0x0830,0x0543},
		{0x1F,0x083A,0x0020},
		{0x1F,0x083C,0x0001},
		{0x1F,0x0855,0x9B9A},
		{0x1F,0x085F,0x2010},
		{0x1F,0x0860,0x6040},
		{0x1F,0x086C,0x0333},
		{0x1F,0x0872,0x88C0},
		{0x1F,0x0873,0x0021},
		{0x1F,0x0879,0x000F},
		{0x1F,0x087B,0x0070},
		{0x1F,0x087C,0x0002},
		{0x1F,0x0897,0x003F},
		{0x1F,0x089E,0x00A2},
		{0x1F,0x0510,0x000F},
};

static const struct dp83tc812_init_reg dp83tc812_cs2_master_init[] = {
		{0x1F,0x0523,0x0001},
		{0x01,0x0834,0xC001},
		{0x1F,0x081C,0x0FE2},
		{0x1F,0x0872,0x0300},
		{0x1F,0x0879,0x0F00},
		{0x1F,0x0806,0x2952},
		{0x1F,0x0807,0x3361},
		{0x1F,0x0808,0x3D7B},
		{0x1F,0x083E,0x045F},
		{0x1F,0x0834,0x8000},
		{0x1F,0x0862,0x00E8},
		{0x1F,0x0896,0x32CB},
		{0x1F,0x003E,0x0009},
};

static const struct dp83tc812_init_reg dp83tc812_cs2_slave_init[] = {
		{0x1F,0x0523,0x0001},
		{0x01,0x0834,0x8001},
		{0x1F,0x0873,0x0821},
		{0x1F,0x0896,0x22FF},
		{0x1F,0x089E,0x0000},
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

void Dp83tc812_initCfg(Dp83tc812_Cfg *cfg)
{
	cfg->txClkShiftEn = true;
	cfg->rxClkShiftEn = true;
	cfg->interruptEn = false;
	cfg->sgmiiAutoNegEn = true;
	cfg->OpeMode = DP83TC812_STRAP_MODE;
}

static bool Dp83tc812_isPhyDevSupported(EnetPhy_Handle hPhy,
                                      const EnetPhy_Version *version)
{
    bool supported = false;

    if ((version->oui == DP83TC812_OUI) &&
        (version->model == (DP83TC812_MODEL)))
    {
        supported = true;
    }

	//Determine PHY version
    if (version->revision == DP83TC812_REV_CS1)
		dp83tc812_params.chip = DP83TC812_CS1;
    else if (version->revision == DP83TC812_REV_CS2)
		dp83tc812_params.chip = DP83TC812_CS2;

    return supported;
}

static bool Dp83tc812_isMacModeSupported(EnetPhy_Handle hPhy,
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

static int32_t Dp83tc812_config(EnetPhy_Handle hPhy,
                                const EnetPhy_Cfg *cfg,
								EnetPhy_Mii mii)
{
    const Dp83tc812_Cfg *extendedCfg = (const Dp83tc812_Cfg *)cfg->extendedCfg;
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
		Dp83tc812_readStraps(hPhy, extendedCfg->OpeMode);
	}

	/* Set Controller/Peripheral mode - through PHY config */
	if(extendedCfg->OpeMode == DP83TC812_CONTROLLER_MODE)
	{
		dp83tc812_params.is_master = true;
		ENETTRACE_DBG("PHY %u: Controller Mode enabled\n", hPhy->addr);
	}
	else if(extendedCfg->OpeMode == DP83TC812_PERIPHERAL_MODE)
	{
		dp83tc812_params.is_master = false;
		ENETTRACE_DBG("PHY %u: Peripheral Mode enabled\n", hPhy->addr);
	}

	/* Init specific chip */
	if (status == ENETPHY_SOK)
    {
		Dp83tc812_chipInit(hPhy);
	}

	/* Configure MII interface */
	if (status == ENETPHY_SOK)
    {
		Dp83tc812_setMiiMode(hPhy, mii);
	}

	/* Configure SGMII auto negotiation */
	if (status == ENETPHY_SOK &&
		mii == ENETPHY_MAC_MII_SGMII)
    {
		Dp83tc812_configAutoNeg(hPhy, extendedCfg->sgmiiAutoNegEn);
	}

	/* Configure RGMII clock shift */
	if (status == ENETPHY_SOK &&
		mii == ENETPHY_MAC_MII_RGMII)
	{
		Dp83tc812_configClkShift(hPhy,
								 extendedCfg->txClkShiftEn,
							     extendedCfg->rxClkShiftEn);
	}

	/* Configure interrupts */
	if (status == ENETPHY_SOK)
    {
		Dp83tc812_configIntr(hPhy, extendedCfg->interruptEn);
	}

	/* Set loopback configuration: enable or disable */
    if (status == ENETPHY_SOK)
    {
        Dp83tc812_setLoopbackCfg(hPhy, cfg->loopbackEn);
    }

    return status;
}

static void Dp83tc812_setLoopbackCfg(EnetPhy_Handle hPhy,
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
		val |= BMCR_LOOPBACK;
    }
    else
    {
		//Normal Mode
        val &= ~BMCR_LOOPBACK;
    }

	/* Specific predefined loopback configuration values are required for
     * normal mode or loopback mode */
    EnetPhy_writeReg(hPhy, PHY_BMCR, val);

    /* Software restart is required after changing LOOPCR register */
    Dp83tc812_reset(hPhy);

    do
    {
        complete = Dp83tc812_isResetComplete(hPhy);
    }
    while (!complete);
}


static void Dp83tc812_reset(EnetPhy_Handle hPhy)
{
    /* Global software reset */
    ENETTRACE_DBG("PHY %u: global soft-reset\n", hPhy->addr);
    EnetPhy_rmwReg(hPhy, MII_DP83TC812_RESET_CTRL, DP83TC812_SW_RESET, DP83TC812_SW_RESET);
}

static void Dp83tc812_resetHw(EnetPhy_Handle hPhy)
{
    /* Global hardware reset */
    ENETTRACE_DBG("PHY %u: global hard-reset\n", hPhy->addr);
    EnetPhy_rmwReg(hPhy, MII_DP83TC812_RESET_CTRL, DP83TC812_HW_RESET, DP83TC812_HW_RESET);
}

static bool Dp83tc812_isResetComplete(EnetPhy_Handle hPhy)
{
    int32_t status;
    uint16_t val;
    bool complete = false;

    /* Reset is complete when RESET bits have self-cleared */
    status = EnetPhy_readReg(hPhy, MII_DP83TC812_RESET_CTRL, &val);
    if (status == ENETPHY_SOK)
    {
        complete = ((val & (DP83TC812_SW_RESET | DP83TC812_HW_RESET)) == 0U);
    }

    ENETTRACE_DBG("PHY %u: global reset is %s complete\n", hPhy->addr, complete ? "" : "not");

    return complete;
}

static int32_t Dp83tc812_readMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t *val)
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

static void Dp83tc812_writeMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t val)
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

static void Dp83tc812_setBitsMmd(EnetPhy_Handle hPhy, uint16_t devad, uint32_t reg, uint16_t val)
{
	uint16_t value;
	int32_t status;
	status = Dp83tc812_readMmd(hPhy, devad, reg, &value);
	if (status == ENETPHY_SOK)
    {
		value = value | val;
		Dp83tc812_writeMmd(hPhy, devad, reg, value);
	}
}

static void Dp83tc812_readStraps(EnetPhy_Handle hPhy, Dp83tc812_operationMode msMode)
{
	uint16_t strap;
	int32_t status;

	status = Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_STRAP, &strap);
	if (status != ENETPHY_SOK)
		return;

	ENETTRACE_DBG("PHY %u: Strap register is 0x%X\n", hPhy->addr, strap);

	if(msMode == DP83TC812_STRAP_MODE)
	{
		if (strap & DP83TC812_MASTER_MODE_EN)
		{
			dp83tc812_params.is_master = true;
			ENETTRACE_DBG("PHY %u: Strap: Master Mode enabled\n", hPhy->addr);
		}
		else
		{
			dp83tc812_params.is_master = false;
			ENETTRACE_DBG("PHY %u: Strap: Slave Mode enabled\n", hPhy->addr);
		}
	}

	if (strap & DP83TC812_RGMII_IS_EN)
	{
		ENETTRACE_DBG("PHY %u: Strap: RGMII Mode enabled\n", hPhy->addr);
		if (((strap & DP83TC812_TX_RX_SHIFT) == DP83TC812_TX_RX_SHIFT_EN) ||
			((strap & DP83TC812_TX_RX_SHIFT) == DP83TC812_TX_SHIFT_EN))
			ENETTRACE_DBG("PHY %u: Strap: TX Clock Shift enabled\n", hPhy->addr);
		if (((strap & DP83TC812_TX_RX_SHIFT) == DP83TC812_TX_RX_SHIFT_EN) ||
			((strap & DP83TC812_TX_RX_SHIFT) == DP83TC812_RX_SHIFT_EN))
			ENETTRACE_DBG("PHY %u: Strap: RX Clock Shift enabled\n", hPhy->addr);
	}
	else
	{
		ENETTRACE_DBG("PHY %u: Strap: SGMII Mode enabled\n", hPhy->addr);
	}

};

static void Dp83tc812_writeSeq(EnetPhy_Handle hPhy, const struct dp83tc812_init_reg *init_data, int size)
{
	int i;
	for (i = 0; i < size; i++) {
		Dp83tc812_writeMmd(hPhy, init_data[i].mmd, init_data[i].reg, init_data[i].val);
	}
}

static void Dp83tc812_chipInit(EnetPhy_Handle hPhy)
{
	bool complete = false;

	/* Perform a hardware reset prior to configuration */
	Dp83tc812_resetHw(hPhy);
	do
    {
        complete = Dp83tc812_isResetComplete(hPhy);
    }
    while (!complete);

	/* Apply chip-specific configuration */
	switch (dp83tc812_params.chip) {
		case DP83TC812_CS1:
			if (dp83tc812_params.is_master){
				Dp83tc812_writeSeq(hPhy, dp83tc812_cs1_master_init,
								   sizeof(dp83tc812_cs1_master_init)/sizeof(dp83tc812_cs1_master_init[0]));
				ENETTRACE_DBG("PHY %u: Applying configuration for DP83TC812 CS1.0 Master\n", hPhy->addr);}
			else{
				Dp83tc812_writeSeq(hPhy, dp83tc812_cs1_slave_init,
								   sizeof(dp83tc812_cs1_slave_init)/sizeof(dp83tc812_cs1_slave_init[0]));
				ENETTRACE_DBG("PHY %u: Applying configuration for DP83TC812 CS1.0 Slave\n", hPhy->addr);}
			break;
		case DP83TC812_CS2:
			if (dp83tc812_params.is_master){
				Dp83tc812_writeSeq(hPhy, dp83tc812_cs2_master_init,
								   sizeof(dp83tc812_cs2_master_init)/sizeof(dp83tc812_cs2_master_init[0]));
				ENETTRACE_DBG("PHY %u: Applying configuration for DP83TC812 CS2.0 Master\n", hPhy->addr);}
			else{
				Dp83tc812_writeSeq(hPhy, dp83tc812_cs2_slave_init,
								   sizeof(dp83tc812_cs2_slave_init)/sizeof(dp83tc812_cs2_slave_init[0]));
				ENETTRACE_DBG("PHY %u: Applying configuration for DP83TC812 CS2.0 Slave\n", hPhy->addr);}
			break;
		default:
			ENETTRACE_DBG("PHY %u: No supported DP83TC812 Chip. Skipping chip-specific configuration!\n", hPhy->addr);
			break;
	};

	/* Perform a software reset to restart the PHY with the updated configuration */
	Dp83tc812_reset(hPhy);
	do
    {
        complete = Dp83tc812_isResetComplete(hPhy);
    }
    while (!complete);

	/* Enable transmitter */
	Dp83tc812_writeMmd(hPhy, DP83TC812_DEVADDR, 0x0523U, 0x0000U);

}

static void Dp83tc812_setMiiMode(EnetPhy_Handle hPhy, EnetPhy_Mii mii)
{
    uint16_t rgmii_val = 0U;
	uint16_t sgmii_val = 0U;
	int32_t status = ENETPHY_SOK;

	status = Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_RGMII_CTRL, &rgmii_val);
	if(status != ENETPHY_SOK)
		return;
	status = Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_SGMII_CTRL, &sgmii_val);
	if(status != ENETPHY_SOK)
		return;

    if (mii == ENETPHY_MAC_MII_RGMII)
    {
		rgmii_val |= DP83TC812_RGMII_EN;
		sgmii_val &= ~DP83TC812_SGMII_EN;
		ENETTRACE_DBG("PHY %u: RGMII Mode enabled\n", hPhy->addr);
    }
	else if (mii == ENETPHY_MAC_MII_SGMII)
	{
		rgmii_val &= ~DP83TC812_RGMII_EN;
		sgmii_val |= DP83TC812_SGMII_EN;
		ENETTRACE_DBG("PHY %u: SGMII Mode enabled\n", hPhy->addr);
	}

	Dp83tc812_writeMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_RGMII_CTRL, rgmii_val);
	Dp83tc812_writeMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_SGMII_CTRL, sgmii_val);
}

static void Dp83tc812_configAutoNeg(EnetPhy_Handle hPhy, bool sgmiiAutoNegEn)
{
	uint16_t val = 0U;
	int32_t status = ENETPHY_SOK;

	status = Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_SGMII_CTRL, &val);

	if(status != ENETPHY_SOK)
		return;

	if(sgmiiAutoNegEn)
	{
		val |= DP83TC812_SGMII_AUTO_NEG_EN;
		ENETTRACE_DBG("PHY %u: SGMII Auto Negotiation enabled\n", hPhy->addr);
	}
	else
	{
		val &= ~DP83TC812_SGMII_AUTO_NEG_EN;
		ENETTRACE_DBG("PHY %u: SGMII Auto Negotiation disabled\n", hPhy->addr);
	}

	Dp83tc812_writeMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_SGMII_CTRL, val);
}

static void Dp83tc812_configClkShift(EnetPhy_Handle hPhy, bool txClkShiftEn, bool rxClkShiftEn)
{
	uint16_t val = 0U;
	int32_t status = ENETPHY_SOK;

	status = Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_RGMII_ID_CTRL, &val);

	if(status != ENETPHY_SOK)
		return;

	if (!txClkShiftEn)
		val &= ~DP83TC812_TX_CLK_SHIFT;
	else
		val |= DP83TC812_TX_CLK_SHIFT;
	if (!rxClkShiftEn)
		val &= ~DP83TC812_RX_CLK_SHIFT;
	else
		val |= DP83TC812_RX_CLK_SHIFT;

	Dp83tc812_writeMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_RGMII_ID_CTRL, val);

	ENETTRACE_DBG("PHY %u: RGMII TX Clock Shift %s\n", hPhy->addr, txClkShiftEn ? "enabled" : "disabled");
	ENETTRACE_DBG("PHY %u: RGMII RX Clock Shift %s\n", hPhy->addr, rxClkShiftEn ? "enabled" : "disabled");
}

static void Dp83tc812_configIntr(EnetPhy_Handle hPhy, bool intrEn)
 {
	uint16_t reg_val;
	int32_t status;

	if (intrEn) {
		ENETTRACE_DBG("PHY %u: Enable interrupts\n", hPhy->addr);
		status = EnetPhy_readReg(hPhy, MII_DP83TC812_INT_STAT1, &reg_val);
        if (status != ENETPHY_SOK)
			return;

        reg_val |= (DP83TC812_ANEG_COMPLETE_INT_EN |
					DP83TC812_ESD_EVENT_INT_EN |
					DP83TC812_LINK_STAT_INT_EN |
                    DP83TC812_ENERGY_DET_INT_EN |
                    DP83TC812_LINK_QUAL_INT_EN);

		EnetPhy_writeReg(hPhy, MII_DP83TC812_INT_STAT1, reg_val);

		status = EnetPhy_readReg(hPhy, MII_DP83TC812_INT_STAT2, &reg_val);
        if (status != ENETPHY_SOK)
			return;

        reg_val |= (DP83TC812_SLEEP_MODE_INT_EN |
					DP83TC812_OVERTEMP_INT_EN |
					DP83TC812_OVERVOLTAGE_INT_EN |
					DP83TC812_UNDERVOLTAGE_INT_EN);

        EnetPhy_writeReg(hPhy, MII_DP83TC812_INT_STAT2, reg_val);

        status = EnetPhy_readReg(hPhy, MII_DP83TC812_INT_STAT3, &reg_val);
        if (status != ENETPHY_SOK)
			return;

        reg_val |= (DP83TC812_LPS_INT_EN |
					DP83TC812_WAKE_REQ_EN |
					DP83TC812_NO_FRAME_INT_EN |
					DP83TC812_POR_DONE_INT_EN);

		EnetPhy_writeReg(hPhy, MII_DP83TC812_INT_STAT3, reg_val);

    }
	else {
		ENETTRACE_DBG("PHY %u: Disable interrupts\n", hPhy->addr);
		EnetPhy_writeReg(hPhy, MII_DP83TC812_INT_STAT1, 0U);

        EnetPhy_writeReg(hPhy, MII_DP83TC812_INT_STAT2, 0U);

        EnetPhy_writeReg(hPhy, MII_DP83TC812_INT_STAT3, 0U);
    }
}

static void Dp83tc812_printRegs(EnetPhy_Handle hPhy)
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

    EnetPhy_readReg(hPhy, DP83TC812_PHYSTS, &val);
    EnetUtils_printf("PHY %u: PHYSTS  = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_PHYCR, &val);
    EnetUtils_printf("PHY %u: PHYCR   = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_MISR1, &val);
    EnetUtils_printf("PHY %u: MISR1   = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_MISR2, &val);
    EnetUtils_printf("PHY %u: MISR2   = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_RECR, &val);
    EnetUtils_printf("PHY %u: RECR    = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_BISCR, &val);
    EnetUtils_printf("PHY %u: BISCR   = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_MISR3, &val);
    EnetUtils_printf("PHY %u: MISR3   = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_REG19, &val);
    EnetUtils_printf("PHY %u: REG19   = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_CDCR, &val);
    EnetUtils_printf("PHY %u: CDCR    = 0x%04x\n", phyAddr, val);
    EnetPhy_readReg(hPhy, DP83TC812_PHYRCR, &val);
    EnetUtils_printf("PHY %u: PHYRCR  = 0x%04x\n", phyAddr, val);

	Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_SGMII_CTRL, &val);
	EnetUtils_printf("PHY %u: SGMII_CTRL      = 0x%04x\n", phyAddr, val);
	Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_RGMII_CTRL, &val);
	EnetUtils_printf("PHY %u: RGMII_CTRL      = 0x%04x\n", phyAddr, val);
	Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR, DP83TC812_RGMII_ID_CTRL, &val);
	EnetUtils_printf("PHY %u: RGMII_ID_CTRL   = 0x%04x\n", phyAddr, val);
	Dp83tc812_readMmd(hPhy, DP83TC812_DEVADDR_MMD1, DP83TC812_MMD1_PMA_CTRL_2, &val);
	EnetUtils_printf("PHY %u: MMD1_PMA_CTRL_2 = 0x%04x\n", phyAddr, val);
}
