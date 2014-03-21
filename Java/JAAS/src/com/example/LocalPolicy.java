package com.example;

import java.security.AccessControlException;

public class LocalPolicy {

   public static void main(String[] args) {
      // set the name of our local policy file - can also be passed  
      // when launching the JVM through -D.

      // This sample explicitly sets a new application specific policy file - 
      // the "=" determines that no other (global or user specific) policy files 
      // are used.
      System.setProperty("java.security.policy", "=local.policy");

      // install a security manager - uses the policy file set through java.security.policy
      System.setSecurityManager(new SecurityManager());

      // If our local.policy file allows access to os.name, we can read it
      try {
         String loginConfig = System.getProperty("os.name");
         System.err.println("Login config: " + loginConfig);
      }catch(AccessControlException e) {
         System.err.println("Error: " + e.getMessage());
      }
   }
}
