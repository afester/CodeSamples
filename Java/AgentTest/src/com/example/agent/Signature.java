package com.example.agent;

public class Signature {

   private String packageName = "";
   private String className = "";
   private String methodName = "";
   private String parameters = "";
   
   public static Signature getSignature(String method) {
      Signature result = new Signature();
      
      int idx = method.indexOf('(');
      if (idx > 0) {
         result.parameters = method.substring(idx + 1, method.length() - 1);
         method = method.substring(0,  idx);
      } else {
         throw new RuntimeException("Invalid method signature: " + method);
      }

      idx = method.lastIndexOf('.');
      if (idx > 0) {
         result.methodName = method.substring(idx + 1);
         method = method.substring(0,  idx);
      } else {
         throw new RuntimeException("Invalid method signature: " + method);
      }

      idx = method.lastIndexOf('.');
      if (idx > 0) {
         result.className = method.substring(idx + 1);
         result.packageName = method.substring(0, idx);
      } else {
         result.className = method;
      }

      
      return result;
   }


   /**
    * @return The full class name of the method represented by this signature,
    *         like com.example.HelloWorld or HelloWorld
    */
   public String getClassName() {
      String result = packageName;
      if (result.length() > 0) {
         result = result + ".";
      }
      return result + className;
   }


   public boolean matchesClassName(String name) {
      String s1 = name.replaceAll("/",  ".");
      String s2 = getClassName();  
      return s1.equals(s2);
   }
   

   @Override
   public String toString() {
      StringBuffer result = new StringBuffer(getClassName());

      result.append('.');
      result.append(methodName);
      result.append('(');
      result.append(parameters);
      result.append(')');

      return result.toString();
   }

}