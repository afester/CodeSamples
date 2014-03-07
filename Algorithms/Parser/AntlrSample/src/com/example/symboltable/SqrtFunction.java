package com.example.symboltable;

public class SqrtFunction extends Function {
   
   public SqrtFunction() {
      super("sqrt");
   }

   
   public double getValue(double arg) {
      return Math.sqrt(arg);
   }
}