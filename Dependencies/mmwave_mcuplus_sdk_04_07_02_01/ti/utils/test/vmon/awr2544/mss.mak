###################################################################################
# VMON Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/vmon
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/vmon/$(PLATFORM_DEVICE_TYPE)


MSS_CPU := R5F

###################################################################################
# Unit Test Files
###################################################################################
MSS_VMON_UNIT_TEST_MAP		 = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_vmon_mss.map
MSS_VMON_UNIT_TEST_OUT		 = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_vmon_mss.$($(MSS_CPU)_EXE_EXT)
MSS_VMON_UNIT_TEST_BIN		 = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_vmon_mss.bin
MSS_VMON_UNIT_TEST_APP_CMD	 = $(PLATFORM_DEVICE_TYPE)/mss_linker.cmd
MSS_VMON_UNIT_TEST_SOURCES	 = main_mss.c

MSS_VMON_UNIT_TEST_SOURCES_GEN = ti_board_config.c	\
								ti_board_open_close.c	\
								ti_dpl_config.c	\
								ti_drivers_config.c	\
								ti_pinmux_config.c	\
								ti_power_clock_config.c	\
								ti_drivers_open_close.c

MSS_VMON_UNIT_TEST_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_VMON_UNIT_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
MSS_VMON_UNIT_TEST_OBJECTS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_VMON_UNIT_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))
MSS_VMON_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MSS_VMON_UNIT_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: buildDirectories mssbuildDirectories $(MSS_VMON_UNIT_TEST_OBJECTS) $(MSS_VMON_UNIT_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $($(MSS_CPU)_COMMON_LOC_LIB) -Wl,-m=$(MSS_VMON_UNIT_TEST_MAP) \
	-o $(MSS_VMON_UNIT_TEST_OUT) $(MSS_VMON_UNIT_TEST_OBJECTS) $(MSS_VMON_UNIT_TEST_OBJECTS_GEN) \
	$($(MSS_CPU)_COMMON_STD_LIB) $(PLATFORM_$(MSS_CPU)_LINK_CMD) $(MSS_VMON_UNIT_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the VMON MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the VMON MSS Unit Test objects'
	@$(DEL) $(MSS_VMON_UNIT_TEST_OBJECTS) $(MSS_VMON_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(MSS_VMON_UNIT_TEST_OUT) $(MSS_VMON_UNIT_TEST_BIN)
	@$(DEL) $(MSS_VMON_UNIT_TEST_MAP) $(MSS_VMON_UNIT_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_VMON_UNIT_TEST_DEPENDS)
