@REM ###############################################################################
@REM #
@REM # Set up tools and build environment variables for mmwave sdk
@REM #
@REM ###############################################################################

@REM ###############################################################################
@REM # Build variables (to be modified based on build need)
@REM ###############################################################################

@REM Select your device. Options (case sensitive) are:
@REM    awr2943, awr2944, awr2544, awr2x44P
set MMWAVE_SDK_DEVICE=awr2x44P

@REM If download via CCS is needed, set below define to yes else no
@REM   yes: Out file created can be loaded using CCS.
@REM        Binary file created can be used to flash
@REM   no:  Out file created cannot be loaded using CCS.
@REM        Binary file created can be used to flash
@REM        (additional features: write-protect of TCMA, etc)
set DOWNLOAD_FROM_CCS=yes

if %MMWAVE_SDK_DEVICE% == awr2x44P (
@REM If memory optimized driver and kernel libraries are to be used for M4 binaries,
@REM   set below flag to 1, else 0.
set M4_RELEASE_OPT=1

@REM For awr2x44P based DDM OOB Demo, if AoA processing is required to be done on MSS,
@REM   set below flag corresponding to awr2x44P and awr2x44ECO respectively to 1, 
@REM   while 0 for doing it on DSP. 
set MSS_AOA_ENABLED=1
set ECO_MSS_AOA_ENABLED=1
)

@REM ###############################################################################
@REM # Customer installed tools variables (to be modified based on the installation)
@REM ###############################################################################


@REM ###############################################################################
@REM # TI tools variables (no changes needed below this line if default mmwave sdk
@REM # installation options are used)
@REM ###############################################################################

set MMWAVE_SDK_TOOLS_INSTALL_PATH=C:/ti/backups

@REM Common settings for all platforms
@REM Path to <mmwave_sdk installation path> folder
set MMWAVE_SDK_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/mmwave_mcuplus_sdk_04_07_02_01

@REM CCS
set CCS_INSTALL_PATH=C:/ti/ccs2010

@REM TI ARM compiler
set R5F_CLANG_INSTALL_PATH=%CCS_INSTALL_PATH%/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS

@REM CCS BIN TOOLS
set CCS_BIN_PATH=%CCS_INSTALL_PATH%/ccs/utils/bin

@REM CCS CYGWIN TOOLS
set CCS_CYGWIN_PATH=%CCS_INSTALL_PATH%/ccs/utils/cygwin

@REM AWR294X MCU PLUS SDK
SET MCU_PLUS_AWR294X_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/mcu_plus_sdk_awr294x_10_02_00_04

@REM AWR2X44P MCU PLUS SDK
SET MCU_PLUS_AWR2X44P_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/mcu_plus_sdk_awr2x44p_10_02_00_04

@REM AWR2544 MCU PLUS SDK
SET MCU_PLUS_AWR2544_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/mcu_plus_sdk_awr2544_10_02_00_04

@REM SYSCONFIG
SET SYSCONFIG_INSTALL_PATH=C:/ti/sysconfig_1.23.0

@REM AWR294X MMWAVE DFP
set MMWAVE_AWR294X_DFP_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/mmwave_dfp_02_04_18_01

@REM awr294x radarss firmware
set AWR294X_RADARSS_IMAGE_BIN=%MMWAVE_AWR294X_DFP_INSTALL_PATH%/firmware/radarss/xwr29xx_radarss_metarprc.bin

@REM awr2544 radarss firmware
set AWR2544_RADARSS_IMAGE_BIN=%MMWAVE_AWR294X_DFP_INSTALL_PATH%/firmware/radarss/xwr25xx_radarss_metarprc.bin

@REM awr2x44P radarss firmware
set AWR2X44P_RADARSS_IMAGE_BIN=%MMWAVE_AWR294X_DFP_INSTALL_PATH%/firmware/radarss/xwr2x4xp_radarss_metarprc.bin

@REM Following only needed for devices with DSP
@REM TI DSP compiler
set C66X_CODEGEN_INSTALL_PATH=%CCS_INSTALL_PATH%/ccs/tools/compiler/ti-cgt-c6000_8.3.13
@REM DSPlib
set C66x_DSPLIB_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/dsplib_c66x_3_4_0_0
@REM MATHlib
set C66x_MATHLIB_INSTALL_PATH=%MMWAVE_SDK_TOOLS_INSTALL_PATH%/mathlib_c66x_3_1_2_1


@REM Call script to check env variables and set paths
call checkenv.bat

@REM Check error code
IF %ERRORLEVEL% NEQ 0 (
  REM ERROR: Failure in checkenv.bat !!!
  REM Return error
  EXIT /B 1
)

EXIT /B 0
