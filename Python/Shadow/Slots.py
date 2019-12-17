#!/usr/bin/env python3

from PyQt5.QtWidgets import QApplication, QDialog, QPushButton, QLabel, QVBoxLayout
import sys

class MyDialog(QDialog):
    
    def __init__(self):
        QDialog.__init__(self)
        layout = QVBoxLayout()
        self.setLayout(layout)
        button = QPushButton("Execute", self)
        layout.addWidget(button)
        lbl1 = QLabel("Hello", self)
        layout.addWidget(lbl1)
        lbl2 = QLabel("World", self)
        layout.addWidget(lbl2)

        button.clicked.connect(lbl1.clear)
        button.clicked.connect(lbl2.clear)

app = QApplication(sys.argv)
dlg = MyDialog()
dlg.show()
app.exec()