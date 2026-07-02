#!/bin/bash
#
# ******************************************************************
# * FILE PURPOSE: Check environment setup
# ******************************************************************
# * FILE NAME: checkenv.sh
# *
# * DESCRIPTION:
# *  Checks the build environment and sets PATH for mmWave SDK build.
# *
# ******************************************************************

. ./print_utilities.sh

# Supported devices
devices_str="awr2943 awr2944 awr2544 awr2x44P"

retval=0

# -------------------------------------------------------------------
# Sanity Check: Ensure that all the required variables are defined
# -------------------------------------------------------------------
if [ -z $MMWAVE_SDK_INSTALL_PATH ]; then
    printErrorMsg "Error: MMWAVE_SDK_INSTALL_PATH [mmWave SDK Package Location] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MMWAVE_SDK_INSTALL_PATH=~/ti/mmwave_mcuplus_sdk_04_07_02_01"
    retval=1
fi

if [ -z $CCS_INSTALL_PATH ]; then
    printErrorMsg "Error: CCS_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export CCS_INSTALL_PATH=~/ti/ccs2010"
    retval=1
fi

if [ -z $R5F_CLANG_INSTALL_PATH ]; then
    printErrorMsg "Error: R5F_CLANG_INSTALL_PATH [R5F Toochain] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export R5F_CLANG_INSTALL_PATH=~/ti/__TI_CGT_ARM_DIR__"
    retval=1
fi
if [ -z $SYSCONFIG_INSTALL_PATH ]; then
    printErrorMsg "Error: SYSCONFIG_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export SYSCONFIG_INSTALL_PATH=~/ti/sysconfig_1.23.0"
    retval=1
fi

if [ ! -d $SYSCONFIG_INSTALL_PATH ]; then
    printErrorMsg "Error: SysConfig folder does not exist"
    retval=1
fi

if [ -z $CCS_BIN_PATH ]; then
    printErrorMsg "Error: CCS_BIN_PATH [CCS BIN Tools] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export CCS_BIN_PATH=~/ti/ccs2010/utils/bin"
    retval=1
fi

if [ -z $CCS_CYGWIN_PATH ]; then
    printErrorMsg "Error: CCS_CYGWIN_PATH [CCS CYGWIN Tools] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export CCS_CYGWIN_PATH=~/ti/ccs2010/utils/cygwin"
    retval=1
fi

# This define is not directly tools related but needed for mmwave sdk build
if [ -z $DOWNLOAD_FROM_CCS ]; then
    printErrorMsg "Error: DOWNLOAD_FROM_CCS needs to be defined to yes or no"
    printInfoMsg  "For example:"
    printInfoMsg  "  export DOWNLOAD_FROM_CCS=yes"
    retval=1
fi
if [ -z $MMWAVE_SDK_DEVICE ]; then
    printErrorMsg "Error: MMWAVE_SDK_DEVICE needs to be defined. Supported devices are ${devices_str}"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MMWAVE_SDK_DEVICE=awr2944"
    retval=1
fi

# check if the specified device is supported
if [[ "${MMWAVE_SDK_DEVICE}" != @(awr2943|awr2944|awr2544|awr2x44P) ]]; then
    printErrorMsg "Error: MMWAVE_SDK_DEVICE $MMWAVE_SDK_DEVICE is not supported. Supported devices are ${devices_str}"
    retval=1
fi

if [ "$MMWAVE_SDK_DEVICE" == "awr2944" ] || [ "$MMWAVE_SDK_DEVICE" == "awr2943" ]; then
if [ -z $MMWAVE_AWR294X_DFP_INSTALL_PATH ]; then
    printErrorMsg "Error: MMWAVE_AWR294X_DFP_INSTALL_PATH [AWR294X DFP] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MMWAVE_AWR294X_DFP_INSTALL_PATH=~/ti/__MMWAVE_AWR294X_DFP_INSTALL_PATH__"
    retval=1
fi
if [ -z $C66X_CODEGEN_INSTALL_PATH ]; then
    printErrorMsg "Error: C66X_CODEGEN_INSTALL_PATH [DSP Toolchain] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C66X_CODEGEN_INSTALL_PATH=~/ti/"
    retval=1
fi
if [ -z $C66x_DSPLIB_INSTALL_PATH ]; then
    printErrorMsg "Error: C66x_DSPLIB_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C66x_DSPLIB_INSTALL_PATH=~/ti/dsplib_c66x_3_4_0_0"
    retval=1
fi
if [ -z $C66x_MATHLIB_INSTALL_PATH ]; then
    printErrorMsg "Error: C66x_MATHLIB_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C66x_MATHLIB_INSTALL_PATH=~/ti/mathlib_c66x_3_1_2_1"
    retval=1
fi
if [ -z $AWR294X_RADARSS_IMAGE_BIN ]; then
    printErrorMsg "Error: AWR294X_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary"
    printInfoMsg  "For example:"
    printInfoMsg  "  export AWR294X_RADARSS_IMAGE_BIN=~/ti/__MMWAVE_AWR294X_DFP_INSTALL_PATH__/firmware/radarss/xwr29xx_radarss_metarprc.bin"
    retval=1
fi
if [ -z $MCU_PLUS_AWR294X_INSTALL_PATH ]; then
    printErrorMsg "Error: MCU_PLUS_AWR294X_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MCU_PLUS_AWR294X_INSTALL_PATH=~/ti/__MCU_PLUS_AWR294X_INSTALL_PATH__"
    retval=1
fi
fi

if [ "$MMWAVE_SDK_DEVICE" == "awr2x44P" ]; then
if [ -z $MMWAVE_AWR294X_DFP_INSTALL_PATH ]; then
    printErrorMsg "Error: MMWAVE_AWR294X_DFP_INSTALL_PATH [AWR294X DFP] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MMWAVE_AWR294X_DFP_INSTALL_PATH=~/ti/__MMWAVE_AWR294X_DFP_INSTALL_PATH__"
    retval=1
fi
if [ -z $C66X_CODEGEN_INSTALL_PATH ]; then
    printErrorMsg "Error: C66X_CODEGEN_INSTALL_PATH [DSP Toolchain] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C66X_CODEGEN_INSTALL_PATH=~/ti/"
    retval=1
fi
if [ -z $C66x_DSPLIB_INSTALL_PATH ]; then
    printErrorMsg "Error: C66x_DSPLIB_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C66x_DSPLIB_INSTALL_PATH=~/ti/dsplib_c66x_3_4_0_0"
    retval=1
fi
if [ -z $C66x_MATHLIB_INSTALL_PATH ]; then
    printErrorMsg "Error: C66x_MATHLIB_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export C66x_MATHLIB_INSTALL_PATH=~/ti/mathlib_c66x_3_1_2_1"
    retval=1
fi
if [ -z $AWR2X44P_RADARSS_IMAGE_BIN ]; then
    printErrorMsg "Error: AWR2X44P_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary"
    printInfoMsg  "For example:"
    printInfoMsg  "  export AWR2X44P_RADARSS_IMAGE_BIN=~/ti/__MMWAVE_AWR294X_DFP_INSTALL_PATH__/firmware/radarss/xwr2x4xp_radarss_metarprc.bin"
    retval=1
fi
if [ -z $MCU_PLUS_AWR2X44P_INSTALL_PATH ]; then
    printErrorMsg "Error: MCU_PLUS_AWR2X44P_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MCU_PLUS_AWR2X44P_INSTALL_PATH=~/ti/__MCU_PLUS_AWR2X44P_INSTALL_PATH__"
    retval=1
fi
fi

if [ "$MMWAVE_SDK_DEVICE" == "awr2544" ]; then
if [ -z $MCU_PLUS_AWR2544_INSTALL_PATH ]; then
    printErrorMsg "Error: MCU_PLUS_AWR2544_INSTALL_PATH needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MCU_PLUS_AWR2544_INSTALL_PATH=~/ti/__MCU_PLUS_AWR2544_INSTALL_PATH__"
    retval=1
fi
if [ -z $MMWAVE_AWR294X_DFP_INSTALL_PATH ]; then
    printErrorMsg "Error: MMWAVE_AWR294X_DFP_INSTALL_PATH [AWR294X DFP] needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export MMWAVE_AWR294X_DFP_INSTALL_PATH=~/ti/__MMWAVE_AWR294X_DFP_INSTALL_PATH__"
    retval=1
fi
if [ -z $AWR2544_RADARSS_IMAGE_BIN ]; then
    printErrorMsg "Error: AWR2544_RADARSS_IMAGE_BIN needs to be defined"
    printInfoMsg  "For example:"
    printInfoMsg  "  export AWR2544_RADARSS_IMAGE_BIN=~/ti/__MMWAVE_AWR294X_DFP_INSTALL_PATH__/firmware/radarss/xwr25xx_radarss_metarprc.bin"
    retval=1
fi
fi


if [ $retval -eq 1 ]; then
    printErrorMsg "Error: One of more build variables are not set correctly. Exiting...."
    return 1
fi

# ---------------------------------------------------------
# Setup the PATH configuration:
# ----------------------------------------------------------
# GMAKE is obtained from CCS_BIN_PATH
# Add CCS BIN to path if it is not already in $PATH
[[ ":$PATH:" != *":$CCS_BIN_PATH:"* ]] && export PATH="$CCS_BIN_PATH:${PATH}"

# ECHO, MKDIR etc. are obtained from CCS_CYGWIN_PATH
# Add CCS CYGWIN to path if it is not already in $PATH
[[ ":$PATH:" != *":$CCS_CYGWIN_PATH:"* ]] && export PATH="$CCS_CYGWIN_PATH:${PATH}"

printInfoMsg "-----------------------------------------------"
printInfoMsg "mmWave Build Environment Configured"
printInfoMsg "-----------------------------------------------"
return 0
