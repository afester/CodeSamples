
package littletux.net.innerclass;

public class AnonymousInnerClass {

    private PrintManager pm;

    public static void main(String[] args) {
        AnonymousInnerClass app = new AnonymousInnerClass();
        app.initApp(); // anonymous inner class is defined locally here ...
        app.runApp(); // ... but is still available here!!
    }

    private void initApp() {
        pm = new PrintManager();

/*****************************************************************/
        // Anonymous inner class - AnonymousInnerClass$1.class
        pm.setPrinter(new Printer() {

            @Override
            public void printString(String value) {
                System.err.println("AnonymousPrinter: " + value);
            }
        });
    /*****************************************************************/
    }

    private void runApp() {
        pm.print("Hello World");
    }
}
