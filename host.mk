###############################################################################
# host.mk - host OS detection and tool locations
#
# Every path uses ?= so it can be overridden per machine, either on the
# command line (make CCS_PATH=/opt/ti/ccs) or in a git-ignored local.mk.
###############################################################################

-include $(dir $(lastword $(MAKEFILE_LIST)))local.mk

# Same scheme as Dependencies/mcu_plus_sdk_awr294x_10_02_00_04/imports.mak:
# on Windows recipes run in cmd.exe and the unix tools come from CCS's cygwin.
ifeq ($(OS),Windows_NT)
    TI_ROOT     ?= C:/ti
    CCS_PATH    ?= $(TI_ROOT)/ccs2030/ccs
    CYGWIN_PATH ?= $(CCS_PATH)/utils/cygwin
    MKDIR       := $(CYGWIN_PATH)/mkdir -p
    RMDIR       := $(CYGWIN_PATH)/rm -rf
    TOUCH       := $(CYGWIN_PATH)/touch
    SHELL       := cmd.exe
else
    TI_ROOT     ?= $(HOME)/ti
    CCS_PATH    ?= $(TI_ROOT)/ccs2030/ccs
    MKDIR       := mkdir -p
    RMDIR       := rm -rf
    TOUCH       := touch
endif

# Tools that ship with CCS 20.3
CGT_ARM_PATH  ?= $(CCS_PATH)/tools/compiler/ti-cgt-armllvm_4.0.3.LTS
CGT_C6X_PATH  ?= $(CCS_PATH)/tools/compiler/ti-cgt-c6000_8.5.0.LTS
NODE          ?= $(CCS_PATH)/tools/node/node
SYSCFG_PATH   ?= $(lastword $(sort $(wildcard $(CCS_PATH)/utils/sysconfig_*)))

# SDKs committed to this repository
DEPENDROOT                      := $(abspath $(dir $(lastword $(MAKEFILE_LIST)))Dependencies)
MCU_PLUS_SDK_PATH               := $(DEPENDROOT)/mcu_plus_sdk_awr294x_10_02_00_04
MMWAVE_SDK_INSTALL_PATH         := $(DEPENDROOT)/mmwave_mcuplus_sdk_04_07_02_01
MMWAVE_AWR294X_DFP_INSTALL_PATH := $(DEPENDROOT)/mmwave_dfp_02_04_18_01
C66x_MATHLIB_INSTALL_PATH       := $(DEPENDROOT)/mathlib_c66x_3_1_2_1
C66x_DSPLIB_INSTALL_PATH        := $(DEPENDROOT)/dsplib_c66x_3_4_0_0
NETWORK_PATH                    := $(MCU_PLUS_SDK_PATH)/source/networking

# Fail early with a readable message instead of "No such file or directory"
ifeq ($(wildcard $(CCS_PATH)),)
    $(error CCS not found at '$(CCS_PATH)'. Set CCS_PATH in local.mk or on the command line)
endif
ifeq ($(SYSCFG_PATH),)
    $(error No SysConfig found under '$(CCS_PATH)/utils'. Set SYSCFG_PATH in local.mk)
endif
