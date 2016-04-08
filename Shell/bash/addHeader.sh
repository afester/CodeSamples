#!/bin/bash

if [ $# -ne 2 ] ; then
    echo "$0 template pattern"
    exit 1
fi

TEMPLATE=$1
PATTERN=$2

for f in `find . -name "${PATTERN}"`; do
   echo $f
   cat ${TEMPLATE} ${f} > x
   mv x ${f}
done

# TODO: Allow to remove an existing header - Starting point:
# But that is greedy!!
# perl -0777 -i.original -pe 's/\/\*.*\*\//X/igs' Example.java
