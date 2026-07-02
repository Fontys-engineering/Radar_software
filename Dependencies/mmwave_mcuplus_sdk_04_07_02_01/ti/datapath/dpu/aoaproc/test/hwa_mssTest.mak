###################################################################################
# aoa PROC Test
###################################################################################
.PHONY: aoaHwaMssTest aoaHwaMssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/aoaproc/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/randomdatagenerator
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src


MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f

###################################################################################
# The AOA PROC Test requires additional libraries
###################################################################################
HWAAOAPROC_TEST_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)	\
						-llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
						-llibaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)
HWAAOAPROC_TEST_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB)	\
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/aoaproc/lib \
						-Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib


###################################################################################
# Unit Test Files
###################################################################################
HWAAOAPROC_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
HWAAOAPROC_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_aoaprochwa_mss.map
HWAAOAPROC_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_aoaprochwa_mss.$($(MSS_CPU)_EXE_EXT)
HWAAOAPROC_TEST_BIN       = test/$(MMWAVE_SDK_DEVICE_TYPE)_aoaprochwa_mss.bin
HWAAOAPROC_TEST_APP_CMD   = test/mss_aoaproc_linker.cmd
HWAAOAPROC_TEST_SOURCES   = aoaprochwa_test_main.c \
								gen_rand_data.c \
								 mathutils.c 

HWAAOAPROC_TEST_SOURCES_GEN   = ti_board_config.c	\
								ti_board_open_close.c	\
								ti_dpl_config.c	\
								ti_drivers_config.c	\
								ti_pinmux_config.c	\
								ti_power_clock_config.c	\
								ti_drivers_open_close.c

HWAAOAPROC_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWAAOAPROC_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
HWAAOAPROC_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(HWAAOAPROC_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

HWAAOAPROC_TEST_OBJECTS_GEN  = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(HWAAOAPROC_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
aoaHwaMssTest: buildDirectories mssbuildDirectories $(HWAAOAPROC_TEST_OBJECTS) $(HWAAOAPROC_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(HWAAOAPROC_TEST_LOC_LIBS) -Wl,-m=$(HWAAOAPROC_TEST_MAP) \
	-o $(HWAAOAPROC_TEST_OUT) $(HWAAOAPROC_TEST_OBJECTS) $(HWAAOAPROC_TEST_OBJECTS_GEN) \
	$(HWAAOAPROC_TEST_STD_LIBS) $(PLATFORM_$(MSS_CPU)_LINK_CMD) $(HWAAOAPROC_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the aoa proc R5F Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
aoaHwaMssTestClean:
	@echo 'Cleaning the AOA PROC R5F Unit Test objects'
	@$(DEL) $(HWAAOAPROC_TEST_OBJECTS) $(HWAAOAPROC_TEST_OBJECTS_GEN)
	@$(DEL) $(HWAAOAPROC_TEST_OUT) $(HWAAOAPROC_TEST_BIN)
	@$(DEL) $(HWAAOAPROC_TEST_MAP) $(HWAAOAPROC_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(HWAAOAPROC_TEST_DEPENDS)

