###################################################################################
#	makefile for music Unit Test on DSP Sub System
###################################################################################

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

.PHONY: test testClean dssTest dssTestClean syscfgClean syscfg

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c

###################################################################################
# Unit Test Files
###################################################################################
MUSICLIB_UNIT_TEST_MAP		 = $(MMWAVE_SDK_DEVICE_TYPE)_music_dsstest.map
MUSICLIB_UNIT_TEST_OUT		 = $(MMWAVE_SDK_DEVICE_TYPE)_music_dsstest.$(C66_EXE_EXT)

MUSICLIB_UNIT_TEST_SOURCES  = 	music_test_main.c

MUSICLIB_UNIT_TEST_SOURCES_GEN  = dssgenerated/ti_board_config.c \
								 dssgenerated/ti_board_open_close.c \
								 dssgenerated/ti_dpl_config.c \
								 dssgenerated/ti_drivers_config.c \
								 dssgenerated/ti_pinmux_config.c \
								 dssgenerated/ti_power_clock_config.c \
								 dssgenerated/ti_drivers_open_close.c

MUSICLIB_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MUSICLIB_UNIT_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MUSICLIB_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MUSICLIB_UNIT_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))

MUSICLIB_UNIT_TEST_DEPENDS_GEN = $(addprefix $(PLATFORM_OBJDIR)/, $(MUSICLIB_UNIT_TEST_SOURCES_GEN:.c=.$(C66_DEP_EXT)))
MUSICLIB_UNIT_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/, $(MUSICLIB_UNIT_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))


###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
C66_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/music/lib  \
				-llibmusic_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)    \

###################################################################################
# Build Syscfg:
###################################################################################
syscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output dssgenerated/ music_test_$(PLATFORM_DEVICE_TYPE).syscfg

syscfg: music_test_$(PLATFORM_DEVICE_TYPE).syscfg
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output dssgenerated/ music_test_$(PLATFORM_DEVICE_TYPE).syscfg


# syscfgClean: This cleans syscfg files
syscfgClean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) dssgenerated

##################################################################################
# Test targets which are device specific
##################################################################################

testClean: syscfgClean dssTestClean
test:      syscfg dssTest

###################################################################################
# Build Test:
###################################################################################
dssTest: buildDirectories dssbuildDirectories $(MUSICLIB_UNIT_TEST_OBJECTS) $(MUSICLIB_UNIT_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MUSICLIB_UNIT_TEST_MAP) $(MUSICLIB_UNIT_TEST_OBJECTS) $(MUSICLIB_UNIT_TEST_OBJECTS_GEN) \
	$(PLATFORM_C66X_LINK_CMD) \
	-o $(MUSICLIB_UNIT_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the musiclib Test OUT'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the test objects'
	@$(DEL) $(MUSICLIB_UNIT_TEST_OBJECTS) $(MUSICLIB_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(MUSICLIB_UNIT_TEST_OUT) $(MUSICLIB_UNIT_TEST_DEPENDS) $(MUSICLIB_UNIT_TEST_DEPENDS_GEN)
	@$(DEL) $(MUSICLIB_UNIT_TEST_MAP) $(PLATFORM_OBJDIR)

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for musiclib  test '
	@echo 'testClean             -> Clean out all the syscfg files, test objects'
	@echo 'test                  -> Build the syscfg and test'
	@echo 'syscfg                -> Generate the syscfg files'
	@echo 'syscfg-gui            -> Open the syscfg gui'
	@echo "****************************************************************************************"

# Dependency handling
-include $(MUSICLIB_UNIT_TEST_DEPENDS) $(MUSICLIB_UNIT_TEST_DEPENDS_GEN)
