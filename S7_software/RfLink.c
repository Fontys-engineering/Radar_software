#include "RfLink.h"

#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include <drivers/mailbox.h>
#include <drivers/hw_include/cslr_soc.h>

#include <kernel/dpl/ClockP.h>
#include <kernel/dpl/DebugP.h>
#include <kernel/dpl/SemaphoreP.h>
#include <kernel/dpl/SystemP.h>

static uint32_t mailbox_dummy_handle;
static RL_P_EVENT_HANDLER g_CommInterruptFunc;

#define RFLINK_MAX_MUTEX 4
#define RFLINK_MAX_SEM   6

typedef struct
{
    bool inUse;
    SemaphoreP_Object obj;
} RfLink_SemObj;

static RfLink_SemObj g_mutexPool[RFLINK_MAX_MUTEX];
static RfLink_SemObj g_semPool[RFLINK_MAX_SEM];

static rlClientCbs_t g_clientCbs;

/* Deferred spawn support for NoRTOS (avoid calling mmWaveLink from ISR) */
#define RFLINK_MAX_SPAWN 4
static volatile RL_P_OSI_SPAWN_ENTRY g_spawnQueue[RFLINK_MAX_SPAWN];
static volatile const void *g_spawnArgs[RFLINK_MAX_SPAWN];
static volatile uint32_t g_spawnHead = 0;
static volatile uint32_t g_spawnTail = 0;

static uint16_t RfLink_crc16Ccitt(const uint8_t *data, uint32_t len)
{
    /* TI radar protocol uses CRC16-CCITT with init=0xFFFF, poly=0x1021 */
    uint16_t crc = 0xFFFFU;

    for (uint32_t i = 0; i < len; i++)
    {
        crc ^= (uint16_t)((uint16_t)data[i] << 8U);
        for (uint32_t bit = 0; bit < 8U; bit++)
        {
            if ((crc & 0x8000U) != 0U)
            {
                crc = (uint16_t)((uint16_t)(crc << 1U) ^ 0x1021U);
            }
            else
            {
                crc = (uint16_t)(crc << 1U);
            }
        }
    }

    return crc;
}

static uint32_t RfLink_crc32(const uint8_t *data, uint32_t len)
{
    uint32_t crc = 0xFFFFFFFFu;

    for (uint32_t i = 0; i < len; i++)
    {
        crc ^= (uint32_t)data[i];
        for (uint32_t bit = 0; bit < 8U; bit++)
        {
            uint32_t mask = (uint32_t)-(int32_t)(crc & 1U);
            crc = (crc >> 1U) ^ (0xEDB88320u & mask);
        }
    }

    return ~crc;
}

static rlInt32_t RfLink_computeCRC(rlUInt8_t *data, rlUInt32_t dataLen, rlUInt8_t crcType, rlUInt8_t *crc)
{
    if ((data == NULL) || (crc == NULL))
    {
        return -1;
    }

    if (crcType == RL_CRC_TYPE_16BIT_CCITT)
    {
        uint16_t sig = RfLink_crc16Ccitt((const uint8_t *)data, (uint32_t)dataLen);
        crc[0] = (rlUInt8_t)(sig & 0xFFU);
        crc[1] = (rlUInt8_t)((sig >> 8U) & 0xFFU);
        return 0;
    }

    if (crcType == RL_CRC_TYPE_32BIT)
    {
        uint32_t sig = RfLink_crc32((const uint8_t *)data, (uint32_t)dataLen);
        crc[0] = (rlUInt8_t)(sig & 0xFFU);
        crc[1] = (rlUInt8_t)((sig >> 8U) & 0xFFU);
        crc[2] = (rlUInt8_t)((sig >> 16U) & 0xFFU);
        crc[3] = (rlUInt8_t)((sig >> 24U) & 0xFFU);
        return 0;
    }

    return -1;
}

static void RfLink_mboxCallback(uint32_t remoteCoreId, void *args)
{
    (void)remoteCoreId;
    (void)args;

    if (g_CommInterruptFunc != NULL)
    {
        g_CommInterruptFunc(0, NULL);
    }
}

static rlComIfHdl_t RfLink_mboxOpen(rlUInt8_t deviceIndex, uint32_t flags)
{
    (void)deviceIndex;
    (void)flags;

    Mailbox_setReadCallback(RfLink_mboxCallback, NULL);
    return (rlComIfHdl_t)&mailbox_dummy_handle;
}

static int32_t RfLink_mboxClose(rlComIfHdl_t fd)
{
    (void)fd;
    return 0;
}

static int32_t RfLink_mboxRead(rlComIfHdl_t fd, uint8_t *pBuff, uint16_t len)
{
    (void)fd;

    int32_t status = Mailbox_read(CSL_CORE_ID_RSS_R4, pBuff, len, SystemP_WAIT_FOREVER);
    if (status != SystemP_SUCCESS)
    {
        DebugP_logInfo("Mailbox Read Error\n");
        return -1;
    }

    return (int32_t)len;
}

static int32_t RfLink_mboxWrite(rlComIfHdl_t fd, uint8_t *pBuff, uint16_t len)
{
    (void)fd;

    int32_t status = Mailbox_write(CSL_CORE_ID_RSS_R4, pBuff, len, SystemP_WAIT_FOREVER);
    if (status != SystemP_SUCCESS)
    {
        DebugP_logInfo("Mailbox Write Error\n");
        return -1;
    }

    return (int32_t)len;
}

static rlInt32_t RfLink_mutexCreate(rlOsiMutexHdl_t *mutexHdl, rlInt8_t *name)
{
    (void)name;

    if (mutexHdl == NULL)
    {
        return -1;
    }

    for (uint32_t i = 0; i < RFLINK_MAX_MUTEX; i++)
    {
        if (!g_mutexPool[i].inUse)
        {
            g_mutexPool[i].inUse = true;
            SemaphoreP_constructMutex(&g_mutexPool[i].obj);
            *mutexHdl = (rlOsiMutexHdl_t)&g_mutexPool[i].obj;
            return 0;
        }
    }

    return -1;
}

static rlInt32_t RfLink_mutexLock(rlOsiMutexHdl_t *mutexHdl, rlOsiTime_t timeout)
{
    (void)timeout;
    if ((mutexHdl == NULL) || (*mutexHdl == NULL))
    {
        return -1;
    }

    int32_t status = SemaphoreP_pend((SemaphoreP_Object *)(*mutexHdl), SystemP_WAIT_FOREVER);
    return (status == SystemP_SUCCESS) ? 0 : -1;
}

static rlInt32_t RfLink_mutexUnlock(rlOsiMutexHdl_t *mutexHdl)
{
    if ((mutexHdl == NULL) || (*mutexHdl == NULL))
    {
        return -1;
    }

    SemaphoreP_post((SemaphoreP_Object *)(*mutexHdl));
    return 0;
}

static rlInt32_t RfLink_mutexDelete(rlOsiMutexHdl_t *mutexHdl)
{
    if ((mutexHdl == NULL) || (*mutexHdl == NULL))
    {
        return -1;
    }

    for (uint32_t i = 0; i < RFLINK_MAX_MUTEX; i++)
    {
        if ((SemaphoreP_Object *)(*mutexHdl) == &g_mutexPool[i].obj)
        {
            SemaphoreP_destruct(&g_mutexPool[i].obj);
            g_mutexPool[i].inUse = false;
            *mutexHdl = (rlOsiMutexHdl_t)NULL;
            return 0;
        }
    }

    return -1;
}

static rlInt32_t RfLink_semCreate(rlOsiSemHdl_t *semHdl, rlInt8_t *name)
{
    (void)name;

    if (semHdl == NULL)
    {
        return -1;
    }

    for (uint32_t i = 0; i < RFLINK_MAX_SEM; i++)
    {
        if (!g_semPool[i].inUse)
        {
            g_semPool[i].inUse = true;
            SemaphoreP_constructBinary(&g_semPool[i].obj, 0U);
            *semHdl = (rlOsiSemHdl_t)&g_semPool[i].obj;
            return 0;
        }
    }

    return -1;
}

static rlInt32_t RfLink_semWait(rlOsiSemHdl_t *semHdl, rlOsiTime_t timeout)
{
    if ((semHdl == NULL) || (*semHdl == NULL))
    {
        return -1;
    }

    uint32_t waitTicks = SystemP_WAIT_FOREVER;
    (void)timeout;

    int32_t status = SemaphoreP_pend((SemaphoreP_Object *)(*semHdl), waitTicks);
    return (status == SystemP_SUCCESS) ? 0 : -1;
}

static rlInt32_t RfLink_semSignal(rlOsiSemHdl_t *semHdl)
{
    if ((semHdl == NULL) || (*semHdl == NULL))
    {
        return -1;
    }

    SemaphoreP_post((SemaphoreP_Object *)(*semHdl));
    return 0;
}

static rlInt32_t RfLink_semDelete(rlOsiSemHdl_t *semHdl)
{
    if ((semHdl == NULL) || (*semHdl == NULL))
    {
        return -1;
    }

    for (uint32_t i = 0; i < RFLINK_MAX_SEM; i++)
    {
        if ((SemaphoreP_Object *)(*semHdl) == &g_semPool[i].obj)
        {
            SemaphoreP_destruct(&g_semPool[i].obj);
            g_semPool[i].inUse = false;
            *semHdl = (rlOsiSemHdl_t)NULL;
            return 0;
        }
    }

    return -1;
}

static rlInt32_t RfLink_spawn(RL_P_OSI_SPAWN_ENTRY pEntry, const void *pValue, uint32_t flags)
{
    (void)flags;

    if (pEntry == NULL)
    {
        return -1;
    }

    /* Queue the spawn request for deferred execution (called from ISR context) */
    uint32_t next = (g_spawnHead + 1U) % RFLINK_MAX_SPAWN;
    if (next == g_spawnTail)
    {
        /* Queue full - should not happen in normal operation */
        return -1;
    }

    g_spawnQueue[g_spawnHead] = pEntry;
    g_spawnArgs[g_spawnHead] = pValue;
    g_spawnHead = next;

    return 0;
}

static rlInt32_t RfLink_delay(rlUInt32_t delayMilliSec)
{
    ClockP_usleep((uint32_t)delayMilliSec * 1000U);
    return 0;
}

static rlInt32_t RfLink_enableDevice(rlUInt8_t deviceIndex)
{
    (void)deviceIndex;
    return 0;
}

static rlInt32_t RfLink_disableDevice(rlUInt8_t deviceIndex)
{
    (void)deviceIndex;
    return 0;
}

static void RfLink_maskHostIrq(rlComIfHdl_t fd)
{
    (void)fd;
}

static void RfLink_unmaskHostIrq(rlComIfHdl_t fd)
{
    (void)fd;
    Mailbox_readDone(CSL_CORE_ID_RSS_R4);
}

static rlInt32_t RfLink_registerInterruptHandler(rlUInt8_t deviceIndex, RL_P_EVENT_HANDLER pHandler, void *pValue)
{
    (void)deviceIndex;
    (void)pValue;

    g_CommInterruptFunc = pHandler;
    return 0;
}

static void RfLink_asyncEventHandler(rlUInt8_t devIndex, rlUInt16_t sbId, rlUInt16_t sbLen, rlUInt8_t *payload)
{
    (void)devIndex;
    (void)payload;

    DebugP_logInfo("mmWaveLink async event: sbId=%u sbLen=%u\n", sbId, sbLen);
}

int32_t RfLink_init(void)
{
    g_CommInterruptFunc = NULL;

    for (uint32_t i = 0; i < RFLINK_MAX_MUTEX; i++)
    {
        g_mutexPool[i].inUse = false;
    }
    for (uint32_t i = 0; i < RFLINK_MAX_SEM; i++)
    {
        g_semPool[i].inUse = false;
    }

    (void)memset(&g_clientCbs, 0, sizeof(g_clientCbs));

    g_clientCbs.ackTimeout = 1000U;
    g_clientCbs.crcType    = RL_CRC_TYPE_16BIT_CCITT;
    g_clientCbs.platform   = RL_PLATFORM_MSS;
    g_clientCbs.arDevType  = RL_AR_DEVICETYPE_294X;

    g_clientCbs.comIfCb.rlComIfOpen  = RfLink_mboxOpen;
    g_clientCbs.comIfCb.rlComIfClose = RfLink_mboxClose;
    g_clientCbs.comIfCb.rlComIfRead  = RfLink_mboxRead;
    g_clientCbs.comIfCb.rlComIfWrite = RfLink_mboxWrite;

    g_clientCbs.osiCb.mutex.rlOsiMutexCreate = RfLink_mutexCreate;
    g_clientCbs.osiCb.mutex.rlOsiMutexLock   = RfLink_mutexLock;
    g_clientCbs.osiCb.mutex.rlOsiMutexUnLock = RfLink_mutexUnlock;
    g_clientCbs.osiCb.mutex.rlOsiMutexDelete = RfLink_mutexDelete;

    g_clientCbs.osiCb.sem.rlOsiSemCreate = RfLink_semCreate;
    g_clientCbs.osiCb.sem.rlOsiSemWait   = RfLink_semWait;
    g_clientCbs.osiCb.sem.rlOsiSemSignal = RfLink_semSignal;
    g_clientCbs.osiCb.sem.rlOsiSemDelete = RfLink_semDelete;

    g_clientCbs.osiCb.queue.rlOsiSpawn = RfLink_spawn;

    g_clientCbs.timerCb.rlDelay = RfLink_delay;

    g_clientCbs.crcCb.rlComputeCRC = RfLink_computeCRC;

    g_clientCbs.devCtrlCb.rlDeviceDisable            = RfLink_disableDevice;
    g_clientCbs.devCtrlCb.rlDeviceEnable             = RfLink_enableDevice;
    g_clientCbs.devCtrlCb.rlDeviceMaskHostIrq        = RfLink_maskHostIrq;
    g_clientCbs.devCtrlCb.rlDeviceUnMaskHostIrq      = RfLink_unmaskHostIrq;
    g_clientCbs.devCtrlCb.rlRegisterInterruptHandler = RfLink_registerInterruptHandler;

    g_clientCbs.eventCb.rlAsyncEvent = RfLink_asyncEventHandler;

    int32_t ret = rlDevicePowerOn(RL_DEVICE_MAP_INTERNAL_BSS, g_clientCbs);
    if (ret != 0)
    {
        DebugP_logError("rlDevicePowerOn failed (%d)\n", ret);
        return ret;
    }

    return 0;
}

int32_t RfLink_getVersion(rlVersion_t *ver)
{
    if (ver == NULL)
    {
        return -1;
    }

    return (int32_t)rlDeviceGetVersion(RL_DEVICE_MAP_INTERNAL_BSS, ver);
}

void RfLink_processSpawn(void)
{
    /* Process any queued spawn requests - call this from main loop */
    while (g_spawnTail != g_spawnHead)
    {
        RL_P_OSI_SPAWN_ENTRY entry = g_spawnQueue[g_spawnTail];
        const void *arg = g_spawnArgs[g_spawnTail];
        g_spawnTail = (g_spawnTail + 1U) % RFLINK_MAX_SPAWN;

        if (entry != NULL)
        {
            entry(arg);
        }
    }
}
