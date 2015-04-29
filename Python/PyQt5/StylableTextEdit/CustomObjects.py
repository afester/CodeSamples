'''
Created on 29.04.2015

@author: afester
'''

from PyQt5.Qt import QObject, QTextObjectInterface, QSize
from PyQt5.QtCore import Qt, QSizeF 
from PyQt5.QtGui import QTextFormat, QImage, QPen
 
class CustomObjectRenderer(QObject, QTextObjectInterface):
    
    def __init__(self):
        QObject.__init__(self)
        QTextObjectInterface.__init__(self)


    # @Override
    def intrinsicSize(self, doc, posInDocument, fmt):
        customObject = fmt.property(QTextFormat.UserProperty+1)
        return customObject.getSize()


    # @Override
    def drawObject(self, painter, rect, doc, posInDocument, fmt):
        customObject = fmt.property(QTextFormat.UserProperty+1)

        # rect is the rectangle to redraw, in document contents coordinates

        painter.save()
        # painter.fillRect(rect, Qt.red)
        painter.translate(rect.topLeft())
        customObject.doDraw(painter, rect)
        painter.restore()

        # Draw the bounding box of the redraw rectangle (DEBUG)
        #painter.setPen(Qt.DashLine)
        #painter.drawRect(rect)

class CustomTextObject:

    def __init__(self):
        self.isSelected = False

    def setSelected(self, flag):
        self.isSelected = flag


class ImageObject(CustomTextObject):

    def __init__(self):
        CustomTextObject.__init__(self)
        self.imagName= None
        self.bufferedImage = QImage('icons/emptyimage.png')

    def getSize(self):
        result = self.bufferedImage.size()
        return QSizeF(result)

    # Paint the object, in the object's coordinate system
    def doDraw(self, painter, rect):
        painter.drawImage(0, 0, self.bufferedImage)

        if self.isSelected:
            painter.setPen(QPen(Qt.red, 1.0, Qt.DashLine))
            painter.drawRect(0, 0, rect.width()-1, rect.height()-1)
            painter.fillRect(0, 0, 7, 7, Qt.red)
            painter.fillRect(rect.width() - 7, 0, 7, 7, Qt.red)
            painter.fillRect(0, rect.height() - 7, 7, 7, Qt.red)
            painter.fillRect(rect.width() - 7, rect.height() - 7, 7, 7, Qt.red)

    def setName(self, imageName):
        '''Sets the file name of the image. Either an absolute path name or 
           a path relative to the current working directory.'''

        self.imageName = imageName
        img = QImage(imageName)
        if not img.isNull():
            self.bufferedImage = img




class MathFormulaObject(CustomTextObject):
    '''A container for a mathematical formula and its rendered .png equivalent'''

    def __init__(self):
        CustomTextObject.__init__(self)

        self.image = None
        self.formula = ''

    def getSize(self):
        result = self.image.size()
        result += QSize(4, 4)
        return QSizeF(result)

    # Paint the object, in the object's coordinate system
    def doDraw(self, painter, rect):
        painter.drawImage(2, 0, self.image)

        if self.isSelected:
            painter.setPen(QPen(Qt.lightGray, 1.0, Qt.DashLine))
            painter.drawRect(1, 0, rect.width() - 2, rect.height() - 2)

    def setFormula(self, formula):
        self.formula= formula

    def renderFormula(self):
        # Render the formula into a png image
        import matplotlib.mathtext as mathtext
        parser = mathtext.MathTextParser("Bitmap")
        
        # Note: the baseline returned by to_png() is not completely accurate!
        # baseline =
        parser.to_png('math.png', r'${}$'.format(self.formula), color='black', fontsize=12, dpi=100)
        self.image = QImage('math.png')
        # percent = baseline / self.image.height()
        # print("Image: w={}, h={}, baseline={} ({}%)".format(self.image.width(), self.image.height(), baseline, percent))

    def __str__(self):
        return self.formula
