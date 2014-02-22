package com.example.symboltable;

public abstract class Function extends Symbol {
   
   public Function(String name) {
      super(name);
   }
   

   public String getType() {
      return "FUNC";
   }

}