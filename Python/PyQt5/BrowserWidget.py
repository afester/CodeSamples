'''
Created on 25.02.2015

@author: afester
'''

from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QTreeWidget, QTreeWidgetItem, QWidget, QToolBar, QAction
from PyQt5.QtWidgets import QVBoxLayout, QDialog, QFileDialog, QDialogButtonBox
from PyQt5 import uic

import os, logging
from Notepad import LocalNotepad, DropboxNotepad
import dropbox
from dropbox.rest import ErrorResponse


class AddNotepadDlg(QDialog):
    
    def __init__(self, parentWidget, settings):
        QDialog.__init__(self, parentWidget)
        self.settings = settings

        self.ui = uic.loadUi('AddNotepadDlg.ui', self)

        self.ui.buttonBox.clicked.connect(self.applyAuth)   # TODO
 
        self.ui.storageTypeUI.setCurrentIndex(0)
        self.ui.dropboxPages.setCurrentIndex(0)
        self.ui.localPath.setFocus()

        self.ui.selectDirectory.clicked.connect(self.choosePath)
        self.ui.storageType.activated.connect(self.changeStorage)


    def choosePath(self):
        pathName = QFileDialog.getExistingDirectory(self, caption='Select Notepad to add')
        self.ui.localPath.setText(pathName)


    def changeStorage(self, index):
        if index == 0:
            self.ui.buttonBox.setStandardButtons(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        elif index == 1:
            if len(self.settings.getDropboxToken()) == 0:
                # No authentication token available
                self.ui.dropboxPages.setCurrentIndex(0)
                self.ui.buttonBox.setStandardButtons(QDialogButtonBox.Apply| QDialogButtonBox.Cancel)

                self.flow = dropbox.client.DropboxOAuth2FlowNoRedirect('9tuq93gk3wkq242','4au1g1d5vp6phq3') 
                authorize_url = self.flow.start()

                self.ui.authUrl.setText('<a href="{0}">{0}</a>'.format(authorize_url))
                self.ui.authCode.setFocus()
            else:
                self.ui.buttonBox.setStandardButtons(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
                self.ui.dropboxPages.setCurrentIndex(1)

                client = dropbox.client.DropboxClient(self.settings.getDropboxToken())
                info = client.account_info()

                self.ui.userName.setText('{} <{}>'.format(info['display_name'], info['email']))
                self.ui.notepadName.setFocus()


    def applyAuth(self, button):
        if button.text() == "Apply":    # TODO!!!!
            authCode = self.ui.authCode.text()

            try:
                accessToken, user_id = self.flow.finish(authCode)
                self.settings.setDropboxToken(accessToken)
                self.changeStorage(1)
            except ErrorResponse as rsp:
                self.ui.errorMessage.setText('Error: {}'.format(rsp))


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
        self.itemExpanded.connect(self.addLazyChildren)


    def getCurrentPath(self):
        result = []

        current = self.currentItem()
        while current is not None:
            result.insert(0, current.getLabel()) 
            current = current.parent()

        return result


    def addLazyChildren(self, item):
        """Adds the children to a given item after it has been clicked.

        If the item was not previously expanded, the children will be added
        to the item. 

        Args:
          item (TreeNode):  The item to expand.
"""

        if not item.isWasExpanded():
            notepad = item.getNotepad()
            
            if item.parent() is None:
                page = notepad.getPage(None)
            else:
                page = notepad.getPage(item.getLabel())
    
            for keyword in page.getLinks():
                linkItem = TreeNode(notepad, keyword)
    
                page = notepad.getPage(keyword)
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
        rootItem = TreeNode(notepad, notepad.getName())
        self.addTopLevelItem(rootItem)
        self.setCurrentItem(rootItem)
        rootItem.setExpanded(True)


    def expandChild(self, item, path):
        """Expands all nodes on the given path, starting at a specific item

        Args:
          item (TreeNode):  The start node

          path ([str, str, ...]): The path to expand - a list of node labels
"""
        item.setExpanded(True)
        if len(path) == 0:
            self.setCurrentItem(item)
            return

        for idx in range(0, item.childCount()):
            childItem = item.child(idx)
            if childItem.getLabel() == path[0]:
                self.expandChild(childItem, path[1:])
                break


    def expandPath(self, path):
        """Expands a given path starting with a root node.

        Args:
          path ([str, str, ...]): The path to expand - a list of node labels.
            The first element is the root node.
"""
        if len(path) == 0:
            return

        rootItem = None
        for idx in range(0, self.topLevelItemCount()):
            item = self.topLevelItem(idx)
            if item.getLabel() == path[0]:
                rootItem = item
                break

        if rootItem is not None:
            self.expandChild(rootItem, path[1:])


    def navigateToChild(self, pageId):
        """Sets a specific child item of the current item as the current item.

        Args:
          pageId (str): The label of the child item to activate.
"""
        current = self.currentItem()
        label = current.getLabel()
        self.addLazyChildren(current)

        node = None
        for idx in range(0, current.childCount()):
            child = current.child(idx)
            label = child.getLabel()
            if label == pageId:
                node = child
                break

        if node is None:
            node = TreeNode(current.getNotepad(), pageId)
            current.addChild(node)
        self.setCurrentItem(node)


    def navigateToFirstChild(self, pageId):
        """Sets the first occurrence of a specific item in the current tree as the
        current item.
        TODO: This requires a (complete) tree traversal which might be expensive
        due to the lazy initialization of the tree structure!

        Args:
          pageId (str): The label of the item to activate.
"""
        notepad = self.currentItem().getNotepad()
        path = notepad.findPathToPage(pageId)
        self.expandPath(path)



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
        hLayout.setContentsMargins(0, 0, 0, 1)
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
        dlg = AddNotepadDlg(self, self.settings)
        if dlg.exec() == QDialog.Accepted:
            notepad = None

            # create a notepad definition from user input
            npType = dlg.ui.storageType.currentIndex()
            if npType == 0:         # LOCAL (TODO: enum)
                npPath = dlg.ui.localPath.text()
                npName = os.path.basename(npPath)
                npDef = {'name' : npName,
                         'type'  : 'local',
                         'path'   : npPath }
                notepad = LocalNotepad(npDef)
            elif npType == 1:       # DROPBOX (TODO: enum)
                npName = dlg.ui.notepadName.text()
                npDef = {'name' : npName,
                         'type' : 'dropbox'}
                notepad = DropboxNotepad(npDef, self.settings)

            if notepad is not None:
                # Add the new notepad to the local settings
                self.settings.addNotepad(npDef)

                # Add the new notepad to the browser tree.
                # This will expand the node and load the page, and the page
                # will get created if it does not exist yet
                self.browserView.addNotepad(notepad)


    def removeNotepad(self):
        pass


    def refresh(self):
        # Reload all notepads
        self.l.debug("Refreshing browser ...")
        
        # Add notepads to the browser tree
        notepads = self.settings.getNotepads()
        for np in notepads:
            if np['type'] == 'local':
                notepad = LocalNotepad(np)
            elif np['type'] == 'dropbox':
                notepad = DropboxNotepad(np, self.settings)
            self.browserView.refresh(notepad)

        # Expand to and select the previous item
        path = self.settings.getBrowserPath()
        self.browserView.expandPath(path)


    def navigate(self, pageId):
        self.browserView.navigateToChild(pageId)

    def navigateDirect(self, pageId):
        self.browserView.navigateToFirstChild(pageId)

    def getCurrentPath(self):
        return self.browserView.getCurrentPath()
