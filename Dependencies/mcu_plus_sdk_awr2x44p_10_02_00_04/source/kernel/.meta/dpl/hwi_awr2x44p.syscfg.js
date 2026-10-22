
let common = system.getScript("/common");

let intcBaseAddr_r5fss0_0 = 0x02080000;

function getIntcBaseAddr() {

    let cpu = common.getSelfSysCfgCoreName();
    let intcAddr = 0;

    if(cpu.match(/r5fss0-0/))
    {
        intcAddr = intcBaseAddr_r5fss0_0;
    }

    return intcAddr;
}

exports = {
    getIntcBaseAddr,
};

