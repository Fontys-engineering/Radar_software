###################################################################################
# DOPPLER PROC Test
###################################################################################
.PHONY: hwaMssTest hwaMssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/randomdatagenerator
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/fft

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f
endif

###################################################################################
# The DOPPLER PROC Test requires additional libraries
###################################################################################
HWADOPPLERPROC_TEST_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)	\
						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
						-llibdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)
HWADOPPLERPROC_TEST_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB)	\
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerproc/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib


###################################################################################
# Unit Test Files
###################################################################################
HWADOPPLERPROC_TEST_CMD        = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWADOPPLERPROC_TEST_MAP        = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_mss.map
HWADOPPLERPROC_TEST_OUT        = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_mss.$($(MSS_CPU)_EXE_EXT)
HWADOPPLERPROC_TEST_BIN        = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_mss.bin
HWADOPPLERPROC_TEST_APP_CMD    = test/mss_dopplerproc_linker.cmd
HWADOPPLERPROC_TEST_SOURCES    = doppleprochwa_test_main.c \
								gen_rand_data.c \
								fft.c \
								mathutils.c

HWADOPPLERPROC_TEST_SOURCES_GEN = ti_board_config.c	\
								  ti_board_open_close.c	\
								  ti_dpl_config.c	\
								  ti_drivers_config.c	\
								  ti_pinmux_config.c	\
								  ti_power_clock_config.c	\
								  ti_drivers_open_close.c

HWADOPPLERPROC_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
HWADOPPLERPROC_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

HWADOPPLERPROC_TEST_OBJECTS_GEN  = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(HWADOPPLERPROC_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
hwaMssTest: buildDirectories mssbuildDirectories $(HWADOPPLERPROC_TEST_OBJECTS) $(HWADOPPLERPROC_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(HWADOPPLERPROC_TEST_LOC_LIBS) -Wl,-m=$(HWADOPPLERPROC_TEST_MAP) \
	-o $(HWADOPPLERPROC_TEST_OUT) $(HWADOPPLERPROC_TEST_OBJECTS) $(HWADOPPLERPROC_TEST_OBJECTS_GEN) \
	$(HWADOPPLERPROC_TEST_STD_LIBS) $(PLATFORM_$(MSS_CPU)_LINK_CMD) $(HWADOPPLERPROC_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the DOPPLER PROC DPU MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
hwaMssTestClean:
	@echo 'Cleaning the DOPPLER PROC DPU MSS Unit Test objects'
	@$(DEL) $(HWADOPPLERPROC_TEST_OBJECTS) $(HWADOPPLERPROC_TEST_OBJECTS_GEN)
	@$(DEL) $(HWADOPPLERPROC_TEST_OUT) $(HWADOPPLERPROC_TEST_BIN)
	@$(DEL) $(HWADOPPLERPROC_TEST_MAP) $(HWADOPPLERPROC_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWADOPPLERPROC_TEST_DEPENDS)
