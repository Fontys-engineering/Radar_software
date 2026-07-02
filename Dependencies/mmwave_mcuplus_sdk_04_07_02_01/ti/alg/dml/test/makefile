###################################################################################
#	makefile for DML Unit Test
###################################################################################

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

.PHONY: test testClean hwaDssTest hwaM4Test hwaDssTestClean hwaM4TestClean syscfgClean syscfg

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
include ./hwa_dssTest.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
include ./hwa_m4Test.mak
include ./hwa_dssTest.mak
endif

##################################################################################
# Test targets which are device specific
#   Build the MSS and DSS Unit Test
##################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output ./dssgenerated/ ./dsstest_$(PLATFORM_DEVICE_TYPE).syscfg
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
m4syscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context m4ss0-1 --part Default --package $(PACKAGE_TYPE) --output ./m4generated/ ./m4test_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output ./dssgenerated/ ./dsstest_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
endif

# syscfg: This cleans syscfg files
syscfgClean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) ./dssgenerated
	@$(DEL) ./m4generated

ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
# syscfg: This generates syscfg files
syscfg:
	@echo Generating Dss test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output ./dssgenerated/ ./dsstest_$(PLATFORM_DEVICE_TYPE).syscfg
test: syscfg hwaDssTest
testClean: syscfgClean hwaDssTestClean
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
# syscfg: This generates syscfg files
syscfg:
	@echo Generating M4 test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context m4ss0-1 --part Default --package $(PACKAGE_TYPE) --output ./m4generated/ ./m4test_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

	@echo Generating Dss test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output ./dssgenerated/ ./dsstest_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
test: syscfg hwaM4Test hwaDssTest
testClean: syscfgClean hwaM4TestClean hwaDssTestClean
endif

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for DML'
	@echo 'test                -> Build the DSS_CM4 and DSS Unit test for HWA DML test'
	@echo 'testClean           -> Clean the DSS_CM4 and DSS Unit test for HWA DML test'
	@echo 'dsssyscfg-gui       -> Opens dss sysconfig gui'
	@echo 'm4syscfg-gui        -> Opens DSS_CM4 syscfg-gui'
	@echo "****************************************************************************************"
