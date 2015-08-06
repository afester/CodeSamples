#!/bin/python

import sys, os, fnmatch
import optparse

from PyQt5 import QtCore
from PyQt5.uic.driver import Driver

if sys.hexversion >= 0x03000000:
    from PyQt5.uic.port_v3.invoke import invoke
else:
    from PyQt5.uic.port_v2.invoke import invoke


def invokeUic(arguments):
	Version = "Python User Interface Compiler %s for Qt version %s" % (QtCore.PYQT_VERSION_STR, QtCore.QT_VERSION_STR)

	parser = optparse.OptionParser(usage="pyuic5 [options] <ui-file>", version=Version)
	parser.add_option("-p", "--preview", dest="preview", action="store_true", default=False,
					  help="show a preview of the UI instead of generating code")
	parser.add_option("-o", "--output", dest="output", default="-", metavar="FILE",
					  help="write generated code to FILE instead of stdout")
	parser.add_option("-x", "--execute", dest="execute", action="store_true", default=False,
					  help="generate extra code to test and display the class")
	parser.add_option("-d", "--debug", dest="debug", action="store_true", default=False, 
					  help="show debug output")
	parser.add_option("-i", "--indent", dest="indent", action="store", type="int", default=4, metavar="N",
					  help="set indent width to N spaces, tab if N is 0 [default: 4]")

	g = optparse.OptionGroup(parser, title="Code generation options")
	g.add_option("--from-imports", dest="from_imports", action="store_true",
			default=False, help="generate imports relative to '.'")
	g.add_option("--resource-suffix", dest="resource_suffix", action="store",
			type="string", default="_rc", metavar="SUFFIX",
			help="append SUFFIX to the basename of resource files [default: _rc]")
	parser.add_option_group(g)

	opts, args = parser.parse_args(arguments)

	if len(args) != 1:
		sys.stderr.write("Error: one input ui-file must be specified\n")
		sys.exit(1)

	invoke(Driver(opts, args[0]))


if len(sys.argv) < 2:
	print('Missing root path.')
	sys.exit(1)

rootPath = sys.argv[1]
print('Rebuilding ui files at {}'.format(rootPath))

for root, dirnames, filenames in os.walk(rootPath):
	for filename in fnmatch.filter(filenames, '*.ui'):
		baseName = filename[:-3]

		uiFileName = baseName + '.ui'
		pyFileName = baseName + '.py'
		relPath = root[len(rootPath)+1:]
		print('... {} => {}'.format(os.path.join(relPath, uiFileName), os.path.join(relPath, pyFileName)))

		uiPath = os.path.join(root, uiFileName)
		pyPath = os.path.join(root, pyFileName)
		params = [uiPath, '-o', pyPath]
		invokeUic(params)
