
import os
import xml.sax

from PyQt5.QtGui import QTextDocument, QTextDocumentFragment 
from PyQt5.QtGui import QTextCursor, QTextImageFormat


class Handler(xml.sax.handler.ContentHandler):

    def __init__(self, contentPath, formatManager):
        self.collectContent = False
        self.content = ""
        self.contentPath = contentPath
        self.href = ""
        self.formatManager = formatManager
        self.keywordLinks = []

        # Note: an empty, newly created QTextDocument() always contains one 
        # initial block. Furthermore, this initial block can not be
        # removed when the next document element is a Frame - additionally,
        # it seems that there is *always* a block before each frame!
        # Hence, for now, we are inserting code with a normal block element.
        self.result = QTextDocument()
        self.result.setUndoRedoEnabled(False)
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
#===============================================================================
#         self.cursor.insertText("Before List")
# 
#         self.cursor.insertList(self.formatManager.getFormat("ul1").getListFormat())
#         self.cursor.insertText("Item1")
#         self.cursor.insertBlock()
#         self.cursor.insertText("Item2")
# 
#         self.cursor.insertList(self.formatManager.getFormat("ul2").getListFormat())
#         self.cursor.insertText("Item1")
#         self.cursor.insertBlock()
#         self.cursor.insertText("Item2")
# 
#         self.cursor.insertBlock(self.formatManager.getFormat("p").getBlockFormat(), self.formatManager.getFormat("p").getCharFormat())
#         self.cursor.insertText("After List")
#===============================================================================

        # Clean up document
        #self.cursor.movePosition(QTextCursor.Start)
        #self.cursor.select(QTextCursor.BlockUnderCursor)
        #self.cursor.deleteChar()

        self.firstLi = False

    def startElement(self, name, attrs):
        if name == "page":
            pass

        # blocks
        elif name in ['h1', 'h2', 'h3', 'p', 'javacode', 'cppcode', 'sqlcode', 'pycode', 'xmlcode', 'pre']:
            self.collectContent = True
            self.insertBlock('', name)
        elif name == "ul":
            self.firstLi = True
        elif name == "li":
            self.collectContent = True
            if self.firstLi:
                self.cursor.insertBlock(self.formatManager.getFormat("p").getBlockFormat(), self.formatManager.getFormat("p").getCharFormat())
                self.cursor.createList(self.formatManager.getFormat("ul1").getListFormat())
            else:
                fmt = self.formatManager.getFormat("p")
                self.cursor.insertBlock()
                self.cursor.setCharFormat(fmt.getCharFormat())

        # Fragments
        elif name in ['em', 'strong', 'tt', 'keyword']:
            # insert previous fragment
            self.insertFragment(self.content, "p")
            self.content = ""
        elif name == "a":
            # insert previous fragment
            self.insertFragment(self.content, "p")
            self.content = ""
            self.href = attrs.getValue("href")
        elif name == "img":
            self.insertImage(attrs)

        else:
            print("INVALID TAG:" + name)


    def characters(self, data):
        if self.collectContent:
            self.content = self.content + data 


    def endElement(self, name):
        if name == "page":
            if not self.result.isEmpty():
                self.cursor.movePosition(QTextCursor.Start)
                b = self.cursor.block()
                if b.length() == 1:
                    cursor = QTextCursor(self.result.findBlockByLineNumber(0))
                    cursor.select(QTextCursor.BlockUnderCursor)
                    cursor.deleteChar()

        # Blocks
        elif name in ['h1', 'h2', 'h3', 'p', 'javacode', 'cppcode', 'sqlcode', 'pycode', 'xmlcode', 'pre']:
            self.collectContent = False
            self.insertFragment(self.content, name)
        elif name == "ul":
            self.collectContent = False
        elif name == "li":
            if self.firstLi:
                self.insertFragment(self.content, 'p')
                self.firstLi = False
                self.collectContent = False
            else:
                self.insertFragment(self.content, 'p')
                self.collectContent = False

        # Fragments
        elif name in ['em', 'strong', 'tt']:
            self.insertFragment(self.content, name)
        elif name == 'a':
            # Insert the link text
            # if self.href.startswith("http://") or self.href.startswith("https://"):
            self.insertAnchor(self.content, "a", self.href)
        elif name == 'keyword':
            self.insertAnchor(self.content, "keyword", self.content)
            self.keywordLinks.append(self.content)

        elif name == "img":
            pass
        else:
            print("INVALID TAG: " + name)
        self.content = ""

    #def insertFrame(self, content, frameFmt):
    #    self.cursor.movePosition(QTextCursor.Start)
    #    self.cursor.movePosition(QTextCursor.StartOfBlock)
    #    self.cursor.select(QTextCursor.BlockUnderCursor)
    #    self.cursor.insertFrame(frameFmt)
    #    self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))


    def insertBlock(self, content, className):
        fmt = self.formatManager.getFormat(className)
        self.cursor.insertBlock(fmt.getBlockFormat(), fmt.getCharFormat())
        self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))


    def insertFragment(self, content, className):
        fmt = self.formatManager.getFormat(className)
        #self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))
        
        content = content.replace('\n', '\u2028')
        
        self.cursor.insertText(content, fmt.getCharFormat())


    def insertAnchor(self, content, className, href):
        fmt = self.formatManager.getFormat(className)
        charFmt = fmt.getCharFormat()
        charFmt.setAnchorHref(href)

        #self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))
        self.cursor.insertText(content, charFmt)

                                  
    def insertImage(self, attrs):
        imageFile = attrs.getValue("src")
        imagePath = os.path.join(self.contentPath, imageFile)
        imageFmt = QTextImageFormat()
        imageFmt.setName(imagePath)
        #self.cursor.insertBlock()
        self.cursor.insertImage(imageFmt)


class XMLImporter:

    def __init__(self, contentPath, contentFile, formatManager):
        self.contentPath = contentPath
        self.contentFile = contentFile
        self.formatManager = formatManager


    def importDocument(self):
        contentFilePath = os.path.join(self.contentPath, self.contentFile)
        with open(contentFilePath, 'r') as content_file:
            parser = xml.sax.make_parser()
            handler = Handler(self.contentPath, self.formatManager)
            parser.setContentHandler(handler)
            parser.parse(content_file)

        self.document = handler.result
        self.links = handler.keywordLinks


    def getDocument(self):
        return self.document
    

    def getLinks(self):
        return self.links
