#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtCore import PYQT_VERSION_STR, QT_VERSION_STR, qVersion, pyqtSignal, Qt
from PyQt5.QtWidgets import QWidget, QTabWidget
from PyQt5.QtWidgets import QTextEdit, QSplitter, QHBoxLayout, QVBoxLayout, QMainWindow
from PyQt5.QtWidgets import QAction, QStatusBar, QMenuBar, QMessageBox, QListView
from PyQt5.QtWebKitWidgets import QWebView, QWebPage
from PyQt5.QtCore import QUrl, QObject, QThread 
from PyQt5.QtGui import QStandardItem, QStandardItemModel, QIcon

import sys, os, platform, re, sqlite3, logging
import pkg_resources, data

from ui.EditorWidget import EditorWidget
from ui.BrowserWidget import BrowserWidget
from model.Page import LocalPage
from ui.SearchWidget import Ui_SearchWidget

from Settings import Settings
from StylableTextEdit.StylableTextModel import TextDocumentTraversal, StructurePrinter
from model.XMLExporter import XMLExporter
from HTMLExporter import HTMLExporter
from model.XMLImporter import XMLImporter

import ui.resources

class SearchWorker(QObject):
    
    searchDone = pyqtSignal()
    addMatch = pyqtSignal(str)

    def __init__(self):
        QObject.__init__(self)
        self.notepad = None
        self.pageList = []

    def startSearch(self, searchText):
        self.aborted = False
        print('Starting search operation "{}" on {} ...'.format(searchText, self.notepad.getName()))

        for pageId in self.pageList:
            if self.aborted:
                return

            page = LocalPage(self.notepad, pageId)

            imp = XMLImporter(page.getPageDir(), page.getFilename(), None)
            topFrame = imp.importModel()
            contents = topFrame.getPlainText()

            if re.search(searchText, contents, re.IGNORECASE):
                self.addMatch.emit(pageId)

        self.searchDone.emit()


    def stopSearch(self):
        '''We can not call this method through a slot, since it would be 
        queued until the actual worker method is done'''

        print('Stopping search operation ...')
        self.aborted = True



class SearchWidget(QWidget):

    startWork = pyqtSignal(str)
    resultSelected = pyqtSignal(str)

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)
        
        self.editorWidget = parentWidget.editorWidget   # TODO: Review class structure

        self.searching = False
        self.ui = Ui_SearchWidget()
        self.ui.setupUi(self)
        self.resultListModel = QStandardItemModel(self.ui.resultList)
        self.ui.resultWidget.setCurrentIndex(0)
        self.ui.resultList.setModel(self.resultListModel)
        self.ui.resultList.selectionModel().selectionChanged.connect(self.doResultSelected)

        self.startIcon = QIcon(':/icons/search-global-start.png')
        self.stopIcon = QIcon(':/icons/search-global-stop.png')
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
        # rootPath = self.editorWidget.page.notepad.getRootpath()

        # Setup worker with required data
        self.worker.notepad = self.editorWidget.page.notepad
        # NOTE: we are querying the page list in this thread,
        # to avoid concurrency issues with SQLite.
        self.worker.pageList = self.editorWidget.page.notepad.getAllPages()
        self.startWork.emit(queryText)


    def searchDone(self):
        print("Search Done.")
        self.searching = False
        if self.resultListModel.rowCount() == 0:
            self.ui.resultWidget.setCurrentIndex(1)  # show error page
        self.ui.startStopButton.setIcon(self.startIcon)

    def addMatch(self, pageId):
        # print("    Adding: {}".format(pageId))
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
    
    resultSelected = pyqtSignal(str)

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
            self.resultSelected.emit(pageId)

    def setContents(self, linkList):
        self.resultListModel.clear()
        for item in linkList:
            resultItem = QStandardItem(item)
            resultItem.setEditable(False)
            self.resultListModel.appendRow(resultItem)


# The central widget for the MainWindow.
class CentralWidget(QWidget):

    l = logging.getLogger('CentralWidget')

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
        self.editTabIdx = self.tabWidget.addTab(tab, "Edit")
#############################

        self.browser = QWebView(self.tabWidget)
        self.browser.page().setLinkDelegationPolicy(QWebPage.DelegateAllLinks)
        self.browser.linkClicked.connect(self.navigateWeb)
        self.webTabIdx = self.tabWidget.addTab(self.browser, "View web")

        self.pdfTabIdx = self.tabWidget.addTab(QWidget(self.tabWidget), "View pdf")

        self.textView = QTextEdit(self.tabWidget)
        self.textView.setReadOnly(True)
        self.textView.setFontFamily('Courier')
        self.textView.setFontPointSize(8)
        self.structureTabIdx = self.tabWidget.addTab(self.textView, "View document structure")

        self.customView = QTextEdit(self.tabWidget)
        self.customView.setReadOnly(True)
        self.customView.setFontFamily('Courier')
        self.customView.setFontPointSize(10)
        self.xmlTabIdx = self.tabWidget.addTab(self.customView, "View XML")

        self.htmlView = QTextEdit(self.tabWidget)
        self.htmlView.setReadOnly(True)
        self.htmlView.setFontFamily('Courier')
        self.htmlView.setFontPointSize(10)
        self.htmlTabIdx = self.tabWidget.addTab(self.htmlView, "View Html")

        self.tabWidget.currentChanged.connect(self.tabSelected)

# Search/Links widget in lower left corner ####################################
        self.searchWidget = SearchWidget(self)
        self.searchWidget.resultSelected.connect(self.navigateDirect)

        self.toLinksWidget = LinklistWidget(self)
        self.toLinksWidget.resultSelected.connect(self.navigateDirect)

        self.fromLinksWidget = LinklistWidget(self)
        self.fromLinksWidget.resultSelected.connect(self.navigateDirect)

        self.listsWidget = QTabWidget(self)
        self.listsWidget.addTab(self.searchWidget, 'Search')
        self.listsWidget.addTab(self.toLinksWidget, 'Links to')
        self.listsWidget.addTab(self.fromLinksWidget, 'Links from')
###############################################################################

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


    def navigateWeb(self, url):
        if url.scheme() == 'file':
            pageId = url.fileName()
            self.navigate(pageId)
            self.tabSelected(1)
        elif url.scheme() == 'http' or url.scheme() == 'https':
            self.browser.setUrl(url)


    def showMessage(self, message):
        self.parent().statusBar.showMessage(message, 3000)


    def navigate(self, pageId):
        """Assumption: pageId is sub page of current page"""
        self.l.debug('Navigating to sub page "{}"'.format(pageId))

        self.editorWidget.save()
        self.browserWidget.navigate(pageId)        # Will implicitly load the page


    def navigateDirect(self, pageId):
        """Assumption: pageId is NOT sub page of current page. 
        Hence we need to let the browser point to the first occurrence of the page."""
        self.l.debug('Navigating directly to "{}"'.format(pageId))

        self.editorWidget.save()
        self.browserWidget.navigateDirect(pageId)   # Will implicitly load the page


    def itemSelected(self):
        treeNode = self.browserWidget.currentItem
        self.l.debug('Selected tree node: {}'.format(treeNode))

        pageId = treeNode.getPageId()
        notepad = treeNode.getNotepad()

        self.editorWidget.setEnabled(True)
        self.editorWidget.save()
        self.editorWidget.load(notepad, pageId)

        self.updateLinkLists(notepad, pageId)


    def updateLinkLists(self, notepad, pageId):
        
        linksTo = notepad.getChildPages(pageId)
        linksFrom = notepad.getParentPages(pageId)

        self.toLinksWidget.setContents(linksTo)
        self.fromLinksWidget.setContents(linksFrom)


    def tabSelected(self, index):
        if index == self.editTabIdx:
            pass
        elif index == self.webTabIdx:
            self.activateWebView()
        elif index == self.pdfTabIdx:
            pass
        elif index == self.structureTabIdx:
            self.activateStructureView()
        elif index == self.xmlTabIdx:
            self.activateXMLView()
        elif index == self.htmlTabIdx:
            self.activateHTMLView()


    def activateWebView(self):
        exporter = HTMLExporter(self.editorWidget.page.getPageDir())
        self.htmlView.setPlainText(exporter.getHtmlString(self.editorWidget.editView.document()))

        ########### get URL for the stylesheet and for the base URL

        webpageCSS = pkg_resources.resource_string(data.__name__, 'webpage.css')
        print("webpage.css file: {} - {}".format(webpageCSS, type(webpageCSS)))

        mypath = os.getcwd()
        mypath = mypath.replace('\\', '/')
        baseURL = QUrl('file:///{}/'.format(mypath))

        # The location must be either a path on the local filesystem, or a 
        # data URL with UTF-8 and Base64 encoded data, such as:
        # "data:text/css;charset=utf-8;base64,cCB7IGJhY2tncm91bmQtY29sb3I6IHJlZCB9Ow=="
        # BASE64 works on bytes!!
        import base64
        cssData = base64.b64encode(webpageCSS)
        cssData = cssData.decode('utf-8')
        cssDataUrl = 'data:text/css;charset=utf-8;base64,{}'.format(cssData)
        print("webpage.css base64: {}".format(cssDataUrl ))
        # cssDataUrl = QUrl('data:text/css;charset=utf-8;base64,{}'.format(cssData)) # 'file:///{}/webpage.css'.format(mypath))

        ###########

        self.browser.settings().setUserStyleSheetUrl(QUrl(cssDataUrl))

        self.browser.setHtml(self.htmlView.toPlainText(), baseURL)


    def activateStructureView(self):
        self.textView.clear()

        doc = self.editorWidget.editView.document()
        traversal = TextDocumentTraversal()
        tree = traversal.traverse(doc)
        
        sp = StructurePrinter(tree, self.textView.insertPlainText)
        sp.traverse()


    def activateXMLView(self):
        exporter = XMLExporter(self.editorWidget.page.getPageDir(), None)
        self.customView.setPlainText(exporter.getXmlString(self.editorWidget.editView.document()))


    def activateHTMLView(self):
        exporter = HTMLExporter(self.editorWidget.page.getPageDir())
        self.htmlView.setPlainText(exporter.getHtmlString(self.editorWidget.editView.document()))


class MainWindow(QMainWindow):

    l = logging.getLogger('MainWindow')

    def __init__(self, app):
        QMainWindow.__init__(self, None)
        self.l.debug('Initializing MainWindow ...')

        self.setWindowTitle('MynPad')
        app.setWindowIcon(QIcon(':/icons/mynpad.png'))

        if os.name == 'nt':
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
        self.mainWidget = CentralWidget(self, self.settings)
        self.mainWidget.updateWindowTitle.connect(self.updateWindowTitle)
        self.setCentralWidget(self.mainWidget)

        # Reset main window size and position
        pos = self.settings.getMainWindowPos()
        self.move(pos.x(), pos.y())
        size = self.settings.getMainWindowSize()
        self.resize(size)

        # initialize the browser tree (add the top nodes and expand the saved path)
        self.mainWidget.browserWidget.initialize()


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

        # Close all notepads - TODO (HACK)
        for x in range (0, self.mainWidget.browserWidget.browserView.topLevelItemCount()):
            notepad = self.mainWidget.browserWidget.browserView.topLevelItem(x).getNotepad()
            notepad.close()


    def handleAbout(self):
        appVersion = "Development version"
        pythonVersion = "%s.%s.%s (%s)" % (sys.version_info[0], sys.version_info[1], sys.version_info[2], sys.version_info[3])
        pyQtVersion = PYQT_VERSION_STR
        pyQtQtVersion = QT_VERSION_STR
        qtRuntimeVersion = qVersion()
        
        platformSystem = platform.system()
        platformRelease = platform.release()

        QMessageBox.about(self, "About MynPad",
                          "Copyright \u00a9 2015 by Andreas Fester<br/>"+
                          "<table>"+
                          "<tr><th align=\"right\">Application version:</th><td>{}</td></tr>".format(appVersion) +
                          "<tr><th align=\"right\">Python version:</th><td>{}</td></tr>".format(pythonVersion) +
                          "<tr><th align=\"right\">PyQt version:</th><td>{} for Qt {}</td></tr>".format(pyQtVersion, pyQtQtVersion) +
                          "<tr><th align=\"right\">Qt runtime version:</th><td>{}</td></tr>".format(qtRuntimeVersion)+
                          "<tr><th align=\"right\">Operating System:</th><td>{} {}</td></tr>".format(platformSystem, platformRelease)+
                          "<tr><th align=\"right\">sqlite version:</th><td>{}</td></tr>".format(sqlite3.version) +
                          "<tr><th align=\"right\">sqlite runtime version:</th><td>{}</td></tr>".format(sqlite3.sqlite_version)+
                          "</table>")
