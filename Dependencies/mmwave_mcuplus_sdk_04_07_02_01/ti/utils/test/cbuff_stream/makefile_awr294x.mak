###################################################################################
#
# LVDS Stream makefile
#
###################################################################################
include ./$(PLATFORM_DEVICE_TYPE)/mss.mak
include ./$(PLATFORM_DEVICE_TYPE)/dss.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean test testClean help

testClean: 	syscfgclean mssTestClean dssTestClean
test: 		syscfg mssTest dssTest

# syscfg: This generates syscfg files
syscfg:
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output $(PLATFORM_DEVICE_TYPE)/mssgenerated/ $(PLATFORM_DEVICE_TYPE)/mss.syscfg
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output $(PLATFORM_DEVICE_TYPE)/dssgenerated/ $(PLATFORM_DEVICE_TYPE)/dss.syscfg

msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context r5fss0-0 --part Default --package ETS --output ./mss/mssgenerated/ $(PLATFORM_DEVICE_TYPE)/mss.syscfg

dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context c66ss0 --part Default --package ETS --output ./dss/mssgenerated/ $(PLATFORM_DEVICE_TYPE)/dss.syscfg

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) $(PLATFORM_DEVICE_TYPE)/mssgenerated
	@$(DEL) $(PLATFORM_DEVICE_TYPE)/dssgenerated


