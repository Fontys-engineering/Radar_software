###################################################################################
# MATH utils Library Makefile
###################################################################################
.PHONY: mathUtilsLib mathUtilsLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Source Files:
###################################################################################
MATH_UTIL_SOURCES = mathutils.c

###################################################################################
# MATH utils Library:
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944 awr2544 awr2x44P),)
MATH_UTIL_R5F_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(MATH_UTIL_SOURCES:.c=.$(R5F_OBJ_EXT)))
MATH_UTIL_C66_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(MATH_UTIL_SOURCES:.c=.$(C66_OBJ_EXT)))
MATH_UTIL_M4_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(MATH_UTIL_SOURCES:.c=.$(M4_OBJ_EXT)))
endif
###################################################################################
# Library Dependency:
###################################################################################
MATH_UTIL_R5F_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MATH_UTIL_SOURCES:.c=.$(R5F_DEP_EXT)))
MATH_UTIL_C66_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MATH_UTIL_SOURCES:.c=.$(C66_DEP_EXT)))
MATH_UTIL_M4_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(MATH_UTIL_SOURCES:.c=.$(M4_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
MATH_UTIL_R5F_DRV_LIB  = lib/libmathutils.$(R5F_LIB_EXT)
MATH_UTIL_C66_DRV_LIB  = lib/libmathutils.$(C66_LIB_EXT)
MATH_UTIL_M4_DRV_LIB  = lib/libmathutils.$(M4_LIB_EXT)

###################################################################################
# MATH utils Library:
###################################################################################
mathUtilsLib: M4_DEFINES += -DDebugP_ASSERT_ENABLED=0 -DDebugP_LOG_ENABLED=0
mathUtilsLib: buildDirectories $(MATH_UTIL_R5F_LIB_OBJECTS) $(MATH_UTIL_C66_LIB_OBJECTS) $(MATH_UTIL_M4_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
ifdef MATH_UTIL_R5F_LIB_OBJECTS
	$(R5F_AR) $(R5F_AR_OPTS) $(MATH_UTIL_R5F_DRV_LIB) $(MATH_UTIL_R5F_LIB_OBJECTS)
endif
ifdef MATH_UTIL_C66_LIB_OBJECTS
	$(C66_AR) $(C66_AR_OPTS) $(MATH_UTIL_C66_DRV_LIB) $(MATH_UTIL_C66_LIB_OBJECTS)
endif
ifdef MATH_UTIL_M4_LIB_OBJECTS
	$(M4_AR) $(M4_AR_OPTS) $(MATH_UTIL_M4_DRV_LIB) $(MATH_UTIL_M4_LIB_OBJECTS)
endif

###################################################################################
# Clean the math utils Library
###################################################################################
mathUtilsLibClean:
	@echo 'Cleaning the MATH utils Library Objects'
	$(DEL) $(MATH_UTIL_R5F_LIB_OBJECTS) $(MATH_UTIL_R5F_DRV_LIB)
	$(DEL) $(MATH_UTIL_R5F_DEPENDS)
	$(DEL) $(MATH_UTIL_C66_LIB_OBJECTS) $(MATH_UTIL_C66_DRV_LIB)
	$(DEL) $(MATH_UTIL_C66_DEPENDS)
	$(DEL) $(MATH_UTIL_M4_LIB_OBJECTS) $(MATH_UTIL_M4_DRV_LIB)
	$(DEL) $(MATH_UTIL_M4_DEPENDS)

###################################################################################
# Dependency handling
###################################################################################
-include $(MATH_UTIL_R5F_DEPENDS)
-include $(MATH_UTIL_C66_DEPENDS)
-include $(MATH_UTIL_M4_DEPENDS)
