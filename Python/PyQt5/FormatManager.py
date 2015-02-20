'''
Created on 20.02.2015

@author: afester
'''

from PyQt5.QtGui import QTextBlockFormat, QTextCharFormat, QTextListFormat, QFont, QColor
from PyQt5.QtCore import Qt

class Format:
    
    def __init__(self, blockFormat, charFormat, listFormat = None):
        self.blockFormat = blockFormat
        self.charFormat = charFormat
        self.listFormat = listFormat

    
    def getBlockFormat(self):
        return self.blockFormat

    
    def getCharFormat(self):
        return self.charFormat

    
    def getListFormat(self):
        return self.listFormat


    def __repr__(self):
        return "Format({})".format(self.__str__())


    def getListStyleName(self, styleId):
        if styleId == QTextListFormat.ListDisc:
            return "disc"
        elif styleId == QTextListFormat.ListCircle:
            return "circle"
        elif styleId == QTextListFormat.ListSquare:
            return "square"
        elif styleId == QTextListFormat.ListDecimal:
            return "decimal"
        else:
            return "unknown"


    def __str__(self):
        result = "{\n"
        
        blockFmt = self.blockFormat
        if blockFmt:
            result = result + '''  block-background-color: {};
  margin: {:.0f}px, {:.0f}px, {:.0f}px, {:.0f}px;
'''.format(blockFmt.background().color().name(), 
           blockFmt.topMargin(),blockFmt.rightMargin(),blockFmt.bottomMargin(),blockFmt.leftMargin())

        charFmt = self.charFormat
        if charFmt:
            result = result + '''  color: {};
  background: {};
  font: {:.0f}pt {} "{}";
'''.format(charFmt.foreground().color().name(),
               charFmt.background().color().name(),
               charFmt.fontPointSize(),
               "bold" if charFmt.fontWeight() == QFont.Bold else "normal",
               charFmt.fontFamily())

        listFmt = self.listFormat
        if listFmt:
            result = result + '''  list-style: {};
  list-level: {};
'''.format(self.getListStyleName(listFmt.style()),
           listFmt.indent())

        return result + '}'
    

class FormatManager:

    def __init__(self):
        pass

    def loadFormats(self):
        self.formats = {}

        blockFmt = QTextBlockFormat()
        blockFmt.setTopMargin(18)
        blockFmt.setBottomMargin(5)
        #blockFmt.setBackground(Qt.red)
        charFmt = QTextCharFormat()
        charFmt.setFontPointSize(18)
        #charFmt.setForeground(Qt.yellow)
        #charFmt.setBackground(Qt.blue)
        charFmt.setFontFamily("Arial")
        self.formats['h1'] = Format(blockFmt, charFmt)

        blockFmt = QTextBlockFormat()
        blockFmt.setTopMargin(14)
        blockFmt.setBottomMargin(3)
        charFmt = QTextCharFormat()
        charFmt.setFontPointSize(16)
        charFmt.setFontFamily("Arial")
        self.formats['h2'] = Format(blockFmt, charFmt)

        blockFmt = QTextBlockFormat()
        blockFmt.setTopMargin(10)
        blockFmt.setBottomMargin(3)
        charFmt = QTextCharFormat()
        charFmt.setFontPointSize(14)
        charFmt.setFontFamily("Arial")
        self.formats['h3'] = Format(blockFmt, charFmt)

        blockFmt = QTextBlockFormat()
        pcharFmt = QTextCharFormat()
        pcharFmt.setFontPointSize(10)
        pcharFmt.setFontFamily("Sans")
        self.formats['p'] = Format(blockFmt, pcharFmt)

        blockFmt = QTextBlockFormat()
        blockFmt.setTopMargin(5)
        blockFmt.setLeftMargin(5)
        blockFmt.setRightMargin(5)
        blockFmt.setBottomMargin(5)
        blockFmt.setBackground(QColor(240, 240, 240))
        charFmt = QTextCharFormat()
        charFmt.setForeground(Qt.black)
        charFmt.setFontFamily("Courier")
        charFmt.setFontPointSize(10)
        self.formats['code'] = Format(blockFmt, charFmt)

        ### List formats
        listFmt = QTextListFormat()
        listFmt.setStyle(QTextListFormat.ListDisc)
        listFmt.setIndent(1)
        self.formats['ul1'] = Format(None, None, listFmt)

        listFmt = QTextListFormat()
        listFmt.setStyle(QTextListFormat.ListCircle)
        listFmt.setIndent(2)
        self.formats['ul2'] = Format(None, None, listFmt)

        listFmt = QTextListFormat()
        listFmt.setStyle(QTextListFormat.ListSquare)
        listFmt.setIndent(3)
        self.formats['ul3'] = Format(None, None, listFmt)

        listFmt = QTextListFormat()
        listFmt.setStyle(QTextListFormat.ListDecimal)
        listFmt.setIndent(1)
        self.formats['ol1'] = Format(None, None, listFmt)

        listFmt = QTextListFormat()
        listFmt.setStyle(QTextListFormat.ListDecimal)
        listFmt.setIndent(2)
        self.formats['ol2'] = Format(None, None, listFmt)

        listFmt = QTextListFormat()
        listFmt.setStyle(QTextListFormat.ListDecimal)
        listFmt.setIndent(3)
        self.formats['ol3'] = Format(None, None, listFmt)

        
        ### Inline formats
        charFmt = QTextCharFormat(pcharFmt)
        charFmt.setFontWeight(QFont.Bold)
        self.formats['em'] = Format(None, charFmt)

        charFmt = QTextCharFormat(pcharFmt)
        charFmt.setAnchor(True)
        charFmt.setForeground(Qt.blue)
        charFmt.setBackground(QColor(220, 220, 220))
        charFmt.setFontUnderline(True)
        self.formats['keyword'] = Format(None, charFmt)

        charFmt = QTextCharFormat(pcharFmt)
        charFmt.setAnchor(True)
        charFmt.setForeground(Qt.blue)
        # charFmt.setBackground(Qt.blue)
        charFmt.setFontUnderline(True)
        self.formats['a'] = Format(None, charFmt)


    def getFormat(self, fmtId):
        result = self.formats.get(fmtId)
        if result is None:
            result = Format(QTextBlockFormat(), QTextCharFormat(), QTextListFormat())
        return result


    def dumpFormats(self):
        for fmtName, fmt in self.formats.items():
            print("{} {}\n".format(fmtName, fmt))
