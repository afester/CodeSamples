'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
import sys


class RichtextSampleWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QHBoxLayout()
        self.splitter = QSplitter(self)
        self.theLayout.addWidget(self.splitter)

        self.leftWidget = QTreeWidget(self)
        self.leftWidget.setColumnCount(1)
        self.leftWidget.setHeaderLabel("Sections")
        #self.leftWidget.itemSelected.connect(self.itemSelected)

        self.rightWidget = QTabWidget(self)
        #self.rightWidget.setColumnCount(2)
        #self.rightWidget.setHorizontalHeaderLabels(['Name', 'Value'])
        #self.rightWidget.verticalHeader().hide()


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

        self.loadLatest()


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
        result = "Line1\u2028Line2\u2028Line3\u2028"
        #for block in self.selectedBlocks():
        #    result = result + block.text() + "\r"
        
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
            self.result = self.result + "|"
            fragment = fragments.fragment()
            text = fragment.text()
            if len(text) > 4:
                print(text, len(text))
                print("!%04x!" %ord(text[5]))
            charFormat = fragment.charFormat()
            if charFormat.fontWeight() == QFont.Bold:
                self.result = self.result + "<b>" + fragment.text() + "</b>"
            else:
                self.result = self.result + fragment.text()
            fragments += 1
            self.result = self.result + "|"

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