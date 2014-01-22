package com.example.tests;

import org.junit.Test;

import com.example.Expression;
import com.example.FunctionDefinition;
import com.example.RealTimeCounter;

public class PerformanceTest {

   
   private static final String FUNC = "(3 + cos(x) * sin(x)) / 5";

   @Test
   public void compilerTest() {
      
      FunctionDefinition fn = new FunctionDefinition(FUNC);
      Expression e = fn.compiled();

      // warmup
      for (int run = 0;  run < 10000;  run++) {
         for (double x = 0; x < 2;  x += 0.1) {
            e.evaluate(x);
         }
      }

      // measurement
      RealTimeCounter rtc = new RealTimeCounter();
      rtc.start();
      for (int run = 0;  run < 10000;  run++) {
         for (double x = 0; x < 2;  x += 0.1) {
            e.evaluate(x);
         }
      }
      rtc.stop();
      System.err.println("Compiler   : " + rtc);
   }



   @Test
   public void interpreterTest() {

      FunctionDefinition fn = new FunctionDefinition(FUNC);
      Expression e = fn.interpreted();

      // warmup
      for (int run = 0;  run < 10000;  run++) {
         for (double x = 0; x < 2;  x += 0.1) {
            e.evaluate(x);
         }
      }

      // measurement
      RealTimeCounter rtc = new RealTimeCounter();
      rtc.start();
      for (int run = 0;  run < 10000;  run++) {
         for (double x = 0; x < 2;  x += 0.1) {
            e.evaluate(x);
         }
      }
      rtc.stop();
      System.err.println("Interpreter: " + rtc);
   }
}
