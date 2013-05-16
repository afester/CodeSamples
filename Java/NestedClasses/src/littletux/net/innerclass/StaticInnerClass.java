
package littletux.net.innerclass;

    public class StaticInnerClass {

    /*****************************************************************/
    // Inner class - StaticInnerClass$StaticInnerPrinter.class
    static class StaticInnerPrinter implements Printer {

        @Override
        public void printString(String value) {
            System.err.println("StaticInnerPrinter: " + value);
        }
    }
    /*****************************************************************/

    public static void main(String[] args) {
        PrintManager pm = new PrintManager();

/*****************************************************************/
        pm.setPrinter(new StaticInnerPrinter());
        pm.print("Hello World");
/*****************************************************************/
    }
}

