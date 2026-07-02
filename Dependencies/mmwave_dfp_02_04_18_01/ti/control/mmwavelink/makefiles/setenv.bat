@REM ###############################################################################
@REM #
@REM # Set up tools and build environment variables for mmWaveLink library generation
@REM #
@REM ###############################################################################

@REM Select your build platform(s). Options (case sensitive) are: r4f, r5f, m4, c66, c674
@REM Multiple platforms can also be selected separated by spaces.
set MMWAVE_BUILD_PLATFORM=r4f r5f m4 c66 c674

set MMWAVE_DFP_TOOLS_INSTALL_PATH=C:/ti

@REM Common settings for all platforms
@REM Path to <mmwave_dfp installation path> folder
set MMWAVE_DFP_INSTALL_PATH=../../../
@REM TI ARM clang compiler
set ARM_CODEGEN_INSTALL_PATH=%MMWAVE_DFP_TOOLS_INSTALL_PATH%/ccs1281/ccs/tools/compiler/ti-cgt-armllvm_1.3.0.LTS
@REM TI XDC TOOLS
set XDC_INSTALL_PATH=%MMWAVE_DFP_TOOLS_INSTALL_PATH%/ccs1281/xdctools_3_62_01_16_core

@REM Following only needed for devices with DSP
@REM TI DSP compiler
set DSP_CODEGEN_INSTALL_PATH=%MMWAVE_DFP_TOOLS_INSTALL_PATH%/ccs1281/ccs/tools/compiler/ti-cgt-c6000_8.3.3

EXIT /B 0

