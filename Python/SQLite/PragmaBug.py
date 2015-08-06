#!/usr/bin/env python3

import sqlite3

print('Using sqlite3 {} with SQLite runtime {}'.format(sqlite3.version, sqlite3.sqlite_version))

conn = sqlite3.connect('sample.dbf')
try:
    conn.execute('''DROP TABLE mytable''')
except:
    pass

conn.execute('''CREATE TABLE mytable (parentId TEXT, childId TEXT)''')
conn.execute('''INSERT INTO mytable VALUES('A', 'B')''')
conn.execute('''INSERT INTO mytable VALUES('C', 'D')''')

c1 = conn.cursor()
c1.execute('''SELECT * FROM mytable''')

c2 = conn.cursor()
c2.execute('''PRAGMA table_info(mytable)''')

for row in c1:
    print(row)

# rows = conn.execute('''SELECT * FROM mytable''')
# conn.execute('''PRAGMA table_info(mytable)''')
