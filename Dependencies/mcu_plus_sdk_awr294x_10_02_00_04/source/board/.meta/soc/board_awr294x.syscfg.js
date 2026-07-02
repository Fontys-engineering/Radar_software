let common = system.getScript("/common");

const driverVer = {
    "flash": {
        version: "v1",
    },
};

const topModules_r5f = [
    "/board/led/led",
    "/board/eeprom/eeprom",
    "/board/flash/flash",
];

const topModules_c66 = [
];

const topModules_hsm = [
];

const topModules_m4 = [
];

exports = {
    getTopModules: function() {
        let topModules = topModules_r5f;
        if(common.getSelfSysCfgCoreName().includes("c66")) {
            topModules = topModules_c66;
        }
        else if(common.getSelfSysCfgCoreName().includes("hsm")) {
            topModules = topModules_hsm;
        }
        else if(common.getSelfSysCfgCoreName().includes("m4")) {
            topModules = topModules_m4;
        }
        return topModules;
    },
    getDriverVer: function(driverName) {
        return driverVer[driverName].version;
    },
};
