#!/bin/env python2.7.10

from __future__ import print_function
import sys
import re


def applyFilter(lineContent):
    (project, errorMessage, profile, errorFile, fafmw) = lineContent.split('~')
    errorParts = [part.strip() for part in errorMessage.split(';')]
    while len(errorParts) < 3:
        errorParts.append('')
    match = re.match('Error\((\d*,\d*)\): (.*)', errorParts[0])
    result = [project] + list(match.groups()) + errorParts[1:] + [profile, errorFile, fafmw.strip()]

    return '~'.join(result) + '\n'


if len(sys.argv) != 2:
    print('Usage: filter fileName')
    sys.exit(0)

with open(sys.argv[1], 'r') as f:
    for line in f:
        line = applyFilter(line)
        print(line, end='')
