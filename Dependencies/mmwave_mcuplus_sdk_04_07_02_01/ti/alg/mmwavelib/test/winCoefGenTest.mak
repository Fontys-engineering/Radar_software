###################################################################################
#	makefile for mmwavelib window coefficients generation on DSP Sub System
###################################################################################

.PHONY: windowCoefGenTest windowCoefGenClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c test/generated

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_winCoefGen_TEST_APP_CMD  = test/dss_mmwavelib_linker.cmd
MMWAVELIB_winCoefGen_TEST_MAP      = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_winCoefGen.map
MMWAVELIB_winCoefGen_TEST_OUT      = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_winCoefGen.$(C66_EXE_EXT)
MMWAVELIB_winCoefGen_TEST_SOURCES  =  lib_windowCoef_gen_test.c

MMWAVELIB_winCoefGen_TEST_SOURCES_GEN = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

MMWAVELIB_winCoefGen_TEST_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_winCoefGen_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVELIB_winCoefGen_TEST_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_winCoefGen_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVELIB_winCoefGen_TEST_OBJECTS_GEN  = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVELIB_winCoefGen_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))


#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
windowCoefGenTest : C66_CFLAGS  += -i$(C66x_MATHLIB_INSTALL_PATH)/packages
#windowCoefGenTest: C66_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C66_CFLAGS))
windowCoefGenTest: C66_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
                                       -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                                       -i$(C66x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib \
                                       -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)    \
                                       -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT) \
                                       -lmathlib.$(C66_LIB_EXT)

windowCoefGenTest: buildDirectories dssbuildDirectories $(MMWAVELIB_winCoefGen_TEST_OBJECTS) $(MMWAVELIB_winCoefGen_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MMWAVELIB_winCoefGen_TEST_MAP) $(MMWAVELIB_winCoefGen_TEST_OBJECTS) $(MMWAVELIB_winCoefGen_TEST_OBJECTS_GEN) \
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_winCoefGen_TEST_APP_CMD) \
	-o $(MMWAVELIB_winCoefGen_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmwavelib Window Factor Generation Test OUT'
	@echo "******************************************************************************"


###################################################################################
# Cleanup Unit Test:
###################################################################################
windowCoefGenClean:
	@echo 'Cleaning the mmwavelib Window Factor Generation test objects'
	@rm -f $(MMWAVELIB_winCoefGen_TEST_OBJECTS) $(MMWAVELIB_winCoefGen_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVELIB_winCoefGen_TEST_OUT) $(MMWAVELIB_winCoefGen_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_winCoefGen_TEST_MAP)

# Dependency handling
-include $(MMWAVELIB_winCoefGen_TEST_DEPENDS)
