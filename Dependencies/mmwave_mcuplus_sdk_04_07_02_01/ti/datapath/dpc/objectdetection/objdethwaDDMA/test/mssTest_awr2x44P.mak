###################################################################################
# Object Detection DPC Test
###################################################################################
.PHONY: mssTest mssTestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/common \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/test \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/board

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2x44P), )
MSS_CPU := R5F
endif
###################################################################################
# The Object Detection DPC Test requires additional libraries
###################################################################################
OBJECTDETECTION_TEST_MSS_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)    \
                        -llibmathutils.$($(MSS_CPU)_LIB_EXT) \

OBJECTDETECTION_TEST_MSS_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB)			\
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib \

###################################################################################
# Unit Test Files
###################################################################################
OBJECTDETECTION_TEST_MSS_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
OBJECTDETECTION_TEST_MSS_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetectionDDMA_test_mss.map
OBJECTDETECTION_TEST_MSS_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetectionDDMA_test_mss.$($(MSS_CPU)_EXE_EXT)
OBJECTDETECTION_TEST_MSS_SOURCES   = objectdetection_elevEst.c \
                                    main_elevEst.c \
                                    antenna_geometry.c \

OBJECTDETECTION_TEST_MSS_SOURCES_GEN  = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

OBJECTDETECTION_TEST_MSS_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_MSS_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
OBJECTDETECTION_TEST_MSS_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_MSS_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

OBJECTDETECTION_TEST_MSS_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(OBJECTDETECTION_TEST_MSS_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
mssTest: $(MSS_CPU)_CFLAGS += -DAPP_RESOURCE_FILE='<ti/demo/$(MMWAVE_SDK_DEVICE_TYPE)/mmw_ddm/mmw_resDDM.h>' \
                            -DOBJ_DETECTION_DDMA_TEST \
                            -DMSS_AOA_ENABLED=1

#mssTest: $(MSS_CPU)_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$($(MSS_CPU)_CFLAGS))


mssTest: buildDirectories mssbuildDirectories $(OBJECTDETECTION_TEST_MSS_OBJECTS) $(OBJECTDETECTION_TEST_MSS_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(OBJECTDETECTION_TEST_MSS_LOC_LIBS) -Wl,-m=$(OBJECTDETECTION_TEST_MSS_MAP) \
    -o $(OBJECTDETECTION_TEST_MSS_OUT) $(OBJECTDETECTION_TEST_MSS_OBJECTS) $(OBJECTDETECTION_TEST_MSS_OBJECTS_GEN) \
    $(OBJECTDETECTION_TEST_MSS_STD_LIBS) $(PLATFORM_$(MSS_CPU)_LINK_CMD)
	@echo "******************************************************************************"
	@echo 'Built the Object Detection HWA DDMA DPC MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssTestClean:
	@echo 'Cleaning the Object Detection HWA DDMA DPC MSS Unit Test objects'
	@$(DEL) $(OBJECTDETECTION_TEST_MSS_OBJECTS) $(OBJECTDETECTION_TEST_MSS_OBJECTS_GEN)
	@$(DEL) $(OBJECTDETECTION_TEST_MSS_OUT) $(OBJECTDETECTION_TEST_MSS_BIN)
	@$(DEL) $(OBJECTDETECTION_TEST_MSS_MAP) $(OBJECTDETECTION_TEST_MSS_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(OBJECTDETECTION_TEST_MSS_DEPENDS)
