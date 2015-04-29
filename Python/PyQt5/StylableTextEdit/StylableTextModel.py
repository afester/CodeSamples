'''
Created on 10.03.2015

@author: afester
'''

from PyQt5.Qt import QTextTable, QTextDocument, QTextCursor, QTextFormat, QTextCharFormat

import tools
import os
from StylableTextEdit.CustomObjects import ImageObject, MathFormulaObject, CustomObjectRenderer


class Node:

    def __init__(self):
        self.children = []

    def add(self, child):
        self.children.append(child)

    def __repr__(self):
        return self.__str__()


# A Frame is a container for child Frames and Paragraphs
class Frame(Node):

    def __init__(self):
        Node.__init__(self)

    def __str__(self):
        return 'Frame'


# A Paragraph is a container for Fragments
class Paragraph(Node):

    def __init__(self, indentLevel, style):
        Node.__init__(self)

        self.indent = indentLevel
        self.style = style

    def __str__(self):
        return 'Paragraph[style={}]'.format(self.style)


# A List is a container for Paragraphs and Lists on a deeper level
class List(Node):

    def __init__(self, style):
        Node.__init__(self)
        self.style = style

    def __str__(self):
        return 'List[style={}]'.format(self.style)



class Table:
    pass


# Fragments do not have children
class Fragment:

    def __init__(self, style):
        self.href = None

        self.style = style

    def setHref(self, text):
        self.href = text

    def __repr__(self):
        return self.__str__()


class TextFragment(Fragment):

    def __init__(self, style):
        Fragment.__init__(self, style)
        self.text = None

    def setText(self, text):
        self.text = text

    def __str__(self):
        return 'TextFragment[style={}, text="{}", href={}]'.format(self.style, self.text, self.href)


class ImageFragment(Fragment):

    def __init__(self):
        Fragment.__init__(self, (None, None, None))
        self.image = None

    def setImage(self, img):
        self.image = img

    def __str__(self):
        return 'ImageFragment[image={}, href={}]'.format(self.image, self.href)


class MathFragment(Fragment):

    def __init__(self):
        Fragment.__init__(self, (None, None, None))
        self.image = None
        self.text = None

    def setText(self, text):
        self.text = text

    def setImage(self, img):
        self.image = img

    def __str__(self):
        return 'MathFragment[text="{}", image={}, href={}]'.format(self.text, self.image, self.href)



class TextDocumentTraversal:

    def __init__(self):
        pass

    def traverse(self, document):
        frm = document.rootFrame()
        result = self.dumpFrame(frm)

        return result


    def dumpFrame(self, frm):

        parentStack = [Frame()]

        currentIndent = 0       # current indent within this frame
        if type(frm) is QTextTable:
            tbl = self.dumpTable(frm)
        else:
            iterator = frm.begin()
            while not iterator.atEnd():
                frame = iterator.currentFrame() # None if not a frame
                block = iterator.currentBlock() # Invalid block if its a frame

                if frame:
                    subframe = self.dumpFrame(frame)
                    # ... TODO
                else:

                    # A block is not further structured (from a QTextDocument's point of view).
                    # We can get the whole block here and handle the inherent structure below.
                    para = self.getParagraph(block)
                    listLevel = para.indent

                    # Add additional List elements as required
                    for x in range(currentIndent, listLevel):
                        l = List(('itemizedlist', 'level', x))
                        parent = parentStack[x]
                        parent.add(l)
                        parent = l
                        if len(parentStack) > x+1:
                            parentStack[x+1] = parent
                        else:
                            parentStack.append(parent)

                    parent = parentStack[listLevel]
                    parent.add(para)

                    currentIndent = listLevel

                iterator += 1

        return parentStack[0]


    def dumpTable(self, table):
        raise None
    
        for row in range(0, table.rows()):
            for column in range(0, table.columns()):
                tableCell = table.cellAt(row, column)

                # BUG: PyQt's QTextTableCell does not expose the frame iterator (begin(), end()) 
                iterator = tableCell.begin()        # AttributeError: 'QTextTableCell' object has no attribute 'begin'
                while not iterator.atEnd():
                    pass

        return Table()  # Mock


    def getParagraph(self, block):
        blockFormat = block.blockFormat()
        blockStyle = blockFormat.property(QTextFormat.UserProperty)

        listLevel = 0
        textList = block.textList()
        if textList:
            listFormat = textList.format()
            listLevel = listFormat.indent()
       
        result = Paragraph(listLevel, blockStyle)

        iterator = block.begin()
        while not iterator.atEnd():
            fragment = iterator.fragment()

            for frag in self.getFragments(fragment):   # Fragment could contain more than one image!
                result.add(frag)
            iterator += 1

        return result


    def getFragments(self, fragment):
        result = []

        text = fragment.text().replace('\u2028', '\n')

        charFormat = fragment.charFormat()
        style = charFormat.property(QTextFormat.UserProperty)

        href = None
        if style and style[0] == 'link':
            href = charFormat.anchorHref()  #escape(charFormat.anchorHref())
            style = None     # Link implicitly defined by setting href

        isObject = (text.find('\ufffc') != -1)
        if isObject:
            if charFormat.isImageFormat():
                assert(False)   # This condition should never be true anymore - we are reendering images as custom objects

    #===========================================================================
    #             imgFmt = charFormat.toImageFormat()
    #             imgName = tools.os_path_split(imgFmt.name())[-1]
    # 
    #             # If the same image repeats multiple times, it is simply represented by
    #             # yet another object replacement character ...
    #             for img in range(0, len(text)):
    #                 frag = ImageFragment()
    #                 frag.image = imgName
    #                 frag.setHref(href)
    #                 result.append(frag)
    #===========================================================================
            else:
                customObject = charFormat.property(QTextCharFormat.UserProperty+1)
                if type(customObject) is ImageObject:
                    frag = ImageFragment()
                    frag.image = tools.os_path_split(customObject.imageName)[-1]
                    frag.setHref(href)
                    result.append(frag)
                elif type(customObject) is MathFormulaObject:
                    frag = MathFragment()
                    frag.setText(customObject.formula)
                    frag.image = customObject.image
                    result.append(frag)
        else:
            # text = escape(text)
            frag = TextFragment(style)
            frag.setHref(href)
            frag.setText(text)
            result.append(frag)

        return result


class StructurePrinter:
    
    def __init__(self, root, out):
        self.rootFrame = root
        self.indent = 0
        self.out = out

    def prefix(self):
        return '\u00a0' * self.indent * 2

    def traverse(self):
        self.recursiveTraverse(self.rootFrame)

    def recursiveTraverse(self, parent):
        self.out("{}{}\n".format(self.prefix(), parent))
        if type(parent) == Paragraph or type(parent) == Frame or type(parent) == List:
            for c in parent.children:
                self.indent += 1
                self.recursiveTraverse(c)
                self.indent -= 1



class DocumentFactory:
    
    def __init__(self, contentPath, formatManager):
        self.formatManager = formatManager
        self.contentPath = contentPath


    def createDocument(self, rootFrame):

        # Create empty document
        self.document = QTextDocument()
        self.document.setUndoRedoEnabled(False)
        self.document.setIndentWidth(20)

        # Register a renderer for custom text objects
        mo = CustomObjectRenderer()
        mo.setParent(self.document)
        self.document.documentLayout().registerHandler(QTextCharFormat.UserObject+1, mo);

        self.cursor = QTextCursor(self.document)
        self.listLevel = 0
        self.paraFormat = None

        # add all root paragraphs
        for n in rootFrame.children:
            self.addNode(n)

        # Clean up the first paragraph if document is not empty
        self.cursor.movePosition(QTextCursor.Start)
        b = self.cursor.block()
        if b.length() == 1:
            cursor = QTextCursor(self.document.findBlockByLineNumber(0))
            cursor.select(QTextCursor.BlockUnderCursor)
            cursor.deleteChar()

        return self.document


    def addNode(self, node):
        if type(node) == Paragraph:
            self.paraFormat = self.formatManager.getFormat(node.style)

            # NOTE: "The block char format is the format used when inserting 
            #        text at the beginning of an empty block."
            #       See also below.
            self.cursor.insertBlock(self.paraFormat.getBlockFormat(), self.paraFormat.getCharFormat())
            # self.cursor.insertFragment(QTextDocumentFragment.fromPlainText(''))

            if self.listLevel > 0:
                # TODO: use list style from list node - requires a stack, though ...
                listStyle = ('itemizedlist', 'level', str(self.listLevel))
                newList = self.cursor.createList(self.formatManager.getFormat(listStyle).getListFormat())
            for n in node.children:
                self.addNode(n)

        elif type(node) == List:
            self.listLevel += 1
            for n in node.children:
                self.addNode(n)
            self.listLevel -= 1

        elif type(node) is ImageFragment:
            imageObject = ImageObject()
            imagePath = os.path.join(self.contentPath, node.image)
            imageObject.setName(imagePath)

            imageObjectFormat = QTextCharFormat()
            imageObjectFormat.setObjectType(QTextFormat.UserObject + 1)
            imageObjectFormat.setProperty(QTextFormat.UserProperty + 1, imageObject)
            self.cursor.insertText('\ufffc', imageObjectFormat);

        elif type(node) is MathFragment:
            mathFormula = MathFormulaObject()
            mathFormula.setFormula(node.text)
            mathFormula.image = node.image #  renderFormula()

            mathObjectFormat = QTextCharFormat()
            mathObjectFormat.setObjectType(QTextFormat.UserObject + 1)
            mathObjectFormat.setVerticalAlignment(QTextCharFormat.AlignMiddle)
            mathObjectFormat.setProperty(QTextFormat.UserProperty + 1, mathFormula)
            self.cursor.insertText('\ufffc', mathObjectFormat);
        elif type(node) is TextFragment:
            text = node.text.replace('\n', '\u2028')
            if node.href is not None:
                fmt = self.formatManager.getFormat(('link', None, None))   # TODO!
                charFmt = fmt.getCharFormat()
                charFmt.setAnchorHref(node.href)
                self.cursor.insertText(text, charFmt)
            else:
                # "The block char format is the format used when inserting text at the beginning of an empty block.
                # Hence, the block char format is only useful for the first fragment -
                # once a fragment is inserted with a different style, and afterwards
                # another fragment is inserted with no specific style, we need to reset
                # the char format to the block's char format explicitly!
    
                if node.style is not None:
                    fmt = self.formatManager.getFormat(node.style)
                else:
                    fmt = self.paraFormat

                self.cursor.insertText(text, fmt.getCharFormat())
