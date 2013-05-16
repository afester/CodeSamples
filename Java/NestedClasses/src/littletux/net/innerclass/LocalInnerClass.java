
package littletux.net.innerclass;

public class LocalInnerClass {

    private PrintManager pm;

    public static void main(String[] args) {
        LocalInnerClass app = new LocalInnerClass();
        app.initApp(); // local inner class is defined locally here ...
        app.runApp(); // ... but is still available here!!
    }

    private void initApp() {
        pm = new PrintManager();

/*****************************************************************/
        // Local inner class - LocalInnerClass$1LocalPrinter.class
        class LocalPrinter implements Printer {

            @Override
            public void printString(String value) {
                System.err.println("LocalPrinter: " + value);
            }
        }
        pm.setPrinter(new LocalPrinter());
/*****************************************************************/
    }

    private void runApp() {
        pm.print("Hello World");
    }
}

