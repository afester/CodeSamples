#!/bin/env python3

'''
Created on Jul 27, 2021

@author: andreas
'''

import sys
import fileinput

def parseDependsEntry(entry):
    result = dict()

    entryElements = entry.split(';')
    # print(' {}'.format(entryElements))
    result['package'] = entryElements[0]
    for attr in entryElements[1:]:
        if attr.startswith('uses:='):
            uses = attr[len('uses:='):].strip('"').split(',')
            uses.sort()
            result['uses'] = uses
        elif attr.startswith('version='):
            version = attr[len('version='):].strip('"')
            result['version'] = version

    return result


def parseDepends(value):
    # print('V|{}|'.format(value))
    # separate the entries
    result = []

    entry = ''
    state = 0
    for c in value:

        if state == 0:      # Outside "
            if c == '"':
                entry += c
                state = 1
            elif c == ',':
                result.append(parseDependsEntry(entry))
                entry = ''
            else:
                entry += c
        elif state == 1:    # Inside "
            entry += c
            if c =='"':
                state = 0
    if state != 0:
        print("PARSE ERROR!")
        sys.exit(1)
    result.append(parseDependsEntry(entry))

    result.sort(key = lambda k : k['package'])
    return result

def processLine(contents):
    result = None

    name, value = contents.split(':', 1)
    name = name.strip()
    value = value.strip()

    if name == 'Export-Package':
        value = parseDepends(value)
        result = (name, value)
    elif name == 'Import-Package':
        value = parseDepends(value)
        result = (name, value)
    else:
        result = (name, value)

    return result

result = []
currentLine = ''
for line in fileinput.input():
    line = line.rstrip()
    # print('|{}|'.format(line))

    # process continuation line
    if line.startswith(' '):
        currentLine = currentLine + line[1:]
    else:
        if len(currentLine) > 0 and not currentLine.startswith('Archive: '):
            result.append(processLine(currentLine))
        currentLine = line

result.sort(key = lambda k: k[0])
for name, value in result:
    if name == 'Export-Package':
        print('{}:'.format(name))
        for pkg in value:
            pkgName = pkg['package']
            version = pkg['version'] if 'version' in pkg else None
            print('  {}{}'.format(pkgName, ' version: {}'.format(version) if version is not None else ''))
            if 'uses' in pkg:
                for use in pkg['uses']:
                    print('    {}'.format(use))
    elif name == 'Import-Package':
        print('{}:'.format(name))
        for pkg in value:
            pkgName = pkg['package']
            version = pkg['version'] if 'version' in pkg else None
            print('  {}{}'.format(pkgName, ' version: {}'.format(version) if version is not None else ''))

    else:
        print('{}: {}'.format(name, value))

