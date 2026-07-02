###################################################################################
#
# Mailbox Test makefile
#
###################################################################################

include ../../../common/mmwave_sdk.mak

ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr294x)
include ./$(MMWAVE_SDK_DEVICE_TYPE)/mss.mak
include ./$(MMWAVE_SDK_DEVICE_TYPE)/dss.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################

.PHONY: all clean test testClean help

ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr294x)
testClean: 	mssTestClean dssTestClean
test: 		mssTest dssTest
endif

# Clean: This cleans all the objects
clean:      testClean

# This will build the test
all: test

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for the mailbox test '
	@echo 'clean             -> Clean out all the objects'
	@echo 'test              -> Builds all the unit tests for the SOC'
	@echo 'testClean         -> Cleans all the Unit Tests for the SOC'
	@echo "****************************************************************************************"


