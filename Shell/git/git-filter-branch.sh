#!/bin/bash

# Get name and path of this script file
SCRIPT=`readlink -fn $0`
SCRIPTPATH=`dirname  ${SCRIPT}`

source ${SCRIPTPATH}/git-tools.sh

# Render branches of the git repository in the current directory
echo
echo "-------------------------------"
renderBranches
echo "-------------------------------"
echo

# Rewrite the history and delete the unwanted directory
git filter-branch --force --index-filter 'git rm -r --cached --ignore-unmatch lib64' --prune-empty --tag-name-filter cat -- --all
git filter-branch --force --index-filter 'git rm --cached --ignore-unmatch src/core' --prune-empty --tag-name-filter cat -- --all

# Clean up the backup - WARNING! This is irreversible!
git for-each-ref --format="%(refname)" refs/original/ | xargs -n 1 git update-ref -d

# garbage collection
git reflog expire --expire=now --all
git gc --prune=now

echo
echo "-------------------------------"
renderBranches
echo "-------------------------------"
echo
