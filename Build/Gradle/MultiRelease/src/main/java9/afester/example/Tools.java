package afester.example;

public class Tools {
   public static long getCurrentPid() {
      ProcessHandle self = ProcessHandle.current();
      return self.pid();
   }
}

