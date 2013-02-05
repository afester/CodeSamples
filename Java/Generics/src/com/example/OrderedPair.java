package com.example;

public class OrderedPair<K, V> implements Pair<K, V> {

   private K theKey;
   private V theValue;

   public OrderedPair(K key, V value) {
      theKey = key;
      theValue = value;
   }
   
   public K getKey() {
      return theKey;
   }
   
   public V getValue() {
      return theValue;
   }
}
