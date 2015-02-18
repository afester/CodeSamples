#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import sys, os
from _tracemalloc import start


class BrowserWidget(QTreeWidget):

    itemSelected = pyqtSignal()

    def __init__(self, parentWidget):
        QTreeWidget.__init__(self, parentWidget)
        self.setColumnCount(1)
        self.setHeaderLabel("Sections")
        self.itemSelectionChanged.connect(self.handleItemSelected)


    def handleItemSelected(self):
        selItems = self.selectedItems()
        if len(selItems) == 1:
            self.currentItem = selItems[0]
            self.itemSelected.emit()


    # platform independant path.split function
    # See http://stackoverflow.com/questions/4579908/cross-platform-splitting-of-path-in-python
    def os_path_split(self, path, debug=False):
        parts = []
        while True:
            newpath, tail = os.path.split(path)
            if debug: print(repr(path), (newpath, tail))
            if newpath == path:
                assert not tail
                if path: parts.append(path)
                break
            parts.append(tail)
            path = newpath
        parts.reverse()
        return parts


    def refresh(self):
        self.rootNodes = []

        for root, dirs, files in os.walk('SampleWiki'):
            dirs.sort()
            for d in dirs:
                path = os.path.join(root, d)
                path = self.os_path_split(path)
                self.addPath(path)

        self.addTopLevelItems(self.rootNodes)

        for node in self.rootNodes:
            node.setExpanded(True)


    def addPath(self, path):
        rootElement = path[0]
        subPath = path[1:]

        # Lookup the root element
        rootItem = None
        for treeWidgetItem in self.rootNodes:
            label = treeWidgetItem.text(0)
            if label == rootElement:
                rootItem = treeWidgetItem
                break

        # Create the root element
        if rootItem is None:
            rootItem = QTreeWidgetItem([rootElement])
            self.rootNodes.append(rootItem)

        # add the remaining child path
        currentItem = rootItem
        for element in subPath:

            # Lookup the current element in the child items of the current node
            foundItem = None
            for idx in range(0, currentItem.childCount()):
                childItem = currentItem.child(idx)
                label = childItem.text(0)
                if label == element:
                    foundItem = childItem
                    break
            if foundItem is None:
                foundItem = QTreeWidgetItem([element])
                currentItem.addChild(foundItem)
            currentItem = foundItem

    def getCurrentPath(self):
        result = [self.currentItem.text(0)]
        parent = self.currentItem.parent()
        while parent is not None:
            result.insert(0, parent.text(0))
            parent = parent.parent()
        return result 



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

        hLayout = QVBoxLayout(self)
        hLayout.addWidget(toolbar)
        hLayout.addWidget(self.editView)


    def load(self, path):
        self.contentFile = os.path.join(*path)
        self.contentFile = os.path.join(self.contentFile, 'content.html')
        with open(self.contentFile, 'r') as content_file:
            content = content_file.read()
            self.editView.setHtml(content)
        self.message.emit("Loaded %s" % self.contentFile)


    def save(self):
        with open(self.contentFile, 'w') as content_file:
            content_file.write(self.editView.toHtml())
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
            result = result + block.text() + "\u2028"

        #fragment = QTextDocumentFragment.fromPlainText(result)
        
        fragment = QTextDocumentFragment.fromPlainText(result)
        cursor = self.editView.textCursor()
        #document = self.editView.document()

        cursor.insertBlock();
        cursor.insertFragment(fragment);

        #startBlock = document.findBlock(cursor.selectionStart())
        #startBlock.setPlainText(result)
        print(result)


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

        cursor = self.textView.textCursor()
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

        elif idx == 4:
            cursor = self.editView.textCursor()
            self.selectWholeBlocks(cursor)
            content = cursor.selection().toPlainText()
            cursor.removeSelectedText()
            cursor.insertFragment(QTextDocumentFragment.fromHtml("<h2>" + content + "</h2>"))

        elif idx == 5:
            content = ""
            for block in self.selectedBlocks():
                content = content + block.text() + "\u2028"

            cursor = self.editView.textCursor()
            self.selectWholeBlocks(cursor)
            # content = cursor.selection().toPlainText()
            cursor.removeSelectedText()
            cursor.insertFragment(QTextDocumentFragment.fromPlainText(content))

        else:
            print("Formatting with %d" % idx)





class RichtextSampleWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QHBoxLayout()
        self.splitter = QSplitter(self)
        self.theLayout.addWidget(self.splitter)

        self.leftWidget = BrowserWidget(self)
        self.leftWidget.itemSelected.connect(self.itemSelected)

        self.rightWidget = QTabWidget(self)

        tab = QWidget()

        tabLayout = QVBoxLayout(tab)
        tab.setLayout(tabLayout)

        self.editorWidget = EditorWidget(tab)

        self.editorWidget.setObjectName("EditorWidget1")
        self.editorWidget.message.connect(self.showMessage)

        tabLayout.addWidget(self.editorWidget)
        self.rightWidget.addTab(tab, "Edit")
#############################

        self.rightWidget.addTab(QWidget(self.rightWidget), "View web")

        self.rightWidget.addTab(QWidget(self.rightWidget), "View pdf")

        self.htmlView = QTextEdit(self.rightWidget)
        self.htmlView.setReadOnly(True)
        self.rightWidget.addTab(self.htmlView, "View html")

        self.textView = QTextEdit(self.rightWidget)
        self.textView.setReadOnly(True)
        self.rightWidget.addTab(self.textView, "View plaintext")

        self.customView = QTextEdit(self.rightWidget)
        self.customView.setReadOnly(True)
        self.rightWidget.addTab(self.customView, "View XML")

        self.rightWidget.currentChanged.connect(self.tabSelected)

        self.splitter.addWidget(self.leftWidget)

        #self.rightLayoutWidget = QHBoxLayout()
        #self.rightLayoutWidget.addWidget(self.rightWidget)
        #self.rightW = QWidget(self.splitter)
        #self.rightW.setLayout(self.rightLayoutWidget)
        self.splitter.addWidget(self.rightWidget)

        self.setLayout(self.theLayout)
        self.splitter.setSizes([100, 400])

        self.leftWidget.refresh()
        self.editorWidget.load(['SampleWiki'])

    def showMessage(self, message):
        self.parent().statusBar.showMessage(message, 3000)

    def itemSelected(self):
        path = self.leftWidget.getCurrentPath()
        # path = os.path.join('SampleWiki', *path)
        self.load(path)


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



    def tabSelected(self, index):
        if index == 3:
            self.htmlView.setPlainText(self.editView.toHtml())
        elif index == 4:
            self.textView.setPlainText(self.editView.toPlainText())
        elif index == 5:
            self.updateCustomFormat()
            

    def updateCustomFormat(self):
        self.result = "<?xml version = '1.0' encoding = 'UTF-8'?>\n<article>\n"

        document = self.editView.document()
        textBlock = document.firstBlock()
        while textBlock.isValid():
            self.emitTextBlock(textBlock)
            textBlock = textBlock.next()

        self.result = self.result + "</article>"

        self.customView.setPlainText(self.result)


    def emitTextBlock(self, textBlock):
        self.result = self.result + "   <p>"

        fragments = textBlock.begin()
        while not fragments.atEnd():
            fragment = fragments.fragment()
            text = fragment.text()
            charFormat = fragment.charFormat()
            if charFormat.fontWeight() == QFont.Bold:
                self.result = self.result + "<b>" + fragment.text() + "</b>"
            else:
                self.result = self.result + fragment.text()
            fragments += 1

        self.result = self.result + "</p>\n"


class MainWindow(QMainWindow):

    def __init__(self, app):
        QMainWindow.__init__(self, None)
        self.setWindowTitle("Richtext editor sample")
        self.theApplication = app

        # Set up the menu bar
        menuBar = QMenuBar(self)

        exitAction = QAction("Exit", self, triggered=self.theApplication.exit)
        fileMenu = menuBar.addMenu("&File")
        fileMenu.addAction(exitAction)

        aboutAction = QAction("About ...", self, triggered = self.handleAbout)
        helpMenu = menuBar.addMenu("&Help")
        helpMenu.addAction(aboutAction)

        self.setMenuBar(menuBar)

        # Setup the status bar
        self.statusBar = QStatusBar()
        self.statusBar.showMessage("Ready.")
        self.setStatusBar(self.statusBar)

        self.mainWidget = RichtextSampleWidget(self)
        self.setCentralWidget(self.mainWidget)

        self.move(240, 200)
        self.resize(QSize(1200, 700))


    def handleAbout(self):
        appVersion = "Development version"
        pythonVersion = "%s.%s.%s (%s)" % (sys.version_info[0], sys.version_info[1], sys.version_info[2], sys.version_info[3])
        pyQtVersion = PYQT_VERSION_STR
        pyQtQtVersion = QT_VERSION_STR
        qtRuntimeVersion = qVersion()

        QMessageBox.about(self, "About udev Device browser",
                          "<table>"+
                          "<tr><th align=\"right\">Application version:</th><td>{}</td></tr>".format(appVersion) +
                          "<tr><th align=\"right\">Python version:</th><td>{}</td></tr>".format(pythonVersion) +
                          "<tr><th align=\"right\">PyQt version:</th><td>{} for Qt {}</td></tr>".format(pyQtVersion, pyQtQtVersion) +
                          "<tr><th align=\"right\">Qt runtime version:</th><td>{}</td></tr>".format(qtRuntimeVersion)+
                          "</table>")





def main():
    # Create the application object
    app = QApplication(sys.argv)
    #app.setStyleSheet("QWidget#EditorWidget1 { background-color: green; border: none; }")

    # Create the main window
    mainWindow = MainWindow(app)

    # Show and run the application
    mainWindow.show()
    app.exec()



if __name__ == '__main__':
    main()
