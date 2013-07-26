import sys

from java.util import ArrayList
from java.lang import System

def main():
    print("Hello Jython: {}".format(sys.version))
    
    arr = ArrayList()
    arr.add("Hello World")
    arr.add("Hello Moon")

    System.err.println(arr)


if __name__ == '__main__':
    main()
