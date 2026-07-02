###################################################################################
# Object Detection DPC Test
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/common \
          src \
          test \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/board

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f
MSS_CPU_INSTANCE_NUM := mcu1_0
endif
###################################################################################
# The Object Detection DPC Test requires additional libraries
###################################################################################
OBJECTDETECTION_TEST_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)    \
                        -llibtestlogger_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
                        -llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT) \
                        -llibmathutils.$($(MSS_CPU)_LIB_EXT) \
                        -llibrangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)     \
                        -llibdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)   \
                        -llibcfarproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)    \
                        -llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)      \
                        -llibaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)

OBJECTDETECTION_TEST_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB)    \
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/testlogger/lib \
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib \
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib \
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/lib  \
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerproc/lib    \
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/cfarproc/lib   \
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib \
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/aoaproc/lib

###################################################################################
# Unit Test Files
###################################################################################
OBJECTDETECTION_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
OBJECTDETECTION_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetection_test_mss.map
OBJECTDETECTION_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetection_test_mss.$($(MSS_CPU)_EXE_EXT)
OBJECTDETECTION_TEST_APP_CMD   = test/mss_objectdetection_test_linker.cmd
OBJECTDETECTION_TEST_SOURCES   = main.c \
								objectdetection.c \
								gen_frame_data.c \
								antenna_geometry.c

OBJECTDETECTION_TEST_SOURCES_GEN  = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

OBJECTDETECTION_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
OBJECTDETECTION_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

OBJECTDETECTION_TEST_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(OBJECTDETECTION_TEST_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build Unit Test:
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
APP_RESOURCE_FILE_DEVICE_TYPE = $(MMWAVE_SDK_DEVICE_TYPE)/mmw_tdm
else
APP_RESOURCE_FILE_DEVICE_TYPE = awr294x/mmw
endif

mssTest: $(MSS_CPU)_CFLAGS += -DAPP_RESOURCE_FILE='<ti/demo/$(APP_RESOURCE_FILE_DEVICE_TYPE)/mmw_resTDM.h>' \
                       -DDebugP_LOG_ENABLED \
                       -DOBJ_DETECTION_TEST

mssTest: buildDirectories mssbuildDirectories $(OBJECTDETECTION_TEST_OBJECTS) $(OBJECTDETECTION_TEST_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(OBJECTDETECTION_TEST_LOC_LIBS) -Wl,-m=$(OBJECTDETECTION_TEST_MAP) \
	-o $(OBJECTDETECTION_TEST_OUT) $(OBJECTDETECTION_TEST_OBJECTS) $(OBJECTDETECTION_TEST_OBJECTS_GEN) \
	$(OBJECTDETECTION_TEST_STD_LIBS) $(PLATFORM_$(MSS_CPU)_LINK_CMD) $(OBJECTDETECTION_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the Object Detection HWA DPC MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the Object Detection DPC MSS Unit Test objects'
	@$(DEL) $(OBJECTDETECTION_TEST_OBJECTS) $(OBJECTDETECTION_TEST_OBJECTS_GEN)
	@$(DEL) $(OBJECTDETECTION_TEST_OUT) $(OBJECTDETECTION_TEST_BIN)
	@$(DEL) $(OBJECTDETECTION_TEST_MAP) $(OBJECTDETECTION_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(OBJECTDETECTION_TEST_DEPENDS)
