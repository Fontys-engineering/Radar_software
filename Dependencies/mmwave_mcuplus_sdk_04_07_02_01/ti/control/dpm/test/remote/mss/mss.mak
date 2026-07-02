###################################################################################
#   MSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: mssRemote mssRemoteClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/remote/mss
vpath %.c test/remote/dpc/hl

MSS_CPU := R5F

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
MSS_REMOTE_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)									\
					-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)		\
					-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)
MSS_REMOTE_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB) \
					-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib \
					-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# MSS Definitions:
###################################################################################
MSS_REMOTE_MAP       = test/remote/mss/$(MMWAVE_SDK_DEVICE_TYPE)_remote_mss.map
MSS_REMOTE_OUT       = test/remote/mss/$(MMWAVE_SDK_DEVICE_TYPE)_remote_mss.$($(MSS_CPU)_EXE_EXT)
MSS_REMOTE_CMD       = test/remote/mss/mss_linker.cmd
MSS_REMOTE_SOURCES   = mss_remote.c \
					   hl_profile_remote.c

MSS_REMOTE_SOURCES_GEN = ti_board_config.c \
						ti_board_open_close.c \
						ti_dpl_config.c \
						ti_drivers_config.c \
						ti_pinmux_config.c \
						ti_power_clock_config.c \
						ti_drivers_open_close.c

MSS_REMOTE_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_REMOTE_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
MSS_REMOTE_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_REMOTE_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

MSS_REMOTE_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MSS_REMOTE_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build the MSS
###################################################################################
mssRemote: buildDirectories mssbuildDirectories $(MSS_REMOTE_OBJECTS) $(MSS_REMOTE_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(MSS_REMOTE_LOC_LIBS) -Wl,-m=$(MSS_REMOTE_MAP) \
	-o $(MSS_REMOTE_OUT) $(MSS_REMOTE_OBJECTS) $(MSS_REMOTE_OBJECTS_GEN) $(MSS_REMOTE_STD_LIBS) \
	$(PLATFORM_$(MSS_CPU)_LINK_CMD) $(MSS_REMOTE_CMD)
	@echo "******************************"
	@echo 'Built the MSS Remote Unit Test'
	@echo "******************************"

###################################################################################
# Clean the MSS
###################################################################################
mssRemoteClean:
	@echo 'Cleaning the MSS Remote Objects'
	@$(DEL) $(MSS_REMOTE_OBJECTS) $(MSS_REMOTE_MAP) $(MSS_REMOTE_OUT) $(MSS_REMOTE_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

objRemotemsstestClean:
	@echo 'Cleaning the MSS Remote Objects'
	@$(DEL) $(MSS_REMOTE_OBJECTS) $(MSS_REMOTE_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_REMOTE_DEPENDS)


