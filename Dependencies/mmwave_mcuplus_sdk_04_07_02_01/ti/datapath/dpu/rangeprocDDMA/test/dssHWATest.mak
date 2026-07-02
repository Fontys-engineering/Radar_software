###################################################################################
# Unit Test on Range Proc HWA DPU Makefile
###################################################################################
.PHONY: dssHWATest dssHWATestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeprocDDMA/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
DSS_CPU := C66
DSS_CPU_INSTANCE := c66
endif

###################################################################################
# Libraries
###################################################################################
RANGEPROC_UNIT_DSS_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)
RANGEPROC_UNIT_DSS_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)

###################################################################################
# Unit Test Files
###################################################################################
RANGEPROC_UNIT_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROC_UNIT_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_DDMA_dss.map
RANGEPROC_UNIT_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_DDMA_dss.$($(DSS_CPU)_EXE_EXT)
RANGEPROC_UNIT_DSS_TEST_APP_CMD   = test/dss_linker.cmd
RANGEPROC_UNIT_DSS_TEST_SOURCES   = hwa_main.c \
								    rangeprochwaDDMA.c \
                                    dpedmahwa.c \
                                    dpedma.c \
                                    mathutils.c

RANGEPROC_UNIT_DSS_TEST_SOURCES_GEN  = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

RANGEPROC_UNIT_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
RANGEPROC_UNIT_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

RANGEPROC_UNIT_DSS_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(RANGEPROC_UNIT_DSS_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
dssHWATest: $(DSS_CPU)_CFLAGS += --define=DATAPATH_TEST
dssHWATest: buildDirectories dssbuildDirectories $(RANGEPROC_UNIT_DSS_TEST_OBJECTS) $(RANGEPROC_UNIT_DSS_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(RANGEPROC_UNIT_DSS_TEST_LOC_LIBS) $(RANGEPROC_UNIT_DSS_TEST_STD_LIBS)	\
	--map_file=$(RANGEPROC_UNIT_DSS_TEST_MAP) $(RANGEPROC_UNIT_DSS_TEST_OBJECTS) $(RANGEPROC_UNIT_DSS_TEST_OBJECTS_GEN)	\
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(RANGEPROC_UNIT_DSS_TEST_APP_CMD) -o $(RANGEPROC_UNIT_DSS_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the Range Proc HWA DPU DSS Unit Test'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssHWATestClean:
	@echo 'Cleaning the Range Proc HWA DPU DSS Unit Test objects'
	@$(DEL) $(RANGEPROC_UNIT_DSS_TEST_OBJECTS) $(RANGEPROC_UNIT_DSS_TEST_OBJECTS_GEN) $(RANGEPROC_UNIT_DSS_TEST_OUT)
	@$(DEL) $(RANGEPROC_UNIT_DSS_TEST_MAP) $(RANGEPROC_UNIT_DSS_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROC_UNIT_DSS_TEST_DEPENDS)
