@echo off
echo %PATH%
set SCRIPTDIR=%~dp0
python %SCRIPTDIR%\winAddFolder.py %*
pause
