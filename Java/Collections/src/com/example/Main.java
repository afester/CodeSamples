package com.example;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.List;

public class Main {

   
   public static void main(String[] args) {
      new Main().run();
   }

   public void run() {
      // create a list of persons

      List<Person> persons = new ArrayList<>();
      persons.add(new Person.Builder("r1") .firstname("Emerson") .lastname("Bowley")   .age(35) .build());
      persons.add(new Person.Builder("r2") .firstname("Blair")   .lastname("Waycott")  .age(33) .build());
      persons.add(new Person.Builder("r3") .firstname("Andreas") .lastname("Fester")   .age(43) .build());
      persons.add(new Person.Builder("r4") .firstname("Kanisha") .lastname("Waycott")  .age(39) .build());
      persons.add(new Person.Builder("r5") .firstname("Brock")   .lastname("Bolognia") .age(52) .build());
      persons.add(new Person.Builder("r6") .firstname("Blair")   .lastname("Malet")    .age(35) .build());
      list(persons);

      // sort persons by lastname, firstname - fixed sort implemented in the
      // Person class itself which implements Comparable
      Collections.sort(persons);
      list(persons);
      
      // custom sort by age
      // Note: this comparator imposes orderings that are inconsistent with equals.
      Comparator<Person> ageComparator = new Comparator<Person>() {

         @Override
         public int compare(Person o1, Person o2) {
            return o1.getAge() - o2.getAge();
         }
      };

      Collections.sort(persons, ageComparator);
      list(persons);
   }

   public void list(List<Person> data ) {
      System.err.printf("Key  | Firstname           | Lastname            | Age\n");
      System.err.printf("-----+---------------------+---------------------+------\n");
      for (Person p : data) {
         System.err.printf("%-5s| %-20s| %-20s| %-3d\n", p.getKey(), p.getFirstname(), 
                           p.getLastname(), p.getAge());
      }
      System.err.println();
   }
}
