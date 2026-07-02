###################################################################################
#   DSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: dssDistributed dssDistributedClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/distributed/dpc/ll
vpath %.c test/distributed/dss

DSS_CPU := C66

###################################################################################
# Additional libraries which are required to build the Unit Test
###################################################################################
DSS_DISTRIBUTED_STD_LIBS =  $($(DSS_CPU)_COMMON_STD_LIB)	      							\
							-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)		\
							-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)
DSS_DISTRIBUTED_LOC_LIBS = 	$($(DSS_CPU)_COMMON_LOC_LIB)									\
							-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib			\
							-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# DSS Unit Test Definitions
###################################################################################
DSS_DISTRIBUTED_MAP       	= test/distributed/dss/$(MMWAVE_SDK_DEVICE_TYPE)_distributed_dss.map
DSS_DISTRIBUTED_OUT       	= test/distributed/dss/$(MMWAVE_SDK_DEVICE_TYPE)_distributed_dss.$($(DSS_CPU)_EXE_EXT)
DSS_DISTRIBUTED_CMD       	= test/distributed/dss/dss_linker.cmd
DSS_DISTRIBUTED_SOURCES   	= dss_distributed.c			\
							  ml_profile_distributed.c

DSS_DISTRIBUTED_SOURCES_GEN = ti_board_config.c \
							  ti_board_open_close.c \
							  ti_dpl_config.c \
							  ti_drivers_config.c \
							  ti_pinmux_config.c \
							  ti_power_clock_config.c \
							  ti_drivers_open_close.c

DSS_DISTRIBUTED_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_DISTRIBUTED_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
DSS_DISTRIBUTED_OBJECTS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_DISTRIBUTED_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

DSS_DISTRIBUTED_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(DSS_DISTRIBUTED_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build the DSS
###################################################################################
dssDistributed: buildDirectories dssbuildDirectories $(DSS_DISTRIBUTED_OBJECTS) $(DSS_DISTRIBUTED_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(DSS_DISTRIBUTED_LOC_LIBS) $(DSS_DISTRIBUTED_STD_LIBS) 		\
	--map_file=$(DSS_DISTRIBUTED_MAP)				\
	$(DSS_DISTRIBUTED_OBJECTS) $(DSS_DISTRIBUTED_OBJECTS_GEN) $(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(DSS_DISTRIBUTED_CMD) 			\
	-o $(DSS_DISTRIBUTED_OUT)
	@echo "***********************************"
	@echo 'Built the DSS Distributed Unit Test'
	@echo "***********************************"

###################################################################################
# Clean the DSS
###################################################################################
dssDistributedClean:
	@echo 'Cleaning the DSS Distributed Objects'
	@$(DEL) $(DSS_DISTRIBUTED_OBJECTS) $(DSS_DISTRIBUTED_OBJECTS_GEN) $(DSS_DISTRIBUTED_MAP) $(DSS_DISTRIBUTED_OUT)
	@$(DEL) $(DSS_DISTRIBUTED_BIN) $(DSS_DISTRIBUTED_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

objDistributedsstestClean:
	@echo 'Cleaning the DSS Distributed Objects'
	@$(DEL) $(DSS_DISTRIBUTED_OBJECTS) $(DSS_DISTRIBUTED_OBJECTS_GEN)
	@$(DEL) $(DSS_DISTRIBUTED_BIN) $(DSS_DISTRIBUTED_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_DISTRIBUTED_DEPENDS)

