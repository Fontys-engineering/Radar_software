###################################################################################
# DPM makefile
###################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

include ../../common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944 awr2x44P), )
include ./dpmlib.mak

include ./test/distributed/dss/dss.mak
include ./test/distributed/mss/mss.mak
include ./test/local/mss/mss.mak
include ./test/remote/dss/dss.mak
include ./test/remote/mss/mss.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean test testClean help

# This builds the DPM Module
lib: dpmLib

# This cleans the DPM Module
libClean: dpmLibClean

###################################################################################
# Test Targets:
###################################################################################
testClean: 	syscfgclean dssDistributedClean mssDistributedClean mssLocalClean mssRemoteClean dssRemoteClean
test:
	$(MAKE) syscfgclean objLocaltestClean localTestSyscfg mssLocal
	$(MAKE) syscfgclean objRemotemsstestClean objRemotedsstestClean RemoteTestSyscfg mssRemote dssRemote
	$(MAKE) syscfgclean objDistributemsstestClean objDistributedsstestClean DistributedSyscfg dssDistributed mssDistributed

localTest:
	$(MAKE) objLocaltestClean mssLocal

remoteTest:
	$(MAKE) objRemotemsstestClean objRemotedsstestClean RemoteTestSyscfg mssRemote dssRemote

distributeTest:
	$(MAKE) objDistributemsstestClean objDistributedsstestClean DistributedSyscfg dssDistributed mssDistributed

REMOTETEST_CORE_0=--script test/remote/mss/test_$(PLATFORM_DEVICE_TYPE).syscfg --context r5fss0-0 --output test/remote/mss/mssgenerated/
REMOTETEST_CORE_1=--script test/remote/dss/test_$(PLATFORM_DEVICE_TYPE).syscfg --context c66ss0 --output test/remote/dss/dssgenerated/

DISTRIBUTEDTEST_CORE_0=--script test/distributed/mss/test_$(PLATFORM_DEVICE_TYPE).syscfg --context r5fss0-0 --output test/distributed/mss/mssgenerated/
DISTRIBUTEDTEST_CORE_1=--script test/distributed/dss/test_$(PLATFORM_DEVICE_TYPE).syscfg --context c66ss0 --output test/distributed/dss/dssgenerated/

REMOTETESTCORES = \
    $(REMOTETEST_CORE_0) \
    $(REMOTETEST_CORE_1) \

DISTRIBUTEDTESTCORES = \
    $(DISTRIBUTEDTEST_CORE_0) \
    $(DISTRIBUTEDTEST_CORE_1) \

RemoteTestSyscfg:
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) $(REMOTETESTCORES)

DistributedSyscfg:
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) $(DISTRIBUTEDTESTCORES)

RemoteTestSyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCONFIG_INSTALL_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --part Default --package $(PACKAGE_TYPE) $(REMOTETESTCORES)

DistributedSyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCONFIG_INSTALL_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --part Default --package $(PACKAGE_TYPE) $(DISTRIBUTEDTESTCORES)

localTestSyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/local/mss/mssgenerated/ test/local/mss/test_$(PLATFORM_DEVICE_TYPE).syscfg

# syscfg: This generates syscfg files
localTestSyscfg:
	@echo Generating test SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/local/mss/mssgenerated/ test/local/mss/test_$(PLATFORM_DEVICE_TYPE).syscfg

syscfg: localTestSyscfg RemoteTestSyscfg DistributedSyscfg

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) test/local/mss/mssgenerated
	@$(DEL) test/remote/mss/mssgenerated
	@$(DEL) test/remote/dss/dssgenerated
	@$(DEL) test/distributed/mss/mssgenerated
	@$(DEL) test/distributed/dss/dssgenerated

# Clean: This cleans all the objects
clean: libClean testClean syscfgclean

# Build driver and all the unit tests
all: lib test

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for the DPM '
	@echo 'clean                -> Clean out all the objects'
	@echo 'lib                  -> Build the Driver only'
	@echo 'libClean             -> Clean the Driver Library only'
	@echo 'test                 -> Build the applicable unit tests'
	@echo 'testClean            -> Clean the unit tests'
	@echo 'syscfg               -> Generates syscfg files for unit tests'
	@echo 'syscfgClean          -> Cleans syscfg files of unit tests'
	@echo "****************************************************************************************"
else
# For some reason the .DEFAULT does not seem to work for lib, test
.PHONY: lib test

lib test .DEFAULT:
	@echo "Error: $(MODULE_NAME) is not supported on $(MMWAVE_SDK_DEVICE) !!!"
endif
