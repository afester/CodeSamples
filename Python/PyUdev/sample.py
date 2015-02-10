#!/usr/bin/env python3

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.Qt import PYQT_VERSION_STR

from sip import SIP_VERSION_STR

import pyudev
import sys
import usb1


class TwoColumnTable(QTableWidget):

    def __init__(self, parentWidget):
        QTableWidget.__init__(self, parentWidget)

    def resizeEvent(self, *args, **kwargs):
        width = args[0].size().width()
        self.setColumnWidth(0, width/2)
        self.setColumnWidth(1, width - width/2)
        return QTableWidget.resizeEvent(self, *args, **kwargs)


class SearchWidget(QWidget):
    
    
    startSearch = pyqtSignal()
    searchNext = pyqtSignal()
    searchPrev = pyqtSignal()

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QHBoxLayout()

        self.searchInputWidget = QLineEdit(self)

        self.searchStartButton = QToolButton(self, clicked = self.startSearch)
        self.searchStartButton.setIcon(QIcon("icons/start-search.png"))

        self.findNextButton = QToolButton(self, clicked = self.searchNext)
        self.findNextButton.setIcon(QIcon("icons/find-next.png"))

        self.findPrevButton = QToolButton(self, clicked = self.searchPrev)
        self.findPrevButton.setIcon(QIcon("icons/find-prev.png"))

        self.theLayout.addWidget(self.searchInputWidget)
        self.theLayout.addWidget(self.searchStartButton)
        self.theLayout.addWidget(self.findNextButton)
        self.theLayout.addWidget(self.findPrevButton)

        self.setLayout(self.theLayout)

    def getSearchText(self):
        return self.searchInputWidget.text()


class UDevTreeWidget(QWidget):

    itemSelected = pyqtSignal()

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QVBoxLayout()

        self.searchWidget = SearchWidget(self)
        self.searchWidget.startSearch.connect(self.startSearch)
        self.searchWidget.searchNext.connect(self.searchNext)
        self.searchWidget.searchPrev.connect(self.searchPrev)

        self.treeWidget = QTreeWidget(self)
        self.treeWidget.setColumnCount(1)
        self.treeWidget.setHeaderLabel("Device browser")
        self.treeWidget.itemSelectionChanged.connect(self.handleItemSelected)

        self.theLayout.addWidget(self.searchWidget)
        self.theLayout.addWidget(self.treeWidget)
        self.setLayout(self.theLayout)

    def handleItemSelected(self):
        selItems = self.treeWidget.selectedItems()
        if len(selItems) == 1:
            self.currentItem = selItems[0]
            self.itemSelected.emit()

    def getCurrentPath(self):
        result = self.currentItem.text(0)
        parent = self.currentItem.parent()
        while parent is not None:
            result = parent.text(0) + "/" + result
            parent = parent.parent()
        return "/" + result 


    def childNodes(self, parent):
        yield parent
        for idx in range(0, parent.childCount()):
            childItem = parent.child(idx)
            for c in self.childNodes(childItem):
                yield c

    def treeNodes(self):
        for topIdx in range(0, self.treeWidget.topLevelItemCount()):
            rootItem = self.treeWidget.topLevelItem(topIdx)
            for c in self.childNodes(rootItem):
                yield c

    def searchItem(self, item, searchText):
        nodes = self.treeNodes()
        print("Node:" + next(nodes).text(0))
        print("Node:" + next(nodes).text(0))

        return

        print("Searching ", item.text(0))
        if searchText in item.text(0):
            self.found = True
            self.itemFound = item
            print("*** FOUND!!")
        else:
            for idx in range(0, item.childCount()):
                childItem = item.child(idx)
                self.searchItem(childItem, searchText)
                if self.found:
                    break;

    def startSearch(self):
        # get all nodes into a list - this is currently the best effort to
        # support a wrap-around like search in both directions!
        self.allNodes = [n for n in self.treeNodes()]
        self.foundIndex = -1
        self.searchNext()

    def searchNext(self):
        searchIndex = self.foundIndex
        searchText = self.searchWidget.getSearchText()
        searching = True
        while searching:
            # calculate next index
            searchIndex = searchIndex + 1
            if searchIndex >= len(self.allNodes):
                searching = False
                searchIndex = 0

            node = self.allNodes[searchIndex]
            if (searchText in node.text(0)):
                searching = False
                self.foundIndex = searchIndex
                self.expandToRoot(node)

    def searchPrev(self):
        searchIndex = self.foundIndex
        searchText = self.searchWidget.getSearchText()
        searching = True
        while searching:
            # calculate next index
            searchIndex = searchIndex - 1
            if searchIndex < 0:
                searching = False
                searchIndex = len(self.allNodes) - 1

            node = self.allNodes[searchIndex]
            if (searchText in node.text(0)):
                searching = False
                self.foundIndex = searchIndex
                self.expandToRoot(node)
                
    def expandToRoot(self, node):
        parent = node.parent()
        while parent is not None:
            parent.setExpanded(True)
            parent = parent.parent()

        for selNode in self.treeWidget.selectedItems():
            selNode.setSelected(False)
        node.setSelected(True)
        self.treeWidget.scrollToItem(node)

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
        # for device in context.list_devices(subsystem='block'):
            path = device.device_path[1:].split("/")
            self.addPath(path)

        usbCtx = usb1.USBContext()
        for device in usbCtx.getDeviceList(skip_on_error=True):
            print('%04x:%04x' % (device.getVendorID(), device.getProductID()), '->'.join(str(x) for x in ['Bus %03i' % (device.getBusNumber(), )] + device.getPortNumberList()), 'Device', device.getDeviceAddress())

        self.treeWidget.addTopLevelItems(self.rootNodes)
        for node in self.rootNodes:
            node.setExpanded(True)


        




class UDevBrowserWidget(QWidget):

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.theLayout = QHBoxLayout()
        self.splitter = QSplitter(self)
        self.theLayout.addWidget(self.splitter)

        self.leftWidget = UDevTreeWidget(self)
        self.leftWidget.itemSelected.connect(self.itemSelected)

        self.rightWidget = TwoColumnTable(self)
        self.rightWidget.setColumnCount(2)
        self.rightWidget.setHorizontalHeaderLabels(['Name', 'Value'])
        self.rightWidget.verticalHeader().hide()

        self.splitter.addWidget(self.leftWidget)

        self.rightLayoutWidget = QHBoxLayout()
        self.rightLayoutWidget.addWidget(self.rightWidget)
        self.rightW = QWidget(self.splitter)
        self.rightW.setLayout(self.rightLayoutWidget)
        self.splitter.addWidget(self.rightW)

        self.setLayout(self.theLayout)
        self.splitter.setSizes([200, 400])

    def itemSelected(self):
        ctx = pyudev.Context()
        path = self.leftWidget.getCurrentPath()

        try:
            device = pyudev.Device.from_sys_path(ctx, "/sys" + path)
    
            self.rightWidget.clearContents()
            self.rightWidget.setRowCount(0)

            self.insertLine("Sys path", device.sys_path)
            self.insertLine("Sys name", device.sys_name)
            self.insertLine("Sys number", device.sys_number)
            self.insertLine("Device path", device.device_path)

            for t in device.tags:
                self.insertLine("Device Tag", t)
            else:
                self.insertLine("Device Tags", "None")

            try:
                self.insertLine("Device subsystem", device.subsystem)
            except AttributeError:
                self.insertLine("Device subsystem", "None")

            self.insertLine("Device driver", device.driver)
            self.insertLine("Device type", device.device_type)
            self.insertLine("Device Node", device.device_node)
            self.insertLine("Device number", device.device_number);
            self.insertLine("All properties", "");
            #self.rightWidget.insertPlainText("\nAll properties:\n")
            for p in device:
                self.insertLine("  " + p, device[p])
        except pyudev.device.DeviceNotFoundAtPathError:
            pass


    def insertLine(self, label, value):
        newRow = self.rightWidget.rowCount()
        self.rightWidget.insertRow(newRow)
        self.rightWidget.setItem(newRow, 0, QTableWidgetItem(label))
        self.rightWidget.setItem(newRow, 1, QTableWidgetItem(value))
        self.rightWidget.setRowHeight(newRow, 20)
        if (newRow % 2) == 0:
            color = QColor(255, 255, 0xDE)
        else:
            color = QColor(255, 255, 0xBF)
        self.rightWidget.item( newRow, 0).setBackground( color );
        self.rightWidget.item( newRow, 0).setForeground( Qt.blue );
        self.rightWidget.item( newRow, 1).setBackground( color );
        self.rightWidget.item( newRow, 1).setForeground( Qt.blue );

    def refresh(self):
        self.leftWidget.refresh()


class MainWindow(QMainWindow):

    def __init__(self, app):
        QMainWindow.__init__(self, None)
        self.setWindowTitle("udev Device browser")
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

        self.udevBrowserWidget = UDevBrowserWidget(self)
        self.udevBrowserWidget.refresh()
        self.setCentralWidget(self.udevBrowserWidget)

        self.move(240, 200)
        self.resize(QSize(1024, 768))
        

    def handleAbout(self):
        appVersion = "Development version"
        pythonVersion = "%s.%s.%s (%s)" % (sys.version_info[0], sys.version_info[1], sys.version_info[2], sys.version_info[3])
        pyUdevVersion = pyudev.__version__
        sipVersion = SIP_VERSION_STR
        pyQtVersion = PYQT_VERSION_STR
        pyQtQtVersion = QT_VERSION_STR
        qtRuntimeVersion = qVersion()

        QMessageBox.about(self, "About udev Device browser",
                          "<table>"+
                          "<tr><th align=\"right\">udev Browser version:</th><td>{}</td></tr>".format(appVersion) +
                          "<tr><th align=\"right\">Python version:</th><td>{}</td></tr>".format(pythonVersion) +
                          "<tr><th align=\"right\">PyUdev version:</th><td>{}</td></tr>".format(pyUdevVersion) +
                          "<tr><th align=\"right\">SIP version:</th><td>{}</td></tr>".format(sipVersion) +
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

if __name__ == "__main__":
    main()
