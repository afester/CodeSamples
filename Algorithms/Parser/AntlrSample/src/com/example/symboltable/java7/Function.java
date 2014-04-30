package com.example.symboltable.java7;

import com.example.symboltable.Symbol;

public abstract class Function extends Symbol {
   
   public Function(String name) {
      super(name);
   }
   

   public String getType() {
      return "FUNC";
   }

}