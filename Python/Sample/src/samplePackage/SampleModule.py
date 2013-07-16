import os
import decimal
import locale
import unicodedata
import sys
import math
import copy
import collections

from random import choice, sample as getSample

from PySide.QtCore import Qt

GlobalVar = "Hello Global"

class Examples():
    def __init__(self, out):
        self.out = out


    def getExamples(self):
        Example = collections.namedtuple("Example", ("label", "function"))
        result = []

        result.append(Example("Calculations", self.calculationsSample))
        result.append(Example("Strings", self.stringSample))
        result.append(Example("Format", self.formatSample))
        result.append(Example("Locale", self.localeSample))
        result.append(Example("Unicode", self.unicodeSample))
        result.append(None)
        result.append(Example("Tuple sample", self.tupleSample))
        result.append(Example("List sample", self.listSample))
        result.append(Example("Set sample", self.setSample))
        result.append(Example("Dictionary sample", self.dictSample))
        result.append(None)
        result.append(Example("File sample", self.fileSample))
        result.append(Example("Program flow sample", self.flowSample))
        result.append(Example("Modules sample", self.modulesSample))

        return result


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


    def generatorSample(self):
        mygenerator = (x*x for x in range(3))
        self.out.writeln("First")
        for i in mygenerator:
            self.out.writeln("  i={}".format(i))
        self.out.writeln("Second")
        for i in mygenerator:
            self.out.writeln("  i={}".format(i))


    def modulesSample(self):
        self.out.writelnColor(Qt.lightGray, 'PYTHONPATH:')
        for pp in os.environ["PYTHONPATH"].split(';'):
            self.out.writeln("  {}".format(pp))

        self.out.writelnColor(Qt.lightGray, '\nsys.path:')
        for p in sys.path:
            self.out.writeln("  {}".format(p))
