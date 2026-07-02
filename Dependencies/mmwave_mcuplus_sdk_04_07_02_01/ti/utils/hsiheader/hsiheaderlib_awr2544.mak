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
# AWR2544:
#   HSI Header Library is available for R5F
###################################################################################
HSI_HEADER_UTIL_R5F_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(R5F_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
HSI_HEADER_UTIL_R5F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(HSI_HEADER_UTIL_SOURCES:.c=.$(R5F_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
HSI_HEADER_UTIL_R5F_DRV_LIB  = lib/libhsiheader_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)

###################################################################################
# HSI Header Library:
###################################################################################
hsiHeaderLib: buildDirectories $(HSI_HEADER_UTIL_R5F_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
ifdef HSI_HEADER_UTIL_R5F_LIB_OBJECTS
	$(R5F_AR) $(R5F_AR_OPTS) $(HSI_HEADER_UTIL_R5F_DRV_LIB) $(HSI_HEADER_UTIL_R5F_LIB_OBJECTS)
endif

###################################################################################
# Clean the HSI Header Library
###################################################################################
hsiHeaderLibClean:
	@echo 'Cleaning the HSI Header Library Objects'
	@$(DEL) $(HSI_HEADER_UTIL_R5F_LIB_OBJECTS) $(HSI_HEADER_UTIL_R5F_DRV_LIB)
	@$(DEL) $(HSI_HEADER_UTIL_R5F_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(HSI_HEADER_UTIL_R5F_DEPENDS)

