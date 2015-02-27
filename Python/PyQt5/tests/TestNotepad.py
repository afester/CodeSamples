'''
Created on Feb 19, 2015

@author: andreas
'''

import unittest
from Settings import Settings
from Notepad import Notepad


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
        
        notepad = Notepad(npDef)
        self.assertEqual('SampleWiki', notepad.getName())

        pageId = notepad.getNextPageId()
        print(str(type(pageId)) + "/" + str(pageId))



if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testImportExport']
    unittest.main()