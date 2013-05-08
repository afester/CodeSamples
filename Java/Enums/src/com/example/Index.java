package com.example;

import java.util.Arrays;

public enum Index {
   ZERO,
   ONE,
   TWO,
   THREE,
   REG,
   INVALID;

   private static Index[] cachedValues = values();
   
   public static Index valueOf(int index) {
      System.out.println(Arrays.toString(values()));
       if (index == 8) {
          return REG;
       }

       if (index >= cachedValues.length) {
          return INVALID;
       }

       return cachedValues[index];
   }
}
