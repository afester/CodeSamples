package com.example.java8;

import java.io.FileOutputStream;
import java.io.PrintWriter;
import java.io.IOException;

// a simple interface
interface Printer {
   void printString(String value);
   
   default void printInt(int value) {
      System.err.println("int: " + value);
   }
}

// another simple interface with the same methods as above
interface LogWriter {
   void printString(String value);

   default void printInt(int value) {
      try {
         PrintWriter pw = new PrintWriter(new FileOutputStream("out.log"));
         pw.println("int: " + value);
      } catch(IOException e) {
         e.printStackTrace();
      }
   }
}

// a class which implements the two interfaces
class DefaultPrinter implements Printer, LogWriter {

   @Override
   public void printString(String value) {
      System.err.println(value);
   }

   @Override
   // pick printInt from Printer interface
   public void printInt(int value) { Printer.super.printInt(value);  }
}


public class VirtualExtensionMethods {
   
   public void run() {
      Printer thePrinter = new DefaultPrinter();
      thePrinter.printString("Hello World");
      thePrinter.printInt(42);
   }
   
   public static void main(String[] args) {
      new VirtualExtensionMethods().run();
   }
}
