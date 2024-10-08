"""
Created on 04.05.2015

@author: afester
"""

import fnmatch
import logging
import os
import sqlite3
import urllib

from model.Page import LocalPage


class NotepadDB:
    
    LOG = logging.getLogger('NotepadDB')

    def __init__(self):
        self.conn = None

    def openDatabase(self, notepad):
        self.rootDir = notepad.getRootpath()
        self.dbFile = self.rootDir + '/mynpad.dbf'

        # Create the database file if it does not exist yet and open the database
        self.LOG.debug('Opening {}'.format(self.dbFile))
        self.conn = sqlite3.connect(self.dbFile)

        self.createDatabase(notepad)

        # self.dumpDatabase()

    def closeDatabase(self):
        self.conn.close()

    def createDatabase(self, notepad):
        result = self.conn.execute('''
SELECT * 
FROM sqlite_master 
WHERE type='table' AND name='pageref'
''')

        if result.fetchone() is None:
            print('Creating database schema ...')
            self.conn.execute('''
CREATE TABLE page  (
    pageId TEXT
)''')
            self.conn.execute('''
CREATE UNIQUE INDEX pageIdx ON page(pageId)
''')

            self.conn.execute('''
CREATE TABLE pageref  (
    parentId TEXT, 
    childId TEXT 
)''')
            self.conn.execute('''
CREATE INDEX parentIdx ON pageref(parentId)
''')
            self.conn.execute('''
CREATE INDEX childIdx ON pageref(childId)
''')

            self.refreshDatabase(notepad)

    def refreshDatabase(self, notepad):
        
        self.LOG.debug('Refreshing database {}'.format(self.dbFile))

        self.conn.execute('''
DELETE FROM pageref''')
        self.conn.execute('''
DELETE FROM page''')

        # TODO: We need a Notepad instance here.
        # Then the following code to search through a notepad can be moved into the Notepad class:

        for root, dirnames, filenames in os.walk(self.rootDir):
            for filename in fnmatch.filter(filenames, '*.xml'):
                pageId = urllib.parse.unquote(filename)[:-4]

                stmt = '''
INSERT INTO page VALUES(?)'''
                self.conn.execute(stmt, (pageId, ))

                page = LocalPage(notepad, pageId)
                page.load()

                # print('\n"{}"'.format(pageId))
                for childLink in page.getLinks():
                    # print('   => "{}"'.format(childLink))
                    stmt = '''
INSERT INTO pageref VALUES(?, ?)'''
                    self.conn.execute(stmt, (pageId, childLink))
                # print()

        self.conn.commit()

    def getAllPages(self):
        result = []

        rows = self.conn.execute('''
SELECT pageId 
FROM page 
ORDER BY pageId 
COLLATE NOCASE''')
        for row in rows:
            result.append(row[0])

        return result

    def getChildPages(self, pageId):
        result = []

        resCursor = self.conn.execute('''
SELECT childId 
FROM pageref
WHERE parentId = ? 
ORDER BY childId''', (pageId, ))
        for row in resCursor:
            result.append(row[0])

        return result

    def getParentPages(self, pageId):
        result = []

        resCursor = self.conn.execute('''
SELECT parentId 
FROM pageref
WHERE childId = ? 
ORDER BY parentId''', (pageId, ))
        for row in resCursor:
            result.append(row[0])

        return result

    def getChildCount(self, pageId):
        resCursor = self.conn.execute('''
SELECT COUNT(*) 
FROM pageref 
WHERE parentId = ? ''', (pageId, ))
        row = resCursor.fetchone()
        return row[0]

    def getChildPagesWithHandle(self, pageId):
        result = []

        resCursor = self.conn.execute('''
SELECT childId 
FROM pageref 
WHERE parentId = ? 
ORDER BY childId''', (pageId, ))
        for row in resCursor:
            pageId = row[0]
            childCount = self.getChildCount(pageId) 
            result.append((pageId, childCount))

        # print('{}'.format(result))
        return result

    def getOrphanedPages(self):
        result = []
        rows = self.conn.execute('''
SELECT pageId 
FROM page 
WHERE pageId <> 'Title page' AND pageId NOT IN (SELECT DISTINCT childId FROM pageref) 
ORDER BY pageId''')
        for row in rows:
            result.append(row[0])
        return result

    def updateLinks(self, pageId, linksTo):
        self.LOG.debug('Updating links for "{}": {}'.format(pageId, linksTo))

        # Make sure that the page exists in the pages table
        try:
            stmt = '''
INSERT INTO page VALUES(?)'''
            self.conn.execute(stmt, (pageId, ))
        except sqlite3.IntegrityError:      # Ignore unique constraint violation
            pass

        stmt = '''
DELETE FROM pageref 
WHERE parentId=?'''
        self.conn.execute(stmt, (pageId, ))

        for childLink in linksTo:
            stmt = '''
INSERT INTO pageref VALUES(?, ?)'''
            self.conn.execute(stmt, (pageId, childLink))

        self.conn.commit()

    def _recFind(self, parentId, pageId, result):
        childLinks = self.getChildPages(parentId)
        for link in childLinks:
            # If the child page is not yet in the path, continue with it as the parent page
            # (Otherwise just continue with the next child)
            if link not in result:
                result.append(link)

                if link == pageId:
                    self.found = True
                    return

                self._recFind(link, pageId, result)

                if self.found:
                    break

                result.pop()
            else:
                NotepadDB.LOG.info("NOTE: Recursion found ({} => {})".format(result, link))

    def getPathToPage(self, pageId):
        """Returns a path to the given page. Multiple paths might exist, so this method
           returns the first path found based on a depth first traversal.

           :param pageId: The id of the page for which to find a path.
           """

        # print('getPathToPage({})'.format(pageId))

        result = []
        if pageId != 'Title page':
            self.found = False
            self._recFind('Title page', pageId, result)

        # print('  => {}'.format(result))
        return result

# TODO: move to unit tests #####################################################

    def printData(self, tableName, selection, rowset):
        # We first need to iterate the rowset and retrieve the rows into
        # a temporary data structure. Otherwise, when executing the PRAGMA table_info
        # statement below, the rowset is messed up
        allRows = []
        for row in rowset:
            allRows.append(row)

        # NOTE: The following statement messes up the rowset passed as parameter,
        # Probably a SQLite bug (or a bug in the python binding)
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
        for row in allRows:
            # print(row)

            idx = 0
            for col in row:
                coldesc = result[idx]
                contents = '{:{n}s}|'.format(str(col), n=coldesc[2]) 
                print(contents, end='')
                idx += 1
            print()

    def selectAll(self, selection, tableName):
        stmt = '''SELECT {} FROM {}'''.format(selection, tableName)
        rows = self.conn.execute(stmt)
        self.printData(tableName, selection, rows)

    def dumpDatabase(self):
        print('All pages\n==================================================')
        allPages = self.getAllPages()
        for pageId in allPages:
            print(pageId)
        print()

        print('Page relationships\n==================================================')
        self.selectAll('*', 'pageref')
        print()

        print('Orphaned pages\n==================================================')
        orphaned = self.getOrphanedPages()
        print(orphaned)

        print('\nChild pages of "Title page"\n==================================================')
        children = self.getChildPages('Title page')
        print(children)

        print('\nNumber of child pages of "Title page"\n==================================================')
        children = self.getChildCount('Title page')
        print(children)

        print('\nChild pages of "Title page", including a child indicator flag\n'
              '==================================================')
        children = self.getChildPagesWithHandle('Sample Page')
        print(children)
