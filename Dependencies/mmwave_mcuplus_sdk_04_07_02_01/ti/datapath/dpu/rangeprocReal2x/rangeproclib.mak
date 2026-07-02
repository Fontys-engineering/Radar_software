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
RANGEPROC_HWA_Real2x_LIB_SOURCES = rangeprochwaReal2x.c 			

###################################################################################
# Library objects
# Build for R5
###################################################################################
RANGEPROC_HWA_Real2x_R5F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_Real2x_LIB_SOURCES:.c=.$(R5F_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
RANGEPROC_HWA_Real2x_R5F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_HWA_Real2x_LIB_SOURCES:.c=.$(R5F_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
RANGEPROC_HWA_Real2x_R5F_DRV_LIB  = lib/librangeproc_hwa_real2x_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)

###################################################################################
# Library Build:
# - AWR2544: Build the R5 Library
###################################################################################

rangeprocHWAReal2xLib: buildDirectories $(RANGEPROC_HWA_Real2x_R5F_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(RANGEPROC_HWA_Real2x_R5F_DRV_LIB) $(RANGEPROC_HWA_Real2x_R5F_DRV_LIB_OBJECTS)

rangeprocLib: rangeprocHWAReal2xLib

###################################################################################
# Clean the Libraries
###################################################################################
rangeprocHWAReal2xLibClean:
	@echo 'Cleaning the rangeproc HWA Library Objects'
	@$(DEL) $(RANGEPROC_HWA_Real2x_R5F_DRV_LIB_OBJECTS) $(RANGEPROC_HWA_Real2x_R5F_DRV_LIB)
	@$(DEL) $(RANGEPROC_HWA_Real2x_R5F_DRV_DEPENDS) 
	@$(DEL) $(PLATFORM_OBJDIR)

rangeprocLibClean: rangeprocHWAReal2xLibClean

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROC_HWA_Real2x_R5F_DRV_DEPENDS)

