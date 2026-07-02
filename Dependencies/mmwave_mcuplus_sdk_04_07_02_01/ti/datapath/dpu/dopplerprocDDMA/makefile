##################################################################################
# dopplerproc makefile
##################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

M4_RELEASE_OPT ?= 1

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
include ./dopplerprocDDMAlib.mak
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
include ./test/hwa_dssTest.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
include ./test/hwa_m4Test.mak
include ./test/hwa_dssTest.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help test testClean help

##################################################################################
# Build/Clean the library
##################################################################################

# This builds the lib
lib: dopplerprocDDMALib

# This cleans the lib
libClean: dopplerprocDDMALibClean

##################################################################################
# Test targets which are device specific
#   Build the MSS and DSS Unit Test
##################################################################################

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(PLATFORM_DEVICE_TYPE).syscfg
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
m4syscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context m4ss0-1 --part Default --package $(PACKAGE_TYPE) --output test/m4generated/ test/m4test_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
endif

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) test/dssgenerated
	@$(DEL) test/m4generated

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
# syscfg: This generates syscfg files
syscfg:
	@echo Generating Dss test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(PLATFORM_DEVICE_TYPE).syscfg
test:
	$(MAKE) hwaDssTestClean
	$(MAKE) hwaDssTest
testClean: syscfgclean hwaDssTestClean
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
# syscfg: This generates syscfg files
syscfg:
	@echo Generating M4 test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context m4ss0-1 --part Default --package $(PACKAGE_TYPE) --output test/m4generated/ test/m4test_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

	@echo Generating Dss test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
test:
	$(MAKE) hwaM4TestClean
	$(MAKE) hwaM4Test
	$(MAKE) hwaDssTestClean
	$(MAKE) hwaDssTest
testClean: syscfgclean hwaM4TestClean hwaDssTestClean
endif

# Clean: This cleans all the objects
clean: libClean testClean syscfgclean

# Build everything
all: lib syscfg test

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for dopplerproc'
	@echo 'all                 -> Build lib and all unit tests'
	@echo 'clean               -> Clean out all the objects'
	@echo 'lib                 -> Build the Driver only'
	@echo 'libClean            -> Clean the Driver Library only'
	@echo 'test                -> Build the MSS and DSS Unit test for HWA Doppler Proc test'
	@echo 'testClean           -> Clean the MSS and DSS Unit test for HWA Doppler Proc test'
	@echo 'syscfg              -> Generates syscfg files for unit tests'
	@echo 'syscfgClean         -> Cleans syscfg files of unit tests'
	@echo 'hwaDssTest          -> Build the DSS Unit test for HWA Doppler Proc test'
	@echo 'hwaM4Test           -> Build the M4 Unit test for HWA Doppler Proc test'
	@echo 'hwaDssTestClean     -> Clean the DSS Unit test for HWA Doppler Proc test'
	@echo 'hwaM4TestClean      -> Clean the M4 Unit test for HWA Doppler Proc test'
	@echo 'dsssyscfg-gui       -> Opens dss sysconfig gui'
	@echo 'm4syscfg-gui        -> Opens M4 syscfg-gui'
	@echo "****************************************************************************************"
else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif
