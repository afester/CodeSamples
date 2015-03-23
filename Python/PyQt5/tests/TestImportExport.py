'''
Created on Feb 19, 2015

@author: andreas
'''

import unittest

from XMLImporter import XMLImporter
from XMLExporter import XMLExporter 


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testImportExport(self):
        self.maxDiff = None

        # Import an XML file into a QTextDocument
        importer = XMLImporter('../SampleWiki', 'content.xml')
        doc = importer.importDocument()
        
        # export the document into XML
        exp = XMLExporter(None)
        actual = exp.getXmlString(doc)

        # Compare the exported XML document with the xml file from the disc
        with open("../SampleWiki/content.xml", 'r') as content_file:
            expected = content_file.read()
            self.assertEqual(actual, expected)


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testImportExport']
    unittest.main()