package item7;

import com.example.RealTimeCounter;


class SimpleClass{
   public static int value;
   
   public SimpleClass() {
      value++;
   }
}

class SimpleClassWithFinalizer{
   public static int value;
   
   public SimpleClassWithFinalizer() {
      value++;
   }
   
   
   @Override
   public void finalize() {
      value--;
   }
}

public class Finalizers {
   public static void main(String[] args) {
      System.out.println("** WARNING: rough numbers only - take them with a grain of salt!");
      System.out.println("\nWithout finalize() method:\n=============================================");

      RealTimeCounter rtc = new RealTimeCounter();

      int MAX_VAL = Integer.MAX_VALUE;
      rtc.start();
      for (int i = 0;  i < MAX_VAL;  i++) {
      }
      rtc.stop();
      System.out.println("  Loop time:" + rtc);
      long loopTime = rtc.getRealTime();
      
      rtc.start();
      for (int i = 0;  i < MAX_VAL;  i++) {
         new SimpleClass();
      }
      rtc.stop();
      
      System.out.println("  Creation time:" + rtc);
      
      long creTime = rtc.getRealTime() - loopTime;
      System.out.println("  Creation only time:" + creTime + "us");
      float creationTime1 = ((float) creTime / MAX_VAL * 1000);
      System.out.println("  Creation time per object:" + creationTime1 + "ns" );

      System.out.println("\nWith finalize() method:\n=============================================");
      
      
      MAX_VAL = 10000000;
      rtc.start();
      for (int i = 0;  i < MAX_VAL;  i++) {
      }
      rtc.stop();
      System.out.println("  Loop time:" + rtc);
      loopTime = rtc.getRealTime();

      rtc.start();
      for (int i = 0;  i < MAX_VAL;  i++) {
         new SimpleClassWithFinalizer();
      }
      rtc.stop();

      System.out.println("  Creation time:" + rtc);

      creTime = rtc.getRealTime() - loopTime;
      System.out.println("  Creation only time:" + creTime + "us");
      float creationTime2 = ((float) creTime / MAX_VAL * 1000);
      System.out.println("  Creation time per object:" + creationTime2 + "ns" );

      System.out.println("\nWith finalize() method, creation time is " + (int) (creationTime2 / creationTime1) + " times slower." );
    }
}
