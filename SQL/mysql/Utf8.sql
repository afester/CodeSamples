-- Note: this file saved as utf-8 file!
--
-- Sample statements to work with UTF-8 data

-- Create a database with default character set utf8.
-- This can be overriden in CREATE TABLE statements by providing an explicit 
-- character set.
CREATE DATABASE dbname CHARACTER SET utf8;
GRANT ALL ON dbname.* TO username;

-- Create a table - default character set (utf-8)
CREATE TABLE mydata (
	cid    int not null auto_increment,
	primary key (cid),
    cvalue varchar(100)
);

-- create a table - explicit character set latin1 
CREATE TABLE sampledata (
	cid    int not null auto_increment,
	primary key (cid),
    cvalue varchar(100)
) CHARACTER SET latin1;
--DEFAULT CHARSET=latin1;
--CHARSET=latin1;
--DEFAULT CHARACTER SET latin1;
--CHARSET is a synonym for CHARACTER SET. DEFAULT is optional (source?)

SELECT table_name, CCSA.character_set_name 
FROM information_schema.`TABLES` T,
     information_schema.`COLLATION_CHARACTER_SET_APPLICABILITY` CCSA
WHERE CCSA.collation_name = T.table_collation
  AND T.table_schema = "schemaname";
+------------+--------------------+
| table_name | character_set_name |
+------------+--------------------+
| sampledata | latin1             |
| mydata     | utf8               |
+------------+--------------------+

SELECT table_name || column_name, character_set_name 
FROM information_schema.`COLUMNS` C
WHERE table_schema = "schemaname";

SELECT HEX("你好") 
FROM dual;

+---------------+
| E4BDA0E5A5BD  |
+---------------+


INSERT INTO mydata (cvalue) VALUES("你好");

SELECT cid, cvalue, hex(cvalue) 
FROM mydata;
+-----+--------+--------------+
| cid | cvalue | hex(cvalue)  |
+-----+--------+--------------+
|   1 | 你好   | E4BDA0E5A5BD |
+-----+--------+--------------+

CREATE TABLE chardata (
	cid    int not null auto_increment,
	primary key (cid),
    cvalue  VARCHAR(100),						-- Takes default character set from table
    cvalue1 VARCHAR(100) CHARACTER SET latin1,	-- Explicitly sets the character set to Latin1
	cvalue2 VARCHAR(100) CHARACTER SET utf8  	-- Explicitly sets the character set to Latin1    
);

