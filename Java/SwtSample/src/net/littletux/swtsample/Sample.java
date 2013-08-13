package net.littletux.swtsample;

import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Color;
import org.eclipse.swt.layout.FillLayout;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Label;
import org.eclipse.swt.widgets.Shell;

public class Sample {

    /**
     * @param args
     */
    public static void main(String[] args) {
        Display display = new Display();    // Link to underlying platform (application wide)
        
        // Create the "main window"
        Shell shell = new Shell(display);
        shell.setText("Hello World");
        shell.setBounds(100, 100, 200, 100);
        shell.setLayout(new FillLayout());

        // Create a red label
        HexEditor hexEditor = new HexEditor(shell, 0); // SWT.CENTER);
        hexEditor.setBackground(new Color(null, 255, 255, 204));
        
//        label.setText("Hello World");
//        Color red = new Color(display, 255, 0, 0);
//        label.setForeground(red);

        // show main window
        shell.open();
        
        // event loop
        while (!shell.isDisposed()) {
            if (!display.readAndDispatch())
                display.sleep();
        }

//        red.dispose();
        display.dispose();
    }

}
