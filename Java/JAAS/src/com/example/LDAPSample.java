package com.example;

import java.io.FileInputStream;
import java.io.IOException;
import java.util.Properties;

import javax.security.auth.login.LoginContext;
import javax.security.auth.login.LoginException;


public class LDAPSample {

   public static void main(String[] args) {
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
         
         
        lc = new LoginContext("LDAPUidLogin", new TextCallbackHandler());

        // attempt authentication
        System.err.println("Logging in ...");
        lc.login();
        System.err.println("Logged in.");
      } catch (LoginException le) {
         le.printStackTrace();
      }

   }
}
