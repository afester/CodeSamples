package com.example;

public class MemoryProbe {
   long totalMemoryStart;
   long totalMemoryStop;
   long freeMemoryStart;
   long freeMemoryStop;


   public void start() {
      freeMemoryStart = Runtime.getRuntime().freeMemory();
      totalMemoryStart = Runtime.getRuntime().totalMemory();
   }
   
   
   public void stop() {
      freeMemoryStop = Runtime.getRuntime().freeMemory();
      totalMemoryStop = Runtime.getRuntime().totalMemory();      
   }

   
   public long getFreeMemoryDelta() {
      return freeMemoryStart - freeMemoryStop;
   }

   
   public long getTotalMemoryDelta() {
      return totalMemoryStop - totalMemoryStart;
   }

   @Override
   public String toString() {
      String free = String.format("%,d",  getFreeMemoryDelta());
      String total = String.format("%,d",  getTotalMemoryDelta());
      
      return "Free memory delta: " + free + " bytes, Total memory delta: " + total + " bytes";
   }
}
