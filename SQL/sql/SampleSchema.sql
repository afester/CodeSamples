-- A stored procedure which drops a table, independant of whether it 
-- already exists or not.
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

-- EXEC does not seem to work - need to use an anonymous PL/SQL block
BEGIN DROP_TABLE_IF_EXISTS('Person'); END;
BEGIN DROP_TABLE_IF_EXISTS('City'); END;

CREATE TABLE Person (
    Id NUMBER(10),
    Firstname VARCHAR2(100),
    Name VARCHAR2(100),
    Email VARCHAR2(256),
    City NUMBER(10)
);

CREATE TABLE City (
    Id NUMBER(10),
    Name VARCHAR2(100)
);

INSERT INTO City VALUES(0, 'Karlsruhe');
INSERT INTO City VALUES(1, 'Frankfurt');
INSERT INTO City VALUES(2, 'Berlin');
INSERT INTO City VALUES(3, 'Stuttgart');
INSERT INTO City VALUES(4, 'München');

INSERT INTO Person VALUES(0, 'Andreas', 'Fester', 'Andreas.Fester@gmx.de', 0);
INSERT INTO Person VALUES(0, 'Sam', 'Example', 'Sam.Example@example.org', 1);
INSERT INTO Person VALUES(0, 'Ali', 'Alien', 'Ali.alien@andromeda.glx', NULL);
INSERT INTO Person VALUES(0, 'Mars', 'Marsian', 'Mars.Marsian@mars.pln', NULL);
