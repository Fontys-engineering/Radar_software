##################################################################################
# rangecfarproc makefile
##################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

M4_RELEASE_OPT ?= 1

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944 awr2x44P),)
include ./rangecfarprocLib.mak
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
include ./test/dssHWATest.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
include ./test/m4HWATest.mak
include ./test/dssHWATest.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help test testClean

##################################################################################
# Build/Clean the library
##################################################################################

# This builds the lib
lib: rangecfarprocLib

# This cleans the lib
libClean: rangecfarprocLibClean

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

# Build everything
# syscfg: This generates syscfg files
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
syscfg:
	@echo Generating DSS test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(PLATFORM_DEVICE_TYPE).syscfg
test: dssHWATest
testClean: syscfgclean dssHWATestClean
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2x44P))
syscfg:
	@echo Generating M4 test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context m4ss0-1 --part Default --package $(PACKAGE_TYPE) --output test/m4generated/ test/m4test_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

	@echo Generating DSS test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
test: m4HWATest dssHWATest
testClean: syscfgclean m4HWATestClean dssHWATestClean
endif

# Clean: This cleans all the objects
clean: libClean syscfgclean testClean

# Build everything
all: lib syscfg test

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for CFAR Proc DPU'
	@echo 'all               -> Build lib and all unit tests'
	@echo 'clean             -> Clean out all the objects'
	@echo 'lib               -> Build the Driver only'
	@echo 'libClean          -> Clean the Driver Library only'
	@echo 'dssHWATest        -> Build the DSS Unit test for HWA CFAR Proc test'
	@echo 'm4HWATest         -> Build the M4 Unit test for HWA CFAR Proc test'
	@echo 'dssHWATestClean   -> Clean the DSS Unit test for HWA CFAR Proc test'
	@echo 'm4HWATestClean    -> Clean the M4 Unit test for HWA CFAR Proc test'
	@echo 'test              -> Build the applicable unit test for HWA CFAR Proc test'
	@echo 'testClean         -> Clean unit test for HWA CFAR Proc test'
	@echo 'dsssyscfg-gui     -> Opens dss sysconfig gui'
	@echo 'm4syscfg-gui      -> Opens m4 sysconfig gui'
	@echo "****************************************************************************************"
else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif
