package item16;

import java.util.Iterator;


/**
 * A simple Vector which allows to add elements, to retrieve elements
 * at a specific index and to print the current list of elements.
 */
public class SimpleVector<E> extends ForwardableVector<E> {

   public String toString() {
      StringBuffer result = new StringBuffer("[");
      Iterator<E> i = iterator();
      while(i.hasNext()) {
         result.append(i.next());
         if (i.hasNext()) {
            result.append(", ");
         }
      }
      result.append(']');

      return result.toString();
   }
}
