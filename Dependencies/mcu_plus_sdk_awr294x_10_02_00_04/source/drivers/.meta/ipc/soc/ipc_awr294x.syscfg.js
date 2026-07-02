

let common = system.getScript("/common");


let config = [
    {
        name: "r5fss0_0",
        displayName: "R5FSS0 Core 0",
    },
    {
        name: "r5fss0_1",
        displayName: "R5FSS0 Core 1",
    },
    {
        name: "c66ss0",
        displayName: "C66 Core 0",
    },
];

const offset_options_8k = [
    {name:0}   , {name:256} , {name:512} , {name:768} , {name:1024}, {name:1280}, {name:1536}, {name:1792},
    {name:2048}, {name:2304}, {name:2560}, {name:2816}, {name:3072}, {name:3328}, {name:3584}, {name:3840},
    {name:4096}, {name:4352}, {name:4608}, {name:4864}, {name:5120}, {name:5376}, {name:5632}, {name:5888},
    {name:6144}, {name:6400}, {name:6656}, {name:6912}, {name:7168}, {name:7424}, {name:7680}, {name:7936},
];

const offset_options_unused = [ { name: -1} ];

const offset_options_4k = [
    {name:0}   , {name:256} , {name:512} , {name:768} , {name:1024}, {name:1280}, {name:1536}, {name:1792},
    {name:2048}, {name:2304}, {name:2560}, {name:2816}, {name:3072}, {name:3328}, {name:3584}, {name:3840},
];

const size_options_8k = [
    {name:0}   , {name:256} , {name:512} , {name:768} , {name:1024}, {name:1280}, {name:1536}, {name:1792},
    {name:2048}, {name:2304}, {name:2560}, {name:2816}, {name:3072}, {name:3328}, {name:3584}, {name:3840},
    {name:4096}, {name:4352}, {name:4608}, {name:4864}, {name:5120}, {name:5376}, {name:5632}, {name:5888},
    {name:6144}, {name:6400}, {name:6656}, {name:6912}, {name:7168}, {name:7424}, {name:7680}, {name:7936},
    {name:8192},
];

const size_options_unused = [ { name: -1} ];

const size_options_4k = [
    {name:0}   , {name:256} , {name:512} , {name:768} , {name:1024}, {name:1280}, {name:1536}, {name:1792},
    {name:2048}, {name:2304}, {name:2560}, {name:2816}, {name:3072}, {name:3328}, {name:3584}, {name:3840},
    {name:4096},
];

const mbox_reserve = [
    {name:0}, {name:512}, {name:1024}, {name:1536}, {name:2048},
];

let customIpcDescription =`
**Custom IPC Config** enables the IPC communication flow based on direct mailbox communication (exactly as specified in the device TRM document)
between any two supported cores. When customIpcConfig is enabled, the IPC notify driver will use the direct mailbox communication
between two cores as given below (instead of using the normal IPC notify flow, refer the device TRM for more details):

1. The sender core writes in the mailbox of receiver core and trigger the write done signal which triggers read request interrupt on receiver core.
2. Then it waits for acknowledgement from receiver core. The receiver core receivers read request interrupt.
3. The receiver core reads the data from it's own mailbox (at correct offset) after getting read request and clears the interrupt.
4. The receiver sends the read done acknowledgment to sender core after read is done.
5. This acknowledgment triggers another interrupt on sender core (read done ack)
6. After getting read done ack, sender clear ths interrupt and message transfer between the cores is completed. The sender may proceed for another transfer.

To use custom IPC- enable this option on all the required cores, set them in "IPC Notify Only" mode, and update the Rx and Tx mailbox offsets and sizes.

The **Rx Mailbox** specifies the offset and size in self-core mailbox memory- where the self core expected data from remote core. It's recommended to allocate
different (non-overlapping) regions for different remote cores in the mailbox memory so that data is not corrupted if multiple remot cores attempt to write in same mailbox memory.

The **Tx Mailbox** specifies the mailbox offset and size in remote-core mailbox memorywhere the self-core will write the data to remote core. Thus it is necessary to ensure
that self core Rx region for a given remote matches to remote core Tx region for this (self) core.

E.g. For IPC custom config to be used between two cores R5SS0-0 and C66SS0, below conditions should be met:

         R5SS0-0 Syscfg view:                              C66SS0 Syscfg view:
    Rx Mailbox Memory Offset (C66 Core 0)   =      Tx Mailbox Memory Offset (R5FSS0 Core 0)
    Rx Mailbox Size (C66 Core 0)            =      Tx Mailbox Size (R5FSS0 Core 0)

    Tx Mailbox Memory Offset (C66 Core 0)   =      Rx Mailbox Memory Offset (R5FSS0 Core 0)
    Tx Mailbox Size (C66 Core 0)            =      Rx Mailbox Size (R5FSS0 Core 0)

This also determines the max size of message data transfer that can happen in single trigger between two cores.
The Sysconfig setting for IPC Custom Config ensures that it throw errors or warning if the mailbox regions (size and offsets) are not correctly set. But
in order for the checkers to work effectiverly, it necessary to use system example multicore sysconfig (where all cores are opened in syscfg) rather than single core example sysconfig
(where only one core sysconfig is opened) when using this IPC with custom config enabled.
Note: If this option is disabled for any core, remove the IPC module for  that core in syscfg else it may throw the error.

Also Note that if "Enable Mailbox IPC with R4" is selected, the IPC Notify will use a separate mailbox driver to communicate the RSS-R4 core with R5FSS0-0 core and C66SS0 core.
This needs to be checked if R4 communication is needed, and if this is enabled along with enabling custom IPC config- it will show two more fields in sysconfig:
1. R5 Mailbox Size reserved for R4 - Size reserved for R4 communication in R5 mailbox at offset 0. Default value 512.
2. C66 Mailbox Size reserved for R4 - Size reserved for R4 communication in C66 mailbox at offset 0. Default value is 512.

In AWR294x device, R4 can communicate with R5ss0-0 or C66SS0 or both, and this setting is done in SBL, and by default both R5 and C66 are enabled in SBL.
While these values can be set from 0 to 2048 (2KB) from given options- they should be only updated to reflect the value set in SBL i.e. both should be same.
As the SBL sets the size reserved for R4 communication, and Sysconfig setting ensures that other cores don't use this region .
    `;

let selfCoreName = common.getSelfSysCfgCoreName();

function getConfigurables() {

    let configurables = [];
    let cpuConfig1, cpuConfig2;

    for( let cpuConfig of config)
    {
        cpuConfig1 = _.cloneDeep(cpuConfig);
        cpuConfig2 = _.cloneDeep(cpuConfig);

        /* mark self CPU with the text 'self' and make it read only */
        if(cpuConfig1.name == getSelfIpcCoreName())
        {
            cpuConfig1.displayName += " (self)";
            cpuConfig1.readOnly = true;
            cpuConfig1.description = "CPU on which this application is running";
        }
        configurables.push(cpuConfig1);

        if(cpuConfig2.name != getSelfIpcCoreName())
        {
            cpuConfig2.name += "_safeipc";
            cpuConfig2.displayName += " SafeIPC";
            cpuConfig2.description = "Enable Safe IPC with remote core.";
            cpuConfig2.default = false;
            cpuConfig2.hidden = true;
            configurables.push(cpuConfig2);
        }
    }

    if((selfCoreName == "c66ss0") || (selfCoreName == "r5fss0-0"))
    {
        let mailboxConfig = {};
        mailboxConfig.name = "enableMailboxIpc";
        mailboxConfig.displayName = "Enable Mailbox IPC With RSS R4";
        mailboxConfig.readOnly = false;
        mailboxConfig.description = "IPC with RSS R4 and this CPU using the 'mailbox' driver API";
        mailboxConfig.default = true;
        configurables.push(mailboxConfig);
    }

    let enableCustomIpcConfig = {};
    enableCustomIpcConfig.name = "enableCustomIpcConfig";
    enableCustomIpcConfig.displayName = "Enable custom IPC config";
    enableCustomIpcConfig.longDescription = customIpcDescription;
    enableCustomIpcConfig.readOnly = false;
    enableCustomIpcConfig.description = "Enable custom IPC config";
    enableCustomIpcConfig.default = false;
    enableCustomIpcConfig.skipTests = ["displayNameCheck"];
    configurables.push(enableCustomIpcConfig);

    let mailboxR5Reserved = {};
    mailboxR5Reserved.name = "mailboxR5Reserved";
    mailboxR5Reserved.displayName = "R5 Mailbox Size reserved for R4";
    mailboxR5Reserved.readOnly = false;
    mailboxR5Reserved.hidden = true;
    mailboxR5Reserved.longDescription = "Mailbox Size reserved on R5FSS0 Core 0 for communication with R4 in SBL at offset 0. \
                             This is configured in SBL and this region  must not be used by other cores. If this has been updated in SBL, \
                             update here in Sysconfig to same value";
    mailboxR5Reserved.default = 512;
    mailboxR5Reserved.options = mbox_reserve;
    mailboxR5Reserved.skipTests = ["displayNameCheck"];
    configurables.push(mailboxR5Reserved);

    let mailboxC66Reserved = {};
    mailboxC66Reserved.name = "mailboxC66Reserved";
    mailboxC66Reserved.displayName = "C66 Mailbox Size reserved for R4";
    mailboxC66Reserved.readOnly = false;
    mailboxC66Reserved.hidden = true;
    mailboxC66Reserved.longDescription = "Mailbox Size reserved on C66 Core 0 for communication with R4 in SBL at offset 0. \
                             This is configured in SBL and this region  must not be used by other cores. If this has been updated in SBL, \
                             update here in Sysconfig to same value";
    mailboxC66Reserved.default = 512;
    mailboxC66Reserved.options = mbox_reserve;
    mailboxC66Reserved.skipTests = ["displayNameCheck"];
    configurables.push(mailboxC66Reserved);

    let rx_mailbox_offset_r5fss0_0 = {};

    rx_mailbox_offset_r5fss0_0.name = "rx_mailbox_offset_r5fss0_0";
    rx_mailbox_offset_r5fss0_0.displayName = "Rx Mailbox Memory Offset (R5FSS0 Core 0)";
    rx_mailbox_offset_r5fss0_0.description = `Offset where ${selfCoreName} core (self core) will read data from r5fss0_0 in its mailbox`;
    rx_mailbox_offset_r5fss0_0.default = 2048;
    rx_mailbox_offset_r5fss0_0.readOnly = false;
    if((selfCoreName == "r5fss0-0") || (selfCoreName == "r5fss0-1"))
    {
        rx_mailbox_offset_r5fss0_0.options= offset_options_8k;
    }
    else
    {
        rx_mailbox_offset_r5fss0_0.options= offset_options_4k;
    }
    rx_mailbox_offset_r5fss0_0.skipTests = ["displayNameCheck"];
    rx_mailbox_offset_r5fss0_0.hidden = true;

    configurables.push(rx_mailbox_offset_r5fss0_0);

    let rx_mailbox_offset_r5fss0_1 = {};

    rx_mailbox_offset_r5fss0_1.name = "rx_mailbox_offset_r5fss0_1";
    rx_mailbox_offset_r5fss0_1.displayName = "Rx Mailbox Memory Offset (R5FSS0 Core 1)";
    rx_mailbox_offset_r5fss0_1.description = `Offset where ${selfCoreName} core (self core) will read data from r5fss0_1 in its own mailbox`;
    rx_mailbox_offset_r5fss0_1.default = 2048;
    rx_mailbox_offset_r5fss0_1.readOnly = false;
    if((selfCoreName == "r5fss0-0") || (selfCoreName == "r5fss0-1"))
    {
        rx_mailbox_offset_r5fss0_1.options= offset_options_8k;
    }
    else
    {
        rx_mailbox_offset_r5fss0_1.options= offset_options_4k;
    }

    rx_mailbox_offset_r5fss0_1.skipTests = ["displayNameCheck"];
    rx_mailbox_offset_r5fss0_1.hidden = true;

    configurables.push(rx_mailbox_offset_r5fss0_1);

    let rx_mailbox_offset_c66ss0 = {};

    rx_mailbox_offset_c66ss0.name = "rx_mailbox_offset_c66ss0";
    rx_mailbox_offset_c66ss0.displayName = "Rx Mailbox Memory Offset (C66 Core 0)";
    rx_mailbox_offset_c66ss0.description = `Offset where ${selfCoreName} core (self core) will read data from c66ss0 in its own mailbox`;
    rx_mailbox_offset_c66ss0.default = 2048;
    rx_mailbox_offset_c66ss0.readOnly = false;
    if((selfCoreName == "r5fss0-0") || (selfCoreName == "r5fss0-1"))
    {
        rx_mailbox_offset_c66ss0.options= offset_options_8k;
    }
    else
    {
        rx_mailbox_offset_c66ss0.options= offset_options_4k;
    }
    rx_mailbox_offset_c66ss0.skipTests = ["displayNameCheck"];
    rx_mailbox_offset_c66ss0.hidden = true;

    configurables.push(rx_mailbox_offset_c66ss0);

    let rx_mailbox_offset_m4ss0_1 = {};

    rx_mailbox_offset_m4ss0_1.name = "rx_mailbox_offset_m4ss0_1";
    rx_mailbox_offset_m4ss0_1.displayName = "Rx Mailbox Memory Offset (M4SS0 Core 1)";
    rx_mailbox_offset_m4ss0_1.description = `Offset where ${selfCoreName} core (self core) will read data from m4ss1_0 in its own mailbox`;
    rx_mailbox_offset_m4ss0_1.default = 2048;
    rx_mailbox_offset_m4ss0_1.readOnly = false;
    if((selfCoreName == "r5fss0-0") || (selfCoreName == "r5fss0-1"))
    {
        rx_mailbox_offset_m4ss0_1.options= offset_options_8k;
    }
    else
    {
        rx_mailbox_offset_m4ss0_1.options= offset_options_4k;
    }
    rx_mailbox_offset_m4ss0_1.skipTests = ["displayNameCheck"];
    rx_mailbox_offset_m4ss0_1.hidden = true;

    configurables.push(rx_mailbox_offset_m4ss0_1);

    let tx_mailbox_offset_r5fss0_0 = {};

    tx_mailbox_offset_r5fss0_0.name = "tx_mailbox_offset_r5fss0_0";
    tx_mailbox_offset_r5fss0_0.displayName = "Tx Mailbox Memory Offset (R5FSS0 Core 0)";
    tx_mailbox_offset_r5fss0_0.description = `Offset where ${selfCoreName} core (self core) will write in r5fss0_0 mailbox`;
    tx_mailbox_offset_r5fss0_0.default = 2048;
    tx_mailbox_offset_r5fss0_0.readOnly = false;
    tx_mailbox_offset_r5fss0_0.options= offset_options_8k;
    tx_mailbox_offset_r5fss0_0.skipTests = ["displayNameCheck"];
    tx_mailbox_offset_r5fss0_0.hidden = true;

    configurables.push(tx_mailbox_offset_r5fss0_0);

    let tx_mailbox_offset_r5fss0_1 = {};

    tx_mailbox_offset_r5fss0_1.name = "tx_mailbox_offset_r5fss0_1";
    tx_mailbox_offset_r5fss0_1.displayName = "Tx Mailbox Memory Offset (R5FSS0 Core 1)";
    tx_mailbox_offset_r5fss0_1.description = `Offset where ${selfCoreName} core (self core) will write in r5fss0_1 mailbox`;
    tx_mailbox_offset_r5fss0_1.default = 4096;
    tx_mailbox_offset_r5fss0_1.readOnly = false;
    tx_mailbox_offset_r5fss0_1.options= offset_options_8k;
    tx_mailbox_offset_r5fss0_1.skipTests = ["displayNameCheck"];
    tx_mailbox_offset_r5fss0_1.hidden = true;

    configurables.push(tx_mailbox_offset_r5fss0_1);

    let tx_mailbox_offset_c66ss0 = {};

    tx_mailbox_offset_c66ss0.name = "tx_mailbox_offset_c66ss0";
    tx_mailbox_offset_c66ss0.displayName = "Tx Mailbox Memory Offset (C66 Core 0)";
    tx_mailbox_offset_c66ss0.description = `Offset where ${selfCoreName} core (self core) will write in c66ss0 mailbox`;
    tx_mailbox_offset_c66ss0.default = 2048;
    tx_mailbox_offset_c66ss0.readOnly = false;
    tx_mailbox_offset_c66ss0.options= offset_options_4k;
    tx_mailbox_offset_c66ss0.skipTests = ["displayNameCheck"];
    tx_mailbox_offset_c66ss0.hidden = true;

    configurables.push(tx_mailbox_offset_c66ss0);

    let tx_mailbox_offset_m4ss0_1 = {};

    tx_mailbox_offset_m4ss0_1.name = "tx_mailbox_offset_m4ss0_1";
    tx_mailbox_offset_m4ss0_1.displayName = "Tx Mailbox Memory Offset (M4SS0 Core 1)";
    tx_mailbox_offset_m4ss0_1.description = `Offset where ${selfCoreName} core (self core) will write in m4ss0_1 mailbox`;
    tx_mailbox_offset_m4ss0_1.default = 3072;
    tx_mailbox_offset_m4ss0_1.readOnly = false;
    tx_mailbox_offset_m4ss0_1.options= offset_options_4k;
    tx_mailbox_offset_m4ss0_1.skipTests = ["displayNameCheck"];
    tx_mailbox_offset_m4ss0_1.hidden = true;

    configurables.push(tx_mailbox_offset_m4ss0_1);

    let rx_mailboxMemoryR5_0 = {};
    rx_mailboxMemoryR5_0.name = "rx_mailboxMemoryR5_0";
    rx_mailboxMemoryR5_0.displayName = "Rx Mailbox Size (R5FSS0 Core 0)";
    rx_mailboxMemoryR5_0.description = `Rx Mailbox Size in Bytes reserved for R5FSS0 Core 0`;
    rx_mailboxMemoryR5_0.readOnly = false;
    rx_mailboxMemoryR5_0.default = 1024;
    rx_mailboxMemoryR5_0.options= size_options_8k;
    rx_mailboxMemoryR5_0.skipTests = ["displayNameCheck"];
    rx_mailboxMemoryR5_0.hidden = true;
    configurables.push(rx_mailboxMemoryR5_0);

    let rx_mailboxMemoryR5_1 = {};
    rx_mailboxMemoryR5_1.name = "rx_mailboxMemoryR5_1";
    rx_mailboxMemoryR5_1.displayName = "Rx Mailbox Size (R5FSS0 Core 1)";
    rx_mailboxMemoryR5_1.description = "Rx Mailbox Size in Bytes reserved for R5FSS0 Core 1";
    rx_mailboxMemoryR5_1.readOnly = false;
    rx_mailboxMemoryR5_1.default = 1024;
    rx_mailboxMemoryR5_1.options= size_options_8k;
    rx_mailboxMemoryR5_1.skipTests = ["displayNameCheck"];
    rx_mailboxMemoryR5_1.hidden = true;
    configurables.push(rx_mailboxMemoryR5_1);

    let rx_mailboxMemoryC66 = {};
    rx_mailboxMemoryC66.name = "rx_mailboxMemoryC66";
    rx_mailboxMemoryC66.displayName = "Rx Mailbox Size (C66 Core 0)";
    rx_mailboxMemoryC66.description = "Rx Mailbox Size in Bytes reserved for C66 Core 0";
    rx_mailboxMemoryC66.readOnly = false;
    rx_mailboxMemoryC66.default = 1024;
    if((selfCoreName == "r5fss0-0") || (selfCoreName == "r5fss0-1"))
    {
        rx_mailboxMemoryC66.options= size_options_8k;
    }
    else
    {
        rx_mailboxMemoryC66.options= size_options_4k;
    }
    rx_mailboxMemoryC66.skipTests = ["displayNameCheck"];
    rx_mailboxMemoryC66.hidden = true;
    configurables.push(rx_mailboxMemoryC66);

    let rx_mailboxMemoryM4_1 = {};
    rx_mailboxMemoryM4_1.name = "rx_mailboxMemoryM4_1";
    rx_mailboxMemoryM4_1.displayName = "Rx Mailbox Size (M4SS0 Core 1)";
    rx_mailboxMemoryM4_1.description = "Rx Mailbox Size in Bytes reserved for M4SS0 Core 1";
    rx_mailboxMemoryM4_1.readOnly = false;
    rx_mailboxMemoryM4_1.default = 1024;
    if((selfCoreName == "r5fss0-0") || (selfCoreName == "r5fss0-1"))
    {
        rx_mailboxMemoryM4_1.options= size_options_8k;
    }
    else
    {
        rx_mailboxMemoryM4_1.options= size_options_4k;
    }
    rx_mailboxMemoryM4_1.skipTests = ["displayNameCheck"];
    rx_mailboxMemoryM4_1.hidden = true;
    configurables.push(rx_mailboxMemoryM4_1);

    let tx_mailboxMemoryR5_0 = {};
    tx_mailboxMemoryR5_0.name = "tx_mailboxMemoryR5_0";
    tx_mailboxMemoryR5_0.displayName = "Tx Mailbox Size (R5FSS0 Core 0)";
    tx_mailboxMemoryR5_0.description = "Tx Mailbox Size in Bytes reserved for R5FSS0 Core 0";
    tx_mailboxMemoryR5_0.readOnly = false;
    tx_mailboxMemoryR5_0.default = 1024;
    tx_mailboxMemoryR5_0.options= size_options_8k;
    tx_mailboxMemoryR5_0.skipTests = ["displayNameCheck"];
    tx_mailboxMemoryR5_0.hidden = true;
    configurables.push(tx_mailboxMemoryR5_0);

    let tx_mailboxMemoryR5_1 = {};
    tx_mailboxMemoryR5_1.name = "tx_mailboxMemoryR5_1";
    tx_mailboxMemoryR5_1.displayName = "Tx Mailbox Size (R5FSS0 Core 1)";
    tx_mailboxMemoryR5_1.description = "Tx Mailbox Size in Bytes reserved for R5FSS0 Core 1";
    tx_mailboxMemoryR5_1.readOnly = false;
    tx_mailboxMemoryR5_1.default = 1024;
    tx_mailboxMemoryR5_1.options= size_options_8k;
    tx_mailboxMemoryR5_1.skipTests = ["displayNameCheck"];
    tx_mailboxMemoryR5_1.hidden = true;
    configurables.push(tx_mailboxMemoryR5_1);

    let tx_mailboxMemoryC66 = {};
    tx_mailboxMemoryC66.name = "tx_mailboxMemoryC66";
    tx_mailboxMemoryC66.displayName = "Tx Mailbox Size (C66 Core 0)";
    tx_mailboxMemoryC66.description = "Tx Mailbox Size in Bytes reserved for C66 Core 0";
    tx_mailboxMemoryC66.readOnly = false;
    tx_mailboxMemoryC66.default = 1024;
    tx_mailboxMemoryC66.options= size_options_4k;
    tx_mailboxMemoryC66.skipTests = ["displayNameCheck"];
    tx_mailboxMemoryC66.hidden = true;
    configurables.push(tx_mailboxMemoryC66);

    let tx_mailboxMemoryM4_1 = {};
    tx_mailboxMemoryM4_1.name = "tx_mailboxMemoryM4_1";
    tx_mailboxMemoryM4_1.displayName = "Tx Mailbox Size (M4SS0 Core 1)";
    tx_mailboxMemoryM4_1.description = "Tx Mailbox Size in Bytes reserved for M4SS0 Core 1";
    tx_mailboxMemoryM4_1.readOnly = false;
    tx_mailboxMemoryM4_1.default = 1024;
    tx_mailboxMemoryM4_1.options= size_options_4k;
    tx_mailboxMemoryM4_1.skipTests = ["displayNameCheck"];
    tx_mailboxMemoryM4_1.hidden = true;
    configurables.push(tx_mailboxMemoryM4_1);


    return configurables;
}

function getSelfIpcCoreName()
{
    let cpuName = common.getSelfSysCfgCoreName();

    switch(cpuName) {
        default:
        case "r5fss0-0":
            return "r5fss0_0";
        case "r5fss0-1":
            return "r5fss0_1";
        case "c66ss0":
            return "c66ss0";
    }
}

function getSysCfgCoreName(ipcCoreName)
{
    switch(ipcCoreName) {
        default:
        case "r5fss0_0":
            return "r5fss0-0";
        case "r5fss0_1":
            return "r5fss0-1";
        case "c66ss0":
            return "c66ss0";
    }
}

function getIpcSharedMemAvailable()
{
    /* size of MSS mailbox memory - space reserved for SW queues (32B per SW queue),
     * right now assume all SW queus are in MSS mailbox memory area
     * 3 CPUs = 3 x (3-1) = 6 SW queues
     * Each SW queue size is 32 bytes
     */
    return ((8*1024) - 512 - (32*6));
}

function getImplementationVersion()
{
    return "v1";
}

function getIpcStaticMemory()
{
    var str = [
    "/* Dedicated mailbox memories address and size */",
    "#define MSS_MBOX_MEM                (CSL_MSS_MBOX_U_BASE)",
    "#define MSS_MBOX_MEM_SIZE           (8U*1024U)",
    "",
    "/*",
    "* SW queue between each pair of CPUs",
    "*",
    "* place SW queues at the bottom of the dedicated mailbox memories.",
    "* Driver assume this memory is init to zero in bootloader as it's ECC protected and",
    "* needs to be intialized only once and to ensure that only one core has done the",
    "* mailbox ram initialization before ipc_init. If SBL is not used then Gel does the initialization.",
    "* We need 4 SW Q's for the 2x R5F to send messages to C66SS0 and each other, i.e 128 B",
    "* and we need 2 SW Q's for C66SS0 to send messages to each R5F, i.e 64 B.",
    "*",
    "* Rest of the mailbox memory can be used for ipc_rpmessage or custom message passing.",
    "*/",
    "#define C66SS0_TO_R5FSS0_0_SW_QUEUE        (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*6U))",
    "#define C66SS0_TO_R5FSS0_1_SW_QUEUE        (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*5U))",
    "#define R5FSS0_1_TO_R5FSS0_0_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*4U))",
    "#define R5FSS0_1_TO_C66SS0_SW_QUEUE        (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*3U))",
    "#define R5FSS0_0_TO_R5FSS0_1_SW_QUEUE      (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*2U))",
    "#define R5FSS0_0_TO_C66SS0_SW_QUEUE        (IpcNotify_SwQueue*)((MSS_MBOX_MEM + MSS_MBOX_MEM_SIZE) - (MAILBOX_MAX_SW_QUEUE_SIZE*1U))",
    ].join("\n");

    return str;
}

function getFirewallGranularity()
{
    return 1024;
}

function getSharedMemAddress()
{
    return 0xC5000200;
}

exports = {
    getConfigurables,
    getSelfIpcCoreName,
    getSysCfgCoreName,
    getIpcSharedMemAvailable,
    getImplementationVersion,
    getFirewallGranularity,
    getSharedMemAddress,
    getIpcStaticMemory,
};
