'''
Created on 25.02.2015

@author: afester
'''

from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QTreeWidget, QTreeWidgetItem, QWidget, QToolBar, QAction
from PyQt5.QtWidgets import QVBoxLayout, QDialog, QFileDialog
from PyQt5 import uic

import os, logging
from Notepad import Notepad
from PyQt5.Qt import QAbstractItemView

class AddNotepadDlg(QDialog):
    
    def __init__(self, parentWidget):
        QDialog.__init__(self, parentWidget)
        self.ui = uic.loadUi('AddNotepadDlg.ui', self)

        self.ui.selectDirectory.clicked.connect(self.choosePath)


    def choosePath(self):
        dir = QFileDialog.getExistingDirectory(self, caption='Select Notepad to add')
        self.ui.localPath.setText(dir)


class TreeNode(QTreeWidgetItem):

    def __init__(self, notepad, label):
        QTreeWidgetItem.__init__(self, [label])

        self.notepad = notepad
        self.wasExpanded = False


    def getLabel(self):
        return self.text(0)


    def getNotepad(self):
        return self.notepad


    def setWasExpanded(self,flag):
        self.wasExpanded = flag


    def isWasExpanded(self):
        return self.wasExpanded

    def __repr__(self, *args, **kwargs):
        return 'TreeNode[label={}, notepad={}]'.format(self.text(0), self.notepad.getName())


class TreeWidget(QTreeWidget):


    def __init__(self, parentWidget):
        QTreeWidget.__init__(self, parentWidget)
        self.setColumnCount(1)
        self.setHeaderLabel("Notepads")
        self.itemExpanded.connect(self.expandItem)


    def expandItem(self, item):
        if not item.isWasExpanded():
            notepad = item.getNotepad()
            
            if item.parent() is None:
                page = notepad.getPage(None)
            else:
                page = notepad.getPage(item.getLabel())
    
            for keyword in page.getLinks():
                linkItem = TreeNode(notepad, keyword)
    
                page = notepad.getPage(keyword)
                page.load()
                if len(page.getLinks()) > 0:
                    linkItem.setChildIndicatorPolicy(QTreeWidgetItem.ShowIndicator)
                item.addChild(linkItem)
            item.setWasExpanded(True)


    # Add all top level items (Notepads).
    # As soon as one of them is expanded (either by the user or programmatically),
    # the expandHandler method takes care of adding the childs as required
    def refresh(self, notepad):
        rootPage = notepad.getPage(None)

        rootItem = TreeNode(notepad, notepad.getName())
        self.addTopLevelItem(rootItem)

        if len(rootPage.getLinks()) > 0:
            rootItem.setChildIndicatorPolicy(QTreeWidgetItem.ShowIndicator)


    def addNotepad(self, notepad):
        rootItem = TreeNode(notepad, notepad.getName(), 0)
        self.addTopLevelItem(rootItem)
        self.setCurrentItem(rootItem)
        rootItem.setExpanded(True)


class BrowserWidget(QWidget):
    ''' Tree widget and button bar above '''

    l = logging.getLogger('Browser')

    itemSelected = pyqtSignal()

    def __init__(self, parentWidget, settings):
        QWidget.__init__(self, parentWidget)
        self.settings = settings

        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)

        addAction = QAction("+", toolbar)
        addAction.triggered.connect(self.addNotepad)
        toolbar.addAction(addAction)

        removeAction = QAction("-", toolbar)
        removeAction.triggered.connect(self.removeNotepad)
        toolbar.addAction(removeAction)

        self.browserView = TreeWidget(self)

        hLayout = QVBoxLayout(self)
        hLayout.addWidget(toolbar)
        hLayout.addWidget(self.browserView)

        self.currentItem = None
        self.browserView.itemSelectionChanged.connect(self.handleItemSelected)


    def handleItemSelected(self):
        selItems = self.browserView.selectedItems()
        if len(selItems) == 1:
            self.currentItem = selItems[0]
            self.itemSelected.emit()


    def addNotepad(self):
        dlg = AddNotepadDlg(self)
        if dlg.exec() == QDialog.Accepted:
            npDef = None

            # create a notepad definition from user input
            npType = dlg.ui.storageType.currentIndex()
            if npType == 0:         # LOCAL (TODO: enum)
                npPath = dlg.ui.localPath.text()
                npName = os.path.basename(npPath)
                npDef = {'name' : npName,
                         'type'  : 'local',
                         'path'   : npPath }
            elif npType == 1:       # DROPBOX (TODO: enum)
                npDef = {'name' : 'TODO',
                         'type' : 'dropbox'}


            if npDef is not None:
                notepad = Notepad(npDef)
                notepad.ensureExists()

                # Add the new notepad to the local settings
                self.settings.addNotepad(npDef)

                # Add the new notepad to the browser tree
                self.browserView.addNotepad(notepad)


    def removeNotepad(self):
        pass


    def refresh(self):
        # Reload all notepads
        self.l.debug("Refreshing browser ...")
        notepads = self.settings.getNotepads()
        for np in notepads:
            notepad = Notepad(np)
            self.browserView.refresh(notepad)

        # Select and expand the first notepad (todo: store last selection as preference)
        first = self.browserView.topLevelItem(0)
        if first:
            first.setSelected(True)
            first.setExpanded(True)
