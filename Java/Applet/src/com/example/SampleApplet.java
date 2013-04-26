package com.example;

import java.awt.Font;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.CharArrayReader;
import java.io.CharArrayWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Enumeration;
import java.util.List;
import java.util.Map.Entry;
import java.util.Properties;
import java.util.Set;

import javax.swing.BorderFactory;
import javax.swing.JApplet;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.border.BevelBorder;
import javax.swing.border.Border;

public class SampleApplet extends JApplet {

   private static final long serialVersionUID = 1L;
   private JTextArea textArea; 


   @Override
   public void init() {
      super.init();
      System.out.println("init");

      JMenuBar mb = new JMenuBar();

      JMenu m = new JMenu("Applet");
      JMenuItem fMenuOpen = new JMenuItem("List system properties");
      m.add(fMenuOpen);
      fMenuOpen.addActionListener(new ActionListener() {

         @Override
         public void actionPerformed(ActionEvent e) {
            clearTextArea();
            listProperties();
         }
      });

      JMenuItem fMenuClear = new JMenuItem("Clear text area");
      m.add(fMenuClear);
      fMenuClear.addActionListener(new ActionListener() {

         @Override
         public void actionPerformed(ActionEvent e) {
            clearTextArea();
         }
      });

      mb.add(m);
      setJMenuBar(mb);

      JScrollPane scrollPane = new JScrollPane();
      Border panelBorder = BorderFactory.createBevelBorder(BevelBorder.LOWERED);
      scrollPane.setBorder(panelBorder);

      textArea = new JTextArea();
      Font f = textArea.getFont();
      Font newFont = new Font("Courier", f.getStyle(), f.getSize());
      textArea.setFont(newFont);
      scrollPane.setViewportView(textArea);

      add(scrollPane);

   }


   /**
    * @return A sorted list of properties, retrieved from the Properties.list()
    * method
    */
//   private List<String> getSortedPropertiesFromList() {
//      List<String> propertyList = new ArrayList<>();
//
//      CharArrayWriter bof = new CharArrayWriter();
//      PrintWriter out = new PrintWriter(bof);
//      System.getProperties().list(out);
//
//      CharArrayReader car = new CharArrayReader(bof.toCharArray());
//      BufferedReader reader = new BufferedReader(car); 
//      String line;
//      try {
//         while( (line = reader.readLine()) != null) {
//            propertyList.add(line);
//         }
//      } catch (IOException e) {
//         e.printStackTrace();
//      }
//      
//      // Sort the string list
//      Collections.sort(propertyList);
//      
//      return propertyList;
//   }


   private List<Property> getSortedProperties() {
      List<Property> propertyList = new ArrayList<>();

      Properties sysprops = System.getProperties();
      Set<Object> o = sysprops.keySet();

      for (String key : sysprops.stringPropertyNames()) {
         String value = (String) sysprops.get(key);
         boolean fromDefault = true;
         if (o.contains(key)) {
            fromDefault = false;
         }

         propertyList.add(new Property(key, value, fromDefault));
      }

      // Sort the property list
      Collections.sort(propertyList);

      return propertyList;
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
   
   void listProperties() {
//    Properties sysprops = System.getProperties();
//    System.out.println(sysprops.stringPropertyNames().size()); // 200

    // The accessors which are inherited from the Hashtable **only** provide
    // access to the current properties. The separate "default properties"
    // are not considered by these methods!
//    System.out.println(sysprops.entrySet().size()); // 36
//    System.out.println(sysprops.keySet().size());   // 36
//    System.out.println(sysprops.values().size());   // 36


      textArea.append("List of all properties (* = default property)\n"); 
      textArea.append("----------------------------------------------\n"); 
    
    // get all properties in a sorted list and display them in the text area
      List<Property> propertyList = getSortedProperties();
      for (Property p : propertyList) {
         String line;
         if (p.isDefault()) {
            line = "*";
         } else {
            line = " ";
         }
         line = line + p.getName() + "=" + p.getValue() + "\n";
         textArea.append(line); 
      }
   }


   void clearTextArea() {
      textArea.setText("");
   }

}


class Property implements Comparable<Property> {
   private String name;
   private String value;
   private boolean fromDefault;
   
   
   public Property(String aKey, String aValue, boolean isFromDefault) {
      name = aKey;
      value = aValue;
      fromDefault = isFromDefault;
   }
   
   public String getName() {
      return name;
   }

   public String getValue() {
      return value;
   }
   
   public boolean isDefault() {
      return fromDefault;
   }

   @Override
   public int compareTo(Property other) {
      return name.compareTo(other.getName());
   }
}
