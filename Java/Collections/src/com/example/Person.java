package com.example;


public class Person implements Comparable<Person> {

   private String key;
   private String firstname;
   private String lastname;
   private int age;
   private String city;
   private String country;

   private Person(Builder b) {
      key = b.key;
      firstname = b.firstname;
      lastname = b.lastname;
      age = b.age;
      city = b.city;
      country = b.country;
   }


   public String getKey() {
      return key;
   }

   public String getFirstname() {
      return firstname;
   }

   public String getLastname() {
      return lastname;
   }

   public int getAge() {
      return age;
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
      private String key;

      // optional parameters
      private String firstname = "";
      private String lastname = "";
      private int age = 0;
      private String city = "unknown";
      private String country = "unknown";

      // constructor takes mandatory parameters
      public Builder(String key) {
         this.key = key;
      }

      public Builder firstname(String val) {
         firstname = val;
         return this;
      }

      public Builder lastname(String val) {
         lastname = val;
         return this;
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
      return "Person[key=" + key + ", firstname=" + firstname + ", lastname=" + 
               lastname + ", age=" + age + ", city=" + city + 
             ", country=" + country + "]";
   }


   @Override
   public int hashCode() {
      final int prime = 31;
      int result = 1;
      result = prime * result + age;
      result = prime * result + ((city == null) ? 0 : city.hashCode());
      result = prime * result + ((country == null) ? 0 : country.hashCode());
      result = prime * result
            + ((firstname == null) ? 0 : firstname.hashCode());
      result = prime * result + ((lastname == null) ? 0 : lastname.hashCode());
      return result;
   }


   @Override
   public boolean equals(Object obj) {
      if (this == obj)
         return true;
      if (obj == null)
         return false;
      if (getClass() != obj.getClass())
         return false;
      Person other = (Person) obj;
      if (age != other.age)
         return false;
      if (city == null) {
         if (other.city != null)
            return false;
      } else if (!city.equals(other.city))
         return false;
      if (country == null) {
         if (other.country != null)
            return false;
      } else if (!country.equals(other.country))
         return false;
      if (firstname == null) {
         if (other.firstname != null)
            return false;
      } else if (!firstname.equals(other.firstname))
         return false;
      if (lastname == null) {
         if (other.lastname != null)
            return false;
      } else if (!lastname.equals(other.lastname))
         return false;
      return true;
   }


   @Override
   public int compareTo(Person o) {
      // fixed sort by lastname, then by firstname

      int result = lastname.compareTo(o.getLastname());
      if (result == 0) {
         // identical lastname, compare by firstname
         result = firstname.compareTo(o.getFirstname());
      }

      return result;
   }
}
