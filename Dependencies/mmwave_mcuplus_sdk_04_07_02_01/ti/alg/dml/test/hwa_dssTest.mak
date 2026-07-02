###################################################################################
# DML PROC Test
###################################################################################
.PHONY: hwaDssTest hwaDssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/alg/dml/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/alg/dml/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944 awr2x44P), )
DSS_CPU := C66
DSS_CPU_INSTANCE := c66
endif

###################################################################################
# The DML Test requires additional libraries
###################################################################################
HWA_DML_DSS_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)	\
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibdml_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibmathutils.$($(DSS_CPU)_LIB_EXT)
HWA_DML_DSS_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)	\
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/dml/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib

###################################################################################
# Unit Test Files
###################################################################################
HWA_DML_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWA_DML_DSS_TEST_MAP       = $(MMWAVE_SDK_DEVICE_TYPE)_dml_dss_test.map
HWA_DML_DSS_TEST_OUT       = $(MMWAVE_SDK_DEVICE_TYPE)_dml_dss_test.$($(DSS_CPU)_EXE_EXT)
HWA_DML_DSS_TEST_BIN       = $(MMWAVE_SDK_DEVICE_TYPE)_dml_dss_test.bin
HWA_DML_DSS_TEST_APP_CMD   = dss_dml_test_linker.cmd
HWA_DML_DSS_TEST_SOURCES   = dml_test.c

HWA_DML_DSS_TEST_SOURCES_GEN  = ti_board_config.c	\
								ti_board_open_close.c	\
								ti_dpl_config.c	\
								ti_drivers_config.c	\
								ti_pinmux_config.c	\
								ti_power_clock_config.c	\
								ti_drivers_open_close.c

HWA_DML_DSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_DML_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
HWA_DML_DSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_DML_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

HWA_DML_DSS_TEST_OBJECTS_GEN	 = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(HWA_DML_DSS_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
hwaDssTest: $(DSS_CPU)_CFLAGS += --define=DATAPATH_TEST
hwaDssTest: buildDirectories dssbuildDirectories $(HWA_DML_DSS_TEST_OBJECTS) $(HWA_DML_DSS_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(HWA_DML_DSS_TEST_LOC_LIBS) $(HWA_DML_DSS_TEST_STD_LIBS) \
	--map_file=$(HWA_DML_DSS_TEST_MAP) $(HWA_DML_DSS_TEST_OBJECTS) $(HWA_DML_DSS_TEST_OBJECTS_GEN) \
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(HWA_DML_DSS_TEST_APP_CMD) -o $(HWA_DML_DSS_TEST_OUT)
#	$($(DSS_CPU)_LD_RTS_FLAGS) -o $(HWA_DML_DSS_TEST_OUT)
	@echo '******************************************************************************'
	@echo 'Built the DML DSP Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
hwaDssTestClean:
	@echo 'Cleaning the DML DSP Unit Test objects'
	@$(DEL) $(HWA_DML_DSS_TEST_OBJECTS) $(HWA_DML_DSS_TEST_OUT) $(HWA_DML_DSS_TEST_BIN)
	@$(DEL) $(HWA_DML_DSS_TEST_MAP) $(HWA_DML_DSS_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWA_DML_DSS_TEST_DEPENDS)
