#!/bin/bash

SAMPLEROOT=/tmp/gitsample

showGraph() {
   local DIR=$1

   echo
   echo "Commit Graph at ${DIR}:"
   echo "-------------------------------------"
#   git log --graph --decorate --oneline
#   git log --graph --full-history --all --pretty=format:"%h%x09%d%x20%s"
   pushd ${DIR} > /dev/null
   git log --graph --full-history --all --color --pretty=format:"%x1b[31m%h%x09%x1b[32m%d%x1b[0m%x20%s"
   popd > /dev/null
}

createRepository() {
   local DEST=$1
   local SAMPLE=$2

   echo "Setting up Repository and Workspace:"
   echo "-------------------------------------"
   git init --bare ${DEST}

   cd ${SAMPLE}
   git clone ${DEST} workdir
   cd ${SAMPLE}/workdir

   echo "Hello World" > file1.txt
   echo "Hello Moon" > file2.txt
   mkdir subdir
   echo "Hello Subdir" > subdir/file1.txt

   git status -uall
   git add .
   git commit -m "Initial checkin"
   git status -uall

   git push origin master
}

REPODIR=${SAMPLEROOT}/upstream
WORKDIR=${SAMPLEROOT}/workdir

rm -rf ${SAMPLEROOT}
createRepository ${REPODIR} ${SAMPLEROOT}
showGraph ${REPODIR}
showGraph ${WORKDIR}

cd ${WORKDIR}
git status -uall
echo "########################################################################################"


echo
echo "Creating develop branch:"
echo "-------------------------------------"
git checkout -b develop

echo
echo "Modifying files:"
echo "-------------------------------------"
echo "Hello World 1" > file1.txt
echo "Hello Moon 1" > file2.txt
# git diff
git status -uall
git add .
git commit -m "Mod 1"

echo "Hello World 2" > file1.txt
echo "Hello Moon 2" > file2.txt
git status -uall
git add .
git commit -m "Mod 2"

echo "Hello World 3" > file1.txt
git status -uall
git add .
git commit -m "Mod 3"

showGraph ${WORKDIR}

#echo
#echo "Merging develop to master branch:"
#echo "-------------------------------------"
#git checkout master
#git merge --no-ff develop
## git merge develop

git checkout master
echo "Hello Master" > subdir/file1.txt
git add .
git commit -m "Mod 4"

echo
echo "Pushing to origin repository"
echo "-------------------------------------"
#git push origin develop
git push origin master develop

git checkout master
git checkout -b experiment
echo "New file" > file3.txt
git add .
git commit -m "Added file3"

echo
echo "Current history"
echo "-------------------------------------"
showGraph ${REPODIR}
showGraph ${WORKDIR}


# Replay commits from the experimental branch on top of develop
# so that experimental is now based on develop
#git rebase -v develop

# git reset --hard develop

#echo
#echo "Final Result"
#echo "-------------------------------------"
#showGraph ${REPODIR}
#showGraph ${WORKDIR}

