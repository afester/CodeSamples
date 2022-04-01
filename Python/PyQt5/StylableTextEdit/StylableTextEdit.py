"""
Created on 29.04.2015

@author: afester
"""

import os

from PyQt6.QtCore import pyqtSignal, Qt, QUrl, QSize, QMimeData, QPointF
from PyQt6.QtGui import QTextCursor, QTextFormat, QGuiApplication, QCursor, QTextCharFormat, QDesktopServices
from PyQt6.QtWidgets import QTextEdit, QFrame, QHBoxLayout, QLineEdit, QPushButton, QMessageBox

from StylableTextEdit.CustomObjects import MathFormulaObject, ImageObject
from StylableTextEdit.StylableTextModel import StructurePrinter, Frame, Paragraph, List, TextFragment, \
    TextDocumentSelectionTraversal


class UrlEditor(QFrame):

    applyUrl = pyqtSignal(str)

    def __init__(self, parentWidget):
        QFrame.__init__(self, parentWidget, Qt.WindowType.Tool)
        layout = QHBoxLayout()
        self.setWindowTitle('Edit URL reference')
        self.setLayout(layout)

        self.editLine = QLineEdit(self)
        layout.addWidget(self.editLine)
        applyButton = QPushButton('Apply')
        applyButton.clicked.connect(self.doApply)
        layout.addWidget(applyButton)

    def doApply(self):
        self.applyUrl.emit(self.editLine.text())
        self.hide()

    def setUrl(self, text):
        self.editLine.setText(text)


class AppXmlPrinter:
    
    def __init__(self, root, out):
        self.rootFrame = root
        self.indent = 0
        self.out = out

    def prefix(self):
        return '\u00a0' * self.indent * 2

    def traverse(self):
        self.recursiveTraverse(self.rootFrame)

    def recursiveTraverse(self, parent):
        print("{}<{}".format(self.prefix(), parent.getLocalName()), end='')
        if type(parent) == Paragraph or type(parent) == TextFragment:
            print(' style="{}"'.format(parent.style), end='')

        if type(parent) == TextFragment:
            print('>', end='')
            print(parent.text, end='')
        else:
            print('>')

        if type(parent) == Paragraph or type(parent) == Frame or type(parent) == List:
            for c in parent.children:
                self.indent += 1
                self.recursiveTraverse(c)
                self.indent -= 1

        if type(parent) == TextFragment:
            print("</{}>".format(parent.getLocalName()))
        else:
            print("{}</{}>".format(self.prefix(), parent.getLocalName()))


class StylableTextEdit(QTextEdit):

    updateBlockFormat = pyqtSignal(object)
    updateCharFormat = pyqtSignal(object)
    navigate = pyqtSignal(str)
    objectSelectionChanged = pyqtSignal()
 
    def __init__(self, parentWidget, formatManager):
        QTextEdit.__init__(self, parentWidget)
        self.formatManager = formatManager

        self.tracking = False
        self.selectedObject = None
        self.orgCursorWidth = self.cursorWidth()

        self.urlEditor = UrlEditor(self)
        self.urlEditor.applyUrl.connect(self.setCurrentUrl)
        self.urlEditor.resize(QSize(500, self.urlEditor.size().height()))
        self.cursorPositionChanged.connect(self.handleCursorPositionChanged)
        self.currentCharFormatChanged.connect(self.handleCharFormatChanged)

    # @SLOT
    def applyBlockStyle(self, style):
        pos = self.textCursor().position()

        fmt = self.formatManager.getFormat(style)
        if fmt.isPre:
            self.textCode(fmt)
        else:
            self.reformatBlock(style)

        self.textCursor().setPosition(pos)

    def textCode(self, fmt):
        """Format the currently selected blocks (or the block where the cursor
           is located) with the given style. All existing styles will be removed,
           and if more than one block is selected all the blocks are merged into one
           block with the block separators replaced by Unicode line separators"""

        cursor = self.textCursor()
        cursor.beginEditBlock()

        self.selectWholeBlocks(cursor)

        # Create a plain text representation of the selected blocks
        content = cursor.selectedText()
        content = content.replace('\n', '\u2028')
        content = content.replace('\u2029', '\u2028')   # TODO?????

        cursor.removeSelectedText()
        cursor.setBlockFormat(fmt.getBlockFormat())
        cursor.setCharFormat(fmt.getCharFormat())
        cursor.insertText(content)

        cursor.endEditBlock()
        self.setFocus()

    def reformatBlock(self, style):
        if style[0] in ['itemizedlist', 'orderedlist']:
            self.formatAsList(style)
        else:
            fmt = self.formatManager.getFormat(style)

            cursor = self.textCursor()
            cursor.beginEditBlock()
    
            self.selectWholeBlocks(cursor)
            content = cursor.selection().toPlainText()
    
            cursor.removeSelectedText()
            cursor.setBlockFormat(fmt.getBlockFormat())
            cursor.setCharFormat(fmt.getCharFormat())
            cursor.setBlockCharFormat(fmt.getCharFormat())  # required for initial fragment of an empty block
            cursor.insertText(content)

            cursor.endEditBlock()
            self.setFocus()

    def formatAsList(self, style):
        cursor = self.textCursor()
        cursor.beginEditBlock()

        fmt = self.formatManager.getFormat(style)
        cursor.createList(fmt.getListFormat())

        cursor.endEditBlock()
        self.setFocus()

    # Selects all blocks which are part of the current selection.
    def selectWholeBlocks(self, cursor):
        start = cursor.anchor()
        end = cursor.position()
        if start > end:
            temp = end
            end = start
            start = temp

        cursor.setPosition(start)
        cursor.movePosition(QTextCursor.MoveOperation.StartOfBlock)
        cursor.setPosition(end, QTextCursor.MoveMode.KeepAnchor)
        cursor.movePosition(QTextCursor.MoveOperation.EndOfBlock, QTextCursor.MoveMode.KeepAnchor)

###############################################################################

    # @SLOT
    def applyTextFormat(self, style, flag):
        pos = self.textCursor().position()

        if flag:
            self.setTextFormat(style)
        else:
            self.removeTextFormat()

        self.textCursor().setPosition(pos)

    def setTextFormat(self, style):
        # TODO: support nested fragment formats (e.g. bold>highlight>/highlight/bold)
        fmt = self.formatManager.getFormat(style).getCharFormat()

        cursor = self.textCursor()
        if not cursor.hasSelection():
            cursor.select(QTextCursor.SelectionType.WordUnderCursor)

        cursor.setCharFormat(fmt)

    def removeTextFormat(self):
        # TODO: Take block char format or char format from previous fragment
        fmt = self.formatManager.getFormat(('para', None, None)).getCharFormat()
        cursor = self.textCursor()
        self.selectCurrentFragment(cursor)
        cursor.setCharFormat(fmt)

    def handleCharFormatChanged(self):
        pass
        # print("CHAR FORMAT CHANGED, ADJUST CURSOR COLOR ...")
        #  self.setTextColor(Qt.blue)
        # p = self.palette();
        # p.setColor(QPalette.WindowText, QColor(255, 0, 0))
        # self.setPalette(p);
        # self.setCursorWidth(5)
        # self.setStyleSheet("QPlainTextEdit{color: #ffff00; background-color: #303030;"
        #                    " selection-background-color: #606060; selection-color: #ffffff;}")

    def selectCurrentFragment(self, cursor):

        cf = cursor.charFormat()
        style = cf.property(QTextFormat.Property.UserProperty)

        # go to left while the style is the same
        searchLeft = True
        while searchLeft:
            cursor.movePosition(QTextCursor.MoveOperation.Left)
            cf = cursor.charFormat()
            styleX = cf.property(QTextFormat.Property.UserProperty)
            if cursor.atBlockStart() or styleX != style:
                searchLeft = False

        # Select all to the right while the style is the same
        searchRight = True
        while searchRight:
            cursor.movePosition(QTextCursor.MoveOperation.Right, QTextCursor.MoveMode.KeepAnchor)
            cf = cursor.charFormat()
            styleX = cf.property(QTextFormat.Property.UserProperty)
            if cursor.atBlockEnd() or styleX != style:
                searchRight = False
        if not cursor.atBlockEnd():
            cursor.movePosition(QTextCursor.MoveOperation.Left, QTextCursor.MoveMode.KeepAnchor)

    def getTextForCurrentFragment(self, cursor):
        self.selectCurrentFragment(cursor)
        result = cursor.selectedText()
        print("RESULT:{}".format(result))
        return result

    def handleCursorPositionChanged(self):
        cursor = self.textCursor()

        # update block format toolbar
        blockFmt = cursor.blockFormat()
        blockStyle = blockFmt.property(QTextFormat.Property.UserProperty)
        self.updateBlockFormat.emit(blockStyle)

        # update text format toolbar
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position
        charStyle = charFmt.property(QTextFormat.Property.UserProperty)
        self.updateCharFormat.emit(charStyle)

        # open/close URL editor for external links
        if charStyle and charStyle[0] == 'link':
            if not (QGuiApplication.keyboardModifiers() & Qt.KeyboardModifier.ControlModifier):
                url = charFmt.anchorHref()

                # get global cursor position
                pos = self.cursorRect()
                pos = pos.bottomLeft()
                pos = self.viewport().mapToGlobal(pos)

                self.urlEditor.move(pos)
                self.urlEditor.setUrl(url)
                self.urlEditor.show()

        else:
            self.urlEditor.hide()

# ===============================================================================
#         # We would like to render the bullets of the current list in a different
#         # color to make it clear which list items belong to the current list.
#         # However, it does currently not seem possible to influence the
#         # bullet color individually, either not at all or only for specific bullet types
#         # Therefore, we currently simply switch the bullet style
# 
#         # Reset previous list
#         listBlock = cursor.block().textList()
#         if self.currentList and listBlock is not self.currentList:
#             fmt = self.currentList.format()
#             fmt.setStyle(QTextListFormat.ListDisc)
#             self.currentList.setFormat(fmt)
# 
#         if listBlock:
#             # Set current list
#             fmt = listBlock.format()
#             fmt.setStyle(QTextListFormat.ListCircle)
#             listBlock.setFormat(fmt)
#             self.currentList = listBlock
# ===============================================================================

    def setCurrentUrl(self, url):
        cursor = self.textCursor()
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position
        charStyle = charFmt.property(QTextFormat.Property.UserProperty)
        if charStyle and charStyle[0] == 'link':
            self.selectCurrentFragment(cursor)
            charFmt.setAnchorHref(url)
            cursor.setCharFormat(charFmt)

    def isAtAnchor(self, pos):
        cursor = self.exactCursorForPosition(pos)
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position

        style = charFmt.property(QTextFormat.Property.UserProperty)
        if style and style[0] in ['link', 'olink']:
            return True

        return False

    def updateIBeamCursor(self, pos):
        if self.isAtAnchor(pos):
            self.viewport().setCursor(Qt.CursorShape.PointingHandCursor)
        else:
            self.viewport().setCursor(Qt.CursorShape.IBeamCursor)

    def mouseMoveEvent(self, event):
        if self.tracking:
            self.updateIBeamCursor(event.pos())

        return QTextEdit.mouseMoveEvent(self, event)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key.Key_Control:
            self.setMouseTracking(True)
            self.tracking = True
            pos = self.mapFromGlobal(QCursor.pos())
            self.updateIBeamCursor(pos)

        return QTextEdit.keyPressEvent(self, event)

    def keyReleaseEvent(self, event):
        if event.key() == Qt.Key.Key_Control:
            self.setMouseTracking(False)
            self.tracking = False
            self.viewport().setCursor(Qt.CursorShape.IBeamCursor)

        return QTextEdit.keyReleaseEvent(self, event)

    def mapToContents(self, point):
        return QPointF(point.x() + self.horizontalScrollBar().value(), point.y() +
                       self.verticalScrollBar().value())

    def exactCursorForPosition(self, pos):
        # get a cursor for a given position
        # Note that the cursor is located AFTER the respective character:
        # ABCDEF
        #    ^ 
        #    +- Mouse positioned on D 
        #             => returns cursor which is located between C and D
        #             => character addressed by cursor would be "C" !!!!!
        #                (position() would return 2)
        # cursor = self.cursorForPosition(event.pos())

        # this is identical to calling the document layout's hitTest() 
        # method with the FuzzyHit parameter:

        # curPos = docLayout.hitTest(event.pos(), Qt.FuzzyHit)

        # A better approch is to use the document layout with the hitTest()
        # method and using the ExactHit parameter:
        docLayout = self.document().documentLayout()

        # Note that docLayout.hitTest() takes the position as coordinates
        # relative to the document itself - thus we need to map it 
        # similar to what Qt does in QTextEdit::cursorForPosition().
        # Unfortunately the mapToContents() method they use there is not
        # public, so we need to provide our own implementation.
        docPos = self.mapToContents(pos)
        curPos = docLayout.hitTest(docPos, Qt.HitTestAccuracy.ExactHit)
        if curPos == -1:
            curPos = 0

        # This returns the cursor position *before* the clicked character.
        # To get the proper format, we need to position the cursor
        # *after* the clicked character:
        cursor = QTextCursor(self.document())
        cursor.setPosition(curPos + 1)

        return cursor

    def mousePressEvent(self, event):
        # =======================================================================
        # cursor = self.cursorForPosition(event.pos())
        # charFmt = cursor.charFormat()
        # if charFmt.isImageFormat():
        #     imgFmt = charFmt.toImageFormat()
        #     print("IMAGE {}: {} x {}".format(imgFmt.name(), imgFmt.width(), imgFmt.height()))
        # =======================================================================

        cursor = self.exactCursorForPosition(event.pos())
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position

        if charFmt.objectType() == QTextCharFormat.ObjectTypes.UserObject+1:
            if self.selectedObject is not None:
                self.selectedObject.setSelected(False)

            self.selectedObject = charFmt.property(QTextCharFormat.Property.UserProperty + 1)
            self.selectedObject.setSelected(True)
            self.setCursorWidth(0)
            self.viewport().update()
            self.objectSelectionChanged.emit()
        else:
            if self.selectedObject is not None:
                self.selectedObject.setSelected(False)
                self.selectedObject = None
                self.setCursorWidth(self.orgCursorWidth)
                self.viewport().update()
                self.objectSelectionChanged.emit()

            if self.tracking:
                style = charFmt.property(QTextFormat.Property.UserProperty)

                if style and style[0] == 'link':
                    url = charFmt.anchorHref()
                    QDesktopServices.openUrl(QUrl(url))
                    return
    
                if style and style[0] == 'olink':
                    keyword = self.getTextForCurrentFragment(cursor)
                    self.navigate.emit(keyword)
                    return

        return QTextEdit.mousePressEvent(self, event)

    def outPut(self, line):
        print(line, end='')

    def createMimeDataFromSelection(self):
        result = QMimeData()
        cursor = self.textCursor()
        selection = cursor.selectedText()
        selection = selection.replace('\u2029', '\n')
        selection = selection.replace('\u2028', '\n')
        result.setText(selection)

        # create an xml representation of the selected document structure 
        traversal = TextDocumentSelectionTraversal()
        frame = traversal.traverse(self.textCursor(), self.document())

        print('\n-----------------------------------------------')
        printer = StructurePrinter(frame, self.outPut)
        printer.traverse()
        print('-----------------------------------------------')

        return result

        sp = AppXmlPrinter(frame, None)
        sp.traverse()

        blafasel = '''<?xml version="1.0" encoding="utf-8"?>
<article version="5.0" xml:lang="en"
         xmlns="http://docbook.org/ns/docbook"
         xmlns:xlink="http://www.w3.org/1999/xlink">
  <title></title></article>'''
        result.setData("application/xml", blafasel)
        return result

    def insertImage(self, image):
        cursor = self.textCursor()

        page = self.parent().page
        fileName = page.saveImage(image)        # returns the file name inside the page's directory
        
        imageObject = ImageObject()
        imagePath = os.path.join(page.getPageDir(), fileName)
        imageObject.setName(imagePath)

        imageObjectFormat = QTextCharFormat()
        imageObjectFormat.setObjectType(QTextFormat.ObjectTypes.UserObject + 1)
        imageObjectFormat.setProperty(QTextFormat.Property.UserProperty + 1, imageObject)
        cursor.insertText('\ufffc', imageObjectFormat)

        # Make sure that the image is also part of the page
        page.save()

    def insertFormula(self):
        cursor = self.textCursor()

        mathFormula = MathFormulaObject()
        mathFormula.setFormula('f(x) := ...')
        mathFormula.renderFormula()

        mathObjectFormat = QTextCharFormat()
        mathObjectFormat.setObjectType(QTextFormat.ObjectTypes.UserObject + 1)
        mathObjectFormat.setVerticalAlignment(QTextCharFormat.VerticalAlignment.AlignMiddle)
        mathObjectFormat.setProperty(QTextFormat.Property.UserProperty + 1, mathFormula)
        cursor.insertText('\ufffc', mathObjectFormat)

        if self.selectedObject is not None:
            self.selectedObject.setSelected(False)
        self.selectedObject = mathFormula
        self.selectedObject.setSelected(True)
        self.viewport().update()
        self.objectSelectionChanged.emit()

    def insertFromMimeData(self, data):

        if data.hasImage():
            image = data.imageData()
            self.insertImage(image)
        elif data.hasText():
            self.insertPlainText(data.text())
        else:
            msg = ''
            for f in data.formats():
                msg = msg + '\n   ' + f
            QMessageBox.information(self, 'Error', 'Unsupported clipboard content: {}'.format(msg))

    def getSelectedObject(self):
        return self.selectedObject

    # @SLOT
    def indentLess(self):
        cursor = self.textCursor()
        curList = cursor.currentList()
        if curList:
            fmt = curList.format()
            styleSelector = fmt.property(QTextFormat.ObjectTypes.UserProperty)
            indent = int(styleSelector[2])
            if indent > 1:
                indent -= 1

                styleSelector = (styleSelector[0], styleSelector[1], str(indent))
                style = self.formatManager.getFormat(styleSelector)
                if style:
                    listFormat = style.getListFormat()
                    cursor.createList(listFormat)

    # @SLOT
    def indentMore(self):
        cursor = self.textCursor()
        curList = cursor.currentList()
        if curList:
            fmt = curList.format()
            styleSelector = fmt.property(QTextFormat.Property.UserProperty)
            indent = int(styleSelector[2])
            if indent < 4:
                indent += 1

                styleSelector = (styleSelector[0], styleSelector[1], str(indent))
                style = self.formatManager.getFormat(styleSelector)
                if style:
                    listFormat = style.getListFormat()
                    cursor.createList(listFormat)
