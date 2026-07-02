###################################################################################
# Makefile for awr2944
###################################################################################

MMWAVE_SDK_DEVICE_TYPE  = awr2544
PLATFORM_DEVICE_TYPE    = awr2544
SYSCONFIG_DEVICE        = AWR2544
PLATFORM_R5F_LINK_CMD   = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/r5f_linker.cmd
PLATFORM_DEFINE         = SOC_AWR2544
DEVICE_TYPE             = SOC_AWR2544
PACKAGE_TYPE            = LOP
MCU_PLUS_INSTALL_PATH   = $(MCU_PLUS_AWR2544_INSTALL_PATH)
PLATFORM_OBJDIR         = obj_$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)