-- A collection of SQL statements for Oracle Text

-- Sample schema creation -----------------------------------------------------
BEGIN DROP_TABLE_IF_EXISTS('TextTable'); END;

CREATE TABLE TextTable (
    C_Id NUMBER(10),
    C_Content VARCHAR2(1000),
    C_CreatedBy VARCHAR2(20)
);
CREATE UNIQUE INDEX TextTablePK ON TextTable(C_Id);

CREATE SEQUENCE TextTablePKSeq START WITH 0 MINVALUE 0 INCREMENT BY 1;

CREATE OR REPLACE TRIGGER TextTablePKTrigger
BEFORE INSERT
   ON TextTable
   FOR EACH ROW

BEGIN
   :new.C_Id := TextTablePKSeq.nextval;
END;
/

INSERT INTO TextTable VALUES(-1, 'This text was created by Andreas Fester', 'afester');
INSERT INTO TextTable VALUES(-1, 'This is a description for some particular process', 'afester');
INSERT INTO TextTable VALUES(-1, 'Here we describe some particular artifact', 'afester');
COMMIT;

-------------------------------------------------------------------------------

CREATE INDEX idx_TextTable
ON TextTable(C_Content)
INDEXTYPE IS CTXSYS.CONTEXT;

SELECT SCORE(1), C_Id, C_Content, C_CreatedBy
FROM TextTable
WHERE CONTAINS(C_Content, 'some', 1) > 0;
-- ==> No Result!

SELECT SCORE(1), C_Id, C_Content, C_CreatedBy
FROM TextTable
WHERE CONTAINS(C_Content, 'particular', 1) > 0;
--
4	4	This is a description for some particular process	afester
4	5	Here we describe some particular artifact			afester


-- Index needs to be manually synchronized!!!!!!!!!
INSERT INTO TextTable VALUES(-1, 'There is another well known process which is particular usefull', 'afester');
COMMIT;

SELECT SCORE(1), C_Id, C_Content, C_CreatedBy
FROM TextTable
WHERE CONTAINS(C_Content, 'particular', 1) > 0;
--
4	4	This is a description for some particular process	afester
4	5	Here we describe some particular artifact			afester

EXEC CTX_DDL.SYNC_INDEX('idx_docs', '2M');

SELECT SCORE(1), C_Id, C_Content, C_CreatedBy
FROM TextTable
WHERE CONTAINS(C_Content, 'particular', 1) > 0;
--
3	4	This is a description for some particular process				afester
3	6	Here we describe some particular artifact						afester
3	7	There is another well known process which is particular usefull	afester

-------------------------------------------------------------------------------
-- Multi column datastore ----------------------------------------------------- 

SELECT SCORE(1), C_Id, C_Content, C_CreatedBy
FROM TextTable
WHERE CONTAINS(C_Content, '%fester', 1) > 0;

CREATE INDEX idx_TextTable
ON TextTable(C_Content, C_CreatedBy)
INDEXTYPE IS CTXSYS.CONTEXT;
--SQL Error: ORA-29851: cannot build a domain index on more than one column
--29851. 00000 -  "cannot build a domain index on more than one column"
--*Cause:    User attempted to build a domain index on more than one column.
--*Action:   Build the domain index only on a single column.


begin
ctx_ddl.create_preference('idx_TextTable', 'MULTI_COLUMN_DATASTORE');
ctx_ddl.set_attribute('idx_TextTable', 'columns', 'C_Content, C_CreatedBy');
end;

-------------------------------------------------------------------------------
-- Querying indexes  

SELECT ctx_report.describe_index('idx_TextTable', 'TEXT')
FROM DUAL;
