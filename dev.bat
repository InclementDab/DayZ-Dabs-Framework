@echo off

set cwd=%cd%

start /wait taskkill /f /im workbenchApp.exe

for /f "tokens=2,*" %%a in ('reg query "HKCU\SOFTWARE\Bohemia Interactive\Dayz Tools" /v "path" 2^>nul') do set "dayz_tools=%%b"
for /f "tokens=2,*" %%a in ('reg query "HKLM\SOFTWARE\WOW6432Node\Bohemia Interactive\Dayz exp" /v "main" 2^>nul') do set "dayz_dir=%%b"

cd /d "P:\"

"%dayz_tools%\Bin\Workbench\workbenchApp.exe" "-mod=DabsFramework\Scripts" "-profiles=Profiles" "-repository=%cwd%" "-dayz=%dayz_dir%" -newErrorsAreWarnings=1