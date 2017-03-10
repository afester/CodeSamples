package com.example.javafx.properties;

import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.collections.FXCollections;
import javafx.collections.ListChangeListener;
import javafx.collections.ObservableList;

public class PropertySample {

    
    public void run() {
        list.addListener( (ListChangeListener.Change<? extends String> change) -> System.err.println(change));

        simpleSample();
        listSample();
    }

    private void simpleSample() {
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

    private ObservableList<String> list = FXCollections.observableArrayList();

    private void listSample() {
//        list.addListener( (ListChangeListener.Change<? extends String> change) -> System.err.println(change));
        list.add("Hello");
        list.add("World");
        list.remove("xyz");
        list.remove("Hello");
        list.addAll("Welcome", "Moon");
        System.err.println(list);
        
        ObservableList<String> list2 = FXCollections.observableArrayList();
    }

    public static void main(String[] args) {
        new PropertySample().run();
    }
}

