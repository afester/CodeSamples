-- A collection of stored procedures and statements used for convenience

-- Create a database
-- NOTE: Do not let the the database name start with "test" - there apply special grants to 
-- databases with this name: http://dev.mysql.com/doc/refman/5.5/en/default-privileges.html
-- "In addition, the mysql.db table contains rows that permit all accounts to 
-- access the test database and other databases with names that start with test_".
CREATE DATABASE dbname;

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
