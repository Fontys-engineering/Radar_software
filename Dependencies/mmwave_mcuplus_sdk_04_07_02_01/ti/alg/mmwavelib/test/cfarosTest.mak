###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: cfarosTest cfarosClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c test/generated

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_CFAROS_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_cfaros.map
MMWAVELIB_CFAROS_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_cfaros.$(C66_EXE_EXT)
MMWAVELIB_CFAROS_TEST_APP_CMD	 = test/dss_mmwavelib_linker.cmd

MMWAVELIB_CFAROS_TEST_SOURCES  = lib_cfaros_test.c

MMWAVELIB_CFAROS_TEST_SOURCES_GEN = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

# Compiler Flags for DSP Unit Test Builds:
OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

MMWAVELIB_CFAROS_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_CFAROS_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVELIB_CFAROS_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_CFAROS_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVELIB_CFAROS_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVELIB_CFAROS_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))

###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
C66_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
				-llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)    \
				-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)   

###################################################################################
# Build Unit Test:
###################################################################################
cfarosTest: C66_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C66_CFLAGS))
cfarosTest: buildDirectories dssbuildDirectories $(MMWAVELIB_CFAROS_TEST_OBJECTS) $(MMWAVELIB_CFAROS_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MMWAVELIB_CFAROS_TEST_MAP) $(MMWAVELIB_CFAROS_TEST_OBJECTS) $(MMWAVELIB_CFAROS_TEST_OBJECTS_GEN) \
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_CFAROS_TEST_APP_CMD) \
	-o $(MMWAVELIB_CFAROS_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmwavelib fixed-point CFAR OS Unit Test OUT'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
cfarosClean:
	@echo 'Cleaning the cfarcaFltTest objects'
	@rm -f $(MMWAVELIB_CFAROS_TEST_OBJECTS) $(MMWAVELIB_CFAROS_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVELIB_CFAROS_TEST_OUT) $(MMWAVELIB_CFAROS_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_CFAROS_TEST_MAP)

# Dependency handling
-include $(MMWAVELIB_CFAROS_TEST_DEPENDS)

