package com.example;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class HelloWorld {

   public String printGreeting(int count) {
      // Logging point: "ENTER printGreeting, count=..."
      
      String s = "Hello World";

      DateFormat df = new SimpleDateFormat("EEEE, d. MMMM YYYY");
      String today = df.format(new Date());
      System.err.println("Today is " + today);
      
      for (int i = 0;  i < count;  i++) {
         // Logging point: "IN printGreeting, i=..."
         System.err.println(s);
      }
      
      // Logging point: "EXIT printGreeting, result=..."
      return today;
   }

}
