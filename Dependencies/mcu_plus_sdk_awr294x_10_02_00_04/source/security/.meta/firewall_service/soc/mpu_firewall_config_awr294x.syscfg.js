const mpu_firewall_config = [
    {
        name: "L2_BANKA",
        regionCount: 8,
        memSpace : [
            { startAddr: 0xC0200000, size: 0x80000 },
        ]
    },
    {
        name: "L2_BANKB",
        regionCount: 8,
        memSpace : [
            { startAddr: 0xC0280000, size: 0x70000 },
        ]
    },
    {
        name: "MSS_MBOX",
        regionCount: 8,
        memSpace : [
            { startAddr: 0xC5000000, size: 0x2000 },
        ]
    },
    {
        name: "MSS_PCRA",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x02000000, size: 0x1000000 },
        ]
    },
    {
        name: "QSPI0",
        regionCount: 8,
        memSpace : [
            { startAddr: 0xC8000000, size: 0x40000 },
            { startAddr: 0xC6000000, size: 0x2000000 },
        ]
    },
    {
        name: "R5SS_COREA_AXIS",
        regionCount: 8,
        memSpace : [
            { startAddr: 0xC1000000, size: 0x10000 },
            { startAddr: 0xC1800000, size: 0x10000 },
            { startAddr: 0xC2000000, size: 0x4000 },
            { startAddr: 0xC2800000, size: 0x4000 },
        ]
    },
    {
        name: "R5SS_COREB_AXIS",
        regionCount: 8,
        memSpace : [
            { startAddr: 0xC3000000, size: 0x10000 },
            { startAddr: 0xC3800000, size: 0x10000 },
            { startAddr: 0xC4000000, size: 0x4000 },
            { startAddr: 0xC4800000, size: 0x4000 },
        ]
    },
    {
        name: "L3_BANKA",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x88000000, size: 0xC0000 },
        ]
    },
    {
        name: "L3_BANKB",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x880C0000, size: 0xC0000 },
        ]
    },
    {
        name: "L3_BANKC",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x88180000, size: 0x80000 },
        ]
    },
    {
        name: "L3_BANKD",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x88200000, size: 0x80000 },
        ]
    },
    {
        name: "HWA_DMA0",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x82000000, size: 0x20000 },
        ]
    },
    {
        name: "HWA_DMA1",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x82100000, size: 0x20000 },
        ]
    },
    {
        name: "DSS_HWA_PROC",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x0, size: 0x100000000 },
        ]
    },
    {
        name: "DSS_MBOX",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x83100000, size: 0x1000 },
        ]
    },

    {
        name: "RSS_DSS2RSS",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x55000000, size: 0x1000000 },
            { startAddr: 0xA0000000, size: 0x5020000 },
        ]
    },
    {
        name: "RSS_MSS2RSS",
        regionCount: 8,
        memSpace : [
            { startAddr: 0x55000000, size: 0x1000000 },
            { startAddr: 0xA0000000, size: 0x5020000 },

        ]
    }
];


const id_list = [
    { name: "2", displayName:"PRIV_ID_2" },
    { name: "3", displayName:"PRIV_ID_3" },
    { name: "5", displayName:"PRIV_ID_5" },
    { name: "6", displayName:"PRIV_ID_6" },
    { name: "8", displayName:"PRIV_ID_8" },
    { name: "10", displayName:"PRIV_ID_10" },
    { name: "12", displayName:"PRIV_ID_12" },
    { name: "14", displayName:"PRIV_ID_14" },
    { name: "AIDX", displayName:"EXTERNAL ID" },
]

const default_id_list = ["2", "6", "8"]

function getConfigArr() {
    return mpu_firewall_config;
}

function getAidList() {
    return id_list;
}

function getdefaultAidList() {
    return default_id_list;
}

exports = {
    getConfigArr,
    getAidList,
    getdefaultAidList,
};
