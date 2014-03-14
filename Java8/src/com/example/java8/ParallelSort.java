package com.example.java8;

import java.util.Arrays;
import java.util.Random;
import com.example.RealTimeCounter;

public class ParallelSort {
   final static int ARR_SIZE = 10 * 1024*1024;    // 10 MB
   final static int LOOPS = 100;                  // execute 100 times

   public static void main(String[] args) {

      // create an array with random numbers
      int[] array = new int[ARR_SIZE];
      Random rand = new Random(1);  // make sure to use the same random sequence each time, to ensure reproduceability
      for (int i = 0;  i < array.length;  i++) {
         array[i] = rand.nextInt(9999999);
      }

      // create the timer probe
      RealTimeCounter rtc = new RealTimeCounter();

      // sort the array - note: inline sorting, need to copy the array first
      rtc.start();
      for (int i = 0;  i < LOOPS;  i++) {
         int[] toSort = Arrays.copyOf(array, array.length);
         Arrays.sort(toSort);
      }
      rtc.stop();
      System.err.println("Normal sort: " + rtc);

      rtc.start();
      for (int i = 0;  i < LOOPS;  i++) {
         int[] toSort = Arrays.copyOf(array, array.length);
         Arrays.parallelSort(toSort);
      }
      rtc.stop();
      System.err.println("Parallel sort: " + rtc);
   }
}
