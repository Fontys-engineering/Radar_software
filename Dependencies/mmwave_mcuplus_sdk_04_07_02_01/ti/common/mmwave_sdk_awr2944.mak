###################################################################################
# Makefile for awr2944
###################################################################################

MMWAVE_SDK_DEVICE_TYPE  = awr2944
PLATFORM_DEVICE_TYPE    = awr294x
SYSCONFIG_DEVICE        = AWR294X
PLATFORM_R5F_LINK_CMD   = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/r5f_linker.cmd
PLATFORM_C66X_LINK_CMD 	= $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/c66x_linker.cmd
PLATFORM_DEFINE         = SOC_AWR294X
DEVICE_TYPE             = SOC_AWR2944
PACKAGE_TYPE            = ETS
MCU_PLUS_INSTALL_PATH   = $(MCU_PLUS_AWR294X_INSTALL_PATH)
PLATFORM_OBJDIR         = obj_$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)