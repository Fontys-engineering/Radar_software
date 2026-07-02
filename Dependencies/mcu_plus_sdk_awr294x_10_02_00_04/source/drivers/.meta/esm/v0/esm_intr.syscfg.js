let common = system.getScript("/common");

let esm_intr_module = {
    displayName: "ESM Direct Interrupt Map",
    defaultInstanceName: "ESM_DIR_INTR_MAP",
    longDescription:
    `This module allows user to directly map events to a DSP interrupt (4 - 11).
    By default, the events will go through an event combiner mapped to a DSP interrupt (12-15).
    Use the checkbox provided to enable direct interrupt mapping
    and select the DSP interrupt number. Higher the interrupt number, lower the priority.
    `,
    config: [
        {
            name: "endirmapesmhi",
            displayName: "DSS_ESM_HI",
            default: false,

        },
        {
            name: "endirmapesmlo",
            displayName: "DSS_ESM_LO",
            default: false,

        },

    ],
    pinmuxRequirements,
};

function pinmuxRequirements(inst) {
    let interfaceName = "DSS_INTR";
    let peripheral;
    let array = new Array();

    if (inst.endirmapesmhi == true)
    {
        peripheral = {
            name          : "esmhi",
            displayName   : "DSS_ESM_HI",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    if (inst.endirmapesmlo == true)
    {
        peripheral = {
            name          : "esmlo",
            displayName   : "DSS_ESM_LO",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)
    }

    return array;
}

exports = esm_intr_module;