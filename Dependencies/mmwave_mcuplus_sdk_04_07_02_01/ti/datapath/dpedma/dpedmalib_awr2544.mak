###################################################################################
# dpedma Library Makefile
###################################################################################
.PHONY: dpedmaLib dpedmaLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Library Source Files:
###################################################################################
# HWA applicable only to specific platforms
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2544), )
DPEDMA_HWA_LIB_SOURCES = dpedma.c \
                         dpedmahwa.c
endif

###################################################################################
# Library objects
#     Build for ARM and DSP
###################################################################################
DPEDMA_HWA_R5F_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DPEDMA_HWA_LIB_SOURCES:.c=.$(R5F_OBJ_EXT)))

###################################################################################
# Library Dependency:
###################################################################################
DPEDMA_HWA_R5F_DRV_LIB_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DPEDMA_HWA_LIB_SOURCES:.c=.$(R5F_DEP_EXT)))

###################################################################################
# Library Names:
###################################################################################
DPEDMA_HWA_R5F_DRV_LIB  = lib/libdpedma_hwa_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)

###################################################################################
# Library Build:
#     - Build the ARM & DSP Library
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2544), )
dpedmaLib: buildDirectories $(DPEDMA_HWA_R5F_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(DPEDMA_HWA_R5F_DRV_LIB) $(DPEDMA_HWA_R5F_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the Libraries
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2544), )
dpedmaLibClean:
	@echo 'Cleaning the dpedma Library Objects'
	@$(DEL) $(DPEDMA_HWA_R5F_DRV_LIB_OBJECTS) $(DPEDMA_HWA_R5F_DRV_LIB)
	@$(DEL) $(DPEDMA_HWA_R5F_DRV_LIB_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)
endif

###################################################################################
# Dependency handling
###################################################################################
ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2544), )
-include $(DPEDMA_HWA_R5F_DRV_LIB_DEPENDS)
endif

