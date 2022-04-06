#!/bin/env python

from __future__ import print_function

import sys

print("\n=============================================================================")

def simpleDecorator(func):  # Always one parameter: the original function!
    def wrapper():
        print(">>> simpleDecorator!")
        func()
        print("<<< simpleDecorator!")

    wrapper.__name__ = func.__name__    # ??? does not work?
    return wrapper

@simpleDecorator
def func1():
    print("Hello func1")
print("===\n")

# func1 = simpleDecorator(func1)    

print(func1)
func1()

print("\n=============================================================================")

def paramDecorator(func):  # Always one parameter: the original function!
    def wrapper(*args, **kwargs):       # pass all parameters from the actual function call ...
        print(">>> simpleDecorator!")
        func(*args, **kwargs)           # ... to the original function
        print("<<< simpleDecorator!")

    wrapper.__name__ = func.__name__    # ??? does not work?
    return wrapper

@paramDecorator
def func2(p1, p2):
    print("Hello func2: {} {}".format(p1, p2))
print("===\n")

# func2 = simpleDecorator(func2)

print(func2)
func2(123, "abc")

print("\n=============================================================================")


# Note: When a decorator is called with parameters, it requires three nested functions -
# the outer and the first inner one are called during decorator instantiation, and the
# inner one is the actual wrapper function!

def decoratorWithParams(d1, d2):
    def decorator(func):  # Always one parameter: the original function!
        def wrapper(*args, **kwargs):       # pass all parameters from the actual function call ...
            print(">>> decoratorWithParams: {} {}".format(d1, d2)) 
            func(*args, **kwargs)           # ... to the original function
            print("<<< decoratorWithParams: {} {}".format(d1, d2)) 

        wrapper.__name__ = func.__name__    # ??? does not work?
        return wrapper
    return decorator

@decoratorWithParams(543, "Hello")
def func3(p1, p2):
    print("Hello func3: {} {}".format(p1, p2))
print("===\n")

# func3 = simpleDecorator(func3)    ??????????????

print(func3)
func3(123, "abc")

print("\n=============================================================================")

# in this case, there is no wrapper function specified, and the decorator() function returns None
# which means that the original function can not be called anymore since there is no reference to it!
def strangeDecorator(d1, d2):
    def decorator(func):    # NOT the wrapper function!
        pass

    return decorator

@strangeDecorator(543, "Hello")
def func4():
    print("Hello func4")
print("===\n")

# func4 = simpleDecorator(func4)

print(func4)    # None
# func4()         # Error

print("\n=============================================================================")

def sideEffectDecorator(func):
    print("   Doing some side effect when defining {}".format(func))

    return func

@sideEffectDecorator
def func5():
    print("Hello func5")
print("===\n")

# func5 = simpleDecorator(func5)

print(func5)
func5()

print("\n=============================================================================")

pathMapping = dict()
def Listener(path):
    def decorator(func):
        print("   Registering {} for {}".format(func, path))
        pathMapping[path] = func.__name__
        return func     # No wrapper required, we just return the original function
    return decorator

@Listener("/app/sample/path")
def func5a():
    print("Hello func5a")

@Listener("/app/sample/path2")
def func5b():
    print("Hello func5b")

for path in pathMapping.iteritems():
    print(path)

print("===\n")
