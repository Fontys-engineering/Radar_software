###################################################################################
# CFAR PROC Test
###################################################################################
.PHONY: cfarHwaMssTest cfarHwaMssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/cfarproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/randomdatagenerator
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f
endif

###################################################################################
# Libraries
###################################################################################
HWACFARPROC_TEST_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)	\
						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
						-llibcfarproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)
HWACFARPROC_TEST_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB)	\
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/cfarproc/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib


###################################################################################
# Unit Test Files
###################################################################################
HWACFARPROC_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWACFARPROC_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_cfarprochwa_mss.map
HWACFARPROC_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_cfarprochwa_mss.$($(MSS_CPU)_EXE_EXT)
HWACFARPROC_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)_cfarprochwa_mss.bin
HWACFARPROC_TEST_APP_CMD   = test/mss_cfarproc_linker.cmd
HWACFARPROC_TEST_SOURCES   = cfarprochwa_test_main.c \
							 gen_rand_data.c \
							 mathutils.c

HWACFARPROC_TEST_SOURCES_GEN  = ti_board_config.c	\
								ti_board_open_close.c	\
								ti_dpl_config.c	\
								ti_drivers_config.c	\
								ti_pinmux_config.c	\
								ti_power_clock_config.c	\
								ti_drivers_open_close.c

HWACFARPROC_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWACFARPROC_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
HWACFARPROC_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWACFARPROC_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

HWACFARPROC_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(HWACFARPROC_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
cfarHwaMssTest: buildDirectories mssbuildDirectories $(HWACFARPROC_TEST_OBJECTS) $(HWACFARPROC_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(HWACFARPROC_TEST_LOC_LIBS) -Wl,-m=$(HWACFARPROC_TEST_MAP) \
	-o $(HWACFARPROC_TEST_OUT) $(HWACFARPROC_TEST_OBJECTS) $(HWACFARPROC_TEST_OBJECTS_GEN) \
	$(HWACFARPROC_TEST_STD_LIBS) $(PLATFORM_$(MSS_CPU)_LINK_CMD) $(HWACFARPROC_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the CFAR HWA DPU MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
cfarHwaMssTestClean:
	@echo 'Cleaning the CFAR HWA DPU MSS Unit Test objects'
	@$(DEL) $(HWACFARPROC_TEST_OBJECTS) $(HWACFARPROC_TEST_OBJECTS_GEN)
	@$(DEL) $(HWACFARPROC_TEST_OUT) $(HWACFARPROC_TEST_BIN)
	@$(DEL) $(HWACFARPROC_TEST_MAP) $(HWACFARPROC_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWACFARPROC_TEST_DEPENDS)
