'''
Created on Feb 19, 2015

@author: andreas
'''

import unittest

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



if __name__ == "__main__":
    #import sys;sys.argv = ['', 'Test.testImportExport']
    unittest.main()