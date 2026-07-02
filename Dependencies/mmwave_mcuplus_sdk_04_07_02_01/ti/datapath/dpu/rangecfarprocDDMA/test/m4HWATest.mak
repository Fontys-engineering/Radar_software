###################################################################################
# Unit Test on Range Proc HWA DPU Makefile
###################################################################################
.PHONY: m4HWATest m4HWATestClean

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangecfarprocDDMA/test
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpu/rangecfarprocDDMA/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/datapath/dpedma/src
vpath %.c $(MMWAVE_SDK_INSTALL_PATH)/ti/utils/mathutils/src

ifneq ($(filter $(MMWAVE_SDK_DEVICE_TYPE), awr2x44P), )
M4_CPU := M4
M4_CPU_INSTANCE := m4
endif

###################################################################################
# Libraries
###################################################################################
RANGECFARPROC_UNIT_M4_TEST_STD_LIBS = $($(M4_CPU)_COMMON_STD_LIB)
RANGECFARPROC_UNIT_M4_TEST_LOC_LIBS = $($(M4_CPU)_COMMON_LOC_LIB)

###################################################################################
# Unit Test Files
###################################################################################
RANGECFARPROC_UNIT_M4_TEST_CMD       = $(MMWAVE_SDK_INSTALL_PATH)/ti/platform/$(MMWAVE_SDK_DEVICE_TYPE)
RANGECFARPROC_UNIT_M4_TEST_MAP       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangecfarprochwa_DDMA_m4.map
RANGECFARPROC_UNIT_M4_TEST_OUT       = test/$(MMWAVE_SDK_DEVICE_TYPE)_rangecfarprochwa_DDMA_m4.$($(M4_CPU)_EXE_EXT)
RANGECFARPROC_UNIT_M4_TEST_APP_CMD   = test/m4_linker_awr2x44P.cmd

RANGECFARPROC_UNIT_M4_TEST_SOURCES   = rangecfarprochwa_test_main.c \
										rangecfarprochwa.c \
										mathutils.c	\
										dpedmahwa.c \
										dpedma.c \

RANGECFARPROC_UNIT_M4_TEST_SOURCES_GEN  = ti_board_config.c	\
									ti_board_open_close.c	\
									ti_dpl_config.c	\
									ti_drivers_config.c	\
									ti_pinmux_config.c	\
									ti_power_clock_config.c	\
									ti_drivers_open_close.c

RANGECFARPROC_UNIT_M4_TEST_DEPENDS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGECFARPROC_UNIT_M4_TEST_SOURCES:.c=.$($(M4_CPU)_DEP_EXT)))
RANGECFARPROC_UNIT_M4_TEST_OBJECTS   = $(addprefix $(PLATFORM_OBJDIR)/, $(RANGECFARPROC_UNIT_M4_TEST_SOURCES:.c=.$($(M4_CPU)_OBJ_EXT)))

RANGECFARPROC_UNIT_M4_TEST_OBJECTS_GEN   = $(addprefix $(PLATFORM_OBJDIR)/m4generated/, $(RANGECFARPROC_UNIT_M4_TEST_SOURCES_GEN:.c=.$($(M4_CPU)_OBJ_EXT)))

#OPTIMIZE_FOR_SPEED_OPTIONS := -o1 -o2 -o3 -O1 -O2 -O3

###################################################################################
# Build Unit Test:
###################################################################################
m4HWATest: buildDirectories m4buildDirectories $(RANGECFARPROC_UNIT_M4_TEST_OBJECTS) $(RANGECFARPROC_UNIT_M4_TEST_OBJECTS_GEN)
	$($(M4_CPU)_LD) $($(M4_CPU)_LDFLAGS) $(RANGECFARPROC_UNIT_M4_TEST_LOC_LIBS) -Wl,-m=$(RANGECFARPROC_UNIT_M4_TEST_MAP) \
	-o $(RANGECFARPROC_UNIT_M4_TEST_OUT) $(RANGECFARPROC_UNIT_M4_TEST_OBJECTS) $(RANGECFARPROC_UNIT_M4_TEST_OBJECTS_GEN) \
	$(RANGECFARPROC_UNIT_M4_TEST_STD_LIBS) $(PLATFORM_$(M4_CPU)_LINK_CMD) $(RANGECFARPROC_UNIT_M4_TEST_APP_CMD)
	@echo "******************************************************************************"
	@echo 'Built the Range Cfar Proc HWA DPU M4 Unit Test'
	@echo "******************************************************************************"

###################################################################################
# Cleanup Unit Test:
###################################################################################
m4HWATestClean:
	@echo 'Cleaning the Range Cfar Proc HWA DPU M4 Unit Test objects'
	@$(DEL) $(RANGECFARPROC_UNIT_M4_TEST_OBJECTS) $(RANGECFARPROC_UNIT_M4_TEST_OBJECTS_GEN) $(RANGECFARPROC_UNIT_M4_TEST_OUT)
	@$(DEL) $(RANGECFARPROC_UNIT_M4_TEST_MAP) $(RANGECFARPROC_UNIT_M4_TEST_DEPENDS)
	@$(DEL) $(PLATFORM_OBJDIR)

###################################################################################
# Dependency handling
###################################################################################
-include $(RANGECFARPROC_UNIT_M4_TEST_DEPENDS)
