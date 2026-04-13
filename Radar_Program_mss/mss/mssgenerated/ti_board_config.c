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
#include "ti_board_config.h"

/*
 * Auto generated file
 */


#include <stdint.h>
#include <enet.h>
#include <networking/enet/core/include/phy/enetphy.h>
#include <networking/enet/core/include/phy/dp83867.h>
#include <networking/enet/utils/include/enet_apputils.h>
#include <drivers/hw_include/cslr_soc.h>
#include <networking/enet/core/src/phy/enetphy_priv.h>

#define CONFIG_ENET_CPSW0_PHY0_ADDR (0U)

/* PHY drivers */
extern EnetPhy_Drv gEnetPhyDrvGeneric;
extern EnetPhy_Drv gEnetPhyDrvDp83822;
extern EnetPhy_Drv gEnetPhyDrvDp83867;
extern EnetPhy_Drv gEnetPhyDrvDp83869;
extern EnetPhy_Drv gEnetPhyDrvVsc8514;

/*! \brief All the registered PHY specific drivers. */
static const EnetPhyDrv_Handle gEnetPhyDrvs[] =
{
    &gEnetPhyDrvDp83867,   /* DP83867 */
    &gEnetPhyDrvGeneric,   /* Generic PHY - must be last */
};

const EnetPhy_DrvInfoTbl gEnetPhyDrvTbl =
{
    .numHandles = ENET_ARRAYSIZE(gEnetPhyDrvs),
    .hPhyDrvList = gEnetPhyDrvs,
};

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

/**
   TPR:MSS_CTRL:CPSW_CONTROL

   Address offset    0x0000016C
   Physical address  0x0212016C
   Instance          MSS_CTRL
   CPSW_CONTROL_RGMII1_ID_MODE     16  Writing 1'b1 would disable the internal clock delays. And those delays need to be handled on board.
   CPSW_CONTROL_RMII_REF_CLK_OE_N  8   To select the rmii_ref_clk from PAD or from MSS_RCM. 0: clock will be from mss_rcm through IO internal loopback 1: will be from
   CPSW_CONTROL_PORT1_MODE_SEL     2:0 Port 1 Interface
                                         00 = GMII/MII
                                         01 = RMII
                                         10 = RGMII
                                         11 = Not Supported
*/

#define MSS_CPSW_CONTROL_PORT_MODE_RMII                                   (0x1U)
#define MSS_CPSW_CONTROL_PORT_MODE_RGMII                                  (0x2U)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* None */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

static const EnetBoard_PortCfg *EnetBoard_getPortCfg(const EnetBoard_EthPort *ethPort);

static const EnetBoard_PortCfg *EnetBoard_findPortCfg(const EnetBoard_EthPort *ethPort,
                                                      const EnetBoard_PortCfg *ethPortCfgs,
                                                      uint32_t numEthPorts);

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

/*!
 * \brief Common Processor Board (CPB) board's DP83867 PHY configuration.
 */
static const Dp83867_Cfg gEnetCpbBoard_dp83867PhyCfg =
{
    .txClkShiftEn         = false,
    .rxClkShiftEn         = true,
    .txDelayInPs          = 250U,   /* 0.250 ns */
    .rxDelayInPs          = 2000U,  /* 2.000 ns */
    .txFifoDepth          = 4U,
    .idleCntThresh        = 4U,     /* Improves short cable performance */
    .impedanceInMilliOhms = 35000,  /* 35 ohms */
    .gpio0Mode            = DP83867_GPIO0_LED3,
    .gpio1Mode            = DP83867_GPIO1_COL, /* Unused */
    .ledMode              =
    {
        DP83867_LED_LINKED,         /* Unused */
        DP83867_LED_LINKED_100BTX,
        DP83867_LED_RXTXACT,
        DP83867_LED_LINKED_1000BT,
    },
};

/*
 * AWR294x board configuration.
 *
 * 1 x RGMII PHY connected to AWR294x CPSW_2G MAC port.
 */
static const EnetBoard_PortCfg gEnetCpbBoard_awr294xEthPort[] =
{
    {    /* "CPSW2G" */
        .enetType = ENET_CPSW_2G,
        .instId   = 0U,
        .macPort  = ENET_MAC_PORT_1,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg   =
        {
            .phyAddr         = 0,
            .isStrapped      = false,
            .skipExtendedCfg = false,
            .extendedCfg     = &gEnetCpbBoard_dp83867PhyCfg,
            .extendedCfgSize = sizeof(gEnetCpbBoard_dp83867PhyCfg),
        },
        .flags    = 0U,
    },
};

/*
 * AWR294x virtual board used for MAC loopback setup.
 */
static const EnetBoard_PortCfg gEnetLpbkBoard_awr294xEthPort[] =
{
    {    /* RGMII MAC loopback */
        .enetType = ENET_CPSW_2G,
        .instId   = 0U,
        .macPort  = ENET_MAC_PORT_1,
        .mii      = { ENET_MAC_LAYER_GMII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg   =
        {
            .phyAddr = ENETPHY_INVALID_PHYADDR,
        },
        .flags    = 0U,
    },
    {    /* RMII MAC loopback */
        .enetType = ENET_CPSW_2G,
        .instId   = 0U,
        .macPort  = ENET_MAC_PORT_1,
        .mii      = { ENET_MAC_LAYER_MII, ENET_MAC_SUBLAYER_REDUCED },
        .phyCfg   =
        {
            .phyAddr = ENETPHY_INVALID_PHYADDR,
        },
        .flags    = 0U,
    },
};

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

const EnetBoard_PhyCfg *EnetBoard_getPhyCfg(const EnetBoard_EthPort *ethPort)
{
    const EnetBoard_PortCfg *portCfg;

    portCfg = EnetBoard_getPortCfg(ethPort);

    return (portCfg != NULL) ? &portCfg->phyCfg : NULL;
}

static const EnetBoard_PortCfg *EnetBoard_getPortCfg(const EnetBoard_EthPort *ethPort)
{
    const EnetBoard_PortCfg *portCfg = NULL;

    if (ENET_NOT_ZERO(ethPort->boardId & ENETBOARD_CPB_ID))
    {
        portCfg = EnetBoard_findPortCfg(ethPort,
                                        gEnetCpbBoard_awr294xEthPort,
                                        ENETPHY_ARRAYSIZE(gEnetCpbBoard_awr294xEthPort));
    }
    if ((portCfg == NULL) &&
        ENET_NOT_ZERO(ethPort->boardId & ENETBOARD_LOOPBACK_ID))
    {
        portCfg = EnetBoard_findPortCfg(ethPort,
                                        gEnetLpbkBoard_awr294xEthPort,
                                        ENETPHY_ARRAYSIZE(gEnetLpbkBoard_awr294xEthPort));
    }

    return portCfg;
}

static const EnetBoard_PortCfg *EnetBoard_findPortCfg(const EnetBoard_EthPort *ethPort,
                                                      const EnetBoard_PortCfg *ethPortCfgs,
                                                      uint32_t numEthPorts)
{
    const EnetBoard_PortCfg *ethPortCfg = NULL;
    bool found = false;
    uint32_t i;

    for (i = 0U; i < numEthPorts; i++)
    {
        ethPortCfg = &ethPortCfgs[i];

        if ((ethPortCfg->enetType == ethPort->enetType) &&
            (ethPortCfg->instId == ethPort->instId) &&
            (ethPortCfg->macPort == ethPort->macPort) &&
            (ethPortCfg->mii.layerType == ethPort->mii.layerType) &&
            (ethPortCfg->mii.sublayerType == ethPort->mii.sublayerType))
        {
            found = true;
            break;
        }
    }

    return found ? ethPortCfg : NULL;
}

void EnetBoard_getMiiConfig(EnetMacPort_Interface *mii)
{
    mii->layerType      = ENET_MAC_LAYER_GMII;
    mii->variantType    = ENET_MAC_VARIANT_FORCED;
    mii->sublayerType   = ENET_MAC_SUBLAYER_REDUCED;
}

int32_t EnetBoard_setupPorts(EnetBoard_EthPort *ethPorts,
                             uint32_t numEthPorts)
{
    CSL_mss_ctrlRegs *mssCtrlRegs = (CSL_mss_ctrlRegs *)CSL_MSS_CTRL_U_BASE;

    DebugP_assert(numEthPorts == 1);
    DebugP_assert(ethPorts->mii.sublayerType == ENET_MAC_SUBLAYER_REDUCED);

    switch(ethPorts->macPort)
    {
        case ENET_MAC_PORT_1:
            CSL_FINS( mssCtrlRegs->CPSW_CONTROL,MSS_CTRL_CPSW_CONTROL_CPSW_CONTROL_PORT1_MODE_SEL, MSS_CPSW_CONTROL_PORT_MODE_RGMII);
            break;
        default:
            DebugP_assert(false);
    }

    /* Nothing else to do */
    return ENET_SOK;
}

void EnetBoard_getMacAddrList(uint8_t macAddr[][ENET_MAC_ADDR_LEN],
                              uint32_t maxMacEntries,
                              uint32_t *pAvailMacEntries)
{
    EnetAppUtils_assert(false);
}

/*
 * Get ethernet board id
 */
uint32_t EnetBoard_getId(void)
{
    return ENETBOARD_AWR294X_EVM;
}



void Board_init(void)
{
}

void Board_deinit(void)
{
}
