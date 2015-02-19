

import xml.sax

from PyQt5.QtGui import QTextDocument, QTextBlockFormat, QTextListFormat, QTextDocumentFragment 
from PyQt5.QtGui import QTextCursor, QTextBlockUserData, QFont
from PyQt5.Qt import QTextCharFormat, Qt, QColor

class UserData(QTextBlockUserData):
    
    def __init__(self, data):
        QTextBlockUserData.__init__(self)

        self.data = data
        
    def getData(self):
        return self.data

class Handler(xml.sax.handler.ContentHandler):

    def __init__(self):
        self.state = 0
        self.content = ""

        self.h1BlockFmt = QTextBlockFormat()
        self.h1BlockFmt.setTopMargin(18)
        self.h1BlockFmt.setBottomMargin(5)
        #self.h1BlockFmt.setBackground(Qt.red)
        self.h1CharFmt = QTextCharFormat()
        self.h1CharFmt.setFontPointSize(18)
        #self.h1CharFmt.setForeground(Qt.yellow)
        #self.h1CharFmt.setBackground(Qt.blue)

        self.h2BlockFmt = QTextBlockFormat()
        self.h2BlockFmt.setTopMargin(14)
        self.h2BlockFmt.setBottomMargin(3)
        self.h2CharFmt = QTextCharFormat()
        self.h2CharFmt.setFontPointSize(16)

        self.h3BlockFmt = QTextBlockFormat()
        self.h3BlockFmt.setTopMargin(10)
        self.h3BlockFmt.setBottomMargin(3)
        self.h3CharFmt = QTextCharFormat()
        self.h3CharFmt.setFontPointSize(14)

        self.pBlockFmt = QTextBlockFormat()
        self.pCharFmt = QTextCharFormat()
        self.pCharFmt.setFontPointSize(10)
        self.pCharFmt.setFontFamily("Sans")
        self.emCharFmt = QTextCharFormat(self.pCharFmt)
        self.emCharFmt.setFontWeight(QFont.Bold)

        self.pCodeBlockFmt = QTextBlockFormat()
        self.pCodeBlockFmt.setTopMargin(5)
        self.pCodeBlockFmt.setLeftMargin(5)
        self.pCodeBlockFmt.setRightMargin(5)
        self.pCodeBlockFmt.setBottomMargin(5)
        self.pCodeBlockFmt.setBackground(QColor(240, 240, 240))
        self.pCodeCharFmt = QTextCharFormat()
        self.pCodeCharFmt.setForeground(Qt.black)
        self.pCodeCharFmt.setFontFamily("Courier")
        self.pCodeCharFmt.setFontPointSize(10)

        self.ul1Fmt = QTextListFormat()
        self.ul1Fmt.setStyle(QTextListFormat.ListDisc)
        self.ul1Fmt.setIndent(1)

        self.ul2Fmt = QTextListFormat()
        self.ul2Fmt.setStyle(QTextListFormat.ListCircle)
        self.ul2Fmt.setIndent(2)

        self.ul3Fmt = QTextListFormat()
        self.ul3Fmt.setStyle(QTextListFormat.ListSquare)
        self.ul3Fmt.setIndent(3)

        self.ol1Fmt = QTextListFormat()
        self.ol1Fmt.setStyle(QTextListFormat.ListDecimal)
        self.ol1Fmt.setIndent(1)

        self.ol2Fmt = QTextListFormat()
        self.ol2Fmt.setStyle(QTextListFormat.ListDecimal)
        self.ol2Fmt.setIndent(2)

        self.ol3Fmt = QTextListFormat()
        self.ol3Fmt.setStyle(QTextListFormat.ListDecimal)
        self.ol3Fmt.setIndent(3)

        #self.codeFrameFmt = QTextFrameFormat()
        #self.codeFrameFmt.setBorder(1.0)
        #self.codeFrameFmt.setBorderStyle(QTextFrameFormat.BorderStyle_Dotted)    
        #self.codeFrameFmt.setBorderBrush(Qt.darkGray)
        #self.codeFrameFmt.setBackground(Qt.lightGray)
        #self.codeFrameFmt.setMargin(5)
        #self.codeFrameFmt.setPadding(5)

        # Note: an empty, newly created QTextDocument() always contains one 
        # initial block. Furthermore, this initial block can not be
        # removed when the next document element is a Frame - additionally,
        # it seems that there is *always* a block before each frame!
        # Hence, for now, we are inserting code with a normal block element.
        self.result = QTextDocument()
        self.result.setIndentWidth(20)
        self.cursor = QTextCursor(self.result)

        # Sample / test
        #self.cursor.insertBlock(self.pCodeBlockFmt, self.pCodeCharFmt)
        #self.cursor.insertText("Frame 1")
        #self.cursor.insertBlock(self.pCodeBlockFmt, self.pCodeCharFmt)
        #self.cursor.insertText("Frame 2")

        # First block
        #self.cursor.movePosition(QTextCursor.Start)

        #self.cursor.insertBlock(self.h1BlockFmt, self.h1CharFmt)
        #self.cursor.insertText("Header")
        #self.cursor.setBlockCharFormat(self.h1CharFmt)
        #self.cursor.setBlockFormat(self.h1BlockFmt)
        #self.cursor.insertText("Header")

        # Next block
        #self.cursor.insertBlock(self.pBlockFmt, self.pCharFmt)
        #self.cursor.setBlockCharFormat(self.pCharFmt)
        #self.cursor.setBlockFormat(self.pBlockFmt)
        #self.cursor.insertText("Para 1")
        #self.cursor.insertText("Para 2")
        #self.cursor.insertText("Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy ei\n")

        #self.cursor.insertBlock(self.h1BlockFmt, self.h1CharFmt)
        #self.cursor.setBlockCharFormat(self.h1CharFmt)
        #self.cursor.setBlockFormat(self.h1BlockFmt)
        #self.cursor.insertText("Header 2\n")

        # List test
        #self.cursor.insertText("Before List")

        #self.cursor.insertList(self.ul1Fmt)
        #self.cursor.insertText("Item1")
        #self.cursor.insertBlock()
        #self.cursor.insertText("Item2")

        #self.cursor.insertList(self.ul2Fmt)
        #self.cursor.insertText("Item1")
        #self.cursor.insertBlock()
        #self.cursor.insertText("Item3")

        #self.cursor.insertBlock(self.pBlockFmt, self.pCharFmt)
        #self.cursor.insertText("After List")

        # Clean up document
        #self.cursor.movePosition(QTextCursor.Start)
        #self.cursor.select(QTextCursor.BlockUnderCursor)
        #self.cursor.deleteChar()

        self.firstLi = False

    def startElement(self, name, attrs):
        if name == "page":
            self.state = 1
        elif name == "h1":
            self.state = 2
        elif name == "h2":
            self.state = 3
        elif name == "h3":
            self.state = 4
        elif name == "p":
            self.state = 5
            self.insertBlock("", self.pBlockFmt, self.pCharFmt, "p")
        elif name == "em":
            self.insertFragment(self.content, self.pCharFmt)
            self.content = ""
            self.state = 6
        elif name == "code":
            self.state = 7
        elif name == "ul":
            self.state = 8
            self.firstLi = True
        elif name == "li":
            self.state = 9
        else:
            print("INVALID TAG:" + name)


    def characters(self, data):
        topState = self.state
        # print("STATE:" + str(self.state) + "/" + str(topState) + "/" + data)
        if (topState > 1 and topState < 10) and topState != 8:
            self.content = self.content + data 
        else:
            pass
            #if len(data) > 0:
            #    print("Found data in unknown state:" + str(self.state) + "/" + data)


    def endElement(self, name):
        topState = self.state
        if name == "page": # and topState != 0:
            if not self.result.isEmpty():
                self.cursor.movePosition(QTextCursor.Start)
                b = self.cursor.block()
                if b.length() == 1:
                    cursor = QTextCursor(self.result.findBlockByLineNumber(0))
                    cursor.select(QTextCursor.BlockUnderCursor)
                    cursor.deleteChar()
        elif name == "h1" and topState == 2:
            self.insertBlock(self.content, self.h1BlockFmt, self.h1CharFmt, "h1")
            self.state = 0
        elif name == "h2" and topState == 3:
            self.insertBlock(self.content, self.h2BlockFmt, self.h2CharFmt, "h2")
            self.state = 0
        elif name == "h3" and topState == 4:
            self.insertBlock(self.content, self.h3BlockFmt, self.h3CharFmt, "h3")
            self.state = 0
        elif name == "p" and topState == 5:
            self.insertFragment(self.content, self.pCharFmt)
            self.state = 0
        elif name == "code" and topState == 7:
            self.content = self.content.replace('\n', '\u2028')
            self.insertBlock(self.content, self.pCodeBlockFmt, self.pCodeCharFmt, "code")
            # self.insertFrame(self.content, self.codeFrameFmt)
            self.state = 0
        elif name == "em" and topState == 6:
            self.insertFragment(self.content, self.emCharFmt)
            self.state = 5
        elif name == "ul" and topState == 10:
            self.state = 0
        elif name == "li": # and topState == 9:
            if self.firstLi:
                self.cursor.setBlockFormat(self.pBlockFmt)
                self.cursor.insertList(self.ul1Fmt)
                self.cursor.setBlockCharFormat(self.pCharFmt)
                print("FIRST:" + self.content)
                self.cursor.insertText(self.content)
                self.firstLi = False
                self.state = 0
            else:
                self.cursor.insertBlock()
                self.cursor.insertText(self.content)
                print("NEXT:" + self.content)
                self.state = 0
        else:
            print("INVALID END TAG/STATE combination:" + name + "/" + str(topState))
        self.content = ""


    #def insertFrame(self, content, frameFmt):
    #    self.cursor.movePosition(QTextCursor.Start)
    #    self.cursor.movePosition(QTextCursor.StartOfBlock)
    #    self.cursor.select(QTextCursor.BlockUnderCursor)
    #    self.cursor.insertFrame(frameFmt)
    #    self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))



    def insertBlock(self, content, blockFmt, charFmt, className):
        self.cursor.insertBlock(blockFmt, charFmt)
        self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))
        self.cursor.block().setUserData(UserData(className))


    def insertFragment(self, content, fragFmt):
        #self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))
        self.cursor.insertText(content, fragFmt)


class XMLImporter:

    def __init__(self, fileHandle):
        self.fileHandle = fileHandle


    def importDocument(self):
        parser = xml.sax.make_parser()
        handler = Handler()
        parser.setContentHandler(handler)
        parser.parse(self.fileHandle)

        return handler.result



