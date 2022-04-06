#!/bin/env python

from __future__ import print_function

import sys


#def myDecorator():
#    print("myDecorator!")

print("(1)")

def myDecorator(func):
    print("DECO!!!")
    def deco(x):
        print(">>> myDecorator!")
        print(func)
        print(x)
        # func()
        print("<<< myDecorator!")
    return deco

print("(2)")

# func1 gets re-assigned to deco()!
@myDecorator
def func1():
    print("Hello func1")
print(func1)

print("(3)")

# func2 gets re-assigned to deco()!
@myDecorator    #("Hello")
def func2():
    print("Hello func2")
print(func2)

@myDecorator
def func3():
    print("Hello func3")
print(func3)

print("(4)")

def main(argv):
    print(sys.version_info)
    print("Launching test: {}".format(argv))
#    func2()

print(func1)
print(func2)
print(func3)

if __name__ == "__main__":
    main(sys.argv)
