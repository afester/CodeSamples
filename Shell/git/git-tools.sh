#!/bin/bash

renderBranches() {
    #git log --graph --abbrev-commit --decorate --date=relative --format=format:'%C(bold blue)%h%C(reset) - %C(bold green)(%ar)%C(reset) %C(white)%s%C(reset) %C(dim white)- %an%C(reset)%C(bold yellow)%d%C(reset)' --all
    #git log --graph --abbrev-commit --decorate --format=format:'%C(bold blue)%h%C(reset) - %C(bold cyan)%aD%C(reset) %C(bold green)(%ar)%C(reset)%C(bold yellow)%d%C(reset)%n''          %C(white)%s%C(reset) %C(dim white)- %an%C(reset)' --all
    #git log --all --graph --decorate=short --color 
    git log --all --graph --decorate=short --color --format=format:'%C(bold blue)%h%C(reset) %C(yellow)%d%C(reset)%n        %C(white)[%cr]%C(reset)  %x09%C(cyan)%an: %s %C(reset)'
}


# Create a sample repository
createSampleRepo() {
	if [ -e ${GIT_SAMPLE_DIR} ] ; then
	    echo "${GIT_SAMPLE_DIR} already exists!"
	    echo -n "Remove existing directory ${GIT_SAMPLE_DIR} (y/n)? "
	    read ans
	    if [ "${ans}" != "y" ] ; then
	        exit
	    fi
	    echo "REMOVING ${GIT_SAMPLE_DIR}"
	    rm -rf ${GIT_SAMPLE_DIR}
	fi
	
	mkdir -p ${GIT_SAMPLE_DIR}
	cd ${GIT_SAMPLE_DIR}
	git init .
	mkdir src
	mkdir bin
	
	echo "Source file 1" > src/File1.java
	echo "Source file 2" > src/File2.java
	echo "Binary file 1" > bin/File1.bin
	echo "Binary file 2" > bin/File2.bin
	
	git add .
	git commit -m "Initial checkin"
	
	echo "Modified Source file 2" >> src/File2.java
	git add .
	git commit -m "Modified File2.java"
	
# Do some modifications on a feature branch
	git checkout -b "Feature1"
	echo "Modified Source file 1" >> src/File1.java
	git add .
	git commit -m "Modified File1.java"
	
	git tag -a Rel1 -m 'First release' 
	
	echo "Modified Source file 2" >> src/File2.java
	git add .
	git commit -m "Modified File2.java"
	
# Do some modifications on master
	git checkout "master"
	echo "Modified Source file 2" >> src/File2.java
	git add .
	git commit -m "Modified File2.java"
	
	git tag -a Rel2 -m 'Tag on master' 
	
	echo "Modified Source file 1" >> src/File1.java
	git add .
	git commit -m "Modified File1.java"
}
