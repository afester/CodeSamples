#!/bin/bash

# Prepends a set of files with a given header.

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
