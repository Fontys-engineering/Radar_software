###################################################################################
#   DSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: dssRemote dssRemoteClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/remote/dss
vpath %.c test/remote/dpc/ll

DSS_CPU := C66

###################################################################################
# Additional libraries which are required to build the Unit Test
###################################################################################
DSS_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)		\
			   -llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)		\
			   -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)
DSS_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)									\
			   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib			\
			   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# DSS Unit Test Definitions
###################################################################################
DSS_MAP       	= test/remote/dss/$(MMWAVE_SDK_DEVICE_TYPE)_remote_dss.map
DSS_OUT       	= test/remote/dss/$(MMWAVE_SDK_DEVICE_TYPE)_remote_dss.$($(DSS_CPU)_EXE_EXT)
DSS_CMD       	= test/remote/dss/dss_linker.cmd
DSS_SOURCES   	= dss_remote.c			\
				  ll_profile_remote.c

DSS_SOURCES_GEN = ti_board_config.c \
				  ti_board_open_close.c \
				  ti_dpl_config.c \
				  ti_drivers_config.c \
				  ti_pinmux_config.c \
				  ti_power_clock_config.c \
				  ti_drivers_open_close.c

DSS_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
DSS_OBJECTS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

DSS_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(DSS_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build the DSS
###################################################################################
dssRemote: buildDirectories dssbuildDirectories $(DSS_OBJECTS) $(DSS_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(DSS_LOC_LIBS) $(DSS_STD_LIBS) 			\
	--map_file=$(DSS_MAP) $(DSS_OBJECTS) $(DSS_OBJECTS_GEN)	\
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(DSS_CMD) -o $(DSS_OUT)
	@echo "******************************"
	@echo 'Built the DSS Remote Unit Test'
	@echo "******************************"

###################################################################################
# Clean the DSS
###################################################################################
dssRemoteClean:
	@echo 'Cleaning the DSS Remote Objects'
	@$(DEL) $(DSS_OBJECTS) $(DSS_OBJECTS_GEN) $(DSS_MAP) $(DSS_OUT) $(DSS_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

objRemotedsstestClean:
	@echo 'Cleaning the DSS Remote Objects'
	@$(DEL) $(DSS_OBJECTS) $(DSS_OBJECTS_GEN) $(DSS_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_DEPENDS)


