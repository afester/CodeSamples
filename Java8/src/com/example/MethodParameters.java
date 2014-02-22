package com.example;

import java.lang.reflect.Method;
import java.lang.reflect.Parameter;

public class MethodParameters {

   public void earlierParameters() {
      Class<String> clz = String.class;
      for (Method m : clz.getDeclaredMethods()) {
         System.err.println(m.getName());
         int paramCount = m.getParameterTypes().length;
         for (int i = 0;  i < paramCount;  i++) {
            System.err.println("  arg" + i);
         }
      }
   }

   
   public void someGreatMethod(Number xParam, Number yParam) {
   }
   
   public void getParameterNames(Class<?> clz) {
      for (Method m : clz.getDeclaredMethods()) {
         System.err.println(m.getName());
         for (Parameter p : m.getParameters()) {
            System.err.println("  " + p.getName());
         }
      }
   }

   public static void main(String[] args) {
      MethodParameters mp = new MethodParameters();

      //mp.earlierParameters();
      
      /**
       * By default, javac does not add parameter names to the class file.
       * This requires the special command line option "-parameter".
       * Since the runtime libraries are not compiled with this option,
       * retrieving the parameters from methods contained in the runtime 
       * libraries always return the default parameter names like "arg0", "arg1", ...
       */
      
      System.err.println("Methods of java.lang.Object class:");
      System.err.println("======================================");
      mp.getParameterNames(Object.class);

      System.err.println("\nMethods of this class:");
      System.err.println("=========================");
      mp.getParameterNames(mp.getClass());
   }
}
