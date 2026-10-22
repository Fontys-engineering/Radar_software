let common = system.getScript("/common");

const staticConfig_r5f = [
    {
        name: "IOMUX",
    },
];

function getStaticConfigArr() {


    return staticConfig_r5f;
}

function getInterfaceName(inst) {
    return "IOMUX";
}


let soc = {

    getStaticConfigArr,
    getInterfaceName,
};

exports = soc;