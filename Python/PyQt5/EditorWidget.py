'''
Created on 18.02.2015

@author: afester
'''

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import os

class UserData(QTextBlockUserData):
    
    def __init__(self, data):
        QTextBlockUserData.__init__(self)

        self.data = data
        
    def getData(self):
        return self.data


class EditorWidget(QWidget):
    message = pyqtSignal(str)

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

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

        textKeywordAction = QAction(QIcon("icons/format-keyword.png"), "Keyword", toolbar)
        textKeywordAction.triggered.connect(self.textKeyword)
        toolbar.addAction(textKeywordAction)

        textBoldAction = QAction(QIcon("icons/format-text-bold.png"), "Bold", toolbar)
        textBoldAction.triggered.connect(self.textBold)
        toolbar.addAction(textBoldAction)

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

        self.editView = QTextEdit(self)
        doc = self.editView.document()
        css = "h1 { color: red; } "
        doc.addResource( QTextDocument.StyleSheetResource, QUrl( "format.css" ), css );
        # self.editView.setStyleSheet("h1 { color: red; } ")

        self.editView.cursorPositionChanged.connect(self.cursorPositionChanged)
        self.editView.currentCharFormatChanged.connect(self.currentCharFormatChanged)

        hLayout = QVBoxLayout(self)
        hLayout.addWidget(toolbar)
        hLayout.addWidget(self.editView)


    def currentCharFormatChanged(self, fmt):
        print("Char format: %s / %d" % (fmt.fontFamily(), fmt.fontWeight()))  

    def cursorPositionChanged(self):
        print("Cursor repositioned")
        cursor = self.editView.textCursor()
        block = cursor.block()
        print("  Tag:" + str(block.userData()))


    def load(self, path):
        self.contentFile = os.path.join(*path)
        self.contentFile = os.path.join(self.contentFile, 'content.html')

        # NOTE: HTML can be loaded with styles attached, but when written back 
        # with toHtml the resulting HTML code does not contain the style information
        # anymore.
        #self.editView.setHtml('''
        #<html><head>
        #     <meta http-equiv='Content-Type' content='text/html;charset=utf-8'/>
        #     <link rel='stylesheet' type='text/css' href='format.css'>
        #     </head>
        #<body>
        #<h1>Title</h1>
        #</body></html>''')

        #doc = self.editView.document()
        #css = "h1 { color: red; } "
        #doc.addResource( QTextDocument.StyleSheetResource, QUrl( "format.css" ), css );

        #return

        with open(self.contentFile, 'r') as content_file:
            content = content_file.read()
            self.editView.setHtml(content)
        self.message.emit("Loaded %s" % self.contentFile)


    def save(self):
        with open(self.contentFile, 'w') as content_file:
            # NOTE: toHtml() writes the text document in a "hard coded"
            # HTML format, converting all style properties to inline style="..." attributes
            # See QTextHtmlExporter in qtbase/src/gui/text/qtextdocument.cpp
            #content_file.write(self.editView.toHtml())
            content_file.write(self.externalize())
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
        fmt = QTextCharFormat()
        fmt.setForeground(Qt.red)
        fmt.setAnchor(True)

        cursor = self.editView.textCursor()
        if not cursor.hasSelection():
            cursor.select(QTextCursor.WordUnderCursor)
        fmt.setAnchorHref(cursor.selectedText())

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
            cursor = self.editView.textCursor()
            self.selectWholeBlocks(cursor)
            content = cursor.selection().toPlainText()
            cursor.removeSelectedText()
            cursor.deleteChar() # also delete the block http://stackoverflow.com/questions/16996679/remove-block-from-qtextdocument
            cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))

        elif idx == 3:
            cursor = self.editView.textCursor()
            self.selectWholeBlocks(cursor)
            content = cursor.selection().toPlainText()
            cursor.removeSelectedText()
            cursor.insertFragment(QTextDocumentFragment.fromHtml("<h1>" + content + "</h1>"))
            
            # NOTE: user data is neither persisted, nor retained when copy&pasting!
            cursor.block().setUserData(UserData("h1"))
            # cursor.blockFormat().setProperty(QTextFormat.UserProperty, "h1")

        elif idx == 4:
            cursor = self.editView.textCursor()
            self.selectWholeBlocks(cursor)
            content = cursor.selection().toPlainText()
            cursor.removeSelectedText()
            cursor.insertFragment(QTextDocumentFragment.fromHtml("<h2>" + content + "</h2>"))
            cursor.block().setUserData(UserData("h2"))

        elif idx == 5:
            cursor = self.editView.textCursor()
            self.selectWholeBlocks(cursor)
            content = cursor.selection().toPlainText()
            cursor.removeSelectedText()
            cursor.insertFragment(QTextDocumentFragment.fromHtml("<h3>" + content + "</h3>"))
            cursor.block().setUserData(UserData("h3"))

        elif idx == 6:
            content = ""
            for block in self.selectedBlocks():
                content = content + block.text() + "\n" # \u2028"

            cursor = self.editView.textCursor()
            self.selectWholeBlocks(cursor)
            # content = cursor.selection().toPlainText()
            cursor.removeSelectedText()
            fmt = QTextFrameFormat()
            fmt.setBorder(2.0)
            frame = cursor.insertFrame(fmt)
            cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))
            cursor.block().setUserData(UserData("javacode"))

        else:
            print("Formatting with %d" % idx)




    def externalize(self):
        # self.result = "<?xml version = '1.0' encoding = 'UTF-8'?>\n<page>\n"

        self.result = "<html>\n"
        document = self.editView.document()
        textBlock = document.firstBlock()
        while textBlock.isValid():
            self.emitTextBlock(textBlock)
            textBlock = textBlock.next()

        self.result = self.result + "</html>"
        return self.result


    def emitTextBlock(self, textBlock):
        userData = None
        if textBlock.userData():
            userData = textBlock.userData().getData()
        print("USERDATA:" + str(userData))

        print("USERPROPERTY:" + str(textBlock.blockFormat().property(QTextFormat.UserProperty)))

        if userData is None:
            self.emitParagraph(textBlock)
        elif userData == "h1":
            self.result = self.result + "\n   <h1>" + textBlock.text() + "</h1>\n"
        elif userData == "h2":
            self.result = self.result + "\n   <h2>" + textBlock.text() + "</h2>\n"
        elif userData == "h3":
            self.result = self.result + "\n   <h3>" + textBlock.text() + "</h3>\n"
        elif userData == "javacode":
            self.result = self.result + "\n   <pre class=\"java\">" + textBlock.text() + "</pre>\n"


    def emitParagraph(self, textBlock):

        self.result = self.result + "   <p>"

        fragments = textBlock.begin()
        while not fragments.atEnd():
            fragment = fragments.fragment()
            text = fragment.text()
            charFormat = fragment.charFormat()
            if charFormat.fontWeight() == QFont.Bold:
                self.result = self.result + "<em>" + text + "</em>"
            else:
                self.result = self.result + text
            fragments += 1

        self.result = self.result + "</p>\n"
