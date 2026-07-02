###################################################################################
# CCS Debug makefile for MSS
###################################################################################
.PHONY: mssCCSDebug mssCCSDebugClean 

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

MSS_CPU := R5F
MSS_CPU_INSTANCE := r5f

###################################################################################
# The CCS Debug Utility application does not require any additional libraries
###################################################################################
CCS_DEBUG_MSS_STD_LIBS = $($(MSS_CPU)_COMMON_STD_LIB)
CCS_DEBUG_MSS_LOC_LIBS = $($(MSS_CPU)_COMMON_LOC_LIB)

###################################################################################
# CCS Debug Utility:
###################################################################################
CCS_DEBUG_MSS_SOURCES       = main.c

CCS_DEBUG_MSS_SOURCES_GEN = ti_board_config.c	\
							ti_board_open_close.c	\
							ti_dpl_config.c	\
							ti_drivers_config.c	\
							ti_pinmux_config.c	\
							ti_power_clock_config.c	\
							ti_drivers_open_close.c

CCS_DEBUG_MSS_OUT           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_mss.$($(MSS_CPU)_EXE_EXT)
CCS_DEBUG_MSS_RPRC          = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_mss.rprc
CCS_DEBUG_MSS_MAP           = $(MMWAVE_SDK_DEVICE_TYPE)_ccsdebug_mss.map
CCS_DEBUG_MSS_LINKER_CMD    = mssccsdebug_linker.cmd
CCS_DEBUG_MSS_DEPENDS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_MSS_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
CCS_DEBUG_MSS_OBJECTS       = $(addprefix $(PLATFORM_OBJDIR)/, $(CCS_DEBUG_MSS_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))

CCS_DEBUG_MSS_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/mssgenerated/, $(CCS_DEBUG_MSS_SOURCES_GEN:.c=.$($(MSS_CPU)_OBJ_EXT)))

###################################################################################
# Build the CCS Debug Utility:
###################################################################################
mssCCSDebug: buildDirectories mssbuildDirectories $(CCS_DEBUG_MSS_OBJECTS) $(CCS_DEBUG_MSS_OBJECTS_GEN)
	$($(MSS_CPU)_LD) $($(MSS_CPU)_LDFLAGS) $(CCS_DEBUG_MSS_LOC_LIBS) -Wl,-m=$(CCS_DEBUG_MSS_MAP) \
	-o $(CCS_DEBUG_MSS_OUT) $(CCS_DEBUG_MSS_OBJECTS) $(CCS_DEBUG_MSS_OBJECTS_GEN) $(CCS_DEBUG_MSS_STD_LIBS) \
	$(PLATFORM_$(MSS_CPU)_LINK_CMD) $(CCS_DEBUG_MSS_LINKER_CMD)
	@echo 'Built the CCS Debug MSS Utility'

###################################################################################
# Cleanup CCS Debug Utility:
###################################################################################
mssCCSDebugClean:
	@echo 'Cleaning the CCS Debug Utility MSS objects'
	@$(DEL) $(CCS_DEBUG_MSS_OBJECTS) $(CCS_DEBUG_MSS_OBJECTS_GEN) $(CCS_DEBUG_MSS_OUT)
	@$(DEL) $(CCS_DEBUG_MSS_MAP)
	@$(DEL) $(CCS_DEBUG_MSS_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)


###################################################################################
# Dependency handling
###################################################################################
-include $(CCS_DEBUG_MSS_DEPENDS)

