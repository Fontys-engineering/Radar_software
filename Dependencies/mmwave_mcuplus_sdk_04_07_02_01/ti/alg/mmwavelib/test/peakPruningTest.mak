###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: peakPruningTest peakPruningClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c test/generated

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_peakPruning_TEST_APP_CMD	 = test/dss_mmwavelib_linker.cmd
MMWAVELIB_peakPruning_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_peakPruning.map
MMWAVELIB_peakPruning_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_peakPruning.$(C66_EXE_EXT)
MMWAVELIB_peakPruning_TEST_SOURCES   = 	lib_peakpruning_test.c \
										lib_peakpruning_debug.c


MMWAVELIB_peakPruning_TEST_SOURCES_GEN = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

MMWAVELIB_peakPruning_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_peakPruning_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVELIB_peakPruning_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_peakPruning_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVELIB_peakPruning_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVELIB_peakPruning_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))

###################################################################################
# Debug Test Files
###################################################################################
lib_peakPruning_DEBUG_MAP  = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_libdebug_peakPruning.map
lib_peakPruning_DEBUG_OUT  = test/$(MMWAVE_SDK_DEVICE_TYPE)/$(MMWAVE_SDK_DEVICE_TYPE)_libdebug_peakPruning.$(C66_EXE_EXT)
lib_peakPruning_DEBUG_SOURCES  =  lib_peakpruning_test.c \
								  lib_peakpruning_debug.c

lib_peakPruning_DEBUG_SOURCES_GEN = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

lib_peakPruning_DEBUG_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(lib_peakPruning_DEBUG_SOURCES:.c=.$(C66_DEP_EXT)))
lib_peakPruning_DEBUG_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(lib_peakPruning_DEBUG_SOURCES:.c=.$(C66_OBJ_EXT)))
lib_peakPruning_DEBUG_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(lib_peakPruning_DEBUG_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))

OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Debug Test:
###################################################################################
peakPruningDebug: C66_CFLAGS += --define=DEBUGPEAKPRUNING
#peakPruningDebug: C66_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C66_CFLAGS))
peakPruningDebug: C66_peakprunign_LDFLAGS = -mv6740 --abi=eabi -g --define=$(PLATFORM_DEFINE) --display_error_number           \
               --diag_warning=225 --diag_wrap=off -z --reread_libs --warn_sections --ram_model --define=MMWAVE_L3RAM_SIZE=0xC0000   \
               -i$(C66_CODEGEN_INSTALL_PATH)/lib --emit_warnings_as_errors \
                -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)
peakPruningDebug: buildDirectories dssbuildDirectories $(lib_peakPruning_DEBUG_OBJECTS) $(lib_peakPruning_DEBUG_OBJECTS_GEN)
	$(C66_LD) $(C66_peakprunign_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(lib_peakPruning_DEBUG_MAP) $(lib_peakPruning_DEBUG_OBJECTS) $(lib_peakPruning_DEBUG_OBJECTS_GEN)	\
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_peakPruning_TEST_APP_CMD) \
	-o $(lib_peakPruning_DEBUG_OUT)
	@echo "******************************************************************************"
	@echo 'Built the Peak Group/Pruning DEBUG OUT'
	@echo "******************************************************************************"


###################################################################################
# Build Unit Test:
###################################################################################
peakPruningTest: C66_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C66_CFLAGS))
peakPruningTest: C66_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
								-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
								-llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)    \
								-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)   
peakPruningTest: buildDirectories dssbuildDirectories $(MMWAVELIB_peakPruning_TEST_OBJECTS) $(MMWAVELIB_peakPruning_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MMWAVELIB_peakPruning_TEST_MAP) $(MMWAVELIB_peakPruning_TEST_OBJECTS) $(MMWAVELIB_peakPruning_TEST_OBJECTS_GEN) \
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_peakPruning_TEST_APP_CMD) \
	-o $(MMWAVELIB_peakPruning_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmwavelib Peak Group/Pruning Test OUT'
	@echo "******************************************************************************"

###################################################################################
# Cleanup DEBUG Test:
###################################################################################
peakPruningDebugClean:
	@echo 'Cleaning the peakPruningDebug objects'
	@rm -f $(lib_peakPruning_DEBUG_OBJECTS) $(lib_peakPruning_DEBUG_OBJECTS_GEN)
	@$(DEL) $(lib_peakPruning_DEBUG_OUT) $(lib_peakPruning_DEBUG_DEPENDS)
	@$(DEL) $(lib_peakPruning_DEBUG_MAP)

###################################################################################
# Cleanup Unit Test:
###################################################################################
peakPruningClean:
	@echo 'Cleaning the peakPruningTest objects'
	@rm -f $(MMWAVELIB_peakPruning_TEST_OBJECTS) $(MMWAVELIB_peakPruning_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVELIB_peakPruning_TEST_OUT) $(MMWAVELIB_peakPruning_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_peakPruning_TEST_MAP)

# Dependency handling
-include $(MMWAVELIB_peakPruning_TEST_DEPENDS)
-include $(lib_peakPruning_DEBUG_DEPENDS)
