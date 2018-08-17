-- A collection of stored procedures used for convenience



-- Copy all columns of a table row and modify individual columns
-- See http://stackoverflow.com/questions/28278681/oracle-copy-row-while-updating-one-field-for-table-with-many-columns
DECLARE
  l_data my_table%rowtype;
BEGIN
  -- fetch the row we want to copy
  SELECT * INTO l_data FROM my_table WHERE pk = 17;

  -- update all fields that need to change
  l_data.pk := 18;

  -- note the lack of parens around l_data in the next line
  INSERT INTO my_table VALUES l_data; 
END;
/

-- Drops a table, independant of whether it already exists or not.
--
-- @param tableName The name of the table to drop.
--  
CREATE OR REPLACE PROCEDURE DROP_TABLE_IF_EXISTS(tableName IN VARCHAR2) IS
BEGIN
    EXECUTE IMMEDIATE 'DROP TABLE ' || tableName;
EXCEPTION
   WHEN OTHERS THEN
      IF SQLCODE != -942 THEN
         RAISE;
      END IF;
END;
/


-- Drops a sequence, independant of whether it already exists or not.
--
-- @param sequenceName The name of the sequence to drop.
--  
CREATE OR REPLACE PROCEDURE DROP_SEQUENCE_IF_EXISTS(sequenceName IN VARCHAR2) IS
BEGIN
    EXECUTE IMMEDIATE 'DROP SEQUENCE ' || sequenceName;
EXCEPTION
   WHEN OTHERS THEN
      IF SQLCODE != -2289 THEN
         RAISE;
      END IF;
END;
/


-- Show tablespaces
SELECT UNIQUE TABLESPACE_NAME 
FROM dba_tablespaces
ORDER BY tablespace_name;

SELECT tablespace_name,
       bytes / (1024*1024) as "Bytes (MiB)", blocks, 
       maxbytes / (1024*1024) as "Max Bytes (MiB)", maxblocks, 
       user_bytes / (1024*1024) as "User Bytes (MiB)", user_blocks,
       substr(file_name, 0, 40)  
FROM dba_data_files;

select * from dba_tablespaces where tablespace_name not in (select tablespace_name from dba_data_files);
select * from dba_tablespaces where tablespace_name in (select tablespace_name from dba_data_files);

select unique tablespace_name 
from dba_data_files
order by tablespace_name;


-- Create user
-- Oracle Database automatically creates a schema when you create a user.
CREATE USER userName 
    IDENTIFIED BY "password"    -- NOTE: quotes required when password starts with letter! But NOT a String (no ' quote!!!)
    DEFAULT TABLESPACE users
    QUOTA unlimited on users    -- NOTE: users require quota on tablespace, default is 0
    TEMPORARY TABLESPACE temp;

-- ALTER USER userName QUOTA unlimited on users;

-- Change password for user
ALTER USER userName IDENTIFIED BY "password";


GRANT connect TO userName;
GRANT create table TO userName;
GRANT create sequence TO userName;
GRANT create view TO userName;
GRANT create procedure to userName;
GRANT EXECUTE ON a.procedure TO userName;
GRANT DELETE, INSERT, SELECT, UPDATE ON table TO userName;

-- show grants for user
-- System privileges
SELECT * 
FROM dba_sys_privs 
WHERE grantee='userName';

-- Table privileges
SELECT * 
FROM DBA_TAB_PRIVS 
WHERE grantee = 'userName';


-- select available users
SELECT UserName, Password, DEFAULT_TABLESPACE, TEMPORARY_TABLESPACE, PROFILE
FROM DBA_USERS;

-- Select tables for user from data dictionary
SELECT table_name 
FROM user_tables
WHERE table_name LIKE 'MY_%'
ORDER BY table_name;

-- Select all objects and their type
SELECT OBJECT_NAME, OBJECT_TYPE
FROM DBA_OBJECTS
WHERE OBJECT_NAME LIKE ...
ORDER BY OBJECT_NAME;

-- Select all invalid Views
SELECT OBJECT_NAME, OBJECT_TYPE
FROM DBA_OBJECTS
WHERE OBJECT_TYPE='VIEW' AND STATUS='INVALID';

-- SELECT VIEW definition of invalid views
SET HEAD OFF
SET LINESIZE 8192
SET TRIMSPOOL ON
SPOOL views.sql
SELECT 'CREATE OR REPLACE VIEW ' || OBJECT_NAME || ' AS ', CLOB(TEXT_VC) || ';'
FROM DBA_OBJECTS
JOIN DBA_VIEWS ON (OBJECT_NAME=VIEW_NAME)
WHERE OBJECT_TYPE='VIEW' AND STATUS='INVALID';

-- Select all columns for a table
SELECT column_name, data_type, data_length, owner
FROM   all_tab_cols
WHERE  table_name = 'MY_TABLE'
ORDER BY column_name;

-- Select view definition for a given view
SELECT TEXT_VC
FROM DBA_VIEWS
WHERE VIEW_NAME='MY_VIEW';

-- Select table definition for a given table
SELECT TABLE_NAME, TABLESPACE_NAME, STATUS, TEMPORARY
FROM DBA_TABLES
WHERE TABLE_NAME LIKE 'xyz%';

-- Select all temporary tables
SELECT TABLE_NAME, TABLESPACE_NAME, STATUS
FROM DBA_TABLES
WHERE TEMPORARY='Y'
ORDER BY TABLE_NAME;

-- Select constraint definition
SELECT OWNER, CONSTRAINT_NAME, CONSTRAINT_TYPE, TABLE_NAME, SEARCH_CONDITION 
FROM ALL_CONSTRAINTS 
WHERE CONSTRAINT_NAME = 'constraintName';

-- Select all Index definitions for a table
SELECT index_name 
FROM user_indexes
WHERE table_name = 'tableName';

-- Retrieve the index definition 
SELECT DBMS_METADATA.GET_DDL('INDEX','IndexName') 
FROM DUAL;


-- Dump the DDL statement used to create a table
SELECT DBMS_METADATA.GET_DDL('TABLE','tableName', 'schemaName') 
FROM DUAL;


-- Select all current USER (not BACKGROUND) sessions
-- "process" is the process ID of the client
SELECT sid, serial#, username, status, process, machine, sql_id
FROM v$session 
WHERE type='USER';

-- Select *current* SQL statement for user 
--SELECT rows_processed, sql_text
--FROM v$sql
--JOIN v$session ON v$session.sql_id=v$sql.sql_id
--WHERE username='userName';
SELECT rows_processed, sql_fulltext
FROM v$sql
JOIN v$session ON v$session.sql_id=v$sql.sql_id
WHERE username='userName';


-- Select current SQL statement for user, including the bind variables
-- (Note: each bind variable results in one row - TODO: Use the PIVOT operator ...) 
SELECT rows_processed, sql_text, Name, Position, Datatype_String,Value_String
FROM v$sql
JOIN v$session ON v$session.sql_id=v$sql.sql_id
JOIN v$sql_bind_capture ON v$session.sql_id=v$sql_bind_capture.sql_id
WHERE username='userName';

-- Selects the current session id (references AUDSID in v$session) 
SELECT SYS_CONTEXT('userenv','sessionid') AUDSID 
FROM dual;

-- Selects the current session id (references SID in v$session)
SELECT SYS_CONTEXT('userenv','SID') SID 
FROM dual;

SELECT osuser, program, user, sid, audsid
FROM v$session 
WHERE sid=SYS_CONTEXT('USERENV', 'SID');

SELECT 'osuser="' || osuser || '", program="' || program || '", dbuser="' || user || 
       '", sid="' || sid || '", audsid="' || audsid || '"' sessionInfo
FROM v$session 
WHERE sid=SYS_CONTEXT('USERENV', 'SID');


SELECT sid, serial# 
FROM SYS.V_$SESSION
WHERE SID = (SELECT DISTINCT SID FROM SYS.V_$MYSTAT);

-- Triggers
COLUMN Owner FORMAT A30
COLUMN Trigger_Name FORMAT A30
SELECT OWNER, TRIGGER_NAME, TRIGGER_TYPE, STATUS
FROM DBA_TRIGGERS
WHERE TRIGGER_TYPE='AFTER EVENT'
ORDER BY OWNER;

ALTER TRIGGER triggerName ENABLE;
ALTER TRIGGER triggerName DISABLE;


-- Execution plan
EXPLAIN PLAN 
SET statement_id = 'somePlanId' 
FOR <SQL Statament>;

SELECT lpad(' ',level-1)||operation||' '||options||' '||object_name "Plan",
		cost, io_cost
   FROM plan_table
CONNECT BY prior id = parent_id AND prior statement_id = statement_id
  START WITH id = 0 AND statement_id = 'somePlanId'
  ORDER BY id;


CREATE OR REPLACE FUNCTION getRowId(theRowId IN VARCHAR2) RETURN VARCHAR2 IS
BEGIN
   RETURN SUBSTR(theRowId, 1, 6)||'-'||SUBSTR(theRowId, 7, 3)||'-'||SUBSTR(theRowId, 10, 6)||'-'||SUBSTR(theRowId, 16, 3);
END;
/
