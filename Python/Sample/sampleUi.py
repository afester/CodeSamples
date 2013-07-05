#!/usr/bin/python

import sys
import decimal

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
        self.addSample("List sample", self.listSample)
        self.addSample("Calculations", self.calculationsSample)
        self.addSample("Strings", self.stringSample)
        self.newRow()
        self.addSample("D", lambda : None)

    
    def addSample(self, label, function):
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


    def listSample(self):
        # List
        myList = [3, 5, 7]
        self.writeln(myList, len(myList), type(myList))
        self.writeln(myList[2])
        # Lists are mutable:
        myList[2] = 10
        self.writeln("myList = %s" % myList)
        self.writeln("myList * 3 = %s" % (myList * 3) )


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
        aLongString = ("Duis %s aute irure dolor in reprehenderit in voluptate velit "
                       "esse cillum dolore eu fugiat nulla pariatur. Excepteur sint" % "TEST")
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
        
        # note that the % operator has been deprecated with Python 3.1.
        # Use the format method from String instead (http://www.python.org/dev/peps/pep-3101/)
        a = 4
        b = 5
        self.writeln("{0} + {1} = {2}".format(a, b, a+b))


# Create a Qt application
app = QApplication(sys.argv)

# Create and whow the main window
wnd = MainWindow()
wnd.show()

# Run the application
app.exec_()
sys.exit()
