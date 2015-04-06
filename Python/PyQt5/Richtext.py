#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtCore import PYQT_VERSION_STR, QT_VERSION_STR, qVersion, pyqtSignal, Qt
from PyQt5.QtWidgets import QWidget, QTabWidget
from PyQt5.QtWidgets import QTextEdit, QSplitter, QHBoxLayout, QVBoxLayout, QMainWindow
from PyQt5.QtWidgets import QAction, QStatusBar, QMenuBar, QApplication, QMessageBox, QListView
from PyQt5.QtWebKitWidgets import QWebView
from PyQt5.QtCore import QUrl, QObject, QThread 
from PyQt5.QtGui import QStandardItem, QStandardItemModel, QIcon
from PyQt5 import uic

import sys, os, fnmatch
import logging.config

from EditorWidget import EditorWidget
from BrowserWidget import BrowserWidget
from Settings import Settings
from TextDocumentTraversal import TextDocumentPrinter, TextXMLPrinter, TextHTMLPrinter

import urllib, re

class SearchWorker(QObject):
    
    searchDone = pyqtSignal()
    addMatch = pyqtSignal(str)

    def __init__(self):
        QObject.__init__(self)

    def startSearch(self, rootPath, searchText):
        self.aborted = False
        print('Starting search operation "{}" on {} ...'.format(searchText, rootPath))

        # TODO: We need a Notepad instance here.
        # Then the following code to search through a notepad can be moved into the Notepad class:

        for root, dirnames, filenames in os.walk(rootPath):
            for filename in fnmatch.filter(filenames, '*.xml'):
                if self.aborted:
                    return

                pageId = urllib.parse.unquote(filename)[:-4]

                filename = os.path.join(root, filename)
                with open(filename, 'r', encoding='utf-8') as f:
                    contents = f.read()
                    
                    if re.search(searchText, contents, re.IGNORECASE):
                    # if contents.find(searchText) != -1:
                        self.addMatch.emit(pageId)
        self.searchDone.emit()


    def stopSearch(self):
        '''We can not call this method through a slot, since it would be 
        queued until the actual worker method is done'''

        print('Stopping search operation ...')
        self.aborted = True



class SearchWidget(QWidget):

    startWork = pyqtSignal(str, str)
    resultSelected = pyqtSignal(str)
    # stopWork = pyqtSignal()

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)
        
        self.editorWidget = parentWidget.editorWidget   # TODO: Review class structure

        self.searching = False
        self.ui = uic.loadUi('SearchWidget.ui', self)
        self.resultListModel = QStandardItemModel(self.ui.resultList)
        self.ui.resultWidget.setCurrentIndex(0) # show (empty) result list by default
        self.ui.resultList.setModel(self.resultListModel)
        self.ui.resultList.selectionModel().selectionChanged.connect(self.doResultSelected)
        self.startIcon = QIcon('icons/search-global-start.png')
        self.stopIcon = QIcon('icons/search-global-stop.png')
        self.ui.startStopButton.setIcon(self.startIcon)

        self.ui.searchInput.returnPressed.connect(self.doReturnKey)
        self.ui.startStopButton.clicked.connect(self.doStartStopButton)

        self.workerThread = QThread()
        self.worker = SearchWorker()
        self.worker.moveToThread(self.workerThread)
        self.startWork.connect(self.worker.startSearch)
        self.worker.searchDone.connect(self.searchDone)
        self.worker.addMatch.connect(self.addMatch)
        # self.stopWork.connect(self.worker.stopSearch)
        self.workerThread.start()


    def doReturnKey(self):
        if self.searching:
            self.worker.stopSearch()        # must not call trough a slot since it would be queued
                                            # ATTENTION! Still a race condition!!! (At least when the search process just finished normally)
        self.startSearch()


    def doStartStopButton(self):
        if self.searching:
            self.worker.stopSearch()        # must not call trough a slot since it would be queued
            self.searchDone()
        else: 
            self.startSearch()


    def startSearch(self):
        self.searching = True
        self.ui.startStopButton.setIcon(self.stopIcon)
        self.resultListModel.clear()
        queryText = self.ui.searchInput.text()
        rootPath = self.editorWidget.page.notepad.getRootpath()
        self.startWork.emit(rootPath, queryText)


    def searchDone(self):
        print("Search Done.")
        self.searching = False
        if self.resultListModel.rowCount() == 0:
            self.ui.resultWidget.setCurrentIndex(1)  # show error page
        self.ui.startStopButton.setIcon(self.startIcon)

    def addMatch(self, pageId):
        print("    Adding: {}".format(pageId))
        self.ui.resultWidget.setCurrentIndex(0)     # make sure to show the list
        resultItem = QStandardItem(pageId)
        resultItem.setEditable(False)
        self.resultListModel.appendRow(resultItem)


    def doResultSelected(self, selectedtItem, idx2):
        indexes = selectedtItem.indexes()
        if len(indexes) == 1:
            item = self.resultListModel.itemFromIndex(indexes[0])
            pageId = item.text()
            self.resultSelected.emit(pageId)


class LinklistWidget(QListView):
    
    def __init__(self, parent):
        QListView.__init__(self, parent)

        self.resultListModel = QStandardItemModel(self)
        self.setModel(self.resultListModel)
        self.selectionModel().selectionChanged.connect(self.doItemSelected)

    def doItemSelected(self, selectedtItem, idx2):
        indexes = selectedtItem.indexes()
        if len(indexes) == 1:
            item = self.resultListModel.itemFromIndex(indexes[0])
            pageId = item.text()
            # self.resultSelected.emit(pageId)
            print("ITEM SELECTED {}".format(pageId))

    def setContents(self, linkList):
        self.resultListModel.clear()
        for item in linkList:
            resultItem = QStandardItem(item)
            resultItem.setEditable(False)
            self.resultListModel.appendRow(resultItem)



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
        self.textView.setFontPointSize(8)
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

        self.searchWidget = SearchWidget(self)
        self.searchWidget.resultSelected.connect(self.navigateDirect)

        self.toLinksWidget = LinklistWidget(self)

        self.listsWidget = QTabWidget(self)
        self.listsWidget.addTab(self.searchWidget, 'Search')
        self.listsWidget.addTab(self.toLinksWidget, 'Links to')
        self.listsWidget.addTab(QWidget(), 'Links from')

        leftWidget = QSplitter(Qt.Vertical, self)
        leftWidget.addWidget(self.browserWidget)
        leftWidget.addWidget(self.listsWidget)

        self.splitter.addWidget(leftWidget)
        self.splitter.addWidget(self.tabWidget)
        leftWidget.setSizes([400, 100])             # TODO

        self.setLayout(self.theLayout)
        self.splitter.setSizes([100, 400])          # TODO
        # self.splitter.setChildrenCollapsible(False)
        self.editorWidget.setEnabled(False)


    def showMessage(self, message):
        self.parent().statusBar.showMessage(message, 3000)


    def navigate(self, pageId):
        """Assumption: pageId is sub page of current page"""
        print('Navigating to sub page "{}"'.format(pageId))

        self.editorWidget.save()
        self.editorWidget.load(self.editorWidget.page.notepad, pageId)
        self.updateLinktoList()

        self.browserWidget.navigate(pageId)


    def navigateDirect(self, pageId):
        """Assumption: pageId is NOT sub page of current page. 
        Hence we need to let the browser point to the first occurrence of the page."""
        print('Navigating directly to "{}"'.format(pageId))

        self.editorWidget.save()
        self.editorWidget.load(self.editorWidget.page.notepad, pageId)
        self.updateLinktoList()

        self.browserWidget.navigateDirect(pageId)


    def itemSelected(self):
        treeNode = self.browserWidget.currentItem
        print("Selected tree node: {}".format(treeNode))

        # get page id (None = title page)
        pageId = None
        if treeNode.parent() is not None:
            pageId = treeNode.getLabel()

        self.editorWidget.setEnabled(True)
        self.editorWidget.save()
        self.editorWidget.load(treeNode.getNotepad(), pageId)
        self.updateLinktoList()


    def updateLinktoList(self):
        print('{}'.format(self.editorWidget.page.getLinks()))
        self.toLinksWidget.setContents(self.editorWidget.page.getLinks())


    def tabSelected(self, index):
        if index == 1:        # Web View
            self.htmlView.clear()
            doc = self.editorWidget.editView.document()
            traversal = TextHTMLPrinter(self.htmlView.insertPlainText, self.editorWidget.page)
            traversal.traverse(doc)

            ########### get URL for the stylesheet and for the base URL
            mypath = os.getcwd()
            mypath = mypath.replace('\\', '/')
            stylesheetURL = QUrl('file:///{}/webpage.css'.format(mypath))
            baseURL = QUrl('file:///{}/'.format(mypath))
            ###########

            self.browser.settings().setUserStyleSheetUrl(stylesheetURL)
            self.browser.setHtml(self.htmlView.toPlainText(), baseURL)

        elif index == 2:      # PDF
            pass

        elif index == 3:
            self.dumpTextStructure()

        elif index == 4:
            self.customView.clear()

            doc = self.editorWidget.editView.document()
            
            from TextDocumentTraversal2 import TextDocumentTraversal2
            traversal = TextDocumentTraversal2()
            tree = traversal.traverse(doc)

            from TextDocumentTraversal2 import DocbookPrinter
            dp = DocbookPrinter(tree, self.customView.insertPlainText)
            dp.traverse()

            #traversal = TextXMLPrinter(self.customView.insertPlainText)
            #traversal.traverse(doc)

        elif index == 5:
            self.htmlView.clear()

            doc = self.editorWidget.editView.document()

            from TextDocumentTraversal2 import TextDocumentTraversal2
            traversal = TextDocumentTraversal2()
            tree = traversal.traverse(doc)
             
            from TextDocumentTraversal2 import HtmlPrinter
            hp = HtmlPrinter(tree, self.htmlView.insertPlainText)
            hp.traverse()

            #traversal = TextHTMLPrinter(self.htmlView.insertPlainText, self.editorWidget.page)
            #traversal.traverse(doc)


    def blocks(self, frame):
        blocks = frame.begin()
        while not blocks.atEnd():
            block = blocks.currentBlock()
            yield block
            blocks += 1


    def dumpTextStructure(self):
        self.textView.clear()

        doc = self.editorWidget.editView.document()
        #traversal = TextDocumentPrinter(self.textView.insertPlainText)

        from TextDocumentTraversal2 import TextDocumentTraversal2
        traversal = TextDocumentTraversal2()
        tree = traversal.traverse(doc)
        
        from TextDocumentTraversal2 import StructurePrinter
        sp = StructurePrinter(tree, self.textView.insertPlainText)
        sp.traverse()



class MainWindow(QMainWindow):

    l = logging.getLogger('MainWindow')

    def __init__(self, app):
        QMainWindow.__init__(self, None)
        self.l.debug('Initializing MainWindow ...')

        self.setWindowTitle('MynPad')
        app.setWindowIcon(QIcon('icons/mynpad.png'))
        # On Windows, make sure to use a unique Application User Model Id, otherwise
        # Windows shows the default python icon in the taskbar
        # see http://stackoverflow.com/questions/1551605/how-to-set-applications-taskbar-icon-in-windows-7
        myappid = 'afester.mynpad'
        import ctypes; ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)

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
