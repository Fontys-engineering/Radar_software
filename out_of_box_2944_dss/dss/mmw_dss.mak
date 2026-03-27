###################################################################################
# Millimeter Wave Demo
###################################################################################

.PHONY: dssDemo dssDemoClean dssDemoObjClean
#mmwDssRTSC
###################################################################################
# Setup the VPATH:
###################################################################################
VPATH_TDM = $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwa/src
VPATH_DDM = $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpc/objectdetection/objdethwaDDMA/src
VPATH_COM = $(MMWAVE_SDK_INSTALL_PATH)/ti/demo/utils \
			./dss

DSS_CPU := C66
DSS_CPU_INSTANCE := c66

###################################################################################
# Additional libraries which are required to build the DEMO:
###################################################################################
DSS_MMW_DEMO_STD_LIBS = $($(DSS_CPU)_COMMON_STD_LIB) \
						-llibdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibmathutils.$($(DSS_CPU)_LIB_EXT) \
						-lmathlib.$($(DSS_CPU)_LIB_EXT) \
						-llibdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-ldsplib.$($(DSS_CPU)_LIB_EXT) \

DSS_MMW_DEMO_LOC_LIBS = $($(DSS_CPU)_COMMON_LOC_LIB) \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/control/dpm/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/lib \
						-i$($(DSS_CPU)x_MATHLIB_INSTALL_PATH)/packages/ti/mathlib/lib \
						-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/lib \
						-i$($(DSS_CPU)x_DSPLIB_INSTALL_PATH)/packages/ti/dsplib/lib \

DSS_MMW_DEMO_TDM_DP_LIBS = -llibrangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibdopplerproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibcfarproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						-llibaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \

DSS_MMW_DEMO_TDM_DP_LOC_LIBS = -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeproc/lib \
							-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerproc/lib \
							-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/cfarproc/lib \
							-i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/aoaproc/lib \

DSS_MMW_DEMO_TDM_LIBS = $(DSS_MMW_DEMO_STD_LIBS) \
						$(DSS_MMW_DEMO_TDM_DP_LIBS) \

DSS_MMW_DEMO_TDM_LOC_LIBS = $(DSS_MMW_DEMO_LOC_LIBS) \
							$(DSS_MMW_DEMO_TDM_DP_LOC_LIBS) \

DSS_MMW_DEMO_DDM_DP_LIBS = -llibrangeproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						   -llibdopplerproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \
						   -llibrangecfarproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSS_CPU)_LIB_EXT) \

DSS_MMW_DEMO_DDM_DP_LOC_LIBS = -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeprocDDMA/lib \
							   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/dopplerprocDDMA/lib \
							   -i$(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangecfarprocDDMA/lib \

DSS_MMW_DEMO_DDM_LIBS = $(DSS_MMW_DEMO_STD_LIBS) \
						$(DSS_MMW_DEMO_DDM_DP_LIBS) \

DSS_MMW_DEMO_DDM_LOC_LIBS = $(DSS_MMW_DEMO_LOC_LIBS) \
							$(DSS_MMW_DEMO_DDM_DP_LOC_LIBS) \

###################################################################################
# Millimeter Wave Demo
###################################################################################
DSS_MMW_ENET_DEMO_MAP    = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_dss$(PROC_CHAIN)ENET.map
DSS_MMW_ENET_DEMO_OUT    = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_dss$(PROC_CHAIN)ENET.$($(DSS_CPU)_EXE_EXT)
DSS_MMW_ENET_DEMO_RPRC   = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_dss$(PROC_CHAIN)ENET.rprc
C66X_ENET_LINK_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)/c66x_linker_enet.cmd
DSS_MMW_CFG_PREFIX       = mmw_dss
DSS_MMW_DEMO_MAP         = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_dss$(PROC_CHAIN).map
DSS_MMW_DEMO_OUT         = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_dss$(PROC_CHAIN).$($(DSS_CPU)_EXE_EXT)
DSS_MMW_DEMO_RPRC        = $(MMWAVE_SDK_DEVICE_TYPE)_mmw_demo_dss$(PROC_CHAIN).rprc
DSS_MMW_DEMO_CMD         = dss/mmw_dss_linker.cmd
DSS_MMW_DEMO_TDM_SOURCES = dss_main.c \
						   data_path.c \
						   objectdetection.c
DSS_MMW_DEMO_DDM_SOURCES = dss_main.c \
						   data_path.c \
						   objectdetection.c

DSS_MMW_DEMO_SOURCES_GEN  = ti_board_config.c	\
							ti_board_open_close.c	\
							ti_dpl_config.c	\
							ti_drivers_config.c	\
							ti_pinmux_config.c	\
							ti_power_clock_config.c	\
							ti_drivers_open_close.c

DSS_MMW_TDM_DEMO_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MMW_DEMO_TDM_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
DSS_MMW_TDM_DEMO_OBJECTS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MMW_DEMO_TDM_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))
DSS_MMW_DDM_DEMO_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MMW_DEMO_DDM_SOURCES:.c=.$($(DSS_CPU)_DEP_EXT)))
DSS_MMW_DDM_DEMO_OBJECTS     = $(addprefix $(PLATFORM_OBJDIR)/, $(DSS_MMW_DEMO_DDM_SOURCES:.c=.$($(DSS_CPU)_OBJ_EXT)))
DSS_MMW_DEMO_OBJECTS_GEN = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(DSS_MMW_DEMO_SOURCES_GEN:.c=.$($(DSS_CPU)_OBJ_EXT)))

# Include OOB Demo Path
C66_COMMON_INCLUDE     += -I$(MMWAVE_SDK_INSTALL_PATH)/ti/demo/awr294x/mmw/dss/dssgenerated \

###################################################################################
# Build the Millimeter Wave Demo
###################################################################################
# dssDemo: $(DSS_CPU)_CFLAGS += -i$($(DSS_CPU)x_MATHLIB_INSTALL_PATH)/packages \
#                         --define=APP_RESOURCE_FILE='<ti/demo/awr294x/mmw/mmw_res.h>' \
#                         --define=DebugP_LOG_ENABLED

# dssDemo: buildDirectories dssbuildDirectories $(DSS_MMW_DEMO_OBJECTS) $(DSS_MMW_DEMO_OBJECTS_GEN)
# 	$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(DSS_MMW_DEMO_LOC_LIBS) $(DSS_MMW_DEMO_STD_LIBS) 	\
# 	--map_file=$(DSS_MMW_DEMO_MAP) $(DSS_MMW_DEMO_OBJECTS) $(DSS_MMW_DEMO_OBJECTS_GEN)	\
# 	$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(DSS_MMW_DEMO_CMD) -o $(DSS_MMW_DEMO_OUT)
# 	@echo "******************************************************************************"
# 	@echo 'Built the DSS for Millimeter Wave Demo'
# 	@echo "******************************************************************************"
# --define=MMWDEMO_$(PROC_CHAIN) \

dssDemo: $(DSS_CPU)_CFLAGS += -i$($(DSS_CPU)x_MATHLIB_INSTALL_PATH)/packages \
                        	--define=APP_RESOURCE_FILE='<ti/demo/awr294x/mmw/mmw_res$(PROC_CHAIN).h>' \
                        	--define=DebugP_LOG_ENABLED \
							--define=INCLUDE_DPM \
							--define=MMWDEMO_$(PROC_CHAIN)
		VPATH=$(VPATH_$(PROC_CHAIN)):$(VPATH_COM)

dssDemo: buildDirectories dssbuildDirectories $(DSS_MMW_$(PROC_CHAIN)_DEMO_OBJECTS) $(DSS_MMW_DEMO_OBJECTS_GEN)
		$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(DSS_MMW_DEMO_$(PROC_CHAIN)_LOC_LIBS) $(DSS_MMW_DEMO_$(PROC_CHAIN)_LIBS) \
		--map_file=$(DSS_MMW_DEMO_MAP) $(DSS_MMW_$(PROC_CHAIN)_DEMO_OBJECTS) $(DSS_MMW_DEMO_OBJECTS_GEN) \
		$(PLATFORM_$(DSS_CPU)X_LINK_CMD) $(DSS_MMW_DEMO_CMD) -o $(DSS_MMW_DEMO_OUT)
		@echo "******************************************************************************"
		@echo 'Built the DSS for Millimeter Wave Demo'
		@echo "******************************************************************************"

dssDemoEnet: $(DSS_CPU)_CFLAGS += -i$($(DSS_CPU)x_MATHLIB_INSTALL_PATH)/packages \
								--define=APP_RESOURCE_FILE='<ti/demo/awr294x/mmw/mmw_res$(PROC_CHAIN).h>' \
								--define=DebugP_LOG_ENABLED \
								--define=INCLUDE_DPM \
								--define=ENET_STREAM \
								--define=MMWDEMO_$(PROC_CHAIN)
		VPATH=$(VPATH_$(PROC_CHAIN)):$(VPATH_COM)

dssDemoEnet: buildDirectories dssbuildDirectories $(DSS_MMW_$(PROC_CHAIN)_DEMO_OBJECTS) $(DSS_MMW_DEMO_OBJECTS_GEN)
		$($(DSS_CPU)_LD) $($(DSS_CPU)_LDFLAGS) $(DSS_MMW_DEMO_$(PROC_CHAIN)_LOC_LIBS) $(DSS_MMW_DEMO_$(PROC_CHAIN)_LIBS) \
		--map_file=$(DSS_MMW_ENET_DEMO_MAP) $(DSS_MMW_$(PROC_CHAIN)_DEMO_OBJECTS) $(DSS_MMW_DEMO_OBJECTS_GEN) \
		$(C66X_ENET_LINK_CMD) $(DSS_MMW_DEMO_CMD) -o $(DSS_MMW_ENET_DEMO_OUT)
		@echo "******************************************************************************"
		@echo 'Built the DSS for Millimeter Wave Demo ENET'
		@echo "******************************************************************************"


###################################################################################
# Cleanup the Millimeter Wave Demo
###################################################################################
dssDemoClean:
	@echo 'Cleaning the Millimeter Wave Demo DSS Objects'
	@rm -f $(DSS_MMW_$(PROC_CHAIN)_DEMO_OBJECTS) $(DSS_MMW_DEMO_OBJECTS_GEN)
	@rm -f $(DSS_MMW_DEMO_MAP) $(DSS_MMW_DEMO_OUT) $(DSS_MMW_$(PROC_CHAIN)_DEMO_DEPENDS)
	@rm -f $(DSS_MMW_ENET_DEMO_MAP) $(DSS_MMW_ENET_DEMO_OUT)
	@$(DEL) $(PLATFORM_OBJDIR)

dssDemoObjClean:
	@echo 'Cleaning the Millimeter Wave Demo DSS Objects'
	@rm -f $(DSS_MMW_$(PROC_CHAIN)_DEMO_OBJECTS) $(DSS_MMW_DEMO_OBJECTS_GEN)
	@rm -f $(DSS_MMW_$(PROC_CHAIN)_DEMO_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DSS_MMW_TDM_DEMO_DEPENDS)
