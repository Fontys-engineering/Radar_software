/*
 *  Copyright (c) Texas Instruments Incorporated 2020
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
 * \file ti_enet_open_close.c
 *
 * \brief This file contains enet driver memory allocation related functionality.
 */


/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */

#include <string.h>
#include <stdint.h>
#include <stdarg.h>

#include <enet.h>
#include "enet_appmemutils.h"
#include "enet_appmemutils_cfg.h"
#include "enet_apputils.h"
#include <enet_cfg.h>
#include <include/core/enet_per.h>
#include <include/core/enet_utils.h>
#include <include/core/enet_dma.h>
#include <include/common/enet_utils_dflt.h>
#include <include/per/cpsw.h>
#include <priv/per/cpsw_cpdma_priv.h>
#include <include/core/enet_soc.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/TaskP.h>
#include <kernel/dpl/EventP.h>
#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/QueueP.h>

#include "ti_enet_config.h"
#include "ti_drivers_config.h"
#include "ti_enet_open_close.h"
#include <utils/include/enet_appsoc.h>

#define ENETAPP_PHY_STATEHANDLER_TASK_PRIORITY        (7U)
#define ENETAPP_PHY_STATEHANDLER_TASK_STACK     (3 * 1024)
#define AppEventId_CPSW_PERIODIC_POLL    (1 << 3)
#define ENETAPP_NUM_IET_VERIFY_ATTEMPTS               (20U)


    

typedef struct EnetAppTxDmaSysCfg_Obj_s
{
    /* TX channel handle */
    EnetDma_TxChHandle hTxCh;
    /* TX channel number */
    uint32_t txChNum;
} EnetAppTxDmaSysCfg_Obj;

typedef struct EnetAppRxDmaSysCfg_Obj_s
{
    /* RX channel handle */
    EnetDma_RxChHandle hRxCh;
    /* RX channel number */
    uint32_t rxChNum;
    /* mac Address valid */
    uint32_t numValidMacAddress;
    /* MAC address. It's port/ports MAC address in Dual-MAC or
     * host port MAC address in Switch */
    uint8_t macAddr[ENET_MAX_NUM_MAC_PER_PHER][ENET_MAC_ADDR_LEN];
} EnetAppRxDmaSysCfg_Obj;


static void EnetAppUtils_openTxCh(Enet_Handle hEnet,
                                  uint32_t coreKey,
                                  uint32_t coreId,
                                  uint32_t *pTxChNum,
                                  EnetDma_TxChHandle *pTxChHandle,
                                  EnetCpdma_OpenTxChPrms *pCpswTxChCfg);

static void EnetAppUtils_openRxCh(Enet_Handle hEnet,
                                  uint32_t coreKey,
                                  uint32_t coreId,
                                  uint32_t *pRxChNum,
                                  EnetDma_RxChHandle *pRxChHandle,
                                  EnetCpdma_OpenRxChPrms *pCpswRxChCfg,
                                  uint32_t allocMacAddrCnt,
                                  uint8_t macAddr[ENET_MAX_NUM_MAC_PER_PHER][ENET_MAC_ADDR_LEN]);

static void EnetAppUtils_setCommonRxChPrms(EnetCpdma_OpenRxChPrms *pRxChPrms);

static void EnetAppUtils_setCommonTxChPrms(EnetCpdma_OpenTxChPrms *pTxChPrms);

static void EnetApp_openRxDma(EnetAppRxDmaSysCfg_Obj *rx,
                              uint32_t numRxPkts,
                              Enet_Handle hEnet,
                              uint32_t coreKey,
                              uint32_t coreId,
                              uint32_t allocMacAddrCnt);

static void EnetApp_openTxDma(EnetAppTxDmaSysCfg_Obj *tx,
                              uint32_t numTxPkts,
                              Enet_Handle hEnet,
                              uint32_t coreKey,
                              uint32_t coreId);

static void EnetAppUtils_closeTxCh(Enet_Handle hEnet,
                                   uint32_t coreKey,
                                   uint32_t coreId,
                                   EnetDma_PktQ *pFqPktInfoQ,
                                   EnetDma_PktQ *pCqPktInfoQ,
                                   EnetDma_TxChHandle hTxChHandle,
                                   uint32_t txChNum);

static void EnetAppUtils_closeRxCh(Enet_Handle hEnet,
                                   uint32_t coreKey,
                                   uint32_t coreId,
                                   EnetDma_PktQ *pFqPktInfoQ,
                                   EnetDma_PktQ *pCqPktInfoQ,
                                   EnetDma_RxChHandle hRxChHandle,
                                   uint32_t rxChNum,
                                   uint32_t freeHostMacAddrCount,
                                   uint8_t macAddr[ENET_MAX_NUM_MAC_PER_PHER][ENET_MAC_ADDR_LEN]);

static int32_t EnetAppUtils_allocRxFlow(Enet_Handle hEnet,
                                        uint32_t coreKey,
                                        uint32_t coreId,
                                        uint32_t *rxFlowStartIdx,
                                        uint32_t *flowIdx);
static int32_t EnetAppUtils_allocTxCh(Enet_Handle hEnet,
                                      uint32_t coreKey,
                                      uint32_t coreId,
                                      uint32_t *txPSILThreadId);
static int32_t EnetAppUtils_freeRxFlow(Enet_Handle hEnet,
                                       uint32_t coreKey,
                                       uint32_t coreId,
                                       uint32_t rxFlowIdx);
static int32_t EnetAppUtils_freeTxCh(Enet_Handle hEnet,
                                     uint32_t coreKey,
                                     uint32_t coreId,
                                     uint32_t txChNum);





typedef struct EnetAppDmaSysCfg_Obj_s
{
    EnetAppTxDmaSysCfg_Obj tx[ENET_SYSCFG_TX_CHANNELS_NUM];
    EnetAppRxDmaSysCfg_Obj rx[ENET_SYSCFG_RX_FLOWS_NUM];
} EnetAppDmaSysCfg_Obj;


typedef struct EnetAppSysCfg_Obj_s
{

    Enet_Handle hEnet;
    EnetAppDmaSysCfg_Obj dma;
    ClockP_Object timerObj;

    TaskP_Object task_phyStateHandlerObj;

    SemaphoreP_Object timerSemObj;

    volatile bool timerTaskShutDownFlag;

    volatile bool timerTaskShutDownDoneFlag;

    uint8_t appPhyStateHandlerTaskStack[ENETAPP_PHY_STATEHANDLER_TASK_STACK] __attribute__ ((aligned(32)));
}EnetAppSysCfg_Obj;

static EnetAppSysCfg_Obj gEnetAppSysCfgObj;

static int32_t EnetApp_enablePorts(Enet_Handle hEnet,
                                   Enet_Type enetType,
                                   uint32_t instId,
                                   uint32_t coreId,
                                   Enet_MacPort macPortList[ENET_MAC_PORT_NUM],
                                   uint8_t numMacPorts);

static void EnetApp_getCpswInitCfg(Enet_Type enetType,
                                   uint32_t instId,
                                   Cpsw_Cfg *cpswCfg);

static void EnetApp_getMacPortInitConfig(CpswMacPort_Cfg *pMacPortCfg, const Enet_MacPort portIdx);

static void EnetApp_openDmaChannels(EnetAppDmaSysCfg_Obj *dma,
                                    Enet_Type enetType,
                                    uint32_t instId,
                                    Enet_Handle hEnet,
                                    uint32_t coreKey,
                                    uint32_t coreId);

static void EnetApp_openAllRxDmaChannels(EnetAppDmaSysCfg_Obj *dma,
                                         Enet_Handle hEnet,
                                         uint32_t coreKey,
                                         uint32_t coreId);

static void EnetApp_openAllTxDmaChannels(EnetAppDmaSysCfg_Obj *dma,
                                         Enet_Handle hEnet,
                                         uint32_t coreKey,
                                         uint32_t coreId);

static void EnetApp_ConfigureDscpMapping(Enet_Type enetType,
                                         uint32_t instId,
                                         uint32_t coreId,
                                         Enet_MacPort macPortList[ENET_MAC_PORT_NUM],
                                         uint8_t numMacPorts);


void EnetApp_phyStateHandler(void * appHandle);

static Enet_Handle EnetApp_doCpswOpen(Enet_Type enetType, uint32_t instId, const Cpsw_Cfg *cpswCfg)
{
    void *perCfg = NULL_PTR;
    uint32_t cfgSize;
    Enet_Handle hEnet;

    EnetAppUtils_assert(true == Enet_isCpswFamily(enetType));

    perCfg = (void *)cpswCfg;
    cfgSize = sizeof(*cpswCfg);

    hEnet = Enet_open(enetType, instId, perCfg, cfgSize);
    if(hEnet == NULL_PTR)
    {
        EnetAppUtils_print("Enet_open failed\r\n");
        EnetAppUtils_assert(hEnet != NULL_PTR);
    }

    return hEnet;
}


static void EnetApp_timerCb(ClockP_Object *clkInst, void * arg)
{
    SemaphoreP_Object *pTimerSem = (SemaphoreP_Object *)arg;

    /* Tick! */
    SemaphoreP_post(pTimerSem);
}
void EnetApp_phyStateHandler(void * appHandle)
{
    SemaphoreP_Object *timerSem;
    EnetAppSysCfg_Obj *hEnetAppObj       = (EnetAppSysCfg_Obj *)appHandle;

    timerSem = &hEnetAppObj->timerSemObj;
    hEnetAppObj->timerTaskShutDownDoneFlag = false;
    while (hEnetAppObj->timerTaskShutDownFlag != true)
    {
        SemaphoreP_pend(timerSem, SystemP_WAIT_FOREVER);
        /* Enet_periodicTick should be called from only task context */
        Enet_periodicTick(hEnetAppObj->hEnet);
    }
    hEnetAppObj->timerTaskShutDownDoneFlag = true;
    TaskP_destruct(&hEnetAppObj->task_phyStateHandlerObj);
    TaskP_exit();
}

static int32_t EnetApp_createPhyStateHandlerTask(EnetAppSysCfg_Obj *hEnetAppObj) // FREERTOS
{
    TaskP_Params tskParams;
    int32_t status;

    status = SemaphoreP_constructCounting(&hEnetAppObj->timerSemObj, 0, 128);
    EnetAppUtils_assert(status == SystemP_SUCCESS);
    {
        ClockP_Params clkParams;
        const uint32_t timPeriodTicks = ClockP_usecToTicks((ENETPHY_FSM_TICK_PERIOD_MS)*1000U);  // Set timer expiry time in OS ticks

        ClockP_Params_init(&clkParams);
        clkParams.start     = TRUE;
        clkParams.timeout   = timPeriodTicks;
        clkParams.period    = timPeriodTicks;
        clkParams.args      = &hEnetAppObj->timerSemObj;
        clkParams.callback  = &EnetApp_timerCb;

        /* Creating timer and setting timer callback function*/
        status = ClockP_construct(&hEnetAppObj->timerObj ,
                                  &clkParams);
        if (status == SystemP_SUCCESS)
        {
            hEnetAppObj->timerTaskShutDownFlag = false;
        }
        else
        {
            EnetAppUtils_print("EnetApp_createClock() failed to create clock\r\n");
        }
    }
    /* Initialize the taskperiodicTick params. Set the task priority higher than the
     * default priority (1) */
    TaskP_Params_init(&tskParams);
    tskParams.priority       = ENETAPP_PHY_STATEHANDLER_TASK_PRIORITY;
    tskParams.stack          = &hEnetAppObj->appPhyStateHandlerTaskStack[0];
    tskParams.stackSize      = sizeof(hEnetAppObj->appPhyStateHandlerTaskStack);
    tskParams.args           = hEnetAppObj;
    tskParams.name           = "EnetApp_PhyStateHandlerTask";
    tskParams.taskMain       =  &EnetApp_phyStateHandler;

    status = TaskP_construct(&hEnetAppObj->task_phyStateHandlerObj, &tskParams);
    EnetAppUtils_assert(status == SystemP_SUCCESS);

    return status;

}

void EnetApp_driverInit()
{
/* keep this implementation that is generic across enetType and instId.
 * Initialization should be done only once.
 */
    int32_t status = ENET_SOK;
    EnetOsal_Cfg osalPrms;
    EnetUtils_Cfg utilsPrms;

    /* Initialize Enet driver with default OSAL, utils */
    Enet_initOsalCfg(&osalPrms);
    Enet_initUtilsCfg(&utilsPrms);
    Enet_init(&osalPrms, &utilsPrms);

    status = EnetMem_init();
    EnetAppUtils_assert(ENET_SOK == status);
}

void EnetApp_driverDeInit()
{
/* keep this implementation that is generic across enetType and instId.
 * Denitialization should be done only once.
 */

    EnetMem_deInit();
    Enet_deinit();
}

int32_t EnetApp_driverOpen(Enet_Type enetType, uint32_t instId)
{
    int32_t status = ENET_SOK;
    Cpsw_Cfg cpswCfg;
    Enet_MacPort macPortList[ENET_MAC_PORT_NUM];
    uint8_t numMacPorts;
    uint32_t selfCoreId;
    EnetRm_ResCfg *resCfg = &cpswCfg.resCfg;
    EnetApp_HandleInfo handleInfo;
    EnetPer_AttachCoreOutArgs attachInfo;

    EnetCpdma_Cfg dmaCfg;

    EnetAppUtils_assert(Enet_isCpswFamily(enetType) == true);
    EnetApp_getEnetInstMacInfo(enetType, instId, macPortList, &numMacPorts);

    /* Set configuration parameters */
    selfCoreId   = EnetSoc_getCoreId();
    cpswCfg.dmaCfg = (void *)&dmaCfg;
    Enet_initCfg(enetType, instId, &cpswCfg, sizeof(cpswCfg));
    cpswCfg.dmaCfg = (void *)&dmaCfg;

    EnetApp_getCpswInitCfg(enetType, instId, &cpswCfg);

    resCfg = &cpswCfg.resCfg;
    EnetAppUtils_initResourceConfig(enetType, instId, selfCoreId, resCfg);

    dmaCfg.maxTxChannels = ENET_SYSCFG_TX_CHANNELS_NUM;
    dmaCfg.maxRxChannels = ENET_SYSCFG_RX_FLOWS_NUM;

    EnetApp_updateCpswInitCfg(enetType, instId, &cpswCfg);


    gEnetAppSysCfgObj.hEnet = EnetApp_doCpswOpen(enetType, instId, &cpswCfg);
    EnetAppUtils_assert(NULL != gEnetAppSysCfgObj.hEnet);

    EnetApp_enablePorts(gEnetAppSysCfgObj.hEnet, enetType, instId, selfCoreId, macPortList, numMacPorts);

    status = EnetApp_createPhyStateHandlerTask(&gEnetAppSysCfgObj);
    EnetAppUtils_assert(status == SystemP_SUCCESS);
    /* Open all DMA channels */
    EnetApp_acquireHandleInfo(enetType,
                              instId,
                              &handleInfo);

    (void)handleInfo; /* Handle info not used. Kill warning */
    EnetApp_coreAttach(enetType,
                       instId,
                       selfCoreId,
                       &attachInfo);
    EnetApp_openDmaChannels(&gEnetAppSysCfgObj.dma,
                            enetType,
                            instId,
                            gEnetAppSysCfgObj.hEnet,
                            attachInfo.coreKey,
                            selfCoreId);
    return status;
}

static void EnetApp_ConfigureDscpMapping(Enet_Type enetType,
                                         uint32_t instId,
                                         uint32_t coreId,
                                         Enet_MacPort macPortList[ENET_MAC_PORT_NUM],
                                         uint8_t numMacPorts)
{
    Enet_IoctlPrms prms;
    int32_t status;
    EnetMacPort_SetIngressDscpPriorityMapInArgs setMacDscpInArgs;
    EnetPort_DscpPriorityMap setHostDscpInArgs;
    uint32_t pri;
    uint8_t i;

    Enet_Handle hEnet = Enet_getHandle(enetType, instId);
    memset(&setMacDscpInArgs, 0, sizeof(setMacDscpInArgs));
    /* Each Port can have different dscp priority mapping values */
    for (i = 0U; i < numMacPorts; i++)
    {
        setMacDscpInArgs.macPort = macPortList[i];
        setMacDscpInArgs.dscpPriorityMap.dscpIPv4En = true;
        /* Example Mapping: 0 to 7  dscp values are mapped to 0 priority
         *                  8 to 15 dscp values are mapped to 1 priority
         *                  and so on
         *                  56 to 63 dscp values are mapped to 7 priority
         */
        for (pri = 0U; pri < 64U; pri++)
        {
            setMacDscpInArgs.dscpPriorityMap.tosMap[pri] = pri/8;
        }
        ENET_IOCTL_SET_IN_ARGS(&prms, &setMacDscpInArgs);
        ENET_IOCTL(hEnet, coreId, ENET_MACPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP, &prms, status);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Failed to set dscp Priority map for Port %u - %d \r\n", macPortList[i], status);
        }
    }

    /* Fill the dscp priority mapping reg of host port */
    memset(&setHostDscpInArgs, 0, sizeof(setHostDscpInArgs));
    setHostDscpInArgs.dscpIPv4En = true;
    for (pri = 0U; pri < 64U; pri++)
    {
        setHostDscpInArgs.tosMap[pri] = pri/8;
    }
    ENET_IOCTL_SET_IN_ARGS(&prms, &setHostDscpInArgs);
    ENET_IOCTL(hEnet, coreId, ENET_HOSTPORT_IOCTL_SET_INGRESS_DSCP_PRI_MAP, &prms, status);
    if (status != ENET_SOK)
    {
        EnetAppUtils_print("Failed to set dscp Priority map for Host Port - %d\r\n", status);
    }
    /* Enable the p0_rx_remap_dscp_ipv4 in CPPI_P0_Control for host port, This is done through syscfg */

}

static int32_t EnetApp_enablePorts(Enet_Handle hEnet,
                                   Enet_Type enetType,
                                   uint32_t instId,
                                   uint32_t coreId,
                                   Enet_MacPort macPortList[ENET_MAC_PORT_NUM],
                                   uint8_t numMacPorts)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    uint8_t i;

    for (i = 0U; i < numMacPorts; i++)
    {
        EnetPer_PortLinkCfg linkArgs;
        CpswMacPort_Cfg cpswMacCfg;

        linkArgs.macCfg = &cpswMacCfg;
        CpswMacPort_initCfg(&cpswMacCfg);
        EnetPhy_initCfg(&linkArgs.phyCfg);

        linkArgs.macPort = macPortList[i];
        EnetApp_getMacPortInitConfig(linkArgs.macCfg, macPortList[i]);
        EnetApp_initLinkArgs(enetType, instId, &linkArgs, macPortList[i]);

        ENET_IOCTL_SET_IN_ARGS(&prms, &linkArgs);
        ENET_IOCTL(hEnet,
                   coreId,
                   ENET_PER_IOCTL_OPEN_PORT_LINK,
                   &prms,
                   status);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetApp_enablePorts() failed to open MAC port: %d\r\n", status);
        }

    }

    if ((status == ENET_SOK) && (Enet_isCpswFamily(enetType)))
    {
        CpswAle_SetPortStateInArgs setPortStateInArgs;

        setPortStateInArgs.portNum   = CPSW_ALE_HOST_PORT_NUM;
        setPortStateInArgs.portState = CPSW_ALE_PORTSTATE_FORWARD;
        ENET_IOCTL_SET_IN_ARGS(&prms, &setPortStateInArgs);
        prms.outArgs = NULL_PTR;
        ENET_IOCTL(hEnet,
                   coreId,
                   CPSW_ALE_IOCTL_SET_PORT_STATE,
                   &prms,
                   status);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("EnetApp_enablePorts() failed CPSW_ALE_IOCTL_SET_PORT_STATE: %d\r\n", status);
        }

        if (status == ENET_SOK)
        {
            ENET_IOCTL_SET_NO_ARGS(&prms);
            ENET_IOCTL(hEnet,
                       coreId,
                       ENET_HOSTPORT_IOCTL_ENABLE,
                       &prms,
                       status);
            if (status != ENET_SOK)
            {
                EnetAppUtils_print("EnetApp_enablePorts() Failed to enable host port: %d\r\n", status);
            }
        }
    }

    /* Show alive PHYs */
    if (status == ENET_SOK)
    {
        Enet_IoctlPrms prms;
        bool alive;
        uint32_t i;

        for (i = 0U; i < ENET_MDIO_PHY_CNT_MAX; i++)
        {
            ENET_IOCTL_SET_INOUT_ARGS(&prms, &i, &alive);
            ENET_IOCTL(hEnet,
                       coreId,
                       ENET_MDIO_IOCTL_IS_ALIVE,
                       &prms,
                       status);
            if (status == ENET_SOK)
            {
                if (alive == true)
                {
                    EnetAppUtils_print("PHY %d is alive\r\n", i);
                }
            }
            else
            {
                EnetAppUtils_print("Failed to get PHY %d alive status: %d\r\n", i, status);
            }
        }
    }

    return status;
}

static void EnetApp_deleteClock(EnetAppSysCfg_Obj *hEnetAppObj)
{

    hEnetAppObj->timerTaskShutDownFlag = true;

    ClockP_stop(&hEnetAppObj->timerObj);

    /* Post Timer Sem once to get the Periodic Tick task terminated */
    SemaphoreP_post(&hEnetAppObj->timerSemObj);

    do
    {
        ClockP_usleep(ClockP_ticksToUsec(1));
    } while (hEnetAppObj->timerTaskShutDownDoneFlag != true);

    ClockP_destruct(&hEnetAppObj->timerObj);
    SemaphoreP_destruct(&hEnetAppObj->timerSemObj);

}
void EnetApp_driverClose(Enet_Type enetType, uint32_t instId)
{
    Enet_IoctlPrms prms;
    int32_t status;
    uint32_t selfCoreId;
    uint32_t i;
    Enet_MacPort macPortList[ENET_MAC_PORT_NUM];
    uint8_t numMacPorts;
    Enet_Handle hEnet = Enet_getHandle(enetType, instId);

    EnetAppUtils_assert(Enet_isCpswFamily(enetType) == true);
    selfCoreId   = EnetSoc_getCoreId();
    EnetApp_getEnetInstMacInfo(enetType, instId, macPortList, &numMacPorts);
    EnetApp_deleteClock(&gEnetAppSysCfgObj);
    /* Disable host port */
	ENET_IOCTL_SET_NO_ARGS(&prms);
	ENET_IOCTL(hEnet,
               selfCoreId,
               ENET_HOSTPORT_IOCTL_DISABLE,
               &prms,
               status);
	if (status != ENET_SOK)
	{
	    EnetAppUtils_print("Failed to disable host port: %d\r\n", status);
	}

    for (i = 0U; i < numMacPorts; i++)
    {
        Enet_MacPort macPort = macPortList[i];

        ENET_IOCTL_SET_IN_ARGS(&prms, &macPort);
        ENET_IOCTL(hEnet,
                   selfCoreId,
                   ENET_PER_IOCTL_CLOSE_PORT_LINK,
                   &prms,
                   status);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("close() failed to close MAC port: %d\r\n", status);
        }
    }

    Enet_close(hEnet);
}




static uint32_t EnetApp_retrieveFreeTxPkts(EnetDma_TxChHandle hTxCh, EnetDma_PktQ *txPktInfoQ)
{
    EnetDma_PktQ txFreeQ;
    EnetDma_Pkt *pktInfo;
    uint32_t txFreeQCnt = 0U;
    int32_t status;

    EnetQueue_initQ(&txFreeQ);

    /* Retrieve any packets that may be free now */
    status = EnetDma_retrieveTxPktQ(hTxCh, &txFreeQ);
    if (status == ENET_SOK)
    {
        txFreeQCnt = EnetQueue_getQCount(&txFreeQ);

        pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        while (NULL != pktInfo)
        {
            EnetDma_checkPktState(&pktInfo->pktState,
                                    ENET_PKTSTATE_MODULE_APP,
                                    ENET_PKTSTATE_APP_WITH_DRIVER,
                                    ENET_PKTSTATE_APP_WITH_FREEQ);

            EnetQueue_enq(txPktInfoQ, &pktInfo->node);
            pktInfo = (EnetDma_Pkt *)EnetQueue_deq(&txFreeQ);
        }
    }
    else
    {
        EnetAppUtils_print("retrieveFreeTxPkts() failed to retrieve pkts: %d\r\n", status);
    }

    return txFreeQCnt;
}



static void EnetApp_openDmaChannels(EnetAppDmaSysCfg_Obj *dma,
                                    Enet_Type enetType,
                                    uint32_t instId,
                                    Enet_Handle hEnet,
                                    uint32_t coreKey,
                                    uint32_t coreId)
{
    EnetApp_openAllTxDmaChannels(dma,
                                 hEnet,
                                 coreKey,
                                 coreId);
    EnetApp_openAllRxDmaChannels(dma,
                                 hEnet,
                                 coreKey,
                                 coreId);
}



static void EnetApp_setPortTsEventPrms(CpswMacPort_TsEventCfg *tsPortEventCfg)
{
    memset(tsPortEventCfg, 0, sizeof(CpswMacPort_TsEventCfg));

    tsPortEventCfg->txAnnexFEn = true;
    tsPortEventCfg->rxAnnexFEn = true;
    tsPortEventCfg->txHostTsEn = true;
    /* Enable ts for SYNC, PDELAY_REQUEST, PDELAY_RESPONSE */
    tsPortEventCfg->messageType = 13;
    tsPortEventCfg->seqIdOffset = 30;
    tsPortEventCfg->domainOffset = 4;
}

int32_t EnetApp_enablePortTsEvent(Enet_Handle hEnet, uint32_t coreId, uint32_t macPort[], uint32_t numPorts)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    CpswMacPort_EnableTsEventInArgs enableTsEventInArgs;
    uint8_t i = 0U;

    EnetApp_setPortTsEventPrms(&(enableTsEventInArgs.tsEventCfg));
    for (i = 0U; i < numPorts; i++)
    {
        enableTsEventInArgs.macPort = (Enet_MacPort)macPort[i];
        ENET_IOCTL_SET_IN_ARGS(&prms, &enableTsEventInArgs);
        ENET_IOCTL(hEnet, coreId, CPSW_MACPORT_IOCTL_ENABLE_CPTS_EVENT, &prms, status);
        if (status != ENET_SOK)
        {
            EnetAppUtils_print("Enet_ioctl ENABLE_CPTS_EVENT failed %d port %d\n", status, macPort[i]);
        }
    }

    return status;
}

int32_t EnetApp_getRxTimeStamp(Enet_Handle hEnet, uint32_t coreId, EnetTimeSync_GetEthTimestampInArgs* inArgs, uint64_t *ts)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, inArgs, ts);
    ENET_IOCTL(hEnet, coreId, ENET_TIMESYNC_IOCTL_GET_ETH_RX_TIMESTAMP, &prms, status);

    return status;
}

int32_t EnetApp_setTimeStampComplete(Enet_Handle hEnet, uint32_t coreId)
{
    int32_t status = ENET_SOK;
    /* Not supported for CPSW, the completion ioctl is only vaild for ICSSG */
    /* Need this funtion definition to align CPSW and ICSSG for TSN libs */
    return status;
}


   


#define ENET_SYSCFG_DEFAULT_NUM_TX_PKT                                     (16U)
#define ENET_SYSCFG_DEFAULT_NUM_RX_PKT                                     (32U)



static void EnetAppUtils_setCommonRxChPrms(EnetCpdma_OpenRxChPrms *pRxChPrms)
{
    pRxChPrms->numRxPkts           = ENET_SYSCFG_DEFAULT_NUM_RX_PKT;
}

static void EnetAppUtils_setCommonTxChPrms(EnetCpdma_OpenTxChPrms *pTxChPrms)
{
    pTxChPrms->numTxPkts           = ENET_SYSCFG_DEFAULT_NUM_TX_PKT;
}

static int32_t EnetAppUtils_allocRxFlow(Enet_Handle hEnet,
                                        uint32_t coreKey,
                                        uint32_t coreId,
                                        uint32_t *rxFlowStartIdx,
                                        uint32_t *flowIdx)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    EnetRm_AllocRxFlowInArgs inArgs;
    EnetRm_AllocRxFlow rxFlowPrms;

    inArgs.coreKey = coreKey;
    inArgs.chIdx   = 0U;

    ENET_IOCTL_SET_INOUT_ARGS(&prms, &inArgs, &rxFlowPrms);
    ENET_IOCTL(hEnet,
               coreId,
               ENET_RM_IOCTL_ALLOC_RX_FLOW,
               &prms,
               status);

    if (status == ENET_SOK)
    {
        *rxFlowStartIdx = rxFlowPrms.startIdx;
        *flowIdx        = rxFlowPrms.flowIdx;
    }
    else
    {
        EnetAppUtils_print("EnetAppUtils_allocRxFlow() failed : %d\n", status);
    }

    return status;
}


static int32_t EnetAppUtils_allocTxCh(Enet_Handle hEnet,
                                      uint32_t coreKey,
                                      uint32_t coreId,
                                      uint32_t *txPSILThreadId)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;

    /* Allocate Tx Ch */
    ENET_IOCTL_SET_INOUT_ARGS(&prms, &coreKey, txPSILThreadId);
    ENET_IOCTL(hEnet,
               coreId,
               ENET_RM_IOCTL_ALLOC_TX_CH_PEERID,
               &prms,
               status);
    if (status != ENET_SOK)
    {
        *txPSILThreadId = ENET_RM_TXCHNUM_INVALID;
        EnetAppUtils_print("EnetAppUtils_allocTxCh() failed: %d\n", status);
    }

    return status;
}

static int32_t EnetAppUtils_freeRxFlow(Enet_Handle hEnet,
                                       uint32_t coreKey,
                                       uint32_t coreId,
                                       uint32_t rxFlowIdx)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    EnetRm_FreeRxFlowInArgs freeRxFlowInArgs;

    /*Free Rx Flow*/
    freeRxFlowInArgs.coreKey = coreKey;
    freeRxFlowInArgs.flowIdx = rxFlowIdx;
    freeRxFlowInArgs.chIdx   = 0U;

    ENET_IOCTL_SET_IN_ARGS(&prms, &freeRxFlowInArgs);
    ENET_IOCTL(hEnet,
               coreId,
               ENET_RM_IOCTL_FREE_RX_FLOW,
               &prms,
               status);

    return status;
}

static int32_t EnetAppUtils_freeTxCh(Enet_Handle hEnet,
                                     uint32_t coreKey,
                                     uint32_t coreId,
                                     uint32_t txChNum)
{
    int32_t status = ENET_SOK;
    Enet_IoctlPrms prms;
    EnetRm_FreeTxChInArgs freeTxChInArgs;

    /* Release Tx Ch */
    freeTxChInArgs.coreKey = coreKey;
    freeTxChInArgs.txChNum = txChNum;

    ENET_IOCTL_SET_IN_ARGS(&prms, &freeTxChInArgs);
    ENET_IOCTL(hEnet,
               coreId,
               ENET_RM_IOCTL_FREE_TX_CH_PEERID,
               &prms,
               status);

    return status;
}

static void EnetAppUtils_openTxCh(Enet_Handle hEnet,
                                  uint32_t coreKey,
                                  uint32_t coreId,
                                  uint32_t *pTxChNum,
                                  EnetDma_TxChHandle *pTxChHandle,
                                  EnetCpdma_OpenTxChPrms *pCpswTxChCfg)
{
    EnetDma_Handle hDma = Enet_getDmaHandle(hEnet);
    int32_t status;

    EnetAppUtils_assert(hDma != NULL);

    pCpswTxChCfg->hEnet = hEnet;
    status = EnetAppUtils_allocTxCh(hEnet,
                                    coreKey,
                                    coreId,
                                    pTxChNum);
    EnetAppUtils_assert(ENET_SOK == status);

    pCpswTxChCfg->chNum = *pTxChNum;

    *pTxChHandle = EnetDma_openTxCh(hDma, (void *)pCpswTxChCfg);
    EnetAppUtils_assert(NULL != *pTxChHandle);
}

static void EnetAppUtils_closeTxCh(Enet_Handle hEnet,
                                   uint32_t coreKey,
                                   uint32_t coreId,
                                   EnetDma_PktQ *pFqPktInfoQ,
                                   EnetDma_PktQ *pCqPktInfoQ,
                                   EnetDma_TxChHandle hTxChHandle,
                                   uint32_t txChNum)
{
    int32_t status;

    EnetQueue_initQ(pFqPktInfoQ);
    EnetQueue_initQ(pCqPktInfoQ);

    EnetDma_disableTxEvent(hTxChHandle);
    status = EnetDma_closeTxCh(hTxChHandle, pFqPktInfoQ, pCqPktInfoQ);
    EnetAppUtils_assert(ENET_SOK == status);

    status = EnetAppUtils_freeTxCh(hEnet,
                                   coreKey,
                                   coreId,
                                   txChNum);
    EnetAppUtils_assert(ENET_SOK == status);
}

static void EnetAppUtils_openRxCh(Enet_Handle hEnet,
                                  uint32_t coreKey,
                                  uint32_t coreId,
                                  uint32_t *pRxChNum,
                                  EnetDma_RxChHandle *pRxChHandle,
                                  EnetCpdma_OpenRxChPrms *pCpswRxChCfg,
                                  uint32_t allocMacAddrCnt,
                                  uint8_t macAddr[ENET_MAX_NUM_MAC_PER_PHER][ENET_MAC_ADDR_LEN])
{
    EnetDma_Handle hDma = Enet_getDmaHandle(hEnet);
    int32_t status;
    uint32_t rxFlowStartIdx;
    uint32_t rxFlowIdx;

    EnetAppUtils_assert(hDma != NULL);

    pCpswRxChCfg->hEnet = hEnet;
    status = EnetAppUtils_allocRxFlow(hEnet,
                                      coreKey,
                                      coreId,
                                      &rxFlowStartIdx,
                                      &rxFlowIdx);
    EnetAppUtils_assert(status == ENET_SOK);
    pCpswRxChCfg->chNum = rxFlowIdx;
	*pRxChNum = rxFlowIdx;

    *pRxChHandle = EnetDma_openRxCh(hDma, (void *)pCpswRxChCfg);
    EnetAppUtils_assert(NULL != *pRxChHandle);

    for (uint32_t i = 0; i < allocMacAddrCnt; i++)
    {
        status = EnetAppUtils_allocMac(hEnet,
                                       coreKey,
                                       coreId,
                                       macAddr[i]);
        EnetAppUtils_assert(ENET_SOK == status);
        EnetAppUtils_addHostPortEntry(hEnet, coreId, macAddr[i]);
    }
}

static void EnetAppUtils_closeRxCh(Enet_Handle hEnet,
                                   uint32_t coreKey,
                                   uint32_t coreId,
                                   EnetDma_PktQ *pFqPktInfoQ,
                                   EnetDma_PktQ *pCqPktInfoQ,
                                   EnetDma_RxChHandle hRxChHandle,
                                   uint32_t rxChNum,
                                   uint32_t freeHostMacAddrCount,
                                   uint8_t macAddr[ENET_MAX_NUM_MAC_PER_PHER][ENET_MAC_ADDR_LEN])
{
    int32_t status;

    EnetQueue_initQ(pFqPktInfoQ);
    EnetQueue_initQ(pCqPktInfoQ);

    status = EnetDma_closeRxCh(hRxChHandle, pFqPktInfoQ, pCqPktInfoQ);
    EnetAppUtils_assert(ENET_SOK == status);
    for (uint32_t macAdrIdx = 0;  macAdrIdx < freeHostMacAddrCount; macAdrIdx++)
    {
        EnetAppUtils_delAddrEntry(hEnet, coreId, macAddr[macAdrIdx]);
        EnetAppUtils_freeMac(hEnet,
                             coreKey,
                             coreId,
                             macAddr[macAdrIdx]);
    }
}


static void EnetApp_openTxDma(EnetAppTxDmaSysCfg_Obj *tx,
                              uint32_t numTxPkts,
                              Enet_Handle hEnet,
                              uint32_t coreKey,
                              uint32_t coreId)
{
    EnetCpdma_OpenTxChPrms txChCfg;

    memset(tx, 0, sizeof(*tx));
    /* Open the TX channel for Regular traffic */
    EnetDma_initTxChParams(&txChCfg);

    txChCfg.cbArg    = NULL;
    txChCfg.notifyCb = NULL;

    EnetAppUtils_setCommonTxChPrms(&txChCfg);

    txChCfg.numTxPkts = numTxPkts;
    EnetAppUtils_openTxCh(hEnet,
                          coreKey,
                          coreId,
                          &tx->txChNum,
                          &tx->hTxCh,
                          &txChCfg);
    return;
}

static void EnetApp_openRxDma(EnetAppRxDmaSysCfg_Obj *rx,
                              uint32_t numRxPkts,
                              Enet_Handle hEnet,
                              uint32_t coreKey,
                              uint32_t coreId,
                              uint32_t allocMacAddrCnt)
{
    EnetCpdma_OpenRxChPrms rxChCfg;

    memset(rx, 0, sizeof(*rx));
    /* Open the TX channel for Regular traffic */
    EnetDma_initRxChParams(&rxChCfg);

    rxChCfg.cbArg    = NULL;
    rxChCfg.notifyCb = NULL;

    EnetAppUtils_setCommonRxChPrms(&rxChCfg);

    rxChCfg.numRxPkts = numRxPkts;
    EnetAppUtils_openRxCh(hEnet,
                          coreKey,
                          coreId,
                          &rx->rxChNum,
                          &rx->hRxCh,
                          &rxChCfg,
                          allocMacAddrCnt,
                          rx->macAddr);

    rx->numValidMacAddress = allocMacAddrCnt;
    return;
}

static void EnetApp_openAllTxDmaChannels(EnetAppDmaSysCfg_Obj *dma,
                                         Enet_Handle hEnet,
                                         uint32_t coreKey,
                                         uint32_t coreId)
{
    uint32_t txNumPkts[ENET_SYSCFG_TX_CHANNELS_NUM] =
                           {
                            16
                           };

    uint32_t i;

    for (i = 0; i < ENET_SYSCFG_TX_CHANNELS_NUM;i++)
    {
        EnetApp_openTxDma(&dma->tx[i], txNumPkts[i], hEnet, coreKey, coreId);
    }
}

static void EnetApp_openAllRxDmaChannels(EnetAppDmaSysCfg_Obj *dma,
                                         Enet_Handle hEnet,
                                         uint32_t coreKey,
                                         uint32_t coreId)
{
    const uint32_t rxNumPkts[ENET_SYSCFG_RX_FLOWS_NUM] =
                           {
                            32
                           };
    const uint32_t allocMacAddrCnt[ENET_SYSCFG_RX_FLOWS_NUM] =
                           {
                            1
                           };


    for (uint32_t i = 0; i < ENET_SYSCFG_RX_FLOWS_NUM;i++)
    {
        EnetApp_openRxDma(&dma->rx[i], rxNumPkts[i], hEnet, coreKey, coreId, allocMacAddrCnt[i]);
    }
}

void EnetApp_closeTxDma(uint32_t enetTxDmaChId,
                        Enet_Handle hEnet,
                        uint32_t coreKey,
                        uint32_t coreId,
                        EnetDma_PktQ *fqPktInfoQ,
                        EnetDma_PktQ *cqPktInfoQ)
{
    EnetAppTxDmaSysCfg_Obj *tx;

    EnetAppUtils_assert(enetTxDmaChId < ENET_ARRAYSIZE(gEnetAppSysCfgObj.dma.tx));
    tx = &gEnetAppSysCfgObj.dma.tx[enetTxDmaChId];
    EnetQueue_initQ(fqPktInfoQ);
    EnetQueue_initQ(cqPktInfoQ);
    EnetApp_retrieveFreeTxPkts(tx->hTxCh, cqPktInfoQ);
    EnetAppUtils_closeTxCh(hEnet,
                           coreKey,
                           coreId,
                           fqPktInfoQ,
                           cqPktInfoQ,
                           tx->hTxCh,
                           tx->txChNum);

    memset(tx, 0, sizeof(*tx));
}

void EnetApp_closeRxDma(uint32_t enetRxDmaChId,
                        Enet_Handle hEnet,
                        uint32_t coreKey,
                        uint32_t coreId,
                        EnetDma_PktQ *fqPktInfoQ,
                        EnetDma_PktQ *cqPktInfoQ)
{
    EnetAppRxDmaSysCfg_Obj *rx;

    EnetAppUtils_assert(enetRxDmaChId < ENET_ARRAYSIZE(gEnetAppSysCfgObj.dma.rx));
    rx = &gEnetAppSysCfgObj.dma.rx[enetRxDmaChId];
    /* Close RX channel */
    EnetQueue_initQ(fqPktInfoQ);
    EnetQueue_initQ(cqPktInfoQ);

	EnetAppUtils_closeRxCh(hEnet,
                           coreKey,
                           coreId,
                           fqPktInfoQ,
                           cqPktInfoQ,
                           rx->hRxCh,
                           rx->rxChNum,
                           rx->numValidMacAddress,
                           rx->macAddr);

    EnetAppSoc_releaseMacAddrList(rx->macAddr, rx->numValidMacAddress);
    memset(rx, 0, sizeof(*rx));
}

void EnetApp_getTxDmaHandle(uint32_t enetTxDmaChId,
                            const EnetApp_GetDmaHandleInArgs *inArgs,
                            EnetApp_GetTxDmaHandleOutArgs *outArgs)
{
    int32_t status;
    EnetAppTxDmaSysCfg_Obj *tx;
    uint32_t txNumPkts[ENET_SYSCFG_TX_CHANNELS_NUM] =
                           {
                            16
                           };

    EnetAppUtils_assert(enetTxDmaChId < ENET_ARRAYSIZE(gEnetAppSysCfgObj.dma.tx));
    tx = &gEnetAppSysCfgObj.dma.tx[enetTxDmaChId];

    EnetAppUtils_assert(tx->hTxCh != NULL);
    status = EnetDma_registerTxEventCb(tx->hTxCh, inArgs->notifyCb, inArgs->cbArg);
    EnetAppUtils_assert(status == ENET_SOK);

    outArgs->hTxCh = tx->hTxCh;
    outArgs->txChNum = tx->txChNum;
    EnetAppUtils_assert(enetTxDmaChId < ENET_ARRAYSIZE(txNumPkts));
    outArgs->maxNumTxPkts = txNumPkts[enetTxDmaChId];
    return;
}

void EnetApp_getMacAddress(uint32_t enetRxDmaChId,
                            EnetApp_GetMacAddrOutArgs *outArgs)
{

    EnetAppUtils_assert(enetRxDmaChId < ENET_ARRAYSIZE(gEnetAppSysCfgObj.dma.rx));
    EnetAppRxDmaSysCfg_Obj* rx = &gEnetAppSysCfgObj.dma.rx[enetRxDmaChId];

    outArgs->macAddressCnt = rx->numValidMacAddress;
    EnetAppUtils_assert(outArgs->macAddressCnt <= ENET_MAX_NUM_MAC_PER_PHER);
    for (uint32_t i = 0; i < outArgs->macAddressCnt; i++)
    {
        EnetUtils_copyMacAddr(outArgs->macAddr[i], rx->macAddr[i]);
    }

}

void EnetApp_getRxDmaHandle(uint32_t enetRxDmaChId,
                            const EnetApp_GetDmaHandleInArgs *inArgs,
                            EnetApp_GetRxDmaHandleOutArgs *outArgs)
{
    int32_t status;
    EnetAppRxDmaSysCfg_Obj *rx;
    uint32_t rxNumPkts[ENET_SYSCFG_RX_FLOWS_NUM] =
                           {
                            32
                           };

    EnetAppUtils_assert(enetRxDmaChId < ENET_ARRAYSIZE(gEnetAppSysCfgObj.dma.rx));
    rx = &gEnetAppSysCfgObj.dma.rx[enetRxDmaChId];

    EnetAppUtils_assert(rx->hRxCh != NULL);
    status = EnetDma_registerRxEventCb(rx->hRxCh, inArgs->notifyCb, inArgs->cbArg);
    EnetAppUtils_assert(status == ENET_SOK);

    outArgs->hRxCh = rx->hRxCh;
    outArgs->rxChNum = rx->rxChNum;
    outArgs->numValidMacAddress = rx->numValidMacAddress;
    for (uint32_t i = 0; i < outArgs->numValidMacAddress; i++)
    {
        EnetUtils_copyMacAddr(outArgs->macAddr[i], rx->macAddr[i]);
    }

    EnetAppUtils_assert(enetRxDmaChId < ENET_ARRAYSIZE(rxNumPkts));
    outArgs->maxNumRxPkts = rxNumPkts[enetRxDmaChId];
    return;
}





#include <string.h>

#include <enet.h>
#include <include/core/enet_utils.h>

#include <include/core/enet_dma.h>
#include <include/per/cpsw.h>

#include "enet_appmemutils.h"
#include "enet_appmemutils_cfg.h"
#include "enet_apputils.h"




static void EnetApp_initEnetLinkCbPrms(Cpsw_Cfg *cpswCfg)
{
    cpswCfg->mdioLinkStateChangeCb     = NULL;
    cpswCfg->mdioLinkStateChangeCbArg  = NULL;

    cpswCfg->portLinkStatusChangeCb    = NULL;
    cpswCfg->portLinkStatusChangeCbArg = NULL;
}

static const CpswAle_Cfg enetAppCpswAleCfg =
{
    .modeFlags = (CPSW_ALE_CFG_MODULE_EN),
    .policerGlobalCfg =
    {
        .policingEn         = false,
        .yellowDropEn       = false,
        .redDropEn          = false,
        .yellowThresh       = CPSW_ALE_POLICER_YELLOWTHRESH_DROP_PERCENT_100,
        .policerNoMatchMode = CPSW_ALE_POLICER_NOMATCH_MODE_GREEN,
        .noMatchPolicer     = {
                                  .peakRateInBitsPerSec   = 0,
                                  .commitRateInBitsPerSec = 0,
                              }
    },
    .agingCfg =
    {
        .autoAgingEn        = true,
        .agingPeriodInMs    = 1000,
    },
    .vlanCfg =
    {
        .aleVlanAwareMode   = true,
        .cpswVlanAwareMode  = false,
        .autoLearnWithVlan  = false,
        .unknownVlanNoLearn = false,
        .unknownForceUntaggedEgressMask = (0),
        .unknownRegMcastFloodMask       = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
        .unknownUnregMcastFloodMask     = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
        .unknownVlanMemberListMask      = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
    },
    .nwSecCfg =
    {
        .hostOuiNoMatchDeny  = false,
        .vid0ModeEn          = true,
        .malformedPktCfg     = {
                                   .srcMcastDropDis = false,
                                   .badLenPktDropEn = false,
                               },
        .ipPktCfg            = {
                                   .dfltNoFragEn          = false,
                                   .dfltNxtHdrWhitelistEn = false,
                                   .ipNxtHdrWhitelistCnt  =  0,
                                   .ipNxtHdrWhitelist     = {
                                                              
                                                            },
                               },


        .macAuthCfg          = {
                                   .authModeEn           = false,
                                   .macAuthDisMask       = (0 | CPSW_ALE_HOST_PORT_MASK),
                               },
    },
    .portCfg =
    {
        [CPSW_ALE_HOST_PORT_NUM] =
        {
            .learningCfg =
            {
                .noLearn         = false,
                .noSaUpdateEn    = false,
            },
            .vlanCfg =
            {
                .vidIngressCheck = false,
                .dropUntagged    = false,
                .dropDualVlan    = false,
                .dropDoubleVlan  = false,
            },
            .macModeCfg =
            {
                .macOnlyCafEn    = false,
                .macOnlyEn       = false,
            },
            .pvidCfg =
            {
                .vlanIdInfo      =
                {
                    .vlanId   = 0,
                    .tagType  = ENET_VLAN_TAG_TYPE_INNER,
                },
                .vlanMemberList          = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
                .unregMcastFloodMask     = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
                .regMcastFloodMask       = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
                .forceUntaggedEgressMask = (0),
                .noLearnMask             = (0),
                .vidIngressCheck         = false,
                .limitIPNxtHdr           = false,
                .disallowIPFrag          = false,
            },
        },
        [CPSW_ALE_MACPORT_TO_ALEPORT(ENET_MAC_PORT_1)] =
        {
            .learningCfg =
            {
                .noLearn         = false,
                .noSaUpdateEn    = false,
            },
            .vlanCfg =
            {
                .vidIngressCheck = false,
                .dropUntagged    = false,
                .dropDualVlan    = false,
                .dropDoubleVlan  = false,
            },
            .macModeCfg =
            {
                .macOnlyCafEn    = false,
                .macOnlyEn       = false,
            },
            .pvidCfg =
            {
                .vlanIdInfo      =
                {
                    .vlanId   = 0,
                    .tagType  = ENET_VLAN_TAG_TYPE_INNER,
                },
                .vlanMemberList          = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
                .unregMcastFloodMask     = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
                .regMcastFloodMask       = (0 | CPSW_ALE_HOST_PORT_MASK | CPSW_ALE_MACPORT_TO_PORTMASK(ENET_MAC_PORT_1)),
                .forceUntaggedEgressMask = (0),
                .noLearnMask             = (0),
                .vidIngressCheck         = false,
                .limitIPNxtHdr           = false,
                .disallowIPFrag          = false,
            },
        },
    }

};

static const Mdio_Cfg enetAppCpswMdioCfg =
{
    .mode               = MDIO_MODE_STATE_CHANGE_MON,
    .mdioBusFreqHz      = 2200000,
    .phyStatePollFreqHz = 22000,
    .pollEnMask         = -1,
    .c45EnMask          = 0,
    .isMaster           = true,
    .disableStateMachineOnInit = false,
};


static const CpswCpts_Cfg enetAppCpswCptsCfg =
{
    .hostRxTsEn     = false,
    .tsCompPolarity = true,
    .tsRxEventsDis  = false,
    .tsGenfClrEn    = false,
    .cptsRftClkFreq = CPSW_CPTS_RFTCLK_FREQ_200MHZ,
};

static const CpswHostPort_Cfg enetAppCpswHostPortCfg =
{
    .crcType           = ENET_CRC_ETHERNET,
    .removeCrc         = false,
    .padShortPacket    = true,
    .passCrcErrors     = false,
    .rxMtu             = 1518,
    .passPriorityTaggedUnchanged = false,
    .rxCsumOffloadEn   = false,
    .txCsumOffloadEn   = false,
    .rxVlanRemapEn     = false,
    .rxDscpIPv4RemapEn = false,
    .rxDscpIPv6RemapEn = false,
    .vlanCfg           =
    {
        .portPri = 0,
        .portCfi = false,
        .portVID = 0,
    },
    .rxPriorityType    = ENET_INGRESS_PRI_TYPE_FIXED,
    .txPriorityType    = ENET_EGRESS_PRI_TYPE_FIXED,
};

static const CpswMacPort_Cfg enetAppCpswMacPortCfg[] =
{
    [ENET_MAC_PORT_1] =
    {
        .loopbackEn = false,
        .crcType    = ENET_CRC_ETHERNET,
        .rxMtu      = 1518,
        .passPriorityTaggedUnchanged = false,
        .vlanCfg =
        {
            .portPri = 0,
            .portCfi = false,
            .portVID = 0,
        },
        .txPriorityType = ENET_EGRESS_PRI_TYPE_FIXED,
        .sgmiiMode      = ENET_MAC_SGMIIMODE_INVALID, // INVALID as SGMII is not supported in MCU+ devices

    },
};

static void EnetApp_initAleConfig(CpswAle_Cfg *pAleCfg)
{
    *pAleCfg = enetAppCpswAleCfg;
}

static void EnetApp_initMdioConfig(Mdio_Cfg *pMdioCfg)
{
    *pMdioCfg = enetAppCpswMdioCfg;
}

static void EnetApp_initCptsConfig(CpswCpts_Cfg *pCptsCfg)
{
    *pCptsCfg = enetAppCpswCptsCfg;
}

static void EnetApp_initHostPortConfig(CpswHostPort_Cfg *pHostPortCfg)
{
    *pHostPortCfg = enetAppCpswHostPortCfg;
}

static void EnetApp_initCpdmaConfig(EnetCpdma_Cfg *pCpdmaCfg)
{
    pCpdmaCfg->enHostRxTsFlag = false;

}

static void EnetApp_getCpswInitCfg(Enet_Type enetType,
                                   uint32_t instId,
                                   Cpsw_Cfg *cpswCfg)
{
    cpswCfg->vlanCfg.vlanAware          = false;
    cpswCfg->hostPortCfg.removeCrc      = true;
    cpswCfg->hostPortCfg.padShortPacket = true;
    cpswCfg->hostPortCfg.passCrcErrors  = true;
    EnetApp_initEnetLinkCbPrms(cpswCfg);
    EnetApp_initAleConfig(&cpswCfg->aleCfg);
    EnetApp_initMdioConfig(&cpswCfg->mdioCfg);
    EnetApp_initCptsConfig(&cpswCfg->cptsCfg);
    EnetApp_initHostPortConfig(&cpswCfg->hostPortCfg);
    EnetApp_initCpdmaConfig((EnetCpdma_Cfg *)cpswCfg->dmaCfg);
}

static void EnetApp_getMacPortInitConfig(CpswMacPort_Cfg *pMacPortCfg, const Enet_MacPort portIdx)
{
    EnetAppUtils_assert(portIdx < ENET_ARRAYSIZE(enetAppCpswMacPortCfg));
    *pMacPortCfg = enetAppCpswMacPortCfg[portIdx];
}



