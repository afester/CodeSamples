package com.example;

import java.security.AccessControlException;
import java.security.PrivilegedAction;

/**
 */
public class ProtectedActions implements PrivilegedAction<Object> {

   public void executeAllowedAction() {
      SecurityManager sec = System.getSecurityManager();
      if (sec != null) {
         RuntimePermission rp = new RuntimePermission("executeAllowedAction");
         sec.checkPermission(rp);
      }

      System.err.println("Execution of executeAllowedAction granted");
   }

   public void executeDeniedAction() {
      SecurityManager sec = System.getSecurityManager();
      if (sec != null) {
         sec.checkPermission(new RuntimePermission("executeDeniedAction"));        
      }
      
      System.err.println("Execution of executeDeniedAction granted");
   }

   @Override
   public Object run() {
      // executing executeDeniedAction is denied
      try {
         executeDeniedAction();
      } catch (AccessControlException ace) {
         System.err.println(ace.getMessage());
      }

      // executing executeAllowedAction is allowed
      executeAllowedAction();

      return null;
   }
}
