package com.example;

public class Native {

   {
      System.loadLibrary("Native");
   }
   
   
   public native void helloWorld();

   public native int sumArray(int[] array);
}
