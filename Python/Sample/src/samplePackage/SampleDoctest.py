#!/usr/bin/python

class Calculator():
    """Simple example how to use doctests"""

    def add(self, a, b):
        """Adds two numbers, a and b, and returns the result.
        
        Usage is simple, just create a Calculator object and call the add() method:

        >>> c = Calculator()
        >>> c.add(1, 3)
        4
        """
        return a+b;

    def multiply(self, a, b):
        """multiplies two numbers, a and b, and returns the result.
        
        Usage is simple, just create a Calculator object and call the multiply() method:

        >>> c = Calculator()
        >>> c.multiply(5, 6)
        30
        """
        return a*b;


if __name__ == "__main__":
    import doctest
    doctest.testmod()
