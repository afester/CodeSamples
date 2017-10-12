@IF DEFINED MSVC_HOME GOTO Done

@echo off
echo Setting native WINxx development environment ...

REM SET "MVN_HOME=c:\Program Files\apache-maven-3.2.5"

rem SET "JAVA_HOME=c:\Program Files\Java\jdk1.6.0_43"
rem SET "JAVA_HOME=c:\Program Files\Java\jdk1.7.0_80"
SET "JAVA_HOME=c:\Program Files\Java\jdk1.8.0_144"
rem SET JAVA_HOME=c:\Program Files\Java\jdk-9

REM SET ANT_HOME=c:\Program Files\apache-ant-1.9.1
SET "CYGWIN_HOME=c:\cygwin64"
REM SET CMAKE_HOME=c:\Program Files\cmake-3.8.2-win64-x64
REM SET MSVC_HOME=D:\Microsoft Visual Studio 12.0\VC

REM SET QT_HOME=d:\Qt5.5.0\5.5\msvc2013
REM SET QT_HOME=d:\cygwin64

SET "GRADLE_HOME=C:\Program Files\gradle-4.2.1"
REM SET NASM_HOME=C:\Users\afester\nasm-2.10.07
REM SET HG_HOME=D:\Program Files\TortoiseHg
rem SET GIT_HOME=C:\Program Files\Git
REM SET GROOVY_HOME=D:\groovy-2.4.5
rem SET MSVC_HOME=C:\Program Files (x86)\Microsoft Visual Studio 14.0\vc
REM SET HASKELL_HOME=D:\Program Files\Haskell Platform\8.0.1

echo.
echo ==========================================================================

IF DEFINED MVN_HOME (
  SET "PATH=%MVN_HOME%\bin;%PATH%"
  where mvn
  echo.
)

IF DEFINED JAVA_HOME (
  SET "PATH=%JAVA_HOME%\bin;%PATH%"
  where java
  echo.
)

IF DEFINED ANT_HOME (
  SET "PATH=%ANT_HOME%\bin;%PATH%"
  where ant
  echo.
)

IF DEFINED CYGWIN_HOME (
  SET "PATH=%CYGWIN_HOME%\bin;%PATH%"
  where bash
  echo.
)

IF DEFINED CMAKE_HOME (
  SET "PATH=%CMAKE_HOME%\bin;%PATH%"
  where cmake
  echo.
)

IF DEFINED MSVC_HOME (
  CALL "%MSVC_HOME%\vcvarsall.bat"
  where cl
  echo.
)

IF DEFINED QT_HOME (
  SET "PATH=%QT_HOME%\bin;%PATH%"
  where qmake
  echo.
)

IF DEFINED GRADLE_HOME (
  SET "PATH=%GRADLE_HOME%\bin;%PATH%"
  where gradle
  echo.
)

IF DEFINED NASM_HOME (
  SET "PATH=%NASM_HOME%;%PATH%"
  where nasm
  echo.
)

IF DEFINED HG_HOME (
  SET "PATH=%HG_HOME%;%PATH%"
  where hg
  echo.
)

IF DEFINED GIT_HOME (
  SET "PATH=%GIT_HOME%\bin;%PATH%"
  where git
  echo.
)

IF DEFINED GROOVY_HOME (
  SET "PATH=%GROOVY_HOME%\bin;%PATH%"
  where groovy
  echo.
)

IF DEFINED HASKELL_HOME (
  SET "PATH=%HASKELL_HOME%\bin;%PATH%"
  where ghci
)

echo ==========================================================================

SET prompt=$_[%computername%] $d$s$t$_$p$_$_$+$g$s

CD /D %HOMEDRIVE%\%HOMEPATH%

:Done
