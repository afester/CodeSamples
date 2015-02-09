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
	rm debian/files \
	   debian/python-sample.debhelper.log debian/python-sample.postinst.debhelper debian/python-sample.prerm.debhelper debian/python-sample.substvars \
	   debian/python3-sample.debhelper.log debian/python3-sample.postinst.debhelper debian/python3-sample.prerm.debhelper debian/python3-sample.substvars
        rm -rf debian/python-sample/
        rm -rf debian/python3-sample/
	;;

    debianize)
	# Use stdeb to create an initial debian package configuration
	# Usually only done once to create the debian directory
	echo "Debianizing package ..."
	# python setup.py --command-packages=stdeb.command bdist_deb
	py2dsc dist/Sample-0.1.tar.gz
	;;

    debsrc)
	echo "Creating debian source package"
	# debuild -S requires an upstream tarball in the parent directory
	# python setup.py sdist
	# mv dist/Sample-0.1.tar.gz ../sample_0.1.orig.tar.gz
        # rm -rf dist
        cd .. ; tar cvzf sample_0.1.orig.tar.gz python-pkg ; cd python-pkg
        debuild -S
	;;

    debbin)
	echo "Creating debian binary package"
	echo "--------------------------------------------------------------------------"
	rm ../python-sample_0.1-1_all.deb ../python3-sample_0.1-1_all.deb
	# dpkg-buildpackage requires an upstream tarball in the parent directory
        cd .. ; tar cvzf sample_0.1.orig.tar.gz python-pkg ; cd python-pkg
	dpkg-buildpackage -rfakeroot -uc -us
	echo "Python2: --------------------------------------------------------------------------"
	dpkg -c ../python-sample_0.1-1_all.deb
	echo "Python3: --------------------------------------------------------------------------"
	dpkg -c ../python3-sample_0.1-1_all.deb
	;;
esac

