#!/bin/env python2

from __future__ import print_function

import sys


class ParentClass:

    print("ParentClass initialization")

    def __init__(self):
        print("ParentClass::__init__")


class SubClass(ParentClass):

    print("ChildClass initialization")
    value = 42

    def __init__(self):
        ParentClass.__init__(self)
        print("SubClass::__init__")

    def printValue(self):
        print("Value: {}".format(SubClass.value))


def main(argv):
    print("Launching test: {}".format(argv))
    sc = SubClass()
    sc.printValue()


if __name__ == "__main__":
    main(sys.argv)
