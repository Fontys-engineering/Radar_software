################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
build-1054366697: ../example.syscfg
	@echo 'SysConfig - building file: "$<"'
	"C:/ti/ccs2041/ccs/utils/sysconfig_1.27.0/sysconfig_cli.bat" -s "C:/ti/backups/mcu_plus_sdk_awr294x_10_02_00_04/.metadata/product.json" -p "ETS" -r "Default" --script "C:/Git/Radar_software/gpio_led_blink_awr294x-evm_r5fss0-0_nortos_ti-arm-clang/example.syscfg" --context "r5fss0-0" -o "syscfg" --compiler ticlang
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/ti_dpl_config.c: build-1054366697 ../example.syscfg
syscfg/ti_dpl_config.h: build-1054366697
syscfg/ti_drivers_config.c: build-1054366697
syscfg/ti_drivers_config.h: build-1054366697
syscfg/ti_drivers_open_close.c: build-1054366697
syscfg/ti_drivers_open_close.h: build-1054366697
syscfg/ti_pinmux_config.c: build-1054366697
syscfg/ti_power_clock_config.c: build-1054366697
syscfg/ti_board_config.c: build-1054366697
syscfg/ti_board_config.h: build-1054366697
syscfg/ti_board_open_close.c: build-1054366697
syscfg/ti_board_open_close.h: build-1054366697
syscfg/ti_enet_config.c: build-1054366697
syscfg/ti_enet_config.h: build-1054366697
syscfg/ti_enet_open_close.c: build-1054366697
syscfg/ti_enet_open_close.h: build-1054366697
syscfg/ti_enet_soc.c: build-1054366697
syscfg/ti_enet_lwipif.c: build-1054366697
syscfg/ti_enet_lwipif.h: build-1054366697
syscfg: build-1054366697

syscfg/%.o: ./syscfg/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -I"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/include/c" -I"C:/ti/backups/mcu_plus_sdk_awr294x_10_02_00_04/source" -DSOC_AWR294X -D_DEBUG_=1 -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -MMD -MP -MF"syscfg/$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Git/Radar_software/gpio_led_blink_awr294x-evm_r5fss0-0_nortos_ti-arm-clang/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -I"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/include/c" -I"C:/ti/backups/mcu_plus_sdk_awr294x_10_02_00_04/source" -DSOC_AWR294X -D_DEBUG_=1 -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/Git/Radar_software/gpio_led_blink_awr294x-evm_r5fss0-0_nortos_ti-arm-clang/Debug/syscfg"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


