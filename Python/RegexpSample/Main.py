'''
Created on 28.04.2015

@author: afester
'''

import re, sys
from PyQt5 import uic
from PyQt5.Qt import Qt, QApplication, QColor, QPointF
from PyQt5.QtGui import QPainter, QPixmap
from PyQt5.QtWidgets import QWidget, QMainWindow

PAGESIZE = 4096

class Canvas(QWidget):

    def __init__(self, parent, width, height):
        QWidget.__init__(self, parent)
        self.pixmap = QPixmap(width, height)

        self.pixmap.fill(Qt.white)

        self.thePainter = QPainter(self.pixmap)
        self.thePainter.setRenderHints(QPainter.Antialiasing, True)
        self.thePainter.setPen(Qt.black)

    # @Override
    def paintEvent(self, event):
        p = QPainter(self)
        p.drawPixmap(0, 0, self.pixmap)

    def drawLine(self, x1, y1, x2, y2):
        col = QColor()
        r=0
        g=0
        b=0
        for y in range(0, 1024):
            for x in range(0, 1024):
                r += 1
                if r > 255:
                    r = 0
                    g += 1
                    if g > 255:
                        g= 0
                        b += 1
                col.setRgb(r, g, b)

                self.drawPoint(x, y, col)
        #self.thePainter.drawLine(x1, y1, x2, y2)
        self.update()

    def drawPoint(self, x, y, col):
        self.thePainter.setPen(col)
        self.thePainter.drawPoint(x, y)

    def drawEllipse(self, x, y, w, h):
        self.thePainter.drawEllipse(x, y, w, h)
        self.update()

class MainWindow(QMainWindow):

    def __init__(self, parent):
        QMainWindow.__init__(self, parent)

        self.ui = uic.loadUi('Sample.ui', self)

        # Add a simple drawing canvas to the main window
        self.canvas = Canvas(self.ui.centralwidget, 1024, 1024);
        self.ui.verticalLayout.addWidget(self.canvas);

        self.ui.actionLine.triggered.connect(self.doActionLine)
        self.ui.actionCircle.triggered.connect(self.doActionCircle)

    def doActionLine(self):
        self.canvas.drawLine(10, 10, 100, 100)

    def doActionCircle(self):
        self.canvas.drawEllipse(100, 100, 50, 50)


class MemoryMapping:
    
    def __init__(self):
        self.startAddr = 0
        self.endAddr = 0
        self.permissions = '----'
        self.fileOffset = 0
        self.deviceId = ''
        self.fileInode = 0
        self.fileName = ''

    def getSize(self):
        return self.endAddr - self.startAddr



class MemoryMappings:
    
    def __init__(self):
        self.mappings = {}


    def addMapping(self, mapping):
        key = mapping.fileName
        if key in self.mappings:
            mapList = self.mappings[key]
            mapList.append(mapping)
        else:
            mapList = [mapping]
        self.mappings[key] = mapList


    def dump(self):
        orderedKeys = sorted(self.mappings.keys())
        procSum = 0
        for key in orderedKeys:
            if key == '':
                print('Anonymous')
            else:
                print(key)

            memSum = 0
            mapping = self.mappings[key]    
            # mapping is a list of MemoryMapping objects. It should already be 
            # ordered since its elements have been added in the order read from /proc
            for m in mapping:
                print("    {:x}-{:x} ({} bytes / {} KiB / {} pages)".format(m.startAddr, m.endAddr, m.getSize(), int(m.getSize() / 1024), int(m.getSize() / PAGESIZE)))
                memSum += m.getSize()
            print('Overall: {} bytes / {} KiB / {} pages\n'.format(memSum, int(memSum / 1024), int(memSum / PAGESIZE)))
            procSum += memSum

        print('\nProcess summary: {} bytes / {} KiB / {} MiB / {} pages\n'.format(procSum, int(procSum / 1024), int(procSum / 1024 / 1024), int(procSum / PAGESIZE)))

if __name__ == '__main__':
    app = QApplication(sys.argv )

    win = MainWindow(None)
    win.show();
    #app.lastWindowClosed.connect(quit)
    #return 
    app.exec();
   
#===============================================================================
#     mappings = MemoryMappings()
#     with open('input.txt', 'r') as f:
#         for line in f:
#             line = line.strip()
#             m = re.match(r'([0-9a-f]*)-([0-9a-f]*)\s*(\S*)\s*([0-9a-f]*)\s*(\S*)\s*(\d*)\s*(.*)$', line)
#             if m == None or len(m.groups()) != 7:
#                 raise RuntimeError('Could not parse: {}'.format(line))
# 
#             groups = m.groups()
#             result = MemoryMapping()
#             result.startAddr = int(groups[0], 16)
#             result.endAddr = int(groups[1], 16)
#             result.permissions = groups[2]
#             result.fileOffset = groups[3]
#             result.deviceId = groups[4]
#             result.fileInode = groups[5]
#             result.fileName = groups[6]
# 
#             mappings.addMapping(result)
# 
#     mappings.dump()
#===============================================================================
