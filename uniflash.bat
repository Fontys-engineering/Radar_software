@echo off
setlocal

set "UNIFLASH_PATH=C:\ti\uniflash_9.6.0"
set "FLASH_BIN=.\out_of_box_1843_mss\isk\out_of_box_1843_isk.bin"
set "CCXML=.\AWR1843_Serial.ccxml"

set "COMPORT=%~1"
if "%COMPORT%"=="" set "COMPORT=COM5"

"%UNIFLASH_PATH%\dslite.bat" -c "%CCXML%" -s COMPort=%COMPORT% -s MemSelectRadio=SFLASH -f "%FLASH_BIN%,1"

endlocal