###################################################################################
#   MSS Unit Test
#
#  NOTE:
#      (C) Copyright 2018 Texas Instruments, Inc.
###################################################################################
.PHONY: mssLocal mssLocalClean

###################################################################################
# Setup the VPATH:
###################################################################################
VPATH :=
vpath %.c test/local/mss
vpath %.c test/local/dpc/ll

MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f
MSS_CPU_INSTANCE_NUM := mcu1_0

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
MMWAVELINK_PATH = $(MMWAVE_AWR294X_DFP_INSTALL_PATH)
else
$(error Please set the environment variable MMWAVE_SDK_DEVICE.)
endif

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
MSS_LOCAL_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)									\
					-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)		\
					-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)
MSS_LOCAL_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB) \
					-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib \
					-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# MSS Definitions:
###################################################################################
MSS_LOCAL_MAP       = test/local/mss/$(MMWAVE_SDK_DEVICE_TYPE)_local_mss.map
MSS_LOCAL_OUT       = test/local/mss/$(MMWAVE_SDK_DEVICE_TYPE)_local_mss.$($(MSS_CPU)_EXE_EXT)
MSS_LOCAL_CMD       = test/local/mss/mss_linker.cmd
MSS_LOCAL_SOURCES   = mss_local.c \
					  ll_profile_local.c

MSS_LOCAL_SOURCES_GEN = ti_board_config.c \
						ti_board_open_close.c \
						ti_dpl_config.c \
						ti_drivers_config.c \
						ti_pinmux_config.c \
						ti_power_clock_config.c \
						ti_drivers_open_close.c

MSS_LOCAL_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_LOCAL_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
MSS_LOCAL_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_LOCAL_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

#MSS_LOCAL_DEPENDS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated, $(MSS_LOCAL_SOURCES_GEN:.c=.$($(MSS_CPU)_DEP_EXT)))
MSS_LOCAL_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MSS_LOCAL_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build the MSS
###################################################################################
mssLocal: buildDirectories mssbuildDirectories $(MSS_LOCAL_OBJECTS) $(MSS_LOCAL_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(MSS_LOCAL_LOC_LIBS) -Wl,-m=$(MSS_LOCAL_MAP) \
	-o $(MSS_LOCAL_OUT) $(MSS_LOCAL_OBJECTS) $(MSS_LOCAL_OBJECTS_GEN) $(MSS_LOCAL_STD_LIBS) \
	$(PLATFORM_$(MSS_CPU)_LINK_CMD) $(MSS_LOCAL_CMD)
	@echo "*****************************"
	@echo 'Built the MSS Local Unit Test'
	@echo "*****************************"

###################################################################################
# Clean the MSS
###################################################################################
mssLocalClean:
	@echo 'Cleaning the MSS Local Objects'
	@$(DEL) $(MSS_LOCAL_OBJECTS) $(MSS_LOCAL_MAP) $(MSS_LOCAL_OUT)
	@$(DEL) $(MSS_LOCAL_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

objLocaltestClean:
	@echo 'Cleaning the MSS Local Objects'
	@$(DEL) $(MSS_LOCAL_OBJECTS)
	@$(DEL) $(MSS_LOCAL_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_LOCAL_DEPENDS)
