#!/usr/bin/env python

import sys
import os

def main(argv):
    if len(argv) != 2:
        print('''Usage: ...''')
        sys.exit(1)
    rootDir = argv[1]

    print('Using Python version %s.%s.%s (%s)' % (sys.version_info[0], sys.version_info[1], sys.version_info[2], sys.version_info[3]))
    print('Checking directory tree at %s' % rootDir)
    print('')

    dirCount = 0
    fileCount = 0
    fileSizeTotal = 0

    for root, dirs, files in os.walk(rootDir):
        print(root)
        fileNames = set()

        for dir in dirs:
            capDirName = dir.upper()
            dirCount = dirCount + 1
            if capDirName in fileNames:
                print('    %s !!! CAP !!!!!!!!!!' % dir)
            elif len(capDirName) > 255:
                print('    %s !!! LEN !!!!!!!!!!' % dir)
            else:
                # print('''     ''' + dir)
                fileNames.add(capDirName)

        for file in files:
            filePath = os.path.join(root, file)
	    statinfo = os.stat(filePath)
            fileSizeTotal = fileSizeTotal + statinfo.st_size

            capFileName = file.upper()
            fileCount = fileCount + 1
            if capFileName in fileNames:
                print('    %s !!! CAP !!!!!!!!!!' % file)
            elif len(capFileName) > 255:
                print('    %s !!! LEN !!!!!!!!!!' % file)
            else:
                # print('''     ''' + file)
                fileNames.add(capFileName)

    print('')
    fileSizeKB = fileSizeTotal / 1024
    fileSizeMB = fileSizeKB / 1024
    print('Directory tree contains %s files (%s MB) in %s directories' % (fileCount, fileSizeMB, dirCount))


if __name__ == '__main__':
    main(sys.argv)
