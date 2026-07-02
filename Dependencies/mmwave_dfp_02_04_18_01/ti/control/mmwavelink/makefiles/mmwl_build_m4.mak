###################################################################################
# Makefile for M4 platform
###################################################################################

###################################################################################
# Tool Definitions:
###################################################################################
M4_CC    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmclang
M4_LD    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmclang
M4_AR    = $(ARM_CODEGEN_INSTALL_PATH)/bin/tiarmar

###################################################################################
# M4 Tools Flag definitions:
###################################################################################
# Setup the Include paths for the M4 Builds:
M4_INCLUDE = -I$(MMWAVE_DFP_INSTALL_PATH) -I$(ARM_CODEGEN_INSTALL_PATH)/include/c $(STD_INCL)

# Compiler flags used for the M4 Builds:
M4_CFLAGS = \
	-mcpu=cortex-m4 \
	-mthumb \
	-mfloat-abi=hard \
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
        M4_CFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(M4_CFLAGS))
        M4_LDFLAGS := $(filter-out $(EMIT_WARNINGS_AS_ERRORS_OPTIONS),$(M4_LDFLAGS))
    endif
endif  

# Archiver options:
M4_AR_OPTS = rc

# File extension to use for M4 library file
M4_LIB_EXT = lib

# File extension to use for M4 Objects
M4_OBJ_EXT = oem4

# File extension to use for M4 Dependency
M4_DEP_EXT = d
