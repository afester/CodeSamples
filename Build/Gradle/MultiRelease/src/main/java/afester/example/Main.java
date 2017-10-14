package afester.example;

import afester.example.Tools;

public class Main {
   public static void main(String[] args) {
      long pid = Tools.getCurrentPid();
      System.err.printf("My process Id is: %d\n", pid);
   }
}
