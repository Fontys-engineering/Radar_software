###################################################################################
# Makefile for R4F platform
###################################################################################

###################################################################################
# Tool Definitions:
###################################################################################
R4F_CC    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmclang
R4F_LD    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmclang
R4F_AR    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmar

###################################################################################
# R4F Tools Flag definitions:
###################################################################################
# Setup the Include paths for the R4 Builds:
R4F_INCLUDE = -I$(MMWAVE_DFP_INSTALL_PATH) -I$(ARM_CODEGEN_INSTALL_PATH)/include/c $(STD_INCL)

# Compiler flags used for the R4 Builds:
R4F_CFLAGS = \
	-mcpu=cortex-r4 \
	-mthumb \
	-mfloat-abi=hard \
	-mfpu=vfpv3-d16 \
	-mlittle-endian \
	-Wall \
	-Werror \
	-g \
	-O3 \
	-Wno-gnu-variable-sized-type-not-at-end \
	-Wno-unused-function \

DEFINES = -D_LITTLE_ENDIAN

# all options used to enable build warnings as errors
EMIT_WARNINGS_AS_ERRORS_OPTIONS := --emit_warnings_as_errors -pdew

# if MMWAVE_DISABLE_WARNINGS_AS_ERRORS is 1 then remove the emit_warnings_as_errors flag
ifneq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)", "")
    ifeq ("$(MMWAVE_DISABLE_WARNINGS_AS_ERRORS)","1")
        R4F_CFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(R4F_CFLAGS))
        R4F_LDFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(R4F_LDFLAGS))
    endif
endif

# Archiver options:
R4F_AR_OPTS = rc

# File extension to use for R4F library file
R4F_LIB_EXT = lib

# File extension to use for R4F Objects
R4F_OBJ_EXT = oer4f

# File extension to use for R4F Asm Objects
R4F_ASM_OBJ_EXT = o4

# File extension to use for R4F Dependency
R4F_DEP_EXT = d
