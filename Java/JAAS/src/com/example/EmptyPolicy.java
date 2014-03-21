package com.example;

import java.security.AccessControlException;

public class EmptyPolicy {
   
   public static void main(String[] args) {
      // set the name of our local policy file - can also be passed  
      // when launching the JVM through -D.
      // This sample explicitly sets a new, empty policy file - the "=" determines
      // that no other (global or user specific) policy files are used.
      System.setProperty("java.security.policy", "=empty.policy");

      // install a security manager - uses the policy file set through java.security.policy
      System.setSecurityManager(new SecurityManager());

      // Now, we can not even read non-sensitive properties anymore.
      try {
         String loginConfig = System.getProperty("os.name");
         System.err.println("Login config: " + loginConfig);
      }catch(AccessControlException e) {
         System.err.println("Error: " + e.getMessage());
      }
   }
}
