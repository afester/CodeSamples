package item13.package1;

class PrivateClass implements PrivateInterface{

   private String name;

   public PrivateClass(String name) {
      this.name = name;

   }

   @Override
   public String getName() {
      return name;
   }

   @Override
   public String toString() {

      return name;
   }

   
   public void cellTest() {
      // Package private method from PublicClass can be called here!
      PublicClass pc = new PublicClass(null, null);
      pc.setCity("Test");
   }
}
