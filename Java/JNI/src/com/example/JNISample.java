package com.example;

public class JNISample {

   public static void main(String[] args) {
      System.err.println("Hello World");

      Native n = new Native();
      n.helloWorld();

      int result = n.sumArray(new int[] {3, 5, 7, 9, 10, 8});
      System.err.println("Result: " + result);
   }
}
