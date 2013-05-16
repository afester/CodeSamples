
package littletux.net.innerclass;

public class PrintManager {

    private Printer printer;

    public void setPrinter(Printer p) {
        printer = p;
    }

    public void print(String s) {
        printer.printString(s);
    }
}

