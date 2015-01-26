#!/usr/bin/python 

import cx_Oracle
import connProps

print("Connecting to {}/***@{}:{}/{}".format(connProps.dbUser, connProps.dbHost, connProps.dbPort, connProps.dbSid))

dsn = cx_Oracle.makedsn(connProps.dbHost, connProps.dbPort, connProps.dbSid)
conn = cx_Oracle.connect(user=connProps.dbUser, password=connProps.dbPassword, dsn=dsn)
# conn = cx_Oracle.connect(user="myuser", password="mypass", dsn=dsnStr)
print("Connected to Oracle " + conn.version)

tableName = "Task"

cur = conn.cursor()
cur.execute(
     "SELECT * "
     "FROM " + tableName);

row = cur.fetchone ()
while (row is not None):
    print("INSERT INTO {} VALUES {}".format(tableName, row))
    row = cur.fetchone ()

conn.close()

