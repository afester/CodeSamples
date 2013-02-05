package com.example.server;

import java.rmi.Remote;
import java.rmi.RemoteException;

public interface StringServer extends Remote {
   String getHello() throws RemoteException;
   String getWorld() throws RemoteException;
   String add(String first, String second) throws RemoteException;
}
