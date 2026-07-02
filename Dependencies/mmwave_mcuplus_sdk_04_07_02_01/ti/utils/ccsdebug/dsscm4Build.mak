###################################################################################
# CCS Debug makefile for DSS_CM4
###################################################################################
.PHONY: m4CCSDebug m4CCSDebugClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

M4_CPU := M4
M4_CPU_INSTANCE := m4

###################################################################################
# The CCS Debug Utility application does not require any additional libraries
###################################################################################
CCS_DEBUG_M4_STD_LIBS = $($(M4_CPU)_COMMON_STD_LIB)
CCS_DEBUG_M4_LOC_LIBS = $($(M4_CPU)_COMMON_LOC_LIB)

###################################################################################
# CCS Debug Utility:
###################################################################################
CCS_DEBUG_M4_SOURCES       = main.c

CCS_DEBUG_M4_SOURCES_GEN = ti_board_config.c	\
							ti_board_open_close.c	\
							ti_dpl_config.c	\
							ti_drivers_config.c	\
							ti_pinmux_config.c	\
							ti_power_clock_config.c	\
							ti_drivers_open_close.c

CCS_DEBUG_M4_OUT           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_m4.$($(M4_CPU)_EXE_EXT)
CCS_DEBUG_M4_RPRC          = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_m4.rprc
CCS_DEBUG_M4_MAP           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_m4.map
CCS_DEBUG_M4_DEPENDS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_M4_SOURCES:.c=.$($(M4_CPU)_DEP_EXT)))
CCS_DEBUG_M4_OBJECTS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_M4_SOURCES:.c=.$($(M4_CPU)_OBJ_EXT)))

CCS_DEBUG_M4_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/m4generated/, $(CCS_DEBUG_M4_SOURCES_GEN:.c=.$($(M4_CPU)_OBJ_EXT)))

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
m4CCSDebug: buildDirectories m4buildDirectories $(CCS_DEBUG_M4_OBJECTS) $(CCS_DEBUG_M4_OBJECTS_GEN)
	$($(M4_CPU)_LD) $($(M4_CPU)_LDFLAGS) $(CCS_DEBUG_M4_LOC_LIBS) -Wl,-m=$(CCS_DEBUG_M4_MAP) \
	-o $(CCS_DEBUG_M4_OUT) $(CCS_DEBUG_M4_OBJECTS) $(CCS_DEBUG_M4_OBJECTS_GEN) $(CCS_DEBUG_M4_STD_LIBS) \
	$(PLATFORM_$(M4_CPU)_LINK_CMD)
	@echo 'Built the CCS Debug M4 Utility'

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
m4CCSDebugClean:
	@echo 'Cleaning the CCS Debug Utility M4 objects'
	@$(DEL) $(CCS_DEBUG_M4_OBJECTS) $(CCS_DEBUG_M4_OBJECTS_GEN) $(CCS_DEBUG_M4_OUT)
	@$(DEL) $(CCS_DEBUG_M4_MAP)
	@$(DEL) $(CCS_DEBUG_M4_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)


###################################################################################
# Dependency handling
###################################################################################
-include $(CCS_DEBUG_M4_DEPENDS)
