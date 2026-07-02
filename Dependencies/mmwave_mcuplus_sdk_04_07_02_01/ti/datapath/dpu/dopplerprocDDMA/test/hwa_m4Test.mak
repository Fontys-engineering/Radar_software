###################################################################################
# DOPPLER PROC Test
###################################################################################
.PHONY: hwaM4Test hwaM4TestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerprocDDMA/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerprocDDMA/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/randomdatagenerator
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/fft

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2x44P), )
M4_CPU := M4
M4_CPU_INSTANCE := m4
endif

###################################################################################
# The DOPPLER PROC Test requires additional libraries
###################################################################################
HWADOPPLERPROC_M4_TEST_STD_LIBS = $($(M4_CPU)_COMMON_STD_LIB)
HWADOPPLERPROC_M4_TEST_LOC_LIBS = $($(M4_CPU)_COMMON_LOC_LIB)


###################################################################################
# Unit Test Files
###################################################################################
HWADOPPLERPROC_M4_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWADOPPLERPROC_M4_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_DDM_m4.map
HWADOPPLERPROC_M4_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_DDM_m4.$($(M4_CPU)_EXE_EXT)
HWADOPPLERPROC_M4_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)_dopplerprochwa_DDM_m4.bin
HWADOPPLERPROC_M4_TEST_APP_CMD   = test/m4_dopplerproc_linker_awr2x44P.cmd

HWADOPPLERPROC_M4_TEST_SOURCES   =  doppleprochwa_test_main.c \
									dopplerprochwaDDMA.c \
									gen_rand_data.c \
                                    dpedmahwa.c \
                                    dpedma.c \
									fft.c \
									mathutils.c

HWADOPPLERPROC_M4_TEST_SOURCES_GEN  = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

HWADOPPLERPROC_M4_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_M4_TEST_SOURCES:.c=.$($(M4_CPU)_DEP_EXT)))
HWADOPPLERPROC_M4_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWADOPPLERPROC_M4_TEST_SOURCES:.c=.$($(M4_CPU)_OBJ_EXT)))

HWADOPPLERPROC_M4_TEST_OBJECTS_GEN	 = $(addprefix $(PLATFORM_OBJDIR)/m4generated/, $(HWADOPPLERPROC_M4_TEST_SOURCES_GEN:.c=.$($(M4_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
hwaM4Test: $(M4_CPU)_CFLAGS += -DDATAPATH_TEST
hwaM4Test: buildDirectories m4buildDirectories $(HWADOPPLERPROC_M4_TEST_OBJECTS) $(HWADOPPLERPROC_M4_TEST_OBJECTS_GEN)
	$($(M4_CPU)_LD) $($(M4_CPU)_LDFLAGS) $(HWADOPPLERPROC_M4_TEST_LOC_LIBS) -Wl,-m=$(HWADOPPLERPROC_M4_TEST_MAP) \
	-o $(HWADOPPLERPROC_M4_TEST_OUT) $(HWADOPPLERPROC_M4_TEST_OBJECTS) $(HWADOPPLERPROC_M4_TEST_OBJECTS_GEN) \
	$(HWADOPPLERPROC_M4_TEST_STD_LIBS) $(PLATFORM_$(M4_CPU)_LINK_CMD) $(HWADOPPLERPROC_M4_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the doppler proc DDMA M4 Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
hwaM4TestClean:
	@echo 'Cleaning the DOPPLER PROC DDMA M4 Unit Test objects'
	@$(DEL) $(HWADOPPLERPROC_M4_TEST_OBJECTS) $(HWADOPPLERPROC_M4_TEST_OUT) $(HWADOPPLERPROC_M4_TEST_BIN)
	@$(DEL) $(HWADOPPLERPROC_M4_TEST_MAP) $(HWADOPPLERPROC_M4_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWADOPPLERPROC_M4_TEST_DEPENDS)
