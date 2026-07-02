###################################################################################
# CFAR PROC Test
###################################################################################
.PHONY: cfarHwaDssTest cfarHwaDssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/cfarproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/randomdatagenerator
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
DSS_CPU := C66
DSS_CPU_INSTANCE := c66
endif

###################################################################################
# Libraries
###################################################################################
HWACFARPROC_DSS_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)	\
						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibcfarproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)
HWACFARPROC_DSS_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)	\
                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/cfarproc/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib


###################################################################################
# Unit Test Files
###################################################################################
HWACFARPROC_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWACFARPROC_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_cfarprochwa_dss.map
HWACFARPROC_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_cfarprochwa_dss.$($(DSS_CPU)_EXE_EXT)
HWACFARPROC_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)_cfarprochwa_dss.bin
HWACFARPROC_DSS_TEST_APP_CMD   = test/dss_cfarproc_linker.cmd
HWACFARPROC_DSS_TEST_SOURCES   = cfarprochwa_test_main.c \
								gen_rand_data.c \
								mathutils.c

HWACFARPROC_DSS_TEST_SOURCES_GEN  = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

HWACFARPROC_DSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWACFARPROC_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
HWACFARPROC_DSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWACFARPROC_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

HWACFARPROC_DSS_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(HWACFARPROC_DSS_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3


###################################################################################
# Build Unit Test:
###################################################################################
#cfarHwaDssTest: $(DSS_CPU)_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$($(DSS_CPU)_CFLAGS))
cfarHwaDssTest: buildDirectories dssbuildDirectories $(HWACFARPROC_DSS_TEST_OBJECTS) $(HWACFARPROC_DSS_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(HWACFARPROC_DSS_TEST_LOC_LIBS) $(HWACFARPROC_DSS_TEST_STD_LIBS) \
	--map_file=$(HWACFARPROC_DSS_TEST_MAP) $(HWACFARPROC_DSS_TEST_OBJECTS) $(HWACFARPROC_DSS_TEST_OBJECTS_GEN) \
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(HWACFARPROC_DSS_TEST_APP_CMD) -o $(HWACFARPROC_DSS_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the CFAR HWA DPU DSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
cfarHwaDssTestClean:
	@echo 'Cleaning the CFAR HWA DPU DSS Unit Test objects'
	@$(DEL) $(HWACFARPROC_DSS_TEST_OBJECTS) $(HWACFARPROC_DSS_TEST_OUT) $(HWACFARPROC_DSS_TEST_BIN)
	@$(DEL) $(HWACFARPROC_DSS_TEST_MAP) $(HWACFARPROC_DSS_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWACFARPROC_DSS_TEST_DEPENDS)
