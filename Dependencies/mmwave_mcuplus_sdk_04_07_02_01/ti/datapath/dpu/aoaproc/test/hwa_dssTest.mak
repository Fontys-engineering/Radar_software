###################################################################################
# AOA PROC Test
###################################################################################
.PHONY: aoaHwaDssTest aoaHwaDssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/aoaproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/randomdatagenerator
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src


DSS_CPU := C66
DSS_CPU_INSTANCE := c66

###################################################################################
# The AOA PROC Test requires additional libraries
###################################################################################
HWAAOAPROC_DSS_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)	\
						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)
HWAAOAPROC_DSS_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)	\
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/aoaproc/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib


###################################################################################
# Unit Test Files
###################################################################################
HWAAOAPROC_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWAAOAPROC_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_aoaprochwa_dss.map
HWAAOAPROC_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_aoaprochwa_dss.$($(DSS_CPU)_EXE_EXT)
HWAAOAPROC_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)_aoaprochwa_dss.bin
HWAAOAPROC_DSS_TEST_APP_CMD   = test/dss_aoaproc_linker.cmd
HWAAOAPROC_DSS_TEST_SOURCES   = aoaprochwa_test_main.c \
								gen_rand_data.c \
								mathutils.c 

HWAAOAPROC_DSS_TEST_SOURCES_GEN   = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

HWAAOAPROC_DSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWAAOAPROC_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
HWAAOAPROC_DSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWAAOAPROC_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

HWAAOAPROC_DSS_TEST_OBJECTS_GEN	 = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(HWAAOAPROC_DSS_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
aoaHwaDssTest: buildDirectories dssbuildDirectories $(HWAAOAPROC_DSS_TEST_OBJECTS) $(HWAAOAPROC_DSS_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(HWAAOAPROC_DSS_TEST_LOC_LIBS) $(HWAAOAPROC_DSS_TEST_STD_LIBS) \
	--map_file=$(HWAAOAPROC_DSS_TEST_MAP) $(HWAAOAPROC_DSS_TEST_OBJECTS) $(HWAAOAPROC_DSS_TEST_OBJECTS_GEN) \
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(HWAAOAPROC_DSS_TEST_APP_CMD) -o $(HWAAOAPROC_DSS_TEST_OUT)

	@echo "******************************************************************************"
	@echo 'Built the aoa proc DSP Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
aoaHwaDssTestClean:
	@echo 'Cleaning the AOA PROC DSP Unit Test objects'
	@$(DEL) $(HWAAOAPROC_DSS_TEST_OBJECTS) $(HWAAOAPROC_DSS_TEST_OBJECTS_GEN) $(HWAAOAPROC_DSS_TEST_OUT) $(HWAAOAPROC_DSS_TEST_BIN)
	@$(DEL) $(HWAAOAPROC_DSS_TEST_MAP) $(HWAAOAPROC_DSS_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWAAOAPROC_DSS_TEST_DEPENDS)

