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
