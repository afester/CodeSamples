'''
Created on 19.02.2015

@author: afester
'''

import os
from TextDocumentTraversal import TextDocumentTraversal, DocbookPrinter

class XMLExporter:
    def __init__(self, contentPath, contentFile):
        self.contentPath = contentPath
        self.contentFile = contentFile


    def exportDocument(self, document):
        contentFilePath = os.path.join(self.contentPath, self.contentFile)
        with open(contentFilePath, 'w', encoding='utf-8') as content_file:
            content_file.write(self.getXmlString(document))


    def getXmlString(self, document):
        self.contents = ''

        traversal = TextDocumentTraversal()
        tree = traversal.traverse(document)

        dp = DocbookPrinter(tree, self.collect)
        dp.traverse()

        #traversal = TextXMLPrinter(self.collect)
        #traversal.traverse(document)

        return self.contents


    def collect(self, data):
        if data is not None:
            self.contents += data
