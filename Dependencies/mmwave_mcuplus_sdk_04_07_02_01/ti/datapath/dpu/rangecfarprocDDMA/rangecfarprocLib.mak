###################################################################################
# cfarproc Library Makefile
###################################################################################
.PHONY: rangecfarprocLib rangecfarprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
RANGECFARPROC_HWA_DDMA_LIB_SOURCES = rangecfarprochwa.c

###################################################################################
# Library objects
#     Build for M4 and DSP
###################################################################################
RANGECFARPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGECFARPROC_HWA_DDMA_LIB_SOURCES:.c=.$(C66_OBJ_EXT)))
RANGECFARPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGECFARPROC_HWA_DDMA_LIB_SOURCES:.c=.$(M4_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
RANGECFARPROC_HWA_DDMA_C66_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGECFARPROC_HWA_DDMA_LIB_SOURCES:.c=.$(C66_DEP_EXT)))
RANGECFARPROC_HWA_DDMA_M4_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGECFARPROC_HWA_DDMA_LIB_SOURCES:.c=.$(M4_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
RANGECFARPROC_HWA_DDMA_C66_DRV_LIB = lib/librangecfarproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)
RANGECFARPROC_HWA_DDMA_M4_DRV_LIB = lib/librangecfarproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$(M4_LIB_EXT)

###################################################################################
# Library Build:
#     - Build the M4 & DSP (C66X) Library
###################################################################################
rangecfarprocHWALib: M4_DEFINES += -DDebugP_ASSERT_ENABLED=0 -DDebugP_LOG_ENABLED=0
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
rangecfarprocHWALib: buildDirectories $(RANGECFARPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(C66_AR) $(C66_AR_OPTS) $(RANGECFARPROC_HWA_DDMA_C66_DRV_LIB) $(RANGECFARPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
rangecfarprocHWALib: buildDirectories $(RANGECFARPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS) $(RANGECFARPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(M4_AR) $(M4_AR_OPTS) $(RANGECFARPROC_HWA_DDMA_M4_DRV_LIB) $(RANGECFARPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS)
	$(C66_AR) $(C66_AR_OPTS) $(RANGECFARPROC_HWA_DDMA_C66_DRV_LIB) $(RANGECFARPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
endif

rangecfarprocLib: rangecfarprocHWALib
###################################################################################
# Clean the Libraries
###################################################################################
rangecfarprocHWALibClean:
	@echo 'Cleaning the cfarproc Library Objects'
	@$(DEL) $(RANGECFARPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS) $(RANGECFARPROC_HWA_DDMA_C66_DRV_LIB)
	@$(DEL) $(RANGECFARPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS) $(RANGECFARPROC_HWA_DDMA_M4_DRV_LIB)
	@$(DEL) $(RANGECFARPROC_HWA_DDMA_C66_DRV_DEPENDS) $(RANGECFARPROC_HWA_DDMA_M4_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

rangecfarprocLibClean: rangecfarprocHWALibClean

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGECFARPROC_HWA_DDMA_C66_DRV_DEPENDS)
-include $(RANGECFARPROC_HWA_DDMA_M4_DRV_DEPENDS)