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

/*
 * QSPI
 */


/* QSPI attributes */
static QSPI_Attrs gQspiAttrs[CONFIG_QSPI_NUM_INSTANCES] =
{
    {
        .baseAddr             = CSL_MSS_QSPI_U_BASE,
        .memMapBaseAddr       = CSL_EXT_FLASH_U_BASE,
        .inputClkFreq         = 80000000U,
        .intrNum              = 35U,
        .intrEnable           = FALSE,
        .dmaEnable            = FALSE,
        .intrPriority         = 4U,
        .rxLines              = QSPI_RX_LINES_QUAD,
        .chipSelect           = QSPI_CS0,
        .csPol                = QSPI_CS_POL_ACTIVE_LOW,
        .dataDelay            = QSPI_DATA_DELAY_0,
        .frmFmt               = QSPI_FF_POL0_PHA0,
        .wrdLen               = 8,
        .baudRateDiv          = 0,
    },
};
/* QSPI objects - initialized by the driver */
static QSPI_Object gQspiObjects[CONFIG_QSPI_NUM_INSTANCES];
/* QSPI driver configuration */
QSPI_Config gQspiConfig[CONFIG_QSPI_NUM_INSTANCES] =
{
    {
        &gQspiAttrs[CONFIG_QSPI0],
        &gQspiObjects[CONFIG_QSPI0],
    },
};

uint32_t gQspiConfigNum = CONFIG_QSPI_NUM_INSTANCES;

/*
 * EDMA
 */
/* EDMA atrributes */
static EDMA_Attrs gEdmaAttrs[CONFIG_EDMA_NUM_INSTANCES] =
{
    {

        .baseAddr           = CSL_DSS_TPCC_B_U_BASE,
        .tcBaseAddr[0]    = CSL_DSS_TPTC_B0_U_BASE,
        .tcBaseAddr[1]    = CSL_DSS_TPTC_B1_U_BASE,
        .numTptc            = 2,
        .compIntrNumber     = CSL_MSS_INTR_DSS_TPCC_B_INTAGG,
        .compIntrNumberDirMap      = 0,
        .isErrIntrAvailable   = 1,
        .errIntrNumber      = CSL_MSS_INTR_DSS_TPCC_B_ERRAGG,
        .errIntrNumberDirMap      = 0,
        .intrAggEnableAddr  = CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_MASK,
        .intrAggEnableMask  = 0x1FF & (~(2U << 2)),
        .intrAggStatusAddr  = CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_INTAGG_STATUS,
        .intrAggClearMask   = (2U << 2),
        .errIntrAggEnableAddr  = CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_MASK,
        .errIntrAggStatusAddr  = CSL_DSS_CTRL_U_BASE + CSL_DSS_CTRL_DSS_TPCC_B_ERRAGG_STATUS,
        .initPrms           =
        {
            .regionId     = 2,
            .queNum       = 0,
            .initParamSet = FALSE,
            .ownResource    =
            {
                .qdmaCh      = 0x40U,
                .dmaCh[0]    = 0xFFFFFFFFU,
                .dmaCh[1]    = 0xFFFFFFFFU,
                .tcc[0]      = 0xFFFFFFFFU,
                .tcc[1]      = 0xFFFFFFFFU,
                .paramSet[0] = 0xFFFFFFFFU,
                .paramSet[1] = 0xFFFFFFFFU,
                .paramSet[2] = 0xFFFFFFFFU,
                .paramSet[3] = 0x0FFFFFFFU,
            },
            .reservedDmaCh[0]    = 0x00000001U,
            .reservedDmaCh[1]    = 0x00000000U,
        },
    },
    {

        .baseAddr           = CSL_RSS_TPCC_A_U_BASE,
        .tcBaseAddr[0]    = CSL_RSS_TPTC_A0_U_BASE,
        .numTptc            = 1,
        .compIntrNumber     = CSL_MSS_INTR_RSS_TPCC_A_INTAGG,
        .compIntrNumberDirMap      = 0,
        .isErrIntrAvailable   = 1,
        .errIntrNumber      = CSL_MSS_INTR_RSS_TPCC_A_ERRAGG,
        .errIntrNumberDirMap      = 0,
        .intrAggEnableAddr  = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_RSS_TPCC_A_INTAGG_MASK,
        .intrAggEnableMask  = 0x1FF & (~(2U << 2)),
        .intrAggStatusAddr  = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_RSS_TPCC_A_INTAGG_STATUS,
        .intrAggClearMask   = (2U << 2),
        .errIntrAggEnableAddr  = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_RSS_TPCC_A_ERRAGG_MASK,
        .errIntrAggStatusAddr  = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_RSS_TPCC_A_ERRAGG_STATUS,
        .initPrms           =
        {
            .regionId     = 2,
            .queNum       = 0,
            .initParamSet = FALSE,
            .ownResource    =
            {
                .qdmaCh      = 0x30U,
                .dmaCh[0]    = 0x00000000U,
                .dmaCh[1]    = 0x0000FFFFU,
                .tcc[0]      = 0x00000000U,
                .tcc[1]      = 0x0000FFFFU,
                .paramSet[0] = 0x00000000U,
                .paramSet[1] = 0x00000000U,
                .paramSet[2] = 0xFFFFFFFFU,
                .paramSet[3] = 0x00000000U,
            },
            .reservedDmaCh[0]    = 0x00000000U,
            .reservedDmaCh[1]    = 0x00000001U,
        },
    },
};

/* EDMA objects - initialized by the driver */
static EDMA_Object gEdmaObjects[CONFIG_EDMA_NUM_INSTANCES];
/* EDMA driver configuration */
EDMA_Config gEdmaConfig[CONFIG_EDMA_NUM_INSTANCES] =
{
    {
        &gEdmaAttrs[CONFIG_EDMA0],
        &gEdmaObjects[CONFIG_EDMA0],
    },
    {
        &gEdmaAttrs[CONFIG_EDMA1],
        &gEdmaObjects[CONFIG_EDMA1],
    },
};

uint32_t gEdmaConfigNum = CONFIG_EDMA_NUM_INSTANCES;

/*
 * ADCBUF
 */
/* ADCBUF atrributes */
static ADCBuf_Attrs gADCBufAttrs[CONFIG_ADCBUF_NUM_INSTANCES] =
{
    {
        .baseAddr           = CSL_RSS_CTRL_U_BASE,
        .interruptNum       = 159U,
        .adcbufBaseAddr     = CSL_RSS_ADCBUF_READ_U_BASE,
        .cqbufBaseAddr      = CSL_BSS_DFE_CQ1_U_BASE,
    },
};
/* ADCBUF objects - initialized by the driver */
static ADCBuf_Object gADCBufObjects[CONFIG_ADCBUF_NUM_INSTANCES];
/* ADCBUF driver configuration */
ADCBuf_Config gADCBufConfig[CONFIG_ADCBUF_NUM_INSTANCES] =
{
    {
        &gADCBufAttrs[CONFIG_ADCBUF0],
        &gADCBufObjects[CONFIG_ADCBUF0],
    },
};

uint32_t gADCBufConfigNum = CONFIG_ADCBUF_NUM_INSTANCES;

/*
 * CBUFF
 */
/* CBUFF atrributes */
CBUFF_Attrs gCbuffAttrs[CONFIG_CBUFF_NUM_INSTANCES] =
{
    {
        .baseAddr                   = CSL_DSS_CBUFF_U_BASE,
        .fifoBaseAddr               = CSL_DSS_CBUFF_FIFO_U_BASE,
        .adcBufBaseAddr             = CSL_RSS_ADCBUF_READ_U_BASE,
        .maxLVDSLanesSupported      = 2,
        .errorIntrNum               = 144,
        .intrNum                    = 143,
        .errorIntrNumDirMap         = 0,
        .intrNumDirMap              = 0,
        .chirpModeStartIndex        = 1,
        .chirpModeEndIndex          = 8,
        .cpSingleChirpInterleavedAddr[0] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0,
        .cpSingleChirpInterleavedAddr[1] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0 + 4,
        .cpSingleChirpInterleavedAddr[2] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0 + 8,
        .cpSingleChirpInterleavedAddr[3] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0 + 12,
        .cpSingleChirpNonInterleavedAddr[0] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0,
        .cpSingleChirpNonInterleavedAddr[1] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0 + 4,
        .cpSingleChirpNonInterleavedAddr[2] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0 + 8,
        .cpSingleChirpNonInterleavedAddr[3] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CPREG0 + 12,
        .cpMultipleChirpNonInterleavedAddr[0] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH0CPREG0,
        .cpMultipleChirpNonInterleavedAddr[1] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH1CPREG0,
        .cpMultipleChirpNonInterleavedAddr[2] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH2CPREG0,
        .cpMultipleChirpNonInterleavedAddr[3] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH3CPREG0,
        .cpMultipleChirpNonInterleavedAddr[4] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH4CPREG0,
        .cpMultipleChirpNonInterleavedAddr[5] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH5CPREG0,
        .cpMultipleChirpNonInterleavedAddr[6] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH6CPREG0,
        .cpMultipleChirpNonInterleavedAddr[7] = CSL_RSS_CTRL_U_BASE + CSL_RSS_CTRL_CH7CPREG0,
        .cbuffChannelId[0]             = 18,
        .cbuffChannelId[1]             = 19,
        .cbuffChannelId[2]             = 20,
        .cbuffChannelId[3]             = 21,
        .cbuffChannelId[4]             = 22,
        .cbuffChannelId[5]             = 23,
        .cbuffChannelId[6]             = 24,
    },
};

/* CBUFF objects - initialized by the driver */
CBUFF_Object gCbuffObject[CONFIG_CBUFF_NUM_INSTANCES];
/* CBUFF objects - storage for CBUFF driver object handles */
CBUFF_Object *gCbuffObjectPtr[CONFIG_CBUFF_NUM_INSTANCES] = { NULL };
/* CBUFF objects count */
uint32_t gCbuffConfigNum = CONFIG_CBUFF_NUM_INSTANCES;

/*
 * ESM
 */
/* ESM atrributes */
static ESM_Attrs gEsmAttrs[CONFIG_ESM_NUM_INSTANCES] =
{
    {
        .ptrESMRegs             = (CSL_esmRegs *)CSL_MSS_ESM_U_BASE,
        .ptrCtrlRegs            = (CSL_mss_ctrlRegs *)CSL_MSS_CTRL_U_BASE,
        .numGroup1Err           = ESM_NUM_INTR_PER_GROUP,
        .highPrioIntNum         = CSL_MSS_INTR_MSS_ESM_HI,
        .lowPrioIntNum          = CSL_MSS_INTR_MSS_ESM_LO,
        .highPrioIntNumDirMap   = 0,
        .lowPrioIntNumDirMap    = 0,
        .intrHighPriority       = 15U,
        .intrLowPriority        = 8U,
    },
};
/* ESM objects - initialized by the driver */
static ESM_Object gEsmObjects[CONFIG_ESM_NUM_INSTANCES];
/* ESM driver configuration */
ESM_Config gEsmConfig[CONFIG_ESM_NUM_INSTANCES] =
{
    {
        &gEsmAttrs[CONFIG_ESM0],
        &gEsmObjects[CONFIG_ESM0],
    },
};

uint32_t gEsmConfigNum = CONFIG_ESM_NUM_INSTANCES;

/*
 * IPC Notify
 */
#include <drivers/ipc_notify.h>
#include <drivers/ipc_notify/v1/ipc_notify_v1.h>

/* Dedicated mailbox memories address and size */
#define MSS_MBOX_MEM                (CSL_MSS_MBOX_U_BASE)
#define MSS_MBOX_MEM_SIZE           (8U*1024U)

/*
* SW queue between each pair of CPUs
*
* place SW queues at the bottom of the dedicated mailbox memories.
* Driver assume this memory is init to zero in bootloader as it's ECC protected and
* needs to be intialized only once and to ensure that only one core has done the
* mailbox ram initialization before ipc_init. If SBL is not used then Gel does the initialization.
* We need 4 SW Q's for the 2x R5F to send messages to C66SS0 and each other, i.e 128 B
* and we need 2 SW Q's for C66SS0 to send messages to each R5F, i.e 64 B.
*
* Rest of the mailbox memory can be used for ipc_rpmessage or custom message passing.
*/
#define C66SS0_TO_R5FSS0_0_SW_QUEUE        (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*6U))
#define C66SS0_TO_R5FSS0_1_SW_QUEUE        (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*5U))
#define R5FSS0_1_TO_R5FSS0_0_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*4U))
#define R5FSS0_1_TO_C66SS0_SW_QUEUE        (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*3U))
#define R5FSS0_0_TO_R5FSS0_1_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*2U))
#define R5FSS0_0_TO_C66SS0_SW_QUEUE        (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*1U))

/*
 * IPC RP Message
 */
#include <drivers/ipc_rpmsg.h>

/* Number of CPUs that are enabled for IPC RPMessage */
#define IPC_RPMESSAGE_NUM_CORES           (2U)
/* Number of VRINGs for the numner of CPUs that are enabled for IPC */
#define IPC_RPMESSAGE_NUM_VRINGS          (IPC_RPMESSAGE_NUM_CORES*(IPC_RPMESSAGE_NUM_CORES-1))
/* Number of a buffers in a VRING, i.e depth of VRING queue */
#define IPC_RPMESSAGE_NUM_VRING_BUF       (1U)
/* Max size of a buffer in a VRING */
#define IPC_RPMESSAGE_MAX_VRING_BUF_SIZE  (1152U)
/* Size of each VRING is
 *     number of buffers x ( size of each buffer + space for data structures of one buffer (32B) )
 */
#define IPC_RPMESSAGE_VRING_SIZE          RPMESSAGE_VRING_SIZE(IPC_RPMESSAGE_NUM_VRING_BUF, IPC_RPMESSAGE_MAX_VRING_BUF_SIZE)

/* Total Shared memory size used for IPC */
#define IPC_SHARED_MEM_SIZE               (2432U)

/* Shared Memory Used for IPC.
*
* IMPORTANT: Make sure of below,
* - The section defined below should be placed at the exact same location in memory for all the CPUs
* - The memory should be marked as non-cached for all the CPUs
* - The section should be marked as NOLOAD in all the CPUs linker command file
*/
uint8_t gIpcSharedMem[IPC_SHARED_MEM_SIZE] __attribute__((aligned(128), section(".bss.ipc_vring_mem")));

/* This function is called within IpcNotify_init, this function returns core specific IPC config */
void IpcNotify_getConfig(IpcNotify_InterruptConfig **interruptConfig, uint32_t *interruptConfigNum)
{
    /* extern globals that are specific to this core */
    extern IpcNotify_InterruptConfig gIpcNotifyInterruptConfig_r5fss0_0[];
    extern uint32_t gIpcNotifyInterruptConfigNum_r5fss0_0;

    *interruptConfig = &gIpcNotifyInterruptConfig_r5fss0_0[0];
    *interruptConfigNum = gIpcNotifyInterruptConfigNum_r5fss0_0;
}

/* This function is called within IpcNotify_init, this function allocates SW queue */
void IpcNotify_allocSwQueue(IpcNotify_MailboxConfig *mailboxConfig)
{
    IpcNotify_MailboxConfig (*mailboxConfigPtr)[CSL_CORE_ID_MAX] = (void *)mailboxConfig;

    mailboxConfigPtr[CSL_CORE_ID_R5FSS0_0][CSL_CORE_ID_C66SS0].swQ = R5FSS0_0_TO_C66SS0_SW_QUEUE;
    mailboxConfigPtr[CSL_CORE_ID_C66SS0][CSL_CORE_ID_R5FSS0_0].swQ = C66SS0_TO_R5FSS0_0_SW_QUEUE;
}


/*
 * UART
 */
#include "drivers/soc.h"

/* UART atrributes */
static UART_Attrs gUartAttrs[CONFIG_UART_NUM_INSTANCES] =
{
    {
        .baseAddr           = CSL_MSS_SCIB_U_BASE,
        .inputClkFreq       = 150000000U,
    },
    {
        .baseAddr           = CSL_MSS_SCIA_U_BASE,
        .inputClkFreq       = 150000000U,
    },
};
/* UART objects - initialized by the driver */
static UART_Object gUartObjects[CONFIG_UART_NUM_INSTANCES];
/* UART driver configuration */
UART_Config gUartConfig[CONFIG_UART_NUM_INSTANCES] =
{
    {
        &gUartAttrs[CONFIG_UART1],
        &gUartObjects[CONFIG_UART1],
    },
    {
        &gUartAttrs[CONFIG_UART0],
        &gUartObjects[CONFIG_UART0],
    },
};

uint32_t gUartConfigNum = CONFIG_UART_NUM_INSTANCES;

void Drivers_uartInit(void)
{
    uint32_t i;
    for (i=0; i<CONFIG_UART_NUM_INSTANCES; i++)
    {
        SOC_RcmPeripheralId periphID;
        if(gUartAttrs[i].baseAddr == CSL_MSS_SCIA_U_BASE) {
            periphID = SOC_RcmPeripheralId_MSS_SCIA;
        } else if (gUartAttrs[i].baseAddr == CSL_MSS_SCIB_U_BASE) {
            periphID = SOC_RcmPeripheralId_MSS_SCIB;
        } else if (gUartAttrs[i].baseAddr == CSL_DSS_SCIA_U_BASE) {
            periphID = SOC_RcmPeripheralId_DSS_SCIA;
        } else {
            continue;
        }
        gUartAttrs[i].inputClkFreq = SOC_rcmGetPeripheralClock(periphID);
    }
    UART_init();
}


void Pinmux_init(void);
void PowerClock_init(void);
void PowerClock_deinit(void);

/*
 * Common Functions
 */



void System_init(void)
{
    /* DPL init sets up address transalation unit, on some CPUs this is needed
     * to access SCICLIENT services, hence this needs to happen first
     */
    Dpl_init();

    
    /* initialize PMU */
    CycleCounterP_init(SOC_getSelfCpuClk());


    PowerClock_init();
    /* Now we can do pinmux */
    Pinmux_init();
    /* finally we initialize all peripheral drivers */
    QSPI_init();
    EDMA_init();
    ADCBuf_init(SystemP_WAIT_FOREVER);
    ESM_init();


    /* IPC Notify */
    {
        IpcNotify_Params notifyParams;
        int32_t status;

        /* initialize parameters to default */
        IpcNotify_Params_init(&notifyParams);

        /* specify the priority of IPC Notify interrupt */
        notifyParams.intrPriority = 12U;

        /* specify the core on which this API is called */
        notifyParams.selfCoreId = CSL_CORE_ID_R5FSS0_0;

        /* list the cores that will do IPC Notify with this core
        * Make sure to NOT list 'self' core in the list below
        */
        notifyParams.numCores = 1;
        notifyParams.coreIdList[0] = CSL_CORE_ID_C66SS0;

        notifyParams.isMailboxIpcEnabled = 1;

        notifyParams.isCrcEnabled = 0;


        notifyParams.isCustomIpcConfigEnabled = 0;

        /* initialize the IPC Notify module */
        status = IpcNotify_init(&notifyParams);
        DebugP_assert(status==SystemP_SUCCESS);

        { /* Mailbox driver MUST be initialized after IPC Notify init */
            Mailbox_Params mailboxInitParams;

            Mailbox_Params_init(&mailboxInitParams);
            status = Mailbox_init(&mailboxInitParams);
            DebugP_assert(status == SystemP_SUCCESS);
        }
    }
    /* IPC RPMessage */
    {
        RPMessage_Params rpmsgParams;
        int32_t status;

        /* initialize parameters to default */
        RPMessage_Params_init(&rpmsgParams);

        /* TX VRINGs */
        rpmsgParams.vringTxBaseAddr[CSL_CORE_ID_C66SS0] = (uintptr_t)(&gIpcSharedMem[0]);
        /* RX VRINGs */
        rpmsgParams.vringRxBaseAddr[CSL_CORE_ID_C66SS0] = (uintptr_t)(&gIpcSharedMem[1216]);
        /* Other VRING properties */
        rpmsgParams.vringSize = IPC_RPMESSAGE_VRING_SIZE;
        rpmsgParams.vringNumBuf = IPC_RPMESSAGE_NUM_VRING_BUF;
        rpmsgParams.vringMsgSize = IPC_RPMESSAGE_MAX_VRING_BUF_SIZE;
        rpmsgParams.isCrcEnabled = 0;

        /* initialize the IPC RP Message module */
        status = RPMessage_init(&rpmsgParams);
        DebugP_assert(status==SystemP_SUCCESS);
    }

    Drivers_uartInit();
}

void System_deinit(void)
{
    QSPI_deinit();
    EDMA_deinit();
    ADCBuf_deinit();
    ESM_deinit();
    RPMessage_deInit();
    IpcNotify_deInit();

    UART_deinit();
    PowerClock_deinit();
    Dpl_deinit();
}
