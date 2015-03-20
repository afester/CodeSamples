#!/usr/bin/python

import sys

from PyQt5.QtCore import Qt, QMimeData
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QTextEdit
from PyQt5.QtWidgets import QPushButton, QVBoxLayout, QGridLayout, QFileDialog
from PyQt5.QtGui import QTextCursor, QPainter


class HexEditWidget(QTextEdit):
    def __init__(self, parent):
        QTextEdit.__init__(self, parent)

        self.setCursorWidth(8)
        self.setLineWrapMode(QTextEdit.NoWrap);

        # set the font
        font = self.font()
        font.setFamily("Courier")
        font.setPointSize(10)
        self.setFont(font)

        self.bytesPerLine = 16
        self.addressWidth = 10

        self.addrColor = Qt.red
        self.addrBackground = Qt.white
        self.dataColor = Qt.black
        self.dataBackground = Qt.white
        self.asciiColor = Qt.black
        self.asciiBackground = Qt.lightGray

        self.cursorPositionChanged.connect(self.showPosition)


    def paintEvent(self, event):
        QTextEdit.paintEvent(self, event)

        viewPort = self.viewport()

        cursor = self.textCursor()
        columnNumber = cursor.positionInBlock()
        rect = self.cursorRect()

        p = QPainter(viewPort)

        byteIndex = (columnNumber - 10) // 3

        # 4 pixels offset to the left, 58 columns to the ascii area
        p.setPen(Qt.red)
        p.drawRect( 4 + (58 + byteIndex) * rect.size().width(),
                   #rect.left() + 384, 
                   rect.top(),# columnNumber * (rect.size().width()), 
                    rect.size().width() - 1, rect.size().height())


    def keyPressEvent(self, event):
        # Ensure that shortcuts like CTRL-C are working
        if event.modifiers() & Qt.ControlModifier:
            if event.key() == Qt.Key_C:
                QTextEdit.keyPressEvent(self, event)

        else:
            # Filter keys (TODO)
            text = event.text()
            if text:
                print(event.text())
            else:
                print("KEY: {}".format(event.key()))



    def keyReleaseEvent(self, event):
        pass


    def mousePressEvent(self, event):
        QTextEdit.mousePressEvent(self, event)

        cursor = self.cursorForPosition(event.pos())
        columnNumber = cursor.positionInBlock()

        # Limit cursor position to the editable area
        # - TODO: remove hard coded numbers
        if columnNumber < self.addressWidth:
            columnNumber = self.addressWidth
        if columnNumber > 73:
            columnNumber = 73;

        # Not all of the characters displayed in the hex dump area can be edited 
        if columnNumber < 58:
            bytePos = columnNumber - self.addressWidth
            byteIdx = bytePos % 3   # 0, 1, 2 => "7F "
            if byteIdx == 2:
                columnNumber -= 1

        block = cursor.block()      # The current block of the cursor
        cursor.setPosition(block.position() + columnNumber)

        self.setTextCursor(cursor)

        self.viewport().update()    # necessary to get the right coordinates in the paint event



    def getCurrentAddr(self, crsr):
        '''Returns the address of the byte where the cursor is located'''

        lineNumber = crsr.blockNumber()
        columnNumber = crsr.positionInBlock()
        if columnNumber < 58:   # data area
            addr = self.bytesPerLine * lineNumber + (columnNumber - self.addressWidth) // 3
        else:
            addr = self.bytesPerLine * lineNumber + (columnNumber - 58)
        return addr


    def getDataPositionForAddr(self, addr):
        '''returns the cursor position within the data area for the given address'''
        lineLength = self.addressWidth + 3* self.bytesPerLine + 16 + 1  # addr: data ascii\n
        blockNo = addr // self.bytesPerLine
        column = (addr % self.bytesPerLine) * 3 + self.addressWidth
        result = blockNo * lineLength + column
        return result


    def getByte(self, addr):
        crsr = self.textCursor()

        byteStart = self.getDataPositionForAddr(addr)
        byteEnd = byteStart + 2
        crsr.setPosition(byteStart)
        crsr.setPosition(byteEnd, QTextCursor.KeepAnchor)
        data = int(crsr.selectedText(), 16)
        return data


    def showPosition(self):
        '''cursorPositionChanged() slot'''

        crsr = self.textCursor()
        addr = self.getCurrentAddr(crsr)
        data = self.getByte(addr)

        # TODO: result be invalid if improper data is available
        if (data & 0b11111110) == 0b11111100:
            count = 6
            unicode = (data & 0b00000001)
        elif (data & 0b11111100) == 0b11111000:
            count = 5
            unicode = (data & 0b00000011)
        elif (data & 0b11111000) == 0b11110000:
            count = 4
            unicode = (data & 0b00000111)
        elif (data & 0b11110000) == 0b11100000:
            count = 3
            unicode = (data & 0b00001111)
        elif (data & 0b11100000) == 0b11000000:
            count = 2
            unicode = (data & 0b00011111)
        elif (data & 0b10000000) == 0b00000000:
            count = 1
            unicode = data
        else:
            count = 0

        for x in range(1, count):
            nextByte = self.getByte(addr + x)

            # shift bits by 6 to get space for the next bunch of bits
            unicode = unicode * 0x40

            # add the next bunch of bits to the unicode value
            unicode = unicode + (nextByte & 0b00111111)

        if count == 0:
            print('Byte address: 0x{0:08X} = {1:02X} = \\u (N/A)'.format(addr, data))
        else:
            print('Byte address: 0x{0:08X} = {1:02X} = \\u{2:2X}'.format(addr, data, unicode))


    def addLine(self, addr, hexDump, asciiDump):
        self.textCursor().movePosition(QTextCursor.End)

        self.setTextColor(self.addrColor)
        self.setTextBackgroundColor(self.addrBackground)
        self.insertPlainText("{0:08X}: ".format(addr))

        self.setTextColor(self.dataColor)
        self.setTextBackgroundColor(self.dataBackground)
        self.insertPlainText("{0:48}".format(hexDump))

        self.setTextColor(self.asciiColor)
        self.setTextBackgroundColor(self.asciiBackground)
        self.insertPlainText("{0}\n".format(asciiDump))


    def setContents(self, contents):
        self.blockSignals(True)

        addr = 0
        hexDump = ""
        asciiDump = ""
        column = 0
        
        self.clear()
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

        self.blockSignals(False)

        crsr = self.textCursor()
        crsr.setPosition(10)
        self.setTextCursor(crsr)


class MainWindow(QMainWindow):
    def __init__(self):

        QMainWindow.__init__(self)
        self.resize(800, 600)

        # Create the main content widget
        mainWidget = QWidget(self)
        self.setCentralWidget(mainWidget)

        # Create a text component at the top area of the main widget
        self.output = HexEditWidget(mainWidget)

        mainLayout = QVBoxLayout(mainWidget)
        mainLayout.addWidget(self.output)

        # Create buttons in a grid layout below the top area
        buttonWidget = QWidget(mainWidget)
        self.buttonLayout = QGridLayout(buttonWidget)
        mainLayout.addWidget(buttonWidget)

        theButton = QPushButton("Load...")
        theButton.clicked.connect(self.chooseFile)
        self.buttonLayout.addWidget(theButton, 0, 0)

        statusBar = self.statusBar();

        self.loadFile("__pycache__/helloPyside.cpython-33.pyc")


    def chooseFile(self):
        fileName = QFileDialog.getOpenFileName(self, "Open file ...");
        fileName = fileName[0]
        if fileName:
            self.loadFile(fileName)


    def loadFile(self, fileName):
        file = open(fileName, 'rb')
        contents = file.read()
        file.close()
        self.output.setContents(contents)


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
