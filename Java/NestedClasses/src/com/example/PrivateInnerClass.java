package com.example;

/* NOTE:
 * This source code should result in two class files:
 *
 *   PrivateInnerClass$InnerClass.class
 *   PrivateInnerClass.class
 *
 * However, with JDK 1.6.0, an additional class file is generated (JDK 1.7 
 * does not create this class file):
 *
 *   PrivateInnerClass$1.class
 *
 * It seems that this is a side effect of the private constructor. Interestingly
 * enough, even though the constructor of the inner class is private, it is 
 * accessible from the enclosing class.
 * ** Private methods of an inner class are still accessible from the enclosing class. **
 * 
 * When the private constructor is made public or package private (default access),
 * the additional class is not generated!
 * 
 * See also 
 *   http://stackoverflow.com/questions/2883181/why-is-an-anonymous-inner-class-containing-nothing-generated-from-this-code
 *   http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=4295934
 */
public class PrivateInnerClass {

   private class InnerClass {
      private InnerClass(String value) {
      }
      
      private void testMe() {
      }
   }

   public void testMe() {
      InnerClass ic = new InnerClass("Hello");
      ic.testMe();
   }
}
