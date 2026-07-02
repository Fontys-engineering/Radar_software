###################################################################################
# mmWave Link Unit Test on DSS Makefile
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c link_test/common
vpath %.c link_test/$(PLATFORM_DEVICE_TYPE)

DSS_CPU := C66
DSS_CPU_INSTANCE := c66

###################################################################################
# The UART Unit test requires additional libraries
###################################################################################
MMWAVE_LINK_DSS_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB) \
                                -lmmwavelink_$(DSS_CPU_INSTANCE).lib \
                                
MMWAVE_LINK_DSS_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB) \
                                -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
                                -i$(MMWAVE_AWR294X_DFP_INSTALL_PATH) \
                                -i$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib   

C66_COMMON_INCLUDE += -I$(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwave/link_test

###################################################################################
# Unit Test Files
###################################################################################
MMWAVE_DSS_UNIT_TEST_MAP		 = link_test/$(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_dss.map
MMWAVE_DSS_UNIT_TEST_OUT		 = link_test/$(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_link_dss.$($(DSS_CPU)_EXE_EXT)
MMWAVE_DSS_UNIT_TEST_APP_CMD	 = link_test/$(PLATFORM_DEVICE_TYPE)/dss_link_linker.cmd
MMWAVE_DSS_UNIT_TEST_SOURCES	 = main_dss.c \
                                   link_test.c \
                                   link_test_monitoring.c \
                                   osi_freertos.c \
                                   link_testcase_config.c \
                                   mmwavelink_test_common.c

MMWAVE_DSS_TEST_SOURCES_GEN  = ti_board_config.c \
                               ti_board_open_close.c \
                               ti_dpl_config.c \
                               ti_drivers_config.c \
                               ti_pinmux_config.c \
                               ti_power_clock_config.c	\
                               ti_drivers_open_close.c

MMWAVE_DSS_UNIT_TEST_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_DSS_UNIT_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
MMWAVE_DSS_UNIT_TEST_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_DSS_UNIT_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

MMWAVE_DSS_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVE_DSS_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: buildDirectories dssbuildDirectories  $(MMWAVE_DSS_UNIT_TEST_OBJECTS) $(MMWAVE_DSS_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(MMWAVE_LINK_DSS_TEST_LOC_LIBS) $(MMWAVE_LINK_DSS_TEST_STD_LIBS) 	\
	--map_file=$(MMWAVE_DSS_UNIT_TEST_MAP)  $(MMWAVE_DSS_UNIT_TEST_OBJECTS) $(MMWAVE_DSS_TEST_OBJECTS_GEN)	    \
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD)  $(MMWAVE_DSS_UNIT_TEST_APP_CMD) -o $(MMWAVE_DSS_UNIT_TEST_OUT) 
	@echo "******************************************************************************"
	@echo 'Built the Link DSS Unit Test'
	@echo "******************************************************************************"

###################################################################################
# Unit Test Object Clean:
###################################################################################
dssTestObjClean:
	@echo 'Cleaning the mmWave DSS Unit Test objects'
	@rm -f $(MMWAVE_DSS_UNIT_TEST_OBJECTS) $(MMWAVE_DSS_TEST_OBJECTS_GEN)
	@rm -f $(MMWAVE_DSS_UNIT_TEST_DEPENDS) $(MMWAVE_DSS_UNIT_TEST_OUT)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the Link DSS Unit Test objects'
	@$(DEL) $(MMWAVE_DSS_UNIT_TEST_OBJECTS) $(MMWAVE_DSS_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVE_DSS_UNIT_TEST_OUT) $(MMWAVE_DSS_UNIT_TEST_MAP)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_DSS_UNIT_TEST_DEPENDS)

