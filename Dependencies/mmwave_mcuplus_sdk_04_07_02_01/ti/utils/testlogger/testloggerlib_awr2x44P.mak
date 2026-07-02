###################################################################################
# Test Logger Library Makefile
###################################################################################
.PHONY: testLoggerLib testLoggerLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Source Files:
###################################################################################
TEST_LOGGER_SOURCES = logger.c

###################################################################################
# Driver Objects:
# - AWR2943 AWR2944: Build R5 and DSP (C66X)
###################################################################################
TEST_LOGGER_R5F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(R5F_OBJ_EXT)))
TEST_LOGGER_C66_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(C66_OBJ_EXT)))

###################################################################################
# Driver Dependency:
###################################################################################
TEST_LOGGER_R5F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(R5F_DEP_EXT)))
TEST_LOGGER_C66_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(C66_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
TEST_LOGGER_R5F_DRV_LIB  = lib/libtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)
TEST_LOGGER_C66_DRV_LIB = lib/libtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)

###################################################################################
# testlogger Library:
# - AWR2943, AWR2944: Build the R5 & DSP (66X) Library
###################################################################################
testLoggerLib: buildDirectories $(TEST_LOGGER_R5F_LIB_OBJECTS) $(TEST_LOGGER_C66_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(TEST_LOGGER_R5F_DRV_LIB) $(TEST_LOGGER_R5F_LIB_OBJECTS)
	$(C66_AR) $(C66_AR_OPTS) $(TEST_LOGGER_C66_DRV_LIB) $(TEST_LOGGER_C66_LIB_OBJECTS)

###################################################################################
# Clean the mmWave Library
###################################################################################
testLoggerLibClean:
	@echo 'Cleaning the Test Logger Library Objects'
	$(DEL) $(TEST_LOGGER_R5F_LIB_OBJECTS) $(TEST_LOGGER_R5F_DRV_LIB)
	$(DEL) $(TEST_LOGGER_C66_LIB_OBJECTS) $(TEST_LOGGER_C66_DRV_LIB)
	$(DEL) $(TEST_LOGGER_R5F_DEPENDS) $(TEST_LOGGER_C66_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(TEST_LOGGER_R5F_DEPENDS)
-include $(TEST_LOGGER_C66_DEPENDS)

