let common = system.getScript("/common");
let pinmux = system.getScript("/drivers/pinmux/pinmux");
let soc = system.getScript(`/drivers/iomux/soc/iomux_${common.getSocName()}`);

let iomux_module_name = "/drivers/iomux/iomux";

function getStaticConfigArr() {
    return system.getScript(`/drivers/iomux/soc/iomux_${common.getSocName()}`).getStaticConfigArr();
};

function getInterfaceName(inst, peripheralName)
{
    return `${peripheralName}`;
}

function getInterfaceNameList(inst) {

        return [
            getInterfaceName(inst, "FRAME_START"),
            getInterfaceName(inst, "SYNC_OUT" ),
            getInterfaceName(inst, "SYNC_IN"),
            getInterfaceName(inst, "PMIC_CLKOUT"),
            getInterfaceName(inst, "MCU_CLKOUT"),
            getInterfaceName(inst, "ADC_VALID"),
            getInterfaceName(inst, "CHIRP_START"),
            getInterfaceName(inst, "CHIRP_END")
        ];

}

function getInterfacePinList(inst, peripheralName)
{
    let interfaceName = getInterfaceName(inst, peripheralName);
    let pinList = [];

    pinList = pinmux.getInterfacePinList(interfaceName);

    return pinList;
}

function getPeripheralPinNames(inst)
{
    let pinList = [];

    pinList.push(getInterfacePinList(inst, "FRAME_START"));
    pinList.push(getInterfacePinList(inst, "SYNC_OUT" ));
    pinList.push(getInterfacePinList(inst, "SYNC_IN"));
    pinList.push(getInterfacePinList(inst, "PMIC_CLKOUT"));
    pinList.push(getInterfacePinList(inst, "MCU_CLKOUT"));
    pinList.push(getInterfacePinList(inst, "ADC_VALID"));
    pinList.push(getInterfacePinList(inst, "CHIRP_START"));
    pinList.push(getInterfacePinList(inst, "CHIRP_END"));

    return pinList;
}

function pinmuxRequirements(inst) {

    let resources = [];
    resources.push( pinmux.getPinRequirements("FRAME_START", "START", "Frame start pin"));
    resources[0].used = false;

    let peripheral1 = {
        name          : "FRAME_START",
        displayName   : "IOMUX Instance",
        interfaceName : "FRAME_START",
        resources     : resources,

    };

    resources = [];
    resources.push( pinmux.getPinRequirements("SYNC_OUT", "OUT", "Sync Out Pin"));
    resources[0].used = false;

    let peripheral2 = {
        name          : "SYNC_OUT",
        displayName   : "IOMUX Instance",
        interfaceName : "SYNC_OUT",
        resources     : resources,

    };

    resources = [];
    resources.push( pinmux.getPinRequirements("SYNC_IN", "IN", "Sync In Pin"));
    resources[0].used = false;

    let peripheral3 = {
        name          : "SYNC_IN",
        displayName   : "IOMUX Instance",
        interfaceName : "SYNC_IN",
        resources     : resources,

    };

    resources = [];
    resources.push( pinmux.getPinRequirements("PMIC_CLKOUT", "CLKOUT", "PMIC Clock out Pin"));
    resources[0].used = false;

    let peripheral4 = {
        name          : "PMIC_CLKOUT",
        displayName   : "IOMUX Instance",
        interfaceName : "PMIC_CLKOUT",
        resources     : resources,

    };

    resources = [];
    resources.push( pinmux.getPinRequirements("MCU_CLKOUT", "CLKOUT", "MCU Clock out Pin"));
    resources[0].used = false;

    let peripheral5 = {
        name          : "MCU_CLKOUT",
        displayName   : "IOMUX Instance",
        interfaceName : "MCU_CLKOUT",
        resources     : resources,

    };

    resources = [];
    resources.push( pinmux.getPinRequirements("ADC_VALID", "VALID", "ADC Valid Pin"));
    resources[0].used = false;

    let peripheral6 = {
        name          : "ADC_VALID",
        displayName   : "IOMUX Instance",
        interfaceName : "ADC_VALID",
        resources     : resources,

    };

    resources = [];
    resources.push( pinmux.getPinRequirements("CHIRP_START", "START", "Chirp Start Pin"));
    resources[0].used = false;

    let peripheral7 = {
        name          : "CHIRP_START",
        displayName   : "IOMUX Instance",
        interfaceName : "CHIRP_START",
        resources     : resources,

    };

    resources = [];
    resources.push( pinmux.getPinRequirements("CHIRP_END", "END", "Chirp End Pin"));
    resources[0].used = false;

    let peripheral8 = {
        name          : "CHIRP_END",
        displayName   : "IOMUX Instance",
        interfaceName : "CHIRP_END",
        resources     : resources,

    };

    return [peripheral1,peripheral2,peripheral3,peripheral4,peripheral5,peripheral6,peripheral7,peripheral8]
}

let iomux_module = {
    displayName: "IOMUX",
    templates: {
        "/drivers/pinmux/pinmux_config.c.xdt": {
            moduleName: iomux_module_name,
        },
        "/drivers/system/power_clock_config.c.xdt": {
            moduleName: iomux_module_name,
        },
    },
    defaultInstanceName: "CONFIG_IOMUX",
    maxInstances: getStaticConfigArr().length,
    getStaticConfigArr,
    pinmuxRequirements,
    getInterfaceNameList,
    getPeripheralPinNames,
};


exports = iomux_module;