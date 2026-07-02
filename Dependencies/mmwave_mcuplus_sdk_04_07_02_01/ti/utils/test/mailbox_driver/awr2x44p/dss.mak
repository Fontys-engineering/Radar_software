###################################################################################
# Mailbox Unit Test on DSS Makefile
###################################################################################
.PHONY: dssTest dssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/mailbox_driver
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/mailbox_driver/$(PLATFORM_DEVICE_TYPE)

###################################################################################
# The mmWave Unit test requires additional libraries
###################################################################################
DSS_MAILBOX_UNIT_TEST_STD_LIBS = $(C66_COMMON_STD_LIB)
DSS_MAILBOX_UNIT_TEST_LOC_LIBS = $(C66_COMMON_LOC_LIB)

###################################################################################
# Unit Test Files
###################################################################################
DSS_MAILBOX_UNIT_TEST_CMD      = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
DSS_MAILBOX_UNIT_TEST_MAP      = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.map
DSS_MAILBOX_UNIT_TEST_OUT      = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.$(C66_EXE_EXT)
DSS_MAILBOX_UNIT_TEST_BIN      = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_dss.bin
DSS_MAILBOX_UNIT_TEST_SOURCES  = main_dss.c

DSS_MAILBOX_UNIT_TEST_SOURCES_GEN = ti_board_config.c \
                                    ti_board_open_close.c \
                                    ti_dpl_config.c \
                                    ti_drivers_config.c \
                                    ti_pinmux_config.c \
                                    ti_power_clock_config.c \
                                    ti_drivers_open_close.c

DSS_MAILBOX_UNIT_TEST_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MAILBOX_UNIT_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
DSS_MAILBOX_UNIT_TEST_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MAILBOX_UNIT_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
DSS_MAILBOX_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(DSS_MAILBOX_UNIT_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
dssTest: buildDirectories dssbuildDirectories $(DSS_MAILBOX_UNIT_TEST_OBJECTS) $(DSS_MAILBOX_UNIT_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB)	\
	--map_file=$(DSS_MAILBOX_UNIT_TEST_MAP) $(DSS_MAILBOX_UNIT_TEST_OBJECTS) $(DSS_MAILBOX_UNIT_TEST_OBJECTS_GEN)	\
	$(PLATFORM_C66X_LINK_CMD) -o $(DSS_MAILBOX_UNIT_TEST_OUT)
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

