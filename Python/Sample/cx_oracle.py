#!/usr/bin/python 

import cx_Oracle

dbHost = "localhost"
dbPort = "1521"
dbUser = "scott"
dbPassword = "tiger"
dbSid = "orcl"

dsn = cx_Oracle.makedsn(dbHost, dbPort, dbSid)
conn = cx_Oracle.connect(user=dbUser, password=dbPassword, dsn=dsn)
# conn = cx_Oracle.connect(user="myuser", password="mypass", dsn=dsnStr)
print(conn.version)
con.close()

