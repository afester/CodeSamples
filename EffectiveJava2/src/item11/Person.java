package item11;

public class Person implements Cloneable {
   private String firstName;
   private String lastName;

   public Person(String first, String last) {
      firstName = first;
      lastName = last;
   }

   @Override
   public Person clone() {
      try {
         // Create a exact copy of this object
         Person result = (Person) super.clone();

         // fix any fields which require adjustments
         result.lastName += " (Copy)";

         return result;
      } catch(CloneNotSupportedException cnse) {   // will never be thrown
         throw new AssertionError();
      }
   }

   public String toString() {
      return "Person[" + firstName + " " + lastName + "]";
   }
}
