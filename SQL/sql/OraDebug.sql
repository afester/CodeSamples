DROP PACKAGE Debug;

CREATE PACKAGE Debug AUTHID CURRENT_USER AS

   PROCEDURE Open(portNumber PLS_INTEGER);

   PROCEDURE WriteLine(data IN VARCHAR2);

   PROCEDURE Close;

END Debug;
/

CREATE PACKAGE BODY Debug AS
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

END Debug;
/


-- Grant permission to specific user to use the Debug package.
-- User needs EXECUTE permission on the Debug package and an
-- ACL entry for the 'connect' privilege on the specified port.
-- TODO: How to move this anonymous PL/SQL block into a package?
DECLARE
  userName VARCHAR2(20) := 'USER';
  portNumber PLS_INTEGER := 1234;

BEGIN
  EXECUTE IMMEDIATE 'GRANT EXECUTE ON Debug TO ' || userName;

  -- Remove existing ACL, ignore if the ACL did not exist
  BEGIN
    DBMS_NETWORK_ACL_ADMIN.DROP_ACL('debug-console-permission.xml');
  EXCEPTION
    WHEN OTHERS THEN
       IF SQLCODE <> -31001 THEN
          RAISE;
       END IF;
  END;

  -- Create new ACL
  DBMS_NETWORK_ACL_ADMIN.CREATE_ACL (
    acl => 'debug-console-permission.xml',
    description => 'Permissions for Network access to remote debug console',
    principal => userName,
    is_grant => TRUE,
    privilege => 'connect'
  );

  -- grant access to localhost:portNumber
  DBMS_NETWORK_ACL_ADMIN.ASSIGN_ACL (
    acl => 'debug-console-permission.xml',
    host => 'localhost',
    lower_port => portNumber,
    upper_port => portNumber
  );
END;
/

SET HEAD OFF
SET LINESIZE 1000
SELECT name, type, line, text FROM sys.user_errors WHERE name='DEBUG';
EXIT;
