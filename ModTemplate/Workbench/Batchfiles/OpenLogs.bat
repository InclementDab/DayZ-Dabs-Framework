@echo off
TITLE Log Viewer
color 0A
SETLOCAL EnableDelayedExpansion


REM ////SET THESE FILES////
set "NPP_Directory=C:\Program Files (x86)\Notepad++\"
set LogsDir=P:\profiles\Client\
REM ///////////////////////

set "LatestFile="
set "SecondLatestFile="

for /F "delims=|" %%I in ('dir %LogsDir%\*.log /B /O:D') do ( 
	set SecondLatestFile=!LatestFile!
	set LatestFile=%%I
)

CD /D %NPP_Directory%
notepad++ %LogsDir%\%LatestFile%
notepad++ %LogsDir%\%SecondLatestFile%
