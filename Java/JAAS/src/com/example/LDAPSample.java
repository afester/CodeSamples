package com.example;

import java.io.FileInputStream;
import java.io.IOException;
import java.security.PrivilegedAction;
import java.util.Properties;

import javax.security.auth.Subject;
import javax.security.auth.login.LoginContext;
import javax.security.auth.login.LoginException;

public class LDAPSample {

   public static void main(String[] args) {
      System.setProperty("java.security.policy", "=ldap.policy");
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
         lc = new LoginContext("LDAPWLSLogin", new TextCallbackHandler());

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
