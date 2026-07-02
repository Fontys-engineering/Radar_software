###################################################################################
#	makefile for mmwavelib FFT twiddle factor generation on DSP Sub System
###################################################################################

.PHONY: twiddleFactorGenTest twiddleFactorGenClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c test/generated
vpath %.c $(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16/c66
vpath %.c $(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16_imre/c66
vpath %.c $(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft32x32/c66

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_twiddleGen_TEST_APP_CMD   = test/dss_mmwavelib_linker.cmd
MMWAVELIB_twiddleGen_TEST_MAP    = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_twiddleGen.map
MMWAVELIB_twiddleGen_TEST_OUT    = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_twiddleGen.$(C66_EXE_EXT)
MMWAVELIB_twiddleGen_TEST_SOURCES  =  lib_ffttwiddle_gen_test.c \
                                      gen_twiddle_fft16x16.c \
                                      gen_twiddle_fft16x16_imre.c \
                                      gen_twiddle_fft32x32.c

MMWAVELIB_twiddleGen_TEST_SOURCES_GEN = ti_board_config.c	\
                                        ti_board_open_close.c	\
                                        ti_dpl_config.c	\
                                        ti_drivers_config.c	\
                                        ti_pinmux_config.c	\
                                        ti_power_clock_config.c	\
                                        ti_drivers_open_close.c

MMWAVELIB_twiddleGen_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_twiddleGen_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVELIB_twiddleGen_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_twiddleGen_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVELIB_twiddleGen_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVELIB_twiddleGen_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))


OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
twiddleFactorGenTest: C66_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$(C66_CFLAGS))
twiddleFactorGenTest: C66_LDFLAGS  += -i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
                                       -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
                                       -llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)    \
                                       -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)

twiddleFactorGenTest: buildDirectories dssbuildDirectories $(MMWAVELIB_twiddleGen_TEST_OBJECTS) $(MMWAVELIB_twiddleGen_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MMWAVELIB_twiddleGen_TEST_MAP) $(MMWAVELIB_twiddleGen_TEST_OBJECTS) $(MMWAVELIB_twiddleGen_TEST_OBJECTS_GEN) \
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_twiddleGen_TEST_APP_CMD) \
	-o $(MMWAVELIB_twiddleGen_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmwavelib Generation of FFT Twiddle Factor Test OUT'
	@echo "******************************************************************************"



###################################################################################
# Cleanup Unit Test:
###################################################################################
twiddleFactorGenClean:
	@echo 'Cleaning the FFT twiddle factor generation test objects'
	@rm -f $(MMWAVELIB_twiddleGen_TEST_OBJECTS) $(MMWAVELIB_twiddleGen_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVELIB_twiddleGen_TEST_OUT) $(MMWAVELIB_twiddleGen_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_twiddleGen_TEST_MAP)

# Dependency handling
-include $(MMWAVELIB_twiddleGen_TEST_DEPENDS)
