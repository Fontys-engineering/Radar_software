###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: cfarcaFltTest cfarcaFltClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c test/generated

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_cfarcaFlt_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_cfarcaFlt.map
MMWAVELIB_cfarcaFlt_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_cfarcaFlt.$(C66_EXE_EXT)
MMWAVELIB_cfarcaFlt_TEST_APP_CMD	 = test/dss_mmwavelib_linker.cmd

MMWAVELIB_cfarcaFlt_TEST_SOURCES  = lib_cfarcaall_test.c

MMWAVELIB_cfarcaFlt_TEST_SOURCES_GEN = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

# Compiler Flags for DSP Builds:
MMWAVELIB_cfarcaFlt_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_cfarcaFlt_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVELIB_cfarcaFlt_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_cfarcaFlt_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVELIB_cfarcaFlt_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVELIB_cfarcaFlt_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))

C66_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
				-llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)    \
				-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)   

###################################################################################
# Build Unit Test:
###################################################################################
cfarcaFltTest: buildDirectories dssbuildDirectories $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS) $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MMWAVELIB_cfarcaFlt_TEST_MAP) $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS) $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS_GEN) \
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_cfarcaFlt_TEST_APP_CMD) \
	-o $(MMWAVELIB_cfarcaFlt_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmwavelib Floating-point CFAR CA, CACC, SO, GO Test OUT'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
cfarcaFltClean:
	@echo 'Cleaning the cfarcaFltTest objects'
	@rm -f $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS) $(MMWAVELIB_cfarcaFlt_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVELIB_cfarcaFlt_TEST_OUT) $(MMWAVELIB_cfarcaFlt_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_cfarcaFlt_TEST_MAP)

# Dependency handling
-include $(MMWAVELIB_cfarcaFlt_TEST_DEPENDS)

