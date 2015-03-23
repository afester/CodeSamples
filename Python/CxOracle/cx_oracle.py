#!/usr/bin/python 

from __future__ import print_function

import sys
import datetime
import cx_Oracle
import connProps

def dumpInserts(dest, conn, tableData):
    tableName = tableData[0]
    whereClause = ''
    if len(tableData) > 1:
        whereClause = tableData[1]

    stmt = "SELECT * FROM {} {}".format(tableName, whereClause)
    print('-- ' + stmt, file=dest)

    cur = conn.cursor()
    cur.execute(stmt)

    row = cur.fetchone ()
    while (row is not None):
        values = ''
        for value in row:
             if value is None:
                 value = 'NULL'
             elif isinstance(value, int):
                 pass
             elif isinstance(value, str):
                 value='\'' + value + '\''
             elif isinstance(value, datetime.datetime):
                 value='TO_DATE(\'{}/{}/{} {}:{}:{}\', \'yyyy/mm/dd hh24:mi:ss\')'.format(value.year, value.month, value.day, value.hour, value.minute, value.second)
             else:
                 print("Unsupported data type: " + type(value))
             if values != '':
                 values = values + ', '
             values = values + str(value)
        print("INSERT INTO {} VALUES ({});".format(tableName, values), file=dest)
        row = cur.fetchone ()


print("Connecting to {}/***@{}:{}/{}".format(connProps.dbUser, connProps.dbHost, connProps.dbPort, connProps.dbSid))

dsn = cx_Oracle.makedsn(connProps.dbHost, connProps.dbPort, connProps.dbSid)
conn = cx_Oracle.connect(user=connProps.dbUser, password=connProps.dbPassword, dsn=dsn)
print("Connected to Oracle " + conn.version)

# destFile = sys.stdout
destFile = open("export.sql", "w")
with open("export.txt", "r") as ins:
    for line in ins:
        line = line.strip()
        lineData = line.split(' ', 1)
        dumpInserts(destFile, conn, lineData)
destFile.close()

conn.close()
