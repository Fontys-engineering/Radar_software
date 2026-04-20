################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
utils/%.o: ../utils/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)" -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/cli_mmwave.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_mcuplus_sdk_04_07_02_01/ti/utils/cli/src/cli_mmwave.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)" -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/enet_apputils.o: C:/ti/backups/mcu_plus_sdk_awr294x_10_02_00_04/source/networking/enet/utils/enet_apputils.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)" -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/enet_cpswconfighandler.o: C:/ti/backups/mmwave_mcuplus_sdk_04_07_02_01/ti/demo/utils/enet_cpswconfighandler.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)" -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/enet_stream.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_mcuplus_sdk_04_07_02_01/ti/demo/utils/enet_stream.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/mailbox.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mcu_plus_sdk_awr294x_10_02_00_04/source/drivers/mailbox/v0/mailbox.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/mmwave.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_mcuplus_sdk_04_07_02_01/ti/control/mmwave/src/mmwave.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/mmwave_fullcfg.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_mcuplus_sdk_04_07_02_01/ti/control/mmwave/src/mmwave_fullcfg.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/mmwave_link_common.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_mcuplus_sdk_04_07_02_01/ti/control/mmwave/src/mmwave_link_common.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/mmwave_link_mailbox.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_mcuplus_sdk_04_07_02_01/ti/control/mmwave/src/mmwave_link_mailbox.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/rl_controller.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_dfp_02_04_18_01/ti/control/mmwavelink/src/rl_controller.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/rl_device.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_dfp_02_04_18_01/ti/control/mmwavelink/src/rl_device.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/rl_driver.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_dfp_02_04_18_01/ti/control/mmwavelink/src/rl_driver.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/rl_monitoring.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_dfp_02_04_18_01/ti/control/mmwavelink/src/rl_monitoring.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/rl_sensor.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mmwave_dfp_02_04_18_01/ti/control/mmwavelink/src/rl_sensor.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

utils/tasks.o: C:/ti/mmwave_mcuplus_sdk_04_07_02_01/mcu_plus_sdk_awr294x_10_02_00_04/source/kernel/freertos/FreeRTOS-Kernel/tasks.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"utils/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


