package com.example.javafx.properties;

import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;

public class PropertySample {

    
    public void run() {
        System.err.println("Running property sample ...");
        
        IntegerProperty ip = new SimpleIntegerProperty(123);
        IntegerProperty ip2 = new SimpleIntegerProperty(456);
        
        System.err.println("ip:" + ip);
        System.err.println("ip2:" + ip2);
        System.err.println("-------------------------------------------------");
        
        ip.bind(ip2);

        ip2.set(789);

        System.err.println("ip:" + ip);
        System.err.println("ip2:" + ip2);
        System.err.println("ip:" + ip.get());
        System.err.println("ip:" + ip);
        System.err.println("-------------------------------------------------");
    }
    
    public static void main(String[] args) {
        new PropertySample().run();
    }
}

