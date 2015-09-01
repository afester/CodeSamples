'''
Created on 01.09.2015

@author: afester
'''
# 
from PyQt5.QtCore import PYQT_VERSION_STR, QT_VERSION_STR, qVersion, pyqtSignal, Qt, QRectF, QPointF
from PyQt5.QtWidgets import QWidget, QTabWidget
from PyQt5.QtWidgets import QTextEdit, QSplitter, QHBoxLayout, QVBoxLayout, QMainWindow
from PyQt5.QtWidgets import QAction, QStatusBar, QMenuBar, QMessageBox, QListView
from PyQt5.QtGui import QStandardItem, QStandardItemModel, QIcon, QPalette, QPainter, QImage, QBrush
from PyQt5.QtSvg import QSvgRenderer
import math
import sys, os, platform, re, sqlite3, logging

from ui.Data import Ui_Form

# import pkg_resources, data
from os.path import expanduser
# 
# from ui.EditorWidget import EditorWidget
# from ui.BrowserWidget import BrowserWidget
# from model.Page import LocalPage
# from ui.SearchWidget import Ui_SearchWidget
# 
from Settings import Settings
from PyQt5.Qt import QPoint, QSizeF
# from StylableTextEdit.StylableTextModel import TextDocumentTraversal, StructurePrinter
# from model.XMLExporter import XMLExporter
# from HTMLExporter import HTMLExporter
# from model.XMLImporter import XMLImporter
# 
# import ui.resources
# 
# class SearchWorker(QObject):
#     
#     searchDone = pyqtSignal()
#     addMatch = pyqtSignal(str)
# 
#     def __init__(self):
#         QObject.__init__(self)
#         self.notepad = None
#         self.pageList = []
# 
#     def startSearch(self, searchText):
#         self.aborted = False
#         print('Starting search operation "{}" on {} ...'.format(searchText, self.notepad.getName()))
# 
#         for pageId in self.pageList:
#             if self.aborted:
#                 return
# 
#             page = LocalPage(self.notepad, pageId)
# 
#             imp = XMLImporter(page.getPageDir(), page.getFilename(), None)
#             topFrame = imp.importModel()
#             contents = topFrame.getPlainText()
# 
#             if re.search(searchText, contents, re.IGNORECASE):
#                 self.addMatch.emit(pageId)
# 
#         self.searchDone.emit()
# 
# 
#     def stopSearch(self):
#         '''We can not call this method through a slot, since it would be 
#         queued until the actual worker method is done'''
# 
#         print('Stopping search operation ...')
#         self.aborted = True
# 
# 
# 
# class SearchWidget(QWidget):
# 
#     startWork = pyqtSignal(str)
#     resultSelected = pyqtSignal(str)
# 
#     def __init__(self, parentWidget):
#         QWidget.__init__(self, parentWidget)
#         
#         self.editorWidget = parentWidget.editorWidget   # TODO: Review class structure
# 
#         self.searching = False
#         self.ui = Ui_SearchWidget()
#         self.ui.setupUi(self)
#         self.resultListModel = QStandardItemModel(self.ui.resultList)
#         self.ui.resultWidget.setCurrentIndex(0)
#         self.ui.resultList.setModel(self.resultListModel)
#         self.ui.resultList.selectionModel().selectionChanged.connect(self.doResultSelected)
# 
#         self.startIcon = QIcon(':/icons/search-global-start.png')
#         self.stopIcon = QIcon(':/icons/search-global-stop.png')
#         self.ui.startStopButton.setIcon(self.startIcon)
# 
#         self.ui.searchInput.returnPressed.connect(self.doReturnKey)
#         self.ui.startStopButton.clicked.connect(self.doStartStopButton)
# 
#         self.workerThread = QThread()
#         self.worker = SearchWorker()
#         self.worker.moveToThread(self.workerThread)
#         self.startWork.connect(self.worker.startSearch)
#         self.worker.searchDone.connect(self.searchDone)
#         self.worker.addMatch.connect(self.addMatch)
#         # self.stopWork.connect(self.worker.stopSearch)
#         self.workerThread.start()
# 
# 
#     def doReturnKey(self):
#         if self.searching:
#             self.worker.stopSearch()        # must not call trough a slot since it would be queued
#                                             # ATTENTION! Still a race condition!!! (At least when the search process just finished normally)
#         self.startSearch()
# 
# 
#     def doStartStopButton(self):
#         if self.searching:
#             self.worker.stopSearch()        # must not call trough a slot since it would be queued
#             self.searchDone()
#         else: 
#             self.startSearch()
# 
# 
#     def startSearch(self):
#         self.searching = True
#         self.ui.startStopButton.setIcon(self.stopIcon)
#         self.resultListModel.clear()
#         queryText = self.ui.searchInput.text()
#         # rootPath = self.editorWidget.page.notepad.getRootpath()
# 
#         # Setup worker with required data
#         self.worker.notepad = self.editorWidget.page.notepad
#         # NOTE: we are querying the page list in this thread,
#         # to avoid concurrency issues with SQLite.
#         self.worker.pageList = self.editorWidget.page.notepad.getAllPages()
#         self.startWork.emit(queryText)
# 
# 
#     def searchDone(self):
#         print("Search Done.")
#         self.searching = False
#         if self.resultListModel.rowCount() == 0:
#             self.ui.resultWidget.setCurrentIndex(1)  # show error page
#         self.ui.startStopButton.setIcon(self.startIcon)
# 
#     def addMatch(self, pageId):
#         # print("    Adding: {}".format(pageId))
#         self.ui.resultWidget.setCurrentIndex(0)     # make sure to show the list
#         resultItem = QStandardItem(pageId)
#         resultItem.setEditable(False)
#         self.resultListModel.appendRow(resultItem)
# 
# 
#     def doResultSelected(self, selectedtItem, idx2):
#         indexes = selectedtItem.indexes()
#         if len(indexes) == 1:
#             item = self.resultListModel.itemFromIndex(indexes[0])
#             pageId = item.text()
#             self.resultSelected.emit(pageId)
# 
# 
# class LinklistWidget(QListView):
#     
#     resultSelected = pyqtSignal(str)
# 
#     def __init__(self, parent):
#         QListView.__init__(self, parent)
# 
#         self.resultListModel = QStandardItemModel(self)
#         self.setModel(self.resultListModel)
#         self.selectionModel().selectionChanged.connect(self.doItemSelected)
# 
#     def doItemSelected(self, selectedtItem, idx2):
#         indexes = selectedtItem.indexes()
#         if len(indexes) == 1:
#             item = self.resultListModel.itemFromIndex(indexes[0])
#             pageId = item.text()
#             self.resultSelected.emit(pageId)
# 
#     def setContents(self, linkList):
#         self.resultListModel.clear()
#         for item in linkList:
#             resultItem = QStandardItem(item)
#             resultItem.setEditable(False)
#             self.resultListModel.appendRow(resultItem)
# 
# 

class ShadowWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.texture = QImage("ui/gras.png")
        self.svgRenderer = QSvgRenderer("ui/tree.svg")

        # set the gras texture as the background image
        self.setAutoFillBackground(True)
        pal = self.palette()
        pal.setBrush(QPalette.Window, QBrush(self.texture))
        self.setPalette(pal)
        self.angle = 0

    def setAngle(self, value):
        self.angle = value
        self.repaint()

    def paintEvent(self, event):
        radius = 50
        
        treeSize = QSizeF(2 * radius, 2 * radius)
        bounds = QRectF( (self.width() - treeSize.height()) / 2,
                         (self.height() - treeSize.width()) / 2,
                         treeSize.width(),
                         treeSize.height())

        painter = QPainter(self)

        # calculate relative coordinates for the shadow center (right on the 
        # arc of a circle around the tree) 
        b = math.radians(self.angle)
        x = math.sin(b) * radius
        y = math.cos(b) * radius

        # draw the shadow
        center = QPointF((self.width() - treeSize.height()) / 2,
                         (self.height() - treeSize.width()) / 2)
        shadow = QRectF(center.x() - x, center.y() - y, 100, 100)
        painter.setBrush(Qt.black)
        painter.setPen(Qt.black)
        painter.setOpacity(0.5)
        painter.drawEllipse(shadow)

        # draw the tree
        painter.setOpacity(1)
        self.svgRenderer.render(painter, bounds)


class DataWidget(QWidget):
    
    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.ui = Ui_Form()
        self.ui.setupUi(self)



class MainWindow(QMainWindow):
# 
    l = logging.getLogger('MainWindow')
# 
    def __init__(self, app):
        QMainWindow.__init__(self, None)
        self.l.debug('Initializing MainWindow ...')

        self.setWindowTitle('Shadow')
#         app.setWindowIcon(QIcon(':/icons/mynpad.png'))
# 
#         if os.name == 'nt':
#             # On Windows, make sure to use a unique Application User Model Id, otherwise
#             # Windows shows the default python icon in the taskbar
#             # see http://stackoverflow.com/questions/1551605/how-to-set-applications-taskbar-icon-in-windows-7
#             myappid = 'afester.mynpad'
#             import ctypes; ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)
 
        self.theApplication = app
        app.aboutToQuit.connect(self.saveState)
 
        # read the local configuration file
        iniPath = 'shadow.ini'
        if not os.path.exists(iniPath):
            iniPath = os.path.join(expanduser("~"), iniPath)
        self.settings = Settings(iniPath)
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
 
#         # Setup the status bar
#         self.statusBar = QStatusBar()
#         self.statusBar.showMessage("Ready.")
#         self.setStatusBar(self.statusBar)
#         self.mainWidget = CentralWidget(self, self.settings)
#         self.mainWidget.updateWindowTitle.connect(self.updateWindowTitle)
#         self.setCentralWidget(self.mainWidget)

        self.centralWidget = QWidget()
        self.left = DataWidget(self.centralWidget)
        self.right = ShadowWidget(self.centralWidget)

        self.left.ui.testSlider.valueChanged.connect(self.right.setAngle)


        layout = QHBoxLayout()
        self.centralWidget.setLayout(layout)
        layout.addWidget(self.left)
        layout.addWidget(self.right)
        layout.setStretch(0, 0);
        layout.setStretch(1, 1);

        self.setCentralWidget(self.centralWidget)

        # Reset main window size and position
        pos = self.settings.getMainWindowPos()
        self.move(pos.x(), pos.y())
        size = self.settings.getMainWindowSize()
        self.resize(size)
 

    def saveState(self):
#         # Note: there is no way to have eclipse shutdown the application faithfully,
#         # see also http://stackoverflow.com/questions/677531/is-it-possible-for-eclipse-to-terminate-gently-instead-of-using-sigkill
#         path = self.mainWidget.browserWidget.getCurrentPath()
#         self.settings.setBrowserPath(path)
        self.settings.setMainWindowPos(self.pos())
        self.settings.setMainWindowSize(self.size())
        self.settings.save()
 
 
    def handleAbout(self):
        appVersion = "Development version"
        pythonVersion = "%s.%s.%s (%s)" % (sys.version_info[0], sys.version_info[1], sys.version_info[2], sys.version_info[3])
        pyQtVersion = PYQT_VERSION_STR
        pyQtQtVersion = QT_VERSION_STR
        qtRuntimeVersion = qVersion()
         
        platformSystem = platform.system()
        platformRelease = platform.release()
 
        QMessageBox.about(self, "About Shadow",
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
