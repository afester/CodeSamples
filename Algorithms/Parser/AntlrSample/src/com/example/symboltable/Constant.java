package com.example.symboltable;

public class Constant extends Symbol {
   protected double value;

   protected Constant(String name) {
      this(name, 0);
   }


   public Constant(String name, double value) {
      super(name);
      this.value = value;
   }

   @Override
   public double getValue(double arg) {
      return value;
   }

   @Override
   public String getType() {
      return "CONST";
   }
}
