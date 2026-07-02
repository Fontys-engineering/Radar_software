##################################################################################
# rangeproc makefile
##################################################################################
# The following needs to be before any makefile inclusion
# MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
# MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2544), )
include ./rangeproclib.mak
include ./test/mssHWATest.mak

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

ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2544)
msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR2544 --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/mssgenerated/ test/msstest_$(PLATFORM_DEVICE_TYPE).syscfg
endif

# Build everything
# syscfg: This generates syscfg files
syscfg:
	@echo Generating MSS test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/mssgenerated/ test/msstest_$(PLATFORM_DEVICE_TYPE).syscfg

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) test/mssgenerated

test: syscfg mssHWATest
testClean: syscfgclean mssHWATestClean

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
	@echo 'mssHWATest        -> Builds MSS Unit test for HWA Range Proc'
	@echo 'mssHWATestClean   -> Cleans MSS Unit test for HWA Range Proc'
	@echo 'msssyscfg-gui     -> Opens mss sysconfig gui'
	@echo "****************************************************************************************"

else
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif

