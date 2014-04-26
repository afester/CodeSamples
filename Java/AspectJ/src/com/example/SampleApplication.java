package com.example;

public class SampleApplication {

   /**
    * @param args
    */
   public static void main(String[] args) {
      new SampleApplication().run();
   }


   protected void run()  {
      HelloWorld hw = new HelloWorld();
      hw.printGreeting(5);
   }

}
