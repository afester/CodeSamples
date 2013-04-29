import java.lang.annotation.Annotation;
import java.lang.reflect.Method;


public class Main {

   public static void main(String[] args) {

      Class<AnnotationsSample> cls = AnnotationsSample.class;
      for (Method m : cls.getDeclaredMethods()) {
         System.out.println(m.getName() + ":");

         for (Annotation a : m.getDeclaredAnnotations()) {
            Class<?> acls = a.annotationType();
            if (acls.isAssignableFrom(Author.class)) {
               Author author = (Author) a;
               System.out.println("    Author: " + author.name() + 
                                  " (" + author.eMail() + ")");
            } else {
               System.out.println("    " + a);
            }
         }

      }
   }

}
