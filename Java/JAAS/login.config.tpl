KerberosLogin {
    com.sun.security.auth.module.Krb5LoginModule REQUIRED 
        useTicketCache=true
        debug=true;
};

/* Requires dn: cn=John Doe,ou=people,dc=example,dc=com */
LDAPLogin {
    com.sun.security.auth.module.LdapLoginModule REQUIRED
        userProvider="ldap://hostname"
        useSSL=false
        authIdentity="cn={USERNAME},ou=people,dc=example,dc=com"
        debug=true;
};  

/* Requires dn: uid=johndoe,ou=people,dc=example,dc=com */
LDAPUidLogin {
    com.sun.security.auth.module.LdapLoginModule REQUIRED
        userProvider="ldap://hostname"
        useSSL=false
        authIdentity="uid={USERNAME},ou=people,dc=example,dc=com"
        debug=true;
};

/* Requires dn: uid=johndoe,ou=people,ou=myrealm,dc=DefaultDomain*/
LDAPWLSLogin {
    com.sun.security.auth.module.LdapLoginModule REQUIRED
        userProvider="ldap://hostname:port"
        useSSL=false
        authIdentity="uid={USERNAME},ou=people,ou=myrealm,dc=DefaultDomain"
        debug=true;
};


WindowsLogin {
    com.sun.security.auth.module.NTLoginModule required;
};


UnixLogin {
    com.sun.security.auth.module.UnixLoginModule required;
};
