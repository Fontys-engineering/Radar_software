###################################################################################
# LVDS Stream Unit Test on DSS Makefile
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/cbuff_stream
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/cbuff_stream/$(PLATFORM_DEVICE_TYPE)

DSS_CPU := C66
DSS_CPU_INSTANCE := c66

###################################################################################
# The LVDS Stream Test requires additional libraries
###################################################################################
DSS_LVDS_STREAM_UNIT_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)	\
									-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)	\
									-lmmwavelink_$(DSS_CPU_INSTANCE).lib	\
									-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)	\
									-llibhsiheader_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)

DSS_LVDS_STREAM_UNIT_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)	\
									-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
									-i$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib	\
									-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib	\
									-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/hsiheader/lib

###################################################################################
# Unit Test Files
###################################################################################
DSS_LVDS_STREAM_UNIT_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
DSS_LVDS_STREAM_UNIT_TEST_MAP       = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_lvds_stream_dss.map
DSS_LVDS_STREAM_UNIT_TEST_OUT       = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_lvds_stream_dss.$($(DSS_CPU)_EXE_EXT)
DSS_LVDS_STREAM_UNIT_TEST_BIN       = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_lvds_stream_dss.bin
DSS_LVDS_STREAM_UNIT_TEST_APP_CMD   = $(PLATFORM_DEVICE_TYPE)/dss_linker.cmd
DSS_LVDS_STREAM_UNIT_TEST_SOURCES   = main_dss.c \
									  lvds_stream.c

DSS_LVDS_STREAM_UNIT_TEST_SOURCES_GEN  = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

DSS_LVDS_STREAM_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_LVDS_STREAM_UNIT_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
DSS_LVDS_STREAM_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_LVDS_STREAM_UNIT_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

DSS_LVDS_STREAM_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(DSS_LVDS_STREAM_UNIT_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: buildDirectories dssbuildDirectories $(DSS_LVDS_STREAM_UNIT_TEST_OBJECTS) $(DSS_LVDS_STREAM_UNIT_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(DSS_LVDS_STREAM_UNIT_TEST_LOC_LIBS) $(DSS_LVDS_STREAM_UNIT_TEST_STD_LIBS) 	\
	--map_file=$(DSS_LVDS_STREAM_UNIT_TEST_MAP) $(DSS_LVDS_STREAM_UNIT_TEST_OBJECTS) $(DSS_LVDS_STREAM_UNIT_TEST_OBJECTS_GEN)	\
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(DSS_LVDS_STREAM_UNIT_TEST_APP_CMD) -o $(DSS_LVDS_STREAM_UNIT_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the LVDS Stream DSS Unit Test'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the LVDS Stream DSS Unit Test objects'
	@$(DEL) $(DSS_LVDS_STREAM_UNIT_TEST_OBJECTS) $(DSS_LVDS_STREAM_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(DSS_LVDS_STREAM_UNIT_TEST_OUT)
	@$(DEL) $(DSS_LVDS_STREAM_UNIT_TEST_BIN) $(DSS_LVDS_STREAM_UNIT_TEST_DEPENDS)
	@$(DEL) $(DSS_LVDS_STREAM_UNIT_TEST_MAP)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_LVDS_STREAM_UNIT_TEST_DEPENDS)

