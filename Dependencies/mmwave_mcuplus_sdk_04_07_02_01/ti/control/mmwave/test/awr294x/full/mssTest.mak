###################################################################################
# mmWave Full Unit Test on MSS Makefile
###################################################################################
.PHONY: mssFullTest mssFullTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c test/$(PLATFORM_DEVICE_TYPE)/full


MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f

###################################################################################
# The mmWave Unit Test requires additional libraries
###################################################################################
MMWAVE_FULL_MSS_UNIT_TEST_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB) \
									-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
									-lmmwavelink_$(MSS_CPU_INSTANCE).lib \
									-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)

MMWAVE_FULL_MSS_UNIT_TEST_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB) \
								-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
								-Wl,-i$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib \
								-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib   \


###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_FULL_MSS_UNIT_TEST_CMD		 = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MMWAVE_FULL_MSS_UNIT_TEST_MAP		 = test/$(PLATFORM_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_mss.map
MMWAVE_FULL_MSS_UNIT_TEST_OUT		 = test/$(PLATFORM_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_mss.$($(MSS_CPU)_EXE_EXT)
MMWAVE_FULL_MSS_UNIT_TEST_BIN		 = test/$(PLATFORM_DEVICE_TYPE)/full/$(MMWAVE_SDK_DEVICE_TYPE)_mmwave_full_mss.bin
MMWAVE_FULL_MSS_UNIT_TEST_APP_CMD	 = test/$(PLATFORM_DEVICE_TYPE)/full/mss_linker.cmd
MMWAVE_FULL_MSS_UNIT_TEST_SOURCES	 = main_full_mss.c \
										common_full.c

MMWAVE_FULL_MSS_UNIT_TEST_SOURCES_GEN = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

MMWAVE_FULL_MSS_UNIT_TEST_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_FULL_MSS_UNIT_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_FULL_MSS_UNIT_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MMWAVE_FULL_MSS_UNIT_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))


###################################################################################
# Build Unit Test:
###################################################################################
mssFullTest: buildDirectories mssbuildDirectories $(MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(MMWAVE_FULL_MSS_UNIT_TEST_LOC_LIBS) -Wl,-m=$(MMWAVE_FULL_MSS_UNIT_TEST_MAP) \
	-o $(MMWAVE_FULL_MSS_UNIT_TEST_OUT) $(MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS_GEN) $(MMWAVE_FULL_MSS_UNIT_TEST_STD_LIBS) \
	$(PLATFORM_$(MSS_CPU)_LINK_CMD) $(MMWAVE_FULL_MSS_UNIT_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the mmWave Full MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Unit Test Object Clean:
###################################################################################
mssFullTestObjClean:
	@echo 'Cleaning the mmWave Full MSS Unit Test objects'
	@rm -f $(MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS_GEN)
	@rm -f $(MMWAVE_FULL_MSS_UNIT_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssFullTestClean:
	@echo 'Cleaning the mmWave Full MSS Unit Test objects'
	@$(DEL) $(MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_FULL_MSS_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVE_FULL_MSS_UNIT_TEST_OUT) $(MMWAVE_FULL_MSS_UNIT_TEST_BIN)
	@$(DEL) $(MMWAVE_FULL_MSS_UNIT_TEST_MAP) $(MMWAVE_FULL_MSS_UNIT_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_FULL_MSS_UNIT_TEST_DEPENDS)

