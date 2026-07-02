###################################################################################
# MAILBOX Unit Test on MSS Makefile
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/mailbox_driver
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/test/mailbox_driver/$(PLATFORM_DEVICE_TYPE)

###################################################################################
# Unit Test Files
###################################################################################
MSS_MAILBOX_UNIT_TEST_CMD        = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
MSS_MAILBOX_UNIT_TEST_MAP        = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.map
MSS_MAILBOX_UNIT_TEST_OUT        = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.$(R5F_EXE_EXT)
MSS_MAILBOX_UNIT_TEST_BIN        = $(PLATFORM_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_mailbox_mss.bin
MSS_MAILBOX_UNIT_TEST_SOURCES    = main_mss.c

MSS_MAILBOX_UNIT_TEST_SOURCES_GEN = ti_board_config.c \
                                    ti_board_open_close.c \
                                    ti_dpl_config.c \
                                    ti_drivers_config.c \
                                    ti_pinmux_config.c \
                                    ti_power_clock_config.c \
                                    ti_drivers_open_close.c

MSS_MAILBOX_UNIT_TEST_DEPENDS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_MAILBOX_UNIT_TEST_SOURCES:.c=.$(R5F_DEP_EXT)))
MSS_MAILBOX_UNIT_TEST_OBJECTS	 = $(addprefix $(PLATFORM_OBJDIR)/, $(MSS_MAILBOX_UNIT_TEST_SOURCES:.c=.$(R5F_OBJ_EXT)))

MSS_MAILBOX_UNIT_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(MSS_MAILBOX_UNIT_TEST_SOURCES_GEN:.c=.$(R5F_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: buildDirectories mssbuildDirectories $(MSS_MAILBOX_UNIT_TEST_OBJECTS) $(MSS_MAILBOX_UNIT_TEST_OBJECTS_GEN)
	$(R5F_LD) $(R5F_LDFLAGS) $(R5F_COMMON_LOC_LIB) -Wl,-m=$(MSS_MAILBOX_UNIT_TEST_MAP) \
	-o $(MSS_MAILBOX_UNIT_TEST_OUT) $(MSS_MAILBOX_UNIT_TEST_OBJECTS) $(MSS_MAILBOX_UNIT_TEST_OBJECTS_GEN) \
	$(R5F_COMMON_STD_LIB) $(PLATFORM_R5F_LINK_CMD)
	@echo "******************************************************************************"
	@echo 'Built the mailbox MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the mailbox MSS Unit Test objects'
	@$(DEL) $(MSS_MAILBOX_UNIT_TEST_OBJECTS) $(MSS_MAILBOX_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(MSS_MAILBOX_UNIT_TEST_OUT) $(MSS_MAILBOX_UNIT_TEST_BIN)
	@$(DEL) $(MSS_MAILBOX_UNIT_TEST_MAP) $(MSS_MAILBOX_UNIT_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(MSS_MAILBOX_UNIT_TEST_DEPENDS)

