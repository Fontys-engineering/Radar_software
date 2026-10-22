
let common = system.getScript("/common");
let soc = system.getScript(`/drivers/soc/drivers_${common.getSocName()}`);

function getModule() {

    let driverVer = soc.getDriverVer("iomux");

    return system.getScript(`/drivers/iomux/${driverVer}/iomux_${driverVer}`);
}

exports = getModule();
