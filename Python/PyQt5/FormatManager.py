'''
Created on 20.02.2015

@author: afester
'''

from PyQt5.QtGui import QTextFormat, QTextBlockFormat, QTextCharFormat, QTextListFormat, QFont, QColor
import cssutils

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


    def loadFormats(self):
        self.formats = {}

        styleSheet = cssutils.parseFile('styles.css')

        blockFormats = { 'h1' : 'h1', 'h2' : 'h2', 'h3' : 'h3', 'p' : 'p', 
                         'code[lang="java"]' : 'javacode',
                         'code[lang="cpp"]' : 'cppcode',
                         'code[lang="xml"]' : 'xmlcode',
                         'code[lang="sql"]' : 'sqlcode',
                         'code[lang="python"]' : 'pycode',
                         'screen' : 'screen'}

        for cssKey in blockFormats.keys():
            tagKey = blockFormats[cssKey]
            cssRule = self.simpleLookup(styleSheet, cssKey)

            blockFmt = QTextBlockFormat()
            blockFmt.setProperty(QTextFormat.UserProperty, tagKey)

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

            value = self.getIntValue(cssRule, 'font-size')
            if value:
                charFmt.setFontPointSize(value)
            value = self.getColorValue(cssRule, 'color')
            if value:
                charFmt.setForeground(value)
            value = self.getStringValue(cssRule, 'font-family')
            if value:
                charFmt.setFontFamily(value)

            self.formats[tagKey] = Format(blockFmt, charFmt)

### List formats

        listFormats = {'ul li' : 'ul1', 'ul ul li' : 'ul2', 'ul ul ul li' : 'ul3', 
                       'ol li' : 'ol1', 'ol ol li' : 'ol2', 'ol ol ol li' : 'ol3'} 
        for cssKey in listFormats.keys():
            tagKey = listFormats[cssKey]
            cssRule = self.simpleLookup(styleSheet, cssKey)

            listFmt = QTextListFormat()
            listFmt.setProperty(QTextFormat.UserProperty, tagKey)

            value = self.getStringValue(cssRule, 'list-style-type')
            if value:
                if value == 'disc':
                    listFmt.setStyle(QTextListFormat.ListDisc)
                elif value == 'circle':
                    listFmt.setStyle(QTextListFormat.ListCircle)
                elif value == 'square':
                    listFmt.setStyle(QTextListFormat.ListSquare)
                elif value == 'decimal':
                    listFmt.setStyle(QTextListFormat.ListDecimal)

            # HACK!!! TODO!
            indent = cssKey.count(' ')
            listFmt.setIndent(indent)

            self.formats[tagKey] = Format(None, None, listFmt)

### Inline formats

        # Base format (?????)
        pcharFmt = QTextCharFormat()
        pcharFmt.setFontPointSize(10)
        pcharFmt.setFontFamily("Sans")

        inlineFormats = { 'em' : 'em', 'strong' : 'strong', 'tt' : 'tt', 
                          'keyword' : 'keyword', 'a' : 'a'}

        for cssKey in inlineFormats.keys():
            tagKey = inlineFormats[cssKey]
            cssRule = self.simpleLookup(styleSheet, cssKey)

            charFmt = QTextCharFormat(pcharFmt)
            charFmt.setProperty(QTextFormat.UserProperty, tagKey)

            # TODO: better approach?
            if tagKey in ['a', 'keyword']:
                charFmt.setAnchor(True)

            value = self.getStringValue(cssRule, 'font-family')
            if value:
                charFmt.setFontFamily(value)
            value = self.getStringValue(cssRule, 'font-weight')
            if value and value == 'bold':
                charFmt.setFontWeight(QFont.Bold)
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
            if value and value == 'underline':
                charFmt.setFontUnderline(True)

            self.formats[tagKey] = Format(None, charFmt)


    def getFormat(self, fmtId):
        result = self.formats.get(fmtId)
        if result is None:
            result = Format(QTextBlockFormat(), QTextCharFormat(), QTextListFormat())
        return result


    def dumpFormats(self):
        for fmtName, fmt in self.formats.items():
            print("{} {}\n".format(fmtName, fmt))
