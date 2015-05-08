'''
Created on 18.02.2015

@author: afester
'''
 
from PyQt5.Qt import QIcon
from PyQt5.QtCore import pyqtSignal, Qt
from PyQt5.QtGui import QTextOption, QImage, QTextDocument
from PyQt5.QtWidgets import QWidget, QMessageBox, QPushButton, QAction
from PyQt5.QtWidgets import QTextEdit, QVBoxLayout, QLineEdit, QHBoxLayout, QFileDialog, QToolButton

import os, logging

from ui.Toolbars import ActionSelector, TextStyleSelector, BlockStyleSelector

from StylableTextEdit.StylableTextEdit import StylableTextEdit, MathFormulaObject
from StylableTextEdit.FormatManager import FormatManager




class CancelAwareLineEdit(QLineEdit):
    
    def __init__(self, parentWidget):
        QLineEdit.__init__(self, parentWidget)

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            self.editingFinished.emit()

        return QLineEdit.keyPressEvent(self, event)


class FindWidget(QWidget):

    def __init__(self, parentWidget, markerFmt, editWidget):
        QWidget.__init__(self, parentWidget)

        self.markerFormat = markerFmt
        self.textEdit = editWidget
        self.startPos = 0
        self.endPos = 0
        self.pattern = ''

        layout = QHBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)

        self.searchPattern = CancelAwareLineEdit(self)
        self.searchPattern.setPlaceholderText('Start typing to find in page')
        self.searchPattern.editingFinished.connect(self.hideWidget)
        self.searchPattern.textEdited.connect(self.doSearch)

        upAction = QAction(QIcon('icons/find-up.png'), "Find backwards (Shift-F3)", self)
        upAction.setShortcut(Qt.SHIFT + Qt.Key_F3);
        upAction.triggered.connect(self.findUpwards)
        self.upButton = QToolButton(self)
        self.upButton.setDefaultAction(upAction)

        downAction = QAction(QIcon('icons/find-down.png'), "Find next (F3)", self)
        downAction.setShortcut(Qt.Key_F3);
        downAction.triggered.connect(self.findDownwards)
        self.downButton = QToolButton(self)
        self.downButton.setDefaultAction(downAction)

        layout.addWidget(self.searchPattern)
        layout.addWidget(self.upButton)
        layout.addWidget(self.downButton)


    def hideWidget(self):
        self.hide()
        self.textEdit.setExtraSelections([])

    def showWidget(self):
        self.show()
        self.searchPattern.setFocus()
        self.searchPattern.clear()


    def doSearch(self, pattern):
        # Modifying the search pattern restarts from the beginning of the document.
        self.startPos = 0
        self.endPos = 0
        self.pattern = pattern
        self.textEdit.setExtraSelections([])
        self.findDownwards()


    def findUpwards(self):
        # print('Search up "{}" starting at {} ...'.format(self.pattern, self.startPos))
        self.find(True)


    def findDownwards(self):
        # print('Search down "{}" starting at {} ...'.format(self.pattern, self.endPos))
        self.find(False)


    def find(self, isUpwards):
        doc = self.textEdit.document()
        if isUpwards:
            foundCrsr = doc.find(self.pattern, self.startPos, QTextDocument.FindBackward)
        else:
            foundCrsr = doc.find(self.pattern, self.endPos)

        if not foundCrsr.isNull():
            self.startPos = foundCrsr.selectionStart()
            self.endPos = foundCrsr.selectionEnd()

            sel = QTextEdit.ExtraSelection()
            sel.cursor = foundCrsr
            sel.format = self.markerFormat
            self.textEdit.setExtraSelections([sel])

            # make sure that the text which was found is also visible
            crsr = self.textEdit.textCursor()
            crsr.setPosition(self.endPos)
            self.textEdit.setTextCursor(crsr)
 

class MathEditWidget(QWidget):

    apply = pyqtSignal()

    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        layout = QHBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        self.setLayout(layout)

        self.formulaEditor = QTextEdit()
        self.formulaEditor.setAcceptRichText(False)

        self.applyButton = QPushButton('Apply')
        self.applyButton.clicked.connect(self.apply)

        layout.addWidget(self.formulaEditor)
        layout.addWidget(self.applyButton)

    def hideWidget(self):
        self.hide()

    def showWidget(self):
        self.show()
        self.formulaEditor.setFocus()

    def setFormula(self, formula):
        self.formulaEditor.setText(formula)

    def getFormula(self):
        return self.formulaEditor.toPlainText()


class EditorWidget(QWidget):
    
    l = logging.getLogger('EditorWidget')

    message = pyqtSignal(str)
    titleChanged = pyqtSignal(str)
    navigate = pyqtSignal(str)


    def __init__(self, parentWidget):
        QWidget.__init__(self, parentWidget)

        self.formatManager = FormatManager()
        self.formatManager.loadFormats()

        self.editView = StylableTextEdit(self, self.formatManager)
        self.editView.navigate.connect(self.navigate)
        self.editView.objectSelectionChanged.connect(self.objectSelectionChanged)

        toolbar = QWidget(self)
        layout = QHBoxLayout()
        layout.setContentsMargins(0,0,0,0)
        toolbar.setLayout(layout)

        self.actionsSelector = ActionSelector(self)
        layout.addWidget(self.actionsSelector)

        self.textFormatSelector = TextStyleSelector(self)
        self.textFormatSelector.styleChanged.connect(self.editView.applyTextFormat)
        self.editView.updateCharFormat.connect(self.textFormatSelector.setCurrentStyle)
        layout.addWidget(self.textFormatSelector)

        self.styleSelector = BlockStyleSelector(self)
        self.styleSelector.styleChanged.connect(self.editView.applyBlockStyle)
        self.styleSelector.indentLess.connect(self.editView.indentLess)
        self.styleSelector.indentMore.connect(self.editView.indentMore)
        self.styleSelector.insertTable.connect(self.insertTable)
        self.editView.updateBlockFormat.connect(self.styleSelector.setCurrentStyle)
        layout.addWidget(self.styleSelector)

        searchMarker = self.formatManager.getFormat( ('searchMarker', None, None) )
        self.findWidget = FindWidget(self, searchMarker.getCharFormat(), self.editView)
        self.findWidget.hide()

        self.editMathWidget = MathEditWidget(self)
        self.editMathWidget.apply.connect(self.applyMathFormula)
        self.editMathWidget.hide()

        #horizontalSpacer = QSpacerItem(0, 0) # 40, 20) # , QSizePolicy.Expanding, QSizePolicy.Minimum)
        #layout.addItem(horizontalSpacer)

        hLayout = QVBoxLayout(self)
        hLayout.setContentsMargins(0, 0, 0, 0)
        hLayout.addWidget(toolbar)
        hLayout.addWidget(self.findWidget)
        hLayout.addWidget(self.editView)
        hLayout.addWidget(self.editMathWidget)

        # The toolbar should take the minimum space and the edit view the remaining space
        hLayout.setStretch(0, 0)
        hLayout.setStretch(1, 0)
        hLayout.setStretch(2, 1)
        hLayout.setStretch(3, 0.1)


    def load(self, notepad, pageId):
        self.l.debug('Loading page "{}" from notepad "{}"'.format(pageId, notepad.getName()))

        self.page = notepad.getPage(pageId)
        doc = self.page.getDocument()

        self.editView.blockSignals(True)
        self.editView.setDocument(doc)
        self.editView.blockSignals(False)

        # TODO: review
        doc.undoAvailable.connect(self.actionsSelector.undoAction.setEnabled)
        doc.redoAvailable.connect(self.actionsSelector.redoAction.setEnabled)

        # Setup modification flag handling
        doc.modificationChanged.connect(self.updateWindowTitle)
        doc.setModified(False)

        doc.setUndoRedoEnabled(True)
        self.actionsSelector.undoAction.setEnabled(False)
        self.actionsSelector.redoAction.setEnabled(False)

        self.editView.setFocus()
        self.editView.handleCursorPositionChanged()    # update toolbars

        self.contentFile = os.path.join(notepad.getName(),str(pageId))
        self.message.emit('Loaded {}: {}'.format(notepad.getName(), self.page.getName()))


    def updateWindowTitle(self, flag):
        self.actionsSelector.saveAction.setEnabled(flag)
        self.titleChanged.emit('{}{}: {}'.format('*' if flag else '', self.page.notepad.getRootpath(), self.page.getName()))


## Actions

    def save(self):
        if self.editView.document().isModified():
            self.page.save()
            self.message.emit('Saved {}: {}'.format(self.page.notepad.getName(), self.page.getName()))


    def toggleNonprintable(self):
        if self.actionsSelector.nonprintableAction.isChecked():
            option = QTextOption()
            option.setFlags(QTextOption.ShowTabsAndSpaces | QTextOption.ShowLineAndParagraphSeparators)
            self.editView.document().setDefaultTextOption(option)
        else:
            option = QTextOption()
            self.editView.document().setDefaultTextOption(option)

    
    def navigateBack(self):
        QMessageBox.information(self, "Error", "navigateBack not yet implemented")


    def navigateForward(self):
        QMessageBox.information(self, "Error", "navigateForward not yet implemented")


    def undo(self):
        self.editView.undo()


    def redo(self):
        self.editView.redo()


    def insertImage(self):
        filePath = QFileDialog.getOpenFileName(self, caption='Select Image file')[0]
        # print('{}: "{}" {}'.format(type(filePath), filePath, len(filePath)))
        if len(filePath) > 0:
            image = QImage(filePath)
            if image.isNull():
                QMessageBox.information(self, "Error", '"{}"\nis not a valid image file'.format(filePath))
            else:
                self.editView.insertImage(image)


    def insertFormula(self):
        self.editView.insertFormula()



    def insertTable(self):
        cursor = self.editView.textCursor()
        
        #fmt = QTextFrameFormat()
        #fmt.setBorder(2)
        #fmt.setBorderStyle(QTextFrameFormat.BorderStyle_Dashed)
        #cursor.insertFrame(fmt)

        cursor.insertTable(2, 2)
        cursor.insertText("Hello World")





#===============================================================================
#     def selectedBlocks(self):
#         cursor = self.editView.textCursor()
#         document = self.editView.document()
# 
#         startBlock = document.findBlock(cursor.selectionStart()) 
#         endBlock = document.findBlock(cursor.selectionEnd())
#         done = False
#         while not done:
#             yield startBlock
#             if startBlock == endBlock:
#                 done = True
#             else:
#                 startBlock = startBlock.next()
#===============================================================================


    def findInPage(self):
        self.findWidget.showWidget()


    def objectSelectionChanged(self):
        obj = self.editView.getSelectedObject()
        if type(obj) is MathFormulaObject:              # type(None) is also NOT MathFormulaObject
            self.editMathWidget.setFormula(obj.formula)
            self.editMathWidget.showWidget()
        else:
            self.editMathWidget.hideWidget()


    def applyMathFormula(self):
        obj = self.editView.getSelectedObject()
        if obj is not None:         # should always be true!
            obj.setFormula(self.editMathWidget.getFormula())
            obj.renderFormula()
            self.editView.viewport().update()
            self.editView.document().setModified(True)
            # self.editMathWidget.setFocus()                # TODO - does not work
