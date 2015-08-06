'''
Created on Feb 19, 2015

@author: andreas
'''

import unittest
from Settings import Settings
from Notepad import LocalNotepad
import urllib.parse


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testPageSequence(self):
        settings = Settings()
        settings.load('testSettings.ini')
        npDef = settings.getNotepads()[0]
        self.assertEqual('SampleWiki', npDef['name'])
        
        notepad = LocalNotepad(npDef)
        self.assertEqual('SampleWiki', notepad.getName())

        pageId = notepad.getNextPageId()


    def testEncode(self):
        sample = "Sample"
        result = urllib.parse.quote_plus(sample)
        self.assertEqual('Sample', result)

        sample = "TODO list"
        result = urllib.parse.quote_plus(sample)
        self.assertEqual('TODO+list', result)

        sample = "TODO $#@=?%^Q^$list"
        result = urllib.parse.quote(sample)
        self.assertEqual('TODO%20%24%23%40%3D%3F%25%5EQ%5E%24list', result)

        sample = "Unix/Linux\Test"
        result = urllib.parse.quote(sample, '')
        self.assertEqual('Unix%2FLinux%5CTest', result)

        back = urllib.parse.unquote(result)
        self.assertEqual('Unix/Linux\Test', back)


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testImportExport']
    unittest.main()