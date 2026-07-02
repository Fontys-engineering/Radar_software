################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
mss/mssgenerated/%.o: ../mss/mssgenerated/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Arm Compiler - building file: "$<"'
	"C:/ti/ccs2041/ccs/tools/compiler/ti-cgt-armllvm_4.0.4.LTS/bin/tiarmclang.exe" -c @"ccsIncludes.opt"  -mcpu=cortex-r5 -mfloat-abi=hard -mfpu=vfpv3-d16 -mlittle-endian -mthumb -Oz -flto -DDOWNLOAD_FROM_CCS -DDRIVERS_RADAR_HWA_V2 -DINCLUDE_DPM -DMMWDEMO_TDM -DSOC_AWR294X -DSOC_AWR2944 -D_LITTLE_ENDIAN -DAPP_RESOURCE_FILE='<'ti/demo/awr294x/mmw/mmw_resTDM.h'>' -DMMWAVE_SDK_DEVICE=awr2944 -DSUBSYS_MSS -DDebugP_LOG_ENABLED -D_DEBUG_=1 -DENET_STREAM -UMMWDEMO_DDM -g -Wall -Wno-gnu-variable-sized-type-not-at-end -Wno-unused-function -Wno-unused-but-set-variable -MMD -MMD -MP -MF"mss/mssgenerated/$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


