###################################################################################
# Test Logger makefile
###################################################################################
include ../../common/mmwave_sdk.mak
ifneq  ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944), )
include ./testloggerlib_awr294x.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),awr2544)
include ./testloggerlib_awr2544.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),awr2x44P)
include ./testloggerlib_awr2x44P.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help

# This builds the test logger library
lib: testLoggerLib

# This cleans the test logger library
libClean: testLoggerLibClean

# Clean: This cleans all the objects
clean: libClean

# Build driver and all the unit tests
all: lib

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for the Test Logger '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the mmWave library only'
	@echo 'libClean             -> Clean the mmWave library only'
	@echo "****************************************************************************************"
