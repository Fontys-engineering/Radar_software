
let common = system.getScript("/common");
let socName = common.getSocName();

function getInstanceConfig(moduleInstance) {
    return {
        ...moduleInstance,
    };
};

let enet_cpsw_tx_ch_module;

if(socName == "awr2544") {
    enet_cpsw_tx_ch_module = {
    displayName: "Tx Channel Config",
    longDescription: `This adds and configures a Enet LLD Tx DMA channel.`,
    alwaysShowLongDescription: false,
    defaultInstanceName: "ENET_DMA_TX_CH",
    config: [
        {
            name: "PacketsCount",
            description: "No of packets required for Tx DMA channel",
            displayName: "Number Of Packets",
            default: 16,
            isInteger:true,
        },
        {
            name: "enableCircBuff",
            description: "",
            displayName: "Enable Circular Buffer",
            default: false,
        },
    ],
    getInstanceConfig,
    };

} else {
    enet_cpsw_tx_ch_module = {
    displayName: "Tx Channel Config",
    longDescription: `This adds and configures a Enet LLD Tx DMA channel.`,
    alwaysShowLongDescription: false,
    defaultInstanceName: "ENET_DMA_TX_CH",
    config: [
        {
            name: "PacketsCount",
            description: "No of packets required for Tx DMA channel",
            displayName: "Number Of Packets",
            default: 16,
            isInteger:true,
        },
    ],
    getInstanceConfig,
    };
}

exports = enet_cpsw_tx_ch_module;
