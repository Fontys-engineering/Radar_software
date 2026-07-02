###################################################################################
# DPM Control Makefile
###################################################################################
.PHONY: dpmLib dpmLibClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src
vpath %.c platform

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
DSP_CPU := C66
endif

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE),awr2943 awr2944 awr2x44P), )
MSS_CPU := R5F
endif

###################################################################################
# Module Source Files:
###################################################################################
DPM_LIB_SOURCES = dpm_core.c 		\
				  dpm_msg.c 		\
				  dpm_pipe.c 		\
				  dpm_listlib.c		\
				  dpm_mailbox.c

###################################################################################
# Device Specific Module Source Files:
###################################################################################
ifeq ($(MMWAVE_SDK_DEVICE_TYPE),$(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2943 awr2944))
DPM_LIB_SOURCES          += dpm_awr294x.c
else ifeq ($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
# ifneq($(MMWAVE_SDK_DEVICE_TYPE), awr2x44P)
DPM_LIB_SOURCES          += dpm_awr2x44p.c
endif

DPM_$(MSS_CPU)_DRV_LIB_OBJECTS  = $(addprefix $(PLATFORM_OBJDIR)/,  $(DPM_LIB_SOURCES:.c=.$($(MSS_CPU)_OBJ_EXT)))
DPM_$(DSP_CPU)_DRV_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/,  $(DPM_LIB_SOURCES:.c=.$($(DSP_CPU)_OBJ_EXT)))


###################################################################################
# Driver Dependency:
###################################################################################
DPM_$(MSS_CPU)_DRV_DEPENDS  = $(addprefix $(PLATFORM_OBJDIR)/, $(DPM_LIB_SOURCES:.c=.$($(MSS_CPU)_DEP_EXT)))
DPM_$(DSP_CPU)_DRV_DEPENDS = $(addprefix $(PLATFORM_OBJDIR)/, $(DPM_LIB_SOURCES:.c=.$($(DSP_CPU)_DEP_EXT)))

###################################################################################
# Driver Library Names:
###################################################################################
DPM_$(MSS_CPU)_DRV_LIB  = lib/libdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(MSS_CPU)_LIB_EXT)
DPM_$(DSP_CPU)_DRV_LIB = lib/libdpm_$(MMWAVE_SDK_DEVICE_TYPE).$($(DSP_CPU)_LIB_EXT)

###################################################################################
# DPM Driver Build:
#  Build the R5F & DSP Library
###################################################################################
dpmLib: buildDirectories $(DPM_$(MSS_CPU)_DRV_LIB_OBJECTS) $(DPM_$(DSP_CPU)_DRV_LIB_OBJECTS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$($(MSS_CPU)_AR) $($(MSS_CPU)_AR_OPTS) $(DPM_$(MSS_CPU)_DRV_LIB) $(DPM_$(MSS_CPU)_DRV_LIB_OBJECTS)
ifneq ($(DPM_$(DSP_CPU)_DRV_LIB_OBJECTS),"")
	$($(DSP_CPU)_AR) $($(DSP_CPU)_AR_OPTS) $(DPM_$(DSP_CPU)_DRV_LIB) $(DPM_$(DSP_CPU)_DRV_LIB_OBJECTS)
endif

###################################################################################
# Clean the DPM Driver Libraries
###################################################################################
dpmLibClean:
	@echo 'Cleaning the DPM Driver Library Objects'
	@$(DEL) $(DPM_$(MSS_CPU)_DRV_LIB_OBJECTS) $(DPM_$(MSS_CPU)_DRV_LIB)
	@$(DEL) $(DPM_$(DSP_CPU)_DRV_LIB_OBJECTS) $(DPM_$(DSP_CPU)_DRV_LIB)
	@$(DEL) $(DPM_$(MSS_CPU)_DRV_DEPENDS)
	@$(DEL) $(DPM_$(DSP_CPU)_DRV_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(DPM_$(MSS_CPU)_DRV_DEPENDS)
-include $(DPM_$(DSP_CPU)_DRV_DEPENDS)
