package com.example;

import java.awt.Color;
import java.awt.Font;

import javax.swing.JFrame;
import javax.swing.JTextArea;
import javax.swing.SwingUtilities;

@SuppressWarnings("serial")
public class WebstartSample extends JFrame implements Runnable {

   private JTextArea textArea;

   public WebstartSample() {
      super("Webstart sample application");
      setDefaultCloseOperation(DISPOSE_ON_CLOSE);

      textArea = new JTextArea();
      textArea.setEditable(false);
      textArea.setBackground(Color.black);
      textArea.setForeground(Color.green);
    //  textArea.setFont(new Font("Courier", Font.PLAIN, 12));
      add(textArea);
      setSize(640,  480);
      setLocation(200,  100);
   }


   public void run() {
      setVisible(true);
      textArea.setText("Webstart detected: " + System.getProperty("javawebstart.version", "None"));
   }


   public static void main(String[] args) {
      SwingUtilities.invokeLater(new Runnable() {

         @Override
         public void run() {
            new WebstartSample().run();
         }
      });
   }
}
