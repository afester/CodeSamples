KerberosLogin {
    com.sun.security.auth.module.Krb5LoginModule REQUIRED 
        useTicketCache=true
        debug=true;
};


LDAPLogin {
    com.sun.security.auth.module.LdapLoginModule REQUIRED
        userProvider="ldap://hostname"
        useSSL=false
        authIdentity="cn={USERNAME},ou=people,dc=example,dc=com"
        authzIdentity="staff"
        debug=true;
};  


LDAPLoginAdmin {
    com.sun.security.auth.module.LdapLoginModule REQUIRED
        userProvider="ldap://hostname/ou=people,ou=myrealm,dc=DefaultDomain"
        authIdentity="cn={USERNAME}"
        debug=true
        useSSL=false;
};


WindowsLogin {
    com.sun.security.auth.module.NTLoginModule required;
};


UnixLogin {
    com.sun.security.auth.module.UnixLoginModule required;
};
