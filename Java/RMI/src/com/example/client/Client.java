package com.example.client;

import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import com.example.server.StringServer;

public class Client {

   /**
    * @param args
    */
   public static void main(String[] args) {
	  // Set up security manager
	  System.setProperty("java.security.policy", "clientsecurity.policy");
      if (System.getSecurityManager() == null) {
          System.setSecurityManager(new SecurityManager());
      }

      System.err.println("Client is running ...");
      try {
    	  System.err.println("Retrieving Registry ...");
          Registry registry = LocateRegistry.getRegistry("localhost"); // args[0]);

    	  System.err.println("Resolving remote object ...");
          String name = "StringServer";
          StringServer srv = (StringServer) registry.lookup(name);

          System.out.println(srv.getHello() + " " + srv.getWorld());
      } catch (Exception e) {
          e.printStackTrace();
      }
   }
}
