@echo off
setlocal enabledelayedexpansion

for /f "tokens=2,*" %%a in ('reg query "HKCU\SOFTWARE\Bohemia Interactive\Dayz Tools" /v "path" 2^>nul') do set "dayz_tools=%%b"

if defined dayz_tools (
    set "workbench_dir=%dayz_tools%\Bin\Workbench"
    echo !workbench_dir!
) else (
    echo DayZ Tools not found in registry...
    pause
    exit -1
)

"%workbench_dir%\\workbenchApp.exe" "-mod=P:\Scripts;P:\DabsFramework\Scripts"

rmdir %cd%\\ToolAddons