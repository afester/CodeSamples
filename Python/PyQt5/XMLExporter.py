'''
Created on 19.02.2015

@author: afester
'''

from PyQt5.QtGui import QFont
from xml.sax.saxutils import escape


class XMLExporter:
    def __init__(self, fileHandle):
        self.fileHandle = fileHandle

    def exportDocument(self, document):
        self.fileHandle.write(self.getXmlString(document))


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
        userData = None
        if textBlock.userData():
            userData = textBlock.userData().getData()

        #print("USERDATA:" + str(userData))
        #print("USERPROPERTY:" + str(textBlock.blockFormat().property(QTextFormat.UserProperty)))

        blkFmt = textBlock.blockFormat()
        print("Block format: " + str(blkFmt.type()))
        print("User data: " + str(userData))

        if textBlock.textList() is not None:
            self.emitList(textBlock)
        else:
            if userData is None:
                self.emitParagraph(textBlock)
            elif userData == "h1":
                self.result = self.result + "\n   <h1>" + escape(textBlock.text()) + "</h1>\n"
            elif userData == "h2":
                self.result = self.result + "\n   <h2>" + escape(textBlock.text()) + "</h2>\n"
            elif userData == "h3":
                self.result = self.result + "\n   <h3>" + escape(textBlock.text()) + "</h3>\n"
            elif userData == "p":
                self.emitParagraph(textBlock)
                #self.result = self.result + "   <p>" + textBlock.text() + "</p>\n"
            elif userData == "code":
                frag = textBlock.text()
                frag = frag.replace('\u2028', '\n')
                self.result = self.result + "   <code lang=\"java\">" + escape(frag) + "</code>\n"


    def emitList(self, block):
        listx = block.textList()

        itemNumber = listx.itemNumber(block)
        if itemNumber == 0:
            self.result = self.result + "   <ul>\n";

        self.result = self.result + "     <li>" + escape(block.text()) + "</li>\n"

        if itemNumber == listx.count() - 1:
            self.result = self.result + "   </ul>\n";


    def emitParagraph(self, textBlock):

        self.result = self.result + "   <p>"

        fragments = textBlock.begin()
        while not fragments.atEnd():
            fragment = fragments.fragment()
            text = escape(fragment.text())
            charFormat = fragment.charFormat()
            if charFormat.fontWeight() == QFont.Bold:
                self.result = self.result + "<em>" + text + "</em>"
            else:
                self.result = self.result + text
            fragments += 1

        self.result = self.result + "</p>\n"
