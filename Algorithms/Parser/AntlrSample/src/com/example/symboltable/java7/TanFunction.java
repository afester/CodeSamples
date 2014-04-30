package com.example.symboltable.java7;

public class TanFunction extends Function {
   
   public TanFunction() {
      super("tan");
   }

   public double getValue(double arg) {
      return Math.tan(arg);
   }
}