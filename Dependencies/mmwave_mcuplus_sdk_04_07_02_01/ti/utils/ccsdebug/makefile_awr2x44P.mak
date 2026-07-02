##################################################################################
# SOC Specific Test Targets
##################################################################################
include mssBuild.mak
include dssBuild.mak
include dsscm4Build.mak

BOOTIMAGE_CORE_ID_r5fss0-0 = 0
BOOTIMAGE_CORE_ID_c66ss0   = 1
BOOTIMAGE_CORE_ID_m4ss0-1  = 2
BOOTIMAGE_CORE_ID_r4       = 3

RADARSS_IMAGE_BIN = $(AWR2X44P_RADARSS_IMAGE_BIN)
MULTI_CORE_IMAGE_PARAMS = \
	$(CCS_DEBUG_MSS_RPRC)@$(BOOTIMAGE_CORE_ID_r5fss0-0) \
	$(CCS_DEBUG_DSS_RPRC)@$(BOOTIMAGE_CORE_ID_c66ss0) \
	$(CCS_DEBUG_M4_RPRC)@$(BOOTIMAGE_CORE_ID_m4ss0-1) \
	$(RADARSS_IMAGE_BIN)@$(BOOTIMAGE_CORE_ID_r4) \

MMW_CCSDEBUG_BIN         = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug.appimage
BOOTIMAGE_TEMP_OUT_FILE  = temp_stdout.txt

msssyscfg: mss_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output mssgenerated/ mss_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

dsssyscfg: dss_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output dssgenerated/ dss_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

m4syscfg: m4_$(MMWAVE_SDK_DEVICE_TYPE).syscfg
	@echo Generating SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context m4ss0-1 --part Default --package $(PACKAGE_TYPE) --output m4generated/ m4_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context r5fss0-0 --part Default --package ETS --output ./mss/mssgenerated/ mss_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context c66ss0 --part Default --package ETS --output ./dss/mssgenerated/ dss_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

m4syscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context m4ss0-1 --part Default --package ETS --output ./m4/m4generated/ m4_$(MMWAVE_SDK_DEVICE_TYPE).syscfg

# syscfg: This generates syscfg files
syscfg: msssyscfg dsssyscfg m4syscfg

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) mssgenerated
	@$(DEL) dssgenerated
	@$(DEL) m4generated

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
binClean:
	@$(DEL) $(MMW_CCSDEBUG_BIN)

ccsDebugClean: mssCCSDebugClean dssCCSDebugClean m4CCSDebugClean binClean syscfgclean

ccsDebugClean: 
	$(MAKE) MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f makefile_awr2x44ECO.mak clean
	$(MAKE) MMWAVE_SDK_DEVICE_TYPE=awr2x44LC -f makefile_awr2x44LC.mak clean

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
mmwDebug: mssCCSDebug dssCCSDebug m4CCSDebug

ecoCcsDebug: 
	$(MAKE) MMWAVE_SDK_DEVICE_TYPE=awr2x44ECO -f makefile_awr2x44ECO.mak all

lcCcsDebug: 
	$(MAKE) MMWAVE_SDK_DEVICE_TYPE=awr2x44LC -f makefile_awr2x44LC.mak all

OUTRPRC_CMD = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/out2rprc/elf2rprc.js
MULTI_CORE_IMAGE_GEN = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/multicoreImageGen/multicoreImageGen.js

bin:
	$(OUTRPRC_CMD) $(CCS_DEBUG_MSS_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(OUTRPRC_CMD) $(CCS_DEBUG_DSS_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(OUTRPRC_CMD) $(CCS_DEBUG_M4_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(MULTI_CORE_IMAGE_GEN) --devID 55 --out $(MMW_CCSDEBUG_BIN) $(MULTI_CORE_IMAGE_PARAMS) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	@$(DEL) $(CCS_DEBUG_MSS_RPRC) $(CCS_DEBUG_DSS_RPRC) $(CCS_DEBUG_M4_RPRC) $(BOOTIMAGE_TEMP_OUT_FILE)

all: syscfg mmwDebug bin ecoCcsDebug lcCcsDebug
