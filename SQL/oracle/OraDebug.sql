-- Usage:
-- 
--   Start nc from the command line like
--   $ nc -lk 1234
-- 
--   Start sqlplus and start logging
--   $ sqlplus user/pass@sid
--   SQL> begin Debug.Open(1234); end;
--     2  /
--   SQL> begin Debug.WriteLine('Sample output');  end;
--     2  /
--
--   All output will be printed by nc.
--

-- DROP PACKAGE Debug;

CREATE OR REPLACE PACKAGE Debug AUTHID CURRENT_USER AS

   PROCEDURE Open(portNumber PLS_INTEGER);

   PROCEDURE WriteLine(data IN VARCHAR2);

   PROCEDURE Close;

--   PROCEDURE GrantPermission(userName IN VARCHAR2);
END Debug;
/


CREATE OR REPLACE PACKAGE BODY Debug AS
   c  utl_tcp.connection;  -- TCP/IP connection to the Debug console
   ret_val pls_integer; 

   PROCEDURE Open(portNumber PLS_INTEGER) IS
   BEGIN
       c := utl_tcp.open_connection(remote_host => 'localhost',
                                    remote_port =>  portNumber,
                                    charset     => 'US7ASCII');
       -- TODO: Error handling

       ret_val := utl_tcp.write_line(c, '-------------------------------------------');
       WriteLine('New debug connection');
   END;

   
   PROCEDURE WriteLine(data IN VARCHAR2) IS
   BEGIN
       ret_val := utl_tcp.write_line(c, TO_CHAR(SYSDATE, '[MM-DD-YYYY HH24:MI:SS] ') || data);
       -- TODO: Error handling
   END;

   
   PROCEDURE Close IS
   BEGIN
       WriteLine('Closing debug connection');
       ret_val := utl_tcp.write_line(c, '-------------------------------------------');
       utl_tcp.close_connection(c);
   END;

--   PROCEDURE GrantPermission(userName IN VARCHAR2) IS
--
--   portNumber PLS_INTEGER := 1234;
--
--   BEGIN
--		EXECUTE IMMEDIATE 'GRANT EXECUTE ON Debug TO :userName';
--		
--		BEGIN
--			-- Privileges acquired via ROLE do not apply with named PL/SQL procedure
--			-- Hence the SYSTEM user can not use the DBMS_NETWORK_ACL_ADMIN package inside this PL/SQL procedure
--			-- explicit GRANT needed to fix this - bug how? user SYSTEM can not grant the EXECUTE 
--          -- privilege on DBMS_NETWORK_ACL_ADMIN to itself!
--			SYS.DBMS_NETWORK_ACL_ADMIN.DROP_ACL('debug-console-permission.xml');
--		EXCEPTION
--			WHEN OTHERS THEN
--				IF SQLCODE <> -31001 AND SQLCODE <> -46114 THEN
--					RAISE;
--				END IF;
--		END;
--   END;

END Debug;
/


-- If using a separate runtime user, then grant execute rights to the package
GRANT EXECUTE ON Debug TO runtimeUser;


-- Grant permission to specific user to use the Debug package.
-- User needs EXECUTE permission on the Debug package and an
-- ACL entry for the 'connect' privilege on the specified port.
-- This needs to be run as a privileged user (sysdba)
--
-- TODO: How to move this anonymous PL/SQL block into a package?
DECLARE
  -- define list of users who should be granted network access
  type userNamesArray IS VARRAY(5) OF VARCHAR2(20);
  userNames userNamesArray := userNamesArray('SCOTT', 'SCOTT_RUNTIME');

  portNumber PLS_INTEGER := 1234;
  aclName VARCHAR2(30) := 'debug-console-permission.xml';

BEGIN
  -- Remove existing ACL, ignore if the ACL did not exist
  BEGIN
    DBMS_NETWORK_ACL_ADMIN.DROP_ACL(aclName);
  EXCEPTION
    WHEN OTHERS THEN
       IF SQLCODE <> -31001 AND SQLCODE <> -46114 THEN
          RAISE;
       END IF;
  END;

  -- Create new ACL
  DBMS_NETWORK_ACL_ADMIN.CREATE_ACL (
    acl => aclName,
    description => 'Permissions for Network access to remote debug console',
    principal => userNames(1),
    is_grant => TRUE,
    privilege => 'connect'
  );

  -- grant access to localhost:portNumber
  DBMS_NETWORK_ACL_ADMIN.ASSIGN_ACL (
    acl => aclName,
    host => 'localhost',
    lower_port => portNumber,
    upper_port => portNumber
  );

  -- add additional principals
  FOR i in 2 .. userNames.count LOOP
	DBMS_NETWORK_ACL_ADMIN.ADD_PRIVILEGE(acl       => aclName,
										 principal => userNames(i),
                                         is_grant  => true,
                                         privilege => 'connect');
  END LOOP;
END;
/

SET HEAD OFF
SET LINESIZE 1000

SELECT name, type, line, text 
FROM sys.user_errors 
WHERE name='DEBUG';

-- SHOW ERRORS;

EXIT;
