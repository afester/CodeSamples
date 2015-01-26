#!/usr/bin/env python3

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *

import pyudev
import sys



class SearchWidget(QWidget):
    
    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QHBoxLayout()

        self.searchInputWidget = QLineEdit("Test", self)
        self.searchStartButton = QPushButton("S", self)
        self.searchNextButton = QPushButton("^ ", self)
        self.searchPrevButton = QPushButton("v", self)

        self.theLayout.addWidget(self.searchInputWidget)
        self.theLayout.addWidget(self.searchStartButton)
        self.theLayout.addWidget(self.searchNextButton)
        self.theLayout.addWidget(self.searchPrevButton)

        self.setLayout(self.theLayout)

class UDevTreeWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QVBoxLayout()

        self.searchWidget = SearchWidget(self)

        self.treeWidget = QTreeWidget(self)
        self.treeWidget.setColumnCount(1)

        self.theLayout.addWidget(self.searchWidget)
        self.theLayout.addWidget(self.treeWidget)
        self.setLayout(self.theLayout)


class UDevBrowserWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QHBoxLayout()
        self.splitter = QSplitter(self)
        self.theLayout.addWidget(self.splitter)

        self.leftWidget = UDevTreeWidget(self)
        # self.treeWidget.setColumnCount(1)

        self.rightWidget = QTextEdit(self)

        self.splitter.addWidget(self.leftWidget)
        self.splitter.addWidget(self.rightWidget)
        self.setLayout(self.theLayout)


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

    def refresh(self):
        context = pyudev.Context()

        self.rootNodes = []
        for device in context.list_devices():
            path = device.device_path[1:].split("/")
            self.addPath(path)

        self.treeWidget.addTopLevelItems(self.rootNodes)
        for node in self.rootNodes:
            node.setExpanded(True)


class MainWindow(QMainWindow):

    def __init__(self):
        QMainWindow.__init__(self, None)
        self.setWindowTitle("udev Device browser")

        # Set up the menu bar
        menuBar = QMenuBar(self)

        exitAction = QAction("Exit", self)
        fileMenu = menuBar.addMenu("&File")
        fileMenu.addAction(exitAction)

        aboutQtAction = QAction("About Qt ...", self)
        aboutAction = QAction("About ...", self)
        helpMenu = menuBar.addMenu("&Help")
        helpMenu.addAction(aboutQtAction)
        helpMenu.addAction(aboutAction)

        # menuBar.addAction(fileMenu.menuAction())

#        QObject.connect(exitAction, SIGNAL("triggered()"), app.exit)
#        QObject.connect(aboutQtAction, SIGNAL("triggered()"), self.aboutQt)
#        QObject.connect(aboutAction, SIGNAL("triggered()"), self.about)
        self.setMenuBar(menuBar)

        self.udevBrowserWidget = UDevBrowserWidget(self)
        #self.udevBrowserWidget.refresh()
        self.setCentralWidget(self.udevBrowserWidget)

        self.move(240, 200)
        self.resize(QSize(640, 480))



print('Using PyDev  {}'.format(pyudev.__version__))
print('Using libudev {}'.format(pyudev.udev_version()))

# Create the application object
app = QApplication(sys.argv)

# Create the main window
mainWindow = MainWindow()

# Show and run the application
mainWindow.show()
app.exec_()
