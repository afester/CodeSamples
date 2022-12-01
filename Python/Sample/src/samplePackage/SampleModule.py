
"""
This module contains some Python examples for many language features.
"""

import os
import decimal
import locale
import unicodedata
import sys
import math
import copy
import collections
import re


from random import choice, sample as getSample

from PySide2.QtCore import Qt

from html.parser import HTMLParser

GlobalVar = "Hello Global"


class MyHTMLParser(HTMLParser):
    def __init__(self, out):
        HTMLParser.__init__(self)
        self.level = None
        self.out = out

    def handle_starttag(self, tag, attrs):
        result = re.match("^h([1-6])", tag)
        if result:
            self.level = int(result.group(1))

    def handle_endtag(self, tag):
        self.level = None

    def handle_data(self, data):
        if self.level:
            self.out.out.writeln("  {0} {1}".format("*" * self.level, data.strip()))


# A data object containing some data for a Task
class TaskEntry:
    def __init__(self, name, description, createDate, assignee, dueDate):
        self.name = name
        self.description = description
        self.createDate = createDate
        self.assignee = assignee
        self.dueDate = dueDate 

    def __str__(self):
        trunc = lambda s, max : s if len(s) < max else s[:max-3] + "..."  
        return "{0:15}|{1:30}|{2}|{3}|{4}".format(trunc(self.name, 15),
                                                trunc(self.description, 30), 
                                                 self.createDate, 
                                                 self.assignee, 
                                                 self.dueDate)


# The Sample decorator. Adds all methods which are decorated with @Sample
# to a list of samples.
AllSamples = []
def Sample(name, newRow = False):
    def decorator(method):
        if newRow:
            AllSamples.append(None)
        AllSamples.append((name, method))
    return decorator


class Vector2D:
    def __init__(self, x = 0, y = 0):
        self.__x = x
        self.__y = y

    def __repr__(self):
        """Returns the representational form of an object"""
        return "{}({}, {})".format(self.__class__.__name__, self.__x, self.__y)

    def __str__(self):
        """Returns the string form of an object"""
        return "({}, {})".format(self.__x, self.__y)

    def __add__(self, other):
        return Vector2D(self.__x + other.__x, self.__y + other.__y)

    def __sub__(self, other):
        return Vector2D(self.__x - other.__x, self.__y - other.__y)

    def __mul__(self, other):
        if isinstance(other, int):
            return Vector2D(self.__x * other, self.__x * other) # Vector * scalar = Vector
        else:
            return self.__x * other.__x + self.__y * other.__y      # Vector * Vector = Scalar

    def __rmul__(self, other):
        """Need rmul to support commutative scalar multiplication"""
        return Vector2D(self.__x * other, self.__y * other) # Scalar * Vector = Vector

    @property           # getter only ; read-only property
    def length(self):
        return math.sqrt(self.__x ** 2 + self.__y ** 2)

    def value(self):
        return (self.__x, self.__y)


class Examples():

    def __init__(self, out):
        self.out = out


    def getExamples(self):
        Example = collections.namedtuple("Example", ("label", "function"))
        result = []
        for s in AllSamples:
            # The @Sample decorator received the unbound functions.
            # To connect them later, we need the bound methods.
            # See also http://stackoverflow.com/questions/1015307/python-bind-an-unbound-method
            if s is None:
                result.append(None)     # new row
            else:                            
                sampleName = s[0]
                boundMethod = s[1].__get__(self)
                result.append(Example(sampleName, boundMethod))
        return result


    @Sample("Calculations")
    def calculationsSample(self):
        a = 10
        b = 3
        self.out.writeln(a / b)        # produces floating point value - 3.333333333333333333333
        self.out.writeln(int(a / b))   # 3
        self.out.writeln(a // b)       # 3
        
        c = a / b
        self.out.writeln(str(c) + " " + str(type(c)))

        b = 5
        c = b
        b += 5
        self.out.writeln("%s %s" % (b, c) )

        self.out.writeln("bin(1972) = %s" % bin(1972))
        self.out.writeln("hex(1972) = %s" % hex(1972))

        # binary literal
        x = 0b10101010
        self.out.writeln("x = 0b10101010, hex(x) = %s" % hex(x))

        d = decimal.Decimal()
        self.out.writeln("d=%s (%s)" % (d, type(d)))
        a = 1.1
        b = 2.2
        self.out.writeln("float: %s + %s = %s" %(a, b, a+b))
        a = decimal.Decimal('1.1')
        b = decimal.Decimal('2.2')
        self.out.writeln("Decimal: %s + %s = %s" %(a, b, a+b))
        
        self.out.writeln(str(sys.float_info))

        # complex numbers
        cplx = 4 + 5j;
        self.out.writeln("cplx = %s" % cplx)
        cplx = 1j
        self.out.writeln("cplx = %s" % cplx**2)


    @Sample("Strings")
    def stringSample(self):
        helloStr = 'Hello World'
        self.out.writeln(helloStr[4])

        # String is immutable
        # helloStr[2] = 'p'
        
        # From int to String:
        myInt = 1234567890
        self.out.writeln(myInt)
        myStr = str(myInt)
        self.out.writeln(myStr, len(myStr))
        
        # From String to int:
        myStr = "123"
        myInt = int(myStr)
        self.out.writeln(type(myInt))
        
        myFloat = 3.4
        self.out.writeln(type(myFloat))
        self.out.writeln(type(myStr))
        
        self.out.writeln('myStr is myInt: ' + str((myStr is myInt)))
        self.out.writeln('myStr == myInt: ' + str((myStr == myInt)))
        self.out.writeln('myStr == str(myInt): ' + str((myStr == str(myInt))))

        # Strings quoting
        aString = "Hello World" # identical to aString = 'Hello World'
        self.out.writeln(aString)
        aString = "Hello \\ World \\ How \\ are \\ things?"
        self.out.writeln(aString)  
        aString = r"Hello \ World \ How \ are \ things?"
        self.out.writeln(aString)  

        # Long string spreading several lines
        
        # approach 1: triple quoting (note that the end of line is added to the string!)
        aLongString = """Lorem ipsum dolor sit amet, consectetur adipisicing elit, 
sed do eiusmod tempor incididunt ut labore et dolore magna ..."""
        self.out.writeln(aLongString)

        # approach 2: join strings - Disadvantage: requires newline escaping
        aLongString = "aliqua. Ut enim ad minim veniam, quis nostrud exercitation " + \
                      "ullamco laboris nisi ut aliquip ex ea commodo consequat..."
        self.out.writeln(aLongString)

        # approach 3: use parentheses (preferred approach)
        aLongString = ("Duis {0} aute irure dolor in reprehenderit in voluptate velit "
                       "esse cillum dolore eu fugiat nulla pariatur. Excepteur sint".format("TEST") )
        self.out.writeln(aLongString)

        # Using UNICODE characters - note: make sure that the editor is treating this
        # file with UTF-8 encoding! Starting with Python 3.0, UTF-8 is the default encoding.
        # Note that the font also needs to support the corresponding glyphs to render the characters
        # form the following string: 
        unicodeString = "\u20AC€\U000020AC\N{tilde}\u2116\u2107\u20A9\u20AF" + chr(0x23B7)
        self.out.writeln("Euro=%s (len=%s)" % (unicodeString, len(unicodeString)) )
        for c in unicodeString:
            self.out.writeln(hex(ord(c)))

        s = "Hello World"
        self.out.writeln('"{}"[4:]  = {}'.format(s, s[4:]))
        self.out.writeln('"{}"[4:7] = {}'.format(s, s[4:7]))
        self.out.writeln('"{}"[0::2]= {}'.format(s, s[0::2]))
        self.out.writeln('"{}"[-4:]= {}'.format(s, s[-4:]))
        self.out.writeln('"{}"[::-2]= {}'.format(s, s[::-2]))
        self.out.writeln('"{}"[::-1]= {}'.format(s, s[::-1]))
        self.out.writeln('"{}" * 3 = {}'.format(s, (s * 3) ))

        l = ["A", "B", "C"]
        self.out.writeln('l = %s; ":".join(l) = %s' % (l, ":".join(l)) )
        
    
    @Sample("Format")
    def formatSample(self):
        s = "Hello World"

        # note that the % operator has been deprecated with Python 3.1.
        # Use the format method from String instead (http://www.python.org/dev/peps/pep-3101/)
        a = 4
        b = 5
        self.out.writeln("{0} + {1} = {2}".format(a, b, a+b))
        self.out.writeln("{a} + {b} = {c}".format(a=a, b=b, c=a+b))

        self.out.writeln("{a[4]}".format(a=s))
        self.out.writeln("\N{GREEK SMALL LETTER PI} = {0.pi}".format(math))

        self.out.writeln("Locals: " + str(locals()))

        values = {'s1':a, 's2':b, 's':a+b}
        self.out.writeln("{s1} + {s2} = {s}".format(**values) )

        # Format conversions
        s = "翻訳で失われる"
        self.out.writeln("{0} - {0!r} - {0!a}".format(s) )

        self.out.writeln("Default: |{0}|".format(s) )
        self.out.writeln("':25'  : |{0:25}|".format(s) )
        self.out.writeln("':>25' : |{0:>25}|".format(s) )
        self.out.writeln("':.^25': |{0:.^25}|".format(s) )
        for i in range(len(s), len(s) + 5):
            self.out.writeln("':.^{1}': |{0:.^{1}}|".format(s, i) )

        f = math.pi
        self.out.writeln("{0:.3}".format(f))    # Note: without "f", the overall number of digits is counted
        self.out.writeln("{0:.5}".format(f))
        f = 123.456
        self.out.writeln("{0:.3f}".format(f))   # three digits after decimal place
        self.out.writeln("{0:.5f}".format(f))   # Five digits after decimal place


    @Sample("Locale")
    def localeSample(self):        
        x, y= (1234567890, 1234.56)

        defaultLocale = locale.getdefaultlocale()
        self.out.writeln("Default locale: " + str(defaultLocale))

        d= "Standard locale: {0:n} {1:n}".format(x,y)
        self.out.writeln(d)

        locale.setlocale(locale.LC_ALL, "C")
        c= "'C' locale: {0:n} {1:n}".format(x,y)    #c== "1234567890 1234.56"
        self.out.writeln(c)

        #locale.setlocale(locale.LC_ALL, "en_US.UTF-8")
        #en= "{0:n} {1:n}".format(x, y) #en== "1,234,567,890 1,234.56"
        #self.out.writeln(en)
        #locale.setlocale(locale.LC_ALL, "de_DE.UTF-8")

        # Note: the locale names are system dependant. See http://msdn.microsoft.com/en-us/library/39cwe7zf%28vs.71%29.aspx
        # for the Windows names, and "locale -a" for the ones installed on a linux system 
        locale.setlocale(locale.LC_ALL, "deu_deu")
        de= "'deu_deu' locale: {0:n} {1:n}".format(x, y) #de== "1.234.567.890 1.234,56"
        self.out.writeln(de)
    
        # On Windows, not even the locales returned by locale.getdefaultlocale() can be set ...
        #locale.setlocale(locale.LC_ALL, "de_DE")    # locale.Error: unsupported locale setting
        #de2 = "'de_DE' locale: {0:n} {1:n}".format(x, y) #de== "1.234.567.890 1.234,56"
        #self.out.writeln(de2)


    def printUnicode(self, startWith):
        widths = [5, 6, 4, 25]
        header = "{0:^{1}} {2:^{3}} {4:^{5}} {6:^{7}}".format("Dec", widths[0], "Hex", widths[1], "Char", widths[2], "Name", widths[3])
        self.out.writeln(header)
        self.out.writeln("-" * len(header))

        for codePoint in range(startWith, startWith + 16):
            unicodeName = unicodedata.name(chr(codePoint), "(Unknown)")
            #charName = string.capwords(unicodeName)
            charName = unicodeName.title()
            self.out.writeln("{0:>{1}} 0x{0:<{2}x} {3:^{4}} {5}".format(
                        codePoint, widths[0], widths[1] - 2, 
                        chr(codePoint), widths[2], 
                        charName ))

    # Converts a "bytes" object into a readable string
    def bytes2Hex(self, byteStr):
        return " ".join("{0:2x}".format(d) for d in byteStr)


    def str2ord(self, s):
        return " ".join("{0:x}".format(ord(c)) for c in s)

    @Sample("Unicode")
    def unicodeSample(self):
        self.out.writeln("Highest UNICODE code point: 0x{0:x}".format(sys.maxunicode))
        self.printUnicode(0x2722)

        s = "翻る"                   # String in UCS-2 or UCS-4 encoding
        b = s.encode()              # get UTF-8 encoding of the string
        self.out.writeln("{0} ({1}) to UTF-8: {2}".format(s, self.str2ord(s), self.bytes2Hex(b)))

        s = "A"
        b = s.encode()
        self.out.writeln("{0} ({1}) to UTF-8: {2}".format(s, self.str2ord(s), self.bytes2Hex(b)))

        s = "€"
        b = s.encode()
        self.out.writeln("{0} ({1}) to UTF-8: {2}".format(s, self.str2ord(s), self.bytes2Hex(b)))


    @Sample("Tuple", True)
    def tupleSample(self):
        myTuple = ("Hello", "World", 42)
        self.out.writeln("myTuple = {0}".format(myTuple))

        a = 5
        b = 7
        self.out.writeln("a = {0}, b = {1}".format(a, b))
        a, b = (b, a)
        self.out.writeln("a = {0}, b = {1}".format(a, b))
        Address = collections.namedtuple("Address", ("Firstname", "Lastname", "Street", "City"))

        myAddr = Address("Max", "Mustermann", "Musterstr.", "Musterstadt")
        self.out.writeln(myAddr)
        self.out.writeln("Firstname: {}".format(myAddr.Firstname))
        self.out.writeln("Lastname: {}".format(myAddr.Lastname))
        self.out.writeln("Name: {Firstname} {Lastname}".format(**myAddr._asdict() ))

 
    # Sample from the book...
    def get_forenames_and_surnames(self):
        forenames = []
        surnames = []
        
        # Outer loop creates a reference to a list and a reference to a filename
        # with each iteration:
        for names, filename in ((forenames, "data/forenames.txt"),
                                (surnames, "data/surnames.txt")):
            # "names" = reference to corresponding list
            # "filename" = reference to file name

            for name in open(filename, encoding="utf8"):
                # Append line to current list
                names.append(name.rstrip())
                
        # returntuple with two lists 
        return forenames, surnames


    @Sample("List")
    def listSample(self):
        # List
        self.out.writelnColor(Qt.lightGray, 'List samples:')
        myList = [3, 5, 7]
        self.out.writeln(" ", myList, len(myList), type(myList))
        self.out.writeln(" ", myList[2])
        # Lists are mutable:
        myList[2] = 10
        self.out.writeln("  myList = {0}".format(myList))
        self.out.writeln("  myList * 3 = {0}".format(myList * 3) )

        *part, last = myList
        self.out.writeln("  part: {}, last: {}".format(part, last) )

        # Replace a whole slice with a new list
        myList[0:2] = [42]
        self.out.writeln("  myList = {0}".format(myList))
        
        L = ["A", "B", "C", "D", "E", "F"]
        self.out.writeln("  L = {}".format(L) )
        L[2:5] = ["X", "Y"]         # ["A", "B", "X", "Y", "F"]
        self.out.writeln("  L = {}".format(L) )
        
        
        item = L.pop()
        self.out.writeln("  L = {}, item = {}".format(L, item) )

        # test the "del" operator
        self.out.writelnColor(Qt.lightGray, '\ndel operator:')
        del item
        try:
            self.out.writeln("L = {}, item = {}".format(L, item) )
        except UnboundLocalError as ule:
            self.out.writelnColor(Qt.red, "  Error: {}".format(ule) )

        self.out.writelnColor(Qt.lightGray, '\nList comprehensions:')
        L = [1, 2, 3, 4, 5, 6, 7, 8, 9]
        self.out.writeln("  L = {}".format(L) )
        L[1::2] = [0] * len(L[1::2])
        self.out.writeln("  L = {}".format(L) )

        L = [1 for n in range(25)]      # same as [1] * 25
        self.out.writeln("  L = {}".format(L) )
        L = [1] * 25
        self.out.writeln("  L = {}".format(L) )

        # all even numbers up to a specific number
        L = [n for n in range(25) if (n % 2) == 0]
        self.out.writeln("  L = {}".format(L) )

        # Using an iterator to print the list 
        # (how python internally handles the for ... in ... loop):
        self.out.writelnColor(Qt.lightGray, '\nIterator:')
        result = ""
        i = L.__iter__()
        try:
            while True:
                v = i.__next__()
                result = result + " " + str(v)
        except StopIteration:
            pass
        self.out.writeln(" " + result)

        # Using an enumerator
        self.out.writelnColor(Qt.lightGray, '\nEnumerator:')
        E = enumerate(L.__iter__())
        i = E.__iter__()
        while True:
            try:
                e = i.__next__()
                self.out.writeln("  " + str(e))
            except StopIteration:
                break

        self.out.writelnColor(Qt.lightGray, '\nCopying lists:')
        L = ["A", "B", "C", "D"]

        self.out.writelnColor(Qt.lightGray, '  by reference:')
        L2 = L
        self.out.writeln("  L={}, L2={}".format(L, L2))
        L2[1] = 42
        self.out.writeln("  L={}, L2={}".format(L, L2))

        L = ["A", "B", "C", "D"]
        self.out.writelnColor(Qt.lightGray, '  by slice:')
        L2 = L[:]
        self.out.writeln("  L={}, L2={}".format(L, L2))
        L2[1] = 42
        self.out.writeln("  L={}, L2={}".format(L, L2))

        L = ["A", "B", "C", "D"]
        self.out.writelnColor(Qt.lightGray, '  by list():')
        L2 = list(L)
        self.out.writeln("  L={}, L2={}".format(L, L2))
        L2[1] = 42
        self.out.writeln("  L={}, L2={}".format(L, L2))

        self.out.writelnColor(Qt.lightGray, '  nested list:')
        L = ["A", "B", ["C", "D"]]
        L2 = list(L)
        self.out.writeln("  L={}, L2={}".format(L, L2))
        L2[2][0] = 42
        self.out.writeln("  L={}, L2={}".format(L, L2))

        self.out.writelnColor(Qt.lightGray, '  nested list with deepcopy():')
        L = ["A", "B", ["C", "D"]]
        L2 = copy.deepcopy(L)
        self.out.writeln("  L={}, L2={}".format(L, L2))
        L2[2][0] = 42
        self.out.writeln("  L={}, L2={}".format(L, L2))

        l1 = [1, 2, 3]
        l2 = [4, 5, 6]
        l1[1] = l2
        l2[1] = l1
        self.out.writeln("  l1={}, l2={}".format(l1, l2))

        l3 = copy.deepcopy(l1)
        self.out.writeln("  l3={}".format(l3))


    @Sample("Set")
    def setSample(self):
        S = {}  # NOTE: Does not create an empty set, but an empty dict!
        self.out.writeln("S = {} ({})".format(S, type(S)) )
        S = set()
        self.out.writeln("S = {} ({})".format(S, type(S)) )

        S = {1, 10, 5, 8, 2, 5}
        self.out.writeln("S = {}".format(S) )
        S = set("apple")
        self.out.writeln("S = {}".format(S) )

        s1 = {'X', 'A', 'Z', 'C'}
        s2 = {'X', 'Z'}
        self.out.writeln("{} - {} = {}".format(s1, s2, s1 - s2) )

        # all even numbers up to a specific number as a set
        S = {n for n in range(25) if (n % 2) == 0}
        self.out.writeln("S = {}".format(S) )
        
        # A set is mutable
        S.add(30)
        self.out.writeln("S = {}".format(S) )

        F = frozenset(S)
        try:
            F.add(30)               # no add() method in a frozenset
        except AttributeError as ae:
            self.out.writelnColor(Qt.red, ae)

    
    @Sample("Dictionary")
    def dictSample(self):

        # Implemented as a local function inside dictSample() 
        def countWords(fileName):
            count = {}
            with open(fileName, 'r') as f:
                for line in f:
                    words = line.split()
                    for word in words:
                        count[word] = count.get(word, 0) + 1    # avoid KeyError
    
                        #if word in count:
                        #    count[word] += 1
                        #else:
                        #    count[word] = 1
    
            for word, num in count.items():
                self.out.writelnColor(Qt.yellow, "{}: {}".format(word, num) )
    

        # Variant of countWords, using a default dictionary
        # Implemented as a local function inside dictSample() 
        def countWordsDef(fileName):
            count = collections.defaultdict(int)
            with open(fileName, 'r') as f:
                for line in f:
                    words = line.split()
                    for word in words:
                        count[word] +=  1

            for word, num in count.items():
                self.out.writelnColor(Qt.yellow, "{}: {}".format(word, num) )
    
       
        D = {'first':1, 'second':2, 'third':3}
        self.out.writeln("D = {}".format(D) )

        D = dict(fourth=4, fifth=5, sixth=6)
        self.out.writeln("D = {}".format(D) )

        self.out.writeln("D['fifth'] = {}".format(D['fifth']) )

        D['seventh'] = 7
        self.out.writeln("D = {}".format(D) )

        for k, v in D.items():
            self.out.writeln("{} => {}".format(k, v) )

        countWords("data/sample.txt")

        F = {name:os.path.getsize(name)     # Key-value pair for each element 
             for name in os.listdir('.')    # Generator
             if os.path.isfile(name)}       # filter
        self.out.writeln("F: {}".format(F) )

        # Create reverse dictionary
        F = {value:name for name, value in F.items() }
        self.out.writeln("F: {}".format(F) )

        countWordsDef("data/sample.txt")


    def __getGenerator(self):
        return (x*x for x in range(3))

    def __getGenerator2(self):
        for i in range(3):
            print("Hello")
            yield i
            print("Hello 2")
            
        #yield (x*x for x in range(3))


    @Sample("Generator")
    def generatorSample(self):
        self.out.writelnColor(Qt.lightGray, 'Simple generator sample:')
        mygenerator = (x*x for x in range(3))
        self.out.writeln("First")
        for i in mygenerator:
            self.out.writeln("  i={}".format(i))
        self.out.writeln("Second")
        for i in mygenerator:
            self.out.writeln("  i={}".format(i))

        self.out.writelnColor(Qt.lightGray, 'Returning a generator from a function:')
        g = self.__getGenerator()
        self.out.writeln("  g={}".format(type(g)))
        for i in g:
            self.out.writeln("  i={}".format(i))

        g = self.__getGenerator2()
        self.out.writeln("  g={}".format(type(g)))
        for i in g.__iter__():
            print("Hello 3")
            self.out.writeln("  i={}".format(i))

    @Sample("RegExp", True)
    def regExpSample(self):
        self.out.writelnColor(Qt.lightGray, 'Regular Expressions sample 1:')

        sampleInput = 'Hello World12345Garbage'
        sampleRegexp = '(\D*)(\d*)'
        self.out.writelnColor(Qt.yellow, '  Input : "' + sampleInput + '"')
        self.out.writelnColor(Qt.yellow, '  Regexp: "' + sampleRegexp + '"')
        m = re.match(sampleRegexp, sampleInput)
        self.out.writeln('  Text  : "' + m.group(1) + '"')
        self.out.writeln('  Number: "' + m.group(2) + '"')

        self.out.writelnColor(Qt.lightGray, '\nRegular Expressions sample 2:')
        sampleInput = '  33.   /home/andreas/sample/Sample.dat    '
        sampleRegexp = '\s*(\d*)\.\s*(.*)'
        self.out.writelnColor(Qt.yellow, '  Input : "' + sampleInput + '"')
        self.out.writelnColor(Qt.yellow, '  Regexp: "' + sampleRegexp + '"')
        m = re.match(sampleRegexp, sampleInput)
        self.out.writeln('  Number         : "' + m.group(1) + '"')
        self.out.writeln('  Path           : "' + m.group(2) + '"')
        self.out.writeln('  Path (stripped): "' + m.group(2).strip() + '"')

        self.out.writelnColor(Qt.lightGray, '\nRegular Expressions sample 3:')
        # sampleInput = 'Non matching pattern'
        sampleInput = '  33.'
        sampleRegexp = '\s*(\d*)\.\s*(.*)'
        self.out.writelnColor(Qt.yellow, '  Input : "' + sampleInput + '"')
        self.out.writelnColor(Qt.yellow, '  Regexp: "' + sampleRegexp + '"')
        m = re.match(sampleRegexp, sampleInput)
        if (m == None):
            self.out.writeln("NO MATCH")    
        else:
            self.out.writeln(str(m.lastindex) + " Matches.")    
            self.out.writeln('  Number         : "' + m.group(1) + '"')
            self.out.writeln('  Path           : "' + m.group(2) + '"')


    @Sample("File", True)
    def fileSample(self):
        self.out.writelnColor(Qt.lightGray, 'Read tuple of two Lists from two files:')
        data = self.get_forenames_and_surnames()
        self.out.writeln("  " + str(data))

        # Note: choice has been imported from random above!
        firstnames, lastnames = data
        self.out.writelnColor(Qt.lightGray, '\nWrite 10 random lines into file:')
        fh = open("test-names1.txt", "w", encoding="utf8")
        for i in range(10):
            line = "{0} {1} {0}.{1}@example.com\n".format(choice(firstnames),
                                                          choice(lastnames))
            self.out.write("  " + line)
            fh.write(line)
        fh.close()

        # Note: sample has been imported as getSample from random above!
        fh = open("test-names2.txt", "w", encoding="utf8")
        self.out.writelnColor(Qt.lightGray, '\nDifferent approach, using zip():')
        for first, last in zip(getSample(firstnames, len(firstnames)),
                               getSample(lastnames, len(lastnames)) ):
            line = "{0} {1} {0}.{1}@example.com\n".format(first, last)
            self.out.write("  " + line)
            fh.write(line)
        fh.close()

        # Note: sample has been imported as getSample from random above!
        self.out.writelnColor(Qt.lightGray, '\nSorted output:')
        sortedList = list( zip(getSample(firstnames, len(firstnames)),
                               getSample(lastnames, len(lastnames))) )
        sortedList = sorted(sortedList, key = lambda k : k[1])
        for e in sortedList:
            self.out.writeln("  " + str(e))


    @Sample("Program flow")
    def flowSample(self):
        """Some sample code to show program flow statements, like if, while,
           for, and exceptions"""
        self.out.writelnColor(Qt.lightGray, 'Conditional expression:')
        x = 2
        self.out.writeln("  Hello " + ("World" if x == 2 else "Moon") )
        x = 1
        self.out.writeln("  Hello " + ("World" if x == 2 else "Moon") )

        self.out.writelnColor(Qt.lightGray, '\nwhile() loop with else:')
        elseExecuted = False
        x = 0
        while x < 3:
            x += 1
            self.out.writeln("  {}. iteration".format(x))
        else:
            elseExecuted = True
        self.out.writeln("  Else suite executed: {}".format(elseExecuted))

        elseExecuted = False
        x = 0
        while x < 10:
            x += 1
            self.out.writeln("  {}. iteration".format(x))
            if x >= 3:
                break       # terminating loop with break does not execute the else suite!
        else:
            elseExecuted = True
        self.out.writeln("  Else suite executed: {}".format(elseExecuted))

        self.out.writelnColor(Qt.lightGray, '\nfor() loop with else:')
        elseExecuted = False
        for a, b in zip(range(3), range(3, 6)):
            self.out.writeln("  a={} b={}".format(a, b))
        else:
            elseExecuted = True
        self.out.writeln("  Else suite executed: {}".format(elseExecuted))

        elseExecuted = False
        for a, b in zip(range(3), range(3, 6)):
            self.out.writeln("  a={} b={}".format(a, b))
            if a+b == 5:
                break;
        else:
            elseExecuted = True
        self.out.writeln("  Else suite executed: {}".format(elseExecuted))

        self.out.writelnColor(Qt.lightGray, '\nException handling:')
        result = 0
        try:
            result = 1 / 0
            D = {1:"Hello", 2:"World", 3:"Moon"}
            result = D[9]
        except (ZeroDivisionError, KeyError) as ex:
            result = type(ex)
        else:
            self.out.writeln("  else suite")        # not executed, since exception thrown
        finally:
            self.out.writeln("  finally suite")     # always executed
        self.out.writeln("  Result: {}".format(result))

        self.out.writelnColor(Qt.lightGray, '-------')
        result = 0
        try:
            result = 1 / 2
            D = {1:"Hello", 2:"World", 3:"Moon"}
            result = D[9]
        except (ZeroDivisionError, KeyError) as ex:
            result = type(ex)
        else:
            self.out.writeln("  else suite")        # not executed, since exception thrown
        finally:
            self.out.writeln("  finally suite")     # always executed
        self.out.writeln("  Result: {}".format(result))

        self.out.writelnColor(Qt.lightGray, '-------')
        result = 0
        try:
            result = 1 / 2
            D = {1:"Hello", 2:"World", 3:"Moon"}
            result = D[3]
        except (ZeroDivisionError, KeyError) as ex:
            result = type(ex)
        else:
            self.out.writeln("  else suite")        # executed, since no exception thrown
        finally:
            self.out.writeln("  finally suite")     # always executed
        self.out.writeln("  Result: {}".format(result))

        self.out.writelnColor(Qt.lightGray, '\nFunction with default arguments')
        # note that default arguments are created at the point of the function 
        # definition, not when the function is called!
        def listFunc(value, theList = []):
            theList.append(value)
            return theList

        result = listFunc(2)
        self.out.writeln("  List contents: {}".format(result))
        result = listFunc(3)
        self.out.writeln("  List contents: {}".format(result))  # [2, 3] !!!
        
        # Correct approach: use None as default value for mutable objects
        def listFunc2(value, theList = None):
            resultList = [] if theList is None else theList # Make sure to create a new list when the function is called!

            resultList.append(value)
            return resultList

        result = listFunc2(2)
        self.out.writeln("  List contents: {}".format(result))
        result = listFunc2(3)
        self.out.writeln("  List contents: {}".format(result))

        self.out.writelnColor(Qt.lightGray, '\nFunction with variable number of arguments')
        def sumAll(*values):
            result = 0
            for s in values:
                result += s
            return result

        L = [1, 2, 3, 4, 5]
        self.out.writeln("  sumAll({}): {}".format(L, sumAll(*L)))  # Note: need to unpack the list if we want to pass a list to the function!

        def sumUs(*, first, second):    # Force keyword arguments only
            return first + second
        # self.out.writeln("  sumUs({}): {}".format(L[0:2], sumUs(L[0], L[1])) )  # Raises TypeError
        self.out.writeln("  sumUs({}): {}".format(L[0:2], sumUs(first=L[0], second=L[1])) )

        self.out.writelnColor(Qt.lightGray, '\nPass a map with the keyword arguments')
        D = {'first':5, 'second':8}
        self.out.writeln("  sumUs({}): {}".format(D, sumUs(**D)) )

        self.out.writelnColor(Qt.lightGray, '\nUsing the global scope')
        
        def someFunc():
            GlobalVar = "Hello World"       # Creates a new local variable
            self.out.writeln("  GlobalVar = {}".format(GlobalVar) )

        def someFunc2():
            global GlobalVar
            GlobalVar = "Hello World"
            self.out.writeln("  GlobalVar = {}".format(GlobalVar) )

        self.out.writeln("  1) GlobalVar = {}".format(GlobalVar) )
        someFunc()
        self.out.writeln("  2) GlobalVar = {}".format(GlobalVar) ) # Still "Hello Global"!
        someFunc2()
        self.out.writeln("  3) GlobalVar = {}".format(GlobalVar) ) # Now changed to "Hello World"

        self.out.writelnColor(Qt.lightGray, '\nThe lambda function:')
        heron = lambda a, b, c : 0.25 * math.sqrt( (a+b+c)*(a+b-c)*(b+c-a)*(c+a-b) )
        self.out.writeln("  heron({}): {}".format("1, 1, sqrt(2)", heron(1, 1, math.sqrt(2))) )  # 0.5

        self.out.writelnColor(Qt.lightGray, '\nThe assert statement:')
        def sumUs2(first, second):    # Force keyword arguments only
            assert first != 0, "First argument must not be 0"
            assert second != 0, "Second argument must not be 0"
            return first + second
        try:
            self.out.writeln("  sumUs2({}): {}".format([0, 3], sumUs2(0, 3)) )
        except AssertionError as ae:
            self.out.writelnColor(Qt.red, "  {}".format(ae))


    @Sample("Modules")
    def modulesSample(self):

        self.out.writelnColor(Qt.lightGray, 'PYTHONPATH:')
        for pp in os.environ["PYTHONPATH"].split(';'):
            self.out.writeln("  {}".format(pp))

        self.out.writelnColor(Qt.lightGray, '\nsys.path:')
        for p in sys.path:
            self.out.writeln("  {}".format(p))

    @Sample("OOP")
    def oopSample(self):

        self.out.writelnColor(Qt.lightGray, "Simple custom class sample:")

        # See definition of Vector2D at the top of this source file
        v = Vector2D()
        self.out.writeln("  {!r}".format(v))
        self.out.writeln("  {}".format(v))

        v = v + Vector2D(2, 5)
        self.out.writeln("  {}".format(v))

        v1 = v * 3
        self.out.writeln("  {} * 3 = {}".format(v, v1))

        v1 = 3 * v
        self.out.writeln("  3 * {} = {}".format(v, v1))

        v2 = Vector2D(4, 7)
        s = v * v2
        self.out.writeln("  {} * {} = {}".format(v, v2, s))

        self.out.writeln("  |{}| = {}".format(v, v.length))

        v2.__x = 6
        self.out.writeln("  {}".format(v2))


        self.out.writelnColor(Qt.lightGray, "\nImmutable custom class sample:")

        class ImmutableDecimal:
            def __init__(self, value = 0, decimals = 0):
                self.__value= value
                self.__decimals = decimals
                self.__factor = math.pow(10, decimals)

            #def __new__(cls):
            #    pass

            def __repr__(self):
                return "{}({}, {})".format(self.__class__.__name__,
                                           self.__value, self.__decimals)

            def __str__(self):
                return "{}.{}".format(int(self.__value // self.__factor), 
                                      int(self.__value % self.__factor))

        x = ImmutableDecimal(1023, 2)
        self.out.writeln("  {}".format(x))
        self.out.writeln("  {!r} (id={})".format(x, id(x)))

        # Note that the attributes of the ImmutableDecimal class can still be changed
        # through its __init__() method!
        x.__init__(512, 1)
        self.out.writeln("  {!r} (id={})".format(x, id(x)))


        class RealImmutableDecimal:
            def __new__(cls, value = 0, decimals = 0):
                res = super().__new__(cls)
                res.__value = value
                res.__decimals = decimals
                res.__factor= math.pow(10, decimals)
                return res

            def __repr__(self):
                return "{}({}, {})".format(self.__class__.__name__,
                                           self.__value, self.__decimals)

            def __str__(self):
                return "{}.{}".format(int(self.__value // self.__factor), 
                                      int(self.__value % self.__factor))

        x = RealImmutableDecimal(1023, 2)
        self.out.writeln("  {}".format(x))
        self.out.writeln("  {!r} (id={})".format(x, id(x)))

        x.__init__(512, 1)  # does not have any effect anymore - object is immutable!
        self.out.writeln("  {!r} (id={})".format(x, id(x)))


    @Sample("Dates && Times", True)
    def datesSample(self):
        import time
        import datetime
        import calendar

        # from time stamp (seconds since epoch) to struct_time (no DST flag):
        structTime_Epoch = time.gmtime(0)
        self.out.writeln("  " + time.strftime("Epoch (UTC)  : %A, %d. %b %Y %H:%M:%S", structTime_Epoch ))

        # from time stamp (seconds since epoch) to struct_time (considers DST):
        structTime_Epoch = time.localtime(0)
        self.out.writeln("  " + time.strftime("Epoch (Local): %A, %d. %b %Y %H:%M:%S %Z", structTime_Epoch ))

        someDateTime = datetime.datetime(2008, 3, 26)
        self.out.writeln("  " + time.strftime("Some date    : %A, %d. %b %Y %H:%M:%S %Z", someDateTime.utctimetuple()))

        now = time.gmtime()
        self.out.writeln("  " + time.strftime("Now (UTC)    : %A, %d. %b %Y %H:%M:%S", now))

        nowLocal = time.localtime()
        self.out.writeln("  " + time.strftime("Now (Local)  : %A, %d. %b %Y %H:%M:%S %Z", nowLocal))

        cal = calendar.TextCalendar()
        self.out.writeln(cal.formatmonth(nowLocal.tm_year, nowLocal.tm_mon))


    @Sample("HTML parsing")
    def htmlSample(self):
        import urllib.request
        import configparser

        self.out.writelnColor(Qt.lightGray, 'Read proxy settings:')
        config = configparser.ConfigParser()
        config.read('settings.ini')
        self.out.writeln("  Sections: {}".format(config.sections()))
        proxies = config["Proxies"]
        self.out.writeln("  Entries: {}".format(proxies))

        httpProxy = proxies["http"] if "http" in proxies else None
        httpsProxy = proxies["https"] if "https" in proxies else None

        self.out.writeln("  http: {}".format(httpProxy))
        self.out.writeln("  https: {}".format(httpsProxy))

        #THE_URL = "http://www.python.org"
        THE_URL = "http://en.wikipedia.org/wiki/Pythons"
        USER_AGENT='Mozilla/5.0'
        #USER_AGENT='Python-urllib/3.3'

        self.out.writelnColor(Qt.lightGray, '\nAccessing {}:'.format(THE_URL))

        # (Fancy)URLopener deprecated since 3.3
        # opener = urllib.request.FancyURLopener(proxies)

        proxies = {'http': httpProxy}
        ph = urllib.request.ProxyHandler(proxies)

        opener = urllib.request.build_opener(ph)
        opener.addheaders = [('User-agent', USER_AGENT)]
        response = opener.open(THE_URL)

        for header in response.headers:
            self.out.writeln("  {}".format(header))

        # read the content
        s = response.read().decode('utf-8')
        #self.out.writelnColor(Qt.cyan, s)

        self.out.writelnColor(Qt.lightGray, '\nList of all header tags:')
        parser = MyHTMLParser(self)
        parser.feed(s)


    @Sample("Save && Load")
    def saveLoadSample(self):
        import datetime
        import pickle
        import struct

        self.out.writelnColor(Qt.lightGray, 'Sample data:')

        # NOTE: Can not use a local class definition to pickle - 
        # pickle requires the path to the class, but this would be a local one here
        #class TaskEntry:
        #    def __init__(self, name, description, createDate, assignee, dueDate):

        tasks = []
        now = datetime.datetime.now()
        tasks.append(TaskEntry("Implement reader", "Implement the functionality to read a file",
                               now, "Andreas", now + datetime.timedelta(10)))
        tasks.append(TaskEntry("Test reader", "Test if the reader really works",
                               now, "Andreas", now + datetime.timedelta(12)))
        tasks.append(TaskEntry("Implement writer", "Implement the functionality to write a file",
                               now, "Andreas", now + datetime.timedelta(15)))
        tasks.append(TaskEntry("Test writer", "Test if the writer really works",
                               now, "Andreas", now + datetime.timedelta(17)))
        tasks.append(TaskEntry("Ship", "Ship the product",
                               now, "Andreas", now + datetime.timedelta(25)))

        for task in tasks:
            self.out.writelnColor(Qt.yellow, "  {}".format(task))

        self.out.writelnColor(Qt.lightGray, '\nPickle Sample')
        self.out.writelnColor(Qt.red, 'NOTE that Python\'s pickle has similar security constraints like Java serialization!')

        # Get pickle as stream for testing and debugging
        # self.out.writelnColor(Qt.cyan, pickle.dumps(tasks))

        self.out.writeln("  Serializing data to file ...")
        fo = open("tasks.dat", "wb")
        pickle.dump(tasks, fo, pickle.HIGHEST_PROTOCOL)
        fo.close()

        fileInfo = os.stat("tasks.dat")
        self.out.writelnColor(Qt.magenta, "  Size of data file: {} bytes.".format(fileInfo.st_size))

        self.out.writeln("  Reading data back from file ")
        fi = open("tasks.dat", "rb")
        readTasks = pickle.load(fi)
        fi.close()
        
        for task in readTasks:
            self.out.writeln("  {}".format(task))

        self.out.writelnColor(Qt.lightGray, '\nBinary sample')

        self.out.writeln("  Writing data to file ...")
        fo = open("tasks.dat", "wb")
        
        # Simple function to return a length-prefixed byte stream of a given string
        packedString = lambda s : struct.pack( "<H{}s".format(len(s)), len(s), s.encode("utf-8"))

        for task in readTasks:
            data = bytearray()
            data.extend(packedString(task.name))
            data.extend(packedString(task.description))
            data.extend(struct.pack("<I", task.createDate.toordinal()))
            data.extend(packedString(task.assignee))
            data.extend(struct.pack("<I", task.dueDate.toordinal()))
            fo.write(data)

        fo.close()

        fileInfo = os.stat("tasks.dat")
        self.out.writelnColor(Qt.magenta, "  Size of data file: {} bytes.".format(fileInfo.st_size))

        self.out.writeln("  Reading data back from file ")
        fi = open("tasks.dat", "rb")
        
        # NOTE: The followig code only uses very rudimentary error handling -
        # its main purpose is to show the unpack mechanism!
        def readString():
            data = fi.read(struct.calcsize("<H"))                       # read length
            if len(data) == 0:
                return None

            stringLen = struct.unpack("<H", data)[0]                    # unpack length

            data = fi.read(struct.calcsize("<{}s".format(stringLen)))   # read string
            return data.decode()                                        # convert to string

        readTasks = []
        while True:
            values = {}
            values["name"] = readString()
            if values["name"] is None:
                break;

            values["description"] = readString()
            data = fi.read(struct.calcsize("<I"))                       # read data ordinal
            values["createDate"] = datetime.datetime.fromordinal(struct.unpack("<I", data)[0])
            values["assignee"] = readString()
            data = fi.read(struct.calcsize("<I"))                       # read data ordinal
            values["dueDate"] = datetime.datetime.fromordinal(struct.unpack("<I", data)[0])

            readTasks.append(TaskEntry(**values))

        fi.close()

        for task in readTasks:
            self.out.writeln("  {}".format(task))

        self.out.writelnColor(Qt.lightGray, '\nXMLsample')
        import xml.etree.ElementTree
        import gzip

        # Create the root node
        root = xml.etree.ElementTree.Element("tasks")
        for task in tasks:
            # Create a task node
            element = xml.etree.ElementTree.Element("task",
                        name = task.name,
                        description = task.description,
                        createDate = task.createDate.isoformat(),
                        assignee = task.assignee,
                        dueDate = task.dueDate.isoformat())
            root.append(element)
        tree = xml.etree.ElementTree.ElementTree(root)
        tree.write("tasks.xml", "utf-8")

        compressed = gzip.open("tasks.xmz", "wb")
        tree.write(compressed, "utf-8")
        compressed.close()

        fileInfo = os.stat("tasks.xml")
        self.out.writelnColor(Qt.magenta, "  Size of data file: {} bytes.".format(fileInfo.st_size))
        fileInfo = os.stat("tasks.xmz")
        self.out.writelnColor(Qt.magenta, "  Size of compressed data file: {} bytes.".format(fileInfo.st_size))

        self.out.writeln("  Reading data back from file")
        readTasks = []
        xml.etree.ElementTree.parse("tasks.xml")
        ISO8601_FORMAT = "%Y-%m-%dT%H:%M:%S.%f" 
        for taskElement in tree.findall("task"):
            name = taskElement.get("name")
            description = taskElement.get("description")
            createDate = datetime.datetime.strptime(taskElement.get("createDate"), ISO8601_FORMAT)
            assignee = taskElement.get("assignee")
            dueDate = datetime.datetime.strptime(taskElement.get("dueDate"), ISO8601_FORMAT)
            readTasks.append(TaskEntry(name, description, createDate, assignee, dueDate))

        for task in readTasks:
            self.out.writeln("  {}".format(task))

        self.out.writeln("  Reading data back from compressed file")

        compressed = gzip.open("tasks.xmz", "rb")

        readTasks = []
        xml.etree.ElementTree.parse(compressed)
        ISO8601_FORMAT = "%Y-%m-%dT%H:%M:%S.%f" 
        for taskElement in tree.findall("task"):
            name = taskElement.get("name")
            description = taskElement.get("description")
            createDate = datetime.datetime.strptime(taskElement.get("createDate"), ISO8601_FORMAT)
            assignee = taskElement.get("assignee")
            dueDate = datetime.datetime.strptime(taskElement.get("dueDate"), ISO8601_FORMAT)
            readTasks.append(TaskEntry(name, description, createDate, assignee, dueDate))
        compressed.close()

        for task in readTasks:
            self.out.writeln("  {}".format(task))


    @Sample("Advanced Procedural", True)
    def advancedSample(self):
        self.out.writelnColor(Qt.lightGray, 'Switch-like construct:')
        
        def printOne(): self.out.writeln("  One")
        def printTwo(): self.out.writeln("  Two")
        def printThree(): self.out.writeln("  Three")
        def printFour(): self.out.writeln("  Four")

        value = 3
        switch = {1 : printOne,
                  2 : printTwo,
                  3 : printThree,
                  4 : printFour}
        switch[value]()

        self.out.writelnColor(Qt.lightGray, '\nAdvanced yield:')

        def evenNumbers(start = 0):
            self.out.writelnColor(Qt.cyan, "  Start of generator")

            result = start
            while True:
                self.out.writelnColor(Qt.cyan, "    Before yield")
                newValue = (yield result)
                self.out.writelnColor(Qt.cyan, "    After yield; result={}".format(newValue))

                if newValue is not None:
                    result = newValue
                else:
                    result = result + 2 
                self.out.writelnColor(Qt.cyan, "    At end of while loop")
            self.out.writelnColor(Qt.cyan, "  End of generator")

        gen = evenNumbers()
        self.out.writelnColor(Qt.magenta, "  Starting for-loop")
        for x in gen:
            self.out.writelnColor(Qt.magenta, "    In for-loop")

            if x == 10:
                self.out.writelnColor(Qt.magenta, "      Sending value to generator")
                gen.send(20)        # switch context to the yield!!
                self.out.writelnColor(Qt.magenta, "      Value sent.")

            self.out.writelnColor(Qt.magenta, "    Checking end condition")
            if x > 30:
                self.out.writelnColor(Qt.magenta, "      Breaking out of loop")
                break
            self.out.writeln("    End of loop: {}".format(x))

        self.out.writelnColor(Qt.lightGray, '\nDynamic code execution:')
        from PySide.QtGui import QInputDialog, QLineEdit
        result = QInputDialog.getText(None, "Enter function", "Function:", QLineEdit.Normal, "math.sin(x)")
        if result[1]:
            fun = result[0]
            self.out.writeln("  f(x) = {0}".format(fun))
            for x in range(10):
                y = eval(fun)
                self.out.writeln("  f({0}) = {1}".format(x, y))            

        self.out.writelnColor(Qt.lightGray, '\nDecorators:')

        def myFunction(): pass
        self.out.writeln("  {0}".format(myFunction))

        def Decorator(func):
            self.out.writeln("  Creating Decorator ...")
            def wrapper():
                self.out.writeln("  In wrapper()")
                func()
            return wrapper

        @Decorator
        def myDecoratedFunction(): 
            self.out.writeln("  In myDecoratedFunction()")
            pass

        self.out.writeln("  {0}".format(myDecoratedFunction))
        myDecoratedFunction()

        self.out.writelnColor(Qt.lightGray, '\nMethod Decorators:')

        global output
        output = self.out
        
        def MethodDecorator(method):
            self.out.writeln("  Creating Decorator ...")
            def wrapper(self):
                output.writeln("  In wrapper()")
                method(self)
            return wrapper

        class myClass:
            @MethodDecorator
            def myMethod(self):
                output.writeln("  In myClass.myMethod()")

        mc = myClass()
        mc.myMethod()

        self.out.writelnColor(Qt.lightGray, '\nFunction annotations:')

        def simpleFunc(a, b, c):
            return 42 * a * b * c
        self.out.writeln("  {0}".format(simpleFunc.__annotations__))

        def simpleFunc2(a : math.sin(x), b : Sample, c : int) -> 123:
            return 42 * a * b * c
        self.out.writeln("  {0}".format(simpleFunc2.__annotations__))


        self.out.writelnColor(Qt.lightGray, '\n__slots__:')
        v = Vector2D(3, 8)              # "normal" custom class - see above
        self.out.writeln("  {0}".format(v.__dict__))

        class Vector2D_slot:
            
            __slots__ = ("__x", "__y")
            
            def __init__(self, x = 0, y = 0):
                self.__x = x
                self.__y = y
        
            def __repr__(self):
                """Returns the representational form of an object"""
                return "{}({}, {})".format(self.__class__.__name__, self.__x, self.__y)
        
            def __str__(self):
                """Returns the string form of an object"""
                return "({}, {})".format(self.__x, self.__y)
        
            def __add__(self, other):
                return Vector2D(self.__x + other.__x, self.__y + other.__y)
        
            def __mul__(self, other):
                if isinstance(other, int):
                    return Vector2D(self.__x * other, self.__x * other) # Vector * scalar = Vector
                else:
                    return self.__x * other.__x + self.__y * other.__y      # Vector * Vector = Scalar
        
            def __rmul__(self, other):
                """Need rmul to support commutative scalar multiplication"""
                return Vector2D(self.__x * other, self.__y * other) # Scalar * Vector = Vector
        
            @property           # getter only ; read-only property
            def length(self):
                return math.sqrt(self.__x ** 2 + self.__y ** 2)

        v = Vector2D_slot(3, 8)
        try:
            self.out.writeln("  {0}".format(v.__dict__))    # 'Vector2D_slot' object has no attribute '__dict__'
        except AttributeError as ae:
            self.out.writelnColor(Qt.red, "  {0}".format(ae))

    @Sample("Advanced OOP")
    def sample100(self):

        self.out.writelnColor(Qt.lightGray, '\nConstants class:')
        class Const:
            def __setattr__(self, name, value):
                if name in self.__dict__:
                    raise ValueError("can not change a const attribute")
                self.__dict__[name]=value

            def __delattr__(self, name):
                if name in self.__dict__:
                    raise ValueError("cannot delete a constattribute")
                raise AttributeError("'{0}' object has no attribute '{1}'".format(self.__class__.__name__, name))            

        c = Const()
        self.out.writeln("  {0}".format(c.__dict__))
        c.LOW = 100
        c.HIGH = 512
        c.STR = "Hello World"
        self.out.writeln("  {0}".format(c.__dict__))

        try:
            c.HIGH = 1024                       # can not change a const attribute
        except ValueError as ve:
            self.out.writelnColor(Qt.red, "  {0}".format(ve))

        self.out.writelnColor(Qt.lightGray, '\nContext managers:')
        
        class Mgr:
            def __enter__(self):
                output.writeln("  In __enter__()")
                return self

            def __exit__(self, exType, ex, tb):
                output.writeln("  In __exit__({}, {}, {})".format(exType, ex, tb))

        class MyException(Exception) : pass

        try:
            with Mgr() as mgr:
                self.out.writeln("  In with suite: {}".format(mgr))
                self.out.writelnColor(Qt.red, "  Raising exception!")
                raise MyException("EXCEPTION!")
        except MyException as me:
            self.out.writelnColor(Qt.red, "  Catched {0}".format(me))


        self.out.writelnColor(Qt.lightGray, '\nDescriptors:')

        class UpCaser:
            def __init__(self, attrName):
                self.attrName = attrName

            def __get__(self, instance, owner=None):
                value = getattr(instance, self.attrName)
                output.writeln("    __get__():  {}".format(value))
                return value.upper()


        class DataClass:
            
            nameInUpperCase = UpCaser("name")

            def __init__(self, name):
                self.name = name

        dc = DataClass("Hello")
        self.out.writeln("  dc.name = {0}".format(dc.name))
        self.out.writeln("  dc.name in upper case = {0}".format(dc.nameInUpperCase))


    @Sample("Functional programming")
    def sample101(self):
        import functools

        self.out.writelnColor(Qt.lightGray, 'Mapping:')
        L = [1, 2, 3, 4, 5]
        self.out.writeln("  L = {}".format(L))
        
        L_mapped = list(map(lambda x : x**2, L)) 
        self.out.writeln("  L_mapped = {}".format(L_mapped))

        self.out.writelnColor(Qt.lightGray, '\nFiltering:')
        L_filtered = list(filter(lambda x : x > 4, L_mapped))
        self.out.writeln("  L_filtered = {}".format(L_filtered))

        self.out.writelnColor(Qt.lightGray, '\nReducing:')
        reduced = functools.reduce(lambda x, y : x * y, L_filtered)
        self.out.writeln("  reduced = {}".format(reduced))

        reducedMin = functools.reduce(min, L_filtered)
        self.out.writeln("  reduced(min) = {}".format(reducedMin))

        reducedMax = functools.reduce(max, L_filtered)
        self.out.writeln("  reduced(max) = {}".format(reducedMax))

        reducedSum = sum(L_filtered)    # Note: min/max etc. take two parameters, but sum() takes the list as one parameter! 
        self.out.writeln("  reduced(sum) = {}".format(reducedSum))

    @Sample("Coroutines")
    def sample102(self):
        
        self.out.writelnColor(Qt.lightGray, 'Coroutine sample:')

        # create a generator function instead of a normal function
        # http://docs.python.org/3/reference/simple_stmts.html#the-yield-statement
        # "Using a yield statement in a function definition is sufficient to cause 
        #  that definition to create a generator function instead of a normal function."
        def myCoroutine():
            self.out.writelnColor(Qt.cyan, '  Starting coroutine ...')
            nextVal = 0
            while True:
                self.out.writelnColor(Qt.cyan, '    In coroutine - now suspending ...')
                data = yield nextVal    # nextVal returned as result from next()
                self.out.writelnColor(Qt.cyan, '    Resuming coroutine with {}'.format(data))
                if data is not None:
                    break
                nextVal += 1
            self.out.writelnColor(Qt.cyan, '  Coroutine done.')

        self.out.writelnColor(Qt.yellow, '  Creating coroutine ...')
        go = myCoroutine()              # Note: this does not actually execute the coroutine, but only creates the generator object!
        self.out.writelnColor(Qt.yellow, '  Created generator: {}'.format(go))

        self.out.writelnColor(Qt.yellow, '  Calling generator.next()')
        result = next(go)                        # NOW, we are calling the actual coroutine the first time 
        self.out.writelnColor(Qt.yellow, '  Back from generator.next(): {}'.format(result))
        
        self.out.writelnColor(Qt.yellow, '  Calling generator.next()')
        result = next(go) 
        self.out.writelnColor(Qt.yellow, '  Back from generator.next(): {}'.format(result))

        self.out.writelnColor(Qt.yellow, '  Sending generator a termination signal ...')
        try:
            go.send(True)
        except StopIteration as si:
            self.out.writelnColor(Qt.red, "Exception: ".format(si))
        self.out.writelnColor(Qt.yellow, '  Termination signal sent.')

        self.out.writelnColor(Qt.yellow, '  Calling generator.next()')
        try:
            result = next(go)                        # NOW, we are calling the actual coroutine the first time 
        except StopIteration as si:
            self.out.writelnColor(Qt.red, "Exception: ".format(si))
        self.out.writelnColor(Qt.yellow, '  Back from generator.next(): {}'.format(result))


    @Sample("Unit tests")
    def sample103(self):
        import unittest
        import io

        class Vector2DTest(unittest.TestCase):
            
            def setUp(self):
                self.first = Vector2D(1, 2)
                self.second= Vector2D(3, 4)

            def testAdd(self) :
                result = self.first + self.second
                self.assertEqual(result.value(), (4, 6))

            def testSub(self) :
                result = self.first - self.second
                self.assertEqual(result.value(), (-2, -2))

            def testMul(self) :
                result = self.first * self.second
                self.assertEqual(result, 42)    # fail intentionally

        self.out.writelnColor(Qt.lightGray, 'Unittest sample:')

        result = io.StringIO()
        runner = unittest.TextTestRunner(result)
        suite = unittest.TestLoader().loadTestsFromTestCase(Vector2DTest)
        runner.run(suite)

        # Print somewhat formatted test run results
        text = result.getvalue()
        lines = text.split("\n")
        for line in lines:
            self.out.writelnColor(Qt.white, "  " + line)


    @Sample("Profiling")
    def sample104(self):
        import timeit
        import cProfile
        import io, pstats

        self.out.writelnColor(Qt.lightGray, 'Profiling sample using timeit:')

        t = timeit.Timer("first + second",
                         ("from samplePackage.SampleModule import Vector2D\n"
                          "first = Vector2D(1, 2)\n"
                          "second = Vector2D(4, 5)") )
        result = t.timeit()         # runs 1000000 times!
        self.out.writeln("  {} sec.".format(result))

        self.out.writelnColor(Qt.lightGray, '\nProfiling sample using cProfile:')

        pr = cProfile.Profile()

        pr.enable()
        first = Vector2D(1, 2)
        second = Vector2D(4, 5)
        for i in range(1000000):
            third = first + second
        pr.disable()

        s = io.StringIO()
        ps = pstats.Stats(pr, stream=s)
        ps.strip_dirs()

        ps.print_stats()    # print result into stream

        for line in s.getvalue().split("\n"):
            self.out.writelnColor(Qt.white, "  " + line)

    @Sample("Matrix", True)
    def sample105(self):

        class Matrix:
            def __init__(self, values = None, w = 0, h = 0):
                if values is None:
                    # Generate a null matrif of size w * h
                    self.matrix = [ ([0] * h) for n in range(w)]
                else:
                    self.matrix = copy.deepcopy(values)

            def __str__(self):
                """Returns the string form of an object"""
                result = ""
                for row in self.matrix:
                    for e in row:
                        result = result + " {:3}".format(e)
                    result = result + "\n"

                return result 

            def det(self):
                if len(self.matrix) != len(self.matrix[0]):
                    raise ValueError("Can not calculate determinant of non-square matrix")
                if len(self.matrix) == 1:
                    return self.matrix[0][0]
                if len(self.matrix) == 2:
                    return self.matrix[0][0] * self.matrix[1][1] - self.matrix[0][1] * self.matrix[1][0]    
                if len(self.matrix) == 3:
                    return self.matrix[0][0] * self.matrix[1][1] * self.matrix[2][2] \
                         + self.matrix[0][1] * self.matrix[1][2] * self.matrix[2][0] \
                         + self.matrix[0][2] * self.matrix[1][0] * self.matrix[2][1] \
                         - self.matrix[0][2] * self.matrix[1][1] * self.matrix[2][0] \
                         - self.matrix[0][1] * self.matrix[1][0] * self.matrix[2][2] \
                         - self.matrix[0][0] * self.matrix[1][2] * self.matrix[2][1]
                raise ValueError("Matrix is too large - only 1, 2, or 3 columns/rows currently supported")

        self.out.writelnColor(Qt.lightGray, 'Matrix sample:')

        m = Matrix(w = 4, h = 4)
        self.out.writeln("{}".format(m))

        m = Matrix([[11, 0, 0, 0],
                    [11, 11, 0, 0],
                    [11, 11, 11, 0],
                    [11, 11, 11, 11]])
        self.out.writeln("{}".format(m))

        m = Matrix([[1, 2, 3],
                    [3, 4, 5],
                    [6, 7, 8],
                    [9, 0, 1]])
        try:
            self.out.writeln("{}".format(m.det()))
        except ValueError as ve:
            self.out.writelnColor(Qt.red, "  {}".format(ve))

        m = Matrix([[1, 2, 3, 0],
                    [3, 4, 5, 1],
                    [6, 7, 8, 2],
                    [9, 0, 1, 3]])
        try:
            self.out.writeln("{}".format(m.det()))
        except ValueError as ve:
            self.out.writelnColor(Qt.red, "  {}".format(ve))

        m = Matrix([[42]])
        self.out.writeln("{}".format(m.det()))

        m = Matrix([[1, 2],
                    [3, 4]])
        self.out.writeln("{}".format(m.det()))

        m = Matrix([[0, 1, 2],
                    [3, 2, 1],
                    [1, 1, 0]])
        self.out.writeln("{}".format(m.det()))

        x1, y1 = (0, 0)
        x2, y2 = (4, 0)
        x3, y3 = (2, 4)
        triangle = Matrix([[1, x1, y1],
                           [1, x2, y2],
                           [1, x3, y3]])
        area = 0.5 * triangle.det()
        self.out.writeln("  triangle area: {}".format(area))


    @Sample("Combinations")
    def sample106(self):
        self.out.writelnColor(Qt.lightGray, 'Permutations sample:')
        self.out.writelnColor(Qt.lightGray, 'Given a list L, calculate all possible permutations of its elements.')

        # From http://stackoverflow.com/questions/104420/how-to-generate-all-permutations-of-a-list-in-python?rq=1
        def all_perms(elements):
            self.out.writelnColor(Qt.red, "  all_perms({})".format(elements))
            # function is called recursively for each right sub list and 
            # creates a generator for all elements of this sub list.
            # if the input is [1, 2, 3], then the function is called three times:
            # [1, 2, 3]    (initial call) 
            # [2, 3]       (first recursion)
            # [3]          (second recursion)
            if len(elements) <=1:
                yield elements
            else:
                for perm in all_perms(elements[1:]):    # recursively create new generator for right sub list
                    #  perm contains the next permutation of the sub list
                    self.out.writelnColor(Qt.red, "    perm = {}".format(perm))

                    # insert the first element inside all positions of the current sub permutation
                    for i in range(len(elements)):
                        yield perm[:i] + elements[0:1] + perm[i:]

        L = ['A', 'B', 'C']
        self.out.writelnColor(Qt.yellow, "  L = {}".format(L))
        for e in all_perms(L):
            self.out.writeln("    {}".format(e))

        L = [1, 2, 3, 4]
        self.out.writelnColor(Qt.yellow, "  L = {}".format(L))
        for e in all_perms(L):
            self.out.writeln("    {}".format(e))

        self.out.writelnColor(Qt.lightGray, 'Generation sample:')
        self.out.writelnColor(Qt.lightGray, 'Given a list L, calculate all possible permutations of its elements.')

        def add(sum1, sum2, base):
            pass

        number = 0
        number = add(number, 1, 3)



    def folderTree(self, folders, indent = 0):
        """Displays all available folders in a tree structure.

        Keyword arguments:
        folders    --  The current Folders iterator
        indent     --  The current indent level
        """
        prefix = ' ' * (indent*2)
        i = 0
        for folder in folders:
            self.out.writelnColor(Qt.yellow, "{}{}. {} ({})".format(prefix, i, folder.Name, folder.DefaultItemType))
            self.folderTree(folder.Folders, indent + 1)
            i = i + 1


    def findFolder(self, folders, searchPath, level=0):
        """Find a folder by following a given  folder path.

        Keyword arguments:
        folders    --  The Folders iterator
        searchPath --  The search path - a string array with the folder names 
        level      --  The current search level
        """
        for folder in folders:
            if folder.Name == searchPath[level]:
                if level < len(searchPath)-1:
                    # Search sub folder
                    folder = self.findFolder(folder.folders, searchPath, level+1)
                return folder
        return None


    def printCalendar(self, folder):
        """Prints calendar events during the next 30 days.

        Keyword arguments:
        folder    --  The Calendar folder to use.
        """
        import datetime
        
        # Get the AppointmentItem objects
        # http://msdn.microsoft.com/en-us/library/office/aa210899(v=office.11).aspx
        items = folder.Items

        # Restrict to items in the next 30 days
        begin = datetime.date.today()
        end = begin + datetime.timedelta(days = 30);
        restriction = "[Start] >= '" + begin.strftime("%m/%d/%Y") + "' AND [End] <= '" +end.strftime("%m/%d/%Y") + "'"
        restrictedItems = items.Restrict(restriction)

        # Print items
        for appointmentItem in restrictedItems:
            self.out.writelnColor(Qt.yellow, "{0} Start: {1}, End: {2}, Organizer: {3}".format(
                  appointmentItem.Subject, appointmentItem.Start, 
                  appointmentItem.End, appointmentItem.Organizer))


    @Sample("Win32 COM")
    def sample107(self):

        import win32com.client

        # get Outlook application object
        Outlook = win32com.client.Dispatch("Outlook.Application")
        self.out.writelnColor(Qt.yellow, "Outlook version: {}".format(Outlook.Version))
        self.out.writelnColor(Qt.yellow, "Default profile name: {}".format(Outlook.DefaultProfileName))

        # get the Namespace / Session object
        # namespace = Outlook.GetNamespace("MAPI") 
        namespace = Outlook.Session         # identical to GetNameSpace("MAPI") (starting with Outlook 98)
        self.out.writelnColor(Qt.yellow, "Current profile name: {}".format(namespace.CurrentProfileName))

        # Dump all available address lists
        self.out.writelnColor(Qt.yellow, "\nAddress lists")
        self.out.writelnColor(Qt.yellow, "-------------")
        addrLists = namespace.AddressLists
        i = 0
        for al in addrLists:
            self.out.writelnColor(Qt.yellow, "{}. {}".format(i, al))
            i = i + 1

        # Show tree of all available folders
        self.out.writelnColor(Qt.yellow, "\nFolders")
        self.out.writelnColor(Qt.yellow, "-------")
        self.folderTree(namespace.Folders)

        # get own calendar and print all entries in the next 30 days
        self.out.writelnColor(Qt.yellow, "\nMy calendar")
        self.out.writelnColor(Qt.yellow, "---------------")
        myCalendar = namespace.GetDefaultFolder(9)
        self.printCalendar(myCalendar)

        # get shared calendar and print all entries in the next 30 days
        # Should work, but could not get it to work actually - "Object not found" ...
        # recipient = namespace.createRecipient("Change Me")
        # self.out.writelnColor(Qt.yellow, recipient)
        # resolved = recipient.Resolve()
        # self.out.writelnColor(Qt.yellow, resolved)
        # self.out.writelnColor(Qt.yellow, recipient.resolved)
        # sharedCalendar = namespace.GetSharedDefaultFolder(recipient, 9)
        # self.out.writelnColor(Qt.yellow, sharedCalendar)
        # self.printCalendar(sharedCalendar)

        # get shared calendar through folder tree
        self.out.writelnColor(Qt.yellow, "\nShared calendar")
        self.out.writelnColor(Qt.yellow, "---------------")
        sharedCalendar = self.findFolder(namespace.Folders, ["Some shared workspace", "Calendar"])
        self.out.writelnColor(Qt.yellow, sharedCalendar)
        self.out.writelnColor(Qt.yellow, sharedCalendar.Parent)
        self.printCalendar(sharedCalendar)
