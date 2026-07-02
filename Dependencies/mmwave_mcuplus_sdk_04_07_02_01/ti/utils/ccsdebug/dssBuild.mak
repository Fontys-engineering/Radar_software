###################################################################################
# CCS Debug makefile for DSS
###################################################################################
.PHONY: dssCCSDebug dssCCSDebugClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# CCS Debug Utility:
###################################################################################
CCS_DEBUG_DSS_SOURCES       = main.c

CCS_DEBUG_DSS_SOURCES_GEN  = ti_board_config.c	\
							ti_board_open_close.c	\
							ti_dpl_config.c	\
							ti_drivers_config.c	\
							ti_pinmux_config.c	\
							ti_power_clock_config.c	\
							ti_drivers_open_close.c

CCS_DEBUG_DSS_OUT           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_dss.$(C66_EXE_EXT)
CCS_DEBUG_DSS_RPRC          = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_dss.rprc
CCS_DEBUG_DSS_MAP           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_dss.map
CCS_DEBUG_DSS_LINKER_CMD    = dss_ccsdebug_linker.cmd
CCS_DEBUG_DSS_DEPENDS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_DSS_SOURCES:.c=.$(C66_DEP_EXT)))
CCS_DEBUG_DSS_OBJECTS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_DSS_SOURCES:.c=.$(C66_OBJ_EXT)))

CCS_DEBUG_DSS_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/dssgenerated/, $(CCS_DEBUG_DSS_SOURCES_GEN:.c=.$(C66_OBJ_EXT)))

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
dssCCSDebug: buildDirectories dssbuildDirectories $(CCS_DEBUG_DSS_OBJECTS) $(CCS_DEBUG_DSS_OBJECTS_GEN)
	$(C66_LD) $(C66_LDFLAGS) $(C66_COMMON_LOC_LIB) $(C66_COMMON_STD_LIB)		\
	--map_file=$(CCS_DEBUG_DSS_MAP) $(CCS_DEBUG_DSS_OBJECTS) $(CCS_DEBUG_DSS_OBJECTS_GEN)	\
	$(PLATFORM_C66X_LINK_CMD) $(CCS_DEBUG_DSS_LINKER_CMD) -o $(CCS_DEBUG_DSS_OUT)
	@echo 'Built the CCS Debug DSS Utility'

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
dssCCSDebugClean:
	@echo 'Cleaning the CCS Debug Utility DSS objects'
	@$(DEL) $(CCS_DEBUG_DSS_OBJECTS) $(CCS_DEBUG_DSS_OBJECTS_GEN) $(CCS_DEBUG_DSS_OUT)
	@$(DEL) $(CCS_DEBUG_DSS_MAP)
	@$(DEL) $(CCS_DEBUG_DSS_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)


###################################################################################
# Dependency handling
###################################################################################
-include $(CCS_DEBUG_DSS_DEPENDS)

