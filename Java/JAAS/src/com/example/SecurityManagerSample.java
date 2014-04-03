package com.example;

import java.security.AccessControlException;

public class SecurityManagerSample {

   


   public void run() {
      // check if a security manager is already installed
      SecurityManager sm = System.getSecurityManager();
      System.err.println(sm);

      // set a security manager and use a new, local policy set
      System.setProperty("java.security.policy", "=local.policy");
      System.setSecurityManager(new SecurityManager());

      // Access to os.name property is granted
      System.err.println(System.getProperty("os.name"));

      // Access to os.version property is denied
      try {
         System.err.println(System.getProperty("os.version"));
      }catch(AccessControlException ace) {
         System.err.println(ace.getMessage());
      }

      ProtectedActions actions = new ProtectedActions();

      // executing executeDeniedAction is denied
      try {
         actions.executeDeniedAction();
      }catch(AccessControlException ace) {
         System.err.println(ace.getMessage());
      }

      // executing executeAllowedAction is allowed
      actions.executeAllowedAction();
   }


   public static void main(String[] args) {
      new SecurityManagerSample().run();
   }

}
