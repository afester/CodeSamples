package com.example.server;

import java.rmi.RMISecurityManager;
import java.rmi.RemoteException;
import java.rmi.registry.LocateRegistry;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

public class Server implements StringServer {

   @Override
   public String getHello() throws RemoteException {
      System.out.println("getHello()");
      return "Hello";
   }

   @Override
   public String getWorld() throws RemoteException {
      System.out.println("getWorld()");
      return "World";
   }

   @Override
   public String add(String first, String second) throws RemoteException {
      System.out.println("add(" + first + "," + second);
      return first + " " + second;
   }

   
   /**
    * http://docs.oracle.com/javase/tutorial/rmi/implementing.html
    *
    * @param args
    */
   public static void main(String[] args) {
	   System.setProperty("java.security.policy","serversecurity.policy");

	   /*
	    * "If an RMI program does not install a security manager, RMI will not download classes 
	    * (other than from the local class path) for objects received as arguments or return 
	    * values of remote method invocations. This restriction ensures that the operations performed 
	    * by downloaded code are subject to a security policy."
	    */
	  if (System.getSecurityManager() == null) {
          System.setSecurityManager(new SecurityManager());
      }
      System.out.println("Server is running ...");

      String name = "StringServer";
      StringServer server = new Server();
      try {
         /*
          * The UnicastRemoteObject.exportObject method exports the supplied remote 
          * object so that it can receive invocations of its remote methods from 
          * remote clients. The second argument, an int, specifies which TCP port 
          * to use to listen for incoming remote invocation requests for the object. 
          * It is common to use the value zero to let the actual port be chosen at runtime 
          * by RMI or the underlying operating system. Once the exportObject invocation 
          * has returned successfully, the Server remote object is ready to 
          * process incoming remote invocations.
          * 
          * The actual class is something like "Proxy0.class".
          */
         StringServer stub = (StringServer) UnicastRemoteObject.exportObject(server, 0);

         /*
          * The no-argument overload of LocateRegistry.getRegistry synthesizes 
          * a reference to a registry on the local host and on the default registry 
          * port, 1099. You must use an overload that has an int parameter if the 
          * registry is created on a port other than 1099.
          */
         // Registry registry = LocateRegistry.getRegistry();

         /*
          * Since we do not have a separate RMI registry running, we simply 
          * create one within the server side JVM.
          */
         Registry registry = LocateRegistry.createRegistry(1099);

         registry.rebind(name, stub);
      } catch (RemoteException e) {
         e.printStackTrace();
      }
      System.out.println("Server running and registered in RMI registry");
   }
}
