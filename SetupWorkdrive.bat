@echo off

cd /D "%~dp0"

IF exist "P:\ModTemplate\" (
	echo Removing existing link P:\ModTemplate
	rmdir "P:\ModTemplate\"
)

echo Creating link P:\ModTemplate
mklink /J "P:\ModTemplate\" "%cd%\ModTemplate\"

echo Done