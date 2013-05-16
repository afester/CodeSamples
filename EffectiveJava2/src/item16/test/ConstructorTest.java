package item16.test;

import static org.junit.Assert.*;
import item16.SubClass;
import item16.SuperClass;

import org.junit.Test;

public class ConstructorTest {

   @Test
   public void test() {
      SuperClass sc = new SuperClass();
      org.junit.Assert.assertNotNull(sc); // can actually never fail
      
      
      SubClass sub = new SubClass();
      
   }

}
