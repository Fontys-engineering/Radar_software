##################################################################################
# dpedma makefile
##################################################################################
# The following needs to be before any makefile inclusion
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MODULE_NAME := $(notdir $(patsubst %/,%,$(dir $(MAKEFILE_PATH))))

M4_RELEASE_OPT ?= 1

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak

##################################################################################
# SOC Specific Test Targets
##################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944), )
include ./dpedmalib_awr294x.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2544)
include ./dpedmalib_awr2544.mak
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
include ./dpedmalib_awr2x44P.mak
endif
###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: all clean lib libClean help

##################################################################################
# Build/Clean the library
##################################################################################

# This builds the lib
lib: dpedmaLib

# This cleans the lib
libClean: dpedmaLibClean

# Clean: This cleans all the objects
clean: libClean

# Build everything
all: lib

# Help: This displays the MAKEFILE Usage.
help:
	@echo "****************************************************************************************"
	@echo '* Makefile Targets for dpedma'
	@echo 'all               -> Build lib and all unit tests'
	@echo 'clean             -> Clean out all the objects'
	@echo 'lib               -> Build the Driver only'
	@echo 'libClean          -> Clean the Driver Library only'
	@echo "****************************************************************************************"

.PHONY: lib
