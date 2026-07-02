###################################################################################
# mmWave makefile
###################################################################################
include ./test/$(PLATFORM_DEVICE_TYPE)/full/mssTest.mak
include ./test/$(PLATFORM_DEVICE_TYPE)/full/dssTest.mak
include ./link_test/$(MMWAVE_SDK_DEVICE_TYPE)/mssTest.mak
include ./link_test/$(MMWAVE_SDK_DEVICE_TYPE)/dssTest.mak

###################################################################################
# Standard Targets which need to be implemented by each mmWave SDK module. This
# plugs into the release scripts.
###################################################################################
.PHONY: lib libClean syscfg msssyscfg-gui syscfgclean dsssyscfg-gui testClean test

###################################################################################
# Setup the VPATH:
###################################################################################
vpath %.c src

###################################################################################
# Driver Source Files:
###################################################################################
MMWAVE_SOURCES = mmwave.c			\
				 mmwave_link_common.c \
				 mmwave_osal.c		\
				 mmwave_fullcfg.c	\
				 mmwave_listlib.c	\
				 mmwave_link_mailbox.c

# R5F Driver Objects:
MMWAVE_R5F_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(R5F_OBJ_EXT)))
MMWAVE_R5F_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(R5F_DEP_EXT)))
MMWAVE_R5F_DRV_LIB     = lib/libmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)

# C66 Driver Objects:
MMWAVE_C66_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(C66_OBJ_EXT)))
MMWAVE_C66_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(C66_DEP_EXT)))
MMWAVE_C66_DRV_LIB     = lib/libmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(C66_LIB_EXT)
LIB_OBJS               = $(MMWAVE_R5F_LIB_OBJECTS) $(MMWAVE_C66_LIB_OBJECTS)

###################################################################################
# mmWave Library for AWR2x44P:
###################################################################################
lib: buildDirectories $(LIB_OBJS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(MMWAVE_R5F_DRV_LIB) $(MMWAVE_R5F_LIB_OBJECTS)
	$(C66_AR) $(C66_AR_OPTS) $(MMWAVE_C66_DRV_LIB) $(MMWAVE_C66_LIB_OBJECTS)

###################################################################################
# Clean the mmWave Library AWR2x44P
###################################################################################
libClean:
	@echo 'Cleaning the mmWave Library Objects'
	@$(DEL) $(MMWAVE_R5F_LIB_OBJECTS) $(MMWAVE_R5F_DRV_LIB)
	@$(DEL) $(MMWAVE_C66_LIB_OBJECTS) $(MMWAVE_C66_DRV_LIB)
	@$(DEL) $(MMWAVE_R5F_DEPENDS) $(MMWAVE_C66_DEPENDS)

###################################################################################
# Test Targets:
# Build the MSS and DSS Unit Test
###################################################################################
testClean: 	syscfgclean mssFullTestClean dssFullTestClean mssTestClean dssTestClean
test: 		mmwaveFullTest mmwaveLinkTest

mmwaveFullTest:
	$(MAKE) syscfgclean mssFullTestObjClean mmwsyscfg mssFullTest dssFullTest

mmwaveLinkTest:
	$(MAKE) syscfgclean mssTestObjClean dssTestObjClean mmwLinksyscfg mssTest dssTest

# syscfg: This generates syscfg files
mmwsyscfg:
	@echo Generating R5F SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/$(PLATFORM_DEVICE_TYPE)/full/mssgenerated/ test/$(PLATFORM_DEVICE_TYPE)/full/mss.syscfg
	@echo Generating C66 SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/$(PLATFORM_DEVICE_TYPE)/full/dssgenerated/ test/$(PLATFORM_DEVICE_TYPE)/full/dss.syscfg

mmwLinksyscfg:
	@echo Generating R5F SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output link_test/$(MMWAVE_SDK_DEVICE_TYPE)/mssgenerated/ link_test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.syscfg
	@echo Generating C66 SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output link_test/$(MMWAVE_SDK_DEVICE_TYPE)/dssgenerated/ link_test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.syscfg

msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/$(PLATFORM_DEVICE_TYPE)/full/mssgenerated/ test/$(PLATFORM_DEVICE_TYPE)/full/mss.syscfg

dsssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output test/$(PLATFORM_DEVICE_TYPE)/full/dssgenerated/ test/$(PLATFORM_DEVICE_TYPE)/full/dss.syscfg

mssLinksyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output link_test/$(MMWAVE_SDK_DEVICE_TYPE)/mssgenerated/ link_test/$(MMWAVE_SDK_DEVICE_TYPE)/mss.syscfg

dssLinksyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context c66ss0 --part Default --package $(PACKAGE_TYPE) --output link_test/$(MMWAVE_SDK_DEVICE_TYPE)/dssgenerated/ link_test/$(MMWAVE_SDK_DEVICE_TYPE)/dss.syscfg

# syscfg: This cleans syscfg files
syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) test/$(PLATFORM_DEVICE_TYPE)/full/mssgenerated
	@$(DEL) test/$(PLATFORM_DEVICE_TYPE)/full/dssgenerated
	@$(DEL) link_test/$(MMWAVE_SDK_DEVICE_TYPE)/mssgenerated
	@$(DEL) link_test/$(MMWAVE_SDK_DEVICE_TYPE)/dssgenerated

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_R5F_DEPENDS)
-include $(MMWAVE_C66_DEPENDS)
