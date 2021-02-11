@echo off
if not defined XEDK (
	echo [91mYou must have the Xbox 360 Development Kit ^(XDK^) installed on your computer![0m
	pause
	exit /B 1
)

set xdevkit_path="%XEDK%\bin\win32\xdevkit.dll"
set xdrpc_path="%XEDK%\bin\win32\xdrpc.dll"

if not exist %xdevkit_path% (
	echo [91mxdevkit.dll not found![0m
	pause
	exit /B 1
)

if not exist %xdrpc_path% (
	echo [91mxdrpc.dll not found![0m
	pause
	exit /B 1
)

xcopy /v/y/q %xdevkit_path% . > nul
xcopy /v/y/q %xdrpc_path% . > nul

echo [92mDLL files successfully copied![0m

pause