###################################################################################
# HSI Header Library Makefile
###################################################################################
.PHONY: hsiHeaderLib hsiHeaderLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Source Files:
###################################################################################
HSI_HEADER_UTIL_SOURCES = hsiheader.c

###################################################################################
# HSI Header Library:
# AWR294x:
#   HSI Header Library is available for both the DSP and R5F
###################################################################################
HSI_HEADER_UTIL_R5F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(R5F_OBJ_EXT)))
HSI_HEADER_UTIL_C66_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(C66_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
HSI_HEADER_UTIL_R5F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(R5F_DEP_EXT)))
HSI_HEADER_UTIL_C66_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(C66_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
HSI_HEADER_UTIL_R5F_DRV_LIB  = lib/libhsiheader_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)
HSI_HEADER_UTIL_C66_DRV_LIB = lib/libhsiheader_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)

###################################################################################
# HSI Header Library:
###################################################################################
hsiHeaderLib: buildDirectories $(HSI_HEADER_UTIL_R5F_LIB_OBJECTS) $(HSI_HEADER_UTIL_C66_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
ifdef HSI_HEADER_UTIL_R5F_LIB_OBJECTS
	$(R5F_AR) $(R5F_AR_OPTS) $(HSI_HEADER_UTIL_R5F_DRV_LIB) $(HSI_HEADER_UTIL_R5F_LIB_OBJECTS)
endif
ifdef HSI_HEADER_UTIL_C66_LIB_OBJECTS
	$(C66_AR) $(C66_AR_OPTS) $(HSI_HEADER_UTIL_C66_DRV_LIB) $(HSI_HEADER_UTIL_C66_LIB_OBJECTS)
endif

###################################################################################
# Clean the HSI Header Library
###################################################################################
hsiHeaderLibClean:
	@echo 'Cleaning the HSI Header Library Objects'
	@$(DEL) $(HSI_HEADER_UTIL_R5F_LIB_OBJECTS) $(HSI_HEADER_UTIL_R5F_DRV_LIB)
	@$(DEL) $(HSI_HEADER_UTIL_R5F_DEPENDS)
	@$(DEL) $(HSI_HEADER_UTIL_C66_LIB_OBJECTS) $(HSI_HEADER_UTIL_C66_DRV_LIB)
	@$(DEL) $(HSI_HEADER_UTIL_C66_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(HSI_HEADER_UTIL_R5F_DEPENDS)
-include $(HSI_HEADER_UTIL_C66_DEPENDS)

