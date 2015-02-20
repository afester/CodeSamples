'''
Created on 20.02.2015

@author: afester
'''

import os

# platform independant path.split function
# See http://stackoverflow.com/questions/4579908/cross-platform-splitting-of-path-in-python
def os_path_split(path, debug = False):
    parts = []
    while True:
        newpath, tail = os.path.split(path)
        if debug: print(repr(path), (newpath, tail))
        if newpath == path:
            assert not tail
            if path: parts.append(path)
            break
        parts.append(tail)
        path = newpath
    parts.reverse()
    return parts

