#!/usr/bin/python

import sys
import traceback

from PySide.QtCore import Qt
from PySide.QtGui import QApplication, QMainWindow, QTextEdit, QPushButton, QWidget
from PySide.QtGui import QVBoxLayout, QGridLayout, QPalette, QTextCursor

import samplePackage.SampleModule               # Import module "SampleModule" in package "samplePackage"
                                                # Use like "x = samplePackage.sampleModule.Examples()"

#from samplePackage.SampleModule import Examples # Import symbol "Examples" from module "sampleModule" in package "samplePackage"
                                                 # Use like "x = Examples()"


class MainWindow(QMainWindow):
    def __init__(self):

        # QMainWindow.__init__(self)
        super().__init__()      # use super() to avoid explicit dependency on the base class name
                                # Note: must not pass the self reference to __init__ in this case!
        self.resize(800, 600)

        # Create the main content widget
        mainWidget = QWidget(self)
        self.setCentralWidget(mainWidget)

        # Create a text component at the top area of the main widget
        self.output = QTextEdit(mainWidget)
        self.output.setReadOnly(True)
        self.output.setLineWrapMode(QTextEdit.NoWrap);

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

        # Add buttons for all the examples - attention: do not make "examples"
        # a local variable - otherwise, the Examples object would be destroyed
        # at the end of __init__ !!!
        self.examples = samplePackage.SampleModule.Examples(self)
        for example in self.examples.getExamples():
            if example is None:
                self.newRow()
            else:
                self.addButton(example.label, example.function)

        # In a Python program, sys.excepthook is called just before the program exits.
        # So we can catch all fatal, uncaught exceptions and log them.
        # NOTE: we must be sure not to set the excepthook BEFORE we an actually
        # log something!! 
        sys.excepthook = self.logException


    def logException(self, exctype, value, tb):
        self.writelnColor(Qt.red, 
                          ("\nFATAL ERROR: Uncaught exception\n"
                           "  {}: {}\n"
                           "{}\n".format(exctype.__name__, value, ''.join(traceback.format_tb(tb)))) )


    def addButton(self, label, function):
        theButton = QPushButton(label)
        theButton.clicked.connect(function)
        self.buttonLayout.addWidget(theButton, self.row, self.column)
        self.column += 1


    def newRow(self):
        self.row += 1
        self.column = 0


    def writeColor(self, color, *text):
        theText =  ' '.join(map(str, text))
        self.output.setTextColor(color)

        # Note: append() adds a new paragraph!
        #self.output.append(theText)
        self.output.textCursor().movePosition(QTextCursor.End)
        self.output.insertPlainText(theText)

        # scroll console window to bottom        
        sb = self.output.verticalScrollBar()
        sb.setValue(sb.maximum())


    def write(self, *text):
        self.writeColor(Qt.green, *text)


    def writelnColor(self, color, *text):
        self.writeColor(color, *text)
        self.write('\n')


    def writeln(self, *text):
        self.writelnColor(Qt.green, *text)


def main():
    
    # Create a Qt application
    app = QApplication(sys.argv)

    # Create and show the main window
    wnd = MainWindow()
    wnd.show()
    
    # Run the application
    app.exec_()
    sys.exit()


if __name__ == "__main__":
    main()
