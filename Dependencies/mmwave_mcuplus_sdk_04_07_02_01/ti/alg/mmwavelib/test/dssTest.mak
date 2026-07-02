###################################################################################
#	makefile for mmwavelib Unit Test on DSP Sub System
###################################################################################
.PHONY: dssTest dssTestClean floatTest floatTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c test/
vpath %.c test/generated
vpath %.c $(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16/c66
vpath %.c $(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft32x32/c66
vpath %.c $(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66

###################################################################################
# Unit Test Files
###################################################################################
MMWAVELIB_UNIT_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dss.map
MMWAVELIB_UNIT_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_dss.$(C66_EXE_EXT)
MMWAVELIB_UNIT_TEST_APP_CMD	 = test/dss_mmwavelib_linker.cmd

MMWAVELIB_UNIT_TEST_SOURCES  = 	lib_unittest.c\
								gen_twiddle_fft16x16.c\
								gen_twiddle_fft32x32.c\
								lib_abslog2_test.c\
								lib_window_test.c\
								lib_shift_histogram_test.c

MMWAVELIB_UNIT_TEST_SOURCES_GEN  = ti_board_config.c \
								ti_board_open_close.c \
								ti_dpl_config.c \
								ti_drivers_config.c \
								ti_pinmux_config.c \
								ti_power_clock_config.c \
								ti_drivers_open_close.c

MMWAVELIB_UNIT_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_UNIT_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVELIB_UNIT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_UNIT_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVELIB_UNIT_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVELIB_UNIT_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))


MMWAVELIB_FLOAT_TEST_SOURCES  = libfloat_test.c\
								gen_twiddle_fft16x16.c

MMWAVELIB_FLOAT_TEST_SOURCES_GEN = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

MMWAVELIB_FLOATTEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_FLOAT_TEST_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVELIB_FLOAT_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVELIB_FLOAT_TEST_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVELIB_FLOAT_TEST_OBJECTS_GEN  = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(MMWAVELIB_FLOAT_TEST_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))

MMWAVELIB_FLOAT_TEST_MAP		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_float.map
MMWAVELIB_FLOAT_TEST_OUT		 = test/$(MMWAVE_SDK_DEVICE_TYPE)_mmwavealg_float.$(C66_EXE_EXT)
								
# Compiler Flags for DSP Builds:
C66_CFLAGS  += -i$(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft16x16/c66	\
				-i$(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSP_fft32x32/c66 \
				-i$(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/src/DSPF_sp_fftSPxSP/c66 


###################################################################################
# Additional libraries which are required to build the Unit Test:
###################################################################################
C66_LDFLAGS  += -i$(C64Px_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib    \
				-i$(C66x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib    \
				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/alg/mmwavelib/lib  \
				-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib    \
				-llibmmwavealg_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)    \
				-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)   \
				-ldsplib.ae66 

###################################################################################
# Build Unit Test:
###################################################################################
floatTest: buildDirectories dssbuildDirectories $(MMWAVELIB_FLOAT_TEST_OBJECTS) $(MMWAVELIB_FLOAT_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MMWAVELIB_FLOAT_TEST_MAP) $(MMWAVELIB_FLOAT_TEST_OBJECTS) $(MMWAVELIB_FLOAT_TEST_OBJECTS_GEN) \
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_UNIT_TEST_APP_CMD) \
	-o $(MMWAVELIB_FLOAT_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmwavelib Floating-Point Chain Test OUT'
	@echo "******************************************************************************"


dssTest: buildDirectories dssbuildDirectories $(MMWAVELIB_UNIT_TEST_OBJECTS) $(MMWAVELIB_UNIT_TEST_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB) \
	--map_file=$(MMWAVELIB_UNIT_TEST_MAP) $(MMWAVELIB_UNIT_TEST_OBJECTS) $(MMWAVELIB_UNIT_TEST_OBJECTS_GEN) \
	$(PLATFORM_C66X_LINK_CMD) $(MMWAVELIB_UNIT_TEST_APP_CMD) \
	-o $(MMWAVELIB_UNIT_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the mmwavelib Fixed-Point Unit Test OUT'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
floatTestClean:
	@echo 'Cleaning the floating-point chain test objects'
	@$(DEL) $(MMWAVELIB_FLOAT_TEST_OBJECTS) $(MMWAVELIB_FLOAT_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVELIB_FLOAT_TEST_OUT) $(MMWAVELIB_FLOAT_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_FLOAT_TEST_MAP)

dssTestClean:
	@echo 'Cleaning the fixed-point unit test objects'
	@$(DEL) $(MMWAVELIB_UNIT_TEST_OBJECTS) $(MMWAVELIB_UNIT_TEST_OBJECTS_GEN)
	@$(DEL) $(MMWAVELIB_UNIT_TEST_OUT) $(MMWAVELIB_UNIT_TEST_DEPENDS)
	@$(DEL) $(MMWAVELIB_UNIT_TEST_MAP)
	
# Dependency handling
-include $(MMWAVELIB_UNIT_TEST_DEPENDS)
-include $(MMWAVELIB_FLOAT_TEST_DEPENDS)

