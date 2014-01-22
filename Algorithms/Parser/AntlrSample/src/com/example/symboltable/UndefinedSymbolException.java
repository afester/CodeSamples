package com.example.symboltable;

@SuppressWarnings("serial")
public class UndefinedSymbolException extends RuntimeException {
   public UndefinedSymbolException(String name) {
      super("Undefined symbol: " + name);
   }
}