'''
Created on 19.02.2015

@author: afester
'''

import tools

from PyQt5.QtGui import QFont, QTextFormat
from xml.sax.saxutils import escape
import os


class XMLExporter:
    def __init__(self, contentPath, contentFile):
    #def __init__(self, fileHandle):
        self.contentPath = contentPath
        self.contentFile = contentFile

        # self.fileHandle = fileHandle

    def exportDocument(self, document):
        contentFilePath = os.path.join(self.contentPath, self.contentFile)
        with open(contentFilePath, 'w') as content_file:
            content_file.write(self.getXmlString(document))


    def getXmlString(self, document):
        self.result = "<?xml version = '1.0' encoding = 'UTF-8'?>\n<page>\n"
        # self.result = "<html>\n"

        #document = self.editView.document()
        textBlock = document.firstBlock()
        while textBlock.isValid():
            self.emitTextBlock(textBlock)
            textBlock = textBlock.next()

        self.result = self.result + "</page>\n"
        return self.result


    def emitTextBlock(self, textBlock):
        blockStyle = str(textBlock.blockFormat().property(QTextFormat.UserProperty))

        if textBlock.textList() is not None:
            self.emitList(textBlock)
        else:
            if blockStyle is None:
                # self.emitBlock(textBlock)
                assert False
            elif blockStyle == "h1":
                self.result = self.result + "\n   <h1>" + escape(textBlock.text()) + "</h1>\n"
            elif blockStyle == "h2":
                self.result = self.result + "\n   <h2>" + escape(textBlock.text()) + "</h2>\n"
            elif blockStyle == "h3":
                self.result = self.result + "\n   <h3>" + escape(textBlock.text()) + "</h3>\n"
            elif blockStyle == "p":
                self.emitBlock(textBlock)
            elif blockStyle == "code":
                frag = textBlock.text()
                frag = frag.replace('\u2028', '\n')
                self.result = self.result + "   <code lang=\"java\">" + escape(frag) + "</code>\n"


    def emitList(self, block):
        listx = block.textList()

        itemNumber = listx.itemNumber(block)
        if itemNumber == 0:
            self.result = self.result + "   <ul>\n";

        self.result = self.result + "     <li>"
        self.emitFragments(block)
        self.result = self.result + "</li>\n"

        if itemNumber == listx.count() - 1:
            self.result = self.result + "   </ul>\n";


    def emitBlock(self, textBlock):

        self.result = self.result + "   <p>"
        self.emitFragments(textBlock)
        self.result = self.result + "</p>\n"


    def emitFragments(self, textBlock):
        fragments = textBlock.begin()
        while not fragments.atEnd():
            self.emitFragment(fragments.fragment())
            fragments += 1


    def emitFragment(self, fragment):
        text = fragment.text()
        charFormat = fragment.charFormat()
        
        styleClass = str(charFormat.property(QTextFormat.UserProperty))

        closeAnchor = False
        if styleClass == 'a':
            href = charFormat.anchorHref()
            # if (!href.isEmpty()) {
            self.result = self.result + '<a href="'
            self.result = self.result + escape(href)
            self.result = self.result + '">'
            closeAnchor = True

        isObject = (text.find('\ufffc') != -1)
        isImage = isObject and charFormat.isImageFormat()

        if isImage:
            imgFmt = charFormat.toImageFormat()
            imgName = tools.os_path_split(imgFmt.name())[-1]

            # If the same image repeats multiple times, it is simply represented by
            # yet another object replacement character ...
            for img in range(0, len(text)):
                self.result = self.result + "<img src=\"%s\" />" % imgName

        elif styleClass == 'em':
            self.result = self.result + "<em>" + escape(text) + "</em>"
        elif styleClass == 'keyword':
            self.result = self.result + "<keyword>" + escape(text) + "</keyword>"
        else:
            self.result = self.result + escape(text)
    
        if closeAnchor:
            self.result = self.result + '</a>'
