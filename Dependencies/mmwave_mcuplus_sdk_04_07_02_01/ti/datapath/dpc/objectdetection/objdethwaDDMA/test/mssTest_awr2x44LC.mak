###################################################################################
# Object Detection DPC Test
###################################################################################
.PHONY: mssLcTest mssLcTestClean

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak
###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/common \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/test \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/board

MSS_CPU := R5F

###################################################################################
# The Object Detection DPC Test requires additional libraries
###################################################################################
OBJECTDETECTION_TEST_LC_MSS_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)    \
                        -llibmathutils.$($(MSS_CPU)_LIB_EXT) \

OBJECTDETECTION_TEST_LC_MSS_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB)			\
                        -Wl,-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib \

###################################################################################
# Unit Test Files
###################################################################################
OBJECTDETECTION_TEST_LC_MSS_CMD   = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/awr2x44x/awr2x44LC/r5f_linker.cmd
OBJECTDETECTION_TEST_LC_MSS_MAP   = test/awr2x44LC$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetectionDDMA_test_mss.map
OBJECTDETECTION_TEST_LC_MSS_OUT   = test/awr2x44LC$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetectionDDMA_test_mss.$($(MSS_CPU)_EXE_EXT)
OBJECTDETECTION_TEST_LC_MSS_SOURCES   = objectdetection_elevEst.c \
                                    main_elevEst.c \
                                    antenna_geometry.c \

OBJECTDETECTION_TEST_LC_MSS_SOURCES_GEN  = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

OBJECTDETECTION_TEST_LC_MSS_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_LC_MSS_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
OBJECTDETECTION_TEST_LC_MSS_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_LC_MSS_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

OBJECTDETECTION_TEST_LC_MSS_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(OBJECTDETECTION_TEST_LC_MSS_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
mssLcTest: $(MSS_CPU)_CFLAGS += -DAPP_RESOURCE_FILE='<ti/demo/awr2x44P/mmw_ddm/mmw_resDDM.h>' \
                            -DOBJ_DETECTION_DDMA_TEST \
                            -DSOC_AWR2X44LC \
                            -DMSS_AOA_ENABLED=1

#mssLcTest: $(MSS_CPU)_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$($(MSS_CPU)_CFLAGS))


mssLcTest: buildDirectories mssbuildDirectories $(OBJECTDETECTION_TEST_LC_MSS_OBJECTS) $(OBJECTDETECTION_TEST_LC_MSS_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(OBJECTDETECTION_TEST_LC_MSS_LOC_LIBS) -Wl,-m=$(OBJECTDETECTION_TEST_LC_MSS_MAP) \
    -o $(OBJECTDETECTION_TEST_LC_MSS_OUT) $(OBJECTDETECTION_TEST_LC_MSS_OBJECTS) $(OBJECTDETECTION_TEST_LC_MSS_OBJECTS_GEN) \
    $(OBJECTDETECTION_TEST_LC_MSS_STD_LIBS) $(OBJECTDETECTION_TEST_LC_MSS_CMD)
	@echo "******************************************************************************"
	@echo 'Built the Object Detection HWA DDMA DPC MSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
mssLcTestClean:
	@echo 'Cleaning the Object Detection HWA DDMA DPC MSS Unit Test objects'
	@$(DEL) $(OBJECTDETECTION_TEST_LC_MSS_OBJECTS) $(OBJECTDETECTION_TEST_LC_MSS_OBJECTS_GEN)
	@$(DEL) $(OBJECTDETECTION_TEST_LC_MSS_OUT)
	@$(DEL) $(OBJECTDETECTION_TEST_LC_MSS_MAP) $(OBJECTDETECTION_TEST_LC_MSS_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(OBJECTDETECTION_TEST_LC_MSS_DEPENDS)
