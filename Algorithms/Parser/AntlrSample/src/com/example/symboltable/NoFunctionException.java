package com.example.symboltable;

@SuppressWarnings("serial")
public class NoFunctionException extends RuntimeException {
   public NoFunctionException(String name) {
      super("Not a function: " + name);
   }
}