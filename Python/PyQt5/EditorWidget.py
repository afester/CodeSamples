'''
Created on 18.02.2015

@author: afester
'''
 
from PyQt5.Qt import QMimeData, QDesktopServices, QUrl, QSize
from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtGui import QTextCursor, QTextFormat, QGuiApplication
from PyQt5.QtGui import QTextOption, QCursor, QImage
from PyQt5.QtWidgets import QWidget, QSpacerItem, QMessageBox, QPushButton
from PyQt5.QtWidgets import QTextEdit, QVBoxLayout, QLineEdit, QFrame, QHBoxLayout, QSizePolicy, QFileDialog

import os

from FormatManager import FormatManager
from Toolbars import ActionSelector, TextStyleSelector, BlockStyleSelector


class UrlEditor(QFrame):

    applyUrl = pyqtSignal(str)

    def __init__(self, parentWidget):
        QFrame.__init__(self, parentWidget, Qt.Tool )
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


class TextEdit(QTextEdit):

    updateBlockFormat = pyqtSignal(object)
    updateCharFormat = pyqtSignal(object)
    navigate = pyqtSignal(str)
 
    def __init__(self, parentWidget):
        QTextEdit.__init__(self, parentWidget)
        self.tracking = False

        self.l = UrlEditor(self)
        self.l.applyUrl.connect(self.setCurrentUrl)
        self.l.resize(QSize(500, self.l.size().height()))
        self.cursorPositionChanged.connect(self.handleCursorPositionChanged)
        # self.editView.currentCharFormatChanged.connect(self.currentCharFormatChanged)


    def selectCurrentFragment(self, cursor):
        cf = cursor.charFormat();
        style = cf.property(QTextFormat.UserProperty)

        # go to left while the style is the same
        searchLeft = True
        while searchLeft:
            cursor.movePosition(QTextCursor.Left)
            cf = cursor.charFormat();
            styleX = cf.property(QTextFormat.UserProperty)
            if cursor.atBlockStart() or styleX != style:
                searchLeft = False

        # Select all to the right while the style is the same
        searchRight = True
        while searchRight:
            cursor.movePosition(QTextCursor.Right, QTextCursor.KeepAnchor)
            cf = cursor.charFormat();
            styleX = cf.property(QTextFormat.UserProperty)
            if cursor.atBlockEnd() or styleX != style:
                searchRight = False
        if not cursor.atBlockEnd():
            cursor.movePosition(QTextCursor.Left, QTextCursor.KeepAnchor)


    def getTextForCurrentFragment(self, cursor):
        self.selectCurrentFragment(cursor)
        result = cursor.selectedText()
        return result


    def handleCursorPositionChanged(self):
        cursor = self.textCursor()

        # update block format toolbar
        blockFmt = cursor.blockFormat()
        blockStyle = blockFmt.property(QTextFormat.UserProperty)
        self.updateBlockFormat.emit(blockStyle)

        # update text format toolbar
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position
        charStyle = charFmt.property(QTextFormat.UserProperty)
        self.updateCharFormat.emit(charStyle)

        # open/close URL editor for external links
        if charStyle and charStyle[0] == 'link':
            if not (QGuiApplication.keyboardModifiers() & Qt.ControlModifier):
                url = charFmt.anchorHref()

                # get global cursor position
                pos = self.cursorRect()
                pos = pos.bottomLeft()
                pos = self.viewport().mapToGlobal(pos)

                self.l.move(pos)
                self.l.setUrl(url)
                self.l.show()

        else:
            self.l.hide()

#===============================================================================
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
#===============================================================================

    def setCurrentUrl(self, url):
        cursor = self.textCursor()
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position
        charStyle = charFmt.property(QTextFormat.UserProperty)
        if charStyle and charStyle[0] == 'link':
            self.selectCurrentFragment(cursor)
            charFmt.setAnchorHref(url)
            cursor.setCharFormat(charFmt)


    def isAtAnchor(self, pos):
        cursor = self.cursorForPosition(pos)
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position

        style = charFmt.property(QTextFormat.UserProperty)
        if style and style[0] in ['link', 'olink']:
            return True

        return False


    def updateIBeamCursor(self, pos):
        if self.isAtAnchor(pos):
            self.viewport().setCursor(Qt.PointingHandCursor)
        else:
            self.viewport().setCursor(Qt.IBeamCursor)


    def mouseMoveEvent(self, event):
        if self.tracking:
            self.updateIBeamCursor(event.pos())

        return QTextEdit.mouseMoveEvent(self, event)


    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Control:
            self.setMouseTracking(True)
            self.tracking = True
            pos = self.mapFromGlobal(QCursor.pos())
            self.updateIBeamCursor(pos)

        return QTextEdit.keyPressEvent(self, event)


    def keyReleaseEvent(self, event):
        if event.key() == Qt.Key_Control:
            self.setMouseTracking(False)
            self.tracking = False
            self.viewport().setCursor(Qt.IBeamCursor)

        return QTextEdit.keyReleaseEvent(self, event)


    def mousePressEvent(self, event):
        #=======================================================================
        # cursor = self.cursorForPosition(event.pos())
        # charFmt = cursor.charFormat()
        # if charFmt.isImageFormat():
        #     imgFmt = charFmt.toImageFormat()
        #     print("IMAGE {}: {} x {}".format(imgFmt.name(), imgFmt.width(), imgFmt.height()))
        #=======================================================================

        if self.tracking: #  and self.isAtAnchor(event.pos()):

            cursor = self.cursorForPosition(event.pos())
            charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position
            style = charFmt.property(QTextFormat.UserProperty)

            if style and style[0] == 'link':
                url = charFmt.anchorHref()
                QDesktopServices.openUrl(QUrl(url))
                return

            if style and style[0] == 'olink':
                keyword = self.getTextForCurrentFragment(cursor)
                self.navigate.emit(keyword)
                return

        return QTextEdit.mousePressEvent(self, event)


    def createMimeDataFromSelection(self):
        result = QMimeData()
        cursor = self.textCursor()
        selection = cursor.selectedText()
        selection = selection.replace('\u2029', '\n')
        result.setText(selection)
        return result


    def insertImage(self, image):
        cursor = self.textCursor()

        page = self.parent().page
        fileName = page.saveImage(image)
        
        cursor.insertImage(image, fileName)

        # Make sure that the image is also part of the page
        page.save()


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


class EditorWidget(QWidget):
    message = pyqtSignal(str)
    titleChanged = pyqtSignal(str)
    navigate = pyqtSignal(str)


    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.formatManager = FormatManager()
        self.formatManager.loadFormats()

        #self.currentList = None

        self.editView = TextEdit(self)
        self.editView.navigate.connect(self.navigate)

        toolbar = QWidget(self)
        layout = QHBoxLayout()
        layout.setContentsMargins(0,0,0,0)
        toolbar.setLayout(layout)

        self.actionsSelector = ActionSelector(self)
        layout.addWidget(self.actionsSelector)

        self.textFormatSelector = TextStyleSelector(self)
        self.textFormatSelector.styleChanged.connect(self.textFormatChanged)
        self.editView.updateCharFormat.connect(self.textFormatSelector.setCurrentStyle)
        layout.addWidget(self.textFormatSelector)

        self.styleSelector = BlockStyleSelector(self)
        self.styleSelector.styleChanged.connect(self.blockStyleChanged)
        self.styleSelector.indentLess.connect(self.indentLess)
        self.styleSelector.indentMore.connect(self.indentMore)
        self.styleSelector.insertTable.connect(self.insertTable)
        self.editView.updateBlockFormat.connect(self.styleSelector.setCurrentStyle)
        layout.addWidget(self.styleSelector)

        horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
        layout.addItem(horizontalSpacer)

        hLayout = QVBoxLayout(self)
        hLayout.setContentsMargins(0, 0, 0, 0)
        hLayout.addWidget(toolbar)
        hLayout.addWidget(self.editView)

        # The toolbar should take the minimum space and the edit view the remaining space
        hLayout.setStretch(0, 0)
        hLayout.setStretch(1, 1)


    def load(self, notepad, pageId):
        print('Loading page "{}" from notepad "{}"'.format(pageId if pageId is not None else 'titlePage', notepad.getName()))
        self.page = notepad.getPage(pageId)

        doc = self.page.getDocument()

        self.editView.blockSignals(True)
        self.editView.setDocument(doc)
        self.editView.blockSignals(False)

        # TODO: review
        doc.undoAvailable.connect(self.actionsSelector.undoAction.setEnabled)
        doc.redoAvailable.connect(self.actionsSelector.redoAction.setEnabled)

        # Setup modification flag handling
        doc.modificationChanged.connect(self.updateWindowTitle)
        doc.setModified(False)

        doc.setUndoRedoEnabled(True)
        self.actionsSelector.undoAction.setEnabled(False)
        self.actionsSelector.redoAction.setEnabled(False)

        self.editView.setFocus()
        self.editView.handleCursorPositionChanged()    # update toolbars

        self.contentFile = os.path.join(notepad.getName(),str(pageId))
        self.message.emit('Loaded {}: {}'.format(notepad.getName(), self.page.getName()))


    def updateWindowTitle(self, flag):
        self.actionsSelector.saveAction.setEnabled(flag)
        self.titleChanged.emit('{}{}: {}'.format('*' if flag else '', self.page.notepad.getRootpath(), self.page.getName()))


## Actions

    def save(self):
        if self.editView.document().isModified():
            self.page.save()
            self.message.emit('Saved {}: {}'.format(self.page.notepad.getName(), self.page.getName()))


    def toggleNonprintable(self):
        if self.actionsSelector.nonprintableAction.isChecked():
            option = QTextOption()
            option.setFlags(QTextOption.ShowTabsAndSpaces | QTextOption.ShowLineAndParagraphSeparators)
            self.editView.document().setDefaultTextOption(option)
        else:
            option = QTextOption()
            self.editView.document().setDefaultTextOption(option)

    
    def navigateBack(self):
        QMessageBox.information(self, "Error", "navigateBack not yet implemented")


    def navigateForward(self):
        QMessageBox.information(self, "Error", "navigateForward not yet implemented")


    def undo(self):
        self.editView.undo()


    def redo(self):
        self.editView.redo()


    def insertImage(self):
        filePath = QFileDialog.getOpenFileName(self, caption='Select Image file')[0]
        # print('{}: "{}" {}'.format(type(filePath), filePath, len(filePath)))
        if len(filePath) > 0:
            image = QImage(filePath)
            if image.isNull():
                QMessageBox.information(self, "Error", '"{}"\nis not a valid image file'.format(filePath))
            else:
                self.editView.insertImage(image)


## Text format

    def textFormatChanged(self, style, flag):
        print("TEXT FORMAT {} {}".format(style, flag))
        pos = self.editView.textCursor().position()

        if flag == True:
            self.applyTextFormat(style)
        else:
            self.removeTextFormat()

        self.editView.textCursor().setPosition(pos)


    def applyTextFormat(self, style):
        fmt = self.formatManager.getFormat(style).getCharFormat()

        cursor = self.editView.textCursor()
        if not cursor.hasSelection():
            cursor.select(QTextCursor.WordUnderCursor)

        cursor.setCharFormat(fmt)


    def removeTextFormat(self):
        fmt = self.formatManager.getFormat( ('para', None, None) ).getCharFormat()
        cursor = self.editView.textCursor()
        self.editView.selectCurrentFragment(cursor)
        cursor.setCharFormat(fmt)


## Block format

    def blockStyleChanged(self, style):
        pos = self.editView.textCursor().position()

        if style[0] in ['programlisting', 'screen']:
            self.textCode(style)
        else:
            self.reformatBlock(style)

        self.editView.textCursor().setPosition(pos)


    def indentLess(self):
        cursor = self.editView.textCursor()
        curList = cursor.currentList()
        if curList:
            fmt = curList.format()
            styleSelector = fmt.property(QTextFormat.UserProperty)
            indent = int(styleSelector[2])
            if indent > 1:
                indent -= 1

                styleSelector = (styleSelector[0], styleSelector[1], str(indent))
                style = self.formatManager.getFormat(styleSelector)
                if style:
                    listFormat = style.getListFormat()
                    cursor.createList(listFormat)


    def indentMore(self):
        cursor = self.editView.textCursor()
        curList = cursor.currentList()
        if curList:
            fmt = curList.format()
            styleSelector = fmt.property(QTextFormat.UserProperty)
            indent = int(styleSelector[2])
            if indent < 4:
                indent += 1

                styleSelector = (styleSelector[0], styleSelector[1], str(indent))
                style = self.formatManager.getFormat(styleSelector)
                if style:
                    listFormat = style.getListFormat()
                    cursor.createList(listFormat)


    def insertTable(self):
        cursor = self.editView.textCursor()
        
        #fmt = QTextFrameFormat()
        #fmt.setBorder(2)
        #fmt.setBorderStyle(QTextFrameFormat.BorderStyle_Dashed)
        #cursor.insertFrame(fmt)

        cursor.insertTable(2, 2)
        cursor.insertText("Hello World")


    # Selects all blocks which are part of the current selection.
    def selectWholeBlocks(self, cursor):
        start = cursor.anchor()
        end = cursor.position()
        if start > end:
            temp = end
            end = start
            start = temp

        cursor.setPosition(start)
        cursor.movePosition(QTextCursor.StartOfBlock)
        cursor.setPosition(end, QTextCursor.KeepAnchor)
        cursor.movePosition(QTextCursor.EndOfBlock, QTextCursor.KeepAnchor)


    def selectedBlocks(self):
        cursor = self.editView.textCursor()
        document = self.editView.document()

        startBlock = document.findBlock(cursor.selectionStart()) 
        endBlock = document.findBlock(cursor.selectionEnd())
        done = False
        while not done:
            yield startBlock
            if startBlock == endBlock:
                done = True
            else:
                startBlock = startBlock.next()


    def reformatBlock(self, style):
        if style[0] in ['itemizedlist', 'orderedlist']:
            self.formatAsList(style)
        else:
            fmt = self.formatManager.getFormat(style)

            cursor = self.editView.textCursor()
            cursor.beginEditBlock()
    
            self.selectWholeBlocks(cursor)
            content = cursor.selection().toPlainText()
    
            cursor.removeSelectedText()
            cursor.setBlockFormat(fmt.getBlockFormat())
            cursor.setCharFormat(fmt.getCharFormat())
            cursor.insertText(content)
    
            cursor.endEditBlock()
            self.editView.setFocus()


    def formatAsList(self, style):
        cursor = self.editView.textCursor()
        cursor.beginEditBlock()

        fmt = self.formatManager.getFormat(style)
        cursor.createList(fmt.getListFormat())

        cursor.endEditBlock()
        self.editView.setFocus()

    
    def textCode(self, style):
        '''Format the currently selected blocks (or the block where the cursor
           is located) with the given style. All existing styles will be removed,
           and if more than one block is selected all the blocks are merged into one
           block with the block separators replaced by Unicode line separators'''

        fmt = self.formatManager.getFormat(style)

        cursor = self.editView.textCursor()
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
        self.editView.setFocus()
