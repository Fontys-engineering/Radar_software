###################################################################################
#   MSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: mssDistributed mssDistributedClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/distributed/mss
vpath %.c test/distributed/dpc/ll
vpath %.c test/distributed/dpc/hl

MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
MMWAVELINK_LIB_PATH = -Wl,-i$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib
else
$(error Please set the environment variable MMWAVE_SDK_DEVICE.)
endif


###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
MSS_DISTRIBUTED_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)						\
							-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)		\
							-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
							-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
							-lmmwavelink_$(MSS_CPU_INSTANCE).lib
MSS_DISTRIBUTED_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB) \
							-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
							-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib \
							-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib \
							-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# MSS Definitions:
###################################################################################
MSS_DISTRIBUTED_MAP       = test/distributed/mss/$(MMWAVE_SDK_DEVICE_TYPE)_distributed_mss.map
MSS_DISTRIBUTED_OUT       = test/distributed/mss/$(MMWAVE_SDK_DEVICE_TYPE)_distributed_mss.$($(MSS_CPU)_EXE_EXT)
MSS_DISTRIBUTED_CMD       = test/distributed/mss/mss_linker.cmd
MSS_DISTRIBUTED_SOURCES   = mss_distributed.c \
							hl_profile_distributed.c \
							ll_profile_distributed.c \

MSS_DISTRIBUTED_SOURCES_GEN = ti_board_config.c \
							  ti_board_open_close.c \
							  ti_dpl_config.c \
							  ti_drivers_config.c \
							  ti_pinmux_config.c \
							  ti_power_clock_config.c \
							  ti_drivers_open_close.c

MSS_DISTRIBUTED_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_DISTRIBUTED_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
MSS_DISTRIBUTED_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_DISTRIBUTED_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

MSS_DISTRIBUTED_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MSS_DISTRIBUTED_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build the MSS
###################################################################################
mssDistributed: buildDirectories mssbuildDirectories $(MSS_DISTRIBUTED_OBJECTS) $(MSS_DISTRIBUTED_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(MSS_DISTRIBUTED_LOC_LIBS) $(MMWAVELINK_LIB_PATH) -Wl,-m=$(MSS_DISTRIBUTED_MAP) \
	-o $(MSS_DISTRIBUTED_OUT) $(MSS_DISTRIBUTED_OBJECTS) $(MSS_DISTRIBUTED_OBJECTS_GEN) $(MSS_DISTRIBUTED_STD_LIBS) \
	$(PLATFORM_$(MSS_CPU)_LINK_CMD) $(MSS_DISTRIBUTED_CMD)
	@echo "***********************************"
	@echo 'Built the MSS Distributed Unit Test'
	@echo "***********************************"

###################################################################################
# Clean the MSS
###################################################################################
mssDistributedClean:
	@echo 'Cleaning the MSS Distributed Objects'
	@$(DEL) $(MSS_DISTRIBUTED_OBJECTS) $(MSS_DISTRIBUTED_OBJECTS_GEN) $(MSS_DISTRIBUTED_MAP) $(MSS_DISTRIBUTED_OUT)
	@$(DEL) $(MSS_DISTRIBUTED_BIN) $(MSS_DISTRIBUTED_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

objDistributemsstestClean:
	@echo 'Cleaning the MSS Distributed Objects'
	@$(DEL) $(MSS_DISTRIBUTED_OBJECTS) $(MSS_DISTRIBUTED_OBJECTS_GEN)
	@$(DEL) $(MSS_DISTRIBUTED_BIN) $(MSS_DISTRIBUTED_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_DISTRIBUTED_DEPENDS)
