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
RANGEPROC_HWA_LIB_SOURCES = rangeprochwa.c

###################################################################################
# Library objects
#     Build for R5 and DSP C66
###################################################################################
RANGEPROC_HWA_R4F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(R4F_OBJ_EXT)))
RANGEPROC_HWA_C674_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(C674_OBJ_EXT)))

RANGEPROC_HWA_R5F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(R5F_OBJ_EXT)))
RANGEPROC_HWA_C66_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(C66_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################

RANGEPROC_HWA_R5F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(R5F_DEP_EXT)))
RANGEPROC_HWA_C66_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_LIB_SOURCES:.c=.$(C66_DEP_EXT)))
###################################################################################
# Library Names:
###################################################################################
RANGEPROC_HWA_R5F_DRV_LIB  = lib/librangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)
RANGEPROC_HWA_C66_DRV_LIB = lib/librangeproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)
###################################################################################
# Library Build:
#     - AWR294X, AWR2x44P: Build the R5 & DSP (C66X) Library
###################################################################################
rangeprocHWALib: buildDirectories $(RANGEPROC_HWA_R5F_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(RANGEPROC_HWA_R5F_DRV_LIB) $(RANGEPROC_HWA_R5F_DRV_LIB_OBJECTS)
	$(C66_AR) $(C66_AR_OPTS) $(RANGEPROC_HWA_C66_DRV_LIB) $(RANGEPROC_HWA_C66_DRV_LIB_OBJECTS)

rangeprocLib: rangeprocHWALib

###################################################################################
# Clean the Libraries
###################################################################################
rangeprocHWALibClean:
	@echo 'Cleaning the rangeproc HWA Library Objects'
	@$(DEL) $(RANGEPROC_HWA_R5F_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_R5F_DRV_LIB)
	@$(DEL) $(RANGEPROC_HWA_C66_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_C66_DRV_LIB)
	@$(DEL) $(RANGEPROC_HWA_R5F_DRV_DEPENDS) $(RANGEPROC_HWA_C66_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

rangeprocLibClean: rangeprocHWALibClean

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROC_HWA_R5F_DRV_DEPENDS)
-include $(RANGEPROC_HWA_C66_DRV_DEPENDS)
