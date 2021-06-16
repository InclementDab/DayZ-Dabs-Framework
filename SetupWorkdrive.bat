@echo off

cd /D "%~dp0"

IF exist "P:\DabsFramework\" (
	echo Removing existing link P:\DabsFramework
	rmdir "P:\DabsFramework\"
)

echo Creating link P:\DabsFramework
mklink /J "P:\DabsFramework\" "%cd%\DabsFramework\"

echo Done