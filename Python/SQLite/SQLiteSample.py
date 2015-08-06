'''
Created on 26.03.2015

@author: afester
'''

import sqlite3


class SQLiteSample():

    def __init__(self):
        '''
        Constructor
        '''

    def initializeDB(self):
        self.conn.execute('''CREATE TABLE address
             (id INTEGER, firstname TEXT, lastname TEXT, eMail TEXT)''')
        self.conn.execute('''CREATE TABLE todoItem
             (id INTEGER, parent INTEGER, assignedTo INTEGER, name TEXT, description TEXT)''')

        self.conn.execute('''INSERT INTO address
             VALUES (0, 'Andreas', 'Fester', 'andreas@example.com')''')
        self.conn.execute('''INSERT INTO address
             VALUES (0, 'John', 'Doe', 'john@example.com')''')

        self.conn.execute('''INSERT INTO todoItem
             VALUES (1, 0, 0, 'Main Tasks', 'The most important tasks')''')
        self.conn.execute('''INSERT INTO todoItem
             VALUES (2, 1, 0, 'Getting involved', 'Get involved with SQLite')''')
        self.conn.execute('''INSERT INTO todoItem
             VALUES (3, 1, 0, 'Test SQLite', 'Do some SQLite tests')''')
        self.conn.execute('''INSERT INTO todoItem
             VALUES (4, 0, 0, 'Minor Tasks', 'Less important tasks')''')
        self.conn.execute('''INSERT INTO todoItem
             VALUES (5, 4, 0, 'Check SQLite SQL syntax', 'Do some more complicated SQL queries')''')
        self.conn.execute('''INSERT INTO todoItem
             VALUES (6, 4, 0, 'Implement prototype', 'Implement a python prototype')''')
        self.conn.execute('''INSERT INTO todoItem
             VALUES (7, 1, 0, 'Another item', 'Another item')''')
        self.conn.execute('''INSERT INTO todoItem
             VALUES (8, 7, 0, 'Level 3 item', 'Level 3 item')''')
        self.conn.execute('''INSERT INTO todoItem
             VALUES (9, 7, 0, 'Another Level 3 item', 'Another Level 3 item')''')


    def dropTable(self, tableName):
        self.conn.execute('''DROP TABLE {}'''.format(tableName))


    def listSchema(self):
        print('Schema reference:')
        result = self.conn.execute('''SELECT * FROM sqlite_master''')
#WHERE type='table'
#ORDER BY name''')
        for row in result:
            print(row)


    def describeTable(self, tableName):
        result = self.conn.execute('''PRAGMA table_info({})'''.format(tableName))
        for row in result:
            print(row)


    def printData(self, tableName, selection, rowset):
        desc = self.conn.execute('''PRAGMA table_info({})'''.format(tableName))

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

        header = False
        for row in rowset:
            if not header:
                idx = 0
                for col in row:
                    coldesc = result[idx]
                    contents = '{:{n}s}|'.format(str(coldesc[0]), n=coldesc[2]) 
                    print(contents, end='')
                    idx += 1
                idx = 0
                print()
                for col in row:
                    coldesc = result[idx]
                    contents = '-' * coldesc[2] + '+' 
                    print(contents, end='')
                    idx += 1
                header = True
                print()

            idx = 0
            for col in row:
                coldesc = result[idx]
                contents = '{:{n}s}|'.format(str(col), n=coldesc[2]) 
                print(contents, end='')
                idx += 1
            print()


    def selectAll(self, tableName, selection):
        rows = self.conn.execute('''SELECT {} FROM {}'''.format(selection, tableName))
        self.printData(tableName, selection, rows)


    def run(self):
        print('Using sqlite3 {} with SQLite runtime {}'.format(sqlite3.version, sqlite3.sqlite_version))

        self.conn = sqlite3.connect('example.dbf')

        self.initializeDB()
        self.listSchema()

        self.describeTable('address')
        addresses = self.conn.execute('''SELECT * FROM address''')
        for row in addresses:
            print(row)
        print()

        self.selectAll('address', '*')
        print()

        self.selectAll('address', 'firstname, lastname')
        print()

        self.selectAll('todoItem', '*')
        print()

        topNodes = self.conn.execute('''SELECT * FROM todoItem WHERE parent = 0''')
        self.printData('todoItem', '*', topNodes)
        print()

        # Depth first tree query:
        hierarchy = self.conn.execute('''
WITH Node(hierlevel, id, name) AS 
  ( SELECT 0, id, name
    FROM todoItem
    WHERE parent = 0
  UNION ALL
     SELECT hierlevel+1, n.id, n.name 
     FROM todoItem n
     JOIN Node p ON (p.id = parent)
     ORDER BY 1 DESC
  )
SELECT hierlevel, name, id
FROM Node
''')
# "ORDER BY 1" == "ORDER BY todoItem.hierlevel+1")

        for h in hierarchy:
            print('{}{} ({})'.format('..' * h[0], h[1], h[2]))
        print()

        self.dropTable('address')
        self.dropTable('todoItem')
        self.listSchema()

        self.conn.close()


if __name__ == '__main__':
    s = SQLiteSample()
    s.run()
