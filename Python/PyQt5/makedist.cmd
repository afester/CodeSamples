@echo off
SETLOCAL

SET VENV_NAME=mynpad
SET PYTHON_HOME=D:\Python342
SET ZIP="unzip.exe"
SET VENV_DIR=C:\temp\%VENV_NAME%
SET PROXY=http://proxy.de:80

SET PATH=%PYTHON_HOME%;%PYTHON_HOME%\Scripts;%PYTHON_HOME%\Tools\Scripts;%PATH%

REM Build the egg file - this file contains the MynPad module, but no 
REM external data files and no helper scripts or similar. It only contains
REM the .pyc files, the source .py files (can also be omitted) and resource files 
RMDIR /Q/S dist build MynPad.egg-info
pip list
python setup.py bdist_egg
%ZIP% -l dist\MynPad-0.1-py3.4.egg

REM Create the virtual environment
echo Creating virtual python environment %VENV_DIR% ...
rem MOVE %VENV_DIR% %VENV_DIR%.xyz
rem RMDIR /S/Q %VENV_DIR%.xyz
pyvenv --clear %VENV_DIR%
echo Activating virtual python environment %VENV_DIR% ...
call %VENV_DIR%\Scripts\activate.bat

REM Install the egg file into the virtual environment
easy_install dist\MynPad-0.1-py3.4.egg

REM Add additional application files
COPY MynPad.cmd %VENV_DIR%

REM Setup PyQt runtime
FOR /F %%f in (qtfiles.txt) DO (
    echo %PYTHON_HOME%\Lib\site-packages\%%f  %VENV_DIR%\Lib\site-packages\%%f
    cmd /c echo F | XCOPY %PYTHON_HOME%\Lib\site-packages\%%f %VENV_DIR%\Lib\site-packages\%%f /E
)
REM XCOPY %PYTHON_HOME%\Lib\site-packages\PyQt5\uic %VENV_DIR%\Lib\site-packages\PyQt5\uic /I/E
COPY %PYTHON_HOME%\Lib\site-packages\sip.pyd %VENV_DIR%\Lib\site-packages\
COPY qt.conf %VENV_DIR%\Scripts

REM Install and upgrade python packages
pip install cssutils --proxy %PROXY%
pip install urllib3 --proxy %PROXY%
pip install --upgrade setuptools --proxy %PROXY%

cd c:\Users\afester\Downloads\dropbox-python-sdk-2.2.0
python setup.py install

pip list

cd %VENV_DIR%\..
zip -r %VENV_NAME%.zip %VENV_NAME%
