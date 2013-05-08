package item5;

import com.example.RealTimeCounter;

public class Unnecessary {
   
   private static final long MAX_VALUE = Integer.MAX_VALUE / 10;  // avoid too long running time
   
   // Hideously slow program! Can you spot the object creation?
   public static long slowMethod() {
      Long sum = 0L;
      for (long i = 0; i <= MAX_VALUE; i++) {
         sum += i;         // <<=
      }

      return sum;
   }

   
   public static long fastMethod() {
      long sum = 0L;
      for (long i = 0; i <= MAX_VALUE; i++) {
         sum += i;
      }

      return sum;
   }

   
   public static void main(String[] args) {
      RealTimeCounter rtc = new RealTimeCounter();

      // loop several times to avoid warmup effects
      for (int loop = 0;  loop < 5;  loop ++) {
         System.out.println("Attempt: " + loop);
         rtc.start();
         slowMethod();
         rtc.stop();
         System.out.println("   Slow method:" + rtc);
         
         rtc.start();
         fastMethod();
         rtc.stop();
         System.out.println("   Fast method:" + rtc);
      }
   }
}
