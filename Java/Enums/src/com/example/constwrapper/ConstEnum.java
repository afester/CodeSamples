package com.example.constwrapper;

import java.util.HashMap;
import java.util.NoSuchElementException;

public enum ConstEnum {
   DML_DELETE(Constants.DML_DELETE),
   DML_INSERT(Constants.DML_INSERT),
   DML_UPDATE(Constants.DML_UPDATE);
   
   private int constValue;

   // we need to create a reverse map for the enum in order to allow reverse lookups
   private static HashMap<Integer, ConstEnum> revMap = new HashMap<>();
   static {
      for (ConstEnum e : values()) {
         revMap.put(e.value(), e);
      }
   }

   // private constructor for the integer value
   private ConstEnum(int someValue) {
      constValue = someValue;
   }

   /**
    * @return The int value of this enum 
    */
   public int value() {
      return constValue;
   }

   /**
    * Look up an enum for a given int value.
    *
    * @param value
    * @return
    */
   public static ConstEnum enumFor(int value) {
      ConstEnum result = revMap.get(value);
      if (result == null) {
         throw new NoSuchElementException(ConstEnum.class.getName() + " for " + value); 
      }
      return result;
   }
}
