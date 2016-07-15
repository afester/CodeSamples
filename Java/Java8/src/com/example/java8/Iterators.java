package com.example.java8;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.function.Consumer;
import java.util.function.BiFunction;

interface TriFunction<T,U,V,R> {
   R apply(T t, U u, V v);
}



public class Iterators {
   public static void main(String[] args) {
      System.err.println("\nIterators sample\n=====================================");

      List<String> aList = new ArrayList<>();
      
      aList.add("First");
      aList.add("Second");
      aList.add("Third");

      // legacy iteration - obviously an "ask" case
      System.err.println("Legacy iteration");
      Iterator<String> iterator = aList.iterator();
      while(iterator.hasNext()) {
         String element = iterator.next();
         System.err.println("  " + element);
      }

      // for-each loop
      System.err.println("\nfor-each loop");
      for (String element : aList) {
         System.err.println("  " + element);
      }
      
      // anonymous class
      System.err.println("\nAnonymous class");
      aList.forEach(new Consumer<String>() {
         public void accept(String element) {
            System.err.println("  " + element);
         }
      });

      // lambda expression
      System.err.println("\nLambda expression");
      aList.forEach(elem -> System.err.println("  " + elem));
      
      // functional interface!!!!
      // anonymous function / method!
      Consumer printMe = elem -> System.err.println("  " + elem);
      System.err.println(printMe.getClass().getName());
      aList.forEach(printMe);

      // python: fun = lambda a, b, c : a + b + c
      // Java:   fun = (a, b, c) -> a + b + c;
      BiFunction<Integer, Integer, Integer> fun = (a, b) -> a + b;
      System.err.println(fun.apply(1, 2));

      TriFunction<Integer, Integer, Integer, Integer> fun2 = (a, b, c) -> a + b + c;
      System.err.println(fun2.apply(1, 2, 3));
   }
}
