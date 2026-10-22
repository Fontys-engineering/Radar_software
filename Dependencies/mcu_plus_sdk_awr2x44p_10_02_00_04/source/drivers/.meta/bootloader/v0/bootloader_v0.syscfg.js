let common = system.getScript("/common");
let soc = system.getScript(`/drivers/bootloader/soc/bootloader_${common.getSocName()}`);

function getConfigArr() {
	return soc.getConfigArr();
}

let romPCIeCfgDesciption = `
If checked, will use the PCIE configuration as done by ROM, and SBL will not
reopen the driver (enable clocks, do link training). SBL will only be
configuring the ATU regions required to receive the application image.
On checking the box, will generate a macro that can be used in the SBL to
selectively compile PCIe reinitialization sections. Unchecking the box, will pull
in PCIe in EP (End Point) mode.
`

let bootloader_module_name = "/drivers/bootloader/bootloader";
function getConfig(){
    let cfg = [
        {
            name: "bootMedia",
            displayName: "Boot Media",
            default: "FLASH",
            options: soc.getBootMediaArr(),
            onChange: function (inst, ui) {
                /* appImageOffset applicable only for OSPI Flash boot */
                if(inst.bootMedia == "FLASH") {
                    ui.appImageOffset.hidden = false;
                }
                else {
                    ui.appImageOffset.hidden = true;
                }
                /* appImageBaseAddress applicable only for Memory boot */
                if(inst.bootMedia == "MEM") {
                    ui.appImageBaseAddress.hidden = false;
                }
                else {
                    ui.appImageBaseAddress.hidden = true;
                }
                /* EMMCAppImageOffset applicable only for EMMC boot */
                if(inst.bootMedia == "EMMC") {
                    ui.EMMCAppImageOffset.hidden = false;
                }
                else {
                    ui.EMMCAppImageOffset.hidden = true;
                }
                /* romPCIeCfg applicabke only for PCIe boot */
                if(inst.bootMedia == "PCIE") {
                    ui.romPCIeCfg.hidden = false;
                }
                else {
                    ui.romPCIeCfg.hidden = true;
                }
            },

        },
        {
            name: "appImageOffset",
            displayName: "Boot Image Offset",
            description: "Offset of the Boot Image in Flash",
            default: "0x00000000",
            hidden: false,
        },
        {
            name: "appImageBaseAddress",
            displayName: "Boot Image Base Address",
            description: "Base address of the Boot Image in SOC memory",
            default: "0x00000000",
            hidden: true,
        },
        {
            name: "EMMCAppImageOffset",
            displayName: "Boot Image Offset",
            description: "Offset of Boot Image in EMMC",
            default: "0x00000000",
            hidden: true,
        },
        {
            name: "romPCIeCfg",
            displayName: "Use ROM PCIe Configuration",
            longDescription: romPCIeCfgDesciption,
            default: true,
            hidden: true,
        }
    ]
    if(["am263x", "am263px"].includes(common.getSocName())) {
        cfg.push(
            {
                name: "R5FSS0operatingMode",
                displayName: "R5FSS0 Operating Mode",
                description: "Operating mode for the R5F Subsystem 0",
                default: "Lockstep",
                options: soc.getOperatingMode(),
            });
        cfg.push(
            {
                name: "R5FSS1operatingMode",
                displayName: "R5FSS1 Operating Mode",
                description: "Operating mode for the R5F Subsystem 1",
                default: "Lockstep",
                options: soc.getOperatingMode(),
            })
    }
    if(common.getSocName() == "am273x") {
        cfg.push(
            {
                name: "R5FSS0operatingMode",
                displayName: "R5FSS0 Operating Mode",
                description: "Operating mode for the R5F Subsystem 0",
                default: "Lockstep",
                options: soc.getOperatingMode(),
            });
    }
    if(["am243x", "am64x","awr294x","awr2544","awr2x44p"].includes(common.getSocName())) {
        cfg.push(
            {
                name: "isAppimageSigned",
                displayName: "Application Image Is X509 Signed",
                default: true,
                onChange: function(inst, ui) {
                    if(inst.isAppimageSigned) {
                        ui.disableAppImageAuth.hidden = false;
                        inst.disableAppImageAuth = false;
                        ui.isAppimageEncrypted.hidden = false;
                    } else {
                        ui.disableAppImageAuth.hidden = true;
                        inst.disableAppImageAuth = true;
                        ui.isAppimageEncrypted.hidden = true;
                        inst.isAppimageEncrypted = false;
                    }
                }
            }
        )
        cfg.push(
            {
                name: "disableAppImageAuth",
                displayName: "Disable Auth For Application Image",
                description: "Selecting this would disable authentication for application images. Make sure that encryption is also turned off for the application image if you're disabling authentication. ",
                default: false,
                hidden: false,
                onChange: function(inst, ui) {
                    if(inst.disableAppImageAuth) {
                        ui.isAppimageEncrypted.hidden = true;
                        inst.isAppimageEncrypted = false;
                    } else {
                        ui.isAppimageEncrypted.hidden = false;
                    }
                }
            }
        )
        cfg.push(
            {
                name: "isAppimageEncrypted",
                displayName: "Decrypt Application Image",
                description: "Indicate to bootloader that application image decryption is required (only used if bootmedia is FLASH, otherwise this field is don't care).",
                longDescription: "Enable this if application image is encrypted and its x509 certificate contains encryption sequence OID. This field has not effect if either \
                                   the device is not HS-SE, image is not x509 signed, authentication is disabled or or L3 RAM MPU Firewall if not set",
                default: false,
                hidden: false,
            }
        );
    }
    if(["am243x", "am64x"].includes(common.getSocName())) {
        cfg.push(
            {
                name: "initICSSCores",
                displayName: "Initialize ICSS Cores",
                description: "Whether to initialize the ICSS cores in the bootloader or not",
                default: true,
            });
    }
    return cfg;
}

let bootloader_module = {
    displayName: "BOOTLOADER",

    templates: {
        "/drivers/system/system_config.c.xdt": {
            driver_config: "/drivers/bootloader/templates/bootloader_config.c.xdt",
        },
        "/drivers/system/system_config.h.xdt": {
            driver_config: "/drivers/bootloader/templates/bootloader.h.xdt",
        },
    },
    defaultInstanceName: "CONFIG_BOOTLOADER",
    validate: validate,
    moduleStatic: {
        modules: function(inst) {
            return [{
                name: "system_common",
                moduleName: "/system_common",
            }]
        },
    },
    config : getConfig(),
    sharedModuleInstances: moduleInstances,
};

function moduleInstances(instance){
    let modInstances = new Array();

    if(instance.bootMedia == "FLASH") {
        modInstances.push({
            name: "flashDriver",
            displayName: "Flash Driver Configuration",
            moduleName: '/board/flash/flash',
        });
        if(instance.isAppimageEncrypted) {
            modInstances.push({
                name: "firewall_service",
                displayName: "Set MPU Firewall Configuration",
                moduleName: '/security/firewall_service/firewall_service',
            });
        }
    }

    if(instance.bootMedia == "EMMC") {
        modInstances.push({
            name: "MMCSDDriver",
            displayName: "EMMC Driver Configuration",
            moduleName: '/drivers/mmcsd/mmcsd',
            requiredArgs: {
                moduleSelect: "MMC0",
            },
        });
    }

    if(instance.bootMedia == "PCIE" && instance.romPCIeCfg != true) {
        modInstances.push({
            name: "PCIEDriver",
            displayName: "Pcie Driver Configuration",
            moduleName: '/drivers/pcie/pcie',
            requiredArgs: {
                operMode: "PCIE_EP_MODE",
                gen: "PCIE_GEN2",
            }
        });
    }

    return (modInstances);
}

function hexValidate(myStr){
    let re = /[0-9A-Fa-f]{1}/g;
    if(re.test(myStr)){
        return true;
    }
    else{
        return false;
    }
}

function validate(inst, report) {
    if(inst.bootMedia == "FLASH"){
        let offset = inst.appImageOffset;
        if(offset.slice(0,2) != "0x" || hexValidate(offset.slice(2)) == false) {
            report.logError("Boot Image Offset should be a hexadecimal number and should start with 0x",inst, "appImageOffset");
        }
    }
    else if(inst.bootMedia == "MEMORY"){
        let baseAddr = inst.appImageBaseAddress;
        if(baseAddr.slice(0, 2) != "0x" || hexValidate(baseAddr.slice(2)) == false) {
            report.logError("Boot Image base address should be a hexadecimal number and should start with 0x",inst, "appImageBaseAddress");
        }
    }
    else if(inst.bootMedia == "EMMC"){
        let offset = inst.EMMCAppImageOffset;
        if(offset.slice(0,2) != "0x" || hexValidate(offset.slice(2)) == false) {
            report.logError("Boot Image Offset should be a hexadecimal number and should start with 0x",inst, "EMMCAppImageOffset");
        }
    }
}

exports = bootloader_module;