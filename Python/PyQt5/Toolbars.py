

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

    styleChanged = pyqtSignal(object)
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

##########################
        d1 = IconDropDown(self, 'icons/format-text-paragraph.png')
        d1.entrySelected.connect(self.styleSelected)

        selector = ('para', None, None)
        d1.addItem("Standard", selector)
        self.styleToDropdown[selector] = d1
        self.styleToIndex[selector] = 0

        selector = ('tip', None, None)
        d1.addItem('Tip', selector)
        self.styleToDropdown[selector] = d1
        self.styleToIndex[selector] = 1

        selector = ('warning', None, None)
        d1.addItem('Warning', selector)
        self.styleToDropdown[selector] = d1
        self.styleToIndex[selector] = 2

        selector = ('blockquote', None, None)
        d1.addItem('Quotation', selector)
        self.styleToDropdown[selector] = d1
        self.styleToIndex[selector] = 3

##########################
        d2 = IconDropDown(self, 'icons/format-text-header.png')
        d2.entrySelected.connect(self.styleSelected)

        selector = ('title', 'level', '1')
        d2.addItem('Title 1', selector)
        self.styleToDropdown[selector] = d2
        self.styleToIndex[selector] = 0

        selector = ('title', 'level', '2')
        d2.addItem('Title 2', selector)
        self.styleToDropdown[selector] = d2
        self.styleToIndex[selector] = 1

        selector = ('title', 'level', '3')
        d2.addItem('Title 3', selector)
        self.styleToDropdown[selector] = d2
        self.styleToIndex[selector] = 2

##########################
        d3 = IconDropDown(self, "icons/format-list-unordered.png")
        d3.entrySelected.connect(self.styleSelected)

        selector = ('itemizedlist', 'level', '1')
        d3.addItem("Unordered List", selector)
        self.styleToDropdown[selector] = d3
        self.styleToIndex[selector] = 0

        selector = ('orderedlist', 'level', '1')
        d3.addItem("Ordered List", selector)
        self.styleToDropdown[selector] = d3
        self.styleToIndex[selector] = 1

##########################
        d4 = IconDropDown(self, "icons/format-text-code.png")
        d4.entrySelected.connect(self.styleSelected)

        selector = ('programlisting', 'language', 'cpp')
        d4.addItem("C++", selector)
        self.styleToDropdown[selector] = d4
        self.styleToIndex[selector] = 0

        selector = ('programlisting', 'language', 'java')
        d4.addItem("Java", selector)
        self.styleToDropdown[selector] = d4
        self.styleToIndex[selector] = 1
        
        selector = ('programlisting', 'language', 'python')
        d4.addItem("Python", selector)
        self.styleToDropdown[selector] = d4
        self.styleToIndex[selector] = 2
        
        selector = ('programlisting', 'language', 'sql')
        d4.addItem("SQL", selector)
        self.styleToDropdown[selector] = d4
        self.styleToIndex[selector] = 3
        
        selector = ('programlisting', 'language', 'xml')
        d4.addItem("XML", selector)
        self.styleToDropdown[selector] = d4
        self.styleToIndex[selector] = 4

        selector = ('programlisting', 'language', 'bash')
        d4.addItem("Bash", selector)
        self.styleToDropdown[selector] = d4
        self.styleToIndex[selector] = 5
        
        selector = ('screen', None, None)
        d4.addItem("Generic", selector)
        self.styleToDropdown[selector] = d4
        self.styleToIndex[selector] = 6

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

    styleChanged = pyqtSignal(object, bool)

    def __init__(self, toolbar):
        TitledToolbar.__init__(self, toolbar, 'Text style')
        self.currentStyle = None

        toolbar = QToolBar(self)
        toolbar.setFloatable(False)
        toolbar.setMovable(False)

        self.styleToAction = {}

        textKeywordAction = QAction(QIcon("icons/format-keyword.png"), "Notepad link", toolbar)
        textKeywordAction.setCheckable(True);
        selector = ('olink', None, None)
        textKeywordAction.setProperty('style', selector)
        self.styleToAction[selector] = textKeywordAction
        textKeywordAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textKeywordAction)

        textLinkAction = QAction(QIcon("icons/format-link.png"), "Internet link", toolbar)
        textLinkAction.setCheckable(True);
        selector = ('link', None, None)
        textLinkAction.setProperty('style', selector)
        self.styleToAction[selector] = textLinkAction 
        textLinkAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textLinkAction)

        textBoldAction = QAction(QIcon("icons/format-text-emphasized.png"), "Emphasize", toolbar)
        textBoldAction.setCheckable(True);
        selector = ('emphasis', None, None)
        textBoldAction.setProperty('style', selector)
        self.styleToAction[selector] = textBoldAction
        textBoldAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textBoldAction)

        textHighlightAction = QAction(QIcon("icons/format-text-highlight.png"), "Highlight", toolbar)
        textHighlightAction.setCheckable(True);
        selector = ('emphasis', 'role', 'highlight')
        textHighlightAction.setProperty('style', selector)
        self.styleToAction[selector] = textHighlightAction
        textHighlightAction.triggered.connect(self.styleSelected)
        toolbar.addAction(textHighlightAction)
 
        textCodeAction = QAction(QIcon("icons/format-text-code.png"), "Code", toolbar)
        textCodeAction.setCheckable(True);
        selector = ('code', None, None)
        textCodeAction.setProperty('style', selector)
        self.styleToAction[selector] = textCodeAction
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

        findInPageAction = QAction(QIcon("icons/edit-find.png"), "Find in page (CTRL-F)", toolbar)
        findInPageAction.setShortcut(Qt.CTRL + Qt.Key_F);
        findInPageAction.triggered.connect(parent.findInPage)
        toolbar.addAction(findInPageAction)

        self.addWidget(toolbar)
