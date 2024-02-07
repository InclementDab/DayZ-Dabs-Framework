@echo off
setlocal enabledelayedexpansion

taskkill /f /im "workbenchApp.exe"

for /f "tokens=2,*" %%a in ('reg query "HKCU\SOFTWARE\Bohemia Interactive\Dayz Tools" /v "path" 2^>nul') do set "dayz_tools=%%b"

if defined dayz_tools (
    set "workbench_dir=%dayz_tools%\Bin\Workbench"
) else (
    echo DayZ Tools not found in registry...
    pause
    exit -1
)

!workbench_dir!\workbenchApp.exe "-mod=P:\DabsFramework\Scripts" "-profiles=%cd%\Profiles\Workbench"
pause

:: this folder gets made after splash is done
:: rmdir /s /q %cd%\\ToolAddons