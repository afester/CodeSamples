package item2;

public class Person {

   private String name;
   private int age;
   private String city;
   private String country;

   private Person(Builder b) {
      name = b.name;

      age = b.age;
      city = b.city;
      country = b.country;
   }


   /**
    * Public static inner class which serves as the builder. 
    * The class collects all mandatory parameters from its constructor,
    * and allows to add any number of optional parameters.
    * When done, the build() method is used to verify the parameters and
    * finally build the actual object.
    */
   public static class Builder {
      
      // mandatory parameters
      private String name;

      // optional parameters
      private int age = 0;
      private String city = "unknown";
      private String country = "unknown";

      public Builder(String name) {
         this.name = name;
      }

      public Builder age(int val) {
         age = val;
         return this;
      }

      public Builder city(String val) {
         city = val;
         return this;
      }

      public Builder country(String val) {
         country = val;
         return this;
      }

      public Person build() {
         
         // validate parameters
         // "It is critical that they be checked after copying the parameters 
         // from the builder to the object, and that they be checked on
         // the object fields rather than the builder fields (see Item 39).
         Person result = new Person(this);
         if (result.city.equals("Karlsruhe") && !result.country.equals("Germany")) {
            throw new IllegalStateException("\"Country\" must be \"Germany\" when \"city\" is \"" + result.city + "\"");
         }

         // return the new object
         return result;
      }
   }


   @Override
   public String toString() {
      return "Person[name=" + name + ", age=" + age + ", city=" + city + 
             ", country=" + country + "]";
   }
   

   public static void main(String[] args) {
      // effectively, this code can be read like using named parameters!
      Person p = new Person.Builder("andreas")
                               .age(40)
                              .city("Karlsruhe")
                           .country("Germany")
                           .build();
      System.out.println(p);

      Person p2 = new Person.Builder("patrick").build();
      System.out.println(p2);
      
      Person p3 = new Person.Builder("emil").city("Karlsruhe").country("France").build();
      System.out.println(p3);
   }

}
