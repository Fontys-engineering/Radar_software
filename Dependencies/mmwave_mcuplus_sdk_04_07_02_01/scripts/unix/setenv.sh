###############################################################################
#
# Set up tools and build environment variables for mmwave sdk
#
###############################################################################

###############################################################################
# Build variables (to be modified based on build need)
###############################################################################

# Select your device. Options (case sensitive) are:
#   awr2943, awr2944, awr2544, awr2x44P
export MMWAVE_SDK_DEVICE=awr2x44P

# If download via CCS is needed, set below define to yes else no
#   yes: Out file created can be loaded using CCS.
#        Binary file created can be used to flash
#   no:  Out file created cannot be loaded using CCS.
#        Binary file created can be used to flash
#        (additional features: write-protect of TCMA, etc)
export DOWNLOAD_FROM_CCS=yes

if [[ "${MMWAVE_SDK_DEVICE}" == "awr2x44P" ]]; then
# If memory optimized driver and kernel libraries are to be used for M4 applications,
#   set below flag to 1, else 0.
export M4_RELEASE_OPT=1

# For awr2x44P based DDM OOB Demo, if AoA processing is required to be done on MSS,
#   set below flag corresponding to awr2x44P and awr2x44ECO respectively to 1, 
#   while 0 for doing it on DSP. 
export MSS_AOA_ENABLED=1
export ECO_MSS_AOA_ENABLED=1
fi

###############################################################################
# Customer installed tools variables (to be modified based on the installation)
###############################################################################


###############################################################################
# TI tools variables (no changes needed below this line if default mmwave sdk
# installation options are used)
###############################################################################

export MMWAVE_SDK_TOOLS_INSTALL_PATH=__MMWAVE_SDK_TOOLS_INSTALL_PATH__

export CCS_INSTALL_PATH=/opt/ti/ccs2010

# Common settings for all platforms
# Path to <mmwave_sdk installation path> folder
export MMWAVE_SDK_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mmwave_mcuplus_sdk_04_07_02_01
# TI ARM compiler
export R5F_CLANG_INSTALL_PATH=${CCS_INSTALL_PATH}/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS
# TI CCS BIN TOOLS
export CCS_BIN_PATH=${CCS_INSTALL_PATH}/ccs/utils/bin
# TI CCS CYGWIN TOOLS
export CCS_CYGWIN_PATH=${CCS_INSTALL_PATH}/ccs/utils/cygwin
# SYSCONFIG
export SYSCONFIG_INSTALL_PATH=/opt/ti/sysconfig_1.23.0

# AWR294X MCU PLUS SDK
export MCU_PLUS_AWR294X_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mcu_plus_sdk_awr294x_10_02_00_04

# AWR2X44P MCU PLUS SDK
export MCU_PLUS_AWR2X44P_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mcu_plus_sdk_awr2x44p_10_02_00_04

# AWR2544 MCU PLUS SDK
export MCU_PLUS_AWR2544_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mcu_plus_sdk_awr2544_10_02_00_04

#TI AWR294X DFP
export MMWAVE_AWR294X_DFP_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mmwave_dfp_02_04_18_01

# awr294x radarss firmware
export AWR294X_RADARSS_IMAGE_BIN=${MMWAVE_AWR294X_DFP_INSTALL_PATH}/firmware/radarss/xwr29xx_radarss_metarprc.bin

# awr2544 radarss firmware
export AWR2544_RADARSS_IMAGE_BIN=${MMWAVE_AWR294X_DFP_INSTALL_PATH}/firmware/radarss/xwr25xx_radarss_metarprc.bin

# awr2x44P radarss firmware
export AWR2X44P_RADARSS_IMAGE_BIN=${MMWAVE_AWR294X_DFP_INSTALL_PATH}/firmware/radarss/xwr2x4xp_radarss_metarprc.bin

# Following only needed for devices with DSP
# TI DSP compiler
export C66X_CODEGEN_INSTALL_PATH=${CCS_INSTALL_PATH}/ccs/tools/compiler/ti-cgt-c6000_8.3.13
# DSPlib
export C66x_DSPLIB_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/dsplib_c66x_3_4_0_0
# MATHlib
export C66x_MATHLIB_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mathlib_c66x_3_1_2_1


# Call script to check env variables and set paths
source ./checkenv.sh
retval=$?
if [[ $retval != 0 ]]; then
    printErrorMsg "ERROR: Failure in checkenv !!!"
fi
return $retval;
