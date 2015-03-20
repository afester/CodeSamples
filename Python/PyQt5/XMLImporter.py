
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
        self.keywordLinks = set()
        self.listLevel = 0
        self.sectionLevel = 0

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
        if name == "article":
            pass

        # blocks
        elif name == 'section':
            self.sectionLevel += 1

        elif name == 'title':
            if self.sectionLevel > 0:   # ignore article title
                style = ('title', 'level', str(self.sectionLevel))
                self.collectContent = True
                self.insertBlock('', style)

        elif name == 'para':
            self.collectContent = True
            if self.listLevel == 0:
                self.insertBlock('', ('para', None, None))

        elif name == 'programlisting':
            self.codeFormat =  ('programlisting', 'language', attrs.getValue('language')) 
            self.collectContent = True
            self.insertBlock('', self.codeFormat)

        elif name == 'screen':
            self.collectContent = True
            self.insertBlock('', ('screen', None, None))

        elif name == "itemizedlist":
            self.listLevel += 1
#===============================================================================
#             self.firstLi = True
# 
#             listFormat = ('itemizedlist', 'level', str(len(self.lists) + 1))
#             self.cursor.insertBlock(self.formatManager.getFormat( ('para', None, None) ).getBlockFormat(), self.formatManager.getFormat( ('para', None, None) ).getCharFormat())
#             newList = self.cursor.createList(self.formatManager.getFormat(listFormat).getListFormat())
#             self.lists.append(newList)
#===============================================================================

        elif name == "listitem":
            listFormat = ('itemizedlist', 'level', str(self.listLevel))
            self.cursor.insertBlock(self.formatManager.getFormat( ('para', None, None) ).getBlockFormat(), self.formatManager.getFormat( ('para', None, None) ).getCharFormat())
            newList = self.cursor.createList(self.formatManager.getFormat(listFormat).getListFormat())

            #===================================================================
            # self.collectContent = True
            # if self.firstLi:
            #     self.firstLi = False
            # else:
            #     fmt = self.formatManager.getFormat( ('para', None, None) )
            #     self.cursor.insertBlock()
            #     self.lists[-1].add(self.cursor.block())
            #     self.cursor.setCharFormat(fmt.getCharFormat())
            #===================================================================

        # Fragments
        elif name == 'emphasis':
            # insert previous fragment
            self.emphasizeRole = attrs.get('role', '')
            self.insertFragment(self.content, ('para', None, None))
            self.content = ""

        elif name == 'code':
            # insert previous fragment
            self.insertFragment(self.content, ('para', None, None))
            self.content = ""

        elif name == 'olink':    # internal link / keyword
            self.insertFragment(self.content, ('para', None, None))
            self.content = ""

        elif name == 'link':   # external link / URL
            # insert previous fragment
            self.insertFragment(self.content, ('para', None, None))
            self.content = ""
            self.href = attrs.get('xlink:href', '')

        elif name in ['mediaobject', 'imageobject']:
            pass

        elif name == "imagedata":
            self.insertImage(attrs)

        else:
            print("INVALID TAG:" + name)


    def characters(self, data):
        if self.collectContent:
            self.content = self.content + data 


    def endElement(self, name):
        if name == "article":
            if not self.result.isEmpty():
                self.cursor.movePosition(QTextCursor.Start)
                b = self.cursor.block()
                if b.length() == 1:
                    cursor = QTextCursor(self.result.findBlockByLineNumber(0))
                    cursor.select(QTextCursor.BlockUnderCursor)
                    cursor.deleteChar()

        # Blocks
        elif name == 'section':
            self.sectionLevel -= 1

        elif name == 'title':
            if self.sectionLevel > 0:   # ignore article title
                style = ('title', 'level', str(self.sectionLevel))
                self.collectContent = False
                self.insertFragment(self.content, style)

        elif name == 'para':
            self.collectContent = False
            self.insertFragment(self.content, ('para', None, None))

        elif name == 'screen':
            self.collectContent = False
            self.insertFragment(self.content, ('screen', None, None))

        elif name == 'programlisting':
            self.collectContent = False
            self.insertFragment(self.content, self.codeFormat)

        elif name == "itemizedlist":
            self.listLevel -= 1
            #self.collectContent = False
            #self.lists.pop()

        elif name == "listitem":
            pass
            #self.insertFragment(self.content, ('para', None, None))
            #self.collectContent = False

        # Fragments
        elif name == 'code':
            self.insertFragment(self.content, ('code', None, None))

        elif name == 'emphasis':
            if self.emphasizeRole == 'highlight':
                self.insertFragment(self.content, ('emphasis', 'role', 'highlight'))
            else:
                self.insertFragment(self.content, ('emphasis', None, None))

        elif name == 'link':
            # Insert the link text
            # if self.href.startswith("http://") or self.href.startswith("https://"):
            self.insertAnchor(self.content, ('link', None, None), self.href)

        elif name == 'olink':
            self.insertAnchor(self.content, ('olink', None, None), self.content)

            keyword = self.content
            if not keyword in self.keywordLinks:
                self.keywordLinks.add(keyword) 

        elif name in ['mediaobject', 'imageobject', 'imagedata']:
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


    def insertBlock(self, content, selector):
        fmt = self.formatManager.getFormat(selector)
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
        imageFile = attrs.getValue("fileref")
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
        with open(contentFilePath, 'r', encoding='utf-8') as content_file:
            self.importFromFile(content_file)


    def importFromFile(self, fileDesc):
        parser = xml.sax.make_parser()
        handler = Handler(self.contentPath, self.formatManager)
        parser.setContentHandler(handler)
        parser.parse(fileDesc)

        self.document = handler.result
        self.links = sorted(handler.keywordLinks)


    def getDocument(self):
        return self.document
    

    def getLinks(self):
        return self.links
