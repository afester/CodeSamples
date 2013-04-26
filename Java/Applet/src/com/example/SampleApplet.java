package com.example;

import java.io.ByteArrayOutputStream;
import java.io.PrintStream;
import java.io.UnsupportedEncodingException;

import javax.swing.BorderFactory;
import javax.swing.JApplet;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JTextArea;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;

public class SampleApplet extends JApplet {

   private static final long serialVersionUID = 1L;

   @Override
   public void init() {
      super.init();
      System.out.println("init");

      JMenuBar mb = new JMenuBar();
      JMenu m = new JMenu("File");
      JMenuItem fMenuOpen = new JMenuItem("Open");
      m.add(fMenuOpen);
      // fMenuOpen.addActionListener (this);

      JMenuItem fMenuClose = new JMenuItem("Close");
      m.add(fMenuClose);
      // fMenuClose.addActionListener (this);
      mb.add(m);

      setJMenuBar(mb);

      JTextArea panel = new JTextArea();
      Border panelBorder = BorderFactory.createBevelBorder(BevelBorder.LOWERED);
      panel.setBorder(panelBorder);
      add(panel);

      ByteArrayOutputStream bof = new ByteArrayOutputStream();
      PrintStream out = new PrintStream(bof);

      try {
         System.getProperties().list(out);
         panel.append(bof.toString("US-ASCII"));
      } catch (Exception e) {
         e.printStackTrace(out);
         try {
            panel.append(bof.toString("US-ASCII"));
         } catch (UnsupportedEncodingException e1) {
            e1.printStackTrace();
         }
      }
   }

   @Override
   public void start() {
      super.start();
      System.out.println("start");
   }

   @Override
   public void stop() {
      super.stop();
      System.out.println("stop");
   }
}
