###################################################################################
# mmWave ccsdebug Level makefile
###################################################################################
include ../../common/mmwave_sdk.mak

ifneq  ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944), )
include ./makefile_awr294x.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),awr2544)
include ./makefile_awr2544.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE),awr2x44P)
include ./makefile_awr2x44P.mak
else
$(error Please set the environment variable MMWAVE_SDK_DEVICE.)
endif

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean help

all: syscfg mmwDebug bin

clean: ccsDebugClean
	@echo "******************************************************************************"
	@echo 'CCS Debug Utility Clean complete '
	@echo "******************************************************************************"

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for the CCS Debug Utility '
	@echo 'all                  -> Build the CCS Debug Utility'
	@echo 'clean                -> Clean the CCS Debug Utility'
	@echo 'syscfg               -> Generates syscfg files for ccsdebug Utility'
	@echo "****************************************************************************************"
