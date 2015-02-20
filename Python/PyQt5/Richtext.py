#!/usr/bin/env python3

'''
Created on Feb 13, 2015

@author: andreas
'''

from PyQt5.QtCore import QSize, PYQT_VERSION_STR, QT_VERSION_STR, qVersion, pyqtSignal
from PyQt5.QtWidgets import QTreeWidget, QTreeWidgetItem, QWidget, QTabWidget
from PyQt5.QtWidgets import QTextEdit, QSplitter, QHBoxLayout, QVBoxLayout, QMainWindow
from PyQt5.QtWidgets import QAction, QStatusBar, QMenuBar, QApplication, QMessageBox

import sys, os, tools

from EditorWidget import EditorWidget
from XMLExporter import XMLExporter


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
                path = tools.os_path_split(path)
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


class RichtextSampleWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QHBoxLayout()
        self.splitter = QSplitter(self)
        self.theLayout.addWidget(self.splitter)

        self.browserWidget = BrowserWidget(self)
        self.browserWidget.itemSelected.connect(self.itemSelected)

        self.tabWidget = QTabWidget(self)

        tab = QWidget()

        tabLayout = QVBoxLayout(tab)
        tab.setLayout(tabLayout)

        self.editorWidget = EditorWidget(tab)

        self.editorWidget.setObjectName("EditorWidget1")
        self.editorWidget.message.connect(self.showMessage)

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

        self.browserWidget.refresh()
        self.editorWidget.load(['SampleWiki'])


    def showMessage(self, message):
        self.parent().statusBar.showMessage(message, 3000)


    def itemSelected(self):
        path = self.browserWidget.getCurrentPath()
        self.editorWidget.load(path)



    def tabSelected(self, index):
        if index == 3:
            self.htmlView.setPlainText(self.editorWidget.editView.toHtml())
        elif index == 4:
            self.textView.setPlainText(self.editorWidget.editView.toPlainText())
        elif index == 5:
            exporter = XMLExporter(None)
            xmlText = exporter.getXmlString(self.editorWidget.editView.document())
            self.customView.setPlainText(xmlText)


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
