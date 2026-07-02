@echo off
rem
rem Run the SysConfig command line tool
rem 
rem For help, use -h or --help 

set DIR=%~dp0

set NODEFLAGS=
if "%1" == "-g" (
	set NODEFLAGS=--inspect --debug-brk
)

set CCS_NODE="%DIR%..\..\tools\node\node.exe"
set STANDALONE_NODE="%DIR%nodejs\node.exe"

rem try to use standalone node, then CCS node, finally any node version available on the path
if exist %STANDALONE_NODE% (
	set NODE=%STANDALONE_NODE%
) else (
	if exist %CCS_NODE% (
		set NODE=%CCS_NODE%
	) else (
		SET NODE=node
	)
)

%NODE% %NODEFLAGS% "%DIR%\dist\cli.js" %*
