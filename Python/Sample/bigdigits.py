# From: Programming in Python 3: A complete introduction to the Python Language, Second Edition
import sys

Zero = ["  ***  ",
        " *   * ",
        "*     *",
        "*     *",
        "*     *",
        " *   * ",
        "  ***  "]

One =  [" * ",
        "** ",
        " * ",
        " * ",
        " * ",
        " * ",
        "***"]

Two = [" *** ",
       "*   *",
       "*  * ",
       "  *  ",
       " *   ",
       "*    ",
       "*****"]

Three = [" *** ",
         "*   *",
         "    *",
         "  ** ",
         "    *",
         "*   *",
         " *** "]

Four = ["*    ",
        "*  * ",
        "*  * ",
        "*****",
        "   * ",
        "   * ",
        "   * "]

Five = ["*****",
        "*    ",
        "*    ",
        "**** ",
        "    *",
        "*   *",
        " *** "]

Six = [" *** ",
       "*    ",
       "*    ",
       "**** ",
       "*   *",
       "*   *",
       " *** "]

Seven = ["******",
         "     *",
         "    * ",
         " **** ",
         "  *   ",
         " *    ",
         "*     "]

Eight = [" **** ",
         "*    *",
         "*    *",
         " **** ",
         "*    *",
         "*    *",
         " ***  "]

Nine = [" *** ",
        "*   *",
        "*   *",
        " ****",
        "    *",
        "    *",
        " *** "]

Digits= [Zero, One,Two,Three, Four, Five, Six, Seven, Eight, Nine]

def bigDigits1(digits):
    try:
        for row in range(7):
            line= ""
    
            for d in digits:
                number = int(d)
                digit = Digits[number]
                line += digit[row] + "  "
    
            print(line)
    
    except ValueError as err:
        print(err, "in", digits)


def bigDigits2(digits):
    try:
        for row in range(7):
            line= ""
    
            for d in digits:
                number = int(d)
                digit = Digits[number]

                for pixel in digit[row]:
                    line += pixel.replace('*', d)
                    #if pixel == '*':
                    #    line += d
                    #else:
                    #    line += ' '

                line += "  "
    
            print(line)
    
    except ValueError as err:
        print(err, "in", digits)


# main()

try:
    param = sys.argv[1]
    bigDigits1(param)
    print()
    bigDigits2(param)
except IndexError:
    print("usage: bigdigits.py <number>")
