public class HelloWorld implements Runnable {

   
   public void run() {
      System.err.println("Hello World");
   }

   public static void main(String[] args) {
      new HelloWorld().run();
   }

}
