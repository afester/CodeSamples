package com.example.symboltable;

public class Variable extends Constant {

   
   public Variable(String name) {
      super(name);
   }
   

   public void setValue(double val) {
      value = val;
   }

   @Override
   public String getType() {
      return "VAR";
   }
}
