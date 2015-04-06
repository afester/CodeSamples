'''
Created on 19.02.2015

@author: afester
'''

import os
from TextDocumentTraversal import TextXMLPrinter

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


#===============================================================================
#         from TextDocumentTraversal2 import TextDocumentTraversal2
#         traversal = TextDocumentTraversal2()
#         tree = traversal.traverse(document)
# 
#         from TextDocumentTraversal2 import DocbookPrinter
#         dp = DocbookPrinter(tree, self.collect)
#         dp.traverse()
#===============================================================================

        traversal = TextXMLPrinter(self.collect)
        traversal.traverse(document)

        return self.contents


    def collect(self, data):
        self.contents += data
