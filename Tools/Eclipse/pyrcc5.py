#!/bin/python

import sys, os, fnmatch

if len(sys.argv) < 2:
	print('Missing root path.')
	sys.exit(1)

rootPath = sys.argv[1]
print('Rebuilding resource files at {}'.format(rootPath))

for root, dirnames, filenames in os.walk(rootPath):
	for filename in fnmatch.filter(filenames, '*.qrc'):
		baseName = filename[:-4]

		qrcFileName = baseName + '.qrc'
		pyFileName = baseName + '.py'
		relPath = root[len(rootPath)+1:]
		print('... {} => {}'.format(os.path.join(relPath, qrcFileName), os.path.join(relPath, pyFileName)))

		qrcPath = os.path.join(root, qrcFileName)
		pyPath = os.path.join(root, pyFileName)
		command = 'pyrcc5 -o {} {}'.format(pyPath, qrcPath)
		os.system(command)
