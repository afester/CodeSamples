@echo off
REM echo Executing Command shell startup ...

SET JAVA_HOME=c:\Program Files\Java\jdk1.7.0_80
SET MVN_HOME=c:\Program Files\apache-maven-3.2.5
SET ANT_HOME=c:\Program Files\apache-ant-1.9.1
SET CYGWIN_HOME=d:\cygwin64

SET PATH=%JAVA_HOME%\bin;%MVN_HOME%\bin;%ANT_HOME%\bin;%CYGWIN_HOME%\bin;%PATH%

echo.
echo ==========================================================================
where mvn
echo.
where java
echo.
where ant
echo.
where bash
echo ==========================================================================

SET prompt=$_[%computername%] $d$s$t$_$p$_$_$+$g$s

CD /D %HOMEDRIVE%\%HOMEPATH%
