@echo off
REM ============================================================================
REM  copy_awr2x44p_dependencies.bat
REM
REM  Fills Radar_software\Dependencies with the AWR2X44P (AWR2944P / AWR2E44P)
REM  parts of TI mmWave MCU+ SDK 04.07.02.01 that Radar_Program_mss/dss now
REM  build against (branch 2E44_Standalone_Test).
REM
REM  Why: the repo copy of mcu_plus_sdk_awr2x44p_10_02_00_04 is incomplete
REM  (no .metadata\product.json, empty source\drivers\lib, ...) because of the
REM  repository size limits, and the AWR2X44P RadarSS firmware is missing from
REM  mmwave_dfp_02_04_18_01. Without these, SysConfig and the link step fail.
REM
REM  Usage: close CCS (or at least the two projects), then double-click this
REM  file or run it from a cmd prompt. Safe to re-run: robocopy only copies
REM  missing/changed files and NOTHING is deleted from Dependencies.
REM ============================================================================
setlocal

REM ---- Edit this if the SDK is installed somewhere else ----------------------
set "TI_ROOT=C:\ti\mmwave_mcuplus_sdk_04_07_02_01"

set "DEP_ROOT=%~dp0"
if "%DEP_ROOT:~-1%"=="\" set "DEP_ROOT=%DEP_ROOT:~0,-1%"

set "MCU_SRC=%TI_ROOT%\mcu_plus_sdk_awr2x44p_10_02_00_04"
set "MCU_DST=%DEP_ROOT%\mcu_plus_sdk_awr2x44p_10_02_00_04"
set "MMW_SRC=%TI_ROOT%\mmwave_mcuplus_sdk_04_07_02_01"
set "MMW_DST=%DEP_ROOT%\mmwave_mcuplus_sdk_04_07_02_01"
set "DFP_SRC=%TI_ROOT%\mmwave_dfp_02_04_18_01"
set "DFP_DST=%DEP_ROOT%\mmwave_dfp_02_04_18_01"

set "RC_OPTS=/E /R:1 /W:1 /NFL /NDL /NJH /NP"
set "ERR=0"

if not exist "%MCU_SRC%\.metadata\product.json" (
    echo [ERROR] "%MCU_SRC%" not found.
    echo         Install TI mmWave MCU+ SDK 04.07.02.01 ^(AWR2X44P^) to C:\ti,
    echo         or edit TI_ROOT at the top of this script.
    set "ERR=1"
    goto :done
)

echo Copying AWR2X44P dependencies
echo   from: %TI_ROOT%
echo   to  : %DEP_ROOT%
echo.

REM 1) MCU+ SDK for AWR2X44P: SysConfig metadata, drivers/kernel/board/networking
REM    sources + prebuilt libs, and tools\boot (elf2rprc, multicoreImageGen,
REM    uart_uniflash.py). docs and examples are skipped to save space.
echo [1/4] MCU+ SDK AWR2X44P (this is the big one, it can take a few minutes)
robocopy "%MCU_SRC%" "%MCU_DST%" %RC_OPTS% /XD "%MCU_SRC%\docs" "%MCU_SRC%\examples"
call :check "MCU+ SDK AWR2X44P" %ERRORLEVEL%

REM 2) RadarSS (BSS) firmware for AWR2X44P, used by the post-build appimage step
echo [2/4] RadarSS firmware xwr2x4xp_radarss_metarprc.bin
robocopy "%DFP_SRC%\firmware\radarss" "%DFP_DST%\firmware\radarss" xwr2x4xp_radarss_metarprc.bin mmwave_radarss_xWR2x4xP_release_notes.pdf /R:1 /W:1 /NFL /NDL /NJH /NP
call :check "RadarSS firmware" %ERRORLEVEL%

REM 3) Flashing helpers for AWR2X44P (sbl_qspi / sbl_uart_uniflash images and
REM    default.cfg used with uart_uniflash.py; Uniflash GUI does not support
REM    the AWR2X44P EVM)
echo [3/4] tools\awr2x44P flashing images
robocopy "%MMW_SRC%\tools\awr2x44P" "%MMW_DST%\tools\awr2x44P" %RC_OPTS%
call :check "tools\awr2x44P" %ERRORLEVEL%

REM 4) TI reference .cfg profiles for the AWR2944P / AWR2E44P EVMs
echo [4/4] TI AWR2X44P demo profiles (.cfg)
robocopy "%MMW_SRC%\ti\demo\awr2x44P\mmw_tdm\profiles" "%MMW_DST%\ti\demo\awr2x44P\mmw_tdm\profiles" %RC_OPTS%
call :check "mmw_tdm profiles" %ERRORLEVEL%
robocopy "%MMW_SRC%\ti\demo\awr2x44P\mmw_ddm\profiles" "%MMW_DST%\ti\demo\awr2x44P\mmw_ddm\profiles" %RC_OPTS%
call :check "mmw_ddm profiles" %ERRORLEVEL%

:done
echo.
if "%ERR%"=="0" (
    echo All AWR2X44P dependencies are in place.
) else (
    echo One or more copies FAILED - see the messages above.
)
pause
endlocal & exit /b %ERR%

REM ---------------------------------------------------------------------------
REM robocopy exit codes 0-7 mean success (files copied / already identical),
REM 8 and above mean at least one file could not be copied.
REM Arguments: %1 = description, %2 = robocopy exit code
:check
if %~2 GEQ 8 (
    echo       [FAILED] %~1 ^(robocopy exit code %~2^)
    set "ERR=1"
) else (
    echo       [OK] %~1
)
exit /b 0
