-- A collection of stored procedures used for convenience


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

-- Select tables from data dictionary
SELECT table_name 
FROM user_tables
WHERE table_name LIKE 'MY_%'
ORDER BY table_name;

-- Select all columns for a table
SELECT column_name, data_type, data_length, owner
FROM   all_tab_cols
WHERE  table_name = 'MY_TABLE'
ORDER BY column_name;

-- Select view definition for a given view
SELECT TEXT
FROM DBA_VIEWS
WHERE VIEW_NAME='MY_VIEW';

