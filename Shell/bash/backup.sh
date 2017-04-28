#!/bin/bash

Usage() {
   echo "Usage: $0 destDir"
   echo "Create Backup from current directory to given destination directory"
}

if [ $# -ne 1 ] ; then
   Usage
   exit 2
fi

DESTDIR=$1
LOGFILE=backup.log

echo "Backup to $1, logfile ${LOGFILE} ..."

nohup rsync --modify-window=1 --update --progress -rtv . ${DESTDIR} &> backup.log &

echo "rsync running ..."
tail -f ${LOGFILE}
