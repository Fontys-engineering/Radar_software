###################################################################################
# mmWave Link Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c link_test/common
vpath %.c link_test/$(MMWAVE_SDK_DEVICE_TYPE)

MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f

###################################################################################
# The mmWave Link Unit Test requires additional libraries
###################################################################################
MMWAVE_MSS_UNIT_TEST_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB) \
								-lmmwavelink_$(MSS_CPU_INSTANCE).lib \
		   						     
MMWAVE_MSS_UNIT_TEST_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB) 	\
								-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
								-Wl,-i$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib   

R5F_COMMON_INCLUDE += -I$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwave/link_test

###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_MSS_UNIT_TEST_MAP		 = link_test/$(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_mss.map
MMWAVE_MSS_UNIT_TEST_OUT		 = link_test/$(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_mss.$($(MSS_CPU)_EXE_EXT)
MMWAVE_MSS_UNIT_TEST_BIN		 = link_test/$(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_mss.bin
MMWAVE_MSS_UNIT_TEST_APP_CMD	 = link_test/$(PLATFORM_DEVICE_TYPE)/mss_link_linker.cmd
MMWAVE_MSS_UNIT_TEST_SOURCES	 = main_mss.c \
								   link_test.c \
								   link_test_monitoring.c \
								   osi_freertos.c \
								   link_testcase_config.c \
								   mmwavelink_test_common.c \

MMWAVE_FULL_MSS_UNIT_TEST_SOURCES_GEN = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

MMWAVE_MSS_UNIT_TEST_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_MSS_UNIT_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
MMWAVE_MSS_UNIT_TEST_OBJECTS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_MSS_UNIT_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

MMWAVE_MSS_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MMWAVE_FULL_MSS_UNIT_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))
## $(MSS_CPU)_CFLAGS +=  --define=DebugP_LOG_ENABLED
###################################################################################
# Build Unit Test:
###################################################################################
mssTest: buildDirectories mssbuildDirectories $(MMWAVE_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_MSS_UNIT_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(MMWAVE_MSS_UNIT_TEST_LOC_LIBS) -Wl,-m=$(MMWAVE_MSS_UNIT_TEST_MAP) \
	-o $(MMWAVE_MSS_UNIT_TEST_OUT) $(MMWAVE_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_MSS_UNIT_TEST_OBJECTS_GEN) \
	$(MMWAVE_MSS_UNIT_TEST_STD_LIBS) $(PLATFORM_$(MSS_CPU)_LINK_CMD) $(MMWAVE_MSS_UNIT_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the Link MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Unit Test Object Clean:
###################################################################################
mssTestObjClean:
	@echo 'Cleaning the mmWave Full MSS Unit Test objects'
	@rm -f $(MMWAVE_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_MSS_UNIT_TEST_OBJECTS_GEN)
	@rm -f $(MMWAVE_MSS_UNIT_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the Link MSS Unit Test objects'
	@$(DEL) $(MMWAVE_MSS_UNIT_TEST_OBJECTS) $(MMWAVE_MSS_UNIT_TEST_OUT) $(MMWAVE_MSS_UNIT_TEST_BIN)
	@$(DEL) $(MMWAVE_MSS_UNIT_TEST_MAP) $(MMWAVE_MSS_UNIT_TEST_DEPENDS)
	@echo 'Cleaning the Link MSS Unit test'
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_MSS_UNIT_TEST_DEPENDS)

