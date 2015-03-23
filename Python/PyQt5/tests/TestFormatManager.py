'''
Created on Feb 19, 2015

@author: andreas
'''

import unittest
import cssutils

from FormatManager import FormatManager


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testFormatManager(self):
        mgr = FormatManager()
        mgr.loadFormats()

        noFormat = mgr.getFormat('xyz')
        print(noFormat)

        h1Format = mgr.getFormat('h1')
        print(h1Format)

        # mgr.dumpFormats()

    def simpleLookup(self, styleSheet, searchText):
        rules = styleSheet.cssRules
        for idx in range(0, rules.length):
            rule = rules.item(idx)

            if rule.type == cssutils.css.CSSRule.STYLE_RULE:    # type(rule) = <class CSSStyleRule>
                # Iterate all (comma separated) selectors - we usually only have one
                for sel in rule.selectorList:   # class cssutils.css.Selector
                    if sel.selectorText == searchText:
                        return rule
        return None


    def testLoadStyles(self):
        styleSheet = cssutils.parseFile('../styles.css')
        rules = styleSheet.cssRules
        self.assertEqual(25, rules.length)

        rule = self.simpleLookup(styleSheet, "h2")
        self.assertIsNotNone(rule)

        rule = self.simpleLookup(styleSheet, "h5")
        self.assertIsNone(rule)

        rule = self.simpleLookup(styleSheet, 'code[lang="java"]')
        self.assertIsNotNone(rule)
        prop = rule.style.getProperty('background-color')
        self.assertEqual(prop.name, 'background-color')
        self.assertEqual(prop.propertyValue.length, 1)  # expect one property value
        value = prop.propertyValue.item(0)
        self.assertEquals(value.type, cssutils.css.Value.COLOR_VALUE)
        colorValue = 'rgb({},{},{})'.format(value.red, value.green, value.blue)
        self.assertEquals(colorValue, 'rgb(0,0,255)')

        #=======================================================================
        # rule = self.simpleLookup(styleSheet, 'code[lang="xml"]')
        # prop = rule.style.getProperty('background-color')
        # value = prop.propertyValue.item(0)
        # colorValue = 'rgb({},{},{})'.format(value.red, value.green, value.blue)
        #=======================================================================

        # test unknown property
        prop = rule.style.getProperty('foreground-color')
        self.assertIsNone(prop)

#===============================================================================
# 
#         for idx in range(0, rules.length):
#             rule = rules.item(idx)
#             
#             if rule.type == cssutils.css.CSSRule.STYLE_RULE:    # type(rule) = <class CSSStyleRule>
#                 # Selector
#                 
#                 selector = rule.selectorText
# 
#                 print("\nRule: {}".format(selector))
# 
#                 # Iterate all (comma separated) selectors - we usually only have one
#                 for sel in rule.selectorList:   # class cssutils.css.Selector
#                     print("  SEL: {}".format(sel.selectorText))
#                     print("  ELEM: {}".format(type(sel)))
#                     pass
# 
# 
#                 # Style declaration
#                 styleDecl = rule.style      # class CSSStyleDeclaration
# 
#                 for name in styleDecl.keys():
#                     prop = styleDecl.getProperty(name)  # <class 'cssutils.css.property.Property'>
# 
#                     propName = prop.name
#                     propValue = prop.propertyValue      # <class 'cssutils.css.value.PropertyValue'>
# 
#                     print("     {}:".format(propName))
#                     for valIdx in range(0, propValue.length):
#                         value = propValue.item(valIdx)  # e.g. <class 'cssutils.css.value.DimensionValue'>
# 
#                         if value.type == cssutils.css.Value.DIMENSION:      # dimension value
#                             theValue = value.value
#                             theDimension = value.dimension
#                             print('       {} "{}"'.format(theValue, theDimension))
# 
#                         elif value.type == cssutils.css.Value.COLOR_VALUE:  # color value
#                             theValue = 'rgb({},{},{})'.format(value.red, value.green, value.blue)
#                             print('       {}'.format(theValue))
# 
#                         elif value.type == cssutils.css.Value.IDENT:    # e.g. bold
#                             print('       {}'.format(value.value))
# 
#                         elif value.type == cssutils.css.Value.STRING:   # e.g. "Courier"
#                             print('       {}'.format(value.value))
# 
#                         else:
#                             print("       {} ({})".format(value.type, type(value.value)))
# 
#             elif rule.type == cssutils.css.CSSRule.COMMENT:
#                 pass
#             else:
#                 print('Unknown rule type: {} - {}'.format(rule.type, rule.typeString))
#===============================================================================


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testImportExport']
    unittest.main()
