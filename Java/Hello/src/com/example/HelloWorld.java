package com.example;

import java.io.Serializable;

import java.text.DecimalFormat;

public class HelloWorld {

public static void main(String[] args) {
     String pattern = "##,##,###.###";
        DecimalFormat myFormatter = new DecimalFormat(pattern);
        String output = myFormatter.format(2564484.125);            
        System.out.println(output);

        pattern = "###,###.###";
        myFormatter = new DecimalFormat(pattern);
        output = myFormatter.format(2564484.125);            
        System.out.println(output);

        pattern = "##,###.###";
        myFormatter = new DecimalFormat(pattern);
        output = myFormatter.format(2564484.125);            
        System.out.println(output);

        pattern = "#,###.###";
        myFormatter = new DecimalFormat(pattern);
        output = myFormatter.format(2564484.125);            
        System.out.println(output);

    }
}/*
public class HelloWorld implements Runnable, Serializable{

   
   public void run() {
      System.err.println("Hello World");
   }

   public static void main(String[] args) {
      new HelloWorld().run();
   }

}
*/