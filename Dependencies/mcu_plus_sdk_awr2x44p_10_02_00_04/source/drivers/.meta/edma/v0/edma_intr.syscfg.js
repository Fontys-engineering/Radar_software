let common = system.getScript("/common");

let edma_intr_module = {
    displayName: "EDMA Direct Interrupt Map",
    defaultInstanceName: "EDMA_DIR_INTR_MAP",
    longDescription:
    `This module allows user to directly map events to a DSP interrupt (4 - 11).
    By default, the events will go through an event combiner mapped to a DSP interrupt (12-15).
    Use the checkbox provided to enable direct interrupt mapping
    and select the DSP interrupt number. Higher the interrupt number, lower the priority.
    `,
    config: [
        {
            name: "enDirMapTpccIntAgg",
            displayName: "TPCC_INTAGG",
            default: false,

        },
        {
            name: "enDirMapTpccErrAgg",
            displayName: "TPCC_ERRAGG",
            default: false,

        },
    ],
    pinmuxRequirements,
};

function pinmuxRequirements(inst) {
    let interfaceName = "DSS_INTR";
    let peripheral;
    let array = new Array();

    if (inst.enDirMapTpccIntAgg == true)
    {
        peripheral = {
            name          : "tpccintagg",
            displayName   : "TPCC_INTAGG",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    if (inst.enDirMapTpccErrAgg == true)
    {
        peripheral = {
            name          : "tpccerragg",
            displayName   : "TPCC_ERRAGG",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    return array;
}

exports = edma_intr_module;