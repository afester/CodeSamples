package com.example.symboltable.java7;

public class SinFunction extends Function {
   
   public SinFunction() {
      super("sin");
   }


   public double getValue(double arg) {
      return Math.sin(arg);
   }
}