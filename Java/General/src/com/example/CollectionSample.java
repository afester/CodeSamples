package com.example;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;

public class CollectionSample {
    public static void main(String[] args) {
        new CollectionSample().run();
    }
    
    public void run() {
        //nestedIteration();
        elementAddition();
    }


    public void nestedIteration() {
        List<Number> l = Arrays.asList(43, 55, 22, 12, 45, 98, 23, 54);
        
        // an iterator is essentially a pointer into the list.
        // hence the list can be iterated multiple times with different iterators.
        Iterator<Number> it1 = l.iterator();
        while(it1.hasNext()) {
            System.err.println(it1.next());
            Iterator<Number> it2 = l.iterator();
            while(it2.hasNext()) {
                System.err.println("   " + it2.next());
            }
        }
    }


    public void elementAdditionException() {
        List<Number> l = new ArrayList<>(Arrays.asList(43, 55, 22, 12, 45, 98, 23, 54));
        Iterator<Number> it1 = l.iterator();
        while(it1.hasNext()) {
            System.err.println(it1.next()); //  java.util.ConcurrentModificationException
            l.add(3, 66);   
        }
    }

    public void elementAddition() {
        List<Number> l = new ArrayList<>(Arrays.asList(43, 55, 22, 12, 45, 98, 23, 54));

        // Add the number 66 after each number
        ListIterator<Number> it1 = l.listIterator();
        while(it1.hasNext()) {
            System.err.println(it1.next());
            it1.add(66);
        }

        System.err.println("\nRESULT:");
        Iterator<Number> it2 = l.iterator();
        while(it2.hasNext()) {
            System.err.println(it2.next());
        }
        
        // remove all numbers 66 from the list
        Iterator<Number> it3 = l.iterator();
        while(it3.hasNext()) {
            if (it3.next().equals(66)) {
                it3.remove();
            }
        }

        System.err.println("\nRESULT 2:");
        Iterator<Number> it4 = l.iterator();
        while(it4.hasNext()) {
            System.err.println(it4.next());
        }
        
    }
}
