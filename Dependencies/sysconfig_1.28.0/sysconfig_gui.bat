@echo off
rem
rem Run the SysConfig graphical user interface with specified parameters
rem 
rem For help with command line arguments, see sysconfig_cli.bat

set DIR=%~dp0
set NW="%DIR%nw\nw.exe"
start "" %NW% "%DIR%\app" %*
