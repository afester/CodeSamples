#!/usr/bin/python
 
import sys

from PySide.QtCore import qVersion 
from PySide.QtGui import QDialog, QFormLayout, QLabel, QApplication


class AboutDialog(QDialog):
    def __init__(self):
        QDialog.__init__(self)

        layout = QFormLayout(self)
        self.setLayout(layout)
        layout.addRow("Python version:", QLabel("%s.%s.%s (%s)" % 
                        (sys.version_info[0], 
                         sys.version_info[1], 
                         sys.version_info[2], 
                         sys.version_info[3])))

        layout.addRow("Qt version:", QLabel( qVersion()))

# Create a Qt application
app = QApplication(sys.argv)

# Create a simple "about" dialog
dlg = AboutDialog()
dlg.show()

# Run the application
app.exec_()
sys.exit()
