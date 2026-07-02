###################################################################################
# Object Detection DPC Test
###################################################################################
.PHONY: m4EcoTest m4EcoTestClean

include $(MMWAVE_SDK_INSTALL_PATH)/ti/common/mmwave_sdk.mak
###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/common \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerprocDDMA/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeprocDDMA/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangecfarprocDDMA/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/test \
          $(MMWAVE_SDK_INSTALL_PATH)/ti/board

M4_CPU := M4

###################################################################################
# The Object Detection DPC Test requires additional libraries
###################################################################################
OBJECTDETECTION_TEST_ECO_M4_STD_LIBS = $($(M4_CPU)_COMMON_STD_LIB) 
OBJECTDETECTION_TEST_ECO_M4_LOC_LIBS = $($(M4_CPU)_COMMON_LOC_LIB)


###################################################################################
# Unit Test Files
###################################################################################
OBJECTDETECTION_TEST_ECO_M4_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/awr2x44x/awr2x44ECO/m4_linker.cmd
OBJECTDETECTION_TEST_ECO_M4_MAP       = test/awr2x44ECO$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetectionDDMA_test_m4.map
OBJECTDETECTION_TEST_ECO_M4_OUT       = test/awr2x44ECO$(MMWAVE_SDK_LIB_BUILD_OPTION)_objectdetectionDDMA_test_m4.$($(M4_CPU)_EXE_EXT)
OBJECTDETECTION_TEST_ECO_M4_APP_CMD   = test/m4_objectdetection_test_linker_awr2x44P.cmd

MSS_AOA_ENABLED?=1

OBJECTDETECTION_TEST_ECO_M4_SOURCES   = objectdetection.c \
                                    rangeprochwaDDMA.c \
                                    dopplerprochwaDDMA.c \
                                    rangecfarprochwa.c \
                                    dpedmahwa.c \
                                    dpedma.c \
                                    main.c \
                                    antenna_geometry.c \
                                    mathutils.c

OBJECTDETECTION_TEST_ECO_M4_SOURCES_GEN  = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

OBJECTDETECTION_TEST_ECO_M4_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_ECO_M4_SOURCES:.c=.$($(M4_CPU)_DEP_EXT)))
OBJECTDETECTION_TEST_ECO_M4_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(OBJECTDETECTION_TEST_ECO_M4_SOURCES:.c=.$($(M4_CPU)_OBJ_EXT)))

OBJECTDETECTION_TEST_ECO_M4_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/m4generated/, $(OBJECTDETECTION_TEST_ECO_M4_SOURCES_GEN:.c=.$($(M4_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################

m4EcoTest: $(M4_CPU)_CFLAGS += -DAPP_RESOURCE_FILE='<ti/demo/awr2x44P/mmw_ddm/mmw_resDDM.h>' \
                            -DOBJ_DETECTION_DDMA_TEST \
                            -DDATAPATH_TEST \
                            -DSOC_AWR2X44ECO \
                            -DMSS_AOA_ENABLED=$(MSS_AOA_ENABLED)
		$(M4_CPU)_LDFLAGS += -Wl,--define=MSS_AOA_ENABLED=$(MSS_AOA_ENABLED)

m4EcoTest: buildDirectories m4buildDirectories $(OBJECTDETECTION_TEST_ECO_M4_OBJECTS) $(OBJECTDETECTION_TEST_ECO_M4_OBJECTS_GEN)
	$($(M4_CPU)_LD) $($(M4_CPU)_LDFLAGS) $(OBJECTDETECTION_TEST_ECO_M4_LOC_LIBS) -Wl,-m=$(OBJECTDETECTION_TEST_ECO_M4_MAP) \
    -o $(OBJECTDETECTION_TEST_ECO_M4_OUT) $(OBJECTDETECTION_TEST_ECO_M4_OBJECTS) $(OBJECTDETECTION_TEST_ECO_M4_OBJECTS_GEN) \
    $(OBJECTDETECTION_TEST_ECO_M4_STD_LIBS) $(OBJECTDETECTION_TEST_ECO_M4_CMD) $(OBJECTDETECTION_TEST_ECO_M4_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the Object Detection HWA DDMA DPC M4 Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
m4EcoTestClean:
	@echo 'Cleaning the Object Detection HWA DDMA DPC M4 Unit Test objects'
	@$(DEL) $(OBJECTDETECTION_TEST_ECO_M4_OBJECTS) $(OBJECTDETECTION_TEST_ECO_M4_OBJECTS_GEN)
	@$(DEL) $(OBJECTDETECTION_TEST_ECO_M4_OUT)
	@$(DEL) $(OBJECTDETECTION_TEST_ECO_M4_MAP) $(OBJECTDETECTION_TEST_ECO_M4_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(OBJECTDETECTION_TEST_ECO_M4_DEPENDS)
