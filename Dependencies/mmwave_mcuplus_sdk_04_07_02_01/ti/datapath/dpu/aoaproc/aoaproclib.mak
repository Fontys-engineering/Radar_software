###################################################################################
# aoaproc Library Makefile
###################################################################################
.PHONY: aoaprocLib aoaprocLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
# HWA applicable only to specific platforms
AOAPROC_HWA_LIB_SOURCES = aoaprochwa.c


###################################################################################
# Library objects
#     Build for R5F and C66 DSP
###################################################################################
AOAPROC_HWA_R5F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(AOAPROC_HWA_LIB_SOURCES:.c=.$(R5F_OBJ_EXT)))
AOAPROC_HWA_C66_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(AOAPROC_HWA_LIB_SOURCES:.c=.$(C66_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
AOAPROC_HWA_R5F_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(AOAPROC_HWA_LIB_SOURCES:.c=.$(R5F_DEP_EXT)))
AOAPROC_HWA_C66_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(AOAPROC_HWA_LIB_SOURCES:.c=.$(C66_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
# HWA applicable only to specific platforms
AOAPROC_HWA_R5F_DRV_LIB  = lib/libaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)
AOAPROC_HWA_C66_DRV_LIB = lib/libaoaproc_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)


###################################################################################
# Library Build:
#     - Build the R5F & DSP Library
###################################################################################

aoaprocHWALib: buildDirectories $(AOAPROC_HWA_R5F_DRV_LIB_OBJECTS) $(AOAPROC_HWA_C66_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(AOAPROC_HWA_R5F_DRV_LIB) $(AOAPROC_HWA_R5F_DRV_LIB_OBJECTS)
	$(C66_AR) $(C66_AR_OPTS) $(AOAPROC_HWA_C66_DRV_LIB) $(AOAPROC_HWA_C66_DRV_LIB_OBJECTS)

aoaprocLib: aoaprocHWALib

###################################################################################
# Clean the Libraries
###################################################################################
aoaprocHWALibClean:
	@echo 'Cleaning the aoaproc Library Objects'
	@$(DEL) $(AOAPROC_HWA_R5F_DRV_LIB_OBJECTS) $(AOAPROC_HWA_R5F_DRV_LIB)
	@$(DEL) $(AOAPROC_HWA_C66_DRV_LIB_OBJECTS) $(AOAPROC_HWA_C66_DRV_LIB)
	@$(DEL) $(AOAPROC_HWA_R5F_DRV_DEPENDS) $(AOAPROC_HWA_C66_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)


aoaprocLibClean: aoaprocHWALibClean

###################################################################################
# Dependency handling
###################################################################################
-include $(AOAPROC_HWA_R5F_DRV_DEPENDS)
-include $(AOAPROC_HWA_C66_DRV_DEPENDS)

