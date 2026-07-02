###################################################################################
# cfarproc Library Makefile
###################################################################################
.PHONY: cfarprocLib cfarprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
CFARPROC_HWA_LIB_SOURCES = cfarprochwa.c

###################################################################################
# Library objects
#     Build for R5 and DSP
###################################################################################
CFARPROC_HWA_R5F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CFARPROC_HWA_LIB_SOURCES:.c=.$(R5F_OBJ_EXT)))
CFARPROC_HWA_C66_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CFARPROC_HWA_LIB_SOURCES:.c=.$(C66_OBJ_EXT)))
###################################################################################
# Library Dependency:
###################################################################################
CFARPROC_HWA_R5F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CFARPROC_HWA_LIB_SOURCES:.c=.$(R5F_DEP_EXT)))
CFARPROC_HWA_C66_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(CFARPROC_HWA_LIB_SOURCES:.c=.$(C66_DEP_EXT)))
###################################################################################
# Library Names:
###################################################################################
CFARPROC_HWA_R5F_DRV_LIB  = lib/libcfarproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)
CFARPROC_HWA_C66_DRV_LIB = lib/libcfarproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)
###################################################################################
# Library Build:
#     - Build the R5 & DSP (C66X) Library
###################################################################################
cfarprocHWALib: buildDirectories $(CFARPROC_HWA_R5F_DRV_LIB_OBJECTS) $(CFARPROC_HWA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(CFARPROC_HWA_R5F_DRV_LIB) $(CFARPROC_HWA_R5F_DRV_LIB_OBJECTS)
	$(C66_AR) $(C66_AR_OPTS) $(CFARPROC_HWA_C66_DRV_LIB) $(CFARPROC_HWA_C66_DRV_LIB_OBJECTS)
cfarprocLib: cfarprocHWALib
###################################################################################
# Clean the Libraries
###################################################################################
cfarprocHWALibClean:
	@echo 'Cleaning the cfarproc Library Objects'
	@$(DEL) $(CFARPROC_HWA_R5F_DRV_LIB_OBJECTS) $(CFARPROC_HWA_R5F_DRV_LIB)
	@$(DEL) $(CFARPROC_HWA_C66_DRV_LIB_OBJECTS) $(CFARPROC_HWA_C66_DRV_LIB)
	@$(DEL) $(CFARPROC_HWA_R5F_DRV_DEPENDS) $(CFARPROC_HWA_C66_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

cfarprocLibClean: cfarprocHWALibClean

###################################################################################
# Dependency handling
###################################################################################
-include $(CFARPROC_HWA_R5F_DRV_DEPENDS)
-include $(CFARPROC_HWA_C66_DRV_DEPENDS)
