'''
Created on 18.02.2015

@author: afester
'''

from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtWidgets import QToolBar, QWidget, QAction, QLabel, QComboBox
from PyQt5.QtWidgets import QTextEdit, QVBoxLayout, QLineEdit, QFrame
from PyQt5.QtGui import QGuiApplication, QIcon, QTextDocumentFragment, QTextCursor, QTextCharFormat 
from PyQt5.QtGui import QTextOption, QFont, QTextListFormat, QCursor

import os

from FormatManager import FormatManager


class UrlEditor(QFrame):

    def __init__(self, parentWidget, href):
        QFrame.__init__(self, parentWidget, Qt.Tool )
        layout = QVBoxLayout()
        self.setLayout(layout)
        
        self.editLine = QLineEdit(href, self)
        layout.addWidget(self.editLine)


class TextEdit(QTextEdit):

    def __init__(self, parentWidget):
        QTextEdit.__init__(self, parentWidget)


    def checkAnchorCursor(self, pos):
        cursor = self.cursorForPosition(pos)
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position
        if charFmt.isAnchor():
            self.viewport().setCursor(Qt.PointingHandCursor)
        else:
            self.viewport().setCursor(Qt.IBeamCursor)


    def mouseMoveEvent(self, event):
        self.checkAnchorCursor(event.pos())

        return QTextEdit.mouseMoveEvent(self, event)


    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Control:
            self.setMouseTracking(True)
            pos = self.mapFromGlobal(QCursor.pos())
            self.checkAnchorCursor(pos)

        return QTextEdit.keyPressEvent(self, event)


    def keyReleaseEvent(self, event):
        if event.key() == Qt.Key_Control:
            self.setMouseTracking(False)
            self.viewport().setCursor(Qt.IBeamCursor)

        return QTextEdit.keyReleaseEvent(self, event)



class EditorWidget(QWidget):
    message = pyqtSignal(str)
    navigate = pyqtSignal(int)
    l = None

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.formatManager = FormatManager()
        self.formatManager.loadFormats()

        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)

        saveAction = QAction(QIcon("icons/file-save.png"), "Save", toolbar)
        saveAction.setShortcut(Qt.CTRL + Qt.Key_S);
        saveAction.triggered.connect(self.save)
        toolbar.addAction(saveAction)

        self.nonprintableAction = QAction(QIcon("icons/view-nonprintable.png"), "View nonprintable chars", toolbar)
        self.nonprintableAction.setCheckable(True);
        self.nonprintableAction.triggered.connect(self.toggleNonprintable)
        toolbar.addAction(self.nonprintableAction)

        toolbar.addSeparator()

        textKeywordAction = QAction(QIcon("icons/format-keyword.png"), "Keyword link", toolbar)
        textKeywordAction.triggered.connect(self.textKeyword)
        toolbar.addAction(textKeywordAction)

        textLinkAction = QAction(QIcon("icons/format-link.png"), "Internet link", toolbar)
        #textKeywordAction.triggered.connect(self.textKeyword)
        toolbar.addAction(textLinkAction)

        textBoldAction = QAction(QIcon("icons/format-text-bold.png"), "Bold", toolbar)
        textBoldAction.triggered.connect(self.textBold)
        toolbar.addAction(textBoldAction)

        insertImageAction = QAction(QIcon("icons/edit-insert-image.png"), "Insert Image", toolbar)
        #textBoldAction.triggered.connect(self.textBold)
        toolbar.addAction(insertImageAction)

        toolbar.addSeparator()

        blockListAction = QAction(QIcon("icons/format-list-unordered.png"), "List", toolbar)
        blockListAction.triggered.connect(self.blockList)
        toolbar.addAction(blockListAction)

        toolbar.addAction(QAction(QIcon("icons/format-list-ordered.png"), "Numbered List", toolbar))

        decIndentAction = QAction(QIcon("icons/format-indent-less.png"), "Decrease indent", toolbar)
        decIndentAction.triggered.connect(self.decrementIndent)
        toolbar.addAction(decIndentAction)

        incIndentAction = QAction(QIcon("icons/format-indent-more.png"), "Incecrease indent", toolbar)
        incIndentAction.triggered.connect(self.incrementIndent)
        toolbar.addAction(incIndentAction)

        self.comboStyle = QComboBox(toolbar)
        toolbar.addWidget(QLabel("Block format:"))
        toolbar.addWidget(self.comboStyle)
        self.comboStyle.addItem("Paragraph")
        self.comboStyle.addItem("Paragraph centered")
        self.comboStyle.addItem("Paragraph block")
        self.comboStyle.addItem("Header 1")
        self.comboStyle.addItem("Header 2")
        self.comboStyle.addItem("Header 3")
        self.comboStyle.addItem("Code (Java)")
        self.comboStyle.addItem("Code (C++)")
        self.comboStyle.addItem("Code (SQL)")
        self.comboStyle.activated.connect(self.blockStyle)

        self.editView = TextEdit(self)
        #doc = self.editView.document()
        #css = "h1 { color: red; } "
        #doc.addResource( QTextDocument.StyleSheetResource, QUrl( "format.css" ), css );
        # self.editView.setStyleSheet("h1 { color: red; } ")

        self.editView.cursorPositionChanged.connect(self.cursorPositionChanged)
        self.editView.currentCharFormatChanged.connect(self.currentCharFormatChanged)

        hLayout = QVBoxLayout(self)
        hLayout.addWidget(toolbar)
        hLayout.addWidget(self.editView)


    def currentCharFormatChanged(self, fmt):
        #print("Char format: %s / %d" % (fmt.fontFamily(), fmt.fontWeight()))
        pass  


    def cursorPositionChanged(self):
        print("CURSOR POSITION CHANGED")

        cursor = self.editView.textCursor()
        charFmt = cursor.charFormat()   # get the QTextCharFormat at the current cursor position
        if charFmt.isAnchor():
            url = charFmt.anchorHref()
            if QGuiApplication.keyboardModifiers() & Qt.ControlModifier:
                if url.startswith("http://") or url.startswith("https://"):
                    print('OPEN EXTERNAL URL {}'.format(url))
                else:
                    pageId = int(url)
                    print("Navigate to page {}".format(pageId))
                    self.navigate.emit(pageId)
            else:
                if url.startswith("http://") or url.startswith("https://"):
                    print("Edit URL {}".format(url))
                    self.l = UrlEditor(self, url)
                    self.l.show()
        else:
            print("UNEDIT URL")
            if self.l:
                self.l.hide()

        print("LEAVE CURSOR POSITION CHANGED")


    def load(self, notepad, pageId):
        print('Loading page {} from notepad "{}"'.format(pageId, notepad.getName()))

        self.page = notepad.getPage(pageId)
        doc = self.page.getDocument()
        
        self.editView.blockSignals(True)
        self.editView.setDocument(doc)
        self.editView.blockSignals(False)

        self.editView.setFocus()
        self.contentFile = os.path.join(notepad.getName(),str(pageId))
        self.message.emit("Loaded %s" % self.contentFile)


    def save(self):
        self.page.save()
        self.message.emit("Saved %s" % self.contentFile)


    def incrementIndent(self):
        cursor = self.editView.textCursor()
        theList = cursor.currentList();
        if theList:
            listFmt = theList.format()
            listFmt.setIndent(listFmt.indent() + 1)
            theList.setFormat(listFmt)


    def decrementIndent(self):
        cursor = self.editView.textCursor()
        theList = cursor.currentList();
        if theList:
            listFmt = theList.format()
            listFmt.setIndent(listFmt.indent() - 1)
            theList.setFormat(listFmt)


    def textCode(self):
        # Create a plain text representation of the selected blocks
        result = '' # "Line1\u2028Line2\u2028Line3\u2028"
        for block in self.selectedBlocks():
            #result = result + block.text() + "\u2028"
            result = result + block.text() + "\n"

        #fragment = QTextDocumentFragment.fromPlainText(result)
        
        fragment = QTextDocumentFragment.fromPlainText(result)
        cursor = self.editView.textCursor()
        #document = self.editView.document()

        cursor.insertBlock();
        cursor.insertFragment(fragment);

        #startBlock = document.findBlock(cursor.selectionStart())
        #startBlock.setPlainText(result)
        print(result)


    def textKeyword(self):
        fmt = self.formatManager.getFormat("keyword").getCharFormat()

        cursor = self.editView.textCursor()
        if not cursor.hasSelection():
            cursor.select(QTextCursor.WordUnderCursor)

        # TODO: check if the selected text already exists as keyword!
        href = self.page.notepad.getNextPageId()
        fmt.setAnchorHref(str(href))

        cursor.mergeCharFormat(fmt);
        self.editView.mergeCurrentCharFormat(fmt);


    def toggleNonprintable(self):
        if self.nonprintableAction.isChecked():
            option = QTextOption()
            option.setFlags(QTextOption.ShowTabsAndSpaces | QTextOption.ShowLineAndParagraphSeparators)
            self.editView.document().setDefaultTextOption(option)
        else:
            option = QTextOption()
            # option.setFlags(QTextOption.ShowTabsAndSpaces | QTextOption.ShowLineAndParagraphSeparators)
            self.editView.document().setDefaultTextOption(option)


    def textBold(self):
        fmt = QTextCharFormat()
        fmt.setFontWeight(QFont.Bold) # actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);

        cursor = self.editView.textCursor()
        if not cursor.hasSelection():
            cursor.select(QTextCursor.WordUnderCursor)
        cursor.mergeCharFormat(fmt);
        self.editView.mergeCurrentCharFormat(fmt);


    def blockList(self):
        cursor = self.editView.textCursor()
        cursor.beginEditBlock()

        blockFmt = cursor.blockFormat()

        listFmt = QTextListFormat()
        if cursor.currentList():
            listFmt = cursor.currentList().format()
        else:
            listFmt.setIndent(blockFmt.indent() + 1)
            blockFmt.setIndent(0)
            cursor.setBlockFormat(blockFmt)

        listFmt.setStyle(QTextListFormat.ListDisc)

        cursor.createList(listFmt)

        cursor.endEditBlock()


    # Selects all blocks which are part of the current selection.
    def selectWholeBlocks(self, cursor):
        #cursor = self.editView.textCursor()

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

    def selectFrame(self, frame, cursor):
        start = frame.firstPosition()
        end = frame.lastPosition()

        print("%d/%d" % (start, end))

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
        

    def blockStyle(self, idx):
        if idx == 0:
            self.reformat('p')

        elif idx == 3:
            self.reformat('h1')

        elif idx == 4:
            self.reformat('h2')

        elif idx == 5:
            self.reformat('h3')

        elif idx == 6:
            content = ""
            for block in self.selectedBlocks():
                content = content + block.text() + "\u2028"

            fmt = self.formatManager.getFormat('code')
    
            cursor = self.editView.textCursor()
            self.selectWholeBlocks(cursor)
            cursor.removeSelectedText()
    
            cursor.setBlockFormat(fmt.getBlockFormat())
            cursor.setCharFormat(fmt.getCharFormat())
            cursor.insertText(content)

        else:
            print("Formatting with %d" % idx)

#
    def reformat(self, formatName):
        fmt = self.formatManager.getFormat(formatName)

        cursor = self.editView.textCursor()
        self.selectWholeBlocks(cursor)
        content = cursor.selection().toPlainText()
        cursor.removeSelectedText()

        cursor.setBlockFormat(fmt.getBlockFormat())
        cursor.setCharFormat(fmt.getCharFormat())
        cursor.insertText(content)
