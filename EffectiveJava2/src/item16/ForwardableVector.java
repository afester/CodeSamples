package item16;

import java.util.Iterator;
import java.util.Vector;

/**
 * This class serves as a "delegate only" wrapper for the underlying Vector.
 * It can safely be sub classed to provide changed or additional functionality.
 */
class ForwardableVector<E> {

   // The delegate
   private Vector<E> v = new Vector<>();

   // Forwarding methods, created by Eclipse
   public E get(int index) {  return v.get(index); }
   public boolean add(E e) {  return v.add(e); }
   public Iterator<E> iterator() { return v.iterator(); }
}
