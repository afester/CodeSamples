package com.example.javafx.properties;

import java.util.ArrayList;
import java.util.Comparator;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.List;
import java.util.Optional;

import javafx.beans.property.IntegerProperty;
import javafx.beans.property.SimpleIntegerProperty;
import javafx.collections.FXCollections;
import javafx.collections.ListChangeListener;
import javafx.collections.ObservableList;

public class PropertySample {

    
    public void run() {
        list.addListener( (ListChangeListener.Change<? extends String> change) -> System.err.println(change));

        //initialValue();
        //simpleSample();
        //listSample();
        sparseArray();
    }

    
    private static class LayerElement<T> {
        private int index;
        private T value;

        public LayerElement(int i, T string) {
            index = i;
            value = string;
        }

        public T getValue() {
            return value;
        }

        public int getIndex() {
            return index;
        }

        @Override
        public String toString() {
            return String.format("LayerElement[idx=%s, val=%s]", index, value);
        }
    }

    private static class LayerList<T> implements Iterable<T> {

        private LinkedList<LayerElement<T>> list = new LinkedList<>();

        public void set(int index, T value) {
            LayerElement<T> e = new LayerElement<>(index, value);
            list.add(e);
            list.sort(new Comparator<LayerElement<T>>() {

                @Override
                public int compare(LayerElement<T> o1, LayerElement<T> o2) {
                    if (o1.index == o2.index) { 
                        return 0;
                    } else if (o1.index < o2.index) {
                        return -1;
                    } else {
                        return 1;
                    }
                }
            });
        }

        @Override
        public Iterator<T> iterator() {

            return new Iterator<T>() {

                Iterator<LayerElement<T>> iter = list.iterator();

                @Override
                public boolean hasNext() {
                    return iter.hasNext();
                }

                @Override
                public T next() {
                    return iter.next().getValue();
                }
            };
        }

        public void remove(int i) {
            Optional<LayerElement<T>> res = list.stream().filter(e -> e.index == i).findFirst();
            res.ifPresent(e -> list.remove(e));
        }


        @Override
        public String toString() {
            StringBuffer res = new StringBuffer();
            boolean first = true;
            res.append("[");
            for (LayerElement<T> e : list) {
                if (!first) {
                    res.append(", ");
                }
                first = false;

                res.append(e.getIndex()); 
                res.append("="); 
                res.append(e.getValue()); 
            }
            res.append("]");
            return res.toString();
        }
    }

    private void sparseArray() {
        List<String> l = new ArrayList<>(10);
        System.err.println(l);

        LayerList<String> myArray = new LayerList<>();

        myArray.set(4, "Hello");
        myArray.set(2, "World");
        myArray.set(7, "Moon");

        myArray.remove(2);
        
       System.err.println(myArray);

        myArray.set(2, "Venus");
        System.err.println(myArray);
        myArray.set(0, "Mars");
        myArray.set(5, "Pluto");
        System.err.println(myArray);
        
        for (String s : myArray) {
            System.err.println("   " + s);
        }
        
        myArray.remove(5);
        System.err.println(myArray);
    }

    
    private void initialValue() {
        IntegerProperty ip = new SimpleIntegerProperty(123);
        IntegerProperty ip2 = new SimpleIntegerProperty(456);
        
        System.err.println("ip:" + ip);     // 123
        System.err.println("ip2:" + ip2);   // 456

        ip.bind(ip2);
        System.err.println("-------------------------------------------------");

        System.err.println("ip:" + ip);         // invalid
        System.err.println("ip:" + ip.get());   // 456
        System.err.println("ip2:" + ip2);       // 456
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

