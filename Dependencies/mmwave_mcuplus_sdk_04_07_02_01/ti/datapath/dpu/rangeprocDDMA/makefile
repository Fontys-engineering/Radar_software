##################################################################################
# rangeproc makefile
##################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

M4_RELEASE_OPT ?= 1

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944 awr2x44P), )
include ./rangeproclib.mak
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
include ./test/dssHWATest.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
include ./test/dssHWATest.mak
include ./test/m4HWATest.mak
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
lib: rangeprocLib

# This cleans the lib
libClean: rangeprocLibClean

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(PLATFORM_DEVICE_TYPE).syscfg
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
m4syscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context m4ss0-1 --part Default --package $(PACKAGE_TYPE) --output test/m4generated/ test/m4test_awr2x44P.syscfg
dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_awr2x44P.syscfg
endif

# syscfgclean: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) test/dssgenerated
	@$(DEL) test/mssgenerated
	@$(DEL) test/m4generated


ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
syscfg:
	@echo Generating DSS test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(PLATFORM_DEVICE_TYPE).syscfg
test:
	$(MAKE) dssHWATestClean
	$(MAKE) dssHWATest
testClean: syscfgclean dssHWATestClean
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
syscfg:
	@echo Generating DSS test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

	@echo Generating M4 test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context m4ss0-1 --part Default --package $(PACKAGE_TYPE) --output test/m4generated/ test/m4test_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
test:
	$(MAKE) dssHWATestClean
	$(MAKE) dssHWATest
	$(MAKE) m4HWATestClean
	$(MAKE) m4HWATest
testClean: syscfgclean dssHWATestClean m4HWATestClean
endif

# Clean: This cleans all the objects
clean: libClean testClean syscfgclean

# Build everything
all: lib syscfg test

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for the Range Proc DPU'
	@echo 'all               -> Build lib and all unit tests'
	@echo 'clean             -> Clean out all the objects'
	@echo 'lib               -> Build the Driver only'
	@echo 'libClean          -> Clean the Driver Library only'
	@echo 'test              -> Build the applicable unit tests'
	@echo 'testClean         -> Clean the unit tests'
	@echo 'syscfg            -> Generates syscfg files for unit tests'
	@echo 'syscfgClean       -> Cleans syscfg files of unit tests'
	@echo 'dssHWATest        -> Builds DSS Unit test for HWA Range Proc'
	@echo 'dssHWATestClean   -> Cleans DSS Unit test for HWA Range Proc'
	@echo 'm4HWATest         -> Builds M4 Unit test for HWA Range Proc'
	@echo 'm4HWATestClean    -> Cleans M4 Unit test for HWA Range Proc'
	@echo 'dsssyscfg-gui     -> Opens dss sysconfig gui'
	@echo 'm4syscfg-gui      -> Opens m4 sysconfig gui'
	@echo "****************************************************************************************"

else
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif
