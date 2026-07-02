###################################################################################
# mmWave Makefile
###################################################################################
.PHONY: lib libClean syscfg msssyscfg-gui syscfgclean testClean test

include ./test/$(PLATFORM_DEVICE_TYPE)/full/mssTest.mak
include ./link_test/$(PLATFORM_DEVICE_TYPE)/mssTest.mak

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

# Driver Objects:
MMWAVE_R5F_LIB_OBJECTS = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(R5F_OBJ_EXT)))
MMWAVE_R5F_DEPENDS     = $(addprefix $(PLATFORM_OBJDIR)/, $(MMWAVE_SOURCES:.c=.$(R5F_DEP_EXT)))
MMWAVE_R5F_DRV_LIB     = lib/libmmwave_$(MMWAVE_SDK_DEVICE_TYPE).$(R5F_LIB_EXT)
LIB_OBJS               = $(MMWAVE_R5F_LIB_OBJECTS)

###################################################################################
# mmWave Library for AWR2544:
###################################################################################
lib: buildDirectories $(LIB_OBJS)
	if [ ! -d "lib" ]; then mkdir lib; fi
	echo "Archiving $@"
	$(R5F_AR) $(R5F_AR_OPTS) $(MMWAVE_R5F_DRV_LIB) $(MMWAVE_R5F_LIB_OBJECTS)

###################################################################################
# Clean the mmWave Library
###################################################################################
libClean:
	@echo 'Cleaning the mmWave Library Objects'
	@$(DEL) $(MMWAVE_R5F_LIB_OBJECTS) $(MMWAVE_R5F_DRV_LIB)
	@$(DEL) $(MMWAVE_R5F_DEPENDS)

# syscfg: This generates syscfg files
mmwsyscfg:
	@echo Generating mmwave unit test R5F SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/$(PLATFORM_DEVICE_TYPE)/full/mssgenerated/ test/$(PLATFORM_DEVICE_TYPE)/full/mss.syscfg

mmwLinksyscfg:
	@echo Generating mmwavelink unit test R5F SysConfig files ...
	$(SYSCFG_NODE) $(SYSCFG_CLI_PATH)/dist/cli.js --product $(SYSCFG_SDKPRODUCT) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output link_test/$(PLATFORM_DEVICE_TYPE)/mssgenerated/ link_test/$(PLATFORM_DEVICE_TYPE)/mss.syscfg

msssyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output test/$(PLATFORM_DEVICE_TYPE)/full/mssgenerated/ test/$(PLATFORM_DEVICE_TYPE)/full/mss.syscfg

msslinksyscfg-gui:
	$(SYSCFG_NWJS) $(SYSCFG_CLI_PATH) --product $(SYSCFG_SDKPRODUCT) --device $(SYSCONFIG_DEVICE) --context r5fss0-0 --part Default --package $(PACKAGE_TYPE) --output link_test/$(PLATFORM_DEVICE_TYPE)/mssgenerated/ link_test/$(PLATFORM_DEVICE_TYPE)/mss.syscfg

syscfgclean:
	@echo 'Cleaning the syscfg files'
	@$(DEL) test/$(PLATFORM_DEVICE_TYPE)/full/mssgenerated
	@$(DEL) link_test/$(PLATFORM_DEVICE_TYPE)/mssgenerated

###################################################################################
# mmWave, Link test for AWR2544:
###################################################################################
mmwaveFullTest:
	$(MAKE) syscfgclean mssFullTestObjClean mmwsyscfg mssFullTest

mmwaveLinkTest:
	$(MAKE) syscfgclean mssTestObjClean mmwLinksyscfg mssTest


testClean: 	syscfgclean mssFullTestClean mssTestClean
test: 		mmwaveFullTest mmwaveLinkTest

###################################################################################
# Dependency handling
###################################################################################
-include $(MMWAVE_R5F_DEPENDS)
