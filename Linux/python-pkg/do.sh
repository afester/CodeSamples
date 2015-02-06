#!/bin/bash

# This shell script shows how to do 
# various things like creating a source package
# or creating a debian package

case $1 in

    srcpkg)
	# Use the python distutils to create a simple source package
	echo "Creating source package"
	echo "--------------------------------------------------------------------------"
	python setup.py sdist
	echo "--------------------------------------------------------------------------"
	tar tvzf dist/Sample-0.1.tar.gz
	;;

    binpkg)
	# Use the python distutils to create a simple binary package
	echo "Creating binary package"
	echo "--------------------------------------------------------------------------"
	python setup.py bdist
	echo "--------------------------------------------------------------------------"
	tar tvzf dist/Sample-0.1.linux-x86_64.tar.gz
	;;

    binpkg3)
	# Use the python distutils to create a simple binary package
	echo "Creating binary package for python 3"
	echo "--------------------------------------------------------------------------"
	python3 setup.py bdist
	echo "--------------------------------------------------------------------------"
	tar tvzf dist/Sample-0.1.linux-x86_64.tar.gz
	;;

    clean)
	echo "Cleaning up workspace"
	rm -rf dist
	rm -rf build
	rm *.pyc
	;;

    debpkg)
	echo "Creating debian package"
	;;
esac

