let common = system.getScript("/common");

let edma_intr_module = {
    displayName: "IPC Direct Interrupt Map",
    defaultInstanceName: "IPC_DIR_INTR_MAP",
    longDescription:
    `This module allows user to directly map events to a DSP interrupt (4 - 11).
    By default, the events will go through an event combiner mapped to a DSP interrupt (12-15).
    Use the checkbox provided to enable direct interrupt mapping
    and select the DSP interrupt number. Higher the interrupt number, lower the priority.
    `,
    config: [
        {
            name: "enDirMapmboxreadreq",
            displayName: "DSS_DSP_MBOX_READ_REQ",
            default: false,

        },
        {
            name: "enDirMapmboxreadack",
            displayName: "DSS_DSP_MBOX_READ_ACK",
            default: false,

        },

    ],
    pinmuxRequirements,
};

function pinmuxRequirements(inst) {
    let interfaceName = "DSS_INTR";
    let peripheral;
    let array = new Array();

    if (inst.enDirMapmboxreadreq == true)
    {
        peripheral = {
            name          : "dspmboxreadreq",
            displayName   : "DSS_DSP_MBOX_READ_REQ",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    if (inst.enDirMapmboxreadack == true)
    {
        peripheral = {
            name          : "dspmboxreadack",
            displayName   : "DSS_DSP_MBOX_READ_ACK",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    return array;
}

exports = edma_intr_module;