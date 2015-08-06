@echo off
SETLOCAL

SET VENV_NAME=mynpad
SET PYTHON_HOME=D:\Python342
SET UNZIPEXE="unzip.exe"
SET ZIPEXE="zip.exe"
SET VENV_DIR=C:\temp\%VENV_NAME%
SET PROXY=

SET PATH=%PYTHON_HOME%;%PYTHON_HOME%\Scripts;%PYTHON_HOME%\Tools\Scripts;%PATH%

REM Build the egg file - this file contains the MynPad module, but no 
REM external data files and no helper scripts or similar. It only contains
REM the .pyc files, the source .py files (can also be omitted) and resource files 
RMDIR /Q/S dist build MynPad.egg-info
pip list
python setup.py bdist_egg
%UNZIPEXE% -l dist\MynPad-0.1-py3.4.egg

REM Create the virtual environment
echo Creating virtual python environment %VENV_DIR% ...
rem MOVE %VENV_DIR% %VENV_DIR%.xyz
rem RMDIR /S/Q %VENV_DIR%.xyz
pyvenv --clear %VENV_DIR%
COPY %SystemRoot%\system32\python34.dll %VENV_DIR%\Scripts
xcopy /E/F %PYTHON_HOME%\Lib /EXCLUDE:exclude.txt %VENV_DIR%\Lib
CD %VENV_DIR%
del pyvenv.cfg
echo Activating virtual python environment %VENV_DIR% ...
call Scripts\activate.bat

REM Install the egg file into the virtual environment
easy_install dist\MynPad-0.1-py3.4.egg

REM Add additional application files
COPY MynPad.cmd %VENV_DIR%

REM Setup PyQt runtime
FOR /F %%f in (qtfiles.txt) DO (
    echo %PYTHON_HOME%\Lib\site-packages\%%f  %VENV_DIR%\Lib\site-packages\%%f
    cmd /c echo F | XCOPY %PYTHON_HOME%\Lib\site-packages\%%f %VENV_DIR%\Lib\site-packages\%%f /E
)
COPY %PYTHON_HOME%\Lib\site-packages\sip.pyd %VENV_DIR%\Lib\site-packages\
COPY qt.conf %VENV_DIR%\Scripts

REM Install and upgrade python packages
pip install cssutils --proxy %PROXY%
pip install urllib3 --proxy %PROXY%
rem pip install matplotlib --proxy %PROXY%
pip install --upgrade setuptools --proxy %PROXY%

REM pending: install numpy and matplotlib with their dependencies
REM six, dateutil and pyparsing can most likely be installed through pip
REM matplotlib requires VC10
rem cp -R six* /cygdrive/c/Program\ Files/mynpad/Lib/site-packages/
rem cp -R dateutil* /cygdrive/c/Program\ Files/mynpad/Lib/site-packages/
rem cp -R pyparsing* /cygdrive/c/Program\ Files/mynpad/Lib/site-packages/
rem cp -R numpy* /cygdrive/c/Program\ Files/mynpad/Lib/site-packages/
rem cp -R matplot* /cygdrive/c/Program\ Files/mynpad/Lib/site-packages/
rem cp -R unittest /cygdrive/c/Program\ Files/mynpad/Lib/

cd c:\Users\afester\Downloads\dropbox-python-sdk-2.2.0
python setup.py install

pip list

cd %VENV_DIR%\..
%ZIPEXE% -r "%VENV_NAME%.zip" "%VENV_NAME%"
