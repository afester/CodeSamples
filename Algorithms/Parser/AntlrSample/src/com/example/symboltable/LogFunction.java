package com.example.symboltable;

public class LogFunction extends Function {
   
   public LogFunction() {
      super("log");
   }

   
   public double getValue(double arg) {
      return Math.log(arg);
   }
}