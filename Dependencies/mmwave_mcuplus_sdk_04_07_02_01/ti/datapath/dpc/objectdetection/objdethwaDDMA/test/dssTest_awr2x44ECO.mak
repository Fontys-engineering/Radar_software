###################################################################################
# Object Detection DPC Test
###################################################################################
.PHONY: dssEcoTest dssEcoTestClean

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak
###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/common \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/test \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/board

DSS_CPU := C66

###################################################################################
# The Object Detection DPC Test requires additional libraries
###################################################################################
OBJECTDETECTION_TEST_ECO_DSS_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB)    \
                        -lmathlib.$($(DSS_CPU)_LIB_EXT) \
                        -llibmathutils.$($(DSS_CPU)_LIB_EXT) \

OBJECTDETECTION_TEST_ECO_DSS_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB)			\
                        -i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib \
                        -i$($(DSS_CPU)x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib \

###################################################################################
# Unit Test Files
###################################################################################
OBJECTDETECTION_TEST_ECO_DSS_MAP       = test/awr2x44ECO$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetectionDDMA_test_dss.map
OBJECTDETECTION_TEST_ECO_DSS_OUT       = test/awr2x44ECO$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetectionDDMA_test_dss.$($(DSS_CPU)_EXE_EXT)
OBJECTDETECTION_TEST_ECO_DSS_APP_CMD   = test/dss_objectdetection_test_linker_awr2x44P.cmd
OBJECTDETECTION_TEST_ECO_DSS_SOURCES   = objectdetection_elevEst.c \
                                    main_elevEst.c \
                                    antenna_geometry.c

OBJECTDETECTION_TEST_ECO_DSS_SOURCES_GEN  = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

OBJECTDETECTION_TEST_ECO_DSS_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_ECO_DSS_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
OBJECTDETECTION_TEST_ECO_DSS_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_ECO_DSS_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))

OBJECTDETECTION_TEST_ECO_DSS_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(OBJECTDETECTION_TEST_ECO_DSS_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################

dssEcoTest: $(DSS_CPU)_CFLAGS += --define=APP_RESOURCE_FILE='<ti/demo/awr2x44P/mmw_ddm/mmw_resDDM.h>' \
                                -i$($(DSS_CPU)x_MATHLIB_INSTALL_PATH)/packages \
                                --define=OBJ_DETECTION_DDMA_TEST \
                            	--define=SOC_AWR2X44ECO \
								--define=MSS_AOA_ENABLED=0
#dssEcoTest: $(DSS_CPU)_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$($(DSS_CPU)_CFLAGS))


dssEcoTest: buildDirectories dssbuildDirectories $(OBJECTDETECTION_TEST_ECO_DSS_OBJECTS) $(OBJECTDETECTION_TEST_ECO_DSS_OBJECTS_GEN)
	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(OBJECTDETECTION_TEST_ECO_DSS_LOC_LIBS) $(OBJECTDETECTION_TEST_ECO_DSS_STD_LIBS) --map_file=$(OBJECTDETECTION_TEST_ECO_DSS_MAP) $(OBJECTDETECTION_TEST_ECO_DSS_OBJECTS) \
	$(OBJECTDETECTION_TEST_ECO_DSS_OBJECTS_GEN) $(OBJECTDETECTION_TEST_ECO_DSS_APP_CMD) -o $(OBJECTDETECTION_TEST_ECO_DSS_OUT)
	@echo "******************************************************************************"
	@echo 'Built the Object Detection HWA DDMA DPC DSS Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
dssEcoTestClean:
	@echo 'Cleaning the Object Detection HWA DDMA DPC DSS Unit Test objects'
	@$(DEL) $(OBJECTDETECTION_TEST_ECO_DSS_OBJECTS) $(OBJECTDETECTION_TEST_ECO_DSS_OBJECTS_GEN)
	@$(DEL) $(OBJECTDETECTION_TEST_ECO_DSS_OUT) $(OBJECTDETECTION_TEST_ECO_DSS_BIN)
	@$(DEL) $(OBJECTDETECTION_TEST_ECO_DSS_MAP) $(OBJECTDETECTION_TEST_ECO_DSS_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(OBJECTDETECTION_TEST_ECO_DSS_DEPENDS)
