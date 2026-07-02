###################################################################################
# Makefile for R5F platform
###################################################################################

###################################################################################
# Tool Definitions:
###################################################################################
R5F_CC    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmclang
R5F_LD    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmclang
R5F_AR    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmar

###################################################################################
# R5F Tools Flag definitions:
###################################################################################
# Setup the Include paths for the R5 Builds:
R5F_INCLUDE = -I$(MMWAVE_DFP_INSTALL_PATH) -I$(ARM_CODEGEN_INSTALL_PATH)/include/c $(STD_INCL)
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
	-O3 \
	-Wno-gnu-variable-sized-type-not-at-end \
	-Wno-unused-function \

DEFINES = -D_LITTLE_ENDIAN

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

# File extension to use for R5F library file
R5F_LIB_EXT = lib

# File extension to use for R5F Objects
R5F_OBJ_EXT = oer5f

# File extension to use for R5F Asm Objects
R5F_ASM_OBJ_EXT = o5

# File extension to use for R5F Dependency
R5F_DEP_EXT = d
