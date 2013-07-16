#!/usr/bin/python

import sys

from PySide.QtCore import Qt 
from PySide.QtGui import QApplication, QMainWindow, QWidget, QTextEdit, QPalette
from PySide.QtGui import QPushButton, QVBoxLayout, QGridLayout, QFileDialog, QTextCursor


class HexEditWidget(QTextEdit):
    def __init__(self, parent):
        QTextEdit.__init__(self, parent)

    def keyPressEvent(self, event):
        text = event.text()
        if text:
            print(event.text())
        else:
            print("KEY: {}".format(event.key()))


    def keyReleaseEvent(self, event):
        pass


class MainWindow(QMainWindow):
    def __init__(self):

        QMainWindow.__init__(self)
        self.resize(800, 600)

        # Create the main content widget
        mainWidget = QWidget(self)
        self.setCentralWidget(mainWidget)

        # Create a text component at the top area of the main widget
        self.output = HexEditWidget(mainWidget)
        self.output.setCursorWidth(8)
        self.output.setLineWrapMode(QTextEdit.NoWrap);

        # set the font
        font = self.output.font()
        font.setFamily("Courier")
        font.setPointSize(10)
        self.output.setFont(font)

        # Set the background color
        pal = self.output.palette()
        pal.setColor(QPalette.Base, Qt.lightGray)
        self.output.setPalette(pal)

        mainLayout = QVBoxLayout(mainWidget)
        mainLayout.addWidget(self.output)

        # Create buttons in a grid layout below the top area
        buttonWidget = QWidget(mainWidget)
        self.buttonLayout = QGridLayout(buttonWidget)
        mainLayout.addWidget(buttonWidget)

        theButton = QPushButton("Load...")
        theButton.clicked.connect(self.loadFile)
        self.buttonLayout.addWidget(theButton, 0, 0)

        statusBar = self.statusBar();


    def addLine(self, addr, hexDump, asciiDump):
        self.output.textCursor().movePosition(QTextCursor.End)

        self.output.setTextColor(Qt.red)
        self.output.insertPlainText("{0:08X}: ".format(addr))
        self.output.setTextColor(Qt.white)
        self.output.insertPlainText("{0:48}".format(hexDump))
        self.output.setTextColor(Qt.black)
        self.output.insertPlainText("|{0}\n".format(asciiDump))


    def loadFile(self):
        fileName = QFileDialog.getOpenFileName(self, "Open file ...");
        fileName = fileName[0]
        if fileName:
            file = open(fileName, 'rb')
            contents = file.read()
            file.close()

            addr = 0
            hexDump = ""
            asciiDump = ""
            column = 0
            for c in contents:
                hexDump = hexDump + "{0:02X} ".format(c)
                asciiDump = asciiDump + ( chr(c) if c > 31 and c < 128 else '.')
                column += 1
                if (column % 16) == 0:
                    self.addLine(addr, hexDump, asciiDump)

                    asciiDump = ""
                    hexDump = ""
                    column = 0
                    addr += 16
            if column > 0:
                self.addLine(addr, hexDump, asciiDump)


class HexDump():
    def __init__(self):
        pass

    def main(self, args):
        # Create a Qt application
        app = QApplication(sys.argv)

        # Create and whow the main window
        wnd = MainWindow()
        wnd.show()
        
        # Run the application
        app.exec_()


if __name__ == "__main__":
    app = HexDump()
    app.main(sys.argv)
