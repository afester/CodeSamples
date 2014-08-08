package com.example;

public class LetterGenerator {

   public static void main(String[] args) {
      new LetterGenerator().run();
   }

   private void run() {
      /*
      System.err.println("A (0)");
      String result = generate(0);   // A
//      System.err.println(result);

      System.err.println("B (1)");
      result = generate(1);  // Y
//      System.err.println(result);

      System.err.println("C (2)");
      result = generate(2);  // Z
//      System.err.println(result);

      System.err.println();

      System.err.println("AA (0, 0)");
      result = generate(3);  // AA
//      System.err.println(result);

      System.err.println("AB (0, 1)");
      result = generate(4);
//      System.err.println(result);

      System.err.println("AC (0, 2)");
      result = generate(5);
//      System.err.println(result);
      
      System.err.println();
      System.err.println("BA (1, 0)");
      result = generate(6);
//      System.err.println(result);
System.err.println("=============");*/
      for (int i = 0;  i < 256;  i++) {
         String result = generate(i, 26);
         System.err.println(result);
      }
   }

   

   private String generate(int num, int DIV) {
      int i = num;
      String result = "";
      do {
         int c = i % DIV;
         i = i / DIV;

         // more than one digit, then we need to adjust the leftmost digit
         if (num >= DIV && i == 0) {
            c--;
         }

         String theChar = Character.toString((char) (c + 65));
         result = theChar + result;
      }while(i != 0);

      return result;
   }
}
