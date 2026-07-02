###################################################################################
#   objdethwa Test makefile
###################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944 awr2x44P), )
include ./test/mssTest.mak
include ./test/dssTest.mak
###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean test testClean mssTest mssTestClean dssTest dssTestClean help

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944 awr2x44P), )
test: syscfg dssTest mssTest
testClean: syscfgclean dssTestClean mssTestClean
endif

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/mssgenerated/ test/mss_$(PLATFORM_DEVICE_TYPE).syscfg

dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dss_$(PLATFORM_DEVICE_TYPE).syscfg
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR2X44P --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/mssgenerated/ test/msstest_$(PLATFORM_DEVICE_TYPE).syscfg

dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR2X44P --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dsstest_$(PLATFORM_DEVICE_TYPE).syscfg
endif

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) test/dssgenerated
	@$(DEL) test/mssgenerated

# syscfg: This generates syscfg files
syscfg:
	@echo Generating DSS SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dss_$(PLATFORM_DEVICE_TYPE).syscfg
	@echo Generating MSS SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/mssgenerated/ test/mss_$(PLATFORM_DEVICE_TYPE).syscfg

# Clean: This cleans all the objects
clean:      testClean syscfgclean

# This will build the hwa_edma test
all: syscfg test


# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for the objdethwa test '
	@echo 'clean             -> Clean out all the objects'
	@echo 'test              -> Builds all the unit tests for the module'
	@echo 'testClean         -> Cleans all the unit Tests for the module'
	@echo 'all               -> Builds all the unit tests for the module'
	@echo 'syscfg            -> Generates syscfg files for unit tests'
	@echo 'syscfgClean       -> Cleans syscfg files of unit tests'
	@echo 'msssyscfg-gui     -> Opens mss sysconfig gui'
	@echo 'dsssyscfg-gui     -> Opens dss sysconfig gui'
	@echo "****************************************************************************************"

else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif
