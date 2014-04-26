package com.example;

import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

public class HelloWorld {

   public void printGreeting() {
      String s = "Hello World";
      System.err.println(s);

      DateFormat df = new SimpleDateFormat("EEEE, d. MMMM YYYY");
      String today = df.format(new Date());
      System.err.println("Today is " + today);
   }

}
