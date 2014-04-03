package com.example;

import java.io.FileInputStream;
import java.io.IOException;
import java.security.PrivilegedAction;
import java.util.Properties;

import javax.security.auth.Subject;
import javax.security.auth.login.LoginContext;
import javax.security.auth.login.LoginException;

/*
 * http://docs.oracle.com/javase/7/docs/jre/api/security/jaas/spec/com/sun/security/auth/module/Krb5LoginModule.html
 * http://docs.oracle.com/javase/7/docs/technotes/guides/security/jgss/single-signon.html
 * http://docs.oracle.com/javase/7/docs/technotes/guides/security/jgss/tutorials/index.html
 * http://docs.oracle.com/javase/7/docs/technotes/guides/security/jgss/tutorials/KerberosReq.html
 */
public class KerberosSample {

   public static void main(String[] args) {
      System.setProperty("java.security.policy", "=kerberos.policy");
      System.setSecurityManager(new SecurityManager());

      // Load security related system properties from property file
      Properties p = System.getProperties();
      try {
         p.load(new FileInputStream("security.properties"));
      } catch (IOException e) {
         e.printStackTrace();
      }
      System.setProperties(p);

      LoginContext lc = null;
      try {
         lc = new LoginContext("KerberosLogin", new TextCallbackHandler());

         // attempt authentication
         System.err.println("Logging in ...");
         lc.login();
         System.err.println("Logged in.");
      } catch (LoginException le) {
         le.printStackTrace();
      }

      Subject mySubject = lc.getSubject();

      PrivilegedAction<?> actions = new ProtectedActions();
      Subject.doAsPrivileged(mySubject, actions, null);
   }
}
