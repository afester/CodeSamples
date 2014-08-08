package com.example.constwrapper8;

import java.util.HashMap;

public class ConstEnumMapper<T, E> {
   T constValue;

   private HashMap<T, E> revMap = new HashMap<>();
   protected ConstEnumMapper() {
      for (E e : values()) {
         revMap.put(e.value(), e);
      }
   }
}
