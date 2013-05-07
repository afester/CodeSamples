package item3;

import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;

public class TestSingleton {

   public static void main(String[] args)  {
      System.out.println("Using the singleton ...\n============================================");
      Highlander himself = Highlander.INSTANCE;
      System.out.println(himself);
      himself.useSword();

      System.out.println("\nCreating a second instance through reflection ...\n============================================");
      // theoretically, still possible to create another instance through reflection -
      // but see the Highlander constructor!
      Constructor<Highlander> cons;
      try {
         cons = Highlander.class.getDeclaredConstructor(new Class[0]);
         cons.setAccessible(true);
         Object o = cons.newInstance(new Object[0]);
         System.out.println(o);
      } catch (NoSuchMethodException | SecurityException | InstantiationException |
               IllegalAccessException | IllegalArgumentException | InvocationTargetException e) {
         e.printStackTrace();
      }

      // Serialize object to a file
      System.out.println("\nSerializing the singleton ...\n============================================");
      try {
         ObjectOutputStream oos = new ObjectOutputStream(new FileOutputStream("singleton.dat"));
         oos.writeObject(himself);
         oos.close();
      } catch (IOException e) {
         e.printStackTrace();
      }

      // Read back the object from the file - remember that constructors are not
      // invoked when deserializing an object!
      // To solve this, see the readResolve() method in the Highlander class
      try (ObjectInputStream ois = new ObjectInputStream(new FileInputStream("singleton.dat"))){
         Object o = ois.readObject();
         System.out.println(o);

         if (o != himself) {
            System.out.println("NOT A SINGLETON!!!");
         } else {
            System.out.println("YES, STILL A SINGLETON!!!"); // expected output
         }
      } catch (IOException | ClassNotFoundException e) {
         e.printStackTrace();
      }
     
   }
}
