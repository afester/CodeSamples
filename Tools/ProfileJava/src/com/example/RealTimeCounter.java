package com.example;

public class RealTimeCounter {
   long realStart;
   long realEnd;

   public void start() {
      realStart = System.currentTimeMillis();
   }


   public void stop() {
      realEnd = System.currentTimeMillis();
   }


   /**
    * @return The number of microseconds measured by this counter.
    *         Note that resolution is only 1 ms, but to keep consistency
    *         with the C/C++ API the method returns microseconds
    */
   public long getRealTime() {
      return (realEnd - realStart) * 1000;
   }
   

   public String toString() {
      return "Real Time: " + getRealTime() + "us (" + getRealTime()/1000 + "ms)";
   }

   // test case
   public static void main(String[] args) throws InterruptedException {
      RealTimeCounter rtc = new RealTimeCounter();
      rtc.start();
      Thread.sleep(120);
      rtc.stop();
      System.out.println(rtc);
   }
}
