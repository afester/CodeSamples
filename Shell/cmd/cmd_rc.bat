@IF DEFINED MSVC_HOME GOTO Done

@echo off
echo Setting native WINxx development environment ...

SET QT_HOME=d:\cygwin64
rem SET JAVA_HOME=c:\Program Files\Java\jdk1.6.0_43
rem SET JAVA_HOME=c:\Program Files\Java\jdk1.7.0_80
SET JAVA_HOME=c:\Program Files\Java\jdk1.8.0_101
rem SET JAVA_HOME=c:\Program Files\Java\jdk-9
rem SET JAVA_HOME=c:\Program Files (x86)\Java\jdk-9
SET MVN_HOME=c:\Program Files\apache-maven-3.2.5
SET ANT_HOME=c:\Program Files\apache-ant-1.9.1
SET CYGWIN_HOME=d:\cygwin64
SET GRADLE_HOME=d:\gradle-2.7
SET GROOVY_HOME=D:\groovy-2.4.5
SET QT_HOME=d:\Qt5.5.0\5.5\msvc2013
SET CMAKE_HOME=c:\Program Files (x86)\cmake-3.3.0-win32-x86
SET MSVC_HOME=D:\Microsoft Visual Studio 12.0\VC
SET NASM_HOME=C:\Users\afester\nasm-2.10.07
SET HG_HOME=D:\Program Files\TortoiseHg
SET GIT_HOME=D:\Program Files\Git
SET HASKELL_HOME=D:\Program Files\Haskell Platform\8.0.1

SET PATH=%GIT_HOME%\bin;%CMAKE_HOME%\bin;%QT_HOME%\bin;%JAVA_HOME%\bin;%MVN_HOME%\bin;%ANT_HOME%\bin;%CYGWIN_HOME%\bin;%GRADLE_HOME%\bin;%NASM_HOME%;%HG_HOME%;%GROOVY_HOME%\bin;%HASKELL_HOME%\bin;%PATH%
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
echo.
where qmake
echo.
where gradle
echo.
where nasm
echo.
where hg
echo.
where git
echo.
where groovy
echo.
where ghci
echo ==========================================================================

SET prompt=$_[%computername%] $d$s$t$_$p$_$_$+$g$s

CD /D %HOMEDRIVE%\%HOMEPATH%

:Done
