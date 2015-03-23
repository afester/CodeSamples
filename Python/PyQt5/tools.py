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

# http://stackoverflow.com/questions/3663450/python-remove-substring-only-at-the-end-of-string
def rchop(thestring, ending):
    if thestring.endswith(ending):
        return thestring[:-len(ending)]
    return thestring


def hexdumpString(stringValue):
    # in Python, a String is a kind of black box with some particular internal
    # representation (which might even change from one version to the other).
    # Whenever creating or writing a string, we need therefore specify an encoding
    # to use when converting the string into a byte array. 
    # See also http://www.gossamer-threads.com/lists/python/python/977040
    # See also https://www.python.org/dev/peps/pep-0393/
    # See also http://stackoverflow.com/questions/1838170/what-is-internal-representation-of-string-in-python-3-x
    contents = bytes(stringValue, encoding='utf-8')
    hexdump(contents)


def hexdump(contents):
    for hexLine in hexdumpLines(contents):
        print('{:08X}: {:48} |{:16}|'.format(hexLine[0], hexLine[1], hexLine[2]))
    print()


def hexdumpLines(contents):
    addr = 0
    hexDump = ""
    asciiDump = ""
    column = 0
    for c in contents:
        hexDump = hexDump + "{:02X} ".format(c)
        asciiDump = asciiDump + ( chr(c) if c > 31 and c < 128 else '.')
        column += 1
        if (column % 16) == 0:
            yield (addr, hexDump, asciiDump)

            asciiDump = ""
            hexDump = ""
            column = 0
            addr += 16
    if column > 0:
        yield (addr, hexDump, asciiDump)
