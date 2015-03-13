'''
Created on 19.02.2015

@author: afester
'''

import tools

from PyQt5.QtGui import QTextFormat
from xml.sax.saxutils import escape
import os
from TextDocumentTraversal import TextXMLPrinter


class XMLExporter:
    def __init__(self, contentPath, contentFile):
        self.contentPath = contentPath
        self.contentFile = contentFile


    def exportDocument(self, document):
        contentFilePath = os.path.join(self.contentPath, self.contentFile)
        with open(contentFilePath, 'w') as content_file:
            content_file.write(self.getXmlString(document))


    def getXmlString(self, document):
        self.contents = ''
        traversal = TextXMLPrinter(self.collect)
        traversal.traverse(document)
        return self.contents


    def collect(self, data):
        self.contents += data

#===============================================================================
#     def collectLines(self, data):
#         self.result += data
# 
#         
#         self.result = "<?xml version = '1.0' encoding = 'UTF-8'?>\n<page>\n"
# 
#         textBlock = document.firstBlock()
#         while textBlock.isValid():
#             self.emitTextBlock(textBlock)
#             textBlock = textBlock.next()
# 
#         self.result = self.result + "</page>\n"
#         return self.result
# 
# 
#     def emitTextBlock(self, textBlock):
#         blockStyle = textBlock.blockFormat().property(QTextFormat.UserProperty)
# 
#         if textBlock.textList() is not None:
#             self.emitList(textBlock)
#         elif blockStyle is None:
#                 assert False
#         else:
#             self.emitBlock(textBlock)   # h1, h2, ..., javacode, ..., p, ...
# 
# 
#     def emitList(self, block):
#         listx = block.textList()
# 
#         itemNumber = listx.itemNumber(block)
#         if itemNumber == 0:
#             self.result = self.result + "   <ul>\n";
# 
#         self.result = self.result + "     <li>"
#         self.emitFragments(block)
#         self.result = self.result + "</li>\n"
# 
#         if itemNumber == listx.count() - 1:
#             self.result = self.result + "   </ul>\n";
# 
# 
#     def emitBlock(self, textBlock):
#         blockStyle = textBlock.blockFormat().property(QTextFormat.UserProperty)
# 
#         if blockStyle in ['h1', 'h2', 'h3']:
#             self.result = self.result + '\n'
# 
#         self.result = self.result + "   <{}>".format(blockStyle)
# 
#         self.emitFragments(textBlock)
# 
#         self.result = self.result + "</{}>\n".format(blockStyle)
# 
# 
#     def emitFragments(self, textBlock):
#         fragments = textBlock.begin()
#         while not fragments.atEnd():
#             self.emitFragment(fragments.fragment())
#             fragments += 1
# 
# 
#     def emitFragment(self, fragment):
#         text = fragment.text()
#         text = text.replace('\u2028', '\n')
# 
#         charFormat = fragment.charFormat()
#         styleClass = charFormat.property(QTextFormat.UserProperty)
# 
#         closeAnchor = False
#         if styleClass == 'a':
#             href = charFormat.anchorHref()
#             self.result = self.result + '<a href="'
#             self.result = self.result + escape(href)
#             self.result = self.result + '">'
#             closeAnchor = True
# 
#         isObject = (text.find('\ufffc') != -1)
#         isImage = isObject and charFormat.isImageFormat()
# 
#         if isImage:
#             imgFmt = charFormat.toImageFormat()
#             imgName = tools.os_path_split(imgFmt.name())[-1]
# 
#             # If the same image repeats multiple times, it is simply represented by
#             # yet another object replacement character ...
#             for img in range(0, len(text)):
#                 self.result = self.result + '<img src="{}" />'.format(imgName)
# 
#         else:
#             if styleClass is not None and not closeAnchor:
#                 self.result = self.result + '<{}>'.format(styleClass)
# 
#             self.result = self.result + escape(text)
# 
#             if styleClass is not None and not closeAnchor:
#                 self.result = self.result + '</{}>'.format(styleClass)
# 
#         if closeAnchor:
#             self.result = self.result + '</a>'
#===============================================================================
