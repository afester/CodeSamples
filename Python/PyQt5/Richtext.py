#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import sys, os


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


    def refresh(self):
        self.rootNodes = []

        for root, dirs, files in os.walk('SampleWiki'):
           dirs.sort()
           for d in dirs:
               path = os.path.join(root, d)
               path = path.split("/")
               path = path[1:]
               self.addPath(path)

        self.addTopLevelItems(self.rootNodes)
        

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

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)



class RichtextSampleWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QHBoxLayout()
        self.splitter = QSplitter(self)
        self.theLayout.addWidget(self.splitter)

        self.leftWidget = BrowserWidget(self)
        self.leftWidget.itemSelected.connect(self.itemSelected)

        self.rightWidget = QTabWidget(self)

        ###########################################
        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)
        toolbar.addAction(QAction(QIcon("icons/file-open.png"), "Open", toolbar))
        saveAction = QAction(QIcon("icons/file-save.png"), "Save", toolbar)
        saveAction.triggered.connect(self.save)
        toolbar.addAction(saveAction)
        toolbar.addSeparator()

        textBoldAction = QAction(QIcon("icons/format-text-bold.png"), "Bold", toolbar)
        textBoldAction.triggered.connect(self.textBold)
        toolbar.addAction(textBoldAction)

        toolbar.addAction(QAction(QIcon("icons/format-text-italic.png"), "Italic", toolbar))

        textCodeAction = QAction(QIcon("icons/format-text-code.png"), "Code", toolbar)
        textCodeAction.triggered.connect(self.textCode)
        toolbar.addAction(textCodeAction)

        self.editView = QTextEdit(self.rightWidget)

        option = QTextOption()
        option.setFlags(QTextOption.ShowTabsAndSpaces | QTextOption.ShowLineAndParagraphSeparators)
        self.editView.document().setDefaultTextOption(option)

        editWidget = QWidget(self)
        hLayout = QVBoxLayout(editWidget)
        editWidget.setLayout(hLayout)
        hLayout.addWidget(toolbar)
        hLayout.addWidget(self.editView)
        ###########################################


        self.rightWidget.addTab(editWidget, "Edit")


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
        self.loadLatest()


    def itemSelected(self):
        path = self.leftWidget.getCurrentPath()
        path = os.path.join('SampleWiki', *path)
        contentFile = os.path.join(path, 'content.html')
        with open(contentFile, 'r') as content_file:
            content = content_file.read()
            self.editView.setHtml(content)


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


    def textBold(self):
        fmt = QTextCharFormat()
        fmt.setFontWeight(QFont.Bold) # actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);

        cursor = self.textView.textCursor()
        if not cursor.hasSelection():
            cursor.select(QTextCursor.WordUnderCursor)
        cursor.mergeCharFormat(fmt);
        self.editView.mergeCurrentCharFormat(fmt);


    def loadLatest(self):
        with open('sample.txt', 'r') as content_file:
            content = content_file.read()
            self.editView.setHtml(content)

    def save(self):
        with open('sample.txt', 'w') as content_file:
            content_file.write(self.editView.toHtml())

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
    
    # Create the main window
    mainWindow = MainWindow(app)

    # Show and run the application
    mainWindow.show()
    app.exec()



if __name__ == '__main__':
    main()
