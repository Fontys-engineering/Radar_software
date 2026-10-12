###############################################################################
# Radar_software - command line build for the AWR2944 EVM (MSS R5F + DSS C66x)
#
#   make            build out/Radar_Program.appimage (SysConfig, both cores, image)
#   make mss / dss  build one core only
#   make syscfg     only (re)generate mssgenerated/ and dssgenerated/
#   make clean      remove out/
#   make V=1        show the full compiler command lines
#
# Host OS and tool paths are set up in host.mk. Flags mirror the CCS 'Debug'
# configuration in Radar_Program_{mss,dss}/.cproject; keep the two in sync.
###############################################################################

include host.mk

MSS_DIR := Radar_Program_mss
DSS_DIR := Radar_Program_dss
OUT     := out
comma   := ,

ifeq ($(V),1)
    Q :=
else
    Q := @
endif

.PHONY: all mss dss syscfg clean
all: $(OUT)/Radar_Program.appimage

###############################################################################
# SysConfig
#
# Both cores must go through SysConfig in a single run, otherwise the IPC
# shared-memory layout between R5F and C66x is generated empty.
###############################################################################
MSS_SYSCFG ?= $(MSS_DIR)/mss/mss_enet.syscfg
DSS_SYSCFG ?= $(DSS_DIR)/dss/dss.syscfg
MSS_GEN    := $(MSS_DIR)/mss/mssgenerated
DSS_GEN    := $(DSS_DIR)/dss/dssgenerated
SYSCFG_OK  := $(OUT)/syscfg.ok

GEN_SRCS := ti_dpl_config.c ti_drivers_config.c ti_drivers_open_close.c ti_pinmux_config.c \
            ti_power_clock_config.c ti_board_config.c ti_board_open_close.c ti_enet_config.c \
            ti_enet_open_close.c ti_enet_soc.c ti_enet_lwipif.c

$(SYSCFG_OK): $(MSS_SYSCFG) $(DSS_SYSCFG)
	@echo "SysConfig  $(notdir $(MSS_SYSCFG)) + $(notdir $(DSS_SYSCFG))"
	$(Q)$(NODE) $(SYSCFG_PATH)/dist/cli.js --product $(MCU_PLUS_SDK_PATH)/.metadata/product.json \
	    --script $(MSS_SYSCFG) --context r5fss0-0 --output $(MSS_GEN) \
	    --script $(DSS_SYSCFG) --context c66ss0 --output $(DSS_GEN)
	$(Q)$(MKDIR) $(@D)
	$(Q)$(TOUCH) $@

$(addprefix $(MSS_GEN)/,$(GEN_SRCS)) $(addprefix $(DSS_GEN)/,$(GEN_SRCS)): $(SYSCFG_OK) ;

syscfg: $(SYSCFG_OK)

###############################################################################
# MSS - Cortex-R5F, tiarmclang
###############################################################################
MSS_CC := $(CGT_ARM_PATH)/bin/tiarmclang

# SDK sources the CCS project pulls in as linked files (see Radar_Program_mss/.project)
MSS_SDK_SRCS := \
    $(MCU_PLUS_SDK_PATH)/source/networking/enet/utils/enet_apputils.c \
    $(MCU_PLUS_SDK_PATH)/source/drivers/ipc_notify/v1/ipc_notify_v1.c \
    $(MCU_PLUS_SDK_PATH)/source/drivers/mailbox/v0/mailbox.c \
    $(MCU_PLUS_SDK_PATH)/source/networking/lwip/lwip-stack/src/core/stats.c \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/FreeRTOS-Kernel/tasks.c \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/cli/src/cli_mmwave.c \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/demo/utils/enet_cpswconfighandler.c \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/demo/utils/enet_stream.c \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/src/mmwave.c \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/src/mmwave_fullcfg.c \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/src/mmwave_link_common.c \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/src/mmwave_link_mailbox.c \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/src/rl_controller.c \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/src/rl_device.c \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/src/rl_driver.c \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/src/rl_monitoring.c \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/src/rl_sensor.c

MSS_SRCS := $(wildcard $(MSS_DIR)/mss/*.c) $(wildcard $(MSS_DIR)/utils/*.c) \
            $(addprefix $(MSS_GEN)/,$(GEN_SRCS)) $(MSS_SDK_SRCS)

MSS_CMDS := $(MSS_DIR)/r5f_linker_BU.cmd $(MSS_DIR)/mss/mmw_mss_linker.cmd

MSS_INCS := \
    $(CGT_ARM_PATH)/include/c \
    $(MCU_PLUS_SDK_PATH) \
    $(MCU_PLUS_SDK_PATH)/source \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/FreeRTOS-Kernel/include \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/config/awr294x/r5f \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH) \
    $(MMWAVE_SDK_INSTALL_PATH) \
    $(MSS_DIR) \
    $(MSS_DIR)/mss \
    $(MSS_GEN) \
    $(NETWORK_PATH)/enet/core/include \
    $(NETWORK_PATH)/enet \
    $(NETWORK_PATH)/enet/soc/awr294x \
    $(NETWORK_PATH)/enet/core \
    $(NETWORK_PATH)/lwip/lwip-stack/src/include \
    $(NETWORK_PATH)/lwip/lwip-config/awr294x \
    $(NETWORK_PATH)/lwip/lwip-port/freertos/include \
    $(NETWORK_PATH)/lwip/lwip-port/include \
    $(NETWORK_PATH)/enet/hw_include \
    $(NETWORK_PATH)/enet/utils/include \
    $(NETWORK_PATH)/enet/core/include/core \
    $(NETWORK_PATH)/enet/core/lwipif/inc \
    $(NETWORK_PATH)/lwip/lwip-stack/src/include/lwip \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink \
    $(C66x_MATHLIB_INSTALL_PATH)/packages

MSS_DEFS := DOWNLOAD_FROM_CCS DRIVERS_RADAR_HWA_V2 INCLUDE_DPM MMWDEMO_TDM SOC_AWR294X SOC_AWR2944 \
            _LITTLE_ENDIAN MMWAVE_SDK_DEVICE=awr2944 SUBSYS_MSS DebugP_LOG_ENABLED _DEBUG_=1 ENET_STREAM

MSS_CFLAGS := -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto \
              $(addprefix -I,$(MSS_INCS)) $(addprefix -D,$(MSS_DEFS)) \
              "-DAPP_RESOURCE_FILE=<ti/demo/awr294x/mmw/mmw_resTDM.h>" -UMMWDEMO_DDM \
              -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable

MSS_LIBDIRS := \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib \
    $(CGT_ARM_PATH)/lib \
    $(MSS_GEN) \
    $(MSS_DIR)

MSS_LIBS := \
    $(MCU_PLUS_SDK_PATH)/source/drivers/lib/drivers.awr294x.r5f.ti-arm-clang.debug.lib \
    libc.a \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/control/mmwave/lib/libmmwave_awr2944.aer5f \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib/libdpm_awr2944.aer5f \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib/libmathutils.aer5f \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/cli/lib/libcli_awr2944.aer5f \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/hsiheader/lib/libhsiheader_awr2944.aer5f \
    $(MCU_PLUS_SDK_PATH)/source/board/lib/board.awr294x.r5f.ti-arm-clang.debug.lib \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/lib/freertos.awr294x.r5f.ti-arm-clang.debug.lib \
    $(NETWORK_PATH)/enet/lib/lwipif-cpsw-freertos.awr294x.r5f.ti-arm-clang.debug.lib \
    $(NETWORK_PATH)/enet/lib/enet-cpsw.awr294x.r5f.ti-arm-clang.debug.lib \
    $(NETWORK_PATH)/lwip/lib/lwip-freertos.awr294x.r5f.ti-arm-clang.debug.lib \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib/mmwavelink_r5f.lib

MSS_LDFLAGS := -Wl,-m$(OUT)/mss/Radar_Program_mss.map $(addprefix -Wl$(comma)-i,$(MSS_LIBDIRS)) \
               -Wl,--reread_libs -Wl,--diag_suppress=10063 -Wl,--diag_wrap=off -Wl,--display_error_number \
               -Wl,--warn_sections -Wl,--ram_model -Wl,--zero_init=on -Wl,--use_memcpy=fast -Wl,--use_memset=fast

MSS_OBJS := $(addprefix $(OUT)/mss/,$(notdir $(filter-out $(MSS_GEN)/%,$(MSS_SRCS:.c=.o)))) \
            $(addprefix $(OUT)/mss/gen/,$(GEN_SRCS:.c=.o))
vpath %.c $(sort $(dir $(filter-out $(MSS_GEN)/%,$(MSS_SRCS))))

$(OUT)/mss/gen/%.o: $(MSS_GEN)/%.c | $(SYSCFG_OK)
	@echo "CC  [mss]  $(notdir $<)"
	$(Q)$(MKDIR) $(@D)
	$(Q)$(MSS_CC) -c $(MSS_CFLAGS) -MMD -MP -MF $(@:.o=.d) -MT $@ -o $@ $<

$(OUT)/mss/%.o: %.c | $(SYSCFG_OK)
	@echo "CC  [mss]  $(notdir $<)"
	$(Q)$(MKDIR) $(@D)
	$(Q)$(MSS_CC) -c $(MSS_CFLAGS) -MMD -MP -MF $(@:.o=.d) -MT $@ -o $@ $<

$(OUT)/mss/Radar_Program_mss.xer5f: $(MSS_OBJS) $(MSS_CMDS)
	@echo "LD  [mss]  $(notdir $@)"
	$(Q)$(MSS_CC) $(MSS_CFLAGS) $(MSS_LDFLAGS) -o $@ $(MSS_OBJS) $(MSS_CMDS) $(addprefix -Wl$(comma)-l,$(MSS_LIBS))

$(OUT)/mss/Radar_Program_mss.rprc: $(OUT)/mss/Radar_Program_mss.xer5f
	$(Q)$(CGT_ARM_PATH)/bin/tiarmstrip -p $< -o=$(<:.xer5f=_strip.xer5f)
	$(Q)$(NODE) $(MCU_PLUS_SDK_PATH)/tools/boot/out2rprc/elf2rprc.js $(<:.xer5f=_strip.xer5f) $@

mss: $(OUT)/mss/Radar_Program_mss.rprc

###############################################################################
# DSS - C66x, cl6x
###############################################################################
DSS_CC := $(CGT_C6X_PATH)/bin/cl6x

DSS_SRCS := $(wildcard $(DSS_DIR)/*.c) $(wildcard $(DSS_DIR)/dss/*.c) $(addprefix $(DSS_GEN)/,$(GEN_SRCS))
DSS_CMDS := $(DSS_DIR)/c66x_linker.cmd $(DSS_DIR)/dss/mmw_dss_linker.cmd

DSS_INCS := \
    $(CGT_C6X_PATH)/include \
    $(MCU_PLUS_SDK_PATH)/source \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/FreeRTOS-Kernel/include \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/portable/TI_CGT/DSP_C66 \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/config/awr294x/c66 \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwa/src \
    $(MMWAVE_SDK_INSTALL_PATH) \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH) \
    $(DSS_GEN) \
    $(DSS_DIR) \
    $(C66x_MATHLIB_INSTALL_PATH)/packages \
    $(C66x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib

DSS_DEFS := SUBSYS_DSS SOC_AWR294X _LITTLE_ENDIAN DebugP_ASSERT_ENABLED INCLUDE_DPM DRIVERS_RADAR_HWA_V2 \
            SOC_AWR2944 _DEBUG_=1 MMWDEMO_TDM ENET_STREAM

DSS_CFLAGS := -mv6600 --abi=eabi -O3 --opt_for_speed=3 \
              $(addprefix --include_path=,$(DSS_INCS)) $(addprefix --define=,$(DSS_DEFS)) \
              "--define=APP_RESOURCE_FILE=<ti/demo/awr294x/mmw/mmw_resTDM.h>" \
              -g --gcc --diag_suppress=238 --diag_suppress=880 --diag_suppress=1110 --diag_warning=225 \
              --diag_wrap=off --display_error_number --emit_warnings_as_errors --quiet \
              --gen_func_subsections=on --interrupt_threshold=10

DSS_LIBDIRS := $(CGT_C6X_PATH)/lib $(DSS_DIR) $(DSS_GEN)

DSS_LIBS := \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib/libdpm_awr2944.ae66 \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib/libmathutils.ae66 \
    $(C66x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib/mathlib.ae66 \
    $(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib/dsplib.ae66 \
    $(MCU_PLUS_SDK_PATH)/source/kernel/freertos/lib/freertos.awr294x.c66.ti-c6000.debug.lib \
    $(MCU_PLUS_SDK_PATH)/source/drivers/lib/drivers.awr294x.c66.ti-c6000.debug.lib \
    $(MCU_PLUS_SDK_PATH)/source/board/lib/board.awr294x.c66.ti-c6000.debug.lib \
    libc.a \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/lib/librangeproc_hwa_awr2944.ae66 \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerproc/lib/libdopplerproc_hwa_awr2944.ae66 \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/cfarproc/lib/libcfarproc_hwa_awr2944.ae66 \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/aoaproc/lib/libaoaproc_hwa_awr2944.ae66 \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib/libdpedma_hwa_awr2944.ae66 \
    $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerprocDDMA/lib/libdopplerproc_hwa_ddma_awr2944.ae66 \
    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/ti/control/mmwavelink/lib/mmwavelink_c66.lib

DSS_LDFLAGS := -z -m$(OUT)/dss/Radar_Program_dss.map $(addprefix -i,$(DSS_LIBDIRS)) \
               --reread_libs --diag_error=10015 --warn_sections --rom_model

DSS_OBJS := $(addprefix $(OUT)/dss/,$(notdir $(filter-out $(DSS_GEN)/%,$(DSS_SRCS:.c=.obj)))) \
            $(addprefix $(OUT)/dss/gen/,$(GEN_SRCS:.c=.obj))
vpath %.c $(sort $(dir $(filter-out $(DSS_GEN)/%,$(DSS_SRCS))))

$(OUT)/dss/gen/%.obj: $(DSS_GEN)/%.c | $(SYSCFG_OK)
	@echo "CC  [dss]  $(notdir $<)"
	$(Q)$(MKDIR) $(@D)
	$(Q)$(DSS_CC) $(DSS_CFLAGS) --preproc_with_compile --preproc_dependency=$(@:.obj=.d) \
	    --obj_directory=$(@D) --output_file=$@ $<

$(OUT)/dss/%.obj: %.c | $(SYSCFG_OK)
	@echo "CC  [dss]  $(notdir $<)"
	$(Q)$(MKDIR) $(@D)
	$(Q)$(DSS_CC) $(DSS_CFLAGS) --preproc_with_compile --preproc_dependency=$(@:.obj=.d) \
	    --obj_directory=$(@D) --output_file=$@ $<

$(OUT)/dss/Radar_Program_dss.xe66: $(DSS_OBJS) $(DSS_CMDS)
	@echo "LD  [dss]  $(notdir $@)"
	$(Q)$(DSS_CC) $(DSS_CFLAGS) $(DSS_LDFLAGS) -o $@ $(DSS_OBJS) $(DSS_CMDS) $(addprefix -l,$(DSS_LIBS))

$(OUT)/dss/Radar_Program_dss.rprc: $(OUT)/dss/Radar_Program_dss.xe66
	$(Q)$(CGT_C6X_PATH)/bin/strip6x -p $< -o=$(<:.xe66=_strip.xe66)
	$(Q)$(NODE) $(MCU_PLUS_SDK_PATH)/tools/boot/out2rprc/elf2rprc.js $(<:.xe66=_strip.xe66) $@

dss: $(OUT)/dss/Radar_Program_dss.rprc

###############################################################################
# Multicore boot image: RADARSS firmware @3, DSS @2, MSS @0
###############################################################################
$(OUT)/Radar_Program.appimage: $(OUT)/mss/Radar_Program_mss.rprc $(OUT)/dss/Radar_Program_dss.rprc
	@echo "IMG        $(notdir $@)"
	$(Q)$(NODE) $(MCU_PLUS_SDK_PATH)/tools/boot/multicoreImageGen/multicoreImageGen.js --devID 55 --out $@ \
	    $(MMWAVE_AWR294X_DFP_INSTALL_PATH)/firmware/radarss/xwr29xx_radarss_metarprc.bin@3 \
	    $(OUT)/dss/Radar_Program_dss.rprc@2 $(OUT)/mss/Radar_Program_mss.rprc@0

clean:
	$(RMDIR) $(OUT)

-include $(MSS_OBJS:.o=.d) $(DSS_OBJS:.obj=.d)
