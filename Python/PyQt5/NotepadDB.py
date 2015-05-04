'''
Created on 04.05.2015

@author: afester
'''

import sqlite3, os, urllib, fnmatch
from Notepad import LocalPage, LocalNotepad

class NotepadDB:
    
    def __init__(self, rootDir):
        self.rootDir = rootDir
        self.dbFile = os.path.join(rootDir, 'mynpad.dbf')


    def openDatabase(self):
        # Create the database file if it does not exist yet and open the database 
        self.conn = sqlite3.connect(self.dbFile)
        self.sqliteBug()
        return

        self.createDatabase()

        #=======================================================================
        npDef = {'name' : 'MynPad',     # TODO
                 'type'  : 'local',
                 'path'   : self.rootDir }
        notepad = LocalNotepad(npDef)
        self.refreshDatabase(notepad)
        #=======================================================================

        self.dumpDatabase()


    def closeDatabase(self):
        self.conn.close()

    def createDatabase(self):
        result = self.conn.execute('''
SELECT * 
FROM sqlite_master 
WHERE type='table' and name='{}'
'''.format('pageref'))

        if result.fetchone() is None:
            print('Creating database schema ...')
            self.conn.execute('''
CREATE TABLE pageref  (
    parentId TEXT, 
    childId TEXT 
)''')



    def refreshDatabase(self, notepad):
        
        print("REFRESHING DATABASE")

        self.conn.execute('''
DELETE FROM {}'''.format('pageref'))

        # TODO: We need a Notepad instance here.
        # Then the following code to search through a notepad can be moved into the Notepad class:

        for root, dirnames, filenames in os.walk(self.rootDir):
            for filename in fnmatch.filter(filenames, '*.xml'):
                # TODO: Proper handling of notepad's root page!!!!
                if root == self.rootDir and filename == 'Title%20page.xml':
                    pageId = None
                else:
                    pageId = urllib.parse.unquote(filename)[:-4]

                page = LocalPage(notepad, pageId)
                page.load()

                print('\n"{}"'.format(pageId))
                for childLink in page.getLinks():
                    # print('   => "{}"'.format(childLink))
                    stmt = '''
INSERT INTO {} VALUES('{}', '{}')'''.format('pageref', pageId, childLink)
                    print("  SQL: {}".format(stmt))

                    self.conn.execute(stmt)
                print()


    def getChildPages(self, pageId):
        result = []

        resCursor = self.conn.execute('''
SELECT childId 
FROM {} 
WHERE parentId = '{}' '''.format('pageref', pageId))
        for row in resCursor:
            result.append(row[0])

        return result



    def printData(self, tableName, selection, rowset):

        desc = self.conn.execute('''PRAGMA table_info({})'''.format(tableName))
        for row in rowset:
            print("==>" + str(row))         

        # Calculate a column description with name, type and width for each column.
        idx = 0
        result = {}
        for header in desc:
            colName = header[1]
            if selection == '*' or colName in selection:    # TODO: might be ambiguous
                colType = header[2]
                if colType == 'INTEGER':
                    colWidth = 10
                else:
                    colWidth = 25
                result[idx] = (colName, colType, colWidth)
                idx += 1

        # print the column headers
        # parentId                 |childId                  |
        for idx in range(0, len(result)):
            coldesc = result[idx]
            contents = '{:{n}s}|'.format(str(coldesc[0]), n=coldesc[2]) 
            print(contents, end='')
        print()

        # print the header separation line
        # -------------+-------------------------+
        for idx in range(0, len(result)):
            coldesc = result[idx]
            contents = '-' * coldesc[2] + '+' 
            print(contents, end='')
        print()

        # Dump the rows.
        for row in rowset:
            print(row)
            #===================================================================
            # idx = 0
            # for col in row:
            #     coldesc = result[idx]
            #     contents = '{:{n}s}|'.format(str(col), n=coldesc[2]) 
            #     print(contents, end='')
            #     idx += 1
            # print()
            #===================================================================


    def selectAll(self, selection, tableName):
        stmt = '''SELECT {} FROM {}'''.format(selection, tableName)
        print("SQL: {}".format(stmt))
        rows = self.conn.execute(stmt)
        self.printData(tableName, selection, rows)


    def dumpDatabase(self):
        self.selectAll('*', 'pageref')
        #rows = self.conn.execute("SELECT * FROM pageref")
        #for row in rows:
        #    print("==>" + str(row))         
