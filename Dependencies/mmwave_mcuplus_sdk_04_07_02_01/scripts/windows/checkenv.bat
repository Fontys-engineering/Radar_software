@REM ******************************************************************
@REM * FILE PURPOSE: Check environment setup
@REM ******************************************************************
@REM * FILE NAME: checkenv.bat
@REM *
@REM * DESCRIPTION:
@REM *  Checks the build environment and sets PATH for mmWave SDK build.
@REM *
@REM ******************************************************************
@echo off

@REM Supported devices
set devices_str=awr2943, awr2944, awr2544, awr2x44P

REM -------------------------------------------------------------------
REM Sanity Check: Ensure that all the required variables are defined
REM -------------------------------------------------------------------
if [%MMWAVE_SDK_INSTALL_PATH%] == [] (
@echo Error: MMWAVE_SDK_INSTALL_PATH [mmWave SDK Package Location] needs to be defined
@echo For Example:
@echo   set MMWAVE_SDK_INSTALL_PATH=C:/ti/mmwave_mcuplus_sdk_04_07_02_01
goto fail
)

if [%CCS_INSTALL_PATH%] == [] (
@echo Error: CCS_INSTALL_PATH [R5F Toochain] needs to be defined
@echo For Example:
@echo   set CCS_INSTALL_PATH=C:/ti/ccs2010
goto fail
)

if [%R5F_CLANG_INSTALL_PATH%] == [] (
@echo Error: R5F_CLANG_INSTALL_PATH [R5F Toochain] needs to be defined
@echo For Example:
@echo   set R5F_CLANG_INSTALL_PATH=C:/ti/ccs2010/ccs/tools/compiler/ti-cgt-armllvm_4.0.2.LTS
goto fail
)
if [%SYSCONFIG_INSTALL_PATH%] == [] (
@echo Error: SYSCONFIG_INSTALL_PATH needs to be defined
@echo For Example:
@echo   export SYSCONFIG_INSTALL_PATH=~/ti/sysconfig_1.23.0
goto fail
) else (
    if not exist %SYSCONFIG_INSTALL_PATH%\ (
        @echo SysConfig folder does not exist
        goto fail
    )
)
if [%MCU_PLUS_AWR294X_INSTALL_PATH%] == [] (
@echo Error: MCU_PLUS_AWR294X_INSTALL_PATH needs to be defined
@echo For Example:
@echo   export MCU_PLUS_AWR294X_INSTALL_PATH=~/ti/mcu_plus_sdk_awr294x_10_02_00_04
goto fail
)
if [%MCU_PLUS_AWR2X44P_INSTALL_PATH%] == [] (
@echo Error: MCU_PLUS_AWR2X44P_INSTALL_PATH needs to be defined
@echo For Example:
@echo   export MCU_PLUS_AWR2X44P_INSTALL_PATH=~/ti/mcu_plus_sdk_awr2x44p_10_02_00_04
goto fail
)
if [%MCU_PLUS_AWR2544_INSTALL_PATH%] == [] (
@echo Error: MCU_PLUS_AWR2544_INSTALL_PATH needs to be defined
@echo For Example:
@echo   export MCU_PLUS_AWR2544_INSTALL_PATH=~/ti/mcu_plus_sdk_awr2544_10_02_00_04
goto fail
)
if [%CCS_BIN_PATH%] == [] (
@echo Error: CCS_BIN_PATH [CCS BIN Tools] needs to be defined
@echo For Example:
@echo   set CCS_BIN_PATH=C:/ti/ccs2010/ccs/utils/bin
goto fail
)
if [%CCS_CYGWIN_PATH%] == [] (
@echo Error: CCS_CYGWIN_PATH [CCS CYGWIN TOOLS] needs to be defined
@echo For Example:
@echo   set CCS_CYGWIN_PATH=C:/ti/ccs2010/ccs/utils/cygwin
goto fail
)

if [%C66X_CODEGEN_INSTALL_PATH%] == [] (
@echo Error: C66X_CODEGEN_INSTALL_PATH [DSP Toolchain] needs to be defined
@echo For Example:
@echo   set C66X_CODEGEN_INSTALL_PATH=c:/ti/ccs2010/ccs/tools/compiler/ti-cgt-c6000_8.3.13
goto fail
)

if [%DOWNLOAD_FROM_CCS%] == [] (
@echo Error: DOWNLOAD_FROM_CCS needs to be defined to yes or no
@echo For Example:
@echo   set DOWNLOAD_FROM_CCS=yes
goto fail
)
if [%MMWAVE_SDK_DEVICE%] == [] (
@echo Error: MMWAVE_SDK_DEVICE needs to be defined. Supported devices are %devices_str%
@echo For Example:
@echo   set MMWAVE_SDK_DEVICE=awr2944
goto fail
)

IF [%MMWAVE_SDK_DEVICE%] == [awr2943] (
    set MMWAVE_SDK_DEVICE_TYPE=awr2943
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [awr2944] (
    set MMWAVE_SDK_DEVICE_TYPE=awr2944
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [awr2544] (
    set MMWAVE_SDK_DEVICE_TYPE=awr2544
) ELSE (
IF [%MMWAVE_SDK_DEVICE%] == [awr2x44P] (
    set MMWAVE_SDK_DEVICE_TYPE=awr2x44P
) ELSE (
    @echo Error: MMWAVE_SDK_DEVICE %MMWAVE_SDK_DEVICE% is not supported. Supported devices are %devices_str%
    goto fail
))))

if [%MMWAVE_SDK_DEVICE_TYPE%] == [awr2943] (
if [%MMWAVE_AWR294X_DFP_INSTALL_PATH%] == [] (
@echo Error: MMWAVE_AWR294X_DFP_INSTALL_PATH [AWR294X DFP] needs to be defined
@echo For Example:
@echo   set MMWAVE_AWR294X_DFP_INSTALL_PATH=C:/ti/mmwave_dfp_02_04_18_01
goto fail
)

if [%C66x_DSPLIB_INSTALL_PATH%] == [] (
@echo Error: C66x_DSPLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C66x_DSPLIB_INSTALL_PATH=C:/ti/dsplib_c66x_3_4_0_0
goto fail
)
if [%C66x_MATHLIB_INSTALL_PATH%] == [] (
@echo Error: C66x_MATHLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C66x_MATHLIB_INSTALL_PATH=C:/ti/mathlib_c66x_3_1_2_1
goto fail
)
if [%AWR294X_RADARSS_IMAGE_BIN%] == [] (
@echo Error: AWR294X_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary
@echo For Example:
@echo   set AWR294X_RADARSS_IMAGE_BIN=C:/ti/mmwave_dfp_02_04_18_01/firmware/radarss/xwr29xx_radarss_metarprc.bin
goto fail
))

if [%MMWAVE_SDK_DEVICE_TYPE%] == [awr2944] (
if [%MMWAVE_AWR294X_DFP_INSTALL_PATH%] == [] (
@echo Error: MMWAVE_AWR294X_DFP_INSTALL_PATH [AWR294X DFP] needs to be defined
@echo For Example:
@echo   set MMWAVE_AWR294X_DFP_INSTALL_PATH=C:/ti/mmwave_dfp_02_04_18_01
goto fail
)

if [%C66x_DSPLIB_INSTALL_PATH%] == [] (
@echo Error: C66x_DSPLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C66x_DSPLIB_INSTALL_PATH=C:/ti/dsplib_c66x_3_4_0_0
goto fail
)
if [%C66x_MATHLIB_INSTALL_PATH%] == [] (
@echo Error: C66x_MATHLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C66x_MATHLIB_INSTALL_PATH=C:/ti/mathlib_c66x_3_1_2_1
goto fail
)
if [%AWR294X_RADARSS_IMAGE_BIN%] == [] (
@echo Error: AWR294X_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary
@echo For Example:
@echo   set AWR294X_RADARSS_IMAGE_BIN=C:/ti/mmwave_dfp_02_04_18_01/firmware/radarss/xwr29xx_radarss_metarprc.bin
goto fail
))

if [%MMWAVE_SDK_DEVICE_TYPE%] == [awr2544] (
if [%MMWAVE_AWR294X_DFP_INSTALL_PATH%] == [] (
@echo Error: MMWAVE_AWR294X_DFP_INSTALL_PATH [AWR294X DFP] needs to be defined
@echo For Example:
@echo   set MMWAVE_AWR294X_DFP_INSTALL_PATH=C:/ti/mmwave_dfp_02_04_18_01
goto fail
)

if [%AWR2544_RADARSS_IMAGE_BIN%] == [] (
@echo Error: AWR2544_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary
@echo For Example:
@echo   set AWR2544_RADARSS_IMAGE_BIN=C:/ti/mmwave_dfp_02_04_18_01/firmware/radarss/xwr25xx_radarss_metarprc.bin
goto fail
))

if [%MMWAVE_SDK_DEVICE_TYPE%] == [awr2x44P] (
if [%MMWAVE_AWR294X_DFP_INSTALL_PATH%] == [] (
@echo Error: MMWAVE_AWR294X_DFP_INSTALL_PATH [AWR294X DFP] needs to be defined
@echo For Example:
@echo   set MMWAVE_AWR294X_DFP_INSTALL_PATH=C:/ti/mmwave_dfp_02_04_18_01
goto fail
)

if [%C66x_DSPLIB_INSTALL_PATH%] == [] (
@echo Error: C66x_DSPLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C66x_DSPLIB_INSTALL_PATH=C:/ti/dsplib_c66x_3_4_0_0
goto fail
)
if [%C66x_MATHLIB_INSTALL_PATH%] == [] (
@echo Error: C66x_MATHLIB_INSTALL_PATH needs to be defined
@echo For Example:
@echo   set C66x_MATHLIB_INSTALL_PATH=C:/ti/mathlib_c66x_3_1_2_1
goto fail
)
if [%AWR2X44P_RADARSS_IMAGE_BIN%] == [] (
@echo Error: AWR2X44P_RADARSS_IMAGE_BIN needs to be defined with the complete path and filename of the radarss binary
@echo For Example:
@echo   set AWR2X44P_RADARSS_IMAGE_BIN=C:/ti/mmwave_dfp_02_04_18_01/firmware/radarss/xwr2x4xp_radarss_metarprc.bin
goto fail
))



REM ----------------------------------------------------------
REM Setup the PATH configuration:
REM Please do not change anything below this
REM ----------------------------------------------------------
REM GMAKE is obtained from CCS_BIN_PATH
REM Add CCS BIN to path if it is not already in $PATH
echo "%PATH%" | find /i "%CCS_BIN_PATH%">nul || set "PATH=%CCS_BIN_PATH%;%PATH%"

REM ECHO, MKDIR etc. are obtained from CCS_CYGWIN_PATH
REM Add CCS CYGWIN to path if it is not already in $PATH
echo "%PATH%" | find /i "%CCS_CYGWIN_PATH%">nul || set "PATH=%CCS_CYGWIN_PATH%;%PATH%"

REM Following tools are optional and needed only for doxygen doc generation
REM Add to path only if these are defined and not already in PATH
if NOT ["%DOXYGEN_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%DOXYGEN_INSTALL_PATH%\bin">nul || set "PATH=%PATH%;%DOXYGEN_INSTALL_PATH%\bin"
)
if NOT ["%GRAPHVIZ_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%GRAPHVIZ_INSTALL_PATH%\bin">nul || set "PATH=%PATH%;%GRAPHVIZ_INSTALL_PATH%\bin"
)
if NOT ["%MIKTEX_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%MIKTEX_INSTALL_PATH%">nul || set "PATH=%PATH%;%MIKTEX_INSTALL_PATH%"
)
if NOT ["%GHOSTSCRIPT_INSTALL_PATH%"] == [""] (
    echo "%PATH%" | find /i "%GHOSTSCRIPT_INSTALL_PATH%\bin">nul || set "PATH=%PATH%;%GHOSTSCRIPT_INSTALL_PATH%\bin"
)

@echo -----------------------------------------------
@echo mmWave Build Environment Configured
@echo -----------------------------------------------

REM Set the Title Window appropriately.
Title mmWave Build Environment
goto end

:fail
@echo Error: Failed to setup mmWave Build Environment !
Title Error: Failed to setup mmWave Build Environment
REM Return error
EXIT /B 1

:end
REM Return success
EXIT /B 0
