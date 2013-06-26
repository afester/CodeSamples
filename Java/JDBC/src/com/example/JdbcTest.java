package com.example;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class JdbcTest {

   public static void main(String[] args) throws ClassNotFoundException, SQLException {
      DatabaseProperties props = new DatabaseProperties();
      
      Class.forName(props.getDriver());
      Connection con = DriverManager.getConnection(props.getUrl(), props.getUser(), props.getPassword());
      System.out.println("Connected." + con);

      Statement s = con.createStatement();
      ResultSet rs = s.executeQuery("SELECT * FROM City");
      while(rs.next()) {
         System.out.println(rs.getString(1) + " " + rs.getString(2));
      }
      rs.close();
      s.close();

//      s = con.createStatement();
//      s.executeQuery("BEGIN\n"+
//                     "    EXECUTE IMMEDIATE 'DROP TABLE Person';\n"+
//                     "END;");

      con.close();
   }
}
