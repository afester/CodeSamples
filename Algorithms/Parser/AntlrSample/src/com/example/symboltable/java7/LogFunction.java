package com.example.symboltable.java7;

public class LogFunction extends Function {
   
   public LogFunction() {
      super("log");
   }

   
   public double getValue(double arg) {
      return Math.log(arg);
   }
}