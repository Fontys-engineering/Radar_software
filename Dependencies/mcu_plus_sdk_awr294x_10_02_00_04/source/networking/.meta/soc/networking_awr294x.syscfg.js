
let common = system.getScript("/common");

const topModules = [
    "/networking/enet_cpsw/enet_cpsw",
];

const driverVer = {
    "enet_cpsw": {
        version: "awr294x",
    },
}

exports = {
    getTopModules: function() {

        if((common.getSelfSysCfgCoreName().includes("c66")) || (common.getSelfSysCfgCoreName().includes("m4"))) {
            return [];
        }
        return topModules;
    },
    getDriverVer: function(driverName) {
        return driverVer[driverName].version;
    },
};
