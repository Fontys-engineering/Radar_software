###################################################################################
#
# LVDS Stream makefile
#
###################################################################################
include ./$(PLATFORM_DEVICE_TYPE)/mss.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean test testClean help

testClean: 	syscfgclean mssTestClean
test: 		syscfg mssTest

# syscfg: This generates syscfg files
syscfg:
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output $(PLATFORM_DEVICE_TYPE)/mssgenerated/ $(PLATFORM_DEVICE_TYPE)/mss.syscfg

msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output ./mss/mssgenerated/ $(PLATFORM_DEVICE_TYPE)/mss.syscfg


# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) $(PLATFORM_DEVICE_TYPE)/mssgenerated
	# @$(DEL) $(PLATFORM_DEVICE_TYPE)/dssgenerated

