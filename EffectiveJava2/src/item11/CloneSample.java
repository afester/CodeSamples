package item11;

public class CloneSample {
   public static void main(String[] args) {
      
      Person p = new Person("Herbert", "Meier");
      System.out.println(p + ", " + Integer.toHexString(p.hashCode()));

      Person p2 = p.clone();
      System.out.println(p2 + ", " + Integer.toHexString(p2.hashCode()));
   }
}
