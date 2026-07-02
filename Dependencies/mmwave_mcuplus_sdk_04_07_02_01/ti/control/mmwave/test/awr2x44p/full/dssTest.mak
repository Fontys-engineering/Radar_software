###################################################################################
# mmWave Full Unit Test on DSS Makefile
###################################################################################
.PHONY: dssFullTest dssFullTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(PLATFORM_DEVICE_TYPE)/full

DSS_CPU := C66
DSS_CPU_INSTANCE := c66

###################################################################################
# The mmWave Unit test requires additional libraries
###################################################################################
MMWAVE_FULL_DSS_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB) \
                                -lmmwavelink_$(DSS_CPU_INSTANCE).lib \
                                -llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)

MMWAVE_FULL_DSS_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB) \
                               -i$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib \
                               -i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib

###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_FULL_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_FULL_DSS_TEST_MAP       = test/$(PLATFORM_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_dss.map
MMWAVE_FULL_DSS_TEST_OUT       = test/$(PLATFORM_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_dss.$($(DSS_CPU)_EXE_EXT)
MMWAVE_FULL_DSS_TEST_BIN       = test/$(PLATFORM_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_dss.bin
MMWAVE_FULL_DSS_TEST_SOURCES   = main_full_dss.c \
                                 common_full.c

MMWAVE_FULL_DSS_TEST_SOURCES_GEN  = ti_board_config.c	\
                                    ti_board_open_close.c	\
                                    ti_dpl_config.c	\
                                    ti_drivers_config.c	\
                                    ti_pinmux_config.c	\
                                    ti_power_clock_config.c	\
                                    ti_drivers_open_close.c

MMWAVE_FULL_DSS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_FULL_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
MMWAVE_FULL_DSS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_FULL_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

MMWAVE_FULL_DSS_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVE_FULL_DSS_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
dssFullTest: buildDirectories dssbuildDirectories $(MMWAVE_FULL_DSS_TEST_OBJECTS) $(MMWAVE_FULL_DSS_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(MMWAVE_FULL_DSS_TEST_LOC_LIBS) $(MMWAVE_FULL_DSS_TEST_STD_LIBS) 	\
	--map_file=$(MMWAVE_FULL_DSS_TEST_MAP) $(MMWAVE_FULL_DSS_TEST_OBJECTS) $(MMWAVE_FULL_DSS_TEST_OBJECTS_GEN)	\
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) -o $(MMWAVE_FULL_DSS_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmWave Full DSS Unit Test'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssFullTestClean:
	@echo 'Cleaning the mmWave Full DSS Unit Test objects'
	@$(DEL) $(MMWAVE_FULL_DSS_TEST_OBJECTS) $(MMWAVE_FULL_DSS_TEST_OBJECTS_GEN) $(MMWAVE_FULL_DSS_TEST_OUT)
	@$(DEL) $(MMWAVE_FULL_DSS_TEST_BIN) $(MMWAVE_FULL_DSS_TEST_DEPENDS)
	@$(DEL) $(MMWAVE_FULL_DSS_TEST_MAP)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_FULL_DSS_TEST_DEPENDS)

