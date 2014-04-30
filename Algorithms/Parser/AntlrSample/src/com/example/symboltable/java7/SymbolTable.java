package com.example.symboltable.java7;

import java.util.HashMap;
import java.util.Map;

import com.example.symboltable.Applyable;
import com.example.symboltable.Symbol;

public class SymbolTable {
   private Map<String, Symbol> table = new HashMap<>();
   
   public Symbol getSymbol(String name) {
      return table.get(name);
   }


   public void putSymbol(Symbol symbol) {
      table.put(symbol.getName(),  symbol);
   }

   
   public void apply(Applyable<Symbol> applier) {
      for (Symbol s : table.values()) {
         applier.each(s);
      }
   }
   

   public void dump() {
      System.err.println("Dump of Symbol table:");
      apply(new Applyable<Symbol>() {

         @Override
         public void each(Symbol s) {
            System.err.println(s);
         }
         
      });
   }
}
