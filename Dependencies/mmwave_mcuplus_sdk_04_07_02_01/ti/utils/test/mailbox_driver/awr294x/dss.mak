###################################################################################
# Mailbox Unit Test on DSS Makefile
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/mailbox_driver
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/mailbox_driver/$(PLATFORM_DEVICE_TYPE)


DSS_CPU := C66
DSS_CPU_INSTANCE := c66
DSS_CPU_PDK_CORE := c66xdsp_1


###################################################################################
# The mmWave Unit test requires additional libraries
###################################################################################
DSS_MAILBOX_UNIT_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)
DSS_MAILBOX_UNIT_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)

###################################################################################
# Unit Test Files
###################################################################################
DSS_MAILBOX_UNIT_TEST_CMD      = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
DSS_MAILBOX_UNIT_TEST_MAP      = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.map
DSS_MAILBOX_UNIT_TEST_OUT      = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.$($(DSS_CPU)_EXE_EXT)
DSS_MAILBOX_UNIT_TEST_BIN      = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.bin
DSS_MAILBOX_UNIT_TEST_APP_CMD  = $(PLATFORM_DEVICE_TYPE)/dss_linker.cmd
DSS_MAILBOX_UNIT_TEST_SOURCES  = main_dss.c

DSS_MAILBOX_UNIT_TEST_SOURCES_GEN = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

DSS_MAILBOX_UNIT_TEST_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MAILBOX_UNIT_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
DSS_MAILBOX_UNIT_TEST_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MAILBOX_UNIT_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))
DSS_MAILBOX_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(DSS_MAILBOX_UNIT_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: buildDirectories dssbuildDirectories $(DSS_MAILBOX_UNIT_TEST_OBJECTS) $(DSS_MAILBOX_UNIT_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $($(DSS_CPU)_COMMON_LOC_LIB) $($(DSS_CPU)_COMMON_STD_LIB)	\
	--map_file=$(DSS_MAILBOX_UNIT_TEST_MAP) $(DSS_MAILBOX_UNIT_TEST_OBJECTS) $(DSS_MAILBOX_UNIT_TEST_OBJECTS_GEN)	\
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(DSS_MAILBOX_UNIT_TEST_APP_CMD) -o $(DSS_MAILBOX_UNIT_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the Mailbox DSS Unit Test'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssTestClean:
	@echo 'Cleaning the mailbox DSS Unit Test objects'
	@$(DEL) $(DSS_MAILBOX_UNIT_TEST_OBJECTS) $(DSS_MAILBOX_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(DSS_MAILBOX_UNIT_TEST_OUT)
	@$(DEL) $(DSS_MAILBOX_UNIT_TEST_BIN) $(DSS_MAILBOX_UNIT_TEST_DEPENDS)
	@$(DEL) $(DSS_MAILBOX_UNIT_TEST_MAP)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_MAILBOX_UNIT_TEST_DEPENDS)

