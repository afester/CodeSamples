#!/bin/bash

Usage() {
	echo "Usage: $0 branch1 branch2 path/to/file"
	echo "   Merge difference between two versions of a file into the current file"
	exit 2
}

if [ $# -ne 3 ] ; then
    Usage
fi

BASE_BRANCH=$1
MOD_BRANCH=$2
FILENAME=$3
echo
echo "Merging diff between ${BASE_BRANCH} and ${MOD_BRANCH}"
echo "        into ${FILENAME}"
echo

BASE_TMPNAME="`basename ${FILENAME}`.${BASE_BRANCH}"
MOD_TMPNAME="`basename ${FILENAME}`.${MOD_BRANCH}"

git show ${BASE_BRANCH}:${FILENAME} > ${BASE_TMPNAME}
git show ${MOD_BRANCH}:${FILENAME} > ${MOD_TMPNAME}

# This MIGHT be required on a mixed Unix/Dos environment, even if core.autocrlf is set to true!
unix2dos ${BASE_TMPNAME}
unix2dos ${MOD_TMPNAME}

git merge-file ${FILENAME} ${BASE_TMPNAME} ${MOD_TMPNAME} 
CONFLICTS=$?

echo "-----------------------------------"
echo "${CONFLICTS} Conflics during merge!"

rm ${BASE_TMPNAME} ${MOD_TMPNAME}
