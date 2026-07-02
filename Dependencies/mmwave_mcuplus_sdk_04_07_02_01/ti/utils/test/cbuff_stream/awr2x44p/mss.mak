###################################################################################
# LVDS Stream Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/cbuff_stream
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/cbuff_stream/$(PLATFORM_DEVICE_TYPE)

###################################################################################
# The LVDS Stream Test requires additional libraries
###################################################################################
MSS_LVDS_STREAM_UNIT_TEST_STD_LIBS = $(R5F_COMMON_STD_LIB)	\
                                    -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)	\
                                    -lmmwavelink_r5f.lib \
                                    -llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)	\
                                    -llibhsiheader_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)

MSS_LVDS_STREAM_UNIT_TEST_LOC_LIBS = $(R5F_COMMON_LOC_LIB) 	\
                                     -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
                                     -Wl,-i$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib	\
                                     -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib	\
                                     -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/hsiheader/lib

###################################################################################
# Unit Test Files
###################################################################################
MSS_LVDS_STREAM_UNIT_TEST_CMD        = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MSS_LVDS_STREAM_UNIT_TEST_MAP        = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_lvds_stream_mss.map
MSS_LVDS_STREAM_UNIT_TEST_OUT        = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_lvds_stream_mss.$(R5F_EXE_EXT)
MSS_LVDS_STREAM_UNIT_TEST_BIN        = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_lvds_stream_mss.bin
MSS_LVDS_STREAM_UNIT_TEST_APP_CMD	 = $(PLATFORM_DEVICE_TYPE)/mss_linker.cmd
MSS_LVDS_STREAM_UNIT_TEST_SOURCES    = main_mss.c \
                                       lvds_stream.c

MSS_LVDS_STREAM_UNIT_TEST_SOURCES_GEN = ti_board_config.c \
                                        ti_board_open_close.c \
                                        ti_dpl_config.c \
                                        ti_drivers_config.c \
                                        ti_pinmux_config.c \
                                        ti_power_clock_config.c \
                                        ti_drivers_open_close.c

MSS_LVDS_STREAM_UNIT_TEST_DEPENDS    = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_LVDS_STREAM_UNIT_TEST_SOURCES:.c=.$(R5F_DEP_EXT)))
MSS_LVDS_STREAM_UNIT_TEST_OBJECTS    = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_LVDS_STREAM_UNIT_TEST_SOURCES:.c=.$(R5F_OBJ_EXT)))

MSS_LVDS_STREAM_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MSS_LVDS_STREAM_UNIT_TEST_SOURCES_GEN:.c=.$(R5F_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: buildDirectories mssbuildDirectories $(MSS_LVDS_STREAM_UNIT_TEST_OBJECTS) $(MSS_LVDS_STREAM_UNIT_TEST_OBJECTS_GEN)
	$(R5F_LD) $(R5F_LDFLAGS) $(MSS_LVDS_STREAM_UNIT_TEST_LOC_LIBS) -Wl,-m=$(MSS_LVDS_STREAM_UNIT_TEST_MAP) \
	-o $(MSS_LVDS_STREAM_UNIT_TEST_OUT) $(MSS_LVDS_STREAM_UNIT_TEST_OBJECTS) $(MSS_LVDS_STREAM_UNIT_TEST_OBJECTS_GEN) \
	$(MSS_LVDS_STREAM_UNIT_TEST_STD_LIBS) $(PLATFORM_R5F_LINK_CMD) $(MSS_LVDS_STREAM_UNIT_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the LVDS Stream MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the LVDS Stream MSS Unit Test objects'
	@$(DEL) $(MSS_LVDS_STREAM_UNIT_TEST_OBJECTS) $(MSS_LVDS_STREAM_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(MSS_LVDS_STREAM_UNIT_TEST_OUT) $(MSS_LVDS_STREAM_UNIT_TEST_BIN)
	@$(DEL) $(MSS_LVDS_STREAM_UNIT_TEST_MAP) $(MSS_LVDS_STREAM_UNIT_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_LVDS_STREAM_UNIT_TEST_DEPENDS)

