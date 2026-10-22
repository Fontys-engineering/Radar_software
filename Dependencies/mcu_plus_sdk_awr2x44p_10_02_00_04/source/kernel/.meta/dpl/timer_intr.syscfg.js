let common = system.getScript("/common");

let timer_intr_module = {
    displayName: "Timer Direct Interrupt Map",
    defaultInstanceName: "TIMER_DIR_INTR_MAP",
    longDescription:
    `This module allows user to directly map events to a DSP interrupt (4 - 11).
    By default, the events will go through an event combiner mapped to a DSP interrupt (12-15).
    Use the checkbox provided to enable direct interrupt mapping
    and select the DSP interrupt number. Higher the interrupt number, lower the priority.
    `,
    config: [
        {
            name: "enDirMapRtiIntr",
            displayName: "DSS_RTI_INTR",
            default: false,
        },

    ],
    pinmuxRequirements,
};

function pinmuxRequirements(inst) {
    let interfaceName = "DSS_INTR";
    let peripheral;
    let array = new Array();

    if (inst.enDirMapRtiIntr == true)
    {
        peripheral = {
            name          : "dssrtiintr",
            displayName   : "DSS_RTI_INTR",
            interfaceName : interfaceName,
            resources     : [],
        };

        array.push(peripheral)

    }

    return array;
}

exports = timer_intr_module;
