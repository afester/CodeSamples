package com.example;

public class GenericMethod {
   
   public static <N extends Number> int convert(N value) {
      return value.intValue();
   }
}
