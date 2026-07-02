###################################################################################
# Range Proc HWA DPU M4 Unit Test
###################################################################################
.PHONY: m4HWATest m4HWATestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeprocDDMA/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangeprocDDMA/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2x44P), )
M4_CPU := M4
M4_CPU_INSTANCE := m4
endif

###################################################################################
# Libraries
###################################################################################
RANGEPROC_UNIT_M4_TEST_STD_LIBS = $($(M4_CPU)_COMMON_STD_LIB)
RANGEPROC_UNIT_M4_TEST_LOC_LIBS = $($(M4_CPU)_COMMON_LOC_LIB) \

###################################################################################
# Unit Test Files
###################################################################################
RANGEPROC_UNIT_M4_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
RANGEPROC_UNIT_M4_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_DDMA_m4.map
RANGEPROC_UNIT_M4_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangeprochwa_DDMA_m4.$($(M4_CPU)_EXE_EXT)
RANGEPROC_UNIT_M4_TEST_APP_CMD   = test/m4_linker_awr2x44P.cmd


RANGEPROC_UNIT_M4_TEST_SOURCES   = hwa_main.c  \
									rangeprochwaDDMA.c \
                                    dpedmahwa.c \
                                    dpedma.c \
                                    mathutils.c

RANGEPROC_UNIT_M4_TEST_SOURCES_GEN  = ti_board_config.c	\
										ti_board_open_close.c	\
										ti_dpl_config.c	\
										ti_drivers_config.c	\
										ti_pinmux_config.c	\
										ti_power_clock_config.c	\
										ti_drivers_open_close.c

RANGEPROC_UNIT_M4_TEST_DEPENDS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_M4_TEST_SOURCES:.c=.$($(M4_CPU)_DEP_EXT)))
RANGEPROC_UNIT_M4_TEST_OBJECTS 	 = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGEPROC_UNIT_M4_TEST_SOURCES:.c=.$($(M4_CPU)_OBJ_EXT)))

RANGEPROC_UNIT_M4_TEST_OBJECTS_GEN	 = $(addprefix $(PLATFORM_OBJDIR)/m4generated/, $(RANGEPROC_UNIT_M4_TEST_SOURCES_GEN:.c=.$($(M4_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
#m4HWATest: $(M4_CPU)_CFLAGS := $(filter-out $(OPTIMIZE_FOR_SPEED_OPTIONS),$($(M4_CPU)_CFLAGS))
m4HWATest: $(M4_CPU)_CFLAGS += -DDATAPATH_TEST
m4HWATest: buildDirectories m4buildDirectories $(RANGEPROC_UNIT_M4_TEST_OBJECTS) $(RANGEPROC_UNIT_M4_TEST_OBJECTS_GEN)
	$($(M4_CPU)_LD) $($(M4_CPU)_LDFLAGS) $(RANGEPROC_UNIT_M4_TEST_LOC_LIBS) -Wl,-m=$(RANGEPROC_UNIT_M4_TEST_MAP) \
	-o $(RANGEPROC_UNIT_M4_TEST_OUT) $(RANGEPROC_UNIT_M4_TEST_OBJECTS) $(RANGEPROC_UNIT_M4_TEST_OBJECTS_GEN) \
	$(RANGEPROC_UNIT_M4_TEST_STD_LIBS) $(PLATFORM_$(M4_CPU)_LINK_CMD) $(RANGEPROC_UNIT_M4_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the Range Proc HWA DPU M4 Unit Test '
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
m4HWATestClean:
	@echo 'Cleaning the Range Proc HWA DPU M4 Unit Test objects'
	@$(DEL) $(RANGEPROC_UNIT_M4_TEST_OBJECTS) $(RANGEPROC_UNIT_M4_TEST_OBJECTS_GEN)
	@$(DEL) $(RANGEPROC_UNIT_M4_TEST_DEPENDS) $(RANGEPROC_UNIT_M4_TEST_OUT)
	@$(DEL) $(RANGEPROC_UNIT_M4_TEST_MAP) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGEPROC_UNIT_M4_TEST_DEPENDS)
