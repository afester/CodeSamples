package item6;

import java.util.Arrays;
import java.util.EmptyStackException;

import com.example.MemoryProbe;

//Can you spot the "memory leak"?
public class Stack {
   public Object[] elements;
   private int size = 0;
   private static final int DEFAULT_INITIAL_CAPACITY = 16;

   public Stack() {
      elements = new Object[DEFAULT_INITIAL_CAPACITY];
   }

   public void push(Object e) {
      ensureCapacity();
      elements[size++] = e;
   }

   public Object pop() {
      if (size == 0) {
         throw new EmptyStackException();
      }
      
      // The following approach effectively leaks memory, since the references
      // to the stack elements are still there:
      // return elements[--size];

      // This approach uses much less memory since it removes the unused 
      // references from the array:
      Object result = elements[--size];
      elements[size] = null;
      return result;
   }

   /**
    * Ensure space for at least one more element, roughly doubling the capacity
    * each time the array needs to grow.
    */
   private void ensureCapacity() {
      if (elements.length == size) {
         elements = Arrays.copyOf(elements, 2 * size + 1);
      }
   }

   // Output independant of "memory leak":
   //   After System.gc():Free memory delta: 419.024 bytes, Total memory delta: 0 bytes
   //   After push:       Free memory delta: -24.477.936 bytes, Total memory delta: 0 bytes
   //   After System.gc():Free memory delta: 5.927.296 bytes, Total memory delta: 0 bytes

   // Output WITH the "memory leak":
   //   After pop all:    Free memory delta: 0 bytes, Total memory delta: 0 bytes
   //   After System.gc():Free memory delta: 538.192 bytes, Total memory delta: 0 bytes

   // Output WITHOUT the "memory leak":
   //   After pop all:    Free memory delta: 0 bytes, Total memory delta: 0 bytes
   //   After System.gc():Free memory delta: 20.996.256 bytes, Total memory delta: 0 bytes
   public static void main(String[] args) {
      MemoryProbe mem = new MemoryProbe();
      Stack s = new Stack();

      mem.start();
      System.gc();
      mem.stop();
      System.out.println("After System.gc():" + mem);

      mem.start();
      for (int i = 0; i < 1000000; i++) {
         s.push(i);
      }
      mem.stop();
      System.out.println("After push:       " + mem);

      mem.start();
      System.gc();
      mem.stop();
      System.out.println("After System.gc():" + mem);

      mem.start();
      for (int i = 0; i < 1000000; i++) {
         s.pop();
      }
      mem.stop();
      System.out.println("After pop all:    " + mem);

      mem.start();
      System.gc();
      mem.stop();
      System.out.println("After System.gc():" + mem);

      // Even though we popped all elements, they are still available in the Array
      // (when using the initial approach where the elements are not nulled out)
      //for (Object o : s.elements) {
      //   System.out.println(o);
      //}
   }

}
