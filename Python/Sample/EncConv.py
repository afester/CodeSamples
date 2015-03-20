'''
Created on 19.03.2015

@author: afester
'''

import sys
import fnmatch
import os
import glob
import tempfile
import shutil

def convert(fileName):
    print('=> {}'.format(fileName))

    destFile = tempfile.NamedTemporaryFile(mode='w+', encoding='utf-8', delete = False)

    with open(fileName, 'r') as inputFile:
        destFile.write(inputFile.read())
        destFile.close()
        shutil.move(destFile.name, fileName)


if __name__ == "__main__":

    recurse = False
    pattern = None
    if len(sys.argv) < 2:
        print("Usage: {} [-r] fileNameOrPattern".format(sys.argv[0]))
        sys.exit()
    if len(sys.argv) == 2:
        pattern = sys.argv[1]
    if len(sys.argv) > 2:
        if sys.argv[1] == '-r':
            recurse = True
        pattern = sys.argv[2]

    print("Using: {} {}\n--------------------------------------------------".format(pattern, '(recursive)' if recurse else '(current dir)'))

    if recurse:
        for root, dirnames, filenames in os.walk('.'):
          for filename in fnmatch.filter(filenames, pattern):
              convert(os.path.join(root, filename))
    else:
        for filename in glob.glob(pattern):
            convert(filename)
