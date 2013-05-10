package item13.package2;

import item13.package1.PublicClass;

// Not possible - PrivateClass from package1 is not visible 
// import item13.package1.PrivateClass;

//Not possible - PrivateClass from package1 is not visible 
// class SubClass extends item13.package1.PrivateClass {}


class SubClass extends PublicClass {

   public SubClass(String name, String city) {
      super(name, city);
   }
   
   public void check() {
      // Not possible - setCity() from PublicClass is not visible (default access) 
      // super.setCity("Test");
   }

}

public class Main {
   
   
   public static final String s = "Hello";   // immutable object; this is fine!
   public static final StringBuffer sb = new StringBuffer("Hello");

   public static void main(String[] args) {
      PublicClass pc = new PublicClass("John", "Los Angeles");
      System.out.println(pc.toString());
      
      // Not possible - setCity() from PublicClass is not visible (default access) 
      // pc.setCity("New York");

      // Note that only the *reference* is final - if the referenced object is
      // mutable, its state can still be changed!
      System.out.println(sb.toString());
      sb.append(" World");
      System.out.println(sb.toString());
   }
}
