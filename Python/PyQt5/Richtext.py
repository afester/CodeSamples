#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtCore import PYQT_VERSION_STR, QT_VERSION_STR, qVersion, Qt
from PyQt5.QtWidgets import QWidget, QTabWidget
from PyQt5.QtWidgets import QTextEdit, QSplitter, QHBoxLayout, QVBoxLayout, QMainWindow
from PyQt5.QtWidgets import QAction, QStatusBar, QMenuBar, QApplication, QMessageBox

import sys
import logging.config

from EditorWidget import EditorWidget
from BrowserWidget import BrowserWidget
from XMLExporter import XMLExporter
from Settings import Settings

class RichtextSampleWidget(QWidget):

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
        self.editorWidget.navigate.connect(self.navigate, Qt.QueuedConnection)

        tabLayout.addWidget(self.editorWidget)
        self.tabWidget.addTab(tab, "Edit")
#############################

        self.tabWidget.addTab(QWidget(self.tabWidget), "View web")

        self.tabWidget.addTab(QWidget(self.tabWidget), "View pdf")

        self.htmlView = QTextEdit(self.tabWidget)
        self.htmlView.setReadOnly(True)
        self.tabWidget.addTab(self.htmlView, "View html")

        self.textView = QTextEdit(self.tabWidget)
        self.textView.setReadOnly(True)
        self.tabWidget.addTab(self.textView, "View plaintext")

        self.customView = QTextEdit(self.tabWidget)
        self.customView.setReadOnly(True)
        self.tabWidget.addTab(self.customView, "View XML")

        self.tabWidget.currentChanged.connect(self.tabSelected)

        self.splitter.addWidget(self.browserWidget)
        self.splitter.addWidget(self.tabWidget)

        self.setLayout(self.theLayout)
        self.splitter.setSizes([100, 400])

        self.editorWidget.setEnabled(False)

        self.browserWidget.refresh()    # refresh the browser tree 


    def showMessage(self, message):
        self.parent().statusBar.showMessage(message, 3000)


    def navigate(self, pageId):
        print("navigating to {}".format(pageId))
        self.editorWidget.save()
        self.editorWidget.load(self.editorWidget.page.notepad, pageId)


    def itemSelected(self):
        treeNode = self.browserWidget.currentItem
        print("Selected tree node: {}".format(treeNode))
        self.editorWidget.setEnabled(True)
        if treeNode.parent() is None:
            self.editorWidget.load(treeNode.getNotepad(), None)
        else:
            self.editorWidget.load(treeNode.getNotepad(), treeNode.getLabel())


    def tabSelected(self, index):
        if index == 3:
            self.htmlView.setPlainText(self.editorWidget.editView.toHtml())
        elif index == 4:
            self.textView.setPlainText(self.editorWidget.editView.toPlainText())
        elif index == 5:
            exporter = XMLExporter(None, None)
            xmlText = exporter.getXmlString(self.editorWidget.editView.document())
            self.customView.setPlainText(xmlText)


class MainWindow(QMainWindow):

    l = logging.getLogger('MainWindow')

    def __init__(self, app):
        QMainWindow.__init__(self, None)
        
        self.l.debug('Initializing MainWindow ...')
        
        self.setWindowTitle("My Notepad")
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

        self.mainWidget = RichtextSampleWidget(self, self.settings)
        self.setCentralWidget(self.mainWidget)

        # Reset main window size and position
        pos = self.settings.getMainWindowPos()
        self.move(pos.x(), pos.y())
        size = self.settings.getMainWindowSize()
        self.resize(size)


    def saveState(self):

        # Note: there is no way to have eclipse shutdown the application faithfully,
        # see also http://stackoverflow.com/questions/677531/is-it-possible-for-eclipse-to-terminate-gently-instead-of-using-sigkill
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
