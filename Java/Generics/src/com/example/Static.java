package com.example;

public class Static<T> {
   // private static T object;
   //
   // Error: Cannot make a static reference to the non-static type T
   //
   // Reason: there is no concrete instance of the class "Static", like Static<String>.
   // If it would be possible to parameterize the access to the static variable,
   // similar to when calling a static generic method, you could store a String but read an Integer, for example:
   // 
   //   Static.<String>object = "Hello";
   //   Integer result = Static.<Integer>object;
   
   public static int getResult1() {
       return 42;
   }

   public static <T> String getResult2(T append) {
       return "42" + append;
   }

   public static void main(String[] args) {
       System.err.println("Result: " + Static.getResult1());
       System.err.println("Result: " + Static.getResult2(55.5));        // type inferred
       System.err.println("Result: " + Static.<Integer>getResult2(55)); // type specified explicitly
   }
}
