package item13.package1;

public class PublicClass extends PrivateClass {

   private String city;
   
   public PublicClass(String name, String city) {
      super(name);
   }

   public String getCity() {
      return city;
   }

   void setCity(String newCity) {
      this.city = newCity;
   }

   @Override
   public String toString() {
      return super.toString() + ", " + city;
   }
   
}
