package com.example;

import java.io.FileInputStream;
import java.util.Properties;

public class DatabaseProperties {
   private String driver = "";
   private String url = "";
   private String user = "";
   private String password = "";

   public DatabaseProperties() {
      try {
         // Load additional system properties.
         Properties p = new Properties();
         p.load(new FileInputStream("database.properties"));

         driver = p.getProperty("db.driver");
         url = p.getProperty("db.url");
         user = p.getProperty("db.user");
         password = p.getProperty("db.password");
      } catch (Exception e) {
         System.err.println("ERROR: No database configuration file found!");
         e.printStackTrace();
      }
   }

   public String getDriver() {
      return driver;
   }
   
   public String getUrl() {
      return url;
   }
   
   public String getUser() {
      return user;
   }
   
   public String getPassword() {
      return password;
   }
}
