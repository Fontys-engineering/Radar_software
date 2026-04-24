/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */

/* ========================================================================== */
/*                             Include Files                                  */
/* ========================================================================== */
#include <string.h>
#include <stdio.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/TaskP.h>
#include <kernel/dpl/ClockP.h>
#include "lwip/opt.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "enet_apputils.h"

#ifdef SOC_AWR294X
#include <ti/demo/awr294x/mmw/mss/mmw_mss.h>
#endif

#ifdef SOC_AWR2X44P
#include <mmw_mss.h>
#endif

/* ========================================================================== */
/*                           Macros & Typedefs                                */
/* ========================================================================== */

#define HOST_SERVER_PORT  (7)

/* ========================================================================== */
/*                         Structure Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                          Function Declarations                             */
/* ========================================================================== */

/* ========================================================================== */
/*                            Global Variables                                */
/* ========================================================================== */

SemaphoreP_Object objDataSemaphoreHandle;
extern MmwDemo_MSS_MCB gMmwMssMCB;
extern MmwDemo_enetStreamObjData gEnetStreamObjData;

/* ========================================================================== */
/*                          Function Definitions                              */
/* ========================================================================== */

static void AppTcp_simpleclient(void *pArg)
{
    struct netconn *pConn = NULL;
    err_t err = ERR_OK, connectError = ERR_OK;

    const enum netconn_type connType = NETCONN_TCP;

    /* Create a new connection identifier. */
    pConn = netconn_new(connType);

    if (pConn != NULL)
    {
        SemaphoreP_pend(&gMmwMssMCB.enetCfg.EnetCfgDoneSemHandle, SystemP_WAIT_FOREVER);

        connectError = netconn_connect(pConn, &gMmwMssMCB.enetCfg.remoteIp, HOST_SERVER_PORT);

        if (connectError != ERR_OK)
        {
            netconn_close(pConn);
            DebugP_log("Connection with the server isn't established\r\n");
        }
        else
        {
            DebugP_log("Connection with the server is established\r\n");
            netconn_set_sendtimeout(pConn, 5);   // 5 ms timeout

        while(1)
        {
            SemaphoreP_pend(&objDataSemaphoreHandle, SystemP_WAIT_FOREVER);

            uint32_t numObj = gEnetStreamObjData.numObj;
            uint32_t len = sizeof(DPIF_PointCloudCartesian) * numObj;
            if (len > 4000)   // tune this threshold ADDED
            {
                    gEnetStreamObjData.ready = 0;
                    continue;   // skip this frame entirely
            }

            if (numObj == 0)
            {
                gEnetStreamObjData.ready = 0;
                continue;
            }

            err = netconn_write(pConn,
                                &(gEnetStreamObjData.numObj),
                                sizeof(uint32_t) + 2,
                                NETCONN_COPY | NETCONN_DONTBLOCK); //DONTBLOCK ADDED

            if (err != ERR_OK)
            {
                DebugP_log("write header failed: %s\n", lwip_strerr(err));
                gEnetStreamObjData.ready = 0;
                ClockP_usleep(1000);   // ADDED
                continue;
            }

            err = netconn_write(pConn,
                                gEnetStreamObjData.objData,
                                len,
                                NETCONN_COPY | NETCONN_DONTBLOCK); //DONTBLOCK ADDED

            if (err != ERR_OK)
            {
                DebugP_log("write data failed: %s\n", lwip_strerr(err));
                gEnetStreamObjData.ready = 0;
                ClockP_usleep(1000);   // ADDED
                continue;
            }

            // ADDED
            gEnetStreamObjData.ready = 0;
        }
        }
    }
}

void AppTcp_startClient(void)
{
    int32_t status = SystemP_SUCCESS;

    status = SemaphoreP_constructBinary(&objDataSemaphoreHandle, 0);
    DebugP_assert(SystemP_SUCCESS == status);

    sys_thread_new("tcpinit_thread", AppTcp_simpleclient, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}
