package com.example.symboltable;

public class SinFunction extends Function {
   
   public SinFunction() {
      super("sin");
   }


   public double getValue(double arg) {
      return Math.sin(arg);
   }
}