#!/bin/bash

GIT_SAMPLE_DIR=/c/Users/afester/Projects/gitSample

source git-tools.sh

# Create the sample repository
createSampleRepo

echo
echo "-------------------------------"
renderBranches
echo "-------------------------------"
echo

# Rewrite the history and delete the unwanted directory
git filter-branch --force --index-filter 'git rm -r --cached --ignore-unmatch bin' --prune-empty --tag-name-filter cat -- --all

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
