#!/bin/bash

# Get parameters passed from the kernel
EXE=`echo $1 | sed -e "s,!,/,g"`
EXEWD=`dirname ${EXE}`
TSTAMP=$8

# Read core file from stdin
COREFILE=/tmp/core_${TSTAMP}
cat > ${COREFILE}

# echo "PATH:${EXE}" > /tmp/crash
# echo "COREFILE:${COREFILE}" >> /tmp/crash

# Launch xterm with debugger session
# xterm -display :1 -e "/bin/bash --rcfile <(echo \"cd ${EXEWD} ; gdb ${EXE} -c ${COREFILE} -ex \"info reg eax\" \")" &
xterm -display :1 -e "gdb ${EXE} -c ${COREFILE} -ex \"info reg eax\"" &
# xterm -display :1 -e "gdb ${EXE} -c ${COREFILE} -ex \"where\"" &
