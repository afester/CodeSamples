package com.example.constwrapper8;

import java.util.NoSuchElementException;

public class Test {
   public static void main(String[] args) {

      // list all available values
      for (ConstEnum val : ConstEnum.values()) {
         System.err.println(val + "=" + val.value());
      }
      
      // convert int to enum
      int someValue = 5;
      ConstEnum e = ConstEnum.enumFor(someValue);
      System.err.println(e + "=" + e.value());

      // convert invalid int to enum
      someValue = 3;
      try {
      e = ConstEnum.enumFor(someValue);
      } catch (NoSuchElementException nse) {
         System.err.println(nse);
      }

      // switch on the enum value
      someValue = 2;
      e = ConstEnum.enumFor(someValue);
      switch(e) {
         case DML_DELETE : System.err.println("DELETE");  break;
         case DML_INSERT : System.err.println("INSERT");  break;
         case DML_UPDATE : System.err.println("UPDATE");  break;
      }
   } 
}
