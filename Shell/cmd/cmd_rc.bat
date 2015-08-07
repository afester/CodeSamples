@IF DEFINED MSVC_HOME GOTO Done

@echo off
echo Setting native WINxx development environment ...

SET JAVA_HOME=c:\Program Files\Java\jdk1.7.0_80
SET MVN_HOME=c:\Program Files\apache-maven-3.2.5
SET ANT_HOME=c:\Program Files\apache-ant-1.9.1
SET CYGWIN_HOME=d:\cygwin64
SET CMAKE_HOME=c:\Program Files (x86)\cmake-3.3.0-win32-x86
SET MSVC_HOME=C:\Program Files (x86)\Microsoft Visual Studio 8\vc

SET PATH=%CMAKE_HOME%\bin;%JAVA_HOME%\bin;%MVN_HOME%\bin;%ANT_HOME%\bin;%CYGWIN_HOME%\bin;%PATH%
CALL "%MSVC_HOME%\vcvarsall.bat"

echo.
echo ==========================================================================
where mvn
echo.
where java
echo.
where ant
echo.
where bash
echo.
where cmake
echo.
where cl
echo ==========================================================================

SET prompt=$_[%computername%] $d$s$t$_$p$_$_$+$g$s

CD /D %HOMEDRIVE%\%HOMEPATH%

:Done
