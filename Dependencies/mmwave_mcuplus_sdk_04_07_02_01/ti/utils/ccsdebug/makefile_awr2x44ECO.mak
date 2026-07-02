##################################################################################
# SOC Specific Test Targets
##################################################################################
include ../../common/mmwave_sdk.mak

include mssBuild.mak
include dssBuild.mak
include dsscm4Build.mak

BOOTIMAGE_CORE_ID_r5fss0-0 = 0
BOOTIMAGE_CORE_ID_c66ss0   = 1
BOOTIMAGE_CORE_ID_m4ss0-1  = 2
BOOTIMAGE_CORE_ID_r4       = 3

RADARSS_IMAGE_BIN = $(AWR2X44P_RADARSS_IMAGE_BIN)
ECO_MULTI_CORE_IMAGE_PARAMS = \
	$(CCS_DEBUG_MSS_RPRC)@$(BOOTIMAGE_CORE_ID_r5fss0-0) \
	$(CCS_DEBUG_DSS_RPRC)@$(BOOTIMAGE_CORE_ID_c66ss0) \
	$(CCS_DEBUG_M4_RPRC)@$(BOOTIMAGE_CORE_ID_m4ss0-1) \
	$(RADARSS_IMAGE_BIN)@$(BOOTIMAGE_CORE_ID_r4) \

ECO_MMW_CCSDEBUG_BIN     = awr2x44ECO_ccsdebug.appimage
BOOTIMAGE_TEMP_OUT_FILE  = temp_stdout.txt

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
ecoBinClean:
	@$(DEL) $(ECO_MMW_CCSDEBUG_BIN)

ccsDebugClean: mssCCSDebugClean dssCCSDebugClean m4CCSDebugClean ecoBinClean

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
ecoMmwDebug: mssCCSDebug dssCCSDebug m4CCSDebug 

OUTRPRC_CMD = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/out2rprc/elf2rprc.js
MULTI_CORE_IMAGE_GEN = $(SYSCFG_NODE) $(MCU_PLUS_INSTALL_PATH)/tools/boot/multicoreImageGen/multicoreImageGen.js

ecoBin:
	$(OUTRPRC_CMD) $(CCS_DEBUG_MSS_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(OUTRPRC_CMD) $(CCS_DEBUG_DSS_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(OUTRPRC_CMD) $(CCS_DEBUG_M4_OUT) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	$(MULTI_CORE_IMAGE_GEN) --devID 55 --out $(ECO_MMW_CCSDEBUG_BIN) $(ECO_MULTI_CORE_IMAGE_PARAMS) >> $(BOOTIMAGE_TEMP_OUT_FILE)
	@$(DEL) $(CCS_DEBUG_MSS_RPRC) $(CCS_DEBUG_DSS_RPRC) $(CCS_DEBUG_M4_RPRC) $(BOOTIMAGE_TEMP_OUT_FILE)

clean: ccsDebugClean

all: ecoMmwDebug ecoBin