package com.example.symboltable.java8;

import java.util.HashMap;
import java.util.Map;

import com.example.symboltable.Applyable;
import com.example.symboltable.Constant;
import com.example.symboltable.Symbol;

public class SymbolTable {
   private Map<String, Symbol> table = new HashMap<>();
   
   public Symbol getSymbol(String name) {
      return table.get(name);
   }


   // Add a function (convenience method)
   public void putSymbol(String name, java.util.function.DoubleFunction<Double> fun) {
	   table.put(name, new Function(name, fun));
   }

   // Add a constant (convenience method)
   public void putSymbol(String name, Double cons) {
       table.put(name, new Constant(name, cons));
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
