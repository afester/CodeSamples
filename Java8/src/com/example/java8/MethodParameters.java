package com.example.java8;

import java.lang.reflect.Method;
import java.lang.reflect.Parameter;

public class MethodParameters {
   
   public void someGreatMethod(Number xParam, int yParam, float zParam) {
   }

   /**
    * Prints all parameter names of a method using the new 
    * {@link #java.lang.reflection.Method.getParameters()} API
    *
    * By default, javac does not add parameter names to the class file.
    * This requires the new command line option "-parameter".
    * Since the runtime libraries are not compiled with this option,
    * retrieving the parameters from methods contained in the runtime
    * libraries always return the default parameter names like "arg0", "arg1", ...
    *
    * @param m The method for which to print the parameter names.
    */
   public void printParameterNames(Method m) {
      System.err.print(m.getName() + "(");
      for (Parameter p : m.getParameters()) {
         System.err.print(" " + p.getName());
      }
      System.err.println(")");
   }

   /**
    * Prints the parameters of a function without the corresponding reflection 
    * meta data available. All we can do here is to print artificial parameter 
    * names like arg0, arg1, ... 
    *
    * @param m The method for which to print the parameter names.
    */
   public void printParameterNamesPre8(Method m) {
      System.err.print(m.getName() + "(");
      int paramCount = m.getParameterTypes().length;
      for (int i = 0;  i < paramCount;  i++) {
         System.err.print(" arg" + i);
      }
      System.err.println(")");
   }


   public static void main(String[] args) {
      System.err.println("\nMethod parameters sample\n=====================================");

      MethodParameters mp = new MethodParameters();

      try {

         // retrieve methods of a runtime class - the runtime library has not
         // been compiled with the "-parameter" option, so all we get is the 
         // artificial arg0, arg1, ... names
         Method m1 = String.class.getDeclaredMethod("regionMatches", 
                   boolean.class, int.class, String.class, int.class, int.class);
         mp.printParameterNames(m1);
         mp.printParameterNamesPre8(m1);
         
         // retrieve methods of a method declared in this class - this class
         // has been compiled with the "-parameter" option, so using the new API
         // we get the real parameter names
         Method m2 = MethodParameters.class.getDeclaredMethod("someGreatMethod",
                   Number.class, int.class, float.class); 
         mp.printParameterNames(m2);
         mp.printParameterNamesPre8(m2);

      } catch (NoSuchMethodException e) {
         e.printStackTrace();
      } catch (SecurityException e) {
         e.printStackTrace();
      }

   }
}
