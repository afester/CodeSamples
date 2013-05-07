package item3;

import java.io.Serializable;

public class Highlander implements Serializable {
   private static final long serialVersionUID = 1486458255281660514L;

   // Support for object deserialization - return the instance when reading
   // a Highlander object
   private Object readResolve() {
      // Return the one true Elvis and let the garbage collector
      // take care of the Elvis impersonator.
      return INSTANCE;
   }
   
   public static final Highlander INSTANCE = new Highlander();

   private Highlander() {
      // avoid creation of more than one instance through reflection
      if (INSTANCE != null) {
         throw new IllegalAccessError("Trying to create a second instance of a singleton!");
      }
   }

   public void useSword() {
      // ...
   }
}
