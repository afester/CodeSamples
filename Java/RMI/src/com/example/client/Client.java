package com.example.client;

import java.awt.event.ComponentEvent;
import java.awt.event.ComponentListener;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;

import javax.swing.JFrame;

import com.example.server.StringServer;

public class Client {

   /**
    * @param args
    */
   public static void main(String[] args) {
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
