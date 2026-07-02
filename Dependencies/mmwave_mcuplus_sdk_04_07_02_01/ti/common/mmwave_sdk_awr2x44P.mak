###################################################################################
# Makefile for awr2x44P
###################################################################################

MMWAVE_SDK_DEVICE_TYPE  = awr2x44P
PLATFORM_DEVICE_TYPE    = awr2x44p
SYSCONFIG_DEVICE        = AWR2X44P
PLATFORM_R5F_LINK_CMD   = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/awr2x44x/$(MMWAVE_SDK_DEVICE_TYPE)/r5f_linker.cmd
PLATFORM_C66X_LINK_CMD 	= $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/awr2x44x/$(MMWAVE_SDK_DEVICE_TYPE)/c66x_linker.cmd
PLATFORM_M4_LINK_CMD 	= $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/awr2x44x/$(MMWAVE_SDK_DEVICE_TYPE)/m4_linker.cmd
PLATFORM_DEFINE         = SOC_AWR2X44P
DEVICE_TYPE             = SOC_AWR2X44P
PACKAGE_TYPE            = LOP
MCU_PLUS_INSTALL_PATH   = $(MCU_PLUS_AWR2X44P_INSTALL_PATH)
PLATFORM_OBJDIR         = obj_$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)