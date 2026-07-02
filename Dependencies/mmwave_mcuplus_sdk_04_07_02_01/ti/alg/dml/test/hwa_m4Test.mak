###################################################################################
# DML Test
###################################################################################
.PHONY: hwaM4Test hwaM4TestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/alg/dml/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/alg/dml/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2x44P), )
M4_CPU := M4
M4_CPU_INSTANCE := m4
endif

###################################################################################
# The DML Test requires additional libraries
###################################################################################
HWA_DML_M4_TEST_STD_LIBS = $($(M4_CPU)_COMMON_STD_LIB)	\
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(M4_CPU)_LIB_EXT) \
						-llibdml_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(M4_CPU)_LIB_EXT) \
						-llibmathutils.$($(M4_CPU)_LIB_EXT)
HWA_DML_M4_TEST_LOC_LIBS = $($(M4_CPU)_COMMON_LOC_LIB)	\
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/dml/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib

###################################################################################
# Unit Test Files
###################################################################################
HWA_DML_M4_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWA_DML_M4_TEST_MAP       = $(MMWAVE_SDK_DEVICE_TYPE)_dml_m4_test.map
HWA_DML_M4_TEST_OUT       = $(MMWAVE_SDK_DEVICE_TYPE)_dml_m4_test.$($(M4_CPU)_EXE_EXT)
HWA_DML_M4_TEST_BIN       = $(MMWAVE_SDK_DEVICE_TYPE)_dml_m4_test.bin
ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
HWA_DML_M4_TEST_APP_CMD   = m4_linker_awr2x44P.cmd
endif
HWA_DML_M4_TEST_SOURCES   =  dml_test.c

HWA_DML_M4_TEST_SOURCES_GEN  = ti_board_config.c	\
							ti_board_open_close.c	\
							ti_dpl_config.c	\
							ti_drivers_config.c	\
							ti_pinmux_config.c	\
							ti_power_clock_config.c	\
							ti_drivers_open_close.c

HWA_DML_M4_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_DML_M4_TEST_SOURCES:.c=.$($(M4_CPU)_DEP_EXT)))
HWA_DML_M4_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWA_DML_M4_TEST_SOURCES:.c=.$($(M4_CPU)_OBJ_EXT)))

HWA_DML_M4_TEST_OBJECTS_GEN	 = $(addprefix $(PLATFORM_OBJDIR)/m4generated/, $(HWA_DML_M4_TEST_SOURCES_GEN:.c=.$($(M4_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
hwaM4Test: $(M4_CPU)_CFLAGS += -DDATAPATH_TEST
hwaM4Test: buildDirectories m4buildDirectories $(HWA_DML_M4_TEST_OBJECTS) $(HWA_DML_M4_TEST_OBJECTS_GEN)
	$($(M4_CPU)_LD) $($(M4_CPU)_LDFLAGS) $(HWA_DML_M4_TEST_LOC_LIBS) -Wl,-m=$(HWA_DML_M4_TEST_MAP) \
	-o $(HWA_DML_M4_TEST_OUT) $(HWA_DML_M4_TEST_OBJECTS) $(HWA_DML_M4_TEST_OBJECTS_GEN) \
	$(HWA_DML_M4_TEST_STD_LIBS) $(PLATFORM_$(M4_CPU)_LINK_CMD) $(HWA_DML_M4_TEST_APP_CMD)
	@echo '******************************************************************************'
	@echo 'Built the DML M4 Unit Test '
	@echo '******************************************************************************'

###################################################################################
# Cleanup Unit Test:
###################################################################################
hwaM4TestClean:
	@echo 'Cleaning the DML M4 Unit Test objects'
	@$(DEL) $(HWA_DML_M4_TEST_OBJECTS) $(HWA_DML_M4_TEST_OUT) $(HWA_DML_M4_TEST_BIN)
	@$(DEL) $(HWA_DML_M4_TEST_MAP) $(HWA_DML_M4_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWA_DML_M4_TEST_DEPENDS)
