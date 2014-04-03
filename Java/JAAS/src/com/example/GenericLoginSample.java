package com.example;

import java.io.FileInputStream;
import java.io.IOException;
import java.security.PrivilegedAction;
import java.util.Properties;

import javax.security.auth.Subject;
import javax.security.auth.login.LoginContext;
import javax.security.auth.login.LoginException;

/*
 * This example shows that the JAAS API is generic enough to use the same code
 * for completely different authentication mechanisms.
 * All we need to do is to configure the name of the login configuration in some
 * configuration file (security.properties in this example).
 */
public class GenericLoginSample {

   public static void main(String[] args) {
      // Load security related system properties from property file
      Properties p = System.getProperties();
      try {
         p.load(new FileInputStream("security.properties"));
      } catch (IOException e) {
         e.printStackTrace();
      }
      System.setProperties(p);

      // read authentication and policy file configuration from the respective properties
      String policyFile = System.getProperty("com.example.PolicyFile");
      String loginConfig = System.getProperty("com.example.LoginConfig");

      System.setProperty("java.security.policy", policyFile);
      System.setSecurityManager(new SecurityManager());

      LoginContext lc = null;
      try {
         lc = new LoginContext(loginConfig, new TextCallbackHandler());

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
