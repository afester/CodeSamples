package com.example;

import java.security.Provider;
import java.security.Provider.Service;
import java.security.Security;
import java.util.Set;

import org.bouncycastle.jce.provider.BouncyCastleProvider;

public class ListProviders implements Runnable {

    
    public void listProvidersAndAlgorithms() {
        System.out.println("List of all installed security providers and the algorithms they provide:");
        System.out.println("=========================================================================");
        
        Provider[] providers = java.security.Security.getProviders();
        for (Provider p : providers) {
            System.out.println("    " + p.getName() + "/" + p.getClass());
            Set<Service> services = p.getServices();
            for (Service s : services) {
                System.out.println("        " + s.getAlgorithm()+ " (" + s.getType() + ")" + " " + s.getClassName());
            }
        }
    }

    
    public void listProviders() {
        System.out.println("List of all installed security providers:");
        System.out.println("=========================================");
        
        Provider[] providers = java.security.Security.getProviders();
        for (Provider p : providers) {
            System.out.println("    " + p.getName() + "/" + p.getClass());
        }
    }

    
    
    public void run() {
        Security.addProvider(new BouncyCastleProvider());

        // listProviders();
        listProvidersAndAlgorithms();
    }

    public static void main(String[] args) {
        new ListProviders().run();
    }
}
