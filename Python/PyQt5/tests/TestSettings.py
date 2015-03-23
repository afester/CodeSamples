'''
Created on Feb 19, 2015

@author: andreas
'''

import unittest

from Settings import Settings


class Test(unittest.TestCase):


    def setUp(self):
        pass


    def tearDown(self):
        pass


    def testLoadSettings(self):
        settings = Settings()
        settings.load('testSettings.ini')

        notepads = settings.getNotepads()
        self.assertEqual(2, len(notepads))
        self.assertEqual('SampleWiki', notepads[0]['name'])


    def testAddNotepad(self):
        settings = Settings()
        settings.load('testSettings.ini')

        notepads = settings.getNotepads()
        self.assertEqual(2, len(notepads))

        settings.addNotepad({'type' : 'local', 'name' : 'Sample2', 'path' : 'C:/temp'})

        notepads = settings.getNotepads()
        self.assertEqual(3, len(notepads))
        self.assertEqual('Sample2', notepads[2]['name'])


if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testImportExport']
    unittest.main()