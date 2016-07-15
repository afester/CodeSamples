#!/usr/bin/python

import sys
from PySide import QtGui, QtCore

X_START = 10
Y_START = 15
MAX_WIDTH = 1350
MAX_LENGH = 1650
ZOOM = 2.5
WIDTH_PEZZO = 150
LENGH_PEZZO = 600
CENTER_OFFSET_X = 15
CENTER_OFFSET_Y = 0
n
class PaintWidget(QtGui.QWidget):
    def __init__(self, parent=None):
        super(PaintWidget, self).__init__(parent)

    def paintEvent(self, e):
        qp = QtGui.QPainter(self)
        print("paint event")
        self.drawRectangles(qp)

    def drawRectangles(self, qp):

        color = QtGui.QColor(0, 0, 0)
        color.setNamedColor('#d4d4d4')
        qp.setPen(color)

        qp.setBrush(QtGui.QColor(200, 0, 0))
        coordCarro = QtCore.QRectF(X_START, Y_START, MAX_WIDTH/ZOOM, MAX_LENGH/ZOOM)
        qp.drawRect(coordCarro)

class Strato(QtGui.QMainWindow):

    def __init__(self, parent=None):
        super(Strato, self).__init__(parent)
        self.centralwidget = PaintWidget(self)
        self.setCentralWidget(self.centralwidget)

if __name__ == "__main__":
    app = QtGui.QApplication(sys.argv)
    window = Strato()
    window.show()
    sys.exit(app.exec_())
