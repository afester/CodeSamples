package item12;

import static org.junit.Assert.*;

import org.junit.Test;

public class MoneyTest {

   @Test
   public void checkCompareToContract() {
      Money m1 = new Money(12, 95); 
      Money m2 = new Money(6, 49);
      Money m3 = new Money(19, 99);
      Money m4 = new Money(12, 95);
      
      
      // 1. Symmetry
      assertTrue(m1.compareTo(m2) == -m2.compareTo(m1));

      // 2. Transitivity
      assertTrue(m2.compareTo(m1) == m1.compareTo(m3) &&
                 m1.compareTo(m3) == m2.compareTo(m3));
      
      // 3. 
      assertTrue(m1.compareTo(m4) == 0 &&
                 m1.compareTo(m3) == m4.compareTo(m3));
      
   }
}
