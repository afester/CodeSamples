
package littletux.net.innerclass;
   public class InnerClass {

    private PrintManager pm;

    /*****************************************************************/
    // Inner class - InnerClass$InnerPrinter.class
    class InnerPrinter implements Printer {

        @Override
        public void printString(String value) {
            System.err.println("InnerPrinter: " + value);
        }
    }
    /*****************************************************************/

    public static void main(String[] args) {
        InnerClass app = new InnerClass();
        app.initApp();
        app.runApp();
    }

    private void initApp() {
        pm = new PrintManager();
        /*****************************************************************/
        pm.setPrinter(new InnerPrinter());
        /*****************************************************************/
    }

    private void runApp() {
        pm.print("Hello World");
    }
}
