let common = system.getScript("/common");

let cbuff_intr_module = {
    displayName: "Cbuff Direct Interrupt Map",
    defaultInstanceName: "CBUFF_DIR_INTR_MAP",
    longDescription:
    `This module allows user to directly map events to a DSP interrupt (4 - 11).
    By default, the events will go through an event combiner mapped to a DSP interrupt (12-15).
    Use the checkbox provided to enable direct interrupt mapping
    and select the DSP interrupt number. Higher the interrupt number, lower the priority.
    `,
    config: [
        {
            name: "enDirMapcbuffinterr",
            displayName: "DSS_CBUFF_INT_ERR",
            default: false,

        },
        {
            name: "enDirMapcbuffint",
            displayName: "DSS_CBUFF_INT",
            default: false,

        },

    ],
    pinmuxRequirements,
};

function pinmuxRequirements(inst) {
    let interfaceName = "DSS_INTR";
    let peripheral;
    let array = new Array();

    if (inst.enDirMapcbuffinterr == true)
    {
        peripheral = {
            name          : "dsscbuffinterr",
            displayName   : "DSS_CBUFF_INT_ERR",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    if (inst.enDirMapcbuffint == true)
    {
        peripheral = {
            name          : "dsscbuffint",
            displayName   : "DSS_CBUFF_INT",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)
    }

    return array;
}

exports = cbuff_intr_module;