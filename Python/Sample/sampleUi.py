#!/usr/bin/python

import sys

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
        self.addSample("C", lambda : None )
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
        self.writeln(myList)


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


# Create a Qt application
app = QApplication(sys.argv)

# Create and whow the main window
wnd = MainWindow()
wnd.show()

# Run the application
app.exec_()
sys.exit()
