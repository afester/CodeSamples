#!/usr/bin/python

import sys
import decimal
import math
import locale
import unicodedata
import collections

from PySide.QtCore import Qt
from PySide.QtGui import QApplication, QMainWindow, QTextEdit, QPushButton, QWidget
from PySide.QtGui import QVBoxLayout, QGridLayout, QPalette

class MainWindow(QMainWindow):
    def __init__(self):
        QMainWindow.__init__(self)
        self.resize(800, 600)

        # Create the main content widget
        mainWidget = QWidget(self)
        self.setCentralWidget(mainWidget)

        # Create a text component at the top area of the main widget
        self.output = QTextEdit(mainWidget)
        self.output.setReadOnly(True)

        # set the font
        font = self.output.font()
        font.setFamily("Courier")
        font.setPointSize(10)
        self.output.setFont(font)

        # Set the background color
        # self.output.setTextBackgroundColor(Qt.red) # Only sets the background color for the text itself, not for the whole widget
        pal = self.output.palette()
        pal.setColor(QPalette.Base, Qt.black)
        self.output.setPalette(pal)

        mainLayout = QVBoxLayout(mainWidget)
        mainLayout.addWidget(self.output)

        # Create buttons in a grid layout below the top area
        buttonWidget = QWidget(mainWidget)
        self.buttonLayout = QGridLayout(buttonWidget)
        mainLayout.addWidget(buttonWidget)

        # Add some buttons to execute python code
        self.row = 0
        self.column = 0
        self.addButton("Clear console", lambda : self.output.clear())

        self.newRow()
        self.addButton("Calculations", self.calculationsSample)
        self.addButton("Strings", self.stringSample)
        self.addButton("Format", self.formatSample)
        self.addButton("Locale", self.localeSample)
        self.addButton("Unicode", self.unicodeSample)

        self.newRow()
        self.addButton("Tuple sample", self.tupleSample)
        self.addButton("List sample", self.listSample)
        self.addButton("Set sample", self.setSample)
        self.addButton("Dictionary sample", self.dictSample)


    def addButton(self, label, function):
        theButton = QPushButton(label)
        theButton.clicked.connect(function)
        self.buttonLayout.addWidget(theButton, self.row, self.column)
        self.column += 1


    def newRow(self):
        self.row += 1
        self.column = 0


    def writeln(self, *text):
        theText =  ' '.join(map(str, text))
        self.output.setTextColor(Qt.green)
        self.output.append(theText)


    def calculationsSample(self):
        a = 10
        b = 3
        self.writeln(a / b)        # produces floating point value - 3.333333333333333333333
        self.writeln(int(a / b))   # 3
        self.writeln(a // b)       # 3
        
        c = a / b
        self.writeln(str(c) + " " + str(type(c)))

        b = 5
        c = b
        b += 5
        self.writeln("%s %s" % (b, c) )

        self.writeln("bin(1972) = %s" % bin(1972))
        self.writeln("hex(1972) = %s" % hex(1972))

        # binary literal
        x = 0b10101010
        self.writeln("x = 0b10101010, hex(x) = %s" % hex(x))

        d = decimal.Decimal()
        self.writeln("d=%s (%s)" % (d, type(d)))
        a = 1.1
        b = 2.2
        self.writeln("float: %s + %s = %s" %(a, b, a+b))
        a = decimal.Decimal('1.1')
        b = decimal.Decimal('2.2')
        self.writeln("Decimal: %s + %s = %s" %(a, b, a+b))
        
        self.writeln(str(sys.float_info))

        # complex numbers
        cplx = 4 + 5j;
        self.writeln("cplx = %s" % cplx)
        cplx = 1j
        self.writeln("cplx = %s" % cplx**2)

    def stringSample(self):
        helloStr = 'Hello World'
        self.writeln(helloStr[4])

        # String is immutable
        # helloStr[2] = 'p'
        
        # From int to String:
        myInt = 1234567890
        self.writeln(myInt)
        myStr = str(myInt)
        self.writeln(myStr, len(myStr))
        
        # From String to int:
        myStr = "123"
        myInt = int(myStr)
        self.writeln(type(myInt))
        
        myFloat = 3.4
        self.writeln(type(myFloat))
        self.writeln(type(myStr))
        
        self.writeln('myStr is myInt: ' + str((myStr is myInt)))
        self.writeln('myStr == myInt: ' + str((myStr == myInt)))
        self.writeln('myStr == str(myInt): ' + str((myStr == str(myInt))))

        # Strings quoting
        aString = "Hello World" # identical to aString = 'Hello World'
        self.writeln(aString)
        aString = "Hello \\ World \\ How \\ are \\ things?"
        self.writeln(aString)  
        aString = r"Hello \ World \ How \ are \ things?"
        self.writeln(aString)  

        # Long string spreading several lines
        
        # approach 1: triple quoting (note that the end of line is added to the string!)
        aLongString = """Lorem ipsum dolor sit amet, consectetur adipisicing elit, 
sed do eiusmod tempor incididunt ut labore et dolore magna ..."""
        self.writeln(aLongString)

        # approach 2: join strings - Disadvantage: requires newline escaping
        aLongString = "aliqua. Ut enim ad minim veniam, quis nostrud exercitation " + \
                      "ullamco laboris nisi ut aliquip ex ea commodo consequat..."
        self.writeln(aLongString)

        # approach 3: use parentheses (preferred approach)
        aLongString = ("Duis {0} aute irure dolor in reprehenderit in voluptate velit "
                       "esse cillum dolore eu fugiat nulla pariatur. Excepteur sint".format("TEST") )
        self.writeln(aLongString)

        # Using UNICODE characters - note: make sure that the editor is treating this
        # file with UTF-8 encoding! Starting with Python 3.0, UTF-8 is the default encoding.
        # Note that the font also needs to support the corresponding glyphs to render the characters
        # form the following string: 
        unicodeString = "\u20AC€\U000020AC\N{tilde}\u2116\u2107\u20A9\u20AF" + chr(0x23B7)
        self.writeln("Euro=%s (len=%s)" % (unicodeString, len(unicodeString)) )
        for c in unicodeString:
            self.writeln(hex(ord(c)))

        s = "Hello World"
        self.writeln("\"Hello World\"[4:]  = %s" % s[4:])
        self.writeln("\"Hello World\"[4:7] = %s" % s[4:7])
        self.writeln("\"Hello World\"[0::2]= %s" % s[0::2])
        self.writeln("\"Hello World\"[-4:]= %s" % s[-4:])
        self.writeln("\"Hello World\"[::-2]= %s" % s[::-2])
        self.writeln("\"Hello World\"[::-1]= %s" % s[::-1])
        self.writeln("\"Hello World\" * 3 = %s" % (s * 3) )
        l = ["A", "B", "C"]
        self.writeln('l = %s; ":".join(l) = %s' % (l, ":".join(l)) )


    def formatSample(self):
        s = "Hello World"

        # note that the % operator has been deprecated with Python 3.1.
        # Use the format method from String instead (http://www.python.org/dev/peps/pep-3101/)
        a = 4
        b = 5
        self.writeln("{0} + {1} = {2}".format(a, b, a+b))
        self.writeln("{a} + {b} = {c}".format(a=a, b=b, c=a+b))

        self.writeln("{a[4]}".format(a=s))
        self.writeln("\N{GREEK SMALL LETTER PI} = {0.pi}".format(math))

        self.writeln("Locals: " + str(locals()))

        values = {'s1':a, 's2':b, 's':a+b}
        self.writeln("{s1} + {s2} = {s}".format(**values) )

        # Format conversions
        s = "翻訳で失われる"
        self.writeln("{0} - {0!r} - {0!a}".format(s) )

        self.writeln("Default: |{0}|".format(s) )
        self.writeln("':25'  : |{0:25}|".format(s) )
        self.writeln("':>25' : |{0:>25}|".format(s) )
        self.writeln("':.^25': |{0:.^25}|".format(s) )
        for i in range(len(s), len(s) + 5):
            self.writeln("':.^{1}': |{0:.^{1}}|".format(s, i) )

        f = math.pi
        self.writeln("{0:.3}".format(f))    # Note: without "f", the overall number of digits is counted
        self.writeln("{0:.5}".format(f))
        f = 123.456
        self.writeln("{0:.3f}".format(f))   # three digits after decimal place
        self.writeln("{0:.5f}".format(f))   # Five digits after decimal place


    def printUnicode(self, startWith):
        widths = [5, 6, 4, 25]
        header = "{0:^{1}} {2:^{3}} {4:^{5}} {6:^{7}}".format("Dec", widths[0], "Hex", widths[1], "Char", widths[2], "Name", widths[3])
        self.writeln(header)
        self.writeln("-" * len(header))

        for codePoint in range(startWith, startWith + 16):
            unicodeName = unicodedata.name(chr(codePoint), "(Unknown)")
            #charName = string.capwords(unicodeName)
            charName = unicodeName.title()
            self.writeln("{0:>{1}} 0x{0:<{2}x} {3:^{4}} {5}".format(
                        codePoint, widths[0], widths[1] - 2, 
                        chr(codePoint), widths[2], 
                        charName ))

    # Converts a "bytes" object into a readable string
    def bytes2Hex(self, byteStr):
        return " ".join("{0:2x}".format(d) for d in byteStr)


    def str2ord(self, s):
        return " ".join("{0:x}".format(ord(c)) for c in s)


    def unicodeSample(self):
        self.writeln("Highest UNICODE code point: 0x{0:x}".format(sys.maxunicode))
        self.printUnicode(0x2722)

        s = "翻る"                   # String in UCS-2 or UCS-4 encoding
        b = s.encode()              # get UTF-8 encoding of the string
        self.writeln("{0} ({1}) to UTF-8: {2}".format(s, self.str2ord(s), self.bytes2Hex(b)))

        s = "A"
        b = s.encode()
        self.writeln("{0} ({1}) to UTF-8: {2}".format(s, self.str2ord(s), self.bytes2Hex(b)))

        s = "€"
        b = s.encode()
        self.writeln("{0} ({1}) to UTF-8: {2}".format(s, self.str2ord(s), self.bytes2Hex(b)))


    def localeSample(self):        
        x, y= (1234567890, 1234.56)

        defaultLocale = locale.getdefaultlocale()
        self.writeln("Default locale: " + str(defaultLocale))

        d= "Standard locale: {0:n} {1:n}".format(x,y)
        self.writeln(d)

        locale.setlocale(locale.LC_ALL, "C")
        c= "'C' locale: {0:n} {1:n}".format(x,y)    #c== "1234567890 1234.56"
        self.writeln(c)

        #locale.setlocale(locale.LC_ALL, "en_US.UTF-8")
        #en= "{0:n} {1:n}".format(x, y) #en== "1,234,567,890 1,234.56"
        #self.writeln(en)
        #locale.setlocale(locale.LC_ALL, "de_DE.UTF-8")

        # Note: the locale names are system dependant. See http://msdn.microsoft.com/en-us/library/39cwe7zf%28vs.71%29.aspx
        # for the Windows names, and "locale -a" for the ones installed on a linux system 
        locale.setlocale(locale.LC_ALL, "deu_deu")
        de= "'deu_deu' locale: {0:n} {1:n}".format(x, y) #de== "1.234.567.890 1.234,56"
        self.writeln(de)
    
        # On Windows, not even the locales returned by locale.getdefaultlocale() can be set ...
        #locale.setlocale(locale.LC_ALL, "de_DE")    # locale.Error: unsupported locale setting
        #de2 = "'de_DE' locale: {0:n} {1:n}".format(x, y) #de== "1.234.567.890 1.234,56"
        #self.writeln(de2)


    def tupleSample(self):
        myTuple = ("Hello", "World", 42)
        self.writeln("myTuple = {0}".format(myTuple))

        a = 5
        b = 7
        self.writeln("a = {0}, b = {1}".format(a, b))
        a, b = (b, a)
        self.writeln("a = {0}, b = {1}".format(a, b))
        Address = collections.namedtuple("Address", ("Firstname", "Lastname", "Street", "City"))
        
        myAddr = Address("Max", "Mustermann", "Musterstr.", "Musterstadt")
        self.writeln(myAddr)
        self.writeln("Firstname: {}".format(myAddr.Firstname))
        self.writeln("Lastname: {}".format(myAddr.Lastname))
        self.writeln("Name: {Firstname} {Lastname}".format(**myAddr._asdict() ))


    def listSample(self):
        # List
        myList = [3, 5, 7]
        self.writeln(myList, len(myList), type(myList))
        self.writeln(myList[2])
        # Lists are mutable:
        myList[2] = 10
        self.writeln("myList = {0}".format(myList))
        self.writeln("myList * 3 = {0}".format(myList * 3) )

        *part, last = myList
        self.writeln("part: {}, last: {}".format(part, last) )

        # Replace a whole slice with a new list
        myList[0:2] = [42]
        self.writeln("myList = {0}".format(myList))


    def setSample(self):
        pass


    def dictSample(self):
        pass


def main():
    
    # Create a Qt application
    app = QApplication(sys.argv)
    
    # Create and whow the main window
    wnd = MainWindow()
    wnd.show()
    
    # Run the application
    app.exec_()
    sys.exit()



main()
