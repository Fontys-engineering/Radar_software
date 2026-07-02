###################################################################################
#
# CSI-TX Stream top level makefile
#
###################################################################################
include ../../../common/mmwave_sdk.mak

ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2544)
include ./makefile_awr2544.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean test testClean help

# Clean: This cleans all the objects
clean: testClean

# This will build the test
all: test

# Help: This displays the MAKEFILE Usage.
help:
	@echo '****************************************************************************************'
	@echo 'Makefile Targets for the CSI-TX Stream test for AWR2544 only'
	@echo 'clean             -> Clean out all the objects'
	@echo 'test              -> Builds all the unit tests for the SOC'
	@echo 'testClean         -> Cleans all the Unit Tests for the SOC'
	@echo 'syscfg            -> Generates syscfg files for unit tests'
	@echo 'syscfgClean       -> Cleans syscfg files of unit tests'
	@echo 'msssyscfg-gui     -> Opens mss sysconfig gui'
	@echo '****************************************************************************************'

