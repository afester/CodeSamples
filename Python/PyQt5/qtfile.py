#!/usr/bin/env python3

from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
import sys

app = QApplication(sys.argv)
QFileDialog.getSaveFileName(None, "Save File", "/tmp/Result.txt", filter ="txt (*.txt *.)")

