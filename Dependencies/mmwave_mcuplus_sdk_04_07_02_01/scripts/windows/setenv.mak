###################################################################################
# mmWave SDK setenv.mak for CCS projects
###################################################################################
export MMWAVE_SDK_DEVICE ?= awr2944
export DOWNLOAD_FROM_CCS = yes

export MMWAVE_SDK_TOOLS_INSTALL_PATH=C:/ti/backups

# Common settings for all platforms
# Path to <mmwave_sdk installation path> folder
export MMWAVE_SDK_INSTALL_PATH ?= $(MMWAVE_SDK_TOOLS_INSTALL_PATH)/mmwave_mcuplus_sdk_04_07_02_01
# TI ARM compiler
export R5F_CLANG_INSTALL_PATH = $(MMWAVE_SDK_TOOLS_INSTALL_PATH)/

# SYSCONFIG
export SYSCONFIG_INSTALL_PATH = $(MMWAVE_SDK_TOOLS_INSTALL_PATH)/sysconfig_1.23.0

# TI 294X DFP
export MMWAVE_AWR294X_DFP_INSTALL_PATH = $(MMWAVE_SDK_TOOLS_INSTALL_PATH)/mmwave_dfp_02_04_18_01

# AWR294X MCU PLUS SDK
export MCU_PLUS_AWR294X_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mcu_plus_sdk_awr294x_10_02_00_04

# AWR2X44P MCU PLUS SDK
export MCU_PLUS_AWR2X44P_INSTALL_PATH=${MMWAVE_SDK_TOOLS_INSTALL_PATH}/mcu_plus_sdk_awr2x44p_10_02_00_04

# awr294x radarss firmware
export AWR294X_RADARSS_IMAGE_BIN = $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/firmware/radarss/xwr29xx_radarss_metarprc.bin

# awr2544 radarss firmware
export AWR2544_RADARSS_IMAGE_BIN = $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/firmware/radarss/xwr25xx_radarss_metarprc.bin

# awr2x44P radarss firmware
export AWR2X44P_RADARSS_IMAGE_BIN = $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/firmware/radarss/xwr2x4xp_radarss_metarprc.bin

# Following only needed for devices with DSP
# TI DSP compiler
export C66X_CODEGEN_INSTALL_PATH = $(MMWAVE_SDK_TOOLS_INSTALL_PATH)/
# DSPlib
export C66x_DSPLIB_INSTALL_PATH = $(MMWAVE_SDK_TOOLS_INSTALL_PATH)/dsplib_c66x_3_4_0_0
# MATHlib
export C66x_MATHLIB_INSTALL_PATH = $(MMWAVE_SDK_TOOLS_INSTALL_PATH)/mathlib_c66x_3_1_2_1
