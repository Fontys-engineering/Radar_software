##################################################################################
# SOC Specific Test Targets
##################################################################################
include mssBuild.mak

BOOTIMAGE_CORE_ID_r5fss0-0 = 0
BOOTIMAGE_CORE_ID_r4       = 1

RADARSS_IMAGE_BIN = $(AWR2544_RADARSS_IMAGE_BIN)
MULTI_CORE_IMAGE_PARAMS = \
	$(CCS_DEBUG_MSS_RPRC)@$(BOOTIMAGE_CORE_ID_r5fss0-0) \
	$(RADARSS_IMAGE_BIN)@$(BOOTIMAGE_CORE_ID_r4) \

MMW_CCSDEBUG_BIN         = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug.appimage
BOOTIMAGE_TEMP_OUT_FILE  = temp_stdout.txt

msssyscfg: mss_$(PLATFORM_DEVICE_TYPE).syscfg
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output mssgenerated/ mss_$(PLATFORM_DEVICE_TYPE).syscfg

msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output ./mss/mssgenerated/ mss_$(PLATFORM_DEVICE_TYPE).syscfg

# syscfg: This generates syscfg files
syscfg: msssyscfg

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) mssgenerated

ccsDebugClean: mssCCSDebugClean binClean syscfgclean

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
binClean:
	@$(DEL) $(MMW_CCSDEBUG_BIN)

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
mmwDebug: mssCCSDebug

OUTRPRC_CMD = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/out2rprc/elf2rprc.js
MULTI_CORE_IMAGE_GEN = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/multicoreImageGen/multicoreImageGen.js

bin:
	$(OUTRPRC_CMD) $(CCS_DEBUG_MSS_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(MULTI_CORE_IMAGE_GEN) --devID 55 --out $(MMW_CCSDEBUG_BIN) $(MULTI_CORE_IMAGE_PARAMS) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	@$(DEL) $(CCS_DEBUG_MSS_RPRC) $(BOOTIMAGE_TEMP_OUT_FILE)

