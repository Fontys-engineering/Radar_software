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
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
include ./test/dssTest_awr294x.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
include ./test/m4Test_awr2x44P.mak
include ./test/dssTest_awr2x44P.mak
include ./test/mssTest_awr2x44P.mak
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean test ecoTest lcTest testClean mssTest mssTestClean dssTest dssTestClean help

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944), )
test: dssTest
testClean: syscfgclean dssTestClean
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
export
ifeq ($(MSS_AOA_ENABLED), 0)
test: m4Test dssTest
ecoTest:
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44ECO MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f ./test/m4Test_awr2x44ECO.mak m4EcoTest 
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44ECO MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f ./test/dssTest_awr2x44ECO.mak dssEcoTest 
else
test: m4Test mssTest
ecoTest:
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44ECO MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f ./test/m4Test_awr2x44ECO.mak m4EcoTest 
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44ECO MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f ./test/mssTest_awr2x44ECO.mak mssEcoTest 
endif
lcTest:
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44LC MMWAVE_SDK_DEVICE_TYPE=awr2x44LC -f ./test/m4Test_awr2x44LC.mak m4LcTest 
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44LC MMWAVE_SDK_DEVICE_TYPE=awr2x44LC -f ./test/mssTest_awr2x44LC.mak mssLcTest
testClean: syscfgclean m4TestClean dssTestClean mssTestClean
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44ECO MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f ./test/m4Test_awr2x44ECO.mak m4EcoTestClean 
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44ECO MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f ./test/mssTest_awr2x44ECO.mak mssEcoTestClean 
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44ECO MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f ./test/dssTest_awr2x44ECO.mak dssEcoTestClean 
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44LC MMWAVE_SDK_DEVICE_TYPE=awr2x44LC -f ./test/m4Test_awr2x44LC.mak m4LcTestClean 
	$(MAKE) PLATFORM_OBJDIR=obj_awr2x44LC MMWAVE_SDK_DEVICE_TYPE=awr2x44LC -f ./test/mssTest_awr2x44LC.mak mssLcTestClean
endif


CORE_0=--script test/m4_$(MMWAVE_SDK_DEVICE_TYPE).syscfg --context m4ss0-1 --output test/m4generated/
CORE_0_dss=--script test/m4_$(MMWAVE_SDK_DEVICE_TYPE)_dss.syscfg --context m4ss0-1 --output test/m4generated/
CORE_1=--script test/dss_awr2x44P.syscfg --context c66ss0 --output test/dssgenerated/
CORE_2=--script test/mss_$(MMWAVE_SDK_DEVICE_TYPE).syscfg --context r5fss0-0 --output test/mssgenerated/

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dss_$(PLATFORM_DEVICE_TYPE).syscfg
# syscfg: This generates syscfg files
syscfg:
	@echo Generating DSS SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/dssgenerated/ test/dss_$(PLATFORM_DEVICE_TYPE).syscfg
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
ifeq ($(MSS_AOA_ENABLED), 0)
syscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR2X44P --part Default --package $(PACKAGE_TYPE) $(CORE_0_dss) $(CORE_1)
# syscfg: This generates syscfg files
syscfg:
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) $(CORE_0_dss) $(CORE_1)
else
syscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR2X44P --part Default --package $(PACKAGE_TYPE) $(CORE_0) $(CORE_2)
# syscfg: This generates syscfg files
syscfg:
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) $(CORE_0) $(CORE_2)
endif
# lcsyscfg: This generates syscfg files
lcsyscfg:
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) $(CORE_0) $(CORE_2)
endif

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) test/dssgenerated
	@$(DEL) test/m4generated
	@$(DEL) test/mssgenerated

# Clean: This cleans all the objects
clean: syscfgclean testClean

# This will build the hwa_edma test
all: syscfg test
ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
all: ecoTest lcsyscfg lcTest
endif

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for the objdethwa test '
	@echo 'clean             -> Clean out all the objects'
ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
	@echo 'test              -> Builds all the unit tests for the module for awr2x44P'
	@echo 'ecoTest           -> Builds all the unit tests for the module for awr2x44ECO'
	@echo 'lcTest            -> Builds all the unit tests for the module for awr2x44LC'
else
	@echo 'test              -> Builds all the unit tests for the module'
endif
	@echo 'testClean         -> Cleans all the unit Tests for the module'
	@echo 'all               -> Builds all the unit tests for the module'
ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
	@echo 'syscfg            -> Generates syscfg files for unit tests fpr awr2x44P and awr2x44ECO'
	@echo 'lcsyscfg          -> Generates syscfg files for unit tests for awr2x44LC'
else
	@echo 'syscfg            -> Generates syscfg files for unit tests'
endif
	@echo 'syscfgClean       -> Cleans syscfg files of unit tests'
	@echo 'dsssyscfg-gui     -> Opens dss sysconfig gui'
	@echo "****************************************************************************************"

else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif
