'''
Created on 18.02.2015

@author: afester
'''

from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtWidgets import QToolBar, QWidget, QAction, QLabel, QComboBox, QSpacerItem, QMessageBox
from PyQt5.QtWidgets import QTextEdit, QVBoxLayout, QLineEdit, QFrame, QHBoxLayout, QSizePolicy, QFileDialog
from PyQt5.QtGui import QIcon, QTextCursor, QTextFormat, QGuiApplication
from PyQt5.QtGui import QTextOption, QCursor, QPixmap, QImage

import os

from FormatManager import FormatManager
from PyQt5.Qt import QMimeData


class UrlEditor(QFrame):

    def __init__(self, parentWidget):
        QFrame.__init__(self, parentWidget, Qt.Tool )
        layout = QVBoxLayout()
        self.setLayout(layout)
        
        self.editLine = QLineEdit(self)
        layout.addWidget(self.editLine)


    def setUrl(self, text):
        self.editLine.setText(text)


class TextEdit(QTextEdit):

    navigate = pyqtSignal(str)

    def __init__(self, parentWidget):
        QTextEdit.__init__(self, parentWidget)
        self.tracking = False


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


    def isAtAnchor(self, pos):
        cursor = self.cursorForPosition(pos)
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position

        style = charFmt.property(QTextFormat.UserProperty)

        if style in ['keyword', 'a']:
            return True

        return False


    def updateIBeamCursor(self, pos):
        if self.isAtAnchor(pos):
            self.viewport().setCursor(Qt.PointingHandCursor)
        else:
            self.viewport().setCursor(Qt.IBeamCursor)


    def mouseMoveEvent(self, event):
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

        if self.tracking and self.isAtAnchor(event.pos()):
            cursor = self.cursorForPosition(event.pos())
            keyword = self.getTextForCurrentFragment(cursor)
            self.navigate.emit(keyword)
            return

        return QTextEdit.mousePressEvent(self, event)


    def createMimeDataFromSelection(self):
        result = QMimeData()
        result.setText("NOT YET!")
        return result
        # return QTextEdit.createMimeDataFromSelection(self)


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
        else:
            QTextEdit.insertFromMimeData(self, data)


class IconDropDown(QWidget):
    
    entrySelected = pyqtSignal(int)

    def __init__(self, parentWidget, iconName):
        QWidget.__init__(self, parentWidget)

        icon = QLabel(self)
        icon.setPixmap(QPixmap(iconName))

        self.dropdown = QComboBox(self)
        self.dropdown.activated.connect(self.entrySelected)

        hLayout = QVBoxLayout(self)
        hLayout.setContentsMargins(5, 5, 5, 5)
        
        self.setLayout(hLayout)
        hLayout.addWidget(icon, 0, Qt.AlignHCenter)
        hLayout.addWidget(self.dropdown)


    def addItem(self, title, tag):
        self.dropdown.addItem(title, tag)
        self.dropdown.setCurrentIndex(-1)   # TODO - can not set index to -1 before items have been added


    def setCurrentIndex(self, idx):
        self.dropdown.setCurrentIndex(idx)


class TitledToolbar(QFrame):

    def __init__(self, parent, title):
        QFrame.__init__(self, parent)
        self.setFrameStyle(QFrame.StyledPanel)

        titleLabel = QLabel(title, self)
        contentPanel = QWidget(self)
        contentPanel.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.MinimumExpanding)
        self.contentLayout = QHBoxLayout()
        self.contentLayout.setContentsMargins(0, 0, 0, 0)
        contentPanel.setLayout(self.contentLayout)

        # style for the label
        titleLabel.setStyleSheet('''.QLabel {
            color: #3E6AAA;
        }''') 

        # style for the content panel
        contentPanel.setStyleSheet('''.QWidget { 
            border-bottom-left-radius: 5px;  
            border-bottom-right-radius: 5px;
            background-color: #D6E5F3; 
        }''')

        # style for the whole toolbar
        self.setStyleSheet('''TitledToolbar { 
            background-color:#C1D9F1; 
            color:#6D6AB7; 
            border-radius: 5px;  
            border-style: solid; 
            border-width: 1px; 
            border-color: #6D6AB7;
        }''')

        myLayout = QVBoxLayout()
        myLayout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(myLayout)

        myLayout.addWidget(titleLabel, 0, Qt.AlignHCenter | Qt.AlignTop)
        myLayout.addWidget(contentPanel)


    def addWidget(self, wdg):
        self.contentLayout.addWidget(wdg)


class BlockStyleSelector(TitledToolbar):

    styleChanged = pyqtSignal(str)

    def __init__(self, parent):
        TitledToolbar.__init__(self, parent, 'Block style')
        self.currentStyle = None

        self.styleToIndex = {}
        self.styleToDropdown = {}

        d2 = IconDropDown(self, "icons/format-text-header.png")
        d2.entrySelected.connect(self.styleSelected)
        d2.addItem('Header 1', 'h1')
        self.styleToDropdown['h1'] = d2
        self.styleToIndex['h1'] = 0
        d2.addItem('Header 2', 'h2')
        self.styleToDropdown['h2'] = d2
        self.styleToIndex['h2'] = 1
        d2.addItem('Header 3', 'h3')
        self.styleToDropdown['h3'] = d2
        self.styleToIndex['h3'] = 2

        d1 = IconDropDown(self, "icons/format-text-paragraph.png")
        d1.entrySelected.connect(self.styleSelected)
        d1.addItem("Standard", 'p')
        self.styleToDropdown['p'] = d1
        self.styleToIndex['p'] = 0
        d1.addItem("Centered", 'pcenter')
        self.styleToDropdown['pcenter'] = d1
        self.styleToIndex['pcenter'] = 1
        d1.addItem("Block", 'pblock')
        self.styleToDropdown['pblock'] = d1
        self.styleToIndex['pblock'] = 2

        d3 = IconDropDown(self, "icons/format-list-unordered.png")
        d3.entrySelected.connect(self.styleSelected)
        d3.addItem("Unordered List", 'ul1')
        self.styleToDropdown['u1l'] = d3
        self.styleToIndex['ul1'] = 0
        d3.addItem("Ordered List", 'ol1')
        self.styleToDropdown['ol1'] = d3
        self.styleToIndex['ol1'] = 1

        d4 = IconDropDown(self, "icons/format-text-code.png")
        d4.entrySelected.connect(self.styleSelected)
        d4.addItem("C++", 'cppcode')
        self.styleToDropdown['cppcode'] = d4
        self.styleToIndex['cppcode'] = 0
        d4.addItem("Java", 'javacode')
        self.styleToDropdown['javacode'] = d4
        self.styleToIndex['javacode'] = 1
        d4.addItem("Python", 'pycode')
        self.styleToDropdown['pycode'] = d4
        self.styleToIndex['pycode'] = 2
        d4.addItem("SQL", 'sqlcode')
        self.styleToDropdown['sqlcode'] = d4
        self.styleToIndex['sqlcode'] = 3
        d4.addItem("XML", 'xmlcode')
        self.styleToDropdown['xmlcode'] = d4
        self.styleToIndex['xmlcode'] = 4
        d4.addItem("Generic", 'pre')
        self.styleToDropdown['pre'] = d4
        self.styleToIndex['pre'] = 5

        self.addWidget(d1)
        self.addWidget(d2)
        self.addWidget(d3)
        self.addWidget(d4)


    def addCategory(self):
        pass
    

    def addStyle(self):
        pass


    def styleSelected(self):
        icd = self.sender()
        dd = icd.dropdown
        style = dd.currentData()
        self.styleChanged.emit(style)


    def setCurrentStyle(self, styleName):
        if self.currentStyle != styleName:
            self.currentStyle = styleName
            for dd in self.styleToDropdown.values():
                dd.setCurrentIndex(-1)
            if styleName is not None:
                dd = self.styleToDropdown[styleName]
                di = self.styleToIndex[styleName]
                if dd is not None and di is not None:
                    dd.setCurrentIndex(di)


class TextStyleSelector(TitledToolbar):

    styleChanged = pyqtSignal(str, bool)

    def __init__(self, toolbar):
        TitledToolbar.__init__(self, toolbar, 'Text style')
        self.currentStyle = None

        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)

        self.styleToAction = {}

        textKeywordAction = QAction(QIcon("icons/format-keyword.png"), "Notepad link", toolbar)
        textKeywordAction.setCheckable(True);
        textKeywordAction.setProperty('style', 'keyword')
        self.styleToAction['keyword'] = textKeywordAction
        textKeywordAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textKeywordAction)

        textLinkAction = QAction(QIcon("icons/format-link.png"), "Internet link", toolbar)
        textLinkAction.setCheckable(True);
        textLinkAction.setProperty('style', 'a')
        self.styleToAction['a'] = textLinkAction 
        textLinkAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textLinkAction)

        textBoldAction = QAction(QIcon("icons/format-text-emphasized.png"), "Emphasize", toolbar)
        textBoldAction.setCheckable(True);
        textBoldAction.setProperty('style', 'strong')
        self.styleToAction['strong'] = textBoldAction
        textBoldAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textBoldAction)

        textHighlightAction = QAction(QIcon("icons/format-text-highlight.png"), "Highlight", toolbar)
        textHighlightAction.setCheckable(True);
        textHighlightAction.setProperty('style', 'em')
        self.styleToAction['em'] = textHighlightAction
        textHighlightAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textHighlightAction)
 
        textCodeAction = QAction(QIcon("icons/format-text-code.png"), "Code", toolbar)
        textCodeAction.setCheckable(True);
        textCodeAction.setProperty('style', 'tt')
        self.styleToAction['tt'] = textCodeAction
        textCodeAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textCodeAction)

        self.addWidget(toolbar)


    def styleSelected(self):
        action = self.sender()
        style = action.property('style')
        flag = action.isChecked()
        self.styleChanged.emit(style, flag)


    def setCurrentStyle(self, styleName):
        if self.currentStyle != styleName:
            self.currentStyle = styleName
            for action in self.styleToAction.values():
                action.setChecked(False)
            if styleName is not None:
                action = self.styleToAction[styleName]
                if action is not None:
                    action.setChecked(True)


class ActionSelector(TitledToolbar):

    def __init__(self, parent):
        TitledToolbar.__init__(self, parent, 'Actions')
 
        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)

        self.saveAction = QAction(QIcon("icons/file-save.png"), "Save (Ctrl-S)", toolbar)
        self.saveAction.setShortcut(Qt.CTRL + Qt.Key_S);
        self.saveAction.triggered.connect(parent.save)
        toolbar.addAction(self.saveAction)

        self.nonprintableAction = QAction(QIcon("icons/view-nonprintable.png"), "View nonprintable chars", toolbar)
        self.nonprintableAction.setCheckable(True);
        self.nonprintableAction.triggered.connect(parent.toggleNonprintable)
        toolbar.addAction(self.nonprintableAction)

        self.undoAction = QAction(QIcon("icons/edit-undo.png"), "Undo (Ctrl-Z)", toolbar)
        # saveAction.setShortcut(Qt.CTRL + Qt.Key_Z);
        self.undoAction.triggered.connect(parent.undo)
        toolbar.addAction(self.undoAction)

        self.redoAction = QAction(QIcon("icons/edit-redo.png"), "Redo (Ctrl-Y)", toolbar)
        # saveAction.setShortcut(Qt.CTRL + Qt.Key_Y);
        self.redoAction.triggered.connect(parent.redo)
        toolbar.addAction(self.redoAction)

        self.backAction = QAction(QIcon("icons/view-back.png"), "Back", toolbar)
        self.backAction.setEnabled(False)
        self.backAction.triggered.connect(parent.navigateBack)
        toolbar.addAction(self.backAction)

        self.forwardAction = QAction(QIcon("icons/view-forward.png"), "Forward", toolbar)
        self.forwardAction.setEnabled(False)
        self.forwardAction.triggered.connect(parent.navigateForward)
        toolbar.addAction(self.forwardAction)

        insertImageAction = QAction(QIcon("icons/edit-insert-image.png"), "Insert Image", toolbar)
        insertImageAction.triggered.connect(parent.insertImage)
        toolbar.addAction(insertImageAction)

        self.addWidget(toolbar)


class EditorWidget(QWidget):
    message = pyqtSignal(str)
    titleChanged = pyqtSignal(str)
    navigate = pyqtSignal(str)


    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.formatManager = FormatManager()
        self.formatManager.loadFormats()

        self.currentList = None
        self.l = UrlEditor(self)

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
        layout.addWidget(self.textFormatSelector)

        self.styleSelector = BlockStyleSelector(self)
        self.styleSelector.styleChanged.connect(self.blockStyleChanged)
        layout.addWidget(self.styleSelector)

        horizontalSpacer = QSpacerItem(40, 20, QSizePolicy.Expanding, QSizePolicy.Minimum)
        layout.addItem(horizontalSpacer)

        self.editView.cursorPositionChanged.connect(self.cursorPositionChanged)
        # self.editView.currentCharFormatChanged.connect(self.currentCharFormatChanged)

        hLayout = QVBoxLayout(self)
        hLayout.setContentsMargins(0, 0, 0, 0)
        hLayout.addWidget(toolbar)
        hLayout.addWidget(self.editView)

        # The toolbar should take the minimum space and the edit view the remaining space
        hLayout.setStretch(0, 0)
        hLayout.setStretch(1, 1)


    def cursorPositionChanged(self):
        cursor = self.editView.textCursor()

        # update block format toolbar
        blockFmt = cursor.blockFormat()
        blockStyle = blockFmt.property(QTextFormat.UserProperty)
        self.styleSelector.setCurrentStyle(blockStyle)

        # update text format toolbar
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position
        charStyle = charFmt.property(QTextFormat.UserProperty)
        self.textFormatSelector.setCurrentStyle(charStyle)

        # open/close URL editor for external links
        if charStyle == 'a':
            url = charFmt.anchorHref()
            if QGuiApplication.keyboardModifiers() & Qt.ControlModifier:
                print("OPEN: '{}'".format(url))
            else:
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
        self.cursorPositionChanged()    # update toolbars

        self.contentFile = os.path.join(notepad.getName(),str(pageId))
        self.message.emit("Loaded %s" % self.contentFile)


    def updateWindowTitle(self, flag):
        self.actionsSelector.saveAction.setEnabled(flag)
        self.titleChanged.emit('{}{}: {}'.format('*' if flag else '', self.page.notepad.getRootpath(), self.page.getName()))


## Actions

    def save(self):
        if self.editView.document().isModified():
            self.page.save()
            self.message.emit("Saved %s" % self.contentFile)


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
        fmt = self.formatManager.getFormat('p').getCharFormat()
        cursor = self.editView.textCursor()
        self.editView.selectCurrentFragment(cursor)
        cursor.setCharFormat(fmt)


## Block format

    def blockStyleChanged(self, style):
        pos = self.editView.textCursor().position()

        if style in ['javacode', 'sqlcode', 'cppcode', 'pycode', 'xmlcode', 'pre']:
            self.textCode(style)
        else:
            self.reformatBlock(style)

        self.editView.textCursor().setPosition(pos)


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
        if style in ['ul1', 'ol1']:
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
