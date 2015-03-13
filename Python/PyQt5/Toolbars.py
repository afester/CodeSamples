

from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtWidgets import QToolBar, QAction, QLabel, QComboBox, QWidget
from PyQt5.QtWidgets import QVBoxLayout, QFrame,  QSizePolicy, QHBoxLayout
from PyQt5.QtGui  import QIcon,  QPixmap


class IconDropDown(QWidget):
    
    entrySelected = pyqtSignal(int)

    def __init__(self, parentWidget, iconName):
        QWidget.__init__(self, parentWidget)

        icon = QLabel(self)
        icon.setPixmap(QPixmap(iconName))

        self.dropdown = QComboBox(self)
        self.dropdown.activated.connect(self.entrySelected)

        hLayout = QVBoxLayout(self)
        hLayout.setContentsMargins(5, 5, 5, 5)
        
        self.setLayout(hLayout)
        hLayout.addWidget(icon, 0, Qt.AlignHCenter)
        hLayout.addWidget(self.dropdown)


    def addItem(self, title, tag):
        self.dropdown.addItem(title, tag)
        self.dropdown.setCurrentIndex(-1)   # TODO - can not set index to -1 before items have been added


    def setCurrentIndex(self, idx):
        self.dropdown.setCurrentIndex(idx)


class TitledToolbar(QFrame):

    def __init__(self, parent, title):
        QFrame.__init__(self, parent)
        self.setFrameStyle(QFrame.StyledPanel)

        titleLabel = QLabel(title, self)
        contentPanel = QWidget(self)
        contentPanel.setSizePolicy(QSizePolicy.Expanding, QSizePolicy.MinimumExpanding)
        self.contentLayout = QHBoxLayout()
        self.contentLayout.setContentsMargins(0, 0, 0, 0)
        contentPanel.setLayout(self.contentLayout)

        # style for the label
        titleLabel.setStyleSheet('''.QLabel {
            color: #3E6AAA;
        }''') 

        # style for the content panel
        contentPanel.setStyleSheet('''.QWidget { 
            border-bottom-left-radius: 5px;  
            border-bottom-right-radius: 5px;
            background-color: #D6E5F3; 
        }''')

        # style for the whole toolbar
        self.setStyleSheet('''TitledToolbar { 
            background-color:#C1D9F1; 
            color:#6D6AB7; 
            border-radius: 5px;  
            border-style: solid; 
            border-width: 1px; 
            border-color: #6D6AB7;
        }''')

        myLayout = QVBoxLayout()
        myLayout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(myLayout)

        myLayout.addWidget(titleLabel, 0, Qt.AlignHCenter | Qt.AlignTop)
        myLayout.addWidget(contentPanel)


    def addWidget(self, wdg):
        self.contentLayout.addWidget(wdg)


class BlockStyleSelector(TitledToolbar):

    styleChanged = pyqtSignal(str)
    indentLess = pyqtSignal()
    indentMore = pyqtSignal()
    insertTable = pyqtSignal()

    def __init__(self, parent):
        TitledToolbar.__init__(self, parent, 'Block style')
        self.currentStyle = None


        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)

        self.styleToIndex = {}
        self.styleToDropdown = {}

        d2 = IconDropDown(self, "icons/format-text-header.png")
        d2.entrySelected.connect(self.styleSelected)
        d2.addItem('Header 1', 'h1')
        self.styleToDropdown['h1'] = d2
        self.styleToIndex['h1'] = 0
        d2.addItem('Header 2', 'h2')
        self.styleToDropdown['h2'] = d2
        self.styleToIndex['h2'] = 1
        d2.addItem('Header 3', 'h3')
        self.styleToDropdown['h3'] = d2
        self.styleToIndex['h3'] = 2

        d1 = IconDropDown(self, "icons/format-text-paragraph.png")
        d1.entrySelected.connect(self.styleSelected)
        d1.addItem("Standard", 'p')
        self.styleToDropdown['p'] = d1
        self.styleToIndex['p'] = 0
        d1.addItem("Centered", 'pcenter')
        self.styleToDropdown['pcenter'] = d1
        self.styleToIndex['pcenter'] = 1
        d1.addItem("Block", 'pblock')
        self.styleToDropdown['pblock'] = d1
        self.styleToIndex['pblock'] = 2

        d3 = IconDropDown(self, "icons/format-list-unordered.png")
        d3.entrySelected.connect(self.styleSelected)
        d3.addItem("Unordered List", 'ul1')
        self.styleToDropdown['u1l'] = d3
        self.styleToIndex['ul1'] = 0
        d3.addItem("Ordered List", 'ol1')
        self.styleToDropdown['ol1'] = d3
        self.styleToIndex['ol1'] = 1

        d4 = IconDropDown(self, "icons/format-text-code.png")
        d4.entrySelected.connect(self.styleSelected)
        d4.addItem("C++", 'cppcode')
        self.styleToDropdown['cppcode'] = d4
        self.styleToIndex['cppcode'] = 0
        d4.addItem("Java", 'javacode')
        self.styleToDropdown['javacode'] = d4
        self.styleToIndex['javacode'] = 1
        d4.addItem("Python", 'pycode')
        self.styleToDropdown['pycode'] = d4
        self.styleToIndex['pycode'] = 2
        d4.addItem("SQL", 'sqlcode')
        self.styleToDropdown['sqlcode'] = d4
        self.styleToIndex['sqlcode'] = 3
        d4.addItem("XML", 'xmlcode')
        self.styleToDropdown['xmlcode'] = d4
        self.styleToIndex['xmlcode'] = 4
        d4.addItem("Generic", 'screen')
        self.styleToDropdown['screen'] = d4
        self.styleToIndex['screen'] = 5

        toolbar.addWidget(d1)
        toolbar.addWidget(d2)
        toolbar.addWidget(d3)
        toolbar.addWidget(d4)

        intendLessAction = QAction(QIcon("icons/format-indent-less.png"), "", toolbar) 
        intendLessAction.triggered.connect(self.indentLess)
        toolbar.addAction(intendLessAction)

        intendMoreAction = QAction(QIcon("icons/format-indent-more.png"), "", toolbar) 
        intendMoreAction.triggered.connect(self.indentMore)
        toolbar.addAction(intendMoreAction)

        # Can not currently support tables because the QTextTableCell python
        # binding is missing methods to traverse the cell contents
        #insertTableAction = QAction("Table", toolbar) 
        #insertTableAction.triggered.connect(self.insertTable)
        #toolbar.addAction(insertTableAction)

        self.addWidget(toolbar)


    def addCategory(self):
        pass
    

    def addStyle(self):
        pass


    def styleSelected(self):
        icd = self.sender()
        dd = icd.dropdown
        style = dd.currentData()
        self.styleChanged.emit(style)


    def setCurrentStyle(self, styleName):
        if self.currentStyle != styleName:
            self.currentStyle = styleName
            for dd in self.styleToDropdown.values():
                dd.setCurrentIndex(-1)
            if styleName is not None and styleName != '':
                dd = self.styleToDropdown[styleName]
                di = self.styleToIndex[styleName]
                if dd is not None and di is not None:
                    dd.setCurrentIndex(di)


class TextStyleSelector(TitledToolbar):

    styleChanged = pyqtSignal(str, bool)

    def __init__(self, toolbar):
        TitledToolbar.__init__(self, toolbar, 'Text style')
        self.currentStyle = None

        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)

        self.styleToAction = {}

        textKeywordAction = QAction(QIcon("icons/format-keyword.png"), "Notepad link", toolbar)
        textKeywordAction.setCheckable(True);
        textKeywordAction.setProperty('style', 'keyword')
        self.styleToAction['keyword'] = textKeywordAction
        textKeywordAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textKeywordAction)

        textLinkAction = QAction(QIcon("icons/format-link.png"), "Internet link", toolbar)
        textLinkAction.setCheckable(True);
        textLinkAction.setProperty('style', 'a')
        self.styleToAction['a'] = textLinkAction 
        textLinkAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textLinkAction)

        textBoldAction = QAction(QIcon("icons/format-text-emphasized.png"), "Emphasize", toolbar)
        textBoldAction.setCheckable(True);
        textBoldAction.setProperty('style', 'strong')
        self.styleToAction['strong'] = textBoldAction
        textBoldAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textBoldAction)

        textHighlightAction = QAction(QIcon("icons/format-text-highlight.png"), "Highlight", toolbar)
        textHighlightAction.setCheckable(True);
        textHighlightAction.setProperty('style', 'em')
        self.styleToAction['em'] = textHighlightAction
        textHighlightAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textHighlightAction)
 
        textCodeAction = QAction(QIcon("icons/format-text-code.png"), "Code", toolbar)
        textCodeAction.setCheckable(True);
        textCodeAction.setProperty('style', 'tt')
        self.styleToAction['tt'] = textCodeAction
        textCodeAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textCodeAction)

        self.addWidget(toolbar)


    def styleSelected(self):
        action = self.sender()
        style = action.property('style')
        flag = action.isChecked()
        self.styleChanged.emit(style, flag)


    def setCurrentStyle(self, styleName):
        if self.currentStyle != styleName:
            self.currentStyle = styleName
            for action in self.styleToAction.values():
                action.setChecked(False)
            if styleName is not None and styleName != '':
                action = self.styleToAction[styleName]
                if action is not None:
                    action.setChecked(True)


class ActionSelector(TitledToolbar):

    def __init__(self, parent):
        TitledToolbar.__init__(self, parent, 'Actions')
 
        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)

        self.saveAction = QAction(QIcon("icons/file-save.png"), "Save (Ctrl-S)", toolbar)
        self.saveAction.setShortcut(Qt.CTRL + Qt.Key_S);
        self.saveAction.triggered.connect(parent.save)
        toolbar.addAction(self.saveAction)

        self.nonprintableAction = QAction(QIcon("icons/view-nonprintable.png"), "View nonprintable chars", toolbar)
        self.nonprintableAction.setCheckable(True);
        self.nonprintableAction.triggered.connect(parent.toggleNonprintable)
        toolbar.addAction(self.nonprintableAction)

        self.undoAction = QAction(QIcon("icons/edit-undo.png"), "Undo (Ctrl-Z)", toolbar)
        # saveAction.setShortcut(Qt.CTRL + Qt.Key_Z);
        self.undoAction.triggered.connect(parent.undo)
        toolbar.addAction(self.undoAction)

        self.redoAction = QAction(QIcon("icons/edit-redo.png"), "Redo (Ctrl-Y)", toolbar)
        # saveAction.setShortcut(Qt.CTRL + Qt.Key_Y);
        self.redoAction.triggered.connect(parent.redo)
        toolbar.addAction(self.redoAction)

        self.backAction = QAction(QIcon("icons/view-back.png"), "Back", toolbar)
        self.backAction.setEnabled(False)
        self.backAction.triggered.connect(parent.navigateBack)
        toolbar.addAction(self.backAction)

        self.forwardAction = QAction(QIcon("icons/view-forward.png"), "Forward", toolbar)
        self.forwardAction.setEnabled(False)
        self.forwardAction.triggered.connect(parent.navigateForward)
        toolbar.addAction(self.forwardAction)

        insertImageAction = QAction(QIcon("icons/edit-insert-image.png"), "Insert Image", toolbar)
        insertImageAction.triggered.connect(parent.insertImage)
        toolbar.addAction(insertImageAction)

        self.addWidget(toolbar)
