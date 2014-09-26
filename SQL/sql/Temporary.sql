-- A collection of SQL statements to work with temporary tables

DROP TABLE SampleTemp;

CREATE GLOBAL TEMPORARY TABLE SampleTemp (
    Id NUMBER(10),
    Name VARCHAR2(100)
) ON COMMIT DELETE ROWS;	-- Transaction scope - the default

CREATE GLOBAL TEMPORARY TABLE SampleTemp (
    Id NUMBER(10),
    Name VARCHAR2(100)
) ON COMMIT PRESERVE ROWS;	-- Session scope

-- Note: When debugging global temporary tables, the usual advice is
-- to switch it to a normal table so that the contents can be seen from 
-- a different session, and then later switch it back to temporary again. 

INSERT INTO SampleTemp VALUES (0, 'Nullest');
INSERT INTO SampleTemp VALUES (1, 'First');
INSERT INTO SampleTemp VALUES (2, 'Second');
INSERT INTO SampleTemp VALUES (3, 'Third');

SELECT * FROM SampleTemp;
