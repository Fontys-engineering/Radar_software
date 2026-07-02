###################################################################################
# mmWave SDK common makefile
###################################################################################

###################################################################################
# Tool Definitions:
###################################################################################
ifeq ($(OS),Windows_NT)
    CCS_PATH=$(CCS_INSTALL_PATH)/ccs
    CYGWIN_PATH=$(CCS_PATH)/utils/cygwin
    DEL=$(CYGWIN_PATH)/rm -rf
    DOXYGEN=doxygen
    COPY_CMD=$(CYGWIN_PATH)/cp
    MKDIR=$(CYGWIN_PATH)/mkdir -p
    MAKE= gmake
else
    export DEL=rm -Rf
    export DOXYGEN=doxygen
    export COPY_CMD=cp -f
    export MKDIR=mkdir
    export MAKE= make
endif

C66_CC    = $(C66X_CODEGEN_INSTALL_PATH)/bin/cl6x
C66_AR    = $(C66X_CODEGEN_INSTALL_PATH)/bin/ar6x
C66_LD    = $(C66X_CODEGEN_INSTALL_PATH)/bin/cl6x
R5F_CC    = $(R5F_CLANG_INSTALL_PATH)/bin/tiarmclang
R5F_LD    = $(R5F_CLANG_INSTALL_PATH)/bin/tiarmclang
R5F_AR    = $(R5F_CLANG_INSTALL_PATH)/bin/tiarmar
M4_CC     = $(R5F_CLANG_INSTALL_PATH)/bin/tiarmclang
M4_LD     = $(R5F_CLANG_INSTALL_PATH)/bin/tiarmclang
M4_AR     = $(R5F_CLANG_INSTALL_PATH)/bin/tiarmar
LIB_TYPE  = release

###################################################################################
# Platform specific definitions:
# Legend:
# MMWAVE_SDK_DEVICE     : awr2943, awr2944, awr2544, awr2x44P
# MMWAVE_SDK_DEVICE_TYPE: awr2943 (common for awr2943),
#                         awr2944 (common for awr2944),
#                         awr2544 (common for awr2544),
#                         awr2x44P (common for awr2x44P),
# PLATFORM_DEFINE       : SOC_AWR294X, SOC_AWR2544 SOC_AWR2X44P
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE), awr2943)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_awr2943.mak
else ifeq ($(MMWAVE_SDK_DEVICE), awr2944)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_awr2944.mak
else ifeq ($(MMWAVE_SDK_DEVICE), awr2544)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_awr2544.mak
else ifeq ($(MMWAVE_SDK_DEVICE), awr2x44P)
include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk_awr2x44P.mak
else
$(error Please set the environment variable MMWAVE_SDK_DEVICE.)
endif

###################################################################################
# SYSCONFIG specific definitions:
###################################################################################
SYSCFG_CLI_PATH  = $(SYSCONFIG_INSTALL_PATH)
SYSCFG_NODE      = $(SYSCONFIG_INSTALL_PATH)/nodejs/node
SYSCFG_NWJS      = $(SYSCONFIG_INSTALL_PATH)/nw/nw
SYSCFG_SDKPRODUCT= $(MCU_PLUS_INSTALL_PATH)/.metadata/product.json

###################################################################################
# ARM Tools Flag definitions:
###################################################################################
# Setup the Include paths for the R5F Builds:
R5F_INCLUDE  = -I$(MMWAVE_AWR294X_DFP_INSTALL_PATH) -I$(MMWAVE_SDK_INSTALL_PATH)
R5F_INCLUDE += -I$(MCU_PLUS_INSTALL_PATH) -I$(R5F_CLANG_INSTALL_PATH)/include/c $(STD_INCL)

R5F_COMMON_INCLUDE += $(R5F_INCLUDE) \
                      -I${MCU_PLUS_INSTALL_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include \
                      -I${MCU_PLUS_INSTALL_PATH}/source/kernel/freertos/portable/TI_ARM_CLANG/ARM_CR5F \
                      -I${MCU_PLUS_INSTALL_PATH}/source/kernel/freertos/config/$(PLATFORM_DEVICE_TYPE)/r5f \
                      -I${MCU_PLUS_INSTALL_PATH}/source \


# Compiler flags used for the R5 Builds:.
R5F_CFLAGS = \
	-mcpu=cortex-r5 \
	-mthumb \
	-mfloat-abi=hard \
	-mfpu=vfpv3-d16 \
	-mlittle-endian \
	-Wall \
	-Werror \
	-g \
	-Os \
	-Oz \
	-flto \
	-Wno-gnu-variable-sized-type-not-at-end \
	-Wno-unused-function \
	-Wno-unused-but-set-variable \

DEFINES = \
	-DSUBSYS_MSS \
	-D$(PLATFORM_DEFINE) \
	-D$(DEVICE_TYPE) \
	-D_LITTLE_ENDIAN \

ifeq ($(DOWNLOAD_FROM_CCS), yes)
R5F_CFLAGS += -DDOWNLOAD_FROM_CCS
endif


# Linker flags used for the R5F Builds:
R5F_LDFLAGS = \
			-Os \
			-O3 \
			-flto \
			-Wl,--diag_suppress=10063 \
			-Wl,--ram_model \
			-Wl,--reread_libs \
			-Wl,--zero_init=on \
			-Wl,--use_memset=fast \
			-Wl,--use_memcpy=fast \

# all options used to enable build warnings as errors
EMIT_WARNINGS_AS_ERRORS_OPTIONS := --emit_warnings_as_errors -pdew

# if MMWAVE_DISABLE_WARNINGS_AS_ERRORS is 1 then remove the emit_warnings_as_errors flag
ifneq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)", "")
    ifeq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)","1")
        R5F_CFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(R5F_CFLAGS))
        R5F_LDFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(R5F_LDFLAGS))
    endif
endif

# Archiver options:
R5F_AR_OPTS = rc

# Linker flag for RTS
# NOTE: this should be the last linker flag in the linker command line, just before the -o option.
R5F_LD_RTS_FLAGS = -l"libc.a"

# File extension to use for R5F library file
R5F_LIB_EXT = aer5f

# File extension to use for R5F Objects
R5F_OBJ_EXT = oer5f

# File extension to use for R5F Asm Objects
R5F_ASM_OBJ_EXT = o5

# File extension to use for R5F Executable
R5F_EXE_EXT = xer5f

# File extension to use for R5F Dependency
R5F_DEP_EXT = d

# File suffix to use for xs files
R5F_XS_SUFFIX = per5ft

# RTOS for R5F applications
R5F_RTOS_TYPE = freertos

# This is a list of all the minimum common libraries which are required on the R5 to build
# any application. NOTE: Applications should add any other libraries which are required
R5F_COMMON_STD_LIB  = \
                     -lboard.$(PLATFORM_DEVICE_TYPE).r5f.ti-arm-clang.$(LIB_TYPE).lib \
                     -lfreertos.$(PLATFORM_DEVICE_TYPE).r5f.ti-arm-clang.$(LIB_TYPE).lib \
                     -ldrivers.$(PLATFORM_DEVICE_TYPE).r5f.ti-arm-clang.$(LIB_TYPE).lib \
                     -llibc.a

R5F_COMMON_LOC_LIB  = \
                     -Wl,-i${MCU_PLUS_INSTALL_PATH}/source/kernel/freertos/lib \
                     -Wl,-i${MCU_PLUS_INSTALL_PATH}/source/drivers/lib \
                     -Wl,-i${MCU_PLUS_INSTALL_PATH}/source/board/lib \
                     -Wl,-i${R5F_CLANG_INSTALL_PATH}/lib \

###################################################################################
# DSP Tools Flag definitions:
###################################################################################
# Setup the Include paths for C66 Builds:
C66_INCLUDE   = -i$(MMWAVE_AWR294X_DFP_INSTALL_PATH) -i$(MMWAVE_SDK_INSTALL_PATH)
C66_INCLUDE  += -i$(C66X_CODEGEN_INSTALL_PATH)/include -i$(C66x_DSPLIB_INSTALL_PATH)/packages $(STD_INCL)


C66_COMMON_INCLUDE += -i${MCU_PLUS_INSTALL_PATH}/source \
                      -i$(MCU_PLUS_INSTALL_PATH)/source/kernel/freertos/FreeRTOS-Kernel/include \
                      -i${MCU_PLUS_INSTALL_PATH}/source/kernel/freertos/FreeRTOS-Kernel/include \
                      -i${MCU_PLUS_INSTALL_PATH}/source/kernel/freertos/portable/TI_CGT/DSP_C66 \
                      -i${MCU_PLUS_INSTALL_PATH}/source/kernel/freertos/config/$(PLATFORM_DEVICE_TYPE)/c66 \

#compiler flag definition with the --define.
# Compiler Flags for C66 Builds:
C66_CFLAGS   = -mv6600 --abi=eabi --gcc -g -O3 -mf3 -mo --define=SUBSYS_DSS 	\
               --define=$(PLATFORM_DEFINE) --define=$(DEVICE_TYPE) --define=_LITTLE_ENDIAN      \
               --display_error_number --define=DebugP_ASSERT_ENABLED --diag_warning=225     \
               --diag_wrap=off --preproc_with_compile $(C66_INCLUDE) --emit_warnings_as_errors


ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944 awr2x44P),)
R5F_CFLAGS += -DDRIVERS_RADAR_HWA_V2
C66_CFLAGS += --define=DRIVERS_RADAR_HWA_V2
endif


# Linker Flags for C66 Builds:
C66_LDFLAGS  = -mv6600 --abi=eabi -g --define=$(PLATFORM_DEFINE) --display_error_number           \
               --diag_warning=225 --diag_wrap=off -z --reread_libs --warn_sections --ram_model    \
               -i$(C66X_CODEGEN_INSTALL_PATH)/lib --emit_warnings_as_errors

# Treat "warning #10015-D: output file xyz cannot be loaded and run on a target system" as error
C66_LDFLAGS  += --diag_error=10015

# if MMWAVE_DISABLE_WARNINGS_AS_ERRORS is 1 then remove the emit_warnings_as_errors flag
ifneq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)", "")
    ifeq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)","1")
        C66_CFLAGS  := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(C66_CFLAGS))
        C66_LDFLAGS  := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(C66_LDFLAGS))
    endif
endif

# Archiver options:
C66_AR_OPTS = r

# File extension to use for library file
C66_LIB_EXT = ae66

# File extension to use for Objects
C66_OBJ_EXT  = oe66

# File extension to use for Executable
C66_EXE_EXT  = xe66

# File extension to use for Dependency
C66_DEP_EXT  = pp

# File suffix to use for xs files
C66_XS_SUFFIX = pe66

# RTOS for C66 applications
C66_RTOS_TYPE = freertos

# This is a list of all the minimum common libraries which are required on the C66 to build
# any application. NOTE: Applications should add any other libraries which are required
C66_COMMON_STD_LIB  = -lboard.$(PLATFORM_DEVICE_TYPE).c66.ti-c6000.$(LIB_TYPE).lib \
                      -lfreertos.$(PLATFORM_DEVICE_TYPE).c66.ti-c6000.$(LIB_TYPE).lib \
                      -ldrivers.$(PLATFORM_DEVICE_TYPE).c66.ti-c6000.$(LIB_TYPE).lib \
                      -llibc.a \

C66_COMMON_LOC_LIB  = -i$(MCU_PLUS_INSTALL_PATH)/source/board/lib \
                      -i$(MCU_PLUS_INSTALL_PATH)/source/kernel/$(R5F_RTOS_TYPE)/lib \
                      -i$(MCU_PLUS_INSTALL_PATH)/source/drivers/lib \
                      -i$(C66X_CODEGEN_INSTALL_PATH)/lib \

###################################################################################
# M4 Tools Flag definitions:
###################################################################################
# Setup the Include paths for M4 Builds:
M4_INCLUDE = -I$(MMWAVE_AWR294X_DFP_INSTALL_PATH) -I$(MMWAVE_SDK_INSTALL_PATH)
M4_INCLUDE  += -I$(MCU_PLUS_INSTALL_PATH) -I$(R5F_CLANG_INSTALL_PATH)/include/c $(STD_INCL)


M4_COMMON_INCLUDE +=  $(M4_INCLUDE) \
                      -I${MCU_PLUS_INSTALL_PATH}/source \
                      -I$(MCU_PLUS_INSTALL_PATH)/source/kernel/nortos \

M4_DEFINES = \
    -DSUBSYS_M4 \
	-D$(PLATFORM_DEFINE) \
	-D$(DEVICE_TYPE) \
	-D_LITTLE_ENDIAN \

# Use release_opt for M4 builds if this flag is set, and disable DebugP for all builds.
# Disabled M4_RELEASE_OPT when building tests.
M4_RELEASE_OPT ?= 0
ifeq ($(M4_RELEASE_OPT),1)
LIB_TYPE_M4 = release_opt
else
LIB_TYPE_M4 = $(LIB_TYPE)
endif

# Compiler Flags for M4 Builds:
M4_CFLAGS = \
        -mcpu=cortex-m4 \
        -mfloat-abi=soft \
        -mlittle-endian \
        -mthumb \
        -Wall \
        -Werror \
        -g \
        -Os \
        -Oz \
        -Wno-gnu-variable-sized-type-not-at-end \
        -Wno-unused-function \

# Linker flags used for the M4 Builds:
M4_LDFLAGS = \
			-Os \
			-O3 \
			-Wl,--ram_model \
			-Wl,--reread_libs \
            -Wl,--zero_init=on \
			-Wl,--use_memset=fast \
			-Wl,--use_memcpy=fast \

# if MMWAVE_DISABLE_WARNINGS_AS_ERRORS is 1 then remove the emit_warnings_as_errors flag
ifneq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)", "")
    ifeq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)","1")
        M4_CFLAGS  := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(M4_CFLAGS))
        M4_LDFLAGS  := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(M4_LDFLAGS))
    endif
endif

# Archiver options:
M4_AR_OPTS = rc

# File extension to use for library file
M4_LIB_EXT = aem4

# File extension to use for Objects
M4_OBJ_EXT  = oem4

# File extension to use for Executable
M4_EXE_EXT  = xem4

# File extension to use for Dependency
M4_DEP_EXT  = d

# File suffix to use for xs files
M4_XS_SUFFIX = pem4

# RTOS for M4 applications
M4_RTOS_TYPE = nortos

# This is a list of all the minimum common libraries which are required on the M4 to build
# any application. NOTE: Applications should add any other libraries which are required
M4_COMMON_STD_LIB = -lnortos.$(PLATFORM_DEVICE_TYPE).m4.ti-arm-clang.$(LIB_TYPE).lib \
                    -ldrivers.$(PLATFORM_DEVICE_TYPE).m4.ti-arm-clang.$(LIB_TYPE).lib \
                    -llibc.a \

M4_COMMON_OPT_LIB = -lnortos.$(PLATFORM_DEVICE_TYPE).m4.ti-arm-clang.$(LIB_TYPE_M4).lib \
                    -ldrivers.$(PLATFORM_DEVICE_TYPE).m4.ti-arm-clang.$(LIB_TYPE_M4).lib \
                    -llibc.a \

M4_COMMON_LOC_LIB  = -Wl,-i$(MCU_PLUS_INSTALL_PATH)/source/kernel/$(M4_RTOS_TYPE)/lib \
                     -Wl,-i$(MCU_PLUS_INSTALL_PATH)/source/drivers/lib \
                     -Wl,-i$(R5F_CLANG_INSTALL_PATH)/lib \

###################################################################################
# Build the object directory
###################################################################################
buildDirectories:
	@$(MKDIR) -p $(PLATFORM_OBJDIR)

mssbuildDirectories:
	@$(MKDIR) -p $(PLATFORM_OBJDIR)/mssgenerated

dssbuildDirectories:
	@$(MKDIR) -p $(PLATFORM_OBJDIR)/dssgenerated

m4buildDirectories:
	@$(MKDIR) -p $(PLATFORM_OBJDIR)/m4generated

###################################################################################
# Build Suffix:
###################################################################################

$(PLATFORM_OBJDIR)/%.$(R5F_OBJ_EXT): %.c
	@echo '[R5 Device/Type: $(MMWAVE_SDK_DEVICE)/$(MMWAVE_SDK_DEVICE_TYPE)] Building file: $<'
	@$(R5F_CC) -c $(R5F_CFLAGS) $(R5F_COMMON_INCLUDE) $(DEFINES) -MMD -o $@ $<

$(PLATFORM_OBJDIR)/%.$(R5F_ASM_OBJ_EXT): %.asm
	@echo '[R5F Device/Type: $(MMWAVE_SDK_DEVICE)/$(MMWAVE_SDK_DEVICE_TYPE)] Building file: $<'
	@$(R5F_CC) -c $(R5F_CFLAGS)  --asm_dependency="$<.d"  $< -o $@
	@$(R5F_CC) -c $(R5F_CFLAGS) $< -o $@

$(PLATFORM_OBJDIR)/%.$(C66_OBJ_EXT): %.c
	@echo '[C66 Device/Type: $(MMWAVE_SDK_DEVICE)/$(MMWAVE_SDK_DEVICE_TYPE)] Building file: $<'
	@$(C66_CC) -c $(C66_CFLAGS) $(C66_COMMON_INCLUDE) -ppd=$(PLATFORM_OBJDIR)/$(basename $(<F)).$(C66_DEP_EXT) $< --output_file $@

$(PLATFORM_OBJDIR)/%.$(M4_OBJ_EXT): %.c
	@echo '[M4 Device/Type: $(MMWAVE_SDK_DEVICE)/$(MMWAVE_SDK_DEVICE_TYPE)] Building file: $<'
	@$(M4_CC) -c $(M4_CFLAGS) $(M4_COMMON_INCLUDE) $(M4_DEFINES) -MMD -o $@ $<