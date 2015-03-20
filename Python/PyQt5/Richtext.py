#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtCore import PYQT_VERSION_STR, QT_VERSION_STR, qVersion, pyqtSignal
from PyQt5.QtWidgets import QWidget, QTabWidget
from PyQt5.QtWidgets import QTextEdit, QSplitter, QHBoxLayout, QVBoxLayout, QMainWindow
from PyQt5.QtWidgets import QAction, QStatusBar, QMenuBar, QApplication, QMessageBox
from PyQt5.QtWebKitWidgets import QWebView
from PyQt5.QtCore import QUrl 

import sys
import logging.config

from EditorWidget import EditorWidget
from BrowserWidget import BrowserWidget
from Settings import Settings
from TextDocumentTraversal import TextDocumentPrinter, TextXMLPrinter, TextHTMLPrinter


class MynPad(QWidget):

    updateWindowTitle = pyqtSignal(str)

    def __init__(self, parentWidget, settings):
        QWidget.__init__(self, parentWidget)

        self.settings = settings
        self.theLayout = QHBoxLayout()
        self.splitter = QSplitter(self)
        self.theLayout.addWidget(self.splitter)

        self.browserWidget = BrowserWidget(self, self.settings)
        self.browserWidget.itemSelected.connect(self.itemSelected)

        self.tabWidget = QTabWidget(self)

        tab = QWidget()

        tabLayout = QVBoxLayout(tab)
        tab.setLayout(tabLayout)

        self.editorWidget = EditorWidget(tab)

        self.editorWidget.setObjectName("EditorWidget1")
        self.editorWidget.message.connect(self.showMessage)
        self.editorWidget.titleChanged.connect(self.updateWindowTitle)
        self.editorWidget.navigate.connect(self.navigate)

        tabLayout.addWidget(self.editorWidget)
        self.tabWidget.addTab(tab, "Edit")
#############################

        self.browser = QWebView(self.tabWidget)
        self.tabWidget.addTab(self.browser, "View web")

        self.tabWidget.addTab(QWidget(self.tabWidget), "View pdf")

        self.textView = QTextEdit(self.tabWidget)
        self.textView.setReadOnly(True)
        self.textView.setFontFamily('Courier')
        self.textView.setFontPointSize(10)
        self.tabWidget.addTab(self.textView, "View document structure")

        self.customView = QTextEdit(self.tabWidget)
        self.customView.setReadOnly(True)
        self.customView.setFontFamily('Courier')
        self.customView.setFontPointSize(10)
        self.tabWidget.addTab(self.customView, "View XML")

        self.htmlView = QTextEdit(self.tabWidget)
        self.htmlView.setReadOnly(True)
        self.htmlView.setFontFamily('Courier')
        self.htmlView.setFontPointSize(10)
        self.tabWidget.addTab(self.htmlView, "View Html")

        self.tabWidget.currentChanged.connect(self.tabSelected)

        self.splitter.addWidget(self.browserWidget)
        self.splitter.addWidget(self.tabWidget)

        self.setLayout(self.theLayout)
        self.splitter.setSizes([100, 400])
        # self.splitter.setChildrenCollapsible(False)
        self.editorWidget.setEnabled(False)


    def showMessage(self, message):
        self.parent().statusBar.showMessage(message, 3000)


    def navigate(self, pageId):
        print('Navigating to "{}"'.format(pageId))
        self.editorWidget.save()
        self.editorWidget.load(self.editorWidget.page.notepad, pageId)

        self.browserWidget.navigate(pageId)


    def itemSelected(self):
        treeNode = self.browserWidget.currentItem
        print("Selected tree node: {}".format(treeNode))
        self.editorWidget.setEnabled(True)

        self.editorWidget.save()
        if treeNode.parent() is None:
            # Load title page
            self.editorWidget.load(treeNode.getNotepad(), None)
        else:
            # Load normal page
            self.editorWidget.load(treeNode.getNotepad(), treeNode.getLabel())


    def tabSelected(self, index):
        if index == 1:        # Web View
            self.htmlView.clear()
            doc = self.editorWidget.editView.document()
            traversal = TextHTMLPrinter(self.htmlView.insertPlainText, self.editorWidget.page)
            traversal.traverse(doc)

            ########### set stylesheet
            import os
            mypath = os.getcwd()
            mypath = mypath.replace('\\', '/')
            self.browser.settings().setUserStyleSheetUrl(QUrl('file:///{}/webpage.css'.format(mypath)))
            ###########

            self.browser.setHtml(self.htmlView.toPlainText())

        elif index == 2:      # PDF
            pass

        elif index == 3:
            self.dumpTextStructure()

        elif index == 4:
            self.customView.clear()

            doc = self.editorWidget.editView.document()
            traversal = TextXMLPrinter(self.customView.insertPlainText)
            traversal.traverse(doc)

        elif index == 5:
            self.htmlView.clear()

            doc = self.editorWidget.editView.document()
            traversal = TextHTMLPrinter(self.htmlView.insertPlainText, self.editorWidget.page)
            traversal.traverse(doc)


    def blocks(self, frame):
        blocks = frame.begin()
        while not blocks.atEnd():
            block = blocks.currentBlock()
            yield block
            blocks += 1


    def dumpTextStructure(self):
        self.textView.clear()

        doc = self.editorWidget.editView.document()
        traversal = TextDocumentPrinter(self.textView.insertPlainText)
        traversal.traverse(doc)



#===============================================================================
#         for block in self.blocks(frm):
#             bfmt = block.blockFormat()
#             style = bfmt.property(QTextFormat.UserProperty)
# 
#             fmtType = 'B'
#             # if bfmt.isListFormat():
#             if block.textList() is not None:
#                 fmtType = 'L({})'.format(block.textList().count())
# 
#             self.textView.append('  {}&lt;{}&gt;: <span style="background: yellow">{}</span>\n'.format(fmtType, style, block.text()))
#===============================================================================

        


class MainWindow(QMainWindow):

    l = logging.getLogger('MainWindow')

    def __init__(self, app):
        QMainWindow.__init__(self, None)
        
        self.l.debug('Initializing MainWindow ...')
        
        self.setWindowTitle("MynPad")
        self.theApplication = app
        app.aboutToQuit.connect(self.saveState)

        # read the local configuration file
        self.settings = Settings()
        self.settings.load()

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
        self.mainWidget = MynPad(self, self.settings)
        self.mainWidget.updateWindowTitle.connect(self.updateWindowTitle)
        self.setCentralWidget(self.mainWidget)

        # Reset main window size and position
        pos = self.settings.getMainWindowPos()
        self.move(pos.x(), pos.y())
        size = self.settings.getMainWindowSize()
        self.resize(size)

        # refresh the browser tree 
        self.mainWidget.browserWidget.refresh()


    def updateWindowTitle(self, title):
        self.setWindowTitle('{} - MynPad'.format(title))


    def saveState(self):
        # Make sure that the current notepad page is saved
        self.mainWidget.editorWidget.save()

        # Note: there is no way to have eclipse shutdown the application faithfully,
        # see also http://stackoverflow.com/questions/677531/is-it-possible-for-eclipse-to-terminate-gently-instead-of-using-sigkill
        path = self.mainWidget.browserWidget.getCurrentPath()
        self.settings.setBrowserPath(path)
        self.settings.setMainWindowPos(self.pos())
        self.settings.setMainWindowSize(self.size())
        self.settings.save()


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
    logging.config.fileConfig('logging.ini')

    # Create the application object
    app = QApplication(sys.argv)

    # Create the main window
    mainWindow = MainWindow(app)

    # Show and run the application
    mainWindow.show()
    app.exec()



if __name__ == '__main__':
    main()
