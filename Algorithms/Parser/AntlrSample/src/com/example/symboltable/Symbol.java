package com.example.symboltable;

public abstract class Symbol {

   private String name;

   public Symbol(String name) {
      this.name = name;
   }

   public String getName() {
      return name;
   }

   public double getValue() {
      return getValue(0);
   }

   public abstract double getValue(double arg);
   

   public abstract String getType();

   
   public String toString() {
      return String.format("  %-10s %s %s", getName(), getType(), getValue());
   }
}
