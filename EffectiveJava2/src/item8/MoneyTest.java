package item8;

import static org.junit.Assert.*;

import org.junit.Test;

public class MoneyTest {

   @Test
   public void checkEqualsContract() {
      Money m1 = new Money(12, 95);
      Money m2 = new Money(12, 95);
      Money m3 = new Money(12, 96);
      Money m4 = new Money(12, 95);

      // 1. Reflexivity
      assertTrue("NOT reflexive!", m1.equals(m1));

      // 2. Symmetry
      assertTrue("NOT Symmetric!", (m2.equals(m1) == true) && 
                                   (m1.equals(m2) == true));
      assertTrue("NOT Symmetric!", (m2.equals(m3) == false) && 
                                   (m3.equals(m2) == false));

      // 3. Transitivity
      assertTrue("NOT Transitive!", m1.equals(m2) && 
                                    m2.equals(m4) &&
                                    m1.equals(m4));
      
      // 4. Consistency
      assertTrue("NOT Consistent!", m1.equals(m2));
      assertTrue("NOT Consistent!", m1.equals(m2));
      assertTrue("NOT Consistent!", m1.equals(m2));
      assertFalse("NOT Consistent!", m1.equals(m3));
      assertFalse("NOT Consistent!", m1.equals(m3));
      assertFalse("NOT Consistent!", m1.equals(m3));

      // 5. Null check
      assertFalse("NULL check failed!", m1.equals(null));
   }

   @Test
   public void checkHashcodeContract() {
      Money m1 = new Money(12, 95);
      Money m2 = new Money(12, 95);
      Money m3 = new Money(12, 96);

      // 1. Consistency
      assertTrue("NOT Consistent!", m1.hashCode() == m1.hashCode());

      // 2. Additional Consistency
      assertTrue("NOT Consistent!", m1.equals(m2) && 
                                    m1.hashCode() == m2.hashCode());
      
      // 3. Not required by the contract, but necessary in reality due to 
      // performance impacts
      assertTrue("NO USEFUL HASHCODE!", (!m1.equals(m3)) &&
                                        (m1.hashCode() != m3.hashCode()));
   }

}
