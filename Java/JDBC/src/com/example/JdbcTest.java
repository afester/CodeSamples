package com.example;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class JdbcTest {

   private Connection con;

   public static void main(String[] args) {
      JdbcTest jdbcTest = new JdbcTest();
      jdbcTest.run();
   }


   public void run() {
      DatabaseProperties props = new DatabaseProperties();

      try {
         Class.forName(props.getDriver());
         con = DriverManager.getConnection(props.getUrl(), props.getUser(), props.getPassword());
         System.out.println("Connected: " + con);
      } catch (ClassNotFoundException e1) {
         e1.printStackTrace();
      } catch (SQLException e) {
         e.printStackTrace();
      }

     // simpleQuery();
      //hierarchicalQuery();
      for (int i = 0;  i < 3;  i++) {
         performanceQuery();
      }

      try {
         con.close();
      } catch (SQLException e) {
         e.printStackTrace();
      }
   }



   public void simpleQuery() {
      try {
         Statement s = con.createStatement();
         ResultSet rs = s.executeQuery("SELECT * FROM City");
         while(rs.next()) {
            System.out.println(rs.getString(1) + " " + rs.getString(2));
         }
         rs.close();
         s.close();
      } catch (SQLException e) {
         e.printStackTrace();
      }

//      s = con.createStatement();
//      s.executeQuery("BEGIN\n"+
//                     "    EXECUTE IMMEDIATE 'DROP TABLE Person';\n"+
//                     "END;");

   }

   
   private void hierarchicalQuery() {
      
      try {
         Statement s = con.createStatement();

         ResultSet rs = s.executeQuery(
               "SELECT LEVEL, Id, Label "+
               "FROM TreeNode "+
               "START WITH Parent = 0 "+
               "CONNECT BY PRIOR Id = Parent");
         while(rs.next()) {
            System.out.println(rs.getString(1) + " " + rs.getString(2) + " " + rs.getString(3));
         }

         rs.close();
         s.close();
      } catch (SQLException e) {
         e.printStackTrace();
      }
   }
   
   private void performanceQuery()  {
      try {
         Statement s = con.createStatement();
         s.setFetchSize(8000);

         List<String> myResult = new ArrayList<>();

         RealTimeCounter rtc = new RealTimeCounter();
         rtc.start();
         ResultSet rs = s.executeQuery("SELECT * FROM Performance");

         rtc.stop();
         System.out.println(rtc);

         rtc.start();
         while(rs.next()) {
            myResult.add(rs.getString(1));
            //System.out.println(rs.getString(1) + " " + rs.getString(2) + " " + rs.getString(3));
         }
         rtc.stop();
         System.out.println(rtc);
   
         rs.close();
         s.close();
      } catch (SQLException e) {
         e.printStackTrace();
      }
   }
}
