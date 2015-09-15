#!/usr/bin/env python3

from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QHBoxLayout, QSlider, QLCDNumber
from PyQt5.QtGui import QPainter
from PyQt5.QtCore import Qt, QRectF
from PyQt5.QtSvg import QSvgRenderer

import sys
from PyQt5.Qt import QVBoxLayout

class ControlWidget(QWidget):
    def __init__(self, parent):
        QWidget.__init__(self, parent)
        
        self.directionSlider = QSlider(Qt.Horizontal, self)
        self.directionSlider.setMinimum(0)
        self.directionSlider.setMaximum(360)

        self.direction = QLCDNumber(self)
        self.directionSlider.valueChanged.connect(self.direction.display)
        self.layout = QVBoxLayout()
        self.setLayout(self.layout)
        self.layout.addWidget(self.directionSlider)
        self.layout.addWidget(self.direction)

class CompassWidget(QWidget):
    def __init__(self, parent):
        QWidget.__init__(self, parent)
        self.compass = QSvgRenderer("compassBody.svg")
        self.needle = QSvgRenderer("compassNeedle.svg")
        self.angle = 0

    def paintEvent(self, event):
        painter = QPainter(self)
        margin = 10
        length = min(self.width(), self.height()) - 2*margin
        xpos = (self.width() - length)/2
        ypos = (self.height() - length)/2
        bounds = QRectF( xpos, ypos, length, length)
        #painter.setBrush(Qt.NoBrush)
        #painter.drawRect(bounds)
        self.compass.render(painter, bounds)

        painter.translate(self.width() / 2, self.height() / 2)
        painter.rotate(self.angle)
        painter.translate(-self.width() / 2, -self.height() / 2)
        #painter.translate(xrad/2, yrad/2)
        self.needle.render(painter, bounds)

    def setDirection(self, angle):
        self.angle = angle
        self.repaint()


class MainWindow(QMainWindow):

    def __init__(self, app):
        QMainWindow.__init__(self, None)
        self.resize(800, 600)

        self.centralWidget = QWidget()
        self.left = ControlWidget(self.centralWidget)
        self.right = CompassWidget(self.centralWidget)

        self.left.directionSlider.valueChanged.connect(self.right.setDirection)

        layout = QHBoxLayout()
        self.centralWidget.setLayout(layout)
        layout.addWidget(self.left)
        layout.addWidget(self.right)
        layout.setStretch(0, 1);
        layout.setStretch(1, 1);

        self.setCentralWidget(self.centralWidget)

def main():
    # Create the application object
    app = QApplication(sys.argv)

    # Create the main window
    mainWindow = MainWindow(app)

    # Show and run the application
    mainWindow.show()
    app.exec()

if __name__ == '__main__':
    main()
