'''
Created on 20.02.2015

@author: afester
'''

from PyQt6.QtCore import QVariant
from PyQt6.QtGui import QTextFormat, QTextBlockFormat, QTextCharFormat, QTextListFormat, QFont, QColor
import cssutils
import re
import io, pkg_resources, data

class Format:
    
    def __init__(self, blockFormat, charFormat, listFormat = None):
        self.blockFormat = blockFormat
        self.charFormat = charFormat
        self.listFormat = listFormat
        self.isPre = False

    
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


    def simpleLookup(self, styleSheet, searchText):
        '''Return the CSSRule for a given selector'''

        rules = styleSheet.cssRules
        for idx in range(0, rules.length):
            rule = rules.item(idx)

            if rule.type == cssutils.css.CSSRule.STYLE_RULE:    # type(rule) = <class CSSStyleRule>
                # Iterate all (comma separated) selectors
                for sel in rule.selectorList:   # class cssutils.css.Selector
                    if sel.selectorText == searchText:
                        return rule
        return None


    def getIntValue(self, cssRule, propertyName):
        prop = cssRule.style.getProperty(propertyName)
        if prop is None:
            return None

        value = prop.propertyValue.item(0)
        if value.type == cssutils.css.Value.NUMBER:
            return value.value
        elif value.type == cssutils.css.Value.DIMENSION:
            return value.value

        return None


    def getStringValue(self, cssRule, propertyName):
        prop = cssRule.style.getProperty(propertyName)
        if prop is None:
            return None

        value = prop.propertyValue.item(0)
        if value.type in [cssutils.css.Value.STRING, cssutils.css.Value.IDENT]:
            return value.value

        return None


    def getColorValue(self, cssRule, propertyName):
        prop = cssRule.style.getProperty(propertyName)
        if prop is None:
            return None

        value = prop.propertyValue.item(0)
        if value.type != cssutils.css.Value.COLOR_VALUE:
            return None

        return QColor(value.red, value.green, value.blue)


    def setCharFormatAttributes(self, cssRule, charFmt):
        value = self.getStringValue(cssRule, 'font-family')
        if value:
            charFmt.setFontFamily(value)
        value = self.getStringValue(cssRule, 'font-weight')
        if value and value == 'bold':
            charFmt.setFontWeight(QFont.Weight.Bold)
        value = self.getIntValue(cssRule, 'font-size')
        if value:
            charFmt.setFontPointSize(value)
        value = self.getColorValue(cssRule, 'background-color')
        if value:
            charFmt.setBackground(value)
        value = self.getColorValue(cssRule, 'color')
        if value:
            charFmt.setForeground(value)

        value = self.getStringValue(cssRule, 'text-decoration')
        if value:
            if value == 'underline':
                charFmt.setFontUnderline(True)
            elif value == 'overline':
                charFmt.setFontOverline(True)

        value = self.getStringValue(cssRule, 'font-style')
        if value:
            if value == 'italic':
                charFmt.setFontItalic(True)


    def loadFormats(self):
        self.formats = {}

        stylesCSS = pkg_resources.resource_string(data.__name__, 'styles.css')
        print("styles.css file: {}".format(stylesCSS))
        styleSheet = cssutils.parseString(stylesCSS)

        blockFormats = ['title[level="1"]', 
                        'title[level="2"]', 
                        'title[level="3"]', 
                        'para',
                        'tip',
                        'warning',
                        'blockquote',
                        'programlisting[language="java"]',
                        'programlisting[language="cpp"]',
                        'programlisting[language="xml"]',
                        'programlisting[language="sql"]',
                        'programlisting[language="python"]',
                        'programlisting[language="bash"]',
                        'screen']

        for cssKey in blockFormats:
            cssRule = self.simpleLookup(styleSheet, cssKey)

            # get the selector as a tuple of class and one attribute selector
            # (Can be extended later, but currently sufficient)
            m = re.match(r'^(\S*?)(?:\[(.*)="(.*)"])?$', cssKey)
            selector = m.groups()

            blockFmt = QTextBlockFormat()
            blockFmt.setProperty(QTextFormat.Property.UserProperty, QVariant(selector))

            value = self.getIntValue(cssRule, 'margin-top')
            if value:
                blockFmt.setTopMargin(value)
            value = self.getIntValue(cssRule, 'margin-right')
            if value:
                blockFmt.setRightMargin(value)
            value = self.getIntValue(cssRule, 'margin-bottom')
            if value:
                blockFmt.setBottomMargin(value)
            value = self.getIntValue(cssRule, 'margin-left')
            if value:
                blockFmt.setLeftMargin(value)
            value = self.getColorValue(cssRule, 'background-color')
            if value:
                blockFmt.setBackground(value)

            charFmt = QTextCharFormat()
            self.setCharFormatAttributes(cssRule, charFmt)

            fmt = Format(blockFmt, charFmt)
            value = self.getStringValue(cssRule, 'white-space')
            if value and value == 'pre':
                fmt.isPre = True
            self.formats[selector] = fmt

### List formats

        listFormats = ['itemizedlist[level="1"]',
                       'itemizedlist[level="2"]',
                       'itemizedlist[level="3"]', 
                       'itemizedlist[level="4"]',
                       'orderedlist[level="1"]',
                       'orderedlist[level="2"]' ,
                       'orderedlist[level="3"]',
                       'orderedlist[level="4"]'] 
        for cssKey in listFormats:
            cssRule = self.simpleLookup(styleSheet, cssKey)

            indent = 0
            m = re.match(r'^(\S*?)(?:\[(.*)="(.*)"])?$', cssKey)
            selector = m.groups()
            if selector[1] == 'level':
                indent = int(selector[2])

            listFmt = QTextListFormat()
            listFmt.setProperty(QTextFormat.Property.UserProperty, QVariant(selector))
            listFmt.setIndent(indent)

            value = self.getStringValue(cssRule, 'list-style-type')
            if value:
                if value == 'disc':
                    listFmt.setStyle(QTextListFormat.Style.ListDisc)
                elif value == 'circle':
                    listFmt.setStyle(QTextListFormat.Style.ListCircle)
                elif value == 'square':
                    listFmt.setStyle(QTextListFormat.Style.ListSquare)
                elif value == 'decimal':
                    listFmt.setStyle(QTextListFormat.Style.ListDecimal)

            self.formats[selector] = Format(None, None, listFmt)

### Inline formats

        # Base format (?????)
        pcharFmt = QTextCharFormat()
        pcharFmt.setFontPointSize(10)
        pcharFmt.setFontFamily("Sans")

        inlineFormats = ['emphasis[role="highlight"]', 
                         'emphasis',
                         'code',
                         'link',
                         'olink']

        for cssKey in inlineFormats:
            cssRule = self.simpleLookup(styleSheet, cssKey)

            m = re.match(r'^(\S*?)(?:\[(.*)="(.*)"])?$', cssKey)
            selector = m.groups()

            charFmt = QTextCharFormat(pcharFmt)
            charFmt.setProperty(QTextFormat.Property.UserProperty, QVariant(selector))

            # TODO: better approach?
            if cssKey in ['link', 'olink']:
                charFmt.setAnchor(True)
            self.setCharFormatAttributes(cssRule, charFmt)

            self.formats[selector] = Format(None, charFmt)

### special formats
        charFmt = QTextCharFormat()
        cssRule = self.simpleLookup(styleSheet, 'searchMarker')
        self.setCharFormatAttributes(cssRule, charFmt)
        self.formats[('searchMarker', None, None)] = Format(None, charFmt)


    def getFormat(self, fmtId):
        result = self.formats.get(fmtId)
        if result is None:
            result = Format(QTextBlockFormat(), QTextCharFormat(), QTextListFormat())
        return result


    def dumpFormats(self):
        for fmtName, fmt in self.formats.items():
            print("{} {}\n".format(fmtName, fmt))
