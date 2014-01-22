package com.example.symboltable;

public class CosFunction extends Function {

   public CosFunction() {
      super("cos");
   }

   public double getValue(double arg) {
      return Math.cos(arg);
   }
}
