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
# - AWR2544: Build R5
###################################################################################
TEST_LOGGER_R5F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(R5F_OBJ_EXT)))

###################################################################################
# Driver Dependency:
###################################################################################
TEST_LOGGER_R5F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(TEST_LOGGER_SOURCES:.c=.$(R5F_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
TEST_LOGGER_R5F_DRV_LIB  = lib/libtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)

###################################################################################
# testlogger Library:
# - AWR2544, AWR2944: Build the R5 Library
###################################################################################
testLoggerLib: buildDirectories $(TEST_LOGGER_R5F_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(TEST_LOGGER_R5F_DRV_LIB) $(TEST_LOGGER_R5F_LIB_OBJECTS)

###################################################################################
# Clean the mmWave Library
###################################################################################
testLoggerLibClean:
	@echo 'Cleaning the Test Logger Library Objects'
	$(DEL) $(TEST_LOGGER_R5F_LIB_OBJECTS) $(TEST_LOGGER_R5F_DRV_LIB)
	$(DEL) $(TEST_LOGGER_R5F_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(TEST_LOGGER_R5F_DEPENDS)

