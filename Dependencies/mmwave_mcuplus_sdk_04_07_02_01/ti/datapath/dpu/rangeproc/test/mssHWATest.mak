###################################################################################
# Range Proc HWA DPU MSS Unit Test
###################################################################################
.PHONY: mssHWATest mssHWATestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f
endif

###################################################################################
# Libraries
###################################################################################
RANGEPROC_UNIT_MSS_TEST_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)	\
						-llibrangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)
RANGEPROC_UNIT_MSS_TEST_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB)	\
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib

###################################################################################
# Unit Test Files
###################################################################################
RANGEPROC_UNIT_MSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROC_UNIT_MSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_mss.map
RANGEPROC_UNIT_MSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_mss.$($(MSS_CPU)_EXE_EXT)
RANGEPROC_UNIT_MSS_TEST_APP_CMD   = test/mss_linker.cmd
RANGEPROC_UNIT_MSS_TEST_SOURCES   = hwa_main.c  \
                                    dpedmahwa.c \
                                    dpedma.c \
                                    mathutils.c

RANGEPROC_UNIT_MSS_TEST_SOURCES_GEN  = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

RANGEPROC_UNIT_MSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_MSS_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
RANGEPROC_UNIT_MSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_MSS_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

RANGEPROC_UNIT_MSS_TEST_OBJECTS_GEN	 = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(RANGEPROC_UNIT_MSS_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
#mssHWATest: $(MSS_CPU)_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$($(MSS_CPU)_CFLAGS))
mssHWATest: buildDirectories mssbuildDirectories $(RANGEPROC_UNIT_MSS_TEST_OBJECTS) $(RANGEPROC_UNIT_MSS_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(RANGEPROC_UNIT_MSS_TEST_LOC_LIBS) -Wl,-m=$(RANGEPROC_UNIT_MSS_TEST_MAP) \
	-o $(RANGEPROC_UNIT_MSS_TEST_OUT) $(RANGEPROC_UNIT_MSS_TEST_OBJECTS) $(RANGEPROC_UNIT_MSS_TEST_OBJECTS_GEN) \
	$(RANGEPROC_UNIT_MSS_TEST_STD_LIBS) $(PLATFORM_$(MSS_CPU)_LINK_CMD) $(RANGEPROC_UNIT_MSS_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the Range Proc HWA DPU MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssHWATestClean:
	@echo 'Cleaning the Range Proc HWA DPU MSS Unit Test objects'
	@$(DEL) $(RANGEPROC_UNIT_MSS_TEST_OBJECTS) $(RANGEPROC_UNIT_MSS_TEST_OBJECTS_GEN)
	@$(DEL) $(RANGEPROC_UNIT_MSS_TEST_DEPENDS) $(RANGEPROC_UNIT_MSS_TEST_OUT)
	@$(DEL) $(RANGEPROC_UNIT_MSS_TEST_MAP) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROC_UNIT_MSS_TEST_DEPENDS)
