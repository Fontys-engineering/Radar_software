###################################################################################
# CSI-TX Stream Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/csitx_stream
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/csitx_stream/$(PLATFORM_DEVICE_TYPE)

MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f

###################################################################################
# The CSI-TX Stream Test requires additional libraries
###################################################################################
MSS_CSITX_STREAM_UNIT_TEST_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)	\
									-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)	\
									-lmmwavelink_$(MSS_CPU_INSTANCE).lib \
									-llibmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)	\
									-llibhsiheader_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)

MSS_CSITX_STREAM_UNIT_TEST_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB) 	\
									-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib	\
									-Wl,-i$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib	\
									-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib	\
									-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/hsiheader/lib


###################################################################################
# Unit Test Files
###################################################################################
MSS_CSITX_STREAM_UNIT_TEST_CMD		 = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MSS_CSITX_STREAM_UNIT_TEST_MAP		 = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_csitx_stream_mss.map
MSS_CSITX_STREAM_UNIT_TEST_OUT		 = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_csitx_stream_mss.$($(MSS_CPU)_EXE_EXT)
MSS_CSITX_STREAM_UNIT_TEST_BIN		 = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_csitx_stream_mss.bin
MSS_CSITX_STREAM_UNIT_TEST_APP_CMD	 = $(PLATFORM_DEVICE_TYPE)/mss_linker.cmd
MSS_CSITX_STREAM_UNIT_TEST_SOURCES	 = main_mss.c \
										csitx_stream.c

MSS_CSITX_STREAM_UNIT_TEST_SOURCES_GEN = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

MSS_CSITX_STREAM_UNIT_TEST_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_CSITX_STREAM_UNIT_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
MSS_CSITX_STREAM_UNIT_TEST_OBJECTS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_CSITX_STREAM_UNIT_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

MSS_CSITX_STREAM_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MSS_CSITX_STREAM_UNIT_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: buildDirectories mssbuildDirectories $(MSS_CSITX_STREAM_UNIT_TEST_OBJECTS) $(MSS_CSITX_STREAM_UNIT_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(MSS_CSITX_STREAM_UNIT_TEST_LOC_LIBS) -Wl,-m=$(MSS_CSITX_STREAM_UNIT_TEST_MAP) \
	-o $(MSS_CSITX_STREAM_UNIT_TEST_OUT) $(MSS_CSITX_STREAM_UNIT_TEST_OBJECTS) $(MSS_CSITX_STREAM_UNIT_TEST_OBJECTS_GEN) $(MSS_CSITX_STREAM_UNIT_TEST_STD_LIBS) \
	$(PLATFORM_$(MSS_CPU)_LINK_CMD) $(MSS_CSITX_STREAM_UNIT_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the CSI-TX Stream MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the CSI-TX Stream MSS Unit Test objects'
	@$(DEL) $(MSS_CSITX_STREAM_UNIT_TEST_OBJECTS) $(MSS_CSITX_STREAM_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(MSS_CSITX_STREAM_UNIT_TEST_OUT) $(MSS_CSITX_STREAM_UNIT_TEST_BIN)
	@$(DEL) $(MSS_CSITX_STREAM_UNIT_TEST_MAP) $(MSS_CSITX_STREAM_UNIT_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_CSITX_STREAM_UNIT_TEST_DEPENDS)

