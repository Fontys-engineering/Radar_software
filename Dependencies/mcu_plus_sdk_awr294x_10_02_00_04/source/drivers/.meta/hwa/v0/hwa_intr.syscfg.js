let common = system.getScript("/common");

let hwa_intr_module = {
    displayName: "HWA Direct Interrupt Map",
    defaultInstanceName: "HWA_DIR_INTR_MAP",
    longDescription:
    `This module allows user to directly map events to a DSP interrupt (4 - 11).
    By default, the events will go through an event combiner mapped to a DSP interrupt (12-15).
    Use the checkbox provided to enable direct interrupt mapping
    and select the DSP interrupt number. Higher the interrupt number, lower the priority.
    `,
    config: [
        {
            name: "enDirMaphwaparamdoneintr1",
            displayName: "DSS_HWA_PARAM_DONE_INTR1",
            default: false,
        },

        {
            name: "enDirMaphwaparamdoneintr2",
            displayName: "DSS_HWA_PARAM_DONE_INTR2",
            default: false,
        },
        {
            name: "enDirMaphwaloopintr1",
            displayName: "DSS_HWA_LOOP_INTR1",
            default: false,
        },
        {
            name: "enDirMaphwaloopintr2",
            displayName: "DSS_HWA_LOOP_INTR2",
            default: false,
        },
        {
            name: "enDirMapinthwaLocalRamErr",
            displayName: "DSS_HWA_LOCAL_RAM_ERR",
            default: false,
        },
    ],
    pinmuxRequirements,
};

function pinmuxRequirements(inst) {
    let interfaceName = "DSS_INTR";
    let peripheral;
    let array = new Array();

    if (inst.enDirMaphwaparamdoneintr1 == true)
    {
        peripheral = {
            name          : "dsshwaparamdoneintr1",
            displayName   : "DSS_HWA_PARAM_DONE_INTR1",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    if (inst.enDirMaphwaparamdoneintr2 == true)
    {
        peripheral = {
            name          : "dsshwaparamdoneintr2",
            displayName   : "DSS_HWA_PARAM_DONE_INTR2",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)
    }

    if (inst.enDirMaphwaloopintr1 == true)
    {
        peripheral = {
            name          : "dsshwaloopintr1",
            displayName   : "DSS_HWA_LOOP_INTR1",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    if (inst.enDirMaphwaloopintr2 == true)
    {
        peripheral = {
            name          : "dsshwaloopintr2",
            displayName   : "DSS_HWA_LOOP_INTR2",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    if (inst.enDirMapinthwaLocalRamErr == true)
    {
        peripheral = {
            name          : "dsshwalocalramerr",
            displayName   : "DSS_HWA_LOCAL_RAM_ERR",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    return array;
}

exports = hwa_intr_module;