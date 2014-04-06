# JAAS configuration properties - adjust as necessary and
# rename the file to security.properties

# Security specific properties
java.security.auth.login.config=login.config

# Kerberos specific properties
java.security.krb5.realm=<realm name, like sample.example.com>
java.security.krb5.kdc=<KDC server address and port, like ourkdc.example.com:88>

# Sample specific properties
#com.example.LoginConfig=KerberosLogin
#com.example.PolicyFile==kerberos.policy
com.example.LoginConfig=LDAPWLSLogin
com.example.PolicyFile==ldap.policy

# Development properties
com.example.UserName=username
com.example.UserPass=secret
