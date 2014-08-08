-- A collection of stored procedures and statements used for convenience

-- Create a database
-- NOTE: Do not let the the database name start with "test" - there apply special grants to 
-- databases with this name: http://dev.mysql.com/doc/refman/5.5/en/default-privileges.html
-- "In addition, the mysql.db table contains rows that permit all accounts to 
-- access the test database and other databases with names that start with test_".
CREATE DATABASE dbname;
CREATE DATABASE dbname CHARACTER SET utf8;

ALTER DATABASE dbname CHARACTER SET utf8;

-- Delete a database
DROP DATABASE dbname;

-- Create a new user
CREATE USER user IDENTIFIED BY 'password';
CREATE USER 'user'@'host' IDENTIFIED BY 'password';

-- Delete a user
DROP USER user;

-- Show all privileges supported by the server
SHOW PRIVILEGES;

-- grant (all) privileges to a user
GRANT ALL ON database.* TO user;

-- Clear the privilege cache in order to make the grants effective
FLUSH PRIVILEGES;

-- Show privileges granted to a user
SHOW GRANTS FOR 'user'@'host';

-- Query engine used for tables in a specific database
SELECT TABLE_NAME, ENGINE
FROM information_schema.TABLES
WHERE TABLE_SCHEMA = 'dbname';

-- Complete sample - creates a user "sample" and a database "mysample"
CREATE USER 'sample'@'localhost' IDENTIFIED BY 'sample';
CREATE DATABASE mysample;
GRANT ALL ON mysample.* TO sample;
FLUSH PRIVILEGES;

CREATE TABLE mydata (
	cid    int not null auto_increment,
	primary key (cid),
    cvalue varchar(100)
);

INSERT INTO mydata (cvalue) VALUES("Hello");
INSERT INTO mydata (cvalue) VALUES("World");
INSERT INTO mydata (cvalue) VALUES("Sun");
INSERT INTO mydata (cvalue) VALUES("äöü€");

-- Show default character set for database
SHOW CREATE DATABASE `dbname`;

-- Show default character set for schema
SELECT default_character_set_name 
FROM information_schema.SCHEMATA S 
WHERE schema_name = "schemaname";

-- Show default character set for table
SELECT CCSA.character_set_name 
FROM information_schema.`TABLES` T,
     information_schema.`COLLATION_CHARACTER_SET_APPLICABILITY` CCSA
WHERE CCSA.collation_name = T.table_collation
  AND T.table_schema = "schema"
  AND T.table_name = "tablename";

-- Show default character set for column
SELECT character_set_name 
FROM information_schema.`COLUMNS` C
WHERE table_schema = "schemaname"
  AND table_name = "tablename"
  AND column_name = "columnname";

-- Show default character set for all columns of a table
SELECT column_name, character_set_name 
FROM information_schema.`COLUMNS` C
WHERE table_schema = "schemaname"
  AND table_name = "tablename";
