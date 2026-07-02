###################################################################################
# Unit Test on Range Proc HWA DPU Makefile
###################################################################################
.PHONY: dssHWATest dssHWATestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangecfarprocDDMA/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
DSS_CPU := C66
DSS_CPU_INSTANCE := c66
endif

###################################################################################
# Libraries
###################################################################################
RANGECFARPROC_UNIT_DSS_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)	\
								-llibrangecfarproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
								-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)
RANGECFARPROC_UNIT_DSS_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)	\
								   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangecfarprocDDMA/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib

###################################################################################
# Unit Test Files
###################################################################################
RANGECFARPROC_UNIT_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
RANGECFARPROC_UNIT_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangecfarprochwa_DDMA_dss.map
RANGECFARPROC_UNIT_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangecfarprochwa_DDMA_dss.$($(DSS_CPU)_EXE_EXT)
RANGECFARPROC_UNIT_DSS_TEST_APP_CMD   = test/dss_linker.cmd
RANGECFARPROC_UNIT_DSS_TEST_SOURCES   = rangecfarprochwa_test_main.c \
										mathutils.c

RANGECFARPROC_UNIT_DSS_TEST_SOURCES_GEN  = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

RANGECFARPROC_UNIT_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGECFARPROC_UNIT_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
RANGECFARPROC_UNIT_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGECFARPROC_UNIT_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

RANGECFARPROC_UNIT_DSS_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(RANGECFARPROC_UNIT_DSS_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
dssHWATest: buildDirectories dssbuildDirectories $(RANGECFARPROC_UNIT_DSS_TEST_OBJECTS) $(RANGECFARPROC_UNIT_DSS_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(RANGECFARPROC_UNIT_DSS_TEST_LOC_LIBS) $(RANGECFARPROC_UNIT_DSS_TEST_STD_LIBS)	\
	--map_file=$(RANGECFARPROC_UNIT_DSS_TEST_MAP) $(RANGECFARPROC_UNIT_DSS_TEST_OBJECTS) $(RANGECFARPROC_UNIT_DSS_TEST_OBJECTS_GEN)	\
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(RANGECFARPROC_UNIT_DSS_TEST_APP_CMD) -o $(RANGECFARPROC_UNIT_DSS_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the Range Cfar Proc HWA DPU DSS Unit Test'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssHWATestClean:
	@echo 'Cleaning the Range Cfar Proc HWA DPU DSS Unit Test objects'
	@$(DEL) $(RANGECFARPROC_UNIT_DSS_TEST_OBJECTS) $(RANGECFARPROC_UNIT_DSS_TEST_OBJECTS_GEN) $(RANGECFARPROC_UNIT_DSS_TEST_OUT)
	@$(DEL) $(RANGECFARPROC_UNIT_DSS_TEST_MAP) $(RANGECFARPROC_UNIT_DSS_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGECFARPROC_UNIT_DSS_TEST_DEPENDS)
