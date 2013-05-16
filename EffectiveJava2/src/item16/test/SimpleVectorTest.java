package item16.test;

import static org.junit.Assert.*;
import item16.SimpleVector;

import org.junit.Test;

public class SimpleVectorTest {

   @Test
   public void test() {
      SimpleVector<Integer> v = new SimpleVector<>();
      v.add(1);
      v.add(5);
      v.add(19);
      v.add(33);
      
      assertEquals("[1, 5, 19, 33]", v.toString());
      assertEquals(Integer.valueOf(1), v.get(0));
      assertEquals(Integer.valueOf(19), v.get(2));
   }
}
