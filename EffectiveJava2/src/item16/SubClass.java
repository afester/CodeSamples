package item16;

public class SubClass extends SuperClass {

   private final String someValue;
   private final int someIntValue;

   public SubClass() {
      someValue = "Hello world";
      someIntValue = 42;
      
      someMethod();
   }

   @Override
   protected void someMethod() {
      System.out.println("Sub class: " + someValue + ", " + someIntValue);
   }
   
}
