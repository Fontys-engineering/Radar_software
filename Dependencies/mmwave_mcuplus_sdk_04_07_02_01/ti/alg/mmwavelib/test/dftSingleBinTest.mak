###################################################################################
#	makefile for mmwavelib DFT single bin related functions tests
###################################################################################

.PHONY: dftSingleBinTest dftSingleBinClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c test/generated

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_dftSingleBin_TEST_APP_CMD	 = test/dss_mmwavelib_linker.cmd
MMWAVELIB_dftSingleBin_TEST_MAP    = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dftSingleBin.map
MMWAVELIB_dftSingleBin_TEST_OUT    = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dftSingleBin.$(C66_EXE_EXT)
MMWAVELIB_dftSingleBin_TEST_SOURCES  =  lib_dftsinglebin_test.c


MMWAVELIB_dftSingleBin_TEST_SOURCES_GEN = ti_board_config.c	\
										  ti_board_open_close.c	\
										  ti_dpl_config.c	\
										  ti_drivers_config.c	\
										  ti_pinmux_config.c	\
										  ti_power_clock_config.c	\
										  ti_drivers_open_close.c

MMWAVELIB_dftSingleBin_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_dftSingleBin_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVELIB_dftSingleBin_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_dftSingleBin_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVELIB_dftSingleBin_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVELIB_dftSingleBin_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))


#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
dftSingleBinTest : C66_CFLAGS  += -i$(C66x_MATHLIB_INSTALL_PATH)/packages
#dftSingleBinTest : C66_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C66_CFLAGS))
dftSingleBinTest : C66_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
                                       -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                                       -i$(C66x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib \
                                       -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)    \
                                       -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT) \
                                       -lmathlib.$(C66_LIB_EXT)

dftSingleBinTest: buildDirectories dssbuildDirectories $(MMWAVELIB_dftSingleBin_TEST_OBJECTS) $(MMWAVELIB_dftSingleBin_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MMWAVELIB_dftSingleBin_TEST_MAP) $(MMWAVELIB_dftSingleBin_TEST_OBJECTS) $(MMWAVELIB_dftSingleBin_TEST_OBJECTS_GEN)	\
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_dftSingleBin_TEST_APP_CMD) \
	-o $(MMWAVELIB_dftSingleBin_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmwavelib DFT single bin related function Test OUT'
	@echo "******************************************************************************"



###################################################################################
# Cleanup Unit Test:
###################################################################################
dftSingleBinClean:
	@echo 'Cleaning the DFT single bin related function test objects'
	@rm -f $(MMWAVELIB_dftSingleBin_TEST_OBJECTS) $(MMWAVELIB_dftSingleBin_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVELIB_dftSingleBin_TEST_OUT) $(MMWAVELIB_dftSingleBin_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_dftSingleBin_TEST_MAP)

# Dependency handling
-include $(MMWAVELIB_dftSingleBin_TEST_DEPENDS)
