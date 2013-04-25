package com.example;

import java.awt.Color;

import javax.swing.BorderFactory;
import javax.swing.JApplet;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JPanel;
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

      JPanel panel = new JPanel();
      Border panelBorder = BorderFactory.createBevelBorder(BevelBorder.LOWERED);
      panel.setBorder(panelBorder);
      add(panel);
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
