###################################################################################
# rangeproc Library Makefile
###################################################################################
.PHONY: rangeprocLib rangeprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

###################################################################################
# Library Source Files:
###################################################################################
RANGEPROC_HWA_DDMA_LIB_SOURCES = rangeprochwaDDMA.c

###################################################################################
# Library objects
#     Build for M4 and DSP C66
###################################################################################
RANGEPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_DDMA_LIB_SOURCES:.c=.$(C66_OBJ_EXT)))
RANGEPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_DDMA_LIB_SOURCES:.c=.$(M4_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
RANGEPROC_HWA_DDMA_C66_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_DDMA_LIB_SOURCES:.c=.$(C66_DEP_EXT)))
RANGEPROC_HWA_DDMA_M4_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_DDMA_LIB_SOURCES:.c=.$(M4_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
RANGEPROC_HWA_DDMA_C66_DRV_LIB = lib/librangeproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)
RANGEPROC_HWA_DDMA_M4_DRV_LIB = lib/librangeproc_hwa_ddma_$(MMWAVE_SDK_DEVICE_TYPE).$(M4_LIB_EXT)

###################################################################################
# Library Build:
#     - AWR294X: Build the DSP (C66X) Library
#     - AWR2x44P: Build the M4
###################################################################################
rangeprocHWADDMALib: M4_DEFINES += -DDebugP_ASSERT_ENABLED=0 -DDebugP_LOG_ENABLED=0
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
rangeprocHWADDMALib: buildDirectories $(RANGEPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(C66_AR) $(C66_AR_OPTS) $(RANGEPROC_HWA_DDMA_C66_DRV_LIB) $(RANGEPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
rangeprocHWADDMALib: buildDirectories $(RANGEPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(M4_AR) $(M4_AR_OPTS) $(RANGEPROC_HWA_DDMA_M4_DRV_LIB) $(RANGEPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS)
	$(C66_AR) $(C66_AR_OPTS) $(RANGEPROC_HWA_DDMA_C66_DRV_LIB) $(RANGEPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS)
endif

rangeprocLib: rangeprocHWADDMALib

###################################################################################
# Clean the Libraries
###################################################################################
rangeprocHWADDMALibClean:
	@echo 'Cleaning the rangeproc HWA Library Objects'
	@$(DEL) $(RANGEPROC_HWA_DDMA_C66_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_DDMA_C66_DRV_LIB)
	@$(DEL) $(RANGEPROC_HWA_DDMA_M4_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_DDMA_M4_DRV_LIB)
	@$(DEL) $(RANGEPROC_HWA_DDMA_C66_DRV_DEPENDS) $(RANGEPROC_HWA_DDMA_M4_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

rangeprocLibClean: rangeprocHWADDMALibClean

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROC_HWA_DDMA_C66_DRV_DEPENDS)
-include $(RANGEPROC_HWA_DDMA_M4_DRV_DEPENDS)
