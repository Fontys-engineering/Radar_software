##################################################################################
# SOC Specific Test Targets
##################################################################################
include mssBuild.mak
include dssBuild.mak

BOOTIMAGE_CORE_ID_r5fss0-0 = 0
BOOTIMAGE_CORE_ID_c66ss0   = 2
BOOTIMAGE_CORE_ID_r4       = 3

RADARSS_IMAGE_BIN = $(AWR294X_RADARSS_IMAGE_BIN)
MULTI_CORE_IMAGE_PARAMS = \
	$(CCS_DEBUG_MSS_RPRC)@$(BOOTIMAGE_CORE_ID_r5fss0-0) \
	$(CCS_DEBUG_DSS_RPRC)@$(BOOTIMAGE_CORE_ID_c66ss0) \
	$(RADARSS_IMAGE_BIN)@$(BOOTIMAGE_CORE_ID_r4) \

MMW_CCSDEBUG_BIN         = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug.appimage
BOOTIMAGE_TEMP_OUT_FILE  = temp_stdout.txt

msssyscfg: mss_$(PLATFORM_DEVICE_TYPE).syscfg
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output mssgenerated/ mss_$(PLATFORM_DEVICE_TYPE).syscfg

dsssyscfg: dss_$(PLATFORM_DEVICE_TYPE).syscfg
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output dssgenerated/ dss_$(PLATFORM_DEVICE_TYPE).syscfg

msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context r5fss0-0 --part Default --package ETS --output ./mss/mssgenerated/ mss_$(PLATFORM_DEVICE_TYPE).syscfg

dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device AWR294X --context c66ss0 --part Default --package ETS --output ./dss/mssgenerated/ dss_$(PLATFORM_DEVICE_TYPE).syscfg

# syscfg: This generates syscfg files
syscfg: msssyscfg dsssyscfg

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) mssgenerated
	@$(DEL) dssgenerated

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
binClean:
	@$(DEL) $(MMW_CCSDEBUG_BIN)

ccsDebugClean: mssCCSDebugClean dssCCSDebugClean binClean syscfgclean

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
mmwDebug: mssCCSDebug dssCCSDebug

OUTRPRC_CMD = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/out2rprc/elf2rprc.js
MULTI_CORE_IMAGE_GEN = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/multicoreImageGen/multicoreImageGen.js

bin:
	$(OUTRPRC_CMD) $(CCS_DEBUG_MSS_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(OUTRPRC_CMD) $(CCS_DEBUG_DSS_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(MULTI_CORE_IMAGE_GEN) --devID 55 --out $(MMW_CCSDEBUG_BIN) $(MULTI_CORE_IMAGE_PARAMS) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	@$(DEL) $(CCS_DEBUG_MSS_RPRC) $(CCS_DEBUG_DSS_RPRC) $(BOOTIMAGE_TEMP_OUT_FILE)

all: syscfg mmwDebug bin
