package com.example;

/**
 * http://docs.oracle.com/javase/tutorial/java/generics/types.html
 * 
 * - General syntax: 
 *     Generic Type:   class name<T1, ..., Tn> { ... }
 *
 *     Generic Method: public <T1, ..., Tn> returnType method(params) { ... }
 *     The syntax for a generic method includes a type parameter, inside angle brackets, 
 *     and appears before the method's return type
 *
 * - A type variable (T1 ... Tn) can be any non-primitive type you specify: 
 *   any class type, any interface type, any array type, or even another type 
 *   variable.
 * - By convention, type parameter names are single, uppercase letters.
 */
public class GenericsSample {

   @SuppressWarnings("unchecked")
   public static void main(String[] args) {
      Box<Integer> myBox = new Box<>();   // Note: Integer is the "type argument"
      myBox.set(4);

      Box<Pair<String, Integer>> box2 = new Box<>();
      
      
      Pair<String, Integer> pair1 = new OrderedPair<>("First",  1);
      Pair<Integer, String> pair2 = new OrderedPair<>(2, "Second");
      
      // Raw types
      Box rawBox = new Box();

      rawBox = myBox;   // OK - assign parameterized type to raw type
      box2 = rawBox;    // WARNING - assign raw type to parameterized type
                        // Type safety: The expression of type Box needs unchecked 
                        // conversion to conform to Box<Pair<String,Integer>>
                        // (not shown now due to the @SuppressWarnings annotation!)

      Pair rawPair = new OrderedPair("First",  1);

      int result = GenericMethod.convert(4.5);
   }
}
