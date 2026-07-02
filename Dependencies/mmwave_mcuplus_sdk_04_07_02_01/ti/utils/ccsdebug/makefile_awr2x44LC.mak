##################################################################################
# SOC Specific Test Targets
##################################################################################
include ../../common/mmwave_sdk.mak

include mssBuild.mak
include dsscm4Build.mak

BOOTIMAGE_CORE_ID_r5fss0-0 = 0
BOOTIMAGE_CORE_ID_m4ss0-1  = 2
BOOTIMAGE_CORE_ID_r4       = 3

RADARSS_IMAGE_BIN = $(AWR2X44P_RADARSS_IMAGE_BIN)
LC_MULTI_CORE_IMAGE_PARAMS = \
	$(CCS_DEBUG_MSS_RPRC)@$(BOOTIMAGE_CORE_ID_r5fss0-0) \
	$(CCS_DEBUG_M4_RPRC)@$(BOOTIMAGE_CORE_ID_m4ss0-1) \
	$(RADARSS_IMAGE_BIN)@$(BOOTIMAGE_CORE_ID_r4) \

LC_MMW_CCSDEBUG_BIN      = awr2x44LC_ccsdebug.appimage
BOOTIMAGE_TEMP_OUT_FILE  = temp_stdout.txt

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
lcBinClean:
	@$(DEL) $(LC_MMW_CCSDEBUG_BIN)

ccsDebugClean: mssCCSDebugClean m4CCSDebugClean lcBinClean 

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
lcMmwDebug: mssCCSDebug m4CCSDebug

OUTRPRC_CMD = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/out2rprc/elf2rprc.js
MULTI_CORE_IMAGE_GEN = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/multicoreImageGen/multicoreImageGen.js

lcBin:
	$(OUTRPRC_CMD) $(CCS_DEBUG_MSS_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(OUTRPRC_CMD) $(CCS_DEBUG_M4_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(MULTI_CORE_IMAGE_GEN) --devID 55 --out $(LC_MMW_CCSDEBUG_BIN) $(LC_MULTI_CORE_IMAGE_PARAMS) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	@$(DEL) $(CCS_DEBUG_MSS_RPRC) $(CCS_DEBUG_M4_RPRC) $(BOOTIMAGE_TEMP_OUT_FILE)

clean: ccsDebugClean

all: lcMmwDebug lcBin