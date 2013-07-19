-- Creates a simple schema to be used for various SQL queries

-- EXEC is SQL*Plus specific command - when used with other worksheets,
-- we need to use an anonmyous PL/SQL block:
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

CREATE TABLE Performance(
    Col1 VARCHAR2(10),
    Col2 VARCHAR2(10),
    Col3 VARCHAR2(10),
    Col4 VARCHAR2(10),
    Col5 VARCHAR2(10),
    Col6 VARCHAR2(10),
    Col7 VARCHAR2(10),
    Col8 VARCHAR2(10),
    Col9 VARCHAR2(10),
    Col10 VARCHAR2(10)
);


CREATE OR REPLACE PROCEDURE CreateSampleData(recordCount IN NUMBER) IS
BEGIN
    FOR cCount IN 1..recordCount LOOP
        INSERT INTO Performance VALUES('ABCDEFGHIJ', 'KLMNOPQRST', 'UVWXYZabcde', 'fghijklmno', 'pqrstuvwxy', 
                                       'z123456789', '0ABCDEFGHI', 'KLMNOPQRSTU', 'UVWXYZabcd', 'efghijklmn');
    END LOOP;
END;
/

BEGIN CreateSampleData(4000); END;
