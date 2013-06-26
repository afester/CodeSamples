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
