################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./mss/mmw_cli.o" \
"./mss/mmw_lvds_stream.o" \
"./mss/mssgenerated/ti_dpl_config.o" \
"./mss/mssgenerated/ti_drivers_config.o" \
"./mss/mssgenerated/ti_drivers_open_close.o" \
"./mss/mssgenerated/ti_pinmux_config.o" \
"./mss/mssgenerated/ti_power_clock_config.o" \
"./mss/mssgenerated/ti_board_config.o" \
"./mss/mssgenerated/ti_board_open_close.o" \
"./mss/mssgenerated/ti_enet_config.o" \
"./mss/mssgenerated/ti_enet_open_close.o" \
"./mss/mssgenerated/ti_enet_soc.o" \
"./mss/mssgenerated/ti_enet_lwipif.o" \
"./mss/mss_main.o" \
"./utils/mmwdemo_adcconfig.o" \
"./utils/mmwdemo_flash.o" \
"./utils/mmwdemo_monitor.o" \
"./utils/mmwdemo_rfparser.o" \
"../r5f_linker.cmd" \
"../mss/mmw_mss_linker.cmd" \
$(GEN_CMDS__FLAG) \
-Wl,-ldrivers.awr294x.r5f.ti-arm-clang.release.lib \
-Wl,-llibc.a \
-Wl,-llibtestlogger_awr2944.aer5f \
-Wl,-lmmwavelink_r5f.lib \
-Wl,-llibmmwave_awr2944.aer5f \
-Wl,-llibdpm_awr2944.aer5f \
-Wl,-llibmathutils.aer5f \
-Wl,-llibcli_awr2944.aer5f \
-Wl,-llibhsiheader_awr2944.aer5f \
-Wl,-lboard.awr294x.r5f.ti-arm-clang.release.lib \
-Wl,-lfreertos.awr294x.r5f.ti-arm-clang.release.lib \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include mss/subdir_vars.mk
-include utils/subdir_vars.mk
-include subdir_rules.mk
-include mss/subdir_rules.mk
-include utils/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
out_of_box_2944_mss.xer5f 

EXE_OUTPUTS__QUOTED += \
"out_of_box_2944_mss.xer5f" 


# All Target
all:
	@$(MAKE) --no-print-directory pre-build
	@$(MAKE) --no-print-directory main-build

# Main-build Target
main-build: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "out_of_box_2944_mss.xer5f"

# Tool invocations
out_of_box_2944_mss.xer5f: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: Arm Linker'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DDebugP_LOG_ENABLED -DINCLUDE_DPM -DMMWDEMO_DDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resDDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -D_DEBUG_=1 -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -Wl,-m"out_of_box_2944_mss.Debug.map" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mmwave_dfp_02_04_18_01/ti/control/mmwavelink/lib" -Wl,-i"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mcu_plus_sdk_awr294x_10_01_00_04/source/kernel/freertos/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mcu_plus_sdk_awr294x_10_01_00_04/source/drivers/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mcu_plus_sdk_awr294x_10_01_00_04/source/board/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mmwave_mcuplus_sdk_04_07_01_04/ti/control/mmwave/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mmwave_mcuplus_sdk_04_07_01_04/ti/utils/cli/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mmwave_mcuplus_sdk_04_07_01_04/ti/control/dpm/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mmwave_mcuplus_sdk_04_07_01_04/ti/utils/mathutils/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mmwave_mcuplus_sdk_04_07_01_04/ti/utils/testlogger/lib" -Wl,-i"C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mmwave_mcuplus_sdk_04_07_01_04/ti/utils/hsiheader/lib" -Wl,-i"C:/Users/petri/OneDrive/Asiakirjat/GitHub/Radar_software/out_of_box_2944_mss/mss/mssgenerated" -Wl,-i"C:/Users/petri/OneDrive/Asiakirjat/GitHub/Radar_software/out_of_box_2944_mss" -Wl,--reread_libs -Wl,--diag_suppress=10063 -Wl,--diag_wrap=off -Wl,--display_error_number -Wl,--warn_sections -Wl,--xml_link_info="out_of_box_2944_mss_linkInfo.xml" -Wl,--ram_model -Wl,--zero_init=on -Wl,--use_memcpy=fast -Wl,--use_memset=fast -o "out_of_box_2944_mss.xer5f" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(GEN_MISC_FILES__QUOTED)$(GEN_FILES__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "mss\mmw_cli.o" "mss\mmw_lvds_stream.o" "mss\mssgenerated\ti_dpl_config.o" "mss\mssgenerated\ti_drivers_config.o" "mss\mssgenerated\ti_drivers_open_close.o" "mss\mssgenerated\ti_pinmux_config.o" "mss\mssgenerated\ti_power_clock_config.o" "mss\mssgenerated\ti_board_config.o" "mss\mssgenerated\ti_board_open_close.o" "mss\mssgenerated\ti_enet_config.o" "mss\mssgenerated\ti_enet_open_close.o" "mss\mssgenerated\ti_enet_soc.o" "mss\mssgenerated\ti_enet_lwipif.o" "mss\mss_main.o" "utils\mmwdemo_adcconfig.o" "utils\mmwdemo_flash.o" "utils\mmwdemo_monitor.o" "utils\mmwdemo_rfparser.o" 
	-$(RM) "mss\mmw_cli.d" "mss\mmw_lvds_stream.d" "mss\mssgenerated\ti_dpl_config.d" "mss\mssgenerated\ti_drivers_config.d" "mss\mssgenerated\ti_drivers_open_close.d" "mss\mssgenerated\ti_pinmux_config.d" "mss\mssgenerated\ti_power_clock_config.d" "mss\mssgenerated\ti_board_config.d" "mss\mssgenerated\ti_board_open_close.d" "mss\mssgenerated\ti_enet_config.d" "mss\mssgenerated\ti_enet_open_close.d" "mss\mssgenerated\ti_enet_soc.d" "mss\mssgenerated\ti_enet_lwipif.d" "mss\mss_main.d" "utils\mmwdemo_adcconfig.d" "utils\mmwdemo_flash.d" "utils\mmwdemo_monitor.d" "utils\mmwdemo_rfparser.d" 
	-@echo 'Finished clean'
	-@echo ' '

pre-build:
	-C:/ti/ccs2041/ccs//utils/cygwin/rm -f C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss.rprc
	-C:/ti/sysconfig_1.23.0/nodejs/node C:/ti/sysconfig_1.23.0/dist/cli.js --product C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mcu_plus_sdk_awr294x_10_01_00_04/.metadata/product.json --script ../mss/mss.syscfg --context r5fss0-0 --output ../mss/mssgenerated/ --script ../../out_of_box_2944_dss/dss/dss.syscfg --context c66ss0 --output ../../out_of_box_2944_dss/dss/dssgenerated/
	-@echo ' '

post-build:
	-C:/ti/ccs2041/ccs//utils/cygwin/rm -f C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss_strip.xer5f
	-C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmstrip -p C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss.xer5f -o=C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss_strip.xer5f
	-C:/ti/ccs2041/ccs//utils/cygwin/rm -f C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss_strip.rprc
	-C:/ti/sysconfig_1.23.0/nodejs/node.exe  C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mcu_plus_sdk_awr294x_10_01_00_04/tools/boot/out2rprc/elf2rprc.js C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss_strip.xer5f
	-C:/ti/ccs2041/ccs//utils/cygwin/mv C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss_strip.rprc C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss.rprc
	-C:/ti/sysconfig_1.23.0/nodejs/node.exe C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mcu_plus_sdk_awr294x_10_01_00_04/tools/boot/multicoreImageGen/multicoreImageGen.js --devID 55 --out C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/out_of_box_2944_Debug.appimage C:/ti/mmwave_mcuplus_sdk_04_07_01_04/mmwave_mcuplus_sdk_04_07_01_04/../mmwave_dfp_02_04_18_01/firmware/radarss/xwr29xx_radarss_metarprc.bin@3  C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/../out_of_box_2944_dss/Debug/out_of_box_2944_dss_4_00_00_05.rprc@2 C:\Users\petri\OneDrive\Asiakirjat\GitHub\Radar_software\out_of_box_2944_mss/Debug/out_of_box_2944_mss.rprc@0
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

