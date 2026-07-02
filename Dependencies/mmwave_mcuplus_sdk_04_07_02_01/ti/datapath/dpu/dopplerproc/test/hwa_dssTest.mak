###################################################################################
# DOPPLER PROC Test
###################################################################################
.PHONY: hwaDssTest hwaDssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/randomdatagenerator
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/fft

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
DSS_CPU := C66
DSS_CPU_INSTANCE := c66
endif

###################################################################################
# The DOPPLER PROC Test requires additional libraries
###################################################################################
HWADOPPLERPROC_DSS_TEST_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)	\
						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT)
HWADOPPLERPROC_DSS_TEST_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)	\
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerproc/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib


###################################################################################
# Unit Test Files
###################################################################################
HWADOPPLERPROC_DSS_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWADOPPLERPROC_DSS_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_dss.map
HWADOPPLERPROC_DSS_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_dss.$($(DSS_CPU)_EXE_EXT)
HWADOPPLERPROC_DSS_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_dss.bin
HWADOPPLERPROC_DSS_TEST_APP_CMD   = test/dss_dopplerproc_linker.cmd
HWADOPPLERPROC_DSS_TEST_SOURCES   = doppleprochwa_test_main.c \
									gen_rand_data.c \
									fft.c \
									mathutils.c

HWADOPPLERPROC_DSS_TEST_SOURCES_GEN  = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

HWADOPPLERPROC_DSS_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
HWADOPPLERPROC_DSS_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_DSS_TEST_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

HWADOPPLERPROC_DSS_TEST_OBJECTS_GEN	 = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(HWADOPPLERPROC_DSS_TEST_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
hwaDssTest: buildDirectories dssbuildDirectories $(HWADOPPLERPROC_DSS_TEST_OBJECTS) $(HWADOPPLERPROC_DSS_TEST_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(HWADOPPLERPROC_DSS_TEST_LOC_LIBS) $(HWADOPPLERPROC_DSS_TEST_STD_LIBS) \
	--map_file=$(HWADOPPLERPROC_DSS_TEST_MAP) $(HWADOPPLERPROC_DSS_TEST_OBJECTS) $(HWADOPPLERPROC_DSS_TEST_OBJECTS_GEN) \
	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(HWADOPPLERPROC_DSS_TEST_APP_CMD) -o $(HWADOPPLERPROC_DSS_TEST_OUT)
#	$($(DSS_CPU)_LD_RTS_FLAGS) -o $(HWADOPPLERPROC_DSS_TEST_OUT)
	@echo "******************************************************************************"
	@echo 'Built the doppler proc DSP Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
hwaDssTestClean:
	@echo 'Cleaning the DOPPLER PROC DSP Unit Test objects'
	@$(DEL) $(HWADOPPLERPROC_DSS_TEST_OBJECTS) $(HWADOPPLERPROC_DSS_TEST_OUT) $(HWADOPPLERPROC_DSS_TEST_BIN)
	@$(DEL) $(HWADOPPLERPROC_DSS_TEST_MAP) $(HWADOPPLERPROC_DSS_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWADOPPLERPROC_DSS_TEST_DEPENDS)
