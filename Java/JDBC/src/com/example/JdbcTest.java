package com.example;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

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
      hierarchicalQuery();

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
}
