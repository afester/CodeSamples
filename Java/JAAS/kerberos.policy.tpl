/* minimal set of permissions required to setup the security system and do the authentication */

/* Krb5Module requires default access to standard extensions, need to grant it
   explicitly since we have defined a brand new policy file (using the "=" syntax).
   Otherwise the runtime can not load the sunEC (elliptic curve) extension library. 
 */
grant codeBase "file:${{java.ext.dirs}}/*" {
   permission java.security.AllPermission;
};

grant {
   permission java.util.PropertyPermission       "*", "read,write";
   permission java.io.FilePermission             "security.properties", "read";

   permission javax.security.auth.AuthPermission "createLoginContext.KerberosLogin";
   permission javax.security.auth.AuthPermission "doAsPrivileged";
};

grant Principal javax.security.auth.kerberos.KerberosPrincipal "user@example.com" {
   permission java.lang.RuntimePermission "executeAllowedAction";
};
