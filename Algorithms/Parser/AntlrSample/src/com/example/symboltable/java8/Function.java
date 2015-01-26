package com.example.symboltable.java8;

import java.util.function.DoubleFunction;

import com.example.symboltable.Symbol;

public class Function extends Symbol {
   
   private DoubleFunction<Double> theFunc;

   public Function(String name, DoubleFunction<Double> lambda) {
      super(name);
      theFunc = lambda;
   }

   @Override
   public double getValue(double x) {
      return theFunc.apply(x);
   }
   

   public String getType() {
      return "FUNC";
   }

}