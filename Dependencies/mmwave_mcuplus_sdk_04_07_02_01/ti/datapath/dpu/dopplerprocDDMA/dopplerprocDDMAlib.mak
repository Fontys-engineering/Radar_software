###################################################################################
# dopplerproc Library Makefile
###################################################################################
.PHONY: dopplerprocDDMALib dopplerprocDDMALibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
# HWA applicable only to specific platforms
DOPPLERPROC_HWA_DDMA_LIB_SOURCES = dopplerprochwaDDMA.c

###################################################################################
# Library objects
#     Build for M4 and DSP
###################################################################################
DOPPLERPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(DOPPLERPROC_HWA_DDMA_LIB_SOURCES:.c=.$(C66_OBJ_EXT)))
DOPPLERPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(DOPPLERPROC_HWA_DDMA_LIB_SOURCES:.c=.$(M4_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
DOPPLERPROC_HWA_DDMA_C66_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(DOPPLERPROC_HWA_DDMA_LIB_SOURCES:.c=.$(C66_DEP_EXT)))
DOPPLERPROC_HWA_DDMA_M4_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(DOPPLERPROC_HWA_DDMA_LIB_SOURCES:.c=.$(M4_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
# HWA applicable only to specific platforms
DOPPLERPROC_HWA_DDMA_C66_DRV_LIB = lib/libdopplerproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)
DOPPLERPROC_HWA_DDMA_M4_DRV_LIB = lib/libdopplerproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$(M4_LIB_EXT)

C66_COMMON_INCLUDE += -I$(C66x_MATHLIB_INSTALL_PATH)/packages
###################################################################################
# Library Build:
#     - Build the DSP & M4 Library
###################################################################################
dopplerprocHWADDMALib: M4_DEFINES += -DDebugP_ASSERT_ENABLED=0 -DDebugP_LOG_ENABLED=0
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
dopplerprocHWADDMALib: buildDirectories $(DOPPLERPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(C66_AR) $(C66_AR_OPTS) $(DOPPLERPROC_HWA_DDMA_C66_DRV_LIB) $(DOPPLERPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
dopplerprocHWADDMALib: buildDirectories $(DOPPLERPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS) $(DOPPLERPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(M4_AR) $(M4_AR_OPTS) $(DOPPLERPROC_HWA_DDMA_M4_DRV_LIB) $(DOPPLERPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS)
	$(C66_AR) $(C66_AR_OPTS) $(DOPPLERPROC_HWA_DDMA_C66_DRV_LIB) $(DOPPLERPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
endif

dopplerprocDDMALib: dopplerprocHWADDMALib

###################################################################################
# Clean the Libraries
###################################################################################
dopplerprocHWADDMALibClean:
	@echo 'Cleaning the HWA dopplerproc Library Objects'
	@$(DEL) $(DOPPLERPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS) $(DOPPLERPROC_HWA_DDMA_C66_DRV_LIB)
	@$(DEL) $(DOPPLERPROC_HWA_DDMA_C66_DRV_DEPENDS)
	@$(DEL) $(DOPPLERPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS) $(DOPPLERPROC_HWA_DDMA_M4_DRV_LIB)
	@$(DEL) $(DOPPLERPROC_HWA_DDMA_M4_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

dopplerprocDDMALibClean: dopplerprocHWADDMALibClean

###################################################################################
# Dependency handling
###################################################################################
-include $(DOPPLERPROC_HWA_DDMA_C66_DRV_DEPENDS)
-include $(DOPPLERPROC_HWA_DDMA_M4_DRV_DEPENDS)
