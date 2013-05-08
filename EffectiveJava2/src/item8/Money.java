package item8;

import java.util.HashSet;
import java.util.Set;


// simple value class for money values with two decimal places
public class Money {

   private long value;  // assumes 2 decimal places!

   public Money(int integer, int decimal) {
      value = 100 * integer + decimal;
   }
   
   private Money(long value) {
      this.value = value;
   }

   public Money add(Money m2) {
      return new Money(value + m2.value);
   }

   @Override
   public String toString() {
      return String.format("%d,%02d", value / 100, value % 100);
   }
   
   @Override
   public boolean equals(Object other) {
      // 1. Use the == operator to check if the argument is a reference to this object.
      // Merely a performance optimization, could also be discarded
      if (other == this) {
         return true;
      }

      // 2. Use the instanceof operator to check if the argument has the correct type.
      // This also satisfies the "Non-nullity" condition!
      if (!(other instanceof Money)) {
         return false;
      }

      // 3. Cast the argument to the correct type.
      Money otherMoney = (Money) other;

      // 4. For each “significant” field in the class, check if that field of the argument
      //    matches the corresponding field of this object.
      return otherMoney.value == value;
   }

   // Always override hashCode() when you override equals()
   @Override
   public int hashCode() {
      return (int) (value ^ (value >>> 32));
   }

   
   public static void main(String[] args) {
      Money m1 = new Money(12, 95);
      Money m2 = new Money(0, 5);
      Money m3 = m1.add(m2);

      System.out.println(m1 + " + " + m2 + " = " + m3);
      
      Money m4 = new Money(13, 00);
      System.out.println(m3 + " == " + m4 + "? " + m3.equals(m4));

      Set<Money> s = new HashSet<>();
      s.add(m2);
      s.add(m3);
      s.add(m4);
      System.out.println(s);
   }
}
