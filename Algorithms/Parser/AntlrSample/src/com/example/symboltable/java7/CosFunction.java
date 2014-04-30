package com.example.symboltable.java7;

public class CosFunction extends Function {

   public CosFunction() {
      super("cos");
   }

   public double getValue(double arg) {
      return Math.cos(arg);
   }
}
