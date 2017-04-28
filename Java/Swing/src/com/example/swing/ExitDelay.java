package com.example.swing;

import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.time.LocalDateTime;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

// http://stackoverflow.com/questions/38716167/what-prevents-program-from-exiting
public class ExitDelay {
    public static void main(String[] args){

        Runtime.getRuntime().addShutdownHook(new Thread(() -> System.err.println(LocalDateTime.now() + " SHUTDOWN") ));

        SwingUtilities.invokeLater(() -> {
    
            JFrame frame = new JFrame();
            frame.setSize(1280,720);
            frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
            frame.addWindowListener(new WindowAdapter(){
                @Override
                public void windowClosed(WindowEvent e){
                    System.err.println(LocalDateTime.now() + " CLOSE");
                    
                    // https://docs.oracle.com/javase/tutorial/uiswing/events/windowlistener.html
                    // When the last displayable window within the Java virtual machine (VM) is 
                    // disposed of, the VM may terminate. Note, however, that there can be a delay 
                    // before the program exits automatically, and that under some circumstances the 
                    // program might keep running. It is quicker and safer to explicitly exit the 
                    // program using System.exit(int).

                    System.exit(0);
                }
            });
            frame.setVisible(true);

        });
    }
}
